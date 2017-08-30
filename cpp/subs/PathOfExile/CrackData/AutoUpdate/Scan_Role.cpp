#include "stdafx.h"
#include "AutoUpdate.h"

bool CAutoUpdateApp::Scan_Role(Cmn_FC_OFstream& ofs)
{
	//pt_pcvoid addr_tmp;
	boost::optional<pt_dword> op_dw_tmp;
	/*
	$-21     005AF250     55                         PUSH EBP                                 ; 这个函数一直会断住
	$-20     005AF251     8BEC                       MOV EBP, ESP
	$-1E     005AF253     83E4 F8                    AND ESP, -0x8
	$-1B     005AF256     83EC 10                    SUB ESP, 0x10
	$-18     005AF259     56                         PUSH ESI
	$-17     005AF25A     57                         PUSH EDI
	$-16     005AF25B     8BF9                       MOV EDI, ECX
	$-14     005AF25D     897C24 10                  MOV DWORD PTR SS:[ESP+0x10], EDI
	$-10     005AF261     8B87 B8080000              MOV EAX, DWORD PTR DS:[EDI+0x8B8]
	$-A      005AF267     85C0                       TEST EAX, EAX
	$-8      005AF269     74 43                      JE SHORT PathOfEx.005AF2AE
	$-6      005AF26B     8B80 943A0000              MOV EAX, DWORD PTR DS:[EAX+0x3A94]       ; 3a94+4=stCD_SendPacket
	$ ==>    005AF271     8B88 C4010000              MOV ECX, DWORD PTR DS:[EAX+0x1C4]        ; 得到EntityComponent对象的偏移
	$+6      005AF277     E8 B48AEDFF                CALL PathOfEx.00487D30
	$+B      005AF27C     8BC8                       MOV ECX, EAX
	$+D      005AF27E     8B87 B8080000              MOV EAX, DWORD PTR DS:[EDI+0x8B8]
	$+13     005AF284     05 E03C0000                ADD EAX, 0x3CE0
	$+18     005AF289     894C24 0C                  MOV DWORD PTR SS:[ESP+0xC], ECX
	$+1C     005AF28D     8B30                       MOV ESI, DWORD PTR DS:[EAX]
	$+1E     005AF28F     3B70 04                    CMP ESI, DWORD PTR DS:[EAX+0x4]
	$+21     005AF292     74 1A                      JE SHORT PathOfEx.005AF2AE
	$+23     005AF294     8BF8                       MOV EDI, EAX
	$+25     005AF296     51                         PUSH ECX
	$+26     005AF297     8BCE                       MOV ECX, ESI
	$+28     005AF299     E8 82193300                CALL PathOfEx.008E0C20
	$+2D     005AF29E     8B4C24 0C                  MOV ECX, DWORD PTR SS:[ESP+0xC]
	$+31     005AF2A2     83C6 3C                    ADD ESI, 0x3C
	$+34     005AF2A5     3B77 04                    CMP ESI, DWORD PTR DS:[EDI+0x4]
	$+37     005AF2A8   ^ 75 EC                      JNZ SHORT PathOfEx.005AF296
	$+39     005AF2AA     8B7C24 10                  MOV EDI, DWORD PTR SS:[ESP+0x10]
	$+3D     005AF2AE     80BF D0080000 00           CMP BYTE PTR DS:[EDI+0x8D0], 0x0
	$+44     005AF2B5     0F57E4                     XORPS XMM4, XMM4
	$+47     005AF2B8     0F84 D6000000              JE PathOfEx.005AF394
	$+4D     005AF2BE     80BF 54070000 00           CMP BYTE PTR DS:[EDI+0x754], 0x0
	$+54     005AF2C5     0F84 C9000000              JE PathOfEx.005AF394
	$+5A     005AF2CB     80BF FC080000 00           CMP BYTE PTR DS:[EDI+0x8FC], 0x0
	$+61     005AF2D2     F3:0F105D 08               MOVSS XMM3, DWORD PTR SS:[EBP+0x8]
	$+66     005AF2D7     74 0F                      JE SHORT PathOfEx.005AF2E8
	$+68     005AF2D9     F3:0F119F F8080000         MOVSS DWORD PTR DS:[EDI+0x8F8], XMM3
	$+70     005AF2E1     C687 FC080000 00           MOV BYTE PTR DS:[EDI+0x8FC], 0x0
	$+77     005AF2E8     F3:0F5C9F F8080000         SUBSS XMM3, DWORD PTR DS:[EDI+0x8F8]
	$+7F     005AF2F0     F3:0F1087 D4080000         MOVSS XMM0, DWORD PTR DS:[EDI+0x8D4]
	$+87     005AF2F8     0F2FC3                     COMISS XMM0, XMM3
	$+8A     005AF2FB     76 7C                      JBE SHORT PathOfEx.005AF379
	$+8C     005AF2FD     F3:0F1097 BC080000         MOVSS XMM2, DWORD PTR DS:[EDI+0x8BC]
	$+94     005AF305     0F5715 406BF300            XORPS XMM2, DQWORD PTR DS:[0xF36B40]
	$+9B     005AF30C     F3:0F5ED8                  DIVSS XMM3, XMM0
	$+9F     005AF310     F3:0F1087 C0080000         MOVSS XMM0, DWORD PTR DS:[EDI+0x8C0]
	$+A7     005AF318     0F5705 406BF300            XORPS XMM0, DQWORD PTR DS:[0xF36B40]
	$+AE     005AF31F     F3:0F59C0                  MULSS XMM0, XMM0
	$+B2     005AF323     F3:0F59D2                  MULSS XMM2, XMM2
	$+B6     005AF327     F3:0F58C2                  ADDSS XMM0, XMM2
	$+BA     005AF32B     E8 306CEFFF                CALL PathOfEx.004A5F60
	$+BF     005AF330     0F2F05 D859F300            COMISS XMM0, DWORD PTR DS:[0xF359D8]
	$+C6     005AF337     76 5B                      JBE SHORT PathOfEx.005AF394
	$+C8     005AF339     F3:0F1015 845BF300         MOVSS XMM2, DWORD PTR DS:[0xF35B84]
	$+D0     005AF341     0F28CB                     MOVAPS XMM1, XMM3
	$+D3     005AF344     F3:0F59CA                  MULSS XMM1, XMM2
	$+D7     005AF348     0F28C2                     MOVAPS XMM0, XMM2
	$+DA     005AF34B     F3:0F59DA                  MULSS XMM3, XMM2
	$+DE     005AF34F     F3:0F5CC1                  SUBSS XMM0, XMM1
	$+E2     005AF353     F3:0F5CD3                  SUBSS XMM2, XMM3
	$+E6     005AF357     F3:0F5987 F0080000         MULSS XMM0, DWORD PTR DS:[EDI+0x8F0]
	$+EE     005AF35F     F3:0F1187 BC080000         MOVSS DWORD PTR DS:[EDI+0x8BC], XMM0
	$+F6     005AF367     F3:0F5997 F4080000         MULSS XMM2, DWORD PTR DS:[EDI+0x8F4]
	$+FE     005AF36F     F3:0F1197 C0080000         MOVSS DWORD PTR DS:[EDI+0x8C0], XMM2
	$+106    005AF377     EB 1B                      JMP SHORT PathOfEx.005AF394
	$+108    005AF379     C787 F0080000 00000000     MOV DWORD PTR DS:[EDI+0x8F0], 0x0
	$+112    005AF383     C787 F4080000 00000000     MOV DWORD PTR DS:[EDI+0x8F4], 0x0
	$+11C    005AF38D     C687 D0080000 00           MOV BYTE PTR DS:[EDI+0x8D0], 0x0
	$+123    005AF394     80BF FC080000 00           CMP BYTE PTR DS:[EDI+0x8FC], 0x0
	$+12A    005AF39B     74 7A                      JE SHORT PathOfEx.005AF417
	$+12C    005AF39D     F3:0F108F D8080000         MOVSS XMM1, DWORD PTR DS:[EDI+0x8D8]
	$+134    005AF3A5     0F2FCC                     COMISS XMM1, XMM4
	$+137    005AF3A8     76 6D                      JBE SHORT PathOfEx.005AF417
	$+139    005AF3AA     F3:0F1045 0C               MOVSS XMM0, DWORD PTR SS:[EBP+0xC]
	$+13E    005AF3AF     F3:0F5CC8                  SUBSS XMM1, XMM0
	$+142    005AF3B3     F3:0F118F D8080000         MOVSS DWORD PTR DS:[EDI+0x8D8], XMM1
	$+14A    005AF3BB     F3:0F108F DC080000         MOVSS XMM1, DWORD PTR DS:[EDI+0x8DC]
	$+152    005AF3C3     F3:0F1097 E0080000         MOVSS XMM2, DWORD PTR DS:[EDI+0x8E0]
	$+15A    005AF3CB     F3:0F59C8                  MULSS XMM1, XMM0
	$+15E    005AF3CF     F3:0F59D0                  MULSS XMM2, XMM0
	$+162    005AF3D3     F3:0F1087 BC080000         MOVSS XMM0, DWORD PTR DS:[EDI+0x8BC]
	$+16A    005AF3DB     F3:0F58C1                  ADDSS XMM0, XMM1
	$+16E    005AF3DF     F3:0F114424 10             MOVSS DWORD PTR SS:[ESP+0x10], XMM0
	$+174    005AF3E5     F3:0F1087 C0080000         MOVSS XMM0, DWORD PTR DS:[EDI+0x8C0]
	$+17C    005AF3ED     8B4C24 10                  MOV ECX, DWORD PTR SS:[ESP+0x10]
	$+180    005AF3F1     F3:0F58C2                  ADDSS XMM0, XMM2
	$+184    005AF3F5     898F BC080000              MOV DWORD PTR DS:[EDI+0x8BC], ECX
	$+18A    005AF3FB     898F F0080000              MOV DWORD PTR DS:[EDI+0x8F0], ECX
	$+190    005AF401     F3:0F114424 14             MOVSS DWORD PTR SS:[ESP+0x14], XMM0
	$+196    005AF407     8B4424 14                  MOV EAX, DWORD PTR SS:[ESP+0x14]
	$+19A    005AF40B     8987 C0080000              MOV DWORD PTR DS:[EDI+0x8C0], EAX
	$+1A0    005AF411     8987 F4080000              MOV DWORD PTR DS:[EDI+0x8F4], EAX
	$+1A6    005AF417     5F                         POP EDI                                  ; ntdll_1z.7774F8D1
	$+1A7    005AF418     5E                         POP ESI                                  ; ntdll_1z.7774F8D1
	$+1A8    005AF419     8BE5                       MOV ESP, EBP
	$+1AA    005AF41B     5D                         POP EBP                                  ; ntdll_1z.7774F8D1
	$+1AB    005AF41C     C2 0800                    RETN 0x8
	*/
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("8b??????????e8????????8b??8b??????????05????????89??????8b??3b", 2);
	assert(op_dw_tmp);
	ofs << "pt_cdword	g_o_get_entity_component_ = " << *op_dw_tmp << ";" << FC_OFstream_EndlLog;
	return true;
}
