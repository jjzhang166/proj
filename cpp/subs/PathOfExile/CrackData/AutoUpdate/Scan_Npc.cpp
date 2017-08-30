#include "stdafx.h"
#include "AutoUpdate.h"

bool CAutoUpdateApp::Scan_Npc(Cmn_FC_OFstream& ofs)
{
	//pt_pcvoid addr_tmp;
	boost::optional<pt_dword> op_dw_tmp;
	pt_dword dw_tmp;
	/*
	$-1F     007F4A1F     CC                   INT3
	$-1E     007F4A20     56                   PUSH ESI
	$-1D     007F4A21     8B7424 08            MOV ESI,DWORD PTR SS:[ESP+0x8]
	$-19     007F4A25     57                   PUSH EDI
	$-18     007F4A26     8BF9                 MOV EDI,ECX
	$-16     007F4A28     8BCE                 MOV ECX,ESI
	$-14     007F4A2A     6A 01                PUSH 0x1
	$-12     007F4A2C     E8 FFBCCAFF          CALL PathOfEx.004A0730
	$-D      007F4A31     8B46 08              MOV EAX,DWORD PTR DS:[ESI+0x8]
	$-A      007F4A34     8BCE                 MOV ECX,ESI
	$-8      007F4A36     8B16                 MOV EDX,DWORD PTR DS:[ESI]
	$-6      007F4A38     6A 01                PUSH 0x1
	$-4      007F4A3A     0FB60402             MOVZX EAX,BYTE PTR DS:[EDX+EAX]          ; edi=stCD_EntityAttrNPC*
	$ ==>    007F4A3E     8887 10010000        MOV BYTE PTR DS:[EDI+0x110],AL           ; g_o_entity_attr_npc_is_locked_
	$+6      007F4A44     FF46 08              INC DWORD PTR DS:[ESI+0x8]
	$+9      007F4A47     E8 E4BCCAFF          CALL PathOfEx.004A0730
	$+E      007F4A4C     8B46 08              MOV EAX,DWORD PTR DS:[ESI+0x8]
	$+11     007F4A4F     8B0E                 MOV ECX,DWORD PTR DS:[ESI]
	$+13     007F4A51     0FB60401             MOVZX EAX,BYTE PTR DS:[ECX+EAX]
	$+17     007F4A55     8887 11010000        MOV BYTE PTR DS:[EDI+0x111],AL
	$+1D     007F4A5B     FF46 08              INC DWORD PTR DS:[ESI+0x8]
	$+20     007F4A5E     5F                   POP EDI
	$+21     007F4A5F     5E                   POP ESI
	$+22     007F4A60     C2 0400              RETN 0x4
	$+25     007F4A63     CC                   INT3
	$+26     007F4A64     CC                   INT3
	$+27     007F4A65     CC                   INT3
	$+28     007F4A66     CC                   INT3
	$+29     007F4A67     CC                   INT3
	$+2A     007F4A68     CC                   INT3
	$+2B     007F4A69     CC                   INT3
	$+2C     007F4A6A     CC                   INT3
	$+2D     007F4A6B     CC                   INT3
	$+2E     007F4A6C     CC                   INT3
	$+2F     007F4A6D     CC                   INT3
	$+30     007F4A6E     CC                   INT3
	$+31     007F4A6F     CC                   INT3
	$+32     007F4A70     55                   PUSH EBP
	$+33     007F4A71     8BEC                 MOV EBP,ESP
	$+35     007F4A73     83E4 F8              AND ESP,0xFFFFFFF8
	$+38     007F4A76     51                   PUSH ECX
	$+39     007F4A77     53                   PUSH EBX
	$+3A     007F4A78     56                   PUSH ESI
	$+3B     007F4A79     8B75 08              MOV ESI,DWORD PTR SS:[EBP+0x8]
	$+3E     007F4A7C     57                   PUSH EDI
	$+3F     007F4A7D     8BF9                 MOV EDI,ECX
	$+41     007F4A7F     8BCE                 MOV ECX,ESI
	$+43     007F4A81     6A 01                PUSH 0x1
	$+45     007F4A83     8A9F 11010000        MOV BL,BYTE PTR DS:[EDI+0x111]
	$+4B     007F4A89     8ABF 10010000        MOV BH,BYTE PTR DS:[EDI+0x110]
	$+51     007F4A8F     E8 9CBCCAFF          CALL PathOfEx.004A0730
	$+56     007F4A94     8B4E 08              MOV ECX,DWORD PTR DS:[ESI+0x8]
	$+59     007F4A97     8B06                 MOV EAX,DWORD PTR DS:[ESI]
	$+5B     007F4A99     6A 01                PUSH 0x1
	$+5D     007F4A9B     0FB60401             MOVZX EAX,BYTE PTR DS:[ECX+EAX]
	$+61     007F4A9F     8BCE                 MOV ECX,ESI                              ; edi=stCD_EntityAttrNPC*
	$+63     007F4AA1     8887 10010000        MOV BYTE PTR DS:[EDI+0x110],AL           ; g_o_entity_attr_npc_is_locked_
	$+69     007F4AA7     FF46 08              INC DWORD PTR DS:[ESI+0x8]
	$+6C     007F4AAA     E8 81BCCAFF          CALL PathOfEx.004A0730
	$+71     007F4AAF     8B4E 08              MOV ECX,DWORD PTR DS:[ESI+0x8]
	$+74     007F4AB2     8B06                 MOV EAX,DWORD PTR DS:[ESI]
	$+76     007F4AB4     0FB60401             MOVZX EAX,BYTE PTR DS:[ECX+EAX]
	$+7A     007F4AB8     8887 11010000        MOV BYTE PTR DS:[EDI+0x111],AL
	$+80     007F4ABE     FF46 08              INC DWORD PTR DS:[ESI+0x8]
	$+83     007F4AC1     3A9F 11010000        CMP BL,BYTE PTR DS:[EDI+0x111]
	$+89     007F4AC7     74 09                JE SHORT PathOfEx.007F4AD2
	$+8B     007F4AC9     57                   PUSH EDI
	$+8C     007F4ACA     8D4F 10              LEA ECX,DWORD PTR DS:[EDI+0x10]
	$+8F     007F4ACD     E8 6EEECBFF          CALL PathOfEx.004B3940
	$+94     007F4AD2     80BF 10010000 00     CMP BYTE PTR DS:[EDI+0x110],0x0
	$+9B     007F4AD9     75 10                JNZ SHORT PathOfEx.007F4AEB
	$+9D     007F4ADB     84FF                 TEST BH,BH
	$+9F     007F4ADD     74 0C                JE SHORT PathOfEx.007F4AEB
	$+A1     007F4ADF     57                   PUSH EDI
	$+A2     007F4AE0     8D8F 90000000        LEA ECX,DWORD PTR DS:[EDI+0x90]
	$+A8     007F4AE6     E8 55EECBFF          CALL PathOfEx.004B3940
	$+AD     007F4AEB     5F                   POP EDI
	$+AE     007F4AEC     5E                   POP ESI
	$+AF     007F4AED     5B                   POP EBX
	$+B0     007F4AEE     8BE5                 MOV ESP,EBP
	$+B2     007F4AF0     5D                   POP EBP
	$+B3     007F4AF1     C2 0400              RETN 0x4
	$+B6     007F4AF4     CC                   INT3
	$+B7     007F4AF5     CC                   INT3
	$+B8     007F4AF6     CC                   INT3
	$+B9     007F4AF7     CC                   INT3
	$+BA     007F4AF8     CC                   INT3
	$+BB     007F4AF9     CC                   INT3
	$+BC     007F4AFA     CC                   INT3
	$+BD     007F4AFB     CC                   INT3
	$+BE     007F4AFC     CC                   INT3
	$+BF     007F4AFD     CC                   INT3
	$+C0     007F4AFE     CC                   INT3
	$+C1     007F4AFF     CC                   INT3
	$+C2     007F4B00     55                   PUSH EBP
	$+C3     007F4B01     8BEC                 MOV EBP,ESP
	$+C5     007F4B03     6A FF                PUSH -0x1
	$+C7     007F4B05     68 D616D200          PUSH PathOfEx.00D216D6                   ; ASCII "¸ÀÚô"
	$+CC     007F4B0A     64:A1 00000000       MOV EAX,DWORD PTR FS:[0]
	$+D2     007F4B10     50                   PUSH EAX
	$+D3     007F4B11     64:8925 00000000     MOV DWORD PTR FS:[0],ESP
	$+DA     007F4B18     83EC 08              SUB ESP,0x8
	$+DD     007F4B1B     53                   PUSH EBX
	$+DE     007F4B1C     56                   PUSH ESI
	$+DF     007F4B1D     57                   PUSH EDI
	$+E0     007F4B1E     8BF2                 MOV ESI,EDX
	$+E2     007F4B20     8BF9                 MOV EDI,ECX
	$+E4     007F4B22     33C9                 XOR ECX,ECX
	$+E6     007F4B24     894D F0              MOV DWORD PTR SS:[EBP-0x10],ECX
	$+E9     007F4B27     8B46 0C              MOV EAX,DWORD PTR DS:[ESI+0xC]
	$+EC     007F4B2A     3948 14              CMP DWORD PTR DS:[EAX+0x14],ECX
	$+EF     007F4B2D     74 21                JE SHORT PathOfEx.007F4B50
	$+F1     007F4B2F     8B40 0C              MOV EAX,DWORD PTR DS:[EAX+0xC]
	$+F4     007F4B32     85C0                 TEST EAX,EAX
	$+F6     007F4B34     74 1A                JE SHORT PathOfEx.007F4B50
	$+F8     007F4B36     8D4D EC              LEA ECX,DWORD PTR SS:[EBP-0x14]
	$+FB     007F4B39     51                   PUSH ECX
	$+FC     007F4B3A     8BC8                 MOV ECX,EAX
	$+FE     007F4B3C     E8 CF72CBFF          CALL PathOfEx.004ABE10
	$+103    007F4B41     B9 01000000          MOV ECX,0x1
	$+108    007F4B46     8378 04 00           CMP DWORD PTR DS:[EAX+0x4],0x0
	$+10C    007F4B4A     74 04                JE SHORT PathOfEx.007F4B50
	$+10E    007F4B4C     32DB                 XOR BL,BL
	$+110    007F4B4E     EB 02                JMP SHORT PathOfEx.007F4B52
	$+112    007F4B50     B3 01                MOV BL,0x1
	$+114    007F4B52     F6C1 01              TEST CL,0x1
	$+117    007F4B55     74 26                JE SHORT PathOfEx.007F4B7D
	$+119    007F4B57     C745 FC 00000000     MOV DWORD PTR SS:[EBP-0x4],0x0
	$+120    007F4B5E     8B45 EC              MOV EAX,DWORD PTR SS:[EBP-0x14]
	$+123    007F4B61     85C0                 TEST EAX,EAX
	$+125    007F4B63     74 11                JE SHORT PathOfEx.007F4B76
	$+127    007F4B65     50                   PUSH EAX
	$+128    007F4B66     51                   PUSH ECX
	$+129    007F4B67     E8 9434F2FF          CALL PathOfEx.00718000
	$+12E    007F4B6C     83C4 04              ADD ESP,0x4
	$+131    007F4B6F     8BC8                 MOV ECX,EAX
	$+133    007F4B71     E8 1A30F2FF          CALL PathOfEx.00717B90
	$+138    007F4B76     C745 FC FFFFFFFF     MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+13F    007F4B7D     84DB                 TEST BL,BL
	$+141    007F4B7F     74 13                JE SHORT PathOfEx.007F4B94
	$+143    007F4B81     33C0                 XOR EAX,EAX
	$+145    007F4B83     8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+148    007F4B86     64:890D 00000000     MOV DWORD PTR FS:[0],ECX
	$+14F    007F4B8D     5F                   POP EDI
	$+150    007F4B8E     5E                   POP ESI
	$+151    007F4B8F     5B                   POP EBX
	$+152    007F4B90     8BE5                 MOV ESP,EBP
	$+154    007F4B92     5D                   POP EBP
	$+155    007F4B93     C3                   RETN
	$+156    007F4B94     8BCE                 MOV ECX,ESI
	$+158    007F4B96     E8 85FDFFFF          CALL PathOfEx.007F4920
	$+15D    007F4B9B     8D4D EC              LEA ECX,DWORD PTR SS:[EBP-0x14]
	$+160    007F4B9E     51                   PUSH ECX
	$+161    007F4B9F     50                   PUSH EAX
	$+162    007F4BA0     8BCF                 MOV ECX,EDI
	$+164    007F4BA2     E8 A91DFFFF          CALL PathOfEx.007E6950
	$+169    007F4BA7     8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+16C    007F4BAA     5F                   POP EDI
	$+16D    007F4BAB     5E                   POP ESI
	$+16E    007F4BAC     64:890D 00000000     MOV DWORD PTR FS:[0],ECX
	$+175    007F4BB3     5B                   POP EBX
	$+176    007F4BB4     8BE5                 MOV ESP,EBP
	$+178    007F4BB6     5D                   POP EBP
	$+179    007F4BB7     C3                   RETN
	$+17A    007F4BB8     CC                   INT3
	*/
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("88??????????ff????e8????????8b????8b??0f??????88", 2);
	assert(op_dw_tmp);
	dw_tmp = *op_dw_tmp;
	assert(dw_tmp > 0x50 && dw_tmp < 0x200);
	ofs << "pt_cdword	g_o_entity_attr_npc_is_locked_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;
	return true;
}
