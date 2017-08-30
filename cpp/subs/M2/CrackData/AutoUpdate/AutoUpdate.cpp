// AutoUpdate.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "AutoUpdate.h"
#include <assert.h>
#include <CmnMix/Cmn_FeatureCode.h>
#include <CmnMix/Cmn_LogOut.h>
#include <CmnMix/Cmn_UsefulClass.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CMN_MY_DLLAPP_DEFINE(CAutoUpdateApp);

CAutoUpdateApp::CAutoUpdateApp()
{
	exe_code_range_end_ = (void*)0x01f20000;
	update_out_file_name_ = "FeatureCode_m2/CD_Updates.h";
}

bool CAutoUpdateApp::Scan_Addr(Cmn_FC_OFstream& ofs)
{
	return true;
}

bool CAutoUpdateApp::Scan_Net(Cmn_FC_OFstream& ofs)
{
	pt_pcvoid addr_tmp, addr_tmp1;
	pt_dword dw_tmp;
	boost::optional<pt_dword> op_dw_tmp;
	/*
	$-29F    009C20EF      CC                        INT3
	$-29E    009C20F0      55                        PUSH EBP
	$-29D    009C20F1      8BEC                      MOV EBP,ESP
	$-29B    009C20F3      6A FF                     PUSH -0x1
	$-299    009C20F5      68 65023701               PUSH MapleSto.01370265
	$-294    009C20FA      64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$-28E    009C2100      50                        PUSH EAX
	$-28D    009C2101      53                        PUSH EBX
	$-28C    009C2102      56                        PUSH ESI
	$-28B    009C2103      57                        PUSH EDI
	$-28A    009C2104      A1 D0687101               MOV EAX,DWORD PTR DS:[0x17168D0]
	$-285    009C2109      33C5                      XOR EAX,EBP
	$-283    009C210B      50                        PUSH EAX
	$-282    009C210C      8D45 F4                   LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-27F    009C210F      64:A3 00000000            MOV DWORD PTR FS:[0],EAX
	$-279    009C2115      8B75 08                   MOV ESI,DWORD PTR SS:[EBP+0x8]
	$-276    009C2118      8B46 04                   MOV EAX,DWORD PTR DS:[ESI+0x4]
	$-273    009C211B      8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$-270    009C211E      83C6 04                   ADD ESI,0x4
	$-26D    009C2121      8BCE                      MOV ECX,ESI
	$-26B    009C2123      FFD2                      CALL EDX
	$-269    009C2125      8B0D 2C417201             MOV ECX,DWORD PTR DS:[0x172412C]
	$-263    009C212B      0FB6D8                    MOVZX EBX,AL
	$-260    009C212E      E8 A0184E01               CALL MapleSto.01EA39D3
	$-25B    009C2133      8BF8                      MOV EDI,EAX
	$-259    009C2135      85FF                      TEST EDI,EDI
	$-257    009C2137      0F84 56010000             JE MapleSto.009C2293
	$-251    009C213D      85DB                      TEST EBX,EBX
	$-24F    009C213F      75 65                     JNZ SHORT MapleSto.009C21A6
	$-24D    009C2141      8BCF                      MOV ECX,EDI
	$-24B    009C2143      E8 DDC3F6FF               CALL MapleSto.0092E525
	$-246    009C2148      8BC8                      MOV ECX,EAX
	$-244    009C214A      E8 94504901               CALL MapleSto.01E571E3
	$-23F    009C214F      8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$-23D    009C2151      8B50 0C                   MOV EDX,DWORD PTR DS:[EAX+0xC]
	$-23A    009C2154      8BCE                      MOV ECX,ESI
	$-238    009C2156      FFD2                      CALL EDX
	$-236    009C2158      85C0                      TEST EAX,EAX
	$-234    009C215A      0F86 33010000             JBE MapleSto.009C2293
	$-22E    009C2160      8945 08                   MOV DWORD PTR SS:[EBP+0x8],EAX
	$-22B    009C2163      8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$-229    009C2165      8B50 0C                   MOV EDX,DWORD PTR DS:[EAX+0xC]
	$-226    009C2168      8BCE                      MOV ECX,ESI
	$-224    009C216A      FFD2                      CALL EDX
	$-222    009C216C      8BD8                      MOV EBX,EAX
	$-220    009C216E      8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$-21E    009C2170      8B50 0C                   MOV EDX,DWORD PTR DS:[EAX+0xC]
	$-21B    009C2173      8BCE                      MOV ECX,ESI
	$-219    009C2175      FFD2                      CALL EDX
	$-217    009C2177      0FB7C0                    MOVZX EAX,AX
	$-214    009C217A      6A 00                     PUSH 0x0
	$-212    009C217C      50                        PUSH EAX
	$-211    009C217D      53                        PUSH EBX
	$-210    009C217E      8BCF                      MOV ECX,EDI
	$-20E    009C2180      E8 A0C3F6FF               CALL MapleSto.0092E525
	$-209    009C2185      8BC8                      MOV ECX,EAX
	$-207    009C2187      E8 F30A4901               CALL MapleSto.01E52C7F
	$-202    009C218C      836D 08 01                SUB DWORD PTR SS:[EBP+0x8],0x1
	$-1FE    009C2190    ^ 75 D1                     JNZ SHORT MapleSto.009C2163
	$-1FC    009C2192      8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-1F9    009C2195      64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$-1F2    009C219C      59                        POP ECX
	$-1F1    009C219D      5F                        POP EDI
	$-1F0    009C219E      5E                        POP ESI
	$-1EF    009C219F      5B                        POP EBX
	$-1EE    009C21A0      8BE5                      MOV ESP,EBP
	$-1EC    009C21A2      5D                        POP EBP
	$-1EB    009C21A3      C2 0400                   RETN 0x4
	$-1E8    009C21A6      83FB 01                   CMP EBX,0x1
	$-1E5    009C21A9      0F85 8C000000             JNZ MapleSto.009C223B
	$-1DF    009C21AF      8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$-1DD    009C21B1      8B50 0C                   MOV EDX,DWORD PTR DS:[EAX+0xC]
	$-1DA    009C21B4      8BCE                      MOV ECX,ESI
	$-1D8    009C21B6      FFD2                      CALL EDX
	$-1D6    009C21B8      8BD8                      MOV EBX,EAX
	$-1D4    009C21BA      8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$-1D2    009C21BC      8B50 0C                   MOV EDX,DWORD PTR DS:[EAX+0xC]
	$-1CF    009C21BF      8BCE                      MOV ECX,ESI
	$-1CD    009C21C1      FFD2                      CALL EDX
	$-1CB    009C21C3      0FB7C0                    MOVZX EAX,AX
	$-1C8    009C21C6      6A 01                     PUSH 0x1
	$-1C6    009C21C8      50                        PUSH EAX
	$-1C5    009C21C9      53                        PUSH EBX
	$-1C4    009C21CA      8BCF                      MOV ECX,EDI
	$-1C2    009C21CC      E8 54C3F6FF               CALL MapleSto.0092E525
	$-1BD    009C21D1      8BC8                      MOV ECX,EAX
	$-1BB    009C21D3      E8 A70A4901               CALL MapleSto.01E52C7F
	$-1B6    009C21D8      6A 10                     PUSH 0x10
	$-1B4    009C21DA      E8 8FED8300               CALL MapleSto.01200F6E                          ; JMP to mfc90.#798
	$-1AF    009C21DF      83C4 04                   ADD ESP,0x4
	$-1AC    009C21E2      8945 08                   MOV DWORD PTR SS:[EBP+0x8],EAX
	$-1A9    009C21E5      C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$-1A2    009C21EC      85C0                      TEST EAX,EAX
	$-1A0    009C21EE      74 0A                     JE SHORT MapleSto.009C21FA
	$-19E    009C21F0      53                        PUSH EBX
	$-19D    009C21F1      8BC8                      MOV ECX,EAX
	$-19B    009C21F3      E8 28D90000               CALL MapleSto.009CFB20
	$-196    009C21F8      EB 02                     JMP SHORT MapleSto.009C21FC
	$-194    009C21FA      33C0                      XOR EAX,EAX
	$-192    009C21FC      83EC 08                   SUB ESP,0x8
	$-18F    009C21FF      8BCC                      MOV ECX,ESP
	$-18D    009C2201      C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-186    009C2208      8965 08                   MOV DWORD PTR SS:[EBP+0x8],ESP
	$-183    009C220B      8941 04                   MOV DWORD PTR DS:[ECX+0x4],EAX
	$-180    009C220E      85C0                      TEST EAX,EAX
	$-17E    009C2210      74 0A                     JE SHORT MapleSto.009C221C
	$-17C    009C2212      83C0 04                   ADD EAX,0x4
	$-179    009C2215      50                        PUSH EAX
	$-178    009C2216      FF15 F8923E01             CALL DWORD PTR DS:[0x13E92F8]
	$-172    009C221C      8B0D C4597201             MOV ECX,DWORD PTR DS:[0x17259C4]
	$-16C    009C2222      E8 B9A2CAFF               CALL MapleSto.0066C4E0
	$-167    009C2227      8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-164    009C222A      64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$-15D    009C2231      59                        POP ECX
	$-15C    009C2232      5F                        POP EDI
	$-15B    009C2233      5E                        POP ESI
	$-15A    009C2234      5B                        POP EBX
	$-159    009C2235      8BE5                      MOV ESP,EBP
	$-157    009C2237      5D                        POP EBP
	$-156    009C2238      C2 0400                   RETN 0x4
	$-153    009C223B      83FB 03                   CMP EBX,0x3
	$-150    009C223E      75 53                     JNZ SHORT MapleSto.009C2293
	$-14E    009C2240      8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$-14C    009C2242      8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$-149    009C2245      8BCE                      MOV ECX,ESI
	$-147    009C2247      FFD2                      CALL EDX
	$-145    009C2249      0FB6C0                    MOVZX EAX,AL
	$-142    009C224C      83F8 01                   CMP EAX,0x1
	$-13F    009C224F      75 1C                     JNZ SHORT MapleSto.009C226D
	$-13D    009C2251      51                        PUSH ECX
	$-13C    009C2252      8BCC                      MOV ECX,ESP
	$-13A    009C2254      8965 08                   MOV DWORD PTR SS:[EBP+0x8],ESP
	$-137    009C2257      68 50303F01               PUSH MapleSto.013F3050
	$-132    009C225C      FF15 D0A13E01             CALL DWORD PTR DS:[0x13EA1D0]                   ; mfc90.#286
	$-12C    009C2262      6A 00                     PUSH 0x0
	$-12A    009C2264      6A 00                     PUSH 0x0
	$-128    009C2266      68 0D080000               PUSH 0x80D
	$-123    009C226B      EB 1E                     JMP SHORT MapleSto.009C228B
	$-121    009C226D      85C0                      TEST EAX,EAX
	$-11F    009C226F      75 22                     JNZ SHORT MapleSto.009C2293
	$-11D    009C2271      51                        PUSH ECX
	$-11C    009C2272      8BCC                      MOV ECX,ESP
	$-11A    009C2274      8965 08                   MOV DWORD PTR SS:[EBP+0x8],ESP
	$-117    009C2277      68 50303F01               PUSH MapleSto.013F3050
	$-112    009C227C      FF15 D0A13E01             CALL DWORD PTR DS:[0x13EA1D0]                   ; mfc90.#286
	$-10C    009C2282      6A 00                     PUSH 0x0
	$-10A    009C2284      6A 00                     PUSH 0x0
	$-108    009C2286      68 0C080000               PUSH 0x80C
	$-103    009C228B      E8 90D9C9FF               CALL MapleSto.0065FC20
	$-FE     009C2290      83C4 10                   ADD ESP,0x10
	$-FB     009C2293      8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-F8     009C2296      64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$-F1     009C229D      59                        POP ECX
	$-F0     009C229E      5F                        POP EDI
	$-EF     009C229F      5E                        POP ESI
	$-EE     009C22A0      5B                        POP EBX
	$-ED     009C22A1      8BE5                      MOV ESP,EBP
	$-EB     009C22A3      5D                        POP EBP
	$-EA     009C22A4      C2 0400                   RETN 0x4
	$-E7     009C22A7      CC                        INT3
	$-E6     009C22A8      CC                        INT3
	$-E5     009C22A9      CC                        INT3
	$-E4     009C22AA      CC                        INT3
	$-E3     009C22AB      CC                        INT3
	$-E2     009C22AC      CC                        INT3
	$-E1     009C22AD      CC                        INT3
	$-E0     009C22AE      CC                        INT3
	$-DF     009C22AF      CC                        INT3
	$-DE     009C22B0      55                        PUSH EBP
	$-DD     009C22B1      8BEC                      MOV EBP,ESP
	$-DB     009C22B3      6A FF                     PUSH -0x1
	$-D9     009C22B5      68 BE023701               PUSH MapleSto.013702BE
	$-D4     009C22BA      64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$-CE     009C22C0      50                        PUSH EAX
	$-CD     009C22C1      83EC 58                   SUB ESP,0x58
	$-CA     009C22C4      53                        PUSH EBX
	$-C9     009C22C5      56                        PUSH ESI
	$-C8     009C22C6      57                        PUSH EDI
	$-C7     009C22C7      A1 D0687101               MOV EAX,DWORD PTR DS:[0x17168D0]
	$-C2     009C22CC      33C5                      XOR EAX,EBP
	$-C0     009C22CE      50                        PUSH EAX
	$-BF     009C22CF      8D45 F4                   LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-BC     009C22D2      64:A3 00000000            MOV DWORD PTR FS:[0],EAX
	$-B6     009C22D8      E8 111C2D01               CALL MapleSto.01C93EEE
	$-B1     009C22DD      8B75 08                   MOV ESI,DWORD PTR SS:[EBP+0x8]
	$-AE     009C22E0      8B46 04                   MOV EAX,DWORD PTR DS:[ESI+0x4]
	$-AB     009C22E3      8B50 0C                   MOV EDX,DWORD PTR DS:[EAX+0xC]
	$-A8     009C22E6      83C6 04                   ADD ESI,0x4
	$-A5     009C22E9      8BCE                      MOV ECX,ESI
	$-A3     009C22EB      FFD2                      CALL EDX
	$-A1     009C22ED      A3 2C4A7201               MOV DWORD PTR DS:[0x1724A2C],EAX
	$-9C     009C22F2      8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$-9A     009C22F4      8B50 10                   MOV EDX,DWORD PTR DS:[EAX+0x10]
	$-97     009C22F7      8BCE                      MOV ECX,ESI
	$-95     009C22F9      FFD2                      CALL EDX
	$-93     009C22FB      8915 144A7201             MOV DWORD PTR DS:[0x1724A14],EDX
	$-8D     009C2301      A3 104A7201               MOV DWORD PTR DS:[0x1724A10],EAX
	$-88     009C2306      8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$-86     009C2308      8B50 08                   MOV EDX,DWORD PTR DS:[EAX+0x8]
	$-83     009C230B      8BCE                      MOV ECX,ESI
	$-81     009C230D      FFD2                      CALL EDX
	$-7F     009C230F      0FB7F8                    MOVZX EDI,AX
	$-7C     009C2312      897D E8                   MOV DWORD PTR SS:[EBP-0x18],EDI
	$-79     009C2315      85FF                      TEST EDI,EDI
	$-77     009C2317      75 21                     JNZ SHORT MapleSto.009C233A
	$-75     009C2319      A1 14A86C01               MOV EAX,DWORD PTR DS:[0x16CA814]
	$-70     009C231E      85C0                      TEST EAX,EAX
	$-6E     009C2320      74 6C                     JE SHORT MapleSto.009C238E
	$-6C     009C2322      83F8 01                   CMP EAX,0x1
	$-69     009C2325      7D 0C                     JGE SHORT MapleSto.009C2333
	$-67     009C2327      C705 18A86C01 01000000    MOV DWORD PTR DS:[0x16CA818],0x1
	$-5D     009C2331      EB 5B                     JMP SHORT MapleSto.009C238E
	$-5B     009C2333      A3 18A86C01               MOV DWORD PTR DS:[0x16CA818],EAX
	$-56     009C2338      EB 54                     JMP SHORT MapleSto.009C238E
	$-54     009C233A      E8 A2862D01               CALL MapleSto.01C9A9E1
	$-4F     009C233F      85C0                      TEST EAX,EAX
	$-4D     009C2341      74 40                     JE SHORT MapleSto.009C2383
	$-4B     009C2343      E8 99862D01               CALL MapleSto.01C9A9E1
	$-46     009C2348      3BC7                      CMP EAX,EDI
	$-44     009C234A      74 37                     JE SHORT MapleSto.009C2383
	$-42     009C234C      8D45 E8                   LEA EAX,DWORD PTR SS:[EBP-0x18]
	$-3F     009C234F      50                        PUSH EAX
	$-3E     009C2350      8D4D 08                   LEA ECX,DWORD PTR SS:[EBP+0x8]
	$-3B     009C2353      68 B2020000               PUSH 0x2B2
	$-36     009C2358      51                        PUSH ECX
	$-35     009C2359      E8 629EBAFF               CALL MapleSto.0056C1C0
	$-30     009C235E      8D55 08                   LEA EDX,DWORD PTR SS:[EBP+0x8]
	$-2D     009C2361      6A 0E                     PUSH 0xE
	$-2B     009C2363      52                        PUSH EDX
	$-2A     009C2364      C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$-23     009C236B      E8 F001E4FF               CALL MapleSto.00802560
	$-1E     009C2370      83C4 14                   ADD ESP,0x14
	$-1B     009C2373      8D4D 08                   LEA ECX,DWORD PTR SS:[EBP+0x8]
	$-18     009C2376      C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-11     009C237D      FF15 DCA13E01             CALL DWORD PTR DS:[0x13EA1DC]                   ; mfc90.#601
	$-B      009C2383      8B0D 88447201             MOV ECX,DWORD PTR DS:[0x1724488]
	$-5      009C2389      E8 0279F7FF               CALL MapleSto.00939C90
	$ ==>    009C238E      893D 14A86C01             MOV DWORD PTR DS:[0x16CA814],EDI                ; g_a_cur_line_
	$+6      009C2394      8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$+8      009C2396      8B50 10                   MOV EDX,DWORD PTR DS:[EAX+0x10]
	$+B      009C2399      8BCE                      MOV ECX,ESI
	$+D      009C239B      FFD2                      CALL EDX
	$+F      009C239D      8915 6C4A7201             MOV DWORD PTR DS:[0x1724A6C],EDX
	$+15     009C23A3      A3 684A7201               MOV DWORD PTR DS:[0x1724A68],EAX
	$+1A     009C23A8      8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$+1C     009C23AA      8B50 10                   MOV EDX,DWORD PTR DS:[EAX+0x10]
	$+1F     009C23AD      8BCE                      MOV ECX,ESI
	$+21     009C23AF      FFD2                      CALL EDX
	$+23     009C23B1      8915 744A7201             MOV DWORD PTR DS:[0x1724A74],EDX
	$+29     009C23B7      A3 704A7201               MOV DWORD PTR DS:[0x1724A70],EAX
	$+2E     009C23BC      8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$+30     009C23BE      8B50 10                   MOV EDX,DWORD PTR DS:[EAX+0x10]
	$+33     009C23C1      8BCE                      MOV ECX,ESI
	$+35     009C23C3      FFD2                      CALL EDX
	$+37     009C23C5      8915 644A7201             MOV DWORD PTR DS:[0x1724A64],EDX
	$+3D     009C23CB      A3 604A7201               MOV DWORD PTR DS:[0x1724A60],EAX
	$+42     009C23D0      8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$+44     009C23D2      8B50 10                   MOV EDX,DWORD PTR DS:[EAX+0x10]
	$+47     009C23D5      8BCE                      MOV ECX,ESI
	$+49     009C23D7      FFD2                      CALL EDX
	$+4B     009C23D9      8BF8                      MOV EDI,EAX
	$+4D     009C23DB      8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$+4F     009C23DD      8BDA                      MOV EBX,EDX
	$+51     009C23DF      8B50 10                   MOV EDX,DWORD PTR DS:[EAX+0x10]
	$+54     009C23E2      8BCE                      MOV ECX,ESI
	$+56     009C23E4      FFD2                      CALL EDX
	$+58     009C23E6      8945 C4                   MOV DWORD PTR SS:[EBP-0x3C],EAX
	$+5B     009C23E9      8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$+5D     009C23EB      8955 C8                   MOV DWORD PTR SS:[EBP-0x38],EDX
	$+60     009C23EE      8B50 10                   MOV EDX,DWORD PTR DS:[EAX+0x10]
	$+63     009C23F1      8BCE                      MOV ECX,ESI
	$+65     009C23F3      FFD2                      CALL EDX
	$+67     009C23F5      8945 CC                   MOV DWORD PTR SS:[EBP-0x34],EAX
	$+6A     009C23F8      8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$+6C     009C23FA      8955 D0                   MOV DWORD PTR SS:[EBP-0x30],EDX
	$+6F     009C23FD      8B50 10                   MOV EDX,DWORD PTR DS:[EAX+0x10]
	$+72     009C2400      8BCE                      MOV ECX,ESI
	$+74     009C2402      FFD2                      CALL EDX
	$+76     009C2404      8945 EC                   MOV DWORD PTR SS:[EBP-0x14],EAX
	$+79     009C2407      8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$+7B     009C2409      8955 F0                   MOV DWORD PTR SS:[EBP-0x10],EDX
	$+7E     009C240C      8B50 10                   MOV EDX,DWORD PTR DS:[EAX+0x10]
	$+81     009C240F      8BCE                      MOV ECX,ESI
	$+83     009C2411      FFD2                      CALL EDX
	$+85     009C2413      8945 BC                   MOV DWORD PTR SS:[EBP-0x44],EAX
	$+88     009C2416      8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$+8A     009C2418      8955 C0                   MOV DWORD PTR SS:[EBP-0x40],EDX
	$+8D     009C241B      8B50 10                   MOV EDX,DWORD PTR DS:[EAX+0x10]
	$+90     009C241E      8BCE                      MOV ECX,ESI
	$+92     009C2420      FFD2                      CALL EDX
	$+94     009C2422      8915 8C4A7201             MOV DWORD PTR DS:[0x1724A8C],EDX
	$+9A     009C2428      A3 884A7201               MOV DWORD PTR DS:[0x1724A88],EAX
	$+9F     009C242D      8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$+A1     009C242F      8B50 10                   MOV EDX,DWORD PTR DS:[EAX+0x10]
	$+A4     009C2432      8BCE                      MOV ECX,ESI
	$+A6     009C2434      FFD2                      CALL EDX
	$+A8     009C2436      8915 944A7201             MOV DWORD PTR DS:[0x1724A94],EDX
	$+AE     009C243C      A3 904A7201               MOV DWORD PTR DS:[0x1724A90],EAX
	$+B3     009C2441      8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$+B5     009C2443      8B50 10                   MOV EDX,DWORD PTR DS:[EAX+0x10]
	$+B8     009C2446      8BCE                      MOV ECX,ESI
	$+BA     009C2448      FFD2                      CALL EDX
	$+BC     009C244A      8915 9C4A7201             MOV DWORD PTR DS:[0x1724A9C],EDX
	$+C2     009C2450      A3 984A7201               MOV DWORD PTR DS:[0x1724A98],EAX
	$+C7     009C2455      8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$+C9     009C2457      8B50 10                   MOV EDX,DWORD PTR DS:[EAX+0x10]
	$+CC     009C245A      8BCE                      MOV ECX,ESI
	$+CE     009C245C      FFD2                      CALL EDX
	$+D0     009C245E      8915 A44A7201             MOV DWORD PTR DS:[0x1724AA4],EDX
	$+D6     009C2464      A3 A04A7201               MOV DWORD PTR DS:[0x1724AA0],EAX
	$+DB     009C2469      8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$+DD     009C246B      8B50 18                   MOV EDX,DWORD PTR DS:[EAX+0x18]
	$+E0     009C246E      8D4D D4                   LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$+E3     009C2471      51                        PUSH ECX
	$+E4     009C2472      8BCE                      MOV ECX,ESI
	$+E6     009C2474      FFD2                      CALL EDX
	$+E8     009C2476      C745 FC 01000000          MOV DWORD PTR SS:[EBP-0x4],0x1
	$+EF     009C247D      8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$+F1     009C247F      8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+F4     009C2482      8BCE                      MOV ECX,ESI
	$+F6     009C2484      FFD2                      CALL EDX
	$+F8     009C2486      8B4D F0                   MOV ECX,DWORD PTR SS:[EBP-0x10]
	$+FB     009C2489      8B55 EC                   MOV EDX,DWORD PTR SS:[EBP-0x14]
	$+FE     009C248C      51                        PUSH ECX
	$+FF     009C248D      8B4D CC                   MOV ECX,DWORD PTR SS:[EBP-0x34]
	$+102    009C2490      0FB6C0                    MOVZX EAX,AL
	$+105    009C2493      52                        PUSH EDX
	$+106    009C2494      8B55 C8                   MOV EDX,DWORD PTR SS:[EBP-0x38]
	$+109    009C2497      8945 08                   MOV DWORD PTR SS:[EBP+0x8],EAX
	$+10C    009C249A      8B45 D0                   MOV EAX,DWORD PTR SS:[EBP-0x30]
	$+10F    009C249D      50                        PUSH EAX
	$+110    009C249E      8B45 C4                   MOV EAX,DWORD PTR SS:[EBP-0x3C]
	$+113    009C24A1      51                        PUSH ECX
	$+114    009C24A2      52                        PUSH EDX
	$+115    009C24A3      50                        PUSH EAX
	$+116    009C24A4      53                        PUSH EBX
	$+117    009C24A5      57                        PUSH EDI
	$+118    009C24A6      E8 D5B5BDFF               CALL MapleSto.0059DA80
	$+11D    009C24AB      8B4D C0                   MOV ECX,DWORD PTR SS:[EBP-0x40]
	$+120    009C24AE      8B55 BC                   MOV EDX,DWORD PTR SS:[EBP-0x44]
	$+123    009C24B1      51                        PUSH ECX
	$+124    009C24B2      52                        PUSH EDX
	$+125    009C24B3      E8 F8B2BDFF               CALL MapleSto.0059D7B0
	$+12A    009C24B8      A1 A4A07201               MOV EAX,DWORD PTR DS:[0x172A0A4]
	$+12F    009C24BD      33FF                      XOR EDI,EDI
	$+131    009C24BF      83C4 28                   ADD ESP,0x28
	$+134    009C24C2      3BC7                      CMP EAX,EDI
	$+136    009C24C4      74 0F                     JE SHORT MapleSto.009C24D5
	$+138    009C24C6      8B4D 08                   MOV ECX,DWORD PTR SS:[EBP+0x8]
	$+13B    009C24C9      51                        PUSH ECX
	$+13C    009C24CA      8D88 CC000000             LEA ECX,DWORD PTR DS:[EAX+0xCC]
	$+142    009C24D0      E8 C24ACAFF               CALL MapleSto.00666F97
	$+147    009C24D5      A1 D84D7201               MOV EAX,DWORD PTR DS:[0x1724DD8]
	$+14C    009C24DA      3BC7                      CMP EAX,EDI
	$+14E    009C24DC      74 10                     JE SHORT MapleSto.009C24EE
	$+150    009C24DE      50                        PUSH EAX
	$+151    009C24DF      FF15 E8983E01             CALL DWORD PTR DS:[0x13E98E8]                   ; msvcr90.free
	$+157    009C24E5      83C4 04                   ADD ESP,0x4
	$+15A    009C24E8      893D D84D7201             MOV DWORD PTR DS:[0x1724DD8],EDI
	$+160    009C24EE      893D DC4D7201             MOV DWORD PTR DS:[0x1724DDC],EDI
	$+166    009C24F4      893D E04D7201             MOV DWORD PTR DS:[0x1724DE0],EDI
	$+16C    009C24FA      8B16                      MOV EDX,DWORD PTR DS:[ESI]
	$+16E    009C24FC      8B42 08                   MOV EAX,DWORD PTR DS:[EDX+0x8]
	$+171    009C24FF      8BCE                      MOV ECX,ESI
	$+173    009C2501      FFD0                      CALL EAX
	$+175    009C2503      0FB7C0                    MOVZX EAX,AX
	$+178    009C2506      3BC7                      CMP EAX,EDI
	$+17A    009C2508      8945 08                   MOV DWORD PTR SS:[EBP+0x8],EAX
	$+17D    009C250B      897D F0                   MOV DWORD PTR SS:[EBP-0x10],EDI
	$+180    009C250E      7E 52                     JLE SHORT MapleSto.009C2562
	$+182    009C2510      8B16                      MOV EDX,DWORD PTR DS:[ESI]
	$+184    009C2512      8B42 0C                   MOV EAX,DWORD PTR DS:[EDX+0xC]
	$+187    009C2515      8BCE                      MOV ECX,ESI
	$+189    009C2517      FFD0                      CALL EAX
	$+18B    009C2519      8B3D DC4D7201             MOV EDI,DWORD PTR DS:[0x1724DDC]
	$+191    009C251F      8BD8                      MOV EBX,EAX
	$+193    009C2521      3B3D E04D7201             CMP EDI,DWORD PTR DS:[0x1724DE0]
	$+199    009C2527      72 16                     JB SHORT MapleSto.009C253F
	$+19B    009C2529      8D4F 01                   LEA ECX,DWORD PTR DS:[EDI+0x1]
	$+19E    009C252C      51                        PUSH ECX
	$+19F    009C252D      B9 D84D7201               MOV ECX,MapleSto.01724DD8
	$+1A4    009C2532      E8 09BA1500               CALL MapleSto.00B1DF40
	$+1A9    009C2537      84C0                      TEST AL,AL
	$+1AB    009C2539      0F84 2A020000             JE MapleSto.009C2769
	$+1B1    009C253F      8B15 D84D7201             MOV EDX,DWORD PTR DS:[0x1724DD8]
	$+1B7    009C2545      8D04BA                    LEA EAX,DWORD PTR DS:[EDX+EDI*4]
	$+1BA    009C2548      85C0                      TEST EAX,EAX
	$+1BC    009C254A      74 02                     JE SHORT MapleSto.009C254E
	$+1BE    009C254C      8918                      MOV DWORD PTR DS:[EAX],EBX
	$+1C0    009C254E      8B45 F0                   MOV EAX,DWORD PTR SS:[EBP-0x10]
	$+1C3    009C2551      FF05 DC4D7201             INC DWORD PTR DS:[0x1724DDC]
	$+1C9    009C2557      40                        INC EAX
	$+1CA    009C2558      3B45 08                   CMP EAX,DWORD PTR SS:[EBP+0x8]
	$+1CD    009C255B      8945 F0                   MOV DWORD PTR SS:[EBP-0x10],EAX
	$+1D0    009C255E    ^ 7C B0                     JL SHORT MapleSto.009C2510
	$+1D2    009C2560      33FF                      XOR EDI,EDI
	$+1D4    009C2562      A1 F84D7201               MOV EAX,DWORD PTR DS:[0x1724DF8]
	$+1D9    009C2567      3BC7                      CMP EAX,EDI
	$+1DB    009C2569      74 10                     JE SHORT MapleSto.009C257B
	$+1DD    009C256B      50                        PUSH EAX
	$+1DE    009C256C      FF15 E8983E01             CALL DWORD PTR DS:[0x13E98E8]                   ; msvcr90.free
	$+1E4    009C2572      83C4 04                   ADD ESP,0x4
	$+1E7    009C2575      893D F84D7201             MOV DWORD PTR DS:[0x1724DF8],EDI
	$+1ED    009C257B      A1 E84D7201               MOV EAX,DWORD PTR DS:[0x1724DE8]
	$+1F2    009C2580      893D FC4D7201             MOV DWORD PTR DS:[0x1724DFC],EDI
	$+1F8    009C2586      893D 004E7201             MOV DWORD PTR DS:[0x1724E00],EDI
	$+1FE    009C258C      3BC7                      CMP EAX,EDI
	$+200    009C258E      74 10                     JE SHORT MapleSto.009C25A0
	$+202    009C2590      50                        PUSH EAX
	$+203    009C2591      FF15 E8983E01             CALL DWORD PTR DS:[0x13E98E8]                   ; msvcr90.free
	$+209    009C2597      83C4 04                   ADD ESP,0x4
	$+20C    009C259A      893D E84D7201             MOV DWORD PTR DS:[0x1724DE8],EDI
	$+212    009C25A0      893D EC4D7201             MOV DWORD PTR DS:[0x1724DEC],EDI
	$+218    009C25A6      893D F04D7201             MOV DWORD PTR DS:[0x1724DF0],EDI
	$+21E    009C25AC      8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$+220    009C25AE      8B50 08                   MOV EDX,DWORD PTR DS:[EAX+0x8]
	$+223    009C25B1      8BCE                      MOV ECX,ESI
	$+225    009C25B3      FFD2                      CALL EDX
	$+227    009C25B5      0FB7C0                    MOVZX EAX,AX
	$+22A    009C25B8      3BC7                      CMP EAX,EDI
	$+22C    009C25BA      8945 08                   MOV DWORD PTR SS:[EBP+0x8],EAX
	$+22F    009C25BD      897D F0                   MOV DWORD PTR SS:[EBP-0x10],EDI
	$+232    009C25C0      7E 50                     JLE SHORT MapleSto.009C2612
	$+234    009C25C2      8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$+236    009C25C4      8B50 0C                   MOV EDX,DWORD PTR DS:[EAX+0xC]
	$+239    009C25C7      8BCE                      MOV ECX,ESI
	$+23B    009C25C9      FFD2                      CALL EDX
	$+23D    009C25CB      8B3D EC4D7201             MOV EDI,DWORD PTR DS:[0x1724DEC]
	$+243    009C25D1      8BD8                      MOV EBX,EAX
	$+245    009C25D3      3B3D F04D7201             CMP EDI,DWORD PTR DS:[0x1724DF0]
	$+24B    009C25D9      72 16                     JB SHORT MapleSto.009C25F1
	$+24D    009C25DB      8D47 01                   LEA EAX,DWORD PTR DS:[EDI+0x1]
	$+250    009C25DE      50                        PUSH EAX
	$+251    009C25DF      B9 E84D7201               MOV ECX,MapleSto.01724DE8
	$+256    009C25E4      E8 57B91500               CALL MapleSto.00B1DF40
	$+25B    009C25E9      84C0                      TEST AL,AL
	$+25D    009C25EB      0F84 78010000             JE MapleSto.009C2769
	$+263    009C25F1      8B0D E84D7201             MOV ECX,DWORD PTR DS:[0x1724DE8]
	$+269    009C25F7      8D04B9                    LEA EAX,DWORD PTR DS:[ECX+EDI*4]
	$+26C    009C25FA      85C0                      TEST EAX,EAX
	$+26E    009C25FC      74 02                     JE SHORT MapleSto.009C2600
	$+270    009C25FE      8918                      MOV DWORD PTR DS:[EAX],EBX
	$+272    009C2600      8B45 F0                   MOV EAX,DWORD PTR SS:[EBP-0x10]
	$+275    009C2603      FF05 EC4D7201             INC DWORD PTR DS:[0x1724DEC]
	$+27B    009C2609      40                        INC EAX
	$+27C    009C260A      3B45 08                   CMP EAX,DWORD PTR SS:[EBP+0x8]
	$+27F    009C260D      8945 F0                   MOV DWORD PTR SS:[EBP-0x10],EAX
	$+282    009C2610    ^ 7C B0                     JL SHORT MapleSto.009C25C2
	$+284    009C2612      8B16                      MOV EDX,DWORD PTR DS:[ESI]
	$+286    009C2614      8B42 10                   MOV EAX,DWORD PTR DS:[EDX+0x10]
	$+289    009C2617      8BCE                      MOV ECX,ESI
	$+28B    009C2619      FFD0                      CALL EAX
	$+28D    009C261B      8BDA                      MOV EBX,EDX
	$+28F    009C261D      8B16                      MOV EDX,DWORD PTR DS:[ESI]
	$+291    009C261F      8B52 20                   MOV EDX,DWORD PTR DS:[EDX+0x20]
	$+294    009C2622      8BF8                      MOV EDI,EAX
	$+296    009C2624      8D45 D8                   LEA EAX,DWORD PTR SS:[EBP-0x28]
	$+299    009C2627      50                        PUSH EAX
	$+29A    009C2628      8BCE                      MOV ECX,ESI
	$+29C    009C262A      FFD2                      CALL EDX
	$+29E    009C262C      51                        PUSH ECX
	$+29F    009C262D      8D45 D8                   LEA EAX,DWORD PTR SS:[EBP-0x28]
	$+2A2    009C2630      8BCC                      MOV ECX,ESP
	$+2A4    009C2632      8965 08                   MOV DWORD PTR SS:[EBP+0x8],ESP
	$+2A7    009C2635      50                        PUSH EAX
	$+2A8    009C2636      C645 FC 02                MOV BYTE PTR SS:[EBP-0x4],0x2
	$+2AC    009C263A      FF15 C8A13E01             CALL DWORD PTR DS:[0x13EA1C8]                   ; mfc90.#280
	$+2B2    009C2640      8B0D 406F7201             MOV ECX,DWORD PTR DS:[0x1726F40]
	$+2B8    009C2646      53                        PUSH EBX
	$+2B9    009C2647      57                        PUSH EDI
	$+2BA    009C2648      E8 7398EDFF               CALL MapleSto.0089BEC0
	$+2BF    009C264D      8B0D 406F7201             MOV ECX,DWORD PTR DS:[0x1726F40]
	$+2C5    009C2653      E8 487FEEFF               CALL MapleSto.008AA5A0
	$+2CA    009C2658      8B16                      MOV EDX,DWORD PTR DS:[ESI]
	$+2CC    009C265A      8B52 18                   MOV EDX,DWORD PTR DS:[EDX+0x18]
	$+2CF    009C265D      8D45 DC                   LEA EAX,DWORD PTR SS:[EBP-0x24]
	$+2D2    009C2660      50                        PUSH EAX
	$+2D3    009C2661      8BCE                      MOV ECX,ESI
	$+2D5    009C2663      FFD2                      CALL EDX
	$+2D7    009C2665      8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$+2D9    009C2667      8B50 18                   MOV EDX,DWORD PTR DS:[EAX+0x18]
	$+2DC    009C266A      8D4D E0                   LEA ECX,DWORD PTR SS:[EBP-0x20]
	$+2DF    009C266D      51                        PUSH ECX
	$+2E0    009C266E      8BCE                      MOV ECX,ESI
	$+2E2    009C2670      C645 FC 03                MOV BYTE PTR SS:[EBP-0x4],0x3
	$+2E6    009C2674      FFD2                      CALL EDX
	$+2E8    009C2676      8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$+2EA    009C2678      8B50 18                   MOV EDX,DWORD PTR DS:[EAX+0x18]
	$+2ED    009C267B      8D4D E4                   LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$+2F0    009C267E      51                        PUSH ECX
	$+2F1    009C267F      8BCE                      MOV ECX,ESI
	$+2F3    009C2681      C645 FC 04                MOV BYTE PTR SS:[EBP-0x4],0x4
	$+2F7    009C2685      FFD2                      CALL EDX
	$+2F9    009C2687      833D 34667201 00          CMP DWORD PTR DS:[0x1726634],0x0
	$+300    009C268E      C645 FC 05                MOV BYTE PTR SS:[EBP-0x4],0x5
	$+304    009C2692      74 1E                     JE SHORT MapleSto.009C26B2
	$+306    009C2694      8D45 DC                   LEA EAX,DWORD PTR SS:[EBP-0x24]
	$+309    009C2697      50                        PUSH EAX
	$+30A    009C2698      E8 A39E1A00               CALL MapleSto.00B6C540
	$+30F    009C269D      8D4D E0                   LEA ECX,DWORD PTR SS:[EBP-0x20]
	$+312    009C26A0      51                        PUSH ECX
	$+313    009C26A1      E8 CA9E1A00               CALL MapleSto.00B6C570
	$+318    009C26A6      8D55 E4                   LEA EDX,DWORD PTR SS:[EBP-0x1C]
	$+31B    009C26A9      52                        PUSH EDX
	$+31C    009C26AA      E8 F1B21A00               CALL MapleSto.00B6D9A0
	$+321    009C26AF      83C4 0C                   ADD ESP,0xC
	$+324    009C26B2      68 EA4E0000               PUSH 0x4EEA
	$+329    009C26B7      E8 74A7F7FF               CALL MapleSto.0093CE30
	$+32E    009C26BC      83C4 04                   ADD ESP,0x4
	$+331    009C26BF      85C0                      TEST EAX,EAX
	$+333    009C26C1      74 0B                     JE SHORT MapleSto.009C26CE
	$+335    009C26C3      8B4D E8                   MOV ECX,DWORD PTR SS:[EBP-0x18]
	$+338    009C26C6      51                        PUSH ECX
	$+339    009C26C7      8BC8                      MOV ECX,EAX
	$+33B    009C26C9      E8 52E21500               CALL MapleSto.00B20920
	$+340    009C26CE      8B0D 48417201             MOV ECX,DWORD PTR DS:[0x1724148]
	$+346    009C26D4      E8 B7151600               CALL MapleSto.00B23C90
	$+34B    009C26D9      8B0D 00277201             MOV ECX,DWORD PTR DS:[0x1722700]
	$+351    009C26DF      E8 31EB2201               CALL MapleSto.01BF1215
	$+356    009C26E4      8B0D C0407201             MOV ECX,DWORD PTR DS:[0x17240C0]
	$+35C    009C26EA      E8 E185B8FF               CALL MapleSto.0054ACD0
	$+361    009C26EF      833D C44A7201 00          CMP DWORD PTR DS:[0x1724AC4],0x0
	$+368    009C26F6      75 30                     JNZ SHORT MapleSto.009C2728
	$+36A    009C26F8      6A 00                     PUSH 0x0
	$+36C    009C26FA      6A 7B                     PUSH 0x7B
	$+36E    009C26FC      8D4D 9C                   LEA ECX,DWORD PTR SS:[EBP-0x64]
	$+371    009C26FF      E8 0C8BB6FF               CALL MapleSto.0052B210
	$+376    009C2704      8D4D 9C                   LEA ECX,DWORD PTR SS:[EBP-0x64]
	$+379    009C2707      C645 FC 06                MOV BYTE PTR SS:[EBP-0x4],0x6
	$+37D    009C270B      E8 E1272901               CALL MapleSto.01C54EF1
	$+382    009C2710      8B45 A4                   MOV EAX,DWORD PTR SS:[EBP-0x5C]
	$+385    009C2713      C745 9C BC504101          MOV DWORD PTR SS:[EBP-0x64],MapleSto.014150BC
	$+38C    009C271A      85C0                      TEST EAX,EAX
	$+38E    009C271C      74 0A                     JE SHORT MapleSto.009C2728
	$+390    009C271E      50                        PUSH EAX
	$+391    009C271F      FF15 E8983E01             CALL DWORD PTR DS:[0x13E98E8]                   ; msvcr90.free
	$+397    009C2725      83C4 04                   ADD ESP,0x4
	$+39A    009C2728      8D4D E4                   LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$+39D    009C272B      FF15 08A23E01             CALL DWORD PTR DS:[0x13EA208]                   ; mfc90.#601
	$+3A3    009C2731      8D4D E0                   LEA ECX,DWORD PTR SS:[EBP-0x20]
	$+3A6    009C2734      FF15 08A23E01             CALL DWORD PTR DS:[0x13EA208]                   ; mfc90.#601
	$+3AC    009C273A      8D4D DC                   LEA ECX,DWORD PTR SS:[EBP-0x24]
	$+3AF    009C273D      FF15 08A23E01             CALL DWORD PTR DS:[0x13EA208]                   ; mfc90.#601
	$+3B5    009C2743      8D4D D8                   LEA ECX,DWORD PTR SS:[EBP-0x28]
	$+3B8    009C2746      FF15 DCA13E01             CALL DWORD PTR DS:[0x13EA1DC]                   ; mfc90.#601
	$+3BE    009C274C      8D4D D4                   LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$+3C1    009C274F      FF15 08A23E01             CALL DWORD PTR DS:[0x13EA208]                   ; mfc90.#601
	$+3C7    009C2755      8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+3CA    009C2758      64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+3D1    009C275F      59                        POP ECX
	$+3D2    009C2760      5F                        POP EDI
	$+3D3    009C2761      5E                        POP ESI
	$+3D4    009C2762      5B                        POP EBX
	$+3D5    009C2763      8BE5                      MOV ESP,EBP
	$+3D7    009C2765      5D                        POP EBP
	$+3D8    009C2766      C2 0400                   RETN 0x4
	$+3DB    009C2769      68 0E000780               PUSH 0x8007000E
	$+3E0    009C276E      E8 ADF3A3FF               CALL MapleSto.00401B20
	$+3E5    009C2773      CC                        INT3
	$+3E6    009C2774      CC                        INT3
	*/
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("893D????????8B068B50108BCEFFD2", 2);
	if (!op_dw_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *op_dw_tmp;
	CheckExeAddr(dw_tmp);
	ofs << "pt_cdword	g_a_cur_line_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	/*
	$-1B2    007FD550      50                   PUSH EAX
	$-1B1    007FD551      8955 E8              MOV DWORD PTR SS:[EBP-0x18],EDX
	$-1AE    007FD554      E8 273DE1FF          CALL MapleSto.00611280
	$-1A9    007FD559      8D4D E8              LEA ECX,DWORD PTR SS:[EBP-0x18]
	$-1A6    007FD55C      51                   PUSH ECX
	$-1A5    007FD55D      50                   PUSH EAX
	$-1A4    007FD55E      8D55 EC              LEA EDX,DWORD PTR SS:[EBP-0x14]
	$-1A1    007FD561      68 A0A14401          PUSH MapleSto.0144A1A0                           ; Socket Send Exception : {0}({1})
	$-19C    007FD566      52                   PUSH EDX
	$-19B    007FD567      C645 FC 03           MOV BYTE PTR SS:[EBP-0x4],0x3
	$-197    007FD56B      E8 F0120000          CALL MapleSto.007FE860
	$-192    007FD570      83C4 10              ADD ESP,0x10
	$-18F    007FD573      B3 05                MOV BL,0x5
	$-18D    007FD575      8D4D E4              LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$-18A    007FD578      885D FC              MOV BYTE PTR SS:[EBP-0x4],BL
	$-187    007FD57B      FF15 08A23E01        CALL DWORD PTR DS:[0x13EA208]                    ; mfc90.#601
	$-181    007FD581      8D4D EC              LEA ECX,DWORD PTR SS:[EBP-0x14]
	$-17E    007FD584      FF15 F4A13E01        CALL DWORD PTR DS:[0x13EA1F4]                    ; mfc90.#3726
	$-178    007FD58A      50                   PUSH EAX
	$-177    007FD58B      E8 608AC4FF          CALL MapleSto.00445FF0
	$-172    007FD590      83C4 04              ADD ESP,0x4
	$-16F    007FD593      833D 90377401 00     CMP DWORD PTR DS:[0x1743790],0x0
	$-168    007FD59A      74 3E                JE SHORT MapleSto.007FD5DA
	$-166    007FD59C      8D4D EC              LEA ECX,DWORD PTR SS:[EBP-0x14]
	$-163    007FD59F      FF15 F4A13E01        CALL DWORD PTR DS:[0x13EA1F4]                    ; mfc90.#3726
	$-15D    007FD5A5      50                   PUSH EAX
	$-15C    007FD5A6      8D85 54FFFFFF        LEA EAX,DWORD PTR SS:[EBP-0xAC]
	$-156    007FD5AC      50                   PUSH EAX
	$-155    007FD5AD      E8 BE4E5F00          CALL MapleSto.00DF2470
	$-150    007FD5B2      8B00                 MOV EAX,DWORD PTR DS:[EAX]
	$-14E    007FD5B4      0FB60D D8E44E01      MOVZX ECX,BYTE PTR DS:[0x14EE4D8]
	$-147    007FD5BB      50                   PUSH EAX
	$-146    007FD5BC      51                   PUSH ECX
	$-145    007FD5BD      6A 01                PUSH 0x1
	$-143    007FD5BF      C645 FC 06           MOV BYTE PTR SS:[EBP-0x4],0x6
	$-13F    007FD5C3      FF15 90377401        CALL DWORD PTR DS:[0x1743790]                    ; MapleSto.008FCF60
	$-139    007FD5C9      83C4 14              ADD ESP,0x14
	$-136    007FD5CC      8D8D 54FFFFFF        LEA ECX,DWORD PTR SS:[EBP-0xAC]
	$-130    007FD5D2      885D FC              MOV BYTE PTR SS:[EBP-0x4],BL
	$-12D    007FD5D5      E8 764E5F00          CALL MapleSto.00DF2450
	$-128    007FD5DA      8D4D EC              LEA ECX,DWORD PTR SS:[EBP-0x14]
	$-125    007FD5DD      FF15 08A23E01        CALL DWORD PTR DS:[0x13EA208]                    ; mfc90.#601
	$-11F    007FD5E3      B8 F5D47F00          MOV EAX,MapleSto.007FD4F5
	$-11A    007FD5E8      C3                   RETN
	$-119    007FD5E9      CC                   INT3
	$-118    007FD5EA      CC                   INT3
	$-117    007FD5EB      CC                   INT3
	$-116    007FD5EC      CC                   INT3
	$-115    007FD5ED      CC                   INT3
	$-114    007FD5EE      CC                   INT3
	$-113    007FD5EF      CC                   INT3
	$-112    007FD5F0      55                   PUSH EBP
	$-111    007FD5F1      8BEC                 MOV EBP,ESP
	$-10F    007FD5F3      6A FF                PUSH -0x1
	$-10D    007FD5F5      68 223C3401          PUSH MapleSto.01343C22
	$-108    007FD5FA      64:A1 00000000       MOV EAX,DWORD PTR FS:[0]
	$-102    007FD600      50                   PUSH EAX
	$-101    007FD601      51                   PUSH ECX
	$-100    007FD602      56                   PUSH ESI
	$-FF     007FD603      57                   PUSH EDI
	$-FE     007FD604      A1 D0687101          MOV EAX,DWORD PTR DS:[0x17168D0]
	$-F9     007FD609      33C5                 XOR EAX,EBP
	$-F7     007FD60B      50                   PUSH EAX
	$-F6     007FD60C      8D45 F4              LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-F3     007FD60F      64:A3 00000000       MOV DWORD PTR FS:[0],EAX
	$-ED     007FD615      8BF1                 MOV ESI,ECX
	$-EB     007FD617      8975 F0              MOV DWORD PTR SS:[EBP-0x10],ESI
	$-E8     007FD61A      C706 30A14401        MOV DWORD PTR DS:[ESI],MapleSto.0144A130
	$-E2     007FD620      C745 FC 08000000     MOV DWORD PTR SS:[EBP-0x4],0x8
	$-DB     007FD627      E8 7C485801          CALL MapleSto.01D81EA8
	$-D6     007FD62C      8D8E C4000000        LEA ECX,DWORD PTR DS:[ESI+0xC4]
	$-D0     007FD632      FF15 08A23E01        CALL DWORD PTR DS:[0x13EA208]                    ; mfc90.#601
	$-CA     007FD638      8D8E B8000000        LEA ECX,DWORD PTR DS:[ESI+0xB8]
	$-C4     007FD63E      FF15 08A23E01        CALL DWORD PTR DS:[0x13EA208]                    ; mfc90.#601
	$-BE     007FD644      8D8E 9C000000        LEA ECX,DWORD PTR DS:[ESI+0x9C]
	$-B8     007FD64A      E8 F12BF7FF          CALL MapleSto.00770240
	$-B3     007FD64F      8D4E 78              LEA ECX,DWORD PTR DS:[ESI+0x78]
	$-B0     007FD652      E8 E92BF7FF          CALL MapleSto.00770240
	$-AB     007FD657      8D4E 60              LEA ECX,DWORD PTR DS:[ESI+0x60]
	$-A8     007FD65A      E8 410D0000          CALL MapleSto.007FE3A0
	$-A3     007FD65F      8B46 58              MOV EAX,DWORD PTR DS:[ESI+0x58]
	$-A0     007FD662      C645 FC 02           MOV BYTE PTR SS:[EBP-0x4],0x2
	$-9C     007FD666      85C0                 TEST EAX,EAX
	$-9A     007FD668      74 31                JE SHORT MapleSto.007FD69B
	$-98     007FD66A      83C0 04              ADD EAX,0x4
	$-95     007FD66D      50                   PUSH EAX
	$-94     007FD66E      FF15 6C933E01        CALL DWORD PTR DS:[0x13E936C]
	$-8E     007FD674      85C0                 TEST EAX,EAX
	$-8C     007FD676      75 1C                JNZ SHORT MapleSto.007FD694
	$-8A     007FD678      8B46 58              MOV EAX,DWORD PTR DS:[ESI+0x58]
	$-87     007FD67B      83C0 04              ADD EAX,0x4
	$-84     007FD67E      50                   PUSH EAX
	$-83     007FD67F      FF15 F8923E01        CALL DWORD PTR DS:[0x13E92F8]
	$-7D     007FD685      8B4E 58              MOV ECX,DWORD PTR DS:[ESI+0x58]
	$-7A     007FD688      85C9                 TEST ECX,ECX
	$-78     007FD68A      74 08                JE SHORT MapleSto.007FD694
	$-76     007FD68C      8B01                 MOV EAX,DWORD PTR DS:[ECX]
	$-74     007FD68E      8B10                 MOV EDX,DWORD PTR DS:[EAX]
	$-72     007FD690      6A 01                PUSH 0x1
	$-70     007FD692      FFD2                 CALL EDX
	$-6E     007FD694      C746 58 00000000     MOV DWORD PTR DS:[ESI+0x58],0x0
	$-67     007FD69B      8B3D FC913E01        MOV EDI,DWORD PTR DS:[0x13E91FC]                 ; ntdll_1z.RtlDeleteCriticalSection
	$-61     007FD6A1      8D46 38              LEA EAX,DWORD PTR DS:[ESI+0x38]
	$-5E     007FD6A4      50                   PUSH EAX
	$-5D     007FD6A5      C746 34 502E3F01     MOV DWORD PTR DS:[ESI+0x34],MapleSto.013F2E50    ; p/@
	$-56     007FD6AC      FFD7                 CALL EDI
	$-54     007FD6AE      8D46 1C              LEA EAX,DWORD PTR DS:[ESI+0x1C]
	$-51     007FD6B1      50                   PUSH EAX
	$-50     007FD6B2      C746 18 502E3F01     MOV DWORD PTR DS:[ESI+0x18],MapleSto.013F2E50    ; p/@
	$-49     007FD6B9      FFD7                 CALL EDI
	$-47     007FD6BB      8D4E 04              LEA ECX,DWORD PTR DS:[ESI+0x4]
	$-44     007FD6BE      C745 FC FFFFFFFF     MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-3D     007FD6C5      E8 361B0000          CALL MapleSto.007FF200
	$-38     007FD6CA      8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-35     007FD6CD      64:890D 00000000     MOV DWORD PTR FS:[0],ECX
	$-2E     007FD6D4      59                   POP ECX
	$-2D     007FD6D5      5F                   POP EDI
	$-2C     007FD6D6      5E                   POP ESI
	$-2B     007FD6D7      8BE5                 MOV ESP,EBP
	$-29     007FD6D9      5D                   POP EBP
	$-28     007FD6DA      C3                   RETN
	$-27     007FD6DB      CC                   INT3
	$-26     007FD6DC      CC                   INT3
	$-25     007FD6DD      CC                   INT3
	$-24     007FD6DE      CC                   INT3
	$-23     007FD6DF      CC                   INT3
	$-22     007FD6E0      55                   PUSH EBP                                         ; GameSocket发包函数,g_c_game_socket_send_
	$-21     007FD6E1      8BEC                 MOV EBP,ESP                                      ; ecx=stCD_GameSocket*
	$-1F     007FD6E3      6A FF                PUSH -0x1                                        ; p1=stCD_OutPacket*
	$-1D     007FD6E5      68 623C3401          PUSH MapleSto.01343C62
	$-18     007FD6EA      64:A1 00000000       MOV EAX,DWORD PTR FS:[0]
	$-12     007FD6F0      50                   PUSH EAX
	$-11     007FD6F1      81EC 90000000        SUB ESP,0x90
	$-B      007FD6F7      53                   PUSH EBX
	$-A      007FD6F8      56                   PUSH ESI
	$-9      007FD6F9      57                   PUSH EDI
	$-8      007FD6FA      A1 D0687101          MOV EAX,DWORD PTR DS:[0x17168D0]
	$-3      007FD6FF      33C5                 XOR EAX,EBP
	$-1      007FD701      50                   PUSH EAX
	$ ==>    007FD702      8D45 F4              LEA EAX,DWORD PTR SS:[EBP-0xC]
	$+3      007FD705      64:A3 00000000       MOV DWORD PTR FS:[0],EAX
	$+9      007FD70B      8965 F0              MOV DWORD PTR SS:[EBP-0x10],ESP
	$+C      007FD70E      8BF9                 MOV EDI,ECX
	$+E      007FD710      BB 01000000          MOV EBX,0x1
	$+13     007FD715      395F 50              CMP DWORD PTR DS:[EDI+0x50],EBX
	$+16     007FD718      0F85 84000000        JNZ MapleSto.007FD7A2
	$+1C     007FD71E      8B47 18              MOV EAX,DWORD PTR DS:[EDI+0x18]
	$+1F     007FD721      8B50 04              MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+22     007FD724      8D77 18              LEA ESI,DWORD PTR DS:[EDI+0x18]
	$+25     007FD727      8BCE                 MOV ECX,ESI
	$+27     007FD729      C745 FC 00000000     MOV DWORD PTR SS:[EBP-0x4],0x0
	$+2E     007FD730      8975 E8              MOV DWORD PTR SS:[EBP-0x18],ESI
	$+31     007FD733      FFD2                 CALL EDX                                         ; 锁一下
	$+33     007FD735      8B0D 34277201        MOV ECX,DWORD PTR DS:[0x1722734]                 ; then,ecx=stCD_CNClientPlatform*
	$+39     007FD73B      8B01                 MOV EAX,DWORD PTR DS:[ECX]
	$+3B     007FD73D      8B55 08              MOV EDX,DWORD PTR SS:[EBP+0x8]
	$+3E     007FD740      8B80 80000000        MOV EAX,DWORD PTR DS:[EAX+0x80]
	$+44     007FD746      52                   PUSH EDX
	$+45     007FD747      885D FC              MOV BYTE PTR SS:[EBP-0x4],BL
	$+48     007FD74A      FFD0                 CALL EAX
	$+4A     007FD74C      8B8F 94000000        MOV ECX,DWORD PTR DS:[EDI+0x94]
	$+50     007FD752      8D9F 94000000        LEA EBX,DWORD PTR DS:[EDI+0x94]
	$+56     007FD758      894D EC              MOV DWORD PTR SS:[EBP-0x14],ECX
	$+59     007FD75B      8B0D 88447201        MOV ECX,DWORD PTR DS:[0x1724488]                 ; then, ecx=stCD_NetworkService*
	$+5F     007FD761      E8 225C5F01          CALL MapleSto.01DF3388
	$+64     007FD766      8B4D 08              MOV ECX,DWORD PTR SS:[EBP+0x8]
	$+67     007FD769      8D55 EC              LEA EDX,DWORD PTR SS:[EBP-0x14]
	$+6A     007FD76C      52                   PUSH EDX
	$+6B     007FD76D      50                   PUSH EAX
	$+6C     007FD76E      E8 CDFD4401          CALL MapleSto.01C4D540
	$+71     007FD773      8B4D 08              MOV ECX,DWORD PTR SS:[EBP+0x8]
	$+74     007FD776      53                   PUSH EBX
	$+75     007FD777      6A 49                PUSH 0x49
	$+77     007FD779      8D87 9C000000        LEA EAX,DWORD PTR DS:[EDI+0x9C]
	$+7D     007FD77F      50                   PUSH EAX
	$+7E     007FD780      E8 DBDCD2FF          CALL MapleSto.0052B460
	$+83     007FD785      53                   PUSH EBX
	$+84     007FD786      E8 EB95D4FF          CALL MapleSto.00546D76
	$+89     007FD78B      8B16                 MOV EDX,DWORD PTR DS:[ESI]
	$+8B     007FD78D      8B42 08              MOV EAX,DWORD PTR DS:[EDX+0x8]
	$+8E     007FD790      83C4 04              ADD ESP,0x4
	$+91     007FD793      8BCE                 MOV ECX,ESI
	$+93     007FD795      C645 FC 00           MOV BYTE PTR SS:[EBP-0x4],0x0
	$+97     007FD799      FFD0                 CALL EAX
	$+99     007FD79B      8BCF                 MOV ECX,EDI
	$+9B     007FD79D      E8 36605801          CALL MapleSto.01D837D8
	$+A0     007FD7A2      8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+A3     007FD7A5      64:890D 00000000     MOV DWORD PTR FS:[0],ECX
	$+AA     007FD7AC      59                   POP ECX
	$+AB     007FD7AD      5F                   POP EDI
	$+AC     007FD7AE      5E                   POP ESI
	$+AD     007FD7AF      5B                   POP EBX
	$+AE     007FD7B0      8BE5                 MOV ESP,EBP
	$+B0     007FD7B2      5D                   POP EBP
	$+B1     007FD7B3      C2 0400              RETN 0x4
	$+B4     007FD7B6      8B4D E4              MOV ECX,DWORD PTR SS:[EBP-0x1C]
	$+B7     007FD7B9      8B11                 MOV EDX,DWORD PTR DS:[ECX]
	$+B9     007FD7BB      8D45 E8              LEA EAX,DWORD PTR SS:[EBP-0x18]
	$+BC     007FD7BE      50                   PUSH EAX
	$+BD     007FD7BF      8955 EC              MOV DWORD PTR SS:[EBP-0x14],EDX
	$+C0     007FD7C2      E8 B93AE1FF          CALL MapleSto.00611280
	$+C5     007FD7C7      8D4D EC              LEA ECX,DWORD PTR SS:[EBP-0x14]
	$+C8     007FD7CA      51                   PUSH ECX
	$+C9     007FD7CB      50                   PUSH EAX
	$+CA     007FD7CC      8D55 08              LEA EDX,DWORD PTR SS:[EBP+0x8]
	$+CD     007FD7CF      68 A0A14401          PUSH MapleSto.0144A1A0                           ; Socket Send Exception : {0}({1})
	$+D2     007FD7D4      52                   PUSH EDX
	$+D3     007FD7D5      C645 FC 03           MOV BYTE PTR SS:[EBP-0x4],0x3
	$+D7     007FD7D9      E8 82100000          CALL MapleSto.007FE860
	$+DC     007FD7DE      83C4 10              ADD ESP,0x10
	$+DF     007FD7E1      B3 05                MOV BL,0x5
	$+E1     007FD7E3      8D4D E8              LEA ECX,DWORD PTR SS:[EBP-0x18]
	$+E4     007FD7E6      885D FC              MOV BYTE PTR SS:[EBP-0x4],BL
	$+E7     007FD7E9      FF15 08A23E01        CALL DWORD PTR DS:[0x13EA208]                    ; mfc90.#601
	$+ED     007FD7EF      8D4D 08              LEA ECX,DWORD PTR SS:[EBP+0x8]
	$+F0     007FD7F2      FF15 F4A13E01        CALL DWORD PTR DS:[0x13EA1F4]                    ; mfc90.#3726
	$+F6     007FD7F8      50                   PUSH EAX
	$+F7     007FD7F9      E8 F287C4FF          CALL MapleSto.00445FF0
	$+FC     007FD7FE      83C4 04              ADD ESP,0x4
	$+FF     007FD801      833D 90377401 00     CMP DWORD PTR DS:[0x1743790],0x0
	$+106    007FD808      74 3E                JE SHORT MapleSto.007FD848
	$+108    007FD80A      8D4D 08              LEA ECX,DWORD PTR SS:[EBP+0x8]
	$+10B    007FD80D      FF15 F4A13E01        CALL DWORD PTR DS:[0x13EA1F4]                    ; mfc90.#3726
	$+111    007FD813      50                   PUSH EAX
	$+112    007FD814      8D85 64FFFFFF        LEA EAX,DWORD PTR SS:[EBP-0x9C]
	$+118    007FD81A      50                   PUSH EAX
	$+119    007FD81B      E8 504C5F00          CALL MapleSto.00DF2470
	$+11E    007FD820      8B00                 MOV EAX,DWORD PTR DS:[EAX]
	$+120    007FD822      0FB60D D8E44E01      MOVZX ECX,BYTE PTR DS:[0x14EE4D8]
	$+127    007FD829      50                   PUSH EAX
	$+128    007FD82A      51                   PUSH ECX
	$+129    007FD82B      6A 01                PUSH 0x1
	$+12B    007FD82D      C645 FC 06           MOV BYTE PTR SS:[EBP-0x4],0x6
	$+12F    007FD831      FF15 90377401        CALL DWORD PTR DS:[0x1743790]                    ; MapleSto.008FCF60
	$+135    007FD837      83C4 14              ADD ESP,0x14
	$+138    007FD83A      8D8D 64FFFFFF        LEA ECX,DWORD PTR SS:[EBP-0x9C]
	$+13E    007FD840      885D FC              MOV BYTE PTR SS:[EBP-0x4],BL
	$+141    007FD843      E8 084C5F00          CALL MapleSto.00DF2450
	$+146    007FD848      8D4D 08              LEA ECX,DWORD PTR SS:[EBP+0x8]
	$+149    007FD84B      FF15 08A23E01        CALL DWORD PTR DS:[0x13EA208]                    ; mfc90.#601
	$+14F    007FD851      B8 A2D77F00          MOV EAX,MapleSto.007FD7A2
	$+154    007FD856      C3                   RETN
	$+155    007FD857      CC                   INT3
	$+156    007FD858      CC                   INT3
	$+157    007FD859      CC                   INT3
	$+158    007FD85A      CC                   INT3
	$+159    007FD85B      CC                   INT3
	$+15A    007FD85C      CC                   INT3
	$+15B    007FD85D      CC                   INT3
	$+15C    007FD85E      CC                   INT3
	$+15D    007FD85F      CC                   INT3
	$+15E    007FD860      55                   PUSH EBP
	$+15F    007FD861      8BEC                 MOV EBP,ESP
	$+161    007FD863      6A FF                PUSH -0x1
	$+163    007FD865      68 803C3401          PUSH MapleSto.01343C80
	$+168    007FD86A      64:A1 00000000       MOV EAX,DWORD PTR FS:[0]
	$+16E    007FD870      50                   PUSH EAX
	$+16F    007FD871      83EC 14              SUB ESP,0x14
	$+172    007FD874      53                   PUSH EBX
	$+173    007FD875      56                   PUSH ESI
	$+174    007FD876      57                   PUSH EDI
	$+175    007FD877      A1 D0687101          MOV EAX,DWORD PTR DS:[0x17168D0]
	$+17A    007FD87C      33C5                 XOR EAX,EBP
	$+17C    007FD87E      50                   PUSH EAX
	$+17D    007FD87F      8D45 F4              LEA EAX,DWORD PTR SS:[EBP-0xC]
	$+180    007FD882      64:A3 00000000       MOV DWORD PTR FS:[0],EAX
	$+186    007FD888      8965 F0              MOV DWORD PTR SS:[EBP-0x10],ESP
	$+189    007FD88B      894D EC              MOV DWORD PTR SS:[EBP-0x14],ECX
	$+18C    007FD88E      8B45 0C              MOV EAX,DWORD PTR SS:[EBP+0xC]
	$+18F    007FD891      C745 FC 00000000     MOV DWORD PTR SS:[EBP-0x4],0x0
	$+196    007FD898      85C0                 TEST EAX,EAX
	$+198    007FD89A      74 1A                JE SHORT MapleSto.007FD8B6
	$+19A    007FD89C      8D4D E8              LEA ECX,DWORD PTR SS:[EBP-0x18]
	$+19D    007FD89F      8945 E4              MOV DWORD PTR SS:[EBP-0x1C],EAX
	$+1A0    007FD8A2      FF15 00A23E01        CALL DWORD PTR DS:[0x13EA200]                    ; mfc90.#316
	$+1A6    007FD8A8      68 F43D5D01          PUSH MapleSto.015D3DF4
	$+1AB    007FD8AD      8D45 E4              LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$+1AE    007FD8B0      50                   PUSH EAX
	$+1AF    007FD8B1      E8 B23EA000          CALL MapleSto.01201768                           ; JMP to msvcr90._CxxThrowException
	$+1B4    007FD8B6      E8 1D5F5801          CALL MapleSto.01D837D8
	$+1B9    007FD8BB      8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+1BC    007FD8BE      64:890D 00000000     MOV DWORD PTR FS:[0],ECX
	$+1C3    007FD8C5      59                   POP ECX
	$+1C4    007FD8C6      5F                   POP EDI
	$+1C5    007FD8C7      5E                   POP ESI
	$+1C6    007FD8C8      5B                   POP EBX
	$+1C7    007FD8C9      8BE5                 MOV ESP,EBP
	$+1C9    007FD8CB      5D                   POP EBP
	$+1CA    007FD8CC      C2 0800              RETN 0x8
	$+1CD    007FD8CF      8B75 0C              MOV ESI,DWORD PTR SS:[EBP+0xC]
	$+1D0    007FD8D2      8B0D B8607201        MOV ECX,DWORD PTR DS:[0x17260B8]
	$+1D6    007FD8D8      6A 00                PUSH 0x0
	$+1D8    007FD8DA      56                   PUSH ESI
	$+1D9    007FD8DB      68 66040000          PUSH 0x466
	$+1DE    007FD8E0      E8 6BC5E6FF          CALL MapleSto.00669E50
	$+1E3    007FD8E5      50                   PUSH EAX
	$+1E4    007FD8E6      FF15 789B3E01        CALL DWORD PTR DS:[0x13E9B78]
	$+1EA    007FD8EC      8B4D EC              MOV ECX,DWORD PTR SS:[EBP-0x14]
	$+1ED    007FD8EF      56                   PUSH ESI
	$+1EE    007FD8F0      E8 DDDD5801          CALL MapleSto.01D8B6D2
	$+1F3    007FD8F5      B8 BBD87F00          MOV EAX,MapleSto.007FD8BB
	$+1F8    007FD8FA      C3                   RETN
	$+1F9    007FD8FB      CC                   INT3
	$+1FA    007FD8FC      CC                   INT3
	$+1FB    007FD8FD      CC                   INT3
	$+1FC    007FD8FE      CC                   INT3
	$+1FD    007FD8FF      CC                   INT3
	$+1FE    007FD900      55                   PUSH EBP
	$+1FF    007FD901      8BEC                 MOV EBP,ESP
	$+201    007FD903      6A FF                PUSH -0x1
	$+203    007FD905      68 AC3C3401          PUSH MapleSto.01343CAC
	$+208    007FD90A      64:A1 00000000       MOV EAX,DWORD PTR FS:[0]
	$+20E    007FD910      50                   PUSH EAX
	$+20F    007FD911      83EC 30              SUB ESP,0x30
	$+212    007FD914      53                   PUSH EBX
	$+213    007FD915      56                   PUSH ESI
	$+214    007FD916      57                   PUSH EDI
	$+215    007FD917      A1 D0687101          MOV EAX,DWORD PTR DS:[0x17168D0]
	$+21A    007FD91C      33C5                 XOR EAX,EBP
	$+21C    007FD91E      50                   PUSH EAX
	$+21D    007FD91F      8D45 F4              LEA EAX,DWORD PTR SS:[EBP-0xC]
	$+220    007FD922      64:A3 00000000       MOV DWORD PTR FS:[0],EAX
	$+226    007FD928      8965 F0              MOV DWORD PTR SS:[EBP-0x10],ESP
	$+229    007FD92B      8BF1                 MOV ESI,ECX
	$+22B    007FD92D      8975 E8              MOV DWORD PTR SS:[EBP-0x18],ESI
	$+22E    007FD930      8B46 18              MOV EAX,DWORD PTR DS:[ESI+0x18]
	$+231    007FD933      8B50 04              MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+234    007FD936      8D7E 18              LEA EDI,DWORD PTR DS:[ESI+0x18]
	$+237    007FD939      33DB                 XOR EBX,EBX
	$+239    007FD93B      8BCF                 MOV ECX,EDI
	$+23B    007FD93D      895D FC              MOV DWORD PTR SS:[EBP-0x4],EBX
	$+23E    007FD940      897D E4              MOV DWORD PTR SS:[EBP-0x1C],EDI
	$+241    007FD943      897D E0              MOV DWORD PTR SS:[EBP-0x20],EDI
	$+244    007FD946      FFD2                 CALL EDX
	$+246    007FD948      B8 02000000          MOV EAX,0x2
	$+24B    007FD94D      3946 50              CMP DWORD PTR DS:[ESI+0x50],EAX
	$+24E    007FD950      74 11                JE SHORT MapleSto.007FD963
	$+250    007FD952      8B07                 MOV EAX,DWORD PTR DS:[EDI]
	$+252    007FD954      8B50 08              MOV EDX,DWORD PTR DS:[EAX+0x8]
	$+255    007FD957      8BCF                 MOV ECX,EDI
	$+257    007FD959      885D FC              MOV BYTE PTR SS:[EBP-0x4],BL
	$+25A    007FD95C      FFD2                 CALL EDX
	$+25C    007FD95E      E9 6E010000          JMP MapleSto.007FDAD1
	$+261    007FD963      6A 00                PUSH 0x0
	$+263    007FD965      899E B4000000        MOV DWORD PTR DS:[ESI+0xB4],EBX
	$+269    007FD96B      895E 50              MOV DWORD PTR DS:[ESI+0x50],EBX
	$+26C    007FD96E      50                   PUSH EAX
	$+26D    007FD96F      8D5E 08              LEA EBX,DWORD PTR DS:[ESI+0x8]
	$+270    007FD972      6A 01                PUSH 0x1
	$+272    007FD974      8BCB                 MOV ECX,EBX
	$+274    007FD976      8845 FC              MOV BYTE PTR SS:[EBP-0x4],AL
	$+277    007FD979      E8 79985801          CALL MapleSto.01D871F7
	$+27C    007FD97E      8B03                 MOV EAX,DWORD PTR DS:[EBX]
	$+27E    007FD980      8D4D EC              LEA ECX,DWORD PTR SS:[EBP-0x14]
	$+281    007FD983      51                   PUSH ECX
	$+282    007FD984      68 7E660480          PUSH 0x8004667E
	$+287    007FD989      50                   PUSH EAX
	$+288    007FD98A      C745 EC 01000000     MOV DWORD PTR SS:[EBP-0x14],0x1
	$+28F    007FD991      FF15 2C9D3E01        CALL DWORD PTR DS:[0x13E9D2C]
	$+295    007FD997      83F8 FF              CMP EAX,-0x1
	$+298    007FD99A      75 20                JNZ SHORT MapleSto.007FD9BC
	$+29A    007FD99C      FF15 349D3E01        CALL DWORD PTR DS:[0x13E9D34]
	$+2A0    007FD9A2      8D4D DC              LEA ECX,DWORD PTR SS:[EBP-0x24]
	$+2A3    007FD9A5      8945 D8              MOV DWORD PTR SS:[EBP-0x28],EAX
	$+2A6    007FD9A8      FF15 00A23E01        CALL DWORD PTR DS:[0x13EA200]                    ; mfc90.#316
	$+2AC    007FD9AE      68 F43D5D01          PUSH MapleSto.015D3DF4
	$+2B1    007FD9B3      8D55 D8              LEA EDX,DWORD PTR SS:[EBP-0x28]
	$+2B4    007FD9B6      52                   PUSH EDX
	$+2B5    007FD9B7      E8 AC3DA000          CALL MapleSto.01201768                           ; JMP to msvcr90._CxxThrowException
	$+2BA    007FD9BC      6A 00                PUSH 0x0
	$+2BC    007FD9BE      6A 33                PUSH 0x33
	$+2BE    007FD9C0      8D4E 04              LEA ECX,DWORD PTR DS:[ESI+0x4]
	$+2C1    007FD9C3      8976 14              MOV DWORD PTR DS:[ESI+0x14],ESI
	$+2C4    007FD9C6      E8 15200000          CALL MapleSto.007FF9E0
	$+2C9    007FD9CB      8D45 08              LEA EAX,DWORD PTR SS:[EBP+0x8]
	$+2CC    007FD9CE      50                   PUSH EAX
	$+2CD    007FD9CF      8D8E B8000000        LEA ECX,DWORD PTR DS:[ESI+0xB8]
	$+2D3    007FD9D5      FF15 D4A13E01        CALL DWORD PTR DS:[0x13EA1D4]                    ; mfc90.#817
	$+2D9    007FD9DB      8B4D 0C              MOV ECX,DWORD PTR SS:[EBP+0xC]
	$+2DC    007FD9DE      8B55 14              MOV EDX,DWORD PTR SS:[EBP+0x14]
	$+2DF    007FD9E1      8D45 08              LEA EAX,DWORD PTR SS:[EBP+0x8]
	$+2E2    007FD9E4      898E BC000000        MOV DWORD PTR DS:[ESI+0xBC],ECX
	$+2E8    007FD9EA      50                   PUSH EAX
	$+2E9    007FD9EB      8D8E C4000000        LEA ECX,DWORD PTR DS:[ESI+0xC4]
	$+2EF    007FD9F1      8996 C0000000        MOV DWORD PTR DS:[ESI+0xC0],EDX
	$+2F5    007FD9F7      FF15 D4A13E01        CALL DWORD PTR DS:[0x13EA1D4]                    ; mfc90.#817
	$+2FB    007FD9FD      8B45 0C              MOV EAX,DWORD PTR SS:[EBP+0xC]
	$+2FE    007FDA00      8B4D 10              MOV ECX,DWORD PTR SS:[EBP+0x10]
	$+301    007FDA03      898E CC000000        MOV DWORD PTR DS:[ESI+0xCC],ECX
	$+307    007FDA09      50                   PUSH EAX
	$+308    007FDA0A      8D4D 08              LEA ECX,DWORD PTR SS:[EBP+0x8]
	$+30B    007FDA0D      8986 C8000000        MOV DWORD PTR DS:[ESI+0xC8],EAX
	$+311    007FDA13      FF15 F4A13E01        CALL DWORD PTR DS:[0x13EA1F4]                    ; mfc90.#3726
	$+317    007FDA19      50                   PUSH EAX
	$+318    007FDA1A      8D4D C8              LEA ECX,DWORD PTR SS:[EBP-0x38]
	$+31B    007FDA1D      E8 0E0B0000          CALL MapleSto.007FE530
	$+320    007FDA22      8B1B                 MOV EBX,DWORD PTR DS:[EBX]
	$+322    007FDA24      6A 10                PUSH 0x10
	$+324    007FDA26      50                   PUSH EAX
	$+325    007FDA27      53                   PUSH EBX
	$+326    007FDA28      FF15 289D3E01        CALL DWORD PTR DS:[0x13E9D28]
	$+32C    007FDA2E      83F8 FF              CMP EAX,-0x1
	$+32F    007FDA31      75 11                JNZ SHORT MapleSto.007FDA44
	$+331    007FDA33      FF15 349D3E01        CALL DWORD PTR DS:[0x13E9D34]
	$+337    007FDA39      3D 33270000          CMP EAX,0x2733
	$+33C    007FDA3E      74 04                JE SHORT MapleSto.007FDA44
	$+33E    007FDA40      33C0                 XOR EAX,EAX
	$+340    007FDA42      EB 05                JMP SHORT MapleSto.007FDA49
	$+342    007FDA44      B8 01000000          MOV EAX,0x1
	$+347    007FDA49      83F8 01              CMP EAX,0x1
	$+34A    007FDA4C      0F94C0               SETE AL
	$+34D    007FDA4F      84C0                 TEST AL,AL
	$+34F    007FDA51      74 2F                JE SHORT MapleSto.007FDA82
	$+351    007FDA53      8B17                 MOV EDX,DWORD PTR DS:[EDI]
	$+353    007FDA55      8B42 08              MOV EAX,DWORD PTR DS:[EDX+0x8]
	$+356    007FDA58      8BCF                 MOV ECX,EDI
	$+358    007FDA5A      C645 FC 00           MOV BYTE PTR SS:[EBP-0x4],0x0
	$+35C    007FDA5E      FFD0                 CALL EAX
	$+35E    007FDA60      8D4D 08              LEA ECX,DWORD PTR SS:[EBP+0x8]
	$+361    007FDA63      FF15 08A23E01        CALL DWORD PTR DS:[0x13EA208]                    ; mfc90.#601
	$+367    007FDA69      B8 01000000          MOV EAX,0x1
	$+36C    007FDA6E      8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+36F    007FDA71      64:890D 00000000     MOV DWORD PTR FS:[0],ECX
	$+376    007FDA78      59                   POP ECX
	$+377    007FDA79      5F                   POP EDI
	$+378    007FDA7A      5E                   POP ESI
	$+379    007FDA7B      5B                   POP EBX
	$+37A    007FDA7C      8BE5                 MOV ESP,EBP
	$+37C    007FDA7E      5D                   POP EBP
	$+37D    007FDA7F      C2 1000              RETN 0x10
	$+380    007FDA82      C745 FC 01000000     MOV DWORD PTR SS:[EBP-0x4],0x1
	$+387    007FDA89      EB 13                JMP SHORT MapleSto.007FDA9E
	$+389    007FDA8B      C745 FC 01000000     MOV DWORD PTR SS:[EBP-0x4],0x1
	$+390    007FDA92      B8 98DA7F00          MOV EAX,MapleSto.007FDA98
	$+395    007FDA97      C3                   RETN
	$+396    007FDA98      8B75 E8              MOV ESI,DWORD PTR SS:[EBP-0x18]
	$+399    007FDA9B      8B7D E4              MOV EDI,DWORD PTR SS:[EBP-0x1C]
	$+39C    007FDA9E      8B17                 MOV EDX,DWORD PTR DS:[EDI]
	$+39E    007FDAA0      8B42 08              MOV EAX,DWORD PTR DS:[EDX+0x8]
	$+3A1    007FDAA3      8BCF                 MOV ECX,EDI
	$+3A3    007FDAA5      C645 FC 00           MOV BYTE PTR SS:[EBP-0x4],0x0
	$+3A7    007FDAA9      FFD0                 CALL EAX
	$+3A9    007FDAAB      FF15 349D3E01        CALL DWORD PTR DS:[0x13E9D34]
	$+3AF    007FDAB1      837D 10 00           CMP DWORD PTR SS:[EBP+0x10],0x0
	$+3B3    007FDAB5      8BF8                 MOV EDI,EAX
	$+3B5    007FDAB7      74 09                JE SHORT MapleSto.007FDAC2
	$+3B7    007FDAB9      57                   PUSH EDI
	$+3B8    007FDABA      E8 66895801          CALL MapleSto.01D86425
	$+3BD    007FDABF      83C4 04              ADD ESP,0x4
	$+3C0    007FDAC2      57                   PUSH EDI
	$+3C1    007FDAC3      8BCE                 MOV ECX,ESI
	$+3C3    007FDAC5      E8 08DC5801          CALL MapleSto.01D8B6D2
	$+3C8    007FDACA      8BCE                 MOV ECX,ESI
	$+3CA    007FDACC      E8 D7435801          CALL MapleSto.01D81EA8
	$+3CF    007FDAD1      8D4D 08              LEA ECX,DWORD PTR SS:[EBP+0x8]
	$+3D2    007FDAD4      FF15 08A23E01        CALL DWORD PTR DS:[0x13EA208]                    ; mfc90.#601
	$+3D8    007FDADA      33C0                 XOR EAX,EAX
	$+3DA    007FDADC      8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+3DD    007FDADF      64:890D 00000000     MOV DWORD PTR FS:[0],ECX
	$+3E4    007FDAE6      59                   POP ECX
	$+3E5    007FDAE7      5F                   POP EDI
	$+3E6    007FDAE8      5E                   POP ESI
	$+3E7    007FDAE9      5B                   POP EBX
	$+3E8    007FDAEA      8BE5                 MOV ESP,EBP
	$+3EA    007FDAEC      5D                   POP EBP
	$+3EB    007FDAED      C2 1000              RETN 0x10
	*/
	addr_tmp = scan_exe_.ScanAddr("8d????64??????????89????8b??bb????????39????0f??????????8b????8b????8d", -0x22);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	if (AsByte(addr_tmp, 0) != 0x55)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_c_game_socket_send_ = " << addr_tmp << ";" << FC_OFstream_EndlLog;

	/*
	$-1D3    00804E4A      57                   PUSH EDI
	$-1D2    00804E4B      017CCC CC            ADD DWORD PTR SS:[ESP+ECX*8-0x34],EDI
	$-1CE    00804E4F      CC                   INT3
	$-1CD    00804E50      55                   PUSH EBP
	$-1CC    00804E51      8BEC                 MOV EBP,ESP
	$-1CA    00804E53      6A FF                PUSH -0x1
	$-1C8    00804E55      68 F8493401          PUSH MapleSto.013449F8
	$-1C3    00804E5A      64:A1 00000000       MOV EAX,DWORD PTR FS:[0]
	$-1BD    00804E60      50                   PUSH EAX
	$-1BC    00804E61      83EC 28              SUB ESP,0x28
	$-1B9    00804E64      56                   PUSH ESI
	$-1B8    00804E65      A1 D0687101          MOV EAX,DWORD PTR DS:[0x17168D0]
	$-1B3    00804E6A      33C5                 XOR EAX,EBP
	$-1B1    00804E6C      50                   PUSH EAX
	$-1B0    00804E6D      8D45 F4              LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-1AD    00804E70      64:A3 00000000       MOV DWORD PTR FS:[0],EAX
	$-1A7    00804E76      8B75 08              MOV ESI,DWORD PTR SS:[EBP+0x8]
	$-1A4    00804E79      8B46 04              MOV EAX,DWORD PTR DS:[ESI+0x4]
	$-1A1    00804E7C      83F8 04              CMP EAX,0x4
	$-19E    00804E7F      0F85 96000000        JNZ MapleSto.00804F1B
	$-198    00804E85      8D4D 08              LEA ECX,DWORD PTR SS:[EBP+0x8]
	$-195    00804E88      FF15 E0A13E01        CALL DWORD PTR DS:[0x13EA1E0]                    ; mfc90.#316
	$-18F    00804E8E      8D45 08              LEA EAX,DWORD PTR SS:[EBP+0x8]
	$-18C    00804E91      50                   PUSH EAX
	$-18B    00804E92      8D4E 0C              LEA ECX,DWORD PTR DS:[ESI+0xC]
	$-188    00804E95      51                   PUSH ECX
	$-187    00804E96      C745 FC 00000000     MOV DWORD PTR SS:[EBP-0x4],0x0
	$-180    00804E9D      E8 DEF6FFFF          CALL MapleSto.00804580
	$-17B    00804EA2      83C4 08              ADD ESP,0x8
	$-178    00804EA5      85C0                 TEST EAX,EAX
	$-176    00804EA7      74 62                JE SHORT MapleSto.00804F0B
	$-174    00804EA9      33D2                 XOR EDX,EDX
	$-172    00804EAB      83F8 09              CMP EAX,0x9
	$-16F    00804EAE      0F95C2               SETNE DL
	$-16C    00804EB1      81C2 05040000        ADD EDX,0x405
	$-166    00804EB7      8BC2                 MOV EAX,EDX
	$-164    00804EB9      50                   PUSH EAX
	$-163    00804EBA      8D45 F0              LEA EAX,DWORD PTR SS:[EBP-0x10]
	$-160    00804EBD      50                   PUSH EAX
	$-15F    00804EBE      E8 BD25C4FF          CALL MapleSto.00447480
	$-15A    00804EC3      83C4 08              ADD ESP,0x8
	$-157    00804EC6      8BF0                 MOV ESI,EAX
	$-155    00804EC8      C645 FC 01           MOV BYTE PTR SS:[EBP-0x4],0x1
	$-151    00804ECC      E8 FE5F5E01          CALL MapleSto.01DEAECF
	$-14C    00804ED1      8378 24 00           CMP DWORD PTR DS:[EAX+0x24],0x0
	$-148    00804ED5      74 10                JE SHORT MapleSto.00804EE7
	$-146    00804ED7      6A 0E                PUSH 0xE
	$-144    00804ED9      56                   PUSH ESI
	$-143    00804EDA      E8 F05F5E01          CALL MapleSto.01DEAECF
	$-13E    00804EDF      8B48 24              MOV ECX,DWORD PTR DS:[EAX+0x24]
	$-13B    00804EE2      E8 39002E00          CALL MapleSto.00AE4F20
	$-136    00804EE7      8D4D F0              LEA ECX,DWORD PTR SS:[EBP-0x10]
	$-133    00804EEA      FF15 DCA13E01        CALL DWORD PTR DS:[0x13EA1DC]                    ; mfc90.#601
	$-12D    00804EF0      8D4D 08              LEA ECX,DWORD PTR SS:[EBP+0x8]
	$-12A    00804EF3      FF15 DCA13E01        CALL DWORD PTR DS:[0x13EA1DC]                    ; mfc90.#601
	$-124    00804EF9      33C0                 XOR EAX,EAX
	$-122    00804EFB      8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-11F    00804EFE      64:890D 00000000     MOV DWORD PTR FS:[0],ECX
	$-118    00804F05      59                   POP ECX
	$-117    00804F06      5E                   POP ESI
	$-116    00804F07      8BE5                 MOV ESP,EBP
	$-114    00804F09      5D                   POP EBP
	$-113    00804F0A      C3                   RETN
	$-112    00804F0B      8D4D 08              LEA ECX,DWORD PTR SS:[EBP+0x8]
	$-10F    00804F0E      C745 FC FFFFFFFF     MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-108    00804F15      FF15 DCA13E01        CALL DWORD PTR DS:[0x13EA1DC]                    ; mfc90.#601
	$-102    00804F1B      6A 02                PUSH 0x2
	$-100    00804F1D      8D4D EC              LEA ECX,DWORD PTR SS:[EBP-0x14]
	$-FD     00804F20      51                   PUSH ECX
	$-FC     00804F21      8B0D 40417201        MOV ECX,DWORD PTR DS:[0x1724140]
	$-F6     00804F27      E8 0E974A01          CALL MapleSto.01CAE63A
	$-F1     00804F2C      8B45 F0              MOV EAX,DWORD PTR SS:[EBP-0x10]
	$-EE     00804F2F      C745 FC 02000000     MOV DWORD PTR SS:[EBP-0x4],0x2
	$-E7     00804F36      85C0                 TEST EAX,EAX
	$-E5     00804F38      0F84 CC000000        JE MapleSto.0080500A
	$-DF     00804F3E      8378 20 02           CMP DWORD PTR DS:[EAX+0x20],0x2
	$-DB     00804F42      75 27                JNZ SHORT MapleSto.00804F6B
	$-D9     00804F44      8D55 08              LEA EDX,DWORD PTR SS:[EBP+0x8]
	$-D6     00804F47      68 08040000          PUSH 0x408
	$-D1     00804F4C      52                   PUSH EDX
	$-D0     00804F4D      E8 2E25C4FF          CALL MapleSto.00447480
	$-CB     00804F52      83C4 08              ADD ESP,0x8
	$-C8     00804F55      8BF0                 MOV ESI,EAX
	$-C6     00804F57      C645 FC 03           MOV BYTE PTR SS:[EBP-0x4],0x3
	$-C2     00804F5B      E8 6F5F5E01          CALL MapleSto.01DEAECF
	$-BD     00804F60      8378 24 00           CMP DWORD PTR DS:[EAX+0x24],0x0
	$-B9     00804F64      74 51                JE SHORT MapleSto.00804FB7
	$-B7     00804F66      6A 0E                PUSH 0xE
	$-B5     00804F68      56                   PUSH ESI
	$-B4     00804F69      EB 3F                JMP SHORT MapleSto.00804FAA
	$-B2     00804F6B      8B48 34              MOV ECX,DWORD PTR DS:[EAX+0x34]
	$-AF     00804F6E      8B50 30              MOV EDX,DWORD PTR DS:[EAX+0x30]
	$-AC     00804F71      51                   PUSH ECX
	$-AB     00804F72      8B48 2C              MOV ECX,DWORD PTR DS:[EAX+0x2C]
	$-A8     00804F75      52                   PUSH EDX
	$-A7     00804F76      8B50 28              MOV EDX,DWORD PTR DS:[EAX+0x28]
	$-A4     00804F79      51                   PUSH ECX
	$-A3     00804F7A      52                   PUSH EDX
	$-A2     00804F7B      8D45 08              LEA EAX,DWORD PTR SS:[EBP+0x8]
	$-9F     00804F7E      50                   PUSH EAX
	$-9E     00804F7F      E8 7C4CDEFF          CALL MapleSto.005E9C00
	$-99     00804F84      6A 01                PUSH 0x1
	$-97     00804F86      6A 01                PUSH 0x1
	$-95     00804F88      68 19040000          PUSH 0x419
	$-90     00804F8D      C645 FC 04           MOV BYTE PTR SS:[EBP-0x4],0x4
	$-8C     00804F91      E8 5FBDFFFF          CALL MapleSto.00800CF5
	$-87     00804F96      83C4 20              ADD ESP,0x20
	$-84     00804F99      E8 315F5E01          CALL MapleSto.01DEAECF
	$-7F     00804F9E      8378 24 00           CMP DWORD PTR DS:[EAX+0x24],0x0
	$-7B     00804FA2      74 13                JE SHORT MapleSto.00804FB7
	$-79     00804FA4      6A 0E                PUSH 0xE
	$-77     00804FA6      8D4D 08              LEA ECX,DWORD PTR SS:[EBP+0x8]
	$-74     00804FA9      51                   PUSH ECX
	$-73     00804FAA      E8 205F5E01          CALL MapleSto.01DEAECF
	$-6E     00804FAF      8B48 24              MOV ECX,DWORD PTR DS:[EAX+0x24]
	$-6B     00804FB2      E8 69FF2D00          CALL MapleSto.00AE4F20
	$-66     00804FB7      8D4D 08              LEA ECX,DWORD PTR SS:[EBP+0x8]
	$-63     00804FBA      FF15 DCA13E01        CALL DWORD PTR DS:[0x13EA1DC]                    ; mfc90.#601
	$-5D     00804FC0      8B45 F0              MOV EAX,DWORD PTR SS:[EBP-0x10]
	$-5A     00804FC3      C745 FC FFFFFFFF     MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-53     00804FCA      85C0                 TEST EAX,EAX
	$-51     00804FCC      74 2A                JE SHORT MapleSto.00804FF8
	$-4F     00804FCE      83C0 04              ADD EAX,0x4
	$-4C     00804FD1      50                   PUSH EAX
	$-4B     00804FD2      FF15 6C933E01        CALL DWORD PTR DS:[0x13E936C]
	$-45     00804FD8      85C0                 TEST EAX,EAX
	$-43     00804FDA      75 1C                JNZ SHORT MapleSto.00804FF8
	$-41     00804FDC      8B55 F0              MOV EDX,DWORD PTR SS:[EBP-0x10]
	$-3E     00804FDF      83C2 04              ADD EDX,0x4
	$-3B     00804FE2      52                   PUSH EDX
	$-3A     00804FE3      FF15 F8923E01        CALL DWORD PTR DS:[0x13E92F8]
	$-34     00804FE9      8B4D F0              MOV ECX,DWORD PTR SS:[EBP-0x10]
	$-31     00804FEC      85C9                 TEST ECX,ECX
	$-2F     00804FEE      74 08                JE SHORT MapleSto.00804FF8
	$-2D     00804FF0      8B01                 MOV EAX,DWORD PTR DS:[ECX]
	$-2B     00804FF2      8B10                 MOV EDX,DWORD PTR DS:[EAX]
	$-29     00804FF4      6A 01                PUSH 0x1
	$-27     00804FF6      FFD2                 CALL EDX
	$-25     00804FF8      33C0                 XOR EAX,EAX
	$-23     00804FFA      8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-20     00804FFD      64:890D 00000000     MOV DWORD PTR FS:[0],ECX
	$-19     00805004      59                   POP ECX
	$-18     00805005      5E                   POP ESI
	$-17     00805006      8BE5                 MOV ESP,EBP
	$-15     00805008      5D                   POP EBP
	$-14     00805009      C3                   RETN
	$-13     0080500A      6A 00                PUSH 0x0
	$-11     0080500C      6A 0B                PUSH 0xB
	$-F      0080500E      8D4D CC              LEA ECX,DWORD PTR SS:[EBP-0x34]
	$-C      00805011      C745 FC FFFFFFFF     MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-5      00805018      E8 F361D2FF          CALL MapleSto.0052B210                           ; 构造OutPacket,g_c_construct_out_packet_
	$ ==>    0080501D      8D45 CC              LEA EAX,DWORD PTR SS:[EBP-0x34]
	$+3      00805020      50                   PUSH EAX
	$+4      00805021      8BCE                 MOV ECX,ESI
	$+6      00805023      C745 FC 05000000     MOV DWORD PTR SS:[EBP-0x4],0x5
	$+D      0080502A      E8 D12DC1FF          CALL MapleSto.00417E00
	$+12     0080502F      8D4D CC              LEA ECX,DWORD PTR SS:[EBP-0x34]
	$+15     00805032      E8 BAFE4401          CALL MapleSto.01C54EF1                           ; OutPacket发包，g_c_out_packet_send_
	$+1A     00805037      8B45 D4              MOV EAX,DWORD PTR SS:[EBP-0x2C]
	$+1D     0080503A      C745 CC BC504101     MOV DWORD PTR SS:[EBP-0x34],MapleSto.014150BC
	$+24     00805041      85C0                 TEST EAX,EAX
	$+26     00805043      74 0A                JE SHORT MapleSto.0080504F
	$+28     00805045      50                   PUSH EAX
	$+29     00805046      FF15 E8983E01        CALL DWORD PTR DS:[0x13E98E8]                    ; msvcr90.free
	$+2F     0080504C      83C4 04              ADD ESP,0x4
	$+32     0080504F      B8 01000000          MOV EAX,0x1
	$+37     00805054      8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+3A     00805057      64:890D 00000000     MOV DWORD PTR FS:[0],ECX
	$+41     0080505E      59                   POP ECX
	$+42     0080505F      5E                   POP ESI
	$+43     00805060      8BE5                 MOV ESP,EBP
	$+45     00805062      5D                   POP EBP
	$+46     00805063      C3                   RETN
	$+47     00805064      CC                   INT3
	$+48     00805065      CC                   INT3
	$+49     00805066      CC                   INT3
	$+4A     00805067      CC                   INT3
	$+4B     00805068      CC                   INT3
	$+4C     00805069      CC                   INT3
	$+4D     0080506A      CC                   INT3
	$+4E     0080506B      CC                   INT3
	$+4F     0080506C      CC                   INT3
	$+50     0080506D      CC                   INT3
	$+51     0080506E      CC                   INT3
	$+52     0080506F      CC                   INT3
	$+53     00805070      55                   PUSH EBP
	$+54     00805071      8BEC                 MOV EBP,ESP
	$+56     00805073      83EC 08              SUB ESP,0x8
	$+59     00805076      56                   PUSH ESI
	$+5A     00805077      8B75 08              MOV ESI,DWORD PTR SS:[EBP+0x8]
	$+5D     0080507A      D97D 0A              FSTCW WORD PTR SS:[EBP+0xA]
	$+60     0080507D      0FB745 0A            MOVZX EAX,WORD PTR SS:[EBP+0xA]
	$+64     00805081      D906                 FLD DWORD PTR DS:[ESI]
	$+66     00805083      0D 000C0000          OR EAX,0xC00
	$+6B     00805088      8945 FC              MOV DWORD PTR SS:[EBP-0x4],EAX
	$+6E     0080508B      57                   PUSH EDI
	$+6F     0080508C      8B7D 10              MOV EDI,DWORD PTR SS:[EBP+0x10]
	$+72     0080508F      D96D FC              FLDCW WORD PTR SS:[EBP-0x4]
	$+75     00805092      DF7D F8              FISTP QWORD PTR SS:[EBP-0x8]
	$+78     00805095      8B55 F8              MOV EDX,DWORD PTR SS:[EBP-0x8]
	$+7B     00805098      D96D 0A              FLDCW WORD PTR SS:[EBP+0xA]
	$+7E     0080509B      69D2 E8030000        IMUL EDX,EDX,0x3E8
	$+84     008050A1      D946 04              FLD DWORD PTR DS:[ESI+0x4]
	$+87     008050A4      D80D 44584401        FMUL DWORD PTR DS:[0x1445844]
	$+8D     008050AA      D97D 0A              FSTCW WORD PTR SS:[EBP+0xA]
	$+90     008050AD      0FB745 0A            MOVZX EAX,WORD PTR SS:[EBP+0xA]
	$+94     008050B1      0D 000C0000          OR EAX,0xC00
	$+99     008050B6      8945 FC              MOV DWORD PTR SS:[EBP-0x4],EAX
	$+9C     008050B9      D96D FC              FLDCW WORD PTR SS:[EBP-0x4]
	$+9F     008050BC      DF7D F8              FISTP QWORD PTR SS:[EBP-0x8]
	$+A2     008050BF      8B45 F8              MOV EAX,DWORD PTR SS:[EBP-0x8]
	$+A5     008050C2      2BD0                 SUB EDX,EAX
	$+A7     008050C4      8917                 MOV DWORD PTR DS:[EDI],EDX
	$+A9     008050C6      8BC2                 MOV EAX,EDX
	$+AB     008050C8      D96D 0A              FLDCW WORD PTR SS:[EBP+0xA]
	$+AE     008050CB      33D2                 XOR EDX,EDX
	$+B0     008050CD      F771 08              DIV DWORD PTR DS:[ECX+0x8]
	$+B3     008050D0      8B45 0C              MOV EAX,DWORD PTR SS:[EBP+0xC]
	$+B6     008050D3      8910                 MOV DWORD PTR DS:[EAX],EDX
	$+B8     008050D5      8339 00              CMP DWORD PTR DS:[ECX],0x0
	$+BB     008050D8      75 0A                JNZ SHORT MapleSto.008050E4
	$+BD     008050DA      5F                   POP EDI
	$+BE     008050DB      33C0                 XOR EAX,EAX
	$+C0     008050DD      5E                   POP ESI
	$+C1     008050DE      8BE5                 MOV ESP,EBP
	$+C3     008050E0      5D                   POP EBP
	$+C4     008050E1      C2 1000              RETN 0x10
	$+C7     008050E4      53                   PUSH EBX
	$+C8     008050E5      8B5D 14              MOV EBX,DWORD PTR SS:[EBP+0x14]
	$+CB     008050E8      33D2                 XOR EDX,EDX
	$+CD     008050EA      8913                 MOV DWORD PTR DS:[EBX],EDX
	$+CF     008050EC      8B00                 MOV EAX,DWORD PTR DS:[EAX]
	$+D1     008050EE      8B09                 MOV ECX,DWORD PTR DS:[ECX]
	$+D3     008050F0      8B0C81               MOV ECX,DWORD PTR DS:[ECX+EAX*4]
	$+D6     008050F3      85C9                 TEST ECX,ECX
	$+D8     008050F5      74 33                JE SHORT MapleSto.0080512A
	$+DA     008050F7      8B3F                 MOV EDI,DWORD PTR DS:[EDI]
	$+DC     008050F9      8DA424 00000000      LEA ESP,DWORD PTR SS:[ESP]
	$+E3     00805100      3979 10              CMP DWORD PTR DS:[ECX+0x10],EDI
	$+E6     00805103      75 1C                JNZ SHORT MapleSto.00805121
	$+E8     00805105      F3:0F1001            MOVSS XMM0,DWORD PTR DS:[ECX]
	$+EC     00805109      0F2E06               UCOMISS XMM0,DWORD PTR DS:[ESI]
	$+EF     0080510C      9F                   LAHF
	$+F0     0080510D      F6C4 44              TEST AH,0x44
	$+F3     00805110      7A 0F                JPE SHORT MapleSto.00805121
	$+F5     00805112      F3:0F1041 04         MOVSS XMM0,DWORD PTR DS:[ECX+0x4]
	$+FA     00805117      0F2E46 04            UCOMISS XMM0,DWORD PTR DS:[ESI+0x4]
	$+FE     0080511B      9F                   LAHF
	$+FF     0080511C      F6C4 44              TEST AH,0x44
	$+102    0080511F      7B 14                JPO SHORT MapleSto.00805135
	$+104    00805121      8BD1                 MOV EDX,ECX
	$+106    00805123      8B49 0C              MOV ECX,DWORD PTR DS:[ECX+0xC]
	$+109    00805126      85C9                 TEST ECX,ECX
	$+10B    00805128    ^ 75 D6                JNZ SHORT MapleSto.00805100
	$+10D    0080512A      5B                   POP EBX
	$+10E    0080512B      5F                   POP EDI
	$+10F    0080512C      33C0                 XOR EAX,EAX
	$+111    0080512E      5E                   POP ESI
	$+112    0080512F      8BE5                 MOV ESP,EBP
	$+114    00805131      5D                   POP EBP
	$+115    00805132      C2 1000              RETN 0x10
	$+118    00805135      8913                 MOV DWORD PTR DS:[EBX],EDX
	$+11A    00805137      5B                   POP EBX
	$+11B    00805138      5F                   POP EDI
	$+11C    00805139      8BC1                 MOV EAX,ECX
	$+11E    0080513B      5E                   POP ESI
	$+11F    0080513C      8BE5                 MOV ESP,EBP
	$+121    0080513E      5D                   POP EBP
	$+122    0080513F      C2 1000              RETN 0x10
	$+125    00805142      CC                   INT3
	$+126    00805143      CC                   INT3
	$+127    00805144      CC                   INT3
	$+128    00805145      CC                   INT3
	$+129    00805146      CC                   INT3
	$+12A    00805147      CC                   INT3
	$+12B    00805148      CC                   INT3
	$+12C    00805149      CC                   INT3
	$+12D    0080514A      CC                   INT3
	$+12E    0080514B      CC                   INT3
	$+12F    0080514C      CC                   INT3
	$+130    0080514D      CC                   INT3
	$+131    0080514E      CC                   INT3
	$+132    0080514F      CC                   INT3
	$+133    00805150      55                   PUSH EBP
	$+134    00805151      8BEC                 MOV EBP,ESP
	$+136    00805153      56                   PUSH ESI
	$+137    00805154      8BF1                 MOV ESI,ECX
	$+139    00805156      837E 2C 00           CMP DWORD PTR DS:[ESI+0x2C],0x0
	$+13D    0080515A      57                   PUSH EDI
	$+13E    0080515B      75 58                JNZ SHORT MapleSto.008051B5
	$+140    0080515D      8B46 24              MOV EAX,DWORD PTR DS:[ESI+0x24]
	$+143    00805160      B9 14000000          MOV ECX,0x14
	$+148    00805165      F7E1                 MUL ECX
	$+14A    00805167      85D2                 TEST EDX,EDX
	$+14C    00805169      77 62                JA SHORT MapleSto.008051CD
	$+14E    0080516B      72 05                JB SHORT MapleSto.00805172
	$+150    0080516D      83F8 FF              CMP EAX,-0x1
	$+153    00805170      77 5B                JA SHORT MapleSto.008051CD
	$+155    00805172      83CA FF              OR EDX,0xFFFFFFFF
	$+158    00805175      2BD0                 SUB EDX,EAX
	$+15A    00805177      83FA 04              CMP EDX,0x4
	$+15D    0080517A      72 51                JB SHORT MapleSto.008051CD
	$+15F    0080517C      83C0 04              ADD EAX,0x4
	$+162    0080517F      50                   PUSH EAX
	$+163    00805180      FF15 F4983E01        CALL DWORD PTR DS:[0x13E98F4]                    ; msvcr90.malloc
	$+169    00805186      83C4 04              ADD ESP,0x4
	$+16C    00805189      85C0                 TEST EAX,EAX
	$+16E    0080518B      74 40                JE SHORT MapleSto.008051CD
	$+170    0080518D      8B4E 28              MOV ECX,DWORD PTR DS:[ESI+0x28]
	$+173    00805190      8908                 MOV DWORD PTR DS:[EAX],ECX
	$+175    00805192      8946 28              MOV DWORD PTR DS:[ESI+0x28],EAX
	$+178    00805195      8B4E 24              MOV ECX,DWORD PTR DS:[ESI+0x24]
	$+17B    00805198      8D1489               LEA EDX,DWORD PTR DS:[ECX+ECX*4]
	$+17E    0080519B      83C1 FF              ADD ECX,-0x1
	$+181    0080519E      8D4490 F0            LEA EAX,DWORD PTR DS:[EAX+EDX*4-0x10]
	$+185    008051A2      78 11                JS SHORT MapleSto.008051B5
	$+187    008051A4      8B56 2C              MOV EDX,DWORD PTR DS:[ESI+0x2C]
	$+18A    008051A7      8950 0C              MOV DWORD PTR DS:[EAX+0xC],EDX
	$+18D    008051AA      8946 2C              MOV DWORD PTR DS:[ESI+0x2C],EAX
	$+190    008051AD      83E8 14              SUB EAX,0x14
	$+193    008051B0      83E9 01              SUB ECX,0x1
	$+196    008051B3    ^ 79 EF                JNS SHORT MapleSto.008051A4
	$+198    008051B5      8B7E 2C              MOV EDI,DWORD PTR DS:[ESI+0x2C]
	$+19B    008051B8      33C0                 XOR EAX,EAX
	$+19D    008051BA      85FF                 TEST EDI,EDI
	$+19F    008051BC      0F95C0               SETNE AL
	$+1A2    008051BF      85C0                 TEST EAX,EAX
	$+1A4    008051C1      75 14                JNZ SHORT MapleSto.008051D7
	$+1A6    008051C3      68 05400080          PUSH 0x80004005
	$+1AB    008051C8      E8 53C9BFFF          CALL MapleSto.00401B20
	$+1B0    008051CD      68 0E000780          PUSH 0x8007000E
	$+1B5    008051D2      E8 49C9BFFF          CALL MapleSto.00401B20
	$+1BA    008051D7      8B47 0C              MOV EAX,DWORD PTR DS:[EDI+0xC]
	$+1BD    008051DA      8946 2C              MOV DWORD PTR DS:[ESI+0x2C],EAX
	$+1C0    008051DD      8B45 08              MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+1C3    008051E0      8B08                 MOV ECX,DWORD PTR DS:[EAX]
	$+1C5    008051E2      890F                 MOV DWORD PTR DS:[EDI],ECX
	$+1C7    008051E4      8B50 04              MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+1CA    008051E7      8B45 10              MOV EAX,DWORD PTR SS:[EBP+0x10]
	$+1CD    008051EA      8947 10              MOV DWORD PTR DS:[EDI+0x10],EAX
	$+1D0    008051ED      8B45 0C              MOV EAX,DWORD PTR SS:[EBP+0xC]
	$+1D3    008051F0      8957 04              MOV DWORD PTR DS:[EDI+0x4],EDX
	$+1D6    008051F3      FF46 04              INC DWORD PTR DS:[ESI+0x4]
	$+1D9    008051F6      8B0E                 MOV ECX,DWORD PTR DS:[ESI]
	$+1DB    008051F8      8B1481               MOV EDX,DWORD PTR DS:[ECX+EAX*4]
	$+1DE    008051FB      8957 0C              MOV DWORD PTR DS:[EDI+0xC],EDX
	$+1E1    008051FE      8B0E                 MOV ECX,DWORD PTR DS:[ESI]
	$+1E3    00805200      893C81               MOV DWORD PTR DS:[ECX+EAX*4],EDI
	$+1E6    00805203      8B46 04              MOV EAX,DWORD PTR DS:[ESI+0x4]
	$+1E9    00805206      3B46 18              CMP EAX,DWORD PTR DS:[ESI+0x18]
	$+1EC    00805209      76 16                JBE SHORT MapleSto.00805221
	$+1EE    0080520B      837E 20 00           CMP DWORD PTR DS:[ESI+0x20],0x0
	$+1F2    0080520F      75 10                JNZ SHORT MapleSto.00805221
	$+1F4    00805211      50                   PUSH EAX
	$+1F5    00805212      8BCE                 MOV ECX,ESI
	$+1F7    00805214      E8 D75E3200          CALL MapleSto.00B2B0F0
	$+1FC    00805219      50                   PUSH EAX
	$+1FD    0080521A      8BCE                 MOV ECX,ESI
	$+1FF    0080521C      E8 6FCDD2FF          CALL MapleSto.00531F90
	$+204    00805221      8BC7                 MOV EAX,EDI
	$+206    00805223      5F                   POP EDI
	$+207    00805224      5E                   POP ESI
	$+208    00805225      5D                   POP EBP
	$+209    00805226      C2 0C00              RETN 0xC
	$+20C    00805229      CC                   INT3
	*/
	addr_tmp = scan_exe_.ScanAddr("8D45CC508BCEC745FC05000000", 0);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	addr_tmp1 = Cmn_FeatureCodeScan::CallAddr2AbsoluteAddr((pt_pcbyte)addr_tmp - 5);
	if (!addr_tmp1)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_c_construct_out_packet_ = " << addr_tmp1 << ";" << FC_OFstream_EndlLog;

	addr_tmp1 = Cmn_FeatureCodeScan::CallAddr2AbsoluteAddr((pt_pcbyte)addr_tmp + 0x15);
	if (!addr_tmp1)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_c_out_packet_send_ = " << addr_tmp1 << ";" << FC_OFstream_EndlLog;

	dw_tmp = AsDword(addr_tmp, 0x29 + 2);
	CheckExeAddr(dw_tmp);
	ofs << "pt_cdword	g_c_msvcr90_free_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;
	return true;
}

bool CAutoUpdateApp::Scan_Role(Cmn_FC_OFstream& ofs)
{
	pt_pcvoid addr_tmp, addr_tmp1;
	pt_dword dw_tmp;
	boost::optional<pt_dword> op_dw_tmp;
	/*
	$-89     0098226F     CC                    INT3
	$-88     00982270     55                    PUSH EBP
	$-87     00982271     8BEC                  MOV EBP,ESP
	$-85     00982273     8B45 08               MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-82     00982276     8901                  MOV DWORD PTR DS:[ECX],EAX
	$-80     00982278     5D                    POP EBP
	$-7F     00982279     C2 0400               RETN 0x4
	$-7C     0098227C     CC                    INT3
	$-7B     0098227D     CC                    INT3
	$-7A     0098227E     CC                    INT3
	$-79     0098227F     CC                    INT3
	$-78     00982280   - E9 B66B4A01           JMP MapleSto.01E28E3B
	$-73     00982285     8D6424 04             LEA ESP,DWORD PTR SS:[ESP+0x4]
	$-6F     00982289   - E9 BB304A01           JMP MapleSto.01E25349
	$-6A     0098228E   ^ 79 CC                 JNS SHORT MapleSto.0098225C
	$-68     00982290     56                    PUSH ESI
	$-67     00982291     8BF1                  MOV ESI,ECX
	$-65     00982293     8D4E 04               LEA ECX,DWORD PTR DS:[ESI+0x4]
	$-62     00982296     E8 1583BEFF           CALL MapleSto.0056A5B0
	$-5D     0098229B     C706 00000000         MOV DWORD PTR DS:[ESI],0x0
	$-57     009822A1     C746 38 00000000      MOV DWORD PTR DS:[ESI+0x38],0x0
	$-50     009822A8     F3:0F1005 34267401    MOVSS XMM0,DWORD PTR DS:[0x1742634]
	$-48     009822B0     F3:0F1146 28          MOVSS DWORD PTR DS:[ESI+0x28],XMM0
	$-43     009822B5     F3:0F1005 38267401    MOVSS XMM0,DWORD PTR DS:[0x1742638]
	$-3B     009822BD     F3:0F1146 2C          MOVSS DWORD PTR DS:[ESI+0x2C],XMM0
	$-36     009822C2     F3:0F1005 3C267401    MOVSS XMM0,DWORD PTR DS:[0x174263C]
	$-2E     009822CA     F3:0F1146 30          MOVSS DWORD PTR DS:[ESI+0x30],XMM0
	$-29     009822CF     8BC6                  MOV EAX,ESI
	$-27     009822D1     5E                    POP ESI
	$-26     009822D2     C3                    RETN
	$-25     009822D3     CC                    INT3
	$-24     009822D4     CC                    INT3
	$-23     009822D5     CC                    INT3
	$-22     009822D6     CC                    INT3
	$-21     009822D7     CC                    INT3
	$-20     009822D8     CC                    INT3
	$-1F     009822D9     CC                    INT3
	$-1E     009822DA     CC                    INT3
	$-1D     009822DB     CC                    INT3
	$-1C     009822DC     CC                    INT3
	$-1B     009822DD     CC                    INT3
	$-1A     009822DE     CC                    INT3
	$-19     009822DF     CC                    INT3
	$-18     009822E0     56                    PUSH ESI
	$-17     009822E1     8BF1                  MOV ESI,ECX
	$-15     009822E3     833E 00               CMP DWORD PTR DS:[ESI],0x0
	$-12     009822E6     75 04                 JNZ SHORT MapleSto.009822EC
	$-10     009822E8     32C0                  XOR AL,AL
	$-E      009822EA     5E                    POP ESI
	$-D      009822EB     C3                    RETN
	$-C      009822EC     8B0D 2C417201         MOV ECX,DWORD PTR DS:[0x172412C]         ; Base_Role
	$-6      009822F2     57                    PUSH EDI
	$-5      009822F3     E8 DB165201           CALL MapleSto.01EA39D3                   ; FnGetMyPlayer,then,eax=stCD_RoleMe*
	$ ==>    009822F8     8B0E                  MOV ECX,DWORD PTR DS:[ESI]
	$+2      009822FA     8BF8                  MOV EDI,EAX
	$+4      009822FC     8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$+6      009822FE     8B50 1C               MOV EDX,DWORD PTR DS:[EAX+0x1C]
	$+9      00982301     FFD2                  CALL EDX
	$+B      00982303     3D 45002006           CMP EAX,0x6200045
	$+10     00982308     74 1F                 JE SHORT MapleSto.00982329
	$+12     0098230A     8B0E                  MOV ECX,DWORD PTR DS:[ESI]
	$+14     0098230C     3BF9                  CMP EDI,ECX
	$+16     0098230E     74 19                 JE SHORT MapleSto.00982329
	$+18     00982310     E8 C3CB4901           CALL MapleSto.01E1EED8
	$+1D     00982315     85C0                  TEST EAX,EAX
	$+1F     00982317     75 10                 JNZ SHORT MapleSto.00982329
	$+21     00982319     8B0E                  MOV ECX,DWORD PTR DS:[ESI]
	$+23     0098231B     8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$+25     0098231D     8B50 1C               MOV EDX,DWORD PTR DS:[EAX+0x1C]
	$+28     00982320     FFD2                  CALL EDX
	$+2A     00982322     3D 64002006           CMP EAX,0x6200064
	$+2F     00982327     75 53                 JNZ SHORT MapleSto.0098237C
	$+31     00982329     8BCF                  MOV ECX,EDI
	$+33     0098232B     E8 44614901           CALL MapleSto.01E18474
	$+38     00982330     85C0                  TEST EAX,EAX
	$+3A     00982332     75 48                 JNZ SHORT MapleSto.0098237C
	$+3C     00982334     8B0D 2C407201         MOV ECX,DWORD PTR DS:[0x172402C]
	$+42     0098233A     6A 07                 PUSH 0x7
	$+44     0098233C     E8 AF11E2FF           CALL MapleSto.007A34F0
	$+49     00982341     84C0                  TEST AL,AL
	$+4B     00982343     75 37                 JNZ SHORT MapleSto.0098237C
	$+4D     00982345     8B0E                  MOV ECX,DWORD PTR DS:[ESI]
	$+4F     00982347     8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$+51     00982349     8B90 E0000000         MOV EDX,DWORD PTR DS:[EAX+0xE0]
	$+57     0098234F     FFD2                  CALL EDX
	$+59     00982351     85C0                  TEST EAX,EAX
	$+5B     00982353     74 27                 JE SHORT MapleSto.0098237C
	$+5D     00982355     837E 38 00            CMP DWORD PTR DS:[ESI+0x38],0x0
	$+61     00982359     75 12                 JNZ SHORT MapleSto.0098236D
	$+63     0098235B     8B0E                  MOV ECX,DWORD PTR DS:[ESI]
	$+65     0098235D     E8 26044A01           CALL MapleSto.01E22788
	$+6A     00982362     8BC8                  MOV ECX,EAX
	$+6C     00982364     E8 1DCF3101           CALL MapleSto.01C9F286
	$+71     00982369     85C0                  TEST EAX,EAX
	$+73     0098236B     75 0F                 JNZ SHORT MapleSto.0098237C
	$+75     0098236D     3B3E                  CMP EDI,DWORD PTR DS:[ESI]
	$+77     0098236F     75 10                 JNZ SHORT MapleSto.00982381
	$+79     00982371     8BCF                  MOV ECX,EDI
	$+7B     00982373     E8 7A124801           CALL MapleSto.01E035F2
	$+80     00982378     85C0                  TEST EAX,EAX
	$+82     0098237A     74 05                 JE SHORT MapleSto.00982381
	$+84     0098237C     5F                    POP EDI
	$+85     0098237D     32C0                  XOR AL,AL
	$+87     0098237F     5E                    POP ESI
	$+88     00982380     C3                    RETN
	$+89     00982381     8B0D 744E7201         MOV ECX,DWORD PTR DS:[0x1724E74]
	$+8F     00982387     E8 143EC7FF           CALL MapleSto.005F61A0
	$+94     0098238C     84C0                  TEST AL,AL
	$+96     0098238E     5F                    POP EDI
	$+97     0098238F     0F95C0                SETNE AL
	$+9A     00982392     5E                    POP ESI
	$+9B     00982393     C3                    RETN
	$+9C     00982394     CC                    INT3
	$+9D     00982395     CC                    INT3
	$+9E     00982396     CC                    INT3
	$+9F     00982397     CC                    INT3
	$+A0     00982398     CC                    INT3
	$+A1     00982399     CC                    INT3
	$+A2     0098239A     CC                    INT3
	$+A3     0098239B     CC                    INT3
	$+A4     0098239C     CC                    INT3
	$+A5     0098239D     CC                    INT3
	$+A6     0098239E     CC                    INT3
	$+A7     0098239F     CC                    INT3
	$+A8     009823A0     55                    PUSH EBP
	$+A9     009823A1     8BEC                  MOV EBP,ESP
	$+AB     009823A3     833D F89E7301 00      CMP DWORD PTR DS:[0x1739EF8],0x0
	$+B2     009823AA     75 06                 JNZ SHORT MapleSto.009823B2
	$+B4     009823AC     32C0                  XOR AL,AL
	$+B6     009823AE     5D                    POP EBP
	$+B7     009823AF     C2 0400               RETN 0x4
	$+BA     009823B2     E8 29FFFFFF           CALL MapleSto.009822E0
	$+BF     009823B7     84C0                  TEST AL,AL
	$+C1     009823B9     75 06                 JNZ SHORT MapleSto.009823C1
	$+C3     009823BB     B0 01                 MOV AL,0x1
	$+C5     009823BD     5D                    POP EBP
	$+C6     009823BE     C2 0400               RETN 0x4
	$+C9     009823C1     8B0D 3C417201         MOV ECX,DWORD PTR DS:[0x172413C]
	$+CF     009823C7     56                    PUSH ESI
	$+D0     009823C8     8B75 08               MOV ESI,DWORD PTR SS:[EBP+0x8]
	$+D3     009823CB     56                    PUSH ESI
	$+D4     009823CC     E8 9FF9F0FF           CALL MapleSto.00891D70
	$+D9     009823D1     85C0                  TEST EAX,EAX
	$+DB     009823D3     75 17                 JNZ SHORT MapleSto.009823EC
	$+DD     009823D5     81FE E1930400         CMP ESI,0x493E1
	$+E3     009823DB     75 16                 JNZ SHORT MapleSto.009823F3
	$+E5     009823DD     8B0D 2C417201         MOV ECX,DWORD PTR DS:[0x172412C]
	$+EB     009823E3     E8 74185201           CALL MapleSto.01EA3C5C
	$+F0     009823E8     85C0                  TEST EAX,EAX
	$+F2     009823EA     74 07                 JE SHORT MapleSto.009823F3
	$+F4     009823EC     32C0                  XOR AL,AL
	$+F6     009823EE     5E                    POP ESI
	$+F7     009823EF     5D                    POP EBP
	$+F8     009823F0     C2 0400               RETN 0x4
	$+FB     009823F3     8B0D 74517201         MOV ECX,DWORD PTR DS:[0x1725174]
	$+101    009823F9     56                    PUSH ESI
	$+102    009823FA     E8 A195F4FF           CALL MapleSto.008CB9A0
	$+107    009823FF     85C0                  TEST EAX,EAX
	$+109    00982401     0F95C0                SETNE AL
	$+10C    00982404     5E                    POP ESI
	$+10D    00982405     5D                    POP EBP
	$+10E    00982406     C2 0400               RETN 0x4
	$+111    00982409     CC                    INT3
	$+112    0098240A     CC                    INT3
	$+113    0098240B     CC                    INT3
	$+114    0098240C     CC                    INT3
	$+115    0098240D     CC                    INT3
	$+116    0098240E     CC                    INT3
	$+117    0098240F     CC                    INT3
	$+118    00982410     55                    PUSH EBP
	$+119    00982411     8BEC                  MOV EBP,ESP
	$+11B    00982413     8B0D 74517201         MOV ECX,DWORD PTR DS:[0x1725174]
	$+121    00982419     5D                    POP EBP
	$+122    0098241A   ^ E9 C19AF4FF           JMP MapleSto.008CBEE0
	$+127    0098241F     CC                    INT3
	$+128    00982420     55                    PUSH EBP
	$+129    00982421     8BEC                  MOV EBP,ESP
	$+12B    00982423     56                    PUSH ESI
	$+12C    00982424     E8 B7FEFFFF           CALL MapleSto.009822E0
	$+131    00982429     8B75 08               MOV ESI,DWORD PTR SS:[EBP+0x8]
	$+134    0098242C     84C0                  TEST AL,AL
	$+136    0098242E     75 17                 JNZ SHORT MapleSto.00982447
	$+138    00982430     81FE E8930400         CMP ESI,0x493E8
	$+13E    00982436     74 0F                 JE SHORT MapleSto.00982447
	$+140    00982438     81FE E9930400         CMP ESI,0x493E9
	$+146    0098243E     74 07                 JE SHORT MapleSto.00982447
	$+148    00982440     32C0                  XOR AL,AL
	$+14A    00982442     5E                    POP ESI
	$+14B    00982443     5D                    POP EBP
	$+14C    00982444     C2 0400               RETN 0x4
	$+14F    00982447     8B0D 3C417201         MOV ECX,DWORD PTR DS:[0x172413C]
	$+155    0098244D     56                    PUSH ESI
	$+156    0098244E     E8 1DF9F0FF           CALL MapleSto.00891D70
	$+15B    00982453     85C0                  TEST EAX,EAX
	$+15D    00982455   ^ 75 E9                 JNZ SHORT MapleSto.00982440
	$+15F    00982457     81FE E1930400         CMP ESI,0x493E1
	$+165    0098245D     75 0F                 JNZ SHORT MapleSto.0098246E
	$+167    0098245F     8B0D 2C417201         MOV ECX,DWORD PTR DS:[0x172412C]
	$+16D    00982465     E8 F2175201           CALL MapleSto.01EA3C5C
	$+172    0098246A     85C0                  TEST EAX,EAX
	$+174    0098246C   ^ 75 D2                 JNZ SHORT MapleSto.00982440
	$+176    0098246E     B0 01                 MOV AL,0x1
	$+178    00982470     5E                    POP ESI
	$+179    00982471     5D                    POP EBP
	$+17A    00982472     C2 0400               RETN 0x4
	$+17D    00982475     CC                    INT3
	*/
	addr_tmp = scan_exe_.ScanAddr("8B0E8BF88B018B501CFFD23D", 0);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = AsDword(addr_tmp, -0xc + 2);
	CheckExeAddr(dw_tmp);
	ofs << "pt_cdword	g_a_map_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;
	addr_tmp1 = Cmn_FeatureCodeScan::CallAddr2AbsoluteAddr((pt_pcbyte)addr_tmp - 5);
	if (!addr_tmp1)
	{
		assert(false);
		return false;
	}
	dw_tmp = AsDword(addr_tmp1, 2);
	assert(dw_tmp > 0x200 && dw_tmp < 0x2a0);
	ofs << "pt_cdword	g_o_get_role_me_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	/*
	$-79     009782BF     CC                    INT3
	$-78     009782C0     55                    PUSH EBP
	$-77     009782C1     8BEC                  MOV EBP,ESP
	$-75     009782C3     6A FF                 PUSH -0x1
	$-73     009782C5     68 61923601           PUSH MapleSto.01369261
	$-6E     009782CA     64:A1 00000000        MOV EAX,DWORD PTR FS:[0]
	$-68     009782D0     50                    PUSH EAX
	$-67     009782D1     56                    PUSH ESI
	$-66     009782D2     A1 D0687101           MOV EAX,DWORD PTR DS:[0x17168D0]
	$-61     009782D7     33C5                  XOR EAX,EBP
	$-5F     009782D9     50                    PUSH EAX
	$-5E     009782DA     8D45 F4               LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-5B     009782DD     64:A3 00000000        MOV DWORD PTR FS:[0],EAX
	$-55     009782E3     8BF1                  MOV ESI,ECX
	$-53     009782E5     8D45 08               LEA EAX,DWORD PTR SS:[EBP+0x8]
	$-50     009782E8     50                    PUSH EAX
	$-4F     009782E9     8D8E 500F0000         LEA ECX,DWORD PTR DS:[ESI+0xF50]
	$-49     009782EF     C745 FC 00000000      MOV DWORD PTR SS:[EBP-0x4],0x0
	$-42     009782F6     FF15 CCA13E01         CALL DWORD PTR DS:[0x13EA1CC]            ; mfc90.#810
	$-3C     009782FC     8B4E 50               MOV ECX,DWORD PTR DS:[ESI+0x50]
	$-39     009782FF     51                    PUSH ECX
	$-38     00978300     E8 BBFC1700           CALL MapleSto.00AF7FC0
	$-33     00978305     83C4 04               ADD ESP,0x4
	$-30     00978308     8D4D 08               LEA ECX,DWORD PTR SS:[EBP+0x8]
	$-2D     0097830B     FF15 DCA13E01         CALL DWORD PTR DS:[0x13EA1DC]            ; mfc90.#601
	$-27     00978311     8B4D F4               MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-24     00978314     64:890D 00000000      MOV DWORD PTR FS:[0],ECX
	$-1D     0097831B     59                    POP ECX
	$-1C     0097831C     5E                    POP ESI
	$-1B     0097831D     8BE5                  MOV ESP,EBP
	$-19     0097831F     5D                    POP EBP
	$-18     00978320     C2 0400               RETN 0x4
	$-15     00978323     CC                    INT3
	$-14     00978324     CC                    INT3
	$-13     00978325     CC                    INT3
	$-12     00978326     CC                    INT3
	$-11     00978327     CC                    INT3
	$-10     00978328     CC                    INT3
	$-F      00978329     CC                    INT3
	$-E      0097832A     CC                    INT3
	$-D      0097832B     CC                    INT3
	$-C      0097832C     CC                    INT3
	$-B      0097832D     CC                    INT3
	$-A      0097832E     CC                    INT3
	$-9      0097832F     CC                    INT3
	$-8      00978330     55                    PUSH EBP
	$-7      00978331     8BEC                  MOV EBP,ESP
	$-5      00978333     51                    PUSH ECX
	$-4      00978334     56                    PUSH ESI
	$-3      00978335     8B75 08               MOV ESI,DWORD PTR SS:[EBP+0x8]
	$ ==>    00978338     81C1 500F0000         ADD ECX,0xF50                            ; g_o_role_name_
	$+6      0097833E     51                    PUSH ECX
	$+7      0097833F     8BCE                  MOV ECX,ESI
	$+9      00978341     C745 FC 00000000      MOV DWORD PTR SS:[EBP-0x4],0x0
	$+10     00978348     FF15 C8A13E01         CALL DWORD PTR DS:[0x13EA1C8]            ; mfc90.#280
	$+16     0097834E     8BC6                  MOV EAX,ESI
	$+18     00978350     5E                    POP ESI
	$+19     00978351     8BE5                  MOV ESP,EBP
	$+1B     00978353     5D                    POP EBP
	$+1C     00978354     C2 0400               RETN 0x4
	$+1F     00978357     CC                    INT3
	$+20     00978358     CC                    INT3
	$+21     00978359     CC                    INT3
	$+22     0097835A     CC                    INT3
	$+23     0097835B     CC                    INT3
	$+24     0097835C     CC                    INT3
	$+25     0097835D     CC                    INT3
	$+26     0097835E     CC                    INT3
	$+27     0097835F     CC                    INT3
	$+28     00978360   - E9 08134B01           JMP MapleSto.01E2966D
	$+2D     00978365     F3:0F1058 E4          MOVSS XMM3,DWORD PTR DS:[EAX-0x1C]
	$+32     0097836A     F3:0F1048 E0          MOVSS XMM1,DWORD PTR DS:[EAX-0x20]
	$+37     0097836F     F3:0F1050 E8          MOVSS XMM2,DWORD PTR DS:[EAX-0x18]
	$+3C     00978374     0F28FB                MOVAPS XMM7,XMM3
	$+3F     00978377     F3:0F59FB             MULSS XMM7,XMM3
	$+43     0097837B     0F28F1                MOVAPS XMM6,XMM1
	$+46     0097837E     F3:0F59F1             MULSS XMM6,XMM1
	$+4A     00978382     F3:0F58F7             ADDSS XMM6,XMM7
	$+4E     00978386     0F28DA                MOVAPS XMM3,XMM2
	$+51     00978389     F3:0F59DA             MULSS XMM3,XMM2
	$+55     0097838D     F3:0F58F3             ADDSS XMM6,XMM3
	$+59     00978391     51                    PUSH ECX
	$+5A     00978392     E8 01000000           CALL MapleSto.00978398
	$+5F     00978397     70 59                 JO SHORT MapleSto.009783F2
	$+61     00978399     8D89 1A9C4A01         LEA ECX,DWORD PTR DS:[ECX+0x14A9C1A]
	$+67     0097839F     FFE1                  JMP ECX
	$+69     009783A1   ^ 7F 9D                 JG SHORT MapleSto.00978340
	$+6B     009783A3     50                    PUSH EAX
	$+6C     009783A4     C7C0 569C8440         MOV EAX,0x40849C56
	$+72     009783AA     8D80 B3302649         LEA EAX,DWORD PTR DS:[EAX+0x492630B3]
	$+78     009783B0     870424                XCHG DWORD PTR SS:[ESP],EAX
	$+7B     009783B3     870424                XCHG DWORD PTR SS:[ESP],EAX
	$+7E     009783B6     8B0424                MOV EAX,DWORD PTR SS:[ESP]
	$+81     009783B9     81EC 08000000         SUB ESP,0x8
	$+87     009783BF     52                    PUSH EDX
	$+88     009783C0     891C24                MOV DWORD PTR SS:[ESP],EBX
	$+8B     009783C3     C7C0 35D9CA35         MOV EAX,0x35CAD935
	$+91     009783C9     8D80 1BBF282F         LEA EAX,DWORD PTR DS:[EAX+0x2F28BF1B]
	$+97     009783CF     50                    PUSH EAX
	$+98     009783D0     57                    PUSH EDI
	$+99     009783D1     8DA424 FAFFFFFF       LEA ESP,DWORD PTR SS:[ESP-0x6]
	$+A0     009783D8     89B424 0A000000       MOV DWORD PTR SS:[ESP+0xA],ESI
	$+A7     009783DF     9C                    PUSHFD
	$+A8     009783E0     E8 01000000           CALL MapleSto.009783E6
	$+AD     009783E5   ^ E1 81                 LOOPDE SHORT MapleSto.00978368
	$+AF     009783E7     04 24                 ADD AL,0x24
	$+B1     009783E9   ^ 71 96                 JNO SHORT MapleSto.00978381
	$+B3     009783EB     4A                    DEC EDX
	$+B4     009783EC     01C3                  ADD EBX,EAX
	$+B6     009783EE     7B 33                 JPO SHORT MapleSto.00978423
	$+B8     009783F0     C039 81               SAR BYTE PTR DS:[ECX],0x81
	$+BB     009783F3     F8                    CLC
	$+BC     009783F4     1000                  ADC BYTE PTR DS:[EAX],AL
	$+BE     009783F6     000F                  ADD BYTE PTR DS:[EDI],CL
	$+C0     009783F8     95                    XCHG EAX,EBP
	$+C1     009783F9     C056 8B B4            RCL BYTE PTR DS:[ESI-0x75],0xB4
	$+C5     009783FD     24 04                 AND AL,0x4
	$+C7     009783FF     0000                  ADD BYTE PTR DS:[EAX],AL
	$+C9     00978401     0056 8D               ADD BYTE PTR DS:[ESI-0x73],DL
	$+CC     00978404     A4                    MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
	$+CD     00978405     24 FC                 AND AL,0xFC
	$+CF     00978407     FF                    DB FF
	$+D0     00978408     FF                    DB FF
	$+D1     00978409     FF8D A424FCFF         DEC DWORD PTR SS:[EBP+0xFFFC24A4]
	$+D7     0097840F     FF                    DB FF
	$+D8     00978410     FF53 68               CALL DWORD PTR DS:[EBX+0x68]
	$+DB     00978413     FE                    DB FE
	$+DC     00978414     E4 11                 IN AL,0x11
	$+DE     00978416     44                    INC ESP
	$+DF     00978417     56                    PUSH ESI
	$+E0     00978418     8BB424 18000000       MOV ESI,DWORD PTR SS:[ESP+0x18]
	$+E7     0097841F     51                    PUSH ECX
	$+E8     00978420     E8 01000000           CALL MapleSto.00978426
	$+ED     00978425     E2 59                 LOOPD SHORT MapleSto.00978480
	$+EF     00978427     8D89 46F94A01         LEA ECX,DWORD PTR DS:[ECX+0x14AF946]
	$+F5     0097842D     FFD1                  CALL ECX
	$+F7     0097842F   ^ 75 E9                 JNZ SHORT MapleSto.0097841A
	$+F9     00978431     FA                    CLI
	$+FA     00978432     C44A 01               LES ECX,FWORD PTR DS:[EDX+0x1]
	$+FD     00978435     8B07                  MOV EAX,DWORD PTR DS:[EDI]
	$+FF     00978437   - E9 43704A01           JMP MapleSto.01E1F47F
	$+104    0097843C     EA CCCCCCC2 0400      JMP FAR 0004:0C2CCCCCC
	$+10B    00978443     CC                    INT3
	$+10C    00978444     CC                    INT3
	$+10D    00978445     CC                    INT3
	*/
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("81??????????518b??c7????????????ff??????????8b??5e8b??5dc2????cccccccccccccccccce9????????f3????????f3????????f3????????0f????f3??????0f", 2);
	if (!op_dw_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *op_dw_tmp;
	assert(dw_tmp > 0xf00 && dw_tmp < 0xfff);
	ofs << "pt_cdword	g_o_role_name_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	//vmp代码
	/*
	$-152    01E035C0       04 24                ADD AL,0x24
	$-150    01E035C2       07                   POP ES
	$-14F    01E035C3       58                   POP EAX
	$-14E    01E035C4       0000                 ADD BYTE PTR DS:[EAX],AL
	$-14C    01E035C6       C3                   RETN
	$-14B    01E035C7       7B 8D                JPO SHORT MapleSto.01E03556
	$-149    01E035C9       A4                   MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
	$-148    01E035CA       24 FC                AND AL,0xFC
	$-146    01E035CC       FF                   DB FF
	$-145    01E035CD       FF                   DB FF
	$-144    01E035CE       FF56 53              CALL DWORD PTR DS:[ESI+0x53]
	$-141    01E035D1       8DA424 FCFFFFFF      LEA ESP,DWORD PTR SS:[ESP-0x4]
	$-13A    01E035D8       53                   PUSH EBX
	$-139    01E035D9       8B9C24 24000000      MOV EBX,DWORD PTR SS:[ESP+0x24]
	$-132    01E035E0       C2 2800              RETN 0x28
	$-12F    01E035E3     ^ 74 9D                JE SHORT MapleSto.01E03582
	$-12D    01E035E5       58                   POP EAX
	$-12C    01E035E6       0F85 A8C20000        JNZ MapleSto.01E0F894
	$-126    01E035EC       E9 A0780000          JMP MapleSto.01E0AE91
	$-121    01E035F1       72 33                JB SHORT MapleSto.01E03626
	$-11F    01E035F3       C083 B9A81700 00     ROL BYTE PTR DS:[EBX+0x17A8B9],0x0
	$-118    01E035FA       010F                 ADD DWORD PTR DS:[EDI],ECX
	$-116    01E035FC       94                   XCHG EAX,ESP
	$-115    01E035FD       C056 8B B4           RCL BYTE PTR DS:[ESI-0x75],0xB4
	$-111    01E03601       24 04                AND AL,0x4
	$-10F    01E03603       0000                 ADD BYTE PTR DS:[EAX],AL
	$-10D    01E03605       0050 9C              ADD BYTE PTR DS:[EAX-0x64],DL
	$-10A    01E03608       68 81D2E32F          PUSH 0x2FE3D281
	$-105    01E0360D       53                   PUSH EBX
	$-104    01E0360E       56                   PUSH ESI
	$-103    01E0360F       E8 BFDB0000          CALL MapleSto.01E111D3
	$-FE     01E03614     - E9 5B8DA424          JMP 2684C374
	$-F9     01E03619       FC                   CLD
	$-F8     01E0361A       FF                   DB FF
	$-F7     01E0361B       FF                   DB FF
	$-F6     01E0361C       FF53 8B              CALL DWORD PTR DS:[EBX-0x75]
	$-F3     01E0361F       9C                   PUSHFD
	$-F2     01E03620       24 24                AND AL,0x24
	$-F0     01E03622       0000                 ADD BYTE PTR DS:[EAX],AL
	$-EE     01E03624       00C2                 ADD DL,AL
	$-EC     01E03626       2800                 SUB BYTE PTR DS:[EAX],AL
	$-EA     01E03628       74 5A                JE SHORT MapleSto.01E03684
	$-E8     01E0362A       5A                   POP EDX
	$-E7     01E0362B       5F                   POP EDI
	$-E6     01E0362C       8DA424 14000000      LEA ESP,DWORD PTR SS:[ESP+0x14]
	$-DF     01E03633       C3                   RETN
	$-DE     01E03634       70 55                JO SHORT MapleSto.01E0368B
	$-DC     01E03636       8BEE                 MOV EBP,ESI
	$-DA     01E03638       C7C1 309C2F29        MOV ECX,0x292F9C30
	$-D4     01E0363E       8BCD                 MOV ECX,EBP
	$-D2     01E03640       5D                   POP EBP
	$-D1     01E03641       E8 2A94B5FE          CALL MapleSto.0095CA70
	$-CC     01E03646       E9 3F5D0000          JMP MapleSto.01E0938A
	$-C7     01E0364B       E2 57                LOOPD SHORT MapleSto.01E036A4
	$-C5     01E0364D       9C                   PUSHFD
	$-C4     01E0364E       898424 04000000      MOV DWORD PTR SS:[ESP+0x4],EAX
	$-BD     01E03655       9D                   POPFD
	$-BC     01E03656       FF15 E8983E01        CALL DWORD PTR DS:[0x13E98E8]              ; msvcr90.free
	$-B6     01E0365C       81C4 04000000        ADD ESP,0x4
	$-B0     01E03662       E8 9B400000          CALL MapleSto.01E07702
	$-AB     01E03667     ^ 72 8D                JB SHORT MapleSto.01E035F6
	$-A9     01E03669       55                   PUSH EBP
	$-A8     01E0366A       F0:9C                LOCK PUSHFD
	$-A6     01E0366C       871424               XCHG DWORD PTR SS:[ESP],EDX
	$-A3     01E0366F       8B1424               MOV EDX,DWORD PTR SS:[ESP]
	$-A0     01E03672       68 5211E597          PUSH 0x97E51152
	$-9B     01E03677       52                   PUSH EDX
	$-9A     01E03678       8B9424 04000000      MOV EDX,DWORD PTR SS:[ESP+0x4]
	$-93     01E0367F       8D92 F4EE1A68        LEA EDX,DWORD PTR DS:[EDX+0x681AEEF4]
	$-8D     01E03685       899424 04000000      MOV DWORD PTR SS:[ESP+0x4],EDX
	$-86     01E0368C       5A                   POP EDX
	$-85     01E0368D       56                   PUSH ESI
	$-84     01E0368E       E9 17BA0000          JMP MapleSto.01E0F0AA
	$-7F     01E03693     ^ 76 8D                JBE SHORT MapleSto.01E03622
	$-7D     01E03695       4D                   DEC EBP
	$-7C     01E03696       F0:897D FC           LOCK MOV DWORD PTR SS:[EBP-0x4],EDI
	$-78     01E0369A       FF15 08A23E01        CALL DWORD PTR DS:[0x13EA208]              ; mfc90.#601
	$-72     01E036A0       9C                   PUSHFD
	$-71     01E036A1       55                   PUSH EBP
	$-70     01E036A2       8DA424 FAFFFFFF      LEA ESP,DWORD PTR SS:[ESP-0x6]
	$-69     01E036A9       898424 0A000000      MOV DWORD PTR SS:[ESP+0xA],EAX
	$-62     01E036B0       8DA424 06000000      LEA ESP,DWORD PTR SS:[ESP+0x6]
	$-5B     01E036B7       5D                   POP EBP
	$-5A     01E036B8       C7C0 E0B25485        MOV EAX,0x8554B2E0
	$-54     01E036BE       8D80 8098F17B        LEA EAX,DWORD PTR DS:[EAX+0x7BF19880]
	$-4E     01E036C4       870424               XCHG DWORD PTR SS:[ESP],EAX
	$-4B     01E036C7       8D4D EC              LEA ECX,DWORD PTR SS:[EBP-0x14]
	$-48     01E036CA       9C                   PUSHFD
	$-47     01E036CB       E8 01000000          CALL MapleSto.01E036D1
	$-42     01E036D0     ^ 79 81                JNS SHORT MapleSto.01E03653
	$-40     01E036D2       04 24                ADD AL,0x24
	$-3E     01E036D4       31DD                 XOR EBP,EBX
	$-3C     01E036D6       FF                   DB FF
	$-3B     01E036D7       FFC3                 INC EBX
	$-39     01E036D9     ^ 73 8D                JNB SHORT MapleSto.01E03668
	$-37     01E036DB       4D                   DEC EBP
	$-36     01E036DC       EC                   IN AL,DX
	$-35     01E036DD       8DA424 FCFFFFFF      LEA ESP,DWORD PTR SS:[ESP-0x4]
	$-2E     01E036E4       9C                   PUSHFD
	$-2D     01E036E5       898C24 04000000      MOV DWORD PTR SS:[ESP+0x4],ECX
	$-26     01E036EC       9D                   POPFD
	$-25     01E036ED       53                   PUSH EBX
	$-24     01E036EE       E8 01000000          CALL MapleSto.01E036F4
	$-1F     01E036F3       EB 5B                JMP SHORT MapleSto.01E03750
	$-1D     01E036F5       8D9B B926B6FE        LEA EBX,DWORD PTR DS:[EBX+0xFEB626B9]
	$-17     01E036FB       FFE3                 JMP EBX
	$-15     01E036FD     - E9 8D4DF051          JMP 53D0848F
	$-10     01E03702       9C                   PUSHFD
	$-F      01E03703       898C24 04000000      MOV DWORD PTR SS:[ESP+0x4],ECX
	$-8      01E0370A       9D                   POPFD
	$-7      01E0370B       E8 F34D0000          CALL MapleSto.01E08503
	$-2      01E03710       78 66                JS SHORT MapleSto.01E03778
	$ ==>    01E03712       8B81 84120000        MOV EAX,DWORD PTR DS:[ECX+0x1284]				;g_o_role_lvl_
	$+6      01E03718       55                   PUSH EBP
	$+7      01E03719       8BAC24 04000000      MOV EBP,DWORD PTR SS:[ESP+0x4]
	$+E      01E03720       50                   PUSH EAX
	$+F      01E03721       C7C0 8C042B38        MOV EAX,0x382B048C
	$+15     01E03727       8D80 5C26E9EB        LEA EAX,DWORD PTR DS:[EAX+0xEBE9265C]
	$+1B     01E0372D       870424               XCHG DWORD PTR SS:[ESP],EAX
	$+1E     01E03730       8DA424 FCFFFFFF      LEA ESP,DWORD PTR SS:[ESP-0x4]
	$+25     01E03737       50                   PUSH EAX
	$+26     01E03738       C7C0 62972676        MOV EAX,0x76269762
	$+2C     01E0373E       8D80 EEB725AB        LEA EAX,DWORD PTR DS:[EAX+0xAB25B7EE]
	$+32     01E03744       870424               XCHG DWORD PTR SS:[ESP],EAX
	$+35     01E03747       8DA424 FCFFFFFF      LEA ESP,DWORD PTR SS:[ESP-0x4]
	$+3C     01E0374E       E8 E7620000          CALL MapleSto.01E09A3A
	$+41     01E03753     ^ E1 8D                LOOPDE SHORT MapleSto.01E036E2
	$+43     01E03755       64:24 04             AND AL,0x4
	$+46     01E03758       8DA424 FCFFFFFF      LEA ESP,DWORD PTR SS:[ESP-0x4]
	$+4D     01E0375F       57                   PUSH EDI
	$+4E     01E03760       8DA424 FCFFFFFF      LEA ESP,DWORD PTR SS:[ESP-0x4]
	$+55     01E03767       9C                   PUSHFD
	$+56     01E03768       57                   PUSH EDI
	$+57     01E03769       8BBC24 24000000      MOV EDI,DWORD PTR SS:[ESP+0x24]
	$+5E     01E03770       C2 2800              RETN 0x28
	$+61     01E03773       E0 5A                LOOPDNE SHORT MapleSto.01E037CF
	$+63     01E03775       C745 FC A3000000     MOV DWORD PTR SS:[EBP-0x4],0xA3
	$+6A     01E0377C       E8 DFC9C1FE          CALL MapleSto.00A20160
	$+6F     01E03781       85C0                 TEST EAX,EAX
	$+71     01E03783     - 0F85 1472B5FE        JNZ MapleSto.0095A99D
	$+77     01E03789     - E9 7F48B5FE          JMP MapleSto.0095800D
	$+7C     01E0378E       7E FF                JLE SHORT MapleSto.01E0378F
	$+7E     01E03790       D0E9                 SHR CL,1
	$+80     01E03792       72 08                JB SHORT MapleSto.01E0379C
	$+82     01E03794       0000                 ADD BYTE PTR DS:[EAX],AL
	$+84     01E03796     ^ EB 9D                JMP SHORT MapleSto.01E03735
	$+86     01E03798       8D4D F0              LEA ECX,DWORD PTR SS:[EBP-0x10]
	$+89     01E0379B       FF15 20A23E01        CALL DWORD PTR DS:[0x13EA220]              ; mfc90.#310
	$+8F     01E037A1       68 157A69E0          PUSH 0xE0697A15
	$+94     01E037A6       51                   PUSH ECX
	$+95     01E037A7       8B8C24 04000000      MOV ECX,DWORD PTR SS:[ESP+0x4]
	$+9C     01E037AE       8D89 0D86961F        LEA ECX,DWORD PTR DS:[ECX+0x1F96860D]
	$+A2     01E037B4       898C24 04000000      MOV DWORD PTR SS:[ESP+0x4],ECX
	$+A9     01E037BB       59                   POP ECX
	$+AA     01E037BC       56                   PUSH ESI
	$+AB     01E037BD       59                   POP ECX
	$+AC     01E037BE       C745 FC A8000000     MOV DWORD PTR SS:[EBP-0x4],0xA8
	$+B3     01E037C5       E8 96C9C1FE          CALL MapleSto.00A20160
	$+B8     01E037CA       50                   PUSH EAX
	$+B9     01E037CB       E8 01000000          CALL MapleSto.01E037D1
	$+BE     01E037D0       78 58                JS SHORT MapleSto.01E0382A
	$+C0     01E037D2       8D80 F85F0000        LEA EAX,DWORD PTR DS:[EAX+0x5FF8]
	$+C6     01E037D8       FFD0                 CALL EAX
	$+C8     01E037DA     ^ 74 9D                JE SHORT MapleSto.01E03779
	$+CA     01E037DC     - 0F85 BC6EB5FE        JNZ MapleSto.0095A69E
	$+D0     01E037E2     - E9 7367B5FE          JMP MapleSto.00959F5A
	$+D5     01E037E7     ^ 79 9C                JNS SHORT MapleSto.01E03785
	$+D7     01E037E9       872C24               XCHG DWORD PTR SS:[ESP],EBP
	$+DA     01E037EC       8B2C24               MOV EBP,DWORD PTR SS:[ESP]
	$+DD     01E037EF       8BEC                 MOV EBP,ESP
	$+DF     01E037F1       8B45 08              MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+E2     01E037F4       8941 14              MOV DWORD PTR DS:[ECX+0x14],EAX
	$+E5     01E037F7       9C                   PUSHFD
	$+E6     01E037F8       87AC24 04000000      XCHG DWORD PTR SS:[ESP+0x4],EBP
	$+ED     01E037FF       8DA424 08000000      LEA ESP,DWORD PTR SS:[ESP+0x8]
	$+F4     01E03806       52                   PUSH EDX
	$+F5     01E03807       8B9424 04000000      MOV EDX,DWORD PTR SS:[ESP+0x4]
	$+FC     01E0380E       53                   PUSH EBX
	$+FD     01E0380F       E8 01000000          CALL MapleSto.01E03815
	$+102    01E03814       EB 5B                JMP SHORT MapleSto.01E03871
	$+104    01E03816       8D9B 3BD8FFFF        LEA EBX,DWORD PTR DS:[EBX-0x27C5]
	$+10A    01E0381C       FFD3                 CALL EBX
	$+10C    01E0381E       70 58                JO SHORT MapleSto.01E03878
	$+10E    01E03820       58                   POP EAX
	$+10F    01E03821       8BCD                 MOV ECX,EBP
	$+111    01E03823       5D                   POP EBP
	$+112    01E03824       C745 FC 7A000000     MOV DWORD PTR SS:[EBP-0x4],0x7A
	$+119    01E0382B       E8 30C9C1FE          CALL MapleSto.00A20160
	$+11E    01E03830       85C0                 TEST EAX,EAX
	$+120    01E03832     - 0F85 74CAB5FE        JNZ MapleSto.009602AC
	$+126    01E03838     ^ E9 9AFAFFFF          JMP MapleSto.01E032D7
	$+12B    01E0383D     ^ E0 9D                LOOPDNE SHORT MapleSto.01E037DC
	$+12D    01E0383F       870424               XCHG DWORD PTR SS:[ESP],EAX
	$+130    01E03842       8B0424               MOV EAX,DWORD PTR SS:[ESP]
	$+133    01E03845       C7C0 65FE5899        MOV EAX,0x9958FE65
	$+139    01E0384B       8D80 5338ED67        LEA EAX,DWORD PTR DS:[EAX+0x67ED3853]
	$+13F    01E03851       870424               XCHG DWORD PTR SS:[ESP],EAX
	$+142    01E03854       9C                   PUSHFD
	$+143    01E03855       9C                   PUSHFD
	$+144    01E03856       89B424 04000000      MOV DWORD PTR SS:[ESP+0x4],ESI
	$+14B    01E0385D       9D                   POPFD
	$+14C    01E0385E       E8 F503D9FF          CALL MapleSto.01B93C58
	$+151    01E03863       81C4 0C000000        ADD ESP,0xC
	$+157    01E03869       833D 3C357301 00     CMP DWORD PTR DS:[0x173353C],0x0
	$+15E    01E03870       C7C0 D31BA728        MOV EAX,0x28A71BD3
	$+164    01E03876       E8 9485B5FE          CALL MapleSto.0095BE0F
	$+169    01E0387B     ^ E1 C6                LOOPDE SHORT MapleSto.01E03843
	$+16B    01E0387D       45                   INC EBP
	$+16C    01E0387E       E8 01E95530          CALL 32362184
	$+171    01E03883       B6 FE                MOV DH,0xFE
	$+173    01E03885       E3 68                JECXZ SHORT MapleSto.01E038EF
	$+175    01E03887       3B80 C15D8734        CMP EAX,DWORD PTR DS:[EAX+0x34875DC1]
	$+17B    01E0388D       24 8B                AND AL,0x8B
	$+17D    01E0388F       34 24                XOR AL,0x24
	$+17F    01E03891       9C                   PUSHFD
	$+180    01E03892       9C                   PUSHFD
	$+181    01E03893       89BC24 04000000      MOV DWORD PTR SS:[ESP+0x4],EDI
	$+188    01E0389A       9D                   POPFD
	$+189    01E0389B       68 6AE88ABB          PUSH 0xBB8AE86A
	$+18E    01E038A0       53                   PUSH EBX
	$+18F    01E038A1     ^ E9 7CDEFFFF          JMP MapleSto.01E01722
	$+194    01E038A6     - E9 5B5B8704          JMP 06679406
	$+199    01E038AB       24 8D                AND AL,0x8D
	$+19B    01E038AD       A4                   MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
	$+19C    01E038AE       24 FC                AND AL,0xFC
	$+19E    01E038B0       FF                   DB FF
	$+19F    01E038B1       FF                   DB FF
	$+1A0    01E038B2       FF51 8B              CALL DWORD PTR DS:[ECX-0x75]
	$+1A3    01E038B5       8C241C               MOV WORD PTR SS:[ESP+EBX],FS
	$+1A6    01E038B8       0000                 ADD BYTE PTR DS:[EAX],AL
	$+1A8    01E038BA       00C2                 ADD DL,AL
	$+1AA    01E038BC       2000                 AND BYTE PTR DS:[EAX],AL
	$+1AC    01E038BE     ^ EB 8D                JMP SHORT MapleSto.01E0384D
	$+1AE    01E038C0       A4                   MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
	$+1AF    01E038C1       24 FC                AND AL,0xFC
	$+1B1    01E038C3       FF                   DB FF
	$+1B2    01E038C4       FF                   DB FF
	$+1B3    01E038C5       FF87 1C248B1C        INC DWORD PTR DS:[EDI+0x1C8B241C]
	$+1B9    01E038CB       24 9C                AND AL,0x9C
	$+1BB    01E038CD       9C                   PUSHFD
	$+1BC    01E038CE       89B424 04000000      MOV DWORD PTR SS:[ESP+0x4],ESI
	$+1C3    01E038D5       9D                   POPFD
	$+1C4    01E038D6       8DA424 FCFFFFFF      LEA ESP,DWORD PTR SS:[ESP-0x4]
	$+1CB    01E038DD       9C                   PUSHFD
	$+1CC    01E038DE       89BC24 04000000      MOV DWORD PTR SS:[ESP+0x4],EDI
	$+1D3    01E038E5       9D                   POPFD
	$+1D4    01E038E6       51                   PUSH ECX
	$+1D5    01E038E7       5F                   POP EDI
	$+1D6    01E038E8       8B47 7C              MOV EAX,DWORD PTR DS:[EDI+0x7C]
	$+1D9    01E038EB       8B48 04              MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+1DC    01E038EE       8D77 64              LEA ESI,DWORD PTR DS:[EDI+0x64]
	$+1DF    01E038F1       53                   PUSH EBX
	$+1E0    01E038F2       E8 01000000          CALL MapleSto.01E038F8
	$+1E5    01E038F7       77 5B                JA SHORT MapleSto.01E03954
	$+1E7    01E038F9       8D9B D5440000        LEA EBX,DWORD PTR DS:[EBX+0x44D5]
	$+1ED    01E038FF       FFD3                 CALL EBX
	$+1EF    01E03901       7B 8D                JPO SHORT MapleSto.01E03890
	$+1F1    01E03903       64:24 04             AND AL,0x4
	$+1F4    01E03906       85C0                 TEST EAX,EAX
	$+1F6    01E03908     - 0F85 A637B5FE        JNZ MapleSto.009570B4
	*/
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("8b??????????558b????????????50c7??????????8d??????????87????8d????????????50c7??????????8d", 2);
	if (!op_dw_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *op_dw_tmp;
	assert(dw_tmp > 0x1200 && dw_tmp < 0x1400);
	ofs << "pt_cdword	g_o_role_lvl_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	/*
	$-1DF    005DB1FF     CC                      INT3
	$-1DE    005DB200     55                      PUSH EBP
	$-1DD    005DB201     8BEC                    MOV EBP,ESP
	$-1DB    005DB203     8B45 08                 MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-1D8    005DB206     56                      PUSH ESI
	$-1D7    005DB207     6A 00                   PUSH 0x0
	$-1D5    005DB209     68 38C16C01             PUSH MapleSto.016CC138
	$-1D0    005DB20E     68 F0C06C01             PUSH MapleSto.016CC0F0
	$-1CB    005DB213     6A 00                   PUSH 0x0
	$-1C9    005DB215     50                      PUSH EAX
	$-1C8    005DB216     8BF1                    MOV ESI,ECX
	$-1C6    005DB218     E8 3F6BC200             CALL MapleSto.01201D5C                      ; JMP to msvcr90.__RTDynamicCast
	$-1C1    005DB21D     83C4 14                 ADD ESP,0x14
	$-1BE    005DB220     85C0                    TEST EAX,EAX
	$-1BC    005DB222     75 07                   JNZ SHORT MapleSto.005DB22B
	$-1BA    005DB224     32C0                    XOR AL,AL
	$-1B8    005DB226     5E                      POP ESI
	$-1B7    005DB227     5D                      POP EBP
	$-1B6    005DB228     C2 0400                 RETN 0x4
	$-1B3    005DB22B     8B4E 04                 MOV ECX,DWORD PTR DS:[ESI+0x4]
	$-1B0    005DB22E     3B48 04                 CMP ECX,DWORD PTR DS:[EAX+0x4]
	$-1AD    005DB231     75 12                   JNZ SHORT MapleSto.005DB245
	$-1AB    005DB233     8B56 08                 MOV EDX,DWORD PTR DS:[ESI+0x8]
	$-1A8    005DB236     3B50 08                 CMP EDX,DWORD PTR DS:[EAX+0x8]
	$-1A5    005DB239     75 0A                   JNZ SHORT MapleSto.005DB245
	$-1A3    005DB23B     B8 01000000             MOV EAX,0x1
	$-19E    005DB240     5E                      POP ESI
	$-19D    005DB241     5D                      POP EBP
	$-19C    005DB242     C2 0400                 RETN 0x4
	$-199    005DB245     33C0                    XOR EAX,EAX
	$-197    005DB247     5E                      POP ESI
	$-196    005DB248     5D                      POP EBP
	$-195    005DB249     C2 0400                 RETN 0x4
	$-192    005DB24C     CC                      INT3
	$-191    005DB24D     CC                      INT3
	$-190    005DB24E     CC                      INT3
	$-18F    005DB24F     CC                      INT3
	$-18E    005DB250     C3                      RETN
	$-18D    005DB251     CC                      INT3
	$-18C    005DB252     CC                      INT3
	$-18B    005DB253     CC                      INT3
	$-18A    005DB254     CC                      INT3
	$-189    005DB255     CC                      INT3
	$-188    005DB256     CC                      INT3
	$-187    005DB257     CC                      INT3
	$-186    005DB258     CC                      INT3
	$-185    005DB259     CC                      INT3
	$-184    005DB25A     CC                      INT3
	$-183    005DB25B     CC                      INT3
	$-182    005DB25C     CC                      INT3
	$-181    005DB25D     CC                      INT3
	$-180    005DB25E     CC                      INT3
	$-17F    005DB25F     CC                      INT3
	$-17E    005DB260     55                      PUSH EBP
	$-17D    005DB261     8BEC                    MOV EBP,ESP
	$-17B    005DB263     83EC 0C                 SUB ESP,0xC
	$-178    005DB266     8B4D 08                 MOV ECX,DWORD PTR SS:[EBP+0x8]
	$-175    005DB269     56                      PUSH ESI
	$-174    005DB26A     33F6                    XOR ESI,ESI
	$-172    005DB26C     57                      PUSH EDI
	$-171    005DB26D     8975 FC                 MOV DWORD PTR SS:[EBP-0x4],ESI
	$-16E    005DB270     3BCE                    CMP ECX,ESI
	$-16C    005DB272     74 1E                   JE SHORT MapleSto.005DB292
	$-16A    005DB274     8B01                    MOV EAX,DWORD PTR DS:[ECX]
	$-168    005DB276     8B40 14                 MOV EAX,DWORD PTR DS:[EAX+0x14]
	$-165    005DB279     8D55 FC                 LEA EDX,DWORD PTR SS:[EBP-0x4]
	$-162    005DB27C     52                      PUSH EDX
	$-161    005DB27D     68 13002006             PUSH 0x6200013
	$-15C    005DB282     FFD0                    CALL EAX
	$-15A    005DB284     84C0                    TEST AL,AL
	$-158    005DB286     74 0A                   JE SHORT MapleSto.005DB292
	$-156    005DB288     8B7D FC                 MOV EDI,DWORD PTR SS:[EBP-0x4]
	$-153    005DB28B     897D 08                 MOV DWORD PTR SS:[EBP+0x8],EDI
	$-150    005DB28E     3BFE                    CMP EDI,ESI
	$-14E    005DB290     75 08                   JNZ SHORT MapleSto.005DB29A
	$-14C    005DB292     5F                      POP EDI
	$-14B    005DB293     33C0                    XOR EAX,EAX
	$-149    005DB295     5E                      POP ESI
	$-148    005DB296     8BE5                    MOV ESP,EBP
	$-146    005DB298     5D                      POP EBP
	$-145    005DB299     C3                      RETN
	$-144    005DB29A     8B4D 0C                 MOV ECX,DWORD PTR SS:[EBP+0xC]
	$-141    005DB29D     53                      PUSH EBX
	$-140    005DB29E     8975 F8                 MOV DWORD PTR SS:[EBP-0x8],ESI
	$-13D    005DB2A1     3BCE                    CMP ECX,ESI
	$-13B    005DB2A3     0F84 9E010000           JE MapleSto.005DB447
	$-135    005DB2A9     8B11                    MOV EDX,DWORD PTR DS:[ECX]
	$-133    005DB2AB     8B52 14                 MOV EDX,DWORD PTR DS:[EDX+0x14]
	$-130    005DB2AE     8D45 F8                 LEA EAX,DWORD PTR SS:[EBP-0x8]
	$-12D    005DB2B1     50                      PUSH EAX
	$-12C    005DB2B2     68 13002006             PUSH 0x6200013
	$-127    005DB2B7     FFD2                    CALL EDX
	$-125    005DB2B9     84C0                    TEST AL,AL
	$-123    005DB2BB     0F84 86010000           JE MapleSto.005DB447
	$-11D    005DB2C1     8B75 F8                 MOV ESI,DWORD PTR SS:[EBP-0x8]
	$-11A    005DB2C4     8BDE                    MOV EBX,ESI
	$-118    005DB2C6     85F6                    TEST ESI,ESI
	$-116    005DB2C8     0F84 79010000           JE MapleSto.005DB447
	$-110    005DB2CE     8BCF                    MOV ECX,EDI
	$-10E    005DB2D0     E8 B3748401             CALL MapleSto.01E22788
	$-109    005DB2D5     8BC8                    MOV ECX,EAX
	$-107    005DB2D7     E8 6F426C01             CALL MapleSto.01C9F54B
	$-102    005DB2DC     85C0                    TEST EAX,EAX
	$-100    005DB2DE     74 03                   JE SHORT MapleSto.005DB2E3
	$-FE     005DB2E0     0945 10                 OR DWORD PTR SS:[EBP+0x10],EAX
	$-FB     005DB2E3     8B06                    MOV EAX,DWORD PTR DS:[ESI]
	$-F9     005DB2E5     8B90 C4010000           MOV EDX,DWORD PTR DS:[EAX+0x1C4]
	$-F3     005DB2EB     8BCE                    MOV ECX,ESI
	$-F1     005DB2ED     FFD2                    CALL EDX
	$-EF     005DB2EF     83F8 02                 CMP EAX,0x2
	$-EC     005DB2F2     0F84 4F010000           JE MapleSto.005DB447
	$-E6     005DB2F8     F645 10 04              TEST BYTE PTR SS:[EBP+0x10],0x4
	$-E2     005DB2FC     75 16                   JNZ SHORT MapleSto.005DB314
	$-E0     005DB2FE     8BCB                    MOV ECX,EBX
	$-DE     005DB300     E8 83748401             CALL MapleSto.01E22788
	$-D9     005DB305     8BC8                    MOV ECX,EAX
	$-D7     005DB307     E8 36786C01             CALL MapleSto.01CA2B42
	$-D2     005DB30C     85C0                    TEST EAX,EAX
	$-D0     005DB30E     0F85 33010000           JNZ MapleSto.005DB447
	$-CA     005DB314     8BC7                    MOV EAX,EDI
	$-C8     005DB316     50                      PUSH EAX
	$-C7     005DB317     E8 4489FCFF             CALL MapleSto.005A3C60
	$-C2     005DB31C     53                      PUSH EBX
	$-C1     005DB31D     8BF0                    MOV ESI,EAX
	$-BF     005DB31F     E8 3C89FCFF             CALL MapleSto.005A3C60
	$-BA     005DB324     83C4 08                 ADD ESP,0x8
	$-B7     005DB327     8BF8                    MOV EDI,EAX
	$-B5     005DB329     85F6                    TEST ESI,ESI
	$-B3     005DB32B     74 20                   JE SHORT MapleSto.005DB34D
	$-B1     005DB32D     85FF                    TEST EDI,EDI
	$-AF     005DB32F     74 1C                   JE SHORT MapleSto.005DB34D
	$-AD     005DB331     8BCF                    MOV ECX,EDI
	$-AB     005DB333     E8 74D08301             CALL MapleSto.01E183AC
	$-A6     005DB338     8BCE                    MOV ECX,ESI
	$-A4     005DB33A     8945 F4                 MOV DWORD PTR SS:[EBP-0xC],EAX
	$-A1     005DB33D     E8 6AD08301             CALL MapleSto.01E183AC
	$-9C     005DB342     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-99     005DB345     33C8                    XOR ECX,EAX
	$-97     005DB347     0F85 FA000000           JNZ MapleSto.005DB447
	$-91     005DB34D     8B75 14                 MOV ESI,DWORD PTR SS:[EBP+0x14]
	$-8E     005DB350     85F6                    TEST ESI,ESI
	$-8C     005DB352     74 25                   JE SHORT MapleSto.005DB379
	$-8A     005DB354     53                      PUSH EBX
	$-89     005DB355     E8 6645FDFF             CALL MapleSto.005AF8C0
	$-84     005DB35A     83C4 04                 ADD ESP,0x4
	$-81     005DB35D     85C0                    TEST EAX,EAX
	$-7F     005DB35F     74 18                   JE SHORT MapleSto.005DB379
	$-7D     005DB361     8BC8                    MOV ECX,EAX
	$-7B     005DB363     E8 D66A7401             CALL MapleSto.01D21E3E
	$-76     005DB368     BA 01000000             MOV EDX,0x1
	$-71     005DB36D     8BC8                    MOV ECX,EAX
	$-6F     005DB36F     D3E2                    SHL EDX,CL
	$-6D     005DB371     85D6                    TEST ESI,EDX
	$-6B     005DB373     0F85 CE000000           JNZ MapleSto.005DB447
	$-65     005DB379     F645 10 01              TEST BYTE PTR SS:[EBP+0x10],0x1
	$-61     005DB37D     8B75 08                 MOV ESI,DWORD PTR SS:[EBP+0x8]
	$-5E     005DB380     0F84 82000000           JE MapleSto.005DB408
	$-58     005DB386     8B06                    MOV EAX,DWORD PTR DS:[ESI]
	$-56     005DB388     8B90 C4010000           MOV EDX,DWORD PTR DS:[EAX+0x1C4]
	$-50     005DB38E     8BCE                    MOV ECX,ESI
	$-4E     005DB390     FFD2                    CALL EDX
	$-4C     005DB392     83F8 02                 CMP EAX,0x2
	$-49     005DB395     74 11                   JE SHORT MapleSto.005DB3A8
	$-47     005DB397     8B06                    MOV EAX,DWORD PTR DS:[ESI]
	$-45     005DB399     8B90 DC010000           MOV EDX,DWORD PTR DS:[EAX+0x1DC]
	$-3F     005DB39F     53                      PUSH EBX
	$-3E     005DB3A0     8BCE                    MOV ECX,ESI
	$-3C     005DB3A2     FFD2                    CALL EDX
	$-3A     005DB3A4     85C0                    TEST EAX,EAX
	$-38     005DB3A6     74 60                   JE SHORT MapleSto.005DB408
	$-36     005DB3A8     85FF                    TEST EDI,EDI
	$-34     005DB3AA     74 1F                   JE SHORT MapleSto.005DB3CB
	$-32     005DB3AC     6A 04                   PUSH 0x4
	$-30     005DB3AE     8BCF                    MOV ECX,EDI
	$-2E     005DB3B0     E8 6BCA3900             CALL MapleSto.00977E20                      ; call tag_获得角色当前属性值
	$-29     005DB3B5     85D2                    TEST EDX,EDX
	$-27     005DB3B7     7C 4F                   JL SHORT MapleSto.005DB408
	$-25     005DB3B9     7F 04                   JG SHORT MapleSto.005DB3BF
	$-23     005DB3BB     85C0                    TEST EAX,EAX
	$-21     005DB3BD     76 49                   JBE SHORT MapleSto.005DB408
	$-1F     005DB3BF     5B                      POP EBX
	$-1E     005DB3C0     5F                      POP EDI
	$-1D     005DB3C1     B8 01000000             MOV EAX,0x1
	$-18     005DB3C6     5E                      POP ESI
	$-17     005DB3C7     8BE5                    MOV ESP,EBP
	$-15     005DB3C9     5D                      POP EBP
	$-14     005DB3CA     C3                      RETN
	$-13     005DB3CB     53                      PUSH EBX
	$-12     005DB3CC     E8 EF44FDFF             CALL MapleSto.005AF8C0
	$-D      005DB3D1     8BF0                    MOV ESI,EAX
	$-B      005DB3D3     83C4 04                 ADD ESP,0x4
	$-8      005DB3D6     85F6                    TEST ESI,ESI
	$-6      005DB3D8     74 2B                   JE SHORT MapleSto.005DB405
	$-4      005DB3DA     6A 04                   PUSH 0x4
	$-2      005DB3DC     8BCE                    MOV ECX,ESI                                 ; tag_获得角色最大属性值 函数就在 tag_获得角色当前属性值 下面
	$ ==>    005DB3DE     E8 3DCA3900             CALL MapleSto.00977E20                      ; call tag_获得角色当前属性值
	$+5      005DB3E3     85D2                    TEST EDX,EDX
	$+7      005DB3E5     7C 06                   JL SHORT MapleSto.005DB3ED
	$+9      005DB3E7   ^ 7F D6                   JG SHORT MapleSto.005DB3BF
	$+B      005DB3E9     85C0                    TEST EAX,EAX
	$+D      005DB3EB   ^ 77 D2                   JA SHORT MapleSto.005DB3BF
	$+F      005DB3ED     6A 04                   PUSH 0x4
	$+11     005DB3EF     8BCE                    MOV ECX,ESI
	$+13     005DB3F1     E8 2ACA3900             CALL MapleSto.00977E20                      ; call tag_获得角色当前属性值
	$+18     005DB3F6     0BC2                    OR EAX,EDX
	$+1A     005DB3F8     75 0B                   JNZ SHORT MapleSto.005DB405
	$+1C     005DB3FA     8BCE                    MOV ECX,ESI
	$+1E     005DB3FC     E8 30617401             CALL MapleSto.01D21531
	$+23     005DB401     85C0                    TEST EAX,EAX
	$+25     005DB403   ^ 75 BA                   JNZ SHORT MapleSto.005DB3BF
	$+27     005DB405     8B75 08                 MOV ESI,DWORD PTR SS:[EBP+0x8]
	$+2A     005DB408     F645 10 02              TEST BYTE PTR SS:[EBP+0x10],0x2
	$+2E     005DB40C     74 39                   JE SHORT MapleSto.005DB447
	$+30     005DB40E     8B06                    MOV EAX,DWORD PTR DS:[ESI]
	$+32     005DB410     8B90 C4010000           MOV EDX,DWORD PTR DS:[EAX+0x1C4]
	$+38     005DB416     8BCE                    MOV ECX,ESI
	$+3A     005DB418     FFD2                    CALL EDX
	$+3C     005DB41A     83F8 02                 CMP EAX,0x2
	$+3F     005DB41D     74 11                   JE SHORT MapleSto.005DB430
	$+41     005DB41F     8B06                    MOV EAX,DWORD PTR DS:[ESI]
	$+43     005DB421     8B90 DC010000           MOV EDX,DWORD PTR DS:[EAX+0x1DC]
	$+49     005DB427     53                      PUSH EBX
	$+4A     005DB428     8BCE                    MOV ECX,ESI
	$+4C     005DB42A     FFD2                    CALL EDX
	$+4E     005DB42C     85C0                    TEST EAX,EAX
	$+50     005DB42E     75 17                   JNZ SHORT MapleSto.005DB447
	$+52     005DB430     6A 04                   PUSH 0x4
	$+54     005DB432     8BCB                    MOV ECX,EBX
	$+56     005DB434     E8 E7C93900             CALL MapleSto.00977E20                      ; call tag_获得角色当前属性值
	$+5B     005DB439     85D2                    TEST EDX,EDX
	$+5D     005DB43B     7C 0A                   JL SHORT MapleSto.005DB447
	$+5F     005DB43D   ^ 7F 80                   JG SHORT MapleSto.005DB3BF
	$+61     005DB43F     85C0                    TEST EAX,EAX
	$+63     005DB441   ^ 0F87 78FFFFFF           JA MapleSto.005DB3BF
	$+69     005DB447     5B                      POP EBX
	$+6A     005DB448     5F                      POP EDI
	$+6B     005DB449     33C0                    XOR EAX,EAX
	$+6D     005DB44B     5E                      POP ESI
	$+6E     005DB44C     8BE5                    MOV ESP,EBP
	$+70     005DB44E     5D                      POP EBP
	$+71     005DB44F     C3                      RETN
	$+72     005DB450     55                      PUSH EBP
	$+73     005DB451     8BEC                    MOV EBP,ESP
	$+75     005DB453     6A FF                   PUSH -0x1
	$+77     005DB455     68 84AA3001             PUSH MapleSto.0130AA84
	$+7C     005DB45A     64:A1 00000000          MOV EAX,DWORD PTR FS:[0]
	$+82     005DB460     50                      PUSH EAX
	$+83     005DB461     51                      PUSH ECX
	$+84     005DB462     56                      PUSH ESI
	$+85     005DB463     57                      PUSH EDI
	$+86     005DB464     A1 D0687101             MOV EAX,DWORD PTR DS:[0x17168D0]
	$+8B     005DB469     33C5                    XOR EAX,EBP
	$+8D     005DB46B     50                      PUSH EAX
	$+8E     005DB46C     8D45 F4                 LEA EAX,DWORD PTR SS:[EBP-0xC]
	$+91     005DB46F     64:A3 00000000          MOV DWORD PTR FS:[0],EAX
	$+97     005DB475     8BF1                    MOV ESI,ECX
	$+99     005DB477     8975 F0                 MOV DWORD PTR SS:[EBP-0x10],ESI
	$+9C     005DB47A     C706 782A4201           MOV DWORD PTR DS:[ESI],MapleSto.01422A78
	$+A2     005DB480     8B7E 54                 MOV EDI,DWORD PTR DS:[ESI+0x54]
	$+A5     005DB483     C745 FC 01000000        MOV DWORD PTR SS:[EBP-0x4],0x1
	$+AC     005DB48A     85FF                    TEST EDI,EDI
	$+AE     005DB48C     74 17                   JE SHORT MapleSto.005DB4A5
	$+B0     005DB48E     8D47 04                 LEA EAX,DWORD PTR DS:[EDI+0x4]
	$+B3     005DB491     50                      PUSH EAX
	$+B4     005DB492     FF15 6C933E01           CALL DWORD PTR DS:[0x13E936C]
	$+BA     005DB498     85C0                    TEST EAX,EAX
	$+BC     005DB49A     75 09                   JNZ SHORT MapleSto.005DB4A5
	$+BE     005DB49C     8B17                    MOV EDX,DWORD PTR DS:[EDI]
	$+C0     005DB49E     8B42 04                 MOV EAX,DWORD PTR DS:[EDX+0x4]
	$+C3     005DB4A1     8BCF                    MOV ECX,EDI
	$+C5     005DB4A3     FFD0                    CALL EAX
	$+C7     005DB4A5     8D4E 50                 LEA ECX,DWORD PTR DS:[ESI+0x50]
	$+CA     005DB4A8     FF15 08A23E01           CALL DWORD PTR DS:[0x13EA208]               ; mfc90.#601
	$+D0     005DB4AE     C706 EC2D3F01           MOV DWORD PTR DS:[ESI],MapleSto.013F2DEC
	$+D6     005DB4B4     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+D9     005DB4B7     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
	$+E0     005DB4BE     59                      POP ECX
	$+E1     005DB4BF     5F                      POP EDI
	$+E2     005DB4C0     5E                      POP ESI
	$+E3     005DB4C1     8BE5                    MOV ESP,EBP
	$+E5     005DB4C3     5D                      POP EBP
	$+E6     005DB4C4     C3                      RETN
	$+E7     005DB4C5     CC                      INT3
	$+E8     005DB4C6     CC                      INT3
	$+E9     005DB4C7     CC                      INT3
	$+EA     005DB4C8     CC                      INT3
	$+EB     005DB4C9     CC                      INT3
	$+EC     005DB4CA     CC                      INT3
	$+ED     005DB4CB     CC                      INT3
	$+EE     005DB4CC     CC                      INT3
	$+EF     005DB4CD     CC                      INT3
	$+F0     005DB4CE     CC                      INT3
	$+F1     005DB4CF     CC                      INT3
	$+F2     005DB4D0   - E9 87A36D01             JMP MapleSto.01CB585C
	$+F7     005DB4D5     F3:0F2C4E 68            CVTTSS2SI ECX,DWORD PTR DS:[ESI+0x68]
	$+FC     005DB4DA     53                      PUSH EBX
	$+FD     005DB4DB     9C                      PUSHFD
	$+FE     005DB4DC     898C24 04000000         MOV DWORD PTR SS:[ESP+0x4],ECX
	$+105    005DB4E3     9D                      POPFD
	$+106    005DB4E4     8D8E FC000000           LEA ECX,DWORD PTR DS:[ESI+0xFC]
	$+10C    005DB4EA     E8 B7706A01             CALL MapleSto.01C825A6
	$+111    005DB4EF   - E9 CC326D01             JMP MapleSto.01CAE7C0
	$+116    005DB4F4     E8 8B00E9E2             CALL E346B584
	$+11B    005DB4F9     D9FF                    FCOS
	$+11D    005DB4FB     FF77 CC                 PUSH DWORD PTR DS:[EDI-0x34]
	$+120    005DB4FE     CC                      INT3
	$+121    005DB4FF     CC                      INT3
	$+122    005DB500     55                      PUSH EBP
	$+123    005DB501     8BEC                    MOV EBP,ESP
	$+125    005DB503     F3:0F1045 08            MOVSS XMM0,DWORD PTR SS:[EBP+0x8]
	$+12A    005DB508     F3:0F1141 10            MOVSS DWORD PTR DS:[ECX+0x10],XMM0
	$+12F    005DB50D     F3:0F1045 0C            MOVSS XMM0,DWORD PTR SS:[EBP+0xC]
	$+134    005DB512     F3:0F1141 14            MOVSS DWORD PTR DS:[ECX+0x14],XMM0
	$+139    005DB517     F3:0F1045 10            MOVSS XMM0,DWORD PTR SS:[EBP+0x10]
	$+13E    005DB51C     F3:0F1141 18            MOVSS DWORD PTR DS:[ECX+0x18],XMM0
	$+143    005DB521     C741 58 00000000        MOV DWORD PTR DS:[ECX+0x58],0x0
	$+14A    005DB528     5D                      POP EBP
	$+14B    005DB529     C2 1C00                 RETN 0x1C
	$+14E    005DB52C     CC                      INT3
	$+14F    005DB52D     CC                      INT3
	$+150    005DB52E     CC                      INT3
	$+151    005DB52F     CC                      INT3
	$+152    005DB530     55                      PUSH EBP
	$+153    005DB531     8BEC                    MOV EBP,ESP
	$+155    005DB533     F3:0F1045 08            MOVSS XMM0,DWORD PTR SS:[EBP+0x8]
	$+15A    005DB538     F3:0F1141 10            MOVSS DWORD PTR DS:[ECX+0x10],XMM0
	$+15F    005DB53D     F3:0F1045 0C            MOVSS XMM0,DWORD PTR SS:[EBP+0xC]
	$+164    005DB542     F3:0F1141 14            MOVSS DWORD PTR DS:[ECX+0x14],XMM0
	$+169    005DB547     F3:0F1045 10            MOVSS XMM0,DWORD PTR SS:[EBP+0x10]
	$+16E    005DB54C     F3:0F1141 18            MOVSS DWORD PTR DS:[ECX+0x18],XMM0
	$+173    005DB551     C741 58 00000000        MOV DWORD PTR DS:[ECX+0x58],0x0
	$+17A    005DB558     5D                      POP EBP
	$+17B    005DB559     C2 1400                 RETN 0x14
	$+17E    005DB55C     CC                      INT3
	$+17F    005DB55D     CC                      INT3
	$+180    005DB55E     CC                      INT3
	$+181    005DB55F     CC                      INT3
	$+182    005DB560     55                      PUSH EBP
	$+183    005DB561     8BEC                    MOV EBP,ESP
	$+185    005DB563     83EC 50                 SUB ESP,0x50
	$+188    005DB566     56                      PUSH ESI
	$+189    005DB567     8B75 08                 MOV ESI,DWORD PTR SS:[EBP+0x8]
	$+18C    005DB56A     57                      PUSH EDI
	$+18D    005DB56B     85F6                    TEST ESI,ESI
	$+18F    005DB56D     0F84 D5030000           JE MapleSto.005DB948
	$+195    005DB573     8B7D 0C                 MOV EDI,DWORD PTR SS:[EBP+0xC]
	$+198    005DB576     85FF                    TEST EDI,EDI
	$+19A    005DB578     0F84 CA030000           JE MapleSto.005DB948
	$+1A0    005DB57E     8B46 34                 MOV EAX,DWORD PTR DS:[ESI+0x34]
	$+1A3    005DB581     B9 01000000             MOV ECX,0x1
	$+1A8    005DB586     C645 0B 00              MOV BYTE PTR SS:[EBP+0xB],0x0
	$+1AC    005DB58A     3BC1                    CMP EAX,ECX
	$+1AE    005DB58C     75 28                   JNZ SHORT MapleSto.005DB5B6
	$+1B0    005DB58E     394F 34                 CMP DWORD PTR DS:[EDI+0x34],ECX
	$+1B3    005DB591     75 23                   JNZ SHORT MapleSto.005DB5B6
	$+1B5    005DB593     51                      PUSH ECX
	$+1B6    005DB594     81C7 98000000           ADD EDI,0x98
	$+1BC    005DB59A     57                      PUSH EDI
	$+1BD    005DB59B     81C6 98000000           ADD ESI,0x98
	$+1C3    005DB5A1     56                      PUSH ESI
	$+1C4    005DB5A2     E8 59220000             CALL MapleSto.005DD800
	$+1C9    005DB5A7     83C4 0C                 ADD ESP,0xC
	$+1CC    005DB5AA     5F                      POP EDI
	$+1CD    005DB5AB     8845 0B                 MOV BYTE PTR SS:[EBP+0xB],AL
	$+1D0    005DB5AE     0FB6C0                  MOVZX EAX,AL
	$+1D3    005DB5B1     5E                      POP ESI
	$+1D4    005DB5B2     8BE5                    MOV ESP,EBP
	$+1D6    005DB5B4     5D                      POP EBP
	$+1D7    005DB5B5     C3                      RETN
	$+1D8    005DB5B6     83F8 02                 CMP EAX,0x2
	$+1DB    005DB5B9     0F85 E3000000           JNZ MapleSto.005DB6A2
	$+1E1    005DB5BF     394F 34                 CMP DWORD PTR DS:[EDI+0x34],ECX
	$+1E4    005DB5C2     0F85 DA000000           JNZ MapleSto.005DB6A2
	$+1EA    005DB5C8     F3:0F1046 7C            MOVSS XMM0,DWORD PTR DS:[ESI+0x7C]
	$+1EF    005DB5CD     F3:0F108F 98000000      MOVSS XMM1,DWORD PTR DS:[EDI+0x98]
	$+1F7    005DB5D5     F3:0F1145 D4            MOVSS DWORD PTR SS:[EBP-0x2C],XMM0
	$+1FC    005DB5DA     F3:0F1086 80000000      MOVSS XMM0,DWORD PTR DS:[ESI+0x80]
	$+204    005DB5E2     F3:0F1145 D8            MOVSS DWORD PTR SS:[EBP-0x28],XMM0
	$+209    005DB5E7     F3:0F1086 84000000      MOVSS XMM0,DWORD PTR DS:[ESI+0x84]
	$+211    005DB5EF     F3:0F114D C8            MOVSS DWORD PTR SS:[EBP-0x38],XMM1
	$+216    005DB5F4     F3:0F108F 9C000000      MOVSS XMM1,DWORD PTR DS:[EDI+0x9C]
	$+21E    005DB5FC     F3:0F1145 DC            MOVSS DWORD PTR SS:[EBP-0x24],XMM0
	$+223    005DB601     F3:0F114D CC            MOVSS DWORD PTR SS:[EBP-0x34],XMM1
	$+228    005DB606     F3:0F1145 D0            MOVSS DWORD PTR SS:[EBP-0x30],XMM0
	$+22D    005DB60B     FF15 549A3E01           CALL DWORD PTR DS:[0x13E9A54]
	$+233    005DB611     8B10                    MOV EDX,DWORD PTR DS:[EAX]
	$+235    005DB613     8B92 24010000           MOV EDX,DWORD PTR DS:[EDX+0x124]
	$+23B    005DB619     8D4D B0                 LEA ECX,DWORD PTR SS:[EBP-0x50]
	$+23E    005DB61C     51                      PUSH ECX
	$+23F    005DB61D     8D8F B0000000           LEA ECX,DWORD PTR DS:[EDI+0xB0]
	$+245    005DB623     51                      PUSH ECX
	$+246    005DB624     8D8F A4000000           LEA ECX,DWORD PTR DS:[EDI+0xA4]
	$+24C    005DB62A     51                      PUSH ECX
	$+24D    005DB62B     8D4D C8                 LEA ECX,DWORD PTR SS:[EBP-0x38]
	$+250    005DB62E     51                      PUSH ECX
	$+251    005DB62F     8D4D D4                 LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$+254    005DB632     51                      PUSH ECX
	$+255    005DB633     8BC8                    MOV ECX,EAX
	$+257    005DB635     FFD2                    CALL EDX
	$+259    005DB637     D95D FC                 FSTP DWORD PTR SS:[EBP-0x4]
	$+25C    005DB63A     F3:0F1046 78            MOVSS XMM0,DWORD PTR DS:[ESI+0x78]
	$+261    005DB63F     0F28C8                  MOVAPS XMM1,XMM0
	$+264    005DB642     F3:0F59C8               MULSS XMM1,XMM0
	$+268    005DB646     0F2F4D FC               COMISS XMM1,DWORD PTR SS:[EBP-0x4]
	$+26C    005DB64A     0F82 EE020000           JB MapleSto.005DB93E
	$+272    005DB650     8BCE                    MOV ECX,ESI
	$+274    005DB652     E8 0B817401             CALL MapleSto.01D23762
	$+279    005DB657     D95D 08                 FSTP DWORD PTR SS:[EBP+0x8]
	$+27C    005DB65A     8BCF                    MOV ECX,EDI
	$+27E    005DB65C     E8 F6006E01             CALL MapleSto.01CBB757
	$+283    005DB661     D945 08                 FLD DWORD PTR SS:[EBP+0x8]
	$+286    005DB664     DFF1                    FCOMIP ST,ST(1)
	$+288    005DB666     DDD8                    FSTP ST
	$+28A    005DB668     76 2A                   JBE SHORT MapleSto.005DB694
	$+28C    005DB66A     8BCE                    MOV ECX,ESI
	$+28E    005DB66C     E8 4D2A7401             CALL MapleSto.01D1E0BE
	$+293    005DB671     D95D 08                 FSTP DWORD PTR SS:[EBP+0x8]
	$+296    005DB674     8BCF                    MOV ECX,EDI
	$+298    005DB676     E8 80C86D01             CALL MapleSto.01CB7EFB
	$+29D    005DB67B     D945 08                 FLD DWORD PTR SS:[EBP+0x8]
	$+2A0    005DB67E     D9C9                    FXCH ST(1)
	$+2A2    005DB680     DFF1                    FCOMIP ST,ST(1)
	$+2A4    005DB682     DDD8                    FSTP ST
	$+2A6    005DB684     76 0E                   JBE SHORT MapleSto.005DB694
	$+2A8    005DB686     C645 0B 01              MOV BYTE PTR SS:[EBP+0xB],0x1
	$+2AC    005DB68A     0FB645 0B               MOVZX EAX,BYTE PTR SS:[EBP+0xB]
	$+2B0    005DB68E     5F                      POP EDI
	$+2B1    005DB68F     5E                      POP ESI
	$+2B2    005DB690     8BE5                    MOV ESP,EBP
	$+2B4    005DB692     5D                      POP EBP
	$+2B5    005DB693     C3                      RETN
	$+2B6    005DB694     C645 0B 00              MOV BYTE PTR SS:[EBP+0xB],0x0
	$+2BA    005DB698     0FB645 0B               MOVZX EAX,BYTE PTR SS:[EBP+0xB]
	$+2BE    005DB69C     5F                      POP EDI
	$+2BF    005DB69D     5E                      POP ESI
	$+2C0    005DB69E     8BE5                    MOV ESP,EBP
	$+2C2    005DB6A0     5D                      POP EBP
	$+2C3    005DB6A1     C3                      RETN
	$+2C4    005DB6A2     53                      PUSH EBX
	$+2C5    005DB6A3     83F8 04                 CMP EAX,0x4
	$+2C8    005DB6A6     0F85 D2010000           JNZ MapleSto.005DB87E
	$+2CE    005DB6AC     394F 34                 CMP DWORD PTR DS:[EDI+0x34],ECX
	$+2D1    005DB6AF     0F85 C9010000           JNZ MapleSto.005DB87E
	$+2D7    005DB6B5     8D87 98000000           LEA EAX,DWORD PTR DS:[EDI+0x98]
	$+2DD    005DB6BB     50                      PUSH EAX
	$+2DE    005DB6BC     8BCE                    MOV ECX,ESI
	$+2E0    005DB6BE     E8 7D200600             CALL MapleSto.0063D740
	$+2E5    005DB6C3     85C0                    TEST EAX,EAX
	$+2E7    005DB6C5     0F85 6E020000           JNZ MapleSto.005DB939
	$+2ED    005DB6CB     8D87 D4000000           LEA EAX,DWORD PTR DS:[EDI+0xD4]
	$+2F3    005DB6D1     33DB                    XOR EBX,EBX
	$+2F5    005DB6D3     8945 FC                 MOV DWORD PTR SS:[EBP-0x4],EAX
	$+2F8    005DB6D6     8B4D FC                 MOV ECX,DWORD PTR SS:[EBP-0x4]
	$+2FB    005DB6D9     51                      PUSH ECX
	$+2FC    005DB6DA     8BCE                    MOV ECX,ESI
	$+2FE    005DB6DC     E8 5F200600             CALL MapleSto.0063D740
	$+303    005DB6E1     85C0                    TEST EAX,EAX
	$+305    005DB6E3     0F85 50020000           JNZ MapleSto.005DB939
	$+30B    005DB6E9     8345 FC 0C              ADD DWORD PTR SS:[EBP-0x4],0xC
	$+30F    005DB6ED     43                      INC EBX
	$+310    005DB6EE     83FB 08                 CMP EBX,0x8
	$+313    005DB6F1   ^ 7C E3                   JL SHORT MapleSto.005DB6D6
	$+315    005DB6F3     8BCF                    MOV ECX,EDI
	$+317    005DB6F5     E8 5D006E01             CALL MapleSto.01CBB757
	$+31C    005DB6FA     D95D FC                 FSTP DWORD PTR SS:[EBP-0x4]
	$+31F    005DB6FD     8BCE                    MOV ECX,ESI
	$+321    005DB6FF     E8 AF017101             CALL MapleSto.01CEB8B3
	$+326    005DB704     D945 FC                 FLD DWORD PTR SS:[EBP-0x4]
	$+329    005DB707     D9C9                    FXCH ST(1)
	$+32B    005DB709     DFF1                    FCOMIP ST,ST(1)
	$+32D    005DB70B     DDD8                    FSTP ST
	$+32F    005DB70D     0F86 2A020000           JBE MapleSto.005DB93D
	$+335    005DB713     8BCF                    MOV ECX,EDI
	$+337    005DB715     E8 E1C76D01             CALL MapleSto.01CB7EFB
	$+33C    005DB71A     D95D FC                 FSTP DWORD PTR SS:[EBP-0x4]
	$+33F    005DB71D     8BCE                    MOV ECX,ESI
	$+341    005DB71F     E8 71AE7001             CALL MapleSto.01CE6595
	$+346    005DB724     D945 FC                 FLD DWORD PTR SS:[EBP-0x4]
	$+349    005DB727     DFF1                    FCOMIP ST,ST(1)
	$+34B    005DB729     DDD8                    FSTP ST
	$+34D    005DB72B     0F86 0C020000           JBE MapleSto.005DB93D
	$+353    005DB731     8D87 98000000           LEA EAX,DWORD PTR DS:[EDI+0x98]
	$+359    005DB737     50                      PUSH EAX
	$+35A    005DB738     8D55 C8                 LEA EDX,DWORD PTR SS:[EBP-0x38]
	$+35D    005DB73B     52                      PUSH EDX
	$+35E    005DB73C     E8 BFB45800             CALL MapleSto.00B66C00
	$+363    005DB741     F30F7E00                MOVQ XMM0,QWORD PTR DS:[EAX]
	$+367    005DB745     F30F7E8E 90000000       MOVQ XMM1,QWORD PTR DS:[ESI+0x90]
	$+36F    005DB74D     8B8E 98000000           MOV ECX,DWORD PTR DS:[ESI+0x98]
	$+375    005DB753     660FD645 D4             MOVQ QWORD PTR SS:[EBP-0x2C],XMM0
	$+37A    005DB758     8B40 08                 MOV EAX,DWORD PTR DS:[EAX+0x8]
	$+37D    005DB75B     0F57C0                  XORPS XMM0,XMM0
	$+380    005DB75E     660FD64D E0             MOVQ QWORD PTR SS:[EBP-0x20],XMM1
	$+385    005DB763     F3:0F104D D4            MOVSS XMM1,DWORD PTR SS:[EBP-0x2C]
	$+38A    005DB768     F3:0F5C4D E0            SUBSS XMM1,DWORD PTR SS:[EBP-0x20]
	$+38F    005DB76D     F3:0F114D C8            MOVSS DWORD PTR SS:[EBP-0x38],XMM1
	$+394    005DB772     F3:0F104D D8            MOVSS XMM1,DWORD PTR SS:[EBP-0x28]
	$+399    005DB777     F3:0F5C4D E4            SUBSS XMM1,DWORD PTR SS:[EBP-0x1C]
	$+39E    005DB77C     F3:0F1145 D0            MOVSS DWORD PTR SS:[EBP-0x30],XMM0
	$+3A3    005DB781     8B55 D0                 MOV EDX,DWORD PTR SS:[EBP-0x30]
	$+3A6    005DB784     8945 DC                 MOV DWORD PTR SS:[EBP-0x24],EAX
	$+3A9    005DB787     F3:0F1145 DC            MOVSS DWORD PTR SS:[EBP-0x24],XMM0
	$+3AE    005DB78C     894D E8                 MOV DWORD PTR SS:[EBP-0x18],ECX
	$+3B1    005DB78F     F3:0F114D CC            MOVSS DWORD PTR SS:[EBP-0x34],XMM1
	$+3B6    005DB794     F30F7E45 C8             MOVQ XMM0,QWORD PTR SS:[EBP-0x38]
	$+3BB    005DB799     83C4 08                 ADD ESP,0x8
	$+3BE    005DB79C     8D4D EC                 LEA ECX,DWORD PTR SS:[EBP-0x14]
	$+3C1    005DB79F     660FD645 EC             MOVQ QWORD PTR SS:[EBP-0x14],XMM0
	$+3C6    005DB7A4     8955 F4                 MOV DWORD PTR SS:[EBP-0xC],EDX
	$+3C9    005DB7A7     E8 441FFDFF             CALL MapleSto.005AD6F0
	$+3CE    005DB7AC     DDD8                    FSTP ST
	$+3D0    005DB7AE     F3:0F1096 88000000      MOVSS XMM2,DWORD PTR DS:[ESI+0x88]
	$+3D8    005DB7B6     F3:0F1045 EC            MOVSS XMM0,DWORD PTR SS:[EBP-0x14]
	$+3DD    005DB7BB     F3:0F104D F0            MOVSS XMM1,DWORD PTR SS:[EBP-0x10]
	$+3E2    005DB7C0     F3:0F105D F4            MOVSS XMM3,DWORD PTR SS:[EBP-0xC]
	$+3E7    005DB7C5     8D45 C8                 LEA EAX,DWORD PTR SS:[EBP-0x38]
	$+3EA    005DB7C8     50                      PUSH EAX
	$+3EB    005DB7C9     8D4D B0                 LEA ECX,DWORD PTR SS:[EBP-0x50]
	$+3EE    005DB7CC     F3:0F59C2               MULSS XMM0,XMM2
	$+3F2    005DB7D0     F3:0F5845 E0            ADDSS XMM0,DWORD PTR SS:[EBP-0x20]
	$+3F7    005DB7D5     F3:0F59CA               MULSS XMM1,XMM2
	$+3FB    005DB7D9     F3:0F584D E4            ADDSS XMM1,DWORD PTR SS:[EBP-0x1C]
	$+400    005DB7DE     F3:0F59DA               MULSS XMM3,XMM2
	$+404    005DB7E2     51                      PUSH ECX
	$+405    005DB7E3     F3:0F1145 C8            MOVSS DWORD PTR SS:[EBP-0x38],XMM0
	$+40A    005DB7E8     F3:0F114D CC            MOVSS DWORD PTR SS:[EBP-0x34],XMM1
	$+40F    005DB7ED     F3:0F115D D0            MOVSS DWORD PTR SS:[EBP-0x30],XMM3
	$+414    005DB7F2     E8 E9B35800             CALL MapleSto.00B66BE0
	$+419    005DB7F7     F3:0F108E 8C000000      MOVSS XMM1,DWORD PTR DS:[ESI+0x8C]
	$+421    005DB7FF     F3:0F1055 F0            MOVSS XMM2,DWORD PTR SS:[EBP-0x10]
	$+426    005DB804     F3:0F105D F4            MOVSS XMM3,DWORD PTR SS:[EBP-0xC]
	$+42B    005DB809     8D55 C8                 LEA EDX,DWORD PTR SS:[EBP-0x38]
	$+42E    005DB80C     0F28C1                  MOVAPS XMM0,XMM1
	$+431    005DB80F     F3:0F5945 EC            MULSS XMM0,DWORD PTR SS:[EBP-0x14]
	$+436    005DB814     F3:0F5845 E0            ADDSS XMM0,DWORD PTR SS:[EBP-0x20]
	$+43B    005DB819     52                      PUSH EDX
	$+43C    005DB81A     8D45 BC                 LEA EAX,DWORD PTR SS:[EBP-0x44]
	$+43F    005DB81D     F3:0F59D1               MULSS XMM2,XMM1
	$+443    005DB821     F3:0F5855 E4            ADDSS XMM2,DWORD PTR SS:[EBP-0x1C]
	$+448    005DB826     F3:0F59D9               MULSS XMM3,XMM1
	$+44C    005DB82A     50                      PUSH EAX
	$+44D    005DB82B     F3:0F1145 C8            MOVSS DWORD PTR SS:[EBP-0x38],XMM0
	$+452    005DB830     F3:0F1155 CC            MOVSS DWORD PTR SS:[EBP-0x34],XMM2
	$+457    005DB835     F3:0F115D D0            MOVSS DWORD PTR SS:[EBP-0x30],XMM3
	$+45C    005DB83A     E8 A1B35800             CALL MapleSto.00B66BE0
	$+461    005DB83F     83C4 10                 ADD ESP,0x10
	$+464    005DB842     8D8F B0000000           LEA ECX,DWORD PTR DS:[EDI+0xB0]
	$+46A    005DB848     51                      PUSH ECX
	$+46B    005DB849     81C7 A4000000           ADD EDI,0xA4
	$+471    005DB84F     57                      PUSH EDI
	$+472    005DB850     8D55 D4                 LEA EDX,DWORD PTR SS:[EBP-0x2C]
	$+475    005DB853     52                      PUSH EDX
	$+476    005DB854     8D45 C8                 LEA EAX,DWORD PTR SS:[EBP-0x38]
	$+479    005DB857     50                      PUSH EAX
	$+47A    005DB858     E8 83B35800             CALL MapleSto.00B66BE0
	$+47F    005DB85D     83C4 08                 ADD ESP,0x8
	$+482    005DB860     50                      PUSH EAX
	$+483    005DB861     8D4D BC                 LEA ECX,DWORD PTR SS:[EBP-0x44]
	$+486    005DB864     51                      PUSH ECX
	$+487    005DB865     8D55 B0                 LEA EDX,DWORD PTR SS:[EBP-0x50]
	$+48A    005DB868     52                      PUSH EDX
	$+48B    005DB869     E8 D21F0000             CALL MapleSto.005DD840
	$+490    005DB86E     83C4 14                 ADD ESP,0x14
	$+493    005DB871     5B                      POP EBX
	$+494    005DB872     5F                      POP EDI
	$+495    005DB873     8845 0B                 MOV BYTE PTR SS:[EBP+0xB],AL
	$+498    005DB876     0FB6C0                  MOVZX EAX,AL
	$+49B    005DB879     5E                      POP ESI
	$+49C    005DB87A     8BE5                    MOV ESP,EBP
	$+49E    005DB87C     5D                      POP EBP
	$+49F    005DB87D     C3                      RETN
	$+4A0    005DB87E     83F8 03                 CMP EAX,0x3
	$+4A3    005DB881     0F85 B6000000           JNZ MapleSto.005DB93D
	$+4A9    005DB887     394F 34                 CMP DWORD PTR DS:[EDI+0x34],ECX
	$+4AC    005DB88A     0F85 AD000000           JNZ MapleSto.005DB93D
	$+4B2    005DB890     8BCE                    MOV ECX,ESI
	$+4B4    005DB892     E8 7C177801             CALL MapleSto.01D5D013
	$+4B9    005DB897     D95D FC                 FSTP DWORD PTR SS:[EBP-0x4]
	$+4BC    005DB89A     8BCF                    MOV ECX,EDI
	$+4BE    005DB89C     E8 B6FE6D01             CALL MapleSto.01CBB757
	$+4C3    005DB8A1     D945 FC                 FLD DWORD PTR SS:[EBP-0x4]
	$+4C6    005DB8A4     DFF1                    FCOMIP ST,ST(1)
	$+4C8    005DB8A6     DDD8                    FSTP ST
	$+4CA    005DB8A8     0F86 8F000000           JBE MapleSto.005DB93D
	$+4D0    005DB8AE     8BCE                    MOV ECX,ESI
	$+4D2    005DB8B0     E8 7E707801             CALL MapleSto.01D62933
	$+4D7    005DB8B5     D95D FC                 FSTP DWORD PTR SS:[EBP-0x4]
	$+4DA    005DB8B8     8BCF                    MOV ECX,EDI
	$+4DC    005DB8BA     E8 3CC66D01             CALL MapleSto.01CB7EFB
	$+4E1    005DB8BF     D945 FC                 FLD DWORD PTR SS:[EBP-0x4]
	$+4E4    005DB8C2     D9C9                    FXCH ST(1)
	$+4E6    005DB8C4     DFF1                    FCOMIP ST,ST(1)
	$+4E8    005DB8C6     DDD8                    FSTP ST
	$+4EA    005DB8C8     76 73                   JBE SHORT MapleSto.005DB93D
	$+4EC    005DB8CA     8D87 98000000           LEA EAX,DWORD PTR DS:[EDI+0x98]
	$+4F2    005DB8D0     50                      PUSH EAX
	$+4F3    005DB8D1     8BCE                    MOV ECX,ESI
	$+4F5    005DB8D3     8945 F8                 MOV DWORD PTR SS:[EBP-0x8],EAX
	$+4F8    005DB8D6     E8 05502000             CALL MapleSto.007E08E0
	$+4FD    005DB8DB     85C0                    TEST EAX,EAX
	$+4FF    005DB8DD     75 5A                   JNZ SHORT MapleSto.005DB939
	$+501    005DB8DF     8D87 D4000000           LEA EAX,DWORD PTR DS:[EDI+0xD4]
	$+507    005DB8E5     33DB                    XOR EBX,EBX
	$+509    005DB8E7     8945 FC                 MOV DWORD PTR SS:[EBP-0x4],EAX
	$+50C    005DB8EA     8D9B 00000000           LEA EBX,DWORD PTR DS:[EBX]
	$+512    005DB8F0     8B4D FC                 MOV ECX,DWORD PTR SS:[EBP-0x4]
	$+515    005DB8F3     51                      PUSH ECX
	$+516    005DB8F4     8BCE                    MOV ECX,ESI
	$+518    005DB8F6     E8 E54F2000             CALL MapleSto.007E08E0
	$+51D    005DB8FB     85C0                    TEST EAX,EAX
	$+51F    005DB8FD     75 3A                   JNZ SHORT MapleSto.005DB939
	$+521    005DB8FF     8345 FC 0C              ADD DWORD PTR SS:[EBP-0x4],0xC
	$+525    005DB903     43                      INC EBX
	$+526    005DB904     83FB 08                 CMP EBX,0x8
	$+529    005DB907   ^ 7C E7                   JL SHORT MapleSto.005DB8F0
	$+52B    005DB909     8D56 6C                 LEA EDX,DWORD PTR DS:[ESI+0x6C]
	$+52E    005DB90C     33DB                    XOR EBX,EBX
	$+530    005DB90E     8955 FC                 MOV DWORD PTR SS:[EBP-0x4],EDX
	$+533    005DB911     8B45 FC                 MOV EAX,DWORD PTR SS:[EBP-0x4]
	$+536    005DB914     50                      PUSH EAX
	$+537    005DB915     8BCF                    MOV ECX,EDI
	$+539    005DB917     E8 B42B0100             CALL MapleSto.005EE4D0
	$+53E    005DB91C     85C0                    TEST EAX,EAX
	$+540    005DB91E     75 19                   JNZ SHORT MapleSto.005DB939
	$+542    005DB920     8345 FC 0C              ADD DWORD PTR SS:[EBP-0x4],0xC
	$+546    005DB924     43                      INC EBX
	$+547    005DB925     83FB 08                 CMP EBX,0x8
	$+54A    005DB928   ^ 7C E7                   JL SHORT MapleSto.005DB911
	$+54C    005DB92A     8B4D F8                 MOV ECX,DWORD PTR SS:[EBP-0x8]
	$+54F    005DB92D     51                      PUSH ECX
	$+550    005DB92E     8BCE                    MOV ECX,ESI
	$+552    005DB930     E8 1B4B2000             CALL MapleSto.007E0450
	$+557    005DB935     85C0                    TEST EAX,EAX
	$+559    005DB937     74 04                   JE SHORT MapleSto.005DB93D
	$+55B    005DB939     C645 0B 01              MOV BYTE PTR SS:[EBP+0xB],0x1
	$+55F    005DB93D     5B                      POP EBX
	$+560    005DB93E     0FB645 0B               MOVZX EAX,BYTE PTR SS:[EBP+0xB]
	$+564    005DB942     5F                      POP EDI
	$+565    005DB943     5E                      POP ESI
	$+566    005DB944     8BE5                    MOV ESP,EBP
	$+568    005DB946     5D                      POP EBP
	$+569    005DB947     C3                      RETN
	$+56A    005DB948     5F                      POP EDI
	$+56B    005DB949     33C0                    XOR EAX,EAX
	$+56D    005DB94B     5E                      POP ESI
	$+56E    005DB94C     8BE5                    MOV ESP,EBP
	$+570    005DB94E     5D                      POP EBP
	$+571    005DB94F     C3                      RETN
	$+572    005DB950     55                      PUSH EBP
	$+573    005DB951     8BEC                    MOV EBP,ESP
	$+575    005DB953     83EC 58                 SUB ESP,0x58
	$+578    005DB956     53                      PUSH EBX
	$+579    005DB957     56                      PUSH ESI
	$+57A    005DB958     8B75 2C                 MOV ESI,DWORD PTR SS:[EBP+0x2C]
	$+57D    005DB95B     8B46 34                 MOV EAX,DWORD PTR DS:[ESI+0x34]
	$+580    005DB95E     BB 01000000             MOV EBX,0x1
	$+585    005DB963     57                      PUSH EDI
	$+586    005DB964     8B7D 30                 MOV EDI,DWORD PTR SS:[EBP+0x30]
	$+589    005DB967     C645 FF 00              MOV BYTE PTR SS:[EBP-0x1],0x0
	$+58D    005DB96B     3BC3                    CMP EAX,EBX
	$+58F    005DB96D     0F85 68010000           JNZ MapleSto.005DBADB
	$+595    005DB973     395F 34                 CMP DWORD PTR DS:[EDI+0x34],EBX
	$+598    005DB976     0F85 5F010000           JNZ MapleSto.005DBADB
	$+59E    005DB97C     81C7 98000000           ADD EDI,0x98
	$+5A4    005DB982     81C6 98000000           ADD ESI,0x98
	$+5AA    005DB988     FF15 549A3E01           CALL DWORD PTR DS:[0x13E9A54]
	$+5B0    005DB98E     8B10                    MOV EDX,DWORD PTR DS:[EAX]
	$+5B2    005DB990     8B92 C8000000           MOV EDX,DWORD PTR DS:[EDX+0xC8]
	$+5B8    005DB996     53                      PUSH EBX
	$+5B9    005DB997     8D4F 18                 LEA ECX,DWORD PTR DS:[EDI+0x18]
	$+5BC    005DB99A     51                      PUSH ECX
	$+5BD    005DB99B     57                      PUSH EDI
	$+5BE    005DB99C     8D4F 0C                 LEA ECX,DWORD PTR DS:[EDI+0xC]
	$+5C1    005DB99F     51                      PUSH ECX
	$+5C2    005DB9A0     8D4E 18                 LEA ECX,DWORD PTR DS:[ESI+0x18]
	$+5C5    005DB9A3     51                      PUSH ECX
	$+5C6    005DB9A4     56                      PUSH ESI
	$+5C7    005DB9A5     83C6 0C                 ADD ESI,0xC
	$+5CA    005DB9A8     56                      PUSH ESI
	$+5CB    005DB9A9     8BC8                    MOV ECX,EAX
	$+5CD    005DB9AB     FFD2                    CALL EDX
	$+5CF    005DB9AD     8845 FF                 MOV BYTE PTR SS:[EBP-0x1],AL
	$+5D2    005DB9B0     84C0                    TEST AL,AL
	$+5D4    005DB9B2     0F84 43060000           JE MapleSto.005DBFFB
	$+5DA    005DB9B8     F3:0F1007               MOVSS XMM0,DWORD PTR DS:[EDI]
	$+5DE    005DB9BC     8B45 10                 MOV EAX,DWORD PTR SS:[EBP+0x10]
	$+5E1    005DB9BF     8B4D 28                 MOV ECX,DWORD PTR SS:[EBP+0x28]
	$+5E4    005DB9C2     F3:0F1145 E0            MOVSS DWORD PTR SS:[EBP-0x20],XMM0
	$+5E9    005DB9C7     F3:0F1047 04            MOVSS XMM0,DWORD PTR DS:[EDI+0x4]
	$+5EE    005DB9CC     F3:0F1145 E4            MOVSS DWORD PTR SS:[EBP-0x1C],XMM0
	$+5F3    005DB9D1     F3:0F1047 08            MOVSS XMM0,DWORD PTR DS:[EDI+0x8]
	$+5F8    005DB9D6     F3:0F1145 E8            MOVSS DWORD PTR SS:[EBP-0x18],XMM0
	$+5FD    005DB9DB     F30F7E45 08             MOVQ XMM0,QWORD PTR SS:[EBP+0x8]
	$+602    005DB9E0     8D55 C8                 LEA EDX,DWORD PTR SS:[EBP-0x38]
	$+605    005DB9E3     8945 D0                 MOV DWORD PTR SS:[EBP-0x30],EAX
	$+608    005DB9E6     52                      PUSH EDX
	$+609    005DB9E7     8D45 D4                 LEA EAX,DWORD PTR SS:[EBP-0x2C]
	$+60C    005DB9EA     660FD645 C8             MOVQ QWORD PTR SS:[EBP-0x38],XMM0
	$+611    005DB9EF     F30F7E45 20             MOVQ XMM0,QWORD PTR SS:[EBP+0x20]
	$+616    005DB9F4     894D DC                 MOV DWORD PTR SS:[EBP-0x24],ECX
	$+619    005DB9F7     50                      PUSH EAX
	$+61A    005DB9F8     8D4D A8                 LEA ECX,DWORD PTR SS:[EBP-0x58]
	$+61D    005DB9FB     660FD645 D4             MOVQ QWORD PTR SS:[EBP-0x2C],XMM0
	$+622    005DBA00     E8 6B4A6600             CALL MapleSto.00C40470
	$+627    005DBA05     8B4D 10                 MOV ECX,DWORD PTR SS:[EBP+0x10]
	$+62A    005DBA08     F30F7E45 08             MOVQ XMM0,QWORD PTR SS:[EBP+0x8]
	$+62F    005DBA0D     8B55 1C                 MOV EDX,DWORD PTR SS:[EBP+0x1C]
	$+632    005DBA10     894D D0                 MOV DWORD PTR SS:[EBP-0x30],ECX
	$+635    005DBA13     8D45 C8                 LEA EAX,DWORD PTR SS:[EBP-0x38]
	$+638    005DBA16     50                      PUSH EAX
	$+639    005DBA17     8D4D D4                 LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$+63C    005DBA1A     660FD645 C8             MOVQ QWORD PTR SS:[EBP-0x38],XMM0
	$+641    005DBA1F     F30F7E45 14             MOVQ XMM0,QWORD PTR SS:[EBP+0x14]
	$+646    005DBA24     51                      PUSH ECX
	$+647    005DBA25     8D4D B8                 LEA ECX,DWORD PTR SS:[EBP-0x48]
	$+64A    005DBA28     660FD645 D4             MOVQ QWORD PTR SS:[EBP-0x2C],XMM0
	$+64F    005DBA2D     8955 DC                 MOV DWORD PTR SS:[EBP-0x24],EDX
	$+652    005DBA30     E8 3B4A6600             CALL MapleSto.00C40470
	$+657    005DBA35     F3:0F105D E4            MOVSS XMM3,DWORD PTR SS:[EBP-0x1C]
	$+65C    005DBA3A     F3:0F1065 E8            MOVSS XMM4,DWORD PTR SS:[EBP-0x18]
	$+661    005DBA3F     F3:0F1045 AC            MOVSS XMM0,DWORD PTR SS:[EBP-0x54]
	$+666    005DBA44     F3:0F104D B0            MOVSS XMM1,DWORD PTR SS:[EBP-0x50]
	$+66B    005DBA49     F3:0F1055 E0            MOVSS XMM2,DWORD PTR SS:[EBP-0x20]
	$+670    005DBA4E     8B4D 34                 MOV ECX,DWORD PTR SS:[EBP+0x34]
	$+673    005DBA51     8B01                    MOV EAX,DWORD PTR DS:[ECX]
	$+675    005DBA53     F3:0F59CC               MULSS XMM1,XMM4
	*/
	addr_tmp = scan_exe_.ScanCall("e8????????85??7c??7f??85??77??6a??8b", 1);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = AsByte(addr_tmp, 0x20);
	if (dw_tmp != 0x55)
	{
		assert(false);
		return false;
	}
	dw_tmp = AsDword(addr_tmp, 0x26 + 3);
	assert(dw_tmp > 0x100 && dw_tmp < 0x200);
	ofs << "pt_cdword	g_o_role_max_attr_value_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	dw_tmp = AsDword(addr_tmp, 0x6 + 3);
	assert(dw_tmp > 0x300 && dw_tmp < 0x400);
	ofs << "pt_cdword	g_o_role_cut_attr_value_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	/*
	$-E2     0076DD4F      CC                     INT3
	$-E1     0076DD50      55                     PUSH EBP
	$-E0     0076DD51      8BEC                   MOV EBP,ESP
	$-DE     0076DD53      56                     PUSH ESI
	$-DD     0076DD54      57                     PUSH EDI
	$-DC     0076DD55      8B7D 08                MOV EDI,DWORD PTR SS:[EBP+0x8]
	$-D9     0076DD58      8BF1                   MOV ESI,ECX
	$-D7     0076DD5A      8B86 0C130000          MOV EAX,DWORD PTR DS:[ESI+0x130C]
	$-D1     0076DD60      6A FF                  PUSH -0x1
	$-CF     0076DD62      50                     PUSH EAX
	$-CE     0076DD63      8BCF                   MOV ECX,EDI
	$-CC     0076DD65      E8 16533700            CALL MapleSto.00AE3080
	$-C7     0076DD6A      84C0                   TEST AL,AL
	$-C5     0076DD6C      75 0A                  JNZ SHORT MapleSto.0076DD78
	$-C3     0076DD6E      68 0E000780            PUSH 0x8007000E
	$-BE     0076DD73      E8 A83DC9FF            CALL MapleSto.00401B20
	$-B9     0076DD78      8B8E 0C130000          MOV ECX,DWORD PTR DS:[ESI+0x130C]
	$-B3     0076DD7E      8B96 08130000          MOV EDX,DWORD PTR DS:[ESI+0x1308]
	$-AD     0076DD84      8B07                   MOV EAX,DWORD PTR DS:[EDI]
	$-AB     0076DD86      51                     PUSH ECX
	$-AA     0076DD87      52                     PUSH EDX
	$-A9     0076DD88      50                     PUSH EAX
	$-A8     0076DD89      E8 9215E2FF            CALL MapleSto.0058F320
	$-A3     0076DD8E      83C4 0C                ADD ESP,0xC
	$-A0     0076DD91      5F                     POP EDI
	$-9F     0076DD92      5E                     POP ESI
	$-9E     0076DD93      5D                     POP EBP
	$-9D     0076DD94      C2 0400                RETN 0x4
	$-9A     0076DD97      CC                     INT3
	$-99     0076DD98      CC                     INT3
	$-98     0076DD99      CC                     INT3
	$-97     0076DD9A      CC                     INT3
	$-96     0076DD9B      CC                     INT3
	$-95     0076DD9C      CC                     INT3
	$-94     0076DD9D      CC                     INT3
	$-93     0076DD9E      CC                     INT3
	$-92     0076DD9F      CC                     INT3
	$-91     0076DDA0      55                     PUSH EBP
	$-90     0076DDA1      8BEC                   MOV EBP,ESP
	$-8E     0076DDA3      56                     PUSH ESI
	$-8D     0076DDA4      57                     PUSH EDI
	$-8C     0076DDA5      8B7D 08                MOV EDI,DWORD PTR SS:[EBP+0x8]
	$-89     0076DDA8      8BF1                   MOV ESI,ECX
	$-87     0076DDAA      8B86 1C130000          MOV EAX,DWORD PTR DS:[ESI+0x131C]
	$-81     0076DDB0      6A FF                  PUSH -0x1
	$-7F     0076DDB2      50                     PUSH EAX
	$-7E     0076DDB3      8BCF                   MOV ECX,EDI
	$-7C     0076DDB5      E8 C6523700            CALL MapleSto.00AE3080
	$-77     0076DDBA      84C0                   TEST AL,AL
	$-75     0076DDBC      75 0A                  JNZ SHORT MapleSto.0076DDC8
	$-73     0076DDBE      68 0E000780            PUSH 0x8007000E
	$-6E     0076DDC3      E8 583DC9FF            CALL MapleSto.00401B20
	$-69     0076DDC8      8B8E 1C130000          MOV ECX,DWORD PTR DS:[ESI+0x131C]
	$-63     0076DDCE      8B96 18130000          MOV EDX,DWORD PTR DS:[ESI+0x1318]
	$-5D     0076DDD4      8B07                   MOV EAX,DWORD PTR DS:[EDI]
	$-5B     0076DDD6      51                     PUSH ECX
	$-5A     0076DDD7      52                     PUSH EDX
	$-59     0076DDD8      50                     PUSH EAX
	$-58     0076DDD9      E8 4215E2FF            CALL MapleSto.0058F320
	$-53     0076DDDE      83C4 0C                ADD ESP,0xC
	$-50     0076DDE1      5F                     POP EDI
	$-4F     0076DDE2      5E                     POP ESI
	$-4E     0076DDE3      5D                     POP EBP
	$-4D     0076DDE4      C2 0400                RETN 0x4
	$-4A     0076DDE7      CC                     INT3
	$-49     0076DDE8      CC                     INT3
	$-48     0076DDE9      CC                     INT3
	$-47     0076DDEA      CC                     INT3
	$-46     0076DDEB      CC                     INT3
	$-45     0076DDEC      CC                     INT3
	$-44     0076DDED      CC                     INT3
	$-43     0076DDEE      CC                     INT3
	$-42     0076DDEF      CC                     INT3
	$-41     0076DDF0      55                     PUSH EBP
	$-40     0076DDF1      8BEC                   MOV EBP,ESP
	$-3E     0076DDF3      6A FF                  PUSH -0x1
	$-3C     0076DDF5      68 C87D3301            PUSH MapleSto.01337DC8
	$-37     0076DDFA      64:A1 00000000         MOV EAX,DWORD PTR FS:[0]
	$-31     0076DE00      50                     PUSH EAX
	$-30     0076DE01      83EC 30                SUB ESP,0x30
	$-2D     0076DE04      53                     PUSH EBX
	$-2C     0076DE05      56                     PUSH ESI
	$-2B     0076DE06      57                     PUSH EDI
	$-2A     0076DE07      A1 D0687101            MOV EAX,DWORD PTR DS:[0x17168D0]
	$-25     0076DE0C      33C5                   XOR EAX,EBP
	$-23     0076DE0E      50                     PUSH EAX
	$-22     0076DE0F      8D45 F4                LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-1F     0076DE12      64:A3 00000000         MOV DWORD PTR FS:[0],EAX
	$-19     0076DE18      8BF1                   MOV ESI,ECX
	$-17     0076DE1A      D945 08                FLD DWORD PTR SS:[EBP+0x8]
	$-14     0076DE1D      51                     PUSH ECX
	$-13     0076DE1E      D91C24                 FSTP DWORD PTR SS:[ESP]
	$-10     0076DE21      E8 4A3D2100            CALL MapleSto.00981B70
	$-B      0076DE26      8B0D EC437201          MOV ECX,DWORD PTR DS:[0x17243EC]
	$-5      0076DE2C      E8 F3BC1700            CALL MapleSto.008E9B24
	$ ==>    0076DE31      D95D DC                FSTP DWORD PTR SS:[EBP-0x24]
	$+3      0076DE34      8B8E 28120000          MOV ECX,DWORD PTR DS:[ESI+0x1228]        ; g_o_get_tbl_item_game_obj_
	$+9      0076DE3A      E8 11D8DAFF            CALL MapleSto.0051B650
	$+E      0076DE3F      84C0                   TEST AL,AL
	$+10     0076DE41      74 6D                  JE SHORT MapleSto.0076DEB0
	$+12     0076DE43      83BE F4000000 00       CMP DWORD PTR DS:[ESI+0xF4],0x0
	$+19     0076DE4A      75 64                  JNZ SHORT MapleSto.0076DEB0
	$+1B     0076DE4C      8B86 28120000          MOV EAX,DWORD PTR DS:[ESI+0x1228]
	$+21     0076DE52      85C0                   TEST EAX,EAX
	$+23     0076DE54      74 5A                  JE SHORT MapleSto.0076DEB0
	$+25     0076DE56      F3:0F1086 98000000     MOVSS XMM0,DWORD PTR DS:[ESI+0x98]
	$+2D     0076DE5E      F3:0F1088 10010000     MOVSS XMM1,DWORD PTR DS:[EAX+0x110]
	$+35     0076DE66      0F2FC8                 COMISS XMM1,XMM0
	$+38     0076DE69      72 1F                  JB SHORT MapleSto.0076DE8A
	$+3A     0076DE6B      8B3D 2C417201          MOV EDI,DWORD PTR DS:[0x172412C]
	$+40     0076DE71      83EC 08                SUB ESP,0x8
	$+43     0076DE74      8BC4                   MOV EAX,ESP
	$+45     0076DE76      8965 E8                MOV DWORD PTR SS:[EBP-0x18],ESP
	$+48     0076DE79      50                     PUSH EAX
	$+49     0076DE7A      8BCE                   MOV ECX,ESI
	$+4B     0076DE7C      E8 4FB60700            CALL MapleSto.007E94D0
	$+50     0076DE81      8BCF                   MOV ECX,EDI
	$+52     0076DE83      E8 58634100            CALL MapleSto.00B841E0
	$+57     0076DE88      EB 26                  JMP SHORT MapleSto.0076DEB0
	$+59     0076DE8A      0F2F80 14010000        COMISS XMM0,DWORD PTR DS:[EAX+0x114]
	$+60     0076DE91      72 1D                  JB SHORT MapleSto.0076DEB0
	$+62     0076DE93      8B3D 2C417201          MOV EDI,DWORD PTR DS:[0x172412C]
	$+68     0076DE99      83EC 08                SUB ESP,0x8
	$+6B     0076DE9C      8BCC                   MOV ECX,ESP
	$+6D     0076DE9E      8965 E8                MOV DWORD PTR SS:[EBP-0x18],ESP
	$+70     0076DEA1      51                     PUSH ECX
	$+71     0076DEA2      8BCE                   MOV ECX,ESI
	$+73     0076DEA4      E8 27B60700            CALL MapleSto.007E94D0
	$+78     0076DEA9      8BCF                   MOV ECX,EDI
	$+7A     0076DEAB      E8 F05C4100            CALL MapleSto.00B83BA0
	$+7F     0076DEB0      83BE 2C120000 00       CMP DWORD PTR DS:[ESI+0x122C],0x0
	$+86     0076DEB7      0F85 F4000000          JNZ MapleSto.0076DFB1
	$+8C     0076DEBD      8DBE 68120000          LEA EDI,DWORD PTR DS:[ESI+0x1268]
	$+92     0076DEC3      8BCF                   MOV ECX,EDI
	$+94     0076DEC5      E8 96DC1D00            CALL MapleSto.0094BB60
	$+99     0076DECA      84C0                   TEST AL,AL
	$+9B     0076DECC      74 55                  JE SHORT MapleSto.0076DF23
	$+9D     0076DECE      8BCE                   MOV ECX,ESI
	$+9F     0076DED0      E8 90622000            CALL MapleSto.00974165
	$+A4     0076DED5      D95D F0                FSTP DWORD PTR SS:[EBP-0x10]
	$+A7     0076DED8      D945 08                FLD DWORD PTR SS:[EBP+0x8]
	$+AA     0076DEDB      8D55 F0                LEA EDX,DWORD PTR SS:[EBP-0x10]
	$+AD     0076DEDE      52                     PUSH EDX
	$+AE     0076DEDF      8D45 E0                LEA EAX,DWORD PTR SS:[EBP-0x20]
	$+B1     0076DEE2      50                     PUSH EAX
	$+B2     0076DEE3      51                     PUSH ECX
	$+B3     0076DEE4      8BCF                   MOV ECX,EDI
	$+B5     0076DEE6      D91C24                 FSTP DWORD PTR SS:[ESP]
	$+B8     0076DEE9      E8 12DE1D00            CALL MapleSto.0094BD00
	$+BD     0076DEEE      8B4D E8                MOV ECX,DWORD PTR SS:[EBP-0x18]
	$+C0     0076DEF1      F30F7E45 E0            MOVQ XMM0,QWORD PTR SS:[EBP-0x20]
	$+C5     0076DEF6      8B16                   MOV EDX,DWORD PTR DS:[ESI]
	$+C7     0076DEF8      8B52 4C                MOV EDX,DWORD PTR DS:[EDX+0x4C]
	$+CA     0076DEFB      83EC 0C                SUB ESP,0xC
	$+CD     0076DEFE      8BC4                   MOV EAX,ESP
	$+CF     0076DF00      660FD600               MOVQ QWORD PTR DS:[EAX],XMM0
	$+D3     0076DF04      8948 08                MOV DWORD PTR DS:[EAX+0x8],ECX
	$+D6     0076DF07      8BCE                   MOV ECX,ESI
	$+D8     0076DF09      FFD2                   CALL EDX
	$+DA     0076DF0B      D945 F0                FLD DWORD PTR SS:[EBP-0x10]
	$+DD     0076DF0E      8B06                   MOV EAX,DWORD PTR DS:[ESI]
	$+DF     0076DF10      8B90 FC000000          MOV EDX,DWORD PTR DS:[EAX+0xFC]
	$+E5     0076DF16      51                     PUSH ECX
	$+E6     0076DF17      8BCE                   MOV ECX,ESI
	$+E8     0076DF19      D91C24                 FSTP DWORD PTR SS:[ESP]
	$+EB     0076DF1C      FFD2                   CALL EDX
	$+ED     0076DF1E      E9 8E000000            JMP MapleSto.0076DFB1
	$+F2     0076DF23      F3:0F1045 08           MOVSS XMM0,DWORD PTR SS:[EBP+0x8]
	$+F7     0076DF28      F3:0F108E 18120000     MOVSS XMM1,DWORD PTR DS:[ESI+0x1218]
	$+FF     0076DF30      8B06                   MOV EAX,DWORD PTR DS:[ESI]
	$+101    0076DF32      8B50 48                MOV EDX,DWORD PTR DS:[EAX+0x48]
	$+104    0076DF35      F3:0F59C8              MULSS XMM1,XMM0
	$+108    0076DF39      F3:0F114D F0           MOVSS DWORD PTR SS:[EBP-0x10],XMM1
	$+10D    0076DF3E      F3:0F108E 1C120000     MOVSS XMM1,DWORD PTR DS:[ESI+0x121C]
	$+115    0076DF46      F3:0F59C8              MULSS XMM1,XMM0
	$+119    0076DF4A      F3:0F114D EC           MOVSS DWORD PTR SS:[EBP-0x14],XMM1
	$+11E    0076DF4F      F3:0F108E 20120000     MOVSS XMM1,DWORD PTR DS:[ESI+0x1220]
	$+126    0076DF57      8D4D C4                LEA ECX,DWORD PTR SS:[EBP-0x3C]
	$+129    0076DF5A      51                     PUSH ECX
	$+12A    0076DF5B      F3:0F59C8              MULSS XMM1,XMM0
	$+12E    0076DF5F      8BCE                   MOV ECX,ESI
	$+130    0076DF61      F3:0F114D E8           MOVSS DWORD PTR SS:[EBP-0x18],XMM1
	$+135    0076DF66      FFD2                   CALL EDX
	$+137    0076DF68      F3:0F1000              MOVSS XMM0,DWORD PTR DS:[EAX]
	$+13B    0076DF6C      F3:0F1048 04           MOVSS XMM1,DWORD PTR DS:[EAX+0x4]
	$+140    0076DF71      F3:0F1050 08           MOVSS XMM2,DWORD PTR DS:[EAX+0x8]
	$+145    0076DF76      F3:0F5845 F0           ADDSS XMM0,DWORD PTR SS:[EBP-0x10]
	$+14A    0076DF7B      F3:0F584D EC           ADDSS XMM1,DWORD PTR SS:[EBP-0x14]
	$+14F    0076DF80      F3:0F5855 E8           ADDSS XMM2,DWORD PTR SS:[EBP-0x18]
	$+154    0076DF85      8B16                   MOV EDX,DWORD PTR DS:[ESI]
	$+156    0076DF87      8B52 4C                MOV EDX,DWORD PTR DS:[EDX+0x4C]
	$+159    0076DF8A      83EC 0C                SUB ESP,0xC
	$+15C    0076DF8D      8BC4                   MOV EAX,ESP
	$+15E    0076DF8F      F3:0F1145 E0           MOVSS DWORD PTR SS:[EBP-0x20],XMM0
	$+163    0076DF94      F3:0F114D E4           MOVSS DWORD PTR SS:[EBP-0x1C],XMM1
	$+168    0076DF99      F30F7E45 E0            MOVQ XMM0,QWORD PTR SS:[EBP-0x20]
	$+16D    0076DF9E      F3:0F1155 E8           MOVSS DWORD PTR SS:[EBP-0x18],XMM2
	$+172    0076DFA3      8B4D E8                MOV ECX,DWORD PTR SS:[EBP-0x18]
	$+175    0076DFA6      660FD600               MOVQ QWORD PTR DS:[EAX],XMM0
	$+179    0076DFAA      8948 08                MOV DWORD PTR DS:[EAX+0x8],ECX
	$+17C    0076DFAD      8BCE                   MOV ECX,ESI
	$+17E    0076DFAF      FFD2                   CALL EDX
	$+180    0076DFB1      D945 08                FLD DWORD PTR SS:[EBP+0x8]
	$+183    0076DFB4      51                     PUSH ECX
	$+184    0076DFB5      8BCE                   MOV ECX,ESI
	$+186    0076DFB7      D91C24                 FSTP DWORD PTR SS:[ESP]
	$+189    0076DFBA      E8 8B8A5B01            CALL MapleSto.01D26A4A
	$+18E    0076DFBF      8BCE                   MOV ECX,ESI
	$+190    0076DFC1      E8 9AA2FFFF            CALL MapleSto.00768260
	$+195    0076DFC6      D945 08                FLD DWORD PTR SS:[EBP+0x8]
	$+198    0076DFC9      51                     PUSH ECX
	$+199    0076DFCA      8BCE                   MOV ECX,ESI
	$+19B    0076DFCC      D91C24                 FSTP DWORD PTR SS:[ESP]
	$+19E    0076DFCF      E8 8CC4FFFF            CALL MapleSto.0076A460
	$+1A3    0076DFD4      8B86 E0120000          MOV EAX,DWORD PTR DS:[ESI+0x12E0]
	$+1A9    0076DFDA      BB 01000000            MOV EBX,0x1
	$+1AE    0076DFDF      85C0                   TEST EAX,EAX
	$+1B0    0076DFE1      74 23                  JE SHORT MapleSto.0076E006
	$+1B2    0076DFE3      E8 C8DFDFFF            CALL MapleSto.0056BFB0                   ; JMP to winmm.timeGetTime
	$+1B7    0076DFE8      2B86 BC120000          SUB EAX,DWORD PTR DS:[ESI+0x12BC]
	$+1BD    0076DFEE      3D E8030000            CMP EAX,0x3E8
	$+1C2    0076DFF3      76 11                  JBE SHORT MapleSto.0076E006
	$+1C4    0076DFF5      E8 B6DFDFFF            CALL MapleSto.0056BFB0                   ; JMP to winmm.timeGetTime
	$+1C9    0076DFFA      8986 BC120000          MOV DWORD PTR DS:[ESI+0x12BC],EAX
	$+1CF    0076E000      899E B8120000          MOV DWORD PTR DS:[ESI+0x12B8],EBX
	$+1D5    0076E006      83BE B8120000 00       CMP DWORD PTR DS:[ESI+0x12B8],0x0
	$+1DC    0076E00D      74 07                  JE SHORT MapleSto.0076E016
	$+1DE    0076E00F      8BCE                   MOV ECX,ESI
	$+1E0    0076E011      E8 BAC5FFFF            CALL MapleSto.0076A5D0
	$+1E5    0076E016      83BE 78100000 00       CMP DWORD PTR DS:[ESI+0x1078],0x0
	$+1EC    0076E01D      0F84 BC000000          JE MapleSto.0076E0DF
	$+1F2    0076E023      8B06                   MOV EAX,DWORD PTR DS:[ESI]
	$+1F4    0076E025      8B90 C4010000          MOV EDX,DWORD PTR DS:[EAX+0x1C4]
	$+1FA    0076E02B      8BCE                   MOV ECX,ESI
	$+1FC    0076E02D      FFD2                   CALL EDX
	$+1FE    0076E02F      83F8 02                CMP EAX,0x2
	$+201    0076E032      0F84 5F010000          JE MapleSto.0076E197
	$+207    0076E038      8B0D 2C417201          MOV ECX,DWORD PTR DS:[0x172412C]
	$+20D    0076E03E      E8 90597301            CALL MapleSto.01EA39D3
	$+212    0076E043      85C0                   TEST EAX,EAX
	$+214    0076E045      0F84 4C010000          JE MapleSto.0076E197
	$+21A    0076E04B      8B16                   MOV EDX,DWORD PTR DS:[ESI]
	$+21C    0076E04D      50                     PUSH EAX
	$+21D    0076E04E      8B82 DC010000          MOV EAX,DWORD PTR DS:[EDX+0x1DC]
	$+223    0076E054      8BCE                   MOV ECX,ESI
	$+225    0076E056      FFD0                   CALL EAX
	$+227    0076E058      85C0                   TEST EAX,EAX
	$+229    0076E05A      74 3E                  JE SHORT MapleSto.0076E09A
	$+22B    0076E05C      8D4D E4                LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$+22E    0076E05F      51                     PUSH ECX
	$+22F    0076E060      8BCE                   MOV ECX,ESI
	$+231    0076E062      E8 8EECFFFF            CALL MapleSto.0076CCF5
	$+236    0076E067      8B40 04                MOV EAX,DWORD PTR DS:[EAX+0x4]
	$+239    0076E06A      8BBE 78100000          MOV EDI,DWORD PTR DS:[ESI+0x1078]
	$+23F    0076E070      8BC8                   MOV ECX,EAX
	$+241    0076E072      C745 FC 00000000       MOV DWORD PTR SS:[EBP-0x4],0x0
	$+248    0076E079      E8 86314D01            CALL MapleSto.01C41204
	$+24D    0076E07E      50                     PUSH EAX
	$+24E    0076E07F      8BCF                   MOV ECX,EDI
	$+250    0076E081      E8 1A9B3800            CALL MapleSto.00AF7BA0
	$+255    0076E086      8D4D E4                LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$+258    0076E089      C745 FC FFFFFFFF       MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+25F    0076E090      E8 7BF03000            CALL MapleSto.00A7D110
	$+264    0076E095      E9 FD000000            JMP MapleSto.0076E197
	$+269    0076E09A      841D E0AA7201          TEST BYTE PTR DS:[0x172AAE0],BL
	$+26F    0076E0A0      75 26                  JNZ SHORT MapleSto.0076E0C8
	$+271    0076E0A2      091D E0AA7201          OR DWORD PTR DS:[0x172AAE0],EBX
	$+277    0076E0A8      8B0D 40277201          MOV ECX,DWORD PTR DS:[0x1722740]
	$+27D    0076E0AE      81C1 F4600000          ADD ECX,0x60F4
	$+283    0076E0B4      895D FC                MOV DWORD PTR SS:[EBP-0x4],EBX
	$+286    0076E0B7      E8 E42DDBFF            CALL MapleSto.00520EA0
	$+28B    0076E0BC      A3 DCAA7201            MOV DWORD PTR DS:[0x172AADC],EAX
	$+290    0076E0C1      C745 FC FFFFFFFF       MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+297    0076E0C8      8B15 DCAA7201          MOV EDX,DWORD PTR DS:[0x172AADC]
	$+29D    0076E0CE      8B8E 78100000          MOV ECX,DWORD PTR DS:[ESI+0x1078]
	$+2A3    0076E0D4      52                     PUSH EDX
	$+2A4    0076E0D5      E8 C69A3800            CALL MapleSto.00AF7BA0
	$+2A9    0076E0DA      E9 B8000000            JMP MapleSto.0076E197
	$+2AE    0076E0DF      A1 40277201            MOV EAX,DWORD PTR DS:[0x1722740]
	$+2B3    0076E0E4      8078 08 00             CMP BYTE PTR DS:[EAX+0x8],0x0
	$+2B7    0076E0E8      0F84 A9000000          JE MapleSto.0076E197
	$+2BD    0076E0EE      8B4E 50                MOV ECX,DWORD PTR DS:[ESI+0x50]
	$+2C0    0076E0F1      51                     PUSH ECX
	$+2C1    0076E0F2      8D55 E4                LEA EDX,DWORD PTR SS:[EBP-0x1C]
	$+2C4    0076E0F5      52                     PUSH EDX
	$+2C5    0076E0F6      E8 759F3800            CALL MapleSto.00AF8070
	$+2CA    0076E0FB      83C4 08                ADD ESP,0x8
	$+2CD    0076E0FE      50                     PUSH EAX
	$+2CE    0076E0FF      8D8E 74100000          LEA ECX,DWORD PTR DS:[ESI+0x1074]
	$+2D4    0076E105      C745 FC 02000000       MOV DWORD PTR SS:[EBP-0x4],0x2
	$+2DB    0076E10C      E8 0FF5DEFF            CALL MapleSto.0055D620
	$+2E0    0076E111      8B78 04                MOV EDI,DWORD PTR DS:[EAX+0x4]
	$+2E3    0076E114      83CB FF                OR EBX,0xFFFFFFFF
	$+2E6    0076E117      8D4D E4                LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$+2E9    0076E11A      895D FC                MOV DWORD PTR SS:[EBP-0x4],EBX
	$+2EC    0076E11D      E8 EEEF3000            CALL MapleSto.00A7D110
	$+2F1    0076E122      85FF                   TEST EDI,EDI
	$+2F3    0076E124      74 71                  JE SHORT MapleSto.0076E197
	$+2F5    0076E126      8D45 E4                LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$+2F8    0076E129      50                     PUSH EAX
	$+2F9    0076E12A      8BCE                   MOV ECX,ESI
	$+2FB    0076E12C      E8 C4EBFFFF            CALL MapleSto.0076CCF5
	$+300    0076E131      8B40 04                MOV EAX,DWORD PTR DS:[EAX+0x4]
	$+303    0076E134      8B50 64                MOV EDX,DWORD PTR DS:[EAX+0x64]
	$+306    0076E137      8B8E 78100000          MOV ECX,DWORD PTR DS:[ESI+0x1078]
	$+30C    0076E13D      52                     PUSH EDX
	$+30D    0076E13E      C745 FC 03000000       MOV DWORD PTR SS:[EBP-0x4],0x3
	$+314    0076E145      E8 369A3800            CALL MapleSto.00AF7B80
	$+319    0076E14A      8D4D E4                LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$+31C    0076E14D      895D FC                MOV DWORD PTR SS:[EBP-0x4],EBX
	$+31F    0076E150      E8 BBEF3000            CALL MapleSto.00A7D110
	$+324    0076E155      8D45 E4                LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$+327    0076E158      50                     PUSH EAX
	$+328    0076E159      8BCE                   MOV ECX,ESI
	$+32A    0076E15B      E8 95EBFFFF            CALL MapleSto.0076CCF5
	$+32F    0076E160      8B40 04                MOV EAX,DWORD PTR DS:[EAX+0x4]
	$+332    0076E163      8BBE 78100000          MOV EDI,DWORD PTR DS:[ESI+0x1078]
	$+338    0076E169      8BC8                   MOV ECX,EAX
	$+33A    0076E16B      C745 FC 04000000       MOV DWORD PTR SS:[EBP-0x4],0x4
	$+341    0076E172      E8 8D304D01            CALL MapleSto.01C41204
	$+346    0076E177      50                     PUSH EAX
	$+347    0076E178      8BCF                   MOV ECX,EDI
	$+349    0076E17A      E8 219A3800            CALL MapleSto.00AF7BA0
	$+34E    0076E17F      8D4D E4                LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$+351    0076E182      895D FC                MOV DWORD PTR SS:[EBP-0x4],EBX
	$+354    0076E185      E8 86EF3000            CALL MapleSto.00A7D110
	$+359    0076E18A      8B8E 78100000          MOV ECX,DWORD PTR DS:[ESI+0x1078]
	$+35F    0076E190      8B11                   MOV EDX,DWORD PTR DS:[ECX]
	$+361    0076E192      8B42 04                MOV EAX,DWORD PTR DS:[EDX+0x4]
	$+364    0076E195      FFD0                   CALL EAX
	$+366    0076E197      83BE D8120000 00       CMP DWORD PTR DS:[ESI+0x12D8],0x0
	$+36D    0076E19E      74 50                  JE SHORT MapleSto.0076E1F0
	$+36F    0076E1A0      8B16                   MOV EDX,DWORD PTR DS:[ESI]
	$+371    0076E1A2      8B52 48                MOV EDX,DWORD PTR DS:[EDX+0x48]
	$+374    0076E1A5      8D45 D0                LEA EAX,DWORD PTR SS:[EBP-0x30]
	$+377    0076E1A8      50                     PUSH EAX
	$+378    0076E1A9      8BCE                   MOV ECX,ESI
	$+37A    0076E1AB      FFD2                   CALL EDX
	$+37C    0076E1AD      D945 DC                FLD DWORD PTR SS:[EBP-0x24]
	$+37F    0076E1B0      56                     PUSH ESI
	$+380    0076E1B1      8D45 D0                LEA EAX,DWORD PTR SS:[EBP-0x30]
	$+383    0076E1B4      50                     PUSH EAX
	$+384    0076E1B5      51                     PUSH ECX
	$+385    0076E1B6      8D8E C0120000          LEA ECX,DWORD PTR DS:[ESI+0x12C0]
	$+38B    0076E1BC      D91C24                 FSTP DWORD PTR SS:[ESP]
	$+38E    0076E1BF      E8 9C9CFFFF            CALL MapleSto.00767E60
	$+393    0076E1C4      8B76 60                MOV ESI,DWORD PTR DS:[ESI+0x60]
	$+396    0076E1C7      85F6                   TEST ESI,ESI
	$+398    0076E1C9      74 25                  JE SHORT MapleSto.0076E1F0
	$+39A    0076E1CB      8D4D D0                LEA ECX,DWORD PTR SS:[EBP-0x30]
	$+39D    0076E1CE      51                     PUSH ECX
	$+39E    0076E1CF      68 52CA95FD            PUSH 0xFD95CA52
	$+3A3    0076E1D4      8BCE                   MOV ECX,ESI
	*/
	addr_tmp = scan_exe_.ScanAddr("D95DDC8B8E????0000E8????????84C0", 0);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = AsDword(addr_tmp, 3 + 2);
	assert(dw_tmp > 0x1100 && dw_tmp < 0x1400);
	ofs << "pt_cdword	g_o_get_tbl_item_game_obj_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;
	addr_tmp1 = Cmn_FeatureCodeScan::CallAddr2AbsoluteAddr((pt_pcbyte)addr_tmp + 9);
	if (!addr_tmp1)
	{
		assert(false);
		return false;
	}
	dw_tmp = AsByte(addr_tmp1, 2);
	assert(dw_tmp > 0x30 && dw_tmp < 0x70);
	ofs << "pt_cdword	g_o_tbl_item_game_obj_friendly_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;
	dw_tmp = AsDword(addr_tmp1, 6 + 2);
	assert(dw_tmp > 0x70 && dw_tmp < 0xf0);
	ofs << "pt_cdword	g_o_tbl_item_game_obj_type_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	/*
	$-188    007AA4E1         8BEC                      MOV EBP,ESP
	$-186    007AA4E3         6A FF                     PUSH -0x1
	$-184    007AA4E5         68 57C33301               PUSH MapleSto.0133C357
	$-17F    007AA4EA         64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$-179    007AA4F0         50                        PUSH EAX
	$-178    007AA4F1         83EC 20                   SUB ESP,0x20
	$-175    007AA4F4         56                        PUSH ESI
	$-174    007AA4F5         A1 D0687101               MOV EAX,DWORD PTR DS:[0x17168D0]
	$-16F    007AA4FA         33C5                      XOR EAX,EBP
	$-16D    007AA4FC         50                        PUSH EAX
	$-16C    007AA4FD         8D45 F4                   LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-169    007AA500         64:A3 00000000            MOV DWORD PTR FS:[0],EAX
	$-163    007AA506         8BF1                      MOV ESI,ECX
	$-161    007AA508         C786 70020000 01000000    MOV DWORD PTR DS:[ESI+0x270],0x1
	$-157    007AA512         E8 991ADCFF               CALL MapleSto.0056BFB0                              ; JMP to winmm.timeGetTime
	$-152    007AA517         6A 00                     PUSH 0x0
	$-150    007AA519         6A 15                     PUSH 0x15
	$-14E    007AA51B         8D4D D4                   LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$-14B    007AA51E         8986 48020000             MOV DWORD PTR DS:[ESI+0x248],EAX
	$-145    007AA524         E8 E70CD8FF               CALL MapleSto.0052B210
	$-140    007AA529         8BCE                      MOV ECX,ESI
	$-13E    007AA52B         C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$-137    007AA532         E8 69EA0300               CALL MapleSto.007E8FA0
	$-132    007AA537         6A 04                     PUSH 0x4
	$-130    007AA539         8D4D D4                   LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$-12D    007AA53C         8BF0                      MOV ESI,EAX
	$-12B    007AA53E         E8 B2F8D0FF               CALL MapleSto.004B9DF5
	$-126    007AA543         837D E0 00                CMP DWORD PTR SS:[EBP-0x20],0x0
	$-122    007AA547         77 0A                     JA SHORT MapleSto.007AA553
	$-120    007AA549         68 57000780               PUSH 0x80070057
	$-11B    007AA54E         E8 CD75C5FF               CALL MapleSto.00401B20
	$-116    007AA553         8B45 EC                   MOV EAX,DWORD PTR SS:[EBP-0x14]
	$-113    007AA556         8B4D DC                   MOV ECX,DWORD PTR SS:[EBP-0x24]
	$-110    007AA559         8B55 08                   MOV EDX,DWORD PTR SS:[EBP+0x8]
	$-10D    007AA55C         893401                    MOV DWORD PTR DS:[ECX+EAX],ESI
	$-10A    007AA55F         8345 EC 04                ADD DWORD PTR SS:[EBP-0x14],0x4
	$-106    007AA563         52                        PUSH EDX
	$-105    007AA564         8D4D D4                   LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$-102    007AA567         E8 F408D8FF               CALL MapleSto.0052AE60
	$-FD     007AA56C         8D4D D4                   LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$-FA     007AA56F         E8 7DA94A01               CALL MapleSto.01C54EF1
	$-F5     007AA574         8B45 DC                   MOV EAX,DWORD PTR SS:[EBP-0x24]
	$-F2     007AA577         C745 D4 BC504101          MOV DWORD PTR SS:[EBP-0x2C],MapleSto.014150BC
	$-EB     007AA57E         85C0                      TEST EAX,EAX
	$-E9     007AA580         74 0A                     JE SHORT MapleSto.007AA58C
	$-E7     007AA582         50                        PUSH EAX
	$-E6     007AA583         FF15 E8983E01             CALL DWORD PTR DS:[0x13E98E8]                       ; msvcr90.free
	$-E0     007AA589         83C4 04                   ADD ESP,0x4
	$-DD     007AA58C         8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-DA     007AA58F         64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$-D3     007AA596         59                        POP ECX
	$-D2     007AA597         5E                        POP ESI
	$-D1     007AA598         8BE5                      MOV ESP,EBP
	$-CF     007AA59A         5D                        POP EBP
	$-CE     007AA59B         C2 0400                   RETN 0x4
	$-CB     007AA59E         CC                        INT3
	$-CA     007AA59F         CC                        INT3
	$-C9     007AA5A0         55                        PUSH EBP
	$-C8     007AA5A1         8BEC                      MOV EBP,ESP
	$-C6     007AA5A3         6A FF                     PUSH -0x1
	$-C4     007AA5A5         68 B2C33301               PUSH MapleSto.0133C3B2
	$-BF     007AA5AA         64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$-B9     007AA5B0         50                        PUSH EAX
	$-B8     007AA5B1         51                        PUSH ECX
	$-B7     007AA5B2         56                        PUSH ESI
	$-B6     007AA5B3         A1 D0687101               MOV EAX,DWORD PTR DS:[0x17168D0]
	$-B1     007AA5B8         33C5                      XOR EAX,EBP
	$-AF     007AA5BA         50                        PUSH EAX
	$-AE     007AA5BB         8D45 F4                   LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-AB     007AA5BE         64:A3 00000000            MOV DWORD PTR FS:[0],EAX
	$-A5     007AA5C4         8BF1                      MOV ESI,ECX
	$-A3     007AA5C6         8975 F0                   MOV DWORD PTR SS:[EBP-0x10],ESI
	$-A0     007AA5C9         C706 34644401             MOV DWORD PTR DS:[ESI],MapleSto.01446434
	$-9A     007AA5CF         C746 04 14644401          MOV DWORD PTR DS:[ESI+0x4],MapleSto.01446414
	$-93     007AA5D6         8B86 24010000             MOV EAX,DWORD PTR DS:[ESI+0x124]
	$-8D     007AA5DC         8B0D 44417201             MOV ECX,DWORD PTR DS:[0x1724144]
	$-87     007AA5E2         50                        PUSH EAX
	$-86     007AA5E3         C745 FC 04000000          MOV DWORD PTR SS:[EBP-0x4],0x4
	$-7F     007AA5EA         E8 C7725A01               CALL MapleSto.01D518B6
	$-7A     007AA5EF         8B8E 28010000             MOV ECX,DWORD PTR DS:[ESI+0x128]
	$-74     007AA5F5         51                        PUSH ECX
	$-73     007AA5F6         8B0D 44417201             MOV ECX,DWORD PTR DS:[0x1724144]
	$-6D     007AA5FC         E8 B5725A01               CALL MapleSto.01D518B6
	$-68     007AA601         8D8E 44020000             LEA ECX,DWORD PTR DS:[ESI+0x244]
	$-62     007AA607         FF15 08A23E01             CALL DWORD PTR DS:[0x13EA208]                       ; mfc90.#601
	$-5C     007AA60D         8D8E 40020000             LEA ECX,DWORD PTR DS:[ESI+0x240]
	$-56     007AA613         FF15 08A23E01             CALL DWORD PTR DS:[0x13EA208]                       ; mfc90.#601
	$-50     007AA619         8D8E 3C020000             LEA ECX,DWORD PTR DS:[ESI+0x23C]
	$-4A     007AA61F         FF15 08A23E01             CALL DWORD PTR DS:[0x13EA208]                       ; mfc90.#601
	$-44     007AA625         8D8E 38020000             LEA ECX,DWORD PTR DS:[ESI+0x238]
	$-3E     007AA62B         FF15 08A23E01             CALL DWORD PTR DS:[0x13EA208]                       ; mfc90.#601
	$-38     007AA631         8D8E 34020000             LEA ECX,DWORD PTR DS:[ESI+0x234]
	$-32     007AA637         FF15 08A23E01             CALL DWORD PTR DS:[0x13EA208]                       ; mfc90.#601
	$-2C     007AA63D         C786 C8010000 EC2D3F01    MOV DWORD PTR DS:[ESI+0x1C8],MapleSto.013F2DEC
	$-22     007AA647         8B86 90010000             MOV EAX,DWORD PTR DS:[ESI+0x190]
	$-1C     007AA64D         8D96 8C010000             LEA EDX,DWORD PTR DS:[ESI+0x18C]
	$-16     007AA653         C645 FC 02                MOV BYTE PTR SS:[EBP-0x4],0x2
	$-12     007AA657         3BC2                      CMP EAX,EDX
	$-10     007AA659         74 24                     JE SHORT MapleSto.007AA67F
	$-E      007AA65B         8B8E 7C010000             MOV ECX,DWORD PTR DS:[ESI+0x17C]
	$-8      007AA661         85C9                      TEST ECX,ECX
	$-6      007AA663         74 1A                     JE SHORT MapleSto.007AA67F
	$-4      007AA665         2BC1                      SUB EAX,ECX
	$-2      007AA667         50                        PUSH EAX
	$-1      007AA668         51                        PUSH ECX
	$ ==>    007AA669         C605 40267401 01          MOV BYTE PTR DS:[0x1742640],0x1
	$+7      007AA670         E8 FBC96200               CALL MapleSto.00DD7070
	$+C      007AA675         83C4 08                   ADD ESP,0x8
	$+F      007AA678         C605 40267401 00          MOV BYTE PTR DS:[0x1742640],0x0
	$+16     007AA67F         8B86 34010000             MOV EAX,DWORD PTR DS:[ESI+0x134]                    ; g_o_drop_item_obj2item_
	$+1C     007AA685         C645 FC 01                MOV BYTE PTR SS:[EBP-0x4],0x1
	$+20     007AA689         85C0                      TEST EAX,EAX
	$+22     007AA68B         74 3A                     JE SHORT MapleSto.007AA6C7
	$+24     007AA68D         83C0 04                   ADD EAX,0x4
	$+27     007AA690         50                        PUSH EAX
	$+28     007AA691         FF15 6C933E01             CALL DWORD PTR DS:[0x13E936C]
	$+2E     007AA697         85C0                      TEST EAX,EAX
	$+30     007AA699         75 22                     JNZ SHORT MapleSto.007AA6BD
	$+32     007AA69B         8B86 34010000             MOV EAX,DWORD PTR DS:[ESI+0x134]                    ; g_o_drop_item_obj2item_
	$+38     007AA6A1         83C0 04                   ADD EAX,0x4
	$+3B     007AA6A4         50                        PUSH EAX
	$+3C     007AA6A5         FF15 F8923E01             CALL DWORD PTR DS:[0x13E92F8]
	$+42     007AA6AB         8B8E 34010000             MOV ECX,DWORD PTR DS:[ESI+0x134]                    ; g_o_drop_item_obj2item_
	$+48     007AA6B1         85C9                      TEST ECX,ECX
	$+4A     007AA6B3         74 08                     JE SHORT MapleSto.007AA6BD
	$+4C     007AA6B5         8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$+4E     007AA6B7         8B10                      MOV EDX,DWORD PTR DS:[EAX]
	$+50     007AA6B9         6A 01                     PUSH 0x1
	$+52     007AA6BB         FFD2                      CALL EDX
	$+54     007AA6BD         C786 34010000 00000000    MOV DWORD PTR DS:[ESI+0x134],0x0                    ; g_o_drop_item_obj2item_
	$+5E     007AA6C7         8D8E 00010000             LEA ECX,DWORD PTR DS:[ESI+0x100]
	$+64     007AA6CD         E8 CE8D3000               CALL MapleSto.00AB34A0
	$+69     007AA6D2         8BCE                      MOV ECX,ESI
	$+6B     007AA6D4         C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+72     007AA6DB         E8 40030400               CALL MapleSto.007EAA20
	$+77     007AA6E0         8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+7A     007AA6E3         64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+81     007AA6EA         59                        POP ECX
	$+82     007AA6EB         5E                        POP ESI
	$+83     007AA6EC         8BE5                      MOV ESP,EBP
	$+85     007AA6EE         5D                        POP EBP
	$+86     007AA6EF         C3                        RETN
	*/
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("c6????????????e8????????83????c6????????????8b??????????c6??????85??74??83????50ff??????????85??75??8b??????????83????50ff",
		0x16 + 2);
	if (!op_dw_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *op_dw_tmp;
	assert(dw_tmp > 0x100 && dw_tmp < 0x155);
	ofs << "pt_cdword	g_o_drop_item_obj2item_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	/*
	$-4F1    0097C4BF     CC                         INT3
	$-4F0    0097C4C0     55                         PUSH EBP
	$-4EF    0097C4C1     8BEC                       MOV EBP,ESP
	$-4ED    0097C4C3     6A FF                      PUSH -0x1
	$-4EB    0097C4C5     68 C6963601                PUSH MapleSto.013696C6
	$-4E6    0097C4CA     64:A1 00000000             MOV EAX,DWORD PTR FS:[0]
	$-4E0    0097C4D0     50                         PUSH EAX
	$-4DF    0097C4D1     83EC 70                    SUB ESP,0x70
	$-4DC    0097C4D4     53                         PUSH EBX
	$-4DB    0097C4D5     56                         PUSH ESI
	$-4DA    0097C4D6     57                         PUSH EDI
	$-4D9    0097C4D7     A1 D0687101                MOV EAX,DWORD PTR DS:[0x17168D0]
	$-4D4    0097C4DC     33C5                       XOR EAX,EBP
	$-4D2    0097C4DE     50                         PUSH EAX
	$-4D1    0097C4DF     8D45 F4                    LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-4CE    0097C4E2     64:A3 00000000             MOV DWORD PTR FS:[0],EAX
	$-4C8    0097C4E8     8BF1                       MOV ESI,ECX
	$-4C6    0097C4EA     F3:0F1045 08               MOVSS XMM0,DWORD PTR SS:[EBP+0x8]
	$-4C1    0097C4EF     F3:0F1186 30110000         MOVSS DWORD PTR DS:[ESI+0x1130],XMM0
	$-4B9    0097C4F7     F3:0F1045 0C               MOVSS XMM0,DWORD PTR SS:[EBP+0xC]
	$-4B4    0097C4FC     F3:0F1186 34110000         MOVSS DWORD PTR DS:[ESI+0x1134],XMM0
	$-4AC    0097C504     F3:0F1045 10               MOVSS XMM0,DWORD PTR SS:[EBP+0x10]
	$-4A7    0097C509     33DB                       XOR EBX,EBX
	$-4A5    0097C50B     F3:0F1186 38110000         MOVSS DWORD PTR DS:[ESI+0x1138],XMM0
	$-49D    0097C513     899E F0110000              MOV DWORD PTR DS:[ESI+0x11F0],EBX
	$-497    0097C519     399E E8000000              CMP DWORD PTR DS:[ESI+0xE8],EBX
	$-491    0097C51F     0F84 FD080000              JE MapleSto.0097CE22
	$-48B    0097C525     8D8E 08060000              LEA ECX,DWORD PTR DS:[ESI+0x608]
	$-485    0097C52B     E8 6579C3FF                CALL MapleSto.005B3E95
	$-480    0097C530     DD45 14                    FLD QWORD PTR SS:[EBP+0x14]
	$-47D    0097C533     0945 20                    OR DWORD PTR SS:[EBP+0x20],EAX
	$-47A    0097C536     D955 18                    FST DWORD PTR SS:[EBP+0x18]
	$-477    0097C539     8D45 F0                    LEA EAX,DWORD PTR SS:[EBP-0x10]
	$-474    0097C53C     50                         PUSH EAX
	$-473    0097C53D     51                         PUSH ECX
	$-472    0097C53E     D91C24                     FSTP DWORD PTR SS:[ESP]
	$-46F    0097C541     56                         PUSH ESI
	$-46E    0097C542     8D4D A8                    LEA ECX,DWORD PTR SS:[EBP-0x58]
	$-46B    0097C545     51                         PUSH ECX
	$-46A    0097C546     8B0D 2C417201              MOV ECX,DWORD PTR DS:[0x172412C]
	$-464    0097C54C     895D F0                    MOV DWORD PTR SS:[EBP-0x10],EBX
	$-461    0097C54F     E8 FC522000                CALL MapleSto.00B81850
	$-45C    0097C554     F3:0F101D 94413F01         MOVSS XMM3,DWORD PTR DS:[0x13F4194]
	$-454    0097C55C     F3:0F1045 A8               MOVSS XMM0,DWORD PTR SS:[EBP-0x58]
	$-44F    0097C561     F3:0F104D AC               MOVSS XMM1,DWORD PTR SS:[EBP-0x54]
	$-44A    0097C566     F3:0F1055 B0               MOVSS XMM2,DWORD PTR SS:[EBP-0x50]
	$-445    0097C56B     F3:0F59C3                  MULSS XMM0,XMM3
	$-441    0097C56F     F3:0F59CB                  MULSS XMM1,XMM3
	$-43D    0097C573     F3:0F59D3                  MULSS XMM2,XMM3
	$-439    0097C577     F3:0F1145 A8               MOVSS DWORD PTR SS:[EBP-0x58],XMM0
	$-434    0097C57C     F3:0F114D AC               MOVSS DWORD PTR SS:[EBP-0x54],XMM1
	$-42F    0097C581     F3:0F1155 B0               MOVSS DWORD PTR SS:[EBP-0x50],XMM2
	$-42A    0097C586     F3:0F1186 6C110000         MOVSS DWORD PTR DS:[ESI+0x116C],XMM0
	$-422    0097C58E     F3:0F118E 70110000         MOVSS DWORD PTR DS:[ESI+0x1170],XMM1
	$-41A    0097C596     F3:0F1196 74110000         MOVSS DWORD PTR DS:[ESI+0x1174],XMM2
	$-412    0097C59E     8D7B 01                    LEA EDI,DWORD PTR DS:[EBX+0x1]
	$-40F    0097C5A1     395D F0                    CMP DWORD PTR SS:[EBP-0x10],EBX
	$-40C    0097C5A4     75 03                      JNZ SHORT MapleSto.0097C5A9
	$-40A    0097C5A6     8B7D 24                    MOV EDI,DWORD PTR SS:[EBP+0x24]
	$-407    0097C5A9     8B8E E8000000              MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$-401    0097C5AF     E8 DCF73E01                CALL MapleSto.01D6BD90
	$-3FC    0097C5B4     85C0                       TEST EAX,EAX
	$-3FA    0097C5B6     74 68                      JE SHORT MapleSto.0097C620
	$-3F8    0097C5B8     8B8E E8000000              MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$-3F2    0097C5BE     E8 9DF1E6FF                CALL MapleSto.007EB760
	$-3ED    0097C5C3     85C0                       TEST EAX,EAX
	$-3EB    0097C5C5     75 59                      JNZ SHORT MapleSto.0097C620
	$-3E9    0097C5C7     8B8E E8000000              MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$-3E3    0097C5CD     E8 AE963F01                CALL MapleSto.01D75C80
	$-3DE    0097C5D2     85C0                       TEST EAX,EAX
	$-3DC    0097C5D4     75 4A                      JNZ SHORT MapleSto.0097C620
	$-3DA    0097C5D6     8B0D 2C417201              MOV ECX,DWORD PTR DS:[0x172412C]
	$-3D4    0097C5DC     E8 0FE51F00                CALL MapleSto.00B7AAF0
	$-3CF    0097C5E1     85C0                       TEST EAX,EAX
	$-3CD    0097C5E3     75 3B                      JNZ SHORT MapleSto.0097C620
	$-3CB    0097C5E5     8B8E E8000000              MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$-3C5    0097C5EB     68 083A7301                PUSH MapleSto.01733A08
	$-3C0    0097C5F0     8D55 D0                    LEA EDX,DWORD PTR SS:[EBP-0x30]
	$-3BD    0097C5F3     52                         PUSH EDX
	$-3BC    0097C5F4     E8 D7F1E6FF                CALL MapleSto.007EB7D0
	$-3B7    0097C5F9     8BC8                       MOV ECX,EAX
	$-3B5    0097C5FB     E8 00A7CBFF                CALL MapleSto.00636D00
	$-3B0    0097C600     8BC8                       MOV ECX,EAX
	$-3AE    0097C602     E8 7911C3FF                CALL MapleSto.005AD780
	$-3A9    0097C607     84C0                       TEST AL,AL
	$-3A7    0097C609     74 15                      JE SHORT MapleSto.0097C620
	$-3A5    0097C60B     68 34267401                PUSH MapleSto.01742634
	$-3A0    0097C610     8D4D 08                    LEA ECX,DWORD PTR SS:[EBP+0x8]
	$-39D    0097C613     E8 C8E7DDFF                CALL MapleSto.0075ADE0
	$-398    0097C618     84C0                       TEST AL,AL
	$-396    0097C61A     0F85 0B010000              JNZ MapleSto.0097C72B
	$-390    0097C620     3BFB                       CMP EDI,EBX
	$-38E    0097C622     0F85 03010000              JNZ MapleSto.0097C72B
	$-388    0097C628     8B06                       MOV EAX,DWORD PTR DS:[ESI]
	$-386    0097C62A     8B90 74010000              MOV EDX,DWORD PTR DS:[EAX+0x174]
	$-380    0097C630     53                         PUSH EBX
	$-37F    0097C631     8BCE                       MOV ECX,ESI
	$-37D    0097C633     FFD2                       CALL EDX
	$-37B    0097C635     85C0                       TEST EAX,EAX
	$-379    0097C637     74 3D                      JE SHORT MapleSto.0097C676
	$-377    0097C639     F3:0F1045 08               MOVSS XMM0,DWORD PTR SS:[EBP+0x8]
	$-372    0097C63E     0F2E05 34267401            UCOMISS XMM0,DWORD PTR DS:[0x1742634]
	$-36B    0097C645     9F                         LAHF
	$-36A    0097C646     F6C4 44                    TEST AH,0x44
	$-367    0097C649     7A 2B                      JPE SHORT MapleSto.0097C676
	$-365    0097C64B     F3:0F1045 0C               MOVSS XMM0,DWORD PTR SS:[EBP+0xC]
	$-360    0097C650     0F2E05 38267401            UCOMISS XMM0,DWORD PTR DS:[0x1742638]
	$-359    0097C657     9F                         LAHF
	$-358    0097C658     F6C4 44                    TEST AH,0x44
	$-355    0097C65B     7A 19                      JPE SHORT MapleSto.0097C676
	$-353    0097C65D     F3:0F1045 10               MOVSS XMM0,DWORD PTR SS:[EBP+0x10]
	$-34E    0097C662     0F2E05 3C267401            UCOMISS XMM0,DWORD PTR DS:[0x174263C]
	$-347    0097C669     9F                         LAHF
	$-346    0097C66A     F6C4 44                    TEST AH,0x44
	$-343    0097C66D     7A 07                      JPE SHORT MapleSto.0097C676
	$-341    0097C66F     B8 01000000                MOV EAX,0x1
	$-33C    0097C674     EB 02                      JMP SHORT MapleSto.0097C678
	$-33A    0097C676     33C0                       XOR EAX,EAX
	$-338    0097C678     D945 18                    FLD DWORD PTR SS:[EBP+0x18]
	$-335    0097C67B     50                         PUSH EAX
	$-334    0097C67C     51                         PUSH ECX
	$-333    0097C67D     8B8E E8000000              MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$-32D    0097C683     D91C24                     FSTP DWORD PTR SS:[ESP]
	$-32A    0097C686     8D45 CC                    LEA EAX,DWORD PTR SS:[EBP-0x34]
	$-327    0097C689     50                         PUSH EAX
	$-326    0097C68A     E8 51FCE6FF                CALL MapleSto.007EC2E0
	$-321    0097C68F     8B8E E4000000              MOV ECX,DWORD PTR DS:[ESI+0xE4]
	$-31B    0097C695     E8 56720800                CALL MapleSto.00A038F0
	$-316    0097C69A     83F8 02                    CMP EAX,0x2
	$-313    0097C69D     74 26                      JE SHORT MapleSto.0097C6C5
	$-311    0097C69F     83F8 05                    CMP EAX,0x5
	$-30E    0097C6A2     74 21                      JE SHORT MapleSto.0097C6C5
	$-30C    0097C6A4     83F8 06                    CMP EAX,0x6
	$-309    0097C6A7     74 1C                      JE SHORT MapleSto.0097C6C5
	$-307    0097C6A9     83F8 10                    CMP EAX,0x10
	$-304    0097C6AC     75 0F                      JNZ SHORT MapleSto.0097C6BD
	$-302    0097C6AE     8B8E E8000000              MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$-2FC    0097C6B4     E8 C7F0E6FF                CALL MapleSto.007EB780
	$-2F7    0097C6B9     85C0                       TEST EAX,EAX
	$-2F5    0097C6BB     75 08                      JNZ SHORT MapleSto.0097C6C5
	$-2F3    0097C6BD     0F57DB                     XORPS XMM3,XMM3
	$-2F0    0097C6C0     0F28E3                     MOVAPS XMM4,XMM3
	$-2ED    0097C6C3     EB 0A                      JMP SHORT MapleSto.0097C6CF
	$-2EB    0097C6C5     F3:0F105D D0               MOVSS XMM3,DWORD PTR SS:[EBP-0x30]
	$-2E6    0097C6CA     F3:0F1065 CC               MOVSS XMM4,DWORD PTR SS:[EBP-0x34]
	$-2E1    0097C6CF     F3:0F106D D4               MOVSS XMM5,DWORD PTR SS:[EBP-0x2C]
	$-2DC    0097C6D4     F3:0F1045 08               MOVSS XMM0,DWORD PTR SS:[EBP+0x8]
	$-2D7    0097C6D9     F3:0F104D 0C               MOVSS XMM1,DWORD PTR SS:[EBP+0xC]
	$-2D2    0097C6DE     F3:0F58C4                  ADDSS XMM0,XMM4
	$-2CE    0097C6E2     F3:0F58CB                  ADDSS XMM1,XMM3
	$-2CA    0097C6E6     0F28D5                     MOVAPS XMM2,XMM5
	$-2C7    0097C6E9     F3:0F5855 10               ADDSS XMM2,DWORD PTR SS:[EBP+0x10]
	$-2C2    0097C6EE     F3:0F1186 3C110000         MOVSS DWORD PTR DS:[ESI+0x113C],XMM0
	$-2BA    0097C6F6     F3:0F118E 40110000         MOVSS DWORD PTR DS:[ESI+0x1140],XMM1
	$-2B2    0097C6FE     F3:0F1196 44110000         MOVSS DWORD PTR DS:[ESI+0x1144],XMM2
	$-2AA    0097C706     F3:0F1145 08               MOVSS DWORD PTR SS:[EBP+0x8],XMM0
	$-2A5    0097C70B     0F28F9                     MOVAPS XMM7,XMM1
	$-2A2    0097C70E     0F28F2                     MOVAPS XMM6,XMM2
	$-29F    0097C711     F3:0F11A6 78110000         MOVSS DWORD PTR DS:[ESI+0x1178],XMM4
	$-297    0097C719     F3:0F119E 7C110000         MOVSS DWORD PTR DS:[ESI+0x117C],XMM3
	$-28F    0097C721     F3:0F11AE 80110000         MOVSS DWORD PTR DS:[ESI+0x1180],XMM5
	$-287    0097C729     EB 0A                      JMP SHORT MapleSto.0097C735
	$-285    0097C72B     F3:0F1075 10               MOVSS XMM6,DWORD PTR SS:[EBP+0x10]
	$-280    0097C730     F3:0F107D 0C               MOVSS XMM7,DWORD PTR SS:[EBP+0xC]
	$-27B    0097C735     F3:0F1045 A8               MOVSS XMM0,DWORD PTR SS:[EBP-0x58]
	$-276    0097C73A     F3:0F5845 08               ADDSS XMM0,DWORD PTR SS:[EBP+0x8]
	$-271    0097C73F     F3:0F104D AC               MOVSS XMM1,DWORD PTR SS:[EBP-0x54]
	$-26C    0097C744     F3:0F1055 B0               MOVSS XMM2,DWORD PTR SS:[EBP-0x50]
	$-267    0097C749     F3:0F58CF                  ADDSS XMM1,XMM7
	$-263    0097C74D     F3:0F58D6                  ADDSS XMM2,XMM6
	$-25F    0097C751     F3:0F1186 48110000         MOVSS DWORD PTR DS:[ESI+0x1148],XMM0
	$-257    0097C759     F3:0F118E 4C110000         MOVSS DWORD PTR DS:[ESI+0x114C],XMM1
	$-24F    0097C761     F3:0F1196 50110000         MOVSS DWORD PTR DS:[ESI+0x1150],XMM2
	$-247    0097C769     8B8E B0100000              MOV ECX,DWORD PTR DS:[ESI+0x10B0]
	$-241    0097C76F     8D55 D8                    LEA EDX,DWORD PTR SS:[EBP-0x28]
	$-23E    0097C772     F3:0F1145 08               MOVSS DWORD PTR SS:[EBP+0x8],XMM0
	$-239    0097C777     F30F7E86 A8100000          MOVQ XMM0,QWORD PTR DS:[ESI+0x10A8]
	$-231    0097C77F     894D BC                    MOV DWORD PTR SS:[EBP-0x44],ECX
	$-22E    0097C782     8B8E E8000000              MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$-228    0097C788     52                         PUSH EDX
	$-227    0097C789     F3:0F114D 0C               MOVSS DWORD PTR SS:[EBP+0xC],XMM1
	$-222    0097C78E     F3:0F1155 10               MOVSS DWORD PTR SS:[EBP+0x10],XMM2
	$-21D    0097C793     660FD645 B4                MOVQ QWORD PTR SS:[EBP-0x4C],XMM0
	$-218    0097C798     E8 33F0E6FF                CALL MapleSto.007EB7D0
	$-213    0097C79D     8BC8                       MOV ECX,EAX
	$-211    0097C79F     E8 5CA5CBFF                CALL MapleSto.00636D00
	$-20C    0097C7A4     8B45 D8                    MOV EAX,DWORD PTR SS:[EBP-0x28]
	$-209    0097C7A7     3B05 083A7301              CMP EAX,DWORD PTR DS:[0x1733A08]
	$-203    0097C7AD     75 0F                      JNZ SHORT MapleSto.0097C7BE
	$-201    0097C7AF     8B4D DC                    MOV ECX,DWORD PTR SS:[EBP-0x24]
	$-1FE    0097C7B2     3B0D 0C3A7301              CMP ECX,DWORD PTR DS:[0x1733A0C]
	$-1F8    0097C7B8     0F84 E7010000              JE MapleSto.0097C9A5
	$-1F2    0097C7BE     8B8E E8000000              MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$-1EC    0097C7C4     E8 07F0E6FF                CALL MapleSto.007EB7D0
	$-1E7    0097C7C9     8BC8                       MOV ECX,EAX
	$-1E5    0097C7CB     E8 50A5CBFF                CALL MapleSto.00636D20
	$-1E0    0097C7D0     8BF8                       MOV EDI,EAX
	$-1DE    0097C7D2     3BFB                       CMP EDI,EBX
	$-1DC    0097C7D4     0F84 CB010000              JE MapleSto.0097C9A5
	$-1D6    0097C7DA     68 0F0000F0                PUSH 0xF000000F
	$-1D1    0097C7DF     8D55 B4                    LEA EDX,DWORD PTR SS:[EBP-0x4C]
	$-1CE    0097C7E2     52                         PUSH EDX
	$-1CD    0097C7E3     68 52CA95FD                PUSH 0xFD95CA52
	$-1C8    0097C7E8     8BCF                       MOV ECX,EDI
	$-1C6    0097C7EA     E8 11D64A00                CALL MapleSto.00E29E00                   ; g_c_get_square_pos_
	$-1C1    0097C7EF     F30F7E45 B4                MOVQ XMM0,QWORD PTR SS:[EBP-0x4C]
	$-1BC    0097C7F4     8B4D BC                    MOV ECX,DWORD PTR SS:[EBP-0x44]
	$-1B9    0097C7F7     83EC 0C                    SUB ESP,0xC
	$-1B6    0097C7FA     8BC4                       MOV EAX,ESP
	$-1B4    0097C7FC     660FD600                   MOVQ QWORD PTR DS:[EAX],XMM0
	$-1B0    0097C800     8948 08                    MOV DWORD PTR DS:[EAX+0x8],ECX
	$-1AD    0097C803     E8 A875C0FF                CALL MapleSto.00583DB0
	$-1A8    0097C808     83C4 0C                    ADD ESP,0xC
	$-1A5    0097C80B     57                         PUSH EDI
	$-1A4    0097C80C     8D55 E4                    LEA EDX,DWORD PTR SS:[EBP-0x1C]
	$-1A1    0097C80F     8986 B4100000              MOV DWORD PTR DS:[ESI+0x10B4],EAX
	$-19B    0097C815     8B0D 2C417201              MOV ECX,DWORD PTR DS:[0x172412C]
	$-195    0097C81B     52                         PUSH EDX
	$-194    0097C81C     E8 7F2B2000                CALL MapleSto.00B7F3A0
	$-18F    0097C821     8B4D E8                    MOV ECX,DWORD PTR SS:[EBP-0x18]
	$-18C    0097C824     895D FC                    MOV DWORD PTR SS:[EBP-0x4],EBX
	$-189    0097C827     3BCB                       CMP ECX,EBX
	$-187    0097C829     74 21                      JE SHORT MapleSto.0097C84C
	$-185    0097C82B     68 34267401                PUSH MapleSto.01742634
	$-180    0097C830     8D45 CC                    LEA EAX,DWORD PTR SS:[EBP-0x34]
	$-17D    0097C833     50                         PUSH EAX
	$-17C    0097C834     E8 A72CC7FF                CALL MapleSto.005EF4E0
	$-177    0097C839     8BC8                       MOV ECX,EAX
	$-175    0097C83B     E8 E017C7FF                CALL MapleSto.005EE020
	$-170    0097C840     84C0                       TEST AL,AL
	$-16E    0097C842     74 08                      JE SHORT MapleSto.0097C84C
	$-16C    0097C844     8B4D E8                    MOV ECX,DWORD PTR SS:[EBP-0x18]
	$-169    0097C847     E8 D6293401                CALL MapleSto.01CBF222
	$-164    0097C84C     83CB FF                    OR EBX,0xFFFFFFFF
	$-161    0097C84F     8D4D E4                    LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$-15E    0097C852     895D FC                    MOV DWORD PTR SS:[EBP-0x4],EBX
	$-15B    0097C855     E8 B6081000                CALL MapleSto.00A7D110
	$-156    0097C85A     8B4D D8                    MOV ECX,DWORD PTR SS:[EBP-0x28]
	$-153    0097C85D     3B8E A0100000              CMP ECX,DWORD PTR DS:[ESI+0x10A0]
	$-14D    0097C863     0F85 3C010000              JNZ MapleSto.0097C9A5
	$-147    0097C869     8B55 DC                    MOV EDX,DWORD PTR SS:[EBP-0x24]
	$-144    0097C86C     3B96 A4100000              CMP EDX,DWORD PTR DS:[ESI+0x10A4]
	$-13E    0097C872     0F85 2D010000              JNZ MapleSto.0097C9A5
	$-138    0097C878     8B0D 2C417201              MOV ECX,DWORD PTR DS:[0x172412C]
	$-132    0097C87E     57                         PUSH EDI
	$-131    0097C87F     8D45 D0                    LEA EAX,DWORD PTR SS:[EBP-0x30]
	$-12E    0097C882     50                         PUSH EAX
	$-12D    0097C883     E8 182B2000                CALL MapleSto.00B7F3A0
	$-128    0097C888     837D D4 00                 CMP DWORD PTR SS:[EBP-0x2C],0x0
	$-124    0097C88C     C745 FC 01000000           MOV DWORD PTR SS:[EBP-0x4],0x1
	$-11D    0097C893     0F84 01010000              JE MapleSto.0097C99A
	$-117    0097C899     F3:0F1045 BC               MOVSS XMM0,DWORD PTR SS:[EBP-0x44]
	$-112    0097C89E     F3:0F5C86 B0100000         SUBSS XMM0,DWORD PTR DS:[ESI+0x10B0]
	$-10A    0097C8A6     F3:0F104D B4               MOVSS XMM1,DWORD PTR SS:[EBP-0x4C]
	$-105    0097C8AB     F3:0F5C8E A8100000         SUBSS XMM1,DWORD PTR DS:[ESI+0x10A8]
	$-FD     0097C8B3     F3:0F1055 B8               MOVSS XMM2,DWORD PTR SS:[EBP-0x48]
	$-F8     0097C8B8     F3:0F5C96 AC100000         SUBSS XMM2,DWORD PTR DS:[ESI+0x10AC]
	$-F0     0097C8C0     0F57DB                     XORPS XMM3,XMM3
	$-ED     0097C8C3     0F2FD8                     COMISS XMM3,XMM0
	$-EA     0097C8C6     F3:0F114D 24               MOVSS DWORD PTR SS:[EBP+0x24],XMM1
	$-E5     0097C8CB     F3:0F1155 EC               MOVSS DWORD PTR SS:[EBP-0x14],XMM2
	$-E0     0097C8D0     F3:0F1145 E8               MOVSS DWORD PTR SS:[EBP-0x18],XMM0
	$-DB     0097C8D5     F3:0F118E 84110000         MOVSS DWORD PTR DS:[ESI+0x1184],XMM1
	$-D3     0097C8DD     F3:0F1196 88110000         MOVSS DWORD PTR DS:[ESI+0x1188],XMM2
	$-CB     0097C8E5     F3:0F1186 8C110000         MOVSS DWORD PTR DS:[ESI+0x118C],XMM0
	$-C3     0097C8ED     72 50                      JB SHORT MapleSto.0097C93F
	$-C1     0097C8EF     F3:0F101D 94413F01         MOVSS XMM3,DWORD PTR DS:[0x13F4194]
	$-B9     0097C8F7     F3:0F59CB                  MULSS XMM1,XMM3
	$-B5     0097C8FB     F3:0F584D 08               ADDSS XMM1,DWORD PTR SS:[EBP+0x8]
	$-B0     0097C900     F3:0F59C3                  MULSS XMM0,XMM3
	$-AC     0097C904     F3:0F5845 10               ADDSS XMM0,DWORD PTR SS:[EBP+0x10]
	$-A7     0097C909     F3:0F59D3                  MULSS XMM2,XMM3
	$-A3     0097C90D     F3:0F105D 0C               MOVSS XMM3,DWORD PTR SS:[EBP+0xC]
	$-9E     0097C912     F3:0F58DA                  ADDSS XMM3,XMM2
	$-9A     0097C916     F3:0F114D 08               MOVSS DWORD PTR SS:[EBP+0x8],XMM1
	$-95     0097C91B     F3:0F115D 0C               MOVSS DWORD PTR SS:[EBP+0xC],XMM3
	$-90     0097C920     F3:0F1145 10               MOVSS DWORD PTR SS:[EBP+0x10],XMM0
	$-8B     0097C925     F3:0F118E 54110000         MOVSS DWORD PTR DS:[ESI+0x1154],XMM1
	$-83     0097C92D     F3:0F119E 58110000         MOVSS DWORD PTR DS:[ESI+0x1158],XMM3
	$-7B     0097C935     F3:0F1186 5C110000         MOVSS DWORD PTR DS:[ESI+0x115C],XMM0
	$-73     0097C93D     EB 5B                      JMP SHORT MapleSto.0097C99A
	$-71     0097C93F     0F2FC3                     COMISS XMM0,XMM3
	$-6E     0097C942     76 56                      JBE SHORT MapleSto.0097C99A
	$-6C     0097C944     8B16                       MOV EDX,DWORD PTR DS:[ESI]
	$-6A     0097C946     8B52 48                    MOV EDX,DWORD PTR DS:[EDX+0x48]
	$-67     0097C949     8D45 90                    LEA EAX,DWORD PTR SS:[EBP-0x70]
	$-64     0097C94C     50                         PUSH EAX
	$-63     0097C94D     8BCE                       MOV ECX,ESI
	$-61     0097C94F     FFD2                       CALL EDX
	$-5F     0097C951     F3:0F1040 04               MOVSS XMM0,DWORD PTR DS:[EAX+0x4]
	$-5A     0097C956     F3:0F1048 08               MOVSS XMM1,DWORD PTR DS:[EAX+0x8]
	$-55     0097C95B     F3:0F1055 24               MOVSS XMM2,DWORD PTR SS:[EBP+0x24]
	$-50     0097C960     F3:0F5810                  ADDSS XMM2,DWORD PTR DS:[EAX]
	$-4C     0097C964     F3:0F5845 EC               ADDSS XMM0,DWORD PTR SS:[EBP-0x14]
	$-47     0097C969     F3:0F584D E8               ADDSS XMM1,DWORD PTR SS:[EBP-0x18]
	$-42     0097C96E     8B16                       MOV EDX,DWORD PTR DS:[ESI]
	$-40     0097C970     8B52 4C                    MOV EDX,DWORD PTR DS:[EDX+0x4C]
	$-3D     0097C973     83EC 0C                    SUB ESP,0xC
	$-3A     0097C976     8BC4                       MOV EAX,ESP
	$-38     0097C978     F3:0F1155 E0               MOVSS DWORD PTR SS:[EBP-0x20],XMM2
	$-33     0097C97D     F3:0F1145 E4               MOVSS DWORD PTR SS:[EBP-0x1C],XMM0
	$-2E     0097C982     F30F7E45 E0                MOVQ XMM0,QWORD PTR SS:[EBP-0x20]
	$-29     0097C987     F3:0F114D E8               MOVSS DWORD PTR SS:[EBP-0x18],XMM1
	$-24     0097C98C     8B4D E8                    MOV ECX,DWORD PTR SS:[EBP-0x18]
	$-21     0097C98F     660FD600                   MOVQ QWORD PTR DS:[EAX],XMM0
	$-1D     0097C993     8948 08                    MOV DWORD PTR DS:[EAX+0x8],ECX
	$-1A     0097C996     8BCE                       MOV ECX,ESI
	$-18     0097C998     FFD2                       CALL EDX
	$-16     0097C99A     8D4D D0                    LEA ECX,DWORD PTR SS:[EBP-0x30]
	$-13     0097C99D     895D FC                    MOV DWORD PTR SS:[EBP-0x4],EBX
	$-10     0097C9A0     E8 6B071000                CALL MapleSto.00A7D110
	$-B      0097C9A5     F3:0F1045 B4               MOVSS XMM0,DWORD PTR SS:[EBP-0x4C]       ; eax=0这里已经改变坐标
	$-6      0097C9AA     8B4D DC                    MOV ECX,DWORD PTR SS:[EBP-0x24]
	$-3      0097C9AD     8B45 D8                    MOV EAX,DWORD PTR SS:[EBP-0x28]
	$ ==>    0097C9B0     F3:0F1186 A8100000         MOVSS DWORD PTR DS:[ESI+0x10A8],XMM0     ; 人物方块坐标,g_o_game_obj_cube_pos_
	$+8      0097C9B8     F3:0F1045 B8               MOVSS XMM0,DWORD PTR SS:[EBP-0x48]
	$+D      0097C9BD     F3:0F1186 AC100000         MOVSS DWORD PTR DS:[ESI+0x10AC],XMM0
	$+15     0097C9C5     F3:0F1045 BC               MOVSS XMM0,DWORD PTR SS:[EBP-0x44]
	$+1A     0097C9CA     F3:0F1186 B0100000         MOVSS DWORD PTR DS:[ESI+0x10B0],XMM0
	$+22     0097C9D2     F3:0F1045 08               MOVSS XMM0,DWORD PTR SS:[EBP+0x8]
	$+27     0097C9D7     8986 A0100000              MOV DWORD PTR DS:[ESI+0x10A0],EAX
	$+2D     0097C9DD     898E A4100000              MOV DWORD PTR DS:[ESI+0x10A4],ECX
	$+33     0097C9E3     F3:0F1186 90110000         MOVSS DWORD PTR DS:[ESI+0x1190],XMM0
	$+3B     0097C9EB     F3:0F1045 0C               MOVSS XMM0,DWORD PTR SS:[EBP+0xC]
	$+40     0097C9F0     F3:0F1186 94110000         MOVSS DWORD PTR DS:[ESI+0x1194],XMM0
	$+48     0097C9F8     F3:0F1045 10               MOVSS XMM0,DWORD PTR SS:[EBP+0x10]
	$+4D     0097C9FD     8D55 90                    LEA EDX,DWORD PTR SS:[EBP-0x70]
	$+50     0097CA00     F3:0F1186 98110000         MOVSS DWORD PTR DS:[ESI+0x1198],XMM0
	$+58     0097CA08     8B8E E8000000              MOV ECX,DWORD PTR DS:[ESI+0xE8]          ; g_o_model_control_
	$+5E     0097CA0E     52                         PUSH EDX
	$+5F     0097CA0F     E8 1CEFE6FF                CALL MapleSto.007EB930
	$+64     0097CA14     D900                       FLD DWORD PTR DS:[EAX]
	$+66     0097CA16     F3:0F1045 08               MOVSS XMM0,DWORD PTR SS:[EBP+0x8]
	$+6B     0097CA1B     D99E A8110000              FSTP DWORD PTR DS:[ESI+0x11A8]
	$+71     0097CA21     D940 04                    FLD DWORD PTR DS:[EAX+0x4]
	$+74     0097CA24     D99E AC110000              FSTP DWORD PTR DS:[ESI+0x11AC]
	$+7A     0097CA2A     D940 08                    FLD DWORD PTR DS:[EAX+0x8]
	$+7D     0097CA2D     D99E B0110000              FSTP DWORD PTR DS:[ESI+0x11B0]
	$+83     0097CA33     0F2E05 34267401            UCOMISS XMM0,DWORD PTR DS:[0x1742634]
	$+8A     0097CA3A     9F                         LAHF
	$+8B     0097CA3B     F6C4 44                    TEST AH,0x44
	$+8E     0097CA3E     7A 24                      JPE SHORT MapleSto.0097CA64
	$+90     0097CA40     F3:0F1045 0C               MOVSS XMM0,DWORD PTR SS:[EBP+0xC]
	$+95     0097CA45     0F2E05 38267401            UCOMISS XMM0,DWORD PTR DS:[0x1742638]
	$+9C     0097CA4C     9F                         LAHF
	$+9D     0097CA4D     F6C4 44                    TEST AH,0x44
	$+A0     0097CA50     7A 12                      JPE SHORT MapleSto.0097CA64
	$+A2     0097CA52     F3:0F1045 10               MOVSS XMM0,DWORD PTR SS:[EBP+0x10]
	$+A7     0097CA57     0F2E05 3C267401            UCOMISS XMM0,DWORD PTR DS:[0x174263C]
	$+AE     0097CA5E     9F                         LAHF
	$+AF     0097CA5F     F6C4 44                    TEST AH,0x44
	$+B2     0097CA62     7B 53                      JPO SHORT MapleSto.0097CAB7
	$+B4     0097CA64     8D45 08                    LEA EAX,DWORD PTR SS:[EBP+0x8]
	$+B7     0097CA67     50                         PUSH EAX
	$+B8     0097CA68     E8 03F0C6FF                CALL MapleSto.005EBA70
	$+BD     0097CA6D     83C4 04                    ADD ESP,0x4
	$+C0     0097CA70     85C0                       TEST EAX,EAX
	$+C2     0097CA72     74 1A                      JE SHORT MapleSto.0097CA8E
	$+C4     0097CA74     8B4D 20                    MOV ECX,DWORD PTR SS:[EBP+0x20]
	$+C7     0097CA77     F30F7E45 08                MOVQ XMM0,QWORD PTR SS:[EBP+0x8]
	$+CC     0097CA7C     8B55 10                    MOV EDX,DWORD PTR SS:[EBP+0x10]
	$+CF     0097CA7F     51                         PUSH ECX
	$+D0     0097CA80     83EC 0C                    SUB ESP,0xC
	$+D3     0097CA83     8BC4                       MOV EAX,ESP
	$+D5     0097CA85     660FD600                   MOVQ QWORD PTR DS:[EAX],XMM0
	$+D9     0097CA89     8950 08                    MOV DWORD PTR DS:[EAX+0x8],EDX
	$+DC     0097CA8C     EB 1E                      JMP SHORT MapleSto.0097CAAC
	$+DE     0097CA8E     8B45 20                    MOV EAX,DWORD PTR SS:[EBP+0x20]
	$+E1     0097CA91     F30F7E05 34267401          MOVQ XMM0,QWORD PTR DS:[0x1742634]
	$+E9     0097CA99     8B0D 3C267401              MOV ECX,DWORD PTR DS:[0x174263C]
	$+EF     0097CA9F     50                         PUSH EAX
	$+F0     0097CAA0     83EC 0C                    SUB ESP,0xC
	$+F3     0097CAA3     8BC4                       MOV EAX,ESP
	$+F5     0097CAA5     660FD600                   MOVQ QWORD PTR DS:[EAX],XMM0
	$+F9     0097CAA9     8948 08                    MOV DWORD PTR DS:[EAX+0x8],ECX
	$+FC     0097CAAC     8B8E E8000000              MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$+102    0097CAB2     E8 79F4E6FF                CALL MapleSto.007EBF30
	$+107    0097CAB7     8B8E E8000000              MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$+10D    0097CABD     8D55 90                    LEA EDX,DWORD PTR SS:[EBP-0x70]
	$+110    0097CAC0     52                         PUSH EDX
	$+111    0097CAC1     E8 6AEEE6FF                CALL MapleSto.007EB930
	$+116    0097CAC6     D900                       FLD DWORD PTR DS:[EAX]
	$+118    0097CAC8     8D8E B4110000              LEA ECX,DWORD PTR DS:[ESI+0x11B4]
	$+11E    0097CACE     D919                       FSTP DWORD PTR DS:[ECX]
	$+120    0097CAD0     51                         PUSH ECX
	$+121    0097CAD1     D940 04                    FLD DWORD PTR DS:[EAX+0x4]
	$+124    0097CAD4     D959 04                    FSTP DWORD PTR DS:[ECX+0x4]
	$+127    0097CAD7     D940 08                    FLD DWORD PTR DS:[EAX+0x8]
	$+12A    0097CADA     D959 08                    FSTP DWORD PTR DS:[ECX+0x8]
	$+12D    0097CADD     E8 8EEFC6FF                CALL MapleSto.005EBA70
	$+132    0097CAE2     83C4 04                    ADD ESP,0x4
	$+135    0097CAE5     85C0                       TEST EAX,EAX
	$+137    0097CAE7     75 4A                      JNZ SHORT MapleSto.0097CB33
	$+139    0097CAE9     8B8E B0110000              MOV ECX,DWORD PTR DS:[ESI+0x11B0]
	$+13F    0097CAEF     F30F7E86 A8110000          MOVQ XMM0,QWORD PTR DS:[ESI+0x11A8]
	$+147    0097CAF7     8B16                       MOV EDX,DWORD PTR DS:[ESI]
	$+149    0097CAF9     8B52 4C                    MOV EDX,DWORD PTR DS:[EDX+0x4C]
	$+14C    0097CAFC     83EC 0C                    SUB ESP,0xC
	$+14F    0097CAFF     8BC4                       MOV EAX,ESP
	$+151    0097CB01     660FD600                   MOVQ QWORD PTR DS:[EAX],XMM0
	$+155    0097CB05     8948 08                    MOV DWORD PTR DS:[EAX+0x8],ECX
	$+158    0097CB08     8BCE                       MOV ECX,ESI
	$+15A    0097CB0A     FFD2                       CALL EDX
	$+15C    0097CB0C     F3:0F1005 34267401         MOVSS XMM0,DWORD PTR DS:[0x1742634]
	$+164    0097CB14     F3:0F1145 08               MOVSS DWORD PTR SS:[EBP+0x8],XMM0
	$+169    0097CB19     F3:0F1005 38267401         MOVSS XMM0,DWORD PTR DS:[0x1742638]
	$+171    0097CB21     F3:0F1145 0C               MOVSS DWORD PTR SS:[EBP+0xC],XMM0
	$+176    0097CB26     F3:0F1005 3C267401         MOVSS XMM0,DWORD PTR DS:[0x174263C]
	$+17E    0097CB2E     F3:0F1145 10               MOVSS DWORD PTR SS:[EBP+0x10],XMM0
	$+183    0097CB33     8B8E E8000000              MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$+189    0097CB39     E8 72ECE6FF                CALL MapleSto.007EB7B0
	$+18E    0097CB3E     A8 04                      TEST AL,0x4
	$+190    0097CB40     75 32                      JNZ SHORT MapleSto.0097CB74
	$+192    0097CB42     A8 01                      TEST AL,0x1
	$+194    0097CB44     74 1C                      JE SHORT MapleSto.0097CB62
	$+196    0097CB46     83BE F0110000 00           CMP DWORD PTR DS:[ESI+0x11F0],0x0
	$+19D    0097CB4D     75 30                      JNZ SHORT MapleSto.0097CB7F
	$+19F    0097CB4F     8B8E E8000000              MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$+1A5    0097CB55     E8 F6ECE6FF                CALL MapleSto.007EB850
	$+1AA    0097CB5A     8986 F0110000              MOV DWORD PTR DS:[ESI+0x11F0],EAX
	$+1B0    0097CB60     EB 1D                      JMP SHORT MapleSto.0097CB7F
	$+1B2    0097CB62     8B06                       MOV EAX,DWORD PTR DS:[ESI]
	$+1B4    0097CB64     8B90 74010000              MOV EDX,DWORD PTR DS:[EAX+0x174]
	$+1BA    0097CB6A     6A 00                      PUSH 0x0
	$+1BC    0097CB6C     8BCE                       MOV ECX,ESI
	$+1BE    0097CB6E     FFD2                       CALL EDX
	$+1C0    0097CB70     85C0                       TEST EAX,EAX
	$+1C2    0097CB72     74 0B                      JE SHORT MapleSto.0097CB7F
	$+1C4    0097CB74     0F57C0                     XORPS XMM0,XMM0
	$+1C7    0097CB77     F3:0F1186 0C100000         MOVSS DWORD PTR DS:[ESI+0x100C],XMM0
	$+1CF    0097CB7F     8B8E E8000000              MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$+1D5    0097CB85     8D45 C0                    LEA EAX,DWORD PTR SS:[EBP-0x40]
	$+1D8    0097CB88     50                         PUSH EAX
	$+1D9    0097CB89     E8 A2EDE6FF                CALL MapleSto.007EB930
	$+1DE    0097CB8E     F30F7E45 C0                MOVQ XMM0,QWORD PTR SS:[EBP-0x40]
	$+1E3    0097CB93     8B55 C8                    MOV EDX,DWORD PTR SS:[EBP-0x38]
	$+1E6    0097CB96     8D4D 9C                    LEA ECX,DWORD PTR SS:[EBP-0x64]
	$+1E9    0097CB99     51                         PUSH ECX
	$+1EA    0097CB9A     8B0D 2C417201              MOV ECX,DWORD PTR DS:[0x172412C]
	$+1F0    0097CBA0     83EC 0C                    SUB ESP,0xC
	$+1F3    0097CBA3     8BC4                       MOV EAX,ESP
	$+1F5    0097CBA5     660FD600                   MOVQ QWORD PTR DS:[EAX],XMM0
	$+1F9    0097CBA9     8950 08                    MOV DWORD PTR DS:[EAX+0x8],EDX
	$+1FC    0097CBAC     E8 EFE91F00                CALL MapleSto.00B7B5A0
	$+201    0097CBB1     85C0                       TEST EAX,EAX
	$+203    0097CBB3     0F85 EE010000              JNZ MapleSto.0097CDA7
	$+209    0097CBB9     F3:0F1065 A4               MOVSS XMM4,DWORD PTR SS:[EBP-0x5C]
	$+20E    0097CBBE     0F57C0                     XORPS XMM0,XMM0
	$+211    0097CBC1     F3:0F1055 10               MOVSS XMM2,DWORD PTR SS:[EBP+0x10]
	$+216    0097CBC6     0F2EE0                     UCOMISS XMM4,XMM0
	$+219    0097CBC9     9F                         LAHF
	$+21A    0097CBCA     F6C4 44                    TEST AH,0x44
	$+21D    0097CBCD     7B 0D                      JPO SHORT MapleSto.0097CBDC
	$+21F    0097CBCF     0F2FC2                     COMISS XMM0,XMM2
	$+222    0097CBD2     76 08                      JBE SHORT MapleSto.0097CBDC
	$+224    0097CBD4     0F28E0                     MOVAPS XMM4,XMM0
	$+227    0097CBD7     F3:0F1165 A4               MOVSS DWORD PTR SS:[EBP-0x5C],XMM4
	$+22C    0097CBDC     F3:0F106D 9C               MOVSS XMM5,DWORD PTR SS:[EBP-0x64]
	$+231    0097CBE1     0F2E2D 34267401            UCOMISS XMM5,DWORD PTR DS:[0x1742634]
	$+238    0097CBE8     F3:0F1075 A0               MOVSS XMM6,DWORD PTR SS:[EBP-0x60]
	$+23D    0097CBED     9F                         LAHF
	$+23E    0097CBEE     F6C4 44                    TEST AH,0x44
	$+241    0097CBF1     7A 1E                      JPE SHORT MapleSto.0097CC11
	$+243    0097CBF3     0F2E35 38267401            UCOMISS XMM6,DWORD PTR DS:[0x1742638]
	$+24A    0097CBFA     9F                         LAHF
	$+24B    0097CBFB     F6C4 44                    TEST AH,0x44
	$+24E    0097CBFE     7A 11                      JPE SHORT MapleSto.0097CC11
	$+250    0097CC00     0F2E25 3C267401            UCOMISS XMM4,DWORD PTR DS:[0x174263C]
	$+257    0097CC07     9F                         LAHF
	$+258    0097CC08     F6C4 44                    TEST AH,0x44
	$+25B    0097CC0B     0F8B 96010000              JPO MapleSto.0097CDA7
	$+261    0097CC11     F3:0F1045 08               MOVSS XMM0,DWORD PTR SS:[EBP+0x8]
	$+266    0097CC16     F3:0F104D 0C               MOVSS XMM1,DWORD PTR SS:[EBP+0xC]
	$+26B    0097CC1B     F3:0F1186 60110000         MOVSS DWORD PTR DS:[ESI+0x1160],XMM0
	$+273    0097CC23     F3:0F118E 64110000         MOVSS DWORD PTR DS:[ESI+0x1164],XMM1
	$+27B    0097CC2B     F3:0F1196 68110000         MOVSS DWORD PTR DS:[ESI+0x1168],XMM2
	$+283    0097CC33     F3:0F101D 00F84101         MOVSS XMM3,DWORD PTR DS:[0x141F800]
	$+28B    0097CC3B     8D45 CC                    LEA EAX,DWORD PTR SS:[EBP-0x34]
	$+28E    0097CC3E     0F57C3                     XORPS XMM0,XMM3
	$+291    0097CC41     0F57CB                     XORPS XMM1,XMM3
	$+294    0097CC44     0F57D3                     XORPS XMM2,XMM3
	$+297    0097CC47     F3:0F59C5                  MULSS XMM0,XMM5
	$+29B    0097CC4B     F3:0F59CE                  MULSS XMM1,XMM6
	$+29F    0097CC4F     F3:0F59D4                  MULSS XMM2,XMM4
	$+2A3    0097CC53     50                         PUSH EAX
	$+2A4    0097CC54     F3:0F1145 CC               MOVSS DWORD PTR SS:[EBP-0x34],XMM0
	$+2A9    0097CC59     F3:0F114D D0               MOVSS DWORD PTR SS:[EBP-0x30],XMM1
	$+2AE    0097CC5E     F3:0F1155 D4               MOVSS DWORD PTR SS:[EBP-0x2C],XMM2
	$+2B3    0097CC63     F3:0F1186 9C110000         MOVSS DWORD PTR DS:[ESI+0x119C],XMM0
	$+2BB    0097CC6B     F3:0F118E A0110000         MOVSS DWORD PTR DS:[ESI+0x11A0],XMM1
	$+2C3    0097CC73     F3:0F1196 A4110000         MOVSS DWORD PTR DS:[ESI+0x11A4],XMM2
	$+2CB    0097CC7B     E8 F0EDC6FF                CALL MapleSto.005EBA70
	$+2D0    0097CC80     83C4 04                    ADD ESP,0x4
	$+2D3    0097CC83     85C0                       TEST EAX,EAX
	$+2D5    0097CC85     75 27                      JNZ SHORT MapleSto.0097CCAE
	$+2D7    0097CC87     F3:0F1005 34267401         MOVSS XMM0,DWORD PTR DS:[0x1742634]
	$+2DF    0097CC8F     F3:0F1145 CC               MOVSS DWORD PTR SS:[EBP-0x34],XMM0
	$+2E4    0097CC94     F3:0F1005 38267401         MOVSS XMM0,DWORD PTR DS:[0x1742638]
	$+2EC    0097CC9C     F3:0F1145 D0               MOVSS DWORD PTR SS:[EBP-0x30],XMM0
	$+2F1    0097CCA1     F3:0F1005 3C267401         MOVSS XMM0,DWORD PTR DS:[0x174263C]
	$+2F9    0097CCA9     F3:0F1145 D4               MOVSS DWORD PTR SS:[EBP-0x2C],XMM0
	$+2FE    0097CCAE     8D4D E0                    LEA ECX,DWORD PTR SS:[EBP-0x20]
	$+301    0097CCB1     51                         PUSH ECX
	$+302    0097CCB2     8B8E E8000000              MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$+308    0097CCB8     E8 73ECE6FF                CALL MapleSto.007EB930
	$+30D    0097CCBD     8B55 20                    MOV EDX,DWORD PTR SS:[EBP+0x20]
	$+310    0097CCC0     F30F7E45 CC                MOVQ XMM0,QWORD PTR SS:[EBP-0x34]
	$+315    0097CCC5     8B4D D4                    MOV ECX,DWORD PTR SS:[EBP-0x2C]
	$+318    0097CCC8     52                         PUSH EDX
	$+319    0097CCC9     83EC 0C                    SUB ESP,0xC
	$+31C    0097CCCC     8BC4                       MOV EAX,ESP
	$+31E    0097CCCE     660FD600                   MOVQ QWORD PTR DS:[EAX],XMM0
	$+322    0097CCD2     8948 08                    MOV DWORD PTR DS:[EAX+0x8],ECX
	$+325    0097CCD5     8B8E E8000000              MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$+32B    0097CCDB     E8 50F2E6FF                CALL MapleSto.007EBF30
	$+330    0097CCE0     8B8E E8000000              MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$+336    0097CCE6     8D55 90                    LEA EDX,DWORD PTR SS:[EBP-0x70]
	$+339    0097CCE9     52                         PUSH EDX
	$+33A    0097CCEA     E8 41ECE6FF                CALL MapleSto.007EB930
	$+33F    0097CCEF     F3:0F1000                  MOVSS XMM0,DWORD PTR DS:[EAX]
	$+343    0097CCF3     8B8E E8000000              MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$+349    0097CCF9     F3:0F1145 C0               MOVSS DWORD PTR SS:[EBP-0x40],XMM0
	$+34E    0097CCFE     F3:0F1040 04               MOVSS XMM0,DWORD PTR DS:[EAX+0x4]
	$+353    0097CD03     F3:0F1145 C4               MOVSS DWORD PTR SS:[EBP-0x3C],XMM0
	$+358    0097CD08     F3:0F1040 08               MOVSS XMM0,DWORD PTR DS:[EAX+0x8]
	$+35D    0097CD0D     8D45 90                    LEA EAX,DWORD PTR SS:[EBP-0x70]
	$+360    0097CD10     50                         PUSH EAX
	$+361    0097CD11     F3:0F1145 C8               MOVSS DWORD PTR SS:[EBP-0x38],XMM0
	$+366    0097CD16     E8 15ECE6FF                CALL MapleSto.007EB930
	$+36B    0097CD1B     D900                       FLD DWORD PTR DS:[EAX]
	$+36D    0097CD1D     D99E C0110000              FSTP DWORD PTR DS:[ESI+0x11C0]
	$+373    0097CD23     8D4D C0                    LEA ECX,DWORD PTR SS:[EBP-0x40]
	$+376    0097CD26     D940 04                    FLD DWORD PTR DS:[EAX+0x4]
	$+379    0097CD29     51                         PUSH ECX
	$+37A    0097CD2A     D99E C4110000              FSTP DWORD PTR DS:[ESI+0x11C4]
	$+380    0097CD30     D940 08                    FLD DWORD PTR DS:[EAX+0x8]
	$+383    0097CD33     D99E C8110000              FSTP DWORD PTR DS:[ESI+0x11C8]
	$+389    0097CD39     E8 32EDC6FF                CALL MapleSto.005EBA70
	$+38E    0097CD3E     83C4 04                    ADD ESP,0x4
	$+391    0097CD41     85C0                       TEST EAX,EAX
	$+393    0097CD43     75 62                      JNZ SHORT MapleSto.0097CDA7
	$+395    0097CD45     8B4D E8                    MOV ECX,DWORD PTR SS:[EBP-0x18]
	$+398    0097CD48     F30F7E45 E0                MOVQ XMM0,QWORD PTR SS:[EBP-0x20]
	$+39D    0097CD4D     8B16                       MOV EDX,DWORD PTR DS:[ESI]
	$+39F    0097CD4F     8B52 4C                    MOV EDX,DWORD PTR DS:[EDX+0x4C]
	$+3A2    0097CD52     83EC 0C                    SUB ESP,0xC
	$+3A5    0097CD55     8BC4                       MOV EAX,ESP
	$+3A7    0097CD57     660FD600                   MOVQ QWORD PTR DS:[EAX],XMM0
	$+3AB    0097CD5B     8948 08                    MOV DWORD PTR DS:[EAX+0x8],ECX
	$+3AE    0097CD5E     8BCE                       MOV ECX,ESI
	$+3B0    0097CD60     FFD2                       CALL EDX
	$+3B2    0097CD62     F3:0F1005 34267401         MOVSS XMM0,DWORD PTR DS:[0x1742634]
	$+3BA    0097CD6A     F3:0F1145 08               MOVSS DWORD PTR SS:[EBP+0x8],XMM0
	$+3BF    0097CD6F     F3:0F1005 38267401         MOVSS XMM0,DWORD PTR DS:[0x1742638]
	$+3C7    0097CD77     F3:0F1145 0C               MOVSS DWORD PTR SS:[EBP+0xC],XMM0
	$+3CC    0097CD7C     F3:0F1005 3C267401         MOVSS XMM0,DWORD PTR DS:[0x174263C]
	$+3D4    0097CD84     F3:0F1145 10               MOVSS DWORD PTR SS:[EBP+0x10],XMM0
	$+3D9    0097CD89     F3:0F1045 E0               MOVSS XMM0,DWORD PTR SS:[EBP-0x20]
	$+3DE    0097CD8E     F3:0F1145 C0               MOVSS DWORD PTR SS:[EBP-0x40],XMM0
	$+3E3    0097CD93     F3:0F1045 E4               MOVSS XMM0,DWORD PTR SS:[EBP-0x1C]
	$+3E8    0097CD98     F3:0F1145 C4               MOVSS DWORD PTR SS:[EBP-0x3C],XMM0
	$+3ED    0097CD9D     F3:0F1045 E8               MOVSS XMM0,DWORD PTR SS:[EBP-0x18]
	$+3F2    0097CDA2     F3:0F1145 C8               MOVSS DWORD PTR SS:[EBP-0x38],XMM0
	$+3F7    0097CDA7     8B4D C8                    MOV ECX,DWORD PTR SS:[EBP-0x38]
	$+3FA    0097CDAA     F30F7E45 C0                MOVQ XMM0,QWORD PTR SS:[EBP-0x40]
	$+3FF    0097CDAF     83EC 0C                    SUB ESP,0xC
	$+402    0097CDB2     8BC4                       MOV EAX,ESP
	$+404    0097CDB4     660FD600                   MOVQ QWORD PTR DS:[EAX],XMM0
	$+408    0097CDB8     8948 08                    MOV DWORD PTR DS:[EAX+0x8],ECX
	$+40B    0097CDBB     8BCE                       MOV ECX,ESI
	$+40D    0097CDBD     E8 EEE3E6FF                CALL MapleSto.007EB1B0
	$+412    0097CDC2     8B0D 744E7201              MOV ECX,DWORD PTR DS:[0x1724E74]
	$+418    0097CDC8     E8 C399C7FF                CALL MapleSto.005F6790
	$+41D    0097CDCD     85C0                       TEST EAX,EAX
	$+41F    0097CDCF     74 17                      JE SHORT MapleSto.0097CDE8
	$+421    0097CDD1     8B0D 744E7201              MOV ECX,DWORD PTR DS:[0x1724E74]
	$+427    0097CDD7     39B1 8C010000              CMP DWORD PTR DS:[ECX+0x18C],ESI
	$+42D    0097CDDD     75 1C                      JNZ SHORT MapleSto.0097CDFB
	$+42F    0097CDDF     E8 BC93C7FF                CALL MapleSto.005F61A0
	$+434    0097CDE4     84C0                       TEST AL,AL
	$+436    0097CDE6     74 13                      JE SHORT MapleSto.0097CDFB
	$+438    0097CDE8     8B16                       MOV EDX,DWORD PTR DS:[ESI]
	$+43A    0097CDEA     D945 18                    FLD DWORD PTR SS:[EBP+0x18]
	$+43D    0097CDED     8B82 F8000000              MOV EAX,DWORD PTR DS:[EDX+0xF8]
	$+443    0097CDF3     51                         PUSH ECX
	$+444    0097CDF4     8BCE                       MOV ECX,ESI
	$+446    0097CDF6     D91C24                     FSTP DWORD PTR SS:[ESP]
	$+449    0097CDF9     FFD0                       CALL EAX
	$+44B    0097CDFB     8B16                       MOV EDX,DWORD PTR DS:[ESI]
	$+44D    0097CDFD     8B52 48                    MOV EDX,DWORD PTR DS:[EDX+0x48]
	$+450    0097CE00     8D45 84                    LEA EAX,DWORD PTR SS:[EBP-0x7C]
	$+453    0097CE03     50                         PUSH EAX
	$+454    0097CE04     8BCE                       MOV ECX,ESI
	$+456    0097CE06     FFD2                       CALL EDX
	$+458    0097CE08     D900                       FLD DWORD PTR DS:[EAX]
	$+45A    0097CE0A     D99E CC110000              FSTP DWORD PTR DS:[ESI+0x11CC]
	$+460    0097CE10     D940 04                    FLD DWORD PTR DS:[EAX+0x4]
	$+463    0097CE13     D99E D0110000              FSTP DWORD PTR DS:[ESI+0x11D0]
	$+469    0097CE19     D940 08                    FLD DWORD PTR DS:[EAX+0x8]
	$+46C    0097CE1C     D99E D4110000              FSTP DWORD PTR DS:[ESI+0x11D4]
	$+472    0097CE22     8B4D F4                    MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+475    0097CE25     64:890D 00000000           MOV DWORD PTR FS:[0],ECX
	$+47C    0097CE2C     59                         POP ECX
	$+47D    0097CE2D     5F                         POP EDI
	$+47E    0097CE2E     5E                         POP ESI
	$+47F    0097CE2F     5B                         POP EBX
	$+480    0097CE30     8BE5                       MOV ESP,EBP
	$+482    0097CE32     5D                         POP EBP
	$+483    0097CE33     C2 2000                    RETN 0x20
	$+486    0097CE36     CC                         INT3
	*/
	addr_tmp = scan_exe_.ScanAddr("f3??????????????f3????????f3??????????????f3????????f3??????????????f3????????89??????????89??????????f3??????????????f3????????f3??????????????f3", 0);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = AsDword(addr_tmp, 4);
	assert(dw_tmp > 0x1000 && dw_tmp < 0x1500);
	ofs << "pt_cdword	g_o_game_obj_cube_pos_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	dw_tmp = AsDword(addr_tmp, 0x58 + 2);
	assert(dw_tmp > 0xa0 && dw_tmp < 0x100);
	ofs << "pt_cdword	g_o_model_control_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;
	return true;
}

bool CAutoUpdateApp::Scan_Item(Cmn_FC_OFstream& ofs)
{
	//pt_pcvoid addr_tmp, addr_tmp1;
	pt_dword dw_tmp;
	boost::optional<pt_dword> op_dw_tmp;
	/*
	$-267    00AB6ADF     CC                    INT3
	$-266    00AB6AE0     55                    PUSH EBP
	$-265    00AB6AE1     8BEC                  MOV EBP,ESP
	$-263    00AB6AE3     6A FF                 PUSH -0x1
	$-261    00AB6AE5     68 FB903801           PUSH MapleSto.013890FB
	$-25C    00AB6AEA     64:A1 00000000        MOV EAX,DWORD PTR FS:[0]
	$-256    00AB6AF0     50                    PUSH EAX
	$-255    00AB6AF1     83EC 0C               SUB ESP,0xC
	$-252    00AB6AF4     56                    PUSH ESI
	$-251    00AB6AF5     A1 D0687101           MOV EAX,DWORD PTR DS:[0x17168D0]
	$-24C    00AB6AFA     33C5                  XOR EAX,EBP
	$-24A    00AB6AFC     50                    PUSH EAX
	$-249    00AB6AFD     8D45 F4               LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-246    00AB6B00     64:A3 00000000        MOV DWORD PTR FS:[0],EAX
	$-240    00AB6B06     8BF1                  MOV ESI,ECX
	$-23E    00AB6B08     E8 B323D9FF           CALL MapleSto.00848EC0
	$-239    00AB6B0D     68 CC974301           PUSH MapleSto.014397CC                          ; ASCII "m_itemBag"
	$-234    00AB6B12     8D4D F0               LEA ECX,DWORD PTR SS:[EBP-0x10]
	$-231    00AB6B15     FF15 20A23E01         CALL DWORD PTR DS:[0x13EA220]                   ; mfc90.#310
	$-22B    00AB6B1B     8D45 F0               LEA EAX,DWORD PTR SS:[EBP-0x10]
	$-228    00AB6B1E     50                    PUSH EAX
	$-227    00AB6B1F     8D4D E8               LEA ECX,DWORD PTR SS:[EBP-0x18]
	$-224    00AB6B22     51                    PUSH ECX
	$-223    00AB6B23     8BCE                  MOV ECX,ESI
	$-221    00AB6B25     C745 FC 00000000      MOV DWORD PTR SS:[EBP-0x4],0x0
	$-21A    00AB6B2C     E8 4F12BFFF           CALL MapleSto.006A7D80
	$-215    00AB6B31     50                    PUSH EAX
	$-214    00AB6B32     8D8E 3C010000         LEA ECX,DWORD PTR DS:[ESI+0x13C]
	$-20E    00AB6B38     C645 FC 01            MOV BYTE PTR SS:[EBP-0x4],0x1
	$-20A    00AB6B3C     E8 DF6AAAFF           CALL MapleSto.0055D620
	$-205    00AB6B41     8B45 EC               MOV EAX,DWORD PTR SS:[EBP-0x14]
	$-202    00AB6B44     C645 FC 00            MOV BYTE PTR SS:[EBP-0x4],0x0
	$-1FE    00AB6B48     85C0                  TEST EAX,EAX
	$-1FC    00AB6B4A     74 31                 JE SHORT MapleSto.00AB6B7D
	$-1FA    00AB6B4C     83C0 04               ADD EAX,0x4
	$-1F7    00AB6B4F     50                    PUSH EAX
	$-1F6    00AB6B50     FF15 6C933E01         CALL DWORD PTR DS:[0x13E936C]
	$-1F0    00AB6B56     85C0                  TEST EAX,EAX
	$-1EE    00AB6B58     75 1C                 JNZ SHORT MapleSto.00AB6B76
	$-1EC    00AB6B5A     8B55 EC               MOV EDX,DWORD PTR SS:[EBP-0x14]
	$-1E9    00AB6B5D     83C2 04               ADD EDX,0x4
	$-1E6    00AB6B60     52                    PUSH EDX
	$-1E5    00AB6B61     FF15 F8923E01         CALL DWORD PTR DS:[0x13E92F8]
	$-1DF    00AB6B67     8B4D EC               MOV ECX,DWORD PTR SS:[EBP-0x14]
	$-1DC    00AB6B6A     85C9                  TEST ECX,ECX
	$-1DA    00AB6B6C     74 08                 JE SHORT MapleSto.00AB6B76
	$-1D8    00AB6B6E     8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$-1D6    00AB6B70     8B10                  MOV EDX,DWORD PTR DS:[EAX]
	$-1D4    00AB6B72     6A 01                 PUSH 0x1
	$-1D2    00AB6B74     FFD2                  CALL EDX
	$-1D0    00AB6B76     C745 EC 00000000      MOV DWORD PTR SS:[EBP-0x14],0x0
	$-1C9    00AB6B7D     8D4D F0               LEA ECX,DWORD PTR SS:[EBP-0x10]
	$-1C6    00AB6B80     C745 FC FFFFFFFF      MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-1BF    00AB6B87     FF15 08A23E01         CALL DWORD PTR DS:[0x13EA208]                   ; mfc90.#601
	$-1B9    00AB6B8D     8BCE                  MOV ECX,ESI
	$-1B7    00AB6B8F     E8 2CF7FFFF           CALL MapleSto.00AB62C0
	$-1B2    00AB6B94     8B4D F4               MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-1AF    00AB6B97     64:890D 00000000      MOV DWORD PTR FS:[0],ECX
	$-1A8    00AB6B9E     59                    POP ECX
	$-1A7    00AB6B9F     5E                    POP ESI
	$-1A6    00AB6BA0     8BE5                  MOV ESP,EBP
	$-1A4    00AB6BA2     5D                    POP EBP
	$-1A3    00AB6BA3     C3                    RETN
	$-1A2    00AB6BA4     CC                    INT3
	$-1A1    00AB6BA5     CC                    INT3
	$-1A0    00AB6BA6     CC                    INT3
	$-19F    00AB6BA7     CC                    INT3
	$-19E    00AB6BA8     CC                    INT3
	$-19D    00AB6BA9     CC                    INT3
	$-19C    00AB6BAA     CC                    INT3
	$-19B    00AB6BAB     CC                    INT3
	$-19A    00AB6BAC     CC                    INT3
	$-199    00AB6BAD     CC                    INT3
	$-198    00AB6BAE     CC                    INT3
	$-197    00AB6BAF     CC                    INT3
	$-196    00AB6BB0     55                    PUSH EBP
	$-195    00AB6BB1     8BEC                  MOV EBP,ESP
	$-193    00AB6BB3     6A FF                 PUSH -0x1
	$-191    00AB6BB5     68 27913801           PUSH MapleSto.01389127
	$-18C    00AB6BBA     64:A1 00000000        MOV EAX,DWORD PTR FS:[0]
	$-186    00AB6BC0     50                    PUSH EAX
	$-185    00AB6BC1     83EC 2C               SUB ESP,0x2C
	$-182    00AB6BC4     56                    PUSH ESI
	$-181    00AB6BC5     57                    PUSH EDI
	$-180    00AB6BC6     A1 D0687101           MOV EAX,DWORD PTR DS:[0x17168D0]
	$-17B    00AB6BCB     33C5                  XOR EAX,EBP
	$-179    00AB6BCD     50                    PUSH EAX
	$-178    00AB6BCE     8D45 F4               LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-175    00AB6BD1     64:A3 00000000        MOV DWORD PTR FS:[0],EAX
	$-16F    00AB6BD7     8BF1                  MOV ESI,ECX
	$-16D    00AB6BD9     E8 0236FDFF           CALL MapleSto.00A8A1E0
	$-168    00AB6BDE     6A 00                 PUSH 0x0
	$-166    00AB6BE0     51                    PUSH ECX
	$-165    00AB6BE1     8BCC                  MOV ECX,ESP
	$-163    00AB6BE3     8965 EC               MOV DWORD PTR SS:[EBP-0x14],ESP
	$-160    00AB6BE6     68 B0E94201           PUSH MapleSto.0142E9B0                          ; ASCII "m_webGuideButton"
	$-15B    00AB6BEB     FF15 20A23E01         CALL DWORD PTR DS:[0x13EA220]                   ; mfc90.#310
	$-155    00AB6BF1     8BCE                  MOV ECX,ESI
	$-153    00AB6BF3     E8 4847FDFF           CALL MapleSto.00A8B340
	$-14E    00AB6BF8     8B46 20               MOV EAX,DWORD PTR DS:[ESI+0x20]
	$-14B    00AB6BFB     85C0                  TEST EAX,EAX
	$-149    00AB6BFD     0F84 36010000         JE MapleSto.00AB6D39
	$-143    00AB6C03     68 AC884301           PUSH MapleSto.014388AC                          ; ASCII "m_inputSearch"
	$-13E    00AB6C08     8D4D F0               LEA ECX,DWORD PTR SS:[EBP-0x10]
	$-13B    00AB6C0B     FF15 20A23E01         CALL DWORD PTR DS:[0x13EA220]                   ; mfc90.#310
	$-135    00AB6C11     8B4E 20               MOV ECX,DWORD PTR DS:[ESI+0x20]
	$-132    00AB6C14     8D86 50010000         LEA EAX,DWORD PTR DS:[ESI+0x150]
	$-12C    00AB6C1A     50                    PUSH EAX
	$-12B    00AB6C1B     8D55 F0               LEA EDX,DWORD PTR SS:[EBP-0x10]
	$-128    00AB6C1E     52                    PUSH EDX
	$-127    00AB6C1F     8D45 E8               LEA EAX,DWORD PTR SS:[EBP-0x18]
	$-124    00AB6C22     50                    PUSH EAX
	$-123    00AB6C23     C745 FC 00000000      MOV DWORD PTR SS:[EBP-0x4],0x0
	$-11C    00AB6C2A     E8 D171C2FF           CALL MapleSto.006DDE00
	$-117    00AB6C2F     8B78 04               MOV EDI,DWORD PTR DS:[EAX+0x4]
	$-114    00AB6C32     8B45 EC               MOV EAX,DWORD PTR SS:[EBP-0x14]
	$-111    00AB6C35     85C0                  TEST EAX,EAX
	$-10F    00AB6C37     74 31                 JE SHORT MapleSto.00AB6C6A
	$-10D    00AB6C39     83C0 04               ADD EAX,0x4
	$-10A    00AB6C3C     50                    PUSH EAX
	$-109    00AB6C3D     FF15 6C933E01         CALL DWORD PTR DS:[0x13E936C]
	$-103    00AB6C43     85C0                  TEST EAX,EAX
	$-101    00AB6C45     75 1C                 JNZ SHORT MapleSto.00AB6C63
	$-FF     00AB6C47     8B4D EC               MOV ECX,DWORD PTR SS:[EBP-0x14]
	$-FC     00AB6C4A     83C1 04               ADD ECX,0x4
	$-F9     00AB6C4D     51                    PUSH ECX
	$-F8     00AB6C4E     FF15 F8923E01         CALL DWORD PTR DS:[0x13E92F8]
	$-F2     00AB6C54     8B4D EC               MOV ECX,DWORD PTR SS:[EBP-0x14]
	$-EF     00AB6C57     85C9                  TEST ECX,ECX
	$-ED     00AB6C59     74 08                 JE SHORT MapleSto.00AB6C63
	$-EB     00AB6C5B     8B11                  MOV EDX,DWORD PTR DS:[ECX]
	$-E9     00AB6C5D     8B02                  MOV EAX,DWORD PTR DS:[EDX]
	$-E7     00AB6C5F     6A 01                 PUSH 0x1
	$-E5     00AB6C61     FFD0                  CALL EAX
	$-E3     00AB6C63     C745 EC 00000000      MOV DWORD PTR SS:[EBP-0x14],0x0
	$-DC     00AB6C6A     8D4D F0               LEA ECX,DWORD PTR SS:[EBP-0x10]
	$-D9     00AB6C6D     C745 FC FFFFFFFF      MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-D2     00AB6C74     FF15 08A23E01         CALL DWORD PTR DS:[0x13EA208]                   ; mfc90.#601
	$-CC     00AB6C7A     85FF                  TEST EDI,EDI
	$-CA     00AB6C7C     74 4A                 JE SHORT MapleSto.00AB6CC8
	$-C8     00AB6C7E     8D4D E0               LEA ECX,DWORD PTR SS:[EBP-0x20]
	$-C5     00AB6C81     51                    PUSH ECX
	$-C4     00AB6C82     8D55 F0               LEA EDX,DWORD PTR SS:[EBP-0x10]
	$-C1     00AB6C85     52                    PUSH EDX
	$-C0     00AB6C86     8D4D EC               LEA ECX,DWORD PTR SS:[EBP-0x14]
	$-BD     00AB6C89     C745 E0 A066AB00      MOV DWORD PTR SS:[EBP-0x20],MapleSto.00AB66A0
	$-B6     00AB6C90     C745 E4 00000000      MOV DWORD PTR SS:[EBP-0x1C],0x0
	$-AF     00AB6C97     8975 F0               MOV DWORD PTR SS:[EBP-0x10],ESI
	$-AC     00AB6C9A     E8 D1060000           CALL MapleSto.00AB7370
	$-A7     00AB6C9F     8B8E 54010000         MOV ECX,DWORD PTR DS:[ESI+0x154]
	$-A1     00AB6CA5     50                    PUSH EAX
	$-A0     00AB6CA6     C745 FC 01000000      MOV DWORD PTR SS:[EBP-0x4],0x1
	$-99     00AB6CAD     E8 8E504300           CALL MapleSto.00EEBD40
	$-94     00AB6CB2     8B4D EC               MOV ECX,DWORD PTR SS:[EBP-0x14]
	$-91     00AB6CB5     C745 FC FFFFFFFF      MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-8A     00AB6CBC     85C9                  TEST ECX,ECX
	$-88     00AB6CBE     74 08                 JE SHORT MapleSto.00AB6CC8
	$-86     00AB6CC0     8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$-84     00AB6CC2     8B10                  MOV EDX,DWORD PTR DS:[EAX]
	$-82     00AB6CC4     6A 01                 PUSH 0x1
	$-80     00AB6CC6     FFD2                  CALL EDX
	$-7E     00AB6CC8     8BCE                  MOV ECX,ESI
	$-7C     00AB6CCA     E8 11ECFFFF           CALL MapleSto.00AB58E0
	$-77     00AB6CCF     8B86 1C010000         MOV EAX,DWORD PTR DS:[ESI+0x11C]
	$-71     00AB6CD5     33FF                  XOR EDI,EDI
	$-6F     00AB6CD7     3BC7                  CMP EAX,EDI
	$-6D     00AB6CD9     7C 1F                 JL SHORT MapleSto.00AB6CFA
	$-6B     00AB6CDB     3B86 30010000         CMP EAX,DWORD PTR DS:[ESI+0x130]
	$-65     00AB6CE1     7D 17                 JGE SHORT MapleSto.00AB6CFA
	$-63     00AB6CE3     72 0A                 JB SHORT MapleSto.00AB6CEF
	$-61     00AB6CE5     68 57000780           PUSH 0x80070057
	$-5C     00AB6CEA     E8 31AE94FF           CALL MapleSto.00401B20
	$-57     00AB6CEF     8B8E 2C010000         MOV ECX,DWORD PTR DS:[ESI+0x12C]
	$-51     00AB6CF5     8B0481                MOV EAX,DWORD PTR DS:[ECX+EAX*4]
	$-4E     00AB6CF8     EB 02                 JMP SHORT MapleSto.00AB6CFC
	$-4C     00AB6CFA     33C0                  XOR EAX,EAX
	$-4A     00AB6CFC     0FB615 38967301       MOVZX EDX,BYTE PTR DS:[0x1739638]
	$-43     00AB6D03     8B4E 20               MOV ECX,DWORD PTR DS:[ESI+0x20]
	$-40     00AB6D06     52                    PUSH EDX
	$-3F     00AB6D07     50                    PUSH EAX
	$-3E     00AB6D08     68 A0E24201           PUSH MapleSto.0142E2A0                          ; ASCII "initData"
	$-39     00AB6D0D     8D45 C8               LEA EAX,DWORD PTR SS:[EBP-0x38]
	$-36     00AB6D10     50                    PUSH EAX
	$-35     00AB6D11     E8 BA0EFEFF           CALL MapleSto.00A97BD0
	$-30     00AB6D16     8B4D CC               MOV ECX,DWORD PTR SS:[EBP-0x34]
	$-2D     00AB6D19     C1E9 06               SHR ECX,0x6
	$-2A     00AB6D1C     F6C1 01               TEST CL,0x1
	$-27     00AB6D1F     74 15                 JE SHORT MapleSto.00AB6D36
	$-25     00AB6D21     8B45 D0               MOV EAX,DWORD PTR SS:[EBP-0x30]
	$-22     00AB6D24     8B4D C8               MOV ECX,DWORD PTR SS:[EBP-0x38]
	$-1F     00AB6D27     8B11                  MOV EDX,DWORD PTR DS:[ECX]
	$-1D     00AB6D29     8B52 08               MOV EDX,DWORD PTR DS:[EDX+0x8]
	$-1A     00AB6D2C     50                    PUSH EAX
	$-19     00AB6D2D     8D45 C8               LEA EAX,DWORD PTR SS:[EBP-0x38]
	$-16     00AB6D30     50                    PUSH EAX
	$-15     00AB6D31     FFD2                  CALL EDX
	$-13     00AB6D33     897D C8               MOV DWORD PTR SS:[EBP-0x38],EDI
	$-10     00AB6D36     897D CC               MOV DWORD PTR SS:[EBP-0x34],EDI
	$-D      00AB6D39     838E 20010000 01      OR DWORD PTR DS:[ESI+0x120],0x1
	$-6      00AB6D40     8B15 644A7201         MOV EDX,DWORD PTR DS:[0x1724A64]                ; 金币的高位
	$ ==>    00AB6D46     8B86 04010000         MOV EAX,DWORD PTR DS:[ESI+0x104]
	$+6      00AB6D4C     8B40 2C               MOV EAX,DWORD PTR DS:[EAX+0x2C]
	$+9      00AB6D4F     8D8E 04010000         LEA ECX,DWORD PTR DS:[ESI+0x104]
	$+F      00AB6D55     52                    PUSH EDX
	$+10     00AB6D56     8B15 604A7201         MOV EDX,DWORD PTR DS:[0x1724A60]                ; g_a_gold_
	$+16     00AB6D5C     52                    PUSH EDX
	$+17     00AB6D5D     FFD0                  CALL EAX
	$+19     00AB6D5F     8B4D F4               MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+1C     00AB6D62     64:890D 00000000      MOV DWORD PTR FS:[0],ECX
	$+23     00AB6D69     59                    POP ECX
	$+24     00AB6D6A     5F                    POP EDI
	$+25     00AB6D6B     5E                    POP ESI
	$+26     00AB6D6C     8BE5                  MOV ESP,EBP
	$+28     00AB6D6E     5D                    POP EBP
	$+29     00AB6D6F     C3                    RETN
	$+2A     00AB6D70     55                    PUSH EBP
	$+2B     00AB6D71     8BEC                  MOV EBP,ESP
	$+2D     00AB6D73     6A FF                 PUSH -0x1
	$+2F     00AB6D75     68 65913801           PUSH MapleSto.01389165
	$+34     00AB6D7A     64:A1 00000000        MOV EAX,DWORD PTR FS:[0]
	$+3A     00AB6D80     50                    PUSH EAX
	$+3B     00AB6D81     83EC 28               SUB ESP,0x28
	$+3E     00AB6D84     56                    PUSH ESI
	$+3F     00AB6D85     A1 D0687101           MOV EAX,DWORD PTR DS:[0x17168D0]
	$+44     00AB6D8A     33C5                  XOR EAX,EBP
	$+46     00AB6D8C     50                    PUSH EAX
	$+47     00AB6D8D     8D45 F4               LEA EAX,DWORD PTR SS:[EBP-0xC]
	$+4A     00AB6D90     64:A3 00000000        MOV DWORD PTR FS:[0],EAX
	$+50     00AB6D96     8BF1                  MOV ESI,ECX
	$+52     00AB6D98     8D4D 08               LEA ECX,DWORD PTR SS:[EBP+0x8]
	$+55     00AB6D9B     C745 FC 00000000      MOV DWORD PTR SS:[EBP-0x4],0x0
	$+5C     00AB6DA2     FF15 C4A13E01         CALL DWORD PTR DS:[0x13EA1C4]                   ; mfc90.#4311
	$+62     00AB6DA8     3C 01                 CMP AL,0x1
	$+64     00AB6DAA     75 1D                 JNZ SHORT MapleSto.00AB6DC9
	$+66     00AB6DAC     51                    PUSH ECX
	$+67     00AB6DAD     8BCC                  MOV ECX,ESP
	$+69     00AB6DAF     8965 F0               MOV DWORD PTR SS:[EBP-0x10],ESP
	$+6C     00AB6DB2     68 50303F01           PUSH MapleSto.013F3050
	$+71     00AB6DB7     FF15 D0A13E01         CALL DWORD PTR DS:[0x13EA1D0]                   ; mfc90.#286
	$+77     00AB6DBD     8BCE                  MOV ECX,ESI
	$+79     00AB6DBF     E8 ECF6FFFF           CALL MapleSto.00AB64B0
	$+7E     00AB6DC4     E9 9C000000           JMP MapleSto.00AB6E65
	$+83     00AB6DC9     68 2D4E0000           PUSH 0x4E2D
	$+88     00AB6DCE     8D4D CC               LEA ECX,DWORD PTR SS:[EBP-0x34]
	$+8B     00AB6DD1     E8 8A5D95FF           CALL MapleSto.0040CB60
	$+90     00AB6DD6     6A 00                 PUSH 0x0
	$+92     00AB6DD8     51                    PUSH ECX
	$+93     00AB6DD9     8D45 08               LEA EAX,DWORD PTR SS:[EBP+0x8]
	$+96     00AB6DDC     8BCC                  MOV ECX,ESP
	$+98     00AB6DDE     8965 F0               MOV DWORD PTR SS:[EBP-0x10],ESP
	$+9B     00AB6DE1     50                    PUSH EAX
	$+9C     00AB6DE2     C645 FC 01            MOV BYTE PTR SS:[EBP-0x4],0x1
	$+A0     00AB6DE6     FF15 C8A13E01         CALL DWORD PTR DS:[0x13EA1C8]                   ; mfc90.#280
	$+A6     00AB6DEC     51                    PUSH ECX
	$+A7     00AB6DED     8BCC                  MOV ECX,ESP
	$+A9     00AB6DEF     8965 EC               MOV DWORD PTR SS:[EBP-0x14],ESP
	$+AC     00AB6DF2     68 AC884301           PUSH MapleSto.014388AC                          ; ASCII "m_inputSearch"
	$+B1     00AB6DF7     C645 FC 02            MOV BYTE PTR SS:[EBP-0x4],0x2
	$+B5     00AB6DFB     FF15 20A23E01         CALL DWORD PTR DS:[0x13EA220]                   ; mfc90.#310
	$+BB     00AB6E01     8D4D CC               LEA ECX,DWORD PTR SS:[EBP-0x34]
	$+BE     00AB6E04     C645 FC 01            MOV BYTE PTR SS:[EBP-0x4],0x1
	$+C2     00AB6E08     E8 235E95FF           CALL MapleSto.0040CC30
	$+C7     00AB6E0D     8B0D 34277201         MOV ECX,DWORD PTR DS:[0x1722734]
	$+CD     00AB6E13     8B11                  MOV EDX,DWORD PTR DS:[ECX]
	$+CF     00AB6E15     8B92 88000000         MOV EDX,DWORD PTR DS:[EDX+0x88]
	$+D5     00AB6E1B     8D45 CC               LEA EAX,DWORD PTR SS:[EBP-0x34]
	$+D8     00AB6E1E     50                    PUSH EAX
	$+D9     00AB6E1F     FFD2                  CALL EDX
	$+DB     00AB6E21     83F8 01               CMP EAX,0x1
	$+DE     00AB6E24     75 33                 JNZ SHORT MapleSto.00AB6E59
	$+E0     00AB6E26     8D45 08               LEA EAX,DWORD PTR SS:[EBP+0x8]
	$+E3     00AB6E29     50                    PUSH EAX
	$+E4     00AB6E2A     8D4D F0               LEA ECX,DWORD PTR SS:[EBP-0x10]
	$+E7     00AB6E2D     51                    PUSH ECX
	$+E8     00AB6E2E     E8 8DA6DFFF           CALL MapleSto.008B14C0
	$+ED     00AB6E33     83C4 04               ADD ESP,0x4
	$+F0     00AB6E36     8D55 F0               LEA EDX,DWORD PTR SS:[EBP-0x10]
	$+F3     00AB6E39     8BCC                  MOV ECX,ESP
	$+F5     00AB6E3B     8965 EC               MOV DWORD PTR SS:[EBP-0x14],ESP
	$+F8     00AB6E3E     52                    PUSH EDX
	$+F9     00AB6E3F     C645 FC 03            MOV BYTE PTR SS:[EBP-0x4],0x3
	$+FD     00AB6E43     FF15 C8A13E01         CALL DWORD PTR DS:[0x13EA1C8]                   ; mfc90.#280
	$+103    00AB6E49     8BCE                  MOV ECX,ESI
	$+105    00AB6E4B     E8 60F6FFFF           CALL MapleSto.00AB64B0
	$+10A    00AB6E50     8D4D F0               LEA ECX,DWORD PTR SS:[EBP-0x10]
	$+10D    00AB6E53     FF15 DCA13E01         CALL DWORD PTR DS:[0x13EA1DC]                   ; mfc90.#601
	$+113    00AB6E59     8D4D CC               LEA ECX,DWORD PTR SS:[EBP-0x34]
	$+116    00AB6E5C     C645 FC 00            MOV BYTE PTR SS:[EBP-0x4],0x0
	$+11A    00AB6E60     E8 3B5D95FF           CALL MapleSto.0040CBA0
	$+11F    00AB6E65     8D4D 08               LEA ECX,DWORD PTR SS:[EBP+0x8]
	$+122    00AB6E68     FF15 DCA13E01         CALL DWORD PTR DS:[0x13EA1DC]                   ; mfc90.#601
	$+128    00AB6E6E     8B4D F4               MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+12B    00AB6E71     64:890D 00000000      MOV DWORD PTR FS:[0],ECX
	$+132    00AB6E78     59                    POP ECX
	$+133    00AB6E79     5E                    POP ESI
	$+134    00AB6E7A     8BE5                  MOV ESP,EBP
	$+136    00AB6E7C     5D                    POP EBP
	$+137    00AB6E7D     C2 0400               RETN 0x4
	$+13A    00AB6E80     55                    PUSH EBP
	$+13B    00AB6E81     8BEC                  MOV EBP,ESP
	$+13D    00AB6E83     51                    PUSH ECX
	$+13E    00AB6E84     56                    PUSH ESI
	$+13F    00AB6E85     8BF1                  MOV ESI,ECX
	$+141    00AB6E87     8B06                  MOV EAX,DWORD PTR DS:[ESI]
	$+143    00AB6E89     8B90 D0000000         MOV EDX,DWORD PTR DS:[EAX+0xD0]
	$+149    00AB6E8F     57                    PUSH EDI
	$+14A    00AB6E90     FFD2                  CALL EDX
	$+14C    00AB6E92     85C0                  TEST EAX,EAX
	$+14E    00AB6E94     0F85 C4000000         JNZ MapleSto.00AB6F5E
	$+154    00AB6E9A     8BCE                  MOV ECX,ESI
	$+156    00AB6E9C     E8 FF37FDFF           CALL MapleSto.00A8A6A0
	$+15B    00AB6EA1     85C0                  TEST EAX,EAX
	$+15D    00AB6EA3     0F85 A1000000         JNZ MapleSto.00AB6F4A
	$+163    00AB6EA9     8B0D 2C407201         MOV ECX,DWORD PTR DS:[0x172402C]
	$+169    00AB6EAF     50                    PUSH EAX
	$+16A    00AB6EB0     50                    PUSH EAX
	$+16B    00AB6EB1     6A 1C                 PUSH 0x1C
	$+16D    00AB6EB3     E8 88C7CEFF           CALL MapleSto.007A3640
	$+172    00AB6EB8     84C0                  TEST AL,AL
	$+174    00AB6EBA     0F84 9E000000         JE MapleSto.00AB6F5E
	$+17A    00AB6EC0     8B46 20               MOV EAX,DWORD PTR DS:[ESI+0x20]
	$+17D    00AB6EC3     6A 00                 PUSH 0x0
	$+17F    00AB6EC5     68 3C206D01           PUSH MapleSto.016D203C
	$+184    00AB6ECA     68 E4BA6C01           PUSH MapleSto.016CBAE4
	$+189    00AB6ECF     6A 00                 PUSH 0x0
	$+18B    00AB6ED1     50                    PUSH EAX
	$+18C    00AB6ED2     E8 85AE7400           CALL MapleSto.01201D5C                          ; JMP to msvcr90.__RTDynamicCast
	$+191    00AB6ED7     8BF8                  MOV EDI,EAX
	$+193    00AB6ED9     83C4 14               ADD ESP,0x14
	$+196    00AB6EDC     85FF                  TEST EDI,EDI
	$+198    00AB6EDE     74 7E                 JE SHORT MapleSto.00AB6F5E
	$+19A    00AB6EE0     833D 54877201 00      CMP DWORD PTR DS:[0x1728754],0x0
	$+1A1    00AB6EE7     74 1F                 JE SHORT MapleSto.00AB6F08
	$+1A3    00AB6EE9     51                    PUSH ECX
	$+1A4    00AB6EEA     8D46 10               LEA EAX,DWORD PTR DS:[ESI+0x10]
	$+1A7    00AB6EED     8BCC                  MOV ECX,ESP
	$+1A9    00AB6EEF     8965 FC               MOV DWORD PTR SS:[EBP-0x4],ESP
	$+1AC    00AB6EF2     50                    PUSH EAX
	$+1AD    00AB6EF3     FF15 ECA13E01         CALL DWORD PTR DS:[0x13EA1EC]                   ; mfc90.#300
	$+1B3    00AB6EF9     8B0D 54877201         MOV ECX,DWORD PTR DS:[0x1728754]
	$+1B9    00AB6EFF     E8 EC7F4300           CALL MapleSto.00EEEEF0
	$+1BE    00AB6F04     85C0                  TEST EAX,EAX
	$+1C0    00AB6F06     74 56                 JE SHORT MapleSto.00AB6F5E
	$+1C2    00AB6F08     8B8E 54010000         MOV ECX,DWORD PTR DS:[ESI+0x154]
	$+1C8    00AB6F0E     85C9                  TEST ECX,ECX
	$+1CA    00AB6F10     74 4C                 JE SHORT MapleSto.00AB6F5E
	$+1CC    00AB6F12     E8 191E4300           CALL MapleSto.00EE8D30
	$+1D1    00AB6F17     85C0                  TEST EAX,EAX
	$+1D3    00AB6F19     74 3A                 JE SHORT MapleSto.00AB6F55
	$+1D5    00AB6F1B     8B8E 54010000         MOV ECX,DWORD PTR DS:[ESI+0x154]
	$+1DB    00AB6F21     51                    PUSH ECX
	$+1DC    00AB6F22     8BD4                  MOV EDX,ESP
	$+1DE    00AB6F24     8965 FC               MOV DWORD PTR SS:[EBP-0x4],ESP
	$+1E1    00AB6F27     52                    PUSH EDX
	$+1E2    00AB6F28     E8 F34E4300           CALL MapleSto.00EEBE20
	$+1E7    00AB6F2D     8BCE                  MOV ECX,ESI
	$+1E9    00AB6F2F     E8 3CFEFFFF           CALL MapleSto.00AB6D70
	*/
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("8B86040100008B402C8D8E0401000052",
		0x10 + 2);
	if (!op_dw_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *op_dw_tmp;
	CheckExeAddr(dw_tmp);
	ofs << "pt_cdword	g_a_gold_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	/*
	$-FD     00AB516F      CC                    INT3
	$-FC     00AB5170      55                    PUSH EBP
	$-FB     00AB5171      8BEC                  MOV EBP,ESP
	$-F9     00AB5173      6A FF                 PUSH -0x1
	$-F7     00AB5175      68 5D8D3801           PUSH MapleSto.01388D5D
	$-F2     00AB517A      64:A1 00000000        MOV EAX,DWORD PTR FS:[0]
	$-EC     00AB5180      50                    PUSH EAX
	$-EB     00AB5181      83EC 0C               SUB ESP,0xC
	$-E8     00AB5184      56                    PUSH ESI
	$-E7     00AB5185      A1 D0687101           MOV EAX,DWORD PTR DS:[0x17168D0]
	$-E2     00AB518A      33C5                  XOR EAX,EBP
	$-E0     00AB518C      50                    PUSH EAX
	$-DF     00AB518D      8D45 F4               LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-DC     00AB5190      64:A3 00000000        MOV DWORD PTR FS:[0],EAX
	$-D6     00AB5196      8BF1                  MOV ESI,ECX
	$-D4     00AB5198      8B86 1C010000         MOV EAX,DWORD PTR DS:[ESI+0x11C]
	$-CE     00AB519E      50                    PUSH EAX
	$-CD     00AB519F      E8 9CFCFFFF           CALL MapleSto.00AB4E40
	$-C8     00AB51A4      68 8CA14701           PUSH MapleSto.0147A18C                   ; ASCII "m_expandEff"
	$-C3     00AB51A9      8D4D F0               LEA ECX,DWORD PTR SS:[EBP-0x10]
	$-C0     00AB51AC      FF15 20A23E01         CALL DWORD PTR DS:[0x13EA220]            ; mfc90.#310
	$-BA     00AB51B2      8D4D F0               LEA ECX,DWORD PTR SS:[EBP-0x10]
	$-B7     00AB51B5      51                    PUSH ECX
	$-B6     00AB51B6      8D55 E8               LEA EDX,DWORD PTR SS:[EBP-0x18]
	$-B3     00AB51B9      52                    PUSH EDX
	$-B2     00AB51BA      8BCE                  MOV ECX,ESI
	$-B0     00AB51BC      C745 FC 00000000      MOV DWORD PTR SS:[EBP-0x4],0x0
	$-A9     00AB51C3      E8 C8F3BDFF           CALL MapleSto.00694590
	$-A4     00AB51C8      8D4D F0               LEA ECX,DWORD PTR SS:[EBP-0x10]
	$-A1     00AB51CB      C645 FC 02            MOV BYTE PTR SS:[EBP-0x4],0x2
	$-9D     00AB51CF      FF15 08A23E01         CALL DWORD PTR DS:[0x13EA208]            ; mfc90.#601
	$-97     00AB51D5      8B4D EC               MOV ECX,DWORD PTR SS:[EBP-0x14]
	$-94     00AB51D8      85C9                  TEST ECX,ECX
	$-92     00AB51DA      74 0C                 JE SHORT MapleSto.00AB51E8
	$-90     00AB51DC      8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$-8E     00AB51DE      8B50 28               MOV EDX,DWORD PTR DS:[EAX+0x28]
	$-8B     00AB51E1      6A 01                 PUSH 0x1
	$-89     00AB51E3      FFD2                  CALL EDX
	$-87     00AB51E5      8B4D EC               MOV ECX,DWORD PTR SS:[EBP-0x14]
	$-84     00AB51E8      C745 FC FFFFFFFF      MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-7D     00AB51EF      85C9                  TEST ECX,ECX
	$-7B     00AB51F1      74 2A                 JE SHORT MapleSto.00AB521D
	$-79     00AB51F3      83C1 04               ADD ECX,0x4
	$-76     00AB51F6      51                    PUSH ECX
	$-75     00AB51F7      FF15 6C933E01         CALL DWORD PTR DS:[0x13E936C]
	$-6F     00AB51FD      85C0                  TEST EAX,EAX
	$-6D     00AB51FF      75 1C                 JNZ SHORT MapleSto.00AB521D
	$-6B     00AB5201      8B45 EC               MOV EAX,DWORD PTR SS:[EBP-0x14]
	$-68     00AB5204      83C0 04               ADD EAX,0x4
	$-65     00AB5207      50                    PUSH EAX
	$-64     00AB5208      FF15 F8923E01         CALL DWORD PTR DS:[0x13E92F8]
	$-5E     00AB520E      8B4D EC               MOV ECX,DWORD PTR SS:[EBP-0x14]
	$-5B     00AB5211      85C9                  TEST ECX,ECX
	$-59     00AB5213      74 08                 JE SHORT MapleSto.00AB521D
	$-57     00AB5215      8B11                  MOV EDX,DWORD PTR DS:[ECX]
	$-55     00AB5217      8B02                  MOV EAX,DWORD PTR DS:[EDX]
	$-53     00AB5219      6A 01                 PUSH 0x1
	$-51     00AB521B      FFD0                  CALL EAX
	$-4F     00AB521D      8B4D F4               MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-4C     00AB5220      64:890D 00000000      MOV DWORD PTR FS:[0],ECX
	$-45     00AB5227      59                    POP ECX
	$-44     00AB5228      5E                    POP ESI
	$-43     00AB5229      8BE5                  MOV ESP,EBP
	$-41     00AB522B      5D                    POP EBP
	$-40     00AB522C      C3                    RETN
	$-3F     00AB522D      CC                    INT3
	$-3E     00AB522E      CC                    INT3
	$-3D     00AB522F      CC                    INT3
	$-3C     00AB5230      55                    PUSH EBP
	$-3B     00AB5231      8BEC                  MOV EBP,ESP
	$-39     00AB5233      6A FF                 PUSH -0x1
	$-37     00AB5235      68 908D3801           PUSH MapleSto.01388D90
	$-32     00AB523A      64:A1 00000000        MOV EAX,DWORD PTR FS:[0]
	$-2C     00AB5240      50                    PUSH EAX
	$-2B     00AB5241      83EC 28               SUB ESP,0x28
	$-28     00AB5244      53                    PUSH EBX
	$-27     00AB5245      56                    PUSH ESI
	$-26     00AB5246      57                    PUSH EDI
	$-25     00AB5247      A1 D0687101           MOV EAX,DWORD PTR DS:[0x17168D0]
	$-20     00AB524C      33C5                  XOR EAX,EBP
	$-1E     00AB524E      50                    PUSH EAX
	$-1D     00AB524F      8D45 F4               LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-1A     00AB5252      64:A3 00000000        MOV DWORD PTR FS:[0],EAX
	$-14     00AB5258      33F6                  XOR ESI,ESI
	$-12     00AB525A      8975 CC               MOV DWORD PTR SS:[EBP-0x34],ESI
	$-F      00AB525D      8975 D0               MOV DWORD PTR SS:[EBP-0x30],ESI
	$-C      00AB5260      8975 D4               MOV DWORD PTR SS:[EBP-0x2C],ESI
	$-9      00AB5263      8975 D8               MOV DWORD PTR SS:[EBP-0x28],ESI
	$-6      00AB5266      8B89 1C010000         MOV ECX,DWORD PTR DS:[ECX+0x11C]
	$ ==>    00AB526C      8D45 CC               LEA EAX,DWORD PTR SS:[EBP-0x34]
	$+3      00AB526F      50                    PUSH EAX
	$+4      00AB5270      51                    PUSH ECX
	$+5      00AB5271      B9 604B7201           MOV ECX,MapleSto.01724B60                ; g_a_bag_mgr_
	$+A      00AB5276      8975 FC               MOV DWORD PTR SS:[EBP-0x4],ESI
	$+D      00AB5279      E8 4223A0FF           CALL MapleSto.004B75C0
	$+12     00AB527E      8B45 D0               MOV EAX,DWORD PTR SS:[EBP-0x30]
	$+15     00AB5281      33DB                  XOR EBX,EBX
	$+17     00AB5283      8975 EC               MOV DWORD PTR SS:[EBP-0x14],ESI
	$+1A     00AB5286      895D F0               MOV DWORD PTR SS:[EBP-0x10],EBX
	$+1D     00AB5289      3BC6                  CMP EAX,ESI
	$+1F     00AB528B      0F86 CF000000         JBE MapleSto.00AB5360
	$+25     00AB5291      3BD8                  CMP EBX,EAX
	$+27     00AB5293      0F83 F7000000         JNB MapleSto.00AB5390
	$+2D     00AB5299      8B55 CC               MOV EDX,DWORD PTR SS:[EBP-0x34]
	$+30     00AB529C      8B7CDA 04             MOV EDI,DWORD PTR DS:[EDX+EBX*8+0x4]
	$+34     00AB52A0      897D E0               MOV DWORD PTR SS:[EBP-0x20],EDI
	$+37     00AB52A3      3BFE                  CMP EDI,ESI
	$+39     00AB52A5      74 0A                 JE SHORT MapleSto.00AB52B1
	$+3B     00AB52A7      8D47 04               LEA EAX,DWORD PTR DS:[EDI+0x4]
	$+3E     00AB52AA      50                    PUSH EAX
	$+3F     00AB52AB      FF15 F8923E01         CALL DWORD PTR DS:[0x13E92F8]
	$+45     00AB52B1      8BCF                  MOV ECX,EDI
	$+47     00AB52B3      C645 FC 01            MOV BYTE PTR SS:[EBP-0x4],0x1
	$+4B     00AB52B7      E8 F62B1501           CALL MapleSto.01C07EB2
	$+50     00AB52BC      8B70 18               MOV ESI,DWORD PTR DS:[EAX+0x18]
	$+53     00AB52BF      8975 E8               MOV DWORD PTR SS:[EBP-0x18],ESI
	$+56     00AB52C2      85F6                  TEST ESI,ESI
	$+58     00AB52C4      74 0A                 JE SHORT MapleSto.00AB52D0
	$+5A     00AB52C6      8D4E 04               LEA ECX,DWORD PTR DS:[ESI+0x4]
	$+5D     00AB52C9      51                    PUSH ECX
	$+5E     00AB52CA      FF15 F8923E01         CALL DWORD PTR DS:[0x13E92F8]
	$+64     00AB52D0      8BCE                  MOV ECX,ESI
	$+66     00AB52D2      C645 FC 02            MOV BYTE PTR SS:[EBP-0x4],0x2
	$+6A     00AB52D6      E8 742BA2FF           CALL MapleSto.004D7E4F
	$+6F     00AB52DB      85C0                  TEST EAX,EAX
	$+71     00AB52DD      74 11                 JE SHORT MapleSto.00AB52F0
	$+73     00AB52DF      837E 2C 00            CMP DWORD PTR DS:[ESI+0x2C],0x0
	$+77     00AB52E3      75 0B                 JNZ SHORT MapleSto.00AB52F0
	$+79     00AB52E5      B8 01000000           MOV EAX,0x1
	$+7E     00AB52EA      8946 2C               MOV DWORD PTR DS:[ESI+0x2C],EAX
	$+81     00AB52ED      8945 EC               MOV DWORD PTR SS:[EBP-0x14],EAX
	$+84     00AB52F0      C645 FC 01            MOV BYTE PTR SS:[EBP-0x4],0x1
	$+88     00AB52F4      85F6                  TEST ESI,ESI
	$+8A     00AB52F6      74 29                 JE SHORT MapleSto.00AB5321
	$+8C     00AB52F8      8D5E 04               LEA EBX,DWORD PTR DS:[ESI+0x4]
	$+8F     00AB52FB      53                    PUSH EBX
	$+90     00AB52FC      FF15 6C933E01         CALL DWORD PTR DS:[0x13E936C]
	$+96     00AB5302      85C0                  TEST EAX,EAX
	$+98     00AB5304      75 11                 JNZ SHORT MapleSto.00AB5317
	$+9A     00AB5306      53                    PUSH EBX
	$+9B     00AB5307      FF15 F8923E01         CALL DWORD PTR DS:[0x13E92F8]
	$+A1     00AB530D      8B16                  MOV EDX,DWORD PTR DS:[ESI]
	$+A3     00AB530F      8B02                  MOV EAX,DWORD PTR DS:[EDX]
	$+A5     00AB5311      6A 01                 PUSH 0x1
	$+A7     00AB5313      8BCE                  MOV ECX,ESI
	$+A9     00AB5315      FFD0                  CALL EAX
	$+AB     00AB5317      8B5D F0               MOV EBX,DWORD PTR SS:[EBP-0x10]
	$+AE     00AB531A      C745 E8 00000000      MOV DWORD PTR SS:[EBP-0x18],0x0
	$+B5     00AB5321      C645 FC 00            MOV BYTE PTR SS:[EBP-0x4],0x0
	$+B9     00AB5325      85FF                  TEST EDI,EDI
	$+BB     00AB5327      74 26                 JE SHORT MapleSto.00AB534F
	$+BD     00AB5329      8D77 04               LEA ESI,DWORD PTR DS:[EDI+0x4]
	$+C0     00AB532C      56                    PUSH ESI
	$+C1     00AB532D      FF15 6C933E01         CALL DWORD PTR DS:[0x13E936C]
	$+C7     00AB5333      85C0                  TEST EAX,EAX
	$+C9     00AB5335      75 11                 JNZ SHORT MapleSto.00AB5348
	$+CB     00AB5337      56                    PUSH ESI
	$+CC     00AB5338      FF15 F8923E01         CALL DWORD PTR DS:[0x13E92F8]
	$+D2     00AB533E      8B17                  MOV EDX,DWORD PTR DS:[EDI]
	$+D4     00AB5340      8B02                  MOV EAX,DWORD PTR DS:[EDX]
	$+D6     00AB5342      6A 01                 PUSH 0x1
	$+D8     00AB5344      8BCF                  MOV ECX,EDI
	$+DA     00AB5346      FFD0                  CALL EAX
	$+DC     00AB5348      C745 E0 00000000      MOV DWORD PTR SS:[EBP-0x20],0x0
	$+E3     00AB534F      8B45 D0               MOV EAX,DWORD PTR SS:[EBP-0x30]
	$+E6     00AB5352      43                    INC EBX
	$+E7     00AB5353      33F6                  XOR ESI,ESI
	$+E9     00AB5355      895D F0               MOV DWORD PTR SS:[EBP-0x10],EBX
	$+EC     00AB5358      3BD8                  CMP EBX,EAX
	$+EE     00AB535A    ^ 0F82 39FFFFFF         JB MapleSto.00AB5299
	$+F4     00AB5360      8B4D CC               MOV ECX,DWORD PTR SS:[EBP-0x34]
	$+F7     00AB5363      3BCE                  CMP ECX,ESI
	$+F9     00AB5365      74 14                 JE SHORT MapleSto.00AB537B
	$+FB     00AB5367      50                    PUSH EAX
	$+FC     00AB5368      51                    PUSH ECX
	$+FD     00AB5369      E8 727396FF           CALL MapleSto.0041C6E0
	$+102    00AB536E      8B4D CC               MOV ECX,DWORD PTR SS:[EBP-0x34]
	$+105    00AB5371      51                    PUSH ECX
	$+106    00AB5372      FF15 E8983E01         CALL DWORD PTR DS:[0x13E98E8]            ; msvcr90.free
	$+10C    00AB5378      83C4 0C               ADD ESP,0xC
	$+10F    00AB537B      8B45 EC               MOV EAX,DWORD PTR SS:[EBP-0x14]
	$+112    00AB537E      8B4D F4               MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+115    00AB5381      64:890D 00000000      MOV DWORD PTR FS:[0],ECX
	$+11C    00AB5388      59                    POP ECX
	$+11D    00AB5389      5F                    POP EDI
	$+11E    00AB538A      5E                    POP ESI
	$+11F    00AB538B      5B                    POP EBX
	$+120    00AB538C      8BE5                  MOV ESP,EBP
	$+122    00AB538E      5D                    POP EBP
	$+123    00AB538F      C3                    RETN
	$+124    00AB5390      68 57000780           PUSH 0x80070057
	$+129    00AB5395      E8 86C794FF           CALL MapleSto.00401B20
	$+12E    00AB539A      CC                    INT3
	$+12F    00AB539B      CC                    INT3
	$+130    00AB539C      CC                    INT3
	$+131    00AB539D      CC                    INT3
	$+132    00AB539E      CC                    INT3
	$+133    00AB539F      CC                    INT3
	$+134    00AB53A0      55                    PUSH EBP
	$+135    00AB53A1      8BEC                  MOV EBP,ESP
	$+137    00AB53A3      6A FF                 PUSH -0x1
	$+139    00AB53A5      68 C68D3801           PUSH MapleSto.01388DC6
	$+13E    00AB53AA      64:A1 00000000        MOV EAX,DWORD PTR FS:[0]
	$+144    00AB53B0      50                    PUSH EAX
	$+145    00AB53B1      81EC 80000000         SUB ESP,0x80
	$+14B    00AB53B7      53                    PUSH EBX
	$+14C    00AB53B8      56                    PUSH ESI
	$+14D    00AB53B9      57                    PUSH EDI
	$+14E    00AB53BA      A1 D0687101           MOV EAX,DWORD PTR DS:[0x17168D0]
	$+153    00AB53BF      33C5                  XOR EAX,EBP
	$+155    00AB53C1      50                    PUSH EAX
	$+156    00AB53C2      8D45 F4               LEA EAX,DWORD PTR SS:[EBP-0xC]
	$+159    00AB53C5      64:A3 00000000        MOV DWORD PTR FS:[0],EAX
	$+15F    00AB53CB      8BF9                  MOV EDI,ECX
	$+161    00AB53CD      8B45 08               MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+164    00AB53D0      50                    PUSH EAX
	$+165    00AB53D1      8D4D 9C               LEA ECX,DWORD PTR SS:[EBP-0x64]
	$+168    00AB53D4      E8 B7E04300           CALL MapleSto.00EF3490
	$+16D    00AB53D9      33DB                  XOR EBX,EBX
	$+16F    00AB53DB      8D4D 9C               LEA ECX,DWORD PTR SS:[EBP-0x64]
	$+172    00AB53DE      895D FC               MOV DWORD PTR SS:[EBP-0x4],EBX
	$+175    00AB53E1      E8 1B344401           CALL MapleSto.01EF8801
	$+17A    00AB53E6      3BC3                  CMP EAX,EBX
	$+17C    00AB53E8      0F8C AA000000         JL MapleSto.00AB5498
	$+182    00AB53EE      8D4D DC               LEA ECX,DWORD PTR SS:[EBP-0x24]
	$+185    00AB53F1      51                    PUSH ECX
	$+186    00AB53F2      8D4D 9C               LEA ECX,DWORD PTR SS:[EBP-0x64]
	$+189    00AB53F5      E8 66E14300           CALL MapleSto.00EF3560
	$+18E    00AB53FA      8B45 E0               MOV EAX,DWORD PTR SS:[EBP-0x20]
	$+191    00AB53FD      C645 FC 01            MOV BYTE PTR SS:[EBP-0x4],0x1
	$+195    00AB5401      A8 8F                 TEST AL,0x8F
	$+197    00AB5403      75 07                 JNZ SHORT MapleSto.00AB540C
	$+199    00AB5405      C1E8 06               SHR EAX,0x6
	$+19C    00AB5408      A8 01                 TEST AL,0x1
	$+19E    00AB540A      EB 6F                 JMP SHORT MapleSto.00AB547B
	$+1A0    00AB540C      6A 01                 PUSH 0x1
	$+1A2    00AB540E      8D45 DC               LEA EAX,DWORD PTR SS:[EBP-0x24]
	$+1A5    00AB5411      50                    PUSH EAX
	$+1A6    00AB5412      8D8D 74FFFFFF         LEA ECX,DWORD PTR SS:[EBP-0x8C]
	$+1AC    00AB5418      E8 D3E14200           CALL MapleSto.00EE35F0
	$+1B1    00AB541D      68 64AF4101           PUSH MapleSto.0141AF64                   ; ASCII "tabIndex"
	$+1B6    00AB5422      8D4D C4               LEA ECX,DWORD PTR SS:[EBP-0x3C]
	$+1B9    00AB5425      51                    PUSH ECX
	$+1BA    00AB5426      8D8D 74FFFFFF         LEA ECX,DWORD PTR SS:[EBP-0x8C]
	$+1C0    00AB542C      C645 FC 02            MOV BYTE PTR SS:[EBP-0x4],0x2
	$+1C4    00AB5430      E8 5BE24200           CALL MapleSto.00EE3690
	$+1C9    00AB5435      8B55 C8               MOV EDX,DWORD PTR SS:[EBP-0x38]
	$+1CC    00AB5438      8B70 08               MOV ESI,DWORD PTR DS:[EAX+0x8]
	$+1CF    00AB543B      C1EA 06               SHR EDX,0x6
	$+1D2    00AB543E      F6C2 01               TEST DL,0x1
	$+1D5    00AB5441      74 15                 JE SHORT MapleSto.00AB5458
	$+1D7    00AB5443      8B55 CC               MOV EDX,DWORD PTR SS:[EBP-0x34]
	$+1DA    00AB5446      8B4D C4               MOV ECX,DWORD PTR SS:[EBP-0x3C]
	$+1DD    00AB5449      8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$+1DF    00AB544B      8B40 08               MOV EAX,DWORD PTR DS:[EAX+0x8]
	$+1E2    00AB544E      52                    PUSH EDX
	$+1E3    00AB544F      8D55 C4               LEA EDX,DWORD PTR SS:[EBP-0x3C]
	$+1E6    00AB5452      52                    PUSH EDX
	$+1E7    00AB5453      FFD0                  CALL EAX
	$+1E9    00AB5455      895D C4               MOV DWORD PTR SS:[EBP-0x3C],EBX
	$+1EC    00AB5458      56                    PUSH ESI
	$+1ED    00AB5459      8BCF                  MOV ECX,EDI
	$+1EF    00AB545B      895D C8               MOV DWORD PTR SS:[EBP-0x38],EBX
	$+1F2    00AB545E      E8 DDF9FFFF           CALL MapleSto.00AB4E40
	$+1F7    00AB5463      8D8D 74FFFFFF         LEA ECX,DWORD PTR SS:[EBP-0x8C]
	$+1FD    00AB5469      C645 FC 01            MOV BYTE PTR SS:[EBP-0x4],0x1
	$+201    00AB546D      E8 9EE64200           CALL MapleSto.00EE3B10
	$+206    00AB5472      8B4D E0               MOV ECX,DWORD PTR SS:[EBP-0x20]
	$+209    00AB5475      C1E9 06               SHR ECX,0x6
	$+20C    00AB5478      F6C1 01               TEST CL,0x1
	$+20F    00AB547B      885D FC               MOV BYTE PTR SS:[EBP-0x4],BL
	$+212    00AB547E      74 15                 JE SHORT MapleSto.00AB5495
	$+214    00AB5480      8B45 E4               MOV EAX,DWORD PTR SS:[EBP-0x1C]
	$+217    00AB5483      8B4D DC               MOV ECX,DWORD PTR SS:[EBP-0x24]
	$+21A    00AB5486      8B11                  MOV EDX,DWORD PTR DS:[ECX]
	$+21C    00AB5488      8B52 08               MOV EDX,DWORD PTR DS:[EDX+0x8]
	$+21F    00AB548B      50                    PUSH EAX
	$+220    00AB548C      8D45 DC               LEA EAX,DWORD PTR SS:[EBP-0x24]
	$+223    00AB548F      50                    PUSH EAX
	$+224    00AB5490      FFD2                  CALL EDX
	$+226    00AB5492      895D DC               MOV DWORD PTR SS:[EBP-0x24],EBX
	$+229    00AB5495      895D E0               MOV DWORD PTR SS:[EBP-0x20],EBX
	$+22C    00AB5498      8D4D 9C               LEA ECX,DWORD PTR SS:[EBP-0x64]
	$+22F    00AB549B      C745 FC FFFFFFFF      MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+236    00AB54A2      E8 A9DF4300           CALL MapleSto.00EF3450
	$+23B    00AB54A7      8B4D F4               MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+23E    00AB54AA      64:890D 00000000      MOV DWORD PTR FS:[0],ECX
	$+245    00AB54B1      59                    POP ECX
	$+246    00AB54B2      5F                    POP EDI
	$+247    00AB54B3      5E                    POP ESI
	$+248    00AB54B4      5B                    POP EBX
	$+249    00AB54B5      8BE5                  MOV ESP,EBP
	$+24B    00AB54B7      5D                    POP EBP
	$+24C    00AB54B8      C2 0400               RETN 0x4
	$+24F    00AB54BB      CC                    INT3
	$+250    00AB54BC      CC                    INT3
	$+251    00AB54BD      CC                    INT3
	$+252    00AB54BE      CC                    INT3
	$+253    00AB54BF      CC                    INT3
	$+254    00AB54C0      55                    PUSH EBP
	$+255    00AB54C1      8BEC                  MOV EBP,ESP
	$+257    00AB54C3      6A FF                 PUSH -0x1
	$+259    00AB54C5      68 068E3801           PUSH MapleSto.01388E06
	$+25E    00AB54CA      64:A1 00000000        MOV EAX,DWORD PTR FS:[0]
	$+264    00AB54D0      50                    PUSH EAX
	$+265    00AB54D1      83EC 28               SUB ESP,0x28
	$+268    00AB54D4      56                    PUSH ESI
	$+269    00AB54D5      A1 D0687101           MOV EAX,DWORD PTR DS:[0x17168D0]
	$+26E    00AB54DA      33C5                  XOR EAX,EBP
	$+270    00AB54DC      50                    PUSH EAX
	$+271    00AB54DD      8D45 F4               LEA EAX,DWORD PTR SS:[EBP-0xC]
	$+274    00AB54E0      64:A3 00000000        MOV DWORD PTR FS:[0],EAX
	$+27A    00AB54E6      8BF1                  MOV ESI,ECX
	$+27C    00AB54E8      E8 937D4300           CALL MapleSto.00EED280
	$+281    00AB54ED      85C0                  TEST EAX,EAX
	$+283    00AB54EF      75 10                 JNZ SHORT MapleSto.00AB5501
	$+285    00AB54F1      8B4D F4               MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+288    00AB54F4      64:890D 00000000      MOV DWORD PTR FS:[0],ECX
	$+28F    00AB54FB      59                    POP ECX
	$+290    00AB54FC      5E                    POP ESI
	$+291    00AB54FD      8BE5                  MOV ESP,EBP
	$+293    00AB54FF      5D                    POP EBP
	$+294    00AB5500      C3                    RETN
	$+295    00AB5501      68 74A14701           PUSH MapleSto.0147A174                   ; ASCII "m_itemBreak"
	$+29A    00AB5506      8D4D F0               LEA ECX,DWORD PTR SS:[EBP-0x10]
	$+29D    00AB5509      FF15 20A23E01         CALL DWORD PTR DS:[0x13EA220]            ; mfc90.#310
	$+2A3    00AB550F      8B4E 20               MOV ECX,DWORD PTR DS:[ESI+0x20]
	$+2A6    00AB5512      8D45 F0               LEA EAX,DWORD PTR SS:[EBP-0x10]
	$+2A9    00AB5515      50                    PUSH EAX
	$+2AA    00AB5516      8D55 E4               LEA EDX,DWORD PTR SS:[EBP-0x1C]
	$+2AD    00AB5519      52                    PUSH EDX
	$+2AE    00AB551A      C745 FC 00000000      MOV DWORD PTR SS:[EBP-0x4],0x0
	$+2B5    00AB5521      E8 5A25BEFF           CALL MapleSto.00697A80
	$+2BA    00AB5526      8D4D F0               LEA ECX,DWORD PTR SS:[EBP-0x10]
	$+2BD    00AB5529      C645 FC 02            MOV BYTE PTR SS:[EBP-0x4],0x2
	$+2C1    00AB552D      FF15 08A23E01         CALL DWORD PTR DS:[0x13EA208]            ; mfc90.#601
	$+2C7    00AB5533      8B45 E8               MOV EAX,DWORD PTR SS:[EBP-0x18]
	$+2CA    00AB5536      85C0                  TEST EAX,EAX
	$+2CC    00AB5538      74 19                 JE SHORT MapleSto.00AB5553
	$+2CE    00AB553A      51                    PUSH ECX
	$+2CF    00AB553B      8BC4                  MOV EAX,ESP
	$+2D1    00AB553D      8965 EC               MOV DWORD PTR SS:[EBP-0x14],ESP
	$+2D4    00AB5540      50                    PUSH EAX
	$+2D5    00AB5541      8BCE                  MOV ECX,ESI
	$+2D7    00AB5543      E8 A8F3FFFF           CALL MapleSto.00AB48F0
	$+2DC    00AB5548      8B4D E8               MOV ECX,DWORD PTR SS:[EBP-0x18]
	$+2DF    00AB554B      E8 A03A4300           CALL MapleSto.00EE8FF0
	$+2E4    00AB5550      8B45 E8               MOV EAX,DWORD PTR SS:[EBP-0x18]
	$+2E7    00AB5553      C745 FC FFFFFFFF      MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+2EE    00AB555A      85C0                  TEST EAX,EAX
	$+2F0    00AB555C      74 2A                 JE SHORT MapleSto.00AB5588
	$+2F2    00AB555E      83C0 04               ADD EAX,0x4
	$+2F5    00AB5561      50                    PUSH EAX
	$+2F6    00AB5562      FF15 6C933E01         CALL DWORD PTR DS:[0x13E936C]
	$+2FC    00AB5568      85C0                  TEST EAX,EAX
	$+2FE    00AB556A      75 1C                 JNZ SHORT MapleSto.00AB5588
	$+300    00AB556C      8B4D E8               MOV ECX,DWORD PTR SS:[EBP-0x18]
	$+303    00AB556F      83C1 04               ADD ECX,0x4
	$+306    00AB5572      51                    PUSH ECX
	$+307    00AB5573      FF15 F8923E01         CALL DWORD PTR DS:[0x13E92F8]
	$+30D    00AB5579      8B4D E8               MOV ECX,DWORD PTR SS:[EBP-0x18]
	$+310    00AB557C      85C9                  TEST ECX,ECX
	$+312    00AB557E      74 08                 JE SHORT MapleSto.00AB5588
	$+314    00AB5580      8B11                  MOV EDX,DWORD PTR DS:[ECX]
	$+316    00AB5582      8B02                  MOV EAX,DWORD PTR DS:[EDX]
	$+318    00AB5584      6A 01                 PUSH 0x1
	$+31A    00AB5586      FFD0                  CALL EAX
	$+31C    00AB5588      8B0D 00257201         MOV ECX,DWORD PTR DS:[0x1722500]
	$+322    00AB558E      6A 39                 PUSH 0x39
	$+324    00AB5590      E8 5B009CFF           CALL MapleSto.004755F0
	$+329    00AB5595      84C0                  TEST AL,AL
	$+32B    00AB5597      75 4F                 JNZ SHORT MapleSto.00AB55E8
	$+32D    00AB5599      68 A8A14701           PUSH MapleSto.0147A1A8                   ; ASCII "m_itemSocketAndGem"
	$+332    00AB559E      8D4D F0               LEA ECX,DWORD PTR SS:[EBP-0x10]
	$+335    00AB55A1      FF15 20A23E01         CALL DWORD PTR DS:[0x13EA220]            ; mfc90.#310
	$+33B    00AB55A7      8B4E 20               MOV ECX,DWORD PTR DS:[ESI+0x20]
	$+33E    00AB55AA      8D55 F0               LEA EDX,DWORD PTR SS:[EBP-0x10]
	$+341    00AB55AD      52                    PUSH EDX
	$+342    00AB55AE      8D45 E4               LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$+345    00AB55B1      50                    PUSH EAX
	$+346    00AB55B2      C745 FC 03000000      MOV DWORD PTR SS:[EBP-0x4],0x3
	$+34D    00AB55B9      E8 C224BEFF           CALL MapleSto.00697A80
	$+352    00AB55BE      8D4D F0               LEA ECX,DWORD PTR SS:[EBP-0x10]
	$+355    00AB55C1      C645 FC 05            MOV BYTE PTR SS:[EBP-0x4],0x5
	$+359    00AB55C5      FF15 08A23E01         CALL DWORD PTR DS:[0x13EA208]            ; mfc90.#601
	$+35F    00AB55CB      8B4D E8               MOV ECX,DWORD PTR SS:[EBP-0x18]
	$+362    00AB55CE      85C9                  TEST ECX,ECX
	$+364    00AB55D0      74 07                 JE SHORT MapleSto.00AB55D9
	$+366    00AB55D2      6A 00                 PUSH 0x0
	$+368    00AB55D4      E8 6DCF4301           CALL MapleSto.01EF2546
	$+36D    00AB55D9      8D4D E4               LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$+370    00AB55DC      C745 FC FFFFFFFF      MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+377    00AB55E3      E8 287BFCFF           CALL MapleSto.00A7D110
	$+37C    00AB55E8      8B4E 20               MOV ECX,DWORD PTR DS:[ESI+0x20]
	$+37F    00AB55EB      85C9                  TEST ECX,ECX
	$+381    00AB55ED      74 2A                 JE SHORT MapleSto.00AB5619
	$+383    00AB55EF      68 98A14701           PUSH MapleSto.0147A198                   ; ASCII "updateButtonPos"
	$+388    00AB55F4      8D55 CC               LEA EDX,DWORD PTR SS:[EBP-0x34]
	$+38B    00AB55F7      52                    PUSH EDX
	$+38C    00AB55F8      E8 83E34200           CALL MapleSto.00EE3980
	$+391    00AB55FD      8B45 D0               MOV EAX,DWORD PTR SS:[EBP-0x30]
	$+394    00AB5600      C1E8 06               SHR EAX,0x6
	$+397    00AB5603      A8 01                 TEST AL,0x1
	$+399    00AB5605      74 12                 JE SHORT MapleSto.00AB5619
	$+39B    00AB5607      8B45 D4               MOV EAX,DWORD PTR SS:[EBP-0x2C]
	$+39E    00AB560A      8B4D CC               MOV ECX,DWORD PTR SS:[EBP-0x34]
	$+3A1    00AB560D      8B11                  MOV EDX,DWORD PTR DS:[ECX]
	$+3A3    00AB560F      8B52 08               MOV EDX,DWORD PTR DS:[EDX+0x8]
	$+3A6    00AB5612      50                    PUSH EAX
	$+3A7    00AB5613      8D45 CC               LEA EAX,DWORD PTR SS:[EBP-0x34]
	$+3AA    00AB5616      50                    PUSH EAX
	$+3AB    00AB5617      FFD2                  CALL EDX
	$+3AD    00AB5619      B8 01000000           MOV EAX,0x1
	$+3B2    00AB561E      8B4D F4               MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+3B5    00AB5621      64:890D 00000000      MOV DWORD PTR FS:[0],ECX
	$+3BC    00AB5628      59                    POP ECX
	$+3BD    00AB5629      5E                    POP ESI
	$+3BE    00AB562A      8BE5                  MOV ESP,EBP
	$+3C0    00AB562C      5D                    POP EBP
	$+3C1    00AB562D      C3                    RETN
	$+3C2    00AB562E      CC                    INT3
	*/
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("8D45CC5051B9", 5 + 1);
	if (!op_dw_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *op_dw_tmp;
	CheckExeAddr(dw_tmp);
	ofs << "pt_cdword	g_a_bag_mgr_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	/*
	$-1ED    009C486F      CC                     INT3
	$-1EC    009C4870      55                     PUSH EBP
	$-1EB    009C4871      8BEC                   MOV EBP,ESP
	$-1E9    009C4873      6A FF                  PUSH -0x1
	$-1E7    009C4875      68 EB053701            PUSH MapleSto.013705EB
	$-1E2    009C487A      64:A1 00000000         MOV EAX,DWORD PTR FS:[0]
	$-1DC    009C4880      50                     PUSH EAX
	$-1DB    009C4881      83EC 1C                SUB ESP,0x1C
	$-1D8    009C4884      53                     PUSH EBX
	$-1D7    009C4885      56                     PUSH ESI
	$-1D6    009C4886      57                     PUSH EDI
	$-1D5    009C4887      A1 D0687101            MOV EAX,DWORD PTR DS:[0x17168D0]
	$-1D0    009C488C      33C5                   XOR EAX,EBP
	$-1CE    009C488E      50                     PUSH EAX
	$-1CD    009C488F      8D45 F4                LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-1CA    009C4892      64:A3 00000000         MOV DWORD PTR FS:[0],EAX
	$-1C4    009C4898      8B75 08                MOV ESI,DWORD PTR SS:[EBP+0x8]
	$-1C1    009C489B      8B46 04                MOV EAX,DWORD PTR DS:[ESI+0x4]
	$-1BE    009C489E      8B50 04                MOV EDX,DWORD PTR DS:[EAX+0x4]
	$-1BB    009C48A1      83C6 04                ADD ESI,0x4
	$-1B8    009C48A4      8BCE                   MOV ECX,ESI
	$-1B6    009C48A6      FFD2                   CALL EDX
	$-1B4    009C48A8      0FB6C8                 MOVZX ECX,AL
	$-1B1    009C48AB      33C0                   XOR EAX,EAX
	$-1AF    009C48AD      83F9 01                CMP ECX,0x1
	$-1AC    009C48B0      75 04                  JNZ SHORT MapleSto.009C48B6
	$-1AA    009C48B2      8BC1                   MOV EAX,ECX
	$-1A8    009C48B4      EB 0D                  JMP SHORT MapleSto.009C48C3
	$-1A6    009C48B6      85C9                   TEST ECX,ECX
	$-1A4    009C48B8      75 09                  JNZ SHORT MapleSto.009C48C3
	$-1A2    009C48BA      8B06                   MOV EAX,DWORD PTR DS:[ESI]
	$-1A0    009C48BC      8B50 0C                MOV EDX,DWORD PTR DS:[EAX+0xC]
	$-19D    009C48BF      8BCE                   MOV ECX,ESI
	$-19B    009C48C1      FFD2                   CALL EDX
	$-199    009C48C3      85C0                   TEST EAX,EAX
	$-197    009C48C5      0F8E 54010000          JLE MapleSto.009C4A1F
	$-191    009C48CB      8945 E8                MOV DWORD PTR SS:[EBP-0x18],EAX
	$-18E    009C48CE      8B06                   MOV EAX,DWORD PTR DS:[ESI]
	$-18C    009C48D0      8B50 1C                MOV EDX,DWORD PTR DS:[EAX+0x1C]
	$-189    009C48D3      8D4D 08                LEA ECX,DWORD PTR SS:[EBP+0x8]
	$-186    009C48D6      51                     PUSH ECX
	$-185    009C48D7      8BCE                   MOV ECX,ESI
	$-183    009C48D9      FFD2                   CALL EDX
	$-181    009C48DB      8B06                   MOV EAX,DWORD PTR DS:[ESI]
	$-17F    009C48DD      8B50 04                MOV EDX,DWORD PTR DS:[EAX+0x4]
	$-17C    009C48E0      8BCE                   MOV ECX,ESI
	$-17A    009C48E2      C745 FC 00000000       MOV DWORD PTR SS:[EBP-0x4],0x0
	$-173    009C48E9      FFD2                   CALL EDX
	$-171    009C48EB      8B16                   MOV EDX,DWORD PTR DS:[ESI]
	$-16F    009C48ED      0FB6C0                 MOVZX EAX,AL
	$-16C    009C48F0      8945 EC                MOV DWORD PTR SS:[EBP-0x14],EAX
	$-169    009C48F3      8B42 04                MOV EAX,DWORD PTR DS:[EDX+0x4]
	$-166    009C48F6      8BCE                   MOV ECX,ESI
	$-164    009C48F8      FFD0                   CALL EAX
	$-162    009C48FA      8B16                   MOV EDX,DWORD PTR DS:[ESI]
	$-160    009C48FC      0FB6F8                 MOVZX EDI,AL
	$-15D    009C48FF      8B42 0C                MOV EAX,DWORD PTR DS:[EDX+0xC]
	$-15A    009C4902      8BCE                   MOV ECX,ESI
	$-158    009C4904      FFD0                   CALL EAX
	$-156    009C4906      8B16                   MOV EDX,DWORD PTR DS:[ESI]
	$-154    009C4908      8BD8                   MOV EBX,EAX
	$-152    009C490A      8B42 0C                MOV EAX,DWORD PTR DS:[EDX+0xC]
	$-14F    009C490D      8BCE                   MOV ECX,ESI
	$-14D    009C490F      FFD0                   CALL EAX
	$-14B    009C4911      51                     PUSH ECX
	$-14A    009C4912      8D55 08                LEA EDX,DWORD PTR SS:[EBP+0x8]
	$-147    009C4915      8BCC                   MOV ECX,ESP
	$-145    009C4917      8965 E0                MOV DWORD PTR SS:[EBP-0x20],ESP
	$-142    009C491A      52                     PUSH EDX
	$-141    009C491B      8945 E4                MOV DWORD PTR SS:[EBP-0x1C],EAX
	$-13E    009C491E      FF15 ECA13E01          CALL DWORD PTR DS:[0x13EA1EC]               ; mfc90.#300
	$-138    009C4924      8B0D 2C417201          MOV ECX,DWORD PTR DS:[0x172412C]
	$-132    009C492A      8D45 D8                LEA EAX,DWORD PTR SS:[EBP-0x28]
	$-12F    009C492D      50                     PUSH EAX
	$-12E    009C492E      E8 3DAB1B00            CALL MapleSto.00B7F470
	$-129    009C4933      8B4D DC                MOV ECX,DWORD PTR SS:[EBP-0x24]
	$-126    009C4936      C645 FC 01             MOV BYTE PTR SS:[EBP-0x4],0x1
	$-122    009C493A      85C9                   TEST ECX,ECX
	$-120    009C493C      74 19                  JE SHORT MapleSto.009C4957
	$-11E    009C493E      8B55 EC                MOV EDX,DWORD PTR SS:[EBP-0x14]
	$-11B    009C4941      52                     PUSH EDX
	$-11A    009C4942      E8 DE182F01            CALL MapleSto.01CB6225
	$-115    009C4947      8B45 E4                MOV EAX,DWORD PTR SS:[EBP-0x1C]
	$-112    009C494A      8B4D DC                MOV ECX,DWORD PTR SS:[EBP-0x24]
	$-10F    009C494D      50                     PUSH EAX
	$-10E    009C494E      53                     PUSH EBX
	$-10D    009C494F      57                     PUSH EDI
	$-10C    009C4950      E8 15A5C2FF            CALL MapleSto.005EEE6A
	$-107    009C4955      EB 72                  JMP SHORT MapleSto.009C49C9
	$-105    009C4957      8B0D 8C797401          MOV ECX,DWORD PTR DS:[0x174798C]
	$-FF     009C495D      8B11                   MOV EDX,DWORD PTR DS:[ECX]
	$-FD     009C495F      8B42 38                MOV EAX,DWORD PTR DS:[EDX+0x38]
	$-FA     009C4962      68 17001006            PUSH 0x6100017
	$-F5     009C4967      FFD0                   CALL EAX
	$-F3     009C4969      C745 F0 00000000       MOV DWORD PTR SS:[EBP-0x10],0x0
	$-EC     009C4970      85C0                   TEST EAX,EAX
	$-EA     009C4972      74 1B                  JE SHORT MapleSto.009C498F
	$-E8     009C4974      8B10                   MOV EDX,DWORD PTR DS:[EAX]
	$-E6     009C4976      8B52 14                MOV EDX,DWORD PTR DS:[EDX+0x14]
	$-E3     009C4979      8D4D F0                LEA ECX,DWORD PTR SS:[EBP-0x10]
	$-E0     009C497C      51                     PUSH ECX
	$-DF     009C497D      68 17001006            PUSH 0x6100017
	$-DA     009C4982      8BC8                   MOV ECX,EAX
	$-D8     009C4984      FFD2                   CALL EDX
	$-D6     009C4986      84C0                   TEST AL,AL
	$-D4     009C4988      74 05                  JE SHORT MapleSto.009C498F
	$-D2     009C498A      8B7D F0                MOV EDI,DWORD PTR SS:[EBP-0x10]
	$-CF     009C498D      EB 02                  JMP SHORT MapleSto.009C4991
	$-CD     009C498F      33FF                   XOR EDI,EDI
	$-CB     009C4991      897D E0                MOV DWORD PTR SS:[EBP-0x20],EDI
	$-C8     009C4994      85FF                   TEST EDI,EDI
	$-C6     009C4996      74 09                  JE SHORT MapleSto.009C49A1
	$-C4     009C4998      8B07                   MOV EAX,DWORD PTR DS:[EDI]
	$-C2     009C499A      8B50 04                MOV EDX,DWORD PTR DS:[EAX+0x4]
	$-BF     009C499D      8BCF                   MOV ECX,EDI
	$-BD     009C499F      FFD2                   CALL EDX
	$-BB     009C49A1      C645 FC 02             MOV BYTE PTR SS:[EBP-0x4],0x2
	$-B7     009C49A5      85FF                   TEST EDI,EDI
	$-B5     009C49A7      74 0F                  JE SHORT MapleSto.009C49B8
	$-B3     009C49A9      8B45 EC                MOV EAX,DWORD PTR SS:[EBP-0x14]
	$-B0     009C49AC      50                     PUSH EAX
	$-AF     009C49AD      8D4D 08                LEA ECX,DWORD PTR SS:[EBP+0x8]
	$-AC     009C49B0      51                     PUSH ECX
	$-AB     009C49B1      8BCF                   MOV ECX,EDI
	$-A9     009C49B3      E8 08E0DBFF            CALL MapleSto.007829C0
	$-A4     009C49B8      C645 FC 01             MOV BYTE PTR SS:[EBP-0x4],0x1
	$-A0     009C49BC      85FF                   TEST EDI,EDI
	$-9E     009C49BE      74 09                  JE SHORT MapleSto.009C49C9
	$-9C     009C49C0      8B17                   MOV EDX,DWORD PTR DS:[EDI]
	$-9A     009C49C2      8B42 08                MOV EAX,DWORD PTR DS:[EDX+0x8]
	$-97     009C49C5      8BCF                   MOV ECX,EDI
	$-95     009C49C7      FFD0                   CALL EAX
	$-93     009C49C9      8B45 DC                MOV EAX,DWORD PTR SS:[EBP-0x24]
	$-90     009C49CC      C645 FC 00             MOV BYTE PTR SS:[EBP-0x4],0x0
	$-8C     009C49D0      85C0                   TEST EAX,EAX
	$-8A     009C49D2      74 31                  JE SHORT MapleSto.009C4A05
	$-88     009C49D4      83C0 04                ADD EAX,0x4
	$-85     009C49D7      50                     PUSH EAX
	$-84     009C49D8      FF15 6C933E01          CALL DWORD PTR DS:[0x13E936C]
	$-7E     009C49DE      85C0                   TEST EAX,EAX
	$-7C     009C49E0      75 1C                  JNZ SHORT MapleSto.009C49FE
	$-7A     009C49E2      8B4D DC                MOV ECX,DWORD PTR SS:[EBP-0x24]
	$-77     009C49E5      83C1 04                ADD ECX,0x4
	$-74     009C49E8      51                     PUSH ECX
	$-73     009C49E9      FF15 F8923E01          CALL DWORD PTR DS:[0x13E92F8]
	$-6D     009C49EF      8B4D DC                MOV ECX,DWORD PTR SS:[EBP-0x24]
	$-6A     009C49F2      85C9                   TEST ECX,ECX
	$-68     009C49F4      74 08                  JE SHORT MapleSto.009C49FE
	$-66     009C49F6      8B11                   MOV EDX,DWORD PTR DS:[ECX]
	$-64     009C49F8      8B02                   MOV EAX,DWORD PTR DS:[EDX]
	$-62     009C49FA      6A 01                  PUSH 0x1
	$-60     009C49FC      FFD0                   CALL EAX
	$-5E     009C49FE      C745 DC 00000000       MOV DWORD PTR SS:[EBP-0x24],0x0
	$-57     009C4A05      8D4D 08                LEA ECX,DWORD PTR SS:[EBP+0x8]
	$-54     009C4A08      C745 FC FFFFFFFF       MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-4D     009C4A0F      FF15 08A23E01          CALL DWORD PTR DS:[0x13EA208]               ; mfc90.#601
	$-47     009C4A15      836D E8 01             SUB DWORD PTR SS:[EBP-0x18],0x1
	$-43     009C4A19    ^ 0F85 AFFEFFFF          JNZ MapleSto.009C48CE
	$-3D     009C4A1F      8B4D F4                MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-3A     009C4A22      64:890D 00000000       MOV DWORD PTR FS:[0],ECX
	$-33     009C4A29      59                     POP ECX
	$-32     009C4A2A      5F                     POP EDI
	$-31     009C4A2B      5E                     POP ESI
	$-30     009C4A2C      5B                     POP EBX
	$-2F     009C4A2D      8BE5                   MOV ESP,EBP
	$-2D     009C4A2F      5D                     POP EBP
	$-2C     009C4A30      C2 0400                RETN 0x4
	$-29     009C4A33      CC                     INT3
	$-28     009C4A34      CC                     INT3
	$-27     009C4A35      CC                     INT3
	$-26     009C4A36      CC                     INT3
	$-25     009C4A37      CC                     INT3
	$-24     009C4A38      CC                     INT3
	$-23     009C4A39      CC                     INT3
	$-22     009C4A3A      CC                     INT3
	$-21     009C4A3B      CC                     INT3
	$-20     009C4A3C      CC                     INT3
	$-1F     009C4A3D      CC                     INT3
	$-1E     009C4A3E      CC                     INT3
	$-1D     009C4A3F      CC                     INT3
	$-1C     009C4A40      55                     PUSH EBP
	$-1B     009C4A41      8BEC                   MOV EBP,ESP
	$-19     009C4A43      6A FF                  PUSH -0x1
	$-17     009C4A45      68 65063701            PUSH MapleSto.01370665
	$-12     009C4A4A      64:A1 00000000         MOV EAX,DWORD PTR FS:[0]
	$-C      009C4A50      50                     PUSH EAX
	$-B      009C4A51      83EC 4C                SUB ESP,0x4C
	$-8      009C4A54      53                     PUSH EBX
	$-7      009C4A55      56                     PUSH ESI
	$-6      009C4A56      57                     PUSH EDI
	$-5      009C4A57      A1 D0687101            MOV EAX,DWORD PTR DS:[0x17168D0]
	$ ==>    009C4A5C      33C5                   XOR EAX,EBP
	$+2      009C4A5E      50                     PUSH EAX
	$+3      009C4A5F      8D45 F4                LEA EAX,DWORD PTR SS:[EBP-0xC]
	$+6      009C4A62      64:A3 00000000         MOV DWORD PTR FS:[0],EAX
	$+C      009C4A68      8B75 08                MOV ESI,DWORD PTR SS:[EBP+0x8]
	$+F      009C4A6B      8B46 04                MOV EAX,DWORD PTR DS:[ESI+0x4]
	$+12     009C4A6E      8B50 04                MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+15     009C4A71      83C6 04                ADD ESI,0x4
	$+18     009C4A74      8BCE                   MOV ECX,ESI
	$+1A     009C4A76      FFD2                   CALL EDX
	$+1C     009C4A78      0FB6C0                 MOVZX EAX,AL
	$+1F     009C4A7B      8945 D8                MOV DWORD PTR SS:[EBP-0x28],EAX
	$+22     009C4A7E      83F8 10                CMP EAX,0x10
	$+25     009C4A81      75 0F                  JNZ SHORT MapleSto.009C4A92
	$+27     009C4A83      B9 F04C7201            MOV ECX,MapleSto.01724CF0                   ; g_a_mail_mgr_
	$+2C     009C4A88      E8 3E7A2C01            CALL MapleSto.01C8C4CB
	$+31     009C4A8D      E9 39080000            JMP MapleSto.009C52CB
	$+36     009C4A92      85C0                   TEST EAX,EAX
	$+38     009C4A94      0F85 C8000000          JNZ MapleSto.009C4B62
	$+3E     009C4A9A      8B06                   MOV EAX,DWORD PTR DS:[ESI]
	$+40     009C4A9C      8B50 0C                MOV EDX,DWORD PTR DS:[EAX+0xC]
	$+43     009C4A9F      8BCE                   MOV ECX,ESI
	$+45     009C4AA1      FFD2                   CALL EDX
	$+47     009C4AA3      85C0                   TEST EAX,EAX
	$+49     009C4AA5      0F8E A6000000          JLE MapleSto.009C4B51
	$+4F     009C4AAB      8B1D F8923E01          MOV EBX,DWORD PTR DS:[0x13E92F8]
	$+55     009C4AB1      8945 F0                MOV DWORD PTR SS:[EBP-0x10],EAX
	$+58     009C4AB4      BF 01000000            MOV EDI,0x1
	$+5D     009C4AB9      68 E0000000            PUSH 0xE0
	$+62     009C4ABE      E8 ABC48300            CALL MapleSto.01200F6E                      ; JMP to mfc90.#798
	$+67     009C4AC3      83C4 04                ADD ESP,0x4
	$+6A     009C4AC6      8945 E0                MOV DWORD PTR SS:[EBP-0x20],EAX
	$+6D     009C4AC9      33F6                   XOR ESI,ESI
	$+6F     009C4ACB      8975 FC                MOV DWORD PTR SS:[EBP-0x4],ESI
	$+72     009C4ACE      3BC6                   CMP EAX,ESI
	$+74     009C4AD0      74 0A                  JE SHORT MapleSto.009C4ADC
	$+76     009C4AD2      56                     PUSH ESI
	$+77     009C4AD3      8BC8                   MOV ECX,EAX
	$+79     009C4AD5      E8 E694B4FF            CALL MapleSto.0050DFC0
	$+7E     009C4ADA      8BF0                   MOV ESI,EAX
	$+80     009C4ADC      8975 CC                MOV DWORD PTR SS:[EBP-0x34],ESI
	$+83     009C4ADF      85F6                   TEST ESI,ESI
	$+85     009C4AE1      74 06                  JE SHORT MapleSto.009C4AE9
	$+87     009C4AE3      8D46 04                LEA EAX,DWORD PTR DS:[ESI+0x4]
	$+8A     009C4AE6      50                     PUSH EAX
	$+8B     009C4AE7      FFD3                   CALL EBX
	$+8D     009C4AE9      8B4D 08                MOV ECX,DWORD PTR SS:[EBP+0x8]
	$+90     009C4AEC      51                     PUSH ECX
	$+91     009C4AED      8BCE                   MOV ECX,ESI
	$+93     009C4AEF      897D FC                MOV DWORD PTR SS:[EBP-0x4],EDI
	$+96     009C4AF2      E8 F98FB4FF            CALL MapleSto.0050DAF0
	$+9B     009C4AF7      83EC 08                SUB ESP,0x8
	$+9E     009C4AFA      8BC4                   MOV EAX,ESP
	$+A0     009C4AFC      8965 E0                MOV DWORD PTR SS:[EBP-0x20],ESP
	$+A3     009C4AFF      8970 04                MOV DWORD PTR DS:[EAX+0x4],ESI
	$+A6     009C4B02      85F6                   TEST ESI,ESI
	$+A8     009C4B04      74 06                  JE SHORT MapleSto.009C4B0C
	$+AA     009C4B06      8D56 04                LEA EDX,DWORD PTR DS:[ESI+0x4]
	$+AD     009C4B09      52                     PUSH EDX
	$+AE     009C4B0A      FFD3                   CALL EBX
	$+B0     009C4B0C      B9 F04C7201            MOV ECX,MapleSto.01724CF0
	$+B5     009C4B11      E8 2AA6BCFF            CALL MapleSto.0058F140
	$+BA     009C4B16      C745 FC FFFFFFFF       MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+C1     009C4B1D      85F6                   TEST ESI,ESI
	$+C3     009C4B1F      74 27                  JE SHORT MapleSto.009C4B48
	$+C5     009C4B21      8D7E 04                LEA EDI,DWORD PTR DS:[ESI+0x4]
	$+C8     009C4B24      57                     PUSH EDI
	$+C9     009C4B25      FF15 6C933E01          CALL DWORD PTR DS:[0x13E936C]
	$+CF     009C4B2B      85C0                   TEST EAX,EAX
	$+D1     009C4B2D      75 0D                  JNZ SHORT MapleSto.009C4B3C
	$+D3     009C4B2F      57                     PUSH EDI
	$+D4     009C4B30      FFD3                   CALL EBX
	$+D6     009C4B32      8B06                   MOV EAX,DWORD PTR DS:[ESI]
	$+D8     009C4B34      8B10                   MOV EDX,DWORD PTR DS:[EAX]
	$+DA     009C4B36      6A 01                  PUSH 0x1
	$+DC     009C4B38      8BCE                   MOV ECX,ESI
	$+DE     009C4B3A      FFD2                   CALL EDX
	$+E0     009C4B3C      C745 CC 00000000       MOV DWORD PTR SS:[EBP-0x34],0x0
	$+E7     009C4B43      BF 01000000            MOV EDI,0x1
	$+EC     009C4B48      297D F0                SUB DWORD PTR SS:[EBP-0x10],EDI
	$+EF     009C4B4B    ^ 0F85 68FFFFFF          JNZ MapleSto.009C4AB9
	$+F5     009C4B51      6A 00                  PUSH 0x0
	$+F7     009C4B53      B9 F04C7201            MOV ECX,MapleSto.01724CF0
	$+FC     009C4B58      E8 BB3E2C01            CALL MapleSto.01C88A18
	$+101    009C4B5D      E9 69070000            JMP MapleSto.009C52CB
	$+106    009C4B62      83F8 11                CMP EAX,0x11
	$+109    009C4B65      75 27                  JNZ SHORT MapleSto.009C4B8E
	$+10B    009C4B67      8B0D 48417201          MOV ECX,DWORD PTR DS:[0x1724148]
	$+111    009C4B6D      68 484E0000            PUSH 0x4E48
	$+116    009C4B72      E8 49161600            CALL MapleSto.00B261C0
	$+11B    009C4B77      85C0                   TEST EAX,EAX
	$+11D    009C4B79      0F85 4C070000          JNZ MapleSto.009C52CB
	$+123    009C4B7F      C705 B8887301 01000000 MOV DWORD PTR DS:[0x17388B8],0x1
	$+12D    009C4B89      E9 3D070000            JMP MapleSto.009C52CB
	$+132    009C4B8E      83F8 01                CMP EAX,0x1
	$+135    009C4B91      75 4F                  JNZ SHORT MapleSto.009C4BE2
	$+137    009C4B93      51                     PUSH ECX
	$+138    009C4B94      8BCC                   MOV ECX,ESP
	$+13A    009C4B96      8965 E8                MOV DWORD PTR SS:[EBP-0x18],ESP
	$+13D    009C4B99      68 50303F01            PUSH MapleSto.013F3050
	$+142    009C4B9E      FF15 D0A13E01          CALL DWORD PTR DS:[0x13EA1D0]               ; mfc90.#286
	$+148    009C4BA4      6A 00                  PUSH 0x0
	$+14A    009C4BA6      6A 00                  PUSH 0x0
	$+14C    009C4BA8      68 13020000            PUSH 0x213
	$+151    009C4BAD      E8 6EB0C9FF            CALL MapleSto.0065FC20
	$+156    009C4BB2      83C4 10                ADD ESP,0x10
	$+159    009C4BB5      6A 00                  PUSH 0x0
	$+15B    009C4BB7      B9 F04C7201            MOV ECX,MapleSto.01724CF0
	$+160    009C4BBC      E8 947EBCFF            CALL MapleSto.0058CA55
	$+165    009C4BC1      68 494E0000            PUSH 0x4E49
	$+16A    009C4BC6      E8 95B90000            CALL MapleSto.009D0560
	$+16F    009C4BCB      83C4 04                ADD ESP,0x4
	$+172    009C4BCE      85C0                   TEST EAX,EAX
	$+174    009C4BD0      0F84 F5060000          JE MapleSto.009C52CB
	$+17A    009C4BD6      8BC8                   MOV ECX,EAX
	$+17C    009C4BD8      E8 33A01000            CALL MapleSto.00ACEC10
	$+181    009C4BDD      E9 E9060000            JMP MapleSto.009C52CB
	$+186    009C4BE2      83F8 02                CMP EAX,0x2
	$+189    009C4BE5      75 5F                  JNZ SHORT MapleSto.009C4C46
	$+18B    009C4BE7      8B06                   MOV EAX,DWORD PTR DS:[ESI]
	$+18D    009C4BE9      8B50 10                MOV EDX,DWORD PTR DS:[EAX+0x10]
	$+190    009C4BEC      8BCE                   MOV ECX,ESI
	$+192    009C4BEE      FFD2                   CALL EDX
	$+194    009C4BF0      8BF8                   MOV EDI,EAX
	$+196    009C4BF2      8B06                   MOV EAX,DWORD PTR DS:[ESI]
	$+198    009C4BF4      8BDA                   MOV EBX,EDX
	$+19A    009C4BF6      8B50 10                MOV EDX,DWORD PTR DS:[EAX+0x10]
	$+19D    009C4BF9      8BCE                   MOV ECX,ESI
	$+19F    009C4BFB      FFD2                   CALL EDX
	$+1A1    009C4BFD      52                     PUSH EDX
	$+1A2    009C4BFE      50                     PUSH EAX
	$+1A3    009C4BFF      53                     PUSH EBX
	$+1A4    009C4C00      57                     PUSH EDI
	$+1A5    009C4C01      B9 F04C7201            MOV ECX,MapleSto.01724CF0
	$+1AA    009C4C06      E8 459CBCFF            CALL MapleSto.0058E850
	$+1AF    009C4C0B      8B0D 48417201          MOV ECX,DWORD PTR DS:[0x1724148]
	$+1B5    009C4C11      68 484E0000            PUSH 0x4E48
	$+1BA    009C4C16      E8 A5151600            CALL MapleSto.00B261C0
	$+1BF    009C4C1B      85C0                   TEST EAX,EAX
	$+1C1    009C4C1D      0F85 A8060000          JNZ MapleSto.009C52CB
	$+1C7    009C4C23      68 484E0000            PUSH 0x4E48
	$+1CC    009C4C28      E8 E3B90000            CALL MapleSto.009D0610
	$+1D1    009C4C2D      83C4 04                ADD ESP,0x4
	$+1D4    009C4C30      85C0                   TEST EAX,EAX
	$+1D6    009C4C32      0F84 93060000          JE MapleSto.009C52CB
	$+1DC    009C4C38      53                     PUSH EBX
	$+1DD    009C4C39      57                     PUSH EDI
	$+1DE    009C4C3A      8BC8                   MOV ECX,EAX
	$+1E0    009C4C3C      E8 5F8A1000            CALL MapleSto.00ACD6A0
	$+1E5    009C4C41      E9 85060000            JMP MapleSto.009C52CB
	$+1EA    009C4C46      83F8 03                CMP EAX,0x3
	$+1ED    009C4C49      75 33                  JNZ SHORT MapleSto.009C4C7E
	$+1EF    009C4C4B      8B06                   MOV EAX,DWORD PTR DS:[ESI]
	$+1F1    009C4C4D      8B50 10                MOV EDX,DWORD PTR DS:[EAX+0x10]
	$+1F4    009C4C50      8BCE                   MOV ECX,ESI
	$+1F6    009C4C52      FFD2                   CALL EDX
	$+1F8    009C4C54      6A 01                  PUSH 0x1
	$+1FA    009C4C56      6A 01                  PUSH 0x1
	$+1FC    009C4C58      68 14020000            PUSH 0x214
	$+201    009C4C5D      8BF0                   MOV ESI,EAX
	$+203    009C4C5F      8BFA                   MOV EDI,EDX
	$+205    009C4C61      E8 8FC0E3FF            CALL MapleSto.00800CF5
	$+20A    009C4C66      83C4 0C                ADD ESP,0xC
	$+20D    009C4C69      6A 00                  PUSH 0x0
	$+20F    009C4C6B      6A 01                  PUSH 0x1
	$+211    009C4C6D      57                     PUSH EDI
	$+212    009C4C6E      56                     PUSH ESI
	$+213    009C4C6F      B9 F04C7201            MOV ECX,MapleSto.01724CF0
	$+218    009C4C74      E8 17A3BCFF            CALL MapleSto.0058EF90
	$+21D    009C4C79      E9 4D060000            JMP MapleSto.009C52CB
	$+222    009C4C7E      83F8 0A                CMP EAX,0xA
	$+225    009C4C81      0F85 29010000          JNZ MapleSto.009C4DB0
	$+22B    009C4C87      8B06                   MOV EAX,DWORD PTR DS:[ESI]
	$+22D    009C4C89      8B50 10                MOV EDX,DWORD PTR DS:[EAX+0x10]
	$+230    009C4C8C      8BCE                   MOV ECX,ESI
	$+232    009C4C8E      FFD2                   CALL EDX
	$+234    009C4C90      8945 D0                MOV DWORD PTR SS:[EBP-0x30],EAX
	$+237    009C4C93      8B06                   MOV EAX,DWORD PTR DS:[ESI]
	$+239    009C4C95      8955 D4                MOV DWORD PTR SS:[EBP-0x2C],EDX
	$+23C    009C4C98      8B50 04                MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+23F    009C4C9B      8BCE                   MOV ECX,ESI
	$+241    009C4C9D      FFD2                   CALL EDX
	$+243    009C4C9F      8B16                   MOV EDX,DWORD PTR DS:[ESI]
	$+245    009C4CA1      0FB6C0                 MOVZX EAX,AL
	$+248    009C4CA4      8945 EC                MOV DWORD PTR SS:[EBP-0x14],EAX
	$+24B    009C4CA7      8B42 04                MOV EAX,DWORD PTR DS:[EDX+0x4]
	$+24E    009C4CAA      8BCE                   MOV ECX,ESI
	$+250    009C4CAC      FFD0                   CALL EAX
	$+252    009C4CAE      8B16                   MOV EDX,DWORD PTR DS:[ESI]
	$+254    009C4CB0      0FB6C8                 MOVZX ECX,AL
	$+257    009C4CB3      8B42 04                MOV EAX,DWORD PTR DS:[EDX+0x4]
	$+25A    009C4CB6      894D E8                MOV DWORD PTR SS:[EBP-0x18],ECX
	$+25D    009C4CB9      8BCE                   MOV ECX,ESI
	$+25F    009C4CBB      FFD0                   CALL EAX
	$+261    009C4CBD      8B16                   MOV EDX,DWORD PTR DS:[ESI]
	$+263    009C4CBF      0FB6F8                 MOVZX EDI,AL
	$+266    009C4CC2      8B42 10                MOV EAX,DWORD PTR DS:[EDX+0x10]
	$+269    009C4CC5      8BCE                   MOV ECX,ESI
	$+26B    009C4CC7      FFD0                   CALL EAX
	$+26D    009C4CC9      8B4D D4                MOV ECX,DWORD PTR SS:[EBP-0x2C]
	$+270    009C4CCC      51                     PUSH ECX
	$+271    009C4CCD      8BDA                   MOV EBX,EDX
	$+273    009C4CCF      8B55 D0                MOV EDX,DWORD PTR SS:[EBP-0x30]
	$+276    009C4CD2      8BF0                   MOV ESI,EAX
	$+278    009C4CD4      52                     PUSH EDX
	$+279    009C4CD5      8D45 C8                LEA EAX,DWORD PTR SS:[EBP-0x38]
	$+27C    009C4CD8      50                     PUSH EAX
	$+27D    009C4CD9      B9 F04C7201            MOV ECX,MapleSto.01724CF0
	$+282    009C4CDE      E8 2D92BCFF            CALL MapleSto.0058DF10
	$+287    009C4CE3      8B4D CC                MOV ECX,DWORD PTR SS:[EBP-0x34]
	$+28A    009C4CE6      C745 FC 02000000       MOV DWORD PTR SS:[EBP-0x4],0x2
	$+291    009C4CED      85C9                   TEST ECX,ECX
	$+293    009C4CEF      75 23                  JNZ SHORT MapleSto.009C4D14
	$+295    009C4CF1      8D4D C8                LEA ECX,DWORD PTR SS:[EBP-0x38]
	$+298    009C4CF4      C745 FC FFFFFFFF       MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+29F    009C4CFB      E8 10840B00            CALL MapleSto.00A7D110
	$+2A4    009C4D00      8B4D F4                MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+2A7    009C4D03      64:890D 00000000       MOV DWORD PTR FS:[0],ECX
	$+2AE    009C4D0A      59                     POP ECX
	$+2AF    009C4D0B      5F                     POP EDI
	$+2B0    009C4D0C      5E                     POP ESI
	$+2B1    009C4D0D      5B                     POP EBX
	$+2B2    009C4D0E      8BE5                   MOV ESP,EBP
	$+2B4    009C4D10      5D                     POP EBP
	$+2B5    009C4D11      C2 0400                RETN 0x4
	$+2B8    009C4D14      837D EC 00             CMP DWORD PTR SS:[EBP-0x14],0x0
	$+2BC    009C4D18      53                     PUSH EBX
	$+2BD    009C4D19      56                     PUSH ESI
	$+2BE    009C4D1A      74 56                  JE SHORT MapleSto.009C4D72
	$+2C0    009C4D1C      57                     PUSH EDI
	$+2C1    009C4D1D      E8 FE82B4FF            CALL MapleSto.0050D020
	$+2C6    009C4D22      837D E8 00             CMP DWORD PTR SS:[EBP-0x18],0x0
	$+2CA    009C4D26      75 4F                  JNZ SHORT MapleSto.009C4D77
	$+2CC    009C4D28      57                     PUSH EDI
	$+2CD    009C4D29      8D4D DC                LEA ECX,DWORD PTR SS:[EBP-0x24]
	$+2D0    009C4D2C      51                     PUSH ECX
	$+2D1    009C4D2D      8B4D CC                MOV ECX,DWORD PTR SS:[EBP-0x34]
	$+2D4    009C4D30      E8 1F432701            CALL MapleSto.01C39054
	$+2D9    009C4D35      8B45 E0                MOV EAX,DWORD PTR SS:[EBP-0x20]
	$+2DC    009C4D38      C645 FC 03             MOV BYTE PTR SS:[EBP-0x4],0x3
	$+2E0    009C4D3C      85C0                   TEST EAX,EAX
	$+2E2    009C4D3E      74 24                  JE SHORT MapleSto.009C4D64
	$+2E4    009C4D40      8378 2C 00             CMP DWORD PTR DS:[EAX+0x2C],0x0
	$+2E8    009C4D44      74 1E                  JE SHORT MapleSto.009C4D64
	$+2EA    009C4D46      8B70 2C                MOV ESI,DWORD PTR DS:[EAX+0x2C]
	$+2ED    009C4D49      68 99020000            PUSH 0x299
	$+2F2    009C4D4E      68 9A020000            PUSH 0x29A
	$+2F7    009C4D53      8BCE                   MOV ECX,ESI
	$+2F9    009C4D55      E8 5664AFFF            CALL MapleSto.004BB1B0
	$+2FE    009C4D5A      50                     PUSH EAX
	$+2FF    009C4D5B      56                     PUSH ESI
	$+300    009C4D5C      E8 BFD7E3FF            CALL MapleSto.00802520
	$+305    009C4D61      83C4 10                ADD ESP,0x10
	$+308    009C4D64      8D4D DC                LEA ECX,DWORD PTR SS:[EBP-0x24]
	$+30B    009C4D67      C645 FC 02             MOV BYTE PTR SS:[EBP-0x4],0x2
	$+30F    009C4D6B      E8 A0830B00            CALL MapleSto.00A7D110
	$+314    009C4D70      EB 05                  JMP SHORT MapleSto.009C4D77
	$+316    009C4D72      E8 A978B4FF            CALL MapleSto.0050C620
	$+31B    009C4D77      8B0D 48417201          MOV ECX,DWORD PTR DS:[0x1724148]
	$+321    009C4D7D      68 484E0000            PUSH 0x4E48
	$+326    009C4D82      E8 39141600            CALL MapleSto.00B261C0
	$+32B    009C4D87      85C0                   TEST EAX,EAX
	$+32D    009C4D89      75 11                  JNZ SHORT MapleSto.009C4D9C
	$+32F    009C4D8B      8B55 D0                MOV EDX,DWORD PTR SS:[EBP-0x30]
	$+332    009C4D8E      8B45 D4                MOV EAX,DWORD PTR SS:[EBP-0x2C]
	$+335    009C4D91      8915 C0887301          MOV DWORD PTR DS:[0x17388C0],EDX
	$+33B    009C4D97      A3 C4887301            MOV DWORD PTR DS:[0x17388C4],EAX
	$+340    009C4D9C      8D4D C8                LEA ECX,DWORD PTR SS:[EBP-0x38]
	$+343    009C4D9F      C745 FC FFFFFFFF       MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+34A    009C4DA6      E8 65830B00            CALL MapleSto.00A7D110
	$+34F    009C4DAB      E9 1B050000            JMP MapleSto.009C52CB
	$+354    009C4DB0      83F8 0B                CMP EAX,0xB
	$+357    009C4DB3      0F85 DD000000          JNZ MapleSto.009C4E96
	$+35D    009C4DB9      8B16                   MOV EDX,DWORD PTR DS:[ESI]
	$+35F    009C4DBB      8B42 10                MOV EAX,DWORD PTR DS:[EDX+0x10]
	$+362    009C4DBE      8BCE                   MOV ECX,ESI
	$+364    009C4DC0      FFD0                   CALL EAX
	$+366    009C4DC2      8955 CC                MOV DWORD PTR SS:[EBP-0x34],EDX
	$+369    009C4DC5      8B16                   MOV EDX,DWORD PTR DS:[ESI]
	$+36B    009C4DC7      8BD8                   MOV EBX,EAX
	$+36D    009C4DC9      8B42 10                MOV EAX,DWORD PTR DS:[EDX+0x10]
	$+370    009C4DCC      8BCE                   MOV ECX,ESI
	$+372    009C4DCE      895D C8                MOV DWORD PTR SS:[EBP-0x38],EBX
	$+375    009C4DD1      FFD0                   CALL EAX
	$+377    009C4DD3      8B4D CC                MOV ECX,DWORD PTR SS:[EBP-0x34]
	$+37A    009C4DD6      51                     PUSH ECX
	$+37B    009C4DD7      8BFA                   MOV EDI,EDX
	$+37D    009C4DD9      53                     PUSH EBX
	$+37E    009C4DDA      8D55 E4                LEA EDX,DWORD PTR SS:[EBP-0x1C]
	$+381    009C4DDD      52                     PUSH EDX
	$+382    009C4DDE      B9 F04C7201            MOV ECX,MapleSto.01724CF0
	$+387    009C4DE3      8BF0                   MOV ESI,EAX
	$+389    009C4DE5      E8 2691BCFF            CALL MapleSto.0058DF10
	$+38E    009C4DEA      8B4D E8                MOV ECX,DWORD PTR SS:[EBP-0x18]
	$+391    009C4DED      C745 FC 04000000       MOV DWORD PTR SS:[EBP-0x4],0x4
	$+398    009C4DF4      85C9                   TEST ECX,ECX
	$+39A    009C4DF6      75 23                  JNZ SHORT MapleSto.009C4E1B
	$+39C    009C4DF8      8D4D E4                LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$+39F    009C4DFB      C745 FC FFFFFFFF       MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+3A6    009C4E02      E8 09830B00            CALL MapleSto.00A7D110
	$+3AB    009C4E07      8B4D F4                MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+3AE    009C4E0A      64:890D 00000000       MOV DWORD PTR FS:[0],ECX
	$+3B5    009C4E11      59                     POP ECX
	$+3B6    009C4E12      5F                     POP EDI
	$+3B7    009C4E13      5E                     POP ESI
	$+3B8    009C4E14      5B                     POP EBX
	$+3B9    009C4E15      8BE5                   MOV ESP,EBP
	$+3BB    009C4E17      5D                     POP EBP
	$+3BC    009C4E18      C2 0400                RETN 0x4
	$+3BF    009C4E1B      57                     PUSH EDI
	$+3C0    009C4E1C      56                     PUSH ESI
	$+3C1    009C4E1D      E8 FE77B4FF            CALL MapleSto.0050C620
	$+3C6    009C4E22      8B4D E8                MOV ECX,DWORD PTR SS:[EBP-0x18]
	$+3C9    009C4E25      57                     PUSH EDI
	$+3CA    009C4E26      56                     PUSH ESI
	$+3CB    009C4E27      E8 2478B4FF            CALL MapleSto.0050C650
	$+3D0    009C4E2C      8B4D E8                MOV ECX,DWORD PTR SS:[EBP-0x18]
	$+3D3    009C4E2F      57                     PUSH EDI
	$+3D4    009C4E30      56                     PUSH ESI
	$+3D5    009C4E31      E8 5A78B4FF            CALL MapleSto.0050C690
	$+3DA    009C4E36      8B4D E8                MOV ECX,DWORD PTR SS:[EBP-0x18]
	$+3DD    009C4E39      57                     PUSH EDI
	$+3DE    009C4E3A      56                     PUSH ESI
	$+3DF    009C4E3B      E8 9078B4FF            CALL MapleSto.0050C6D0
	$+3E4    009C4E40      33DB                   XOR EBX,EBX
	$+3E6    009C4E42      8B4D E8                MOV ECX,DWORD PTR SS:[EBP-0x18]
	$+3E9    009C4E45      57                     PUSH EDI
	$+3EA    009C4E46      56                     PUSH ESI
	$+3EB    009C4E47      53                     PUSH EBX
	$+3EC    009C4E48      E8 D381B4FF            CALL MapleSto.0050D020
	$+3F1    009C4E4D      43                     INC EBX
	$+3F2    009C4E4E      83FB 05                CMP EBX,0x5
	$+3F5    009C4E51    ^ 7C EF                  JL SHORT MapleSto.009C4E42
	$+3F7    009C4E53      8B0D 48417201          MOV ECX,DWORD PTR DS:[0x1724148]
	$+3FD    009C4E59      68 484E0000            PUSH 0x4E48
	$+402    009C4E5E      E8 5D131600            CALL MapleSto.00B261C0
	$+407    009C4E63      85C0                   TEST EAX,EAX
	$+409    009C4E65      75 1B                  JNZ SHORT MapleSto.009C4E82
	$+40B    009C4E67      8B45 C8                MOV EAX,DWORD PTR SS:[EBP-0x38]
	$+40E    009C4E6A      8B4D CC                MOV ECX,DWORD PTR SS:[EBP-0x34]
	$+411    009C4E6D      A3 C0887301            MOV DWORD PTR DS:[0x17388C0],EAX
	$+416    009C4E72      890D C4887301          MOV DWORD PTR DS:[0x17388C4],ECX
	$+41C    009C4E78      C705 B8887301 01000000 MOV DWORD PTR DS:[0x17388B8],0x1
	$+426    009C4E82      8D4D E4                LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$+429    009C4E85      C745 FC FFFFFFFF       MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+430    009C4E8C      E8 7F820B00            CALL MapleSto.00A7D110
	$+435    009C4E91      E9 35040000            JMP MapleSto.009C52CB
	$+43A    009C4E96      83F8 0C                CMP EAX,0xC
	$+43D    009C4E99      0F85 85000000          JNZ MapleSto.009C4F24
	$+443    009C4E9F      8B16                   MOV EDX,DWORD PTR DS:[ESI]
	$+445    009C4EA1      8B42 10                MOV EAX,DWORD PTR DS:[EDX+0x10]
	$+448    009C4EA4      8BCE                   MOV ECX,ESI
	$+44A    009C4EA6      FFD0                   CALL EAX
	$+44C    009C4EA8      8BDA                   MOV EBX,EDX
	$+44E    009C4EAA      8B16                   MOV EDX,DWORD PTR DS:[ESI]
	$+450    009C4EAC      8BF8                   MOV EDI,EAX
	$+452    009C4EAE      8B42 10                MOV EAX,DWORD PTR DS:[EDX+0x10]
	$+455    009C4EB1      8BCE                   MOV ECX,ESI
	$+457    009C4EB3      FFD0                   CALL EAX
	$+459    009C4EB5      53                     PUSH EBX
	$+45A    009C4EB6      57                     PUSH EDI
	$+45B    009C4EB7      8D4D C8                LEA ECX,DWORD PTR SS:[EBP-0x38]
	$+45E    009C4EBA      51                     PUSH ECX
	$+45F    009C4EBB      B9 F04C7201            MOV ECX,MapleSto.01724CF0
	$+464    009C4EC0      8BF0                   MOV ESI,EAX
	$+466    009C4EC2      8955 D4                MOV DWORD PTR SS:[EBP-0x2C],EDX
	$+469    009C4EC5      E8 4690BCFF            CALL MapleSto.0058DF10
	$+46E    009C4ECA      8B4D CC                MOV ECX,DWORD PTR SS:[EBP-0x34]
	$+471    009C4ECD      C745 FC 05000000       MOV DWORD PTR SS:[EBP-0x4],0x5
	$+478    009C4ED4      85C9                   TEST ECX,ECX
	$+47A    009C4ED6    ^ 0F84 15FEFFFF          JE MapleSto.009C4CF1
	$+480    009C4EDC      8B55 D4                MOV EDX,DWORD PTR SS:[EBP-0x2C]
	$+483    009C4EDF      52                     PUSH EDX
	$+484    009C4EE0      56                     PUSH ESI
	$+485    009C4EE1      E8 4A76B4FF            CALL MapleSto.0050C530
	$+48A    009C4EE6      8B0D 48417201          MOV ECX,DWORD PTR DS:[0x1724148]
	$+490    009C4EEC      68 484E0000            PUSH 0x4E48
	$+495    009C4EF1      E8 CA121600            CALL MapleSto.00B261C0
	$+49A    009C4EF6      85C0                   TEST EAX,EAX
	$+49C    009C4EF8      75 16                  JNZ SHORT MapleSto.009C4F10
	$+49E    009C4EFA      893D C0887301          MOV DWORD PTR DS:[0x17388C0],EDI
	$+4A4    009C4F00      891D C4887301          MOV DWORD PTR DS:[0x17388C4],EBX
	$+4AA    009C4F06      C705 B8887301 01000000 MOV DWORD PTR DS:[0x17388B8],0x1
	$+4B4    009C4F10      8D4D C8                LEA ECX,DWORD PTR SS:[EBP-0x38]
	$+4B7    009C4F13      C745 FC FFFFFFFF       MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+4BE    009C4F1A      E8 F1810B00            CALL MapleSto.00A7D110
	$+4C3    009C4F1F      E9 A7030000            JMP MapleSto.009C52CB
	$+4C8    009C4F24      83F8 0D                CMP EAX,0xD
	$+4CB    009C4F27      75 40                  JNZ SHORT MapleSto.009C4F69
	$+4CD    009C4F29      8B06                   MOV EAX,DWORD PTR DS:[ESI]
	$+4CF    009C4F2B      8B50 10                MOV EDX,DWORD PTR DS:[EAX+0x10]
	$+4D2    009C4F2E      8BCE                   MOV ECX,ESI
	$+4D4    009C4F30      FFD2                   CALL EDX
	$+4D6    009C4F32      6A 41                  PUSH 0x41
	$+4D8    009C4F34      6A 01                  PUSH 0x1
	$+4DA    009C4F36      52                     PUSH EDX
	$+4DB    009C4F37      50                     PUSH EAX
	$+4DC    009C4F38      B9 F04C7201            MOV ECX,MapleSto.01724CF0
	$+4E1    009C4F3D      E8 4EA0BCFF            CALL MapleSto.0058EF90
	$+4E6    009C4F42      8B0D 48417201          MOV ECX,DWORD PTR DS:[0x1724148]
	$+4EC    009C4F48      68 484E0000            PUSH 0x4E48
	$+4F1    009C4F4D      E8 6E121600            CALL MapleSto.00B261C0
	$+4F6    009C4F52      85C0                   TEST EAX,EAX
	$+4F8    009C4F54      0F85 71030000          JNZ MapleSto.009C52CB
	$+4FE    009C4F5A      C705 BC887301 01000000 MOV DWORD PTR DS:[0x17388BC],0x1
	$+508    009C4F64      E9 62030000            JMP MapleSto.009C52CB
	$+50D    009C4F69      83F8 0E                CMP EAX,0xE
	$+510    009C4F6C      0F85 8A000000          JNZ MapleSto.009C4FFC
	$+516    009C4F72      8B06                   MOV EAX,DWORD PTR DS:[ESI]
	$+518    009C4F74      8B50 0C                MOV EDX,DWORD PTR DS:[EAX+0xC]
	$+51B    009C4F77      8BCE                   MOV ECX,ESI
	$+51D    009C4F79      FFD2                   CALL EDX
	$+51F    009C4F7B      8BF0                   MOV ESI,EAX
	$+521    009C4F7D      56                     PUSH ESI
	$+522    009C4F7E      B9 F04C7201            MOV ECX,MapleSto.01724CF0
	$+527    009C4F83      E8 903A2C01            CALL MapleSto.01C88A18
	$+52C    009C4F88      85F6                   TEST ESI,ESI
	$+52E    009C4F8A      74 20                  JE SHORT MapleSto.009C4FAC
	$+530    009C4F8C      8B0D 2C417201          MOV ECX,DWORD PTR DS:[0x172412C]
	$+536    009C4F92      E8 E9DA1B00            CALL MapleSto.00B82A80
	$+53B    009C4F97      84C0                   TEST AL,AL
	$+53D    009C4F99      75 11                  JNZ SHORT MapleSto.009C4FAC
	$+53F    009C4F9B      6A 01                  PUSH 0x1
	$+541    009C4F9D      6A 08                  PUSH 0x8
	$+543    009C4F9F      68 16020000            PUSH 0x216
	$+548    009C4FA4      E8 4CBDE3FF            CALL MapleSto.00800CF5
	$+54D    009C4FA9      83C4 0C                ADD ESP,0xC
	$+550    009C4FAC      8B0D 48417201          MOV ECX,DWORD PTR DS:[0x1724148]
	$+556    009C4FB2      68 484E0000            PUSH 0x4E48
	$+55B    009C4FB7      E8 04121600            CALL MapleSto.00B261C0
	$+560    009C4FBC      85C0                   TEST EAX,EAX
	$+562    009C4FBE      0F85 07030000          JNZ MapleSto.009C52CB
	$+568    009C4FC4      50                     PUSH EAX
	$+569    009C4FC5      6A 26                  PUSH 0x26
	$+56B    009C4FC7      8D4D A8                LEA ECX,DWORD PTR SS:[EBP-0x58]
	$+56E    009C4FCA      E8 4162B6FF            CALL MapleSto.0052B210
	$+573    009C4FCF      6A 00                  PUSH 0x0
	$+575    009C4FD1      8D4D A8                LEA ECX,DWORD PTR SS:[EBP-0x58]
	$+578    009C4FD4      C745 FC 06000000       MOV DWORD PTR SS:[EBP-0x4],0x6
	$+57F    009C4FDB      E8 F8A82301            CALL MapleSto.01BFF8D8
	$+584    009C4FE0      8D4D A8                LEA ECX,DWORD PTR SS:[EBP-0x58]
	$+587    009C4FE3      E8 09FF2801            CALL MapleSto.01C54EF1
	$+58C    009C4FE8      8D4D A8                LEA ECX,DWORD PTR SS:[EBP-0x58]
	$+58F    009C4FEB      C745 FC FFFFFFFF       MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+596    009C4FF2      E8 3956AFFF            CALL MapleSto.004BA630
	$+59B    009C4FF7      E9 CF020000            JMP MapleSto.009C52CB
	$+5A0    009C4FFC      83F8 0F                CMP EAX,0xF
	$+5A3    009C4FFF      75 55                  JNZ SHORT MapleSto.009C5056
	$+5A5    009C5001      8B0D 2C417201          MOV ECX,DWORD PTR DS:[0x172412C]
	$+5AB    009C5007      E8 74DA1B00            CALL MapleSto.00B82A80
	$+5B0    009C500C      84C0                   TEST AL,AL
	$+5B2    009C500E      75 11                  JNZ SHORT MapleSto.009C5021
	$+5B4    009C5010      6A 00                  PUSH 0x0
	$+5B6    009C5012      6A 01                  PUSH 0x1
	$+5B8    009C5014      68 42020000            PUSH 0x242
	$+5BD    009C5019      E8 D7BCE3FF            CALL MapleSto.00800CF5
	$+5C2    009C501E      83C4 0C                ADD ESP,0xC
	$+5C5    009C5021      8D45 E8                LEA EAX,DWORD PTR SS:[EBP-0x18]
	$+5C8    009C5024      68 43020000            PUSH 0x243
	$+5CD    009C5029      50                     PUSH EAX
	$+5CE    009C502A      E8 5124A8FF            CALL MapleSto.00447480
	$+5D3    009C502F      6A 0E                  PUSH 0xE
	$+5D5    009C5031      50                     PUSH EAX
	$+5D6    009C5032      C745 FC 07000000       MOV DWORD PTR SS:[EBP-0x4],0x7
	$+5DD    009C5039      E8 22D5E3FF            CALL MapleSto.00802560
	$+5E2    009C503E      83C4 10                ADD ESP,0x10
	$+5E5    009C5041      8D4D E8                LEA ECX,DWORD PTR SS:[EBP-0x18]
	$+5E8    009C5044      C745 FC FFFFFFFF       MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+5EF    009C504B      FF15 DCA13E01          CALL DWORD PTR DS:[0x13EA1DC]               ; mfc90.#601
	$+5F5    009C5051      E9 75020000            JMP MapleSto.009C52CB
	$+5FA    009C5056      83F8 14                CMP EAX,0x14
	$+5FD    009C5059      0F85 6C020000          JNZ MapleSto.009C52CB
	$+603    009C505F      8B16                   MOV EDX,DWORD PTR DS:[ESI]
	$+605    009C5061      8B42 04                MOV EAX,DWORD PTR DS:[EDX+0x4]
	$+608    009C5064      8BCE                   MOV ECX,ESI
	$+60A    009C5066      FFD0                   CALL EAX
	$+60C    009C5068      8B16                   MOV EDX,DWORD PTR DS:[ESI]
	$+60E    009C506A      0FB6C8                 MOVZX ECX,AL
	$+611    009C506D      8B42 04                MOV EAX,DWORD PTR DS:[EDX+0x4]
	$+614    009C5070      894D EC                MOV DWORD PTR SS:[EBP-0x14],ECX
	$+617    009C5073      8BCE                   MOV ECX,ESI
	$+619    009C5075      FFD0                   CALL EAX
	$+61B    009C5077      0FB6F0                 MOVZX ESI,AL
	$+61E    009C507A      8D4D E0                LEA ECX,DWORD PTR SS:[EBP-0x20]
	$+621    009C507D      8975 E8                MOV DWORD PTR SS:[EBP-0x18],ESI
	$+624    009C5080      FF15 00A23E01          CALL DWORD PTR DS:[0x13EA200]               ; mfc90.#316
	$+62A    009C5086      8D46 FF                LEA EAX,DWORD PTR DS:[ESI-0x1]
	$+62D    009C5089      C745 FC 08000000       MOV DWORD PTR SS:[EBP-0x4],0x8
	$+634    009C5090      83F8 20                CMP EAX,0x20
	$+637    009C5093      0F87 09020000          JA MapleSto.009C52A2
	$+63D    009C5099      0FB688 64539C00        MOVZX ECX,BYTE PTR DS:[EAX+0x9C5364]
	$+644    009C50A0      FF248D 10539C00        JMP DWORD PTR DS:[ECX*4+0x9C5310]
	*/
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("33C5508D45F464A3000000008B75088B46048B500483C6048BCEFFD20FB6C08945D883F810", 0x27 + 1);
	if (!op_dw_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *op_dw_tmp;
	CheckExeAddr(dw_tmp);
	ofs << "pt_cdword	g_a_mail_mgr_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;
	return true;
}

bool CAutoUpdateApp::Scan_Npc(Cmn_FC_OFstream& ofs)
{
	return true;
}

bool CAutoUpdateApp::Scan_Map(Cmn_FC_OFstream& ofs)
{
	pt_pcvoid addr_tmp;
	pt_dword dw_tmp;
	boost::optional<pt_dword> op_dw_tmp;

	/*
	$-165    008A76CF      CC                  INT3
	$-164    008A76D0      55                  PUSH EBP
	$-163    008A76D1      8BEC                MOV EBP,ESP
	$-161    008A76D3      51                  PUSH ECX
	$-160    008A76D4      57                  PUSH EDI
	$-15F    008A76D5      8BF9                MOV EDI,ECX
	$-15D    008A76D7      8A47 1D             MOV AL,BYTE PTR DS:[EDI+0x1D]
	$-15A    008A76DA      8447 1C             TEST BYTE PTR DS:[EDI+0x1C],AL
	$-157    008A76DD      74 5B               JE SHORT MapleSto.008A773A
	$-155    008A76DF      56                  PUSH ESI
	$-154    008A76E0      8B75 08             MOV ESI,DWORD PTR SS:[EBP+0x8]
	$-151    008A76E3      68 010000F0         PUSH 0xF0000001
	$-14C    008A76E8      8D4D FF             LEA ECX,DWORD PTR SS:[EBP-0x1]
	$-149    008A76EB      51                  PUSH ECX
	$-148    008A76EC      8B4E 10             MOV ECX,DWORD PTR DS:[ESI+0x10]
	$-145    008A76EF      68 0E86F923         PUSH 0x23F9860E
	$-140    008A76F4      C645 FF 00          MOV BYTE PTR SS:[EBP-0x1],0x0
	$-13C    008A76F8      E8 03275800         CALL MapleSto.00E29E00
	$-137    008A76FD      807D FF 00          CMP BYTE PTR SS:[EBP-0x1],0x0
	$-133    008A7701      74 36               JE SHORT MapleSto.008A7739
	$-131    008A7703      8B46 10             MOV EAX,DWORD PTR DS:[ESI+0x10]
	$-12E    008A7706      50                  PUSH EAX
	$-12D    008A7707      8BCF                MOV ECX,EDI
	$-12B    008A7709      E8 02FEFFFF         CALL MapleSto.008A7510
	$-126    008A770E      85C0                TEST EAX,EAX
	$-124    008A7710      74 14               JE SHORT MapleSto.008A7726
	$-122    008A7712      83B8 C4000000 00    CMP DWORD PTR DS:[EAX+0xC4],0x0
	$-11B    008A7719      75 0B               JNZ SHORT MapleSto.008A7726
	$-119    008A771B      8B56 10             MOV EDX,DWORD PTR DS:[ESI+0x10]
	$-116    008A771E      52                  PUSH EDX
	$-115    008A771F      8BC8                MOV ECX,EAX
	$-113    008A7721      E8 7A86F1FF         CALL MapleSto.007BFDA0
	$-10E    008A7726      8B76 10             MOV ESI,DWORD PTR DS:[ESI+0x10]
	$-10B    008A7729      8B46 54             MOV EAX,DWORD PTR DS:[ESI+0x54]
	$-108    008A772C      8B4E 50             MOV ECX,DWORD PTR DS:[ESI+0x50]
	$-105    008A772F      50                  PUSH EAX
	$-104    008A7730      51                  PUSH ECX
	$-103    008A7731      8D4F 24             LEA ECX,DWORD PTR DS:[EDI+0x24]
	$-100    008A7734      E8 67470000         CALL MapleSto.008ABEA0
	$-FB     008A7739      5E                  POP ESI
	$-FA     008A773A      5F                  POP EDI
	$-F9     008A773B      8BE5                MOV ESP,EBP
	$-F7     008A773D      5D                  POP EBP
	$-F6     008A773E      C2 0C00             RETN 0xC
	$-F3     008A7741      CC                  INT3
	$-F2     008A7742      CC                  INT3
	$-F1     008A7743      CC                  INT3
	$-F0     008A7744      CC                  INT3
	$-EF     008A7745      CC                  INT3
	$-EE     008A7746      CC                  INT3
	$-ED     008A7747      CC                  INT3
	$-EC     008A7748      CC                  INT3
	$-EB     008A7749      CC                  INT3
	$-EA     008A774A      CC                  INT3
	$-E9     008A774B      CC                  INT3
	$-E8     008A774C      CC                  INT3
	$-E7     008A774D      CC                  INT3
	$-E6     008A774E      CC                  INT3
	$-E5     008A774F      CC                  INT3
	$-E4     008A7750      55                  PUSH EBP
	$-E3     008A7751      8BEC                MOV EBP,ESP
	$-E1     008A7753      83EC 28             SUB ESP,0x28
	$-DE     008A7756      53                  PUSH EBX
	$-DD     008A7757      56                  PUSH ESI
	$-DC     008A7758      8B75 08             MOV ESI,DWORD PTR SS:[EBP+0x8]
	$-D9     008A775B      33DB                XOR EBX,EBX
	$-D7     008A775D      57                  PUSH EDI
	$-D6     008A775E      894D F4             MOV DWORD PTR SS:[EBP-0xC],ECX
	$-D3     008A7761      3BF3                CMP ESI,EBX
	$-D1     008A7763      0F84 15010000       JE MapleSto.008A787E
	$-CB     008A7769      8B7D 0C             MOV EDI,DWORD PTR SS:[EBP+0xC]
	$-C8     008A776C      3BFB                CMP EDI,EBX
	$-C6     008A776E      0F84 0A010000       JE MapleSto.008A787E
	$-C0     008A7774      68 010000F0         PUSH 0xF0000001
	$-BB     008A7779      8D45 0B             LEA EAX,DWORD PTR SS:[EBP+0xB]
	$-B8     008A777C      50                  PUSH EAX
	$-B7     008A777D      68 0E86F923         PUSH 0x23F9860E
	$-B2     008A7782      8BCF                MOV ECX,EDI
	$-B0     008A7784      885D 0B             MOV BYTE PTR SS:[EBP+0xB],BL
	$-AD     008A7787      E8 74265800         CALL MapleSto.00E29E00
	$-A8     008A778C      385D 0B             CMP BYTE PTR SS:[EBP+0xB],BL
	$-A5     008A778F      0F84 E9000000       JE MapleSto.008A787E
	$-9F     008A7795      391D 48357301       CMP DWORD PTR DS:[0x1733548],EBX
	$-99     008A779B      74 19               JE SHORT MapleSto.008A77B6
	$-97     008A779D      8B86 94000000       MOV EAX,DWORD PTR DS:[ESI+0x94]
	$-91     008A77A3      8D48 01             LEA ECX,DWORD PTR DS:[EAX+0x1]
	$-8E     008A77A6      3B4E 20             CMP ECX,DWORD PTR DS:[ESI+0x20]
	$-8B     008A77A9      76 0B               JBE SHORT MapleSto.008A77B6
	$-89     008A77AB      8D1400              LEA EDX,DWORD PTR DS:[EAX+EAX]
	$-86     008A77AE      52                  PUSH EDX
	$-85     008A77AF      8BCE                MOV ECX,ESI
	$-83     008A77B1      E8 251DF2FF         CALL MapleSto.007C94DB
	$-7E     008A77B6      57                  PUSH EDI
	$-7D     008A77B7      8BCE                MOV ECX,ESI
	$-7B     008A77B9      E8 B290F1FF         CALL MapleSto.007C0870
	$-76     008A77BE      85C0                TEST EAX,EAX
	$-74     008A77C0      0F84 AA000000       JE MapleSto.008A7870
	$-6E     008A77C6      FF86 94000000       INC DWORD PTR DS:[ESI+0x94]
	$-68     008A77CC      8BCE                MOV ECX,ESI
	$-66     008A77CE      E8 9DD70400         CALL MapleSto.008F4F70
	$-61     008A77D3      C646 25 01          MOV BYTE PTR DS:[ESI+0x25],0x1
	$-5D     008A77D7      E8 C4910000         CALL MapleSto.008B09A0
	$-58     008A77DC      8945 F8             MOV DWORD PTR SS:[EBP-0x8],EAX
	$-55     008A77DF      8D45 0F             LEA EAX,DWORD PTR SS:[EBP+0xF]
	$-52     008A77E2      50                  PUSH EAX
	$-51     008A77E3      68 79B1B6A0         PUSH 0xA0B6B179
	$-4C     008A77E8      8BCF                MOV ECX,EDI
	$-4A     008A77EA      885D 0F             MOV BYTE PTR SS:[EBP+0xF],BL
	$-47     008A77ED      E8 8EE6D0FF         CALL MapleSto.005B5E80
	$-42     008A77F2      8D4D FF             LEA ECX,DWORD PTR SS:[EBP-0x1]
	$-3F     008A77F5      51                  PUSH ECX
	$-3E     008A77F6      68 F4D97F2A         PUSH 0x2A7FD9F4
	$-39     008A77FB      8BCF                MOV ECX,EDI
	$-37     008A77FD      C645 FF 01          MOV BYTE PTR SS:[EBP-0x1],0x1
	$-33     008A7801      E8 7AE6D0FF         CALL MapleSto.005B5E80
	$-2E     008A7806      8B0D 2C417201       MOV ECX,DWORD PTR DS:[0x172412C]
	$-28     008A780C      8D5F 50             LEA EBX,DWORD PTR DS:[EDI+0x50]
	$-25     008A780F      53                  PUSH EBX
	$-24     008A7810      E8 BBCD2D00         CALL MapleSto.00B845D0
	$-1F     008A7815      84C0                TEST AL,AL
	$-1D     008A7817      75 28               JNZ SHORT MapleSto.008A7841
	$-1B     008A7819      57                  PUSH EDI
	$-1A     008A781A      E8 B1EE2B00         CALL MapleSto.00B666D0
	$-15     008A781F      8B4D F8             MOV ECX,DWORD PTR SS:[EBP-0x8]
	$-12     008A7822      8B11                MOV EDX,DWORD PTR DS:[ECX]
	$-10     008A7824      83C4 04             ADD ESP,0x4
	$-D      008A7827      50                  PUSH EAX
	$-C      008A7828      8945 0C             MOV DWORD PTR SS:[EBP+0xC],EAX
	$-9      008A782B      8B42 50             MOV EAX,DWORD PTR DS:[EDX+0x50]
	$-6      008A782E      57                  PUSH EDI
	$-5      008A782F      FFD0                CALL EAX
	$-3      008A7831      8B4D 0C             MOV ECX,DWORD PTR SS:[EBP+0xC]
	$ ==>    008A7834      51                  PUSH ECX
	$+1      008A7835      8B0D 9CE47201       MOV ECX,DWORD PTR DS:[0x172E49C]                 ; g_a_light_service_
	$+7      008A783B      57                  PUSH EDI
	$+8      008A783C      E8 CF930600         CALL MapleSto.00910C10
	$+D      008A7841      8B13                MOV EDX,DWORD PTR DS:[EBX]
	$+F      008A7843      8B43 04             MOV EAX,DWORD PTR DS:[EBX+0x4]
	$+12     008A7846      8D4D D8             LEA ECX,DWORD PTR SS:[EBP-0x28]
	$+15     008A7849      51                  PUSH ECX
	$+16     008A784A      8955 D8             MOV DWORD PTR SS:[EBP-0x28],EDX
	$+19     008A784D      8945 DC             MOV DWORD PTR SS:[EBP-0x24],EAX
	$+1C     008A7850      8B45 F4             MOV EAX,DWORD PTR SS:[EBP-0xC]
	$+1F     008A7853      8D55 E8             LEA EDX,DWORD PTR SS:[EBP-0x18]
	$+22     008A7856      8D48 20             LEA ECX,DWORD PTR DS:[EAX+0x20]
	$+25     008A7859      52                  PUSH EDX
	$+26     008A785A      51                  PUSH ECX
	$+27     008A785B      8D48 54             LEA ECX,DWORD PTR DS:[EAX+0x54]
	$+2A     008A785E      8975 E0             MOV DWORD PTR SS:[EBP-0x20],ESI
	$+2D     008A7861      E8 1A730000         CALL MapleSto.008AEB80
	$+32     008A7866      8BC8                MOV ECX,EAX
	$+34     008A7868      83C1 64             ADD ECX,0x64
	$+37     008A786B      E8 60E4FFFF         CALL MapleSto.008A5CD0
	$+3C     008A7870      5F                  POP EDI
	$+3D     008A7871      5E                  POP ESI
	$+3E     008A7872      B8 01000000         MOV EAX,0x1
	$+43     008A7877      5B                  POP EBX
	$+44     008A7878      8BE5                MOV ESP,EBP
	$+46     008A787A      5D                  POP EBP
	$+47     008A787B      C2 0800             RETN 0x8
	$+4A     008A787E      5F                  POP EDI
	$+4B     008A787F      5E                  POP ESI
	$+4C     008A7880      33C0                XOR EAX,EAX
	$+4E     008A7882      5B                  POP EBX
	$+4F     008A7883      8BE5                MOV ESP,EBP
	$+51     008A7885      5D                  POP EBP
	$+52     008A7886      C2 0800             RETN 0x8
	$+55     008A7889      CC                  INT3
	$+56     008A788A      CC                  INT3
	$+57     008A788B      CC                  INT3
	$+58     008A788C      CC                  INT3
	$+59     008A788D      CC                  INT3
	$+5A     008A788E      CC                  INT3
	$+5B     008A788F      CC                  INT3
	$+5C     008A7890      55                  PUSH EBP
	$+5D     008A7891      8BEC                MOV EBP,ESP
	$+5F     008A7893      6A FF               PUSH -0x1
	$+61     008A7895      68 425C3501         PUSH MapleSto.01355C42
	$+66     008A789A      64:A1 00000000      MOV EAX,DWORD PTR FS:[0]
	$+6C     008A78A0      50                  PUSH EAX
	$+6D     008A78A1      81EC E0000000       SUB ESP,0xE0
	$+73     008A78A7      A1 D0687101         MOV EAX,DWORD PTR DS:[0x17168D0]
	$+78     008A78AC      33C5                XOR EAX,EBP
	$+7A     008A78AE      8945 F0             MOV DWORD PTR SS:[EBP-0x10],EAX
	$+7D     008A78B1      53                  PUSH EBX
	$+7E     008A78B2      56                  PUSH ESI
	$+7F     008A78B3      57                  PUSH EDI
	$+80     008A78B4      50                  PUSH EAX
	$+81     008A78B5      8D45 F4             LEA EAX,DWORD PTR SS:[EBP-0xC]
	$+84     008A78B8      64:A3 00000000      MOV DWORD PTR FS:[0],EAX
	$+8A     008A78BE      8B75 08             MOV ESI,DWORD PTR SS:[EBP+0x8]
	$+8D     008A78C1      33DB                XOR EBX,EBX
	$+8F     008A78C3      8BF9                MOV EDI,ECX
	$+91     008A78C5      89BD 58FFFFFF       MOV DWORD PTR SS:[EBP-0xA8],EDI
	$+97     008A78CB      3BF3                CMP ESI,EBX
	$+99     008A78CD      0F84 82000000       JE MapleSto.008A7955
	$+9F     008A78D3      53                  PUSH EBX
	$+A0     008A78D4      53                  PUSH EBX
	$+A1     008A78D5      56                  PUSH ESI
	$+A2     008A78D6      8D8D 64FFFFFF       LEA ECX,DWORD PTR SS:[EBP-0x9C]
	$+A8     008A78DC      E8 7FEAFFFF         CALL MapleSto.008A6360
	$+AD     008A78E1      8B45 0C             MOV EAX,DWORD PTR SS:[EBP+0xC]
	$+B0     008A78E4      8D8D 64FFFFFF       LEA ECX,DWORD PTR SS:[EBP-0x9C]
	$+B6     008A78EA      51                  PUSH ECX
	$+B7     008A78EB      8BCF                MOV ECX,EDI
	$+B9     008A78ED      895D FC             MOV DWORD PTR SS:[EBP-0x4],EBX
	$+BC     008A78F0      C745 C8 01000000    MOV DWORD PTR SS:[EBP-0x38],0x1
	$+C3     008A78F7      8945 B0             MOV DWORD PTR SS:[EBP-0x50],EAX
	$+C6     008A78FA      E8 41FBFFFF         CALL MapleSto.008A7440
	$+CB     008A78FF      3BC3                CMP EAX,EBX
	$+CD     008A7901      75 40               JNZ SHORT MapleSto.008A7943
	$+CF     008A7903      8D95 63FFFFFF       LEA EDX,DWORD PTR SS:[EBP-0x9D]
	$+D5     008A7909      52                  PUSH EDX
	$+D6     008A790A      68 E02D3F01         PUSH MapleSto.013F2DE0
	$+DB     008A790F      8D4D D0             LEA ECX,DWORD PTR SS:[EBP-0x30]
	$+DE     008A7912      C745 CC 00D24201    MOV DWORD PTR SS:[EBP-0x34],MapleSto.0142D200    ; ASCII "熊g"
	$+E5     008A7919      E8 02A4CCFF         CALL MapleSto.00571D20
	$+EA     008A791E      885D E8             MOV BYTE PTR SS:[EBP-0x18],BL
	$+ED     008A7921      895D EC             MOV DWORD PTR SS:[EBP-0x14],EBX
	$+F0     008A7924      8D5E 50             LEA EBX,DWORD PTR DS:[ESI+0x50]
	$+F3     008A7927      53                  PUSH EBX
	$+F4     008A7928      8BCF                MOV ECX,EDI
	$+F6     008A792A      C645 FC 01          MOV BYTE PTR SS:[EBP-0x4],0x1
	$+FA     008A792E      E8 DDF6FFFF         CALL MapleSto.008A7010
	$+FF     008A7933      85C0                TEST EAX,EAX
	$+101    008A7935      74 25               JE SHORT MapleSto.008A795C
	$+103    008A7937      8D4D CC             LEA ECX,DWORD PTR SS:[EBP-0x34]
	$+106    008A793A      C645 FC 00          MOV BYTE PTR SS:[EBP-0x4],0x0
	$+10A    008A793E      E8 3D63DDFF         CALL MapleSto.0067DC80
	$+10F    008A7943      8D8D 64FFFFFF       LEA ECX,DWORD PTR SS:[EBP-0x9C]
	$+115    008A7949      C745 FC FFFFFFFF    MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+11C    008A7950      E8 BBE9FFFF         CALL MapleSto.008A6310
	$+121    008A7955      33C0                XOR EAX,EAX
	$+123    008A7957      E9 CC010000         JMP MapleSto.008A7B28
	$+128    008A795C      68 010000F0         PUSH 0xF0000001
	$+12D    008A7961      8D85 62FFFFFF       LEA EAX,DWORD PTR SS:[EBP-0x9E]
	$+133    008A7967      50                  PUSH EAX
	$+134    008A7968      68 0E86F923         PUSH 0x23F9860E
	$+139    008A796D      8BCE                MOV ECX,ESI
	$+13B    008A796F      C685 62FFFFFF 00    MOV BYTE PTR SS:[EBP-0x9E],0x0
	$+142    008A7976      E8 85245800         CALL MapleSto.00E29E00
	$+147    008A797B      80BD 62FFFFFF 00    CMP BYTE PTR SS:[EBP-0x9E],0x0
	$+14E    008A7982    ^ 74 B3               JE SHORT MapleSto.008A7937
	$+150    008A7984      8D8D 14FFFFFF       LEA ECX,DWORD PTR SS:[EBP-0xEC]
	$+156    008A798A      56                  PUSH ESI
	$+157    008A798B      51                  PUSH ECX
	$+158    008A798C      E8 9FA40000         CALL MapleSto.008B1E30
	$+15D    008A7991      83C4 08             ADD ESP,0x8
	$+160    008A7994      50                  PUSH EAX
	$+161    008A7995      8D4D CC             LEA ECX,DWORD PTR SS:[EBP-0x34]
	$+164    008A7998      C645 FC 02          MOV BYTE PTR SS:[EBP-0x4],0x2
	$+168    008A799C      E8 8FDEF1FF         CALL MapleSto.007C5830
	$+16D    008A79A1      8D8D 14FFFFFF       LEA ECX,DWORD PTR SS:[EBP-0xEC]
	$+173    008A79A7      C645 FC 01          MOV BYTE PTR SS:[EBP-0x4],0x1
	$+177    008A79AB      E8 D062DDFF         CALL MapleSto.0067DC80
	$+17C    008A79B0      68 B4377401         PUSH MapleSto.017437B4
	$+181    008A79B5      8D4D D0             LEA ECX,DWORD PTR SS:[EBP-0x30]
	$+184    008A79B8      E8 A3E3CFFF         CALL MapleSto.005A5D60
	$+189    008A79BD      85C0                TEST EAX,EAX
	$+18B    008A79BF    ^ 0F84 72FFFFFF       JE MapleSto.008A7937
	$+191    008A79C5      56                  PUSH ESI
	$+192    008A79C6      E8 65F2FFFF         CALL MapleSto.008A6C30
	$+197    008A79CB      83C4 04             ADD ESP,0x4
	$+19A    008A79CE      84C0                TEST AL,AL
	$+19C    008A79D0    ^ 0F85 61FFFFFF       JNZ MapleSto.008A7937
	$+1A2    008A79D6      68 E8000000         PUSH 0xE8
	$+1A7    008A79DB      E8 8E959500         CALL MapleSto.01200F6E                           ; JMP to mfc90.#798
	$+1AC    008A79E0      83C4 04             ADD ESP,0x4
	$+1AF    008A79E3      8985 5CFFFFFF       MOV DWORD PTR SS:[EBP-0xA4],EAX
	$+1B5    008A79E9      C645 FC 03          MOV BYTE PTR SS:[EBP-0x4],0x3
	$+1B9    008A79ED      85C0                TEST EAX,EAX
	$+1BB    008A79EF      74 12               JE SHORT MapleSto.008A7A03
	$+1BD    008A79F1      8D95 64FFFFFF       LEA EDX,DWORD PTR SS:[EBP-0x9C]
	$+1C3    008A79F7      52                  PUSH EDX
	$+1C4    008A79F8      8BC8                MOV ECX,EAX
	$+1C6    008A79FA      E8 1182F1FF         CALL MapleSto.007BFC10
	$+1CB    008A79FF      8BF8                MOV EDI,EAX
	$+1CD    008A7A01      EB 02               JMP SHORT MapleSto.008A7A05
	$+1CF    008A7A03      33FF                XOR EDI,EDI
	$+1D1    008A7A05      8B45 10             MOV EAX,DWORD PTR SS:[EBP+0x10]
	$+1D4    008A7A08      50                  PUSH EAX
	$+1D5    008A7A09      56                  PUSH ESI
	$+1D6    008A7A0A      8BCF                MOV ECX,EDI
	$+1D8    008A7A0C      C645 FC 01          MOV BYTE PTR SS:[EBP-0x4],0x1
	$+1DC    008A7A10      E8 FB80F1FF         CALL MapleSto.007BFB10
	$+1E1    008A7A15      8B8D 58FFFFFF       MOV ECX,DWORD PTR SS:[EBP-0xA8]
	$+1E7    008A7A1B      8D41 20             LEA EAX,DWORD PTR DS:[ECX+0x20]
	$+1EA    008A7A1E      8D95 54FFFFFF       LEA EDX,DWORD PTR SS:[EBP-0xAC]
	$+1F0    008A7A24      52                  PUSH EDX
	$+1F1    008A7A25      83C1 54             ADD ECX,0x54
	$+1F4    008A7A28      50                  PUSH EAX
	$+1F5    008A7A29      89BD 54FFFFFF       MOV DWORD PTR SS:[EBP-0xAC],EDI
	$+1FB    008A7A2F      8985 58FFFFFF       MOV DWORD PTR SS:[EBP-0xA8],EAX
	$+201    008A7A35      898D 5CFFFFFF       MOV DWORD PTR SS:[EBP-0xA4],ECX
	$+207    008A7A3B      E8 40710000         CALL MapleSto.008AEB80
	$+20C    008A7A40      8BC8                MOV ECX,EAX
	$+20E    008A7A42      83C1 48             ADD ECX,0x48
	$+211    008A7A45      E8 A62E2F00         CALL MapleSto.00B9A8F0
	$+216    008A7A4A      8BCF                MOV ECX,EDI
	$+218    008A7A4C      E8 1FD50400         CALL MapleSto.008F4F70
	$+21D    008A7A51      8B03                MOV EAX,DWORD PTR DS:[EBX]
	$+21F    008A7A53      8B4B 04             MOV ECX,DWORD PTR DS:[EBX+0x4]
	$+222    008A7A56      8D95 44FFFFFF       LEA EDX,DWORD PTR SS:[EBP-0xBC]
	$+228    008A7A5C      8985 44FFFFFF       MOV DWORD PTR SS:[EBP-0xBC],EAX
	$+22E    008A7A62      52                  PUSH EDX
	$+22F    008A7A63      898D 48FFFFFF       MOV DWORD PTR SS:[EBP-0xB8],ECX
	$+235    008A7A69      8B8D 58FFFFFF       MOV ECX,DWORD PTR SS:[EBP-0xA8]
	$+23B    008A7A6F      8D85 38FFFFFF       LEA EAX,DWORD PTR SS:[EBP-0xC8]
	$+241    008A7A75      50                  PUSH EAX
	$+242    008A7A76      51                  PUSH ECX
	$+243    008A7A77      8B8D 5CFFFFFF       MOV ECX,DWORD PTR SS:[EBP-0xA4]
	$+249    008A7A7D      89BD 4CFFFFFF       MOV DWORD PTR SS:[EBP-0xB4],EDI
	$+24F    008A7A83      E8 F8700000         CALL MapleSto.008AEB80
	$+254    008A7A88      8BC8                MOV ECX,EAX
	$+256    008A7A8A      83C1 64             ADD ECX,0x64
	$+259    008A7A8D      E8 3EE2FFFF         CALL MapleSto.008A5CD0
	$+25E    008A7A92      E8 098F0000         CALL MapleSto.008B09A0
	$+263    008A7A97      8D95 61FFFFFF       LEA EDX,DWORD PTR SS:[EBP-0x9F]
	$+269    008A7A9D      52                  PUSH EDX
	$+26A    008A7A9E      68 79B1B6A0         PUSH 0xA0B6B179
	$+26F    008A7AA3      8BCE                MOV ECX,ESI
	$+271    008A7AA5      8985 5CFFFFFF       MOV DWORD PTR SS:[EBP-0xA4],EAX
	$+277    008A7AAB      C685 61FFFFFF 00    MOV BYTE PTR SS:[EBP-0x9F],0x0
	$+27E    008A7AB2      E8 C9E3D0FF         CALL MapleSto.005B5E80
	$+283    008A7AB7      8D85 63FFFFFF       LEA EAX,DWORD PTR SS:[EBP-0x9D]
	$+289    008A7ABD      50                  PUSH EAX
	$+28A    008A7ABE      68 F4D97F2A         PUSH 0x2A7FD9F4
	$+28F    008A7AC3      8BCE                MOV ECX,ESI
	$+291    008A7AC5      C685 63FFFFFF 01    MOV BYTE PTR SS:[EBP-0x9D],0x1
	$+298    008A7ACC      E8 AFE3D0FF         CALL MapleSto.005B5E80
	$+29D    008A7AD1      8B0D 2C417201       MOV ECX,DWORD PTR DS:[0x172412C]
	$+2A3    008A7AD7      53                  PUSH EBX
	$+2A4    008A7AD8      E8 F3CA2D00         CALL MapleSto.00B845D0
	$+2A9    008A7ADD      84C0                TEST AL,AL
	$+2AB    008A7ADF      75 27               JNZ SHORT MapleSto.008A7B08
	$+2AD    008A7AE1      56                  PUSH ESI
	$+2AE    008A7AE2      E8 E9EB2B00         CALL MapleSto.00B666D0
	$+2B3    008A7AE7      8B8D 5CFFFFFF       MOV ECX,DWORD PTR SS:[EBP-0xA4]
	$+2B9    008A7AED      8B11                MOV EDX,DWORD PTR DS:[ECX]
	$+2BB    008A7AEF      83C4 04             ADD ESP,0x4
	$+2BE    008A7AF2      8BD8                MOV EBX,EAX
	$+2C0    008A7AF4      8B42 50             MOV EAX,DWORD PTR DS:[EDX+0x50]
	$+2C3    008A7AF7      53                  PUSH EBX
	$+2C4    008A7AF8      56                  PUSH ESI
	$+2C5    008A7AF9      FFD0                CALL EAX
	$+2C7    008A7AFB      8B0D 9CE47201       MOV ECX,DWORD PTR DS:[0x172E49C]
	$+2CD    008A7B01      53                  PUSH EBX
	$+2CE    008A7B02      56                  PUSH ESI
	$+2CF    008A7B03      E8 08910600         CALL MapleSto.00910C10
	$+2D4    008A7B08      8D4D CC             LEA ECX,DWORD PTR SS:[EBP-0x34]
	$+2D7    008A7B0B      C645 FC 00          MOV BYTE PTR SS:[EBP-0x4],0x0
	$+2DB    008A7B0F      E8 6C61DDFF         CALL MapleSto.0067DC80
	$+2E0    008A7B14      8D8D 64FFFFFF       LEA ECX,DWORD PTR SS:[EBP-0x9C]
	$+2E6    008A7B1A      C745 FC FFFFFFFF    MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+2ED    008A7B21      E8 EAE7FFFF         CALL MapleSto.008A6310
	$+2F2    008A7B26      8BC7                MOV EAX,EDI
	$+2F4    008A7B28      8B4D F4             MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+2F7    008A7B2B      64:890D 00000000    MOV DWORD PTR FS:[0],ECX
	$+2FE    008A7B32      59                  POP ECX
	$+2FF    008A7B33      5F                  POP EDI
	$+300    008A7B34      5E                  POP ESI
	$+301    008A7B35      5B                  POP EBX
	$+302    008A7B36      8B4D F0             MOV ECX,DWORD PTR SS:[EBP-0x10]
	$+305    008A7B39      33CD                XOR ECX,EBP
	$+307    008A7B3B      E8 0C9C9500         CALL MapleSto.0120174C
	$+30C    008A7B40      8BE5                MOV ESP,EBP
	$+30E    008A7B42      5D                  POP EBP
	$+30F    008A7B43      C2 0C00             RETN 0xC
	$+312    008A7B46      CC                  INT3
	*/
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("518B0D????????57E8????????8B138B43048D4DD851",
		1 + 2);
	if (!op_dw_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *op_dw_tmp;
	CheckExeAddr(dw_tmp);
	ofs << "pt_cdword	g_a_light_service_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	/*
	$-48B    0097C34F      CC                     INT3
	$-48A    0097C350      55                     PUSH EBP
	$-489    0097C351      8BEC                   MOV EBP,ESP
	$-487    0097C353      6A FF                  PUSH -0x1
	$-485    0097C355      68 78963601            PUSH MapleSto.01369678
	$-480    0097C35A      64:A1 00000000         MOV EAX,DWORD PTR FS:[0]
	$-47A    0097C360      50                     PUSH EAX
	$-479    0097C361      83EC 08                SUB ESP,0x8
	$-476    0097C364      56                     PUSH ESI
	$-475    0097C365      A1 D0687101            MOV EAX,DWORD PTR DS:[0x17168D0]
	$-470    0097C36A      33C5                   XOR EAX,EBP
	$-46E    0097C36C      50                     PUSH EAX
	$-46D    0097C36D      8D45 F4                LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-46A    0097C370      64:A3 00000000         MOV DWORD PTR FS:[0],EAX
	$-464    0097C376      8BF1                   MOV ESI,ECX
	$-462    0097C378      8B06                   MOV EAX,DWORD PTR DS:[ESI]
	$-460    0097C37A      8B90 64010000          MOV EDX,DWORD PTR DS:[EAX+0x164]
	$-45A    0097C380      6A 00                  PUSH 0x0
	$-458    0097C382      C745 FC 00000000       MOV DWORD PTR SS:[EBP-0x4],0x0
	$-451    0097C389      FFD2                   CALL EDX
	$-44F    0097C38B      8B0D B4407201          MOV ECX,DWORD PTR DS:[0x17240B4]
	$-449    0097C391      8D45 08                LEA EAX,DWORD PTR SS:[EBP+0x8]
	$-446    0097C394      50                     PUSH EAX
	$-445    0097C395      E8 B6570300            CALL MapleSto.009B1B50
	$-440    0097C39A      8B16                   MOV EDX,DWORD PTR DS:[ESI]
	$-43E    0097C39C      8B92 44010000          MOV EDX,DWORD PTR DS:[EDX+0x144]
	$-438    0097C3A2      8D45 EC                LEA EAX,DWORD PTR SS:[EBP-0x14]
	$-435    0097C3A5      50                     PUSH EAX
	$-434    0097C3A6      8BCE                   MOV ECX,ESI
	$-432    0097C3A8      FFD2                   CALL EDX
	$-430    0097C3AA      8B4D F0                MOV ECX,DWORD PTR SS:[EBP-0x10]
	$-42D    0097C3AD      C645 FC 01             MOV BYTE PTR SS:[EBP-0x4],0x1
	$-429    0097C3B1      85C9                   TEST ECX,ECX
	$-427    0097C3B3      74 16                  JE SHORT MapleSto.0097C3CB
	$-425    0097C3B5      8D45 08                LEA EAX,DWORD PTR SS:[EBP+0x8]
	$-422    0097C3B8      50                     PUSH EAX
	$-421    0097C3B9      E8 02171000            CALL MapleSto.00A7DAC0
	$-41C    0097C3BE      8B4D F0                MOV ECX,DWORD PTR SS:[EBP-0x10]
	$-419    0097C3C1      8B11                   MOV EDX,DWORD PTR DS:[ECX]
	$-417    0097C3C3      8B42 04                MOV EAX,DWORD PTR DS:[EDX+0x4]
	$-414    0097C3C6      FFD0                   CALL EAX
	$-412    0097C3C8      8B4D F0                MOV ECX,DWORD PTR SS:[EBP-0x10]
	$-40F    0097C3CB      C645 FC 00             MOV BYTE PTR SS:[EBP-0x4],0x0
	$-40B    0097C3CF      85C9                   TEST ECX,ECX
	$-409    0097C3D1      74 2A                  JE SHORT MapleSto.0097C3FD
	$-407    0097C3D3      83C1 04                ADD ECX,0x4
	$-404    0097C3D6      51                     PUSH ECX
	$-403    0097C3D7      FF15 6C933E01          CALL DWORD PTR DS:[0x13E936C]
	$-3FD    0097C3DD      85C0                   TEST EAX,EAX
	$-3FB    0097C3DF      75 1C                  JNZ SHORT MapleSto.0097C3FD
	$-3F9    0097C3E1      8B4D F0                MOV ECX,DWORD PTR SS:[EBP-0x10]
	$-3F6    0097C3E4      83C1 04                ADD ECX,0x4
	$-3F3    0097C3E7      51                     PUSH ECX
	$-3F2    0097C3E8      FF15 F8923E01          CALL DWORD PTR DS:[0x13E92F8]
	$-3EC    0097C3EE      8B4D F0                MOV ECX,DWORD PTR SS:[EBP-0x10]
	$-3E9    0097C3F1      85C9                   TEST ECX,ECX
	$-3E7    0097C3F3      74 08                  JE SHORT MapleSto.0097C3FD
	$-3E5    0097C3F5      8B11                   MOV EDX,DWORD PTR DS:[ECX]
	$-3E3    0097C3F7      8B02                   MOV EAX,DWORD PTR DS:[EDX]
	$-3E1    0097C3F9      6A 01                  PUSH 0x1
	$-3DF    0097C3FB      FFD0                   CALL EAX
	$-3DD    0097C3FD      8D4D 08                LEA ECX,DWORD PTR SS:[EBP+0x8]
	$-3DA    0097C400      FF15 DCA13E01          CALL DWORD PTR DS:[0x13EA1DC]                    ; mfc90.#601
	$-3D4    0097C406      8B4D F4                MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-3D1    0097C409      64:890D 00000000       MOV DWORD PTR FS:[0],ECX
	$-3CA    0097C410      59                     POP ECX
	$-3C9    0097C411      5E                     POP ESI
	$-3C8    0097C412      8BE5                   MOV ESP,EBP
	$-3C6    0097C414      5D                     POP EBP
	$-3C5    0097C415      C2 0400                RETN 0x4
	$-3C2    0097C418      CC                     INT3
	$-3C1    0097C419      CC                     INT3
	$-3C0    0097C41A      CC                     INT3
	$-3BF    0097C41B      CC                     INT3
	$-3BE    0097C41C      CC                     INT3
	$-3BD    0097C41D      CC                     INT3
	$-3BC    0097C41E      CC                     INT3
	$-3BB    0097C41F      CC                     INT3
	$-3BA    0097C420      55                     PUSH EBP
	$-3B9    0097C421      8BEC                   MOV EBP,ESP
	$-3B7    0097C423      6A FF                  PUSH -0x1
	$-3B5    0097C425      68 9B963601            PUSH MapleSto.0136969B
	$-3B0    0097C42A      64:A1 00000000         MOV EAX,DWORD PTR FS:[0]
	$-3AA    0097C430      50                     PUSH EAX
	$-3A9    0097C431      83EC 08                SUB ESP,0x8
	$-3A6    0097C434      A1 D0687101            MOV EAX,DWORD PTR DS:[0x17168D0]
	$-3A1    0097C439      33C5                   XOR EAX,EBP
	$-39F    0097C43B      50                     PUSH EAX
	$-39E    0097C43C      8D45 F4                LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-39B    0097C43F      64:A3 00000000         MOV DWORD PTR FS:[0],EAX
	$-395    0097C445      8B01                   MOV EAX,DWORD PTR DS:[ECX]
	$-393    0097C447      8B80 34010000          MOV EAX,DWORD PTR DS:[EAX+0x134]
	$-38D    0097C44D      8D55 EC                LEA EDX,DWORD PTR SS:[EBP-0x14]
	$-38A    0097C450      52                     PUSH EDX
	$-389    0097C451      FFD0                   CALL EAX
	$-387    0097C453      8B4D F0                MOV ECX,DWORD PTR SS:[EBP-0x10]
	$-384    0097C456      C745 FC 00000000       MOV DWORD PTR SS:[EBP-0x4],0x0
	$-37D    0097C45D      85C9                   TEST ECX,ECX
	$-37B    0097C45F      74 18                  JE SHORT MapleSto.0097C479
	$-379    0097C461      8B55 14                MOV EDX,DWORD PTR SS:[EBP+0x14]
	$-376    0097C464      8B45 10                MOV EAX,DWORD PTR SS:[EBP+0x10]
	$-373    0097C467      52                     PUSH EDX
	$-372    0097C468      8B55 0C                MOV EDX,DWORD PTR SS:[EBP+0xC]
	$-36F    0097C46B      50                     PUSH EAX
	$-36E    0097C46C      8B45 08                MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-36B    0097C46F      52                     PUSH EDX
	$-36A    0097C470      50                     PUSH EAX
	$-369    0097C471      E8 5A501300            CALL MapleSto.00AB14D0
	$-364    0097C476      8B4D F0                MOV ECX,DWORD PTR SS:[EBP-0x10]
	$-361    0097C479      C745 FC FFFFFFFF       MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-35A    0097C480      85C9                   TEST ECX,ECX
	$-358    0097C482      74 2A                  JE SHORT MapleSto.0097C4AE
	$-356    0097C484      83C1 04                ADD ECX,0x4
	$-353    0097C487      51                     PUSH ECX
	$-352    0097C488      FF15 6C933E01          CALL DWORD PTR DS:[0x13E936C]
	$-34C    0097C48E      85C0                   TEST EAX,EAX
	$-34A    0097C490      75 1C                  JNZ SHORT MapleSto.0097C4AE
	$-348    0097C492      8B4D F0                MOV ECX,DWORD PTR SS:[EBP-0x10]
	$-345    0097C495      83C1 04                ADD ECX,0x4
	$-342    0097C498      51                     PUSH ECX
	$-341    0097C499      FF15 F8923E01          CALL DWORD PTR DS:[0x13E92F8]
	$-33B    0097C49F      8B4D F0                MOV ECX,DWORD PTR SS:[EBP-0x10]
	$-338    0097C4A2      85C9                   TEST ECX,ECX
	$-336    0097C4A4      74 08                  JE SHORT MapleSto.0097C4AE
	$-334    0097C4A6      8B11                   MOV EDX,DWORD PTR DS:[ECX]
	$-332    0097C4A8      8B02                   MOV EAX,DWORD PTR DS:[EDX]
	$-330    0097C4AA      6A 01                  PUSH 0x1
	$-32E    0097C4AC      FFD0                   CALL EAX
	$-32C    0097C4AE      8B4D F4                MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-329    0097C4B1      64:890D 00000000       MOV DWORD PTR FS:[0],ECX
	$-322    0097C4B8      59                     POP ECX
	$-321    0097C4B9      8BE5                   MOV ESP,EBP
	$-31F    0097C4BB      5D                     POP EBP
	$-31E    0097C4BC      C2 1000                RETN 0x10
	$-31B    0097C4BF      CC                     INT3
	$-31A    0097C4C0      55                     PUSH EBP
	$-319    0097C4C1      8BEC                   MOV EBP,ESP
	$-317    0097C4C3      6A FF                  PUSH -0x1
	$-315    0097C4C5      68 C6963601            PUSH MapleSto.013696C6
	$-310    0097C4CA      64:A1 00000000         MOV EAX,DWORD PTR FS:[0]
	$-30A    0097C4D0      50                     PUSH EAX
	$-309    0097C4D1      83EC 70                SUB ESP,0x70
	$-306    0097C4D4      53                     PUSH EBX
	$-305    0097C4D5      56                     PUSH ESI
	$-304    0097C4D6      57                     PUSH EDI
	$-303    0097C4D7      A1 D0687101            MOV EAX,DWORD PTR DS:[0x17168D0]
	$-2FE    0097C4DC      33C5                   XOR EAX,EBP
	$-2FC    0097C4DE      50                     PUSH EAX
	$-2FB    0097C4DF      8D45 F4                LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-2F8    0097C4E2      64:A3 00000000         MOV DWORD PTR FS:[0],EAX
	$-2F2    0097C4E8      8BF1                   MOV ESI,ECX
	$-2F0    0097C4EA      F3:0F1045 08           MOVSS XMM0,DWORD PTR SS:[EBP+0x8]
	$-2EB    0097C4EF      F3:0F1186 30110000     MOVSS DWORD PTR DS:[ESI+0x1130],XMM0
	$-2E3    0097C4F7      F3:0F1045 0C           MOVSS XMM0,DWORD PTR SS:[EBP+0xC]
	$-2DE    0097C4FC      F3:0F1186 34110000     MOVSS DWORD PTR DS:[ESI+0x1134],XMM0
	$-2D6    0097C504      F3:0F1045 10           MOVSS XMM0,DWORD PTR SS:[EBP+0x10]
	$-2D1    0097C509      33DB                   XOR EBX,EBX
	$-2CF    0097C50B      F3:0F1186 38110000     MOVSS DWORD PTR DS:[ESI+0x1138],XMM0
	$-2C7    0097C513      899E F0110000          MOV DWORD PTR DS:[ESI+0x11F0],EBX
	$-2C1    0097C519      399E E8000000          CMP DWORD PTR DS:[ESI+0xE8],EBX
	$-2BB    0097C51F      0F84 FD080000          JE MapleSto.0097CE22
	$-2B5    0097C525      8D8E 08060000          LEA ECX,DWORD PTR DS:[ESI+0x608]
	$-2AF    0097C52B      E8 6579C3FF            CALL MapleSto.005B3E95
	$-2AA    0097C530      DD45 14                FLD QWORD PTR SS:[EBP+0x14]
	$-2A7    0097C533      0945 20                OR DWORD PTR SS:[EBP+0x20],EAX
	$-2A4    0097C536      D955 18                FST DWORD PTR SS:[EBP+0x18]
	$-2A1    0097C539      8D45 F0                LEA EAX,DWORD PTR SS:[EBP-0x10]
	$-29E    0097C53C      50                     PUSH EAX
	$-29D    0097C53D      51                     PUSH ECX
	$-29C    0097C53E      D91C24                 FSTP DWORD PTR SS:[ESP]
	$-299    0097C541      56                     PUSH ESI
	$-298    0097C542      8D4D A8                LEA ECX,DWORD PTR SS:[EBP-0x58]
	$-295    0097C545      51                     PUSH ECX
	$-294    0097C546      8B0D 2C417201          MOV ECX,DWORD PTR DS:[0x172412C]
	$-28E    0097C54C      895D F0                MOV DWORD PTR SS:[EBP-0x10],EBX
	$-28B    0097C54F      E8 FC522000            CALL MapleSto.00B81850
	$-286    0097C554      F3:0F101D 94413F01     MOVSS XMM3,DWORD PTR DS:[0x13F4194]
	$-27E    0097C55C      F3:0F1045 A8           MOVSS XMM0,DWORD PTR SS:[EBP-0x58]
	$-279    0097C561      F3:0F104D AC           MOVSS XMM1,DWORD PTR SS:[EBP-0x54]
	$-274    0097C566      F3:0F1055 B0           MOVSS XMM2,DWORD PTR SS:[EBP-0x50]
	$-26F    0097C56B      F3:0F59C3              MULSS XMM0,XMM3
	$-26B    0097C56F      F3:0F59CB              MULSS XMM1,XMM3
	$-267    0097C573      F3:0F59D3              MULSS XMM2,XMM3
	$-263    0097C577      F3:0F1145 A8           MOVSS DWORD PTR SS:[EBP-0x58],XMM0
	$-25E    0097C57C      F3:0F114D AC           MOVSS DWORD PTR SS:[EBP-0x54],XMM1
	$-259    0097C581      F3:0F1155 B0           MOVSS DWORD PTR SS:[EBP-0x50],XMM2
	$-254    0097C586      F3:0F1186 6C110000     MOVSS DWORD PTR DS:[ESI+0x116C],XMM0
	$-24C    0097C58E      F3:0F118E 70110000     MOVSS DWORD PTR DS:[ESI+0x1170],XMM1
	$-244    0097C596      F3:0F1196 74110000     MOVSS DWORD PTR DS:[ESI+0x1174],XMM2
	$-23C    0097C59E      8D7B 01                LEA EDI,DWORD PTR DS:[EBX+0x1]
	$-239    0097C5A1      395D F0                CMP DWORD PTR SS:[EBP-0x10],EBX
	$-236    0097C5A4      75 03                  JNZ SHORT MapleSto.0097C5A9
	$-234    0097C5A6      8B7D 24                MOV EDI,DWORD PTR SS:[EBP+0x24]
	$-231    0097C5A9      8B8E E8000000          MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$-22B    0097C5AF      E8 DCF73E01            CALL MapleSto.01D6BD90
	$-226    0097C5B4      85C0                   TEST EAX,EAX
	$-224    0097C5B6      74 68                  JE SHORT MapleSto.0097C620
	$-222    0097C5B8      8B8E E8000000          MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$-21C    0097C5BE      E8 9DF1E6FF            CALL MapleSto.007EB760
	$-217    0097C5C3      85C0                   TEST EAX,EAX
	$-215    0097C5C5      75 59                  JNZ SHORT MapleSto.0097C620
	$-213    0097C5C7      8B8E E8000000          MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$-20D    0097C5CD      E8 AE963F01            CALL MapleSto.01D75C80
	$-208    0097C5D2      85C0                   TEST EAX,EAX
	$-206    0097C5D4      75 4A                  JNZ SHORT MapleSto.0097C620
	$-204    0097C5D6      8B0D 2C417201          MOV ECX,DWORD PTR DS:[0x172412C]
	$-1FE    0097C5DC      E8 0FE51F00            CALL MapleSto.00B7AAF0
	$-1F9    0097C5E1      85C0                   TEST EAX,EAX
	$-1F7    0097C5E3      75 3B                  JNZ SHORT MapleSto.0097C620
	$-1F5    0097C5E5      8B8E E8000000          MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$-1EF    0097C5EB      68 083A7301            PUSH MapleSto.01733A08
	$-1EA    0097C5F0      8D55 D0                LEA EDX,DWORD PTR SS:[EBP-0x30]
	$-1E7    0097C5F3      52                     PUSH EDX
	$-1E6    0097C5F4      E8 D7F1E6FF            CALL MapleSto.007EB7D0
	$-1E1    0097C5F9      8BC8                   MOV ECX,EAX
	$-1DF    0097C5FB      E8 00A7CBFF            CALL MapleSto.00636D00
	$-1DA    0097C600      8BC8                   MOV ECX,EAX
	$-1D8    0097C602      E8 7911C3FF            CALL MapleSto.005AD780
	$-1D3    0097C607      84C0                   TEST AL,AL
	$-1D1    0097C609      74 15                  JE SHORT MapleSto.0097C620
	$-1CF    0097C60B      68 34267401            PUSH MapleSto.01742634
	$-1CA    0097C610      8D4D 08                LEA ECX,DWORD PTR SS:[EBP+0x8]
	$-1C7    0097C613      E8 C8E7DDFF            CALL MapleSto.0075ADE0
	$-1C2    0097C618      84C0                   TEST AL,AL
	$-1C0    0097C61A      0F85 0B010000          JNZ MapleSto.0097C72B
	$-1BA    0097C620      3BFB                   CMP EDI,EBX
	$-1B8    0097C622      0F85 03010000          JNZ MapleSto.0097C72B
	$-1B2    0097C628      8B06                   MOV EAX,DWORD PTR DS:[ESI]
	$-1B0    0097C62A      8B90 74010000          MOV EDX,DWORD PTR DS:[EAX+0x174]
	$-1AA    0097C630      53                     PUSH EBX
	$-1A9    0097C631      8BCE                   MOV ECX,ESI
	$-1A7    0097C633      FFD2                   CALL EDX
	$-1A5    0097C635      85C0                   TEST EAX,EAX
	$-1A3    0097C637      74 3D                  JE SHORT MapleSto.0097C676
	$-1A1    0097C639      F3:0F1045 08           MOVSS XMM0,DWORD PTR SS:[EBP+0x8]
	$-19C    0097C63E      0F2E05 34267401        UCOMISS XMM0,DWORD PTR DS:[0x1742634]
	$-195    0097C645      9F                     LAHF
	$-194    0097C646      F6C4 44                TEST AH,0x44
	$-191    0097C649      7A 2B                  JPE SHORT MapleSto.0097C676
	$-18F    0097C64B      F3:0F1045 0C           MOVSS XMM0,DWORD PTR SS:[EBP+0xC]
	$-18A    0097C650      0F2E05 38267401        UCOMISS XMM0,DWORD PTR DS:[0x1742638]
	$-183    0097C657      9F                     LAHF
	$-182    0097C658      F6C4 44                TEST AH,0x44
	$-17F    0097C65B      7A 19                  JPE SHORT MapleSto.0097C676
	$-17D    0097C65D      F3:0F1045 10           MOVSS XMM0,DWORD PTR SS:[EBP+0x10]
	$-178    0097C662      0F2E05 3C267401        UCOMISS XMM0,DWORD PTR DS:[0x174263C]
	$-171    0097C669      9F                     LAHF
	$-170    0097C66A      F6C4 44                TEST AH,0x44
	$-16D    0097C66D      7A 07                  JPE SHORT MapleSto.0097C676
	$-16B    0097C66F      B8 01000000            MOV EAX,0x1
	$-166    0097C674      EB 02                  JMP SHORT MapleSto.0097C678
	$-164    0097C676      33C0                   XOR EAX,EAX
	$-162    0097C678      D945 18                FLD DWORD PTR SS:[EBP+0x18]
	$-15F    0097C67B      50                     PUSH EAX
	$-15E    0097C67C      51                     PUSH ECX
	$-15D    0097C67D      8B8E E8000000          MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$-157    0097C683      D91C24                 FSTP DWORD PTR SS:[ESP]
	$-154    0097C686      8D45 CC                LEA EAX,DWORD PTR SS:[EBP-0x34]
	$-151    0097C689      50                     PUSH EAX
	$-150    0097C68A      E8 51FCE6FF            CALL MapleSto.007EC2E0
	$-14B    0097C68F      8B8E E4000000          MOV ECX,DWORD PTR DS:[ESI+0xE4]
	$-145    0097C695      E8 56720800            CALL MapleSto.00A038F0
	$-140    0097C69A      83F8 02                CMP EAX,0x2
	$-13D    0097C69D      74 26                  JE SHORT MapleSto.0097C6C5
	$-13B    0097C69F      83F8 05                CMP EAX,0x5
	$-138    0097C6A2      74 21                  JE SHORT MapleSto.0097C6C5
	$-136    0097C6A4      83F8 06                CMP EAX,0x6
	$-133    0097C6A7      74 1C                  JE SHORT MapleSto.0097C6C5
	$-131    0097C6A9      83F8 10                CMP EAX,0x10
	$-12E    0097C6AC      75 0F                  JNZ SHORT MapleSto.0097C6BD
	$-12C    0097C6AE      8B8E E8000000          MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$-126    0097C6B4      E8 C7F0E6FF            CALL MapleSto.007EB780
	$-121    0097C6B9      85C0                   TEST EAX,EAX
	$-11F    0097C6BB      75 08                  JNZ SHORT MapleSto.0097C6C5
	$-11D    0097C6BD      0F57DB                 XORPS XMM3,XMM3
	$-11A    0097C6C0      0F28E3                 MOVAPS XMM4,XMM3
	$-117    0097C6C3      EB 0A                  JMP SHORT MapleSto.0097C6CF
	$-115    0097C6C5      F3:0F105D D0           MOVSS XMM3,DWORD PTR SS:[EBP-0x30]
	$-110    0097C6CA      F3:0F1065 CC           MOVSS XMM4,DWORD PTR SS:[EBP-0x34]
	$-10B    0097C6CF      F3:0F106D D4           MOVSS XMM5,DWORD PTR SS:[EBP-0x2C]
	$-106    0097C6D4      F3:0F1045 08           MOVSS XMM0,DWORD PTR SS:[EBP+0x8]
	$-101    0097C6D9      F3:0F104D 0C           MOVSS XMM1,DWORD PTR SS:[EBP+0xC]
	$-FC     0097C6DE      F3:0F58C4              ADDSS XMM0,XMM4
	$-F8     0097C6E2      F3:0F58CB              ADDSS XMM1,XMM3
	$-F4     0097C6E6      0F28D5                 MOVAPS XMM2,XMM5
	$-F1     0097C6E9      F3:0F5855 10           ADDSS XMM2,DWORD PTR SS:[EBP+0x10]
	$-EC     0097C6EE      F3:0F1186 3C110000     MOVSS DWORD PTR DS:[ESI+0x113C],XMM0
	$-E4     0097C6F6      F3:0F118E 40110000     MOVSS DWORD PTR DS:[ESI+0x1140],XMM1
	$-DC     0097C6FE      F3:0F1196 44110000     MOVSS DWORD PTR DS:[ESI+0x1144],XMM2
	$-D4     0097C706      F3:0F1145 08           MOVSS DWORD PTR SS:[EBP+0x8],XMM0
	$-CF     0097C70B      0F28F9                 MOVAPS XMM7,XMM1
	$-CC     0097C70E      0F28F2                 MOVAPS XMM6,XMM2
	$-C9     0097C711      F3:0F11A6 78110000     MOVSS DWORD PTR DS:[ESI+0x1178],XMM4
	$-C1     0097C719      F3:0F119E 7C110000     MOVSS DWORD PTR DS:[ESI+0x117C],XMM3
	$-B9     0097C721      F3:0F11AE 80110000     MOVSS DWORD PTR DS:[ESI+0x1180],XMM5
	$-B1     0097C729      EB 0A                  JMP SHORT MapleSto.0097C735
	$-AF     0097C72B      F3:0F1075 10           MOVSS XMM6,DWORD PTR SS:[EBP+0x10]
	$-AA     0097C730      F3:0F107D 0C           MOVSS XMM7,DWORD PTR SS:[EBP+0xC]
	$-A5     0097C735      F3:0F1045 A8           MOVSS XMM0,DWORD PTR SS:[EBP-0x58]
	$-A0     0097C73A      F3:0F5845 08           ADDSS XMM0,DWORD PTR SS:[EBP+0x8]
	$-9B     0097C73F      F3:0F104D AC           MOVSS XMM1,DWORD PTR SS:[EBP-0x54]
	$-96     0097C744      F3:0F1055 B0           MOVSS XMM2,DWORD PTR SS:[EBP-0x50]
	$-91     0097C749      F3:0F58CF              ADDSS XMM1,XMM7
	$-8D     0097C74D      F3:0F58D6              ADDSS XMM2,XMM6
	$-89     0097C751      F3:0F1186 48110000     MOVSS DWORD PTR DS:[ESI+0x1148],XMM0
	$-81     0097C759      F3:0F118E 4C110000     MOVSS DWORD PTR DS:[ESI+0x114C],XMM1
	$-79     0097C761      F3:0F1196 50110000     MOVSS DWORD PTR DS:[ESI+0x1150],XMM2
	$-71     0097C769      8B8E B0100000          MOV ECX,DWORD PTR DS:[ESI+0x10B0]
	$-6B     0097C76F      8D55 D8                LEA EDX,DWORD PTR SS:[EBP-0x28]
	$-68     0097C772      F3:0F1145 08           MOVSS DWORD PTR SS:[EBP+0x8],XMM0
	$-63     0097C777      F30F7E86 A8100000      MOVQ XMM0,QWORD PTR DS:[ESI+0x10A8]
	$-5B     0097C77F      894D BC                MOV DWORD PTR SS:[EBP-0x44],ECX
	$-58     0097C782      8B8E E8000000          MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$-52     0097C788      52                     PUSH EDX
	$-51     0097C789      F3:0F114D 0C           MOVSS DWORD PTR SS:[EBP+0xC],XMM1
	$-4C     0097C78E      F3:0F1155 10           MOVSS DWORD PTR SS:[EBP+0x10],XMM2
	$-47     0097C793      660FD645 B4            MOVQ QWORD PTR SS:[EBP-0x4C],XMM0
	$-42     0097C798      E8 33F0E6FF            CALL MapleSto.007EB7D0
	$-3D     0097C79D      8BC8                   MOV ECX,EAX
	$-3B     0097C79F      E8 5CA5CBFF            CALL MapleSto.00636D00
	$-36     0097C7A4      8B45 D8                MOV EAX,DWORD PTR SS:[EBP-0x28]
	$-33     0097C7A7      3B05 083A7301          CMP EAX,DWORD PTR DS:[0x1733A08]
	$-2D     0097C7AD      75 0F                  JNZ SHORT MapleSto.0097C7BE
	$-2B     0097C7AF      8B4D DC                MOV ECX,DWORD PTR SS:[EBP-0x24]
	$-28     0097C7B2      3B0D 0C3A7301          CMP ECX,DWORD PTR DS:[0x1733A0C]
	$-22     0097C7B8      0F84 E7010000          JE MapleSto.0097C9A5
	$-1C     0097C7BE      8B8E E8000000          MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$-16     0097C7C4      E8 07F0E6FF            CALL MapleSto.007EB7D0
	$-11     0097C7C9      8BC8                   MOV ECX,EAX
	$-F      0097C7CB      E8 50A5CBFF            CALL MapleSto.00636D20
	$-A      0097C7D0      8BF8                   MOV EDI,EAX
	$-8      0097C7D2      3BFB                   CMP EDI,EBX
	$-6      0097C7D4      0F84 CB010000          JE MapleSto.0097C9A5
	$ ==>    0097C7DA      68 0F0000F0            PUSH 0xF000000F
	$+5      0097C7DF      8D55 B4                LEA EDX,DWORD PTR SS:[EBP-0x4C]
	$+8      0097C7E2      52                     PUSH EDX
	$+9      0097C7E3      68 52CA95FD            PUSH 0xFD95CA52
	$+E      0097C7E8      8BCF                   MOV ECX,EDI
	$+10     0097C7EA      E8 11D64A00            CALL MapleSto.00E29E00                           ; g_c_get_square_pos_
	$+15     0097C7EF      F30F7E45 B4            MOVQ XMM0,QWORD PTR SS:[EBP-0x4C]
	$+1A     0097C7F4      8B4D BC                MOV ECX,DWORD PTR SS:[EBP-0x44]
	$+1D     0097C7F7      83EC 0C                SUB ESP,0xC
	$+20     0097C7FA      8BC4                   MOV EAX,ESP
	$+22     0097C7FC      660FD600               MOVQ QWORD PTR DS:[EAX],XMM0
	$+26     0097C800      8948 08                MOV DWORD PTR DS:[EAX+0x8],ECX
	$+29     0097C803      E8 A875C0FF            CALL MapleSto.00583DB0
	$+2E     0097C808      83C4 0C                ADD ESP,0xC
	$+31     0097C80B      57                     PUSH EDI
	$+32     0097C80C      8D55 E4                LEA EDX,DWORD PTR SS:[EBP-0x1C]
	$+35     0097C80F      8986 B4100000          MOV DWORD PTR DS:[ESI+0x10B4],EAX
	$+3B     0097C815      8B0D 2C417201          MOV ECX,DWORD PTR DS:[0x172412C]
	$+41     0097C81B      52                     PUSH EDX
	$+42     0097C81C      E8 7F2B2000            CALL MapleSto.00B7F3A0
	$+47     0097C821      8B4D E8                MOV ECX,DWORD PTR SS:[EBP-0x18]
	$+4A     0097C824      895D FC                MOV DWORD PTR SS:[EBP-0x4],EBX
	$+4D     0097C827      3BCB                   CMP ECX,EBX
	$+4F     0097C829      74 21                  JE SHORT MapleSto.0097C84C
	$+51     0097C82B      68 34267401            PUSH MapleSto.01742634
	$+56     0097C830      8D45 CC                LEA EAX,DWORD PTR SS:[EBP-0x34]
	$+59     0097C833      50                     PUSH EAX
	$+5A     0097C834      E8 A72CC7FF            CALL MapleSto.005EF4E0
	$+5F     0097C839      8BC8                   MOV ECX,EAX
	$+61     0097C83B      E8 E017C7FF            CALL MapleSto.005EE020
	$+66     0097C840      84C0                   TEST AL,AL
	$+68     0097C842      74 08                  JE SHORT MapleSto.0097C84C
	$+6A     0097C844      8B4D E8                MOV ECX,DWORD PTR SS:[EBP-0x18]
	$+6D     0097C847      E8 D6293401            CALL MapleSto.01CBF222
	$+72     0097C84C      83CB FF                OR EBX,0xFFFFFFFF
	$+75     0097C84F      8D4D E4                LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$+78     0097C852      895D FC                MOV DWORD PTR SS:[EBP-0x4],EBX
	$+7B     0097C855      E8 B6081000            CALL MapleSto.00A7D110
	$+80     0097C85A      8B4D D8                MOV ECX,DWORD PTR SS:[EBP-0x28]
	$+83     0097C85D      3B8E A0100000          CMP ECX,DWORD PTR DS:[ESI+0x10A0]
	$+89     0097C863      0F85 3C010000          JNZ MapleSto.0097C9A5
	$+8F     0097C869      8B55 DC                MOV EDX,DWORD PTR SS:[EBP-0x24]
	$+92     0097C86C      3B96 A4100000          CMP EDX,DWORD PTR DS:[ESI+0x10A4]
	$+98     0097C872      0F85 2D010000          JNZ MapleSto.0097C9A5
	$+9E     0097C878      8B0D 2C417201          MOV ECX,DWORD PTR DS:[0x172412C]
	$+A4     0097C87E      57                     PUSH EDI
	$+A5     0097C87F      8D45 D0                LEA EAX,DWORD PTR SS:[EBP-0x30]
	$+A8     0097C882      50                     PUSH EAX
	$+A9     0097C883      E8 182B2000            CALL MapleSto.00B7F3A0
	$+AE     0097C888      837D D4 00             CMP DWORD PTR SS:[EBP-0x2C],0x0
	$+B2     0097C88C      C745 FC 01000000       MOV DWORD PTR SS:[EBP-0x4],0x1
	$+B9     0097C893      0F84 01010000          JE MapleSto.0097C99A
	$+BF     0097C899      F3:0F1045 BC           MOVSS XMM0,DWORD PTR SS:[EBP-0x44]
	$+C4     0097C89E      F3:0F5C86 B0100000     SUBSS XMM0,DWORD PTR DS:[ESI+0x10B0]
	$+CC     0097C8A6      F3:0F104D B4           MOVSS XMM1,DWORD PTR SS:[EBP-0x4C]
	$+D1     0097C8AB      F3:0F5C8E A8100000     SUBSS XMM1,DWORD PTR DS:[ESI+0x10A8]
	$+D9     0097C8B3      F3:0F1055 B8           MOVSS XMM2,DWORD PTR SS:[EBP-0x48]
	$+DE     0097C8B8      F3:0F5C96 AC100000     SUBSS XMM2,DWORD PTR DS:[ESI+0x10AC]
	$+E6     0097C8C0      0F57DB                 XORPS XMM3,XMM3
	$+E9     0097C8C3      0F2FD8                 COMISS XMM3,XMM0
	$+EC     0097C8C6      F3:0F114D 24           MOVSS DWORD PTR SS:[EBP+0x24],XMM1
	$+F1     0097C8CB      F3:0F1155 EC           MOVSS DWORD PTR SS:[EBP-0x14],XMM2
	$+F6     0097C8D0      F3:0F1145 E8           MOVSS DWORD PTR SS:[EBP-0x18],XMM0
	$+FB     0097C8D5      F3:0F118E 84110000     MOVSS DWORD PTR DS:[ESI+0x1184],XMM1
	$+103    0097C8DD      F3:0F1196 88110000     MOVSS DWORD PTR DS:[ESI+0x1188],XMM2
	$+10B    0097C8E5      F3:0F1186 8C110000     MOVSS DWORD PTR DS:[ESI+0x118C],XMM0
	$+113    0097C8ED      72 50                  JB SHORT MapleSto.0097C93F
	$+115    0097C8EF      F3:0F101D 94413F01     MOVSS XMM3,DWORD PTR DS:[0x13F4194]
	$+11D    0097C8F7      F3:0F59CB              MULSS XMM1,XMM3
	$+121    0097C8FB      F3:0F584D 08           ADDSS XMM1,DWORD PTR SS:[EBP+0x8]
	$+126    0097C900      F3:0F59C3              MULSS XMM0,XMM3
	$+12A    0097C904      F3:0F5845 10           ADDSS XMM0,DWORD PTR SS:[EBP+0x10]
	$+12F    0097C909      F3:0F59D3              MULSS XMM2,XMM3
	$+133    0097C90D      F3:0F105D 0C           MOVSS XMM3,DWORD PTR SS:[EBP+0xC]
	$+138    0097C912      F3:0F58DA              ADDSS XMM3,XMM2
	$+13C    0097C916      F3:0F114D 08           MOVSS DWORD PTR SS:[EBP+0x8],XMM1
	$+141    0097C91B      F3:0F115D 0C           MOVSS DWORD PTR SS:[EBP+0xC],XMM3
	$+146    0097C920      F3:0F1145 10           MOVSS DWORD PTR SS:[EBP+0x10],XMM0
	$+14B    0097C925      F3:0F118E 54110000     MOVSS DWORD PTR DS:[ESI+0x1154],XMM1
	$+153    0097C92D      F3:0F119E 58110000     MOVSS DWORD PTR DS:[ESI+0x1158],XMM3
	$+15B    0097C935      F3:0F1186 5C110000     MOVSS DWORD PTR DS:[ESI+0x115C],XMM0
	$+163    0097C93D      EB 5B                  JMP SHORT MapleSto.0097C99A
	$+165    0097C93F      0F2FC3                 COMISS XMM0,XMM3
	$+168    0097C942      76 56                  JBE SHORT MapleSto.0097C99A
	$+16A    0097C944      8B16                   MOV EDX,DWORD PTR DS:[ESI]
	$+16C    0097C946      8B52 48                MOV EDX,DWORD PTR DS:[EDX+0x48]
	$+16F    0097C949      8D45 90                LEA EAX,DWORD PTR SS:[EBP-0x70]
	$+172    0097C94C      50                     PUSH EAX
	$+173    0097C94D      8BCE                   MOV ECX,ESI
	$+175    0097C94F      FFD2                   CALL EDX
	$+177    0097C951      F3:0F1040 04           MOVSS XMM0,DWORD PTR DS:[EAX+0x4]
	$+17C    0097C956      F3:0F1048 08           MOVSS XMM1,DWORD PTR DS:[EAX+0x8]
	$+181    0097C95B      F3:0F1055 24           MOVSS XMM2,DWORD PTR SS:[EBP+0x24]
	$+186    0097C960      F3:0F5810              ADDSS XMM2,DWORD PTR DS:[EAX]
	$+18A    0097C964      F3:0F5845 EC           ADDSS XMM0,DWORD PTR SS:[EBP-0x14]
	$+18F    0097C969      F3:0F584D E8           ADDSS XMM1,DWORD PTR SS:[EBP-0x18]
	$+194    0097C96E      8B16                   MOV EDX,DWORD PTR DS:[ESI]
	$+196    0097C970      8B52 4C                MOV EDX,DWORD PTR DS:[EDX+0x4C]
	$+199    0097C973      83EC 0C                SUB ESP,0xC
	$+19C    0097C976      8BC4                   MOV EAX,ESP
	$+19E    0097C978      F3:0F1155 E0           MOVSS DWORD PTR SS:[EBP-0x20],XMM2
	$+1A3    0097C97D      F3:0F1145 E4           MOVSS DWORD PTR SS:[EBP-0x1C],XMM0
	$+1A8    0097C982      F30F7E45 E0            MOVQ XMM0,QWORD PTR SS:[EBP-0x20]
	$+1AD    0097C987      F3:0F114D E8           MOVSS DWORD PTR SS:[EBP-0x18],XMM1
	$+1B2    0097C98C      8B4D E8                MOV ECX,DWORD PTR SS:[EBP-0x18]
	$+1B5    0097C98F      660FD600               MOVQ QWORD PTR DS:[EAX],XMM0
	$+1B9    0097C993      8948 08                MOV DWORD PTR DS:[EAX+0x8],ECX
	$+1BC    0097C996      8BCE                   MOV ECX,ESI
	$+1BE    0097C998      FFD2                   CALL EDX
	$+1C0    0097C99A      8D4D D0                LEA ECX,DWORD PTR SS:[EBP-0x30]
	$+1C3    0097C99D      895D FC                MOV DWORD PTR SS:[EBP-0x4],EBX
	$+1C6    0097C9A0      E8 6B071000            CALL MapleSto.00A7D110
	$+1CB    0097C9A5      F3:0F1045 B4           MOVSS XMM0,DWORD PTR SS:[EBP-0x4C]
	$+1D0    0097C9AA      8B4D DC                MOV ECX,DWORD PTR SS:[EBP-0x24]
	$+1D3    0097C9AD      8B45 D8                MOV EAX,DWORD PTR SS:[EBP-0x28]
	$+1D6    0097C9B0      F3:0F1186 A8100000     MOVSS DWORD PTR DS:[ESI+0x10A8],XMM0
	$+1DE    0097C9B8      F3:0F1045 B8           MOVSS XMM0,DWORD PTR SS:[EBP-0x48]
	$+1E3    0097C9BD      F3:0F1186 AC100000     MOVSS DWORD PTR DS:[ESI+0x10AC],XMM0
	$+1EB    0097C9C5      F3:0F1045 BC           MOVSS XMM0,DWORD PTR SS:[EBP-0x44]
	$+1F0    0097C9CA      F3:0F1186 B0100000     MOVSS DWORD PTR DS:[ESI+0x10B0],XMM0
	$+1F8    0097C9D2      F3:0F1045 08           MOVSS XMM0,DWORD PTR SS:[EBP+0x8]
	$+1FD    0097C9D7      8986 A0100000          MOV DWORD PTR DS:[ESI+0x10A0],EAX
	$+203    0097C9DD      898E A4100000          MOV DWORD PTR DS:[ESI+0x10A4],ECX
	$+209    0097C9E3      F3:0F1186 90110000     MOVSS DWORD PTR DS:[ESI+0x1190],XMM0
	$+211    0097C9EB      F3:0F1045 0C           MOVSS XMM0,DWORD PTR SS:[EBP+0xC]
	$+216    0097C9F0      F3:0F1186 94110000     MOVSS DWORD PTR DS:[ESI+0x1194],XMM0
	$+21E    0097C9F8      F3:0F1045 10           MOVSS XMM0,DWORD PTR SS:[EBP+0x10]
	$+223    0097C9FD      8D55 90                LEA EDX,DWORD PTR SS:[EBP-0x70]
	$+226    0097CA00      F3:0F1186 98110000     MOVSS DWORD PTR DS:[ESI+0x1198],XMM0
	$+22E    0097CA08      8B8E E8000000          MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$+234    0097CA0E      52                     PUSH EDX
	$+235    0097CA0F      E8 1CEFE6FF            CALL MapleSto.007EB930
	$+23A    0097CA14      D900                   FLD DWORD PTR DS:[EAX]
	$+23C    0097CA16      F3:0F1045 08           MOVSS XMM0,DWORD PTR SS:[EBP+0x8]
	$+241    0097CA1B      D99E A8110000          FSTP DWORD PTR DS:[ESI+0x11A8]
	$+247    0097CA21      D940 04                FLD DWORD PTR DS:[EAX+0x4]
	$+24A    0097CA24      D99E AC110000          FSTP DWORD PTR DS:[ESI+0x11AC]
	$+250    0097CA2A      D940 08                FLD DWORD PTR DS:[EAX+0x8]
	$+253    0097CA2D      D99E B0110000          FSTP DWORD PTR DS:[ESI+0x11B0]
	$+259    0097CA33      0F2E05 34267401        UCOMISS XMM0,DWORD PTR DS:[0x1742634]
	$+260    0097CA3A      9F                     LAHF
	$+261    0097CA3B      F6C4 44                TEST AH,0x44
	$+264    0097CA3E      7A 24                  JPE SHORT MapleSto.0097CA64
	$+266    0097CA40      F3:0F1045 0C           MOVSS XMM0,DWORD PTR SS:[EBP+0xC]
	$+26B    0097CA45      0F2E05 38267401        UCOMISS XMM0,DWORD PTR DS:[0x1742638]
	$+272    0097CA4C      9F                     LAHF
	$+273    0097CA4D      F6C4 44                TEST AH,0x44
	$+276    0097CA50      7A 12                  JPE SHORT MapleSto.0097CA64
	$+278    0097CA52      F3:0F1045 10           MOVSS XMM0,DWORD PTR SS:[EBP+0x10]
	$+27D    0097CA57      0F2E05 3C267401        UCOMISS XMM0,DWORD PTR DS:[0x174263C]
	$+284    0097CA5E      9F                     LAHF
	$+285    0097CA5F      F6C4 44                TEST AH,0x44
	$+288    0097CA62      7B 53                  JPO SHORT MapleSto.0097CAB7
	$+28A    0097CA64      8D45 08                LEA EAX,DWORD PTR SS:[EBP+0x8]
	$+28D    0097CA67      50                     PUSH EAX
	$+28E    0097CA68      E8 03F0C6FF            CALL MapleSto.005EBA70
	$+293    0097CA6D      83C4 04                ADD ESP,0x4
	$+296    0097CA70      85C0                   TEST EAX,EAX
	$+298    0097CA72      74 1A                  JE SHORT MapleSto.0097CA8E
	$+29A    0097CA74      8B4D 20                MOV ECX,DWORD PTR SS:[EBP+0x20]
	$+29D    0097CA77      F30F7E45 08            MOVQ XMM0,QWORD PTR SS:[EBP+0x8]
	$+2A2    0097CA7C      8B55 10                MOV EDX,DWORD PTR SS:[EBP+0x10]
	$+2A5    0097CA7F      51                     PUSH ECX
	$+2A6    0097CA80      83EC 0C                SUB ESP,0xC
	$+2A9    0097CA83      8BC4                   MOV EAX,ESP
	$+2AB    0097CA85      660FD600               MOVQ QWORD PTR DS:[EAX],XMM0
	$+2AF    0097CA89      8950 08                MOV DWORD PTR DS:[EAX+0x8],EDX
	$+2B2    0097CA8C      EB 1E                  JMP SHORT MapleSto.0097CAAC
	$+2B4    0097CA8E      8B45 20                MOV EAX,DWORD PTR SS:[EBP+0x20]
	$+2B7    0097CA91      F30F7E05 34267401      MOVQ XMM0,QWORD PTR DS:[0x1742634]
	$+2BF    0097CA99      8B0D 3C267401          MOV ECX,DWORD PTR DS:[0x174263C]
	$+2C5    0097CA9F      50                     PUSH EAX
	$+2C6    0097CAA0      83EC 0C                SUB ESP,0xC
	$+2C9    0097CAA3      8BC4                   MOV EAX,ESP
	$+2CB    0097CAA5      660FD600               MOVQ QWORD PTR DS:[EAX],XMM0
	$+2CF    0097CAA9      8948 08                MOV DWORD PTR DS:[EAX+0x8],ECX
	$+2D2    0097CAAC      8B8E E8000000          MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$+2D8    0097CAB2      E8 79F4E6FF            CALL MapleSto.007EBF30
	$+2DD    0097CAB7      8B8E E8000000          MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$+2E3    0097CABD      8D55 90                LEA EDX,DWORD PTR SS:[EBP-0x70]
	$+2E6    0097CAC0      52                     PUSH EDX
	$+2E7    0097CAC1      E8 6AEEE6FF            CALL MapleSto.007EB930
	$+2EC    0097CAC6      D900                   FLD DWORD PTR DS:[EAX]
	$+2EE    0097CAC8      8D8E B4110000          LEA ECX,DWORD PTR DS:[ESI+0x11B4]
	$+2F4    0097CACE      D919                   FSTP DWORD PTR DS:[ECX]
	$+2F6    0097CAD0      51                     PUSH ECX
	$+2F7    0097CAD1      D940 04                FLD DWORD PTR DS:[EAX+0x4]
	$+2FA    0097CAD4      D959 04                FSTP DWORD PTR DS:[ECX+0x4]
	$+2FD    0097CAD7      D940 08                FLD DWORD PTR DS:[EAX+0x8]
	$+300    0097CADA      D959 08                FSTP DWORD PTR DS:[ECX+0x8]
	$+303    0097CADD      E8 8EEFC6FF            CALL MapleSto.005EBA70
	$+308    0097CAE2      83C4 04                ADD ESP,0x4
	$+30B    0097CAE5      85C0                   TEST EAX,EAX
	$+30D    0097CAE7      75 4A                  JNZ SHORT MapleSto.0097CB33
	$+30F    0097CAE9      8B8E B0110000          MOV ECX,DWORD PTR DS:[ESI+0x11B0]
	$+315    0097CAEF      F30F7E86 A8110000      MOVQ XMM0,QWORD PTR DS:[ESI+0x11A8]
	$+31D    0097CAF7      8B16                   MOV EDX,DWORD PTR DS:[ESI]
	$+31F    0097CAF9      8B52 4C                MOV EDX,DWORD PTR DS:[EDX+0x4C]
	$+322    0097CAFC      83EC 0C                SUB ESP,0xC
	$+325    0097CAFF      8BC4                   MOV EAX,ESP
	$+327    0097CB01      660FD600               MOVQ QWORD PTR DS:[EAX],XMM0
	$+32B    0097CB05      8948 08                MOV DWORD PTR DS:[EAX+0x8],ECX
	$+32E    0097CB08      8BCE                   MOV ECX,ESI
	$+330    0097CB0A      FFD2                   CALL EDX
	$+332    0097CB0C      F3:0F1005 34267401     MOVSS XMM0,DWORD PTR DS:[0x1742634]
	$+33A    0097CB14      F3:0F1145 08           MOVSS DWORD PTR SS:[EBP+0x8],XMM0
	$+33F    0097CB19      F3:0F1005 38267401     MOVSS XMM0,DWORD PTR DS:[0x1742638]
	$+347    0097CB21      F3:0F1145 0C           MOVSS DWORD PTR SS:[EBP+0xC],XMM0
	$+34C    0097CB26      F3:0F1005 3C267401     MOVSS XMM0,DWORD PTR DS:[0x174263C]
	$+354    0097CB2E      F3:0F1145 10           MOVSS DWORD PTR SS:[EBP+0x10],XMM0
	$+359    0097CB33      8B8E E8000000          MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$+35F    0097CB39      E8 72ECE6FF            CALL MapleSto.007EB7B0
	$+364    0097CB3E      A8 04                  TEST AL,0x4
	$+366    0097CB40      75 32                  JNZ SHORT MapleSto.0097CB74
	$+368    0097CB42      A8 01                  TEST AL,0x1
	$+36A    0097CB44      74 1C                  JE SHORT MapleSto.0097CB62
	$+36C    0097CB46      83BE F0110000 00       CMP DWORD PTR DS:[ESI+0x11F0],0x0
	$+373    0097CB4D      75 30                  JNZ SHORT MapleSto.0097CB7F
	$+375    0097CB4F      8B8E E8000000          MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$+37B    0097CB55      E8 F6ECE6FF            CALL MapleSto.007EB850
	$+380    0097CB5A      8986 F0110000          MOV DWORD PTR DS:[ESI+0x11F0],EAX
	$+386    0097CB60      EB 1D                  JMP SHORT MapleSto.0097CB7F
	$+388    0097CB62      8B06                   MOV EAX,DWORD PTR DS:[ESI]
	$+38A    0097CB64      8B90 74010000          MOV EDX,DWORD PTR DS:[EAX+0x174]
	$+390    0097CB6A      6A 00                  PUSH 0x0
	$+392    0097CB6C      8BCE                   MOV ECX,ESI
	$+394    0097CB6E      FFD2                   CALL EDX
	$+396    0097CB70      85C0                   TEST EAX,EAX
	$+398    0097CB72      74 0B                  JE SHORT MapleSto.0097CB7F
	$+39A    0097CB74      0F57C0                 XORPS XMM0,XMM0
	$+39D    0097CB77      F3:0F1186 0C100000     MOVSS DWORD PTR DS:[ESI+0x100C],XMM0
	$+3A5    0097CB7F      8B8E E8000000          MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$+3AB    0097CB85      8D45 C0                LEA EAX,DWORD PTR SS:[EBP-0x40]
	$+3AE    0097CB88      50                     PUSH EAX
	$+3AF    0097CB89      E8 A2EDE6FF            CALL MapleSto.007EB930
	$+3B4    0097CB8E      F30F7E45 C0            MOVQ XMM0,QWORD PTR SS:[EBP-0x40]
	$+3B9    0097CB93      8B55 C8                MOV EDX,DWORD PTR SS:[EBP-0x38]
	$+3BC    0097CB96      8D4D 9C                LEA ECX,DWORD PTR SS:[EBP-0x64]
	$+3BF    0097CB99      51                     PUSH ECX
	$+3C0    0097CB9A      8B0D 2C417201          MOV ECX,DWORD PTR DS:[0x172412C]
	$+3C6    0097CBA0      83EC 0C                SUB ESP,0xC
	$+3C9    0097CBA3      8BC4                   MOV EAX,ESP
	$+3CB    0097CBA5      660FD600               MOVQ QWORD PTR DS:[EAX],XMM0
	$+3CF    0097CBA9      8950 08                MOV DWORD PTR DS:[EAX+0x8],EDX
	$+3D2    0097CBAC      E8 EFE91F00            CALL MapleSto.00B7B5A0
	$+3D7    0097CBB1      85C0                   TEST EAX,EAX
	$+3D9    0097CBB3      0F85 EE010000          JNZ MapleSto.0097CDA7
	$+3DF    0097CBB9      F3:0F1065 A4           MOVSS XMM4,DWORD PTR SS:[EBP-0x5C]
	$+3E4    0097CBBE      0F57C0                 XORPS XMM0,XMM0
	$+3E7    0097CBC1      F3:0F1055 10           MOVSS XMM2,DWORD PTR SS:[EBP+0x10]
	$+3EC    0097CBC6      0F2EE0                 UCOMISS XMM4,XMM0
	$+3EF    0097CBC9      9F                     LAHF
	$+3F0    0097CBCA      F6C4 44                TEST AH,0x44
	$+3F3    0097CBCD      7B 0D                  JPO SHORT MapleSto.0097CBDC
	$+3F5    0097CBCF      0F2FC2                 COMISS XMM0,XMM2
	$+3F8    0097CBD2      76 08                  JBE SHORT MapleSto.0097CBDC
	$+3FA    0097CBD4      0F28E0                 MOVAPS XMM4,XMM0
	$+3FD    0097CBD7      F3:0F1165 A4           MOVSS DWORD PTR SS:[EBP-0x5C],XMM4
	$+402    0097CBDC      F3:0F106D 9C           MOVSS XMM5,DWORD PTR SS:[EBP-0x64]
	$+407    0097CBE1      0F2E2D 34267401        UCOMISS XMM5,DWORD PTR DS:[0x1742634]
	$+40E    0097CBE8      F3:0F1075 A0           MOVSS XMM6,DWORD PTR SS:[EBP-0x60]
	$+413    0097CBED      9F                     LAHF
	$+414    0097CBEE      F6C4 44                TEST AH,0x44
	$+417    0097CBF1      7A 1E                  JPE SHORT MapleSto.0097CC11
	$+419    0097CBF3      0F2E35 38267401        UCOMISS XMM6,DWORD PTR DS:[0x1742638]
	$+420    0097CBFA      9F                     LAHF
	$+421    0097CBFB      F6C4 44                TEST AH,0x44
	$+424    0097CBFE      7A 11                  JPE SHORT MapleSto.0097CC11
	$+426    0097CC00      0F2E25 3C267401        UCOMISS XMM4,DWORD PTR DS:[0x174263C]
	$+42D    0097CC07      9F                     LAHF
	$+42E    0097CC08      F6C4 44                TEST AH,0x44
	$+431    0097CC0B      0F8B 96010000          JPO MapleSto.0097CDA7
	$+437    0097CC11      F3:0F1045 08           MOVSS XMM0,DWORD PTR SS:[EBP+0x8]
	$+43C    0097CC16      F3:0F104D 0C           MOVSS XMM1,DWORD PTR SS:[EBP+0xC]
	$+441    0097CC1B      F3:0F1186 60110000     MOVSS DWORD PTR DS:[ESI+0x1160],XMM0
	$+449    0097CC23      F3:0F118E 64110000     MOVSS DWORD PTR DS:[ESI+0x1164],XMM1
	$+451    0097CC2B      F3:0F1196 68110000     MOVSS DWORD PTR DS:[ESI+0x1168],XMM2
	$+459    0097CC33      F3:0F101D 00F84101     MOVSS XMM3,DWORD PTR DS:[0x141F800]
	$+461    0097CC3B      8D45 CC                LEA EAX,DWORD PTR SS:[EBP-0x34]
	$+464    0097CC3E      0F57C3                 XORPS XMM0,XMM3
	$+467    0097CC41      0F57CB                 XORPS XMM1,XMM3
	$+46A    0097CC44      0F57D3                 XORPS XMM2,XMM3
	$+46D    0097CC47      F3:0F59C5              MULSS XMM0,XMM5
	$+471    0097CC4B      F3:0F59CE              MULSS XMM1,XMM6
	$+475    0097CC4F      F3:0F59D4              MULSS XMM2,XMM4
	$+479    0097CC53      50                     PUSH EAX
	$+47A    0097CC54      F3:0F1145 CC           MOVSS DWORD PTR SS:[EBP-0x34],XMM0
	$+47F    0097CC59      F3:0F114D D0           MOVSS DWORD PTR SS:[EBP-0x30],XMM1
	$+484    0097CC5E      F3:0F1155 D4           MOVSS DWORD PTR SS:[EBP-0x2C],XMM2
	$+489    0097CC63      F3:0F1186 9C110000     MOVSS DWORD PTR DS:[ESI+0x119C],XMM0
	$+491    0097CC6B      F3:0F118E A0110000     MOVSS DWORD PTR DS:[ESI+0x11A0],XMM1
	$+499    0097CC73      F3:0F1196 A4110000     MOVSS DWORD PTR DS:[ESI+0x11A4],XMM2
	$+4A1    0097CC7B      E8 F0EDC6FF            CALL MapleSto.005EBA70
	$+4A6    0097CC80      83C4 04                ADD ESP,0x4
	$+4A9    0097CC83      85C0                   TEST EAX,EAX
	$+4AB    0097CC85      75 27                  JNZ SHORT MapleSto.0097CCAE
	$+4AD    0097CC87      F3:0F1005 34267401     MOVSS XMM0,DWORD PTR DS:[0x1742634]
	$+4B5    0097CC8F      F3:0F1145 CC           MOVSS DWORD PTR SS:[EBP-0x34],XMM0
	$+4BA    0097CC94      F3:0F1005 38267401     MOVSS XMM0,DWORD PTR DS:[0x1742638]
	$+4C2    0097CC9C      F3:0F1145 D0           MOVSS DWORD PTR SS:[EBP-0x30],XMM0
	$+4C7    0097CCA1      F3:0F1005 3C267401     MOVSS XMM0,DWORD PTR DS:[0x174263C]
	$+4CF    0097CCA9      F3:0F1145 D4           MOVSS DWORD PTR SS:[EBP-0x2C],XMM0
	$+4D4    0097CCAE      8D4D E0                LEA ECX,DWORD PTR SS:[EBP-0x20]
	$+4D7    0097CCB1      51                     PUSH ECX
	$+4D8    0097CCB2      8B8E E8000000          MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$+4DE    0097CCB8      E8 73ECE6FF            CALL MapleSto.007EB930
	$+4E3    0097CCBD      8B55 20                MOV EDX,DWORD PTR SS:[EBP+0x20]
	$+4E6    0097CCC0      F30F7E45 CC            MOVQ XMM0,QWORD PTR SS:[EBP-0x34]
	$+4EB    0097CCC5      8B4D D4                MOV ECX,DWORD PTR SS:[EBP-0x2C]
	$+4EE    0097CCC8      52                     PUSH EDX
	$+4EF    0097CCC9      83EC 0C                SUB ESP,0xC
	$+4F2    0097CCCC      8BC4                   MOV EAX,ESP
	$+4F4    0097CCCE      660FD600               MOVQ QWORD PTR DS:[EAX],XMM0
	$+4F8    0097CCD2      8948 08                MOV DWORD PTR DS:[EAX+0x8],ECX
	$+4FB    0097CCD5      8B8E E8000000          MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$+501    0097CCDB      E8 50F2E6FF            CALL MapleSto.007EBF30
	$+506    0097CCE0      8B8E E8000000          MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$+50C    0097CCE6      8D55 90                LEA EDX,DWORD PTR SS:[EBP-0x70]
	$+50F    0097CCE9      52                     PUSH EDX
	$+510    0097CCEA      E8 41ECE6FF            CALL MapleSto.007EB930
	$+515    0097CCEF      F3:0F1000              MOVSS XMM0,DWORD PTR DS:[EAX]
	$+519    0097CCF3      8B8E E8000000          MOV ECX,DWORD PTR DS:[ESI+0xE8]
	$+51F    0097CCF9      F3:0F1145 C0           MOVSS DWORD PTR SS:[EBP-0x40],XMM0
	$+524    0097CCFE      F3:0F1040 04           MOVSS XMM0,DWORD PTR DS:[EAX+0x4]
	$+529    0097CD03      F3:0F1145 C4           MOVSS DWORD PTR SS:[EBP-0x3C],XMM0
	$+52E    0097CD08      F3:0F1040 08           MOVSS XMM0,DWORD PTR DS:[EAX+0x8]
	$+533    0097CD0D      8D45 90                LEA EAX,DWORD PTR SS:[EBP-0x70]
	$+536    0097CD10      50                     PUSH EAX
	$+537    0097CD11      F3:0F1145 C8           MOVSS DWORD PTR SS:[EBP-0x38],XMM0
	$+53C    0097CD16      E8 15ECE6FF            CALL MapleSto.007EB930
	$+541    0097CD1B      D900                   FLD DWORD PTR DS:[EAX]
	$+543    0097CD1D      D99E C0110000          FSTP DWORD PTR DS:[ESI+0x11C0]
	$+549    0097CD23      8D4D C0                LEA ECX,DWORD PTR SS:[EBP-0x40]
	$+54C    0097CD26      D940 04                FLD DWORD PTR DS:[EAX+0x4]
	$+54F    0097CD29      51                     PUSH ECX
	$+550    0097CD2A      D99E C4110000          FSTP DWORD PTR DS:[ESI+0x11C4]
	$+556    0097CD30      D940 08                FLD DWORD PTR DS:[EAX+0x8]
	$+559    0097CD33      D99E C8110000          FSTP DWORD PTR DS:[ESI+0x11C8]
	$+55F    0097CD39      E8 32EDC6FF            CALL MapleSto.005EBA70
	$+564    0097CD3E      83C4 04                ADD ESP,0x4
	$+567    0097CD41      85C0                   TEST EAX,EAX
	$+569    0097CD43      75 62                  JNZ SHORT MapleSto.0097CDA7
	$+56B    0097CD45      8B4D E8                MOV ECX,DWORD PTR SS:[EBP-0x18]
	$+56E    0097CD48      F30F7E45 E0            MOVQ XMM0,QWORD PTR SS:[EBP-0x20]
	$+573    0097CD4D      8B16                   MOV EDX,DWORD PTR DS:[ESI]
	$+575    0097CD4F      8B52 4C                MOV EDX,DWORD PTR DS:[EDX+0x4C]
	$+578    0097CD52      83EC 0C                SUB ESP,0xC
	$+57B    0097CD55      8BC4                   MOV EAX,ESP
	$+57D    0097CD57      660FD600               MOVQ QWORD PTR DS:[EAX],XMM0
	$+581    0097CD5B      8948 08                MOV DWORD PTR DS:[EAX+0x8],ECX
	$+584    0097CD5E      8BCE                   MOV ECX,ESI
	$+586    0097CD60      FFD2                   CALL EDX
	$+588    0097CD62      F3:0F1005 34267401     MOVSS XMM0,DWORD PTR DS:[0x1742634]
	$+590    0097CD6A      F3:0F1145 08           MOVSS DWORD PTR SS:[EBP+0x8],XMM0
	$+595    0097CD6F      F3:0F1005 38267401     MOVSS XMM0,DWORD PTR DS:[0x1742638]
	$+59D    0097CD77      F3:0F1145 0C           MOVSS DWORD PTR SS:[EBP+0xC],XMM0
	$+5A2    0097CD7C      F3:0F1005 3C267401     MOVSS XMM0,DWORD PTR DS:[0x174263C]
	$+5AA    0097CD84      F3:0F1145 10           MOVSS DWORD PTR SS:[EBP+0x10],XMM0
	$+5AF    0097CD89      F3:0F1045 E0           MOVSS XMM0,DWORD PTR SS:[EBP-0x20]
	$+5B4    0097CD8E      F3:0F1145 C0           MOVSS DWORD PTR SS:[EBP-0x40],XMM0
	$+5B9    0097CD93      F3:0F1045 E4           MOVSS XMM0,DWORD PTR SS:[EBP-0x1C]
	$+5BE    0097CD98      F3:0F1145 C4           MOVSS DWORD PTR SS:[EBP-0x3C],XMM0
	$+5C3    0097CD9D      F3:0F1045 E8           MOVSS XMM0,DWORD PTR SS:[EBP-0x18]
	$+5C8    0097CDA2      F3:0F1145 C8           MOVSS DWORD PTR SS:[EBP-0x38],XMM0
	$+5CD    0097CDA7      8B4D C8                MOV ECX,DWORD PTR SS:[EBP-0x38]
	$+5D0    0097CDAA      F30F7E45 C0            MOVQ XMM0,QWORD PTR SS:[EBP-0x40]
	$+5D5    0097CDAF      83EC 0C                SUB ESP,0xC
	$+5D8    0097CDB2      8BC4                   MOV EAX,ESP
	$+5DA    0097CDB4      660FD600               MOVQ QWORD PTR DS:[EAX],XMM0
	$+5DE    0097CDB8      8948 08                MOV DWORD PTR DS:[EAX+0x8],ECX
	$+5E1    0097CDBB      8BCE                   MOV ECX,ESI
	$+5E3    0097CDBD      E8 EEE3E6FF            CALL MapleSto.007EB1B0
	$+5E8    0097CDC2      8B0D 744E7201          MOV ECX,DWORD PTR DS:[0x1724E74]
	$+5EE    0097CDC8      E8 C399C7FF            CALL MapleSto.005F6790
	$+5F3    0097CDCD      85C0                   TEST EAX,EAX
	$+5F5    0097CDCF      74 17                  JE SHORT MapleSto.0097CDE8
	$+5F7    0097CDD1      8B0D 744E7201          MOV ECX,DWORD PTR DS:[0x1724E74]
	$+5FD    0097CDD7      39B1 8C010000          CMP DWORD PTR DS:[ECX+0x18C],ESI
	$+603    0097CDDD      75 1C                  JNZ SHORT MapleSto.0097CDFB
	$+605    0097CDDF      E8 BC93C7FF            CALL MapleSto.005F61A0
	$+60A    0097CDE4      84C0                   TEST AL,AL
	$+60C    0097CDE6      74 13                  JE SHORT MapleSto.0097CDFB
	$+60E    0097CDE8      8B16                   MOV EDX,DWORD PTR DS:[ESI]
	$+610    0097CDEA      D945 18                FLD DWORD PTR SS:[EBP+0x18]
	$+613    0097CDED      8B82 F8000000          MOV EAX,DWORD PTR DS:[EDX+0xF8]
	$+619    0097CDF3      51                     PUSH ECX
	$+61A    0097CDF4      8BCE                   MOV ECX,ESI
	$+61C    0097CDF6      D91C24                 FSTP DWORD PTR SS:[ESP]
	$+61F    0097CDF9      FFD0                   CALL EAX
	$+621    0097CDFB      8B16                   MOV EDX,DWORD PTR DS:[ESI]
	$+623    0097CDFD      8B52 48                MOV EDX,DWORD PTR DS:[EDX+0x48]
	$+626    0097CE00      8D45 84                LEA EAX,DWORD PTR SS:[EBP-0x7C]
	$+629    0097CE03      50                     PUSH EAX
	$+62A    0097CE04      8BCE                   MOV ECX,ESI
	$+62C    0097CE06      FFD2                   CALL EDX
	$+62E    0097CE08      D900                   FLD DWORD PTR DS:[EAX]
	$+630    0097CE0A      D99E CC110000          FSTP DWORD PTR DS:[ESI+0x11CC]
	$+636    0097CE10      D940 04                FLD DWORD PTR DS:[EAX+0x4]
	$+639    0097CE13      D99E D0110000          FSTP DWORD PTR DS:[ESI+0x11D0]
	$+63F    0097CE19      D940 08                FLD DWORD PTR DS:[EAX+0x8]
	$+642    0097CE1C      D99E D4110000          FSTP DWORD PTR DS:[ESI+0x11D4]
	$+648    0097CE22      8B4D F4                MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+64B    0097CE25      64:890D 00000000       MOV DWORD PTR FS:[0],ECX
	$+652    0097CE2C      59                     POP ECX
	$+653    0097CE2D      5F                     POP EDI
	$+654    0097CE2E      5E                     POP ESI
	$+655    0097CE2F      5B                     POP EBX
	$+656    0097CE30      8BE5                   MOV ESP,EBP
	$+658    0097CE32      5D                     POP EBP
	$+659    0097CE33      C2 2000                RETN 0x20
	$+65C    0097CE36      CC                     INT3
	*/
	addr_tmp = scan_exe_.ScanCall("680F0000F08D55B4526852CA95FD8BCF", 0x10 + 1);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_c_get_square_pos_ = " << addr_tmp << ";" << FC_OFstream_EndlLog;

	/*
	$-21F    006CB36F     CC                         INT3
	$-21E    006CB370     55                         PUSH EBP
	$-21D    006CB371     8BEC                       MOV EBP,ESP
	$-21B    006CB373     56                         PUSH ESI
	$-21A    006CB374     8BF1                       MOV ESI,ECX
	$-218    006CB376     8B0D 00257201              MOV ECX,DWORD PTR DS:[0x1722500]
	$-212    006CB37C     6A 3E                      PUSH 0x3E
	$-210    006CB37E     E8 6DA2DAFF                CALL MapleSto.004755F0
	$-20B    006CB383     84C0                       TEST AL,AL
	$-209    006CB385     74 4F                      JE SHORT MapleSto.006CB3D6
	$-207    006CB387     8B0D 2C417201              MOV ECX,DWORD PTR DS:[0x172412C]
	$-201    006CB38D     E8 41867D01                CALL MapleSto.01EA39D3
	$-1FC    006CB392     85C0                       TEST EAX,EAX
	$-1FA    006CB394     74 40                      JE SHORT MapleSto.006CB3D6
	$-1F8    006CB396     8BC8                       MOV ECX,EAX
	$-1F6    006CB398     E8 E1E37301                CALL MapleSto.01E0977E
	$-1F1    006CB39D     85C0                       TEST EAX,EAX
	$-1EF    006CB39F     74 35                      JE SHORT MapleSto.006CB3D6
	$-1ED    006CB3A1     837E 08 00                 CMP DWORD PTR DS:[ESI+0x8],0x0
	$-1E9    006CB3A5     74 2F                      JE SHORT MapleSto.006CB3D6
	$-1E7    006CB3A7     8B0D 28277201              MOV ECX,DWORD PTR DS:[0x1722728]
	$-1E1    006CB3AD     6A 01                      PUSH 0x1
	$-1DF    006CB3AF     E8 7D6D6D00                CALL MapleSto.00DA2131
	$-1DA    006CB3B4     85C0                       TEST EAX,EAX
	$-1D8    006CB3B6     75 1E                      JNZ SHORT MapleSto.006CB3D6
	$-1D6    006CB3B8     D945 10                    FLD DWORD PTR SS:[EBP+0x10]
	$-1D3    006CB3BB     8B4E 08                    MOV ECX,DWORD PTR DS:[ESI+0x8]
	$-1D0    006CB3BE     8B01                       MOV EAX,DWORD PTR DS:[ECX]
	$-1CE    006CB3C0     8B55 08                    MOV EDX,DWORD PTR SS:[EBP+0x8]
	$-1CB    006CB3C3     8B40 18                    MOV EAX,DWORD PTR DS:[EAX+0x18]
	$-1C8    006CB3C6     83EC 08                    SUB ESP,0x8
	$-1C5    006CB3C9     D95C24 04                  FSTP DWORD PTR SS:[ESP+0x4]
	$-1C1    006CB3CD     D945 0C                    FLD DWORD PTR SS:[EBP+0xC]
	$-1BE    006CB3D0     D91C24                     FSTP DWORD PTR SS:[ESP]
	$-1BB    006CB3D3     52                         PUSH EDX
	$-1BA    006CB3D4     FFD0                       CALL EAX
	$-1B8    006CB3D6     5E                         POP ESI
	$-1B7    006CB3D7     5D                         POP EBP
	$-1B6    006CB3D8     C2 0C00                    RETN 0xC
	$-1B3    006CB3DB     CC                         INT3
	$-1B2    006CB3DC     CC                         INT3
	$-1B1    006CB3DD     CC                         INT3
	$-1B0    006CB3DE     CC                         INT3
	$-1AF    006CB3DF     CC                         INT3
	$-1AE    006CB3E0     55                         PUSH EBP
	$-1AD    006CB3E1     8BEC                       MOV EBP,ESP
	$-1AB    006CB3E3     56                         PUSH ESI
	$-1AA    006CB3E4     8BF1                       MOV ESI,ECX
	$-1A8    006CB3E6     8B0D 00257201              MOV ECX,DWORD PTR DS:[0x1722500]
	$-1A2    006CB3EC     6A 3E                      PUSH 0x3E
	$-1A0    006CB3EE     E8 FDA1DAFF                CALL MapleSto.004755F0
	$-19B    006CB3F3     84C0                       TEST AL,AL
	$-199    006CB3F5     74 56                      JE SHORT MapleSto.006CB44D
	$-197    006CB3F7     8B0D 2C417201              MOV ECX,DWORD PTR DS:[0x172412C]
	$-191    006CB3FD     E8 D1857D01                CALL MapleSto.01EA39D3
	$-18C    006CB402     85C0                       TEST EAX,EAX
	$-18A    006CB404     74 47                      JE SHORT MapleSto.006CB44D
	$-188    006CB406     8BC8                       MOV ECX,EAX
	$-186    006CB408     E8 71E37301                CALL MapleSto.01E0977E
	$-181    006CB40D     85C0                       TEST EAX,EAX
	$-17F    006CB40F     74 3C                      JE SHORT MapleSto.006CB44D
	$-17D    006CB411     837E 08 00                 CMP DWORD PTR DS:[ESI+0x8],0x0
	$-179    006CB415     74 36                      JE SHORT MapleSto.006CB44D
	$-177    006CB417     8B0D 28277201              MOV ECX,DWORD PTR DS:[0x1722728]
	$-171    006CB41D     6A 01                      PUSH 0x1
	$-16F    006CB41F     E8 0D6D6D00                CALL MapleSto.00DA2131
	$-16A    006CB424     85C0                       TEST EAX,EAX
	$-168    006CB426     75 25                      JNZ SHORT MapleSto.006CB44D
	$-166    006CB428     D945 14                    FLD DWORD PTR SS:[EBP+0x14]
	$-163    006CB42B     8B4E 08                    MOV ECX,DWORD PTR DS:[ESI+0x8]
	$-160    006CB42E     8B01                       MOV EAX,DWORD PTR DS:[ECX]
	$-15E    006CB430     8B55 08                    MOV EDX,DWORD PTR SS:[EBP+0x8]
	$-15B    006CB433     8B40 1C                    MOV EAX,DWORD PTR DS:[EAX+0x1C]
	$-158    006CB436     83EC 0C                    SUB ESP,0xC
	$-155    006CB439     D95C24 08                  FSTP DWORD PTR SS:[ESP+0x8]
	$-151    006CB43D     D945 10                    FLD DWORD PTR SS:[EBP+0x10]
	$-14E    006CB440     D95C24 04                  FSTP DWORD PTR SS:[ESP+0x4]
	$-14A    006CB444     D945 0C                    FLD DWORD PTR SS:[EBP+0xC]
	$-147    006CB447     D91C24                     FSTP DWORD PTR SS:[ESP]
	$-144    006CB44A     52                         PUSH EDX
	$-143    006CB44B     FFD0                       CALL EAX
	$-141    006CB44D     5E                         POP ESI
	$-140    006CB44E     5D                         POP EBP
	$-13F    006CB44F     C2 1000                    RETN 0x10
	$-13C    006CB452     CC                         INT3
	$-13B    006CB453     CC                         INT3
	$-13A    006CB454     CC                         INT3
	$-139    006CB455     CC                         INT3
	$-138    006CB456     CC                         INT3
	$-137    006CB457     CC                         INT3
	$-136    006CB458     CC                         INT3
	$-135    006CB459     CC                         INT3
	$-134    006CB45A     CC                         INT3
	$-133    006CB45B     CC                         INT3
	$-132    006CB45C     CC                         INT3
	$-131    006CB45D     CC                         INT3
	$-130    006CB45E     CC                         INT3
	$-12F    006CB45F     CC                         INT3
	$-12E    006CB460     55                         PUSH EBP
	$-12D    006CB461     8BEC                       MOV EBP,ESP
	$-12B    006CB463     6A FF                      PUSH -0x1
	$-129    006CB465     68 F85C3201                PUSH MapleSto.01325CF8
	$-124    006CB46A     64:A1 00000000             MOV EAX,DWORD PTR FS:[0]
	$-11E    006CB470     50                         PUSH EAX
	$-11D    006CB471     83EC 4C                    SUB ESP,0x4C
	$-11A    006CB474     53                         PUSH EBX
	$-119    006CB475     56                         PUSH ESI
	$-118    006CB476     57                         PUSH EDI
	$-117    006CB477     A1 D0687101                MOV EAX,DWORD PTR DS:[0x17168D0]
	$-112    006CB47C     33C5                       XOR EAX,EBP
	$-110    006CB47E     50                         PUSH EAX
	$-10F    006CB47F     8D45 F4                    LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-10C    006CB482     64:A3 00000000             MOV DWORD PTR FS:[0],EAX
	$-106    006CB488     8B0D 2C417201              MOV ECX,DWORD PTR DS:[0x172412C]
	$-100    006CB48E     E8 40857D01                CALL MapleSto.01EA39D3
	$-FB     006CB493     8BF0                       MOV ESI,EAX
	$-F9     006CB495     85F6                       TEST ESI,ESI
	$-F7     006CB497     0F84 B5020000              JE MapleSto.006CB752
	$-F1     006CB49D     8BCE                       MOV ECX,ESI
	$-EF     006CB49F     E8 DAE27301                CALL MapleSto.01E0977E
	$-EA     006CB4A4     85C0                       TEST EAX,EAX
	$-E8     006CB4A6     0F84 A6020000              JE MapleSto.006CB752
	$-E2     006CB4AC     8BCE                       MOV ECX,ESI
	$-E0     006CB4AE     E8 5F2C7501                CALL MapleSto.01E1E112
	$-DB     006CB4B3     8BD8                       MOV EBX,EAX
	$-D9     006CB4B5     85DB                       TEST EBX,EBX
	$-D7     006CB4B7     0F84 95020000              JE MapleSto.006CB752
	$-D1     006CB4BD     8B0D 48417201              MOV ECX,DWORD PTR DS:[0x1724148]
	$-CB     006CB4C3     6A 00                      PUSH 0x0
	$-C9     006CB4C5     E8 167D4500                CALL MapleSto.00B231E0
	$-C4     006CB4CA     8B0D 28277201              MOV ECX,DWORD PTR DS:[0x1722728]
	$-BE     006CB4D0     6A 01                      PUSH 0x1
	$-BC     006CB4D2     E8 5A6C6D00                CALL MapleSto.00DA2131
	$-B7     006CB4D7     85C0                       TEST EAX,EAX
	$-B5     006CB4D9     0F85 73020000              JNZ MapleSto.006CB752
	$-AF     006CB4DF     8B0D 2C407201              MOV ECX,DWORD PTR DS:[0x172402C]
	$-A9     006CB4E5     50                         PUSH EAX
	$-A8     006CB4E6     50                         PUSH EAX
	$-A7     006CB4E7     50                         PUSH EAX
	$-A6     006CB4E8     E8 53820D00                CALL MapleSto.007A3740
	$-A1     006CB4ED     84C0                       TEST AL,AL
	$-9F     006CB4EF     0F84 5D020000              JE MapleSto.006CB752
	$-99     006CB4F5     D9EE                       FLDZ
	$-97     006CB4F7     8D45 EC                    LEA EAX,DWORD PTR SS:[EBP-0x14]
	$-94     006CB4FA     50                         PUSH EAX
	$-93     006CB4FB     D955 E8                    FST DWORD PTR SS:[EBP-0x18]
	$-90     006CB4FE     8D4D E8                    LEA ECX,DWORD PTR SS:[EBP-0x18]
	$-8D     006CB501     D95D EC                    FSTP DWORD PTR SS:[EBP-0x14]
	$-8A     006CB504     51                         PUSH ECX
	$-89     006CB505     8B0D B8607201              MOV ECX,DWORD PTR DS:[0x17260B8]
	$-83     006CB50B     E8 B9097E01                CALL MapleSto.01EABEC9
	$-7E     006CB510     D945 EC                    FLD DWORD PTR SS:[EBP-0x14]
	$-7B     006CB513     8B0D 744E7201              MOV ECX,DWORD PTR DS:[0x1724E74]
	$-75     006CB519     6A 00                      PUSH 0x0
	$-73     006CB51B     6A 00                      PUSH 0x0
	$-71     006CB51D     6A 01                      PUSH 0x1
	$-6F     006CB51F     6A 01                      PUSH 0x1
	$-6D     006CB521     83EC 08                    SUB ESP,0x8
	$-6A     006CB524     D95C24 04                  FSTP DWORD PTR SS:[ESP+0x4]
	$-66     006CB528     8D55 F0                    LEA EDX,DWORD PTR SS:[EBP-0x10]
	$-63     006CB52B     D945 E8                    FLD DWORD PTR SS:[EBP-0x18]
	$-60     006CB52E     D91C24                     FSTP DWORD PTR SS:[ESP]
	$-5D     006CB531     52                         PUSH EDX
	$-5C     006CB532     E8 89B2F2FF                CALL MapleSto.005F67C0
	$-57     006CB537     8B4D F0                    MOV ECX,DWORD PTR SS:[EBP-0x10]
	$-54     006CB53A     C745 FC 00000000           MOV DWORD PTR SS:[EBP-0x4],0x0
	$-4D     006CB541     85C9                       TEST ECX,ECX
	$-4B     006CB543     0F84 09020000              JE MapleSto.006CB752
	$-45     006CB549     8B41 1C                    MOV EAX,DWORD PTR DS:[ECX+0x1C]
	$-42     006CB54C     33F6                       XOR ESI,ESI
	$-40     006CB54E     8975 E4                    MOV DWORD PTR SS:[EBP-0x1C],ESI
	$-3D     006CB551     3970 10                    CMP DWORD PTR DS:[EAX+0x10],ESI
	$-3A     006CB554     0F86 D5010000              JBE MapleSto.006CB72F
	$-34     006CB55A     8B40 04                    MOV EAX,DWORD PTR DS:[EAX+0x4]
	$-31     006CB55D     8B04B0                     MOV EAX,DWORD PTR DS:[EAX+ESI*4]
	$-2E     006CB560     85C0                       TEST EAX,EAX
	$-2C     006CB562     0F84 03010000              JE MapleSto.006CB66B
	$-26     006CB568     8B00                       MOV EAX,DWORD PTR DS:[EAX]
	$-24     006CB56A     50                         PUSH EAX
	$-23     006CB56B     68 CCC07301                PUSH MapleSto.0173C0CC
	$-1E     006CB570     E8 2B6AF1FF                CALL MapleSto.005E1FA0
	$-19     006CB575     83C4 08                    ADD ESP,0x8
	$-16     006CB578     85C0                       TEST EAX,EAX
	$-14     006CB57A     0F84 E8000000              JE MapleSto.006CB668
	$-E      006CB580     8B88 90000000              MOV ECX,DWORD PTR DS:[EAX+0x90]
	$-8      006CB586     F30F7E80 88000000          MOVQ XMM0,QWORD PTR DS:[EAX+0x88]
	$ ==>    006CB58E     8B13                       MOV EDX,DWORD PTR DS:[EBX]
	$+2      006CB590     8B52 50                    MOV EDX,DWORD PTR DS:[EDX+0x50]
	$+5      006CB593     8B3D 884E7201              MOV EDI,DWORD PTR DS:[0x1724E88]         ; g_c_map_information_service_
	$+B      006CB599     660FD645 C0                MOVQ QWORD PTR SS:[EBP-0x40],XMM0
	$+10     006CB59E     F3:0F104D C0               MOVSS XMM1,DWORD PTR SS:[EBP-0x40]
	$+15     006CB5A3     F3:0F1055 C4               MOVSS XMM2,DWORD PTR SS:[EBP-0x3C]
	$+1A     006CB5A8     894D C8                    MOV DWORD PTR SS:[EBP-0x38],ECX
	$+1D     006CB5AB     F3:0F1045 C8               MOVSS XMM0,DWORD PTR SS:[EBP-0x38]
	$+22     006CB5B0     F3:0F5805 C8034201         ADDSS XMM0,DWORD PTR DS:[0x14203C8]
	$+2A     006CB5B8     F3:0F1145 D8               MOVSS DWORD PTR SS:[EBP-0x28],XMM0
	$+2F     006CB5BD     F3:0F1145 D4               MOVSS DWORD PTR SS:[EBP-0x2C],XMM0
	$+34     006CB5C2     F30F7E80 88000000          MOVQ XMM0,QWORD PTR DS:[EAX+0x88]
	$+3C     006CB5CA     8D45 A8                    LEA EAX,DWORD PTR SS:[EBP-0x58]
	$+3F     006CB5CD     8BF1                       MOV ESI,ECX
	$+41     006CB5CF     50                         PUSH EAX
	$+42     006CB5D0     8BCB                       MOV ECX,EBX
	$+44     006CB5D2     F3:0F114D E0               MOVSS DWORD PTR SS:[EBP-0x20],XMM1
	$+49     006CB5D7     F3:0F1155 DC               MOVSS DWORD PTR SS:[EBP-0x24],XMM2
	$+4E     006CB5DC     F3:0F114D CC               MOVSS DWORD PTR SS:[EBP-0x34],XMM1
	$+53     006CB5E1     F3:0F1155 D0               MOVSS DWORD PTR SS:[EBP-0x30],XMM2
	$+58     006CB5E6     660FD645 B4                MOVQ QWORD PTR SS:[EBP-0x4C],XMM0
	$+5D     006CB5EB     FFD2                       CALL EDX
	$+5F     006CB5ED     D905 60EB4001              FLD DWORD PTR DS:[0x140EB60]
	$+65     006CB5F3     F30F7E05 70497201          MOVQ XMM0,QWORD PTR DS:[0x1724970]
	$+6D     006CB5FB     8B15 78497201              MOV EDX,DWORD PTR DS:[0x1724978]
	$+73     006CB601     83EC 0C                    SUB ESP,0xC
	$+76     006CB604     8BCC                       MOV ECX,ESP
	$+78     006CB606     660FD601                   MOVQ QWORD PTR DS:[ECX],XMM0
	$+7C     006CB60A     F30F7E00                   MOVQ XMM0,QWORD PTR DS:[EAX]
	$+80     006CB60E     8B40 08                    MOV EAX,DWORD PTR DS:[EAX+0x8]
	$+83     006CB611     8951 08                    MOV DWORD PTR DS:[ECX+0x8],EDX
	$+86     006CB614     83EC 10                    SUB ESP,0x10
	$+89     006CB617     D95C24 0C                  FSTP DWORD PTR SS:[ESP+0xC]
	$+8D     006CB61B     8BCC                       MOV ECX,ESP
	$+8F     006CB61D     660FD601                   MOVQ QWORD PTR DS:[ECX],XMM0
	$+93     006CB621     F30F7E45 B4                MOVQ XMM0,QWORD PTR SS:[EBP-0x4C]
	$+98     006CB626     8941 08                    MOV DWORD PTR DS:[ECX+0x8],EAX
	$+9B     006CB629     83EC 0C                    SUB ESP,0xC
	$+9E     006CB62C     8BC4                       MOV EAX,ESP
	$+A0     006CB62E     660FD600                   MOVQ QWORD PTR DS:[EAX],XMM0
	$+A4     006CB632     8BCF                       MOV ECX,EDI
	$+A6     006CB634     8970 08                    MOV DWORD PTR DS:[EAX+0x8],ESI
	$+A9     006CB637     E8 64440B00                CALL MapleSto.0077FAA0
	$+AE     006CB63C     85C0                       TEST EAX,EAX
	$+B0     006CB63E     74 25                      JE SHORT MapleSto.006CB665
	$+B2     006CB640     F30F7E45 CC                MOVQ XMM0,QWORD PTR SS:[EBP-0x34]
	$+B7     006CB645     8B7D D4                    MOV EDI,DWORD PTR SS:[EBP-0x2C]
	$+BA     006CB648     8B0D 884E7201              MOV ECX,DWORD PTR DS:[0x1724E88]
	$+C0     006CB64E     83EC 0C                    SUB ESP,0xC
	$+C3     006CB651     8BC4                       MOV EAX,ESP
	$+C5     006CB653     660FD600                   MOVQ QWORD PTR DS:[EAX],XMM0
	$+C9     006CB657     8978 08                    MOV DWORD PTR DS:[EAX+0x8],EDI
	$+CC     006CB65A     E8 71AB0B00                CALL MapleSto.007861D0
	$+D1     006CB65F     8BF0                       MOV ESI,EAX
	$+D3     006CB661     85F6                       TEST ESI,ESI
	$+D5     006CB663     75 1B                      JNZ SHORT MapleSto.006CB680
	$+D7     006CB665     8B75 E4                    MOV ESI,DWORD PTR SS:[EBP-0x1C]
	$+DA     006CB668     8B4D F0                    MOV ECX,DWORD PTR SS:[EBP-0x10]
	$+DD     006CB66B     8B41 1C                    MOV EAX,DWORD PTR DS:[ECX+0x1C]
	$+E0     006CB66E     46                         INC ESI
	$+E1     006CB66F     8975 E4                    MOV DWORD PTR SS:[EBP-0x1C],ESI
	$+E4     006CB672     3B70 10                    CMP ESI,DWORD PTR DS:[EAX+0x10]
	$+E7     006CB675   ^ 0F82 DFFEFFFF              JB MapleSto.006CB55A
	$+ED     006CB67B     E9 AF000000                JMP MapleSto.006CB72F
	$+F2     006CB680     F3:0F1045 E0               MOVSS XMM0,DWORD PTR SS:[EBP-0x20]
	$+F7     006CB685     F3:0F104D DC               MOVSS XMM1,DWORD PTR SS:[EBP-0x24]
	$+FC     006CB68A     F3:0F1055 D8               MOVSS XMM2,DWORD PTR SS:[EBP-0x28]
	$+101    006CB68F     F3:0F5815 74DE4001         ADDSS XMM2,DWORD PTR DS:[0x140DE74]
	$+109    006CB697     8B15 884E7201              MOV EDX,DWORD PTR DS:[0x1724E88]
	$+10F    006CB69D     F3:0F1145 C0               MOVSS DWORD PTR SS:[EBP-0x40],XMM0
	$+114    006CB6A2     F3:0F114D C4               MOVSS DWORD PTR SS:[EBP-0x3C],XMM1
	$+119    006CB6A7     F30F7E45 C0                MOVQ XMM0,QWORD PTR SS:[EBP-0x40]
	$+11E    006CB6AC     83EC 0C                    SUB ESP,0xC
	$+121    006CB6AF     8BC4                       MOV EAX,ESP
	$+123    006CB6B1     660FD645 B4                MOVQ QWORD PTR SS:[EBP-0x4C],XMM0
	$+128    006CB6B6     F30F7E45 CC                MOVQ XMM0,QWORD PTR SS:[EBP-0x34]
	$+12D    006CB6BB     660FD600                   MOVQ QWORD PTR DS:[EAX],XMM0
	$+131    006CB6BF     8978 08                    MOV DWORD PTR DS:[EAX+0x8],EDI
	$+134    006CB6C2     F3:0F1155 C8               MOVSS DWORD PTR SS:[EBP-0x38],XMM2
	$+139    006CB6C7     8B4D C8                    MOV ECX,DWORD PTR SS:[EBP-0x38]
	$+13C    006CB6CA     8D45 A8                    LEA EAX,DWORD PTR SS:[EBP-0x58]
	$+13F    006CB6CD     50                         PUSH EAX
	$+140    006CB6CE     F3:0F1155 DC               MOVSS DWORD PTR SS:[EBP-0x24],XMM2
	$+145    006CB6D3     894D BC                    MOV DWORD PTR SS:[EBP-0x44],ECX
	$+148    006CB6D6     8955 D8                    MOV DWORD PTR SS:[EBP-0x28],EDX
	$+14B    006CB6D9     E8 6285EBFF                CALL MapleSto.00583C40
	$+150    006CB6DE     F30F7E00                   MOVQ XMM0,QWORD PTR DS:[EAX]
	$+154    006CB6E2     8B50 08                    MOV EDX,DWORD PTR DS:[EAX+0x8]
	$+157    006CB6E5     83C4 04                    ADD ESP,0x4
	$+15A    006CB6E8     8BCC                       MOV ECX,ESP
	$+15C    006CB6EA     660FD601                   MOVQ QWORD PTR DS:[ECX],XMM0
	$+160    006CB6EE     8951 08                    MOV DWORD PTR DS:[ECX+0x8],EDX
	$+163    006CB6F1     8B4D D8                    MOV ECX,DWORD PTR SS:[EBP-0x28]
	$+166    006CB6F4     E8 67C20B00                CALL MapleSto.00787960
	$+16B    006CB6F9     3BF0                       CMP ESI,EAX
	$+16D    006CB6FB     75 12                      JNZ SHORT MapleSto.006CB70F
	$+16F    006CB6FD     F3:0F1045 DC               MOVSS XMM0,DWORD PTR SS:[EBP-0x24]
	$+174    006CB702     F3:0F5805 88E34001         ADDSS XMM0,DWORD PTR DS:[0x140E388]
	$+17C    006CB70A     F3:0F1145 BC               MOVSS DWORD PTR SS:[EBP-0x44],XMM0
	$+181    006CB70F     F30F7E45 B4                MOVQ XMM0,QWORD PTR SS:[EBP-0x4C]
	$+186    006CB714     8B4D BC                    MOV ECX,DWORD PTR SS:[EBP-0x44]
	$+189    006CB717     8B13                       MOV EDX,DWORD PTR DS:[EBX]
	$+18B    006CB719     8B52 4C                    MOV EDX,DWORD PTR DS:[EDX+0x4C]
	$+18E    006CB71C     83EC 0C                    SUB ESP,0xC
	$+191    006CB71F     8BC4                       MOV EAX,ESP
	$+193    006CB721     660FD600                   MOVQ QWORD PTR DS:[EAX],XMM0
	$+197    006CB725     8948 08                    MOV DWORD PTR DS:[EAX+0x8],ECX
	$+19A    006CB728     8BCB                       MOV ECX,EBX
	$+19C    006CB72A     FFD2                       CALL EDX
	$+19E    006CB72C     8B4D F0                    MOV ECX,DWORD PTR SS:[EBP-0x10]
	$+1A1    006CB72F     C745 FC FFFFFFFF           MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+1A8    006CB736     85C9                       TEST ECX,ECX
	$+1AA    006CB738     74 18                      JE SHORT MapleSto.006CB752
	$+1AC    006CB73A     51                         PUSH ECX
	$+1AD    006CB73B     8BF1                       MOV ESI,ECX
	$+1AF    006CB73D     FF15 6C933E01              CALL DWORD PTR DS:[0x13E936C]
	$+1B5    006CB743     85C0                       TEST EAX,EAX
	$+1B7    006CB745     75 0B                      JNZ SHORT MapleSto.006CB752
	$+1B9    006CB747     6A 20                      PUSH 0x20
	$+1BB    006CB749     56                         PUSH ESI
	$+1BC    006CB74A     E8 716E7100                CALL MapleSto.00DE25C0
	$+1C1    006CB74F     83C4 08                    ADD ESP,0x8
	$+1C4    006CB752     8B4D F4                    MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+1C7    006CB755     64:890D 00000000           MOV DWORD PTR FS:[0],ECX
	$+1CE    006CB75C     59                         POP ECX
	$+1CF    006CB75D     5F                         POP EDI
	$+1D0    006CB75E     5E                         POP ESI
	$+1D1    006CB75F     5B                         POP EBX
	$+1D2    006CB760     8BE5                       MOV ESP,EBP
	$+1D4    006CB762     5D                         POP EBP
	$+1D5    006CB763     C3                         RETN
	$+1D6    006CB764     CC                         INT3
	$+1D7    006CB765     CC                         INT3
	$+1D8    006CB766     CC                         INT3
	$+1D9    006CB767     CC                         INT3
	$+1DA    006CB768     CC                         INT3
	$+1DB    006CB769     CC                         INT3
	$+1DC    006CB76A     CC                         INT3
	$+1DD    006CB76B     CC                         INT3
	$+1DE    006CB76C     CC                         INT3
	$+1DF    006CB76D     CC                         INT3
	$+1E0    006CB76E     CC                         INT3
	$+1E1    006CB76F     CC                         INT3
	$+1E2    006CB770     55                         PUSH EBP
	$+1E3    006CB771     8BEC                       MOV EBP,ESP
	$+1E5    006CB773     6A FF                      PUSH -0x1
	$+1E7    006CB775     68 1B5D3201                PUSH MapleSto.01325D1B
	$+1EC    006CB77A     64:A1 00000000             MOV EAX,DWORD PTR FS:[0]
	$+1F2    006CB780     50                         PUSH EAX
	$+1F3    006CB781     83EC 4C                    SUB ESP,0x4C
	$+1F6    006CB784     53                         PUSH EBX
	$+1F7    006CB785     56                         PUSH ESI
	$+1F8    006CB786     57                         PUSH EDI
	$+1F9    006CB787     A1 D0687101                MOV EAX,DWORD PTR DS:[0x17168D0]
	$+1FE    006CB78C     33C5                       XOR EAX,EBP
	$+200    006CB78E     50                         PUSH EAX
	$+201    006CB78F     8D45 F4                    LEA EAX,DWORD PTR SS:[EBP-0xC]
	$+204    006CB792     64:A3 00000000             MOV DWORD PTR FS:[0],EAX
	$+20A    006CB798     8B0D 2C417201              MOV ECX,DWORD PTR DS:[0x172412C]
	$+210    006CB79E     E8 30827D01                CALL MapleSto.01EA39D3
	$+215    006CB7A3     8BF0                       MOV ESI,EAX
	$+217    006CB7A5     85F6                       TEST ESI,ESI
	$+219    006CB7A7     0F84 B5020000              JE MapleSto.006CBA62
	$+21F    006CB7AD     8BCE                       MOV ECX,ESI
	$+221    006CB7AF     E8 CADF7301                CALL MapleSto.01E0977E
	$+226    006CB7B4     85C0                       TEST EAX,EAX
	$+228    006CB7B6     0F84 A6020000              JE MapleSto.006CBA62
	$+22E    006CB7BC     8BCE                       MOV ECX,ESI
	$+230    006CB7BE     E8 4F297501                CALL MapleSto.01E1E112
	$+235    006CB7C3     8BD8                       MOV EBX,EAX
	$+237    006CB7C5     85DB                       TEST EBX,EBX
	$+239    006CB7C7     0F84 95020000              JE MapleSto.006CBA62
	$+23F    006CB7CD     8B0D 48417201              MOV ECX,DWORD PTR DS:[0x1724148]
	$+245    006CB7D3     6A 01                      PUSH 0x1
	$+247    006CB7D5     E8 067A4500                CALL MapleSto.00B231E0
	$+24C    006CB7DA     8B0D 28277201              MOV ECX,DWORD PTR DS:[0x1722728]
	$+252    006CB7E0     6A 01                      PUSH 0x1
	$+254    006CB7E2     E8 4A696D00                CALL MapleSto.00DA2131
	$+259    006CB7E7     85C0                       TEST EAX,EAX
	$+25B    006CB7E9     0F85 73020000              JNZ MapleSto.006CBA62
	$+261    006CB7EF     8B0D 2C407201              MOV ECX,DWORD PTR DS:[0x172402C]
	$+267    006CB7F5     50                         PUSH EAX
	$+268    006CB7F6     50                         PUSH EAX
	$+269    006CB7F7     50                         PUSH EAX
	$+26A    006CB7F8     E8 437F0D00                CALL MapleSto.007A3740
	$+26F    006CB7FD     84C0                       TEST AL,AL
	$+271    006CB7FF     0F84 5D020000              JE MapleSto.006CBA62
	$+277    006CB805     D9EE                       FLDZ
	$+279    006CB807     8D45 EC                    LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+27C    006CB80A     50                         PUSH EAX
	$+27D    006CB80B     D955 E8                    FST DWORD PTR SS:[EBP-0x18]
	$+280    006CB80E     8D4D E8                    LEA ECX,DWORD PTR SS:[EBP-0x18]
	$+283    006CB811     D95D EC                    FSTP DWORD PTR SS:[EBP-0x14]
	$+286    006CB814     51                         PUSH ECX
	$+287    006CB815     8B0D B8607201              MOV ECX,DWORD PTR DS:[0x17260B8]
	$+28D    006CB81B     E8 A9067E01                CALL MapleSto.01EABEC9
	$+292    006CB820     D945 EC                    FLD DWORD PTR SS:[EBP-0x14]
	$+295    006CB823     8B0D 744E7201              MOV ECX,DWORD PTR DS:[0x1724E74]
	$+29B    006CB829     6A 00                      PUSH 0x0
	$+29D    006CB82B     6A 00                      PUSH 0x0
	$+29F    006CB82D     6A 01                      PUSH 0x1
	$+2A1    006CB82F     6A 01                      PUSH 0x1
	$+2A3    006CB831     83EC 08                    SUB ESP,0x8
	$+2A6    006CB834     D95C24 04                  FSTP DWORD PTR SS:[ESP+0x4]
	$+2AA    006CB838     8D55 F0                    LEA EDX,DWORD PTR SS:[EBP-0x10]
	$+2AD    006CB83B     D945 E8                    FLD DWORD PTR SS:[EBP-0x18]
	$+2B0    006CB83E     D91C24                     FSTP DWORD PTR SS:[ESP]
	$+2B3    006CB841     52                         PUSH EDX
	$+2B4    006CB842     E8 79AFF2FF                CALL MapleSto.005F67C0
	$+2B9    006CB847     8B4D F0                    MOV ECX,DWORD PTR SS:[EBP-0x10]
	$+2BC    006CB84A     C745 FC 00000000           MOV DWORD PTR SS:[EBP-0x4],0x0
	$+2C3    006CB851     85C9                       TEST ECX,ECX
	$+2C5    006CB853     0F84 09020000              JE MapleSto.006CBA62
	$+2CB    006CB859     8B41 1C                    MOV EAX,DWORD PTR DS:[ECX+0x1C]
	$+2CE    006CB85C     33F6                       XOR ESI,ESI
	$+2D0    006CB85E     8975 E4                    MOV DWORD PTR SS:[EBP-0x1C],ESI
	$+2D3    006CB861     3970 10                    CMP DWORD PTR DS:[EAX+0x10],ESI
	$+2D6    006CB864     0F86 D5010000              JBE MapleSto.006CBA3F
	$+2DC    006CB86A     8B40 04                    MOV EAX,DWORD PTR DS:[EAX+0x4]
	$+2DF    006CB86D     8B04B0                     MOV EAX,DWORD PTR DS:[EAX+ESI*4]
	$+2E2    006CB870     85C0                       TEST EAX,EAX
	$+2E4    006CB872     0F84 03010000              JE MapleSto.006CB97B
	$+2EA    006CB878     8B00                       MOV EAX,DWORD PTR DS:[EAX]
	$+2EC    006CB87A     50                         PUSH EAX
	$+2ED    006CB87B     68 CCC07301                PUSH MapleSto.0173C0CC
	$+2F2    006CB880     E8 1B67F1FF                CALL MapleSto.005E1FA0
	$+2F7    006CB885     83C4 08                    ADD ESP,0x8
	$+2FA    006CB888     85C0                       TEST EAX,EAX
	$+2FC    006CB88A     0F84 E8000000              JE MapleSto.006CB978
	$+302    006CB890     8B88 90000000              MOV ECX,DWORD PTR DS:[EAX+0x90]
	$+308    006CB896     F30F7E80 88000000          MOVQ XMM0,QWORD PTR DS:[EAX+0x88]
	$+310    006CB89E     8B13                       MOV EDX,DWORD PTR DS:[EBX]
	$+312    006CB8A0     8B52 50                    MOV EDX,DWORD PTR DS:[EDX+0x50]
	$+315    006CB8A3     8B3D 884E7201              MOV EDI,DWORD PTR DS:[0x1724E88]         ; g_c_map_information_service_
	$+31B    006CB8A9     660FD645 C0                MOVQ QWORD PTR SS:[EBP-0x40],XMM0
	$+320    006CB8AE     F3:0F104D C0               MOVSS XMM1,DWORD PTR SS:[EBP-0x40]
	$+325    006CB8B3     F3:0F1055 C4               MOVSS XMM2,DWORD PTR SS:[EBP-0x3C]
	$+32A    006CB8B8     894D C8                    MOV DWORD PTR SS:[EBP-0x38],ECX
	$+32D    006CB8BB     F3:0F1045 C8               MOVSS XMM0,DWORD PTR SS:[EBP-0x38]
	$+332    006CB8C0     F3:0F5805 C8034201         ADDSS XMM0,DWORD PTR DS:[0x14203C8]
	$+33A    006CB8C8     F3:0F1145 D8               MOVSS DWORD PTR SS:[EBP-0x28],XMM0
	$+33F    006CB8CD     F3:0F1145 D4               MOVSS DWORD PTR SS:[EBP-0x2C],XMM0
	$+344    006CB8D2     F30F7E80 88000000          MOVQ XMM0,QWORD PTR DS:[EAX+0x88]
	$+34C    006CB8DA     8D45 A8                    LEA EAX,DWORD PTR SS:[EBP-0x58]
	$+34F    006CB8DD     8BF1                       MOV ESI,ECX
	$+351    006CB8DF     50                         PUSH EAX
	$+352    006CB8E0     8BCB                       MOV ECX,EBX
	$+354    006CB8E2     F3:0F114D E0               MOVSS DWORD PTR SS:[EBP-0x20],XMM1
	$+359    006CB8E7     F3:0F1155 DC               MOVSS DWORD PTR SS:[EBP-0x24],XMM2
	$+35E    006CB8EC     F3:0F114D CC               MOVSS DWORD PTR SS:[EBP-0x34],XMM1
	$+363    006CB8F1     F3:0F1155 D0               MOVSS DWORD PTR SS:[EBP-0x30],XMM2
	$+368    006CB8F6     660FD645 B4                MOVQ QWORD PTR SS:[EBP-0x4C],XMM0
	$+36D    006CB8FB     FFD2                       CALL EDX
	$+36F    006CB8FD     D905 60EB4001              FLD DWORD PTR DS:[0x140EB60]
	$+375    006CB903     F30F7E05 70497201          MOVQ XMM0,QWORD PTR DS:[0x1724970]
	$+37D    006CB90B     8B15 78497201              MOV EDX,DWORD PTR DS:[0x1724978]
	$+383    006CB911     83EC 0C                    SUB ESP,0xC
	$+386    006CB914     8BCC                       MOV ECX,ESP
	$+388    006CB916     660FD601                   MOVQ QWORD PTR DS:[ECX],XMM0
	$+38C    006CB91A     F30F7E00                   MOVQ XMM0,QWORD PTR DS:[EAX]
	$+390    006CB91E     8B40 08                    MOV EAX,DWORD PTR DS:[EAX+0x8]
	$+393    006CB921     8951 08                    MOV DWORD PTR DS:[ECX+0x8],EDX
	$+396    006CB924     83EC 10                    SUB ESP,0x10
	$+399    006CB927     D95C24 0C                  FSTP DWORD PTR SS:[ESP+0xC]
	$+39D    006CB92B     8BCC                       MOV ECX,ESP
	$+39F    006CB92D     660FD601                   MOVQ QWORD PTR DS:[ECX],XMM0
	$+3A3    006CB931     F30F7E45 B4                MOVQ XMM0,QWORD PTR SS:[EBP-0x4C]
	$+3A8    006CB936     8941 08                    MOV DWORD PTR DS:[ECX+0x8],EAX
	$+3AB    006CB939     83EC 0C                    SUB ESP,0xC
	$+3AE    006CB93C     8BC4                       MOV EAX,ESP
	$+3B0    006CB93E     660FD600                   MOVQ QWORD PTR DS:[EAX],XMM0
	$+3B4    006CB942     8BCF                       MOV ECX,EDI
	$+3B6    006CB944     8970 08                    MOV DWORD PTR DS:[EAX+0x8],ESI
	$+3B9    006CB947     E8 54410B00                CALL MapleSto.0077FAA0
	$+3BE    006CB94C     85C0                       TEST EAX,EAX
	$+3C0    006CB94E     74 25                      JE SHORT MapleSto.006CB975
	$+3C2    006CB950     F30F7E45 CC                MOVQ XMM0,QWORD PTR SS:[EBP-0x34]
	$+3C7    006CB955     8B7D D4                    MOV EDI,DWORD PTR SS:[EBP-0x2C]
	$+3CA    006CB958     8B0D 884E7201              MOV ECX,DWORD PTR DS:[0x1724E88]
	$+3D0    006CB95E     83EC 0C                    SUB ESP,0xC
	$+3D3    006CB961     8BC4                       MOV EAX,ESP
	$+3D5    006CB963     660FD600                   MOVQ QWORD PTR DS:[EAX],XMM0
	$+3D9    006CB967     8978 08                    MOV DWORD PTR DS:[EAX+0x8],EDI
	$+3DC    006CB96A     E8 61A80B00                CALL MapleSto.007861D0
	$+3E1    006CB96F     8BF0                       MOV ESI,EAX
	$+3E3    006CB971     85F6                       TEST ESI,ESI
	$+3E5    006CB973     75 1B                      JNZ SHORT MapleSto.006CB990
	$+3E7    006CB975     8B75 E4                    MOV ESI,DWORD PTR SS:[EBP-0x1C]
	$+3EA    006CB978     8B4D F0                    MOV ECX,DWORD PTR SS:[EBP-0x10]
	$+3ED    006CB97B     8B41 1C                    MOV EAX,DWORD PTR DS:[ECX+0x1C]
	$+3F0    006CB97E     46                         INC ESI
	$+3F1    006CB97F     8975 E4                    MOV DWORD PTR SS:[EBP-0x1C],ESI
	$+3F4    006CB982     3B70 10                    CMP ESI,DWORD PTR DS:[EAX+0x10]
	$+3F7    006CB985   ^ 0F82 DFFEFFFF              JB MapleSto.006CB86A
	$+3FD    006CB98B     E9 AF000000                JMP MapleSto.006CBA3F
	$+402    006CB990     F3:0F1045 E0               MOVSS XMM0,DWORD PTR SS:[EBP-0x20]
	$+407    006CB995     F3:0F104D DC               MOVSS XMM1,DWORD PTR SS:[EBP-0x24]
	$+40C    006CB99A     F3:0F1055 D8               MOVSS XMM2,DWORD PTR SS:[EBP-0x28]
	$+411    006CB99F     F3:0F5815 74DE4001         ADDSS XMM2,DWORD PTR DS:[0x140DE74]
	$+419    006CB9A7     8B15 884E7201              MOV EDX,DWORD PTR DS:[0x1724E88]
	$+41F    006CB9AD     F3:0F1145 C0               MOVSS DWORD PTR SS:[EBP-0x40],XMM0
	$+424    006CB9B2     F3:0F114D C4               MOVSS DWORD PTR SS:[EBP-0x3C],XMM1
	$+429    006CB9B7     F30F7E45 C0                MOVQ XMM0,QWORD PTR SS:[EBP-0x40]
	$+42E    006CB9BC     83EC 0C                    SUB ESP,0xC
	$+431    006CB9BF     8BC4                       MOV EAX,ESP
	$+433    006CB9C1     660FD645 B4                MOVQ QWORD PTR SS:[EBP-0x4C],XMM0
	$+438    006CB9C6     F30F7E45 CC                MOVQ XMM0,QWORD PTR SS:[EBP-0x34]
	$+43D    006CB9CB     660FD600                   MOVQ QWORD PTR DS:[EAX],XMM0
	$+441    006CB9CF     8978 08                    MOV DWORD PTR DS:[EAX+0x8],EDI
	$+444    006CB9D2     F3:0F1155 C8               MOVSS DWORD PTR SS:[EBP-0x38],XMM2
	$+449    006CB9D7     8B4D C8                    MOV ECX,DWORD PTR SS:[EBP-0x38]
	$+44C    006CB9DA     8D45 A8                    LEA EAX,DWORD PTR SS:[EBP-0x58]
	$+44F    006CB9DD     50                         PUSH EAX
	$+450    006CB9DE     F3:0F1155 DC               MOVSS DWORD PTR SS:[EBP-0x24],XMM2
	$+455    006CB9E3     894D BC                    MOV DWORD PTR SS:[EBP-0x44],ECX
	$+458    006CB9E6     8955 D8                    MOV DWORD PTR SS:[EBP-0x28],EDX
	$+45B    006CB9E9     E8 5282EBFF                CALL MapleSto.00583C40
	$+460    006CB9EE     F30F7E00                   MOVQ XMM0,QWORD PTR DS:[EAX]
	$+464    006CB9F2     8B50 08                    MOV EDX,DWORD PTR DS:[EAX+0x8]
	$+467    006CB9F5     83C4 04                    ADD ESP,0x4
	$+46A    006CB9F8     8BCC                       MOV ECX,ESP
	$+46C    006CB9FA     660FD601                   MOVQ QWORD PTR DS:[ECX],XMM0
	$+470    006CB9FE     8951 08                    MOV DWORD PTR DS:[ECX+0x8],EDX
	$+473    006CBA01     8B4D D8                    MOV ECX,DWORD PTR SS:[EBP-0x28]
	$+476    006CBA04     E8 57BF0B00                CALL MapleSto.00787960
	$+47B    006CBA09     3BF0                       CMP ESI,EAX
	$+47D    006CBA0B     75 12                      JNZ SHORT MapleSto.006CBA1F
	$+47F    006CBA0D     F3:0F1045 DC               MOVSS XMM0,DWORD PTR SS:[EBP-0x24]
	$+484    006CBA12     F3:0F5805 88E34001         ADDSS XMM0,DWORD PTR DS:[0x140E388]
	$+48C    006CBA1A     F3:0F1145 BC               MOVSS DWORD PTR SS:[EBP-0x44],XMM0
	$+491    006CBA1F     F30F7E45 B4                MOVQ XMM0,QWORD PTR SS:[EBP-0x4C]
	$+496    006CBA24     8B4D BC                    MOV ECX,DWORD PTR SS:[EBP-0x44]
	$+499    006CBA27     8B13                       MOV EDX,DWORD PTR DS:[EBX]
	$+49B    006CBA29     8B52 4C                    MOV EDX,DWORD PTR DS:[EDX+0x4C]
	$+49E    006CBA2C     83EC 0C                    SUB ESP,0xC
	$+4A1    006CBA2F     8BC4                       MOV EAX,ESP
	$+4A3    006CBA31     660FD600                   MOVQ QWORD PTR DS:[EAX],XMM0
	$+4A7    006CBA35     8948 08                    MOV DWORD PTR DS:[EAX+0x8],ECX
	$+4AA    006CBA38     8BCB                       MOV ECX,EBX
	$+4AC    006CBA3A     FFD2                       CALL EDX
	$+4AE    006CBA3C     8B4D F0                    MOV ECX,DWORD PTR SS:[EBP-0x10]
	$+4B1    006CBA3F     C745 FC FFFFFFFF           MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+4B8    006CBA46     85C9                       TEST ECX,ECX
	$+4BA    006CBA48     74 18                      JE SHORT MapleSto.006CBA62
	$+4BC    006CBA4A     51                         PUSH ECX
	$+4BD    006CBA4B     8BF1                       MOV ESI,ECX
	$+4BF    006CBA4D     FF15 6C933E01              CALL DWORD PTR DS:[0x13E936C]
	$+4C5    006CBA53     85C0                       TEST EAX,EAX
	$+4C7    006CBA55     75 0B                      JNZ SHORT MapleSto.006CBA62
	$+4C9    006CBA57     6A 20                      PUSH 0x20
	$+4CB    006CBA59     56                         PUSH ESI
	$+4CC    006CBA5A     E8 616B7100                CALL MapleSto.00DE25C0
	$+4D1    006CBA5F     83C4 08                    ADD ESP,0x8
	$+4D4    006CBA62     8B4D F4                    MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+4D7    006CBA65     64:890D 00000000           MOV DWORD PTR FS:[0],ECX
	$+4DE    006CBA6C     59                         POP ECX
	$+4DF    006CBA6D     5F                         POP EDI
	$+4E0    006CBA6E     5E                         POP ESI
	$+4E1    006CBA6F     5B                         POP EBX
	$+4E2    006CBA70     8BE5                       MOV ESP,EBP
	$+4E4    006CBA72     5D                         POP EBP
	$+4E5    006CBA73     C3                         RETN
	$+4E6    006CBA74     CC                         INT3
	$+4E7    006CBA75     CC                         INT3
	$+4E8    006CBA76     CC                         INT3
	$+4E9    006CBA77     CC                         INT3
	$+4EA    006CBA78     CC                         INT3
	$+4EB    006CBA79     CC                         INT3
	$+4EC    006CBA7A     CC                         INT3
	$+4ED    006CBA7B     CC                         INT3
	$+4EE    006CBA7C     CC                         INT3
	$+4EF    006CBA7D     CC                         INT3
	$+4F0    006CBA7E     CC                         INT3
	$+4F1    006CBA7F     CC                         INT3
	$+4F2    006CBA80     55                         PUSH EBP
	$+4F3    006CBA81     8BEC                       MOV EBP,ESP
	$+4F5    006CBA83     6A FF                      PUSH -0x1
	$+4F7    006CBA85     68 3E5D3201                PUSH MapleSto.01325D3E
	$+4FC    006CBA8A     64:A1 00000000             MOV EAX,DWORD PTR FS:[0]
	$+502    006CBA90     50                         PUSH EAX
	$+503    006CBA91     83EC 08                    SUB ESP,0x8
	$+506    006CBA94     56                         PUSH ESI
	$+507    006CBA95     57                         PUSH EDI
	$+508    006CBA96     A1 D0687101                MOV EAX,DWORD PTR DS:[0x17168D0]
	$+50D    006CBA9B     33C5                       XOR EAX,EBP
	$+50F    006CBA9D     50                         PUSH EAX
	$+510    006CBA9E     8D45 F4                    LEA EAX,DWORD PTR SS:[EBP-0xC]
	$+513    006CBAA1     64:A3 00000000             MOV DWORD PTR FS:[0],EAX
	$+519    006CBAA7     8B0D 2C417201              MOV ECX,DWORD PTR DS:[0x172412C]
	$+51F    006CBAAD     E8 217F7D01                CALL MapleSto.01EA39D3
	$+524    006CBAB2     85C0                       TEST EAX,EAX
	$+526    006CBAB4     0F84 A8000000              JE MapleSto.006CBB62
	$+52C    006CBABA     8BC8                       MOV ECX,EAX
	$+52E    006CBABC     E8 51D67201                CALL MapleSto.01DF9112
	$+533    006CBAC1     8BF0                       MOV ESI,EAX
	$+535    006CBAC3     85F6                       TEST ESI,ESI
	$+537    006CBAC5     0F84 97000000              JE MapleSto.006CBB62
	$+53D    006CBACB     8BCE                       MOV ECX,ESI
	$+53F    006CBACD     E8 0FE26501                CALL MapleSto.01D29CE1
	$+544    006CBAD2     85C0                       TEST EAX,EAX
	$+546    006CBAD4     0F85 88000000              JNZ MapleSto.006CBB62
	$+54C    006CBADA     8B45 08                    MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+54F    006CBADD     8B0D A0897201              MOV ECX,DWORD PTR DS:[0x17289A0]
	$+555    006CBAE3     50                         PUSH EAX
	$+556    006CBAE4     E8 77120B00                CALL MapleSto.0077CD60
	$+55B    006CBAE9     8D4D EC                    LEA ECX,DWORD PTR SS:[EBP-0x14]
	$+55E    006CBAEC     51                         PUSH ECX
	$+55F    006CBAED     8B0D A0897201              MOV ECX,DWORD PTR DS:[0x17289A0]
	$+565    006CBAF3     E8 B80A0B00                CALL MapleSto.0077C5B0
	$+56A    006CBAF8     8B7D F0                    MOV EDI,DWORD PTR SS:[EBP-0x10]
	$+56D    006CBAFB     C745 FC 00000000           MOV DWORD PTR SS:[EBP-0x4],0x0
	$+574    006CBB02     8BCE                       MOV ECX,ESI
	$+576    006CBB04     85FF                       TEST EDI,EDI
	$+578    006CBB06     74 38                      JE SHORT MapleSto.006CBB40
	$+57A    006CBB08     E8 63980A00                CALL MapleSto.00775370
	$+57F    006CBB0D     3B47 10                    CMP EAX,DWORD PTR DS:[EDI+0x10]
	$+582    006CBB10     75 05                      JNZ SHORT MapleSto.006CBB17
	$+584    006CBB12     3B57 14                    CMP EDX,DWORD PTR DS:[EDI+0x14]
	$+587    006CBB15     74 0D                      JE SHORT MapleSto.006CBB24
	$+589    006CBB17     8B55 F0                    MOV EDX,DWORD PTR SS:[EBP-0x10]
	$+58C    006CBB1A     52                         PUSH EDX
	$+58D    006CBB1B     8BCE                       MOV ECX,ESI
	$+58F    006CBB1D     E8 8EA00A00                CALL MapleSto.00775BB0
	$+594    006CBB22     EB 2F                      JMP SHORT MapleSto.006CBB53
	$+596    006CBB24     8B7D F0                    MOV EDI,DWORD PTR SS:[EBP-0x10]
	$+599    006CBB27     8BCE                       MOV ECX,ESI
	$+59B    006CBB29     E8 47DD0A00                CALL MapleSto.00779875
	$+5A0    006CBB2E     3B47 0C                    CMP EAX,DWORD PTR DS:[EDI+0xC]
	$+5A3    006CBB31     74 20                      JE SHORT MapleSto.006CBB53
	$+5A5    006CBB33     8B45 F0                    MOV EAX,DWORD PTR SS:[EBP-0x10]
	$+5A8    006CBB36     50                         PUSH EAX
	$+5A9    006CBB37     8BCE                       MOV ECX,ESI
	$+5AB    006CBB39     E8 72A00A00                CALL MapleSto.00775BB0
	$+5B0    006CBB3E     EB 13                      JMP SHORT MapleSto.006CBB53
	$+5B2    006CBB40     E8 752C6601                CALL MapleSto.01D2E7BA
	$+5B7    006CBB45     83F8 01                    CMP EAX,0x1
	$+5BA    006CBB48     74 09                      JE SHORT MapleSto.006CBB53
	$+5BC    006CBB4A     6A 01                      PUSH 0x1
	$+5BE    006CBB4C     8BCE                       MOV ECX,ESI
	$+5C0    006CBB4E     E8 5FE56501                CALL MapleSto.01D2A0B2
	$+5C5    006CBB53     8D4D EC                    LEA ECX,DWORD PTR SS:[EBP-0x14]
	$+5C8    006CBB56     C745 FC FFFFFFFF           MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+5CF    006CBB5D     E8 AE153B00                CALL MapleSto.00A7D110
	$+5D4    006CBB62     8B4D F4                    MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+5D7    006CBB65     64:890D 00000000           MOV DWORD PTR FS:[0],ECX
	$+5DE    006CBB6C     59                         POP ECX
	$+5DF    006CBB6D     5F                         POP EDI
	$+5E0    006CBB6E     5E                         POP ESI
	$+5E1    006CBB6F     8BE5                       MOV ESP,EBP
	$+5E3    006CBB71     5D                         POP EBP
	$+5E4    006CBB72     C3                         RETN
	$+5E5    006CBB73     CC                         INT3
	$+5E6    006CBB74     CC                         INT3
	$+5E7    006CBB75     CC                         INT3
	$+5E8    006CBB76     CC                         INT3
	$+5E9    006CBB77     CC                         INT3
	$+5EA    006CBB78     CC                         INT3
	$+5EB    006CBB79     CC                         INT3
	$+5EC    006CBB7A     CC                         INT3
	$+5ED    006CBB7B     CC                         INT3
	$+5EE    006CBB7C     CC                         INT3
	$+5EF    006CBB7D     CC                         INT3
	$+5F0    006CBB7E     CC                         INT3
	$+5F1    006CBB7F     CC                         INT3
	$+5F2    006CBB80     55                         PUSH EBP
	$+5F3    006CBB81     8BEC                       MOV EBP,ESP
	$+5F5    006CBB83     6A FF                      PUSH -0x1
	$+5F7    006CBB85     68 615D3201                PUSH MapleSto.01325D61
	$+5FC    006CBB8A     64:A1 00000000             MOV EAX,DWORD PTR FS:[0]
	$+602    006CBB90     50                         PUSH EAX
	$+603    006CBB91     83EC 08                    SUB ESP,0x8
	$+606    006CBB94     56                         PUSH ESI
	$+607    006CBB95     57                         PUSH EDI
	$+608    006CBB96     A1 D0687101                MOV EAX,DWORD PTR DS:[0x17168D0]
	$+60D    006CBB9B     33C5                       XOR EAX,EBP
	$+60F    006CBB9D     50                         PUSH EAX
	$+610    006CBB9E     8D45 F4                    LEA EAX,DWORD PTR SS:[EBP-0xC]
	$+613    006CBBA1     64:A3 00000000             MOV DWORD PTR FS:[0],EAX
	$+619    006CBBA7     8B0D 2C417201              MOV ECX,DWORD PTR DS:[0x172412C]
	$+61F    006CBBAD     E8 217E7D01                CALL MapleSto.01EA39D3
	$+624    006CBBB2     85C0                       TEST EAX,EAX
	$+626    006CBBB4     0F84 A8000000              JE MapleSto.006CBC62
	$+62C    006CBBBA     8BC8                       MOV ECX,EAX
	$+62E    006CBBBC     E8 51D57201                CALL MapleSto.01DF9112
	$+633    006CBBC1     8BF0                       MOV ESI,EAX
	$+635    006CBBC3     85F6                       TEST ESI,ESI
	$+637    006CBBC5     0F84 97000000              JE MapleSto.006CBC62
	$+63D    006CBBCB     8BCE                       MOV ECX,ESI
	$+63F    006CBBCD     E8 0FE16501                CALL MapleSto.01D29CE1
	$+644    006CBBD2     85C0                       TEST EAX,EAX
	$+646    006CBBD4     0F85 88000000              JNZ MapleSto.006CBC62
	$+64C    006CBBDA     8B45 08                    MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+64F    006CBBDD     8B0D A0897201              MOV ECX,DWORD PTR DS:[0x17289A0]
	$+655    006CBBE3     50                         PUSH EAX
	$+656    006CBBE4     E8 B7120B00                CALL MapleSto.0077CEA0
	$+65B    006CBBE9     8D4D EC                    LEA ECX,DWORD PTR SS:[EBP-0x14]
	$+65E    006CBBEC     51                         PUSH ECX
	$+65F    006CBBED     8B0D A0897201              MOV ECX,DWORD PTR DS:[0x17289A0]
	$+665    006CBBF3     E8 B8090B00                CALL MapleSto.0077C5B0
	$+66A    006CBBF8     8B7D F0                    MOV EDI,DWORD PTR SS:[EBP-0x10]
	$+66D    006CBBFB     C745 FC 00000000           MOV DWORD PTR SS:[EBP-0x4],0x0
	$+674    006CBC02     8BCE                       MOV ECX,ESI
	$+676    006CBC04     85FF                       TEST EDI,EDI
	$+678    006CBC06     74 38                      JE SHORT MapleSto.006CBC40
	$+67A    006CBC08     E8 63970A00                CALL MapleSto.00775370
	$+67F    006CBC0D     3B47 10                    CMP EAX,DWORD PTR DS:[EDI+0x10]
	$+682    006CBC10     75 05                      JNZ SHORT MapleSto.006CBC17
	$+684    006CBC12     3B57 14                    CMP EDX,DWORD PTR DS:[EDI+0x14]
	$+687    006CBC15     74 0D                      JE SHORT MapleSto.006CBC24
	$+689    006CBC17     8B55 F0                    MOV EDX,DWORD PTR SS:[EBP-0x10]
	$+68C    006CBC1A     52                         PUSH EDX
	$+68D    006CBC1B     8BCE                       MOV ECX,ESI
	$+68F    006CBC1D     E8 8E9F0A00                CALL MapleSto.00775BB0
	$+694    006CBC22     EB 2F                      JMP SHORT MapleSto.006CBC53
	$+696    006CBC24     8B7D F0                    MOV EDI,DWORD PTR SS:[EBP-0x10]
	$+699    006CBC27     8BCE                       MOV ECX,ESI
	$+69B    006CBC29     E8 47DC0A00                CALL MapleSto.00779875
	$+6A0    006CBC2E     3B47 0C                    CMP EAX,DWORD PTR DS:[EDI+0xC]
	$+6A3    006CBC31     74 20                      JE SHORT MapleSto.006CBC53
	$+6A5    006CBC33     8B45 F0                    MOV EAX,DWORD PTR SS:[EBP-0x10]
	$+6A8    006CBC36     50                         PUSH EAX
	$+6A9    006CBC37     8BCE                       MOV ECX,ESI
	$+6AB    006CBC39     E8 729F0A00                CALL MapleSto.00775BB0
	$+6B0    006CBC3E     EB 13                      JMP SHORT MapleSto.006CBC53
	$+6B2    006CBC40     E8 752B6601                CALL MapleSto.01D2E7BA
	$+6B7    006CBC45     83F8 01                    CMP EAX,0x1
	$+6BA    006CBC48     74 09                      JE SHORT MapleSto.006CBC53
	$+6BC    006CBC4A     6A 01                      PUSH 0x1
	$+6BE    006CBC4C     8BCE                       MOV ECX,ESI
	$+6C0    006CBC4E     E8 5FE46501                CALL MapleSto.01D2A0B2
	$+6C5    006CBC53     8D4D EC                    LEA ECX,DWORD PTR SS:[EBP-0x14]
	$+6C8    006CBC56     C745 FC FFFFFFFF           MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+6CF    006CBC5D     E8 AE143B00                CALL MapleSto.00A7D110
	$+6D4    006CBC62     8B4D F4                    MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+6D7    006CBC65     64:890D 00000000           MOV DWORD PTR FS:[0],ECX
	$+6DE    006CBC6C     59                         POP ECX
	$+6DF    006CBC6D     5F                         POP EDI
	$+6E0    006CBC6E     5E                         POP ESI
	$+6E1    006CBC6F     8BE5                       MOV ESP,EBP
	$+6E3    006CBC71     5D                         POP EBP
	$+6E4    006CBC72     C3                         RETN
	$+6E5    006CBC73     CC                         INT3
	*/
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("8B138B52508B3D????????660FD645C0F30F104DC0",
		5 + 2);
	if (!op_dw_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *op_dw_tmp;
	CheckExeAddr(dw_tmp);
	ofs << "pt_cdword	g_c_map_information_service_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;
	return true;
}

bool CAutoUpdateApp::Scan_Skill(Cmn_FC_OFstream& ofs)
{
	//pt_pcvoid addr_tmp, addr_tmp1;
	pt_dword dw_tmp;
	boost::optional<pt_dword> op_dw_tmp;

	/*
	$-27E    00931A08     F4                       HLT
	$-27D    00931A09     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$-276    00931A10     53                       PUSH EBX
	$-275    00931A11     C7C1 4DF53C3F            MOV ECX,0x3F3CF54D
	$-26F    00931A17     8D89 033B22F9            LEA ECX,DWORD PTR DS:[ECX+0xF9223B03]
	$-269    00931A1D     51                       PUSH ECX
	$-268    00931A1E     8DA424 F6FFFFFF          LEA ESP,DWORD PTR SS:[ESP-0xA]
	$-261    00931A25     8B8C24 12000000          MOV ECX,DWORD PTR SS:[ESP+0x12]
	$-25A    00931A2C     8DA424 16000000          LEA ESP,DWORD PTR SS:[ESP+0x16]
	$-253    00931A33     C7C6 EE866249            MOV ESI,0x496286EE
	$-24D    00931A39     8DB6 829C9614            LEA ESI,DWORD PTR DS:[ESI+0x14969C82]
	$-247    00931A3F     56                       PUSH ESI
	$-246    00931A40     8BBC24 04000000          MOV EDI,DWORD PTR SS:[ESP+0x4]
	$-23F    00931A47     8DA424 08000000          LEA ESP,DWORD PTR SS:[ESP+0x8]
	$-238    00931A4E     8B3424                   MOV ESI,DWORD PTR SS:[ESP]
	$-235    00931A51     8DA424 04000000          LEA ESP,DWORD PTR SS:[ESP+0x4]
	$-22E    00931A58     52                       PUSH EDX
	$-22D    00931A59     E8 01000000              CALL MapleSto.00931A5F
	$-228    00931A5E     70 5A                    JO SHORT MapleSto.00931ABA
	$-226    00931A60     8D92 30AC4C01            LEA EDX,DWORD PTR DS:[EDX+0x14CAC30]
	$-220    00931A66     FFE2                     JMP EDX
	$-21E    00931A68   ^ 7F 8D                    JG SHORT MapleSto.009319F7
	$-21C    00931A6A     8927                     MOV DWORD PTR DS:[EDI],ESP
	$-21A    00931A6C     14 FB                    ADC AL,0xFB
	$-218    00931A6E     BA 8BC98BCF              MOV EDX,0xCF8BC98B
	$-213    00931A73     5F                       POP EDI
	$-212    00931A74     E8 27795A00              CALL MapleSto.00ED93A0
	$-20D    00931A79   - E9 402E4C01              JMP MapleSto.01DF48BE
	$-208    00931A7E     73 58                    JNB SHORT MapleSto.00931AD8
	$-206    00931A80     58                       POP EAX
	$-205    00931A81     85C0                     TEST EAX,EAX
	$-203    00931A83     0F85 E1980000            JNZ MapleSto.0093B36A
	$-1FD    00931A89   - E9 77364C01              JMP MapleSto.01DF5105
	$-1F8    00931A8E   ^ 78 8D                    JS SHORT MapleSto.00931A1D
	$-1F6    00931A90     A4                       MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
	$-1F5    00931A91     24 04                    AND AL,0x4
	$-1F3    00931A93     0000                     ADD BYTE PTR DS:[EAX],AL
	$-1F1    00931A95     000F                     ADD BYTE PTR DS:[EDI],CL
	$-1EF    00931A97     84B3 C14C01E9            TEST BYTE PTR DS:[EBX+0xE9014CC1],DH
	$-1E9    00931A9D     EC                       IN AL,DX
	$-1E8    00931A9E     804C01 E0 8D             OR BYTE PTR DS:[ECX+EAX-0x20],0x8D
	$-1E3    00931AA3     A4                       MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
	$-1E2    00931AA4     24 FC                    AND AL,0xFC
	$-1E0    00931AA6     FF                       DB FF
	$-1DF    00931AA7     FF                       DB FF
	$-1DE    00931AA8     FF8D A424FCFF            DEC DWORD PTR SS:[EBP+0xFFFC24A4]
	$-1D8    00931AAE     FF                       DB FF
	$-1D7    00931AAF     FF8D A424F4FF            DEC DWORD PTR SS:[EBP+0xFFF424A4]
	$-1D1    00931AB5     FF                       DB FF
	$-1D0    00931AB6     FF8B B4241400            DEC DWORD PTR DS:[EBX+0x1424B4]
	$-1CA    00931ABC     0000                     ADD BYTE PTR DS:[EAX],AL
	$-1C8    00931ABE     8DA424 18000000          LEA ESP,DWORD PTR SS:[ESP+0x18]
	$-1C1    00931AC5     50                       PUSH EAX
	$-1C0    00931AC6     8B8424 04000000          MOV EAX,DWORD PTR SS:[ESP+0x4]
	$-1B9    00931ACD     55                       PUSH EBP
	$-1B8    00931ACE     8DA424 FCFFFFFF          LEA ESP,DWORD PTR SS:[ESP-0x4]
	$-1B1    00931AD5     8DA424 FCFFFFFF          LEA ESP,DWORD PTR SS:[ESP-0x4]
	$-1AA    00931ADC     56                       PUSH ESI
	$-1A9    00931ADD     53                       PUSH EBX
	$-1A8    00931ADE     50                       PUSH EAX
	$-1A7    00931ADF     C7C0 5FB5E051            MOV EAX,0x51E0B55F
	$-1A1    00931AE5     8D80 79757E00            LEA EAX,DWORD PTR DS:[EAX+0x7E7579]
	$-19B    00931AEB     870424                   XCHG DWORD PTR SS:[ESP],EAX
	$-198    00931AEE     52                       PUSH EDX
	$-197    00931AEF     50                       PUSH EAX
	$-196    00931AF0     8B8424 20000000          MOV EAX,DWORD PTR SS:[ESP+0x20]
	$-18F    00931AF7     9C                       PUSHFD
	$-18E    00931AF8     E8 01000000              CALL MapleSto.00931AFE
	$-189    00931AFD   ^ 72 81                    JB SHORT MapleSto.00931A80
	$-187    00931AFF     04 24                    ADD AL,0x24
	$-185    00931B01     9E                       SAHF
	$-184    00931B02     A4                       MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
	$-183    00931B03     C0FF C3                  SAR BH,0xC3
	$-180    00931B06   ^ 75 C7                    JNZ SHORT MapleSto.00931ACF
	$-17E    00931B08     C160 EA 92               SHL DWORD PTR DS:[EAX-0x16],0x92
	$-17A    00931B0C     638D 892C64DF            ARPL WORD PTR SS:[EBP+0xDF642C89],CX
	$-174    00931B12     9D                       POPFD
	$-173    00931B13     8B09                     MOV ECX,DWORD PTR DS:[ECX]
	$-171    00931B15     9C                       PUSHFD
	$-170    00931B16     870424                   XCHG DWORD PTR SS:[ESP],EAX
	$-16D    00931B19     8B0424                   MOV EAX,DWORD PTR SS:[ESP]
	$-16A    00931B1C     E8 FF25E9FF              CALL MapleSto.007C4120
	$-165    00931B21     50                       PUSH EAX
	$-164    00931B22     5B                       POP EBX
	$-163    00931B23     85DB                     TEST EBX,EBX
	$-161    00931B25   - 0F84 791A4C01            JE MapleSto.01DF35A4
	$-15B    00931B2B     51                       PUSH ECX
	$-15A    00931B2C     E8 01000000              CALL MapleSto.00931B32
	$-155    00931B31     78 59                    JS SHORT MapleSto.00931B8C
	$-153    00931B33     8D89 17010000            LEA ECX,DWORD PTR DS:[ECX+0x117]
	$-14D    00931B39     FFE1                     JMP ECX
	$-14B    00931B3B     77 59                    JA SHORT MapleSto.00931B96
	$-149    00931B3D     53                       PUSH EBX
	$-148    00931B3E     8BD8                     MOV EBX,EAX
	$-146    00931B40     C7C1 6A7E4F24            MOV ECX,0x244F7E6A
	$-140    00931B46     8BCB                     MOV ECX,EBX
	$-13E    00931B48     5B                       POP EBX
	$-13D    00931B49     9C                       PUSHFD
	$-13C    00931B4A     E8 01000000              CALL MapleSto.00931B50
	$-137    00931B4F   ^ E3 81                    JECXZ SHORT MapleSto.00931AD2
	$-135    00931B51     04 24                    ADD AL,0x24
	$-133    00931B53     E4 29                    IN AL,0x29
	$-131    00931B55     4C                       DEC ESP
	$-130    00931B56     01C3                     ADD EBX,EAX
	$-12E    00931B58   ^ 79 8B                    JNS SHORT MapleSto.00931AE5
	$-12C    00931B5A     B9 D0010000              MOV ECX,0x1D0
	$-127    00931B5F     8B81 BC010000            MOV EAX,DWORD PTR DS:[ECX+0x1BC]
	$-121    00931B65     85F6                     TEST ESI,ESI
	$-11F    00931B67     E8 686D4C01              CALL MapleSto.01DF88D4
	$-11A    00931B6C   ^ 77 8D                    JA SHORT MapleSto.00931AFB
	$-118    00931B6E     64:24 04                 AND AL,0x4
	$-115    00931B71     8DA424 04000000          LEA ESP,DWORD PTR SS:[ESP+0x4]
	$-10E    00931B78     8BE5                     MOV ESP,EBP
	$-10C    00931B7A     50                       PUSH EAX
	$-10B    00931B7B     87AC24 04000000          XCHG DWORD PTR SS:[ESP+0x4],EBP
	$-104    00931B82     8DA424 08000000          LEA ESP,DWORD PTR SS:[ESP+0x8]
	$-FD     00931B89     50                       PUSH EAX
	$-FC     00931B8A     8B8424 04000000          MOV EAX,DWORD PTR SS:[ESP+0x4]
	$-F5     00931B91     9C                       PUSHFD
	$-F4     00931B92     51                       PUSH ECX
	$-F3     00931B93     9C                       PUSHFD
	$-F2     00931B94     8DA424 FCFFFFFF          LEA ESP,DWORD PTR SS:[ESP-0x4]
	$-EB     00931B9B     9C                       PUSHFD
	$-EA     00931B9C     68 06F72720              PUSH 0x2027F706
	$-E5     00931BA1     52                       PUSH EDX
	$-E4     00931BA2     E8 01000000              CALL MapleSto.00931BA8
	$-DF     00931BA7     73 5A                    JNB SHORT MapleSto.00931C03
	$-DD     00931BA9     8D92 7E2D4C01            LEA EDX,DWORD PTR DS:[EDX+0x14C2D7E]
	$-D7     00931BAF     FFE2                     JMP EDX
	$-D5     00931BB1   ^ 7E 8D                    JLE SHORT MapleSto.00931B40
	$-D3     00931BB3     64:24 04                 AND AL,0x4
	$-D0     00931BB6     59                       POP ECX
	$-CF     00931BB7     FF15 6C933E01            CALL DWORD PTR DS:[0x13E936C]
	$-C9     00931BBD     85C0                     TEST EAX,EAX
	$-C7     00931BBF     0F85 8B1C0000            JNZ MapleSto.00933850
	$-C1     00931BC5   - E9 B1434C01              JMP MapleSto.01DF5F7B
	$-BC     00931BCA     77 59                    JA SHORT MapleSto.00931C25
	$-BA     00931BCC     8B75 08                  MOV ESI,DWORD PTR SS:[EBP+0x8]
	$-B7     00931BCF     9C                       PUSHFD
	$-B6     00931BD0     9C                       PUSHFD
	$-B5     00931BD1     89B424 04000000          MOV DWORD PTR SS:[ESP+0x4],ESI
	$-AE     00931BD8     9D                       POPFD
	$-AD     00931BD9     50                       PUSH EAX
	$-AC     00931BDA     59                       POP ECX
	$-AB     00931BDB     E8 90431A00              CALL MapleSto.00AD5F70
	$-A6     00931BE0     56                       PUSH ESI
	$-A5     00931BE1     58                       POP EAX
	$-A4     00931BE2     9C                       PUSHFD
	$-A3     00931BE3     E8 01000000              CALL MapleSto.00931BE9
	$-9E     00931BE8     E8 8104246A              CALL 6AB7206E
	$-99     00931BED     5E                       POP ESI
	$-98     00931BEE     4C                       DEC ESP
	$-97     00931BEF     01C3                     ADD EBX,EAX
	$-95     00931BF1     75 5A                    JNZ SHORT MapleSto.00931C4D
	$-93     00931BF3     5A                       POP EDX
	$-92     00931BF4     85FF                     TEST EDI,EDI
	$-90     00931BF6   ^ 0F84 B8F2FFFF            JE MapleSto.00930EB4
	$-8A     00931BFC   ^ E9 99DBFFFF              JMP MapleSto.0092F79A
	$-85     00931C01   ^ E3 8B                    JECXZ SHORT MapleSto.00931B8E
	$-83     00931C03     4D                       DEC EBP
	$-82     00931C04     EC                       IN AL,DX
	$-81     00931C05     8B55 DC                  MOV EDX,DWORD PTR SS:[EBP-0x24]
	$-7E     00931C08     8A45 08                  MOV AL,BYTE PTR SS:[EBP+0x8]
	$-7B     00931C0B     88040A                   MOV BYTE PTR DS:[EDX+ECX],AL
	$-78     00931C0E     FF45 EC                  INC DWORD PTR SS:[EBP-0x14]
	$-75     00931C11     8D4D D4                  LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$-72     00931C14     E8 D8323201              CALL MapleSto.01C54EF1
	$-6D     00931C19     8B45 DC                  MOV EAX,DWORD PTR SS:[EBP-0x24]
	$-6A     00931C1C     C745 D4 BC504101         MOV DWORD PTR SS:[EBP-0x2C],MapleSto.014150BC
	$-63     00931C23     85C0                     TEST EAX,EAX
	$-61     00931C25     0F84 A5630000            JE MapleSto.00937FD0
	$-5B     00931C2B     E8 F0764C01              CALL MapleSto.01DF9320
	$-56     00931C30   ^ 75 8B                    JNZ SHORT MapleSto.00931BBD
	$-54     00931C32     4E                       DEC ESI
	$-53     00931C33     182B                     SBB BYTE PTR DS:[EBX],CH
	$-51     00931C35     4E                       DEC ESI
	$-50     00931C36     14 8B                    ADC AL,0x8B
	$-4E     00931C38     8680 000000C1            XCHG BYTE PTR DS:[EAX+0xC1000000],AL
	$-48     00931C3E     F9                       STC
	$-47     00931C3F     0257 50                  ADD DL,BYTE PTR DS:[EDI+0x50]
	$-44     00931C42   - E9 52C64C01              JMP MapleSto.01DFE299
	$-3F     00931C47     76 59                    JBE SHORT MapleSto.00931CA2
	$-3D     00931C49   - E9 8AD14C01              JMP MapleSto.01DFEDD8
	$-38     00931C4E   ^ 76 CC                    JBE SHORT MapleSto.00931C1C
	$-36     00931C50     55                       PUSH EBP
	$-35     00931C51     8BEC                     MOV EBP,ESP
	$-33     00931C53     6A FF                    PUSH -0x1
	$-31     00931C55     68 D0283601              PUSH MapleSto.013628D0
	$-2C     00931C5A     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$-26     00931C60     50                       PUSH EAX
	$-25     00931C61     83EC 2C                  SUB ESP,0x2C
	$-22     00931C64     53                       PUSH EBX
	$-21     00931C65     56                       PUSH ESI
	$-20     00931C66     57                       PUSH EDI
	$-1F     00931C67     A1 D0687101              MOV EAX,DWORD PTR DS:[0x17168D0]
	$-1A     00931C6C     33C5                     XOR EAX,EBP
	$-18     00931C6E     50                       PUSH EAX
	$-17     00931C6F     8D45 F4                  LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-14     00931C72     64:A3 00000000           MOV DWORD PTR FS:[0],EAX
	$-E      00931C78     8BF1                     MOV ESI,ECX
	$-C      00931C7A     8975 EC                  MOV DWORD PTR SS:[EBP-0x14],ESI
	$-9      00931C7D     33DB                     XOR EBX,EBX
	$-7      00931C7F     895D F0                  MOV DWORD PTR SS:[EBP-0x10],EBX
	$-4      00931C82     8B7D 0C                  MOV EDI,DWORD PTR SS:[EBP+0xC]
	$-1      00931C85     57                       PUSH EDI
	$ ==>    00931C86     8D8E 40170000            LEA ECX,DWORD PTR DS:[ESI+0x1740]                 ; g_o_skill_mgr_
	$+6      00931C8C     E8 B3C92F01              CALL MapleSto.01C2E644
	$+B      00931C91     85C0                     TEST EAX,EAX
	$+D      00931C93     0F85 91000000            JNZ MapleSto.00931D2A
	$+13     00931C99     68 0C254601              PUSH MapleSto.0146250C                            ; UNICODE "s_err_not_learn_skill"
	$+18     00931C9E     8D4D 1C                  LEA ECX,DWORD PTR SS:[EBP+0x1C]
	$+1B     00931CA1     FF15 D0A13E01            CALL DWORD PTR DS:[0x13EA1D0]                     ; mfc90.#286
	$+21     00931CA7     8D45 1C                  LEA EAX,DWORD PTR SS:[EBP+0x1C]
	$+24     00931CAA     50                       PUSH EAX
	$+25     00931CAB     8D4D 10                  LEA ECX,DWORD PTR SS:[EBP+0x10]
	$+28     00931CAE     51                       PUSH ECX
	$+29     00931CAF     C745 FC 01000000         MOV DWORD PTR SS:[EBP-0x4],0x1
	$+30     00931CB6     E8 0558B1FF              CALL MapleSto.004474C0
	$+35     00931CBB     6A 01                    PUSH 0x1
	$+37     00931CBD     6A 01                    PUSH 0x1
	$+39     00931CBF     50                       PUSH EAX
	$+3A     00931CC0     C645 FC 02               MOV BYTE PTR SS:[EBP-0x4],0x2
	$+3E     00931CC4     E8 B72AEDFF              CALL MapleSto.00804780
	$+43     00931CC9     83C4 14                  ADD ESP,0x14
	$+46     00931CCC     8D4D 10                  LEA ECX,DWORD PTR SS:[EBP+0x10]
	$+49     00931CCF     FF15 DCA13E01            CALL DWORD PTR DS:[0x13EA1DC]                     ; mfc90.#601
	$+4F     00931CD5     8D4D 1C                  LEA ECX,DWORD PTR SS:[EBP+0x1C]
	$+52     00931CD8     885D FC                  MOV BYTE PTR SS:[EBP-0x4],BL
	$+55     00931CDB     FF15 DCA13E01            CALL DWORD PTR DS:[0x13EA1DC]                     ; mfc90.#601
	$+5B     00931CE1     68 E02D3F01              PUSH MapleSto.013F2DE0
	$+60     00931CE6     8D4D 18                  LEA ECX,DWORD PTR SS:[EBP+0x18]
	$+63     00931CE9     FF15 20A23E01            CALL DWORD PTR DS:[0x13EA220]                     ; mfc90.#310
	$+69     00931CEF     8D55 18                  LEA EDX,DWORD PTR SS:[EBP+0x18]
	$+6C     00931CF2     52                       PUSH EDX
	$+6D     00931CF3     68 142C7301              PUSH MapleSto.01732C14
	$+72     00931CF8     C745 FC 03000000         MOV DWORD PTR SS:[EBP-0x4],0x3
	$+79     00931CFF     E8 6C11EDFF              CALL MapleSto.00802E70
	$+7E     00931D04     83C4 08                  ADD ESP,0x8
	$+81     00931D07     8D4D 18                  LEA ECX,DWORD PTR SS:[EBP+0x18]
	$+84     00931D0A     FF15 08A23E01            CALL DWORD PTR DS:[0x13EA208]                     ; mfc90.#601
	$+8A     00931D10     8B45 08                  MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+8D     00931D13     8958 04                  MOV DWORD PTR DS:[EAX+0x4],EBX
	$+90     00931D16     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+93     00931D19     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+9A     00931D20     59                       POP ECX
	$+9B     00931D21     5F                       POP EDI
	$+9C     00931D22     5E                       POP ESI
	$+9D     00931D23     5B                       POP EBX
	$+9E     00931D24     8BE5                     MOV ESP,EBP
	$+A0     00931D26     5D                       POP EBP
	$+A1     00931D27     C2 1800                  RETN 0x18
	$+A4     00931D2A     8B45 1C                  MOV EAX,DWORD PTR SS:[EBP+0x1C]
	$+A7     00931D2D     8B4D 18                  MOV ECX,DWORD PTR SS:[EBP+0x18]
	$+AA     00931D30     8B5D 14                  MOV EBX,DWORD PTR SS:[EBP+0x14]
	$+AD     00931D33     8B55 10                  MOV EDX,DWORD PTR SS:[EBP+0x10]
	$+B0     00931D36     50                       PUSH EAX
	$+B1     00931D37     51                       PUSH ECX
	$+B2     00931D38     53                       PUSH EBX
	$+B3     00931D39     52                       PUSH EDX
	$+B4     00931D3A     57                       PUSH EDI
	$+B5     00931D3B     8D45 D8                  LEA EAX,DWORD PTR SS:[EBP-0x28]
	$+B8     00931D3E     81C6 4C190000            ADD ESI,0x194C
	$+BE     00931D44     50                       PUSH EAX
	$+BF     00931D45     8BCE                     MOV ECX,ESI
	$+C1     00931D47     E8 E4D90A00              CALL MapleSto.009DF730
	$+C6     00931D4C     8B45 DC                  MOV EAX,DWORD PTR SS:[EBP-0x24]
	$+C9     00931D4F     C745 FC 04000000         MOV DWORD PTR SS:[EBP-0x4],0x4
	$+D0     00931D56     85C0                     TEST EAX,EAX
	$+D2     00931D58     0F84 1A010000            JE MapleSto.00931E78
	$+D8     00931D5E     C780 C0000000 00000000   MOV DWORD PTR DS:[EAX+0xC0],0x0
	$+E2     00931D68     8B55 DC                  MOV EDX,DWORD PTR SS:[EBP-0x24]
	$+E5     00931D6B     8B45 1C                  MOV EAX,DWORD PTR SS:[EBP+0x1C]
	$+E8     00931D6E     33C9                     XOR ECX,ECX
	$+EA     00931D70     66:898A C4000000         MOV WORD PTR DS:[EDX+0xC4],CX
	$+F1     00931D77     8B0D 74517201            MOV ECX,DWORD PTR DS:[0x1725174]
	$+F7     00931D7D     50                       PUSH EAX
	$+F8     00931D7E     E8 8DB9F9FF              CALL MapleSto.008CD710
	$+FD     00931D83     8BFA                     MOV EDI,EDX
	$+FF     00931D85     57                       PUSH EDI
	$+100    00931D86     8BF0                     MOV ESI,EAX
	$+102    00931D88     56                       PUSH ESI
	$+103    00931D89     8D4D D0                  LEA ECX,DWORD PTR SS:[EBP-0x30]
	$+106    00931D8C     51                       PUSH ECX
	$+107    00931D8D     B9 604B7201              MOV ECX,MapleSto.01724B60
	$+10C    00931D92     E8 2935B8FF              CALL MapleSto.004B52C0
	$+111    00931D97     33DB                     XOR EBX,EBX
	$+113    00931D99     C645 FC 05               MOV BYTE PTR SS:[EBP-0x4],0x5
	$+117    00931D9D     395D D4                  CMP DWORD PTR SS:[EBP-0x2C],EBX
	$+11A    00931DA0     75 27                    JNZ SHORT MapleSto.00931DC9
	$+11C    00931DA2     8BD6                     MOV EDX,ESI
	$+11E    00931DA4     0BD7                     OR EDX,EDI
	$+120    00931DA6     74 21                    JE SHORT MapleSto.00931DC9
	$+122    00931DA8     8B75 08                  MOV ESI,DWORD PTR SS:[EBP+0x8]
	$+125    00931DAB     8D4D D0                  LEA ECX,DWORD PTR SS:[EBP-0x30]
	$+128    00931DAE     895E 04                  MOV DWORD PTR DS:[ESI+0x4],EBX
	$+12B    00931DB1     C745 F0 01000000         MOV DWORD PTR SS:[EBP-0x10],0x1
	$+132    00931DB8     C645 FC 04               MOV BYTE PTR SS:[EBP-0x4],0x4
	$+136    00931DBC     E8 4FB31400              CALL MapleSto.00A7D110
	$+13B    00931DC1     885D FC                  MOV BYTE PTR SS:[EBP-0x4],BL
	$+13E    00931DC4     E9 13030000              JMP MapleSto.009320DC
	$+143    00931DC9     8B45 DC                  MOV EAX,DWORD PTR SS:[EBP-0x24]
	$+146    00931DCC     89B0 C8000000            MOV DWORD PTR DS:[EAX+0xC8],ESI
	$+14C    00931DD2     8B4D DC                  MOV ECX,DWORD PTR SS:[EBP-0x24]
	$+14F    00931DD5     89B9 CC000000            MOV DWORD PTR DS:[ECX+0xCC],EDI
	$+155    00931DDB     395D D4                  CMP DWORD PTR SS:[EBP-0x2C],EBX
	$+158    00931DDE     0F84 83000000            JE MapleSto.00931E67
	$+15E    00931DE4     8B4D EC                  MOV ECX,DWORD PTR SS:[EBP-0x14]
	$+161    00931DE7     8B11                     MOV EDX,DWORD PTR DS:[ECX]
	$+163    00931DE9     8B82 2C010000            MOV EAX,DWORD PTR DS:[EDX+0x12C]
	$+169    00931DEF     FFD0                     CALL EAX
	$+16B    00931DF1     0FB7C8                   MOVZX ECX,AX
	$+16E    00931DF4     8D55 1C                  LEA EDX,DWORD PTR SS:[EBP+0x1C]
	$+171    00931DF7     894D 1C                  MOV DWORD PTR SS:[EBP+0x1C],ECX
	$+174    00931DFA     8B4D D4                  MOV ECX,DWORD PTR SS:[EBP-0x2C]
	$+177    00931DFD     52                       PUSH EDX
	$+178    00931DFE     E8 5E6D2D01              CALL MapleSto.01C08B61
	$+17D    00931E03     85C0                     TEST EAX,EAX
	$+17F    00931E05     75 60                    JNZ SHORT MapleSto.00931E67
	$+181    00931E07     68 68964201              PUSH MapleSto.01429668                            ; UNICODE "s_err_not_enough_level"
	$+186    00931E0C     8D4D 18                  LEA ECX,DWORD PTR SS:[EBP+0x18]
	$+189    00931E0F     FF15 D0A13E01            CALL DWORD PTR DS:[0x13EA1D0]                     ; mfc90.#286
	$+18F    00931E15     8D45 18                  LEA EAX,DWORD PTR SS:[EBP+0x18]
	$+192    00931E18     50                       PUSH EAX
	$+193    00931E19     8D4D 0C                  LEA ECX,DWORD PTR SS:[EBP+0xC]
	$+196    00931E1C     51                       PUSH ECX
	$+197    00931E1D     C645 FC 06               MOV BYTE PTR SS:[EBP-0x4],0x6
	$+19B    00931E21     E8 9A56B1FF              CALL MapleSto.004474C0
	$+1A0    00931E26     6A 01                    PUSH 0x1
	$+1A2    00931E28     6A 01                    PUSH 0x1
	$+1A4    00931E2A     50                       PUSH EAX
	$+1A5    00931E2B     C645 FC 07               MOV BYTE PTR SS:[EBP-0x4],0x7
	$+1A9    00931E2F     E8 4C29EDFF              CALL MapleSto.00804780
	$+1AE    00931E34     83C4 14                  ADD ESP,0x14
	$+1B1    00931E37     8D4D 0C                  LEA ECX,DWORD PTR SS:[EBP+0xC]
	$+1B4    00931E3A     FF15 DCA13E01            CALL DWORD PTR DS:[0x13EA1DC]                     ; mfc90.#601
	$+1BA    00931E40     8D4D 18                  LEA ECX,DWORD PTR SS:[EBP+0x18]
	$+1BD    00931E43     FF15 DCA13E01            CALL DWORD PTR DS:[0x13EA1DC]                     ; mfc90.#601
	$+1C3    00931E49     8B75 08                  MOV ESI,DWORD PTR SS:[EBP+0x8]
	$+1C6    00931E4C     8D4D D0                  LEA ECX,DWORD PTR SS:[EBP-0x30]
	$+1C9    00931E4F     895E 04                  MOV DWORD PTR DS:[ESI+0x4],EBX
	$+1CC    00931E52     C745 F0 01000000         MOV DWORD PTR SS:[EBP-0x10],0x1
	$+1D3    00931E59     C645 FC 04               MOV BYTE PTR SS:[EBP-0x4],0x4
	$+1D7    00931E5D     E8 AEB21400              CALL MapleSto.00A7D110
	$+1DC    00931E62     E9 71020000              JMP MapleSto.009320D8
	$+1E1    00931E67     8D4D D0                  LEA ECX,DWORD PTR SS:[EBP-0x30]
	$+1E4    00931E6A     C645 FC 04               MOV BYTE PTR SS:[EBP-0x4],0x4
	$+1E8    00931E6E     E8 9DB21400              CALL MapleSto.00A7D110
	$+1ED    00931E73     E9 42020000              JMP MapleSto.009320BA
	$+1F2    00931E78     8D55 C8                  LEA EDX,DWORD PTR SS:[EBP-0x38]
	$+1F5    00931E7B     57                       PUSH EDI
	$+1F6    00931E7C     52                       PUSH EDX
	$+1F7    00931E7D     E8 65523301              CALL MapleSto.01C670E7
	$+1FC    00931E82     83C4 08                  ADD ESP,0x8
	$+1FF    00931E85     8B45 CC                  MOV EAX,DWORD PTR SS:[EBP-0x34]
	$+202    00931E88     C645 FC 08               MOV BYTE PTR SS:[EBP-0x4],0x8
	$+206    00931E8C     85C0                     TEST EAX,EAX
	$+208    00931E8E     75 1E                    JNZ SHORT MapleSto.00931EAE
	$+20A    00931E90     8B75 08                  MOV ESI,DWORD PTR SS:[EBP+0x8]
	$+20D    00931E93     8D4D C8                  LEA ECX,DWORD PTR SS:[EBP-0x38]
	$+210    00931E96     8946 04                  MOV DWORD PTR DS:[ESI+0x4],EAX
	$+213    00931E99     C745 F0 01000000         MOV DWORD PTR SS:[EBP-0x10],0x1
	$+21A    00931EA0     C645 FC 04               MOV BYTE PTR SS:[EBP-0x4],0x4
	$+21E    00931EA4     E8 67B21400              CALL MapleSto.00A7D110
	$+223    00931EA9     E9 2A020000              JMP MapleSto.009320D8
	$+228    00931EAE     83B8 90000000 00         CMP DWORD PTR DS:[EAX+0x90],0x0
	$+22F    00931EB5     0F84 F3010000            JE MapleSto.009320AE
	$+235    00931EBB     57                       PUSH EDI
	$+236    00931EBC     8D45 D0                  LEA EAX,DWORD PTR SS:[EBP-0x30]
	$+239    00931EBF     50                       PUSH EAX
	$+23A    00931EC0     8BCE                     MOV ECX,ESI
	$+23C    00931EC2     E8 ADDD0A00              CALL MapleSto.009DFC74
	$+241    00931EC7     33C0                     XOR EAX,EAX
	$+243    00931EC9     C645 FC 09               MOV BYTE PTR SS:[EBP-0x4],0x9
	$+247    00931ECD     3945 D4                  CMP DWORD PTR SS:[EBP-0x2C],EAX
	$+24A    00931ED0     75 11                    JNZ SHORT MapleSto.00931EE3
	$+24C    00931ED2     8D4D D0                  LEA ECX,DWORD PTR SS:[EBP-0x30]
	$+24F    00931ED5     C645 FC 08               MOV BYTE PTR SS:[EBP-0x4],0x8
	$+253    00931ED9     E8 32B21400              CALL MapleSto.00A7D110
	$+258    00931EDE     E9 CB010000              JMP MapleSto.009320AE
	$+25D    00931EE3     50                       PUSH EAX
	$+25E    00931EE4     8BCE                     MOV ECX,ESI
	$+260    00931EE6     8945 0C                  MOV DWORD PTR SS:[EBP+0xC],EAX
	$+263    00931EE9     8945 14                  MOV DWORD PTR SS:[EBP+0x14],EAX
	$+266    00931EEC     E8 04995201              CALL MapleSto.01E5B7F5
	$+26B    00931EF1     8D4D 14                  LEA ECX,DWORD PTR SS:[EBP+0x14]
	$+26E    00931EF4     51                       PUSH ECX
	$+26F    00931EF5     8B4D D4                  MOV ECX,DWORD PTR SS:[EBP-0x2C]
	$+272    00931EF8     8D55 0C                  LEA EDX,DWORD PTR SS:[EBP+0xC]
	$+275    00931EFB     52                       PUSH EDX
	$+276    00931EFC     53                       PUSH EBX
	$+277    00931EFD     83EC 08                  SUB ESP,0x8
	$+27A    00931F00     8945 E8                  MOV DWORD PTR SS:[EBP-0x18],EAX
	$+27D    00931F03     8BC4                     MOV EAX,ESP
	$+27F    00931F05     8948 04                  MOV DWORD PTR DS:[EAX+0x4],ECX
	$+282    00931F08     8B45 D4                  MOV EAX,DWORD PTR SS:[EBP-0x2C]
	$+285    00931F0B     8965 E4                  MOV DWORD PTR SS:[EBP-0x1C],ESP
	$+288    00931F0E     85C0                     TEST EAX,EAX
	$+28A    00931F10     74 0A                    JE SHORT MapleSto.00931F1C
	$+28C    00931F12     83C0 04                  ADD EAX,0x4
	$+28F    00931F15     50                       PUSH EAX
	$+290    00931F16     FF15 F8923E01            CALL DWORD PTR DS:[0x13E92F8]
	$+296    00931F1C     8BCE                     MOV ECX,ESI
	$+298    00931F1E     E8 EDCD0A00              CALL MapleSto.009DED10
	$+29D    00931F23     8B55 E8                  MOV EDX,DWORD PTR SS:[EBP-0x18]
	$+2A0    00931F26     52                       PUSH EDX
	$+2A1    00931F27     8BCE                     MOV ECX,ESI
	$+2A3    00931F29     8945 E4                  MOV DWORD PTR SS:[EBP-0x1C],EAX
	$+2A6    00931F2C     E8 C4985201              CALL MapleSto.01E5B7F5
	$+2AB    00931F31     B9 01000000              MOV ECX,0x1
	$+2B0    00931F36     394D E4                  CMP DWORD PTR SS:[EBP-0x1C],ECX
	$+2B3    00931F39     75 11                    JNZ SHORT MapleSto.00931F4C
	$+2B5    00931F3B     8D4D D0                  LEA ECX,DWORD PTR SS:[EBP-0x30]
	$+2B8    00931F3E     C645 FC 08               MOV BYTE PTR SS:[EBP-0x4],0x8
	$+2BC    00931F42     E8 C9B11400              CALL MapleSto.00A7D110
	$+2C1    00931F47     E9 62010000              JMP MapleSto.009320AE
	$+2C6    00931F4C     8B45 0C                  MOV EAX,DWORD PTR SS:[EBP+0xC]
	$+2C9    00931F4F     83F8 07                  CMP EAX,0x7
	$+2CC    00931F52     74 35                    JE SHORT MapleSto.00931F89
	$+2CE    00931F54     83F8 0A                  CMP EAX,0xA
	$+2D1    00931F57     74 30                    JE SHORT MapleSto.00931F89
	$+2D3    00931F59     837D 14 48               CMP DWORD PTR SS:[EBP+0x14],0x48
	$+2D7    00931F5D     74 2A                    JE SHORT MapleSto.00931F89
	$+2D9    00931F5F     8B75 08                  MOV ESI,DWORD PTR SS:[EBP+0x8]
	$+2DC    00931F62     C746 04 00000000         MOV DWORD PTR DS:[ESI+0x4],0x0
	$+2E3    00931F69     894D F0                  MOV DWORD PTR SS:[EBP-0x10],ECX
	$+2E6    00931F6C     8D4D D0                  LEA ECX,DWORD PTR SS:[EBP-0x30]
	$+2E9    00931F6F     C645 FC 08               MOV BYTE PTR SS:[EBP-0x4],0x8
	$+2ED    00931F73     E8 98B11400              CALL MapleSto.00A7D110
	$+2F2    00931F78     8D4D C8                  LEA ECX,DWORD PTR SS:[EBP-0x38]
	$+2F5    00931F7B     C645 FC 04               MOV BYTE PTR SS:[EBP-0x4],0x4
	$+2F9    00931F7F     E8 8CB11400              CALL MapleSto.00A7D110
	$+2FE    00931F84     E9 4F010000              JMP MapleSto.009320D8
	$+303    00931F89     8B4D EC                  MOV ECX,DWORD PTR SS:[EBP-0x14]
	$+306    00931F8C     E8 AAC54E01              CALL MapleSto.01E1E53B
	$+30B    00931F91     8B4D 1C                  MOV ECX,DWORD PTR SS:[EBP+0x1C]
	$+30E    00931F94     8B55 18                  MOV EDX,DWORD PTR SS:[EBP+0x18]
	$+311    00931F97     51                       PUSH ECX
	$+312    00931F98     8B4D 10                  MOV ECX,DWORD PTR SS:[EBP+0x10]
	$+315    00931F9B     52                       PUSH EDX
	$+316    00931F9C     53                       PUSH EBX
	$+317    00931F9D     51                       PUSH ECX
	$+318    00931F9E     50                       PUSH EAX
	$+319    00931F9F     8D55 E0                  LEA EDX,DWORD PTR SS:[EBP-0x20]
	$+31C    00931FA2     52                       PUSH EDX
	$+31D    00931FA3     8BCE                     MOV ECX,ESI
	$+31F    00931FA5     E8 86D70A00              CALL MapleSto.009DF730
	$+324    00931FAA     50                       PUSH EAX
	$+325    00931FAB     8D4D D8                  LEA ECX,DWORD PTR SS:[EBP-0x28]
	$+328    00931FAE     C645 FC 0A               MOV BYTE PTR SS:[EBP-0x4],0xA
	$+32C    00931FB2     E8 69B6C2FF              CALL MapleSto.0055D620
	$+331    00931FB7     8B45 E4                  MOV EAX,DWORD PTR SS:[EBP-0x1C]
	$+334    00931FBA     8B1D 6C933E01            MOV EBX,DWORD PTR DS:[0x13E936C]
	$+33A    00931FC0     C645 FC 09               MOV BYTE PTR SS:[EBP-0x4],0x9
	$+33E    00931FC4     85C0                     TEST EAX,EAX
	$+340    00931FC6     74 26                    JE SHORT MapleSto.00931FEE
	$+342    00931FC8     83C0 04                  ADD EAX,0x4
	$+345    00931FCB     50                       PUSH EAX
	$+346    00931FCC     FFD3                     CALL EBX
	$+348    00931FCE     85C0                     TEST EAX,EAX
	$+34A    00931FD0     75 1C                    JNZ SHORT MapleSto.00931FEE
	$+34C    00931FD2     8B45 E4                  MOV EAX,DWORD PTR SS:[EBP-0x1C]
	$+34F    00931FD5     83C0 04                  ADD EAX,0x4
	$+352    00931FD8     50                       PUSH EAX
	$+353    00931FD9     FF15 F8923E01            CALL DWORD PTR DS:[0x13E92F8]
	$+359    00931FDF     8B4D E4                  MOV ECX,DWORD PTR SS:[EBP-0x1C]
	$+35C    00931FE2     85C9                     TEST ECX,ECX
	$+35E    00931FE4     74 08                    JE SHORT MapleSto.00931FEE
	$+360    00931FE6     8B11                     MOV EDX,DWORD PTR DS:[ECX]
	$+362    00931FE8     8B02                     MOV EAX,DWORD PTR DS:[EDX]
	$+364    00931FEA     6A 01                    PUSH 0x1
	$+366    00931FEC     FFD0                     CALL EAX
	$+368    00931FEE     8B45 DC                  MOV EAX,DWORD PTR SS:[EBP-0x24]
	$+36B    00931FF1     85C0                     TEST EAX,EAX
	$+36D    00931FF3     0F84 84000000            JE MapleSto.0093207D
	$+373    00931FF9     89B8 C0000000            MOV DWORD PTR DS:[EAX+0xC0],EDI
	$+379    00931FFF     8B55 DC                  MOV EDX,DWORD PTR SS:[EBP-0x24]
	$+37C    00932002     8B45 1C                  MOV EAX,DWORD PTR SS:[EBP+0x1C]
	$+37F    00932005     B9 01000000              MOV ECX,0x1
	$+384    0093200A     66:898A C4000000         MOV WORD PTR DS:[EDX+0xC4],CX
	$+38B    00932011     8B0D 74517201            MOV ECX,DWORD PTR DS:[0x1725174]
	$+391    00932017     50                       PUSH EAX
	$+392    00932018     E8 F3B6F9FF              CALL MapleSto.008CD710
	$+397    0093201D     8BFA                     MOV EDI,EDX
	$+399    0093201F     57                       PUSH EDI
	$+39A    00932020     8BF0                     MOV ESI,EAX
	$+39C    00932022     56                       PUSH ESI
	$+39D    00932023     8D4D E0                  LEA ECX,DWORD PTR SS:[EBP-0x20]
	$+3A0    00932026     51                       PUSH ECX
	$+3A1    00932027     B9 604B7201              MOV ECX,MapleSto.01724B60
	$+3A6    0093202C     E8 8F32B8FF              CALL MapleSto.004B52C0
	$+3AB    00932031     837D E4 00               CMP DWORD PTR SS:[EBP-0x1C],0x0
	$+3AF    00932035     75 28                    JNZ SHORT MapleSto.0093205F
	$+3B1    00932037     8BD6                     MOV EDX,ESI
	$+3B3    00932039     0BD7                     OR EDX,EDI
	$+3B5    0093203B     74 22                    JE SHORT MapleSto.0093205F
	$+3B7    0093203D     8B75 08                  MOV ESI,DWORD PTR SS:[EBP+0x8]
	$+3BA    00932040     8D4D E0                  LEA ECX,DWORD PTR SS:[EBP-0x20]
	$+3BD    00932043     C746 04 00000000         MOV DWORD PTR DS:[ESI+0x4],0x0
	$+3C4    0093204A     C745 F0 01000000         MOV DWORD PTR SS:[EBP-0x10],0x1
	$+3CB    00932051     C645 FC 09               MOV BYTE PTR SS:[EBP-0x4],0x9
	$+3CF    00932055     E8 B6B01400              CALL MapleSto.00A7D110
	$+3D4    0093205A   ^ E9 0DFFFFFF              JMP MapleSto.00931F6C
	$+3D9    0093205F     8B45 DC                  MOV EAX,DWORD PTR SS:[EBP-0x24]
	$+3DC    00932062     89B0 C8000000            MOV DWORD PTR DS:[EAX+0xC8],ESI
	$+3E2    00932068     8B4D DC                  MOV ECX,DWORD PTR SS:[EBP-0x24]
	$+3E5    0093206B     89B9 CC000000            MOV DWORD PTR DS:[ECX+0xCC],EDI
	$+3EB    00932071     8D4D E0                  LEA ECX,DWORD PTR SS:[EBP-0x20]
	$+3EE    00932074     C645 FC 09               MOV BYTE PTR SS:[EBP-0x4],0x9
	$+3F2    00932078     E8 93B01400              CALL MapleSto.00A7D110
	$+3F7    0093207D     8B45 D4                  MOV EAX,DWORD PTR SS:[EBP-0x2C]
	$+3FA    00932080     C645 FC 08               MOV BYTE PTR SS:[EBP-0x4],0x8
	$+3FE    00932084     85C0                     TEST EAX,EAX
	$+400    00932086     74 26                    JE SHORT MapleSto.009320AE
	$+402    00932088     83C0 04                  ADD EAX,0x4
	$+405    0093208B     50                       PUSH EAX
	$+406    0093208C     FFD3                     CALL EBX
	$+408    0093208E     85C0                     TEST EAX,EAX
	$+40A    00932090     75 1C                    JNZ SHORT MapleSto.009320AE
	$+40C    00932092     8B55 D4                  MOV EDX,DWORD PTR SS:[EBP-0x2C]
	$+40F    00932095     83C2 04                  ADD EDX,0x4
	$+412    00932098     52                       PUSH EDX
	$+413    00932099     FF15 F8923E01            CALL DWORD PTR DS:[0x13E92F8]
	$+419    0093209F     8B4D D4                  MOV ECX,DWORD PTR SS:[EBP-0x2C]
	$+41C    009320A2     85C9                     TEST ECX,ECX
	$+41E    009320A4     74 08                    JE SHORT MapleSto.009320AE
	$+420    009320A6     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+422    009320A8     8B10                     MOV EDX,DWORD PTR DS:[EAX]
	$+424    009320AA     6A 01                    PUSH 0x1
	$+426    009320AC     FFD2                     CALL EDX
	$+428    009320AE     8D4D C8                  LEA ECX,DWORD PTR SS:[EBP-0x38]
	$+42B    009320B1     C645 FC 04               MOV BYTE PTR SS:[EBP-0x4],0x4
	$+42F    009320B5     E8 56B01400              CALL MapleSto.00A7D110
	$+434    009320BA     8B45 DC                  MOV EAX,DWORD PTR SS:[EBP-0x24]
	$+437    009320BD     8B75 08                  MOV ESI,DWORD PTR SS:[EBP+0x8]
	$+43A    009320C0     8946 04                  MOV DWORD PTR DS:[ESI+0x4],EAX
	$+43D    009320C3     85C0                     TEST EAX,EAX
	$+43F    009320C5     74 0A                    JE SHORT MapleSto.009320D1
	$+441    009320C7     83C0 04                  ADD EAX,0x4
	$+444    009320CA     50                       PUSH EAX
	$+445    009320CB     FF15 F8923E01            CALL DWORD PTR DS:[0x13E92F8]
	$+44B    009320D1     C745 F0 01000000         MOV DWORD PTR SS:[EBP-0x10],0x1
	$+452    009320D8     C645 FC 00               MOV BYTE PTR SS:[EBP-0x4],0x0
	$+456    009320DC     8D4D D8                  LEA ECX,DWORD PTR SS:[EBP-0x28]
	$+459    009320DF     E8 2CB01400              CALL MapleSto.00A7D110
	$+45E    009320E4     8BC6                     MOV EAX,ESI
	$+460    009320E6     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+463    009320E9     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+46A    009320F0     59                       POP ECX
	$+46B    009320F1     5F                       POP EDI
	$+46C    009320F2     5E                       POP ESI
	$+46D    009320F3     5B                       POP EBX
	$+46E    009320F4     8BE5                     MOV ESP,EBP
	$+470    009320F6     5D                       POP EBP
	$+471    009320F7     C2 1800                  RETN 0x18
	$+474    009320FA     CC                       INT3
	*/
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("8d??????????e8????????85??0f??????????68????????8d????ff??????????8d????508d????51c7????????????e8????????6a",
		2);
	if (!op_dw_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *op_dw_tmp;
	assert(dw_tmp > 0x1600 && dw_tmp < 0x1800);
	ofs << "pt_cdword	g_o_skill_mgr_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;
	return true;
}

bool CAutoUpdateApp::Scan_Task(Cmn_FC_OFstream& ofs)
{
	pt_pcvoid addr_tmp;
	pt_dword dw_tmp;
	//boost::optional<pt_dword> op_dw_tmp;

	/*
	$-518    00936123     8BCE                    MOV ECX,ESI
	$-516    00936125     E8 76DFFFFF             CALL MapleSto.009340A0
	$-511    0093612A     3BC7                    CMP EAX,EDI
	$-50F    0093612C     75 14                   JNZ SHORT MapleSto.00936142
	$-50D    0093612E     8D4D 08                 LEA ECX,DWORD PTR SS:[EBP+0x8]
	$-50A    00936131     C745 FC FFFFFFFF        MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-503    00936138     E8 D36F1400             CALL MapleSto.00A7D110
	$-4FE    0093613D     E9 A4020000             JMP MapleSto.009363E6
	$-4F9    00936142     8B55 0C                 MOV EDX,DWORD PTR SS:[EBP+0xC]
	$-4F6    00936145     8B42 2C                 MOV EAX,DWORD PTR DS:[EDX+0x2C]
	$-4F3    00936148     8B40 18                 MOV EAX,DWORD PTR DS:[EAX+0x18]
	$-4F0    0093614B     57                      PUSH EDI
	$-4EF    0093614C     8D9E 4C190000           LEA EBX,DWORD PTR DS:[ESI+0x194C]
	$-4E9    00936152     50                      PUSH EAX
	$-4E8    00936153     8BCB                    MOV ECX,EBX
	$-4E6    00936155     E8 D6600A00             CALL MapleSto.009DC230
	$-4E1    0093615A     D9EE                    FLDZ
	$-4DF    0093615C     D9C9                    FXCH ST(1)
	$-4DD    0093615E     DFF1                    FCOMIP ST,ST(1)
	$-4DB    00936160     DDD8                    FSTP ST
	$-4D9    00936162     0F86 8B000000           JBE MapleSto.009361F3
	$-4D3    00936168     8BCE                    MOV ECX,ESI
	$-4D1    0093616A     E8 B1240400             CALL MapleSto.00978620
	$-4CC    0093616F     83F8 10                 CMP EAX,0x10
	$-4C9    00936172   ^ 75 BA                   JNZ SHORT MapleSto.0093612E
	$-4C7    00936174     57                      PUSH EDI
	$-4C6    00936175     68 F80E6E01             PUSH MapleSto.016E0EF8
	$-4C1    0093617A     68 E0D46C01             PUSH MapleSto.016CD4E0
	$-4BC    0093617F     57                      PUSH EDI
	$-4BB    00936180     8BCE                    MOV ECX,ESI
	$-4B9    00936182     E8 1A3A4F01             CALL MapleSto.01E29BA1
	$-4B4    00936187     50                      PUSH EAX
	$-4B3    00936188     E8 CFBB8C00             CALL MapleSto.01201D5C                             ; JMP to msvcr90.__RTDynamicCast
	$-4AE    0093618D     8BF0                    MOV ESI,EAX
	$-4AC    0093618F     83C4 14                 ADD ESP,0x14
	$-4A9    00936192     3BF7                    CMP ESI,EDI
	$-4A7    00936194   ^ 74 98                   JE SHORT MapleSto.0093612E
	$-4A5    00936196     83EC 08                 SUB ESP,0x8
	$-4A2    00936199     8D55 08                 LEA EDX,DWORD PTR SS:[EBP+0x8]
	$-49F    0093619C     8BCC                    MOV ECX,ESP
	$-49D    0093619E     8965 10                 MOV DWORD PTR SS:[EBP+0x10],ESP
	$-49A    009361A1     52                      PUSH EDX
	$-499    009361A2     E8 99DDBDFF             CALL MapleSto.00513F40
	$-494    009361A7     8BCE                    MOV ECX,ESI
	$-492    009361A9     E8 F2370D00             CALL MapleSto.00A099A0
	$-48D    009361AE     8D45 E4                 LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$-48A    009361B1     50                      PUSH EAX
	$-489    009361B2     8BCE                    MOV ECX,ESI
	$-487    009361B4     E8 22BE5201             CALL MapleSto.01E61FDB
	$-482    009361B9     8B45 E8                 MOV EAX,DWORD PTR SS:[EBP-0x18]
	$-47F    009361BC     3BC7                    CMP EAX,EDI
	$-47D    009361BE     74 03                   JE SHORT MapleSto.009361C3
	$-47B    009361C0     8978 74                 MOV DWORD PTR DS:[EAX+0x74],EDI
	$-478    009361C3     8D4D E4                 LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$-475    009361C6     E8 456F1400             CALL MapleSto.00A7D110
	$-470    009361CB     8D4D 08                 LEA ECX,DWORD PTR SS:[EBP+0x8]
	$-46D    009361CE     C745 FC FFFFFFFF        MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-466    009361D5     E8 366F1400             CALL MapleSto.00A7D110
	$-461    009361DA     B8 01000000             MOV EAX,0x1
	$-45C    009361DF     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-459    009361E2     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
	$-452    009361E9     59                      POP ECX
	$-451    009361EA     5F                      POP EDI
	$-450    009361EB     5E                      POP ESI
	$-44F    009361EC     5B                      POP EBX
	$-44E    009361ED     8BE5                    MOV ESP,EBP
	$-44C    009361EF     5D                      POP EBP
	$-44B    009361F0     C2 0C00                 RETN 0xC
	$-448    009361F3     0F57C0                  XORPS XMM0,XMM0
	$-445    009361F6     8B8E E4000000           MOV ECX,DWORD PTR DS:[ESI+0xE4]
	$-43F    009361FC     57                      PUSH EDI
	$-43E    009361FD     68 F80E6E01             PUSH MapleSto.016E0EF8
	$-439    00936202     68 E0D46C01             PUSH MapleSto.016CD4E0
	$-434    00936207     57                      PUSH EDI
	$-433    00936208     F3:0F1145 EC            MOVSS DWORD PTR SS:[EBP-0x14],XMM0
	$-42E    0093620D     F3:0F1145 10            MOVSS DWORD PTR SS:[EBP+0x10],XMM0
	$-429    00936212     E8 09D70C00             CALL MapleSto.00A03920
	$-424    00936217     50                      PUSH EAX
	$-423    00936218     E8 3FBB8C00             CALL MapleSto.01201D5C                             ; JMP to msvcr90.__RTDynamicCast
	$-41E    0093621D     8BF8                    MOV EDI,EAX
	$-41C    0093621F     83C4 14                 ADD ESP,0x14
	$-419    00936222     85FF                    TEST EDI,EDI
	$-417    00936224     74 33                   JE SHORT MapleSto.00936259
	$-415    00936226     8D4D DC                 LEA ECX,DWORD PTR SS:[EBP-0x24]
	$-412    00936229     51                      PUSH ECX
	$-411    0093622A     8BCF                    MOV ECX,EDI
	$-40F    0093622C     E8 AABD5201             CALL MapleSto.01E61FDB
	$-40A    00936231     837D E0 00              CMP DWORD PTR SS:[EBP-0x20],0x0
	$-406    00936235     C645 FC 01              MOV BYTE PTR SS:[EBP-0x4],0x1
	$-402    00936239     74 12                   JE SHORT MapleSto.0093624D
	$-400    0093623B     8BBF 5C010000           MOV EDI,DWORD PTR DS:[EDI+0x15C]
	$-3FA    00936241     57                      PUSH EDI
	$-3F9    00936242     E8 19FF2501             CALL MapleSto.01B96160
	$-3F4    00936247     D95D 10                 FSTP DWORD PTR SS:[EBP+0x10]
	$-3F1    0093624A     83C4 04                 ADD ESP,0x4
	$-3EE    0093624D     8D4D DC                 LEA ECX,DWORD PTR SS:[EBP-0x24]
	$-3EB    00936250     C645 FC 00              MOV BYTE PTR SS:[EBP-0x4],0x0
	$-3E7    00936254     E8 B76E1400             CALL MapleSto.00A7D110
	$-3E2    00936259     8D55 F0                 LEA EDX,DWORD PTR SS:[EBP-0x10]
	$-3DF    0093625C     52                      PUSH EDX
	$-3DE    0093625D     83EC 08                 SUB ESP,0x8
	$-3DB    00936260     8D45 08                 LEA EAX,DWORD PTR SS:[EBP+0x8]
	$-3D8    00936263     8BCC                    MOV ECX,ESP
	$-3D6    00936265     8965 E8                 MOV DWORD PTR SS:[EBP-0x18],ESP
	$-3D3    00936268     50                      PUSH EAX
	$-3D2    00936269     C745 F0 00000000        MOV DWORD PTR SS:[EBP-0x10],0x0
	$-3CB    00936270     E8 CBDCBDFF             CALL MapleSto.00513F40
	$-3C6    00936275     56                      PUSH ESI
	$-3C5    00936276     E8 25460D00             CALL MapleSto.00A0A8A0
	$-3C0    0093627B     6A 14                   PUSH 0x14
	$-3BE    0093627D     8BF8                    MOV EDI,EAX
	$-3BC    0093627F     E8 EAAC8C00             CALL MapleSto.01200F6E                             ; JMP to mfc90.#798
	$-3B7    00936284     83C4 14                 ADD ESP,0x14
	$-3B4    00936287     8945 E8                 MOV DWORD PTR SS:[EBP-0x18],EAX
	$-3B1    0093628A     C645 FC 02              MOV BYTE PTR SS:[EBP-0x4],0x2
	$-3AD    0093628E     85C0                    TEST EAX,EAX
	$-3AB    00936290     74 1B                   JE SHORT MapleSto.009362AD
	$-3A9    00936292     8B4D 0C                 MOV ECX,DWORD PTR SS:[EBP+0xC]
	$-3A6    00936295     8B51 34                 MOV EDX,DWORD PTR DS:[ECX+0x34]
	$-3A3    00936298     0FB752 14               MOVZX EDX,WORD PTR DS:[EDX+0x14]
	$-39F    0093629C     8B49 2C                 MOV ECX,DWORD PTR DS:[ECX+0x2C]
	$-39C    0093629F     8B49 18                 MOV ECX,DWORD PTR DS:[ECX+0x18]
	$-399    009362A2     52                      PUSH EDX
	$-398    009362A3     51                      PUSH ECX
	$-397    009362A4     8BC8                    MOV ECX,EAX
	$-395    009362A6     E8 752C0000             CALL MapleSto.00938F20
	$-390    009362AB     EB 02                   JMP SHORT MapleSto.009362AF
	$-38E    009362AD     33C0                    XOR EAX,EAX
	$-38C    009362AF     83EC 08                 SUB ESP,0x8
	$-389    009362B2     8BCC                    MOV ECX,ESP
	$-387    009362B4     C645 FC 00              MOV BYTE PTR SS:[EBP-0x4],0x0
	$-383    009362B8     8965 E8                 MOV DWORD PTR SS:[EBP-0x18],ESP
	$-380    009362BB     8941 04                 MOV DWORD PTR DS:[ECX+0x4],EAX
	$-37D    009362BE     85C0                    TEST EAX,EAX
	$-37B    009362C0     74 0A                   JE SHORT MapleSto.009362CC
	$-379    009362C2     83C0 04                 ADD EAX,0x4
	$-376    009362C5     50                      PUSH EAX
	$-375    009362C6     FF15 F8923E01           CALL DWORD PTR DS:[0x13E92F8]
	$-36F    009362CC     8B0D C4597201           MOV ECX,DWORD PTR DS:[0x17259C4]
	$-369    009362D2     E8 0962D3FF             CALL MapleSto.0066C4E0
	$-364    009362D7     83EC 08                 SUB ESP,0x8
	$-361    009362DA     8D55 08                 LEA EDX,DWORD PTR SS:[EBP+0x8]
	$-35E    009362DD     8BCC                    MOV ECX,ESP
	$-35C    009362DF     8965 E8                 MOV DWORD PTR SS:[EBP-0x18],ESP
	$-359    009362E2     52                      PUSH EDX
	$-358    009362E3     E8 58DCBDFF             CALL MapleSto.00513F40
	$-353    009362E8     8BCB                    MOV ECX,EBX
	$-351    009362EA     E8 915D0A00             CALL MapleSto.009DC080
	$-34C    009362EF     85FF                    TEST EDI,EDI
	$-34A    009362F1     74 58                   JE SHORT MapleSto.0093634B
	$-348    009362F3     8B45 0C                 MOV EAX,DWORD PTR SS:[EBP+0xC]
	$-345    009362F6     8B48 34                 MOV ECX,DWORD PTR DS:[EAX+0x34]
	$-342    009362F9     E8 45133301             CALL MapleSto.01C67643
	$-33D    009362FE     85C0                    TEST EAX,EAX
	$-33B    00936300     74 2F                   JE SHORT MapleSto.00936331
	$-339    00936302     8D4D 08                 LEA ECX,DWORD PTR SS:[EBP+0x8]
	$-336    00936305     51                      PUSH ECX
	$-335    00936306     8D8E 380F0000           LEA ECX,DWORD PTR DS:[ESI+0xF38]
	$-32F    0093630C     E8 DF9BADFF             CALL MapleSto.0040FEF0
	$-32A    00936311     8B16                    MOV EDX,DWORD PTR DS:[ESI]
	$-328    00936313     8B82 DC000000           MOV EAX,DWORD PTR DS:[EDX+0xDC]
	$-322    00936319     6A 01                   PUSH 0x1
	$-320    0093631B     6A 00                   PUSH 0x0
	$-31E    0093631D     6A 00                   PUSH 0x0
	$-31C    0093631F     6A 10                   PUSH 0x10
	$-31A    00936321     8BCE                    MOV ECX,ESI
	$-318    00936323     FFD0                    CALL EAX
	$-316    00936325     F3:0F1045 10            MOVSS XMM0,DWORD PTR SS:[EBP+0x10]
	$-311    0093632A     F3:0F1145 EC            MOVSS DWORD PTR SS:[EBP-0x14],XMM0
	$-30C    0093632F     EB 1A                   JMP SHORT MapleSto.0093634B
	$-30A    00936331     83EC 08                 SUB ESP,0x8
	$-307    00936334     8D55 08                 LEA EDX,DWORD PTR SS:[EBP+0x8]
	$-304    00936337     8BCC                    MOV ECX,ESP
	$-302    00936339     8965 10                 MOV DWORD PTR SS:[EBP+0x10],ESP
	$-2FF    0093633C     52                      PUSH EDX
	$-2FE    0093633D     E8 FEDBBDFF             CALL MapleSto.00513F40
	$-2F9    00936342     56                      PUSH ESI
	$-2F8    00936343     E8 38820D00             CALL MapleSto.00A0E580
	$-2F3    00936348     83C4 0C                 ADD ESP,0xC
	$-2F0    0093634B     8B45 0C                 MOV EAX,DWORD PTR SS:[EBP+0xC]
	$-2ED    0093634E     8B48 34                 MOV ECX,DWORD PTR DS:[EAX+0x34]
	$-2EA    00936351     0FB749 14               MOVZX ECX,WORD PTR DS:[ECX+0x14]
	$-2E6    00936355     8B40 2C                 MOV EAX,DWORD PTR DS:[EAX+0x2C]
	$-2E3    00936358     8B50 18                 MOV EDX,DWORD PTR DS:[EAX+0x18]
	$-2E0    0093635B     51                      PUSH ECX
	$-2DF    0093635C     52                      PUSH EDX
	$-2DE    0093635D     8BCB                    MOV ECX,EBX
	$-2DC    0093635F     E8 28CB5101             CALL MapleSto.01E52E8C
	$-2D7    00936364     837D F0 00              CMP DWORD PTR SS:[EBP-0x10],0x0
	$-2D3    00936368     74 1F                   JE SHORT MapleSto.00936389
	$-2D1    0093636A     D945 EC                 FLD DWORD PTR SS:[EBP-0x14]
	$-2CE    0093636D     83EC 0C                 SUB ESP,0xC
	$-2CB    00936370     8D45 08                 LEA EAX,DWORD PTR SS:[EBP+0x8]
	$-2C8    00936373     D95C24 08               FSTP DWORD PTR SS:[ESP+0x8]
	$-2C4    00936377     8BCC                    MOV ECX,ESP
	$-2C2    00936379     8965 10                 MOV DWORD PTR SS:[EBP+0x10],ESP
	$-2BF    0093637C     50                      PUSH EAX
	$-2BE    0093637D     E8 BEDBBDFF             CALL MapleSto.00513F40
	$-2B9    00936382     8BCE                    MOV ECX,ESI
	$-2B7    00936384     E8 37F6FFFF             CALL MapleSto.009359C0
	$-2B2    00936389     8D4D 08                 LEA ECX,DWORD PTR SS:[EBP+0x8]
	$-2AF    0093638C     C745 FC FFFFFFFF        MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-2A8    00936393     E8 786D1400             CALL MapleSto.00A7D110
	$-2A3    00936398     B8 01000000             MOV EAX,0x1
	$-29E    0093639D     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-29B    009363A0     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
	$-294    009363A7     59                      POP ECX
	$-293    009363A8     5F                      POP EDI
	$-292    009363A9     5E                      POP ESI
	$-291    009363AA     5B                      POP EBX
	$-290    009363AB     8BE5                    MOV ESP,EBP
	$-28E    009363AD     5D                      POP EBP
	$-28D    009363AE     C2 0C00                 RETN 0xC
	$-28A    009363B1     C745 FC FFFFFFFF        MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-283    009363B8     3BC7                    CMP EAX,EDI
	$-281    009363BA     74 2A                   JE SHORT MapleSto.009363E6
	$-27F    009363BC     83C0 04                 ADD EAX,0x4
	$-27C    009363BF     50                      PUSH EAX
	$-27B    009363C0     FF15 6C933E01           CALL DWORD PTR DS:[0x13E936C]
	$-275    009363C6     85C0                    TEST EAX,EAX
	$-273    009363C8     75 1C                   JNZ SHORT MapleSto.009363E6
	$-271    009363CA     8B4D 0C                 MOV ECX,DWORD PTR SS:[EBP+0xC]
	$-26E    009363CD     83C1 04                 ADD ECX,0x4
	$-26B    009363D0     51                      PUSH ECX
	$-26A    009363D1     FF15 F8923E01           CALL DWORD PTR DS:[0x13E92F8]
	$-264    009363D7     8B4D 0C                 MOV ECX,DWORD PTR SS:[EBP+0xC]
	$-261    009363DA     3BCF                    CMP ECX,EDI
	$-25F    009363DC     74 08                   JE SHORT MapleSto.009363E6
	$-25D    009363DE     8B11                    MOV EDX,DWORD PTR DS:[ECX]
	$-25B    009363E0     8B02                    MOV EAX,DWORD PTR DS:[EDX]
	$-259    009363E2     6A 01                   PUSH 0x1
	$-257    009363E4     FFD0                    CALL EAX
	$-255    009363E6     33C0                    XOR EAX,EAX
	$-253    009363E8     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-250    009363EB     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
	$-249    009363F2     59                      POP ECX
	$-248    009363F3     5F                      POP EDI
	$-247    009363F4     5E                      POP ESI
	$-246    009363F5     5B                      POP EBX
	$-245    009363F6     8BE5                    MOV ESP,EBP
	$-243    009363F8     5D                      POP EBP
	$-242    009363F9     C2 0C00                 RETN 0xC
	$-23F    009363FC     CC                      INT3
	$-23E    009363FD     CC                      INT3
	$-23D    009363FE     CC                      INT3
	$-23C    009363FF     CC                      INT3
	$-23B    00936400     55                      PUSH EBP
	$-23A    00936401     8BEC                    MOV EBP,ESP
	$-238    00936403     6A FF                   PUSH -0x1
	$-236    00936405     68 33303601             PUSH MapleSto.01363033
	$-231    0093640A     64:A1 00000000          MOV EAX,DWORD PTR FS:[0]
	$-22B    00936410     50                      PUSH EAX
	$-22A    00936411     83EC 30                 SUB ESP,0x30
	$-227    00936414     53                      PUSH EBX
	$-226    00936415     56                      PUSH ESI
	$-225    00936416     57                      PUSH EDI
	$-224    00936417     A1 D0687101             MOV EAX,DWORD PTR DS:[0x17168D0]
	$-21F    0093641C     33C5                    XOR EAX,EBP
	$-21D    0093641E     50                      PUSH EAX
	$-21C    0093641F     8D45 F4                 LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-219    00936422     64:A3 00000000          MOV DWORD PTR FS:[0],EAX
	$-213    00936428     8BF1                    MOV ESI,ECX
	$-211    0093642A     E8 BC1D0500             CALL MapleSto.009881EB
	$-20C    0093642F     8BF8                    MOV EDI,EAX
	$-20A    00936431     33DB                    XOR EBX,EBX
	$-208    00936433     897D F0                 MOV DWORD PTR SS:[EBP-0x10],EDI
	$-205    00936436     3BFB                    CMP EDI,EBX
	$-203    00936438     74 09                   JE SHORT MapleSto.00936443
	$-201    0093643A     8B07                    MOV EAX,DWORD PTR DS:[EDI]
	$-1FF    0093643C     8B50 04                 MOV EDX,DWORD PTR DS:[EAX+0x4]
	$-1FC    0093643F     8BCF                    MOV ECX,EDI
	$-1FA    00936441     FFD2                    CALL EDX
	$-1F8    00936443     895D FC                 MOV DWORD PTR SS:[EBP-0x4],EBX
	$-1F5    00936446     3BFB                    CMP EDI,EBX
	$-1F3    00936448     75 0B                   JNZ SHORT MapleSto.00936455
	$-1F1    0093644A     8D8E 6C0F0000           LEA ECX,DWORD PTR DS:[ESI+0xF6C]
	$-1EB    00936450     E8 EBC60400             CALL MapleSto.00982B40
	$-1E6    00936455     D945 08                 FLD DWORD PTR SS:[EBP+0x8]
	$-1E3    00936458     51                      PUSH ECX
	$-1E2    00936459     8BCE                    MOV ECX,ESI
	$-1E0    0093645B     D91C24                  FSTP DWORD PTR SS:[ESP]
	$-1DD    0093645E     E8 FDE80300             CALL MapleSto.00974D60
	$-1D8    00936463     3BFB                    CMP EDI,EBX
	$-1D6    00936465     74 0D                   JE SHORT MapleSto.00936474
	$-1D4    00936467     8BCF                    MOV ECX,EDI
	$-1D2    00936469     E8 C79CEBFF             CALL MapleSto.007F0135
	$-1CD    0093646E     8BCF                    MOV ECX,EDI
	$-1CB    00936470     85C0                    TEST EAX,EAX
	$-1C9    00936472     75 02                   JNZ SHORT MapleSto.00936476
	$-1C7    00936474     8BCE                    MOV ECX,ESI
	$-1C5    00936476     E8 C6C14E01             CALL MapleSto.01E22641
	$-1C0    0093647B     8BCE                    MOV ECX,ESI
	$-1BE    0093647D     E8 F21F4E01             CALL MapleSto.01E18474
	$-1B9    00936482     85C0                    TEST EAX,EAX
	$-1B7    00936484     75 07                   JNZ SHORT MapleSto.0093648D
	$-1B5    00936486     8BCE                    MOV ECX,ESI
	$-1B3    00936488     E8 48E44B01             CALL MapleSto.01DF48D5
	$-1AE    0093648D     8BCE                    MOV ECX,ESI
	$-1AC    0093648F     E8 62084C01             CALL MapleSto.01DF6CF6
	$-1A7    00936494     8B06                    MOV EAX,DWORD PTR DS:[ESI]
	$-1A5    00936496     8B80 F0000000           MOV EAX,DWORD PTR DS:[EAX+0xF0]
	$-19F    0093649C     8D4D D0                 LEA ECX,DWORD PTR SS:[EBP-0x30]
	$-19C    0093649F     51                      PUSH ECX
	$-19B    009364A0     8D55 C4                 LEA EDX,DWORD PTR SS:[EBP-0x3C]
	$-198    009364A3     52                      PUSH EDX
	$-197    009364A4     8BCE                    MOV ECX,ESI
	$-195    009364A6     FFD0                    CALL EAX
	$-193    009364A8     8D4D DC                 LEA ECX,DWORD PTR SS:[EBP-0x24]
	$-190    009364AB     51                      PUSH ECX
	$-18F    009364AC     8BCE                    MOV ECX,ESI
	$-18D    009364AE     E8 9D2CEBFF             CALL MapleSto.007E9150
	$-188    009364B3     8B86 1C110000           MOV EAX,DWORD PTR DS:[ESI+0x111C]
	$-182    009364B9     83F8 0A                 CMP EAX,0xA
	$-17F    009364BC     0F84 98000000           JE MapleSto.0093655A
	$-179    009364C2     83F8 09                 CMP EAX,0x9
	$-176    009364C5     0F84 8F000000           JE MapleSto.0093655A
	$-170    009364CB     F3:0F1045 DC            MOVSS XMM0,DWORD PTR SS:[EBP-0x24]
	$-16B    009364D0     0F2E05 34267401         UCOMISS XMM0,DWORD PTR DS:[0x1742634]
	$-164    009364D7     9F                      LAHF
	$-163    009364D8     F6C4 44                 TEST AH,0x44
	$-160    009364DB     7A 24                   JPE SHORT MapleSto.00936501
	$-15E    009364DD     F3:0F1045 E0            MOVSS XMM0,DWORD PTR SS:[EBP-0x20]
	$-159    009364E2     0F2E05 38267401         UCOMISS XMM0,DWORD PTR DS:[0x1742638]
	$-152    009364E9     9F                      LAHF
	$-151    009364EA     F6C4 44                 TEST AH,0x44
	$-14E    009364ED     7A 12                   JPE SHORT MapleSto.00936501
	$-14C    009364EF     F3:0F1045 E4            MOVSS XMM0,DWORD PTR SS:[EBP-0x1C]
	$-147    009364F4     0F2E05 3C267401         UCOMISS XMM0,DWORD PTR DS:[0x174263C]
	$-140    009364FB     9F                      LAHF
	$-13F    009364FC     F6C4 44                 TEST AH,0x44
	$-13C    009364FF     7B 7E                   JPO SHORT MapleSto.0093657F
	$-13A    00936501     8BCE                    MOV ECX,ESI
	$-138    00936503     E8 99364F01             CALL MapleSto.01E29BA1
	$-133    00936508     8B10                    MOV EDX,DWORD PTR DS:[EAX]
	$-131    0093650A     8BC8                    MOV ECX,EAX
	$-12F    0093650C     8B42 70                 MOV EAX,DWORD PTR DS:[EDX+0x70]
	$-12C    0093650F     FFD0                    CALL EAX
	$-12A    00936511     85C0                    TEST EAX,EAX
	$-128    00936513     74 11                   JE SHORT MapleSto.00936526
	$-126    00936515     8D4D D0                 LEA ECX,DWORD PTR SS:[EBP-0x30]
	$-123    00936518     51                      PUSH ECX
	$-122    00936519     8D55 DC                 LEA EDX,DWORD PTR SS:[EBP-0x24]
	$-11F    0093651C     52                      PUSH EDX
	$-11E    0093651D     8BCE                    MOV ECX,ESI
	$-11C    0093651F     E8 9C170400             CALL MapleSto.00977CC0
	$-117    00936524     DDD8                    FSTP ST
	$-115    00936526     83BE 1C110000 0E        CMP DWORD PTR DS:[ESI+0x111C],0xE
	$-10E    0093652D     75 50                   JNZ SHORT MapleSto.0093657F
	$-10C    0093652F     F3:0F1005 74BD4001      MOVSS XMM0,DWORD PTR DS:[0x140BD74]
	$-104    00936537     F3:0F5C86 680F0000      SUBSS XMM0,DWORD PTR DS:[ESI+0xF68]
	$-FC     0093653F     F3:0F5905 88BD4001      MULSS XMM0,DWORD PTR DS:[0x140BD88]
	$-F4     00936547     51                      PUSH ECX
	$-F3     00936548     8B0D 744E7201           MOV ECX,DWORD PTR DS:[0x1724E74]
	$-ED     0093654E     F3:0F110424             MOVSS DWORD PTR SS:[ESP],XMM0
	$-E8     00936553     E8 EC8BCCFF             CALL MapleSto.005FF144
	$-E3     00936558     EB 25                   JMP SHORT MapleSto.0093657F
	$-E1     0093655A     8BCE                    MOV ECX,ESI
	$-DF     0093655C     E8 40364F01             CALL MapleSto.01E29BA1
	$-DA     00936561     8B10                    MOV EDX,DWORD PTR DS:[EAX]
	$-D8     00936563     8BC8                    MOV ECX,EAX
	$-D6     00936565     8B42 70                 MOV EAX,DWORD PTR DS:[EDX+0x70]
	$-D3     00936568     FFD0                    CALL EAX
	$-D1     0093656A     85C0                    TEST EAX,EAX
	$-CF     0093656C     74 11                   JE SHORT MapleSto.0093657F
	$-CD     0093656E     8D4D D0                 LEA ECX,DWORD PTR SS:[EBP-0x30]
	$-CA     00936571     51                      PUSH ECX
	$-C9     00936572     8D55 DC                 LEA EDX,DWORD PTR SS:[EBP-0x24]
	$-C6     00936575     52                      PUSH EDX
	$-C5     00936576     8BCE                    MOV ECX,ESI
	$-C3     00936578     E8 43170400             CALL MapleSto.00977CC0
	$-BE     0093657D     DDD8                    FSTP ST
	$-BC     0093657F     A1 744E7201             MOV EAX,DWORD PTR DS:[0x1724E74]
	$-B7     00936584     8B88 FC000000           MOV ECX,DWORD PTR DS:[EAX+0xFC]
	$-B1     0093658A     3BCB                    CMP ECX,EBX
	$-AF     0093658C     74 11                   JE SHORT MapleSto.0093659F
	$-AD     0093658E     8B11                    MOV EDX,DWORD PTR DS:[ECX]
	$-AB     00936590     8B42 04                 MOV EAX,DWORD PTR DS:[EDX+0x4]
	$-A8     00936593     FFD0                    CALL EAX
	$-A6     00936595     83F8 06                 CMP EAX,0x6
	$-A3     00936598     7C 05                   JL SHORT MapleSto.0093659F
	$-A1     0093659A     83F8 07                 CMP EAX,0x7
	$-9E     0093659D     7E 20                   JLE SHORT MapleSto.009365BF
	$-9C     0093659F     8BCE                    MOV ECX,ESI
	$-9A     009365A1     E8 FB354F01             CALL MapleSto.01E29BA1
	$-95     009365A6     8B10                    MOV EDX,DWORD PTR DS:[EAX]
	$-93     009365A8     F3:0F1045 08            MOVSS XMM0,DWORD PTR SS:[EBP+0x8]
	$-8E     009365AD     83EC 08                 SUB ESP,0x8
	$-8B     009365B0     8BC8                    MOV ECX,EAX
	$-89     009365B2     8B42 24                 MOV EAX,DWORD PTR DS:[EDX+0x24]
	$-86     009365B5     0F5AC0                  CVTPS2PD XMM0,XMM0
	$-83     009365B8     F20F110424              MOVSD QWORD PTR SS:[ESP],XMM0
	$-7E     009365BD     FFD0                    CALL EAX
	$-7C     009365BF     8BCE                    MOV ECX,ESI
	$-7A     009365C1     E8 B8314D01             CALL MapleSto.01E0977E
	$-75     009365C6     85C0                    TEST EAX,EAX
	$-73     009365C8     74 0B                   JE SHORT MapleSto.009365D5
	$-71     009365CA     8B8E BC170000           MOV ECX,DWORD PTR DS:[ESI+0x17BC]
	$-6B     009365D0     E8 4B1BE4FF             CALL MapleSto.00778120
	$-66     009365D5     8B8E 401A0000           MOV ECX,DWORD PTR DS:[ESI+0x1A40]
	$-60     009365DB     3BCB                    CMP ECX,EBX
	$-5E     009365DD     74 0C                   JE SHORT MapleSto.009365EB
	$-5C     009365DF     D945 08                 FLD DWORD PTR SS:[EBP+0x8]
	$-59     009365E2     51                      PUSH ECX
	$-58     009365E3     D91C24                  FSTP DWORD PTR SS:[ESP]
	$-55     009365E6     E8 35BCECFF             CALL MapleSto.00802220
	$-50     009365EB     8BCE                    MOV ECX,ESI
	$-4E     009365ED     E8 63580100             CALL MapleSto.0094BE55
	$-49     009365F2     8D8E 501A0000           LEA ECX,DWORD PTR DS:[ESI+0x1A50]					; g_o_role_action_mgr_
	$-43     009365F8     E8 0308C7FF             CALL MapleSto.005A6E00
	$-3E     009365FD     D945 08                 FLD DWORD PTR SS:[EBP+0x8]
	$-3B     00936600     51                      PUSH ECX
	$-3A     00936601     8D8E 4C190000           LEA ECX,DWORD PTR DS:[ESI+0x194C]
	$-34     00936607     D91C24                  FSTP DWORD PTR SS:[ESP]
	$-31     0093660A     E8 A17C0A00             CALL MapleSto.009DE2B0
	$-2C     0093660F     8B0D 784E7201           MOV ECX,DWORD PTR DS:[0x1724E78]
	$-26     00936615     E8 1688CFFF             CALL MapleSto.0062EE30
	$-21     0093661A     8B0D 3C417201           MOV ECX,DWORD PTR DS:[0x172413C]
	$-1B     00936620     E8 5BD4F5FF             CALL MapleSto.00893A80
	$-16     00936625     8B0D 48667201           MOV ECX,DWORD PTR DS:[0x1726648]
	$-10     0093662B     E8 409ED0FF             CALL MapleSto.00640470
	$-B      00936630     8B0D 0C457201           MOV ECX,DWORD PTR DS:[0x172450C]                   ; g_a_quest_mgr_
	$-5      00936636     E8 45E50600             CALL MapleSto.009A4B80
	$ ==>    0093663B     8B0D E06A7201           MOV ECX,DWORD PTR DS:[0x1726AE0]
	$+6      00936641     E8 BA35D4FF             CALL MapleSto.00679C00
	$+B      00936646     8B8E EC1A0000           MOV ECX,DWORD PTR DS:[ESI+0x1AEC]
	$+11     0093664C     3BCB                    CMP ECX,EBX
	$+13     0093664E     74 45                   JE SHORT MapleSto.00936695
	$+15     00936650     E8 DAE24E01             CALL MapleSto.01E2492F
	$+1A     00936655     85C0                    TEST EAX,EAX
	$+1C     00936657     74 12                   JE SHORT MapleSto.0093666B
	$+1E     00936659     8B0D 744E7201           MOV ECX,DWORD PTR DS:[0x1724E74]
	$+24     0093665F     3999 8C010000           CMP DWORD PTR DS:[ECX+0x18C],EBX
	$+2A     00936665     74 04                   JE SHORT MapleSto.0093666B
	$+2C     00936667     53                      PUSH EBX
	$+2D     00936668     53                      PUSH EBX
	$+2E     00936669     EB 25                   JMP SHORT MapleSto.00936690
	$+30     0093666B     8B8E EC1A0000           MOV ECX,DWORD PTR DS:[ESI+0x1AEC]
	$+36     00936671     E8 B9E24E01             CALL MapleSto.01E2492F
	$+3B     00936676     85C0                    TEST EAX,EAX
	$+3D     00936678     75 1B                   JNZ SHORT MapleSto.00936695
	$+3F     0093667A     8B0D 744E7201           MOV ECX,DWORD PTR DS:[0x1724E74]
	$+45     00936680     3999 8C010000           CMP DWORD PTR DS:[ECX+0x18C],EBX
	$+4B     00936686     75 0D                   JNZ SHORT MapleSto.00936695
	$+4D     00936688     8B96 EC1A0000           MOV EDX,DWORD PTR DS:[ESI+0x1AEC]
	$+53     0093668E     53                      PUSH EBX
	$+54     0093668F     52                      PUSH EDX
	$+55     00936690     E8 0BF8CBFF             CALL MapleSto.005F5EA0
	$+5A     00936695     8B46 44                 MOV EAX,DWORD PTR DS:[ESI+0x44]
	$+5D     00936698     3BC3                    CMP EAX,EBX
	$+5F     0093669A     74 21                   JE SHORT MapleSto.009366BD
	$+61     0093669C     8078 28 00              CMP BYTE PTR DS:[EAX+0x28],0x0
	$+65     009366A0     74 1B                   JE SHORT MapleSto.009366BD
	$+67     009366A2     8BCE                    MOV ECX,ESI
	$+69     009366A4     E8 C72AEBFF             CALL MapleSto.007E9170
	$+6E     009366A9     50                      PUSH EAX
	$+6F     009366AA     E8 21002300             CALL MapleSto.00B666D0
	$+74     009366AF     83C4 04                 ADD ESP,0x4
	$+77     009366B2     3BC3                    CMP EAX,EBX
	$+79     009366B4     74 07                   JE SHORT MapleSto.009366BD
	$+7B     009366B6     8BC8                    MOV ECX,EAX
	$+7D     009366B8     E8 F3F42F00             CALL MapleSto.00C35BB0
	$+82     009366BD     8B0D 2C407201           MOV ECX,DWORD PTR DS:[0x172402C]
	$+88     009366C3     E8 08CEE6FF             CALL MapleSto.007A34D0
	$+8D     009366C8     84C0                    TEST AL,AL
	$+8F     009366CA     75 3D                   JNZ SHORT MapleSto.00936709
	$+91     009366CC     8B86 B01A0000           MOV EAX,DWORD PTR DS:[ESI+0x1AB0]
	$+97     009366D2     3BC3                    CMP EAX,EBX
	$+99     009366D4     74 33                   JE SHORT MapleSto.00936709
	$+9B     009366D6     50                      PUSH EAX
	$+9C     009366D7     8BCE                    MOV ECX,ESI
	$+9E     009366D9     E8 842C4E01             CALL MapleSto.01E19362
	$+A3     009366DE     8BC8                    MOV ECX,EAX
	$+A5     009366E0     E8 CBBD0400             CALL MapleSto.009824B0
	$+AA     009366E5     84C0                    TEST AL,AL
	$+AC     009366E7     75 20                   JNZ SHORT MapleSto.00936709
	$+AE     009366E9     8BCE                    MOV ECX,ESI
	$+B0     009366EB     E8 301F0400             CALL MapleSto.00978620
	$+B5     009366F0     83F8 10                 CMP EAX,0x10
	$+B8     009366F3     74 14                   JE SHORT MapleSto.00936709
	$+BA     009366F5     399E B41A0000           CMP DWORD PTR DS:[ESI+0x1AB4],EBX
	$+C0     009366FB     74 0C                   JE SHORT MapleSto.00936709
	$+C2     009366FD     899E B41A0000           MOV DWORD PTR DS:[ESI+0x1AB4],EBX
	$+C8     00936703     899E B01A0000           MOV DWORD PTR DS:[ESI+0x1AB0],EBX
	$+CE     00936709     8BB6 E41A0000           MOV ESI,DWORD PTR DS:[ESI+0x1AE4]
	$+D4     0093670F     3BF3                    CMP ESI,EBX
	$+D6     00936711     8B1D F8923E01           MOV EBX,DWORD PTR DS:[0x13E92F8]
	$+DC     00936717     8975 EC                 MOV DWORD PTR SS:[EBP-0x14],ESI
	$+DF     0093671A     74 06                   JE SHORT MapleSto.00936722
	$+E1     0093671C     8D46 04                 LEA EAX,DWORD PTR DS:[ESI+0x4]
	$+E4     0093671F     50                      PUSH EAX
	$+E5     00936720     FFD3                    CALL EBX
	$+E7     00936722     C645 FC 01              MOV BYTE PTR SS:[EBP-0x4],0x1
	$+EB     00936726     85F6                    TEST ESI,ESI
	$+ED     00936728     74 33                   JE SHORT MapleSto.0093675D
	$+EF     0093672A     837E 38 00              CMP DWORD PTR DS:[ESI+0x38],0x0
	$+F3     0093672E     75 2D                   JNZ SHORT MapleSto.0093675D
	$+F5     00936730     E8 AB59C3FF             CALL MapleSto.0056C0E0
	$+FA     00936735     8B4E 34                 MOV ECX,DWORD PTR DS:[ESI+0x34]
	$+FD     00936738     3BCA                    CMP ECX,EDX
	$+FF     0093673A     7F 21                   JG SHORT MapleSto.0093675D
	$+101    0093673C     7C 07                   JL SHORT MapleSto.00936745
	$+103    0093673E     8B56 30                 MOV EDX,DWORD PTR DS:[ESI+0x30]
	$+106    00936741     3BD0                    CMP EDX,EAX
	$+108    00936743     73 18                   JNB SHORT MapleSto.0093675D
	$+10A    00936745     6A 01                   PUSH 0x1
	$+10C    00936747     6A 01                   PUSH 0x1
	$+10E    00936749     68 8A0A0000             PUSH 0xA8A
	$+113    0093674E     C746 38 01000000        MOV DWORD PTR DS:[ESI+0x38],0x1
	$+11A    00936755     E8 9BA5ECFF             CALL MapleSto.00800CF5
	$+11F    0093675A     83C4 0C                 ADD ESP,0xC
	$+122    0093675D     C645 FC 00              MOV BYTE PTR SS:[EBP-0x4],0x0
	$+126    00936761     85F6                    TEST ESI,ESI
	$+128    00936763     74 1E                   JE SHORT MapleSto.00936783
	$+12A    00936765     8D7E 04                 LEA EDI,DWORD PTR DS:[ESI+0x4]
	$+12D    00936768     57                      PUSH EDI
	$+12E    00936769     FF15 6C933E01           CALL DWORD PTR DS:[0x13E936C]
	$+134    0093676F     85C0                    TEST EAX,EAX
	$+136    00936771     75 0D                   JNZ SHORT MapleSto.00936780
	$+138    00936773     57                      PUSH EDI
	$+139    00936774     FFD3                    CALL EBX
	$+13B    00936776     8B06                    MOV EAX,DWORD PTR DS:[ESI]
	$+13D    00936778     8B10                    MOV EDX,DWORD PTR DS:[EAX]
	$+13F    0093677A     6A 01                   PUSH 0x1
	$+141    0093677C     8BCE                    MOV ECX,ESI
	$+143    0093677E     FFD2                    CALL EDX
	$+145    00936780     8B7D F0                 MOV EDI,DWORD PTR SS:[EBP-0x10]
	$+148    00936783     C745 FC FFFFFFFF        MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+14F    0093678A     85FF                    TEST EDI,EDI
	$+151    0093678C     74 09                   JE SHORT MapleSto.00936797
	$+153    0093678E     8B07                    MOV EAX,DWORD PTR DS:[EDI]
	$+155    00936790     8B50 08                 MOV EDX,DWORD PTR DS:[EAX+0x8]
	$+158    00936793     8BCF                    MOV ECX,EDI
	$+15A    00936795     FFD2                    CALL EDX
	$+15C    00936797     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+15F    0093679A     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
	$+166    009367A1     59                      POP ECX
	$+167    009367A2     5F                      POP EDI
	$+168    009367A3     5E                      POP ESI
	$+169    009367A4     5B                      POP EBX
	$+16A    009367A5     8BE5                    MOV ESP,EBP
	$+16C    009367A7     5D                      POP EBP
	$+16D    009367A8     C2 0400                 RETN 0x4
	$+170    009367AB     CC                      INT3
	$+171    009367AC     CC                      INT3
	$+172    009367AD     CC                      INT3
	$+173    009367AE     CC                      INT3
	$+174    009367AF     CC                      INT3
	$+175    009367B0     55                      PUSH EBP
	$+176    009367B1     8BEC                    MOV EBP,ESP
	$+178    009367B3     6A FF                   PUSH -0x1
	$+17A    009367B5     68 D4303601             PUSH MapleSto.013630D4
	$+17F    009367BA     64:A1 00000000          MOV EAX,DWORD PTR FS:[0]
	$+185    009367C0     50                      PUSH EAX
	$+186    009367C1     51                      PUSH ECX
	$+187    009367C2     53                      PUSH EBX
	$+188    009367C3     56                      PUSH ESI
	$+189    009367C4     57                      PUSH EDI
	$+18A    009367C5     A1 D0687101             MOV EAX,DWORD PTR DS:[0x17168D0]
	$+18F    009367CA     33C5                    XOR EAX,EBP
	$+191    009367CC     50                      PUSH EAX
	$+192    009367CD     8D45 F4                 LEA EAX,DWORD PTR SS:[EBP-0xC]
	$+195    009367D0     64:A3 00000000          MOV DWORD PTR FS:[0],EAX
	$+19B    009367D6     8BF1                    MOV ESI,ECX
	$+19D    009367D8     8975 F0                 MOV DWORD PTR SS:[EBP-0x10],ESI
	$+1A0    009367DB     C706 84254601           MOV DWORD PTR DS:[ESI],MapleSto.01462584
	$+1A6    009367E1     C746 04 68254601        MOV DWORD PTR DS:[ESI+0x4],MapleSto.01462568
	$+1AD    009367E8     C786 20190000 60254601  MOV DWORD PTR DS:[ESI+0x1920],MapleSto.01462560
	$+1B7    009367F2     8B8E 401A0000           MOV ECX,DWORD PTR DS:[ESI+0x1A40]
	$+1BD    009367F8     33DB                    XOR EBX,EBX
	$+1BF    009367FA     C745 FC 09000000        MOV DWORD PTR SS:[EBP-0x4],0x9
	$+1C6    00936801     3BCB                    CMP ECX,EBX
	$+1C8    00936803     74 08                   JE SHORT MapleSto.0093680D
	$+1CA    00936805     8B01                    MOV EAX,DWORD PTR DS:[ECX]
	$+1CC    00936807     8B10                    MOV EDX,DWORD PTR DS:[EAX]
	$+1CE    00936809     6A 01                   PUSH 0x1
	$+1D0    0093680B     FFD2                    CALL EDX
	$+1D2    0093680D     8B86 E41A0000           MOV EAX,DWORD PTR DS:[ESI+0x1AE4]
	$+1D8    00936813     8B3D F8923E01           MOV EDI,DWORD PTR DS:[0x13E92F8]
	$+1DE    00936819     3BC3                    CMP EAX,EBX
	$+1E0    0093681B     74 32                   JE SHORT MapleSto.0093684F
	$+1E2    0093681D     83C0 04                 ADD EAX,0x4
	$+1E5    00936820     50                      PUSH EAX
	$+1E6    00936821     FF15 6C933E01           CALL DWORD PTR DS:[0x13E936C]
	$+1EC    00936827     85C0                    TEST EAX,EAX
	$+1EE    00936829     75 1E                   JNZ SHORT MapleSto.00936849
	$+1F0    0093682B     8B86 E41A0000           MOV EAX,DWORD PTR DS:[ESI+0x1AE4]
	$+1F6    00936831     83C0 04                 ADD EAX,0x4
	$+1F9    00936834     50                      PUSH EAX
	$+1FA    00936835     FFD7                    CALL EDI
	$+1FC    00936837     8B8E E41A0000           MOV ECX,DWORD PTR DS:[ESI+0x1AE4]
	$+202    0093683D     3BCB                    CMP ECX,EBX
	$+204    0093683F     74 08                   JE SHORT MapleSto.00936849
	$+206    00936841     8B01                    MOV EAX,DWORD PTR DS:[ECX]
	$+208    00936843     8B10                    MOV EDX,DWORD PTR DS:[EAX]
	$+20A    00936845     6A 01                   PUSH 0x1
	$+20C    00936847     FFD2                    CALL EDX
	$+20E    00936849     899E E41A0000           MOV DWORD PTR DS:[ESI+0x1AE4],EBX
	$+214    0093684F     899E 401A0000           MOV DWORD PTR DS:[ESI+0x1A40],EBX
	$+21A    00936855     8B86 001B0000           MOV EAX,DWORD PTR DS:[ESI+0x1B00]
	$+220    0093685B     3BC3                    CMP EAX,EBX
	$+222    0093685D     74 32                   JE SHORT MapleSto.00936891
	$+224    0093685F     83C0 04                 ADD EAX,0x4
	$+227    00936862     50                      PUSH EAX
	$+228    00936863     FF15 6C933E01           CALL DWORD PTR DS:[0x13E936C]
	$+22E    00936869     85C0                    TEST EAX,EAX
	$+230    0093686B     75 1E                   JNZ SHORT MapleSto.0093688B
	$+232    0093686D     8B86 001B0000           MOV EAX,DWORD PTR DS:[ESI+0x1B00]
	$+238    00936873     83C0 04                 ADD EAX,0x4
	$+23B    00936876     50                      PUSH EAX
	$+23C    00936877     FFD7                    CALL EDI
	$+23E    00936879     8B8E 001B0000           MOV ECX,DWORD PTR DS:[ESI+0x1B00]
	$+244    0093687F     3BCB                    CMP ECX,EBX
	$+246    00936881     74 08                   JE SHORT MapleSto.0093688B
	$+248    00936883     8B01                    MOV EAX,DWORD PTR DS:[ECX]
	$+24A    00936885     8B10                    MOV EDX,DWORD PTR DS:[EAX]
	$+24C    00936887     6A 01                   PUSH 0x1
	$+24E    00936889     FFD2                    CALL EDX
	$+250    0093688B     899E 001B0000           MOV DWORD PTR DS:[ESI+0x1B00],EBX
	$+256    00936891     8B86 DC1A0000           MOV EAX,DWORD PTR DS:[ESI+0x1ADC]
	$+25C    00936897     3BC3                    CMP EAX,EBX
	$+25E    00936899     74 32                   JE SHORT MapleSto.009368CD
	$+260    0093689B     83C0 04                 ADD EAX,0x4
	$+263    0093689E     50                      PUSH EAX
	$+264    0093689F     FF15 6C933E01           CALL DWORD PTR DS:[0x13E936C]
	$+26A    009368A5     85C0                    TEST EAX,EAX
	$+26C    009368A7     75 1E                   JNZ SHORT MapleSto.009368C7
	$+26E    009368A9     8B86 DC1A0000           MOV EAX,DWORD PTR DS:[ESI+0x1ADC]
	$+274    009368AF     83C0 04                 ADD EAX,0x4
	$+277    009368B2     50                      PUSH EAX
	$+278    009368B3     FFD7                    CALL EDI
	$+27A    009368B5     8B8E DC1A0000           MOV ECX,DWORD PTR DS:[ESI+0x1ADC]
	$+280    009368BB     3BCB                    CMP ECX,EBX
	$+282    009368BD     74 08                   JE SHORT MapleSto.009368C7
	$+284    009368BF     8B01                    MOV EAX,DWORD PTR DS:[ECX]
	$+286    009368C1     8B10                    MOV EDX,DWORD PTR DS:[EAX]
	$+288    009368C3     6A 01                   PUSH 0x1
	$+28A    009368C5     FFD2                    CALL EDX
	$+28C    009368C7     899E DC1A0000           MOV DWORD PTR DS:[ESI+0x1ADC],EBX
	$+292    009368CD     8D8E 281B0000           LEA ECX,DWORD PTR DS:[ESI+0x1B28]
	$+298    009368D3     E8 48951600             CALL MapleSto.00A9FE20
	$+29D    009368D8     8B86 001B0000           MOV EAX,DWORD PTR DS:[ESI+0x1B00]
	$+2A3    009368DE     C645 FC 07              MOV BYTE PTR SS:[EBP-0x4],0x7
	$+2A7    009368E2     3BC3                    CMP EAX,EBX
	$+2A9    009368E4     74 32                   JE SHORT MapleSto.00936918
	$+2AB    009368E6     83C0 04                 ADD EAX,0x4
	$+2AE    009368E9     50                      PUSH EAX
	$+2AF    009368EA     FF15 6C933E01           CALL DWORD PTR DS:[0x13E936C]
	$+2B5    009368F0     85C0                    TEST EAX,EAX
	$+2B7    009368F2     75 1E                   JNZ SHORT MapleSto.00936912
	$+2B9    009368F4     8B86 001B0000           MOV EAX,DWORD PTR DS:[ESI+0x1B00]
	$+2BF    009368FA     83C0 04                 ADD EAX,0x4
	$+2C2    009368FD     50                      PUSH EAX
	$+2C3    009368FE     FFD7                    CALL EDI
	$+2C5    00936900     8B8E 001B0000           MOV ECX,DWORD PTR DS:[ESI+0x1B00]
	$+2CB    00936906     3BCB                    CMP ECX,EBX
	$+2CD    00936908     74 08                   JE SHORT MapleSto.00936912
	$+2CF    0093690A     8B01                    MOV EAX,DWORD PTR DS:[ECX]
	$+2D1    0093690C     8B10                    MOV EDX,DWORD PTR DS:[EAX]
	$+2D3    0093690E     6A 01                   PUSH 0x1
	$+2D5    00936910     FFD2                    CALL EDX
	$+2D7    00936912     899E 001B0000           MOV DWORD PTR DS:[ESI+0x1B00],EBX
	$+2DD    00936918     8B86 E41A0000           MOV EAX,DWORD PTR DS:[ESI+0x1AE4]
	$+2E3    0093691E     C645 FC 06              MOV BYTE PTR SS:[EBP-0x4],0x6
	$+2E7    00936922     3BC3                    CMP EAX,EBX
	$+2E9    00936924     74 32                   JE SHORT MapleSto.00936958
	$+2EB    00936926     83C0 04                 ADD EAX,0x4
	$+2EE    00936929     50                      PUSH EAX
	$+2EF    0093692A     FF15 6C933E01           CALL DWORD PTR DS:[0x13E936C]
	$+2F5    00936930     85C0                    TEST EAX,EAX
	$+2F7    00936932     75 1E                   JNZ SHORT MapleSto.00936952
	$+2F9    00936934     8B86 E41A0000           MOV EAX,DWORD PTR DS:[ESI+0x1AE4]
	$+2FF    0093693A     83C0 04                 ADD EAX,0x4
	$+302    0093693D     50                      PUSH EAX
	$+303    0093693E     FFD7                    CALL EDI
	$+305    00936940     8B8E E41A0000           MOV ECX,DWORD PTR DS:[ESI+0x1AE4]
	$+30B    00936946     3BCB                    CMP ECX,EBX
	$+30D    00936948     74 08                   JE SHORT MapleSto.00936952
	$+30F    0093694A     8B01                    MOV EAX,DWORD PTR DS:[ECX]
	$+311    0093694C     8B10                    MOV EDX,DWORD PTR DS:[EAX]
	$+313    0093694E     6A 01                   PUSH 0x1
	$+315    00936950     FFD2                    CALL EDX
	$+317    00936952     899E E41A0000           MOV DWORD PTR DS:[ESI+0x1AE4],EBX
	$+31D    00936958     8B86 DC1A0000           MOV EAX,DWORD PTR DS:[ESI+0x1ADC]
	$+323    0093695E     C645 FC 05              MOV BYTE PTR SS:[EBP-0x4],0x5
	$+327    00936962     3BC3                    CMP EAX,EBX
	$+329    00936964     74 32                   JE SHORT MapleSto.00936998
	$+32B    00936966     83C0 04                 ADD EAX,0x4
	$+32E    00936969     50                      PUSH EAX
	$+32F    0093696A     FF15 6C933E01           CALL DWORD PTR DS:[0x13E936C]
	$+335    00936970     85C0                    TEST EAX,EAX
	$+337    00936972     75 1E                   JNZ SHORT MapleSto.00936992
	$+339    00936974     8B86 DC1A0000           MOV EAX,DWORD PTR DS:[ESI+0x1ADC]
	$+33F    0093697A     83C0 04                 ADD EAX,0x4
	$+342    0093697D     50                      PUSH EAX
	$+343    0093697E     FFD7                    CALL EDI
	$+345    00936980     8B8E DC1A0000           MOV ECX,DWORD PTR DS:[ESI+0x1ADC]
	$+34B    00936986     3BCB                    CMP ECX,EBX
	$+34D    00936988     74 08                   JE SHORT MapleSto.00936992
	$+34F    0093698A     8B01                    MOV EAX,DWORD PTR DS:[ECX]
	$+351    0093698C     8B10                    MOV EDX,DWORD PTR DS:[EAX]
	$+353    0093698E     6A 01                   PUSH 0x1
	$+355    00936990     FFD2                    CALL EDX
	$+357    00936992     899E DC1A0000           MOV DWORD PTR DS:[ESI+0x1ADC],EBX
	$+35D    00936998     8D8E B81A0000           LEA ECX,DWORD PTR DS:[ESI+0x1AB8]
	$+363    0093699E     C645 FC 04              MOV BYTE PTR SS:[EBP-0x4],0x4
	$+367    009369A2     E8 E9CFB5FF             CALL MapleSto.00493990
	$+36C    009369A7     8D8E 501A0000           LEA ECX,DWORD PTR DS:[ESI+0x1A50]
	$+372    009369AD     C645 FC 03              MOV BYTE PTR SS:[EBP-0x4],0x3
	$+376    009369B1     E8 FA13C7FF             CALL MapleSto.005A7DB0
	$+37B    009369B6     8B8E 481A0000           MOV ECX,DWORD PTR DS:[ESI+0x1A48]
	$+381    009369BC     C645 FC 02              MOV BYTE PTR SS:[EBP-0x4],0x2
	$+385    009369C0     3BCB                    CMP ECX,EBX
	$+387    009369C2     74 07                   JE SHORT MapleSto.009369CB
	$+389    009369C4     8B01                    MOV EAX,DWORD PTR DS:[ECX]
	$+38B    009369C6     8B50 08                 MOV EDX,DWORD PTR DS:[EAX+0x8]
	$+38E    009369C9     FFD2                    CALL EDX
	$+390    009369CB     8D8E 4C190000           LEA ECX,DWORD PTR DS:[ESI+0x194C]
	$+396    009369D1     C645 FC 01              MOV BYTE PTR SS:[EBP-0x4],0x1
	$+39A    009369D5     E8 462F0000             CALL MapleSto.00939920
	$+39F    009369DA     8D8E 24190000           LEA ECX,DWORD PTR DS:[ESI+0x1924]
	$+3A5    009369E0     885D FC                 MOV BYTE PTR SS:[EBP-0x4],BL
	$+3A8    009369E3     E8 08E5E3FF             CALL MapleSto.00774EF0
	$+3AD    009369E8     8BCE                    MOV ECX,ESI
	$+3AF    009369EA     C745 FC FFFFFFFF        MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+3B6    009369F1     E8 0ACD0300             CALL MapleSto.00973700
	$+3BB    009369F6     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+3BE    009369F9     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
	$+3C5    00936A00     59                      POP ECX
	$+3C6    00936A01     5F                      POP EDI
	$+3C7    00936A02     5E                      POP ESI
	$+3C8    00936A03     5B                      POP EBX
	$+3C9    00936A04     8BE5                    MOV ESP,EBP
	$+3CB    00936A06     5D                      POP EBP
	$+3CC    00936A07     C3                      RETN
	$+3CD    00936A08     CC                      INT3
	*/
	addr_tmp = scan_exe_.ScanAddr("8B0D????????E8????????8B8E????????3BCB74", 0);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = AsDword((pt_pcbyte)addr_tmp, -0x49 + 2);
	assert(dw_tmp > 0x1a00 && dw_tmp < 0x1c00);
	ofs << "pt_cdword	g_o_role_action_mgr_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	dw_tmp = AsDword((pt_pcbyte)addr_tmp, -0xb + 2);
	CheckExeAddr(dw_tmp);
	ofs << "pt_cdword	g_a_quest_mgr_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	/*
	$-1C4    009AA6EF     CC                      INT3
	$-1C3    009AA6F0     55                      PUSH EBP
	$-1C2    009AA6F1     8BEC                    MOV EBP,ESP
	$-1C0    009AA6F3     6A FF                   PUSH -0x1
	$-1BE    009AA6F5     68 D8D93601             PUSH MapleSto.0136D9D8
	$-1B9    009AA6FA     64:A1 00000000          MOV EAX,DWORD PTR FS:[0]
	$-1B3    009AA700     50                      PUSH EAX
	$-1B2    009AA701     83EC 10                 SUB ESP,0x10
	$-1AF    009AA704     56                      PUSH ESI
	$-1AE    009AA705     A1 D0687101             MOV EAX,DWORD PTR DS:[0x17168D0]
	$-1A9    009AA70A     33C5                    XOR EAX,EBP
	$-1A7    009AA70C     50                      PUSH EAX
	$-1A6    009AA70D     8D45 F4                 LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-1A3    009AA710     64:A3 00000000          MOV DWORD PTR FS:[0],EAX
	$-19D    009AA716     C745 F0 00000000        MOV DWORD PTR SS:[EBP-0x10],0x0
	$-196    009AA71D     8B75 08                 MOV ESI,DWORD PTR SS:[EBP+0x8]
	$-193    009AA720     C746 04 00000000        MOV DWORD PTR DS:[ESI+0x4],0x0
	$-18C    009AA727     8D45 EC                 LEA EAX,DWORD PTR SS:[EBP-0x14]
	$-189    009AA72A     50                      PUSH EAX
	$-188    009AA72B     8B45 0C                 MOV EAX,DWORD PTR SS:[EBP+0xC]
	$-185    009AA72E     8D4D E8                 LEA ECX,DWORD PTR SS:[EBP-0x18]
	$-182    009AA731     51                      PUSH ECX
	$-181    009AA732     8D55 E4                 LEA EDX,DWORD PTR SS:[EBP-0x1C]
	$-17E    009AA735     52                      PUSH EDX
	$-17D    009AA736     50                      PUSH EAX
	$-17C    009AA737     B9 8C3C6E01             MOV ECX,MapleSto.016E3C8C
	$-177    009AA73C     C745 FC 00000000        MOV DWORD PTR SS:[EBP-0x4],0x0
	$-170    009AA743     C745 F0 01000000        MOV DWORD PTR SS:[EBP-0x10],0x1
	$-169    009AA74A     E8 7167B7FF             CALL MapleSto.00520EC0
	$-164    009AA74F     85C0                    TEST EAX,EAX
	$-162    009AA751     74 0B                   JE SHORT MapleSto.009AA75E
	$-160    009AA753     83C0 04                 ADD EAX,0x4
	$-15D    009AA756     50                      PUSH EAX
	$-15C    009AA757     8BCE                    MOV ECX,ESI
	$-15A    009AA759     E8 C22EBBFF             CALL MapleSto.0055D620
	$-155    009AA75E     8BC6                    MOV EAX,ESI
	$-153    009AA760     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-150    009AA763     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
	$-149    009AA76A     59                      POP ECX
	$-148    009AA76B     5E                      POP ESI
	$-147    009AA76C     8BE5                    MOV ESP,EBP
	$-145    009AA76E     5D                      POP EBP
	$-144    009AA76F     C3                      RETN
	$-143    009AA770     55                      PUSH EBP
	$-142    009AA771     8BEC                    MOV EBP,ESP
	$-140    009AA773     6A FF                   PUSH -0x1
	$-13E    009AA775     68 15DA3601             PUSH MapleSto.0136DA15
	$-139    009AA77A     64:A1 00000000          MOV EAX,DWORD PTR FS:[0]
	$-133    009AA780     50                      PUSH EAX
	$-132    009AA781     83EC 0C                 SUB ESP,0xC
	$-12F    009AA784     56                      PUSH ESI
	$-12E    009AA785     A1 D0687101             MOV EAX,DWORD PTR DS:[0x17168D0]
	$-129    009AA78A     33C5                    XOR EAX,EBP
	$-127    009AA78C     50                      PUSH EAX
	$-126    009AA78D     8D45 F4                 LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-123    009AA790     64:A3 00000000          MOV DWORD PTR FS:[0],EAX
	$-11D    009AA796     8B45 0C                 MOV EAX,DWORD PTR SS:[EBP+0xC]
	$-11A    009AA799     50                      PUSH EAX
	$-119    009AA79A     8D4D E8                 LEA ECX,DWORD PTR SS:[EBP-0x18]
	$-116    009AA79D     51                      PUSH ECX
	$-115    009AA79E     C745 F0 00000000        MOV DWORD PTR SS:[EBP-0x10],0x0
	$-10E    009AA7A5     E8 46FFFFFF             CALL MapleSto.009AA6F0
	$-109    009AA7AA     83C4 08                 ADD ESP,0x8
	$-106    009AA7AD     8B45 EC                 MOV EAX,DWORD PTR SS:[EBP-0x14]
	$-103    009AA7B0     8B75 08                 MOV ESI,DWORD PTR SS:[EBP+0x8]
	$-100    009AA7B3     C745 FC 01000000        MOV DWORD PTR SS:[EBP-0x4],0x1
	$-F9     009AA7BA     8BCE                    MOV ECX,ESI
	$-F7     009AA7BC     85C0                    TEST EAX,EAX
	$-F5     009AA7BE     74 5C                   JE SHORT MapleSto.009AA81C
	$-F3     009AA7C0     83C0 14                 ADD EAX,0x14
	$-F0     009AA7C3     50                      PUSH EAX
	$-EF     009AA7C4     FF15 C8A13E01           CALL DWORD PTR DS:[0x13EA1C8]                      ; mfc90.#280
	$-E9     009AA7CA     8B45 EC                 MOV EAX,DWORD PTR SS:[EBP-0x14]
	$-E6     009AA7CD     C745 F0 01000000        MOV DWORD PTR SS:[EBP-0x10],0x1
	$-DF     009AA7D4     C645 FC 00              MOV BYTE PTR SS:[EBP-0x4],0x0
	$-DB     009AA7D8     85C0                    TEST EAX,EAX
	$-D9     009AA7DA     0F84 83000000           JE MapleSto.009AA863
	$-D3     009AA7E0     83C0 04                 ADD EAX,0x4
	$-D0     009AA7E3     50                      PUSH EAX
	$-CF     009AA7E4     FF15 6C933E01           CALL DWORD PTR DS:[0x13E936C]
	$-C9     009AA7EA     85C0                    TEST EAX,EAX
	$-C7     009AA7EC     75 75                   JNZ SHORT MapleSto.009AA863
	$-C5     009AA7EE     8B55 EC                 MOV EDX,DWORD PTR SS:[EBP-0x14]
	$-C2     009AA7F1     83C2 04                 ADD EDX,0x4
	$-BF     009AA7F4     52                      PUSH EDX
	$-BE     009AA7F5     FF15 F8923E01           CALL DWORD PTR DS:[0x13E92F8]
	$-B8     009AA7FB     8B4D EC                 MOV ECX,DWORD PTR SS:[EBP-0x14]
	$-B5     009AA7FE     85C9                    TEST ECX,ECX
	$-B3     009AA800     74 61                   JE SHORT MapleSto.009AA863
	$-B1     009AA802     8B01                    MOV EAX,DWORD PTR DS:[ECX]
	$-AF     009AA804     8B10                    MOV EDX,DWORD PTR DS:[EAX]
	$-AD     009AA806     6A 01                   PUSH 0x1
	$-AB     009AA808     FFD2                    CALL EDX
	$-A9     009AA80A     8BC6                    MOV EAX,ESI
	$-A7     009AA80C     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-A4     009AA80F     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
	$-9D     009AA816     59                      POP ECX
	$-9C     009AA817     5E                      POP ESI
	$-9B     009AA818     8BE5                    MOV ESP,EBP
	$-99     009AA81A     5D                      POP EBP
	$-98     009AA81B     C3                      RETN
	$-97     009AA81C     68 50303F01             PUSH MapleSto.013F3050
	$-92     009AA821     FF15 D0A13E01           CALL DWORD PTR DS:[0x13EA1D0]                      ; mfc90.#286
	$-8C     009AA827     8B45 EC                 MOV EAX,DWORD PTR SS:[EBP-0x14]
	$-89     009AA82A     C745 F0 01000000        MOV DWORD PTR SS:[EBP-0x10],0x1
	$-82     009AA831     C645 FC 00              MOV BYTE PTR SS:[EBP-0x4],0x0
	$-7E     009AA835     85C0                    TEST EAX,EAX
	$-7C     009AA837     74 2A                   JE SHORT MapleSto.009AA863
	$-7A     009AA839     83C0 04                 ADD EAX,0x4
	$-77     009AA83C     50                      PUSH EAX
	$-76     009AA83D     FF15 6C933E01           CALL DWORD PTR DS:[0x13E936C]
	$-70     009AA843     85C0                    TEST EAX,EAX
	$-6E     009AA845     75 1C                   JNZ SHORT MapleSto.009AA863
	$-6C     009AA847     8B45 EC                 MOV EAX,DWORD PTR SS:[EBP-0x14]
	$-69     009AA84A     83C0 04                 ADD EAX,0x4
	$-66     009AA84D     50                      PUSH EAX
	$-65     009AA84E     FF15 F8923E01           CALL DWORD PTR DS:[0x13E92F8]
	$-5F     009AA854     8B4D EC                 MOV ECX,DWORD PTR SS:[EBP-0x14]
	$-5C     009AA857     85C9                    TEST ECX,ECX
	$-5A     009AA859     74 08                   JE SHORT MapleSto.009AA863
	$-58     009AA85B     8B11                    MOV EDX,DWORD PTR DS:[ECX]
	$-56     009AA85D     8B02                    MOV EAX,DWORD PTR DS:[EDX]
	$-54     009AA85F     6A 01                   PUSH 0x1
	$-52     009AA861     FFD0                    CALL EAX
	$-50     009AA863     8BC6                    MOV EAX,ESI
	$-4E     009AA865     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-4B     009AA868     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
	$-44     009AA86F     59                      POP ECX
	$-43     009AA870     5E                      POP ESI
	$-42     009AA871     8BE5                    MOV ESP,EBP
	$-40     009AA873     5D                      POP EBP
	$-3F     009AA874     C3                      RETN
	$-3E     009AA875     CC                      INT3
	$-3D     009AA876     CC                      INT3
	$-3C     009AA877     CC                      INT3
	$-3B     009AA878     CC                      INT3
	$-3A     009AA879     CC                      INT3
	$-39     009AA87A     CC                      INT3
	$-38     009AA87B     CC                      INT3
	$-37     009AA87C     CC                      INT3
	$-36     009AA87D     CC                      INT3
	$-35     009AA87E     CC                      INT3
	$-34     009AA87F     CC                      INT3
	$-33     009AA880     55                      PUSH EBP
	$-32     009AA881     8BEC                    MOV EBP,ESP
	$-30     009AA883     6A FF                   PUSH -0x1
	$-2E     009AA885     68 38DA3601             PUSH MapleSto.0136DA38
	$-29     009AA88A     64:A1 00000000          MOV EAX,DWORD PTR FS:[0]
	$-23     009AA890     50                      PUSH EAX
	$-22     009AA891     83EC 08                 SUB ESP,0x8
	$-1F     009AA894     56                      PUSH ESI
	$-1E     009AA895     A1 D0687101             MOV EAX,DWORD PTR DS:[0x17168D0]
	$-19     009AA89A     33C5                    XOR EAX,EBP
	$-17     009AA89C     50                      PUSH EAX
	$-16     009AA89D     8D45 F4                 LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-13     009AA8A0     64:A3 00000000          MOV DWORD PTR FS:[0],EAX
	$-D      009AA8A6     8B45 08                 MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-A      009AA8A9     50                      PUSH EAX
	$-9      009AA8AA     8D4D EC                 LEA ECX,DWORD PTR SS:[EBP-0x14]
	$-6      009AA8AD     51                      PUSH ECX
	$-5      009AA8AE     E8 3DFEFFFF             CALL MapleSto.009AA6F0                             ; g_c_get_chapter_info_by_id_
	$ ==>    009AA8B3     83C4 08                 ADD ESP,0x8
	$+3      009AA8B6     8B45 F0                 MOV EAX,DWORD PTR SS:[EBP-0x10]
	$+6      009AA8B9     85C0                    TEST EAX,EAX
	$+8      009AA8BB     74 46                   JE SHORT MapleSto.009AA903
	$+A      009AA8BD     8B70 18                 MOV ESI,DWORD PTR DS:[EAX+0x18]
	$+D      009AA8C0     83C0 04                 ADD EAX,0x4
	$+10     009AA8C3     50                      PUSH EAX
	$+11     009AA8C4     C745 FC FFFFFFFF        MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+18     009AA8CB     FF15 6C933E01           CALL DWORD PTR DS:[0x13E936C]
	$+1E     009AA8D1     85C0                    TEST EAX,EAX
	$+20     009AA8D3     75 1C                   JNZ SHORT MapleSto.009AA8F1
	$+22     009AA8D5     8B55 F0                 MOV EDX,DWORD PTR SS:[EBP-0x10]
	$+25     009AA8D8     83C2 04                 ADD EDX,0x4
	$+28     009AA8DB     52                      PUSH EDX
	$+29     009AA8DC     FF15 F8923E01           CALL DWORD PTR DS:[0x13E92F8]
	$+2F     009AA8E2     8B4D F0                 MOV ECX,DWORD PTR SS:[EBP-0x10]
	$+32     009AA8E5     85C9                    TEST ECX,ECX
	$+34     009AA8E7     74 08                   JE SHORT MapleSto.009AA8F1
	$+36     009AA8E9     8B01                    MOV EAX,DWORD PTR DS:[ECX]
	$+38     009AA8EB     8B10                    MOV EDX,DWORD PTR DS:[EAX]
	$+3A     009AA8ED     6A 01                   PUSH 0x1
	$+3C     009AA8EF     FFD2                    CALL EDX
	$+3E     009AA8F1     8BC6                    MOV EAX,ESI
	$+40     009AA8F3     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+43     009AA8F6     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
	$+4A     009AA8FD     59                      POP ECX
	$+4B     009AA8FE     5E                      POP ESI
	$+4C     009AA8FF     8BE5                    MOV ESP,EBP
	$+4E     009AA901     5D                      POP EBP
	$+4F     009AA902     C3                      RETN
	$+50     009AA903     33C0                    XOR EAX,EAX
	$+52     009AA905     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+55     009AA908     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
	$+5C     009AA90F     59                      POP ECX
	$+5D     009AA910     5E                      POP ESI
	$+5E     009AA911     8BE5                    MOV ESP,EBP
	$+60     009AA913     5D                      POP EBP
	$+61     009AA914     C3                      RETN
	$+62     009AA915     CC                      INT3
	$+63     009AA916     CC                      INT3
	$+64     009AA917     CC                      INT3
	$+65     009AA918     CC                      INT3
	$+66     009AA919     CC                      INT3
	$+67     009AA91A     CC                      INT3
	$+68     009AA91B     CC                      INT3
	$+69     009AA91C     CC                      INT3
	$+6A     009AA91D     CC                      INT3
	$+6B     009AA91E     CC                      INT3
	$+6C     009AA91F     CC                      INT3
	$+6D     009AA920     55                      PUSH EBP
	$+6E     009AA921     8BEC                    MOV EBP,ESP
	$+70     009AA923     6A FF                   PUSH -0x1
	$+72     009AA925     68 ACDA3601             PUSH MapleSto.0136DAAC
	$+77     009AA92A     64:A1 00000000          MOV EAX,DWORD PTR FS:[0]
	$+7D     009AA930     50                      PUSH EAX
	$+7E     009AA931     83EC 08                 SUB ESP,0x8
	$+81     009AA934     56                      PUSH ESI
	$+82     009AA935     57                      PUSH EDI
	$+83     009AA936     A1 D0687101             MOV EAX,DWORD PTR DS:[0x17168D0]
	$+88     009AA93B     33C5                    XOR EAX,EBP
	$+8A     009AA93D     50                      PUSH EAX
	$+8B     009AA93E     8D45 F4                 LEA EAX,DWORD PTR SS:[EBP-0xC]
	$+8E     009AA941     64:A3 00000000          MOV DWORD PTR FS:[0],EAX
	$+94     009AA947     8BF1                    MOV ESI,ECX
	$+96     009AA949     8975 EC                 MOV DWORD PTR SS:[EBP-0x14],ESI
	$+99     009AA94C     66:0FEFC0               PXOR MM0,MM0
	$+9D     009AA950     33FF                    XOR EDI,EDI
	$+9F     009AA952     660FD606                MOVQ QWORD PTR DS:[ESI],XMM0
	$+A3     009AA956     897E 08                 MOV DWORD PTR DS:[ESI+0x8],EDI
	$+A6     009AA959     897D FC                 MOV DWORD PTR SS:[EBP-0x4],EDI
	$+A9     009AA95C     C746 0C 188F4601        MOV DWORD PTR DS:[ESI+0xC],MapleSto.01468F18
	$+B0     009AA963     8D4E 14                 LEA ECX,DWORD PTR DS:[ESI+0x14]
	$+B3     009AA966     C706 408F4601           MOV DWORD PTR DS:[ESI],MapleSto.01468F40
	$+B9     009AA96C     C746 0C 388F4601        MOV DWORD PTR DS:[ESI+0xC],MapleSto.01468F38
	$+C0     009AA973     FF15 E0A13E01           CALL DWORD PTR DS:[0x13EA1E0]                      ; mfc90.#316
	$+C6     009AA979     8D4E 18                 LEA ECX,DWORD PTR DS:[ESI+0x18]
	$+C9     009AA97C     FF15 E0A13E01           CALL DWORD PTR DS:[0x13EA1E0]                      ; mfc90.#316
	$+CF     009AA982     8D4E 1C                 LEA ECX,DWORD PTR DS:[ESI+0x1C]
	$+D2     009AA985     FF15 E0A13E01           CALL DWORD PTR DS:[0x13EA1E0]                      ; mfc90.#316
	$+D8     009AA98B     8D4E 20                 LEA ECX,DWORD PTR DS:[ESI+0x20]
	$+DB     009AA98E     FF15 E0A13E01           CALL DWORD PTR DS:[0x13EA1E0]                      ; mfc90.#316
	$+E1     009AA994     897E 24                 MOV DWORD PTR DS:[ESI+0x24],EDI
	$+E4     009AA997     897E 28                 MOV DWORD PTR DS:[ESI+0x28],EDI
	$+E7     009AA99A     897E 2C                 MOV DWORD PTR DS:[ESI+0x2C],EDI
	$+EA     009AA99D     897E 30                 MOV DWORD PTR DS:[ESI+0x30],EDI
	$+ED     009AA9A0     897E 34                 MOV DWORD PTR DS:[ESI+0x34],EDI
	$+F0     009AA9A3     897E 38                 MOV DWORD PTR DS:[ESI+0x38],EDI
	$+F3     009AA9A6     897E 3C                 MOV DWORD PTR DS:[ESI+0x3C],EDI
	$+F6     009AA9A9     897E 40                 MOV DWORD PTR DS:[ESI+0x40],EDI
	$+F9     009AA9AC     C645 FC 07              MOV BYTE PTR SS:[EBP-0x4],0x7
	$+FD     009AA9B0     393D 603C6E01           CMP DWORD PTR DS:[0x16E3C60],EDI
	$+103    009AA9B6     0F85 B8000000           JNZ MapleSto.009AAA74
	$+109    009AA9BC     8D45 F0                 LEA EAX,DWORD PTR SS:[EBP-0x10]
	$+10C    009AA9BF     50                      PUSH EAX
	$+10D    009AA9C0     6A 09                   PUSH 0x9
	$+10F    009AA9C2     B9 5C3C6E01             MOV ECX,MapleSto.016E3C5C
	$+114    009AA9C7     C745 F0 70999A00        MOV DWORD PTR SS:[EBP-0x10],MapleSto.009A9970
	$+11B    009AA9CE     E8 8D560F00             CALL MapleSto.00AA0060
	$+120    009AA9D3     8D4D F0                 LEA ECX,DWORD PTR SS:[EBP-0x10]
	$+123    009AA9D6     51                      PUSH ECX
	$+124    009AA9D7     6A 13                   PUSH 0x13
	$+126    009AA9D9     B9 5C3C6E01             MOV ECX,MapleSto.016E3C5C
	$+12B    009AA9DE     C745 F0 109B9A00        MOV DWORD PTR SS:[EBP-0x10],MapleSto.009A9B10
	$+132    009AA9E5     E8 76560F00             CALL MapleSto.00AA0060
	$+137    009AA9EA     8D55 F0                 LEA EDX,DWORD PTR SS:[EBP-0x10]
	$+13A    009AA9ED     52                      PUSH EDX
	$+13B    009AA9EE     6A 15                   PUSH 0x15
	$+13D    009AA9F0     B9 5C3C6E01             MOV ECX,MapleSto.016E3C5C
	$+142    009AA9F5     C745 F0 90979A00        MOV DWORD PTR SS:[EBP-0x10],MapleSto.009A9790
	$+149    009AA9FC     E8 5F560F00             CALL MapleSto.00AA0060
	$+14E    009AAA01     8D45 F0                 LEA EAX,DWORD PTR SS:[EBP-0x10]
	$+151    009AAA04     50                      PUSH EAX
	$+152    009AAA05     6A 16                   PUSH 0x16
	$+154    009AAA07     B9 5C3C6E01             MOV ECX,MapleSto.016E3C5C
	$+159    009AAA0C     C745 F0 90979A00        MOV DWORD PTR SS:[EBP-0x10],MapleSto.009A9790
	$+160    009AAA13     E8 48560F00             CALL MapleSto.00AA0060
	$+165    009AAA18     8D4D F0                 LEA ECX,DWORD PTR SS:[EBP-0x10]
	$+168    009AAA1B     51                      PUSH ECX
	$+169    009AAA1C     6A 1C                   PUSH 0x1C
	$+16B    009AAA1E     B9 5C3C6E01             MOV ECX,MapleSto.016E3C5C
	$+170    009AAA23     C745 F0 409C9A00        MOV DWORD PTR SS:[EBP-0x10],MapleSto.009A9C40
	$+177    009AAA2A     E8 31560F00             CALL MapleSto.00AA0060
	$+17C    009AAA2F     8D55 F0                 LEA EDX,DWORD PTR SS:[EBP-0x10]
	$+17F    009AAA32     52                      PUSH EDX
	$+180    009AAA33     6A 02                   PUSH 0x2
	$+182    009AAA35     B9 5C3C6E01             MOV ECX,MapleSto.016E3C5C
	$+187    009AAA3A     C745 F0 409A9A00        MOV DWORD PTR SS:[EBP-0x10],MapleSto.009A9A40
	$+18E    009AAA41     E8 1A560F00             CALL MapleSto.00AA0060
	$+193    009AAA46     8D45 F0                 LEA EAX,DWORD PTR SS:[EBP-0x10]
	$+196    009AAA49     50                      PUSH EAX
	$+197    009AAA4A     6A 03                   PUSH 0x3
	$+199    009AAA4C     B9 5C3C6E01             MOV ECX,MapleSto.016E3C5C
	$+19E    009AAA51     C745 F0 409A9A00        MOV DWORD PTR SS:[EBP-0x10],MapleSto.009A9A40
	$+1A5    009AAA58     E8 03560F00             CALL MapleSto.00AA0060
	$+1AA    009AAA5D     8D4D F0                 LEA ECX,DWORD PTR SS:[EBP-0x10]
	$+1AD    009AAA60     51                      PUSH ECX
	$+1AE    009AAA61     6A 6F                   PUSH 0x6F
	$+1B0    009AAA63     B9 5C3C6E01             MOV ECX,MapleSto.016E3C5C
	$+1B5    009AAA68     C745 F0 B09F9A00        MOV DWORD PTR SS:[EBP-0x10],MapleSto.009A9FB0
	$+1BC    009AAA6F     E8 EC550F00             CALL MapleSto.00AA0060
	$+1C1    009AAA74     8BC6                    MOV EAX,ESI
	$+1C3    009AAA76     897E 10                 MOV DWORD PTR DS:[ESI+0x10],EDI
	$+1C6    009AAA79     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+1C9    009AAA7C     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
	$+1D0    009AAA83     59                      POP ECX
	$+1D1    009AAA84     5F                      POP EDI
	$+1D2    009AAA85     5E                      POP ESI
	$+1D3    009AAA86     8BE5                    MOV ESP,EBP
	$+1D5    009AAA88     5D                      POP EBP
	$+1D6    009AAA89     C3                      RETN
	$+1D7    009AAA8A     CC                      INT3
	$+1D8    009AAA8B     CC                      INT3
	$+1D9    009AAA8C     CC                      INT3
	$+1DA    009AAA8D     CC                      INT3
	$+1DB    009AAA8E     CC                      INT3
	$+1DC    009AAA8F     CC                      INT3
	$+1DD    009AAA90     55                      PUSH EBP
	$+1DE    009AAA91     8BEC                    MOV EBP,ESP
	$+1E0    009AAA93     6A FF                   PUSH -0x1
	$+1E2    009AAA95     68 31DB3601             PUSH MapleSto.0136DB31
	$+1E7    009AAA9A     64:A1 00000000          MOV EAX,DWORD PTR FS:[0]
	$+1ED    009AAAA0     50                      PUSH EAX
	$+1EE    009AAAA1     83EC 1C                 SUB ESP,0x1C
	$+1F1    009AAAA4     53                      PUSH EBX
	$+1F2    009AAAA5     56                      PUSH ESI
	$+1F3    009AAAA6     A1 D0687101             MOV EAX,DWORD PTR DS:[0x17168D0]
	$+1F8    009AAAAB     33C5                    XOR EAX,EBP
	$+1FA    009AAAAD     50                      PUSH EAX
	$+1FB    009AAAAE     8D45 F4                 LEA EAX,DWORD PTR SS:[EBP-0xC]
	$+1FE    009AAAB1     64:A3 00000000          MOV DWORD PTR FS:[0],EAX
	$+204    009AAAB7     8B75 0C                 MOV ESI,DWORD PTR SS:[EBP+0xC]
	$+207    009AAABA     8D45 D8                 LEA EAX,DWORD PTR SS:[EBP-0x28]
	$+20A    009AAABD     56                      PUSH ESI
	$+20B    009AAABE     50                      PUSH EAX
	$+20C    009AAABF     C745 E8 00000000        MOV DWORD PTR SS:[EBP-0x18],0x0
	$+213    009AAAC6     E8 D5F2FFFF             CALL MapleSto.009A9DA0
	$+218    009AAACB     83C4 08                 ADD ESP,0x8
	$+21B    009AAACE     837D DC 00              CMP DWORD PTR SS:[EBP-0x24],0x0
	$+21F    009AAAD2     C745 FC 01000000        MOV DWORD PTR SS:[EBP-0x4],0x1
	$+226    009AAAD9     75 6B                   JNZ SHORT MapleSto.009AAB46
	$+228    009AAADB     8B75 08                 MOV ESI,DWORD PTR SS:[EBP+0x8]
	$+22B    009AAADE     68 50303F01             PUSH MapleSto.013F3050
	$+230    009AAAE3     8BCE                    MOV ECX,ESI
	$+232    009AAAE5     FF15 D0A13E01           CALL DWORD PTR DS:[0x13EA1D0]                      ; mfc90.#286
	$+238    009AAAEB     8B45 DC                 MOV EAX,DWORD PTR SS:[EBP-0x24]
	$+23B    009AAAEE     C745 E8 01000000        MOV DWORD PTR SS:[EBP-0x18],0x1
	$+242    009AAAF5     C645 FC 00              MOV BYTE PTR SS:[EBP-0x4],0x0
	$+246    009AAAF9     85C0                    TEST EAX,EAX
	$+248    009AAAFB     0F84 DE010000           JE MapleSto.009AACDF
	$+24E    009AAB01     83C0 04                 ADD EAX,0x4
	$+251    009AAB04     50                      PUSH EAX
	$+252    009AAB05     FF15 6C933E01           CALL DWORD PTR DS:[0x13E936C]
	$+258    009AAB0B     85C0                    TEST EAX,EAX
	$+25A    009AAB0D     0F85 CC010000           JNZ MapleSto.009AACDF
	$+260    009AAB13     8B4D DC                 MOV ECX,DWORD PTR SS:[EBP-0x24]
	$+263    009AAB16     83C1 04                 ADD ECX,0x4
	$+266    009AAB19     51                      PUSH ECX
	$+267    009AAB1A     FF15 F8923E01           CALL DWORD PTR DS:[0x13E92F8]
	$+26D    009AAB20     8B4D DC                 MOV ECX,DWORD PTR SS:[EBP-0x24]
	$+270    009AAB23     85C9                    TEST ECX,ECX
	$+272    009AAB25     0F84 B4010000           JE MapleSto.009AACDF
	$+278    009AAB2B     8B11                    MOV EDX,DWORD PTR DS:[ECX]
	$+27A    009AAB2D     8B02                    MOV EAX,DWORD PTR DS:[EDX]
	$+27C    009AAB2F     6A 01                   PUSH 0x1
	$+27E    009AAB31     FFD0                    CALL EAX
	$+280    009AAB33     8BC6                    MOV EAX,ESI
	$+282    009AAB35     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+285    009AAB38     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
	$+28C    009AAB3F     59                      POP ECX
	$+28D    009AAB40     5E                      POP ESI
	$+28E    009AAB41     5B                      POP EBX
	$+28F    009AAB42     8BE5                    MOV ESP,EBP
	$+291    009AAB44     5D                      POP EBP
	$+292    009AAB45     C3                      RETN
	$+293    009AAB46     8D4D F0                 LEA ECX,DWORD PTR SS:[EBP-0x10]
	$+296    009AAB49     FF15 E0A13E01           CALL DWORD PTR DS:[0x13EA1E0]                      ; mfc90.#316
	$+29C    009AAB4F     BB 02000000             MOV EBX,0x2
	$+2A1    009AAB54     56                      PUSH ESI
	$+2A2    009AAB55     885D FC                 MOV BYTE PTR SS:[EBP-0x4],BL
	$+2A5    009AAB58     E8 A8BEB8FF             CALL MapleSto.00536A05
	$+2AA    009AAB5D     83C4 04                 ADD ESP,0x4
	$+2AD    009AAB60     83F8 01                 CMP EAX,0x1
	$+2B0    009AAB63     75 53                   JNZ SHORT MapleSto.009AABB8
	$+2B2    009AAB65     8D4D E4                 LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$+2B5    009AAB68     68 AA050000             PUSH 0x5AA
	$+2BA    009AAB6D     51                      PUSH ECX
	$+2BB    009AAB6E     E8 0DC9A9FF             CALL MapleSto.00447480
	$+2C0    009AAB73     8B55 DC                 MOV EDX,DWORD PTR SS:[EBP-0x24]
	$+2C3    009AAB76     83C2 14                 ADD EDX,0x14
	$+2C6    009AAB79     52                      PUSH EDX
	$+2C7    009AAB7A     50                      PUSH EAX
	$+2C8    009AAB7B     8D45 EC                 LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+2CB    009AAB7E     68 98054201             PUSH MapleSto.01420598                             ; UNICODE "{0} {1}"
	$+2D0    009AAB83     50                      PUSH EAX
	$+2D1    009AAB84     C645 FC 03              MOV BYTE PTR SS:[EBP-0x4],0x3
	$+2D5    009AAB88     E8 435DBBFF             CALL MapleSto.005608D0
	$+2DA    009AAB8D     83C4 18                 ADD ESP,0x18
	$+2DD    009AAB90     50                      PUSH EAX
	$+2DE    009AAB91     8D4D F0                 LEA ECX,DWORD PTR SS:[EBP-0x10]
	$+2E1    009AAB94     C645 FC 04              MOV BYTE PTR SS:[EBP-0x4],0x4
	$+2E5    009AAB98     FF15 CCA13E01           CALL DWORD PTR DS:[0x13EA1CC]                      ; mfc90.#810
	$+2EB    009AAB9E     8D4D EC                 LEA ECX,DWORD PTR SS:[EBP-0x14]
	$+2EE    009AABA1     FF15 DCA13E01           CALL DWORD PTR DS:[0x13EA1DC]                      ; mfc90.#601
	$+2F4    009AABA7     8D4D E4                 LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$+2F7    009AABAA     885D FC                 MOV BYTE PTR SS:[EBP-0x4],BL
	$+2FA    009AABAD     FF15 DCA13E01           CALL DWORD PTR DS:[0x13EA1DC]                      ; mfc90.#601
	$+300    009AABB3     E9 D3000000             JMP MapleSto.009AAC8B
	$+305    009AABB8     56                      PUSH ESI
	$+306    009AABB9     E8 47BEB8FF             CALL MapleSto.00536A05
	$+30B    009AABBE     83C4 04                 ADD ESP,0x4
	$+30E    009AABC1     3BC3                    CMP EAX,EBX
	$+310    009AABC3     75 3E                   JNZ SHORT MapleSto.009AAC03
	$+312    009AABC5     8D4D EC                 LEA ECX,DWORD PTR SS:[EBP-0x14]
	$+315    009AABC8     68 AB050000             PUSH 0x5AB
	$+31A    009AABCD     51                      PUSH ECX
	$+31B    009AABCE     E8 ADC8A9FF             CALL MapleSto.00447480
	$+320    009AABD3     8B55 DC                 MOV EDX,DWORD PTR SS:[EBP-0x24]
	$+323    009AABD6     83C2 14                 ADD EDX,0x14
	$+326    009AABD9     52                      PUSH EDX
	$+327    009AABDA     50                      PUSH EAX
	$+328    009AABDB     8D45 E4                 LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$+32B    009AABDE     68 98054201             PUSH MapleSto.01420598                             ; UNICODE "{0} {1}"
	$+330    009AABE3     50                      PUSH EAX
	$+331    009AABE4     C645 FC 05              MOV BYTE PTR SS:[EBP-0x4],0x5
	$+335    009AABE8     E8 E35CBBFF             CALL MapleSto.005608D0
	$+33A    009AABED     83C4 18                 ADD ESP,0x18
	$+33D    009AABF0     50                      PUSH EAX
	$+33E    009AABF1     8D4D F0                 LEA ECX,DWORD PTR SS:[EBP-0x10]
	$+341    009AABF4     C645 FC 06              MOV BYTE PTR SS:[EBP-0x4],0x6
	$+345    009AABF8     FF15 CCA13E01           CALL DWORD PTR DS:[0x13EA1CC]                      ; mfc90.#810
	$+34B    009AABFE     8D4D E4                 LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$+34E    009AAC01     EB 64                   JMP SHORT MapleSto.009AAC67
	$+350    009AAC03     56                      PUSH ESI
	$+351    009AAC04     E8 FCBDB8FF             CALL MapleSto.00536A05
	$+356    009AAC09     83C4 04                 ADD ESP,0x4
	$+359    009AAC0C     83F8 03                 CMP EAX,0x3
	$+35C    009AAC0F     75 6A                   JNZ SHORT MapleSto.009AAC7B
	$+35E    009AAC11     68 548F4601             PUSH MapleSto.01468F54                             ; UNICODE "s_quest_repeat_week"
	$+363    009AAC16     8D4D EC                 LEA ECX,DWORD PTR SS:[EBP-0x14]
	$+366    009AAC19     FF15 D0A13E01           CALL DWORD PTR DS:[0x13EA1D0]                      ; mfc90.#286
	$+36C    009AAC1F     8D4D EC                 LEA ECX,DWORD PTR SS:[EBP-0x14]
	$+36F    009AAC22     51                      PUSH ECX
	$+370    009AAC23     8D55 E0                 LEA EDX,DWORD PTR SS:[EBP-0x20]
	$+373    009AAC26     52                      PUSH EDX
	$+374    009AAC27     C645 FC 07              MOV BYTE PTR SS:[EBP-0x4],0x7
	$+378    009AAC2B     E8 90C8A9FF             CALL MapleSto.004474C0
	$+37D    009AAC30     8B4D DC                 MOV ECX,DWORD PTR SS:[EBP-0x24]
	$+380    009AAC33     83C1 14                 ADD ECX,0x14
	$+383    009AAC36     51                      PUSH ECX
	$+384    009AAC37     50                      PUSH EAX
	$+385    009AAC38     8D55 E4                 LEA EDX,DWORD PTR SS:[EBP-0x1C]
	$+388    009AAC3B     68 98054201             PUSH MapleSto.01420598                             ; UNICODE "{0} {1}"
	$+38D    009AAC40     52                      PUSH EDX
	$+38E    009AAC41     C645 FC 08              MOV BYTE PTR SS:[EBP-0x4],0x8
	$+392    009AAC45     E8 865CBBFF             CALL MapleSto.005608D0
	$+397    009AAC4A     83C4 18                 ADD ESP,0x18
	$+39A    009AAC4D     50                      PUSH EAX
	$+39B    009AAC4E     8D4D F0                 LEA ECX,DWORD PTR SS:[EBP-0x10]
	$+39E    009AAC51     C645 FC 09              MOV BYTE PTR SS:[EBP-0x4],0x9
	$+3A2    009AAC55     FF15 CCA13E01           CALL DWORD PTR DS:[0x13EA1CC]                      ; mfc90.#810
	$+3A8    009AAC5B     8D4D E4                 LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$+3AB    009AAC5E     FF15 DCA13E01           CALL DWORD PTR DS:[0x13EA1DC]                      ; mfc90.#601
	$+3B1    009AAC64     8D4D E0                 LEA ECX,DWORD PTR SS:[EBP-0x20]
	$+3B4    009AAC67     FF15 DCA13E01           CALL DWORD PTR DS:[0x13EA1DC]                      ; mfc90.#601
	$+3BA    009AAC6D     8D4D EC                 LEA ECX,DWORD PTR SS:[EBP-0x14]
	$+3BD    009AAC70     885D FC                 MOV BYTE PTR SS:[EBP-0x4],BL
	$+3C0    009AAC73     FF15 DCA13E01           CALL DWORD PTR DS:[0x13EA1DC]                      ; mfc90.#601
	$+3C6    009AAC79     EB 10                   JMP SHORT MapleSto.009AAC8B
	$+3C8    009AAC7B     8B45 DC                 MOV EAX,DWORD PTR SS:[EBP-0x24]
	$+3CB    009AAC7E     83C0 14                 ADD EAX,0x14
	$+3CE    009AAC81     50                      PUSH EAX
	$+3CF    009AAC82     8D4D F0                 LEA ECX,DWORD PTR SS:[EBP-0x10]
	$+3D2    009AAC85     FF15 CCA13E01           CALL DWORD PTR DS:[0x13EA1CC]                      ; mfc90.#810
	$+3D8    009AAC8B     8B75 08                 MOV ESI,DWORD PTR SS:[EBP+0x8]
	$+3DB    009AAC8E     8D4D F0                 LEA ECX,DWORD PTR SS:[EBP-0x10]
	$+3DE    009AAC91     51                      PUSH ECX
	$+3DF    009AAC92     8BCE                    MOV ECX,ESI
	$+3E1    009AAC94     FF15 C8A13E01           CALL DWORD PTR DS:[0x13EA1C8]                      ; mfc90.#280
	$+3E7    009AAC9A     8D4D F0                 LEA ECX,DWORD PTR SS:[EBP-0x10]
	$+3EA    009AAC9D     C745 E8 01000000        MOV DWORD PTR SS:[EBP-0x18],0x1
	$+3F1    009AACA4     FF15 DCA13E01           CALL DWORD PTR DS:[0x13EA1DC]                      ; mfc90.#601
	$+3F7    009AACAA     8B45 DC                 MOV EAX,DWORD PTR SS:[EBP-0x24]
	$+3FA    009AACAD     C645 FC 00              MOV BYTE PTR SS:[EBP-0x4],0x0
	$+3FE    009AACB1     85C0                    TEST EAX,EAX
	$+400    009AACB3     74 2A                   JE SHORT MapleSto.009AACDF
	$+402    009AACB5     83C0 04                 ADD EAX,0x4
	$+405    009AACB8     50                      PUSH EAX
	$+406    009AACB9     FF15 6C933E01           CALL DWORD PTR DS:[0x13E936C]
	$+40C    009AACBF     85C0                    TEST EAX,EAX
	$+40E    009AACC1     75 1C                   JNZ SHORT MapleSto.009AACDF
	$+410    009AACC3     8B55 DC                 MOV EDX,DWORD PTR SS:[EBP-0x24]
	$+413    009AACC6     83C2 04                 ADD EDX,0x4
	$+416    009AACC9     52                      PUSH EDX
	$+417    009AACCA     FF15 F8923E01           CALL DWORD PTR DS:[0x13E92F8]
	$+41D    009AACD0     8B4D DC                 MOV ECX,DWORD PTR SS:[EBP-0x24]
	$+420    009AACD3     85C9                    TEST ECX,ECX
	$+422    009AACD5     74 08                   JE SHORT MapleSto.009AACDF
	$+424    009AACD7     8B01                    MOV EAX,DWORD PTR DS:[ECX]
	$+426    009AACD9     8B10                    MOV EDX,DWORD PTR DS:[EAX]
	$+428    009AACDB     6A 01                   PUSH 0x1
	$+42A    009AACDD     FFD2                    CALL EDX
	$+42C    009AACDF     8BC6                    MOV EAX,ESI
	$+42E    009AACE1     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+431    009AACE4     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
	$+438    009AACEB     59                      POP ECX
	$+439    009AACEC     5E                      POP ESI
	$+43A    009AACED     5B                      POP EBX
	$+43B    009AACEE     8BE5                    MOV ESP,EBP
	$+43D    009AACF0     5D                      POP EBP
	$+43E    009AACF1     C3                      RETN
	$+43F    009AACF2     CC                      INT3
	*/
	addr_tmp = scan_exe_.ScanCall("83C4088B45F085C0????8B701883C00450C745FCFFFFFFFF", -5 + 1);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_c_get_chapter_info_by_id_ = " << addr_tmp << ";" << FC_OFstream_EndlLog;

	/*
	$-5C1    00B0A20A     8B75 08                 MOV ESI,DWORD PTR SS:[EBP+0x8]
	$-5BE    00B0A20D     56                      PUSH ESI
	$-5BD    00B0A20E     E8 6D06EAFF             CALL MapleSto.009AA880
	$-5B8    00B0A213     83C4 04                 ADD ESP,0x4
	$-5B5    00B0A216     85C0                    TEST EAX,EAX
	$-5B3    00B0A218   ^ 75 DA                   JNZ SHORT MapleSto.00B0A1F4
	$-5B1    00B0A21A     68 C4064801             PUSH MapleSto.014806C4                             ; ASCII "m_progressOnly"
	$-5AC    00B0A21F     8D4D F0                 LEA ECX,DWORD PTR SS:[EBP-0x10]
	$-5A9    00B0A222     897D D8                 MOV DWORD PTR SS:[EBP-0x28],EDI
	$-5A6    00B0A225     FF15 20A23E01           CALL DWORD PTR DS:[0x13EA220]                      ; mfc90.#310
	$-5A0    00B0A22B     8D45 F0                 LEA EAX,DWORD PTR SS:[EBP-0x10]
	$-59D    00B0A22E     50                      PUSH EAX
	$-59C    00B0A22F     8D4D DC                 LEA ECX,DWORD PTR SS:[EBP-0x24]
	$-599    00B0A232     51                      PUSH ECX
	$-598    00B0A233     8BCB                    MOV ECX,EBX
	$-596    00B0A235     897D FC                 MOV DWORD PTR SS:[EBP-0x4],EDI
	$-593    00B0A238     E8 0360BAFF             CALL MapleSto.006B0240
	$-58E    00B0A23D     8D4D F0                 LEA ECX,DWORD PTR SS:[EBP-0x10]
	$-58B    00B0A240     C645 FC 02              MOV BYTE PTR SS:[EBP-0x4],0x2
	$-587    00B0A244     FF15 08A23E01           CALL DWORD PTR DS:[0x13EA208]                      ; mfc90.#601
	$-581    00B0A24A     8B4D E0                 MOV ECX,DWORD PTR SS:[EBP-0x20]
	$-57E    00B0A24D     3BCF                    CMP ECX,EDI
	$-57C    00B0A24F     74 14                   JE SHORT MapleSto.00B0A265
	$-57A    00B0A251     E8 DFA33D00             CALL MapleSto.00EE4635
	$-575    00B0A256     84C0                    TEST AL,AL
	$-573    00B0A258     74 0B                   JE SHORT MapleSto.00B0A265
	$-571    00B0A25A     8B4D E0                 MOV ECX,DWORD PTR SS:[EBP-0x20]
	$-56E    00B0A25D     E8 85F83E01             CALL MapleSto.01EF9AE7
	$-569    00B0A262     8945 D8                 MOV DWORD PTR SS:[EBP-0x28],EAX
	$-566    00B0A265     8D4D DC                 LEA ECX,DWORD PTR SS:[EBP-0x24]
	$-563    00B0A268     C745 FC FFFFFFFF        MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-55C    00B0A26F     E8 9C2EF7FF             CALL MapleSto.00A7D110
	$-557    00B0A274     6A 01                   PUSH 0x1
	$-555    00B0A276     8D8D 5CFFFFFF           LEA ECX,DWORD PTR SS:[EBP-0xA4]
	$-54F    00B0A27C     897D F0                 MOV DWORD PTR SS:[EBP-0x10],EDI
	$-54C    00B0A27F     E8 BC923D00             CALL MapleSto.00EE3540
	$-547    00B0A284     6A 01                   PUSH 0x1
	$-545    00B0A286     8D8D 34FFFFFF           LEA ECX,DWORD PTR SS:[EBP-0xCC]
	$-53F    00B0A28C     C745 FC 03000000        MOV DWORD PTR SS:[EBP-0x4],0x3
	$-538    00B0A293     E8 189E3D00             CALL MapleSto.00EE40B0
	$-533    00B0A298     897D B4                 MOV DWORD PTR SS:[EBP-0x4C],EDI
	$-530    00B0A29B     897D B8                 MOV DWORD PTR SS:[EBP-0x48],EDI
	$-52D    00B0A29E     897D BC                 MOV DWORD PTR SS:[EBP-0x44],EDI
	$-52A    00B0A2A1     897D C0                 MOV DWORD PTR SS:[EBP-0x40],EDI
	$-527    00B0A2A4     897D C4                 MOV DWORD PTR SS:[EBP-0x3C],EDI
	$-524    00B0A2A7     897D C8                 MOV DWORD PTR SS:[EBP-0x38],EDI
	$-521    00B0A2AA     897D CC                 MOV DWORD PTR SS:[EBP-0x34],EDI
	$-51E    00B0A2AD     897D D0                 MOV DWORD PTR SS:[EBP-0x30],EDI
	$-51B    00B0A2B0     8D55 B4                 LEA EDX,DWORD PTR SS:[EBP-0x4C]
	$-518    00B0A2B3     52                      PUSH EDX
	$-517    00B0A2B4     56                      PUSH ESI
	$-516    00B0A2B5     C645 FC 06              MOV BYTE PTR SS:[EBP-0x4],0x6
	$-512    00B0A2B9     E8 A2B9A2FF             CALL MapleSto.00535C60
	$-50D    00B0A2BE     8B45 B8                 MOV EAX,DWORD PTR SS:[EBP-0x48]
	$-50A    00B0A2C1     83C4 08                 ADD ESP,0x8
	$-507    00B0A2C4     33F6                    XOR ESI,ESI
	$-505    00B0A2C6     3BC7                    CMP EAX,EDI
	$-503    00B0A2C8     76 7B                   JBE SHORT MapleSto.00B0A345
	$-501    00B0A2CA     B3 07                   MOV BL,0x7
	$-4FF    00B0A2CC     8D6424 00               LEA ESP,DWORD PTR SS:[ESP]
	$-4FB    00B0A2D0     8B0D 0C457201           MOV ECX,DWORD PTR DS:[0x172450C]
	$-4F5    00B0A2D6     83C1 08                 ADD ECX,0x8
	$-4F2    00B0A2D9     3BF0                    CMP ESI,EAX
	$-4F0    00B0A2DB     73 7F                   JNB SHORT MapleSto.00B0A35C
	$-4EE    00B0A2DD     8B55 B4                 MOV EDX,DWORD PTR SS:[EBP-0x4C]
	$-4EB    00B0A2E0     8B14B2                  MOV EDX,DWORD PTR DS:[EDX+ESI*4]
	$-4E8    00B0A2E3     8B01                    MOV EAX,DWORD PTR DS:[ECX]
	$-4E6    00B0A2E5     8B40 0C                 MOV EAX,DWORD PTR DS:[EAX+0xC]
	$-4E3    00B0A2E8     52                      PUSH EDX
	$-4E2    00B0A2E9     8D55 E8                 LEA EDX,DWORD PTR SS:[EBP-0x18]
	$-4DF    00B0A2EC     52                      PUSH EDX
	$-4DE    00B0A2ED     FFD0                    CALL EAX
	$-4DC    00B0A2EF     8B45 EC                 MOV EAX,DWORD PTR SS:[EBP-0x14]
	$-4D9    00B0A2F2     885D FC                 MOV BYTE PTR SS:[EBP-0x4],BL
	$-4D6    00B0A2F5     3BC7                    CMP EAX,EDI
	$-4D4    00B0A2F7     74 0F                   JE SHORT MapleSto.00B0A308
	$-4D2    00B0A2F9     8D4D E8                 LEA ECX,DWORD PTR SS:[EBP-0x18]
	$-4CF    00B0A2FC     51                      PUSH ECX
	$-4CE    00B0A2FD     8D4D C4                 LEA ECX,DWORD PTR SS:[EBP-0x3C]
	$-4CB    00B0A300     E8 8B099BFF             CALL MapleSto.004BAC90
	$-4C6    00B0A305     8B45 EC                 MOV EAX,DWORD PTR SS:[EBP-0x14]
	$-4C3    00B0A308     C645 FC 06              MOV BYTE PTR SS:[EBP-0x4],0x6
	$-4BF    00B0A30C     3BC7                    CMP EAX,EDI
	$-4BD    00B0A30E     74 2D                   JE SHORT MapleSto.00B0A33D
	$-4BB    00B0A310     83C0 04                 ADD EAX,0x4
	$-4B8    00B0A313     50                      PUSH EAX
	$-4B7    00B0A314     FF15 6C933E01           CALL DWORD PTR DS:[0x13E936C]
	$-4B1    00B0A31A     85C0                    TEST EAX,EAX
	$-4AF    00B0A31C     75 1C                   JNZ SHORT MapleSto.00B0A33A
	$-4AD    00B0A31E     8B55 EC                 MOV EDX,DWORD PTR SS:[EBP-0x14]
	$-4AA    00B0A321     83C2 04                 ADD EDX,0x4
	$-4A7    00B0A324     52                      PUSH EDX
	$-4A6    00B0A325     FF15 F8923E01           CALL DWORD PTR DS:[0x13E92F8]
	$-4A0    00B0A32B     8B4D EC                 MOV ECX,DWORD PTR SS:[EBP-0x14]
	$-49D    00B0A32E     3BCF                    CMP ECX,EDI
	$-49B    00B0A330     74 08                   JE SHORT MapleSto.00B0A33A
	$-499    00B0A332     8B01                    MOV EAX,DWORD PTR DS:[ECX]
	$-497    00B0A334     8B10                    MOV EDX,DWORD PTR DS:[EAX]
	$-495    00B0A336     6A 01                   PUSH 0x1
	$-493    00B0A338     FFD2                    CALL EDX
	$-491    00B0A33A     897D EC                 MOV DWORD PTR SS:[EBP-0x14],EDI
	$-48E    00B0A33D     8B45 B8                 MOV EAX,DWORD PTR SS:[EBP-0x48]
	$-48B    00B0A340     46                      INC ESI
	$-48A    00B0A341     3BF0                    CMP ESI,EAX
	$-488    00B0A343   ^ 72 8B                   JB SHORT MapleSto.00B0A2D0
	$-486    00B0A345     897D E0                 MOV DWORD PTR SS:[EBP-0x20],EDI
	$-483    00B0A348     397D C8                 CMP DWORD PTR SS:[EBP-0x38],EDI
	$-480    00B0A34B     0F86 79030000           JBE MapleSto.00B0A6CA
	$-47A    00B0A351     8B45 C4                 MOV EAX,DWORD PTR SS:[EBP-0x3C]
	$-477    00B0A354     83C0 04                 ADD EAX,0x4
	$-474    00B0A357     8945 EC                 MOV DWORD PTR SS:[EBP-0x14],EAX
	$-471    00B0A35A     EB 0C                   JMP SHORT MapleSto.00B0A368
	$-46F    00B0A35C     68 57000780             PUSH 0x80070057
	$-46A    00B0A361     E8 BA778FFF             CALL MapleSto.00401B20
	$-465    00B0A366     33FF                    XOR EDI,EDI
	$-463    00B0A368     8B4D C8                 MOV ECX,DWORD PTR SS:[EBP-0x38]
	$-460    00B0A36B     394D E0                 CMP DWORD PTR SS:[EBP-0x20],ECX
	$-45D    00B0A36E   ^ 73 EC                   JNB SHORT MapleSto.00B0A35C
	$-45B    00B0A370     8B55 EC                 MOV EDX,DWORD PTR SS:[EBP-0x14]
	$-458    00B0A373     8B32                    MOV ESI,DWORD PTR DS:[EDX]
	$-456    00B0A375     8B1D F8923E01           MOV EBX,DWORD PTR DS:[0x13E92F8]
	$-450    00B0A37B     8975 B0                 MOV DWORD PTR SS:[EBP-0x50],ESI
	$-44D    00B0A37E     3BF7                    CMP ESI,EDI
	$-44B    00B0A380     74 06                   JE SHORT MapleSto.00B0A388
	$-449    00B0A382     8D46 04                 LEA EAX,DWORD PTR DS:[ESI+0x4]
	$-446    00B0A385     50                      PUSH EAX
	$-445    00B0A386     FFD3                    CALL EBX
	$-443    00B0A388     8B56 0C                 MOV EDX,DWORD PTR DS:[ESI+0xC]
	$-440    00B0A38B     8B42 0C                 MOV EAX,DWORD PTR DS:[EDX+0xC]
	$-43D    00B0A38E     8D4E 0C                 LEA ECX,DWORD PTR DS:[ESI+0xC]
	$-43A    00B0A391     C645 FC 08              MOV BYTE PTR SS:[EBP-0x4],0x8
	$-436    00B0A395     FFD0                    CALL EAX
	$-434    00B0A397     85C0                    TEST EAX,EAX
	$-432    00B0A399     74 50                   JE SHORT MapleSto.00B0A3EB
	$-430    00B0A39B     8B46 18                 MOV EAX,DWORD PTR DS:[ESI+0x18]
	$-42D    00B0A39E     8B40 28                 MOV EAX,DWORD PTR DS:[EAX+0x28]
	$-42A    00B0A3A1     50                      PUSH EAX
	$-429    00B0A3A2     E8 5EC6A2FF             CALL MapleSto.00536A05
	$-424    00B0A3A7     83C4 04                 ADD ESP,0x4
	$-421    00B0A3AA     83F8 02                 CMP EAX,0x2
	$-41E    00B0A3AD     74 14                   JE SHORT MapleSto.00B0A3C3
	$-41C    00B0A3AF     8B46 18                 MOV EAX,DWORD PTR DS:[ESI+0x18]
	$-419    00B0A3B2     8B40 28                 MOV EAX,DWORD PTR DS:[EAX+0x28]
	$-416    00B0A3B5     50                      PUSH EAX
	$-415    00B0A3B6     E8 4AC6A2FF             CALL MapleSto.00536A05
	$-410    00B0A3BB     83C4 04                 ADD ESP,0x4
	$-40D    00B0A3BE     83F8 03                 CMP EAX,0x3
	$-40A    00B0A3C1     75 28                   JNZ SHORT MapleSto.00B0A3EB
	$-408    00B0A3C3     8D7E 04                 LEA EDI,DWORD PTR DS:[ESI+0x4]
	$-405    00B0A3C6     57                      PUSH EDI
	$-404    00B0A3C7     C645 FC 06              MOV BYTE PTR SS:[EBP-0x4],0x6
	$-400    00B0A3CB     FF15 6C933E01           CALL DWORD PTR DS:[0x13E936C]
	$-3FA    00B0A3D1     85C0                    TEST EAX,EAX
	$-3F8    00B0A3D3     0F85 74010000           JNZ MapleSto.00B0A54D
	$-3F2    00B0A3D9     57                      PUSH EDI
	$-3F1    00B0A3DA     FFD3                    CALL EBX
	$-3EF    00B0A3DC     8B16                    MOV EDX,DWORD PTR DS:[ESI]
	$-3ED    00B0A3DE     8B02                    MOV EAX,DWORD PTR DS:[EDX]
	$-3EB    00B0A3E0     6A 01                   PUSH 0x1
	$-3E9    00B0A3E2     8BCE                    MOV ECX,ESI
	$-3E7    00B0A3E4     FFD0                    CALL EAX
	$-3E5    00B0A3E6     E9 62010000             JMP MapleSto.00B0A54D
	$-3E0    00B0A3EB     397D D8                 CMP DWORD PTR SS:[EBP-0x28],EDI
	$-3DD    00B0A3EE     74 06                   JE SHORT MapleSto.00B0A3F6
	$-3DB    00B0A3F0     837E 10 01              CMP DWORD PTR DS:[ESI+0x10],0x1
	$-3D7    00B0A3F4   ^ 75 CD                   JNZ SHORT MapleSto.00B0A3C3
	$-3D5    00B0A3F6     6A 01                   PUSH 0x1
	$-3D3    00B0A3F8     8D4D 84                 LEA ECX,DWORD PTR SS:[EBP-0x7C]
	$-3D0    00B0A3FB     E8 40913D00             CALL MapleSto.00EE3540
	$-3CB    00B0A400     8B46 18                 MOV EAX,DWORD PTR DS:[ESI+0x18]
	$-3C8    00B0A403     8B40 28                 MOV EAX,DWORD PTR DS:[EAX+0x28]
	$-3C5    00B0A406     50                      PUSH EAX
	$-3C4    00B0A407     68 18303F01             PUSH MapleSto.013F3018                             ; ASCII "id"
	$-3BF    00B0A40C     8D4D 84                 LEA ECX,DWORD PTR SS:[EBP-0x7C]
	$-3BC    00B0A40F     C645 FC 09              MOV BYTE PTR SS:[EBP-0x4],0x9
	$-3B8    00B0A413     E8 C8CDABFF             CALL MapleSto.005C71E0
	$-3B3    00B0A418     8BCE                    MOV ECX,ESI
	$-3B1    00B0A41A     E8 C17AE9FF             CALL MapleSto.009A1EE0
	$-3AC    00B0A41F     50                      PUSH EAX
	$-3AB    00B0A420     68 50044801             PUSH MapleSto.01480450                             ; ASCII "regist"
	$-3A6    00B0A425     8D4D 84                 LEA ECX,DWORD PTR SS:[EBP-0x7C]
	$-3A3    00B0A428     E8 E38D0400             CALL MapleSto.00B53210
	$-39E    00B0A42D     8B46 18                 MOV EAX,DWORD PTR DS:[ESI+0x18]
	$-39B    00B0A430     8B40 28                 MOV EAX,DWORD PTR DS:[EAX+0x28]
	$-398    00B0A433     50                      PUSH EAX
	$-397    00B0A434     E8 37FBA2FF             CALL MapleSto.00539F70
	$-392    00B0A439     83C4 04                 ADD ESP,0x4
	$-38F    00B0A43C     50                      PUSH EAX
	$-38E    00B0A43D     68 88084801             PUSH MapleSto.01480888                             ; ASCII "forceRegist"
	$-389    00B0A442     8D4D 84                 LEA ECX,DWORD PTR SS:[EBP-0x7C]
	$-386    00B0A445     E8 C68D0400             CALL MapleSto.00B53210
	$-381    00B0A44A     837E 10 01              CMP DWORD PTR DS:[ESI+0x10],0x1
	$-37D    00B0A44E     0F94C1                  SETE CL
	$-37A    00B0A451     0FB6D1                  MOVZX EDX,CL
	$-377    00B0A454     52                      PUSH EDX
	$-376    00B0A455     68 F82E4701             PUSH MapleSto.01472EF8                             ; ASCII "progress"
	$-371    00B0A45A     8D4D 84                 LEA ECX,DWORD PTR SS:[EBP-0x7C]
	$-36E    00B0A45D     E8 6E02A4FF             CALL MapleSto.0054A6D0
	$-369    00B0A462     51                      PUSH ECX
	$-368    00B0A463     8965 F0                 MOV DWORD PTR SS:[EBP-0x10],ESP
	$-365    00B0A466     8BFC                    MOV EDI,ESP
	$-363    00B0A468     51                      PUSH ECX
	$-362    00B0A469     8BCC                    MOV ECX,ESP
	$-360    00B0A46B     8965 F0                 MOV DWORD PTR SS:[EBP-0x10],ESP
	$-35D    00B0A46E     68 20044801             PUSH MapleSto.01480420                             ; UNICODE "s_quest_list_quest_name"
	$-358    00B0A473     FF15 D0A13E01           CALL DWORD PTR DS:[0x13EA1D0]                      ; mfc90.#286
	$-352    00B0A479     8B46 18                 MOV EAX,DWORD PTR DS:[ESI+0x18]
	$-34F    00B0A47C     8B40 28                 MOV EAX,DWORD PTR DS:[EAX+0x28]
	$-34C    00B0A47F     50                      PUSH EAX
	$-34B    00B0A480     57                      PUSH EDI
	$-34A    00B0A481     E8 7A0CEAFF             CALL MapleSto.009AB100
	$-345    00B0A486     83C4 0C                 ADD ESP,0xC
	$-342    00B0A489     68 9C0A4201             PUSH MapleSto.01420A9C                             ; ASCII "label"
	$-33D    00B0A48E     8D4D 84                 LEA ECX,DWORD PTR SS:[EBP-0x7C]
	$-33A    00B0A491     E8 5A01A4FF             CALL MapleSto.0054A5F0
	$-335    00B0A496     8B46 18                 MOV EAX,DWORD PTR DS:[ESI+0x18]
	$-332    00B0A499     8B50 28                 MOV EDX,DWORD PTR DS:[EAX+0x28]
	$-32F    00B0A49C     8B45 E4                 MOV EAX,DWORD PTR SS:[EBP-0x1C]
	$-32C    00B0A49F     8B80 28010000           MOV EAX,DWORD PTR DS:[EAX+0x128]
	$-326    00B0A4A5     33C9                    XOR ECX,ECX
	$-324    00B0A4A7     85C0                    TEST EAX,EAX
	$-322    00B0A4A9     7E 22                   JLE SHORT MapleSto.00B0A4CD
	$-320    00B0A4AB     3BC8                    CMP ECX,EAX
	$-31E    00B0A4AD   ^ 0F83 A9FEFFFF           JNB MapleSto.00B0A35C
	$-318    00B0A4B3     8B7D E4                 MOV EDI,DWORD PTR SS:[EBP-0x1C]
	$-315    00B0A4B6     8BBF 24010000           MOV EDI,DWORD PTR DS:[EDI+0x124]
	$-30F    00B0A4BC     39148F                  CMP DWORD PTR DS:[EDI+ECX*4],EDX
	$-30C    00B0A4BF     8D3C8F                  LEA EDI,DWORD PTR DS:[EDI+ECX*4]
	$-309    00B0A4C2     0F84 0E010000           JE MapleSto.00B0A5D6
	$-303    00B0A4C8     41                      INC ECX
	$-302    00B0A4C9     3BC8                    CMP ECX,EAX
	$-300    00B0A4CB   ^ 7C E0                   JL SHORT MapleSto.00B0A4AD
	$-2FE    00B0A4CD     33C0                    XOR EAX,EAX
	$-2FC    00B0A4CF     50                      PUSH EAX
	$-2FB    00B0A4D0     68 78374301             PUSH MapleSto.01433778                             ; ASCII "isNew"
	$-2F6    00B0A4D5     8D4D 84                 LEA ECX,DWORD PTR SS:[EBP-0x7C]
	$-2F3    00B0A4D8     E8 338D0400             CALL MapleSto.00B53210
	$-2EE    00B0A4DD     51                      PUSH ECX
	$-2ED    00B0A4DE     8BCC                    MOV ECX,ESP
	$-2EB    00B0A4E0     8965 F0                 MOV DWORD PTR SS:[EBP-0x10],ESP
	$-2E8    00B0A4E3     56                      PUSH ESI
	$-2E7    00B0A4E4     51                      PUSH ECX
	$-2E6    00B0A4E5     8B4D E4                 MOV ECX,DWORD PTR SS:[EBP-0x1C]
	$-2E3    00B0A4E8     E8 B3D1FFFF             CALL MapleSto.00B076A0
	$-2DE    00B0A4ED     68 C42F3F01             PUSH MapleSto.013F2FC4                             ; ASCII "icon"
	$-2D9    00B0A4F2     8D4D 84                 LEA ECX,DWORD PTR SS:[EBP-0x7C]
	$-2D6    00B0A4F5     E8 1600A4FF             CALL MapleSto.0054A510
	$-2D1    00B0A4FA     68 08044801             PUSH MapleSto.01480408                             ; ASCII "qd_renderer_questList"
	$-2CC    00B0A4FF     68 94024301             PUSH MapleSto.01430294                             ; ASCII "rendererClassName"
	$-2C7    00B0A504     8D4D 84                 LEA ECX,DWORD PTR SS:[EBP-0x7C]
	$-2C4    00B0A507     E8 14B1ABFF             CALL MapleSto.005C5620
	$-2BF    00B0A50C     8D55 84                 LEA EDX,DWORD PTR SS:[EBP-0x7C]
	$-2BC    00B0A50F     52                      PUSH EDX
	$-2BB    00B0A510     8D8D 34FFFFFF           LEA ECX,DWORD PTR SS:[EBP-0xCC]
	$-2B5    00B0A516     E8 D59D3D00             CALL MapleSto.00EE42F0
	$-2B0    00B0A51B     8D4D 84                 LEA ECX,DWORD PTR SS:[EBP-0x7C]
	$-2AD    00B0A51E     C745 F0 01000000        MOV DWORD PTR SS:[EBP-0x10],0x1
	$-2A6    00B0A525     C645 FC 08              MOV BYTE PTR SS:[EBP-0x4],0x8
	$-2A2    00B0A529     E8 E2953D00             CALL MapleSto.00EE3B10
	$-29D    00B0A52E     8D7E 04                 LEA EDI,DWORD PTR DS:[ESI+0x4]
	$-29A    00B0A531     57                      PUSH EDI
	$-299    00B0A532     C645 FC 06              MOV BYTE PTR SS:[EBP-0x4],0x6
	$-295    00B0A536     FF15 6C933E01           CALL DWORD PTR DS:[0x13E936C]
	$-28F    00B0A53C     85C0                    TEST EAX,EAX
	$-28D    00B0A53E     75 0D                   JNZ SHORT MapleSto.00B0A54D
	$-28B    00B0A540     57                      PUSH EDI
	$-28A    00B0A541     FFD3                    CALL EBX
	$-288    00B0A543     8B06                    MOV EAX,DWORD PTR DS:[ESI]
	$-286    00B0A545     8B10                    MOV EDX,DWORD PTR DS:[EAX]
	$-284    00B0A547     6A 01                   PUSH 0x1
	$-282    00B0A549     8BCE                    MOV ECX,ESI
	$-280    00B0A54B     FFD2                    CALL EDX
	$-27E    00B0A54D     8B45 E0                 MOV EAX,DWORD PTR SS:[EBP-0x20]
	$-27B    00B0A550     8345 EC 08              ADD DWORD PTR SS:[EBP-0x14],0x8
	$-277    00B0A554     40                      INC EAX
	$-276    00B0A555     C745 B0 00000000        MOV DWORD PTR SS:[EBP-0x50],0x0
	$-26F    00B0A55C     8945 E0                 MOV DWORD PTR SS:[EBP-0x20],EAX
	$-26C    00B0A55F     3B45 C8                 CMP EAX,DWORD PTR SS:[EBP-0x38]
	$-269    00B0A562   ^ 0F82 FEFDFFFF           JB MapleSto.00B0A366
	$-263    00B0A568     837D F0 00              CMP DWORD PTR SS:[EBP-0x10],0x0
	$-25F    00B0A56C     0F84 56010000           JE MapleSto.00B0A6C8
	$-259    00B0A572     8B45 08                 MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-256    00B0A575     50                      PUSH EAX
	$-255    00B0A576     8D4D E8                 LEA ECX,DWORD PTR SS:[EBP-0x18]
	$-252    00B0A579     51                      PUSH ECX
	$-251    00B0A57A     E8 7101EAFF             CALL MapleSto.009AA6F0
	$-24C    00B0A57F     83C4 08                 ADD ESP,0x8
	$-249    00B0A582     837D EC 00              CMP DWORD PTR SS:[EBP-0x14],0x0
	$-245    00B0A586     BB 0A000000             MOV EBX,0xA
	$-240    00B0A58B     885D FC                 MOV BYTE PTR SS:[EBP-0x4],BL
	$-23D    00B0A58E     0F84 E9000000           JE MapleSto.00B0A67D
	$-237    00B0A594     8B75 08                 MOV ESI,DWORD PTR SS:[EBP+0x8]
	$-234    00B0A597     56                      PUSH ESI
	$-233    00B0A598     68 E8E64201             PUSH MapleSto.0142E6E8                             ; ASCII "chapter"
	$-22E    00B0A59D     8D8D 5CFFFFFF           LEA ECX,DWORD PTR SS:[EBP-0xA4]
	$-228    00B0A5A3     E8 688C0400             CALL MapleSto.00B53210
	$-223    00B0A5A8     8B7D E4                 MOV EDI,DWORD PTR SS:[EBP-0x1C]
	$-220    00B0A5AB     51                      PUSH ECX
	$-21F    00B0A5AC     8BD4                    MOV EDX,ESP
	$-21D    00B0A5AE     8965 08                 MOV DWORD PTR SS:[EBP+0x8],ESP
	$-21A    00B0A5B1     56                      PUSH ESI
	$-219    00B0A5B2     52                      PUSH EDX
	$-218    00B0A5B3     8BCF                    MOV ECX,EDI
	$-216    00B0A5B5     E8 66D8FFFF             CALL MapleSto.00B07E20
	$-211    00B0A5BA     68 703C3F01             PUSH MapleSto.013F3C70                             ; ASCII "level"
	$-20C    00B0A5BF     8D8D 5CFFFFFF           LEA ECX,DWORD PTR SS:[EBP-0xA4]
	$-206    00B0A5C5     E8 2600A4FF             CALL MapleSto.0054A5F0
	$-201    00B0A5CA     8B45 EC                 MOV EAX,DWORD PTR SS:[EBP-0x14]
	$-1FE    00B0A5CD     85C0                    TEST EAX,EAX
	$-1FC    00B0A5CF     74 18                   JE SHORT MapleSto.00B0A5E9
	$-1FA    00B0A5D1     83C0 14                 ADD EAX,0x14
	$-1F7    00B0A5D4     EB 2C                   JMP SHORT MapleSto.00B0A602
	$-1F5    00B0A5D6     83F9 FF                 CMP ECX,-0x1
	$-1F2    00B0A5D9   ^ 0F84 EEFEFFFF           JE MapleSto.00B0A4CD
	$-1EC    00B0A5DF     B8 01000000             MOV EAX,0x1
	$-1E7    00B0A5E4   ^ E9 E6FEFFFF             JMP MapleSto.00B0A4CF
	$-1E2    00B0A5E9     68 50303F01             PUSH MapleSto.013F3050
	$-1DD    00B0A5EE     8D4D 08                 LEA ECX,DWORD PTR SS:[EBP+0x8]
	$-1DA    00B0A5F1     FF15 D0A13E01           CALL DWORD PTR DS:[0x13EA1D0]                      ; mfc90.#286
	$-1D4    00B0A5F7     C645 FC 0B              MOV BYTE PTR SS:[EBP-0x4],0xB
	$-1D0    00B0A5FB     C745 D4 01000000        MOV DWORD PTR SS:[EBP-0x2C],0x1
	$-1C9    00B0A602     51                      PUSH ECX
	$-1C8    00B0A603     8BCC                    MOV ECX,ESP
	$-1C6    00B0A605     8965 D8                 MOV DWORD PTR SS:[EBP-0x28],ESP
	$-1C3    00B0A608     50                      PUSH EAX
	$-1C2    00B0A609     FF15 C8A13E01           CALL DWORD PTR DS:[0x13EA1C8]                      ; mfc90.#280
	$-1BC    00B0A60F     68 9C0A4201             PUSH MapleSto.01420A9C                             ; ASCII "label"
	$-1B7    00B0A614     8D8D 5CFFFFFF           LEA ECX,DWORD PTR SS:[EBP-0xA4]
	$-1B1    00B0A61A     E8 D1FFA3FF             CALL MapleSto.0054A5F0
	$-1AC    00B0A61F     F645 D4 01              TEST BYTE PTR SS:[EBP-0x2C],0x1
	$-1A8    00B0A623     895D FC                 MOV DWORD PTR SS:[EBP-0x4],EBX
	$-1A5    00B0A626     74 09                   JE SHORT MapleSto.00B0A631
	$-1A3    00B0A628     8D4D 08                 LEA ECX,DWORD PTR SS:[EBP+0x8]
	$-1A0    00B0A62B     FF15 DCA13E01           CALL DWORD PTR DS:[0x13EA1DC]                      ; mfc90.#601
	$-19A    00B0A631     56                      PUSH ESI
	$-199    00B0A632     8BCF                    MOV ECX,EDI
	$-197    00B0A634     E8 57EFFFFF             CALL MapleSto.00B09590
	$-192    00B0A639     F7D8                    NEG EAX
	$-190    00B0A63B     1BC0                    SBB EAX,EAX
	$-18E    00B0A63D     83C0 02                 ADD EAX,0x2
	$-18B    00B0A640     50                      PUSH EAX
	$-18A    00B0A641     68 982F3F01             PUSH MapleSto.013F2F98                             ; ASCII "type"
	$-185    00B0A646     8D8D 5CFFFFFF           LEA ECX,DWORD PTR SS:[EBP-0xA4]
	$-17F    00B0A64C     E8 BF8B0400             CALL MapleSto.00B53210
	$-17A    00B0A651     68 74084801             PUSH MapleSto.01480874                             ; ASCII "qd_renderer_chapter"
	$-175    00B0A656     68 94024301             PUSH MapleSto.01430294                             ; ASCII "rendererClassName"
	$-170    00B0A65B     8D8D 5CFFFFFF           LEA ECX,DWORD PTR SS:[EBP-0xA4]
	$-16A    00B0A661     E8 BAAFABFF             CALL MapleSto.005C5620
	$-165    00B0A666     8D85 34FFFFFF           LEA EAX,DWORD PTR SS:[EBP-0xCC]
	$-15F    00B0A66C     50                      PUSH EAX
	$-15E    00B0A66D     68 30064301             PUSH MapleSto.01430630                             ; ASCII "nodes"
	$-159    00B0A672     8D8D 5CFFFFFF           LEA ECX,DWORD PTR SS:[EBP-0xA4]
	$-153    00B0A678     E8 A3AEF8FF             CALL MapleSto.00A95520
	$-14E    00B0A67D     8D8D 5CFFFFFF           LEA ECX,DWORD PTR SS:[EBP-0xA4]
	$-148    00B0A683     51                      PUSH ECX
	$-147    00B0A684     8B4D 0C                 MOV ECX,DWORD PTR SS:[EBP+0xC]
	$-144    00B0A687     E8 649C3D00             CALL MapleSto.00EE42F0
	$-13F    00B0A68C     8B45 EC                 MOV EAX,DWORD PTR SS:[EBP-0x14]
	$-13C    00B0A68F     C645 FC 06              MOV BYTE PTR SS:[EBP-0x4],0x6
	$-138    00B0A693     85C0                    TEST EAX,EAX
	$-136    00B0A695     74 31                   JE SHORT MapleSto.00B0A6C8
	$-134    00B0A697     83C0 04                 ADD EAX,0x4
	$-131    00B0A69A     50                      PUSH EAX
	$-130    00B0A69B     FF15 6C933E01           CALL DWORD PTR DS:[0x13E936C]
	$-12A    00B0A6A1     85C0                    TEST EAX,EAX
	$-128    00B0A6A3     75 1C                   JNZ SHORT MapleSto.00B0A6C1
	$-126    00B0A6A5     8B55 EC                 MOV EDX,DWORD PTR SS:[EBP-0x14]
	$-123    00B0A6A8     83C2 04                 ADD EDX,0x4
	$-120    00B0A6AB     52                      PUSH EDX
	$-11F    00B0A6AC     FF15 F8923E01           CALL DWORD PTR DS:[0x13E92F8]
	$-119    00B0A6B2     8B4D EC                 MOV ECX,DWORD PTR SS:[EBP-0x14]
	$-116    00B0A6B5     85C9                    TEST ECX,ECX
	$-114    00B0A6B7     74 08                   JE SHORT MapleSto.00B0A6C1
	$-112    00B0A6B9     8B01                    MOV EAX,DWORD PTR DS:[ECX]
	$-110    00B0A6BB     8B10                    MOV EDX,DWORD PTR DS:[EAX]
	$-10E    00B0A6BD     6A 01                   PUSH 0x1
	$-10C    00B0A6BF     FFD2                    CALL EDX
	$-10A    00B0A6C1     C745 EC 00000000        MOV DWORD PTR SS:[EBP-0x14],0x0
	$-103    00B0A6C8     33FF                    XOR EDI,EDI
	$-101    00B0A6CA     8B5D C4                 MOV EBX,DWORD PTR SS:[EBP-0x3C]
	$-FE     00B0A6CD     8B35 E8983E01           MOV ESI,DWORD PTR DS:[0x13E98E8]                   ; msvcr90.free
	$-F8     00B0A6D3     3BDF                    CMP EBX,EDI
	$-F6     00B0A6D5     74 10                   JE SHORT MapleSto.00B0A6E7
	$-F4     00B0A6D7     8B45 C8                 MOV EAX,DWORD PTR SS:[EBP-0x38]
	$-F1     00B0A6DA     50                      PUSH EAX
	$-F0     00B0A6DB     53                      PUSH EBX
	$-EF     00B0A6DC     E8 FF1F91FF             CALL MapleSto.0041C6E0
	$-EA     00B0A6E1     53                      PUSH EBX
	$-E9     00B0A6E2     FFD6                    CALL ESI
	$-E7     00B0A6E4     83C4 0C                 ADD ESP,0xC
	$-E4     00B0A6E7     8B45 B4                 MOV EAX,DWORD PTR SS:[EBP-0x4C]
	$-E1     00B0A6EA     3BC7                    CMP EAX,EDI
	$-DF     00B0A6EC     74 06                   JE SHORT MapleSto.00B0A6F4
	$-DD     00B0A6EE     50                      PUSH EAX
	$-DC     00B0A6EF     FFD6                    CALL ESI
	$-DA     00B0A6F1     83C4 04                 ADD ESP,0x4
	$-D7     00B0A6F4     8D8D 34FFFFFF           LEA ECX,DWORD PTR SS:[EBP-0xCC]
	$-D1     00B0A6FA     C645 FC 03              MOV BYTE PTR SS:[EBP-0x4],0x3
	$-CD     00B0A6FE     E8 0D983D00             CALL MapleSto.00EE3F10
	$-C8     00B0A703     8D8D 5CFFFFFF           LEA ECX,DWORD PTR SS:[EBP-0xA4]
	$-C2     00B0A709     C745 FC FFFFFFFF        MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-BB     00B0A710     E8 FB933D00             CALL MapleSto.00EE3B10
	$-B6     00B0A715     8B45 F0                 MOV EAX,DWORD PTR SS:[EBP-0x10]
	$-B3     00B0A718     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-B0     00B0A71B     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
	$-A9     00B0A722     59                      POP ECX
	$-A8     00B0A723     5F                      POP EDI
	$-A7     00B0A724     5E                      POP ESI
	$-A6     00B0A725     5B                      POP EBX
	$-A5     00B0A726     8BE5                    MOV ESP,EBP
	$-A3     00B0A728     5D                      POP EBP
	$-A2     00B0A729     C2 0800                 RETN 0x8
	$-9F     00B0A72C     CC                      INT3
	$-9E     00B0A72D     CC                      INT3
	$-9D     00B0A72E     CC                      INT3
	$-9C     00B0A72F     CC                      INT3
	$-9B     00B0A730     55                      PUSH EBP
	$-9A     00B0A731     8BEC                    MOV EBP,ESP
	$-98     00B0A733     6A FF                   PUSH -0x1
	$-96     00B0A735     68 D22F3901             PUSH MapleSto.01392FD2
	$-91     00B0A73A     64:A1 00000000          MOV EAX,DWORD PTR FS:[0]
	$-8B     00B0A740     50                      PUSH EAX
	$-8A     00B0A741     83EC 5C                 SUB ESP,0x5C
	$-87     00B0A744     53                      PUSH EBX
	$-86     00B0A745     56                      PUSH ESI
	$-85     00B0A746     57                      PUSH EDI
	$-84     00B0A747     A1 D0687101             MOV EAX,DWORD PTR DS:[0x17168D0]
	$-7F     00B0A74C     33C5                    XOR EAX,EBP
	$-7D     00B0A74E     50                      PUSH EAX
	$-7C     00B0A74F     8D45 F4                 LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-79     00B0A752     64:A3 00000000          MOV DWORD PTR FS:[0],EAX
	$-73     00B0A758     8BF1                    MOV ESI,ECX
	$-71     00B0A75A     8B5D 08                 MOV EBX,DWORD PTR SS:[EBP+0x8]
	$-6E     00B0A75D     8D45 E0                 LEA EAX,DWORD PTR SS:[EBP-0x20]
	$-6B     00B0A760     53                      PUSH EBX
	$-6A     00B0A761     50                      PUSH EAX
	$-69     00B0A762     E8 46F3A2FF             CALL MapleSto.00539AAD
	$-64     00B0A767     83C4 08                 ADD ESP,0x8
	$-61     00B0A76A     33FF                    XOR EDI,EDI
	$-5F     00B0A76C     897D FC                 MOV DWORD PTR SS:[EBP-0x4],EDI
	$-5C     00B0A76F     397D E4                 CMP DWORD PTR SS:[EBP-0x1C],EDI
	$-59     00B0A772     0F84 28030000           JE MapleSto.00B0AAA0
	$-53     00B0A778     8BCE                    MOV ECX,ESI
	$-51     00B0A77A     E8 E1C5FFFF             CALL MapleSto.00B06D60
	$-4C     00B0A77F     68 CC084801             PUSH MapleSto.014808CC                             ; ASCII "m_questInfo"
	$-47     00B0A784     8D4D 08                 LEA ECX,DWORD PTR SS:[EBP+0x8]
	$-44     00B0A787     FF15 20A23E01           CALL DWORD PTR DS:[0x13EA220]                      ; mfc90.#310
	$-3E     00B0A78D     8D4D 08                 LEA ECX,DWORD PTR SS:[EBP+0x8]
	$-3B     00B0A790     51                      PUSH ECX
	$-3A     00B0A791     8D55 D8                 LEA EDX,DWORD PTR SS:[EBP-0x28]
	$-37     00B0A794     52                      PUSH EDX
	$-36     00B0A795     8BCE                    MOV ECX,ESI
	$-34     00B0A797     C645 FC 01              MOV BYTE PTR SS:[EBP-0x4],0x1
	$-30     00B0A79B     E8 F0BCB8FF             CALL MapleSto.00696490
	$-2B     00B0A7A0     8D4D 08                 LEA ECX,DWORD PTR SS:[EBP+0x8]
	$-28     00B0A7A3     C645 FC 03              MOV BYTE PTR SS:[EBP-0x4],0x3
	$-24     00B0A7A7     FF15 08A23E01           CALL DWORD PTR DS:[0x13EA208]                      ; mfc90.#601
	$-1E     00B0A7AD     8B45 DC                 MOV EAX,DWORD PTR SS:[EBP-0x24]
	$-1B     00B0A7B0     3BC7                    CMP EAX,EDI
	$-19     00B0A7B2     0F84 7F020000           JE MapleSto.00B0AA37
	$-13     00B0A7B8     6A 01                   PUSH 0x1
	$-11     00B0A7BA     8D4D 98                 LEA ECX,DWORD PTR SS:[EBP-0x68]
	$-E      00B0A7BD     E8 7E8D3D00             CALL MapleSto.00EE3540
	$-9      00B0A7C2     53                      PUSH EBX
	$-8      00B0A7C3     68 18303F01             PUSH MapleSto.013F3018                             ; ASCII "id"
	$-3      00B0A7C8     8D4D 98                 LEA ECX,DWORD PTR SS:[EBP-0x68]
	$ ==>    00B0A7CB     C645 FC 04              MOV BYTE PTR SS:[EBP-0x4],0x4
	$+4      00B0A7CF     E8 0CCAABFF             CALL MapleSto.005C71E0
	$+9      00B0A7D4     51                      PUSH ECX
	$+A      00B0A7D5     8BC4                    MOV EAX,ESP
	$+C      00B0A7D7     8965 F0                 MOV DWORD PTR SS:[EBP-0x10],ESP
	$+F      00B0A7DA     53                      PUSH EBX
	$+10     00B0A7DB     50                      PUSH EAX
	$+11     00B0A7DC     E8 AF02EAFF             CALL MapleSto.009AAA90                             ; g_c_get_quest_name_by_id_
	$+16     00B0A7E1     83C4 08                 ADD ESP,0x8
	$+19     00B0A7E4     68 7C2E3F01             PUSH MapleSto.013F2E7C                             ; ASCII "title"
	$+1E     00B0A7E9     8D4D 98                 LEA ECX,DWORD PTR SS:[EBP-0x68]
	$+21     00B0A7EC     E8 FFFDA3FF             CALL MapleSto.0054A5F0
	$+26     00B0A7F1     8B4D E4                 MOV ECX,DWORD PTR SS:[EBP-0x1C]
	$+29     00B0A7F4     83C1 2C                 ADD ECX,0x2C
	$+2C     00B0A7F7     51                      PUSH ECX
	$+2D     00B0A7F8     8BD4                    MOV EDX,ESP
	$+2F     00B0A7FA     8965 F0                 MOV DWORD PTR SS:[EBP-0x10],ESP
	$+32     00B0A7FD     51                      PUSH ECX
	$+33     00B0A7FE     68 2C040000             PUSH 0x42C
	$+38     00B0A803     52                      PUSH EDX
	$+39     00B0A804     E8 B719A6FF             CALL MapleSto.0056C1C0
	$+3E     00B0A809     83C4 0C                 ADD ESP,0xC
	$+41     00B0A80C     68 703C3F01             PUSH MapleSto.013F3C70                             ; ASCII "level"
	$+46     00B0A811     8D4D 98                 LEA ECX,DWORD PTR SS:[EBP-0x68]
	$+49     00B0A814     E8 D7FDA3FF             CALL MapleSto.0054A5F0
	$+4E     00B0A819     57                      PUSH EDI
	$+4F     00B0A81A     68 C0084801             PUSH MapleSto.014808C0                             ; ASCII "hideReward"
	$+54     00B0A81F     8D4D 98                 LEA ECX,DWORD PTR SS:[EBP-0x68]
	$+57     00B0A822     E8 E9890400             CALL MapleSto.00B53210
	$+5C     00B0A827     8B45 E4                 MOV EAX,DWORD PTR SS:[EBP-0x1C]
	$+5F     00B0A82A     8B48 38                 MOV ECX,DWORD PTR DS:[EAX+0x38]
	$+62     00B0A82D     51                      PUSH ECX
	$+63     00B0A82E     68 A8AF4101             PUSH MapleSto.0141AFA8                             ; ASCII "disableGiveup"
	$+68     00B0A833     8D4D 98                 LEA ECX,DWORD PTR SS:[EBP-0x68]
	$+6B     00B0A836     E8 D5890400             CALL MapleSto.00B53210
	$+70     00B0A83B     8B55 E4                 MOV EDX,DWORD PTR SS:[EBP-0x1C]
	$+73     00B0A83E     397A 1C                 CMP DWORD PTR DS:[EDX+0x1C],EDI
	$+76     00B0A841     0F94C0                  SETE AL
	$+79     00B0A844     0FB6C8                  MOVZX ECX,AL
	$+7C     00B0A847     51                      PUSH ECX
	$+7D     00B0A848     68 D06F4101             PUSH MapleSto.01416FD0                             ; ASCII "main"
	$+82     00B0A84D     8D4D 98                 LEA ECX,DWORD PTR SS:[EBP-0x68]
	$+85     00B0A850     E8 7BFEA3FF             CALL MapleSto.0054A6D0
	$+8A     00B0A855     8BCE                    MOV ECX,ESI
	$+8C     00B0A857     E8 04D8FFFF             CALL MapleSto.00B08060
	$+91     00B0A85C     2BC7                    SUB EAX,EDI
	$+93     00B0A85E     75 41                   JNZ SHORT MapleSto.00B0A8A1
	$+95     00B0A860     68 50303F01             PUSH MapleSto.013F3050
	$+9A     00B0A865     68 302F4701             PUSH MapleSto.01472F30                             ; ASCII "date"
	$+9F     00B0A86A     8D4D 98                 LEA ECX,DWORD PTR SS:[EBP-0x68]
	$+A2     00B0A86D     E8 EEACABFF             CALL MapleSto.005C5560
	$+A7     00B0A872     51                      PUSH ECX
	$+A8     00B0A873     8BD4                    MOV EDX,ESP
	$+AA     00B0A875     8965 F0                 MOV DWORD PTR SS:[EBP-0x10],ESP
	$+AD     00B0A878     53                      PUSH EBX
	$+AE     00B0A879     52                      PUSH EDX
	$+AF     00B0A87A     8BCE                    MOV ECX,ESI
	$+B1     00B0A87C     E8 9FD1FFFF             CALL MapleSto.00B07A20
	$+B6     00B0A881     68 B8084801             PUSH MapleSto.014808B8                             ; ASCII "story"
	$+BB     00B0A886     8D4D 98                 LEA ECX,DWORD PTR SS:[EBP-0x68]
	$+BE     00B0A889     E8 62FDA3FF             CALL MapleSto.0054A5F0
	$+C3     00B0A88E     57                      PUSH EDI
	$+C4     00B0A88F     68 F82E4701             PUSH MapleSto.01472EF8                             ; ASCII "progress"
	$+C9     00B0A894     8D4D 98                 LEA ECX,DWORD PTR SS:[EBP-0x68]
	$+CC     00B0A897     E8 74890400             CALL MapleSto.00B53210
	$+D1     00B0A89C     E9 43010000             JMP MapleSto.00B0A9E4
	$+D6     00B0A8A1     8B0D 0C457201           MOV ECX,DWORD PTR DS:[0x172450C]
	$+DC     00B0A8A7     8B41 08                 MOV EAX,DWORD PTR DS:[ECX+0x8]
	$+DF     00B0A8AA     8B40 0C                 MOV EAX,DWORD PTR DS:[EAX+0xC]
	$+E2     00B0A8AD     83C1 08                 ADD ECX,0x8
	$+E5     00B0A8B0     53                      PUSH EBX
	$+E6     00B0A8B1     8D55 E8                 LEA EDX,DWORD PTR SS:[EBP-0x18]
	$+E9     00B0A8B4     52                      PUSH EDX
	$+EA     00B0A8B5     FFD0                    CALL EAX
	$+EC     00B0A8B7     C645 FC 05              MOV BYTE PTR SS:[EBP-0x4],0x5
	$+F0     00B0A8BB     397D EC                 CMP DWORD PTR SS:[EBP-0x14],EDI
	$+F3     00B0A8BE     75 47                   JNZ SHORT MapleSto.00B0A907
	$+F5     00B0A8C0     8D4D E8                 LEA ECX,DWORD PTR SS:[EBP-0x18]
	$+F8     00B0A8C3     C645 FC 04              MOV BYTE PTR SS:[EBP-0x4],0x4
	$+FC     00B0A8C7     E8 4428F7FF             CALL MapleSto.00A7D110
	$+101    00B0A8CC     8D4D 98                 LEA ECX,DWORD PTR SS:[EBP-0x68]
	$+104    00B0A8CF     C645 FC 03              MOV BYTE PTR SS:[EBP-0x4],0x3
	$+108    00B0A8D3     E8 38923D00             CALL MapleSto.00EE3B10
	$+10D    00B0A8D8     8D4D D8                 LEA ECX,DWORD PTR SS:[EBP-0x28]
	$+110    00B0A8DB     C645 FC 00              MOV BYTE PTR SS:[EBP-0x4],0x0
	$+114    00B0A8DF     E8 2C28F7FF             CALL MapleSto.00A7D110
	$+119    00B0A8E4     8D4D E0                 LEA ECX,DWORD PTR SS:[EBP-0x20]
	$+11C    00B0A8E7     C745 FC FFFFFFFF        MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+123    00B0A8EE     E8 1D28F7FF             CALL MapleSto.00A7D110
	$+128    00B0A8F3     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+12B    00B0A8F6     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
	$+132    00B0A8FD     59                      POP ECX
	$+133    00B0A8FE     5F                      POP EDI
	$+134    00B0A8FF     5E                      POP ESI
	$+135    00B0A900     5B                      POP EBX
	$+136    00B0A901     8BE5                    MOV ESP,EBP
	$+138    00B0A903     5D                      POP EBP
	$+139    00B0A904     C2 0400                 RETN 0x4
	$+13C    00B0A907     8B0D 2C417201           MOV ECX,DWORD PTR DS:[0x172412C]
	$+142    00B0A90D     E8 43B83901             CALL MapleSto.01EA6155
	$+147    00B0A912     8BF8                    MOV EDI,EAX
	$+149    00B0A914     8B45 EC                 MOV EAX,DWORD PTR SS:[EBP-0x14]
	$+14C    00B0A917     8B48 38                 MOV ECX,DWORD PTR DS:[EAX+0x38]
	$+14F    00B0A91A     8B40 3C                 MOV EAX,DWORD PTR DS:[EAX+0x3C]
	$+152    00B0A91D     51                      PUSH ECX
	$+153    00B0A91E     8BD4                    MOV EDX,ESP
	$+155    00B0A920     8965 F0                 MOV DWORD PTR SS:[EBP-0x10],ESP
	$+158    00B0A923     50                      PUSH EAX
	$+159    00B0A924     51                      PUSH ECX
	$+15A    00B0A925     52                      PUSH EDX
	$+15B    00B0A926     8BCE                    MOV ECX,ESI
	$+15D    00B0A928     E8 63C1FFFF             CALL MapleSto.00B06A90
	$+162    00B0A92D     68 302F4701             PUSH MapleSto.01472F30                             ; ASCII "date"
	$+167    00B0A932     8D4D 98                 LEA ECX,DWORD PTR SS:[EBP-0x68]
	$+16A    00B0A935     E8 B6FCA3FF             CALL MapleSto.0054A5F0
	$+16F    00B0A93A     51                      PUSH ECX
	$+170    00B0A93B     8B4D EC                 MOV ECX,DWORD PTR SS:[EBP-0x14]
	$+173    00B0A93E     8BC4                    MOV EAX,ESP
	$+175    00B0A940     8965 F0                 MOV DWORD PTR SS:[EBP-0x10],ESP
	$+178    00B0A943     51                      PUSH ECX
	$+179    00B0A944     50                      PUSH EAX
	$+17A    00B0A945     8BCE                    MOV ECX,ESI
	$+17C    00B0A947     E8 84CEFFFF             CALL MapleSto.00B077D0
	$+181    00B0A94C     68 B8084801             PUSH MapleSto.014808B8                             ; ASCII "story"
	$+186    00B0A951     8D4D 98                 LEA ECX,DWORD PTR SS:[EBP-0x68]
	$+189    00B0A954     E8 97FCA3FF             CALL MapleSto.0054A5F0
	$+18E    00B0A959     8B4D EC                 MOV ECX,DWORD PTR SS:[EBP-0x14]
	$+191    00B0A95C     8B51 0C                 MOV EDX,DWORD PTR DS:[ECX+0xC]
	$+194    00B0A95F     8B42 0C                 MOV EAX,DWORD PTR DS:[EDX+0xC]
	$+197    00B0A962     83C1 0C                 ADD ECX,0xC
	$+19A    00B0A965     FFD0                    CALL EAX
	$+19C    00B0A967     85C0                    TEST EAX,EAX
	$+19E    00B0A969     0F94C1                  SETE CL
	$+1A1    00B0A96C     0FB6D1                  MOVZX EDX,CL
	$+1A4    00B0A96F     52                      PUSH EDX
	$+1A5    00B0A970     68 F82E4701             PUSH MapleSto.01472EF8                             ; ASCII "progress"
	$+1AA    00B0A975     8D4D 98                 LEA ECX,DWORD PTR SS:[EBP-0x68]
	$+1AD    00B0A978     E8 53FDA3FF             CALL MapleSto.0054A6D0
	$+1B2    00B0A97D     8B4D EC                 MOV ECX,DWORD PTR SS:[EBP-0x14]
	$+1B5    00B0A980     8B41 0C                 MOV EAX,DWORD PTR DS:[ECX+0xC]
	$+1B8    00B0A983     8B50 14                 MOV EDX,DWORD PTR DS:[EAX+0x14]
	$+1BB    00B0A986     83C1 0C                 ADD ECX,0xC
	$+1BE    00B0A989     57                      PUSH EDI
	$+1BF    00B0A98A     FFD2                    CALL EDX
	$+1C1    00B0A98C     50                      PUSH EAX
	$+1C2    00B0A98D     68 A8084801             PUSH MapleSto.014808A8                             ; ASCII "enableGotoNpc"
	$+1C7    00B0A992     8D4D 98                 LEA ECX,DWORD PTR SS:[EBP-0x68]
	$+1CA    00B0A995     E8 76880400             CALL MapleSto.00B53210
	$+1CF    00B0A99A     8B4D EC                 MOV ECX,DWORD PTR SS:[EBP-0x14]
	$+1D2    00B0A99D     8B41 0C                 MOV EAX,DWORD PTR DS:[ECX+0xC]
	$+1D5    00B0A9A0     8B50 18                 MOV EDX,DWORD PTR DS:[EAX+0x18]
	$+1D8    00B0A9A3     83C1 0C                 ADD ECX,0xC
	$+1DB    00B0A9A6     57                      PUSH EDI
	$+1DC    00B0A9A7     FFD2                    CALL EDX
	$+1DE    00B0A9A9     85C0                    TEST EAX,EAX
	$+1E0    00B0A9AB     74 0D                   JE SHORT MapleSto.00B0A9BA
	$+1E2    00B0A9AD     E8 4E2EA9FF             CALL MapleSto.0059D800
	$+1E7    00B0A9B2     84C0                    TEST AL,AL
	$+1E9    00B0A9B4     74 04                   JE SHORT MapleSto.00B0A9BA
	$+1EB    00B0A9B6     B0 01                   MOV AL,0x1
	$+1ED    00B0A9B8     EB 02                   JMP SHORT MapleSto.00B0A9BC
	$+1EF    00B0A9BA     32C0                    XOR AL,AL
	$+1F1    00B0A9BC     50                      PUSH EAX
	$+1F2    00B0A9BD     68 94084801             PUSH MapleSto.01480894                             ; ASCII "enableGotoDungeon"
	$+1F7    00B0A9C2     8D4D 98                 LEA ECX,DWORD PTR SS:[EBP-0x68]
	$+1FA    00B0A9C5     E8 06FDA3FF             CALL MapleSto.0054A6D0
	$+1FF    00B0A9CA     8D45 98                 LEA EAX,DWORD PTR SS:[EBP-0x68]
	$+202    00B0A9CD     50                      PUSH EAX
	$+203    00B0A9CE     53                      PUSH EBX
	$+204    00B0A9CF     8BCE                    MOV ECX,ESI
	$+206    00B0A9D1     E8 0AC4FFFF             CALL MapleSto.00B06DE0
	$+20B    00B0A9D6     8D4D E8                 LEA ECX,DWORD PTR SS:[EBP-0x18]
	$+20E    00B0A9D9     C645 FC 04              MOV BYTE PTR SS:[EBP-0x4],0x4
	$+212    00B0A9DD     E8 2E27F7FF             CALL MapleSto.00A7D110
	$+217    00B0A9E2     33FF                    XOR EDI,EDI
	$+219    00B0A9E4     8D4D 98                 LEA ECX,DWORD PTR SS:[EBP-0x68]
	$+21C    00B0A9E7     51                      PUSH ECX
	$+21D    00B0A9E8     53                      PUSH EBX
	$+21E    00B0A9E9     8BCE                    MOV ECX,ESI
	$+220    00B0A9EB     E8 70D8FFFF             CALL MapleSto.00B08260
	$+225    00B0A9F0     8B4D DC                 MOV ECX,DWORD PTR SS:[EBP-0x24]
	$+228    00B0A9F3     8D55 98                 LEA EDX,DWORD PTR SS:[EBP-0x68]
	$+22B    00B0A9F6     52                      PUSH EDX
	$+22C    00B0A9F7     68 2CFB4201             PUSH MapleSto.0142FB2C                             ; ASCII "setData"
	$+231    00B0A9FC     8D45 C0                 LEA EAX,DWORD PTR SS:[EBP-0x40]
	$+234    00B0A9FF     50                      PUSH EAX
	$+235    00B0AA00     E8 2B0EBBFF             CALL MapleSto.006BB830
	$+23A    00B0AA05     8B4D C4                 MOV ECX,DWORD PTR SS:[EBP-0x3C]
	$+23D    00B0AA08     C1E9 06                 SHR ECX,0x6
	$+240    00B0AA0B     F6C1 01                 TEST CL,0x1
	$+243    00B0AA0E     74 15                   JE SHORT MapleSto.00B0AA25
	$+245    00B0AA10     8B45 C8                 MOV EAX,DWORD PTR SS:[EBP-0x38]
	$+248    00B0AA13     8B4D C0                 MOV ECX,DWORD PTR SS:[EBP-0x40]
	$+24B    00B0AA16     8B11                    MOV EDX,DWORD PTR DS:[ECX]
	$+24D    00B0AA18     8B52 08                 MOV EDX,DWORD PTR DS:[EDX+0x8]
	$+250    00B0AA1B     50                      PUSH EAX
	$+251    00B0AA1C     8D45 C0                 LEA EAX,DWORD PTR SS:[EBP-0x40]
	$+254    00B0AA1F     50                      PUSH EAX
	$+255    00B0AA20     FFD2                    CALL EDX
	$+257    00B0AA22     897D C0                 MOV DWORD PTR SS:[EBP-0x40],EDI
	$+25A    00B0AA25     8D4D 98                 LEA ECX,DWORD PTR SS:[EBP-0x68]
	$+25D    00B0AA28     897D C4                 MOV DWORD PTR SS:[EBP-0x3C],EDI
	$+260    00B0AA2B     C645 FC 03              MOV BYTE PTR SS:[EBP-0x4],0x3
	$+264    00B0AA2F     E8 DC903D00             CALL MapleSto.00EE3B10
	$+269    00B0AA34     8B45 DC                 MOV EAX,DWORD PTR SS:[EBP-0x24]
	$+26C    00B0AA37     8B1D 6C933E01           MOV EBX,DWORD PTR DS:[0x13E936C]
	$+272    00B0AA3D     8B35 F8923E01           MOV ESI,DWORD PTR DS:[0x13E92F8]
	$+278    00B0AA43     C645 FC 00              MOV BYTE PTR SS:[EBP-0x4],0x0
	$+27C    00B0AA47     3BC7                    CMP EAX,EDI
	$+27E    00B0AA49     74 25                   JE SHORT MapleSto.00B0AA70
	$+280    00B0AA4B     83C0 04                 ADD EAX,0x4
	$+283    00B0AA4E     50                      PUSH EAX
	$+284    00B0AA4F     FFD3                    CALL EBX
	$+286    00B0AA51     85C0                    TEST EAX,EAX
	$+288    00B0AA53     75 18                   JNZ SHORT MapleSto.00B0AA6D
	$+28A    00B0AA55     8B45 DC                 MOV EAX,DWORD PTR SS:[EBP-0x24]
	$+28D    00B0AA58     83C0 04                 ADD EAX,0x4
	$+290    00B0AA5B     50                      PUSH EAX
	$+291    00B0AA5C     FFD6                    CALL ESI
	$+293    00B0AA5E     8B4D DC                 MOV ECX,DWORD PTR SS:[EBP-0x24]
	$+296    00B0AA61     3BCF                    CMP ECX,EDI
	$+298    00B0AA63     74 08                   JE SHORT MapleSto.00B0AA6D
	$+29A    00B0AA65     8B11                    MOV EDX,DWORD PTR DS:[ECX]
	$+29C    00B0AA67     8B02                    MOV EAX,DWORD PTR DS:[EDX]
	$+29E    00B0AA69     6A 01                   PUSH 0x1
	$+2A0    00B0AA6B     FFD0                    CALL EAX
	$+2A2    00B0AA6D     897D DC                 MOV DWORD PTR SS:[EBP-0x24],EDI
	$+2A5    00B0AA70     8B45 E4                 MOV EAX,DWORD PTR SS:[EBP-0x1C]
	$+2A8    00B0AA73     C745 FC FFFFFFFF        MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+2AF    00B0AA7A     3BC7                    CMP EAX,EDI
	$+2B1    00B0AA7C     74 22                   JE SHORT MapleSto.00B0AAA0
	$+2B3    00B0AA7E     83C0 04                 ADD EAX,0x4
	$+2B6    00B0AA81     50                      PUSH EAX
	$+2B7    00B0AA82     FFD3                    CALL EBX
	$+2B9    00B0AA84     85C0                    TEST EAX,EAX
	$+2BB    00B0AA86     75 18                   JNZ SHORT MapleSto.00B0AAA0
	$+2BD    00B0AA88     8B4D E4                 MOV ECX,DWORD PTR SS:[EBP-0x1C]
	$+2C0    00B0AA8B     83C1 04                 ADD ECX,0x4
	$+2C3    00B0AA8E     51                      PUSH ECX
	$+2C4    00B0AA8F     FFD6                    CALL ESI
	$+2C6    00B0AA91     8B4D E4                 MOV ECX,DWORD PTR SS:[EBP-0x1C]
	$+2C9    00B0AA94     3BCF                    CMP ECX,EDI
	$+2CB    00B0AA96     74 08                   JE SHORT MapleSto.00B0AAA0
	$+2CD    00B0AA98     8B11                    MOV EDX,DWORD PTR DS:[ECX]
	$+2CF    00B0AA9A     8B02                    MOV EAX,DWORD PTR DS:[EDX]
	$+2D1    00B0AA9C     6A 01                   PUSH 0x1
	$+2D3    00B0AA9E     FFD0                    CALL EAX
	$+2D5    00B0AAA0     8B4D F4                 MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+2D8    00B0AAA3     64:890D 00000000        MOV DWORD PTR FS:[0],ECX
	$+2DF    00B0AAAA     59                      POP ECX
	$+2E0    00B0AAAB     5F                      POP EDI
	$+2E1    00B0AAAC     5E                      POP ESI
	$+2E2    00B0AAAD     5B                      POP EBX
	$+2E3    00B0AAAE     8BE5                    MOV ESP,EBP
	$+2E5    00B0AAB0     5D                      POP EBP
	$+2E6    00B0AAB1     C2 0400                 RETN 0x4
	$+2E9    00B0AAB4     CC                      INT3
	$+2EA    00B0AAB5     CC                      INT3
	*/
	addr_tmp = scan_exe_.ScanCall("C645FC04E8????????518BC48965F05350", 0x11 + 1);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_c_get_quest_name_by_id_ = " << addr_tmp << ";" << FC_OFstream_EndlLog;
	return true;
}

bool CAutoUpdateApp::Scan_Db(Cmn_FC_OFstream& ofs)
{
	pt_pcvoid addr_tmp, addr_tmp1;
	pt_dword dw_tmp;
	//boost::optional<pt_dword> op_dw_tmp;

	/*
	$-353    00AE815F      CC                    INT3
	$-352    00AE8160      55                    PUSH EBP
	$-351    00AE8161      8BEC                  MOV EBP,ESP
	$-34F    00AE8163      83EC 18               SUB ESP,0x18
	$-34C    00AE8166      56                    PUSH ESI
	$-34B    00AE8167      8BF1                  MOV ESI,ECX
	$-349    00AE8169      E8 C2FEFFFF           CALL MapleSto.00AE8030
	$-344    00AE816E      8B4E 20               MOV ECX,DWORD PTR DS:[ESI+0x20]
	$-341    00AE8171      5E                    POP ESI
	$-340    00AE8172      85C9                  TEST ECX,ECX
	$-33E    00AE8174      74 2B                 JE SHORT MapleSto.00AE81A1
	$-33C    00AE8176      68 D4DD4701           PUSH MapleSto.0147DDD4                   ; ASCII "changeStageSize"
	$-337    00AE817B      8D45 E8               LEA EAX,DWORD PTR SS:[EBP-0x18]
	$-334    00AE817E      50                    PUSH EAX
	$-333    00AE817F      E8 FCB73F00           CALL MapleSto.00EE3980
	$-32E    00AE8184      8B4D EC               MOV ECX,DWORD PTR SS:[EBP-0x14]
	$-32B    00AE8187      C1E9 06               SHR ECX,0x6
	$-328    00AE818A      F6C1 01               TEST CL,0x1
	$-325    00AE818D      74 12                 JE SHORT MapleSto.00AE81A1
	$-323    00AE818F      8B45 F0               MOV EAX,DWORD PTR SS:[EBP-0x10]
	$-320    00AE8192      8B4D E8               MOV ECX,DWORD PTR SS:[EBP-0x18]
	$-31D    00AE8195      8B11                  MOV EDX,DWORD PTR DS:[ECX]
	$-31B    00AE8197      8B52 08               MOV EDX,DWORD PTR DS:[EDX+0x8]
	$-318    00AE819A      50                    PUSH EAX
	$-317    00AE819B      8D45 E8               LEA EAX,DWORD PTR SS:[EBP-0x18]
	$-314    00AE819E      50                    PUSH EAX
	$-313    00AE819F      FFD2                  CALL EDX
	$-311    00AE81A1      8BE5                  MOV ESP,EBP
	$-30F    00AE81A3      5D                    POP EBP
	$-30E    00AE81A4      C3                    RETN
	$-30D    00AE81A5      CC                    INT3
	$-30C    00AE81A6      CC                    INT3
	$-30B    00AE81A7      CC                    INT3
	$-30A    00AE81A8      CC                    INT3
	$-309    00AE81A9      CC                    INT3
	$-308    00AE81AA      CC                    INT3
	$-307    00AE81AB      CC                    INT3
	$-306    00AE81AC      CC                    INT3
	$-305    00AE81AD      CC                    INT3
	$-304    00AE81AE      CC                    INT3
	$-303    00AE81AF      CC                    INT3
	$-302    00AE81B0      56                    PUSH ESI
	$-301    00AE81B1      8BF1                  MOV ESI,ECX
	$-2FF    00AE81B3      E8 E824FAFF           CALL MapleSto.00A8A6A0
	$-2FA    00AE81B8      85C0                  TEST EAX,EAX
	$-2F8    00AE81BA      74 07                 JE SHORT MapleSto.00AE81C3
	$-2F6    00AE81BC      B8 01000000           MOV EAX,0x1
	$-2F1    00AE81C1      5E                    POP ESI
	$-2F0    00AE81C2      C3                    RETN
	$-2EF    00AE81C3      8B06                  MOV EAX,DWORD PTR DS:[ESI]
	$-2ED    00AE81C5      8B90 D0000000         MOV EDX,DWORD PTR DS:[EAX+0xD0]
	$-2E7    00AE81CB      8BCE                  MOV ECX,ESI
	$-2E5    00AE81CD      FFD2                  CALL EDX
	$-2E3    00AE81CF      85C0                  TEST EAX,EAX
	$-2E1    00AE81D1      75 72                 JNZ SHORT MapleSto.00AE8245
	$-2DF    00AE81D3      3946 20               CMP DWORD PTR DS:[ESI+0x20],EAX
	$-2DC    00AE81D6      74 6D                 JE SHORT MapleSto.00AE8245
	$-2DA    00AE81D8      3986 60010000         CMP DWORD PTR DS:[ESI+0x160],EAX
	$-2D4    00AE81DE      75 65                 JNZ SHORT MapleSto.00AE8245
	$-2D2    00AE81E0      3986 5C010000         CMP DWORD PTR DS:[ESI+0x15C],EAX
	$-2CC    00AE81E6      74 08                 JE SHORT MapleSto.00AE81F0
	$-2CA    00AE81E8      3905 F89E7301         CMP DWORD PTR DS:[0x1739EF8],EAX
	$-2C4    00AE81EE      74 4E                 JE SHORT MapleSto.00AE823E
	$-2C2    00AE81F0      8B0D 2C407201         MOV ECX,DWORD PTR DS:[0x172402C]
	$-2BC    00AE81F6      6A 00                 PUSH 0x0
	$-2BA    00AE81F8      6A 00                 PUSH 0x0
	$-2B8    00AE81FA      68 07940400           PUSH 0x49407
	$-2B3    00AE81FF      E8 4CB6CBFF           CALL MapleSto.007A3850
	$-2AE    00AE8204      84C0                  TEST AL,AL
	$-2AC    00AE8206      74 1E                 JE SHORT MapleSto.00AE8226
	$-2AA    00AE8208      8B46 14               MOV EAX,DWORD PTR DS:[ESI+0x14]
	$-2A7    00AE820B      8B0D 28277201         MOV ECX,DWORD PTR DS:[0x1722728]
	$-2A1    00AE8211      50                    PUSH EAX
	$-2A0    00AE8212      E8 4FD33D01           CALL MapleSto.01EC5566
	$-29B    00AE8217      85C0                  TEST EAX,EAX
	$-299    00AE8219      75 2A                 JNZ SHORT MapleSto.00AE8245
	$-297    00AE821B      8BCE                  MOV ECX,ESI
	$-295    00AE821D      E8 9EFEFFFF           CALL MapleSto.00AE80C0
	$-290    00AE8222      33C0                  XOR EAX,EAX
	$-28E    00AE8224      5E                    POP ESI
	$-28D    00AE8225      C3                    RETN
	$-28C    00AE8226      8B0D 2C407201         MOV ECX,DWORD PTR DS:[0x172402C]
	$-286    00AE822C      6A 00                 PUSH 0x0
	$-284    00AE822E      6A 00                 PUSH 0x0
	$-282    00AE8230      68 07940400           PUSH 0x49407
	$-27D    00AE8235      E8 66B6CBFF           CALL MapleSto.007A38A0
	$-278    00AE823A      84C0                  TEST AL,AL
	$-276    00AE823C      74 07                 JE SHORT MapleSto.00AE8245
	$-274    00AE823E      8BCE                  MOV ECX,ESI
	$-272    00AE8240      E8 EBFDFFFF           CALL MapleSto.00AE8030
	$-26D    00AE8245      33C0                  XOR EAX,EAX
	$-26B    00AE8247      5E                    POP ESI
	$-26A    00AE8248      C3                    RETN
	$-269    00AE8249      CC                    INT3
	$-268    00AE824A      CC                    INT3
	$-267    00AE824B      CC                    INT3
	$-266    00AE824C      CC                    INT3
	$-265    00AE824D      CC                    INT3
	$-264    00AE824E      CC                    INT3
	$-263    00AE824F      CC                    INT3
	$-262    00AE8250      55                    PUSH EBP
	$-261    00AE8251      8BEC                  MOV EBP,ESP
	$-25F    00AE8253      6A FF                 PUSH -0x1
	$-25D    00AE8255      68 2FF13801           PUSH MapleSto.0138F12F
	$-258    00AE825A      64:A1 00000000        MOV EAX,DWORD PTR FS:[0]
	$-252    00AE8260      50                    PUSH EAX
	$-251    00AE8261      83EC 1C               SUB ESP,0x1C
	$-24E    00AE8264      56                    PUSH ESI
	$-24D    00AE8265      57                    PUSH EDI
	$-24C    00AE8266      A1 D0687101           MOV EAX,DWORD PTR DS:[0x17168D0]
	$-247    00AE826B      33C5                  XOR EAX,EBP
	$-245    00AE826D      50                    PUSH EAX
	$-244    00AE826E      8D45 F4               LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-241    00AE8271      64:A3 00000000        MOV DWORD PTR FS:[0],EAX
	$-23B    00AE8277      8BF9                  MOV EDI,ECX
	$-239    00AE8279      837D 0C 00            CMP DWORD PTR SS:[EBP+0xC],0x0
	$-235    00AE827D      C745 FC 00000000      MOV DWORD PTR SS:[EBP-0x4],0x0
	$-22E    00AE8284      0F84 D9010000         JE MapleSto.00AE8463
	$-228    00AE828A      51                    PUSH ECX
	$-227    00AE828B      8D47 14               LEA EAX,DWORD PTR DS:[EDI+0x14]
	$-224    00AE828E      8BCC                  MOV ECX,ESP
	$-222    00AE8290      8965 F0               MOV DWORD PTR SS:[EBP-0x10],ESP
	$-21F    00AE8293      50                    PUSH EAX
	$-21E    00AE8294      FF15 ECA13E01         CALL DWORD PTR DS:[0x13EA1EC]            ; mfc90.#300
	$-218    00AE829A      B9 9C8C7301           MOV ECX,MapleSto.01738C9C
	$-213    00AE829F      FF15 F4A13E01         CALL DWORD PTR DS:[0x13EA1F4]            ; mfc90.#3726
	$-20D    00AE82A5      8B75 08               MOV ESI,DWORD PTR SS:[EBP+0x8]
	$-20A    00AE82A8      50                    PUSH EAX
	$-209    00AE82A9      8BCE                  MOV ECX,ESI
	$-207    00AE82AB      E8 6022A6FF           CALL MapleSto.0054A510
	$-202    00AE82B0      51                    PUSH ECX
	$-201    00AE82B1      8D57 18               LEA EDX,DWORD PTR DS:[EDI+0x18]
	$-1FE    00AE82B4      8BCC                  MOV ECX,ESP
	$-1FC    00AE82B6      8965 08               MOV DWORD PTR SS:[EBP+0x8],ESP
	$-1F9    00AE82B9      52                    PUSH EDX
	$-1F8    00AE82BA      FF15 ECA13E01         CALL DWORD PTR DS:[0x13EA1EC]            ; mfc90.#300
	$-1F2    00AE82C0      B9 A08C7301           MOV ECX,MapleSto.01738CA0
	$-1ED    00AE82C5      FF15 F4A13E01         CALL DWORD PTR DS:[0x13EA1F4]            ; mfc90.#3726
	$-1E7    00AE82CB      50                    PUSH EAX
	$-1E6    00AE82CC      8BCE                  MOV ECX,ESI
	$-1E4    00AE82CE      E8 3D22A6FF           CALL MapleSto.0054A510
	$-1DF    00AE82D3      51                    PUSH ECX
	$-1DE    00AE82D4      8D47 1C               LEA EAX,DWORD PTR DS:[EDI+0x1C]
	$-1DB    00AE82D7      8BCC                  MOV ECX,ESP
	$-1D9    00AE82D9      8965 08               MOV DWORD PTR SS:[EBP+0x8],ESP
	$-1D6    00AE82DC      50                    PUSH EAX
	$-1D5    00AE82DD      FF15 ECA13E01         CALL DWORD PTR DS:[0x13EA1EC]            ; mfc90.#300
	$-1CF    00AE82E3      B9 A48C7301           MOV ECX,MapleSto.01738CA4
	$-1CA    00AE82E8      FF15 F4A13E01         CALL DWORD PTR DS:[0x13EA1F4]            ; mfc90.#3726
	$-1C4    00AE82EE      50                    PUSH EAX
	$-1C3    00AE82EF      8BCE                  MOV ECX,ESI
	$-1C1    00AE82F1      E8 1A22A6FF           CALL MapleSto.0054A510
	$-1BC    00AE82F6      8B4F 34               MOV ECX,DWORD PTR DS:[EDI+0x34]
	$-1B9    00AE82F9      51                    PUSH ECX
	$-1B8    00AE82FA      B9 A88C7301           MOV ECX,MapleSto.01738CA8
	$-1B3    00AE82FF      FF15 F4A13E01         CALL DWORD PTR DS:[0x13EA1F4]            ; mfc90.#3726
	$-1AD    00AE8305      50                    PUSH EAX
	$-1AC    00AE8306      8BCE                  MOV ECX,ESI
	$-1AA    00AE8308      E8 03AF0600           CALL MapleSto.00B53210
	$-1A5    00AE830D      8B57 38               MOV EDX,DWORD PTR DS:[EDI+0x38]
	$-1A2    00AE8310      52                    PUSH EDX
	$-1A1    00AE8311      B9 AC8C7301           MOV ECX,MapleSto.01738CAC
	$-19C    00AE8316      FF15 F4A13E01         CALL DWORD PTR DS:[0x13EA1F4]            ; mfc90.#3726
	$-196    00AE831C      50                    PUSH EAX
	$-195    00AE831D      8BCE                  MOV ECX,ESI
	$-193    00AE831F      E8 ECAE0600           CALL MapleSto.00B53210
	$-18E    00AE8324      51                    PUSH ECX
	$-18D    00AE8325      8D47 20               LEA EAX,DWORD PTR DS:[EDI+0x20]
	$-18A    00AE8328      8BCC                  MOV ECX,ESP
	$-188    00AE832A      8965 08               MOV DWORD PTR SS:[EBP+0x8],ESP
	$-185    00AE832D      50                    PUSH EAX
	$-184    00AE832E      FF15 C8A13E01         CALL DWORD PTR DS:[0x13EA1C8]            ; mfc90.#280
	$-17E    00AE8334      B9 B08C7301           MOV ECX,MapleSto.01738CB0
	$-179    00AE8339      FF15 F4A13E01         CALL DWORD PTR DS:[0x13EA1F4]            ; mfc90.#3726
	$-173    00AE833F      50                    PUSH EAX
	$-172    00AE8340      8BCE                  MOV ECX,ESI
	$-170    00AE8342      E8 A922A6FF           CALL MapleSto.0054A5F0
	$-16B    00AE8347      83EC 28               SUB ESP,0x28
	$-168    00AE834A      8D57 50               LEA EDX,DWORD PTR DS:[EDI+0x50]
	$-165    00AE834D      8BCC                  MOV ECX,ESP
	$-163    00AE834F      8965 08               MOV DWORD PTR SS:[EBP+0x8],ESP
	$-160    00AE8352      52                    PUSH EDX
	$-15F    00AE8353      E8 1834B5FF           CALL MapleSto.0063B770
	$-15A    00AE8358      B9 B48C7301           MOV ECX,MapleSto.01738CB4
	$-155    00AE835D      FF15 F4A13E01         CALL DWORD PTR DS:[0x13EA1F4]            ; mfc90.#3726
	$-14F    00AE8363      50                    PUSH EAX
	$-14E    00AE8364      8BCE                  MOV ECX,ESI
	$-14C    00AE8366      E8 E5B7C3FF           CALL MapleSto.00723B50
	$-147    00AE836B      51                    PUSH ECX
	$-146    00AE836C      8D47 24               LEA EAX,DWORD PTR DS:[EDI+0x24]
	$-143    00AE836F      8BCC                  MOV ECX,ESP
	$-141    00AE8371      8965 08               MOV DWORD PTR SS:[EBP+0x8],ESP
	$-13E    00AE8374      50                    PUSH EAX
	$-13D    00AE8375      FF15 C8A13E01         CALL DWORD PTR DS:[0x13EA1C8]            ; mfc90.#280
	$-137    00AE837B      B9 C88C7301           MOV ECX,MapleSto.01738CC8
	$-132    00AE8380      FF15 F4A13E01         CALL DWORD PTR DS:[0x13EA1F4]            ; mfc90.#3726
	$-12C    00AE8386      50                    PUSH EAX
	$-12B    00AE8387      8BCE                  MOV ECX,ESI
	$-129    00AE8389      E8 6222A6FF           CALL MapleSto.0054A5F0
	$-124    00AE838E      8B4F 40               MOV ECX,DWORD PTR DS:[EDI+0x40]
	$-121    00AE8391      51                    PUSH ECX
	$-120    00AE8392      68 F0DD4701           PUSH MapleSto.0147DDF0                   ; ASCII "inFieldNpc"
	$-11B    00AE8397      8BCE                  MOV ECX,ESI
	$-119    00AE8399      E8 72AE0600           CALL MapleSto.00B53210
	$-114    00AE839E      51                    PUSH ECX
	$-113    00AE839F      8D57 44               LEA EDX,DWORD PTR DS:[EDI+0x44]
	$-110    00AE83A2      8BCC                  MOV ECX,ESP
	$-10E    00AE83A4      8965 08               MOV DWORD PTR SS:[EBP+0x8],ESP
	$-10B    00AE83A7      52                    PUSH EDX
	$-10A    00AE83A8      FF15 C8A13E01         CALL DWORD PTR DS:[0x13EA1C8]            ; mfc90.#280
	$-104    00AE83AE      68 38054101           PUSH MapleSto.01410538                   ; ASCII "groupType"
	$-FF     00AE83B3      8BCE                  MOV ECX,ESI
	$-FD     00AE83B5      E8 3622A6FF           CALL MapleSto.0054A5F0
	$-F8     00AE83BA      51                    PUSH ECX
	$-F7     00AE83BB      8D47 48               LEA EAX,DWORD PTR DS:[EDI+0x48]
	$-F4     00AE83BE      8BCC                  MOV ECX,ESP
	$-F2     00AE83C0      8965 08               MOV DWORD PTR SS:[EBP+0x8],ESP
	$-EF     00AE83C3      50                    PUSH EAX
	$-EE     00AE83C4      FF15 C8A13E01         CALL DWORD PTR DS:[0x13EA1C8]            ; mfc90.#280
	$-E8     00AE83CA      68 E4DD4701           PUSH MapleSto.0147DDE4                   ; ASCII "subValue"
	$-E3     00AE83CF      8BCE                  MOV ECX,ESI
	$-E1     00AE83D1      E8 1A22A6FF           CALL MapleSto.0054A5F0
	$-DC     00AE83D6      8B4F 30               MOV ECX,DWORD PTR DS:[EDI+0x30]
	$-D9     00AE83D9      F30F7E47 28           MOVQ XMM0,QWORD PTR DS:[EDI+0x28]
	$-D4     00AE83DE      8D55 D8               LEA EDX,DWORD PTR SS:[EBP-0x28]
	$-D1     00AE83E1      52                    PUSH EDX
	$-D0     00AE83E2      8D45 E4               LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$-CD     00AE83E5      894D EC               MOV DWORD PTR SS:[EBP-0x14],ECX
	$-CA     00AE83E8      8B4D 0C               MOV ECX,DWORD PTR SS:[EBP+0xC]
	$-C7     00AE83EB      50                    PUSH EAX
	$-C6     00AE83EC      660FD645 E4           MOVQ QWORD PTR SS:[EBP-0x1C],XMM0
	$-C1     00AE83F1      E8 BA2BE1FF           CALL MapleSto.008FAFB0
	$-BC     00AE83F6      84C0                  TEST AL,AL
	$-BA     00AE83F8      74 0D                 JE SHORT MapleSto.00AE8407
	$-B8     00AE83FA      837F 3C 00            CMP DWORD PTR DS:[EDI+0x3C],0x0
	$-B4     00AE83FE      74 07                 JE SHORT MapleSto.00AE8407
	$-B2     00AE8400      BF 01000000           MOV EDI,0x1
	$-AD     00AE8405      EB 02                 JMP SHORT MapleSto.00AE8409
	$-AB     00AE8407      33FF                  XOR EDI,EDI
	$-A9     00AE8409      D945 D8               FLD DWORD PTR SS:[EBP-0x28]
	$-A6     00AE840C      51                    PUSH ECX
	$-A5     00AE840D      D91C24                FSTP DWORD PTR SS:[ESP]
	$-A2     00AE8410      68 0C384101           PUSH MapleSto.0141380C                   ; UNICODE "x"
	$-9D     00AE8415      8BCE                  MOV ECX,ESI
	$-9B     00AE8417      E8 84EEADFF           CALL MapleSto.005C72A0
	$-96     00AE841C      D945 DC               FLD DWORD PTR SS:[EBP-0x24]
	$-93     00AE841F      51                    PUSH ECX
	$-92     00AE8420      D91C24                FSTP DWORD PTR SS:[ESP]
	$-8F     00AE8423      68 08384101           PUSH MapleSto.01413808                   ; UNICODE "y"
	$-8A     00AE8428      8BCE                  MOV ECX,ESI
	$-88     00AE842A      E8 71EEADFF           CALL MapleSto.005C72A0
	$-83     00AE842F      57                    PUSH EDI
	$-82     00AE8430      68 EC154101           PUSH MapleSto.014115EC                   ; ASCII "visible"
	$-7D     00AE8435      8BCE                  MOV ECX,ESI
	$-7B     00AE8437      E8 D4AD0600           CALL MapleSto.00B53210
	$-76     00AE843C      8B45 0C               MOV EAX,DWORD PTR SS:[EBP+0xC]
	$-73     00AE843F      C745 FC FFFFFFFF      MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-6C     00AE8446      85C0                  TEST EAX,EAX
	$-6A     00AE8448      74 19                 JE SHORT MapleSto.00AE8463
	$-68     00AE844A      8BF0                  MOV ESI,EAX
	$-66     00AE844C      83C0 04               ADD EAX,0x4
	$-63     00AE844F      50                    PUSH EAX
	$-62     00AE8450      FF15 6C933E01         CALL DWORD PTR DS:[0x13E936C]
	$-5C     00AE8456      85C0                  TEST EAX,EAX
	$-5A     00AE8458      75 09                 JNZ SHORT MapleSto.00AE8463
	$-58     00AE845A      8B16                  MOV EDX,DWORD PTR DS:[ESI]
	$-56     00AE845C      8B42 04               MOV EAX,DWORD PTR DS:[EDX+0x4]
	$-53     00AE845F      8BCE                  MOV ECX,ESI
	$-51     00AE8461      FFD0                  CALL EAX
	$-4F     00AE8463      8B4D F4               MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-4C     00AE8466      64:890D 00000000      MOV DWORD PTR FS:[0],ECX
	$-45     00AE846D      59                    POP ECX
	$-44     00AE846E      5F                    POP EDI
	$-43     00AE846F      5E                    POP ESI
	$-42     00AE8470      8BE5                  MOV ESP,EBP
	$-40     00AE8472      5D                    POP EBP
	$-3F     00AE8473      C2 0800               RETN 0x8
	$-3C     00AE8476      CC                    INT3
	$-3B     00AE8477      CC                    INT3
	$-3A     00AE8478      CC                    INT3
	$-39     00AE8479      CC                    INT3
	$-38     00AE847A      CC                    INT3
	$-37     00AE847B      CC                    INT3
	$-36     00AE847C      CC                    INT3
	$-35     00AE847D      CC                    INT3
	$-34     00AE847E      CC                    INT3
	$-33     00AE847F      CC                    INT3
	$-32     00AE8480      55                    PUSH EBP
	$-31     00AE8481      8BEC                  MOV EBP,ESP
	$-2F     00AE8483      6A FF                 PUSH -0x1
	$-2D     00AE8485      68 6EF13801           PUSH MapleSto.0138F16E
	$-28     00AE848A      64:A1 00000000        MOV EAX,DWORD PTR FS:[0]
	$-22     00AE8490      50                    PUSH EAX
	$-21     00AE8491      51                    PUSH ECX
	$-20     00AE8492      56                    PUSH ESI
	$-1F     00AE8493      57                    PUSH EDI
	$-1E     00AE8494      A1 D0687101           MOV EAX,DWORD PTR DS:[0x17168D0]
	$-19     00AE8499      33C5                  XOR EAX,EBP
	$-17     00AE849B      50                    PUSH EAX
	$-16     00AE849C      8D45 F4               LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-13     00AE849F      64:A3 00000000        MOV DWORD PTR FS:[0],EAX
	$-D      00AE84A5      C745 F0 00000000      MOV DWORD PTR SS:[EBP-0x10],0x0
	$-6      00AE84AC      8B7D 0C               MOV EDI,DWORD PTR SS:[EBP+0xC]
	$-3      00AE84AF      8B47 18               MOV EAX,DWORD PTR DS:[EDI+0x18]
	$ ==>    00AE84B2      50                    PUSH EAX
	$+1      00AE84B3      6A 0A                 PUSH 0xA
	$+3      00AE84B5      8D4D 0C               LEA ECX,DWORD PTR SS:[EBP+0xC]
	$+6      00AE84B8      51                    PUSH ECX
	$+7      00AE84B9      8B0D 1C257201         MOV ECX,DWORD PTR DS:[0x172251C]         ; g_a_xml_string_
	$+D      00AE84BF      E8 4C48F3FF           CALL MapleSto.00A1CD10                   ; g_c_find_xml_str_by_id_
	$+12     00AE84C4      8D4D 0C               LEA ECX,DWORD PTR SS:[EBP+0xC]
	$+15     00AE84C7      C745 FC 00000000      MOV DWORD PTR SS:[EBP-0x4],0x0
	$+1C     00AE84CE      FF15 C4A13E01         CALL DWORD PTR DS:[0x13EA1C4]            ; mfc90.#4311
	$+22     00AE84D4      84C0                  TEST AL,AL
	$+24     00AE84D6      0F85 89000000         JNZ MapleSto.00AE8565
	$+2A     00AE84DC      8BCF                  MOV ECX,EDI
	$+2C     00AE84DE      E8 DD30A3FF           CALL MapleSto.0051B5C0
	$+31     00AE84E3      85C0                  TEST EAX,EAX
	$+33     00AE84E5      74 23                 JE SHORT MapleSto.00AE850A
	$+35     00AE84E7      8B35 40277201         MOV ESI,DWORD PTR DS:[0x1722740]
	$+3B     00AE84ED      81C6 F4600000         ADD ESI,0x60F4
	$+41     00AE84F3      807E 04 00            CMP BYTE PTR DS:[ESI+0x4],0x0
	$+45     00AE84F7      75 0C                 JNZ SHORT MapleSto.00AE8505
	$+47     00AE84F9      68 88463F01           PUSH MapleSto.013F4688                   ; ASCII "constants is not set"
	$+4C     00AE84FE      8BCE                  MOV ECX,ESI
	$+4E     00AE8500      E8 4B5F96FF           CALL MapleSto.0044E450
	$+53     00AE8505      8B46 40               MOV EAX,DWORD PTR DS:[ESI+0x40]
	$+56     00AE8508      EB 07                 JMP SHORT MapleSto.00AE8511
	$+58     00AE850A      8BCF                  MOV ECX,EDI
	$+5A     00AE850C      E8 F38C1501           CALL MapleSto.01C41204
	$+5F     00AE8511      50                    PUSH EAX
	$+60     00AE8512      8D55 0C               LEA EDX,DWORD PTR SS:[EBP+0xC]
	$+63     00AE8515      52                    PUSH EDX
	$+64     00AE8516      8D45 F0               LEA EAX,DWORD PTR SS:[EBP-0x10]
	$+67     00AE8519      50                    PUSH EAX
	$+68     00AE851A      E8 611BF4FF           CALL MapleSto.00A2A080
	$+6D     00AE851F      8B75 14               MOV ESI,DWORD PTR SS:[EBP+0x14]
	$+70     00AE8522      8B4F 18               MOV ECX,DWORD PTR DS:[EDI+0x18]
	$+73     00AE8525      F7DE                  NEG ESI
	$+75     00AE8527      51                    PUSH ECX
	$+76     00AE8528      8D55 14               LEA EDX,DWORD PTR SS:[EBP+0x14]
	$+79     00AE852B      1BF6                  SBB ESI,ESI
	$+7B     00AE852D      52                    PUSH EDX
	$+7C     00AE852E      C645 FC 01            MOV BYTE PTR SS:[EBP-0x4],0x1
	$+80     00AE8532      81C6 F4060000         ADD ESI,0x6F4
	$+86     00AE8538      E8 23FA91FF           CALL MapleSto.00407F60
	$+8B     00AE853D      8B4D 10               MOV ECX,DWORD PTR SS:[EBP+0x10]
	$+8E     00AE8540      50                    PUSH EAX
	$+8F     00AE8541      8D45 F0               LEA EAX,DWORD PTR SS:[EBP-0x10]
	$+92     00AE8544      50                    PUSH EAX
	$+93     00AE8545      51                    PUSH ECX
	$+94     00AE8546      56                    PUSH ESI
	$+95     00AE8547      8B75 08               MOV ESI,DWORD PTR SS:[EBP+0x8]
	$+98     00AE854A      56                    PUSH ESI
	$+99     00AE854B      C645 FC 02            MOV BYTE PTR SS:[EBP-0x4],0x2
	$+9D     00AE854F      E8 BCC7AEFF           CALL MapleSto.005D4D10
	$+A2     00AE8554      83C4 28               ADD ESP,0x28
	$+A5     00AE8557      8D4D 14               LEA ECX,DWORD PTR SS:[EBP+0x14]
	$+A8     00AE855A      FF15 DCA13E01         CALL DWORD PTR DS:[0x13EA1DC]            ; mfc90.#601
	$+AE     00AE8560      8D4D F0               LEA ECX,DWORD PTR SS:[EBP-0x10]
	$+B1     00AE8563      EB 26                 JMP SHORT MapleSto.00AE858B
	$+B3     00AE8565      8B57 18               MOV EDX,DWORD PTR DS:[EDI+0x18]
	$+B6     00AE8568      52                    PUSH EDX
	$+B7     00AE8569      8D45 14               LEA EAX,DWORD PTR SS:[EBP+0x14]
	$+BA     00AE856C      50                    PUSH EAX
	$+BB     00AE856D      E8 EEF991FF           CALL MapleSto.00407F60
	$+C0     00AE8572      8B75 08               MOV ESI,DWORD PTR SS:[EBP+0x8]
	$+C3     00AE8575      50                    PUSH EAX
	$+C4     00AE8576      68 F7060000           PUSH 0x6F7
	$+C9     00AE857B      56                    PUSH ESI
	$+CA     00AE857C      C645 FC 03            MOV BYTE PTR SS:[EBP-0x4],0x3
	$+CE     00AE8580      E8 6B6C9DFF           CALL MapleSto.004BF1F0
	$+D3     00AE8585      83C4 14               ADD ESP,0x14
	$+D6     00AE8588      8D4D 14               LEA ECX,DWORD PTR SS:[EBP+0x14]
	$+D9     00AE858B      FF15 DCA13E01         CALL DWORD PTR DS:[0x13EA1DC]            ; mfc90.#601
	$+DF     00AE8591      8D4D 0C               LEA ECX,DWORD PTR SS:[EBP+0xC]
	$+E2     00AE8594      FF15 DCA13E01         CALL DWORD PTR DS:[0x13EA1DC]            ; mfc90.#601
	$+E8     00AE859A      8BC6                  MOV EAX,ESI
	$+EA     00AE859C      8B4D F4               MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+ED     00AE859F      64:890D 00000000      MOV DWORD PTR FS:[0],ECX
	$+F4     00AE85A6      59                    POP ECX
	$+F5     00AE85A7      5F                    POP EDI
	$+F6     00AE85A8      5E                    POP ESI
	$+F7     00AE85A9      8BE5                  MOV ESP,EBP
	$+F9     00AE85AB      5D                    POP EBP
	$+FA     00AE85AC      C2 1000               RETN 0x10
	$+FD     00AE85AF      CC                    INT3
	$+FE     00AE85B0      55                    PUSH EBP
	$+FF     00AE85B1      8BEC                  MOV EBP,ESP
	$+101    00AE85B3      6A FF                 PUSH -0x1
	$+103    00AE85B5      68 ABF13801           PUSH MapleSto.0138F1AB
	$+108    00AE85BA      64:A1 00000000        MOV EAX,DWORD PTR FS:[0]
	$+10E    00AE85C0      50                    PUSH EAX
	$+10F    00AE85C1      83EC 1C               SUB ESP,0x1C
	$+112    00AE85C4      53                    PUSH EBX
	$+113    00AE85C5      56                    PUSH ESI
	$+114    00AE85C6      A1 D0687101           MOV EAX,DWORD PTR DS:[0x17168D0]
	$+119    00AE85CB      33C5                  XOR EAX,EBP
	$+11B    00AE85CD      50                    PUSH EAX
	$+11C    00AE85CE      8D45 F4               LEA EAX,DWORD PTR SS:[EBP-0xC]
	$+11F    00AE85D1      64:A3 00000000        MOV DWORD PTR FS:[0],EAX
	$+125    00AE85D7      8BF1                  MOV ESI,ECX
	$+127    00AE85D9      B9 388D7301           MOV ECX,MapleSto.01738D38
	$+12C    00AE85DE      C745 F0 00000000      MOV DWORD PTR SS:[EBP-0x10],0x0
	$+133    00AE85E5      FF15 F4A13E01         CALL DWORD PTR DS:[0x13EA1F4]            ; mfc90.#3726
	$+139    00AE85EB      50                    PUSH EAX
	$+13A    00AE85EC      8D45 D8               LEA EAX,DWORD PTR SS:[EBP-0x28]
	$+13D    00AE85EF      50                    PUSH EAX
	$+13E    00AE85F0      8D4E 50               LEA ECX,DWORD PTR DS:[ESI+0x50]
	$+141    00AE85F3      E8 98B03F00           CALL MapleSto.00EE3690
	$+146    00AE85F8      8B4D DC               MOV ECX,DWORD PTR SS:[EBP-0x24]
	$+149    00AE85FB      8BC1                  MOV EAX,ECX
	$+14B    00AE85FD      25 8F000000           AND EAX,0x8F
	$+150    00AE8602      BB 01000000           MOV EBX,0x1
	$+155    00AE8607      895D FC               MOV DWORD PTR SS:[EBP-0x4],EBX
	$+158    00AE860A      83F8 06               CMP EAX,0x6
	$+15B    00AE860D      75 43                 JNZ SHORT MapleSto.00AE8652
	$+15D    00AE860F      C1E9 06               SHR ECX,0x6
	$+160    00AE8612      84CB                  TEST BL,CL
	$+162    00AE8614      74 07                 JE SHORT MapleSto.00AE861D
	$+164    00AE8616      8B4D E0               MOV ECX,DWORD PTR SS:[EBP-0x20]
	$+167    00AE8619      8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$+169    00AE861B      EB 03                 JMP SHORT MapleSto.00AE8620
	$+16B    00AE861D      8B45 E0               MOV EAX,DWORD PTR SS:[EBP-0x20]
	$+16E    00AE8620      8B75 08               MOV ESI,DWORD PTR SS:[EBP+0x8]
	$+171    00AE8623      50                    PUSH EAX
	$+172    00AE8624      56                    PUSH ESI
	$+173    00AE8625      E8 1695A8FF           CALL MapleSto.00571B40
	$+178    00AE862A      83C4 08               ADD ESP,0x8
	$+17B    00AE862D      8B55 DC               MOV EDX,DWORD PTR SS:[EBP-0x24]
	$+17E    00AE8630      C1EA 06               SHR EDX,0x6
	$+181    00AE8633      895D F0               MOV DWORD PTR SS:[EBP-0x10],EBX
	$+184    00AE8636      C645 FC 00            MOV BYTE PTR SS:[EBP-0x4],0x0
	$+188    00AE863A      84D3                  TEST BL,DL
	$+18A    00AE863C      74 5D                 JE SHORT MapleSto.00AE869B
	$+18C    00AE863E      8B55 E0               MOV EDX,DWORD PTR SS:[EBP-0x20]
	$+18F    00AE8641      8B4D D8               MOV ECX,DWORD PTR SS:[EBP-0x28]
	$+192    00AE8644      8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$+194    00AE8646      8B40 08               MOV EAX,DWORD PTR DS:[EAX+0x8]
	$+197    00AE8649      52                    PUSH EDX
	$+198    00AE864A      8D55 D8               LEA EDX,DWORD PTR SS:[EBP-0x28]
	$+19B    00AE864D      52                    PUSH EDX
	$+19C    00AE864E      FFD0                  CALL EAX
	$+19E    00AE8650      EB 49                 JMP SHORT MapleSto.00AE869B
	$+1A0    00AE8652      8B75 08               MOV ESI,DWORD PTR SS:[EBP+0x8]
	$+1A3    00AE8655      83F8 07               CMP EAX,0x7
	$+1A6    00AE8658      75 11                 JNZ SHORT MapleSto.00AE866B
	$+1A8    00AE865A      8B4D E0               MOV ECX,DWORD PTR SS:[EBP-0x20]
	$+1AB    00AE865D      6A 00                 PUSH 0x0
	$+1AD    00AE865F      51                    PUSH ECX
	$+1AE    00AE8660      56                    PUSH ESI
	$+1AF    00AE8661      E8 1A89BEFF           CALL MapleSto.006D0F80
	$+1B4    00AE8666      83C4 0C               ADD ESP,0xC
	$+1B7    00AE8669    ^ EB C2                 JMP SHORT MapleSto.00AE862D
	$+1B9    00AE866B      68 50303F01           PUSH MapleSto.013F3050
	$+1BE    00AE8670      8BCE                  MOV ECX,ESI
	$+1C0    00AE8672      FF15 D0A13E01         CALL DWORD PTR DS:[0x13EA1D0]            ; mfc90.#286
	$+1C6    00AE8678      8B4D DC               MOV ECX,DWORD PTR SS:[EBP-0x24]
	$+1C9    00AE867B      C1E9 06               SHR ECX,0x6
	$+1CC    00AE867E      895D F0               MOV DWORD PTR SS:[EBP-0x10],EBX
	$+1CF    00AE8681      C645 FC 00            MOV BYTE PTR SS:[EBP-0x4],0x0
	$+1D3    00AE8685      84CB                  TEST BL,CL
	$+1D5    00AE8687      74 12                 JE SHORT MapleSto.00AE869B
	$+1D7    00AE8689      8B45 E0               MOV EAX,DWORD PTR SS:[EBP-0x20]
	$+1DA    00AE868C      8B4D D8               MOV ECX,DWORD PTR SS:[EBP-0x28]
	$+1DD    00AE868F      8B11                  MOV EDX,DWORD PTR DS:[ECX]
	$+1DF    00AE8691      8B52 08               MOV EDX,DWORD PTR DS:[EDX+0x8]
	$+1E2    00AE8694      50                    PUSH EAX
	$+1E3    00AE8695      8D45 D8               LEA EAX,DWORD PTR SS:[EBP-0x28]
	$+1E6    00AE8698      50                    PUSH EAX
	$+1E7    00AE8699      FFD2                  CALL EDX
	$+1E9    00AE869B      8BC6                  MOV EAX,ESI
	$+1EB    00AE869D      8B4D F4               MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+1EE    00AE86A0      64:890D 00000000      MOV DWORD PTR FS:[0],ECX
	$+1F5    00AE86A7      59                    POP ECX
	$+1F6    00AE86A8      5E                    POP ESI
	$+1F7    00AE86A9      5B                    POP EBX
	$+1F8    00AE86AA      8BE5                  MOV ESP,EBP
	$+1FA    00AE86AC      5D                    POP EBP
	$+1FB    00AE86AD      C2 0400               RETN 0x4
	$+1FE    00AE86B0      55                    PUSH EBP
	$+1FF    00AE86B1      8BEC                  MOV EBP,ESP
	$+201    00AE86B3      6A FF                 PUSH -0x1
	$+203    00AE86B5      68 CFF13801           PUSH MapleSto.0138F1CF
	$+208    00AE86BA      64:A1 00000000        MOV EAX,DWORD PTR FS:[0]
	$+20E    00AE86C0      50                    PUSH EAX
	$+20F    00AE86C1      51                    PUSH ECX
	$+210    00AE86C2      56                    PUSH ESI
	$+211    00AE86C3      A1 D0687101           MOV EAX,DWORD PTR DS:[0x17168D0]
	$+216    00AE86C8      33C5                  XOR EAX,EBP
	$+218    00AE86CA      50                    PUSH EAX
	$+219    00AE86CB      8D45 F4               LEA EAX,DWORD PTR SS:[EBP-0xC]
	$+21C    00AE86CE      64:A3 00000000        MOV DWORD PTR FS:[0],EAX
	$+222    00AE86D4      8BF1                  MOV ESI,ECX
	$+224    00AE86D6      51                    PUSH ECX
	$+225    00AE86D7      8D45 08               LEA EAX,DWORD PTR SS:[EBP+0x8]
	$+228    00AE86DA      8BCC                  MOV ECX,ESP
	$+22A    00AE86DC      8965 F0               MOV DWORD PTR SS:[EBP-0x10],ESP
	$+22D    00AE86DF      50                    PUSH EAX
	$+22E    00AE86E0      C745 FC 00000000      MOV DWORD PTR SS:[EBP-0x4],0x0
	$+235    00AE86E7      FF15 C8A13E01         CALL DWORD PTR DS:[0x13EA1C8]            ; mfc90.#280
	$+23B    00AE86ED      B9 388D7301           MOV ECX,MapleSto.01738D38
	$+240    00AE86F2      FF15 F4A13E01         CALL DWORD PTR DS:[0x13EA1F4]            ; mfc90.#3726
	$+246    00AE86F8      50                    PUSH EAX
	$+247    00AE86F9      8D4E 50               LEA ECX,DWORD PTR DS:[ESI+0x50]
	$+24A    00AE86FC      E8 EF1EA6FF           CALL MapleSto.0054A5F0
	$+24F    00AE8701      8D4D 08               LEA ECX,DWORD PTR SS:[EBP+0x8]
	$+252    00AE8704      FF15 DCA13E01         CALL DWORD PTR DS:[0x13EA1DC]            ; mfc90.#601
	$+258    00AE870A      8B4D F4               MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+25B    00AE870D      64:890D 00000000      MOV DWORD PTR FS:[0],ECX
	$+262    00AE8714      59                    POP ECX
	$+263    00AE8715      5E                    POP ESI
	$+264    00AE8716      8BE5                  MOV ESP,EBP
	$+266    00AE8718      5D                    POP EBP
	$+267    00AE8719      C2 0400               RETN 0x4
	$+26A    00AE871C      CC                    INT3
	$+26B    00AE871D      CC                    INT3
	$+26C    00AE871E      CC                    INT3
	$+26D    00AE871F      CC                    INT3
	$+26E    00AE8720      55                    PUSH EBP
	$+26F    00AE8721      8BEC                  MOV EBP,ESP
	$+271    00AE8723      83EC 0C               SUB ESP,0xC
	$+274    00AE8726      56                    PUSH ESI
	$+275    00AE8727      8BF1                  MOV ESI,ECX
	$+277    00AE8729      8B86 F8020000         MOV EAX,DWORD PTR DS:[ESI+0x2F8]
	$+27D    00AE872F      85C0                  TEST EAX,EAX
	$+27F    00AE8731      74 63                 JE SHORT MapleSto.00AE8796
	$+281    00AE8733      8B4E 20               MOV ECX,DWORD PTR DS:[ESI+0x20]
	$+284    00AE8736      C745 F8 00000000      MOV DWORD PTR SS:[EBP-0x8],0x0
	$+28B    00AE873D      C745 FC 00000000      MOV DWORD PTR SS:[EBP-0x4],0x0
	$+292    00AE8744      85C9                  TEST ECX,ECX
	$+294    00AE8746      74 1F                 JE SHORT MapleSto.00AE8767
	$+296    00AE8748      8D45 F8               LEA EAX,DWORD PTR SS:[EBP-0x8]
	$+299    00AE874B      50                    PUSH EAX
	$+29A    00AE874C      68 A8DD4701           PUSH MapleSto.0147DDA8                   ; ASCII "m_windowWidth"
	$+29F    00AE8751      E8 79554001           CALL MapleSto.01EEDCCF
	$+2A4    00AE8756      8B4E 20               MOV ECX,DWORD PTR DS:[ESI+0x20]
	$+2A7    00AE8759      8D55 FC               LEA EDX,DWORD PTR SS:[EBP-0x4]
	$+2AA    00AE875C      52                    PUSH EDX
	$+2AB    00AE875D      68 98DD4701           PUSH MapleSto.0147DD98                   ; ASCII "m_windowHeight"
	$+2B0    00AE8762      E8 68554001           CALL MapleSto.01EEDCCF
	$+2B5    00AE8767      8B45 FC               MOV EAX,DWORD PTR SS:[EBP-0x4]
	$+2B8    00AE876A      8B4D F8               MOV ECX,DWORD PTR SS:[EBP-0x8]
	$+2BB    00AE876D      50                    PUSH EAX
	$+2BC    00AE876E      8B46 20               MOV EAX,DWORD PTR DS:[ESI+0x20]
	$+2BF    00AE8771      51                    PUSH ECX
	$+2C0    00AE8772      83EC 08               SUB ESP,0x8
	$+2C3    00AE8775      8BCC                  MOV ECX,ESP
	$+2C5    00AE8777      8965 F4               MOV DWORD PTR SS:[EBP-0xC],ESP
	$+2C8    00AE877A      8941 04               MOV DWORD PTR DS:[ECX+0x4],EAX
	$+2CB    00AE877D      85C0                  TEST EAX,EAX
	$+2CD    00AE877F      74 0A                 JE SHORT MapleSto.00AE878B
	$+2CF    00AE8781      83C0 04               ADD EAX,0x4
	$+2D2    00AE8784      50                    PUSH EAX
	$+2D3    00AE8785      FF15 F8923E01         CALL DWORD PTR DS:[0x13E92F8]
	$+2D9    00AE878B      8B8E F8020000         MOV ECX,DWORD PTR DS:[ESI+0x2F8]
	$+2DF    00AE8791      E8 FA72F9FF           CALL MapleSto.00A7FA90
	$+2E4    00AE8796      5E                    POP ESI
	$+2E5    00AE8797      8BE5                  MOV ESP,EBP
	$+2E7    00AE8799      5D                    POP EBP
	$+2E8    00AE879A      C3                    RETN
	$+2E9    00AE879B      CC                    INT3
	$+2EA    00AE879C      CC                    INT3
	$+2EB    00AE879D      CC                    INT3
	$+2EC    00AE879E      CC                    INT3
	$+2ED    00AE879F      CC                    INT3
	$+2EE    00AE87A0      55                    PUSH EBP
	$+2EF    00AE87A1      8BEC                  MOV EBP,ESP
	$+2F1    00AE87A3      83EC 18               SUB ESP,0x18
	$+2F4    00AE87A6      56                    PUSH ESI
	$+2F5    00AE87A7      8BF1                  MOV ESI,ECX
	$+2F7    00AE87A9      C786 5C010000 0000000>MOV DWORD PTR DS:[ESI+0x15C],0x0
	$+301    00AE87B3      E8 78F8FFFF           CALL MapleSto.00AE8030
	$+306    00AE87B8      8B4E 20               MOV ECX,DWORD PTR DS:[ESI+0x20]
	$+309    00AE87BB      85C9                  TEST ECX,ECX
	$+30B    00AE87BD      74 2B                 JE SHORT MapleSto.00AE87EA
	$+30D    00AE87BF      68 D4DD4701           PUSH MapleSto.0147DDD4                   ; ASCII "changeStageSize"
	$+312    00AE87C4      8D45 E8               LEA EAX,DWORD PTR SS:[EBP-0x18]
	$+315    00AE87C7      50                    PUSH EAX
	$+316    00AE87C8      E8 B3B13F00           CALL MapleSto.00EE3980
	$+31B    00AE87CD      8B4D EC               MOV ECX,DWORD PTR SS:[EBP-0x14]
	$+31E    00AE87D0      C1E9 06               SHR ECX,0x6
	$+321    00AE87D3      F6C1 01               TEST CL,0x1
	$+324    00AE87D6      74 12                 JE SHORT MapleSto.00AE87EA
	$+326    00AE87D8      8B45 F0               MOV EAX,DWORD PTR SS:[EBP-0x10]
	$+329    00AE87DB      8B4D E8               MOV ECX,DWORD PTR SS:[EBP-0x18]
	$+32C    00AE87DE      8B11                  MOV EDX,DWORD PTR DS:[ECX]
	$+32E    00AE87E0      8B52 08               MOV EDX,DWORD PTR DS:[EDX+0x8]
	$+331    00AE87E3      50                    PUSH EAX
	$+332    00AE87E4      8D45 E8               LEA EAX,DWORD PTR SS:[EBP-0x18]
	$+335    00AE87E7      50                    PUSH EAX
	$+336    00AE87E8      FFD2                  CALL EDX
	$+338    00AE87EA      837E 44 00            CMP DWORD PTR DS:[ESI+0x44],0x0
	$+33C    00AE87EE      74 0B                 JE SHORT MapleSto.00AE87FB
	$+33E    00AE87F0      8B8E F8020000         MOV ECX,DWORD PTR DS:[ESI+0x2F8]
	$+344    00AE87F6      E8 B563F9FF           CALL MapleSto.00A7EBB0
	$+349    00AE87FB      8BCE                  MOV ECX,ESI
	$+34B    00AE87FD      E8 1EFFFFFF           CALL MapleSto.00AE8720
	$+350    00AE8802      5E                    POP ESI
	$+351    00AE8803      8BE5                  MOV ESP,EBP
	$+353    00AE8805      5D                    POP EBP
	$+354    00AE8806      C3                    RETN
	$+355    00AE8807      CC                    INT3
	$+356    00AE8808      CC                    INT3
	$+357    00AE8809      CC                    INT3
	$+358    00AE880A      CC                    INT3
	$+359    00AE880B      CC                    INT3
	$+35A    00AE880C      CC                    INT3
	$+35B    00AE880D      CC                    INT3
	$+35C    00AE880E      CC                    INT3
	$+35D    00AE880F      CC                    INT3
	$+35E    00AE8810      55                    PUSH EBP
	$+35F    00AE8811      8BEC                  MOV EBP,ESP
	$+361    00AE8813      6A FF                 PUSH -0x1
	$+363    00AE8815      68 FDF13801           PUSH MapleSto.0138F1FD
	$+368    00AE881A      64:A1 00000000        MOV EAX,DWORD PTR FS:[0]
	$+36E    00AE8820      50                    PUSH EAX
	$+36F    00AE8821      83EC 7C               SUB ESP,0x7C
	$+372    00AE8824      53                    PUSH EBX
	$+373    00AE8825      56                    PUSH ESI
	$+374    00AE8826      57                    PUSH EDI
	$+375    00AE8827      A1 D0687101           MOV EAX,DWORD PTR DS:[0x17168D0]
	$+37A    00AE882C      33C5                  XOR EAX,EBP
	$+37C    00AE882E      50                    PUSH EAX
	$+37D    00AE882F      8D45 F4               LEA EAX,DWORD PTR SS:[EBP-0xC]
	$+380    00AE8832      64:A3 00000000        MOV DWORD PTR FS:[0],EAX
	$+386    00AE8838      8BF9                  MOV EDI,ECX
	$+388    00AE883A      33F6                  XOR ESI,ESI
	$+38A    00AE883C      3977 5C               CMP DWORD PTR DS:[EDI+0x5C],ESI
	$+38D    00AE883F      0F84 45050000         JE MapleSto.00AE8D8A
	$+393    00AE8845      8B4F 20               MOV ECX,DWORD PTR DS:[EDI+0x20]
	$+396    00AE8848      3BCE                  CMP ECX,ESI
	$+398    00AE884A      0F84 3A050000         JE MapleSto.00AE8D8A
	$+39E    00AE8850      0F57C0                XORPS XMM0,XMM0
	$+3A1    00AE8853      8B87 58010000         MOV EAX,DWORD PTR DS:[EDI+0x158]
	$+3A7    00AE8859      8B9F 54010000         MOV EBX,DWORD PTR DS:[EDI+0x154]
	$+3AD    00AE885F      68 10DE4701           PUSH MapleSto.0147DE10                   ; ASCII "getSaveData"
	$+3B2    00AE8864      8D55 A0               LEA EDX,DWORD PTR SS:[EBP-0x60]
	$+3B5    00AE8867      F3:0F1145 F0          MOVSS DWORD PTR SS:[EBP-0x10],XMM0
	$+3BA    00AE886C      F3:0F1145 EC          MOVSS DWORD PTR SS:[EBP-0x14],XMM0
	$+3BF    00AE8871      F3:0F1005 50473F01    MOVSS XMM0,DWORD PTR DS:[0x13F4750]
	$+3C7    00AE8879      52                    PUSH EDX
	$+3C8    00AE887A      F3:0F1145 E0          MOVSS DWORD PTR SS:[EBP-0x20],XMM0
	$+3CD    00AE887F      C745 DC 1E000000      MOV DWORD PTR SS:[EBP-0x24],0x1E
	$+3D4    00AE8886      C745 D8 64000000      MOV DWORD PTR SS:[EBP-0x28],0x64
	$+3DB    00AE888D      8975 E4               MOV DWORD PTR SS:[EBP-0x1C],ESI
	$+3DE    00AE8890      8975 E8               MOV DWORD PTR SS:[EBP-0x18],ESI
	$+3E1    00AE8893      8945 D4               MOV DWORD PTR SS:[EBP-0x2C],EAX
	$+3E4    00AE8896      E8 E5B03F00           CALL MapleSto.00EE3980
	$+3E9    00AE889B      F645 A4 8F            TEST BYTE PTR SS:[EBP-0x5C],0x8F
	$+3ED    00AE889F      8975 FC               MOV DWORD PTR SS:[EBP-0x4],ESI
	$+3F0    00AE88A2      0F84 2B020000         JE MapleSto.00AE8AD3
	$+3F6    00AE88A8      6A 01                 PUSH 0x1
	$+3F8    00AE88AA      8D45 A0               LEA EAX,DWORD PTR SS:[EBP-0x60]
	$+3FB    00AE88AD      50                    PUSH EAX
	$+3FC    00AE88AE      8D8D 78FFFFFF         LEA ECX,DWORD PTR SS:[EBP-0x88]
	$+402    00AE88B4      E8 37AD3F00           CALL MapleSto.00EE35F0
	$+407    00AE88B9      68 0C384101           PUSH MapleSto.0141380C                   ; UNICODE "x"
	$+40C    00AE88BE      8D4D B8               LEA ECX,DWORD PTR SS:[EBP-0x48]
	$+40F    00AE88C1      51                    PUSH ECX
	$+410    00AE88C2      8D8D 78FFFFFF         LEA ECX,DWORD PTR SS:[EBP-0x88]
	$+416    00AE88C8      C645 FC 01            MOV BYTE PTR SS:[EBP-0x4],0x1
	$+41A    00AE88CC      E8 BFAD3F00           CALL MapleSto.00EE3690
	$+41F    00AE88D1      8B55 BC               MOV EDX,DWORD PTR SS:[EBP-0x44]
	$+422    00AE88D4      F20F1040 08           MOVSD XMM0,QWORD PTR DS:[EAX+0x8]
	$+427    00AE88D9      C1EA 06               SHR EDX,0x6
	$+42A    00AE88DC      660F5AC0              CVTPD2PS XMM0,XMM0
	$+42E    00AE88E0      F3:0F1145 F0          MOVSS DWORD PTR SS:[EBP-0x10],XMM0
	$+433    00AE88E5      F6C2 01               TEST DL,0x1
	$+436    00AE88E8      74 12                 JE SHORT MapleSto.00AE88FC
	$+438    00AE88EA      8B55 C0               MOV EDX,DWORD PTR SS:[EBP-0x40]
	$+43B    00AE88ED      8B4D B8               MOV ECX,DWORD PTR SS:[EBP-0x48]
	$+43E    00AE88F0      8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$+440    00AE88F2      8B40 08               MOV EAX,DWORD PTR DS:[EAX+0x8]
	$+443    00AE88F5      52                    PUSH EDX
	$+444    00AE88F6      8D55 B8               LEA EDX,DWORD PTR SS:[EBP-0x48]
	$+447    00AE88F9      52                    PUSH EDX
	$+448    00AE88FA      FFD0                  CALL EAX
	$+44A    00AE88FC      68 08384101           PUSH MapleSto.01413808                   ; UNICODE "y"
	$+44F    00AE8901      8D4D B8               LEA ECX,DWORD PTR SS:[EBP-0x48]
	$+452    00AE8904      51                    PUSH ECX
	$+453    00AE8905      8D8D 78FFFFFF         LEA ECX,DWORD PTR SS:[EBP-0x88]
	$+459    00AE890B      E8 80AD3F00           CALL MapleSto.00EE3690
	$+45E    00AE8910      8B55 BC               MOV EDX,DWORD PTR SS:[EBP-0x44]
	$+461    00AE8913      F20F1040 08           MOVSD XMM0,QWORD PTR DS:[EAX+0x8]
	$+466    00AE8918      C1EA 06               SHR EDX,0x6
	$+469    00AE891B      660F5AC0              CVTPD2PS XMM0,XMM0
	$+46D    00AE891F      F3:0F1145 EC          MOVSS DWORD PTR SS:[EBP-0x14],XMM0
	$+472    00AE8924      F6C2 01               TEST DL,0x1
	$+475    00AE8927      74 12                 JE SHORT MapleSto.00AE893B
	$+477    00AE8929      8B55 C0               MOV EDX,DWORD PTR SS:[EBP-0x40]
	$+47A    00AE892C      8B4D B8               MOV ECX,DWORD PTR SS:[EBP-0x48]
	$+47D    00AE892F      8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$+47F    00AE8931      8B40 08               MOV EAX,DWORD PTR DS:[EAX+0x8]
	$+482    00AE8934      52                    PUSH EDX
	$+483    00AE8935      8D55 B8               LEA EDX,DWORD PTR SS:[EBP-0x48]
	$+486    00AE8938      52                    PUSH EDX
	$+487    00AE8939      FFD0                  CALL EAX
	$+489    00AE893B      68 5C413F01           PUSH MapleSto.013F415C                   ; ASCII "width"
	$+48E    00AE8940      8D4D B8               LEA ECX,DWORD PTR SS:[EBP-0x48]
	$+491    00AE8943      51                    PUSH ECX
	$+492    00AE8944      8D8D 78FFFFFF         LEA ECX,DWORD PTR SS:[EBP-0x88]
	$+498    00AE894A      E8 41AD3F00           CALL MapleSto.00EE3690
	$+49D    00AE894F      8B50 08               MOV EDX,DWORD PTR DS:[EAX+0x8]
	$+4A0    00AE8952      8B45 BC               MOV EAX,DWORD PTR SS:[EBP-0x44]
	$+4A3    00AE8955      C1E8 06               SHR EAX,0x6
	$+4A6    00AE8958      8955 E4               MOV DWORD PTR SS:[EBP-0x1C],EDX
	$+4A9    00AE895B      A8 01                 TEST AL,0x1
	$+4AB    00AE895D      74 12                 JE SHORT MapleSto.00AE8971
	$+4AD    00AE895F      8B45 C0               MOV EAX,DWORD PTR SS:[EBP-0x40]
	$+4B0    00AE8962      8B4D B8               MOV ECX,DWORD PTR SS:[EBP-0x48]
	$+4B3    00AE8965      8B11                  MOV EDX,DWORD PTR DS:[ECX]
	$+4B5    00AE8967      8B52 08               MOV EDX,DWORD PTR DS:[EDX+0x8]
	$+4B8    00AE896A      50                    PUSH EAX
	$+4B9    00AE896B      8D45 B8               LEA EAX,DWORD PTR SS:[EBP-0x48]
	$+4BC    00AE896E      50                    PUSH EAX
	$+4BD    00AE896F      FFD2                  CALL EDX
	$+4BF    00AE8971      68 54413F01           PUSH MapleSto.013F4154                   ; ASCII "height"
	$+4C4    00AE8976      8D45 B8               LEA EAX,DWORD PTR SS:[EBP-0x48]
	$+4C7    00AE8979      50                    PUSH EAX
	$+4C8    00AE897A      8D8D 78FFFFFF         LEA ECX,DWORD PTR SS:[EBP-0x88]
	$+4CE    00AE8980      E8 0BAD3F00           CALL MapleSto.00EE3690
	$+4D3    00AE8985      8B55 BC               MOV EDX,DWORD PTR SS:[EBP-0x44]
	$+4D6    00AE8988      8B48 08               MOV ECX,DWORD PTR DS:[EAX+0x8]
	$+4D9    00AE898B      C1EA 06               SHR EDX,0x6
	$+4DC    00AE898E      894D E8               MOV DWORD PTR SS:[EBP-0x18],ECX
	$+4DF    00AE8991      F6C2 01               TEST DL,0x1
	$+4E2    00AE8994      74 12                 JE SHORT MapleSto.00AE89A8
	$+4E4    00AE8996      8B55 C0               MOV EDX,DWORD PTR SS:[EBP-0x40]
	$+4E7    00AE8999      8B4D B8               MOV ECX,DWORD PTR SS:[EBP-0x48]
	$+4EA    00AE899C      8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$+4EC    00AE899E      8B40 08               MOV EAX,DWORD PTR DS:[EAX+0x8]
	$+4EF    00AE89A1      52                    PUSH EDX
	$+4F0    00AE89A2      8D55 B8               LEA EDX,DWORD PTR SS:[EBP-0x48]
	$+4F3    00AE89A5      52                    PUSH EDX
	$+4F4    00AE89A6      FFD0                  CALL EAX
	$+4F6    00AE89A8      68 04DE4701           PUSH MapleSto.0147DE04                   ; ASCII "mapAlpha"
	$+4FB    00AE89AD      8D4D B8               LEA ECX,DWORD PTR SS:[EBP-0x48]
	$+4FE    00AE89B0      51                    PUSH ECX
	$+4FF    00AE89B1      8D8D 78FFFFFF         LEA ECX,DWORD PTR SS:[EBP-0x88]
	$+505    00AE89B7      E8 D4AC3F00           CALL MapleSto.00EE3690
	$+50A    00AE89BC      8B50 08               MOV EDX,DWORD PTR DS:[EAX+0x8]
	$+50D    00AE89BF      8B45 BC               MOV EAX,DWORD PTR SS:[EBP-0x44]
	$+510    00AE89C2      C1E8 06               SHR EAX,0x6
	$+513    00AE89C5      8955 D8               MOV DWORD PTR SS:[EBP-0x28],EDX
	$+516    00AE89C8      A8 01                 TEST AL,0x1
	$+518    00AE89CA      74 12                 JE SHORT MapleSto.00AE89DE
	$+51A    00AE89CC      8B45 C0               MOV EAX,DWORD PTR SS:[EBP-0x40]
	$+51D    00AE89CF      8B4D B8               MOV ECX,DWORD PTR SS:[EBP-0x48]
	$+520    00AE89D2      8B11                  MOV EDX,DWORD PTR DS:[ECX]
	$+522    00AE89D4      8B52 08               MOV EDX,DWORD PTR DS:[EDX+0x8]
	$+525    00AE89D7      50                    PUSH EAX
	$+526    00AE89D8      8D45 B8               LEA EAX,DWORD PTR SS:[EBP-0x48]
	$+529    00AE89DB      50                    PUSH EAX
	$+52A    00AE89DC      FFD2                  CALL EDX
	$+52C    00AE89DE      68 D4074501           PUSH MapleSto.014507D4                   ; ASCII "bgAlpha"
	$+531    00AE89E3      8D45 B8               LEA EAX,DWORD PTR SS:[EBP-0x48]
	$+534    00AE89E6      50                    PUSH EAX
	$+535    00AE89E7      8D8D 78FFFFFF         LEA ECX,DWORD PTR SS:[EBP-0x88]
	$+53B    00AE89ED      E8 9EAC3F00           CALL MapleSto.00EE3690
	$+540    00AE89F2      8B55 BC               MOV EDX,DWORD PTR SS:[EBP-0x44]
	$+543    00AE89F5      8B48 08               MOV ECX,DWORD PTR DS:[EAX+0x8]
	$+546    00AE89F8      C1EA 06               SHR EDX,0x6
	$+549    00AE89FB      894D DC               MOV DWORD PTR SS:[EBP-0x24],ECX
	$+54C    00AE89FE      F6C2 01               TEST DL,0x1
	$+54F    00AE8A01      74 12                 JE SHORT MapleSto.00AE8A15
	$+551    00AE8A03      8B55 C0               MOV EDX,DWORD PTR SS:[EBP-0x40]
	$+554    00AE8A06      8B4D B8               MOV ECX,DWORD PTR SS:[EBP-0x48]
	$+557    00AE8A09      8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$+559    00AE8A0B      8B40 08               MOV EAX,DWORD PTR DS:[EAX+0x8]
	$+55C    00AE8A0E      52                    PUSH EDX
	$+55D    00AE8A0F      8D55 B8               LEA EDX,DWORD PTR SS:[EBP-0x48]
	$+560    00AE8A12      52                    PUSH EDX
	$+561    00AE8A13      FFD0                  CALL EAX
	$+563    00AE8A15      68 D0383F01           PUSH MapleSto.013F38D0                   ; ASCII "scale"
	$+568    00AE8A1A      8D4D B8               LEA ECX,DWORD PTR SS:[EBP-0x48]
	$+56B    00AE8A1D      51                    PUSH ECX
	$+56C    00AE8A1E      8D8D 78FFFFFF         LEA ECX,DWORD PTR SS:[EBP-0x88]
	$+572    00AE8A24      E8 67AC3F00           CALL MapleSto.00EE3690
	$+577    00AE8A29      8B55 BC               MOV EDX,DWORD PTR SS:[EBP-0x44]
	$+57A    00AE8A2C      F20F1040 08           MOVSD XMM0,QWORD PTR DS:[EAX+0x8]
	$+57F    00AE8A31      C1EA 06               SHR EDX,0x6
	$+582    00AE8A34      660F5AC0              CVTPD2PS XMM0,XMM0
	$+586    00AE8A38      F3:0F1145 E0          MOVSS DWORD PTR SS:[EBP-0x20],XMM0
	$+58B    00AE8A3D      F6C2 01               TEST DL,0x1
	$+58E    00AE8A40      74 12                 JE SHORT MapleSto.00AE8A54
	$+590    00AE8A42      8B55 C0               MOV EDX,DWORD PTR SS:[EBP-0x40]
	$+593    00AE8A45      8B4D B8               MOV ECX,DWORD PTR SS:[EBP-0x48]
	$+596    00AE8A48      8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$+598    00AE8A4A      8B40 08               MOV EAX,DWORD PTR DS:[EAX+0x8]
	$+59B    00AE8A4D      52                    PUSH EDX
	$+59C    00AE8A4E      8D55 B8               LEA EDX,DWORD PTR SS:[EBP-0x48]
	$+59F    00AE8A51      52                    PUSH EDX
	$+5A0    00AE8A52      FFD0                  CALL EAX
	$+5A2    00AE8A54      68 FCDD4701           PUSH MapleSto.0147DDFC                   ; ASCII "fullmap"
	$+5A7    00AE8A59      8D4D B8               LEA ECX,DWORD PTR SS:[EBP-0x48]
	$+5AA    00AE8A5C      51                    PUSH ECX
	$+5AB    00AE8A5D      8D8D 78FFFFFF         LEA ECX,DWORD PTR SS:[EBP-0x88]
	$+5B1    00AE8A63      E8 28AC3F00           CALL MapleSto.00EE3690
	$+5B6    00AE8A68      8B50 08               MOV EDX,DWORD PTR DS:[EAX+0x8]
	$+5B9    00AE8A6B      8B45 BC               MOV EAX,DWORD PTR SS:[EBP-0x44]
	$+5BC    00AE8A6E      C1E8 06               SHR EAX,0x6
	$+5BF    00AE8A71      8955 D4               MOV DWORD PTR SS:[EBP-0x2C],EDX
	$+5C2    00AE8A74      A8 01                 TEST AL,0x1
	$+5C4    00AE8A76      74 12                 JE SHORT MapleSto.00AE8A8A
	$+5C6    00AE8A78      8B45 C0               MOV EAX,DWORD PTR SS:[EBP-0x40]
	$+5C9    00AE8A7B      8B4D B8               MOV ECX,DWORD PTR SS:[EBP-0x48]
	$+5CC    00AE8A7E      8B11                  MOV EDX,DWORD PTR DS:[ECX]
	$+5CE    00AE8A80      8B52 08               MOV EDX,DWORD PTR DS:[EDX+0x8]
	$+5D1    00AE8A83      50                    PUSH EAX
	$+5D2    00AE8A84      8D45 B8               LEA EAX,DWORD PTR SS:[EBP-0x48]
	$+5D5    00AE8A87      50                    PUSH EAX
	$+5D6    00AE8A88      FFD2                  CALL EDX
	$+5D8    00AE8A8A      68 3C824701           PUSH MapleSto.0147823C                   ; ASCII "minimize"
	$+5DD    00AE8A8F      8D45 B8               LEA EAX,DWORD PTR SS:[EBP-0x48]
	$+5E0    00AE8A92      50                    PUSH EAX
	$+5E1    00AE8A93      8D8D 78FFFFFF         LEA ECX,DWORD PTR SS:[EBP-0x88]
	$+5E7    00AE8A99      E8 F2AB3F00           CALL MapleSto.00EE3690
	$+5EC    00AE8A9E      8B4D BC               MOV ECX,DWORD PTR SS:[EBP-0x44]
	$+5EF    00AE8AA1      8B58 08               MOV EBX,DWORD PTR DS:[EAX+0x8]
	$+5F2    00AE8AA4      C1E9 06               SHR ECX,0x6
	$+5F5    00AE8AA7      F6C1 01               TEST CL,0x1
	$+5F8    00AE8AAA      74 15                 JE SHORT MapleSto.00AE8AC1
	$+5FA    00AE8AAC      8B45 C0               MOV EAX,DWORD PTR SS:[EBP-0x40]
	$+5FD    00AE8AAF      8B4D B8               MOV ECX,DWORD PTR SS:[EBP-0x48]
	$+600    00AE8AB2      8B11                  MOV EDX,DWORD PTR DS:[ECX]
	$+602    00AE8AB4      8B52 08               MOV EDX,DWORD PTR DS:[EDX+0x8]
	$+605    00AE8AB7      50                    PUSH EAX
	$+606    00AE8AB8      8D45 B8               LEA EAX,DWORD PTR SS:[EBP-0x48]
	$+609    00AE8ABB      50                    PUSH EAX
	$+60A    00AE8ABC      FFD2                  CALL EDX
	$+60C    00AE8ABE      8975 B8               MOV DWORD PTR SS:[EBP-0x48],ESI
	$+60F    00AE8AC1      8D8D 78FFFFFF         LEA ECX,DWORD PTR SS:[EBP-0x88]
	$+615    00AE8AC7      8975 BC               MOV DWORD PTR SS:[EBP-0x44],ESI
	$+618    00AE8ACA      C645 FC 00            MOV BYTE PTR SS:[EBP-0x4],0x0
	$+61C    00AE8ACE      E8 3DB03F00           CALL MapleSto.00EE3B10
	$+621    00AE8AD3      D945 F0               FLD DWORD PTR SS:[EBP-0x10]
	$+624    00AE8AD6      8B75 08               MOV ESI,DWORD PTR SS:[EBP+0x8]
	$+627    00AE8AD9      6A 01                 PUSH 0x1
	$+629    00AE8ADB      51                    PUSH ECX
	$+62A    00AE8ADC      D91C24                FSTP DWORD PTR SS:[ESP]
	$+62D    00AE8ADF      68 0C384101           PUSH MapleSto.0141380C                   ; UNICODE "x"
	$+632    00AE8AE4      68 44064101           PUSH MapleSto.01410644                   ; ASCII "transform"
	$+637    00AE8AE9      56                    PUSH ESI
	$+638    00AE8AEA      E8 117CA8FF           CALL MapleSto.00570700
	$+63D    00AE8AEF      D945 EC               FLD DWORD PTR SS:[EBP-0x14]
	$+640    00AE8AF2      83C4 14               ADD ESP,0x14
	$+643    00AE8AF5      6A 01                 PUSH 0x1
	$+645    00AE8AF7      51                    PUSH ECX
	$+646    00AE8AF8      D91C24                FSTP DWORD PTR SS:[ESP]
	$+649    00AE8AFB      68 08384101           PUSH MapleSto.01413808                   ; UNICODE "y"
	$+64E    00AE8B00      68 44064101           PUSH MapleSto.01410644                   ; ASCII "transform"
	$+653    00AE8B05      56                    PUSH ESI
	$+654    00AE8B06      E8 F57BA8FF           CALL MapleSto.00570700
	$+659    00AE8B0B      A1 E04E7201           MOV EAX,DWORD PTR DS:[0x1724EE0]
	$+65E    00AE8B10      D940 18               FLD DWORD PTR DS:[EAX+0x18]
	$+661    00AE8B13      8B0D 30967301         MOV ECX,DWORD PTR DS:[0x1739630]
	$+667    00AE8B19      83C4 0C               ADD ESP,0xC
	$+66A    00AE8B1C      D95C24 04             FSTP DWORD PTR SS:[ESP+0x4]
	$+66E    00AE8B20      DB05 30967301         FILD DWORD PTR DS:[0x1739630]
	$+674    00AE8B26      85C9                  TEST ECX,ECX
	$+676    00AE8B28      7D 06                 JGE SHORT MapleSto.00AE8B30
	$+678    00AE8B2A      D805 E42D3F01         FADD DWORD PTR DS:[0x13F2DE4]
	$+67E    00AE8B30      D91C24                FSTP DWORD PTR SS:[ESP]
	$+681    00AE8B33      E8 08B5D1FF           CALL MapleSto.00804040
	*/
	addr_tmp = scan_exe_.ScanAddr("506A0A8D4D0C518B0D", 0);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = AsDword(addr_tmp, 7 + 2);
	CheckExeAddr(dw_tmp);
	ofs << "pt_cdword	g_a_xml_string_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;
	addr_tmp1 = Cmn_FeatureCodeScan::CallAddr2AbsoluteAddr((pt_pcbyte)addr_tmp + 0xd);
	if (!addr_tmp1)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_c_find_xml_str_by_id_ = " << addr_tmp1 << ";" << FC_OFstream_EndlLog;
	return true;
}

bool CAutoUpdateApp::Scan_Ui(Cmn_FC_OFstream& ofs)
{
	pt_pcvoid addr_tmp;
	pt_dword dw_tmp;
	boost::optional<pt_dword> op_dw_tmp;

	/*
	$-C3     007A343F      CC                        INT3
	$-C2     007A3440      55                        PUSH EBP
	$-C1     007A3441      8BEC                      MOV EBP,ESP
	$-BF     007A3443      56                        PUSH ESI
	$-BE     007A3444      8BF1                      MOV ESI,ECX
	$-BC     007A3446      E8 95F4FFFF               CALL MapleSto.007A28E0
	$-B7     007A344B      F645 08 01                TEST BYTE PTR SS:[EBP+0x8],0x1
	$-B3     007A344F      74 09                     JE SHORT MapleSto.007A345A
	$-B1     007A3451      56                        PUSH ESI
	$-B0     007A3452      E8 EDDAA500               CALL MapleSto.01200F44                          ; JMP to mfc90.#800
	$-AB     007A3457      83C4 04                   ADD ESP,0x4
	$-A8     007A345A      8BC6                      MOV EAX,ESI
	$-A6     007A345C      5E                        POP ESI
	$-A5     007A345D      5D                        POP EBP
	$-A4     007A345E      C2 0400                   RETN 0x4
	$-A1     007A3461      CC                        INT3
	$-A0     007A3462      CC                        INT3
	$-9F     007A3463      CC                        INT3
	$-9E     007A3464      CC                        INT3
	$-9D     007A3465      CC                        INT3
	$-9C     007A3466      CC                        INT3
	$-9B     007A3467      CC                        INT3
	$-9A     007A3468      CC                        INT3
	$-99     007A3469      CC                        INT3
	$-98     007A346A      CC                        INT3
	$-97     007A346B      CC                        INT3
	$-96     007A346C      CC                        INT3
	$-95     007A346D      CC                        INT3
	$-94     007A346E      CC                        INT3
	$-93     007A346F      CC                        INT3
	$-92     007A3470      C2 0400                   RETN 0x4
	$-8F     007A3473      CC                        INT3
	$-8E     007A3474      CC                        INT3
	$-8D     007A3475      CC                        INT3
	$-8C     007A3476      CC                        INT3
	$-8B     007A3477      CC                        INT3
	$-8A     007A3478      CC                        INT3
	$-89     007A3479      CC                        INT3
	$-88     007A347A      CC                        INT3
	$-87     007A347B      CC                        INT3
	$-86     007A347C      CC                        INT3
	$-85     007A347D      CC                        INT3
	$-84     007A347E      CC                        INT3
	$-83     007A347F      CC                        INT3
	$-82     007A3480      55                        PUSH EBP
	$-81     007A3481      8BEC                      MOV EBP,ESP
	$-7F     007A3483      8A45 08                   MOV AL,BYTE PTR SS:[EBP+0x8]
	$-7C     007A3486      8841 05                   MOV BYTE PTR DS:[ECX+0x5],AL
	$-79     007A3489      5D                        POP EBP
	$-78     007A348A      C2 0400                   RETN 0x4
	$-75     007A348D      CC                        INT3
	$-74     007A348E      CC                        INT3
	$-73     007A348F      CC                        INT3
	$-72     007A3490      8BC1                      MOV EAX,ECX
	$-70     007A3492      A3 2C407201               MOV DWORD PTR DS:[0x172402C],EAX
	$-6B     007A3497      32C9                      XOR CL,CL
	$-69     007A3499      C700 7C614401             MOV DWORD PTR DS:[EAX],MapleSto.0144617C
	$-63     007A349F      8848 04                   MOV BYTE PTR DS:[EAX+0x4],CL
	$-60     007A34A2      8848 05                   MOV BYTE PTR DS:[EAX+0x5],CL
	$-5D     007A34A5      8848 06                   MOV BYTE PTR DS:[EAX+0x6],CL
	$-5A     007A34A8      8848 07                   MOV BYTE PTR DS:[EAX+0x7],CL
	$-57     007A34AB      C3                        RETN
	$-56     007A34AC      CC                        INT3
	$-55     007A34AD      CC                        INT3
	$-54     007A34AE      CC                        INT3
	$-53     007A34AF      CC                        INT3
	$-52     007A34B0      C701 74614401             MOV DWORD PTR DS:[ECX],MapleSto.01446174
	$-4C     007A34B6      C705 2C407201 00000000    MOV DWORD PTR DS:[0x172402C],0x0
	$-42     007A34C0      C3                        RETN
	$-41     007A34C1      CC                        INT3
	$-40     007A34C2      CC                        INT3
	$-3F     007A34C3      CC                        INT3
	$-3E     007A34C4      CC                        INT3
	$-3D     007A34C5      CC                        INT3
	$-3C     007A34C6      CC                        INT3
	$-3B     007A34C7      CC                        INT3
	$-3A     007A34C8      CC                        INT3
	$-39     007A34C9      CC                        INT3
	$-38     007A34CA      CC                        INT3
	$-37     007A34CB      CC                        INT3
	$-36     007A34CC      CC                        INT3
	$-35     007A34CD      CC                        INT3
	$-34     007A34CE      CC                        INT3
	$-33     007A34CF      CC                        INT3
	$-32     007A34D0      A1 C04A7201               MOV EAX,DWORD PTR DS:[0x1724AC0]
	$-2D     007A34D5      8D50 FF                   LEA EDX,DWORD PTR DS:[EAX-0x1]
	$-2A     007A34D8      83FA 63                   CMP EDX,0x63
	$-27     007A34DB      77 08                     JA SHORT MapleSto.007A34E5
	$-25     007A34DD      83F8 32                   CMP EAX,0x32
	$-22     007A34E0      7D 03                     JGE SHORT MapleSto.007A34E5
	$-20     007A34E2      32C0                      XOR AL,AL
	$-1E     007A34E4      C3                        RETN
	$-1D     007A34E5      8A41 04                   MOV AL,BYTE PTR DS:[ECX+0x4]
	$-1A     007A34E8      C3                        RETN
	$-19     007A34E9      CC                        INT3
	$-18     007A34EA      CC                        INT3
	$-17     007A34EB      CC                        INT3
	$-16     007A34EC      CC                        INT3
	$-15     007A34ED      CC                        INT3
	$-14     007A34EE      CC                        INT3
	$-13     007A34EF      CC                        INT3
	$-12     007A34F0      55                        PUSH EBP
	$-11     007A34F1      8BEC                      MOV EBP,ESP
	$-F      007A34F3      53                        PUSH EBX
	$-E      007A34F4      56                        PUSH ESI
	$-D      007A34F5      8BF1                      MOV ESI,ECX
	$-B      007A34F7      8B0D 48417201             MOV ECX,DWORD PTR DS:[0x1724148]                ; g_a_ui_mgr_
	$-5      007A34FD      E8 EEFE3700               CALL MapleSto.00B233F0
	$ ==>    007A3502      8A5D 08                   MOV BL,BYTE PTR SS:[EBP+0x8]
	$+3      007A3505      85C0                      TEST EAX,EAX
	$+5      007A3507      74 0D                     JE SHORT MapleSto.007A3516
	$+7      007A3509      F6C3 02                   TEST BL,0x2
	$+A      007A350C      74 08                     JE SHORT MapleSto.007A3516
	$+C      007A350E      5E                        POP ESI
	$+D      007A350F      B0 01                     MOV AL,0x1
	$+F      007A3511      5B                        POP EBX
	$+10     007A3512      5D                        POP EBP
	$+11     007A3513      C2 0400                   RETN 0x4
	$+14     007A3516      8B0D 48667201             MOV ECX,DWORD PTR DS:[0x1726648]
	$+1A     007A351C      E8 8FB4E9FF               CALL MapleSto.0063E9B0
	$+1F     007A3521      84C0                      TEST AL,AL
	$+21     007A3523      74 05                     JE SHORT MapleSto.007A352A
	$+23     007A3525      F6C3 04                   TEST BL,0x4
	$+26     007A3528    ^ 75 E4                     JNZ SHORT MapleSto.007A350E
	$+28     007A352A      807E 05 00                CMP BYTE PTR DS:[ESI+0x5],0x0
	$+2C     007A352E      74 05                     JE SHORT MapleSto.007A3535
	$+2E     007A3530      F6C3 01                   TEST BL,0x1
	$+31     007A3533    ^ 75 D9                     JNZ SHORT MapleSto.007A350E
	$+33     007A3535      5E                        POP ESI
	$+34     007A3536      32C0                      XOR AL,AL
	$+36     007A3538      5B                        POP EBX
	$+37     007A3539      5D                        POP EBP
	$+38     007A353A      C2 0400                   RETN 0x4
	$+3B     007A353D      CC                        INT3
	$+3C     007A353E      CC                        INT3
	$+3D     007A353F      CC                        INT3
	$+3E     007A3540      8B0D 94607201             MOV ECX,DWORD PTR DS:[0x1726094]
	$+44     007A3546      6A 01                     PUSH 0x1
	$+46     007A3548      E8 334FECFF               CALL MapleSto.00668480
	$+4B     007A354D      84C0                      TEST AL,AL
	$+4D     007A354F      74 03                     JE SHORT MapleSto.007A3554
	$+4F     007A3551      B0 01                     MOV AL,0x1
	$+51     007A3553      C3                        RETN
	$+52     007A3554      8B0D 74517201             MOV ECX,DWORD PTR DS:[0x1725174]
	$+58     007A355A      68 09940400               PUSH 0x49409
	$+5D     007A355F      E8 EC821200               CALL MapleSto.008CB850
	$+62     007A3564      85C0                      TEST EAX,EAX
	$+64     007A3566      0F95C0                    SETNE AL
	$+67     007A3569      C3                        RETN
	$+68     007A356A      CC                        INT3
	$+69     007A356B      CC                        INT3
	$+6A     007A356C      CC                        INT3
	$+6B     007A356D      CC                        INT3
	$+6C     007A356E      CC                        INT3
	$+6D     007A356F      CC                        INT3
	$+6E     007A3570      8B0D 94607201             MOV ECX,DWORD PTR DS:[0x1726094]
	$+74     007A3576      6A 01                     PUSH 0x1
	$+76     007A3578      E8 8351ECFF               CALL MapleSto.00668700
	$+7B     007A357D      84C0                      TEST AL,AL
	$+7D     007A357F      74 03                     JE SHORT MapleSto.007A3584
	$+7F     007A3581      B0 01                     MOV AL,0x1
	$+81     007A3583      C3                        RETN
	$+82     007A3584      8B0D 74517201             MOV ECX,DWORD PTR DS:[0x1725174]
	$+88     007A358A      68 09940400               PUSH 0x49409
	$+8D     007A358F      E8 0C841200               CALL MapleSto.008CB9A0
	$+92     007A3594      85C0                      TEST EAX,EAX
	$+94     007A3596      0F95C0                    SETNE AL
	$+97     007A3599      C3                        RETN
	$+98     007A359A      CC                        INT3
	$+99     007A359B      CC                        INT3
	$+9A     007A359C      CC                        INT3
	$+9B     007A359D      CC                        INT3
	$+9C     007A359E      CC                        INT3
	$+9D     007A359F      CC                        INT3
	$+9E     007A35A0      8B0D 94607201             MOV ECX,DWORD PTR DS:[0x1726094]
	$+A4     007A35A6      6A 1C                     PUSH 0x1C
	$+A6     007A35A8      E8 D34EECFF               CALL MapleSto.00668480
	$+AB     007A35AD      84C0                      TEST AL,AL
	$+AD     007A35AF      74 03                     JE SHORT MapleSto.007A35B4
	$+AF     007A35B1      B0 01                     MOV AL,0x1
	$+B1     007A35B3      C3                        RETN
	$+B2     007A35B4      8B0D 74517201             MOV ECX,DWORD PTR DS:[0x1725174]
	$+B8     007A35BA      68 08940400               PUSH 0x49408
	$+BD     007A35BF      E8 8C821200               CALL MapleSto.008CB850
	$+C2     007A35C4      85C0                      TEST EAX,EAX
	$+C4     007A35C6      0F95C0                    SETNE AL
	$+C7     007A35C9      C3                        RETN
	$+C8     007A35CA      CC                        INT3
	$+C9     007A35CB      CC                        INT3
	$+CA     007A35CC      CC                        INT3
	$+CB     007A35CD      CC                        INT3
	$+CC     007A35CE      CC                        INT3
	$+CD     007A35CF      CC                        INT3
	$+CE     007A35D0      8B0D 94607201             MOV ECX,DWORD PTR DS:[0x1726094]
	$+D4     007A35D6      6A 1C                     PUSH 0x1C
	$+D6     007A35D8      E8 2351ECFF               CALL MapleSto.00668700
	$+DB     007A35DD      84C0                      TEST AL,AL
	$+DD     007A35DF      74 03                     JE SHORT MapleSto.007A35E4
	$+DF     007A35E1      B0 01                     MOV AL,0x1
	$+E1     007A35E3      C3                        RETN
	$+E2     007A35E4      8B0D 74517201             MOV ECX,DWORD PTR DS:[0x1725174]
	$+E8     007A35EA      68 08940400               PUSH 0x49408
	$+ED     007A35EF      E8 AC831200               CALL MapleSto.008CB9A0
	$+F2     007A35F4      85C0                      TEST EAX,EAX
	$+F4     007A35F6      0F95C0                    SETNE AL
	$+F7     007A35F9      C3                        RETN
	$+F8     007A35FA      CC                        INT3
	$+F9     007A35FB      CC                        INT3
	$+FA     007A35FC      CC                        INT3
	$+FB     007A35FD      CC                        INT3
	$+FC     007A35FE      CC                        INT3
	$+FD     007A35FF      CC                        INT3
	$+FE     007A3600      55                        PUSH EBP
	$+FF     007A3601      8BEC                      MOV EBP,ESP
	$+101    007A3603      807D 0C 00                CMP BYTE PTR SS:[EBP+0xC],0x0
	$+105    007A3607      A1 C04A7201               MOV EAX,DWORD PTR DS:[0x1724AC0]
	$+10A    007A360C      74 19                     JE SHORT MapleSto.007A3627
	$+10C    007A360E      8D50 FF                   LEA EDX,DWORD PTR DS:[EAX-0x1]
	$+10F    007A3611      83FA 63                   CMP EDX,0x63
	$+112    007A3614      77 05                     JA SHORT MapleSto.007A361B
	$+114    007A3616      83F8 32                   CMP EAX,0x32
	$+117    007A3619      7C 06                     JL SHORT MapleSto.007A3621
	$+119    007A361B      8079 04 00                CMP BYTE PTR DS:[ECX+0x4],0x0
	$+11D    007A361F      75 06                     JNZ SHORT MapleSto.007A3627
	$+11F    007A3621      32C0                      XOR AL,AL
	$+121    007A3623      5D                        POP EBP
	$+122    007A3624      C2 0C00                   RETN 0xC
	$+125    007A3627      3B45 10                   CMP EAX,DWORD PTR SS:[EBP+0x10]
	$+128    007A362A    ^ 7C F5                     JL SHORT MapleSto.007A3621
	$+12A    007A362C      8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+12D    007A362F      8B0D 94607201             MOV ECX,DWORD PTR DS:[0x1726094]
	$+133    007A3635      50                        PUSH EAX
	$+134    007A3636      E8 854CECFF               CALL MapleSto.006682C0
	$+139    007A363B      5D                        POP EBP
	$+13A    007A363C      C2 0C00                   RETN 0xC
	$+13D    007A363F      CC                        INT3
	$+13E    007A3640      55                        PUSH EBP
	$+13F    007A3641      8BEC                      MOV EBP,ESP
	$+141    007A3643      807D 0C 00                CMP BYTE PTR SS:[EBP+0xC],0x0
	$+145    007A3647      A1 C04A7201               MOV EAX,DWORD PTR DS:[0x1724AC0]
	$+14A    007A364C      74 19                     JE SHORT MapleSto.007A3667
	$+14C    007A364E      8D50 FF                   LEA EDX,DWORD PTR DS:[EAX-0x1]
	$+14F    007A3651      83FA 63                   CMP EDX,0x63
	$+152    007A3654      77 05                     JA SHORT MapleSto.007A365B
	$+154    007A3656      83F8 32                   CMP EAX,0x32
	$+157    007A3659      7C 06                     JL SHORT MapleSto.007A3661
	$+159    007A365B      8079 04 00                CMP BYTE PTR DS:[ECX+0x4],0x0
	$+15D    007A365F      75 06                     JNZ SHORT MapleSto.007A3667
	$+15F    007A3661      32C0                      XOR AL,AL
	$+161    007A3663      5D                        POP EBP
	$+162    007A3664      C2 0C00                   RETN 0xC
	$+165    007A3667      3B45 10                   CMP EAX,DWORD PTR SS:[EBP+0x10]
	$+168    007A366A    ^ 7C F5                     JL SHORT MapleSto.007A3661
	$+16A    007A366C      8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+16D    007A366F      8B0D 94607201             MOV ECX,DWORD PTR DS:[0x1726094]
	$+173    007A3675      50                        PUSH EAX
	$+174    007A3676      E8 054EECFF               CALL MapleSto.00668480
	$+179    007A367B      5D                        POP EBP
	$+17A    007A367C      C2 0C00                   RETN 0xC
	$+17D    007A367F      CC                        INT3
	$+17E    007A3680      55                        PUSH EBP
	$+17F    007A3681      8BEC                      MOV EBP,ESP
	$+181    007A3683      807D 0C 00                CMP BYTE PTR SS:[EBP+0xC],0x0
	$+185    007A3687      A1 C04A7201               MOV EAX,DWORD PTR DS:[0x1724AC0]
	$+18A    007A368C      74 19                     JE SHORT MapleSto.007A36A7
	$+18C    007A368E      8D50 FF                   LEA EDX,DWORD PTR DS:[EAX-0x1]
	$+18F    007A3691      83FA 63                   CMP EDX,0x63
	$+192    007A3694      77 05                     JA SHORT MapleSto.007A369B
	$+194    007A3696      83F8 32                   CMP EAX,0x32
	$+197    007A3699      7C 06                     JL SHORT MapleSto.007A36A1
	$+199    007A369B      8079 04 00                CMP BYTE PTR DS:[ECX+0x4],0x0
	$+19D    007A369F      75 06                     JNZ SHORT MapleSto.007A36A7
	$+19F    007A36A1      32C0                      XOR AL,AL
	$+1A1    007A36A3      5D                        POP EBP
	$+1A2    007A36A4      C2 0C00                   RETN 0xC
	$+1A5    007A36A7      3B45 10                   CMP EAX,DWORD PTR SS:[EBP+0x10]
	$+1A8    007A36AA    ^ 7C F5                     JL SHORT MapleSto.007A36A1
	$+1AA    007A36AC      8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+1AD    007A36AF      8B0D 94607201             MOV ECX,DWORD PTR DS:[0x1726094]
	$+1B3    007A36B5      50                        PUSH EAX
	$+1B4    007A36B6      E8 4550ECFF               CALL MapleSto.00668700
	$+1B9    007A36BB      5D                        POP EBP
	$+1BA    007A36BC      C2 0C00                   RETN 0xC
	$+1BD    007A36BF      CC                        INT3
	$+1BE    007A36C0      55                        PUSH EBP
	$+1BF    007A36C1      8BEC                      MOV EBP,ESP
	$+1C1    007A36C3      807D 0C 00                CMP BYTE PTR SS:[EBP+0xC],0x0
	$+1C5    007A36C7      A1 C04A7201               MOV EAX,DWORD PTR DS:[0x1724AC0]
	$+1CA    007A36CC      74 19                     JE SHORT MapleSto.007A36E7
	$+1CC    007A36CE      8D50 FF                   LEA EDX,DWORD PTR DS:[EAX-0x1]
	$+1CF    007A36D1      83FA 63                   CMP EDX,0x63
	$+1D2    007A36D4      77 05                     JA SHORT MapleSto.007A36DB
	$+1D4    007A36D6      83F8 32                   CMP EAX,0x32
	$+1D7    007A36D9      7C 06                     JL SHORT MapleSto.007A36E1
	$+1D9    007A36DB      8079 04 00                CMP BYTE PTR DS:[ECX+0x4],0x0
	$+1DD    007A36DF      75 06                     JNZ SHORT MapleSto.007A36E7
	$+1DF    007A36E1      32C0                      XOR AL,AL
	$+1E1    007A36E3      5D                        POP EBP
	$+1E2    007A36E4      C2 0C00                   RETN 0xC
	$+1E5    007A36E7      3B45 10                   CMP EAX,DWORD PTR SS:[EBP+0x10]
	$+1E8    007A36EA    ^ 7C F5                     JL SHORT MapleSto.007A36E1
	$+1EA    007A36EC      8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+1ED    007A36EF      8B0D 94607201             MOV ECX,DWORD PTR DS:[0x1726094]
	$+1F3    007A36F5      50                        PUSH EAX
	$+1F4    007A36F6      E8 854AECFF               CALL MapleSto.00668180
	$+1F9    007A36FB      5D                        POP EBP
	$+1FA    007A36FC      C2 0C00                   RETN 0xC
	$+1FD    007A36FF      CC                        INT3
	$+1FE    007A3700      55                        PUSH EBP
	$+1FF    007A3701      8BEC                      MOV EBP,ESP
	$+201    007A3703      807D 0C 00                CMP BYTE PTR SS:[EBP+0xC],0x0
	$+205    007A3707      A1 C04A7201               MOV EAX,DWORD PTR DS:[0x1724AC0]
	$+20A    007A370C      74 19                     JE SHORT MapleSto.007A3727
	$+20C    007A370E      8D50 FF                   LEA EDX,DWORD PTR DS:[EAX-0x1]
	$+20F    007A3711      83FA 63                   CMP EDX,0x63
	$+212    007A3714      77 05                     JA SHORT MapleSto.007A371B
	$+214    007A3716      83F8 32                   CMP EAX,0x32
	$+217    007A3719      7C 06                     JL SHORT MapleSto.007A3721
	$+219    007A371B      8079 04 00                CMP BYTE PTR DS:[ECX+0x4],0x0
	$+21D    007A371F      75 06                     JNZ SHORT MapleSto.007A3727
	$+21F    007A3721      32C0                      XOR AL,AL
	$+221    007A3723      5D                        POP EBP
	$+222    007A3724      C2 0C00                   RETN 0xC
	$+225    007A3727      3B45 10                   CMP EAX,DWORD PTR SS:[EBP+0x10]
	$+228    007A372A    ^ 7C F5                     JL SHORT MapleSto.007A3721
	$+22A    007A372C      8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+22D    007A372F      8B0D 94607201             MOV ECX,DWORD PTR DS:[0x1726094]
	$+233    007A3735      50                        PUSH EAX
	$+234    007A3736      E8 854AECFF               CALL MapleSto.006681C0
	$+239    007A373B      5D                        POP EBP
	$+23A    007A373C      C2 0C00                   RETN 0xC
	$+23D    007A373F      CC                        INT3
	$+23E    007A3740      55                        PUSH EBP
	$+23F    007A3741      8BEC                      MOV EBP,ESP
	$+241    007A3743      807D 0C 00                CMP BYTE PTR SS:[EBP+0xC],0x0
	$+245    007A3747      A1 C04A7201               MOV EAX,DWORD PTR DS:[0x1724AC0]
	$+24A    007A374C      74 19                     JE SHORT MapleSto.007A3767
	$+24C    007A374E      8D50 FF                   LEA EDX,DWORD PTR DS:[EAX-0x1]
	$+24F    007A3751      83FA 63                   CMP EDX,0x63
	$+252    007A3754      77 05                     JA SHORT MapleSto.007A375B
	$+254    007A3756      83F8 32                   CMP EAX,0x32
	$+257    007A3759      7C 06                     JL SHORT MapleSto.007A3761
	$+259    007A375B      8079 04 00                CMP BYTE PTR DS:[ECX+0x4],0x0
	$+25D    007A375F      75 06                     JNZ SHORT MapleSto.007A3767
	$+25F    007A3761      32C0                      XOR AL,AL
	$+261    007A3763      5D                        POP EBP
	$+262    007A3764      C2 0C00                   RETN 0xC
	$+265    007A3767      3B45 10                   CMP EAX,DWORD PTR SS:[EBP+0x10]
	$+268    007A376A    ^ 7C F5                     JL SHORT MapleSto.007A3761
	$+26A    007A376C      8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+26D    007A376F      8B0D 94607201             MOV ECX,DWORD PTR DS:[0x1726094]
	$+273    007A3775      50                        PUSH EAX
	$+274    007A3776      E8 854AECFF               CALL MapleSto.00668200
	$+279    007A377B      5D                        POP EBP
	$+27A    007A377C      C2 0C00                   RETN 0xC
	$+27D    007A377F      CC                        INT3
	$+27E    007A3780      55                        PUSH EBP
	$+27F    007A3781      8BEC                      MOV EBP,ESP
	$+281    007A3783      807D 14 00                CMP BYTE PTR SS:[EBP+0x14],0x0
	$+285    007A3787      A1 C04A7201               MOV EAX,DWORD PTR DS:[0x1724AC0]
	$+28A    007A378C      74 19                     JE SHORT MapleSto.007A37A7
	$+28C    007A378E      8D50 FF                   LEA EDX,DWORD PTR DS:[EAX-0x1]
	$+28F    007A3791      83FA 63                   CMP EDX,0x63
	$+292    007A3794      77 05                     JA SHORT MapleSto.007A379B
	$+294    007A3796      83F8 32                   CMP EAX,0x32
	$+297    007A3799      7C 06                     JL SHORT MapleSto.007A37A1
	$+299    007A379B      8079 04 00                CMP BYTE PTR DS:[ECX+0x4],0x0
	$+29D    007A379F      75 06                     JNZ SHORT MapleSto.007A37A7
	$+29F    007A37A1      32C0                      XOR AL,AL
	$+2A1    007A37A3      5D                        POP EBP
	$+2A2    007A37A4      C2 1400                   RETN 0x14
	$+2A5    007A37A7      3B45 18                   CMP EAX,DWORD PTR SS:[EBP+0x18]
	$+2A8    007A37AA    ^ 7C F5                     JL SHORT MapleSto.007A37A1
	$+2AA    007A37AC      833D F89E7301 00          CMP DWORD PTR DS:[0x1739EF8],0x0
	$+2B1    007A37B3    ^ 74 EC                     JE SHORT MapleSto.007A37A1
	$+2B3    007A37B5      8B0D 48417201             MOV ECX,DWORD PTR DS:[0x1724148]
	$+2B9    007A37BB      E8 50FD3700               CALL MapleSto.00B23510
	$+2BE    007A37C0      8B4D 08                   MOV ECX,DWORD PTR SS:[EBP+0x8]
	$+2C1    007A37C3      85C0                      TEST EAX,EAX
	$+2C3    007A37C5      8B45 0C                   MOV EAX,DWORD PTR SS:[EBP+0xC]
	$+2C6    007A37C8      74 1F                     JE SHORT MapleSto.007A37E9
	$+2C8    007A37CA      56                        PUSH ESI
	$+2C9    007A37CB      8B75 10                   MOV ESI,DWORD PTR SS:[EBP+0x10]
	$+2CC    007A37CE      56                        PUSH ESI
	$+2CD    007A37CF      50                        PUSH EAX
	$+2CE    007A37D0      51                        PUSH ECX
	$+2CF    007A37D1      8B0D 94607201             MOV ECX,DWORD PTR DS:[0x1726094]
	$+2D5    007A37D7      E8 644AECFF               CALL MapleSto.00668240
	$+2DA    007A37DC      C706 00000000             MOV DWORD PTR DS:[ESI],0x0
	$+2E0    007A37E2      32C0                      XOR AL,AL
	$+2E2    007A37E4      5E                        POP ESI
	$+2E3    007A37E5      5D                        POP EBP
	$+2E4    007A37E6      C2 1400                   RETN 0x14
	$+2E7    007A37E9      8B55 10                   MOV EDX,DWORD PTR SS:[EBP+0x10]
	$+2EA    007A37EC      52                        PUSH EDX
	$+2EB    007A37ED      50                        PUSH EAX
	$+2EC    007A37EE      51                        PUSH ECX
	$+2ED    007A37EF      8B0D 94607201             MOV ECX,DWORD PTR DS:[0x1726094]
	$+2F3    007A37F5      E8 464AECFF               CALL MapleSto.00668240
	$+2F8    007A37FA      5D                        POP EBP
	$+2F9    007A37FB      C2 1400                   RETN 0x14
	$+2FC    007A37FE      CC                        INT3
	*/
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("8A5D0885C074", -0xb + 2);
	if (!op_dw_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *op_dw_tmp;
	CheckExeAddr(dw_tmp);
	ofs << "pt_cdword	g_a_ui_mgr_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	/*
	$-2B1    00AD060F     CC                        INT3
	$-2B0    00AD0610     55                        PUSH EBP
	$-2AF    00AD0611     8BEC                      MOV EBP,ESP
	$-2AD    00AD0613     6A FF                     PUSH -0x1
	$-2AB    00AD0615     68 FFC13801               PUSH MapleSto.0138C1FF
	$-2A6    00AD061A     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$-2A0    00AD0620     50                        PUSH EAX
	$-29F    00AD0621     83EC 08                   SUB ESP,0x8
	$-29C    00AD0624     56                        PUSH ESI
	$-29B    00AD0625     A1 D0687101               MOV EAX,DWORD PTR DS:[0x17168D0]
	$-296    00AD062A     33C5                      XOR EAX,EBP
	$-294    00AD062C     50                        PUSH EAX
	$-293    00AD062D     8D45 F4                   LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-290    00AD0630     64:A3 00000000            MOV DWORD PTR FS:[0],EAX
	$-28A    00AD0636     8D45 EC                   LEA EAX,DWORD PTR SS:[EBP-0x14]
	$-287    00AD0639     50                        PUSH EAX
	$-286    00AD063A     E8 B1F9FFFF               CALL MapleSto.00ACFFF0
	$-281    00AD063F     50                        PUSH EAX
	$-280    00AD0640     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$-279    00AD0647     E8 34944100               CALL MapleSto.00EE9A80
	$-274    00AD064C     83C4 04                   ADD ESP,0x4
	$-271    00AD064F     C745 F0 00000000          MOV DWORD PTR SS:[EBP-0x10],0x0
	$-26A    00AD0656     85C0                      TEST EAX,EAX
	$-268    00AD0658     74 1B                     JE SHORT MapleSto.00AD0675
	$-266    00AD065A     8B10                      MOV EDX,DWORD PTR DS:[EAX]
	$-264    00AD065C     8B52 0C                   MOV EDX,DWORD PTR DS:[EDX+0xC]
	$-261    00AD065F     8D4D F0                   LEA ECX,DWORD PTR SS:[EBP-0x10]
	$-25E    00AD0662     51                        PUSH ECX
	$-25D    00AD0663     68 2A002006               PUSH 0x620002A
	$-258    00AD0668     8BC8                      MOV ECX,EAX
	$-256    00AD066A     FFD2                      CALL EDX
	$-254    00AD066C     84C0                      TEST AL,AL
	$-252    00AD066E     74 05                     JE SHORT MapleSto.00AD0675
	$-250    00AD0670     8B75 F0                   MOV ESI,DWORD PTR SS:[EBP-0x10]
	$-24D    00AD0673     EB 02                     JMP SHORT MapleSto.00AD0677
	$-24B    00AD0675     33F6                      XOR ESI,ESI
	$-249    00AD0677     8D4D EC                   LEA ECX,DWORD PTR SS:[EBP-0x14]
	$-246    00AD067A     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-23F    00AD0681     FF15 08A23E01             CALL DWORD PTR DS:[0x13EA208]            ; mfc90.#601
	$-239    00AD0687     85F6                      TEST ESI,ESI
	$-237    00AD0689     74 41                     JE SHORT MapleSto.00AD06CC
	$-235    00AD068B     8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-232    00AD068E     50                        PUSH EAX
	$-231    00AD068F     8BCE                      MOV ECX,ESI
	$-22F    00AD0691     E8 9CFA4101               CALL MapleSto.01EF0132
	$-22A    00AD0696     C745 08 00000000          MOV DWORD PTR SS:[EBP+0x8],0x0
	$-223    00AD069D     85C0                      TEST EAX,EAX
	$-221    00AD069F     74 2B                     JE SHORT MapleSto.00AD06CC
	$-21F    00AD06A1     8B10                      MOV EDX,DWORD PTR DS:[EAX]
	$-21D    00AD06A3     8B52 0C                   MOV EDX,DWORD PTR DS:[EDX+0xC]
	$-21A    00AD06A6     8D4D 08                   LEA ECX,DWORD PTR SS:[EBP+0x8]
	$-217    00AD06A9     51                        PUSH ECX
	$-216    00AD06AA     68 25002006               PUSH 0x6200025
	$-211    00AD06AF     8BC8                      MOV ECX,EAX
	$-20F    00AD06B1     FFD2                      CALL EDX
	$-20D    00AD06B3     84C0                      TEST AL,AL
	$-20B    00AD06B5     74 15                     JE SHORT MapleSto.00AD06CC
	$-209    00AD06B7     8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-206    00AD06BA     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-203    00AD06BD     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$-1FC    00AD06C4     59                        POP ECX
	$-1FB    00AD06C5     5E                        POP ESI
	$-1FA    00AD06C6     8BE5                      MOV ESP,EBP
	$-1F8    00AD06C8     5D                        POP EBP
	$-1F7    00AD06C9     C2 0400                   RETN 0x4
	$-1F4    00AD06CC     33C0                      XOR EAX,EAX
	$-1F2    00AD06CE     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-1EF    00AD06D1     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$-1E8    00AD06D8     59                        POP ECX
	$-1E7    00AD06D9     5E                        POP ESI
	$-1E6    00AD06DA     8BE5                      MOV ESP,EBP
	$-1E4    00AD06DC     5D                        POP EBP
	$-1E3    00AD06DD     C2 0400                   RETN 0x4
	$-1E0    00AD06E0     55                        PUSH EBP
	$-1DF    00AD06E1     8BEC                      MOV EBP,ESP
	$-1DD    00AD06E3     6A FF                     PUSH -0x1
	$-1DB    00AD06E5     68 23C23801               PUSH MapleSto.0138C223
	$-1D6    00AD06EA     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$-1D0    00AD06F0     50                        PUSH EAX
	$-1CF    00AD06F1     83EC 0C                   SUB ESP,0xC
	$-1CC    00AD06F4     53                        PUSH EBX
	$-1CB    00AD06F5     56                        PUSH ESI
	$-1CA    00AD06F6     57                        PUSH EDI
	$-1C9    00AD06F7     A1 D0687101               MOV EAX,DWORD PTR DS:[0x17168D0]
	$-1C4    00AD06FC     33C5                      XOR EAX,EBP
	$-1C2    00AD06FE     50                        PUSH EAX
	$-1C1    00AD06FF     8D45 F4                   LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-1BE    00AD0702     64:A3 00000000            MOV DWORD PTR FS:[0],EAX
	$-1B8    00AD0708     8D45 E8                   LEA EAX,DWORD PTR SS:[EBP-0x18]
	$-1B5    00AD070B     50                        PUSH EAX
	$-1B4    00AD070C     E8 DFF8FFFF               CALL MapleSto.00ACFFF0
	$-1AF    00AD0711     33DB                      XOR EBX,EBX
	$-1AD    00AD0713     50                        PUSH EAX
	$-1AC    00AD0714     895D FC                   MOV DWORD PTR SS:[EBP-0x4],EBX
	$-1A9    00AD0717     E8 64934100               CALL MapleSto.00EE9A80
	$-1A4    00AD071C     83C4 04                   ADD ESP,0x4
	$-1A1    00AD071F     895D F0                   MOV DWORD PTR SS:[EBP-0x10],EBX
	$-19E    00AD0722     3BC3                      CMP EAX,EBX
	$-19C    00AD0724     74 1B                     JE SHORT MapleSto.00AD0741
	$-19A    00AD0726     8B10                      MOV EDX,DWORD PTR DS:[EAX]
	$-198    00AD0728     8B52 0C                   MOV EDX,DWORD PTR DS:[EDX+0xC]
	$-195    00AD072B     8D4D F0                   LEA ECX,DWORD PTR SS:[EBP-0x10]
	$-192    00AD072E     51                        PUSH ECX
	$-191    00AD072F     68 2A002006               PUSH 0x620002A
	$-18C    00AD0734     8BC8                      MOV ECX,EAX
	$-18A    00AD0736     FFD2                      CALL EDX
	$-188    00AD0738     84C0                      TEST AL,AL
	$-186    00AD073A     74 05                     JE SHORT MapleSto.00AD0741
	$-184    00AD073C     8B7D F0                   MOV EDI,DWORD PTR SS:[EBP-0x10]
	$-181    00AD073F     EB 02                     JMP SHORT MapleSto.00AD0743
	$-17F    00AD0741     33FF                      XOR EDI,EDI
	$-17D    00AD0743     8D4D E8                   LEA ECX,DWORD PTR SS:[EBP-0x18]
	$-17A    00AD0746     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-173    00AD074D     FF15 08A23E01             CALL DWORD PTR DS:[0x13EA208]            ; mfc90.#601
	$-16D    00AD0753     3BFB                      CMP EDI,EBX
	$-16B    00AD0755     74 4C                     JE SHORT MapleSto.00AD07A3
	$-169    00AD0757     33F6                      XOR ESI,ESI
	$-167    00AD0759     8DA424 00000000           LEA ESP,DWORD PTR SS:[ESP]
	$-160    00AD0760     8B86 F0906E01             MOV EAX,DWORD PTR DS:[ESI+0x16E90F0]
	$-15A    00AD0766     50                        PUSH EAX
	$-159    00AD0767     8BCF                      MOV ECX,EDI
	$-157    00AD0769     E8 C4F94101               CALL MapleSto.01EF0132
	$-152    00AD076E     895D EC                   MOV DWORD PTR SS:[EBP-0x14],EBX
	$-14F    00AD0771     3BC3                      CMP EAX,EBX
	$-14D    00AD0773     74 26                     JE SHORT MapleSto.00AD079B
	$-14B    00AD0775     8B10                      MOV EDX,DWORD PTR DS:[EAX]
	$-149    00AD0777     8B52 0C                   MOV EDX,DWORD PTR DS:[EDX+0xC]
	$-146    00AD077A     8D4D EC                   LEA ECX,DWORD PTR SS:[EBP-0x14]
	$-143    00AD077D     51                        PUSH ECX
	$-142    00AD077E     68 25002006               PUSH 0x6200025
	$-13D    00AD0783     8BC8                      MOV ECX,EAX
	$-13B    00AD0785     FFD2                      CALL EDX
	$-139    00AD0787     84C0                      TEST AL,AL
	$-137    00AD0789     74 10                     JE SHORT MapleSto.00AD079B
	$-135    00AD078B     8B4D EC                   MOV ECX,DWORD PTR SS:[EBP-0x14]
	$-132    00AD078E     3BCB                      CMP ECX,EBX
	$-130    00AD0790     74 09                     JE SHORT MapleSto.00AD079B
	$-12E    00AD0792     E8 1987F0FF               CALL MapleSto.009D8EB0
	$-129    00AD0797     85C0                      TEST EAX,EAX
	$-127    00AD0799     74 1F                     JE SHORT MapleSto.00AD07BA
	$-125    00AD079B     83C6 04                   ADD ESI,0x4
	$-122    00AD079E     83FE 58                   CMP ESI,0x58
	$-11F    00AD07A1   ^ 72 BD                     JB SHORT MapleSto.00AD0760
	$-11D    00AD07A3     B8 01000000               MOV EAX,0x1
	$-118    00AD07A8     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-115    00AD07AB     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$-10E    00AD07B2     59                        POP ECX
	$-10D    00AD07B3     5F                        POP EDI
	$-10C    00AD07B4     5E                        POP ESI
	$-10B    00AD07B5     5B                        POP EBX
	$-10A    00AD07B6     8BE5                      MOV ESP,EBP
	$-108    00AD07B8     5D                        POP EBP
	$-107    00AD07B9     C3                        RETN
	$-106    00AD07BA     33C0                      XOR EAX,EAX
	$-104    00AD07BC     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-101    00AD07BF     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$-FA     00AD07C6     59                        POP ECX
	$-F9     00AD07C7     5F                        POP EDI
	$-F8     00AD07C8     5E                        POP ESI
	$-F7     00AD07C9     5B                        POP EBX
	$-F6     00AD07CA     8BE5                      MOV ESP,EBP
	$-F4     00AD07CC     5D                        POP EBP
	$-F3     00AD07CD     C3                        RETN
	$-F2     00AD07CE     CC                        INT3
	$-F1     00AD07CF     CC                        INT3
	$-F0     00AD07D0     55                        PUSH EBP
	$-EF     00AD07D1     8BEC                      MOV EBP,ESP
	$-ED     00AD07D3     6A FF                     PUSH -0x1
	$-EB     00AD07D5     68 47C23801               PUSH MapleSto.0138C247
	$-E6     00AD07DA     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$-E0     00AD07E0     50                        PUSH EAX
	$-DF     00AD07E1     83EC 0C                   SUB ESP,0xC
	$-DC     00AD07E4     53                        PUSH EBX
	$-DB     00AD07E5     56                        PUSH ESI
	$-DA     00AD07E6     57                        PUSH EDI
	$-D9     00AD07E7     A1 D0687101               MOV EAX,DWORD PTR DS:[0x17168D0]
	$-D4     00AD07EC     33C5                      XOR EAX,EBP
	$-D2     00AD07EE     50                        PUSH EAX
	$-D1     00AD07EF     8D45 F4                   LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-CE     00AD07F2     64:A3 00000000            MOV DWORD PTR FS:[0],EAX
	$-C8     00AD07F8     8D45 E8                   LEA EAX,DWORD PTR SS:[EBP-0x18]
	$-C5     00AD07FB     50                        PUSH EAX
	$-C4     00AD07FC     E8 EFF7FFFF               CALL MapleSto.00ACFFF0
	$-BF     00AD0801     33DB                      XOR EBX,EBX
	$-BD     00AD0803     50                        PUSH EAX
	$-BC     00AD0804     895D FC                   MOV DWORD PTR SS:[EBP-0x4],EBX
	$-B9     00AD0807     E8 74924100               CALL MapleSto.00EE9A80
	$-B4     00AD080C     83C4 04                   ADD ESP,0x4
	$-B1     00AD080F     895D F0                   MOV DWORD PTR SS:[EBP-0x10],EBX
	$-AE     00AD0812     3BC3                      CMP EAX,EBX
	$-AC     00AD0814     74 1B                     JE SHORT MapleSto.00AD0831
	$-AA     00AD0816     8B10                      MOV EDX,DWORD PTR DS:[EAX]
	$-A8     00AD0818     8B52 0C                   MOV EDX,DWORD PTR DS:[EDX+0xC]
	$-A5     00AD081B     8D4D F0                   LEA ECX,DWORD PTR SS:[EBP-0x10]
	$-A2     00AD081E     51                        PUSH ECX
	$-A1     00AD081F     68 2A002006               PUSH 0x620002A
	$-9C     00AD0824     8BC8                      MOV ECX,EAX
	$-9A     00AD0826     FFD2                      CALL EDX
	$-98     00AD0828     84C0                      TEST AL,AL
	$-96     00AD082A     74 05                     JE SHORT MapleSto.00AD0831
	$-94     00AD082C     8B7D F0                   MOV EDI,DWORD PTR SS:[EBP-0x10]
	$-91     00AD082F     EB 02                     JMP SHORT MapleSto.00AD0833
	$-8F     00AD0831     33FF                      XOR EDI,EDI
	$-8D     00AD0833     8D4D E8                   LEA ECX,DWORD PTR SS:[EBP-0x18]
	$-8A     00AD0836     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-83     00AD083D     FF15 08A23E01             CALL DWORD PTR DS:[0x13EA208]            ; mfc90.#601
	$-7D     00AD0843     3BFB                      CMP EDI,EBX
	$-7B     00AD0845     74 4A                     JE SHORT MapleSto.00AD0891
	$-79     00AD0847     33F6                      XOR ESI,ESI
	$-77     00AD0849     8DA424 00000000           LEA ESP,DWORD PTR SS:[ESP]
	$-70     00AD0850     8B86 48916E01             MOV EAX,DWORD PTR DS:[ESI+0x16E9148]
	$-6A     00AD0856     50                        PUSH EAX
	$-69     00AD0857     8BCF                      MOV ECX,EDI
	$-67     00AD0859     E8 D4F84101               CALL MapleSto.01EF0132
	$-62     00AD085E     895D EC                   MOV DWORD PTR SS:[EBP-0x14],EBX
	$-5F     00AD0861     3BC3                      CMP EAX,EBX
	$-5D     00AD0863     74 24                     JE SHORT MapleSto.00AD0889
	$-5B     00AD0865     8B10                      MOV EDX,DWORD PTR DS:[EAX]
	$-59     00AD0867     8B52 0C                   MOV EDX,DWORD PTR DS:[EDX+0xC]
	$-56     00AD086A     8D4D EC                   LEA ECX,DWORD PTR SS:[EBP-0x14]
	$-53     00AD086D     51                        PUSH ECX
	$-52     00AD086E     68 25002006               PUSH 0x6200025
	$-4D     00AD0873     8BC8                      MOV ECX,EAX
	$-4B     00AD0875     FFD2                      CALL EDX
	$-49     00AD0877     84C0                      TEST AL,AL
	$-47     00AD0879     74 0E                     JE SHORT MapleSto.00AD0889
	$-45     00AD087B     8B4D EC                   MOV ECX,DWORD PTR SS:[EBP-0x14]
	$-42     00AD087E     3BCB                      CMP ECX,EBX
	$-40     00AD0880     74 07                     JE SHORT MapleSto.00AD0889
	$-3E     00AD0882     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$-3C     00AD0884     8B50 28                   MOV EDX,DWORD PTR DS:[EAX+0x28]
	$-39     00AD0887     FFD2                      CALL EDX
	$-37     00AD0889     83C6 04                   ADD ESI,0x4
	$-34     00AD088C     83FE 58                   CMP ESI,0x58
	$-31     00AD088F   ^ 72 BF                     JB SHORT MapleSto.00AD0850
	$-2F     00AD0891     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-2C     00AD0894     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$-25     00AD089B     59                        POP ECX
	$-24     00AD089C     5F                        POP EDI
	$-23     00AD089D     5E                        POP ESI
	$-22     00AD089E     5B                        POP EBX
	$-21     00AD089F     8BE5                      MOV ESP,EBP
	$-1F     00AD08A1     5D                        POP EBP
	$-1E     00AD08A2     C3                        RETN
	$-1D     00AD08A3     CC                        INT3
	$-1C     00AD08A4     CC                        INT3
	$-1B     00AD08A5     CC                        INT3
	$-1A     00AD08A6     CC                        INT3
	$-19     00AD08A7     CC                        INT3
	$-18     00AD08A8     CC                        INT3
	$-17     00AD08A9     CC                        INT3
	$-16     00AD08AA     CC                        INT3
	$-15     00AD08AB     CC                        INT3
	$-14     00AD08AC     CC                        INT3
	$-13     00AD08AD     CC                        INT3
	$-12     00AD08AE     CC                        INT3
	$-11     00AD08AF     CC                        INT3
	$-10     00AD08B0     C705 A8897301 01000000    MOV DWORD PTR DS:[0x17389A8],0x1
	$-6      00AD08BA     C3                        RETN
	$-5      00AD08BB     CC                        INT3
	$-4      00AD08BC     CC                        INT3
	$-3      00AD08BD     CC                        INT3
	$-2      00AD08BE     CC                        INT3
	$-1      00AD08BF     CC                        INT3
	$ ==>    00AD08C0     56                        PUSH ESI
	$+1      00AD08C1     57                        PUSH EDI
	$+2      00AD08C2     8BF9                      MOV EDI,ECX
	$+4      00AD08C4     33F6                      XOR ESI,ESI
	$+6      00AD08C6     8B0D 2C407201             MOV ECX,DWORD PTR DS:[0x172402C]
	$+C      00AD08CC     6A 00                     PUSH 0x0
	$+E      00AD08CE     6A 00                     PUSH 0x0
	$+10     00AD08D0     8D86 21A10700             LEA EAX,DWORD PTR DS:[ESI+0x7A121]
	$+16     00AD08D6     50                        PUSH EAX
	$+17     00AD08D7     E8 742FCDFF               CALL MapleSto.007A3850
	$+1C     00AD08DC     84C0                      TEST AL,AL
	$+1E     00AD08DE     74 13                     JE SHORT MapleSto.00AD08F3
	$+20     00AD08E0     56                        PUSH ESI
	$+21     00AD08E1     8BCF                      MOV ECX,EDI
	$+23     00AD08E3     E8 28FDFFFF               CALL MapleSto.00AD0610                   ; g_c_get_skill_slot_by_idx_
	$+28     00AD08E8     85C0                      TEST EAX,EAX
	$+2A     00AD08EA     74 07                     JE SHORT MapleSto.00AD08F3
	$+2C     00AD08EC     8BC8                      MOV ECX,EAX
	$+2E     00AD08EE     E8 6CA43801               CALL MapleSto.01E5AD5F
	$+33     00AD08F3     46                        INC ESI
	$+34     00AD08F4     83FE 16                   CMP ESI,0x16
	$+37     00AD08F7   ^ 72 CD                     JB SHORT MapleSto.00AD08C6
	$+39     00AD08F9     5F                        POP EDI
	$+3A     00AD08FA     5E                        POP ESI
	$+3B     00AD08FB     C3                        RETN
	$+3C     00AD08FC     CC                        INT3
	$+3D     00AD08FD     CC                        INT3
	$+3E     00AD08FE     CC                        INT3
	$+3F     00AD08FF     CC                        INT3
	$+40     00AD0900     55                        PUSH EBP
	$+41     00AD0901     8BEC                      MOV EBP,ESP
	$+43     00AD0903     6A FF                     PUSH -0x1
	$+45     00AD0905     68 6BC23801               PUSH MapleSto.0138C26B
	$+4A     00AD090A     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$+50     00AD0910     50                        PUSH EAX
	$+51     00AD0911     83EC 08                   SUB ESP,0x8
	$+54     00AD0914     56                        PUSH ESI
	$+55     00AD0915     A1 D0687101               MOV EAX,DWORD PTR DS:[0x17168D0]
	$+5A     00AD091A     33C5                      XOR EAX,EBP
	$+5C     00AD091C     50                        PUSH EAX
	$+5D     00AD091D     8D45 F4                   LEA EAX,DWORD PTR SS:[EBP-0xC]
	$+60     00AD0920     64:A3 00000000            MOV DWORD PTR FS:[0],EAX
	$+66     00AD0926     8D45 EC                   LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+69     00AD0929     50                        PUSH EAX
	$+6A     00AD092A     E8 C1F6FFFF               CALL MapleSto.00ACFFF0
	$+6F     00AD092F     50                        PUSH EAX
	$+70     00AD0930     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$+77     00AD0937     E8 44914100               CALL MapleSto.00EE9A80
	$+7C     00AD093C     83C4 04                   ADD ESP,0x4
	$+7F     00AD093F     C745 F0 00000000          MOV DWORD PTR SS:[EBP-0x10],0x0
	$+86     00AD0946     85C0                      TEST EAX,EAX
	$+88     00AD0948     74 1B                     JE SHORT MapleSto.00AD0965
	$+8A     00AD094A     8B10                      MOV EDX,DWORD PTR DS:[EAX]
	$+8C     00AD094C     8B52 0C                   MOV EDX,DWORD PTR DS:[EDX+0xC]
	$+8F     00AD094F     8D4D F0                   LEA ECX,DWORD PTR SS:[EBP-0x10]
	$+92     00AD0952     51                        PUSH ECX
	$+93     00AD0953     68 2A002006               PUSH 0x620002A
	$+98     00AD0958     8BC8                      MOV ECX,EAX
	$+9A     00AD095A     FFD2                      CALL EDX
	$+9C     00AD095C     84C0                      TEST AL,AL
	$+9E     00AD095E     74 05                     JE SHORT MapleSto.00AD0965
	$+A0     00AD0960     8B75 F0                   MOV ESI,DWORD PTR SS:[EBP-0x10]
	$+A3     00AD0963     EB 02                     JMP SHORT MapleSto.00AD0967
	$+A5     00AD0965     33F6                      XOR ESI,ESI
	$+A7     00AD0967     8D4D EC                   LEA ECX,DWORD PTR SS:[EBP-0x14]
	$+AA     00AD096A     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+B1     00AD0971     FF15 08A23E01             CALL DWORD PTR DS:[0x13EA208]            ; mfc90.#601
	$+B7     00AD0977     85F6                      TEST ESI,ESI
	$+B9     00AD0979     74 07                     JE SHORT MapleSto.00AD0982
	$+BB     00AD097B     8BCE                      MOV ECX,ESI
	$+BD     00AD097D     E8 4EACF0FF               CALL MapleSto.009DB5D0
	$+C2     00AD0982     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+C5     00AD0985     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+CC     00AD098C     59                        POP ECX
	$+CD     00AD098D     5E                        POP ESI
	$+CE     00AD098E     8BE5                      MOV ESP,EBP
	$+D0     00AD0990     5D                        POP EBP
	$+D1     00AD0991     C3                        RETN
	$+D2     00AD0992     CC                        INT3
	$+D3     00AD0993     CC                        INT3
	$+D4     00AD0994     CC                        INT3
	$+D5     00AD0995     CC                        INT3
	$+D6     00AD0996     CC                        INT3
	$+D7     00AD0997     CC                        INT3
	$+D8     00AD0998     CC                        INT3
	$+D9     00AD0999     CC                        INT3
	$+DA     00AD099A     CC                        INT3
	$+DB     00AD099B     CC                        INT3
	$+DC     00AD099C     CC                        INT3
	$+DD     00AD099D     CC                        INT3
	$+DE     00AD099E     CC                        INT3
	$+DF     00AD099F     CC                        INT3
	$+E0     00AD09A0     55                        PUSH EBP
	$+E1     00AD09A1     8BEC                      MOV EBP,ESP
	$+E3     00AD09A3     6A FF                     PUSH -0x1
	$+E5     00AD09A5     68 8FC23801               PUSH MapleSto.0138C28F
	$+EA     00AD09AA     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$+F0     00AD09B0     50                        PUSH EAX
	$+F1     00AD09B1     83EC 08                   SUB ESP,0x8
	$+F4     00AD09B4     56                        PUSH ESI
	$+F5     00AD09B5     A1 D0687101               MOV EAX,DWORD PTR DS:[0x17168D0]
	$+FA     00AD09BA     33C5                      XOR EAX,EBP
	$+FC     00AD09BC     50                        PUSH EAX
	$+FD     00AD09BD     8D45 F4                   LEA EAX,DWORD PTR SS:[EBP-0xC]
	$+100    00AD09C0     64:A3 00000000            MOV DWORD PTR FS:[0],EAX
	$+106    00AD09C6     8D45 EC                   LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+109    00AD09C9     50                        PUSH EAX
	$+10A    00AD09CA     E8 21F6FFFF               CALL MapleSto.00ACFFF0
	$+10F    00AD09CF     50                        PUSH EAX
	$+110    00AD09D0     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$+117    00AD09D7     E8 A4904100               CALL MapleSto.00EE9A80
	$+11C    00AD09DC     83C4 04                   ADD ESP,0x4
	$+11F    00AD09DF     C745 F0 00000000          MOV DWORD PTR SS:[EBP-0x10],0x0
	$+126    00AD09E6     85C0                      TEST EAX,EAX
	$+128    00AD09E8     74 1B                     JE SHORT MapleSto.00AD0A05
	$+12A    00AD09EA     8B10                      MOV EDX,DWORD PTR DS:[EAX]
	$+12C    00AD09EC     8B52 0C                   MOV EDX,DWORD PTR DS:[EDX+0xC]
	$+12F    00AD09EF     8D4D F0                   LEA ECX,DWORD PTR SS:[EBP-0x10]
	$+132    00AD09F2     51                        PUSH ECX
	$+133    00AD09F3     68 2A002006               PUSH 0x620002A
	$+138    00AD09F8     8BC8                      MOV ECX,EAX
	$+13A    00AD09FA     FFD2                      CALL EDX
	$+13C    00AD09FC     84C0                      TEST AL,AL
	$+13E    00AD09FE     74 05                     JE SHORT MapleSto.00AD0A05
	$+140    00AD0A00     8B75 F0                   MOV ESI,DWORD PTR SS:[EBP-0x10]
	$+143    00AD0A03     EB 02                     JMP SHORT MapleSto.00AD0A07
	$+145    00AD0A05     33F6                      XOR ESI,ESI
	$+147    00AD0A07     8D4D EC                   LEA ECX,DWORD PTR SS:[EBP-0x14]
	$+14A    00AD0A0A     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+151    00AD0A11     FF15 08A23E01             CALL DWORD PTR DS:[0x13EA208]            ; mfc90.#601
	$+157    00AD0A17     85F6                      TEST ESI,ESI
	$+159    00AD0A19     74 07                     JE SHORT MapleSto.00AD0A22
	$+15B    00AD0A1B     8BCE                      MOV ECX,ESI
	$+15D    00AD0A1D     E8 DEACF0FF               CALL MapleSto.009DB700
	$+162    00AD0A22     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+165    00AD0A25     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+16C    00AD0A2C     59                        POP ECX
	$+16D    00AD0A2D     5E                        POP ESI
	$+16E    00AD0A2E     8BE5                      MOV ESP,EBP
	$+170    00AD0A30     5D                        POP EBP
	$+171    00AD0A31     C3                        RETN
	$+172    00AD0A32     CC                        INT3
	$+173    00AD0A33     CC                        INT3
	$+174    00AD0A34     CC                        INT3
	$+175    00AD0A35     CC                        INT3
	$+176    00AD0A36     CC                        INT3
	$+177    00AD0A37     CC                        INT3
	$+178    00AD0A38     CC                        INT3
	$+179    00AD0A39     CC                        INT3
	$+17A    00AD0A3A     CC                        INT3
	$+17B    00AD0A3B     CC                        INT3
	$+17C    00AD0A3C     CC                        INT3
	$+17D    00AD0A3D     CC                        INT3
	$+17E    00AD0A3E     CC                        INT3
	$+17F    00AD0A3F     CC                        INT3
	$+180    00AD0A40     55                        PUSH EBP
	$+181    00AD0A41     8BEC                      MOV EBP,ESP
	$+183    00AD0A43     6A FF                     PUSH -0x1
	$+185    00AD0A45     68 B5C23801               PUSH MapleSto.0138C2B5
	$+18A    00AD0A4A     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$+190    00AD0A50     50                        PUSH EAX
	$+191    00AD0A51     51                        PUSH ECX
	$+192    00AD0A52     56                        PUSH ESI
	$+193    00AD0A53     A1 D0687101               MOV EAX,DWORD PTR DS:[0x17168D0]
	$+198    00AD0A58     33C5                      XOR EAX,EBP
	$+19A    00AD0A5A     50                        PUSH EAX
	$+19B    00AD0A5B     8D45 F4                   LEA EAX,DWORD PTR SS:[EBP-0xC]
	$+19E    00AD0A5E     64:A3 00000000            MOV DWORD PTR FS:[0],EAX
	$+1A4    00AD0A64     8B0D 2C417201             MOV ECX,DWORD PTR DS:[0x172412C]
	$+1AA    00AD0A6A     E8 642F3D01               CALL MapleSto.01EA39D3
	$+1AF    00AD0A6F     8BF0                      MOV ESI,EAX
	$+1B1    00AD0A71     85F6                      TEST ESI,ESI
	$+1B3    00AD0A73     74 34                     JE SHORT MapleSto.00AD0AA9
	$+1B5    00AD0A75     6A 1C                     PUSH 0x1C
	$+1B7    00AD0A77     E8 F2047300               CALL MapleSto.01200F6E                   ; JMP to mfc90.#798
	$+1BC    00AD0A7C     83C4 04                   ADD ESP,0x4
	$+1BF    00AD0A7F     8945 F0                   MOV DWORD PTR SS:[EBP-0x10],EAX
	$+1C2    00AD0A82     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$+1C9    00AD0A89     85C0                      TEST EAX,EAX
	$+1CB    00AD0A8B     74 0B                     JE SHORT MapleSto.00AD0A98
	$+1CD    00AD0A8D     6A 00                     PUSH 0x0
	$+1CF    00AD0A8F     8BC8                      MOV ECX,EAX
	$+1D1    00AD0A91     E8 8AD4A6FF               CALL MapleSto.0053DF20
	$+1D6    00AD0A96     EB 02                     JMP SHORT MapleSto.00AD0A9A
	$+1D8    00AD0A98     33C0                      XOR EAX,EAX
	$+1DA    00AD0A9A     50                        PUSH EAX
	$+1DB    00AD0A9B     8BCE                      MOV ECX,ESI
	$+1DD    00AD0A9D     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+1E4    00AD0AA4     E8 0724E6FF               CALL MapleSto.00932EB0
	$+1E9    00AD0AA9     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+1EC    00AD0AAC     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+1F3    00AD0AB3     59                        POP ECX
	$+1F4    00AD0AB4     5E                        POP ESI
	$+1F5    00AD0AB5     8BE5                      MOV ESP,EBP
	$+1F7    00AD0AB7     5D                        POP EBP
	$+1F8    00AD0AB8     C3                        RETN
	$+1F9    00AD0AB9     CC                        INT3
	*/
	addr_tmp = scan_exe_.ScanCall("56578BF933F68B0D????????6A006A008D86????????50", 0x23 + 1);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_c_get_skill_slot_by_idx_ = " << addr_tmp << ";" << FC_OFstream_EndlLog;
	return true;
}

bool CAutoUpdateApp::Scan_Vtbl(Cmn_FC_OFstream& ofs)
{
	pt_dword dw_tmp;
	boost::optional<pt_dword> op_dw_tmp;
	/*
	$-6      00E2B7FF     CC                  INT3
	$-5      00E2B800     53                  PUSH EBX
	$-4      00E2B801     55                  PUSH EBP
	$-3      00E2B802     8BE9                MOV EBP,ECX
	$-1      00E2B804     56                  PUSH ESI
	$ ==>    00E2B805     C745 00 A0544F01    MOV DWORD PTR SS:[EBP],MapleSto.014F54A0        ; g_va_model_
	$+7      00E2B80C     E8 7FFEFFFF         CALL MapleSto.00E2B690
	$+C      00E2B811     8D75 20             LEA ESI,DWORD PTR SS:[EBP+0x20]
	$+F      00E2B814     33DB                XOR EBX,EBX
	$+11     00E2B816     395E 10             CMP DWORD PTR DS:[ESI+0x10],EBX
	$+14     00E2B819     74 17               JE SHORT MapleSto.00E2B832
	$+16     00E2B81B     8B46 04             MOV EAX,DWORD PTR DS:[ESI+0x4]
	$+19     00E2B81E     50                  PUSH EAX
	$+1A     00E2B81F     8BCE                MOV ECX,ESI
	$+1C     00E2B821     E8 8AC4F9FF         CALL MapleSto.00DC7CB0
	$+21     00E2B826     8976 08             MOV DWORD PTR DS:[ESI+0x8],ESI
	$+24     00E2B829     895E 04             MOV DWORD PTR DS:[ESI+0x4],EBX
	$+27     00E2B82C     8976 0C             MOV DWORD PTR DS:[ESI+0xC],ESI
	$+2A     00E2B82F     895E 10             MOV DWORD PTR DS:[ESI+0x10],EBX
	$+2D     00E2B832     8B75 10             MOV ESI,DWORD PTR SS:[EBP+0x10]
	$+30     00E2B835     8D45 08             LEA EAX,DWORD PTR SS:[EBP+0x8]
	$+33     00E2B838     3BF0                CMP ESI,EAX
	$+35     00E2B83A     0F95C0              SETNE AL
	$+38     00E2B83D     57                  PUSH EDI
	$+39     00E2B83E     3AC3                CMP AL,BL
	$+3B     00E2B840     0F84 8E000000       JE MapleSto.00E2B8D4
	$+41     00E2B846     8B4D 58             MOV ECX,DWORD PTR SS:[EBP+0x58]
	$+44     00E2B849     8B7E 14             MOV EDI,DWORD PTR DS:[ESI+0x14]
	$+47     00E2B84C     8B56 10             MOV EDX,DWORD PTR DS:[ESI+0x10]
	$+4A     00E2B84F     33C0                XOR EAX,EAX
	$+4C     00E2B851     3BCB                CMP ECX,EBX
	$+4E     00E2B853     74 06               JE SHORT MapleSto.00E2B85B
	$+50     00E2B855     52                  PUSH EDX
	$+51     00E2B856     E8 A50C0000         CALL MapleSto.00E2C500
	$+56     00E2B85B     F685 A4000000 03    TEST BYTE PTR SS:[EBP+0xA4],0x3
	$+5D     00E2B862     74 0B               JE SHORT MapleSto.00E2B86F
	$+5F     00E2B864     8B48 5C             MOV ECX,DWORD PTR DS:[EAX+0x5C]
	$+62     00E2B867     80E1 08             AND CL,0x8
	$+65     00E2B86A     80F9 08             CMP CL,0x8
	$+68     00E2B86D     EB 09               JMP SHORT MapleSto.00E2B878
	$+6A     00E2B86F     8B50 5C             MOV EDX,DWORD PTR DS:[EAX+0x5C]
	$+6D     00E2B872     80E2 10             AND DL,0x10
	$+70     00E2B875     80FA 10             CMP DL,0x10
	$+73     00E2B878     0F94C0              SETE AL
	$+76     00E2B87B     3AC3                CMP AL,BL
	$+78     00E2B87D     75 0E               JNZ SHORT MapleSto.00E2B88D
	$+7A     00E2B87F     3BFB                CMP EDI,EBX
	$+7C     00E2B881     74 0A               JE SHORT MapleSto.00E2B88D
	$+7E     00E2B883     8B07                MOV EAX,DWORD PTR DS:[EDI]
	$+80     00E2B885     8B10                MOV EDX,DWORD PTR DS:[EAX]
	$+82     00E2B887     6A 01               PUSH 0x1
	$+84     00E2B889     8BCF                MOV ECX,EDI
	$+86     00E2B88B     FFD2                CALL EDX
	$+88     00E2B88D     8B46 0C             MOV EAX,DWORD PTR DS:[ESI+0xC]
	$+8B     00E2B890     3BC3                CMP EAX,EBX
	$+8D     00E2B892     74 17               JE SHORT MapleSto.00E2B8AB
	$+8F     00E2B894     8BF0                MOV ESI,EAX
	$+91     00E2B896     8B46 08             MOV EAX,DWORD PTR DS:[ESI+0x8]
	$+94     00E2B899     3BC3                CMP EAX,EBX
	$+96     00E2B89B     74 27               JE SHORT MapleSto.00E2B8C4
	$+98     00E2B89D     8D49 00             LEA ECX,DWORD PTR DS:[ECX]
	$+9B     00E2B8A0     8BF0                MOV ESI,EAX
	$+9D     00E2B8A2     8B46 08             MOV EAX,DWORD PTR DS:[ESI+0x8]
	$+A0     00E2B8A5     3BC3                CMP EAX,EBX
	$+A2     00E2B8A7   ^ 75 F7               JNZ SHORT MapleSto.00E2B8A0
	$+A4     00E2B8A9     EB 19               JMP SHORT MapleSto.00E2B8C4
	$+A6     00E2B8AB     8B46 04             MOV EAX,DWORD PTR DS:[ESI+0x4]
	$+A9     00E2B8AE     3B70 0C             CMP ESI,DWORD PTR DS:[EAX+0xC]
	$+AC     00E2B8B1     75 0A               JNZ SHORT MapleSto.00E2B8BD
	$+AE     00E2B8B3     8BF0                MOV ESI,EAX
	$+B0     00E2B8B5     8B40 04             MOV EAX,DWORD PTR DS:[EAX+0x4]
	$+B3     00E2B8B8     3B70 0C             CMP ESI,DWORD PTR DS:[EAX+0xC]
	$+B6     00E2B8BB   ^ 74 F6               JE SHORT MapleSto.00E2B8B3
	$+B8     00E2B8BD     3946 0C             CMP DWORD PTR DS:[ESI+0xC],EAX
	$+BB     00E2B8C0     74 02               JE SHORT MapleSto.00E2B8C4
	$+BD     00E2B8C2     8BF0                MOV ESI,EAX
	$+BF     00E2B8C4     8D45 08             LEA EAX,DWORD PTR SS:[EBP+0x8]
	$+C2     00E2B8C7     3BF0                CMP ESI,EAX
	$+C4     00E2B8C9     0F95C0              SETNE AL
	$+C7     00E2B8CC     3AC3                CMP AL,BL
	$+C9     00E2B8CE   ^ 0F85 72FFFFFF       JNZ MapleSto.00E2B846
	$+CF     00E2B8D4     399D 04010000       CMP DWORD PTR SS:[EBP+0x104],EBX
	$+D5     00E2B8DA     8DB5 F4000000       LEA ESI,DWORD PTR SS:[EBP+0xF4]
	$+DB     00E2B8E0     74 17               JE SHORT MapleSto.00E2B8F9
	$+DD     00E2B8E2     8B46 04             MOV EAX,DWORD PTR DS:[ESI+0x4]
	$+E0     00E2B8E5     50                  PUSH EAX
	$+E1     00E2B8E6     8BCE                MOV ECX,ESI
	$+E3     00E2B8E8     E8 6364BAFF         CALL MapleSto.009D1D50
	$+E8     00E2B8ED     8976 08             MOV DWORD PTR DS:[ESI+0x8],ESI
	$+EB     00E2B8F0     895E 04             MOV DWORD PTR DS:[ESI+0x4],EBX
	$+EE     00E2B8F3     8976 0C             MOV DWORD PTR DS:[ESI+0xC],ESI
	$+F1     00E2B8F6     895E 10             MOV DWORD PTR DS:[ESI+0x10],EBX
	$+F4     00E2B8F9     399D EC000000       CMP DWORD PTR SS:[EBP+0xEC],EBX
	$+FA     00E2B8FF     8DB5 DC000000       LEA ESI,DWORD PTR SS:[EBP+0xDC]
	$+100    00E2B905     74 17               JE SHORT MapleSto.00E2B91E
	$+102    00E2B907     8B4E 04             MOV ECX,DWORD PTR DS:[ESI+0x4]
	$+105    00E2B90A     51                  PUSH ECX
	$+106    00E2B90B     8BCE                MOV ECX,ESI
	$+108    00E2B90D     E8 9E44BAFF         CALL MapleSto.009CFDB0
	$+10D    00E2B912     8976 08             MOV DWORD PTR DS:[ESI+0x8],ESI
	$+110    00E2B915     895E 04             MOV DWORD PTR DS:[ESI+0x4],EBX
	$+113    00E2B918     8976 0C             MOV DWORD PTR DS:[ESI+0xC],ESI
	$+116    00E2B91B     895E 10             MOV DWORD PTR DS:[ESI+0x10],EBX
	$+119    00E2B91E     399D D0000000       CMP DWORD PTR SS:[EBP+0xD0],EBX
	$+11F    00E2B924     8DB5 C0000000       LEA ESI,DWORD PTR SS:[EBP+0xC0]
	$+125    00E2B92A     74 17               JE SHORT MapleSto.00E2B943
	$+127    00E2B92C     8B56 04             MOV EDX,DWORD PTR DS:[ESI+0x4]
	$+12A    00E2B92F     52                  PUSH EDX
	$+12B    00E2B930     8BCE                MOV ECX,ESI
	$+12D    00E2B932     E8 29D5FFFF         CALL MapleSto.00E28E60
	$+132    00E2B937     8976 08             MOV DWORD PTR DS:[ESI+0x8],ESI
	$+135    00E2B93A     895E 04             MOV DWORD PTR DS:[ESI+0x4],EBX
	$+138    00E2B93D     8976 0C             MOV DWORD PTR DS:[ESI+0xC],ESI
	$+13B    00E2B940     895E 10             MOV DWORD PTR DS:[ESI+0x10],EBX
	$+13E    00E2B943     399D 94000000       CMP DWORD PTR SS:[EBP+0x94],EBX
	$+144    00E2B949     8DB5 84000000       LEA ESI,DWORD PTR SS:[EBP+0x84]
	$+14A    00E2B94F     74 17               JE SHORT MapleSto.00E2B968
	$+14C    00E2B951     8B46 04             MOV EAX,DWORD PTR DS:[ESI+0x4]
	$+14F    00E2B954     50                  PUSH EAX
	$+150    00E2B955     8BCE                MOV ECX,ESI
	$+152    00E2B957     E8 2489FBFF         CALL MapleSto.00DE4280
	$+157    00E2B95C     8976 08             MOV DWORD PTR DS:[ESI+0x8],ESI
	$+15A    00E2B95F     895E 04             MOV DWORD PTR DS:[ESI+0x4],EBX
	$+15D    00E2B962     8976 0C             MOV DWORD PTR DS:[ESI+0xC],ESI
	$+160    00E2B965     895E 10             MOV DWORD PTR DS:[ESI+0x10],EBX
	$+163    00E2B968     8D7D 64             LEA EDI,DWORD PTR SS:[EBP+0x64]
	$+166    00E2B96B     395F 18             CMP DWORD PTR DS:[EDI+0x18],EBX
	$+169    00E2B96E     8D77 08             LEA ESI,DWORD PTR DS:[EDI+0x8]
	$+16C    00E2B971     74 17               JE SHORT MapleSto.00E2B98A
	$+16E    00E2B973     8B4E 04             MOV ECX,DWORD PTR DS:[ESI+0x4]
	$+171    00E2B976     51                  PUSH ECX
	$+172    00E2B977     8BCE                MOV ECX,ESI
	$+174    00E2B979     E8 9289FBFF         CALL MapleSto.00DE4310
	$+179    00E2B97E     8976 08             MOV DWORD PTR DS:[ESI+0x8],ESI
	$+17C    00E2B981     895E 04             MOV DWORD PTR DS:[ESI+0x4],EBX
	$+17F    00E2B984     8976 0C             MOV DWORD PTR DS:[ESI+0xC],ESI
	$+182    00E2B987     895E 10             MOV DWORD PTR DS:[ESI+0x10],EBX
	$+185    00E2B98A     8B37                MOV ESI,DWORD PTR DS:[EDI]
	$+187    00E2B98C     3BF7                CMP ESI,EDI
	$+189    00E2B98E     74 20               JE SHORT MapleSto.00E2B9B0
	$+18B    00E2B990     8BC6                MOV EAX,ESI
	$+18D    00E2B992     8B36                MOV ESI,DWORD PTR DS:[ESI]
	$+18F    00E2B994     6A 0C               PUSH 0xC
	$+191    00E2B996     50                  PUSH EAX
	$+192    00E2B997     C605 40267401 01    MOV BYTE PTR DS:[0x1742640],0x1
	$+199    00E2B99E     E8 CDB6FAFF         CALL MapleSto.00DD7070
	$+19E    00E2B9A3     83C4 08             ADD ESP,0x8
	$+1A1    00E2B9A6     881D 40267401       MOV BYTE PTR DS:[0x1742640],BL
	$+1A7    00E2B9AC     3BF7                CMP ESI,EDI
	$+1A9    00E2B9AE   ^ 75 E0               JNZ SHORT MapleSto.00E2B990
	$+1AB    00E2B9B0     893F                MOV DWORD PTR DS:[EDI],EDI
	$+1AD    00E2B9B2     897F 04             MOV DWORD PTR DS:[EDI+0x4],EDI
	$+1B0    00E2B9B5     8B4D 58             MOV ECX,DWORD PTR SS:[EBP+0x58]
	$+1B3    00E2B9B8     5F                  POP EDI
	$+1B4    00E2B9B9     3BCB                CMP ECX,EBX
	$+1B6    00E2B9BB     74 0C               JE SHORT MapleSto.00E2B9C9
	$+1B8    00E2B9BD     8341 04 FF          ADD DWORD PTR DS:[ECX+0x4],-0x1
	$+1BC    00E2B9C1     75 06               JNZ SHORT MapleSto.00E2B9C9
	$+1BE    00E2B9C3     8B11                MOV EDX,DWORD PTR DS:[ECX]
	$+1C0    00E2B9C5     8B02                MOV EAX,DWORD PTR DS:[EDX]
	$+1C2    00E2B9C7     FFD0                CALL EAX
	$+1C4    00E2B9C9     395D 48             CMP DWORD PTR SS:[EBP+0x48],EBX
	$+1C7    00E2B9CC     8D75 38             LEA ESI,DWORD PTR SS:[EBP+0x38]
	$+1CA    00E2B9CF     74 17               JE SHORT MapleSto.00E2B9E8
	$+1CC    00E2B9D1     8B4E 04             MOV ECX,DWORD PTR DS:[ESI+0x4]
	$+1CF    00E2B9D4     51                  PUSH ECX
	$+1D0    00E2B9D5     8BCE                MOV ECX,ESI
	$+1D2    00E2B9D7     E8 A4C7FFFF         CALL MapleSto.00E28180
	$+1D7    00E2B9DC     8976 08             MOV DWORD PTR DS:[ESI+0x8],ESI
	$+1DA    00E2B9DF     895E 04             MOV DWORD PTR DS:[ESI+0x4],EBX
	$+1DD    00E2B9E2     8976 0C             MOV DWORD PTR DS:[ESI+0xC],ESI
	$+1E0    00E2B9E5     895E 10             MOV DWORD PTR DS:[ESI+0x10],EBX
	$+1E3    00E2B9E8     395D 30             CMP DWORD PTR SS:[EBP+0x30],EBX
	$+1E6    00E2B9EB     8D75 20             LEA ESI,DWORD PTR SS:[EBP+0x20]
	$+1E9    00E2B9EE     74 17               JE SHORT MapleSto.00E2BA07
	$+1EB    00E2B9F0     8B56 04             MOV EDX,DWORD PTR DS:[ESI+0x4]
	$+1EE    00E2B9F3     52                  PUSH EDX
	$+1EF    00E2B9F4     8BCE                MOV ECX,ESI
	$+1F1    00E2B9F6     E8 B5C2F9FF         CALL MapleSto.00DC7CB0
	$+1F6    00E2B9FB     8976 08             MOV DWORD PTR DS:[ESI+0x8],ESI
	$+1F9    00E2B9FE     895E 04             MOV DWORD PTR DS:[ESI+0x4],EBX
	$+1FC    00E2BA01     8976 0C             MOV DWORD PTR DS:[ESI+0xC],ESI
	$+1FF    00E2BA04     895E 10             MOV DWORD PTR DS:[ESI+0x10],EBX
	$+202    00E2BA07     395D 18             CMP DWORD PTR SS:[EBP+0x18],EBX
	$+205    00E2BA0A     8D75 08             LEA ESI,DWORD PTR SS:[EBP+0x8]
	$+208    00E2BA0D     74 17               JE SHORT MapleSto.00E2BA26
	$+20A    00E2BA0F     8B46 04             MOV EAX,DWORD PTR DS:[ESI+0x4]
	$+20D    00E2BA12     50                  PUSH EAX
	$+20E    00E2BA13     8BCE                MOV ECX,ESI
	$+210    00E2BA15     E8 6688FBFF         CALL MapleSto.00DE4280
	$+215    00E2BA1A     8976 08             MOV DWORD PTR DS:[ESI+0x8],ESI
	$+218    00E2BA1D     895E 04             MOV DWORD PTR DS:[ESI+0x4],EBX
	$+21B    00E2BA20     8976 0C             MOV DWORD PTR DS:[ESI+0xC],ESI
	$+21E    00E2BA23     895E 10             MOV DWORD PTR DS:[ESI+0x10],EBX
	$+221    00E2BA26     5E                  POP ESI
	$+222    00E2BA27     C745 00 DC324201    MOV DWORD PTR SS:[EBP],MapleSto.014232DC
	$+229    00E2BA2E     5D                  POP EBP
	$+22A    00E2BA2F     5B                  POP EBX
	$+22B    00E2BA30     C3                  RETN
	$+22C    00E2BA31     CC                  INT3
	$+22D    00E2BA32     CC                  INT3
	$+22E    00E2BA33     CC                  INT3
	$+22F    00E2BA34     CC                  INT3
	$+230    00E2BA35     CC                  INT3
	$+231    00E2BA36     CC                  INT3
	$+232    00E2BA37     CC                  INT3
	$+233    00E2BA38     CC                  INT3
	$+234    00E2BA39     CC                  INT3
	$+235    00E2BA3A     CC                  INT3
	$+236    00E2BA3B     CC                  INT3
	$+237    00E2BA3C     CC                  INT3
	$+238    00E2BA3D     CC                  INT3
	$+239    00E2BA3E     CC                  INT3
	$+23A    00E2BA3F     CC                  INT3
	$+23B    00E2BA40     B8 B8797401         MOV EAX,MapleSto.017479B8
	$+240    00E2BA45     C3                  RETN
	$+241    00E2BA46     CC                  INT3
	$+242    00E2BA47     CC                  INT3
	$+243    00E2BA48     CC                  INT3
	$+244    00E2BA49     CC                  INT3
	$+245    00E2BA4A     CC                  INT3
	$+246    00E2BA4B     CC                  INT3
	$+247    00E2BA4C     CC                  INT3
	$+248    00E2BA4D     CC                  INT3
	$+249    00E2BA4E     CC                  INT3
	$+24A    00E2BA4F     CC                  INT3
	$+24B    00E2BA50     56                  PUSH ESI
	$+24C    00E2BA51     8B35 B8797401       MOV ESI,DWORD PTR DS:[0x17479B8]
	$+252    00E2BA57     33C0                XOR EAX,EAX
	$+254    00E2BA59     57                  PUSH EDI
	$+255    00E2BA5A     85F6                TEST ESI,ESI
	$+257    00E2BA5C     76 15               JBE SHORT MapleSto.00E2BA73
	$+259    00E2BA5E     8B7C24 0C           MOV EDI,DWORD PTR SS:[ESP+0xC]
	$+25D    00E2BA62     BA BC797401         MOV EDX,MapleSto.017479BC
	$+262    00E2BA67     393A                CMP DWORD PTR DS:[EDX],EDI
	$+264    00E2BA69     74 1A               JE SHORT MapleSto.00E2BA85
	$+266    00E2BA6B     40                  INC EAX
	$+267    00E2BA6C     83C2 10             ADD EDX,0x10
	$+26A    00E2BA6F     3BC6                CMP EAX,ESI
	$+26C    00E2BA71   ^ 72 F4               JB SHORT MapleSto.00E2BA67
	$+26E    00E2BA73     8B4C24 10           MOV ECX,DWORD PTR SS:[ESP+0x10]
	$+272    00E2BA77     33C0                XOR EAX,EAX
	$+274    00E2BA79     85C0                TEST EAX,EAX
	$+276    00E2BA7B     5F                  POP EDI
	$+277    00E2BA7C     8901                MOV DWORD PTR DS:[ECX],EAX
	$+279    00E2BA7E     0F95C0              SETNE AL
	$+27C    00E2BA81     5E                  POP ESI
	$+27D    00E2BA82     C2 0800             RETN 0x8
	$+280    00E2BA85     C1E0 04             SHL EAX,0x4
	$+283    00E2BA88     8B80 C8797401       MOV EAX,DWORD PTR DS:[EAX+0x17479C8]
	$+289    00E2BA8E     03C1                ADD EAX,ECX
	$+28B    00E2BA90     8B4C24 10           MOV ECX,DWORD PTR SS:[ESP+0x10]
	$+28F    00E2BA94     85C0                TEST EAX,EAX
	$+291    00E2BA96     5F                  POP EDI
	$+292    00E2BA97     8901                MOV DWORD PTR DS:[ECX],EAX
	$+294    00E2BA99     0F95C0              SETNE AL
	$+297    00E2BA9C     5E                  POP ESI
	$+298    00E2BA9D     C2 0800             RETN 0x8
	$+29B    00E2BAA0     0FB65424 10         MOVZX EDX,BYTE PTR SS:[ESP+0x10]
	$+2A0    00E2BAA5     83EC 08             SUB ESP,0x8
	$+2A3    00E2BAA8     56                  PUSH ESI
	$+2A4    00E2BAA9     8BF1                MOV ESI,ECX
	$+2A6    00E2BAAB     C706 A0544F01       MOV DWORD PTR DS:[ESI],MapleSto.014F54A0        ; g_va_model_
	$+2AC    00E2BAB1     33C9                XOR ECX,ECX
	$+2AE    00E2BAB3     894E 04             MOV DWORD PTR DS:[ESI+0x4],ECX
	$+2B1    00E2BAB6     894E 08             MOV DWORD PTR DS:[ESI+0x8],ECX
	$+2B4    00E2BAB9     8856 1C             MOV BYTE PTR DS:[ESI+0x1C],DL
	$+2B7    00E2BABC     0FB65424 1C         MOVZX EDX,BYTE PTR SS:[ESP+0x1C]
	$+2BC    00E2BAC1     884E 08             MOV BYTE PTR DS:[ESI+0x8],CL
	$+2BF    00E2BAC4     894E 0C             MOV DWORD PTR DS:[ESI+0xC],ECX
	$+2C2    00E2BAC7     894E 18             MOV DWORD PTR DS:[ESI+0x18],ECX
	$+2C5    00E2BACA     8D46 08             LEA EAX,DWORD PTR DS:[ESI+0x8]
	$+2C8    00E2BACD     8940 08             MOV DWORD PTR DS:[EAX+0x8],EAX
	$+2CB    00E2BAD0     8940 0C             MOV DWORD PTR DS:[EAX+0xC],EAX
	$+2CE    00E2BAD3     894E 20             MOV DWORD PTR DS:[ESI+0x20],ECX
	$+2D1    00E2BAD6     8856 34             MOV BYTE PTR DS:[ESI+0x34],DL
	$+2D4    00E2BAD9     0FB65424 1C         MOVZX EDX,BYTE PTR SS:[ESP+0x1C]
	$+2D9    00E2BADE     884E 20             MOV BYTE PTR DS:[ESI+0x20],CL
	$+2DC    00E2BAE1     894E 24             MOV DWORD PTR DS:[ESI+0x24],ECX
	$+2DF    00E2BAE4     894E 30             MOV DWORD PTR DS:[ESI+0x30],ECX
	$+2E2    00E2BAE7     8D46 20             LEA EAX,DWORD PTR DS:[ESI+0x20]
	$+2E5    00E2BAEA     8940 08             MOV DWORD PTR DS:[EAX+0x8],EAX
	$+2E8    00E2BAED     8940 0C             MOV DWORD PTR DS:[EAX+0xC],EAX
	$+2EB    00E2BAF0     8D46 38             LEA EAX,DWORD PTR DS:[ESI+0x38]
	$+2EE    00E2BAF3     8908                MOV DWORD PTR DS:[EAX],ECX
	$+2F0    00E2BAF5     8850 14             MOV BYTE PTR DS:[EAX+0x14],DL
	$+2F3    00E2BAF8     8B5424 18           MOV EDX,DWORD PTR SS:[ESP+0x18]
	$+2F7    00E2BAFC     8808                MOV BYTE PTR DS:[EAX],CL
	$+2F9    00E2BAFE     8948 04             MOV DWORD PTR DS:[EAX+0x4],ECX
	$+2FC    00E2BB01     8940 08             MOV DWORD PTR DS:[EAX+0x8],EAX
	$+2FF    00E2BB04     8940 0C             MOV DWORD PTR DS:[EAX+0xC],EAX
	$+302    00E2BB07     8948 10             MOV DWORD PTR DS:[EAX+0x10],ECX
	$+305    00E2BB0A     8B4424 14           MOV EAX,DWORD PTR SS:[ESP+0x14]
	$+309    00E2BB0E     8946 50             MOV DWORD PTR DS:[ESI+0x50],EAX
	$+30C    00E2BB11     8B4424 10           MOV EAX,DWORD PTR SS:[ESP+0x10]
	$+310    00E2BB15     8956 54             MOV DWORD PTR DS:[ESI+0x54],EDX
	$+313    00E2BB18     8946 58             MOV DWORD PTR DS:[ESI+0x58],EAX
	$+316    00E2BB1B     3BC1                CMP EAX,ECX
	$+318    00E2BB1D     74 03               JE SHORT MapleSto.00E2BB22
	$+31A    00E2BB1F     FF40 04             INC DWORD PTR DS:[EAX+0x4]
	$+31D    00E2BB22     894E 5C             MOV DWORD PTR DS:[ESI+0x5C],ECX
	$+320    00E2BB25     894E 60             MOV DWORD PTR DS:[ESI+0x60],ECX
	$+323    00E2BB28     0FB65424 1C         MOVZX EDX,BYTE PTR SS:[ESP+0x1C]
	$+328    00E2BB2D     8D46 64             LEA EAX,DWORD PTR DS:[ESI+0x64]
	$+32B    00E2BB30     8900                MOV DWORD PTR DS:[EAX],EAX
	$+32D    00E2BB32     8940 04             MOV DWORD PTR DS:[EAX+0x4],EAX
	$+330    00E2BB35     8948 08             MOV DWORD PTR DS:[EAX+0x8],ECX
	$+333    00E2BB38     8850 1C             MOV BYTE PTR DS:[EAX+0x1C],DL
	$+336    00E2BB3B     8848 08             MOV BYTE PTR DS:[EAX+0x8],CL
	$+339    00E2BB3E     8948 0C             MOV DWORD PTR DS:[EAX+0xC],ECX
	$+33C    00E2BB41     8948 18             MOV DWORD PTR DS:[EAX+0x18],ECX
	$+33F    00E2BB44     0FB65424 1C         MOVZX EDX,BYTE PTR SS:[ESP+0x1C]
	$+344    00E2BB49     83C0 08             ADD EAX,0x8
	$+347    00E2BB4C     8940 08             MOV DWORD PTR DS:[EAX+0x8],EAX
	$+34A    00E2BB4F     8940 0C             MOV DWORD PTR DS:[EAX+0xC],EAX
	$+34D    00E2BB52     898E 84000000       MOV DWORD PTR DS:[ESI+0x84],ECX
	$+353    00E2BB58     8896 98000000       MOV BYTE PTR DS:[ESI+0x98],DL
	$+359    00E2BB5E     888E 84000000       MOV BYTE PTR DS:[ESI+0x84],CL
	$+35F    00E2BB64     898E 88000000       MOV DWORD PTR DS:[ESI+0x88],ECX
	$+365    00E2BB6A     898E 94000000       MOV DWORD PTR DS:[ESI+0x94],ECX
	$+36B    00E2BB70     0FB65424 1C         MOVZX EDX,BYTE PTR SS:[ESP+0x1C]
	$+370    00E2BB75     8D86 84000000       LEA EAX,DWORD PTR DS:[ESI+0x84]
	$+376    00E2BB7B     8940 08             MOV DWORD PTR DS:[EAX+0x8],EAX
	$+379    00E2BB7E     8940 0C             MOV DWORD PTR DS:[EAX+0xC],EAX
	$+37C    00E2BB81     898E 9C000000       MOV DWORD PTR DS:[ESI+0x9C],ECX
	$+382    00E2BB87     898E A0000000       MOV DWORD PTR DS:[ESI+0xA0],ECX
	$+388    00E2BB8D     898E A4000000       MOV DWORD PTR DS:[ESI+0xA4],ECX
	$+38E    00E2BB93     33C0                XOR EAX,EAX
	$+390    00E2BB95     8986 A8000000       MOV DWORD PTR DS:[ESI+0xA8],EAX
	$+396    00E2BB9B     8986 AC000000       MOV DWORD PTR DS:[ESI+0xAC],EAX
	$+39C    00E2BBA1     8986 B0000000       MOV DWORD PTR DS:[ESI+0xB0],EAX
	$+3A2    00E2BBA7     8986 B4000000       MOV DWORD PTR DS:[ESI+0xB4],EAX
	$+3A8    00E2BBAD     898E BC000000       MOV DWORD PTR DS:[ESI+0xBC],ECX
	$+3AE    00E2BBB3     898E C0000000       MOV DWORD PTR DS:[ESI+0xC0],ECX
	$+3B4    00E2BBB9     8896 D4000000       MOV BYTE PTR DS:[ESI+0xD4],DL
	$+3BA    00E2BBBF     0FB65424 1C         MOVZX EDX,BYTE PTR SS:[ESP+0x1C]
	$+3BF    00E2BBC4     888E C0000000       MOV BYTE PTR DS:[ESI+0xC0],CL
	$+3C5    00E2BBCA     898E C4000000       MOV DWORD PTR DS:[ESI+0xC4],ECX
	$+3CB    00E2BBD0     898E D0000000       MOV DWORD PTR DS:[ESI+0xD0],ECX
	$+3D1    00E2BBD6     8D86 C0000000       LEA EAX,DWORD PTR DS:[ESI+0xC0]
	$+3D7    00E2BBDC     8940 08             MOV DWORD PTR DS:[EAX+0x8],EAX
	$+3DA    00E2BBDF     8940 0C             MOV DWORD PTR DS:[EAX+0xC],EAX
	$+3DD    00E2BBE2     888E D8000000       MOV BYTE PTR DS:[ESI+0xD8],CL
	$+3E3    00E2BBE8     898E DC000000       MOV DWORD PTR DS:[ESI+0xDC],ECX
	$+3E9    00E2BBEE     8D86 DC000000       LEA EAX,DWORD PTR DS:[ESI+0xDC]
	$+3EF    00E2BBF4     8850 14             MOV BYTE PTR DS:[EAX+0x14],DL
	$+3F2    00E2BBF7     0FB65424 1C         MOVZX EDX,BYTE PTR SS:[ESP+0x1C]
	$+3F7    00E2BBFC     8808                MOV BYTE PTR DS:[EAX],CL
	$+3F9    00E2BBFE     8948 04             MOV DWORD PTR DS:[EAX+0x4],ECX
	$+3FC    00E2BC01     8940 08             MOV DWORD PTR DS:[EAX+0x8],EAX
	$+3FF    00E2BC04     8940 0C             MOV DWORD PTR DS:[EAX+0xC],EAX
	$+402    00E2BC07     8948 10             MOV DWORD PTR DS:[EAX+0x10],ECX
	$+405    00E2BC0A     8D86 F4000000       LEA EAX,DWORD PTR DS:[ESI+0xF4]
	$+40B    00E2BC10     8908                MOV DWORD PTR DS:[EAX],ECX
	$+40D    00E2BC12     8808                MOV BYTE PTR DS:[EAX],CL
	$+40F    00E2BC14     8948 04             MOV DWORD PTR DS:[EAX+0x4],ECX
	$+412    00E2BC17     8940 08             MOV DWORD PTR DS:[EAX+0x8],EAX
	$+415    00E2BC1A     8940 0C             MOV DWORD PTR DS:[EAX+0xC],EAX
	$+418    00E2BC1D     8948 10             MOV DWORD PTR DS:[EAX+0x10],ECX
	$+41B    00E2BC20     8850 14             MOV BYTE PTR DS:[EAX+0x14],DL
	$+41E    00E2BC23     384C24 1C           CMP BYTE PTR SS:[ESP+0x1C],CL
	$+422    00E2BC27     74 07               JE SHORT MapleSto.00E2BC30
	$+424    00E2BC29     838E A4000000 02    OR DWORD PTR DS:[ESI+0xA4],0x2
	$+42B    00E2BC30     394E 50             CMP DWORD PTR DS:[ESI+0x50],ECX
	$+42E    00E2BC33     75 20               JNZ SHORT MapleSto.00E2BC55
	$+430    00E2BC35     394E 54             CMP DWORD PTR DS:[ESI+0x54],ECX
	$+433    00E2BC38     75 1B               JNZ SHORT MapleSto.00E2BC55
	$+435    00E2BC3A     8D4424 04           LEA EAX,DWORD PTR SS:[ESP+0x4]
	$+439    00E2BC3E     50                  PUSH EAX
	$+43A    00E2BC3F     E8 ECC60000         CALL MapleSto.00E38330
	$+43F    00E2BC44     8B4C24 08           MOV ECX,DWORD PTR SS:[ESP+0x8]
	$+443    00E2BC48     8B5424 0C           MOV EDX,DWORD PTR SS:[ESP+0xC]
	$+447    00E2BC4C     83C4 04             ADD ESP,0x4
	$+44A    00E2BC4F     894E 50             MOV DWORD PTR DS:[ESI+0x50],ECX
	$+44D    00E2BC52     8956 54             MOV DWORD PTR DS:[ESI+0x54],EDX
	$+450    00E2BC55     8BC6                MOV EAX,ESI
	$+452    00E2BC57     5E                  POP ESI
	$+453    00E2BC58     83C4 08             ADD ESP,0x8
	$+456    00E2BC5B     C2 1000             RETN 0x10
	$+459    00E2BC5E     CC                  INT3
	*/
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("c7????????????e8????????8d????33??39????74??8b", 3);
	if (!op_dw_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *op_dw_tmp;
	CheckExeAddr(dw_tmp);
	ofs << "pt_cdword	g_va_model_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;
	return true;
}

bool CAutoUpdateApp::Scan_Hooks(Cmn_FC_OFstream& ofs)
{
	return true;
}

bool CAutoUpdateApp::Scan_Patch(Cmn_FC_OFstream& ofs)
{
	return true;
}

bool CAutoUpdateApp::Scan_Other(Cmn_FC_OFstream& ofs)
{
	//pt_pcvoid addr_tmp, addr_tmp1;
	pt_dword dw_tmp;
	boost::optional<pt_dword> op_dw_tmp;

	/*
	$-178    00C2EBFF      CC                        INT3
	$-177    00C2EC00      8B4424 04                 MOV EAX,DWORD PTR SS:[ESP+0x4]
	$-173    00C2EC04      83EC 14                   SUB ESP,0x14
	$-170    00C2EC07      53                        PUSH EBX
	$-16F    00C2EC08      8B5C24 24                 MOV EBX,DWORD PTR SS:[ESP+0x24]
	$-16B    00C2EC0C      55                        PUSH EBP
	$-16A    00C2EC0D      56                        PUSH ESI
	$-169    00C2EC0E      57                        PUSH EDI
	$-168    00C2EC0F      8B7C24 34                 MOV EDI,DWORD PTR SS:[ESP+0x34]
	$-164    00C2EC13      57                        PUSH EDI
	$-163    00C2EC14      53                        PUSH EBX
	$-162    00C2EC15      50                        PUSH EAX
	$-161    00C2EC16      8BF1                      MOV ESI,ECX
	$-15F    00C2EC18      E8 63E9FFFF               CALL MapleSto.00C2D580
	$-15A    00C2EC1D      8B4424 2C                 MOV EAX,DWORD PTR SS:[ESP+0x2C]
	$-156    00C2EC21      33ED                      XOR EBP,EBP
	$-154    00C2EC23      C706 18F04801             MOV DWORD PTR DS:[ESI],MapleSto.0148F018        ; ASCII "0砺"
	$-14E    00C2EC29      8946 44                   MOV DWORD PTR DS:[ESI+0x44],EAX
	$-14B    00C2EC2C      896E 48                   MOV DWORD PTR DS:[ESI+0x48],EBP
	$-148    00C2EC2F      C786 64010000 01000000    MOV DWORD PTR DS:[ESI+0x164],0x1
	$-13E    00C2EC39      89AE 68010000             MOV DWORD PTR DS:[ESI+0x168],EBP
	$-138    00C2EC3F      3BC5                      CMP EAX,EBP
	$-136    00C2EC41      0F84 B2000000             JE MapleSto.00C2ECF9
	$-130    00C2EC47      8D5424 10                 LEA EDX,DWORD PTR SS:[ESP+0x10]
	$-12C    00C2EC4B      52                        PUSH EDX
	$-12B    00C2EC4C      C74424 14 14000000        MOV DWORD PTR SS:[ESP+0x14],0x14
	$-123    00C2EC54      C74424 18 10000000        MOV DWORD PTR SS:[ESP+0x18],0x10
	$-11B    00C2EC5C      896C24 1C                 MOV DWORD PTR SS:[ESP+0x1C],EBP
	$-117    00C2EC60      896C24 20                 MOV DWORD PTR SS:[ESP+0x20],EBP
	$-113    00C2EC64      C74424 24 C4090000        MOV DWORD PTR SS:[ESP+0x24],0x9C4
	$-10B    00C2EC6C      8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-109    00C2EC6E      6A 05                     PUSH 0x5
	$-107    00C2EC70      50                        PUSH EAX
	$-106    00C2EC71      8B41 18                   MOV EAX,DWORD PTR DS:[ECX+0x18]
	$-103    00C2EC74      FFD0                      CALL EAX
	$-101    00C2EC76      85C0                      TEST EAX,EAX
	$-FF     00C2EC78      7C 16                     JL SHORT MapleSto.00C2EC90
	$-FD     00C2EC7A      8B46 44                   MOV EAX,DWORD PTR DS:[ESI+0x44]
	$-FA     00C2EC7D      8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$-F8     00C2EC7F      8B51 1C                   MOV EDX,DWORD PTR DS:[ECX+0x1C]
	$-F5     00C2EC82      50                        PUSH EAX
	$-F4     00C2EC83      FFD2                      CALL EDX
	$-F2     00C2EC85      85C0                      TEST EAX,EAX
	$-F0     00C2EC87      7D 07                     JGE SHORT MapleSto.00C2EC90
	$-EE     00C2EC89      C746 1C 06000080          MOV DWORD PTR DS:[ESI+0x1C],0x80000006
	$-E7     00C2EC90      8B46 38                   MOV EAX,DWORD PTR DS:[ESI+0x38]
	$-E4     00C2EC93      8958 10                   MOV DWORD PTR DS:[EAX+0x10],EBX
	$-E1     00C2EC96      8978 14                   MOV DWORD PTR DS:[EAX+0x14],EDI
	$-DE     00C2EC99      8B46 38                   MOV EAX,DWORD PTR DS:[ESI+0x38]
	$-DB     00C2EC9C      8958 30                   MOV DWORD PTR DS:[EAX+0x30],EBX
	$-D8     00C2EC9F      8978 34                   MOV DWORD PTR DS:[EAX+0x34],EDI
	$-D5     00C2ECA2      83C0 20                   ADD EAX,0x20
	$-D2     00C2ECA5      8B46 38                   MOV EAX,DWORD PTR DS:[ESI+0x38]
	$-CF     00C2ECA8      8958 50                   MOV DWORD PTR DS:[EAX+0x50],EBX
	$-CC     00C2ECAB      8978 54                   MOV DWORD PTR DS:[EAX+0x54],EDI
	$-C9     00C2ECAE      83C0 40                   ADD EAX,0x40
	$-C6     00C2ECB1      8B46 38                   MOV EAX,DWORD PTR DS:[ESI+0x38]
	$-C3     00C2ECB4      8958 70                   MOV DWORD PTR DS:[EAX+0x70],EBX
	$-C0     00C2ECB7      8978 74                   MOV DWORD PTR DS:[EAX+0x74],EDI
	$-BD     00C2ECBA      83C0 60                   ADD EAX,0x60
	$-BA     00C2ECBD      8B46 38                   MOV EAX,DWORD PTR DS:[ESI+0x38]
	$-B7     00C2ECC0      8998 90000000             MOV DWORD PTR DS:[EAX+0x90],EBX
	$-B1     00C2ECC6      89B8 94000000             MOV DWORD PTR DS:[EAX+0x94],EDI
	$-AB     00C2ECCC      83E8 80                   SUB EAX,-0x80
	$-A8     00C2ECCF      8B46 38                   MOV EAX,DWORD PTR DS:[ESI+0x38]
	$-A5     00C2ECD2      05 A0000000               ADD EAX,0xA0
	$-A0     00C2ECD7      8958 10                   MOV DWORD PTR DS:[EAX+0x10],EBX
	$-9D     00C2ECDA      8978 14                   MOV DWORD PTR DS:[EAX+0x14],EDI
	$-9A     00C2ECDD      8B46 38                   MOV EAX,DWORD PTR DS:[ESI+0x38]
	$-97     00C2ECE0      05 C0000000               ADD EAX,0xC0
	$-92     00C2ECE5      8958 10                   MOV DWORD PTR DS:[EAX+0x10],EBX
	$-8F     00C2ECE8      8978 14                   MOV DWORD PTR DS:[EAX+0x14],EDI
	$-8C     00C2ECEB      8B46 38                   MOV EAX,DWORD PTR DS:[ESI+0x38]
	$-89     00C2ECEE      05 E0000000               ADD EAX,0xE0
	$-84     00C2ECF3      8958 10                   MOV DWORD PTR DS:[EAX+0x10],EBX
	$-81     00C2ECF6      8978 14                   MOV DWORD PTR DS:[EAX+0x14],EDI
	$-7E     00C2ECF9      B8 204E0000               MOV EAX,0x4E20
	$-79     00C2ECFE      5F                        POP EDI
	$-78     00C2ECFF      8986 6C010000             MOV DWORD PTR DS:[ESI+0x16C],EAX
	$-72     00C2ED05      8986 70010000             MOV DWORD PTR DS:[ESI+0x170],EAX
	$-6C     00C2ED0B      89AE 74010000             MOV DWORD PTR DS:[ESI+0x174],EBP
	$-66     00C2ED11      89AE 78010000             MOV DWORD PTR DS:[ESI+0x178],EBP
	$-60     00C2ED17      8BC6                      MOV EAX,ESI
	$-5E     00C2ED19      5E                        POP ESI
	$-5D     00C2ED1A      5D                        POP EBP
	$-5C     00C2ED1B      5B                        POP EBX
	$-5B     00C2ED1C      83C4 14                   ADD ESP,0x14
	$-58     00C2ED1F      C2 1000                   RETN 0x10
	$-55     00C2ED22      CC                        INT3
	$-54     00C2ED23      CC                        INT3
	$-53     00C2ED24      CC                        INT3
	$-52     00C2ED25      CC                        INT3
	$-51     00C2ED26      CC                        INT3
	$-50     00C2ED27      CC                        INT3
	$-4F     00C2ED28      CC                        INT3
	$-4E     00C2ED29      CC                        INT3
	$-4D     00C2ED2A      CC                        INT3
	$-4C     00C2ED2B      CC                        INT3
	$-4B     00C2ED2C      CC                        INT3
	$-4A     00C2ED2D      CC                        INT3
	$-49     00C2ED2E      CC                        INT3
	$-48     00C2ED2F      CC                        INT3
	$-47     00C2ED30      56                        PUSH ESI
	$-46     00C2ED31      8BF1                      MOV ESI,ECX
	$-44     00C2ED33      E8 48F6FFFF               CALL MapleSto.00C2E380
	$-3F     00C2ED38      F64424 08 01              TEST BYTE PTR SS:[ESP+0x8],0x1
	$-3A     00C2ED3D      74 0E                     JE SHORT MapleSto.00C2ED4D
	$-38     00C2ED3F      68 7C010000               PUSH 0x17C
	$-33     00C2ED44      56                        PUSH ESI
	$-32     00C2ED45      E8 76381B00               CALL MapleSto.00DE25C0
	$-2D     00C2ED4A      83C4 08                   ADD ESP,0x8
	$-2A     00C2ED4D      8BC6                      MOV EAX,ESI
	$-28     00C2ED4F      5E                        POP ESI
	$-27     00C2ED50      C2 0400                   RETN 0x4
	$-24     00C2ED53      CC                        INT3
	$-23     00C2ED54      CC                        INT3
	$-22     00C2ED55      CC                        INT3
	$-21     00C2ED56      CC                        INT3
	$-20     00C2ED57      CC                        INT3
	$-1F     00C2ED58      CC                        INT3
	$-1E     00C2ED59      CC                        INT3
	$-1D     00C2ED5A      CC                        INT3
	$-1C     00C2ED5B      CC                        INT3
	$-1B     00C2ED5C      CC                        INT3
	$-1A     00C2ED5D      CC                        INT3
	$-19     00C2ED5E      CC                        INT3
	$-18     00C2ED5F      CC                        INT3
	$-17     00C2ED60      8B4424 04                 MOV EAX,DWORD PTR SS:[ESP+0x4]
	$-13     00C2ED64      83EC 14                   SUB ESP,0x14
	$-10     00C2ED67      56                        PUSH ESI
	$-F      00C2ED68      8BF1                      MOV ESI,ECX
	$-D      00C2ED6A      85C0                      TEST EAX,EAX
	$-B      00C2ED6C      74 74                     JE SHORT MapleSto.00C2EDE2
	$-9      00C2ED6E      8946 48                   MOV DWORD PTR DS:[ESI+0x48],EAX
	$-6      00C2ED71      8B15 1CB17301             MOV EDX,DWORD PTR DS:[0x173B11C]                ; g_a_game_wnd_handle_
	$ ==>    00C2ED77      8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$+2      00C2ED79      6A 05                     PUSH 0x5
	$+4      00C2ED7B      52                        PUSH EDX
	$+5      00C2ED7C      50                        PUSH EAX
	$+6      00C2ED7D      8B41 34                   MOV EAX,DWORD PTR DS:[ECX+0x34]
	$+9      00C2ED80      FFD0                      CALL EAX
	$+B      00C2ED82      8B46 48                   MOV EAX,DWORD PTR DS:[ESI+0x48]
	$+E      00C2ED85      8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$+10     00C2ED87      8B51 2C                   MOV EDX,DWORD PTR DS:[ECX+0x2C]
	$+13     00C2ED8A      68 9C505701               PUSH MapleSto.0157509C
	$+18     00C2ED8F      50                        PUSH EAX
	$+19     00C2ED90      FFD2                      CALL EDX
	$+1B     00C2ED92      8B46 48                   MOV EAX,DWORD PTR DS:[ESI+0x48]
	$+1E     00C2ED95      8D5424 04                 LEA EDX,DWORD PTR SS:[ESP+0x4]
	$+22     00C2ED99      52                        PUSH EDX
	$+23     00C2ED9A      C74424 08 14000000        MOV DWORD PTR SS:[ESP+0x8],0x14
	$+2B     00C2EDA2      C74424 0C 10000000        MOV DWORD PTR SS:[ESP+0xC],0x10
	$+33     00C2EDAA      C74424 10 00000000        MOV DWORD PTR SS:[ESP+0x10],0x0
	$+3B     00C2EDB2      C74424 14 00000000        MOV DWORD PTR SS:[ESP+0x14],0x0
	$+43     00C2EDBA      C74424 18 00000000        MOV DWORD PTR SS:[ESP+0x18],0x0
	$+4B     00C2EDC2      8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$+4D     00C2EDC4      6A 09                     PUSH 0x9
	$+4F     00C2EDC6      50                        PUSH EAX
	$+50     00C2EDC7      8B41 18                   MOV EAX,DWORD PTR DS:[ECX+0x18]
	$+53     00C2EDCA      FFD0                      CALL EAX
	$+55     00C2EDCC      85C0                      TEST EAX,EAX
	$+57     00C2EDCE      7C 12                     JL SHORT MapleSto.00C2EDE2
	$+59     00C2EDD0      8BCE                      MOV ECX,ESI
	$+5B     00C2EDD2      E8 B9F4FFFF               CALL MapleSto.00C2E290
	$+60     00C2EDD7      8B76 48                   MOV ESI,DWORD PTR DS:[ESI+0x48]
	$+63     00C2EDDA      8B0E                      MOV ECX,DWORD PTR DS:[ESI]
	$+65     00C2EDDC      8B51 1C                   MOV EDX,DWORD PTR DS:[ECX+0x1C]
	$+68     00C2EDDF      56                        PUSH ESI
	$+69     00C2EDE0      FFD2                      CALL EDX
	$+6B     00C2EDE2      5E                        POP ESI
	$+6C     00C2EDE3      83C4 14                   ADD ESP,0x14
	$+6F     00C2EDE6      C2 0400                   RETN 0x4
	$+72     00C2EDE9      CC                        INT3
	$+73     00C2EDEA      CC                        INT3
	$+74     00C2EDEB      CC                        INT3
	$+75     00C2EDEC      CC                        INT3
	$+76     00C2EDED      CC                        INT3
	$+77     00C2EDEE      CC                        INT3
	$+78     00C2EDEF      CC                        INT3
	$+79     00C2EDF0      81EC 58010000             SUB ESP,0x158
	$+7F     00C2EDF6      55                        PUSH EBP
	$+80     00C2EDF7      56                        PUSH ESI
	$+81     00C2EDF8      8BF1                      MOV ESI,ECX
	$+83     00C2EDFA      33ED                      XOR EBP,EBP
	$+85     00C2EDFC      896E 1C                   MOV DWORD PTR DS:[ESI+0x1C],EBP
	$+88     00C2EDFF      E8 DCE6FFFF               CALL MapleSto.00C2D4E0
	$+8D     00C2EE04      8B46 44                   MOV EAX,DWORD PTR DS:[ESI+0x44]
	$+90     00C2EE07      3BC5                      CMP EAX,EBP
	$+92     00C2EE09      75 11                     JNZ SHORT MapleSto.00C2EE1C
	$+94     00C2EE0B      B8 04000080               MOV EAX,0x80000004
	$+99     00C2EE10      8946 1C                   MOV DWORD PTR DS:[ESI+0x1C],EAX
	$+9C     00C2EE13      5E                        POP ESI
	$+9D     00C2EE14      5D                        POP EBP
	$+9E     00C2EE15      81C4 58010000             ADD ESP,0x158
	$+A4     00C2EE1B      C3                        RETN
	$+A5     00C2EE1C      C74424 08 11000000        MOV DWORD PTR SS:[ESP+0x8],0x11
	$+AD     00C2EE24      8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$+AF     00C2EE26      8B51 1C                   MOV EDX,DWORD PTR DS:[ECX+0x1C]
	$+B2     00C2EE29      50                        PUSH EAX
	$+B3     00C2EE2A      FFD2                      CALL EDX
	$+B5     00C2EE2C      3BC5                      CMP EAX,EBP
	$+B7     00C2EE2E      7D 47                     JGE SHORT MapleSto.00C2EE77
	$+B9     00C2EE30      83F8 01                   CMP EAX,0x1
	$+BC     00C2EE33      74 42                     JE SHORT MapleSto.00C2EE77
	$+BE     00C2EE35      3D 1E000780               CMP EAX,0x8007001E
	$+C3     00C2EE3A      74 21                     JE SHORT MapleSto.00C2EE5D
	$+C5     00C2EE3C      3D 0C000780               CMP EAX,0x8007000C
	$+CA     00C2EE41      74 1A                     JE SHORT MapleSto.00C2EE5D
	$+CC     00C2EE43      3D 09020480               CMP EAX,0x80040209
	$+D1     00C2EE48      74 13                     JE SHORT MapleSto.00C2EE5D
	$+D3     00C2EE4A      C746 1C 04000080          MOV DWORD PTR DS:[ESI+0x1C],0x80000004
	$+DA     00C2EE51      8B46 1C                   MOV EAX,DWORD PTR DS:[ESI+0x1C]
	$+DD     00C2EE54      5E                        POP ESI
	$+DE     00C2EE55      5D                        POP EBP
	$+DF     00C2EE56      81C4 58010000             ADD ESP,0x158
	$+E5     00C2EE5C      C3                        RETN
	$+E6     00C2EE5D      C746 1C 0A000080          MOV DWORD PTR DS:[ESI+0x1C],0x8000000A
	$+ED     00C2EE64      8B46 1C                   MOV EAX,DWORD PTR DS:[ESI+0x1C]
	$+F0     00C2EE67      C746 0C 02000000          MOV DWORD PTR DS:[ESI+0xC],0x2
	$+F7     00C2EE6E      5E                        POP ESI
	$+F8     00C2EE6F      5D                        POP EBP
	$+F9     00C2EE70      81C4 58010000             ADD ESP,0x158
	$+FF     00C2EE76      C3                        RETN
	$+100    00C2EE77      8B46 44                   MOV EAX,DWORD PTR DS:[ESI+0x44]
	$+103    00C2EE7A      896E 0C                   MOV DWORD PTR DS:[ESI+0xC],EBP
	$+106    00C2EE7D      8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$+108    00C2EE7F      8B51 64                   MOV EDX,DWORD PTR DS:[ECX+0x64]
	$+10B    00C2EE82      50                        PUSH EAX
	$+10C    00C2EE83      FFD2                      CALL EDX
	$+10E    00C2EE85      3BC5                      CMP EAX,EBP
	$+110    00C2EE87      7D 09                     JGE SHORT MapleSto.00C2EE92
	$+112    00C2EE89      83F8 01                   CMP EAX,0x1
	$+115    00C2EE8C    ^ 0F85 79FFFFFF             JNZ MapleSto.00C2EE0B
	$+11B    00C2EE92      8B46 44                   MOV EAX,DWORD PTR DS:[ESI+0x44]
	$+11E    00C2EE95      8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$+120    00C2EE97      55                        PUSH EBP
	$+121    00C2EE98      8D5424 0C                 LEA EDX,DWORD PTR SS:[ESP+0xC]
	$+125    00C2EE9C      52                        PUSH EDX
	$+126    00C2EE9D      8D5424 14                 LEA EDX,DWORD PTR SS:[ESP+0x14]
	$+12A    00C2EEA1      52                        PUSH EDX
	$+12B    00C2EEA2      6A 14                     PUSH 0x14
	$+12D    00C2EEA4      50                        PUSH EAX
	$+12E    00C2EEA5      8B41 28                   MOV EAX,DWORD PTR DS:[ECX+0x28]
	$+131    00C2EEA8      FFD0                      CALL EAX
	$+133    00C2EEAA      85C0                      TEST EAX,EAX
	$+135    00C2EEAC      0F8C 5C010000             JL MapleSto.00C2F00E
	$+13B    00C2EEB2      8B4424 08                 MOV EAX,DWORD PTR SS:[ESP+0x8]
	$+13F    00C2EEB6      3BC5                      CMP EAX,EBP
	$+141    00C2EEB8      0F84 47010000             JE MapleSto.00C2F005
	$+147    00C2EEBE      53                        PUSH EBX
	$+148    00C2EEBF      33DB                      XOR EBX,EBX
	$+14A    00C2EEC1      57                        PUSH EDI
	$+14B    00C2EEC2      89AE 5C010000             MOV DWORD PTR DS:[ESI+0x15C],EBP
	$+151    00C2EEC8      89AE 60010000             MOV DWORD PTR DS:[ESI+0x160],EBP
	$+157    00C2EECE      3BC5                      CMP EAX,EBP
	$+159    00C2EED0      76 1C                     JBE SHORT MapleSto.00C2EEEE
	$+15B    00C2EED2      8D7C24 24                 LEA EDI,DWORD PTR SS:[ESP+0x24]
	$+15F    00C2EED6      8B4F F4                   MOV ECX,DWORD PTR DS:[EDI-0xC]
	$+162    00C2EED9      8B17                      MOV EDX,DWORD PTR DS:[EDI]
	$+164    00C2EEDB      51                        PUSH ECX
	$+165    00C2EEDC      52                        PUSH EDX
	$+166    00C2EEDD      8BCE                      MOV ECX,ESI
	$+168    00C2EEDF      E8 9CF5FFFF               CALL MapleSto.00C2E480
	$+16D    00C2EEE4      43                        INC EBX
	$+16E    00C2EEE5      83C7 14                   ADD EDI,0x14
	$+171    00C2EEE8      3B5C24 10                 CMP EBX,DWORD PTR SS:[ESP+0x10]
	$+175    00C2EEEC    ^ 72 E8                     JB SHORT MapleSto.00C2EED6
	$+177    00C2EEEE      B9 01000000               MOV ECX,0x1
	$+17C    00C2EEF3      33FF                      XOR EDI,EDI
	$+17E    00C2EEF5      858E 5C010000             TEST DWORD PTR DS:[ESI+0x15C],ECX
	$+184    00C2EEFB      75 19                     JNZ SHORT MapleSto.00C2EF16
	$+186    00C2EEFD      8B46 30                   MOV EAX,DWORD PTR DS:[ESI+0x30]
	$+189    00C2EF00      0FB61438                  MOVZX EDX,BYTE PTR DS:[EAX+EDI]
	$+18D    00C2EF04      03C7                      ADD EAX,EDI
	$+18F    00C2EF06      8850 02                   MOV BYTE PTR DS:[EAX+0x2],DL
	$+192    00C2EF09      0FB650 01                 MOVZX EDX,BYTE PTR DS:[EAX+0x1]
	$+196    00C2EF0D      8850 03                   MOV BYTE PTR DS:[EAX+0x3],DL
	$+199    00C2EF10      8B50 08                   MOV EDX,DWORD PTR DS:[EAX+0x8]
	$+19C    00C2EF13      8950 0C                   MOV DWORD PTR DS:[EAX+0xC],EDX
	$+19F    00C2EF16      D1C1                      ROL ECX,1
	$+1A1    00C2EF18      858E 5C010000             TEST DWORD PTR DS:[ESI+0x15C],ECX
	$+1A7    00C2EF1E      75 1C                     JNZ SHORT MapleSto.00C2EF3C
	$+1A9    00C2EF20      8B46 30                   MOV EAX,DWORD PTR DS:[ESI+0x30]
	$+1AC    00C2EF23      0FB65438 10               MOVZX EDX,BYTE PTR DS:[EAX+EDI+0x10]
	$+1B1    00C2EF28      8D4438 10                 LEA EAX,DWORD PTR DS:[EAX+EDI+0x10]
	$+1B5    00C2EF2C      8850 02                   MOV BYTE PTR DS:[EAX+0x2],DL
	$+1B8    00C2EF2F      0FB650 01                 MOVZX EDX,BYTE PTR DS:[EAX+0x1]
	$+1BC    00C2EF33      8850 03                   MOV BYTE PTR DS:[EAX+0x3],DL
	$+1BF    00C2EF36      8B50 08                   MOV EDX,DWORD PTR DS:[EAX+0x8]
	$+1C2    00C2EF39      8950 0C                   MOV DWORD PTR DS:[EAX+0xC],EDX
	$+1C5    00C2EF3C      D1C1                      ROL ECX,1
	$+1C7    00C2EF3E      858E 5C010000             TEST DWORD PTR DS:[ESI+0x15C],ECX
	$+1CD    00C2EF44      75 1C                     JNZ SHORT MapleSto.00C2EF62
	$+1CF    00C2EF46      8B46 30                   MOV EAX,DWORD PTR DS:[ESI+0x30]
	$+1D2    00C2EF49      0FB65438 20               MOVZX EDX,BYTE PTR DS:[EAX+EDI+0x20]
	$+1D7    00C2EF4E      8D4438 20                 LEA EAX,DWORD PTR DS:[EAX+EDI+0x20]
	$+1DB    00C2EF52      8850 02                   MOV BYTE PTR DS:[EAX+0x2],DL
	$+1DE    00C2EF55      0FB650 01                 MOVZX EDX,BYTE PTR DS:[EAX+0x1]
	$+1E2    00C2EF59      8850 03                   MOV BYTE PTR DS:[EAX+0x3],DL
	$+1E5    00C2EF5C      8B50 08                   MOV EDX,DWORD PTR DS:[EAX+0x8]
	$+1E8    00C2EF5F      8950 0C                   MOV DWORD PTR DS:[EAX+0xC],EDX
	$+1EB    00C2EF62      D1C1                      ROL ECX,1
	$+1ED    00C2EF64      858E 5C010000             TEST DWORD PTR DS:[ESI+0x15C],ECX
	$+1F3    00C2EF6A      75 1C                     JNZ SHORT MapleSto.00C2EF88
	$+1F5    00C2EF6C      8B46 30                   MOV EAX,DWORD PTR DS:[ESI+0x30]
	$+1F8    00C2EF6F      0FB65438 30               MOVZX EDX,BYTE PTR DS:[EAX+EDI+0x30]
	$+1FD    00C2EF74      8D4438 30                 LEA EAX,DWORD PTR DS:[EAX+EDI+0x30]
	$+201    00C2EF78      8850 02                   MOV BYTE PTR DS:[EAX+0x2],DL
	$+204    00C2EF7B      0FB650 01                 MOVZX EDX,BYTE PTR DS:[EAX+0x1]
	$+208    00C2EF7F      8850 03                   MOV BYTE PTR DS:[EAX+0x3],DL
	$+20B    00C2EF82      8B50 08                   MOV EDX,DWORD PTR DS:[EAX+0x8]
	$+20E    00C2EF85      8950 0C                   MOV DWORD PTR DS:[EAX+0xC],EDX
	$+211    00C2EF88      83C7 40                   ADD EDI,0x40
	$+214    00C2EF8B      D1C1                      ROL ECX,1
	$+216    00C2EF8D      81FF 00010000             CMP EDI,0x100
	$+21C    00C2EF93    ^ 0F82 5CFFFFFF             JB MapleSto.00C2EEF5
	$+222    00C2EF99      F686 60010000 01          TEST BYTE PTR DS:[ESI+0x160],0x1
	$+229    00C2EFA0      5F                        POP EDI
	$+22A    00C2EFA1      5B                        POP EBX
	$+22B    00C2EFA2      75 0E                     JNZ SHORT MapleSto.00C2EFB2
	$+22D    00C2EFA4      8B46 38                   MOV EAX,DWORD PTR DS:[ESI+0x38]
	$+230    00C2EFA7      8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$+232    00C2EFA9      8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+235    00C2EFAC      8948 08                   MOV DWORD PTR DS:[EAX+0x8],ECX
	$+238    00C2EFAF      8950 0C                   MOV DWORD PTR DS:[EAX+0xC],EDX
	$+23B    00C2EFB2      F686 60010000 02          TEST BYTE PTR DS:[ESI+0x160],0x2
	$+242    00C2EFB9      75 12                     JNZ SHORT MapleSto.00C2EFCD
	$+244    00C2EFBB      8B46 38                   MOV EAX,DWORD PTR DS:[ESI+0x38]
	$+247    00C2EFBE      8B48 20                   MOV ECX,DWORD PTR DS:[EAX+0x20]
	$+24A    00C2EFC1      83C0 20                   ADD EAX,0x20
	$+24D    00C2EFC4      8948 08                   MOV DWORD PTR DS:[EAX+0x8],ECX
	$+250    00C2EFC7      8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+253    00C2EFCA      8950 0C                   MOV DWORD PTR DS:[EAX+0xC],EDX
	$+256    00C2EFCD      F686 60010000 04          TEST BYTE PTR DS:[ESI+0x160],0x4
	$+25D    00C2EFD4      75 12                     JNZ SHORT MapleSto.00C2EFE8
	$+25F    00C2EFD6      8B46 38                   MOV EAX,DWORD PTR DS:[ESI+0x38]
	$+262    00C2EFD9      8B48 40                   MOV ECX,DWORD PTR DS:[EAX+0x40]
	$+265    00C2EFDC      83C0 40                   ADD EAX,0x40
	$+268    00C2EFDF      8948 08                   MOV DWORD PTR DS:[EAX+0x8],ECX
	$+26B    00C2EFE2      8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+26E    00C2EFE5      8950 0C                   MOV DWORD PTR DS:[EAX+0xC],EDX
	$+271    00C2EFE8      F686 60010000 08          TEST BYTE PTR DS:[ESI+0x160],0x8
	$+278    00C2EFEF      75 1D                     JNZ SHORT MapleSto.00C2F00E
	$+27A    00C2EFF1      8B46 38                   MOV EAX,DWORD PTR DS:[ESI+0x38]
	$+27D    00C2EFF4      8B48 60                   MOV ECX,DWORD PTR DS:[EAX+0x60]
	$+280    00C2EFF7      83C0 60                   ADD EAX,0x60
	$+283    00C2EFFA      8948 08                   MOV DWORD PTR DS:[EAX+0x8],ECX
	$+286    00C2EFFD      8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+289    00C2F000      8950 0C                   MOV DWORD PTR DS:[EAX+0xC],EDX
	$+28C    00C2F003      EB 09                     JMP SHORT MapleSto.00C2F00E
	$+28E    00C2F005      8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$+290    00C2F007      8B50 1C                   MOV EDX,DWORD PTR DS:[EAX+0x1C]
	$+293    00C2F00A      8BCE                      MOV ECX,ESI
	$+295    00C2F00C      FFD2                      CALL EDX
	$+297    00C2F00E      8B46 48                   MOV EAX,DWORD PTR DS:[ESI+0x48]
	$+29A    00C2F011      3BC5                      CMP EAX,EBP
	$+29C    00C2F013      74 13                     JE SHORT MapleSto.00C2F028
	$+29E    00C2F015      8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$+2A0    00C2F017      8B51 1C                   MOV EDX,DWORD PTR DS:[ECX+0x1C]
	$+2A3    00C2F01A      50                        PUSH EAX
	$+2A4    00C2F01B      FFD2                      CALL EDX
	$+2A6    00C2F01D      8B46 48                   MOV EAX,DWORD PTR DS:[ESI+0x48]
	$+2A9    00C2F020      8B08                      MOV ECX,DWORD PTR DS:[EAX]
	$+2AB    00C2F022      8B51 64                   MOV EDX,DWORD PTR DS:[ECX+0x64]
	$+2AE    00C2F025      50                        PUSH EAX
	$+2AF    00C2F026      FFD2                      CALL EDX
	$+2B1    00C2F028      8B46 1C                   MOV EAX,DWORD PTR DS:[ESI+0x1C]
	$+2B4    00C2F02B      5E                        POP ESI
	$+2B5    00C2F02C      5D                        POP EBP
	$+2B6    00C2F02D      81C4 58010000             ADD ESP,0x158
	$+2BC    00C2F033      C3                        RETN
	$+2BD    00C2F034      CC                        INT3
	*/
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("8B086A0552508B4134FFD0", -6 + 2);
	if (!op_dw_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *op_dw_tmp;
	CheckExeAddr(dw_tmp);
	ofs << "pt_cdword	g_a_game_wnd_handle_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	/*
	$-480    009749BF     CC                       INT3
	$-47F    009749C0     55                       PUSH EBP
	$-47E    009749C1     8BEC                     MOV EBP,ESP
	$-47C    009749C3     6A FF                    PUSH -0x1
	$-47A    009749C5     68 588F3601              PUSH MapleSto.01368F58
	$-475    009749CA     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$-46F    009749D0     50                       PUSH EAX
	$-46E    009749D1     83EC 28                  SUB ESP,0x28
	$-46B    009749D4     53                       PUSH EBX
	$-46A    009749D5     56                       PUSH ESI
	$-469    009749D6     57                       PUSH EDI
	$-468    009749D7     A1 D0687101              MOV EAX,DWORD PTR DS:[0x17168D0]
	$-463    009749DC     33C5                     XOR EAX,EBP
	$-461    009749DE     50                       PUSH EAX
	$-460    009749DF     8D45 F4                  LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-45D    009749E2     64:A3 00000000           MOV DWORD PTR FS:[0],EAX
	$-457    009749E8     8BF1                     MOV ESI,ECX
	$-455    009749EA     6A 05                    PUSH 0x5
	$-453    009749EC     8D45 DC                  LEA EAX,DWORD PTR SS:[EBP-0x24]
	$-450    009749EF     33DB                     XOR EBX,EBX
	$-44E    009749F1     50                       PUSH EAX
	$-44D    009749F2     895D EC                  MOV DWORD PTR SS:[EBP-0x14],EBX
	$-44A    009749F5     895D F0                  MOV DWORD PTR SS:[EBP-0x10],EBX
	$-447    009749F8     E8 D387FFFF              CALL MapleSto.0096D1D0
	$-442    009749FD     8B4D E0                  MOV ECX,DWORD PTR SS:[EBP-0x20]
	$-43F    00974A00     895D FC                  MOV DWORD PTR SS:[EBP-0x4],EBX
	$-43C    00974A03     3BCB                     CMP ECX,EBX
	$-43A    00974A05     0F85 A5000000            JNZ MapleSto.00974AB0
	$-434    00974A0B     6A 05                    PUSH 0x5
	$-432    00974A0D     8D4D D4                  LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$-42F    00974A10     51                       PUSH ECX
	$-42E    00974A11     8BCE                     MOV ECX,ESI
	$-42C    00974A13     E8 5887FFFF              CALL MapleSto.0096D170
	$-427    00974A18     50                       PUSH EAX
	$-426    00974A19     8D4D DC                  LEA ECX,DWORD PTR SS:[EBP-0x24]
	$-423    00974A1C     C645 FC 01               MOV BYTE PTR SS:[EBP-0x4],0x1
	$-41F    00974A20     E8 FB8BBEFF              CALL MapleSto.0055D620
	$-41A    00974A25     8B45 D8                  MOV EAX,DWORD PTR SS:[EBP-0x28]
	$-417    00974A28     885D FC                  MOV BYTE PTR SS:[EBP-0x4],BL
	$-414    00974A2B     3BC3                     CMP EAX,EBX
	$-412    00974A2D     74 2A                    JE SHORT MapleSto.00974A59
	$-410    00974A2F     83C0 04                  ADD EAX,0x4
	$-40D    00974A32     50                       PUSH EAX
	$-40C    00974A33     FF15 6C933E01            CALL DWORD PTR DS:[0x13E936C]
	$-406    00974A39     85C0                     TEST EAX,EAX
	$-404    00974A3B     75 1C                    JNZ SHORT MapleSto.00974A59
	$-402    00974A3D     8B55 D8                  MOV EDX,DWORD PTR SS:[EBP-0x28]
	$-3FF    00974A40     83C2 04                  ADD EDX,0x4
	$-3FC    00974A43     52                       PUSH EDX
	$-3FB    00974A44     FF15 F8923E01            CALL DWORD PTR DS:[0x13E92F8]
	$-3F5    00974A4A     8B4D D8                  MOV ECX,DWORD PTR SS:[EBP-0x28]
	$-3F2    00974A4D     3BCB                     CMP ECX,EBX
	$-3F0    00974A4F     74 08                    JE SHORT MapleSto.00974A59
	$-3EE    00974A51     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$-3EC    00974A53     8B10                     MOV EDX,DWORD PTR DS:[EAX]
	$-3EA    00974A55     6A 01                    PUSH 0x1
	$-3E8    00974A57     FFD2                     CALL EDX
	$-3E6    00974A59     8B4D E0                  MOV ECX,DWORD PTR SS:[EBP-0x20]
	$-3E3    00974A5C     3BCB                     CMP ECX,EBX
	$-3E1    00974A5E     75 50                    JNZ SHORT MapleSto.00974AB0
	$-3DF    00974A60     3BF3                     CMP ESI,EBX
	$-3DD    00974A62     74 0B                    JE SHORT MapleSto.00974A6F
	$-3DB    00974A64     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$-3D9    00974A66     8B50 18                  MOV EDX,DWORD PTR DS:[EAX+0x18]
	$-3D6    00974A69     8BCE                     MOV ECX,ESI
	$-3D4    00974A6B     FFD2                     CALL EDX
	$-3D2    00974A6D     EB 02                    JMP SHORT MapleSto.00974A71
	$-3D0    00974A6F     33C0                     XOR EAX,EAX
	$-3CE    00974A71     68 44002006              PUSH 0x6200044
	$-3C9    00974A76     50                       PUSH EAX
	$-3C8    00974A77     E8 44F7BFFF              CALL MapleSto.005741C0
	$-3C3    00974A7C     83C4 08                  ADD ESP,0x8
	$-3C0    00974A7F     84C0                     TEST AL,AL
	$-3BE    00974A81     74 26                    JE SHORT MapleSto.00974AA9
	$-3BC    00974A83     6A 05                    PUSH 0x5
	$-3BA    00974A85     8D45 D4                  LEA EAX,DWORD PTR SS:[EBP-0x2C]
	$-3B7    00974A88     50                       PUSH EAX
	$-3B6    00974A89     8BCE                     MOV ECX,ESI
	$-3B4    00974A8B     E8 1087FFFF              CALL MapleSto.0096D1A0
	$-3AF    00974A90     50                       PUSH EAX
	$-3AE    00974A91     8D4D DC                  LEA ECX,DWORD PTR SS:[EBP-0x24]
	$-3AB    00974A94     C645 FC 02               MOV BYTE PTR SS:[EBP-0x4],0x2
	$-3A7    00974A98     E8 838BBEFF              CALL MapleSto.0055D620
	$-3A2    00974A9D     8D4D D4                  LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$-39F    00974AA0     C645 FC 00               MOV BYTE PTR SS:[EBP-0x4],0x0
	$-39B    00974AA4     E8 67861000              CALL MapleSto.00A7D110
	$-396    00974AA9     8B4D E0                  MOV ECX,DWORD PTR SS:[EBP-0x20]
	$-393    00974AAC     3BCB                     CMP ECX,EBX
	$-391    00974AAE     74 39                    JE SHORT MapleSto.00974AE9
	$-38F    00974AB0     E8 A2BBE4FF              CALL MapleSto.007C0657
	$-38A    00974AB5     8B88 F0000000            MOV ECX,DWORD PTR DS:[EAX+0xF0]
	$-384    00974ABB     894D EC                  MOV DWORD PTR SS:[EBP-0x14],ECX
	$-381    00974ABE     8B4D E0                  MOV ECX,DWORD PTR SS:[EBP-0x20]
	$-37E    00974AC1     3BCB                     CMP ECX,EBX
	$-37C    00974AC3     74 24                    JE SHORT MapleSto.00974AE9
	$-37A    00974AC5     E8 9626E5FF              CALL MapleSto.007C7160
	$-375    00974ACA     3958 04                  CMP DWORD PTR DS:[EAX+0x4],EBX
	$-372    00974ACD     74 1A                    JE SHORT MapleSto.00974AE9
	$-370    00974ACF     8B4D E0                  MOV ECX,DWORD PTR SS:[EBP-0x20]
	$-36D    00974AD2     E8 8926E5FF              CALL MapleSto.007C7160
	$-368    00974AD7     8B40 04                  MOV EAX,DWORD PTR DS:[EAX+0x4]
	$-365    00974ADA     50                       PUSH EAX
	$-364    00974ADB     8BCE                     MOV ECX,ESI
	$-362    00974ADD     E8 3E5AFFFF              CALL MapleSto.0096A520
	$-35D    00974AE2     85C0                     TEST EAX,EAX
	$-35B    00974AE4     75 03                    JNZ SHORT MapleSto.00974AE9
	$-359    00974AE6     895D EC                  MOV DWORD PTR SS:[EBP-0x14],EBX
	$-356    00974AE9     6A 04                    PUSH 0x4
	$-354    00974AEB     8D55 E4                  LEA EDX,DWORD PTR SS:[EBP-0x1C]
	$-351    00974AEE     52                       PUSH EDX
	$-350    00974AEF     8BCE                     MOV ECX,ESI
	$-34E    00974AF1     E8 DA86FFFF              CALL MapleSto.0096D1D0
	$-349    00974AF6     8B4D E8                  MOV ECX,DWORD PTR SS:[EBP-0x18]
	$-346    00974AF9     C645 FC 03               MOV BYTE PTR SS:[EBP-0x4],0x3
	$-342    00974AFD     3BCB                     CMP ECX,EBX
	$-340    00974AFF     0F85 A6000000            JNZ MapleSto.00974BAB
	$-33A    00974B05     6A 04                    PUSH 0x4
	$-338    00974B07     8D45 D4                  LEA EAX,DWORD PTR SS:[EBP-0x2C]
	$-335    00974B0A     50                       PUSH EAX
	$-334    00974B0B     8BCE                     MOV ECX,ESI
	$-332    00974B0D     E8 5E86FFFF              CALL MapleSto.0096D170
	$-32D    00974B12     50                       PUSH EAX
	$-32C    00974B13     8D4D E4                  LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$-329    00974B16     C645 FC 04               MOV BYTE PTR SS:[EBP-0x4],0x4
	$-325    00974B1A     E8 018BBEFF              CALL MapleSto.0055D620
	$-320    00974B1F     8B45 D8                  MOV EAX,DWORD PTR SS:[EBP-0x28]
	$-31D    00974B22     C645 FC 03               MOV BYTE PTR SS:[EBP-0x4],0x3
	$-319    00974B26     3BC3                     CMP EAX,EBX
	$-317    00974B28     74 2A                    JE SHORT MapleSto.00974B54
	$-315    00974B2A     83C0 04                  ADD EAX,0x4
	$-312    00974B2D     50                       PUSH EAX
	$-311    00974B2E     FF15 6C933E01            CALL DWORD PTR DS:[0x13E936C]
	$-30B    00974B34     85C0                     TEST EAX,EAX
	$-309    00974B36     75 1C                    JNZ SHORT MapleSto.00974B54
	$-307    00974B38     8B4D D8                  MOV ECX,DWORD PTR SS:[EBP-0x28]
	$-304    00974B3B     83C1 04                  ADD ECX,0x4
	$-301    00974B3E     51                       PUSH ECX
	$-300    00974B3F     FF15 F8923E01            CALL DWORD PTR DS:[0x13E92F8]
	$-2FA    00974B45     8B4D D8                  MOV ECX,DWORD PTR SS:[EBP-0x28]
	$-2F7    00974B48     3BCB                     CMP ECX,EBX
	$-2F5    00974B4A     74 08                    JE SHORT MapleSto.00974B54
	$-2F3    00974B4C     8B11                     MOV EDX,DWORD PTR DS:[ECX]
	$-2F1    00974B4E     8B02                     MOV EAX,DWORD PTR DS:[EDX]
	$-2EF    00974B50     6A 01                    PUSH 0x1
	$-2ED    00974B52     FFD0                     CALL EAX
	$-2EB    00974B54     8B4D E8                  MOV ECX,DWORD PTR SS:[EBP-0x18]
	$-2E8    00974B57     3BCB                     CMP ECX,EBX
	$-2E6    00974B59     75 50                    JNZ SHORT MapleSto.00974BAB
	$-2E4    00974B5B     3BF3                     CMP ESI,EBX
	$-2E2    00974B5D     74 0B                    JE SHORT MapleSto.00974B6A
	$-2E0    00974B5F     8B16                     MOV EDX,DWORD PTR DS:[ESI]
	$-2DE    00974B61     8B42 18                  MOV EAX,DWORD PTR DS:[EDX+0x18]
	$-2DB    00974B64     8BCE                     MOV ECX,ESI
	$-2D9    00974B66     FFD0                     CALL EAX
	$-2D7    00974B68     EB 02                    JMP SHORT MapleSto.00974B6C
	$-2D5    00974B6A     33C0                     XOR EAX,EAX
	$-2D3    00974B6C     68 44002006              PUSH 0x6200044
	$-2CE    00974B71     50                       PUSH EAX
	$-2CD    00974B72     E8 49F6BFFF              CALL MapleSto.005741C0
	$-2C8    00974B77     83C4 08                  ADD ESP,0x8
	$-2C5    00974B7A     84C0                     TEST AL,AL
	$-2C3    00974B7C     74 26                    JE SHORT MapleSto.00974BA4
	$-2C1    00974B7E     6A 04                    PUSH 0x4
	$-2BF    00974B80     8D4D D4                  LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$-2BC    00974B83     51                       PUSH ECX
	$-2BB    00974B84     8BCE                     MOV ECX,ESI
	$-2B9    00974B86     E8 1586FFFF              CALL MapleSto.0096D1A0
	$-2B4    00974B8B     50                       PUSH EAX
	$-2B3    00974B8C     8D4D E4                  LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$-2B0    00974B8F     C645 FC 05               MOV BYTE PTR SS:[EBP-0x4],0x5
	$-2AC    00974B93     E8 888ABEFF              CALL MapleSto.0055D620
	$-2A7    00974B98     8D4D D4                  LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$-2A4    00974B9B     C645 FC 03               MOV BYTE PTR SS:[EBP-0x4],0x3
	$-2A0    00974B9F     E8 6C851000              CALL MapleSto.00A7D110
	$-29B    00974BA4     8B4D E8                  MOV ECX,DWORD PTR SS:[EBP-0x18]
	$-298    00974BA7     3BCB                     CMP ECX,EBX
	$-296    00974BA9     74 39                    JE SHORT MapleSto.00974BE4
	$-294    00974BAB     E8 A7BAE4FF              CALL MapleSto.007C0657
	$-28F    00974BB0     8B4D E8                  MOV ECX,DWORD PTR SS:[EBP-0x18]
	$-28C    00974BB3     8B90 F0000000            MOV EDX,DWORD PTR DS:[EAX+0xF0]
	$-286    00974BB9     8955 F0                  MOV DWORD PTR SS:[EBP-0x10],EDX
	$-283    00974BBC     3BCB                     CMP ECX,EBX
	$-281    00974BBE     74 24                    JE SHORT MapleSto.00974BE4
	$-27F    00974BC0     E8 9B25E5FF              CALL MapleSto.007C7160
	$-27A    00974BC5     3958 04                  CMP DWORD PTR DS:[EAX+0x4],EBX
	$-277    00974BC8     74 1A                    JE SHORT MapleSto.00974BE4
	$-275    00974BCA     8B4D E8                  MOV ECX,DWORD PTR SS:[EBP-0x18]
	$-272    00974BCD     E8 8E25E5FF              CALL MapleSto.007C7160
	$-26D    00974BD2     8B40 04                  MOV EAX,DWORD PTR DS:[EAX+0x4]
	$-26A    00974BD5     50                       PUSH EAX
	$-269    00974BD6     8BCE                     MOV ECX,ESI
	$-267    00974BD8     E8 4359FFFF              CALL MapleSto.0096A520
	$-262    00974BDD     85C0                     TEST EAX,EAX
	$-260    00974BDF     75 03                    JNZ SHORT MapleSto.00974BE4
	$-25E    00974BE1     895D F0                  MOV DWORD PTR SS:[EBP-0x10],EBX
	$-25B    00974BE4     8B45 E8                  MOV EAX,DWORD PTR SS:[EBP-0x18]
	$-258    00974BE7     3945 E0                  CMP DWORD PTR SS:[EBP-0x20],EAX
	$-255    00974BEA     75 03                    JNZ SHORT MapleSto.00974BEF
	$-253    00974BEC     895D F0                  MOV DWORD PTR SS:[EBP-0x10],EBX
	$-250    00974BEF     8B4D F0                  MOV ECX,DWORD PTR SS:[EBP-0x10]
	$-24D    00974BF2     8B55 EC                  MOV EDX,DWORD PTR SS:[EBP-0x14]
	$-24A    00974BF5     51                       PUSH ECX
	$-249    00974BF6     52                       PUSH EDX
	$-248    00974BF7     E8 E98B5201              CALL MapleSto.01E9D7E5
	$-243    00974BFC     83C4 08                  ADD ESP,0x8
	$-240    00974BFF     85C0                     TEST EAX,EAX
	$-23E    00974C01     75 03                    JNZ SHORT MapleSto.00974C06
	$-23C    00974C03     895D F0                  MOV DWORD PTR SS:[EBP-0x10],EBX
	$-239    00974C06     8BBE 34120000            MOV EDI,DWORD PTR DS:[ESI+0x1234]
	$-233    00974C0C     897D D0                  MOV DWORD PTR SS:[EBP-0x30],EDI
	$-230    00974C0F     3BFB                     CMP EDI,EBX
	$-22E    00974C11     74 0A                    JE SHORT MapleSto.00974C1D
	$-22C    00974C13     8D47 04                  LEA EAX,DWORD PTR DS:[EDI+0x4]
	$-229    00974C16     50                       PUSH EAX
	$-228    00974C17     FF15 F8923E01            CALL DWORD PTR DS:[0x13E92F8]
	$-222    00974C1D     8B4D F0                  MOV ECX,DWORD PTR SS:[EBP-0x10]
	$-21F    00974C20     8B55 EC                  MOV EDX,DWORD PTR SS:[EBP-0x14]
	$-21C    00974C23     51                       PUSH ECX
	$-21B    00974C24     52                       PUSH EDX
	$-21A    00974C25     8D45 D4                  LEA EAX,DWORD PTR SS:[EBP-0x2C]
	$-217    00974C28     B3 06                    MOV BL,0x6
	$-215    00974C2A     50                       PUSH EAX
	$-214    00974C2B     885D FC                  MOV BYTE PTR SS:[EBP-0x4],BL
	$-211    00974C2E     E8 55DC5101              CALL MapleSto.01E92888
	$-20C    00974C33     83C4 0C                  ADD ESP,0xC
	$-209    00974C36     50                       PUSH EAX
	$-208    00974C37     8D8E 30120000            LEA ECX,DWORD PTR DS:[ESI+0x1230]
	$-202    00974C3D     C645 FC 07               MOV BYTE PTR SS:[EBP-0x4],0x7
	$-1FE    00974C41     E8 DA89BEFF              CALL MapleSto.0055D620
	$-1F9    00974C46     8B45 D8                  MOV EAX,DWORD PTR SS:[EBP-0x28]
	$-1F6    00974C49     885D FC                  MOV BYTE PTR SS:[EBP-0x4],BL
	$-1F3    00974C4C     85C0                     TEST EAX,EAX
	$-1F1    00974C4E     74 31                    JE SHORT MapleSto.00974C81
	$-1EF    00974C50     83C0 04                  ADD EAX,0x4
	$-1EC    00974C53     50                       PUSH EAX
	$-1EB    00974C54     FF15 6C933E01            CALL DWORD PTR DS:[0x13E936C]
	$-1E5    00974C5A     85C0                     TEST EAX,EAX
	$-1E3    00974C5C     75 1C                    JNZ SHORT MapleSto.00974C7A
	$-1E1    00974C5E     8B4D D8                  MOV ECX,DWORD PTR SS:[EBP-0x28]
	$-1DE    00974C61     83C1 04                  ADD ECX,0x4
	$-1DB    00974C64     51                       PUSH ECX
	$-1DA    00974C65     FF15 F8923E01            CALL DWORD PTR DS:[0x13E92F8]
	$-1D4    00974C6B     8B4D D8                  MOV ECX,DWORD PTR SS:[EBP-0x28]
	$-1D1    00974C6E     85C9                     TEST ECX,ECX
	$-1CF    00974C70     74 08                    JE SHORT MapleSto.00974C7A
	$-1CD    00974C72     8B11                     MOV EDX,DWORD PTR DS:[ECX]
	$-1CB    00974C74     8B02                     MOV EAX,DWORD PTR DS:[EDX]
	$-1C9    00974C76     6A 01                    PUSH 0x1
	$-1C7    00974C78     FFD0                     CALL EAX
	$-1C5    00974C7A     C745 D8 00000000         MOV DWORD PTR SS:[EBP-0x28],0x0
	$-1BE    00974C81     8B86 34120000            MOV EAX,DWORD PTR DS:[ESI+0x1234]
	$-1B8    00974C87     3BF8                     CMP EDI,EAX
	$-1B6    00974C89     74 15                    JE SHORT MapleSto.00974CA0
	$-1B4    00974C8B     85C0                     TEST EAX,EAX
	$-1B2    00974C8D     74 11                    JE SHORT MapleSto.00974CA0
	$-1B0    00974C8F     8BCE                     MOV ECX,ESI
	$-1AE    00974C91     E8 A5984A01              CALL MapleSto.01E1E53B
	$-1A9    00974C96     6A 01                    PUSH 0x1
	$-1A7    00974C98     50                       PUSH EAX
	$-1A6    00974C99     8BCE                     MOV ECX,ESI
	$-1A4    00974C9B     E8 4F6C4A01              CALL MapleSto.01E1B8EF
	$-19F    00974CA0     8BB6 34120000            MOV ESI,DWORD PTR DS:[ESI+0x1234]
	$-199    00974CA6     33DB                     XOR EBX,EBX
	$-197    00974CA8     3BFE                     CMP EDI,ESI
	$-195    00974CAA     0F95C3                   SETNE BL
	$-192    00974CAD     C645 FC 03               MOV BYTE PTR SS:[EBP-0x4],0x3
	$-18E    00974CB1     85FF                     TEST EDI,EDI
	$-18C    00974CB3     74 1F                    JE SHORT MapleSto.00974CD4
	$-18A    00974CB5     8D77 04                  LEA ESI,DWORD PTR DS:[EDI+0x4]
	$-187    00974CB8     56                       PUSH ESI
	$-186    00974CB9     FF15 6C933E01            CALL DWORD PTR DS:[0x13E936C]
	$-180    00974CBF     85C0                     TEST EAX,EAX
	$-17E    00974CC1     75 11                    JNZ SHORT MapleSto.00974CD4
	$-17C    00974CC3     56                       PUSH ESI
	$-17B    00974CC4     FF15 F8923E01            CALL DWORD PTR DS:[0x13E92F8]
	$-175    00974CCA     8B17                     MOV EDX,DWORD PTR DS:[EDI]
	$-173    00974CCC     8B02                     MOV EAX,DWORD PTR DS:[EDX]
	$-171    00974CCE     6A 01                    PUSH 0x1
	$-16F    00974CD0     8BCF                     MOV ECX,EDI
	$-16D    00974CD2     FFD0                     CALL EAX
	$-16B    00974CD4     8B45 E8                  MOV EAX,DWORD PTR SS:[EBP-0x18]
	$-168    00974CD7     C645 FC 00               MOV BYTE PTR SS:[EBP-0x4],0x0
	$-164    00974CDB     85C0                     TEST EAX,EAX
	$-162    00974CDD     74 31                    JE SHORT MapleSto.00974D10
	$-160    00974CDF     83C0 04                  ADD EAX,0x4
	$-15D    00974CE2     50                       PUSH EAX
	$-15C    00974CE3     FF15 6C933E01            CALL DWORD PTR DS:[0x13E936C]
	$-156    00974CE9     85C0                     TEST EAX,EAX
	$-154    00974CEB     75 1C                    JNZ SHORT MapleSto.00974D09
	$-152    00974CED     8B4D E8                  MOV ECX,DWORD PTR SS:[EBP-0x18]
	$-14F    00974CF0     83C1 04                  ADD ECX,0x4
	$-14C    00974CF3     51                       PUSH ECX
	$-14B    00974CF4     FF15 F8923E01            CALL DWORD PTR DS:[0x13E92F8]
	$-145    00974CFA     8B4D E8                  MOV ECX,DWORD PTR SS:[EBP-0x18]
	$-142    00974CFD     85C9                     TEST ECX,ECX
	$-140    00974CFF     74 08                    JE SHORT MapleSto.00974D09
	$-13E    00974D01     8B11                     MOV EDX,DWORD PTR DS:[ECX]
	$-13C    00974D03     8B02                     MOV EAX,DWORD PTR DS:[EDX]
	$-13A    00974D05     6A 01                    PUSH 0x1
	$-138    00974D07     FFD0                     CALL EAX
	$-136    00974D09     C745 E8 00000000         MOV DWORD PTR SS:[EBP-0x18],0x0
	$-12F    00974D10     8B45 E0                  MOV EAX,DWORD PTR SS:[EBP-0x20]
	$-12C    00974D13     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-125    00974D1A     85C0                     TEST EAX,EAX
	$-123    00974D1C     74 2A                    JE SHORT MapleSto.00974D48
	$-121    00974D1E     83C0 04                  ADD EAX,0x4
	$-11E    00974D21     50                       PUSH EAX
	$-11D    00974D22     FF15 6C933E01            CALL DWORD PTR DS:[0x13E936C]
	$-117    00974D28     85C0                     TEST EAX,EAX
	$-115    00974D2A     75 1C                    JNZ SHORT MapleSto.00974D48
	$-113    00974D2C     8B4D E0                  MOV ECX,DWORD PTR SS:[EBP-0x20]
	$-110    00974D2F     83C1 04                  ADD ECX,0x4
	$-10D    00974D32     51                       PUSH ECX
	$-10C    00974D33     FF15 F8923E01            CALL DWORD PTR DS:[0x13E92F8]
	$-106    00974D39     8B4D E0                  MOV ECX,DWORD PTR SS:[EBP-0x20]
	$-103    00974D3C     85C9                     TEST ECX,ECX
	$-101    00974D3E     74 08                    JE SHORT MapleSto.00974D48
	$-FF     00974D40     8B11                     MOV EDX,DWORD PTR DS:[ECX]
	$-FD     00974D42     8B02                     MOV EAX,DWORD PTR DS:[EDX]
	$-FB     00974D44     6A 01                    PUSH 0x1
	$-F9     00974D46     FFD0                     CALL EAX
	$-F7     00974D48     8BC3                     MOV EAX,EBX
	$-F5     00974D4A     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-F2     00974D4D     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$-EB     00974D54     59                       POP ECX
	$-EA     00974D55     5F                       POP EDI
	$-E9     00974D56     5E                       POP ESI
	$-E8     00974D57     5B                       POP EBX
	$-E7     00974D58     8BE5                     MOV ESP,EBP
	$-E5     00974D5A     5D                       POP EBP
	$-E4     00974D5B     C3                       RETN
	$-E3     00974D5C     CC                       INT3
	$-E2     00974D5D     CC                       INT3
	$-E1     00974D5E     CC                       INT3
	$-E0     00974D5F     CC                       INT3
	$-DF     00974D60     55                       PUSH EBP
	$-DE     00974D61     8BEC                     MOV EBP,ESP
	$-DC     00974D63     6A FF                    PUSH -0x1
	$-DA     00974D65     68 7B8F3601              PUSH MapleSto.01368F7B
	$-D5     00974D6A     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$-CF     00974D70     50                       PUSH EAX
	$-CE     00974D71     83EC 1C                  SUB ESP,0x1C
	$-CB     00974D74     53                       PUSH EBX
	$-CA     00974D75     56                       PUSH ESI
	$-C9     00974D76     57                       PUSH EDI
	$-C8     00974D77     A1 D0687101              MOV EAX,DWORD PTR DS:[0x17168D0]
	$-C3     00974D7C     33C5                     XOR EAX,EBP
	$-C1     00974D7E     50                       PUSH EAX
	$-C0     00974D7F     8D45 F4                  LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-BD     00974D82     64:A3 00000000           MOV DWORD PTR FS:[0],EAX
	$-B7     00974D88     8BF1                     MOV ESI,ECX
	$-B5     00974D8A     E8 A6940000              CALL MapleSto.0097E235
	$-B0     00974D8F     833D 44357301 00         CMP DWORD PTR DS:[0x1733544],0x0
	$-A9     00974D96     74 0C                    JE SHORT MapleSto.00974DA4
	$-A7     00974D98     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$-A5     00974D9A     8B90 28020000            MOV EDX,DWORD PTR DS:[EAX+0x228]
	$-9F     00974DA0     8BCE                     MOV ECX,ESI
	$-9D     00974DA2     FFD2                     CALL EDX
	$-9B     00974DA4     F30F5A45 08              CVTSS2SD XMM0,DWORD PTR SS:[EBP+0x8]
	$-96     00974DA9     83EC 08                  SUB ESP,0x8
	$-93     00974DAC     8D8E 88120000            LEA ECX,DWORD PTR DS:[ESI+0x1288]
	$-8D     00974DB2     F20F110424               MOVSD QWORD PTR SS:[ESP],XMM0
	$-88     00974DB7     E8 D4C8E5FF              CALL MapleSto.007D1690
	$-83     00974DBC     D945 08                  FLD DWORD PTR SS:[EBP+0x8]
	$-80     00974DBF     51                       PUSH ECX
	$-7F     00974DC0     8BCE                     MOV ECX,ESI
	$-7D     00974DC2     D91C24                   FSTP DWORD PTR SS:[ESP]
	$-7A     00974DC5     E8 A6CD0000              CALL MapleSto.00981B70
	$-75     00974DCA     8B86 68120000            MOV EAX,DWORD PTR DS:[ESI+0x1268]
	$-6F     00974DD0     8B15 104A7201            MOV EDX,DWORD PTR DS:[0x1724A10]
	$-69     00974DD6     8B8E 6C120000            MOV ECX,DWORD PTR DS:[ESI+0x126C]
	$-63     00974DDC     3BD0                     CMP EDX,EAX
	$-61     00974DDE     75 0A                    JNZ SHORT MapleSto.00974DEA
	$-5F     00974DE0     8B15 144A7201            MOV EDX,DWORD PTR DS:[0x1724A14]
	$-59     00974DE6     3BD1                     CMP EDX,ECX
	$-57     00974DE8     74 19                    JE SHORT MapleSto.00974E03
	$-55     00974DEA     51                       PUSH ECX
	$-54     00974DEB     8B0D 804E7201            MOV ECX,DWORD PTR DS:[0x1724E80]
	$-4E     00974DF1     50                       PUSH EAX
	$-4D     00974DF2     E8 19A6FEFF              CALL MapleSto.0095F410
	$-48     00974DF7     85C0                     TEST EAX,EAX
	$-46     00974DF9     75 08                    JNZ SHORT MapleSto.00974E03
	$-44     00974DFB     8986 98170000            MOV DWORD PTR DS:[ESI+0x1798],EAX
	$-3E     00974E01     EB 11                    JMP SHORT MapleSto.00974E14
	$-3C     00974E03     8B0D 804E7201            MOV ECX,DWORD PTR DS:[0x1724E80]         ; g_a_party_mgr_
	$-36     00974E09     E8 D27EFEFF              CALL MapleSto.0095CCE0
	$-31     00974E0E     8986 98170000            MOV DWORD PTR DS:[ESI+0x1798],EAX
	$-2B     00974E14     6A 00                    PUSH 0x0
	$-29     00974E16     8BCE                     MOV ECX,ESI
	$-27     00974E18     E8 295F4A01              CALL MapleSto.01E1AD46
	$-22     00974E1D     8DBE C40F0000            LEA EDI,DWORD PTR DS:[ESI+0xFC4]
	$-1C     00974E23     8BCF                     MOV ECX,EDI
	$-1A     00974E25     E8 D4572F01              CALL MapleSto.01C6A5FE
	$-15     00974E2A     85C0                     TEST EAX,EAX
	$-13     00974E2C     75 11                    JNZ SHORT MapleSto.00974E3F
	$-11     00974E2E     8BCF                     MOV ECX,EDI
	$-F      00974E30     E8 C6492F01              CALL MapleSto.01C697FB
	$-A      00974E35     85C0                     TEST EAX,EAX
	$-8      00974E37     74 06                    JE SHORT MapleSto.00974E3F
	$-6      00974E39     C645 F0 00               MOV BYTE PTR SS:[EBP-0x10],0x0
	$-2      00974E3D     EB 10                    JMP SHORT MapleSto.00974E4F
	$ ==>    00974E3F     68 C8000000              PUSH 0xC8
	$+5      00974E44     8BCF                     MOV ECX,EDI
	$+7      00974E46     C645 F0 01               MOV BYTE PTR SS:[EBP-0x10],0x1
	$+B      00974E4A     E8 57D73001              CALL MapleSto.01C825A6
	$+10     00974E4F     8B4D F0                  MOV ECX,DWORD PTR SS:[EBP-0x10]
	$+13     00974E52     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$+15     00974E54     8B90 60010000            MOV EDX,DWORD PTR DS:[EAX+0x160]
	$+1B     00974E5A     51                       PUSH ECX
	$+1C     00974E5B     8BCE                     MOV ECX,ESI
	$+1E     00974E5D     FFD2                     CALL EDX
	$+20     00974E5F     8BCE                     MOV ECX,ESI
	$+22     00974E61     E8 71984A01              CALL MapleSto.01E1E6D7
	$+27     00974E66     8B8E 40120000            MOV ECX,DWORD PTR DS:[ESI+0x1240]
	$+2D     00974E6C     85C9                     TEST ECX,ECX
	$+2F     00974E6E     74 12                    JE SHORT MapleSto.00974E82
	$+31     00974E70     F30F5A45 08              CVTSS2SD XMM0,DWORD PTR SS:[EBP+0x8]
	$+36     00974E75     83EC 08                  SUB ESP,0x8
	$+39     00974E78     F20F110424               MOVSD QWORD PTR SS:[ESP],XMM0
	$+3E     00974E7D     E8 4E75C7FF              CALL MapleSto.005EC3D0
	$+43     00974E82     8BCE                     MOV ECX,ESI
	$+45     00974E84     E8 1D514A01              CALL MapleSto.01E19FA6
	$+4A     00974E89     8B8E 80170000            MOV ECX,DWORD PTR DS:[ESI+0x1780]
	$+50     00974E8F     8B1D F8923E01            MOV EBX,DWORD PTR DS:[0x13E92F8]
	$+56     00974E95     85C9                     TEST ECX,ECX
	$+58     00974E97     0F84 32010000            JE MapleSto.00974FCF
	$+5E     00974E9D     E8 3EA7C7FF              CALL MapleSto.005EF5E0
	$+63     00974EA2     83F8 03                  CMP EAX,0x3
	$+66     00974EA5     74 13                    JE SHORT MapleSto.00974EBA
	$+68     00974EA7     8BCE                     MOV ECX,ESI
	$+6A     00974EA9     E8 72370000              CALL MapleSto.00978620
	$+6F     00974EAE     83F8 0C                  CMP EAX,0xC
	$+72     00974EB1     74 07                    JE SHORT MapleSto.00974EBA
	$+74     00974EB3     BF 01000000              MOV EDI,0x1
	$+79     00974EB8     EB 02                    JMP SHORT MapleSto.00974EBC
	$+7B     00974EBA     33FF                     XOR EDI,EDI
	$+7D     00974EBC     8B8E 80170000            MOV ECX,DWORD PTR DS:[ESI+0x1780]
	$+83     00974EC2     E8 3B7B3401              CALL MapleSto.01CBCA02
	$+88     00974EC7     85C0                     TEST EAX,EAX
	$+8A     00974EC9     0F85 C0000000            JNZ MapleSto.00974F8F
	$+90     00974ECF     85FF                     TEST EDI,EDI
	$+92     00974ED1     0F85 B8000000            JNZ MapleSto.00974F8F
	$+98     00974ED7     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$+9A     00974ED9     8B50 48                  MOV EDX,DWORD PTR DS:[EAX+0x48]
	$+9D     00974EDC     8D4D D8                  LEA ECX,DWORD PTR SS:[EBP-0x28]
	$+A0     00974EDF     51                       PUSH ECX
	$+A1     00974EE0     8BCE                     MOV ECX,ESI
	$+A3     00974EE2     FFD2                     CALL EDX
	$+A5     00974EE4     B9 8C797401              MOV ECX,MapleSto.0174798C
	$+AA     00974EE9     E8 52CEA9FF              CALL MapleSto.00411D40
	$+AF     00974EEE     8BF8                     MOV EDI,EAX
	$+B1     00974EF0     897D F0                  MOV DWORD PTR SS:[EBP-0x10],EDI
	$+B4     00974EF3     85FF                     TEST EDI,EDI
	$+B6     00974EF5     74 09                    JE SHORT MapleSto.00974F00
	$+B8     00974EF7     8B07                     MOV EAX,DWORD PTR DS:[EDI]
	$+BA     00974EF9     8B50 04                  MOV EDX,DWORD PTR DS:[EAX+0x4]
	$+BD     00974EFC     8BCF                     MOV ECX,EDI
	$+BF     00974EFE     FFD2                     CALL EDX
	$+C1     00974F00     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$+C8     00974F07     85FF                     TEST EDI,EDI
	$+CA     00974F09     74 3C                    JE SHORT MapleSto.00974F47
	$+CC     00974F0B     8B4D E0                  MOV ECX,DWORD PTR SS:[EBP-0x20]
	$+CF     00974F0E     F30F7E45 D8              MOVQ XMM0,QWORD PTR SS:[EBP-0x28]
	$+D4     00974F13     83EC 0C                  SUB ESP,0xC
	$+D7     00974F16     8BC4                     MOV EAX,ESP
	$+D9     00974F18     660FD600                 MOVQ QWORD PTR DS:[EAX],XMM0
	$+DD     00974F1C     8D55 E4                  LEA EDX,DWORD PTR SS:[EBP-0x1C]
	$+E0     00974F1F     8948 08                  MOV DWORD PTR DS:[EAX+0x8],ECX
	$+E3     00974F22     52                       PUSH EDX
	$+E4     00974F23     8BCF                     MOV ECX,EDI
	$+E6     00974F25     E8 C641E1FF              CALL MapleSto.007890F0
	$+EB     00974F2A     F3:0F1000                MOVSS XMM0,DWORD PTR DS:[EAX]
	$+EF     00974F2E     F3:0F1145 D8             MOVSS DWORD PTR SS:[EBP-0x28],XMM0
	$+F4     00974F33     F3:0F1040 04             MOVSS XMM0,DWORD PTR DS:[EAX+0x4]
	$+F9     00974F38     F3:0F1145 DC             MOVSS DWORD PTR SS:[EBP-0x24],XMM0
	$+FE     00974F3D     F3:0F1040 08             MOVSS XMM0,DWORD PTR DS:[EAX+0x8]
	$+103    00974F42     F3:0F1145 E0             MOVSS DWORD PTR SS:[EBP-0x20],XMM0
	$+108    00974F47     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+10F    00974F4E     85FF                     TEST EDI,EDI
	$+111    00974F50     74 09                    JE SHORT MapleSto.00974F5B
	$+113    00974F52     8B07                     MOV EAX,DWORD PTR DS:[EDI]
	$+115    00974F54     8B50 08                  MOV EDX,DWORD PTR DS:[EAX+0x8]
	$+118    00974F57     8BCF                     MOV ECX,EDI
	$+11A    00974F59     FFD2                     CALL EDX
	$+11C    00974F5B     8B4D E0                  MOV ECX,DWORD PTR SS:[EBP-0x20]
	$+11F    00974F5E     F30F7E45 D8              MOVQ XMM0,QWORD PTR SS:[EBP-0x28]
	$+124    00974F63     83EC 0C                  SUB ESP,0xC
	$+127    00974F66     8BC4                     MOV EAX,ESP
	$+129    00974F68     660FD600                 MOVQ QWORD PTR DS:[EAX],XMM0
	$+12D    00974F6C     8948 08                  MOV DWORD PTR DS:[EAX+0x8],ECX
	$+130    00974F6F     8B8E 80170000            MOV ECX,DWORD PTR DS:[ESI+0x1780]
	$+136    00974F75     E8 36B0C7FF              CALL MapleSto.005EFFB0
	$+13B    00974F7A     8BBE 80170000            MOV EDI,DWORD PTR DS:[ESI+0x1780]
	$+141    00974F80     E8 2B70BFFF              CALL MapleSto.0056BFB0                   ; JMP to winmm.timeGetTime
	$+146    00974F85     50                       PUSH EAX
	$+147    00974F86     8BCF                     MOV ECX,EDI
	$+149    00974F88     E8 73B1C7FF              CALL MapleSto.005F0100
	$+14E    00974F8D     EB 40                    JMP SHORT MapleSto.00974FCF
	$+150    00974F8F     8B86 80170000            MOV EAX,DWORD PTR DS:[ESI+0x1780]
	$+156    00974F95     85C0                     TEST EAX,EAX
	$+158    00974F97     74 36                    JE SHORT MapleSto.00974FCF
	$+15A    00974F99     83C0 04                  ADD EAX,0x4
	$+15D    00974F9C     50                       PUSH EAX
	$+15E    00974F9D     FF15 6C933E01            CALL DWORD PTR DS:[0x13E936C]
	$+164    00974FA3     85C0                     TEST EAX,EAX
	$+166    00974FA5     75 1E                    JNZ SHORT MapleSto.00974FC5
	$+168    00974FA7     8B96 80170000            MOV EDX,DWORD PTR DS:[ESI+0x1780]
	$+16E    00974FAD     83C2 04                  ADD EDX,0x4
	$+171    00974FB0     52                       PUSH EDX
	$+172    00974FB1     FFD3                     CALL EBX
	$+174    00974FB3     8B8E 80170000            MOV ECX,DWORD PTR DS:[ESI+0x1780]
	$+17A    00974FB9     85C9                     TEST ECX,ECX
	$+17C    00974FBB     74 08                    JE SHORT MapleSto.00974FC5
	$+17E    00974FBD     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+180    00974FBF     8B10                     MOV EDX,DWORD PTR DS:[EAX]
	$+182    00974FC1     6A 01                    PUSH 0x1
	$+184    00974FC3     FFD2                     CALL EDX
	$+186    00974FC5     C786 80170000 00000000   MOV DWORD PTR DS:[ESI+0x1780],0x0
	$+190    00974FCF     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$+192    00974FD1     8B90 38010000            MOV EDX,DWORD PTR DS:[EAX+0x138]
	$+198    00974FD7     8D4D E8                  LEA ECX,DWORD PTR SS:[EBP-0x18]
	$+19B    00974FDA     51                       PUSH ECX
	$+19C    00974FDB     8BCE                     MOV ECX,ESI
	$+19E    00974FDD     FFD2                     CALL EDX
	$+1A0    00974FDF     8B78 04                  MOV EDI,DWORD PTR DS:[EAX+0x4]
	$+1A3    00974FE2     8B45 EC                  MOV EAX,DWORD PTR SS:[EBP-0x14]
	$+1A6    00974FE5     897D F0                  MOV DWORD PTR SS:[EBP-0x10],EDI
	$+1A9    00974FE8     85C0                     TEST EAX,EAX
	$+1AB    00974FEA     74 2D                    JE SHORT MapleSto.00975019
	$+1AD    00974FEC     83C0 04                  ADD EAX,0x4
	$+1B0    00974FEF     50                       PUSH EAX
	$+1B1    00974FF0     FF15 6C933E01            CALL DWORD PTR DS:[0x13E936C]
	$+1B7    00974FF6     85C0                     TEST EAX,EAX
	$+1B9    00974FF8     75 18                    JNZ SHORT MapleSto.00975012
	$+1BB    00974FFA     8B45 EC                  MOV EAX,DWORD PTR SS:[EBP-0x14]
	$+1BE    00974FFD     83C0 04                  ADD EAX,0x4
	$+1C1    00975000     50                       PUSH EAX
	$+1C2    00975001     FFD3                     CALL EBX
	$+1C4    00975003     8B4D EC                  MOV ECX,DWORD PTR SS:[EBP-0x14]
	$+1C7    00975006     85C9                     TEST ECX,ECX
	$+1C9    00975008     74 08                    JE SHORT MapleSto.00975012
	$+1CB    0097500A     8B11                     MOV EDX,DWORD PTR DS:[ECX]
	$+1CD    0097500C     8B02                     MOV EAX,DWORD PTR DS:[EDX]
	$+1CF    0097500E     6A 01                    PUSH 0x1
	$+1D1    00975010     FFD0                     CALL EAX
	$+1D3    00975012     C745 EC 00000000         MOV DWORD PTR SS:[EBP-0x14],0x0
	$+1DA    00975019     85FF                     TEST EDI,EDI
	$+1DC    0097501B     0F84 B1000000            JE MapleSto.009750D2
	$+1E2    00975021     8B0D 2C417201            MOV ECX,DWORD PTR DS:[0x172412C]
	$+1E8    00975027     E8 A7E95201              CALL MapleSto.01EA39D3
	$+1ED    0097502C     8BD8                     MOV EBX,EAX
	$+1EF    0097502E     85DB                     TEST EBX,EBX
	$+1F1    00975030     0F84 9C000000            JE MapleSto.009750D2
	$+1F7    00975036     8B3D 40277201            MOV EDI,DWORD PTR DS:[0x1722740]
	$+1FD    0097503C     81C7 A4620000            ADD EDI,0x62A4
	$+203    00975042     807F 04 00               CMP BYTE PTR DS:[EDI+0x4],0x0
	$+207    00975046     75 0C                    JNZ SHORT MapleSto.00975054
	$+209    00975048     68 88463F01              PUSH MapleSto.013F4688                   ; ASCII "constants is not set"
	$+20E    0097504D     8BCF                     MOV ECX,EDI
	$+210    0097504F     E8 FC93ADFF              CALL MapleSto.0044E450
	$+215    00975054     8B7F 40                  MOV EDI,DWORD PTR DS:[EDI+0x40]
	$+218    00975057     3BF3                     CMP ESI,EBX
	$+21A    00975059     75 0E                    JNZ SHORT MapleSto.00975069
	$+21C    0097505B     8B3D 40277201            MOV EDI,DWORD PTR DS:[0x1722740]
	$+222    00975061     81C7 5C620000            ADD EDI,0x625C
	$+228    00975067     EB 1D                    JMP SHORT MapleSto.00975086
	$+22A    00975069     8B13                     MOV EDX,DWORD PTR DS:[EBX]
	$+22C    0097506B     8B82 DC010000            MOV EAX,DWORD PTR DS:[EDX+0x1DC]
	$+232    00975071     56                       PUSH ESI
	$+233    00975072     8BCB                     MOV ECX,EBX
	$+235    00975074     FFD0                     CALL EAX
	$+237    00975076     85C0                     TEST EAX,EAX
	$+239    00975078     74 23                    JE SHORT MapleSto.0097509D
	$+23B    0097507A     8B3D 40277201            MOV EDI,DWORD PTR DS:[0x1722740]
	$+241    00975080     81C7 EC620000            ADD EDI,0x62EC
	$+247    00975086     807F 04 00               CMP BYTE PTR DS:[EDI+0x4],0x0
	$+24B    0097508A     75 0C                    JNZ SHORT MapleSto.00975098
	$+24D    0097508C     68 88463F01              PUSH MapleSto.013F4688                   ; ASCII "constants is not set"
	$+252    00975091     8BCF                     MOV ECX,EDI
	$+254    00975093     E8 B893ADFF              CALL MapleSto.0044E450
	$+259    00975098     8B7F 40                  MOV EDI,DWORD PTR DS:[EDI+0x40]
	$+25C    0097509B     EB 2C                    JMP SHORT MapleSto.009750C9
	$+25E    0097509D     8B8E 6C120000            MOV ECX,DWORD PTR DS:[ESI+0x126C]
	$+264    009750A3     8B86 68120000            MOV EAX,DWORD PTR DS:[ESI+0x1268]
	$+26A    009750A9     51                       PUSH ECX
	$+26B    009750AA     50                       PUSH EAX
	$+26C    009750AB     8BCB                     MOV ECX,EBX
	$+26E    009750AD     E8 7EA4FBFF              CALL MapleSto.0092F530
	$+273    009750B2     85C0                     TEST EAX,EAX
	$+275    009750B4     74 13                    JE SHORT MapleSto.009750C9
	$+277    009750B6     8B0D 40277201            MOV ECX,DWORD PTR DS:[0x1722740]
	$+27D    009750BC     81C1 34630000            ADD ECX,0x6334
	$+283    009750C2     E8 D9BDBAFF              CALL MapleSto.00520EA0
	$+288    009750C7     8BF8                     MOV EDI,EAX
	$+28A    009750C9     8B4D F0                  MOV ECX,DWORD PTR SS:[EBP-0x10]
	$+28D    009750CC     57                       PUSH EDI
	$+28E    009750CD     E8 CE2A1800              CALL MapleSto.00AF7BA0
	$+293    009750D2     D945 08                  FLD DWORD PTR SS:[EBP+0x8]
	$+296    009750D5     51                       PUSH ECX
	$+297    009750D6     8BCE                     MOV ECX,ESI
	$+299    009750D8     D91C24                   FSTP DWORD PTR SS:[ESP]
	$+29C    009750DB     E8 80C0FFFF              CALL MapleSto.00971160
	$+2A1    009750E0     83BE A8170000 00         CMP DWORD PTR DS:[ESI+0x17A8],0x0
	$+2A8    009750E7     74 0F                    JE SHORT MapleSto.009750F8
	$+2AA    009750E9     8B8E BC170000            MOV ECX,DWORD PTR DS:[ESI+0x17BC]
	$+2B0    009750EF     85C9                     TEST ECX,ECX
	$+2B2    009750F1     74 05                    JE SHORT MapleSto.009750F8
	$+2B4    009750F3     E8 5802E0FF              CALL MapleSto.00775350
	$+2B9    009750F8     8BCE                     MOV ECX,ESI
	$+2BB    009750FA     E8 21C1FFFF              CALL MapleSto.00971220
	$+2C0    009750FF     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+2C3    00975102     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+2CA    00975109     59                       POP ECX
	$+2CB    0097510A     5F                       POP EDI
	$+2CC    0097510B     5E                       POP ESI
	$+2CD    0097510C     5B                       POP EBX
	$+2CE    0097510D     8BE5                     MOV ESP,EBP
	$+2D0    0097510F     5D                       POP EBP
	$+2D1    00975110     C2 0400                  RETN 0x4
	$+2D4    00975113     CC                       INT3
	*/
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("68C80000008BCFC645F001", -0x3c + 2);
	if (!op_dw_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *op_dw_tmp;
	CheckExeAddr(dw_tmp);
	ofs << "pt_cdword	g_a_party_mgr_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;
	return true;
}

bool CAutoUpdateApp::Scan_Offset(Cmn_FC_OFstream& ofs)
{
	return true;
}

bool CAutoUpdateApp::Scan_Call(Cmn_FC_OFstream& ofs)
{
	pt_pcvoid addr_tmp;
	//pt_dword dw_tmp;
	//boost::optional<pt_dword> op_dw_tmp;

	/*
	$-156    00A74E3F     CC                        INT3
	$-155    00A74E40     55                        PUSH EBP
	$-154    00A74E41     8BEC                      MOV EBP,ESP
	$-152    00A74E43     56                        PUSH ESI
	$-151    00A74E44     57                        PUSH EDI
	$-150    00A74E45     8B7D 08                   MOV EDI,DWORD PTR SS:[EBP+0x8]
	$-14D    00A74E48     57                        PUSH EDI
	$-14C    00A74E49     8BF1                      MOV ESI,ECX
	$-14A    00A74E4B     E8 00450100               CALL MapleSto.00A89350
	$-145    00A74E50     8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$-143    00A74E52     8B90 D0000000             MOV EDX,DWORD PTR DS:[EAX+0xD0]
	$-13D    00A74E58     8BCE                      MOV ECX,ESI
	$-13B    00A74E5A     FFD2                      CALL EDX
	$-139    00A74E5C     85C0                      TEST EAX,EAX
	$-137    00A74E5E     75 44                     JNZ SHORT MapleSto.00A74EA4
	$-135    00A74E60     8BCF                      MOV ECX,EDI
	$-133    00A74E62     E8 CEFDAAFF               CALL MapleSto.00524C35
	$-12E    00A74E67     83F8 07                   CMP EAX,0x7
	$-12B    00A74E6A     74 13                     JE SHORT MapleSto.00A74E7F
	$-129    00A74E6C     83F8 3B                   CMP EAX,0x3B
	$-126    00A74E6F     75 33                     JNZ SHORT MapleSto.00A74EA4
	$-124    00A74E71     57                        PUSH EDI
	$-123    00A74E72     8BCE                      MOV ECX,ESI
	$-121    00A74E74     E8 17FDFFFF               CALL MapleSto.00A74B90
	$-11C    00A74E79     5F                        POP EDI
	$-11B    00A74E7A     5E                        POP ESI
	$-11A    00A74E7B     5D                        POP EBP
	$-119    00A74E7C     C2 0400                   RETN 0x4
	$-116    00A74E7F     8B47 04                   MOV EAX,DWORD PTR DS:[EDI+0x4]
	$-113    00A74E82     8B50 04                   MOV EDX,DWORD PTR DS:[EAX+0x4]
	$-110    00A74E85     8D4F 04                   LEA ECX,DWORD PTR DS:[EDI+0x4]
	$-10D    00A74E88     FFD2                      CALL EDX
	$-10B    00A74E8A     3C 01                     CMP AL,0x1
	$-109    00A74E8C     75 16                     JNZ SHORT MapleSto.00A74EA4
	$-107    00A74E8E     E8 70183801               CALL MapleSto.01DF6703
	$-102    00A74E93     8BCE                      MOV ECX,ESI
	$-100    00A74E95     C786 80010000 00000000    MOV DWORD PTR DS:[ESI+0x180],0x0
	$-F6     00A74E9F     E8 3CFAFFFF               CALL MapleSto.00A748E0
	$-F1     00A74EA4     5F                        POP EDI
	$-F0     00A74EA5     5E                        POP ESI
	$-EF     00A74EA6     5D                        POP EBP
	$-EE     00A74EA7     C2 0400                   RETN 0x4
	$-EB     00A74EAA     CC                        INT3
	$-EA     00A74EAB     CC                        INT3
	$-E9     00A74EAC     CC                        INT3
	$-E8     00A74EAD     CC                        INT3
	$-E7     00A74EAE     CC                        INT3
	$-E6     00A74EAF     CC                        INT3
	$-E5     00A74EB0     55                        PUSH EBP
	$-E4     00A74EB1     8BEC                      MOV EBP,ESP
	$-E2     00A74EB3     6A FF                     PUSH -0x1
	$-E0     00A74EB5     68 D7193801               PUSH MapleSto.013819D7
	$-DB     00A74EBA     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$-D5     00A74EC0     50                        PUSH EAX
	$-D4     00A74EC1     83EC 08                   SUB ESP,0x8
	$-D1     00A74EC4     56                        PUSH ESI
	$-D0     00A74EC5     A1 D0687101               MOV EAX,DWORD PTR DS:[0x17168D0]
	$-CB     00A74ECA     33C5                      XOR EAX,EBP
	$-C9     00A74ECC     50                        PUSH EAX
	$-C8     00A74ECD     8D45 F4                   LEA EAX,DWORD PTR SS:[EBP-0xC]
	$-C5     00A74ED0     64:A3 00000000            MOV DWORD PTR FS:[0],EAX
	$-BF     00A74ED6     8BF1                      MOV ESI,ECX
	$-BD     00A74ED8     8B45 10                   MOV EAX,DWORD PTR SS:[EBP+0x10]
	$-BA     00A74EDB     50                        PUSH EAX
	$-B9     00A74EDC     8D4D EC                   LEA ECX,DWORD PTR SS:[EBP-0x14]
	$-B6     00A74EDF     51                        PUSH ECX
	$-B5     00A74EE0     E8 BB581901               CALL MapleSto.01C0A7A0
	$-B0     00A74EE5     83C4 08                   ADD ESP,0x8
	$-AD     00A74EE8     8B45 F0                   MOV EAX,DWORD PTR SS:[EBP-0x10]
	$-AA     00A74EEB     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$-A3     00A74EF2     85C0                      TEST EAX,EAX
	$-A1     00A74EF4     75 12                     JNZ SHORT MapleSto.00A74F08
	$-9F     00A74EF6     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-9C     00A74EF9     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$-95     00A74F00     59                        POP ECX
	$-94     00A74F01     5E                        POP ESI
	$-93     00A74F02     8BE5                      MOV ESP,EBP
	$-91     00A74F04     5D                        POP EBP
	$-90     00A74F05     C2 0C00                   RETN 0xC
	$-8D     00A74F08     8B55 0C                   MOV EDX,DWORD PTR SS:[EBP+0xC]
	$-8A     00A74F0B     05 E8000000               ADD EAX,0xE8
	$-85     00A74F10     50                        PUSH EAX
	$-84     00A74F11     8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
	$-81     00A74F14     52                        PUSH EDX
	$-80     00A74F15     50                        PUSH EAX
	$-7F     00A74F16     8BCE                      MOV ECX,ESI
	$-7D     00A74F18     E8 53F6FFFF               CALL MapleSto.00A74570
	$-78     00A74F1D     8BF0                      MOV ESI,EAX
	$-76     00A74F1F     8B45 F0                   MOV EAX,DWORD PTR SS:[EBP-0x10]
	$-73     00A74F22     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-6C     00A74F29     85C0                      TEST EAX,EAX
	$-6A     00A74F2B     74 2A                     JE SHORT MapleSto.00A74F57
	$-68     00A74F2D     83C0 04                   ADD EAX,0x4
	$-65     00A74F30     50                        PUSH EAX
	$-64     00A74F31     FF15 6C933E01             CALL DWORD PTR DS:[0x13E936C]
	$-5E     00A74F37     85C0                      TEST EAX,EAX
	$-5C     00A74F39     75 1C                     JNZ SHORT MapleSto.00A74F57
	$-5A     00A74F3B     8B4D F0                   MOV ECX,DWORD PTR SS:[EBP-0x10]
	$-57     00A74F3E     83C1 04                   ADD ECX,0x4
	$-54     00A74F41     51                        PUSH ECX
	$-53     00A74F42     FF15 F8923E01             CALL DWORD PTR DS:[0x13E92F8]
	$-4D     00A74F48     8B4D F0                   MOV ECX,DWORD PTR SS:[EBP-0x10]
	$-4A     00A74F4B     85C9                      TEST ECX,ECX
	$-48     00A74F4D     74 08                     JE SHORT MapleSto.00A74F57
	$-46     00A74F4F     8B11                      MOV EDX,DWORD PTR DS:[ECX]
	$-44     00A74F51     8B02                      MOV EAX,DWORD PTR DS:[EDX]
	$-42     00A74F53     6A 01                     PUSH 0x1
	$-40     00A74F55     FFD0                      CALL EAX
	$-3E     00A74F57     8BC6                      MOV EAX,ESI
	$-3C     00A74F59     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-39     00A74F5C     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$-32     00A74F63     59                        POP ECX
	$-31     00A74F64     5E                        POP ESI
	$-30     00A74F65     8BE5                      MOV ESP,EBP
	$-2E     00A74F67     5D                        POP EBP
	$-2D     00A74F68     C2 0C00                   RETN 0xC
	$-2A     00A74F6B     CC                        INT3
	$-29     00A74F6C     CC                        INT3
	$-28     00A74F6D     CC                        INT3
	$-27     00A74F6E     CC                        INT3
	$-26     00A74F6F     CC                        INT3
	$-25     00A74F70     55                        PUSH EBP								;g_c_create_role_by_name_
	$-24     00A74F71     8BEC                      MOV EBP,ESP
	$-22     00A74F73     6A FF                     PUSH -0x1
	$-20     00A74F75     68 2A1A3801               PUSH MapleSto.01381A2A
	$-1B     00A74F7A     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$-15     00A74F80     50                        PUSH EAX
	$-14     00A74F81     81EC 88000000             SUB ESP,0x88
	$-E      00A74F87     53                        PUSH EBX
	$-D      00A74F88     56                        PUSH ESI
	$-C      00A74F89     57                        PUSH EDI
	$-B      00A74F8A     A1 D0687101               MOV EAX,DWORD PTR DS:[0x17168D0]
	$-6      00A74F8F     33C5                      XOR EAX,EBP
	$-4      00A74F91     50                        PUSH EAX
	$-3      00A74F92     8D45 F4                   LEA EAX,DWORD PTR SS:[EBP-0xC]
	$ ==>    00A74F95     64:A3 00000000            MOV DWORD PTR FS:[0],EAX
	$+6      00A74F9B     894D F0                   MOV DWORD PTR SS:[EBP-0x10],ECX
	$+9      00A74F9E     8B0D 2C417201             MOV ECX,DWORD PTR DS:[0x172412C]
	$+F      00A74FA4     E8 2AEA4201               CALL MapleSto.01EA39D3
	$+14     00A74FA9     8BF0                      MOV ESI,EAX
	$+16     00A74FAB     33FF                      XOR EDI,EDI
	$+18     00A74FAD     3BF7                      CMP ESI,EDI
	$+1A     00A74FAF     75 16                     JNZ SHORT MapleSto.00A74FC7
	$+1C     00A74FB1     33C0                      XOR EAX,EAX
	$+1E     00A74FB3     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+21     00A74FB6     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+28     00A74FBD     59                        POP ECX
	$+29     00A74FBE     5F                        POP EDI
	$+2A     00A74FBF     5E                        POP ESI
	$+2B     00A74FC0     5B                        POP EBX
	$+2C     00A74FC1     8BE5                      MOV ESP,EBP
	$+2E     00A74FC3     5D                        POP EBP
	$+2F     00A74FC4     C2 0400                   RETN 0x4
	$+32     00A74FC7     8D8D 74FFFFFF             LEA ECX,DWORD PTR SS:[EBP-0x8C]
	$+38     00A74FCD     E8 9E2B0000               CALL MapleSto.00A77B70
	$+3D     00A74FD2     8BCE                      MOV ECX,ESI
	$+3F     00A74FD4     897D FC                   MOV DWORD PTR SS:[EBP-0x4],EDI
	$+42     00A74FD7     E8 6C903901               CALL MapleSto.01E0E048
	$+47     00A74FDC     8BCE                      MOV ECX,ESI
	$+49     00A74FDE     8885 74FFFFFF             MOV BYTE PTR SS:[EBP-0x8C],AL
	$+4F     00A74FE4     E8 7A1D3901               CALL MapleSto.01E06D63
	$+54     00A74FE9     8BC8                      MOV ECX,EAX
	$+56     00A74FEB     E8 C032A8FF               CALL MapleSto.004F82B0
	$+5B     00A74FF0     8985 78FFFFFF             MOV DWORD PTR SS:[EBP-0x88],EAX
	$+61     00A74FF6     8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+64     00A74FF9     50                        PUSH EAX
	$+65     00A74FFA     8D8D 7CFFFFFF             LEA ECX,DWORD PTR SS:[EBP-0x84]
	$+6B     00A75000     FF15 CCA13E01             CALL DWORD PTR DS:[0x13EA1CC]                     ; mfc90.#810
	$+71     00A75006     57                        PUSH EDI
	$+72     00A75007     8BCE                      MOV ECX,ESI
	$+74     00A75009     E8 524BEFFF               CALL MapleSto.00969B60
	$+79     00A7500E     50                        PUSH EAX
	$+7A     00A7500F     E8 9C1A0F00               CALL MapleSto.00B66AB0
	$+7F     00A75014     83C4 04                   ADD ESP,0x4
	$+82     00A75017     6A 01                     PUSH 0x1
	$+84     00A75019     8BCE                      MOV ECX,ESI
	$+86     00A7501B     8945 80                   MOV DWORD PTR SS:[EBP-0x80],EAX
	$+89     00A7501E     E8 3D4BEFFF               CALL MapleSto.00969B60
	$+8E     00A75023     50                        PUSH EAX
	$+8F     00A75024     E8 871A0F00               CALL MapleSto.00B66AB0
	$+94     00A75029     83C4 04                   ADD ESP,0x4
	$+97     00A7502C     8945 84                   MOV DWORD PTR SS:[EBP-0x7C],EAX
	$+9A     00A7502F     897D D0                   MOV DWORD PTR SS:[EBP-0x30],EDI
	$+9D     00A75032     897D D4                   MOV DWORD PTR SS:[EBP-0x2C],EDI
	$+A0     00A75035     897D D8                   MOV DWORD PTR SS:[EBP-0x28],EDI
	$+A3     00A75038     897D DC                   MOV DWORD PTR SS:[EBP-0x24],EDI
	$+A6     00A7503B     897D E0                   MOV DWORD PTR SS:[EBP-0x20],EDI
	$+A9     00A7503E     C745 E4 0A000000          MOV DWORD PTR SS:[EBP-0x1C],0xA
	$+B0     00A75045     8D4D D0                   LEA ECX,DWORD PTR SS:[EBP-0x30]
	$+B3     00A75048     51                        PUSH ECX
	$+B4     00A75049     8BCE                      MOV ECX,ESI
	$+B6     00A7504B     C645 FC 01                MOV BYTE PTR SS:[EBP-0x4],0x1
	$+BA     00A7504F     E8 CE373901               CALL MapleSto.01E08822
	$+BF     00A75054     8BC8                      MOV ECX,EAX
	$+C1     00A75056     E8 556DD5FF               CALL MapleSto.007CBDB0
	$+C6     00A7505B     8B55 D8                   MOV EDX,DWORD PTR SS:[EBP-0x28]
	$+C9     00A7505E     52                        PUSH EDX
	$+CA     00A7505F     8D4D 90                   LEA ECX,DWORD PTR SS:[EBP-0x70]
	$+CD     00A75062     E8 F2BD1F01               CALL MapleSto.01C70E59
	$+D2     00A75067     8B5D D0                   MOV EBX,DWORD PTR SS:[EBP-0x30]
	$+D5     00A7506A     3BDF                      CMP EBX,EDI
	$+D7     00A7506C     0F84 C0000000             JE MapleSto.00A75132
	$+DD     00A75072     33C0                      XOR EAX,EAX
	$+DF     00A75074     3BDF                      CMP EBX,EDI
	$+E1     00A75076     0F95C0                    SETNE AL
	$+E4     00A75079     8945 E8                   MOV DWORD PTR SS:[EBP-0x18],EAX
	$+E7     00A7507C     3BC7                      CMP EAX,EDI
	$+E9     00A7507E     0F84 E4000000             JE MapleSto.00A75168
	$+EF     00A75084     8B4B 0C                   MOV ECX,DWORD PTR DS:[EBX+0xC]
	$+F2     00A75087     E8 D420D5FF               CALL MapleSto.007C7160
	$+F7     00A7508C     8B70 04                   MOV ESI,DWORD PTR DS:[EAX+0x4]
	$+FA     00A7508F     89B5 70FFFFFF             MOV DWORD PTR SS:[EBP-0x90],ESI
	$+100    00A75095     3BF7                      CMP ESI,EDI
	$+102    00A75097     74 0A                     JE SHORT MapleSto.00A750A3
	$+104    00A75099     8D46 04                   LEA EAX,DWORD PTR DS:[ESI+0x4]
	$+107    00A7509C     50                        PUSH EAX
	$+108    00A7509D     FF15 F8923E01             CALL DWORD PTR DS:[0x13E92F8]
	$+10E    00A750A3     8BCE                      MOV ECX,ESI
	$+110    00A750A5     C645 FC 02                MOV BYTE PTR SS:[EBP-0x4],0x2
	$+114    00A750A9     E8 F6241901               CALL MapleSto.01C075A4
	$+119    00A750AE     50                        PUSH EAX
	$+11A    00A750AF     8D4D 90                   LEA ECX,DWORD PTR SS:[EBP-0x70]
	$+11D    00A750B2     E8 4824AFFF               CALL MapleSto.005674FF
	$+122    00A750B7     8D4D EC                   LEA ECX,DWORD PTR SS:[EBP-0x14]
	$+125    00A750BA     51                        PUSH ECX
	$+126    00A750BB     8BCE                      MOV ECX,ESI
	$+128    00A750BD     E8 3E60A4FF               CALL MapleSto.004BB100
	$+12D    00A750C2     50                        PUSH EAX
	$+12E    00A750C3     8D4D 90                   LEA ECX,DWORD PTR SS:[EBP-0x70]
	$+131    00A750C6     C645 FC 03                MOV BYTE PTR SS:[EBP-0x4],0x3
	$+135    00A750CA     E8 D19FAEFF               CALL MapleSto.0055F0A0
	$+13A    00A750CF     8D4D EC                   LEA ECX,DWORD PTR SS:[EBP-0x14]
	$+13D    00A750D2     C645 FC 02                MOV BYTE PTR SS:[EBP-0x4],0x2
	$+141    00A750D6     FF15 08A23E01             CALL DWORD PTR DS:[0x13EA208]                     ; mfc90.#601
	$+147    00A750DC     8BCE                      MOV ECX,ESI
	$+149    00A750DE     E8 D5111901               CALL MapleSto.01C062B8
	$+14E    00A750E3     8B10                      MOV EDX,DWORD PTR DS:[EAX]
	$+150    00A750E5     8B52 14                   MOV EDX,DWORD PTR DS:[EDX+0x14]
	$+153    00A750E8     8D4D 90                   LEA ECX,DWORD PTR SS:[EBP-0x70]
	$+156    00A750EB     51                        PUSH ECX
	$+157    00A750EC     8BC8                      MOV ECX,EAX
	$+159    00A750EE     FFD2                      CALL EDX
	$+15B    00A750F0     C645 FC 01                MOV BYTE PTR SS:[EBP-0x4],0x1
	$+15F    00A750F4     3BF7                      CMP ESI,EDI
	$+161    00A750F6     74 2B                     JE SHORT MapleSto.00A75123
	$+163    00A750F8     8D7E 04                   LEA EDI,DWORD PTR DS:[ESI+0x4]
	$+166    00A750FB     57                        PUSH EDI
	$+167    00A750FC     FF15 6C933E01             CALL DWORD PTR DS:[0x13E936C]
	$+16D    00A75102     85C0                      TEST EAX,EAX
	$+16F    00A75104     75 11                     JNZ SHORT MapleSto.00A75117
	$+171    00A75106     57                        PUSH EDI
	$+172    00A75107     FF15 F8923E01             CALL DWORD PTR DS:[0x13E92F8]
	$+178    00A7510D     8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$+17A    00A7510F     8B10                      MOV EDX,DWORD PTR DS:[EAX]
	$+17C    00A75111     6A 01                     PUSH 0x1
	$+17E    00A75113     8BCE                      MOV ECX,ESI
	$+180    00A75115     FFD2                      CALL EDX
	$+182    00A75117     C785 70FFFFFF 00000000    MOV DWORD PTR SS:[EBP-0x90],0x0
	$+18C    00A75121     33FF                      XOR EDI,EDI
	$+18E    00A75123     397D E8                   CMP DWORD PTR SS:[EBP-0x18],EDI
	$+191    00A75126     74 40                     JE SHORT MapleSto.00A75168
	$+193    00A75128     8B1B                      MOV EBX,DWORD PTR DS:[EBX]
	$+195    00A7512A     3BDF                      CMP EBX,EDI
	$+197    00A7512C   ^ 0F85 40FFFFFF             JNZ MapleSto.00A75072
	$+19D    00A75132     8B45 F0                   MOV EAX,DWORD PTR SS:[EBP-0x10]
	$+1A0    00A75135     F20F2C88 A8010000         CVTTSD2SI ECX,QWORD PTR DS:[EAX+0x1A8]
	$+1A8    00A7513D     57                        PUSH EDI
	$+1A9    00A7513E     894D AC                   MOV DWORD PTR SS:[EBP-0x54],ECX
	$+1AC    00A75141     6A 04                     PUSH 0x4
	$+1AE    00A75143     8D4D B0                   LEA ECX,DWORD PTR SS:[EBP-0x50]
	$+1B1    00A75146     E8 C560ABFF               CALL MapleSto.0052B210                            ; 构造OutPacket
	$+1B6    00A7514B     6A 01                     PUSH 0x1
	$+1B8    00A7514D     8D4D B0                   LEA ECX,DWORD PTR SS:[EBP-0x50]
	$+1BB    00A75150     C645 FC 04                MOV BYTE PTR SS:[EBP-0x4],0x4
	$+1BF    00A75154     E8 9C4CA4FF               CALL MapleSto.004B9DF5
	$+1C4    00A75159     397D BC                   CMP DWORD PTR SS:[EBP-0x44],EDI
	$+1C7    00A7515C     77 14                     JA SHORT MapleSto.00A75172
	$+1C9    00A7515E     68 57000780               PUSH 0x80070057
	$+1CE    00A75163     E8 B8C998FF               CALL MapleSto.00401B20
	$+1D3    00A75168     68 05400080               PUSH 0x80004005
	$+1D8    00A7516D     E8 AEC998FF               CALL MapleSto.00401B20
	$+1DD    00A75172     8B55 C8                   MOV EDX,DWORD PTR SS:[EBP-0x38]
	$+1E0    00A75175     8B45 B8                   MOV EAX,DWORD PTR SS:[EBP-0x48]
	$+1E3    00A75178     8D4D B0                   LEA ECX,DWORD PTR SS:[EBP-0x50]
	$+1E6    00A7517B     51                        PUSH ECX
	$+1E7    00A7517C     C60410 01                 MOV BYTE PTR DS:[EAX+EDX],0x1
	$+1EB    00A75180     FF45 C8                   INC DWORD PTR SS:[EBP-0x38]
	$+1EE    00A75183     8D8D 74FFFFFF             LEA ECX,DWORD PTR SS:[EBP-0x8C]
	$+1F4    00A75189     E8 F2469AFF               CALL MapleSto.00419880
	$+1F9    00A7518E     8D4D B0                   LEA ECX,DWORD PTR SS:[EBP-0x50]
	$+1FC    00A75191     E8 5BFD1D01               CALL MapleSto.01C54EF1                            ; 发包创建角色
	$+201    00A75196     8B4D 08                   MOV ECX,DWORD PTR SS:[EBP+0x8]
	$+204    00A75199     8B35 34277201             MOV ESI,DWORD PTR DS:[0x1722734]
	$+20A    00A7519F     57                        PUSH EDI
	$+20B    00A751A0     FF15 B8A13E01             CALL DWORD PTR DS:[0x13EA1B8]                     ; mfc90.#3726
	$+211    00A751A6     50                        PUSH EAX
	$+212    00A751A7     8D55 08                   LEA EDX,DWORD PTR SS:[EBP+0x8]
	$+215    00A751AA     52                        PUSH EDX
	$+216    00A751AB     E8 10A09CFF               CALL MapleSto.0043F1C0
	$+21B    00A751B0     8B10                      MOV EDX,DWORD PTR DS:[EAX]
	$+21D    00A751B2     8B0E                      MOV ECX,DWORD PTR DS:[ESI]
	$+21F    00A751B4     8B41 4C                   MOV EAX,DWORD PTR DS:[ECX+0x4C]
	$+222    00A751B7     52                        PUSH EDX
	$+223    00A751B8     68 D4A14601               PUSH MapleSto.0146A1D4                            ; charName = %s
	$+228    00A751BD     6A 32                     PUSH 0x32
	$+22A    00A751BF     56                        PUSH ESI
	$+22B    00A751C0     C645 FC 05                MOV BYTE PTR SS:[EBP-0x4],0x5
	$+22F    00A751C4     FFD0                      CALL EAX
	$+231    00A751C6     83C4 1C                   ADD ESP,0x1C
	$+234    00A751C9     8D4D 08                   LEA ECX,DWORD PTR SS:[EBP+0x8]
	$+237    00A751CC     FF15 08A23E01             CALL DWORD PTR DS:[0x13EA208]                     ; mfc90.#601
	$+23D    00A751D2     8B45 B8                   MOV EAX,DWORD PTR SS:[EBP-0x48]
	$+240    00A751D5     8B35 E8983E01             MOV ESI,DWORD PTR DS:[0x13E98E8]                  ; msvcr90.free
	$+246    00A751DB     C745 B0 BC504101          MOV DWORD PTR SS:[EBP-0x50],MapleSto.014150BC
	$+24D    00A751E2     3BC7                      CMP EAX,EDI
	$+24F    00A751E4     74 06                     JE SHORT MapleSto.00A751EC
	$+251    00A751E6     50                        PUSH EAX
	$+252    00A751E7     FFD6                      CALL ESI
	$+254    00A751E9     83C4 04                   ADD ESP,0x4
	$+257    00A751EC     8D4D D0                   LEA ECX,DWORD PTR SS:[EBP-0x30]
	$+25A    00A751EF     E8 BCC0B9FF               CALL MapleSto.006112B0
	$+25F    00A751F4     8B45 94                   MOV EAX,DWORD PTR SS:[EBP-0x6C]
	$+262    00A751F7     C745 88 2CA94101          MOV DWORD PTR SS:[EBP-0x78],MapleSto.0141A92C
	$+269    00A751FE     C745 8C E4A84101          MOV DWORD PTR SS:[EBP-0x74],MapleSto.0141A8E4     ;  锬
	$+270    00A75205     C745 90 9CA84101          MOV DWORD PTR SS:[EBP-0x70],MapleSto.0141A89C
	$+277    00A7520C     3BC7                      CMP EAX,EDI
	$+279    00A7520E     74 06                     JE SHORT MapleSto.00A75216
	$+27B    00A75210     50                        PUSH EAX
	$+27C    00A75211     FFD6                      CALL ESI
	$+27E    00A75213     83C4 04                   ADD ESP,0x4
	$+281    00A75216     8D8D 7CFFFFFF             LEA ECX,DWORD PTR SS:[EBP-0x84]
	$+287    00A7521C     FF15 DCA13E01             CALL DWORD PTR DS:[0x13EA1DC]                     ; mfc90.#601
	$+28D    00A75222     B8 01000000               MOV EAX,0x1
	$+292    00A75227     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+295    00A7522A     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+29C    00A75231     59                        POP ECX
	$+29D    00A75232     5F                        POP EDI
	$+29E    00A75233     5E                        POP ESI
	$+29F    00A75234     5B                        POP EBX
	$+2A0    00A75235     8BE5                      MOV ESP,EBP
	$+2A2    00A75237     5D                        POP EBP
	$+2A3    00A75238     C2 0400                   RETN 0x4
	$+2A6    00A7523B     CC                        INT3
	$+2A7    00A7523C     CC                        INT3
	$+2A8    00A7523D     CC                        INT3
	$+2A9    00A7523E     CC                        INT3
	$+2AA    00A7523F     CC                        INT3
	$+2AB    00A75240     55                        PUSH EBP
	$+2AC    00A75241     8BEC                      MOV EBP,ESP
	$+2AE    00A75243     6A FF                     PUSH -0x1
	$+2B0    00A75245     68 501A3801               PUSH MapleSto.01381A50
	$+2B5    00A7524A     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$+2BB    00A75250     50                        PUSH EAX
	$+2BC    00A75251     83EC 08                   SUB ESP,0x8
	$+2BF    00A75254     53                        PUSH EBX
	$+2C0    00A75255     56                        PUSH ESI
	$+2C1    00A75256     57                        PUSH EDI
	$+2C2    00A75257     A1 D0687101               MOV EAX,DWORD PTR DS:[0x17168D0]
	$+2C7    00A7525C     33C5                      XOR EAX,EBP
	$+2C9    00A7525E     50                        PUSH EAX
	$+2CA    00A7525F     8D45 F4                   LEA EAX,DWORD PTR SS:[EBP-0xC]
	$+2CD    00A75262     64:A3 00000000            MOV DWORD PTR FS:[0],EAX
	$+2D3    00A75268     8BD9                      MOV EBX,ECX
	$+2D5    00A7526A     8B0D 2C417201             MOV ECX,DWORD PTR DS:[0x172412C]
	$+2DB    00A75270     E8 5EE74201               CALL MapleSto.01EA39D3
	$+2E0    00A75275     8BF0                      MOV ESI,EAX
	$+2E2    00A75277     85F6                      TEST ESI,ESI
	$+2E4    00A75279     0F84 89000000             JE MapleSto.00A75308
	$+2EA    00A7527F     8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+2ED    00A75282     50                        PUSH EAX
	$+2EE    00A75283     8D4D EC                   LEA ECX,DWORD PTR SS:[EBP-0x14]
	$+2F1    00A75286     51                        PUSH ECX
	$+2F2    00A75287     E8 14551901               CALL MapleSto.01C0A7A0
	$+2F7    00A7528C     83C4 08                   ADD ESP,0x8
	$+2FA    00A7528F     8B7D F0                   MOV EDI,DWORD PTR SS:[EBP-0x10]
	$+2FD    00A75292     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$+304    00A75299     85FF                      TEST EDI,EDI
	$+306    00A7529B     74 5C                     JE SHORT MapleSto.00A752F9
	$+308    00A7529D     8BCE                      MOV ECX,ESI
	$+30A    00A7529F     E8 A48D3901               CALL MapleSto.01E0E048
	$+30F    00A752A4     3987 30010000             CMP DWORD PTR DS:[EDI+0x130],EAX
	$+315    00A752AA     8B45 F0                   MOV EAX,DWORD PTR SS:[EBP-0x10]
	$+318    00A752AD     74 09                     JE SHORT MapleSto.00A752B8
	$+31A    00A752AF     83B8 30010000 02          CMP DWORD PTR DS:[EAX+0x130],0x2
	$+321    00A752B6     75 41                     JNZ SHORT MapleSto.00A752F9
	$+323    00A752B8     8B55 10                   MOV EDX,DWORD PTR SS:[EBP+0x10]
	$+326    00A752BB     05 E8000000               ADD EAX,0xE8
	$+32B    00A752C0     50                        PUSH EAX
	$+32C    00A752C1     8B45 0C                   MOV EAX,DWORD PTR SS:[EBP+0xC]
	$+32F    00A752C4     52                        PUSH EDX
	$+330    00A752C5     50                        PUSH EAX
	$+331    00A752C6     8BCB                      MOV ECX,EBX
	$+333    00A752C8     E8 A3F2FFFF               CALL MapleSto.00A74570
	$+338    00A752CD     85C0                      TEST EAX,EAX
	$+33A    00A752CF     74 28                     JE SHORT MapleSto.00A752F9
	$+33C    00A752D1     8D4D EC                   LEA ECX,DWORD PTR SS:[EBP-0x14]
	$+33F    00A752D4     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+346    00A752DB     E8 307E0000               CALL MapleSto.00A7D110
	$+34B    00A752E0     B8 01000000               MOV EAX,0x1
	$+350    00A752E5     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+353    00A752E8     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+35A    00A752EF     59                        POP ECX
	$+35B    00A752F0     5F                        POP EDI
	$+35C    00A752F1     5E                        POP ESI
	$+35D    00A752F2     5B                        POP EBX
	$+35E    00A752F3     8BE5                      MOV ESP,EBP
	$+360    00A752F5     5D                        POP EBP
	$+361    00A752F6     C2 0C00                   RETN 0xC
	$+364    00A752F9     8D4D EC                   LEA ECX,DWORD PTR SS:[EBP-0x14]
	$+367    00A752FC     C745 FC FFFFFFFF          MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+36E    00A75303     E8 087E0000               CALL MapleSto.00A7D110
	$+373    00A75308     33C0                      XOR EAX,EAX
	$+375    00A7530A     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+378    00A7530D     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+37F    00A75314     59                        POP ECX
	$+380    00A75315     5F                        POP EDI
	$+381    00A75316     5E                        POP ESI
	$+382    00A75317     5B                        POP EBX
	$+383    00A75318     8BE5                      MOV ESP,EBP
	$+385    00A7531A     5D                        POP EBP
	$+386    00A7531B     C2 0C00                   RETN 0xC
	$+389    00A7531E     CC                        INT3
	$+38A    00A7531F     CC                        INT3
	$+38B    00A75320     55                        PUSH EBP
	$+38C    00A75321     8BEC                      MOV EBP,ESP
	$+38E    00A75323     56                        PUSH ESI
	$+38F    00A75324     8BF1                      MOV ESI,ECX
	$+391    00A75326     83BE 70010000 00          CMP DWORD PTR DS:[ESI+0x170],0x0
	$+398    00A7532D     74 44                     JE SHORT MapleSto.00A75373
	$+39A    00A7532F     53                        PUSH EBX
	$+39B    00A75330     8B5D 08                   MOV EBX,DWORD PTR SS:[EBP+0x8]
	$+39E    00A75333     83FB 01                   CMP EBX,0x1
	$+3A1    00A75336     77 3A                     JA SHORT MapleSto.00A75372
	$+3A3    00A75338     83BE 80010000 00          CMP DWORD PTR DS:[ESI+0x180],0x0
	$+3AA    00A7533F     75 31                     JNZ SHORT MapleSto.00A75372
	$+3AC    00A75341     57                        PUSH EDI
	$+3AD    00A75342     E8 094FC7FF               CALL MapleSto.006EA250
	$+3B2    00A75347     8BF8                      MOV EDI,EAX
	$+3B4    00A75349     85FF                      TEST EDI,EDI
	$+3B6    00A7534B     74 24                     JE SHORT MapleSto.00A75371
	$+3B8    00A7534D     8B0D 2C417201             MOV ECX,DWORD PTR DS:[0x172412C]
	$+3BE    00A75353     E8 7BE64201               CALL MapleSto.01EA39D3
	$+3C3    00A75358     85C0                      TEST EAX,EAX
	$+3C5    00A7535A     74 15                     JE SHORT MapleSto.00A75371
	$+3C7    00A7535C     53                        PUSH EBX
	$+3C8    00A7535D     8BCF                      MOV ECX,EDI
	$+3CA    00A7535F     E8 48573D01               CALL MapleSto.01E4AAAC
	$+3CF    00A75364     6A 00                     PUSH 0x0
	$+3D1    00A75366     8D8E 6C010000             LEA ECX,DWORD PTR DS:[ESI+0x16C]
	$+3D7    00A7536C     E8 1FEDB4FF               CALL MapleSto.005C4090
	$+3DC    00A75371     5F                        POP EDI
	$+3DD    00A75372     5B                        POP EBX
	$+3DE    00A75373     5E                        POP ESI
	$+3DF    00A75374     5D                        POP EBP
	$+3E0    00A75375     C2 0400                   RETN 0x4
	$+3E3    00A75378     CC                        INT3
	*/
	addr_tmp = scan_exe_.ScanAddr("64??????????89????8b??????????e8????????8b??33??3b??75??33??8b", -0x25);
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	if (AsByte(addr_tmp, 0) != 0x55)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_c_create_role_by_name_ = " << addr_tmp << ";" << FC_OFstream_EndlLog;
	return true;
}
