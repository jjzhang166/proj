#include "stdafx.h"
#include "AutoUpdate.h"
#include <CmnMix/Cmn_LogOut.h>

bool CAutoUpdateApp::Scan_Offset(Cmn_FC_OFstream& ofs)
{
	boost::optional<pt_dword> op_dw_tmp;
	boost::optional<pt_byte> op_by_tmp;
	pt_dword dw_tmp;
	pt_pcvoid addr_tmp;
	pt_pcvoid addr_tmp1;
	/*
	$-6A     008F986F     CC                  INT3
	$-69     008F9870     56                  PUSH ESI
	$-68     008F9871     8BF1                MOV ESI,ECX
	$-66     008F9873     E8 C8E9EFFF         CALL PathOfEx.007F8240
	$-61     008F9878     F64424 08 01        TEST BYTE PTR SS:[ESP+0x8],0x1
	$-5C     008F987D     74 0E               JE SHORT PathOfEx.008F988D
	$-5A     008F987F     68 18010000         PUSH 0x118
	$-55     008F9884     56                  PUSH ESI
	$-54     008F9885     E8 7E2E3800         CALL PathOfEx.00C7C708
	$-4F     008F988A     83C4 08             ADD ESP,0x8
	$-4C     008F988D     8BC6                MOV EAX,ESI
	$-4A     008F988F     5E                  POP ESI
	$-49     008F9890     C2 0400             RETN 0x4
	$-46     008F9893     CC                  INT3
	$-45     008F9894     CC                  INT3
	$-44     008F9895     CC                  INT3
	$-43     008F9896     CC                  INT3
	$-42     008F9897     CC                  INT3
	$-41     008F9898     CC                  INT3
	$-40     008F9899     CC                  INT3
	$-3F     008F989A     CC                  INT3
	$-3E     008F989B     CC                  INT3
	$-3D     008F989C     CC                  INT3
	$-3C     008F989D     CC                  INT3
	$-3B     008F989E     CC                  INT3
	$-3A     008F989F     CC                  INT3
	$-39     008F98A0     53                  PUSH EBX                                 ; tag_点击副本里的门会断到,改变Transitionable属性对象中的状态为已打开
	$-38     008F98A1     56                  PUSH ESI
	$-37     008F98A2     57                  PUSH EDI
	$-36     008F98A3     8B7C24 10           MOV EDI,DWORD PTR SS:[ESP+0x10]
	$-32     008F98A7     8BF1                MOV ESI,ECX
	$-30     008F98A9     6A 01               PUSH 0x1
	$-2E     008F98AB     8BCF                MOV ECX,EDI
	$-2C     008F98AD     E8 7E6EBAFF         CALL PathOfEx.004A0730
	$-27     008F98B2     8B4F 08             MOV ECX,DWORD PTR DS:[EDI+0x8]
	$-24     008F98B5     8B07                MOV EAX,DWORD PTR DS:[EDI]
	$-22     008F98B7     8A1C01              MOV BL,BYTE PTR DS:[ECX+EAX]
	$-1F     008F98BA     8D41 01             LEA EAX,DWORD PTR DS:[ECX+0x1]
	$-1C     008F98BD     8947 08             MOV DWORD PTR DS:[EDI+0x8],EAX
	$-19     008F98C0     3A9E 0C010000       CMP BL,BYTE PTR DS:[ESI+0x10C]
	$-13     008F98C6     76 5A               JBE SHORT PathOfEx.008F9922
	$-11     008F98C8     55                  PUSH EBP
	$-10     008F98C9     BD 02000000         MOV EBP,0x2
	$-B      008F98CE     66:90               NOP
	$-9      008F98D0     8B8E 10010000       MOV ECX,DWORD PTR DS:[ESI+0x110]
	$-3      008F98D6     FF76 04             PUSH DWORD PTR DS:[ESI+0x4]
	$ ==>    008F98D9     FE86 0C010000       INC BYTE PTR DS:[ESI+0x10C]              ; g_o_transitionable_state_
	$+6      008F98DF     83C1 08             ADD ECX,0x8
	$+9      008F98E2     E8 49C3F1FF         CALL PathOfEx.00815C30
	$+E      008F98E7     0FB686 0C010000     MOVZX EAX,BYTE PTR DS:[ESI+0x10C]
	$+15     008F98EE     FF76 04             PUSH DWORD PTR DS:[ESI+0x4]
	$+18     008F98F1     83F8 02             CMP EAX,0x2
	$+1B     008F98F4     0F42C5              CMOVB EAX,EBP
	$+1E     008F98F7     83E8 02             SUB EAX,0x2
	$+21     008F98FA     8D0C40              LEA ECX,DWORD PTR DS:[EAX+EAX*2]
	$+24     008F98FD     8B86 10010000       MOV EAX,DWORD PTR DS:[ESI+0x110]
	$+2A     008F9903     8B40 20             MOV EAX,DWORD PTR DS:[EAX+0x20]
	$+2D     008F9906     8D0C88              LEA ECX,DWORD PTR DS:[EAX+ECX*4]
	$+30     008F9909     E8 22C3F1FF         CALL PathOfEx.00815C30
	$+35     008F990E     FF76 04             PUSH DWORD PTR DS:[ESI+0x4]
	$+38     008F9911     8D4E 0C             LEA ECX,DWORD PTR DS:[ESI+0xC]
	$+3B     008F9914     E8 27A0BBFF         CALL PathOfEx.004B3940
	$+40     008F9919     3A9E 0C010000       CMP BL,BYTE PTR DS:[ESI+0x10C]
	$+46     008F991F   ^ 77 AF               JA SHORT PathOfEx.008F98D0
	$+48     008F9921     5D                  POP EBP
	$+49     008F9922     73 5A               JNB SHORT PathOfEx.008F997E
	$+4B     008F9924     0F1F40 00           NOP DWORD PTR DS:[EAX]
	$+4F     008F9928     0F1F8400 00000000   NOP DWORD PTR DS:[EAX+EAX]
	$+57     008F9930     8B8E 10010000       MOV ECX,DWORD PTR DS:[ESI+0x110]
	$+5D     008F9936     FF76 04             PUSH DWORD PTR DS:[ESI+0x4]
	$+60     008F9939     FE8E 0C010000       DEC BYTE PTR DS:[ESI+0x10C]
	$+66     008F993F     83C1 14             ADD ECX,0x14
	$+69     008F9942     E8 E9C2F1FF         CALL PathOfEx.00815C30
	$+6E     008F9947     0FB686 0C010000     MOVZX EAX,BYTE PTR DS:[ESI+0x10C]
	$+75     008F994E     FF76 04             PUSH DWORD PTR DS:[ESI+0x4]
	$+78     008F9951     8D0C40              LEA ECX,DWORD PTR DS:[EAX+EAX*2]
	$+7B     008F9954     8B86 10010000       MOV EAX,DWORD PTR DS:[ESI+0x110]
	$+81     008F995A     8D49 FD             LEA ECX,DWORD PTR DS:[ECX-0x3]
	$+84     008F995D     8B40 2C             MOV EAX,DWORD PTR DS:[EAX+0x2C]
	$+87     008F9960     8D0C88              LEA ECX,DWORD PTR DS:[EAX+ECX*4]
	$+8A     008F9963     E8 C8C2F1FF         CALL PathOfEx.00815C30
	$+8F     008F9968     FF76 04             PUSH DWORD PTR DS:[ESI+0x4]
	$+92     008F996B     8D8E 8C000000       LEA ECX,DWORD PTR DS:[ESI+0x8C]
	$+98     008F9971     E8 CA9FBBFF         CALL PathOfEx.004B3940
	$+9D     008F9976     3A9E 0C010000       CMP BL,BYTE PTR DS:[ESI+0x10C]
	$+A3     008F997C   ^ 72 B2               JB SHORT PathOfEx.008F9930
	$+A5     008F997E     5F                  POP EDI
	$+A6     008F997F     5E                  POP ESI
	$+A7     008F9980     5B                  POP EBX
	$+A8     008F9981     C2 0400             RETN 0x4
	$+AB     008F9984     CC                  INT3
	*/
	
	//gj3
	/*
	$-30C    0080406F     CC                   INT3
	$-30B    00804070     55                   PUSH EBP
	$-30A    00804071     8BEC                 MOV EBP,ESP
	$-308    00804073     64:A1 00000000       MOV EAX,DWORD PTR FS:[0]
	$-302    00804079     6A FF                PUSH -0x1
	$-300    0080407B     68 9391DA00          PUSH PathOfEx.00DA9193
	$-2FB    00804080     50                   PUSH EAX
	$-2FA    00804081     64:8925 00000000     MOV DWORD PTR FS:[0],ESP
	$-2F3    00804088     81EC 10020000        SUB ESP,0x210
	$-2ED    0080408E     53                   PUSH EBX
	$-2EC    0080408F     56                   PUSH ESI
	$-2EB    00804090     8B75 08              MOV ESI,DWORD PTR SS:[EBP+0x8]
	$-2E8    00804093     8BD9                 MOV EBX,ECX
	$-2E6    00804095     57                   PUSH EDI
	$-2E5    00804096     8B7D 0C              MOV EDI,DWORD PTR SS:[EBP+0xC]
	$-2E2    00804099     57                   PUSH EDI
	$-2E1    0080409A     56                   PUSH ESI
	$-2E0    0080409B     E8 E0FFEEFF          CALL PathOfEx.006F4080
	$-2DB    008040A0     84C0                 TEST AL,AL
	$-2D9    008040A2     75 41                JNZ SHORT PathOfEx.008040E5
	$-2D7    008040A4     BA C094F600          MOV EDX,PathOfEx.00F694C0
	$-2D2    008040A9     8BCE                 MOV ECX,ESI
	$-2D0    008040AB     E8 C0CAB5FF          CALL PathOfEx.00360B70
	$-2CB    008040B0     84C0                 TEST AL,AL
	$-2C9    008040B2     74 46                JE SHORT PathOfEx.008040FA
	$-2C7    008040B4     8B73 04              MOV ESI,DWORD PTR DS:[EBX+0x4]
	$-2C4    008040B7     83EC 18              SUB ESP,0x18
	$-2C1    008040BA     8BCC                 MOV ECX,ESP
	$-2BF    008040BC     33C0                 XOR EAX,EAX
	$-2BD    008040BE     6A FF                PUSH -0x1
	$-2BB    008040C0     50                   PUSH EAX
	$-2BA    008040C1     C741 14 07000000     MOV DWORD PTR DS:[ECX+0x14],0x7
	$-2B3    008040C8     C741 10 00000000     MOV DWORD PTR DS:[ECX+0x10],0x0
	$-2AC    008040CF     57                   PUSH EDI
	$-2AB    008040D0     66:8901              MOV WORD PTR DS:[ECX],AX
	$-2A8    008040D3     E8 0823B2FF          CALL PathOfEx.003263E0
	$-2A3    008040D8     8D53 3C              LEA EDX,DWORD PTR DS:[EBX+0x3C]
	$-2A0    008040DB     8BCE                 MOV ECX,ESI
	$-29E    008040DD     E8 DEA1EFFF          CALL PathOfEx.006FE2C0
	$-299    008040E2     83C4 18              ADD ESP,0x18
	$-296    008040E5     B0 01                MOV AL,0x1
	$-294    008040E7     8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-291    008040EA     64:890D 00000000     MOV DWORD PTR FS:[0],ECX
	$-28A    008040F1     5F                   POP EDI
	$-289    008040F2     5E                   POP ESI
	$-288    008040F3     5B                   POP EBX
	$-287    008040F4     8BE5                 MOV ESP,EBP
	$-285    008040F6     5D                   POP EBP
	$-284    008040F7     C2 0800              RETN 0x8
	$-281    008040FA     BA D494F600          MOV EDX,PathOfEx.00F694D4
	$-27C    008040FF     8BCE                 MOV ECX,ESI
	$-27A    00804101     E8 6ACAB5FF          CALL PathOfEx.00360B70
	$-275    00804106     84C0                 TEST AL,AL
	$-273    00804108     74 21                JE SHORT PathOfEx.0080412B
	$-271    0080410A     57                   PUSH EDI
	$-270    0080410B     FF73 04              PUSH DWORD PTR DS:[EBX+0x4]
	$-26D    0080410E     8D4B 48              LEA ECX,DWORD PTR DS:[EBX+0x48]
	$-26A    00804111     E8 BAE4C8FF          CALL PathOfEx.004925D0
	$-265    00804116     B0 01                MOV AL,0x1
	$-263    00804118     8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-260    0080411B     64:890D 00000000     MOV DWORD PTR FS:[0],ECX
	$-259    00804122     5F                   POP EDI
	$-258    00804123     5E                   POP ESI
	$-257    00804124     5B                   POP EBX
	$-256    00804125     8BE5                 MOV ESP,EBP
	$-254    00804127     5D                   POP EBP
	$-253    00804128     C2 0800              RETN 0x8
	$-250    0080412B     BA 68B5F500          MOV EDX,PathOfEx.00F5B568
	$-24B    00804130     8BCE                 MOV ECX,ESI
	$-249    00804132     E8 598EEEFF          CALL PathOfEx.006ECF90
	$-244    00804137     8BCE                 MOV ECX,ESI
	$-242    00804139     84C0                 TEST AL,AL
	$-240    0080413B     0F84 BF000000        JE PathOfEx.00804200
	$-23A    00804141     6A FF                PUSH -0x1
	$-238    00804143     6A 0D                PUSH 0xD
	$-236    00804145     8D45 DC              LEA EAX,DWORD PTR SS:[EBP-0x24]
	$-233    00804148     50                   PUSH EAX
	$-232    00804149     E8 A249C5FF          CALL PathOfEx.00458AF0
	$-22D    0080414E     8BC8                 MOV ECX,EAX
	$-22B    00804150     C745 FC 00000000     MOV DWORD PTR SS:[EBP-0x4],0x0
	$-224    00804157     E8 C491EEFF          CALL PathOfEx.006ED320
	$-21F    0080415C     8D4D DC              LEA ECX,DWORD PTR SS:[EBP-0x24]
	$-21C    0080415F     C745 FC FFFFFFFF     MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-215    00804166     8BF0                 MOV ESI,EAX
	$-213    00804168     E8 730DB2FF          CALL PathOfEx.00324EE0
	$-20E    0080416D     83FE 01              CMP ESI,0x1
	$-20B    00804170     73 2E                JNB SHORT PathOfEx.008041A0
	$-209    00804172     68 6CA6F500          PUSH PathOfEx.00F5A66C                     ; ASCII 43,"annot transition to that state"
	$-204    00804177     8D4D C4              LEA ECX,DWORD PTR SS:[EBP-0x3C]
	$-201    0080417A     E8 A114B2FF          CALL PathOfEx.00325620
	$-1FC    0080417F     8D45 C4              LEA EAX,DWORD PTR SS:[EBP-0x3C]
	$-1F9    00804182     C745 FC 01000000     MOV DWORD PTR SS:[EBP-0x4],0x1
	$-1F2    00804189     50                   PUSH EAX
	$-1F1    0080418A     8D4D E8              LEA ECX,DWORD PTR SS:[EBP-0x18]
	$-1EE    0080418D     E8 4E30B5FF          CALL PathOfEx.003571E0
	$-1E9    00804192     68 C0280501          PUSH PathOfEx.010528C0
	$-1E4    00804197     8D45 E8              LEA EAX,DWORD PTR SS:[EBP-0x18]
	$-1E1    0080419A     50                   PUSH EAX
	$-1E0    0080419B     E8 D3FB4D00          CALL PathOfEx.00CE3D73
	$-1DB    008041A0     0FB643 38            MOVZX EAX,BYTE PTR DS:[EBX+0x38]
	$-1D7    008041A4     3BF0                 CMP ESI,EAX
	$-1D5    008041A6     76 2E                JBE SHORT PathOfEx.008041D6
	$-1D3    008041A8     68 20A7F500          PUSH PathOfEx.00F5A720
	$-1CE    008041AD     8D4D C4              LEA ECX,DWORD PTR SS:[EBP-0x3C]
	$-1CB    008041B0     E8 6B14B2FF          CALL PathOfEx.00325620
	$-1C6    008041B5     8D45 C4              LEA EAX,DWORD PTR SS:[EBP-0x3C]
	$-1C3    008041B8     C745 FC 02000000     MOV DWORD PTR SS:[EBP-0x4],0x2
	$-1BC    008041BF     50                   PUSH EAX
	$-1BB    008041C0     8D4D E8              LEA ECX,DWORD PTR SS:[EBP-0x18]
	$-1B8    008041C3     E8 1830B5FF          CALL PathOfEx.003571E0
	$-1B3    008041C8     68 C0280501          PUSH PathOfEx.010528C0
	$-1AE    008041CD     8D45 E8              LEA EAX,DWORD PTR SS:[EBP-0x18]
	$-1AB    008041D0     50                   PUSH EAX
	$-1AA    008041D1     E8 9DFB4D00          CALL PathOfEx.00CE3D73
	$-1A5    008041D6     8B43 54              MOV EAX,DWORD PTR DS:[EBX+0x54]
	$-1A2    008041D9     8D4E FF              LEA ECX,DWORD PTR DS:[ESI-0x1]
	$-19F    008041DC     57                   PUSH EDI
	$-19E    008041DD     FF73 04              PUSH DWORD PTR DS:[EBX+0x4]
	$-19B    008041E0     8D0C49               LEA ECX,DWORD PTR DS:[ECX+ECX*2]
	$-198    008041E3     8D0C88               LEA ECX,DWORD PTR DS:[EAX+ECX*4]
	$-195    008041E6     E8 E5E3C8FF          CALL PathOfEx.004925D0
	$-190    008041EB     B0 01                MOV AL,0x1
	$-18E    008041ED     8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-18B    008041F0     64:890D 00000000     MOV DWORD PTR FS:[0],ECX
	$-184    008041F7     5F                   POP EDI
	$-183    008041F8     5E                   POP ESI
	$-182    008041F9     5B                   POP EBX
	$-181    008041FA     8BE5                 MOV ESP,EBP
	$-17F    008041FC     5D                   POP EBP
	$-17E    008041FD     C2 0800              RETN 0x8
	$-17B    00804200     BA EC94F600          MOV EDX,PathOfEx.00F694EC
	$-176    00804205     E8 66C9B5FF          CALL PathOfEx.00360B70
	$-171    0080420A     84C0                 TEST AL,AL
	$-16F    0080420C     74 62                JE SHORT PathOfEx.00804270
	$-16D    0080420E     837F 14 08           CMP DWORD PTR DS:[EDI+0x14],0x8
	$-169    00804212     72 04                JB SHORT PathOfEx.00804218
	$-167    00804214     8B0F                 MOV ECX,DWORD PTR DS:[EDI]
	$-165    00804216     EB 02                JMP SHORT PathOfEx.0080421A
	$-163    00804218     8BCF                 MOV ECX,EDI
	$-161    0080421A     E8 B119EAFF          CALL PathOfEx.006A5BD0
	$-15C    0080421F     8943 60              MOV DWORD PTR DS:[EBX+0x60],EAX
	$-159    00804222     3D E9050000          CMP EAX,0x5E9
	$-154    00804227   ^ 0F85 B8FEFFFF        JNZ PathOfEx.008040E5
	$-14E    0080422D     8D8D E4FDFFFF        LEA ECX,DWORD PTR SS:[EBP-0x21C]
	$-148    00804233     E8 38C5E0FF          CALL PathOfEx.00610770
	$-143    00804238     57                   PUSH EDI
	$-142    00804239     68 DCAEF500          PUSH PathOfEx.00F5AEDC                     ; UNICODE "Not a valid quest flag: "
	$-13D    0080423E     8BC8                 MOV ECX,EAX
	$-13B    00804240     C745 FC 03000000     MOV DWORD PTR SS:[EBP-0x4],0x3
	$-134    00804247     E8 B478BFFF          CALL PathOfEx.003FBB00
	$-12F    0080424C     8BC8                 MOV ECX,EAX
	$-12D    0080424E     E8 AD7CD7FF          CALL PathOfEx.0057BF00
	$-128    00804253     50                   PUSH EAX
	$-127    00804254     8D8D D4FEFFFF        LEA ECX,DWORD PTR SS:[EBP-0x12C]
	$-121    0080425A     E8 6188BFFF          CALL PathOfEx.003FCAC0
	$-11C    0080425F     68 90290501          PUSH PathOfEx.01052990
	$-117    00804264     8D85 D4FEFFFF        LEA EAX,DWORD PTR SS:[EBP-0x12C]
	$-111    0080426A     50                   PUSH EAX
	$-110    0080426B     E8 03FB4D00          CALL PathOfEx.00CE3D73
	$-10B    00804270     8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-108    00804273     32C0                 XOR AL,AL
	$-106    00804275     5F                   POP EDI
	$-105    00804276     5E                   POP ESI
	$-104    00804277     64:890D 00000000     MOV DWORD PTR FS:[0],ECX
	$-FD     0080427E     5B                   POP EBX
	$-FC     0080427F     8BE5                 MOV ESP,EBP
	$-FA     00804281     5D                   POP EBP
	$-F9     00804282     C2 0800              RETN 0x8
	$-F6     00804285     CC                   INT3
	$-F5     00804286     CC                   INT3
	$-F4     00804287     CC                   INT3
	$-F3     00804288     CC                   INT3
	$-F2     00804289     CC                   INT3
	$-F1     0080428A     CC                   INT3
	$-F0     0080428B     CC                   INT3
	$-EF     0080428C     CC                   INT3
	$-EE     0080428D     CC                   INT3
	$-ED     0080428E     CC                   INT3
	$-EC     0080428F     CC                   INT3
	$-EB     00804290     56                   PUSH ESI
	$-EA     00804291     FF7424 0C            PUSH DWORD PTR SS:[ESP+0xC]
	$-E6     00804295     8BF1                 MOV ESI,ECX
	$-E4     00804297     FF7424 0C            PUSH DWORD PTR SS:[ESP+0xC]
	$-E0     0080429B     E8 9000EFFF          CALL PathOfEx.006F4330
	$-DB     008042A0     84C0                 TEST AL,AL
	$-D9     008042A2     74 31                JE SHORT PathOfEx.008042D5
	$-D7     008042A4     8B4E 58              MOV ECX,DWORD PTR DS:[ESI+0x58]
	$-D4     008042A7     B8 ABAAAA2A          MOV EAX,0x2AAAAAAB
	$-CF     008042AC     2B4E 54              SUB ECX,DWORD PTR DS:[ESI+0x54]
	$-CC     008042AF     57                   PUSH EDI
	$-CB     008042B0     8D7E 54              LEA EDI,DWORD PTR DS:[ESI+0x54]
	$-C8     008042B3     0FB676 38            MOVZX ESI,BYTE PTR DS:[ESI+0x38]
	$-C4     008042B7     F7E9                 IMUL ECX
	$-C2     008042B9     D1FA                 SAR EDX,1
	$-C0     008042BB     8BC2                 MOV EAX,EDX
	$-BE     008042BD     C1E8 1F              SHR EAX,0x1F
	$-BB     008042C0     03C2                 ADD EAX,EDX
	$-B9     008042C2     3BF0                 CMP ESI,EAX
	$-B7     008042C4     76 08                JBE SHORT PathOfEx.008042CE
	$-B5     008042C6     56                   PUSH ESI
	$-B4     008042C7     8BCF                 MOV ECX,EDI
	$-B2     008042C9     E8 327EEEFF          CALL PathOfEx.006EC100
	$-AD     008042CE     5F                   POP EDI
	$-AC     008042CF     B0 01                MOV AL,0x1
	$-AA     008042D1     5E                   POP ESI
	$-A9     008042D2     C2 0800              RETN 0x8
	$-A6     008042D5     32C0                 XOR AL,AL
	$-A4     008042D7     5E                   POP ESI
	$-A3     008042D8     C2 0800              RETN 0x8
	$-A0     008042DB     CC                   INT3
	$-9F     008042DC     CC                   INT3
	$-9E     008042DD     CC                   INT3
	$-9D     008042DE     CC                   INT3
	$-9C     008042DF     CC                   INT3
	$-9B     008042E0     56                   PUSH ESI
	$-9A     008042E1     8BF1                 MOV ESI,ECX
	$-98     008042E3     E8 1803EFFF          CALL PathOfEx.006F4600
	$-93     008042E8     F64424 08 01         TEST BYTE PTR SS:[ESP+0x8],0x1
	$-8E     008042ED     74 0E                JE SHORT PathOfEx.008042FD
	$-8C     008042EF     68 18010000          PUSH 0x118
	$-87     008042F4     56                   PUSH ESI
	$-86     008042F5     E8 035F4C00          CALL PathOfEx.00CCA1FD
	$-81     008042FA     83C4 08              ADD ESP,0x8
	$-7E     008042FD     8BC6                 MOV EAX,ESI
	$-7C     008042FF     5E                   POP ESI
	$-7B     00804300     C2 0400              RETN 0x4
	$-78     00804303     CC                   INT3
	$-77     00804304     CC                   INT3
	$-76     00804305     CC                   INT3
	$-75     00804306     CC                   INT3
	$-74     00804307     CC                   INT3
	$-73     00804308     CC                   INT3
	$-72     00804309     CC                   INT3
	$-71     0080430A     CC                   INT3
	$-70     0080430B     CC                   INT3
	$-6F     0080430C     CC                   INT3
	$-6E     0080430D     CC                   INT3
	$-6D     0080430E     CC                   INT3
	$-6C     0080430F     CC                   INT3
	$-6B     00804310     53                   PUSH EBX                                   ; tag_点击副本里的门会断到,改变Transitionable属性对象中的状态为已打开
	$-6A     00804311     56                   PUSH ESI
	$-69     00804312     57                   PUSH EDI
	$-68     00804313     8B7C24 10            MOV EDI,DWORD PTR SS:[ESP+0x10]
	$-64     00804317     8BF1                 MOV ESI,ECX
	$-62     00804319     6A 01                PUSH 0x1
	$-60     0080431B     8BCF                 MOV ECX,EDI
	$-5E     0080431D     E8 AE91B5FF          CALL PathOfEx.0035D4D0
	$-59     00804322     8B4F 08              MOV ECX,DWORD PTR DS:[EDI+0x8]
	$-56     00804325     8B07                 MOV EAX,DWORD PTR DS:[EDI]
	$-54     00804327     8A1C01               MOV BL,BYTE PTR DS:[ECX+EAX]
	$-51     0080432A     8D41 01              LEA EAX,DWORD PTR DS:[ECX+0x1]
	$-4E     0080432D     8947 08              MOV DWORD PTR DS:[EDI+0x8],EAX
	$-4B     00804330     8B86 14010000        MOV EAX,DWORD PTR DS:[ESI+0x114]
	$-45     00804336     8178 60 E9050000     CMP DWORD PTR DS:[EAX+0x60],0x5E9
	$-3E     0080433D     74 3C                JE SHORT PathOfEx.0080437B
	$-3C     0080433F     8B4E 04              MOV ECX,DWORD PTR DS:[ESI+0x4]
	$-39     00804342     E8 49F4EFFF          CALL PathOfEx.00703790
	$-34     00804347     85C0                 TEST EAX,EAX
	$-32     00804349     74 30                JE SHORT PathOfEx.0080437B
	$-30     0080434B     8B46 04              MOV EAX,DWORD PTR DS:[ESI+0x4]
	$-2D     0080434E     8B40 1C              MOV EAX,DWORD PTR DS:[EAX+0x1C]
	$-2A     00804351     8B88 58010000        MOV ECX,DWORD PTR DS:[EAX+0x158]
	$-24     00804357     85C9                 TEST ECX,ECX
	$-22     00804359     74 20                JE SHORT PathOfEx.0080437B
	$-20     0080435B     E8 0061B4FF          CALL PathOfEx.0034A460
	$-1B     00804360     85C0                 TEST EAX,EAX
	$-19     00804362     74 17                JE SHORT PathOfEx.0080437B
	$-17     00804364     8BBE 14010000        MOV EDI,DWORD PTR DS:[ESI+0x114]
	$-11     0080436A     8BC8                 MOV ECX,EAX
	$-F      0080436C     FF77 60              PUSH DWORD PTR DS:[EDI+0x60]
	$-C      0080436F     E8 6CF6BAFF          CALL PathOfEx.003B39E0
	$-7      00804374     84C0                 TEST AL,AL
	$-5      00804376     74 03                JE SHORT PathOfEx.0080437B
	$-3      00804378     8A5F 38              MOV BL,BYTE PTR DS:[EDI+0x38]
	$ ==>    0080437B     3A9E 0C010000        CMP BL,BYTE PTR DS:[ESI+0x10C]             ; g_o_transitionable_state_
	$+6      00804381     76 15                JBE SHORT PathOfEx.00804398
	$+8      00804383     FE86 0C010000        INC BYTE PTR DS:[ESI+0x10C]
	$+E      00804389     8BCE                 MOV ECX,ESI
	$+10     0080438B     E8 E002EFFF          CALL PathOfEx.006F4670
	$+15     00804390     3A9E 0C010000        CMP BL,BYTE PTR DS:[ESI+0x10C]
	$+1B     00804396   ^ 77 EB                JA SHORT PathOfEx.00804383
	$+1D     00804398     73 1B                JNB SHORT PathOfEx.008043B5
	$+1F     0080439A     66:0F1F4400 00       NOP WORD PTR DS:[EAX+EAX]
	$+25     008043A0     FE8E 0C010000        DEC BYTE PTR DS:[ESI+0x10C]
	$+2B     008043A6     8BCE                 MOV ECX,ESI
	$+2D     008043A8     E8 5303EFFF          CALL PathOfEx.006F4700
	$+32     008043AD     3A9E 0C010000        CMP BL,BYTE PTR DS:[ESI+0x10C]
	$+38     008043B3   ^ 72 EB                JB SHORT PathOfEx.008043A0
	$+3A     008043B5     5F                   POP EDI
	$+3B     008043B6     5E                   POP ESI
	$+3C     008043B7     5B                   POP EBX
	$+3D     008043B8     C2 0400              RETN 0x4
	$+40     008043BB     CC                   INT3
	$+41     008043BC     CC                   INT3
	$+42     008043BD     CC                   INT3
	$+43     008043BE     CC                   INT3
	$+44     008043BF     CC                   INT3
	$+45     008043C0     53                   PUSH EBX
	$+46     008043C1     56                   PUSH ESI
	$+47     008043C2     8B7424 0C            MOV ESI,DWORD PTR SS:[ESP+0xC]
	$+4B     008043C6     57                   PUSH EDI
	$+4C     008043C7     8BF9                 MOV EDI,ECX
	$+4E     008043C9     8BCE                 MOV ECX,ESI
	$+50     008043CB     6A 01                PUSH 0x1
	$+52     008043CD     E8 FE90B5FF          CALL PathOfEx.0035D4D0
	$+57     008043D2     8B56 08              MOV EDX,DWORD PTR DS:[ESI+0x8]
	$+5A     008043D5     8B06                 MOV EAX,DWORD PTR DS:[ESI]
	$+5C     008043D7     8A1C02               MOV BL,BYTE PTR DS:[EDX+EAX]
	$+5F     008043DA     8D42 01              LEA EAX,DWORD PTR DS:[EDX+0x1]
	$+62     008043DD     8946 08              MOV DWORD PTR DS:[ESI+0x8],EAX
	$+65     008043E0     8AC3                 MOV AL,BL
	$+67     008043E2     C0E8 07              SHR AL,0x7
	$+6A     008043E5     80E3 7F              AND BL,0x7F
	$+6D     008043E8     8887 0E010000        MOV BYTE PTR DS:[EDI+0x10E],AL
	$+73     008043EE     8B87 14010000        MOV EAX,DWORD PTR DS:[EDI+0x114]
	$+79     008043F4     889F 0C010000        MOV BYTE PTR DS:[EDI+0x10C],BL
	$+7F     008043FA     3A58 38              CMP BL,BYTE PTR DS:[EAX+0x38]
	$+82     008043FD     73 1A                JNB SHORT PathOfEx.00804419
	$+84     008043FF     8BCF                 MOV ECX,EDI
	$+86     00804401     E8 9A000000          CALL PathOfEx.008044A0
	$+8B     00804406     84C0                 TEST AL,AL
	$+8D     00804408     74 0F                JE SHORT PathOfEx.00804419
	$+8F     0080440A     8B87 14010000        MOV EAX,DWORD PTR DS:[EDI+0x114]
	$+95     00804410     8A40 38              MOV AL,BYTE PTR DS:[EAX+0x38]
	$+98     00804413     8887 0C010000        MOV BYTE PTR DS:[EDI+0x10C],AL
	$+9E     00804419     5F                   POP EDI
	$+9F     0080441A     5E                   POP ESI
	$+A0     0080441B     5B                   POP EBX
	$+A1     0080441C     C2 0400              RETN 0x4
	$+A4     0080441F     CC                   INT3
	$+A5     00804420     56                   PUSH ESI
	$+A6     00804421     8BF1                 MOV ESI,ECX
	$+A8     00804423     8B8E 14010000        MOV ECX,DWORD PTR DS:[ESI+0x114]
	$+AE     00804429     8A86 0C010000        MOV AL,BYTE PTR DS:[ESI+0x10C]
	$+B4     0080442F     3A41 38              CMP AL,BYTE PTR DS:[ECX+0x38]
	$+B7     00804432     73 61                JNB SHORT PathOfEx.00804495
	$+B9     00804434     8BCE                 MOV ECX,ESI
	$+BB     00804436     E8 65000000          CALL PathOfEx.008044A0
	$+C0     0080443B     84C0                 TEST AL,AL
	$+C2     0080443D     74 56                JE SHORT PathOfEx.00804495
	$+C4     0080443F     8B8E 14010000        MOV ECX,DWORD PTR DS:[ESI+0x114]
	$+CA     00804445     8A86 0C010000        MOV AL,BYTE PTR DS:[ESI+0x10C]
	$+D0     0080444B     3A41 38              CMP AL,BYTE PTR DS:[ECX+0x38]
	$+D3     0080444E     73 45                JNB SHORT PathOfEx.00804495
	$+D5     00804450     8B86 10010000        MOV EAX,DWORD PTR DS:[ESI+0x110]
	$+DB     00804456     8A8E 0C010000        MOV CL,BYTE PTR DS:[ESI+0x10C]
	$+E1     0080445C     3A48 38              CMP CL,BYTE PTR DS:[EAX+0x38]
	$+E4     0080445F     73 23                JNB SHORT PathOfEx.00804484
	$+E6     00804461     80BE 0F010000 00     CMP BYTE PTR DS:[ESI+0x10F],0x0
	$+ED     00804468     75 1A                JNZ SHORT PathOfEx.00804484
	$+EF     0080446A     FEC1                 INC CL
	$+F1     0080446C     C686 0E010000 01     MOV BYTE PTR DS:[ESI+0x10E],0x1
	$+F8     00804473     888E 0C010000        MOV BYTE PTR DS:[ESI+0x10C],CL
	$+FE     00804479     8BCE                 MOV ECX,ESI
	$+100    0080447B     C646 08 01           MOV BYTE PTR DS:[ESI+0x8],0x1
	$+104    0080447F     E8 EC01EFFF          CALL PathOfEx.006F4670
	$+109    00804484     8B8E 14010000        MOV ECX,DWORD PTR DS:[ESI+0x114]
	$+10F    0080448A     8A86 0C010000        MOV AL,BYTE PTR DS:[ESI+0x10C]
	$+115    00804490     3A41 38              CMP AL,BYTE PTR DS:[ECX+0x38]
	$+118    00804493   ^ 72 BB                JB SHORT PathOfEx.00804450
	$+11A    00804495     5E                   POP ESI
	$+11B    00804496     C3                   RETN
	$+11C    00804497     CC                   INT3
	$+11D    00804498     CC                   INT3
	$+11E    00804499     CC                   INT3
	$+11F    0080449A     CC                   INT3
	$+120    0080449B     CC                   INT3
	$+121    0080449C     CC                   INT3
	$+122    0080449D     CC                   INT3
	$+123    0080449E     CC                   INT3
	$+124    0080449F     CC                   INT3
	$+125    008044A0     56                   PUSH ESI
	$+126    008044A1     8BF1                 MOV ESI,ECX
	$+128    008044A3     8B86 14010000        MOV EAX,DWORD PTR DS:[ESI+0x114]
	$+12E    008044A9     8178 60 E9050000     CMP DWORD PTR DS:[EAX+0x60],0x5E9
	$+135    008044B0     74 37                JE SHORT PathOfEx.008044E9
	$+137    008044B2     8B4E 04              MOV ECX,DWORD PTR DS:[ESI+0x4]
	$+13A    008044B5     E8 D6F2EFFF          CALL PathOfEx.00703790
	$+13F    008044BA     85C0                 TEST EAX,EAX
	$+141    008044BC     74 2B                JE SHORT PathOfEx.008044E9
	$+143    008044BE     8B46 04              MOV EAX,DWORD PTR DS:[ESI+0x4]
	$+146    008044C1     8B40 1C              MOV EAX,DWORD PTR DS:[EAX+0x1C]
	$+149    008044C4     8B88 58010000        MOV ECX,DWORD PTR DS:[EAX+0x158]
	$+14F    008044CA     85C9                 TEST ECX,ECX
	$+151    008044CC     74 1B                JE SHORT PathOfEx.008044E9
	$+153    008044CE     E8 8D5FB4FF          CALL PathOfEx.0034A460
	$+158    008044D3     85C0                 TEST EAX,EAX
	$+15A    008044D5     74 12                JE SHORT PathOfEx.008044E9
	$+15C    008044D7     8B8E 14010000        MOV ECX,DWORD PTR DS:[ESI+0x114]
	$+162    008044DD     FF71 60              PUSH DWORD PTR DS:[ECX+0x60]
	$+165    008044E0     8BC8                 MOV ECX,EAX
	$+167    008044E2     E8 F9F4BAFF          CALL PathOfEx.003B39E0
	$+16C    008044E7     5E                   POP ESI
	$+16D    008044E8     C3                   RETN
	$+16E    008044E9     32C0                 XOR AL,AL
	$+170    008044EB     5E                   POP ESI
	$+171    008044EC     C3                   RETN
	$+172    008044ED     CC                   INT3
	$+173    008044EE     CC                   INT3
	$+174    008044EF     CC                   INT3
	$+175    008044F0     53                   PUSH EBX
	$+176    008044F1     56                   PUSH ESI
	$+177    008044F2     8BF1                 MOV ESI,ECX
	$+179    008044F4     57                   PUSH EDI
	$+17A    008044F5     80BE 0E010000 00     CMP BYTE PTR DS:[ESI+0x10E],0x0
	$+181    008044FC     74 17                JE SHORT PathOfEx.00804515
	$+183    008044FE     E8 6D01EFFF          CALL PathOfEx.006F4670
	$+188    00804503     80BE 0C010000 01     CMP BYTE PTR DS:[ESI+0x10C],0x1
	$+18F    0080450A     75 2A                JNZ SHORT PathOfEx.00804536
	$+191    0080450C     8BCE                 MOV ECX,ESI
	$+193    0080450E     E8 ED01EFFF          CALL PathOfEx.006F4700
	$+198    00804513     EB 21                JMP SHORT PathOfEx.00804536
	$+19A    00804515     8B8E 14010000        MOV ECX,DWORD PTR DS:[ESI+0x114]
	$+1A0    0080451B     8A86 0C010000        MOV AL,BYTE PTR DS:[ESI+0x10C]
	$+1A6    00804521     FF76 04              PUSH DWORD PTR DS:[ESI+0x4]
	$+1A9    00804524     3A41 38              CMP AL,BYTE PTR DS:[ECX+0x38]
	$+1AC    00804527     75 05                JNZ SHORT PathOfEx.0080452E
	$+1AE    00804529     83C1 3C              ADD ECX,0x3C
	$+1B1    0080452C     EB 03                JMP SHORT PathOfEx.00804531
	$+1B3    0080452E     83C1 48              ADD ECX,0x48
	$+1B6    00804531     E8 6A15F1FF          CALL PathOfEx.00715AA0
	$+1BB    00804536     0FB686 0C010000      MOVZX EAX,BYTE PTR DS:[ESI+0x10C]
	$+1C2    0080453D     8B96 14010000        MOV EDX,DWORD PTR DS:[ESI+0x114]
	$+1C8    00804543     8B5E 04              MOV EBX,DWORD PTR DS:[ESI+0x4]
	$+1CB    00804546     8D0C40               LEA ECX,DWORD PTR DS:[EAX+EAX*2]
	$+1CE    00804549     8B42 54              MOV EAX,DWORD PTR DS:[EDX+0x54]
	$+1D1    0080454C     8B7C88 F8            MOV EDI,DWORD PTR DS:[EAX+ECX*4-0x8]
	$+1D5    00804550     8B7488 F4            MOV ESI,DWORD PTR DS:[EAX+ECX*4-0xC]
	$+1D9    00804554     3BF7                 CMP ESI,EDI
	$+1DB    00804556     74 13                JE SHORT PathOfEx.0080456B
	$+1DD    00804558     53                   PUSH EBX
	$+1DE    00804559     8D46 08              LEA EAX,DWORD PTR DS:[ESI+0x8]
	$+1E1    0080455C     50                   PUSH EAX
	$+1E2    0080455D     FF36                 PUSH DWORD PTR DS:[ESI]
	$+1E4    0080455F     E8 6C14F1FF          CALL PathOfEx.007159D0
	$+1E9    00804564     83C6 30              ADD ESI,0x30
	$+1EC    00804567     3BF7                 CMP ESI,EDI
	$+1EE    00804569   ^ 75 ED                JNZ SHORT PathOfEx.00804558
	$+1F0    0080456B     5F                   POP EDI
	$+1F1    0080456C     5E                   POP ESI
	$+1F2    0080456D     5B                   POP EBX
	$+1F3    0080456E     C3                   RETN
	$+1F4    0080456F     CC                   INT3
	$+1F5    00804570     56                   PUSH ESI
	$+1F6    00804571     6A 0C                PUSH 0xC
	$+1F8    00804573     8BF1                 MOV ESI,ECX
	$+1FA    00804575     E8 915C4C00          CALL PathOfEx.00CCA20B
	$+1FF    0080457A     83C4 04              ADD ESP,0x4
	$+202    0080457D     C700 0080F600        MOV DWORD PTR DS:[EAX],PathOfEx.00F68000
	$+208    00804583     66:8B56 04           MOV DX,WORD PTR DS:[ESI+0x4]
	$+20C    00804587     66:8950 04           MOV WORD PTR DS:[EAX+0x4],DX
	$+210    0080458B     C700 38C2F600        MOV DWORD PTR DS:[EAX],PathOfEx.00F6C238
	$+216    00804591     8A4E 08              MOV CL,BYTE PTR DS:[ESI+0x8]
	$+219    00804594     8848 08              MOV BYTE PTR DS:[EAX+0x8],CL
	$+21C    00804597     5E                   POP ESI
	$+21D    00804598     C3                   RETN
	$+21E    00804599     CC                   INT3
	$+21F    0080459A     CC                   INT3
	$+220    0080459B     CC                   INT3
	$+221    0080459C     CC                   INT3
	$+222    0080459D     CC                   INT3
	$+223    0080459E     CC                   INT3
	$+224    0080459F     CC                   INT3
	$+225    008045A0     B8 03000000          MOV EAX,0x3
	$+22A    008045A5     C701 2CC2F600        MOV DWORD PTR DS:[ECX],PathOfEx.00F6C22C
	$+230    008045AB     66:8941 04           MOV WORD PTR DS:[ECX+0x4],AX
	$+234    008045AF     33C0                 XOR EAX,EAX
	$+236    008045B1     C741 1C 07000000     MOV DWORD PTR DS:[ECX+0x1C],0x7
	$+23D    008045B8     C741 18 00000000     MOV DWORD PTR DS:[ECX+0x18],0x0
	$+244    008045BF     66:8941 08           MOV WORD PTR DS:[ECX+0x8],AX
	$+248    008045C3     8BC1                 MOV EAX,ECX
	$+24A    008045C5     C3                   RETN
	$+24B    008045C6     CC                   INT3
	$+24C    008045C7     CC                   INT3
	$+24D    008045C8     CC                   INT3
	$+24E    008045C9     CC                   INT3
	$+24F    008045CA     CC                   INT3
	$+250    008045CB     CC                   INT3
	$+251    008045CC     CC                   INT3
	$+252    008045CD     CC                   INT3
	$+253    008045CE     CC                   INT3
	$+254    008045CF     CC                   INT3
	$+255    008045D0     55                   PUSH EBP
	$+256    008045D1     8BEC                 MOV EBP,ESP
	$+258    008045D3     6A FF                PUSH -0x1
	$+25A    008045D5     68 C75FDA00          PUSH PathOfEx.00DA5FC7
	$+25F    008045DA     64:A1 00000000       MOV EAX,DWORD PTR FS:[0]
	$+265    008045E0     50                   PUSH EAX
	$+266    008045E1     64:8925 00000000     MOV DWORD PTR FS:[0],ESP
	$+26D    008045E8     51                   PUSH ECX
	$+26E    008045E9     56                   PUSH ESI
	$+26F    008045EA     57                   PUSH EDI
	$+270    008045EB     6A 20                PUSH 0x20
	$+272    008045ED     8BF1                 MOV ESI,ECX
	$+274    008045EF     E8 175C4C00          CALL PathOfEx.00CCA20B
	$+279    008045F4     8BF8                 MOV EDI,EAX
	$+27B    008045F6     83C4 04              ADD ESP,0x4
	$+27E    008045F9     897D F0              MOV DWORD PTR SS:[EBP-0x10],EDI
	$+281    008045FC     C745 FC 00000000     MOV DWORD PTR SS:[EBP-0x4],0x0
	$+288    00804603     C707 0080F600        MOV DWORD PTR DS:[EDI],PathOfEx.00F68000
	$+28E    00804609     66:8B4E 04           MOV CX,WORD PTR DS:[ESI+0x4]
	$+292    0080460D     66:894F 04           MOV WORD PTR DS:[EDI+0x4],CX
	$+296    00804611     C645 FC 01           MOV BYTE PTR SS:[EBP-0x4],0x1
	$+29A    00804615     8D4F 08              LEA ECX,DWORD PTR DS:[EDI+0x8]
	$+29D    00804618     33D2                 XOR EDX,EDX
	$+29F    0080461A     C707 2CC2F600        MOV DWORD PTR DS:[EDI],PathOfEx.00F6C22C
	$+2A5    00804620     6A FF                PUSH -0x1
	$+2A7    00804622     8D46 08              LEA EAX,DWORD PTR DS:[ESI+0x8]
	$+2AA    00804625     C741 14 07000000     MOV DWORD PTR DS:[ECX+0x14],0x7
	$+2B1    0080462C     52                   PUSH EDX
	$+2B2    0080462D     C741 10 00000000     MOV DWORD PTR DS:[ECX+0x10],0x0
	$+2B9    00804634     50                   PUSH EAX
	$+2BA    00804635     66:8911              MOV WORD PTR DS:[ECX],DX
	$+2BD    00804638     E8 A31DB2FF          CALL PathOfEx.003263E0
	$+2C2    0080463D     8B4D F4              MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+2C5    00804640     8BC7                 MOV EAX,EDI
	$+2C7    00804642     5F                   POP EDI
	$+2C8    00804643     5E                   POP ESI
	$+2C9    00804644     64:890D 00000000     MOV DWORD PTR FS:[0],ECX
	$+2D0    0080464B     8BE5                 MOV ESP,EBP
	$+2D2    0080464D     5D                   POP EBP
	$+2D3    0080464E     C3                   RETN
	$+2D4    0080464F     CC                   INT3
	$+2D5    00804650     56                   PUSH ESI
	$+2D6    00804651     6A 08                PUSH 0x8
	$+2D8    00804653     8BF1                 MOV ESI,ECX
	$+2DA    00804655     E8 B15B4C00          CALL PathOfEx.00CCA20B
	$+2DF    0080465A     83C4 04              ADD ESP,0x4
	$+2E2    0080465D     C700 0080F600        MOV DWORD PTR DS:[EAX],PathOfEx.00F68000
	$+2E8    00804663     66:8B4E 04           MOV CX,WORD PTR DS:[ESI+0x4]
	$+2EC    00804667     66:8948 04           MOV WORD PTR DS:[EAX+0x4],CX
	$+2F0    0080466B     C700 20C2F600        MOV DWORD PTR DS:[EAX],PathOfEx.00F6C220
	$+2F6    00804671     5E                   POP ESI
	$+2F7    00804672     C3                   RETN
	$+2F8    00804673     CC                   INT3
	*/
#ifdef VER_GJ3_
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("3a??????????76??fe??????????8b??e8????????3a??????????77", 2);
#else
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("3a??????????76??fe??????????8b??e8????????3a??????????77", 2);
#endif
	if (!op_dw_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *op_dw_tmp;
	assert(dw_tmp > 0xa0 && dw_tmp < 0x150);
	ofs << "pt_cdword	g_o_transitionable_state_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	/*
	$-221    01846CCF     CC                    INT3
	$-220    01846CD0     55                    PUSH EBP
	$-21F    01846CD1     8BEC                  MOV EBP,ESP
	$-21D    01846CD3     64:A1 00000000        MOV EAX,DWORD PTR FS:[0]
	$-217    01846CD9     6A FF                 PUSH -0x1
	$-215    01846CDB     68 E897CD01           PUSH PathOfEx.01CD97E8
	$-210    01846CE0     50                    PUSH EAX
	$-20F    01846CE1     64:8925 00000000      MOV DWORD PTR FS:[0],ESP
	$-208    01846CE8     83EC 20               SUB ESP,0x20
	$-205    01846CEB     53                    PUSH EBX
	$-204    01846CEC     8B5D 08               MOV EBX,DWORD PTR SS:[EBP+0x8]
	$-201    01846CEF     56                    PUSH ESI
	$-200    01846CF0     57                    PUSH EDI
	$-1FF    01846CF1     8BF9                  MOV EDI,ECX
	$-1FD    01846CF3     8B0B                  MOV ECX,DWORD PTR DS:[EBX]
	$-1FB    01846CF5     85C9                  TEST ECX,ECX
	$-1F9    01846CF7     74 05                 JE SHORT PathOfEx.01846CFE
	$-1F7    01846CF9     E8 12F8E3FF           CALL PathOfEx.01686510
	$-1F2    01846CFE     8B03                  MOV EAX,DWORD PTR DS:[EBX]
	$-1F0    01846D00     C745 08 789CEA01      MOV DWORD PTR SS:[EBP+0x8],PathOfEx.01EA9C78    ; Player
	$-1E9    01846D07     8B70 30               MOV ESI,DWORD PTR DS:[EAX+0x30]
	$-1E6    01846D0A     8D45 08               LEA EAX,DWORD PTR SS:[EBP+0x8]
	$-1E3    01846D0D     50                    PUSH EAX
	$-1E2    01846D0E     8D45 F0               LEA EAX,DWORD PTR SS:[EBP-0x10]
	$-1DF    01846D11     50                    PUSH EAX
	$-1DE    01846D12     8D4E 14               LEA ECX,DWORD PTR DS:[ESI+0x14]
	$-1DB    01846D15     E8 A61BBAFF           CALL PathOfEx.013E88C0
	$-1D6    01846D1A     8B45 F0               MOV EAX,DWORD PTR SS:[EBP-0x10]
	$-1D3    01846D1D     3B46 18               CMP EAX,DWORD PTR DS:[ESI+0x18]
	$-1D0    01846D20     0F84 C2000000         JE PathOfEx.01846DE8
	$-1CA    01846D26     8B48 0C               MOV ECX,DWORD PTR DS:[EAX+0xC]
	$-1C7    01846D29     83F9 FF               CMP ECX,-0x1
	$-1C4    01846D2C     0F84 B6000000         JE PathOfEx.01846DE8
	$-1BE    01846D32     8B43 04               MOV EAX,DWORD PTR DS:[EBX+0x4]
	$-1BB    01846D35     8B3488                MOV ESI,DWORD PTR DS:[EAX+ECX*4]
	$-1B8    01846D38     85F6                  TEST ESI,ESI
	$-1B6    01846D3A     0F84 A8000000         JE PathOfEx.01846DE8
	$-1B0    01846D40     8B43 14               MOV EAX,DWORD PTR DS:[EBX+0x14]
	$-1AD    01846D43     3B87 AC3A0000         CMP EAX,DWORD PTR DS:[EDI+0x3AAC]
	$-1A7    01846D49     0F84 D1000000         JE PathOfEx.01846E20
	$-1A1    01846D4F     33C9                  XOR ECX,ECX
	$-19F    01846D51     C745 E8 07000000      MOV DWORD PTR SS:[EBP-0x18],0x7
	$-198    01846D58     6A FF                 PUSH -0x1
	$-196    01846D5A     51                    PUSH ECX
	$-195    01846D5B     8D46 10               LEA EAX,DWORD PTR DS:[ESI+0x10]
	$-192    01846D5E     66:894D D4            MOV WORD PTR SS:[EBP-0x2C],CX
	$-18E    01846D62     50                    PUSH EAX
	$-18D    01846D63     8D4D D4               LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$-18A    01846D66     C745 E4 00000000      MOV DWORD PTR SS:[EBP-0x1C],0x0
	$-183    01846D6D     E8 AE33BAFF           CALL PathOfEx.013EA120
	$-17E    01846D72     C745 FC 00000000      MOV DWORD PTR SS:[EBP-0x4],0x0
	$-177    01846D79     8D45 D4               LEA EAX,DWORD PTR SS:[EBP-0x2C]
	$-174    01846D7C     8B97 543A0000         MOV EDX,DWORD PTR DS:[EDI+0x3A54]
	$-16E    01846D82     8B8F 503A0000         MOV ECX,DWORD PTR DS:[EDI+0x3A50]
	$-168    01846D88     8945 08               MOV DWORD PTR SS:[EBP+0x8],EAX
	$-165    01846D8B     8D45 08               LEA EAX,DWORD PTR SS:[EBP+0x8]
	$-162    01846D8E     50                    PUSH EAX
	$-161    01846D8F     E8 2C510000           CALL PathOfEx.0184BEC0
	$-15C    01846D94     83C4 04               ADD ESP,0x4
	$-159    01846D97     8845 F0               MOV BYTE PTR SS:[EBP-0x10],AL
	$-156    01846D9A     8D45 EC               LEA EAX,DWORD PTR SS:[EBP-0x14]
	$-153    01846D9D     8975 EC               MOV DWORD PTR SS:[EBP-0x14],ESI
	$-150    01846DA0     8D8F C43A0000         LEA ECX,DWORD PTR DS:[EDI+0x3AC4]
	$-14A    01846DA6     50                    PUSH EAX
	$-149    01846DA7     E8 D474C8FF           CALL PathOfEx.014CE280
	$-144    01846DAC     A1 603A1302           MOV EAX,DWORD PTR DS:[0x2133A60]
	$-13F    01846DB1     85C0                  TEST EAX,EAX
	$-13D    01846DB3     74 22                 JE SHORT PathOfEx.01846DD7
	$-13B    01846DB5     8B8F C83A0000         MOV ECX,DWORD PTR DS:[EDI+0x3AC8]
	$-135    01846DBB     8D50 18               LEA EDX,DWORD PTR DS:[EAX+0x18]
	$-132    01846DBE     2B8F C43A0000         SUB ECX,DWORD PTR DS:[EDI+0x3AC4]
	$-12C    01846DC4     8D45 08               LEA EAX,DWORD PTR SS:[EBP+0x8]
	$-129    01846DC7     C1F9 03               SAR ECX,0x3
	$-126    01846DCA     41                    INC ECX
	$-125    01846DCB     390A                  CMP DWORD PTR DS:[EDX],ECX
	$-123    01846DCD     894D 08               MOV DWORD PTR SS:[EBP+0x8],ECX
	$-120    01846DD0     0F43C2                CMOVNB EAX,EDX
	$-11D    01846DD3     8B00                  MOV EAX,DWORD PTR DS:[EAX]
	$-11B    01846DD5     8902                  MOV DWORD PTR DS:[EDX],EAX
	$-119    01846DD7     8D4D D4               LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$-116    01846DDA     C745 FC FFFFFFFF      MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-10F    01846DE1     E8 FA31BAFF           CALL PathOfEx.013E9FE0
	$-10A    01846DE6     EB 0C                 JMP SHORT PathOfEx.01846DF4
	$-108    01846DE8     53                    PUSH EBX
	$-107    01846DE9     8D8F D03A0000         LEA ECX,DWORD PTR DS:[EDI+0x3AD0]
	$-101    01846DEF     E8 5C1A0100           CALL PathOfEx.01858850
	$-FC     01846DF4     8B03                  MOV EAX,DWORD PTR DS:[EBX]
	$-FA     01846DF6     3B87 B43A0000         CMP EAX,DWORD PTR DS:[EDI+0x3AB4]
	$-F4     01846DFC     75 22                 JNZ SHORT PathOfEx.01846E20
	$-F2     01846DFE     8B8F 8C380000         MOV ECX,DWORD PTR DS:[EDI+0x388C]
	$-EC     01846E04     8D97 483D0000         LEA EDX,DWORD PTR DS:[EDI+0x3D48]
	$-E6     01846E0A     83C1 04               ADD ECX,0x4
	$-E3     01846E0D     E8 DEE2F3FF           CALL PathOfEx.017850F0
	$-DE     01846E12     84C0                  TEST AL,AL
	$-DC     01846E14     74 0A                 JE SHORT PathOfEx.01846E20
	$-DA     01846E16     6A 00                 PUSH 0x0
	$-D8     01846E18     8D4F F8               LEA ECX,DWORD PTR DS:[EDI-0x8]
	$-D5     01846E1B     E8 301D0000           CALL PathOfEx.01848B50
	$-D0     01846E20     8B4D F4               MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-CD     01846E23     5F                    POP EDI
	$-CC     01846E24     5E                    POP ESI
	$-CB     01846E25     5B                    POP EBX
	$-CA     01846E26     64:890D 00000000      MOV DWORD PTR FS:[0],ECX
	$-C3     01846E2D     8BE5                  MOV ESP,EBP
	$-C1     01846E2F     5D                    POP EBP
	$-C0     01846E30     C2 0400               RETN 0x4
	$-BD     01846E33     CC                    INT3
	$-BC     01846E34     CC                    INT3
	$-BB     01846E35     CC                    INT3
	$-BA     01846E36     CC                    INT3
	$-B9     01846E37     CC                    INT3
	$-B8     01846E38     CC                    INT3
	$-B7     01846E39     CC                    INT3
	$-B6     01846E3A     CC                    INT3
	$-B5     01846E3B     CC                    INT3
	$-B4     01846E3C     CC                    INT3
	$-B3     01846E3D     CC                    INT3
	$-B2     01846E3E     CC                    INT3
	$-B1     01846E3F     CC                    INT3
	$-B0     01846E40     83EC 08               SUB ESP,0x8
	$-AD     01846E43     53                    PUSH EBX
	$-AC     01846E44     56                    PUSH ESI
	$-AB     01846E45     57                    PUSH EDI
	$-AA     01846E46     8B7C24 18             MOV EDI,DWORD PTR SS:[ESP+0x18]
	$-A6     01846E4A     8BD9                  MOV EBX,ECX
	$-A4     01846E4C     8B0F                  MOV ECX,DWORD PTR DS:[EDI]
	$-A2     01846E4E     85C9                  TEST ECX,ECX
	$-A0     01846E50     74 05                 JE SHORT PathOfEx.01846E57
	$-9E     01846E52     E8 B9F6E3FF           CALL PathOfEx.01686510
	$-99     01846E57     8B07                  MOV EAX,DWORD PTR DS:[EDI]
	$-97     01846E59     C74424 18 789CEA01    MOV DWORD PTR SS:[ESP+0x18],PathOfEx.01EA9C78   ; Player
	$-8F     01846E61     8B70 30               MOV ESI,DWORD PTR DS:[EAX+0x30]
	$-8C     01846E64     8D4424 18             LEA EAX,DWORD PTR SS:[ESP+0x18]
	$-88     01846E68     50                    PUSH EAX
	$-87     01846E69     8D4424 10             LEA EAX,DWORD PTR SS:[ESP+0x10]
	$-83     01846E6D     50                    PUSH EAX
	$-82     01846E6E     8D4E 14               LEA ECX,DWORD PTR DS:[ESI+0x14]
	$-7F     01846E71     E8 4A1ABAFF           CALL PathOfEx.013E88C0
	$-7A     01846E76     8B4424 0C             MOV EAX,DWORD PTR SS:[ESP+0xC]
	$-76     01846E7A     3B46 18               CMP EAX,DWORD PTR DS:[ESI+0x18]
	$-73     01846E7D     74 52                 JE SHORT PathOfEx.01846ED1
	$-71     01846E7F     8B70 0C               MOV ESI,DWORD PTR DS:[EAX+0xC]
	$-6E     01846E82     83FE FF               CMP ESI,-0x1
	$-6B     01846E85     74 4A                 JE SHORT PathOfEx.01846ED1
	$-69     01846E87     8B47 04               MOV EAX,DWORD PTR DS:[EDI+0x4]
	$-66     01846E8A     8B34B0                MOV ESI,DWORD PTR DS:[EAX+ESI*4]
	$-63     01846E8D     897424 10             MOV DWORD PTR SS:[ESP+0x10],ESI
	$-5F     01846E91     85F6                  TEST ESI,ESI
	$-5D     01846E93     74 44                 JE SHORT PathOfEx.01846ED9
	$-5B     01846E95     8B47 14               MOV EAX,DWORD PTR DS:[EDI+0x14]
	$-58     01846E98     3B83 AC3A0000         CMP EAX,DWORD PTR DS:[EBX+0x3AAC]
	$-52     01846E9E     74 45                 JE SHORT PathOfEx.01846EE5
	$-50     01846EA0     8B83 C83A0000         MOV EAX,DWORD PTR DS:[EBX+0x3AC8]
	$-4A     01846EA6     8D8B C43A0000         LEA ECX,DWORD PTR DS:[EBX+0x3AC4]
	$-44     01846EAC     8B11                  MOV EDX,DWORD PTR DS:[ECX]
	$-42     01846EAE     3BD0                  CMP EDX,EAX
	$-40     01846EB0     74 0B                 JE SHORT PathOfEx.01846EBD
	$-3E     01846EB2     3932                  CMP DWORD PTR DS:[EDX],ESI
	$-3C     01846EB4     74 07                 JE SHORT PathOfEx.01846EBD
	$-3A     01846EB6     83C2 08               ADD EDX,0x8
	$-37     01846EB9     3BD0                  CMP EDX,EAX
	$-35     01846EBB   ^ 75 F5                 JNZ SHORT PathOfEx.01846EB2
	$-33     01846EBD     52                    PUSH EDX
	$-32     01846EBE     8D4424 1C             LEA EAX,DWORD PTR SS:[ESP+0x1C]
	$-2E     01846EC2     50                    PUSH EAX
	$-2D     01846EC3     E8 982F0000           CALL PathOfEx.01849E60
	$-28     01846EC8     5F                    POP EDI
	$-27     01846EC9     5E                    POP ESI
	$-26     01846ECA     5B                    POP EBX
	$-25     01846ECB     83C4 08               ADD ESP,0x8
	$-22     01846ECE     C2 0400               RETN 0x4
	$-1F     01846ED1     C74424 10 00000000    MOV DWORD PTR SS:[ESP+0x10],0x0
	$-17     01846ED9     57                    PUSH EDI
	$-16     01846EDA     8D8B D03A0000         LEA ECX,DWORD PTR DS:[EBX+0x3AD0]
	$-10     01846EE0     E8 6B1C0100           CALL PathOfEx.01858B50
	$-B      01846EE5     5F                    POP EDI
	$-A      01846EE6     5E                    POP ESI
	$-9      01846EE7     5B                    POP EBX
	$-8      01846EE8     83C4 08               ADD ESP,0x8
	$-5      01846EEB     C2 0400               RETN 0x4
	$-2      01846EEE     CC                    INT3
	$-1      01846EEF     CC                    INT3                                            ; 这个函数一直会到，下层函数是执行当前技能的
	$ ==>    01846EF0     8B81 88380000         MOV EAX,DWORD PTR DS:[ECX+0x3888]               ; tag_一直运行当前技能对象
	$+6      01846EF6     83B8 68010000 02      CMP DWORD PTR DS:[EAX+0x168],0x2                ; ecx=stCD_GameLogic*+0xc
	$+D      01846EFD     74 11                 JE SHORT PathOfEx.01846F10
	$+F      01846EFF     FFB1 AC3A0000         PUSH DWORD PTR DS:[ECX+0x3AAC]
	$+15     01846F05     81C1 2C390000         ADD ECX,0x392C                                  ; g_o_cur_using_skill_info_
	$+1B     01846F0B     E8 F0F72000           CALL PathOfEx.01A56700                          ; call tag_运行一次技能
	$+20     01846F10     C3                    RETN
	$+21     01846F11     CC                    INT3
	$+22     01846F12     CC                    INT3
	$+23     01846F13     CC                    INT3
	$+24     01846F14     CC                    INT3
	$+25     01846F15     CC                    INT3
	$+26     01846F16     CC                    INT3
	$+27     01846F17     CC                    INT3
	$+28     01846F18     CC                    INT3
	$+29     01846F19     CC                    INT3
	$+2A     01846F1A     CC                    INT3
	$+2B     01846F1B     CC                    INT3
	$+2C     01846F1C     CC                    INT3
	$+2D     01846F1D     CC                    INT3
	$+2E     01846F1E     CC                    INT3
	$+2F     01846F1F     CC                    INT3
	$+30     01846F20     83B9 643D0000 02      CMP DWORD PTR DS:[ECX+0x3D64],0x2
	$+37     01846F27     75 1D                 JNZ SHORT PathOfEx.01846F46
	$+39     01846F29     0FB681 A83C0000       MOVZX EAX,BYTE PTR DS:[ECX+0x3CA8]
	$+40     01846F30     C781 643D0000 0000000>MOV DWORD PTR DS:[ECX+0x3D64],0x0
	$+4A     01846F3A     81C1 88150000         ADD ECX,0x1588
	$+50     01846F40     50                    PUSH EAX
	$+51     01846F41     E8 FAA7BEFF           CALL PathOfEx.01431740
	$+56     01846F46     C3                    RETN
	$+57     01846F47     CC                    INT3
	$+58     01846F48     CC                    INT3
	$+59     01846F49     CC                    INT3
	$+5A     01846F4A     CC                    INT3
	$+5B     01846F4B     CC                    INT3
	$+5C     01846F4C     CC                    INT3
	$+5D     01846F4D     CC                    INT3
	$+5E     01846F4E     CC                    INT3
	$+5F     01846F4F     CC                    INT3
	$+60     01846F50     55                    PUSH EBP                                        ; tag_根据最新的鼠标坐标来更新技能对象的坐标
	$+61     01846F51     8BEC                  MOV EBP,ESP                                     ; ecx=stCD_GameLogic*+0xc
	$+63     01846F53     64:A1 00000000        MOV EAX,DWORD PTR FS:[0]                        ; p1=stCD_SkillObj*
	$+69     01846F59     6A FF                 PUSH -0x1
	$+6B     01846F5B     68 90C8D101           PUSH PathOfEx.01D1C890
	$+70     01846F60     50                    PUSH EAX
	$+71     01846F61     64:8925 00000000      MOV DWORD PTR FS:[0],ESP
	$+78     01846F68     83EC 18               SUB ESP,0x18
	$+7B     01846F6B     57                    PUSH EDI
	$+7C     01846F6C     8BF9                  MOV EDI,ECX
	$+7E     01846F6E     8B87 88380000         MOV EAX,DWORD PTR DS:[EDI+0x3888]
	$+84     01846F74     83B8 68010000 02      CMP DWORD PTR DS:[EAX+0x168],0x2
	$+8B     01846F7B     0F84 ED000000         JE PathOfEx.0184706E
	$+91     01846F81     56                    PUSH ESI
	$+92     01846F82     8B75 08               MOV ESI,DWORD PTR SS:[EBP+0x8]
	$+95     01846F85     8BCE                  MOV ECX,ESI
	$+97     01846F87     E8 D41C2500           CALL PathOfEx.01A98C60
	$+9C     01846F8C     B9 00800000           MOV ECX,0x8000
	$+A1     01846F91     66:85C1               TEST CX,AX
	$+A4     01846F94     0F85 D3000000         JNZ PathOfEx.0184706D
	$+AA     01846F9A     837E 1C 00            CMP DWORD PTR DS:[ESI+0x1C],0x0                 ; g_so_skill_obj_update_mouse_pos_
	$+AE     01846F9E     74 44                 JE SHORT PathOfEx.01846FE4
	$+B0     01846FA0     B8 12000000           MOV EAX,0x12
	$+B5     01846FA5     C745 E0 5483EB01      MOV DWORD PTR SS:[EBP-0x20],PathOfEx.01EB8354
	$+BC     01846FAC     66:8945 E4            MOV WORD PTR SS:[EBP-0x1C],AX
	$+C0     01846FB0     8BCE                  MOV ECX,ESI
	$+C2     01846FB2     C745 FC 00000000      MOV DWORD PTR SS:[EBP-0x4],0x0
	$+C9     01846FB9     E8 F21F2500           CALL PathOfEx.01A98FB0
	$+CE     01846FBE     8BCE                  MOV ECX,ESI
	$+D0     01846FC0     66:8945 EC            MOV WORD PTR SS:[EBP-0x14],AX
	$+D4     01846FC4     E8 971C2500           CALL PathOfEx.01A98C60
	$+D9     01846FC9     66:8945 EE            MOV WORD PTR SS:[EBP-0x12],AX
	$+DD     01846FCD     8D4D E0               LEA ECX,DWORD PTR SS:[EBP-0x20]
	$+E0     01846FD0     8A46 17               MOV AL,BYTE PTR DS:[ESI+0x17]
	$+E3     01846FD3     8845 F0               MOV BYTE PTR SS:[EBP-0x10],AL
	$+E6     01846FD6     8B46 1C               MOV EAX,DWORD PTR DS:[ESI+0x1C]
	$+E9     01846FD9     8B40 14               MOV EAX,DWORD PTR DS:[EAX+0x14]
	$+EC     01846FDC     8945 E8               MOV DWORD PTR SS:[EBP-0x18],EAX
	$+EF     01846FDF     8B45 E0               MOV EAX,DWORD PTR SS:[EBP-0x20]
	$+F2     01846FE2     EB 73                 JMP SHORT PathOfEx.01847057
	$+F4     01846FE4     F3:0F1087 A43A0000    MOVSS XMM0,DWORD PTR DS:[EDI+0x3AA4]
	$+FC     01846FEC     F3:0F5905 D808E301    MULSS XMM0,DWORD PTR DS:[0x1E308D8]
	$+104    01846FF4     C745 DC 0C83EB01      MOV DWORD PTR SS:[EBP-0x24],PathOfEx.01EB830C
	*/

	//gj3
	/*
	$-221    007E03CF     CC                       INT3
	$-220    007E03D0     55                       PUSH EBP
	$-21F    007E03D1     8BEC                     MOV EBP,ESP
	$-21D    007E03D3     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$-217    007E03D9     6A FF                    PUSH -0x1
	$-215    007E03DB     68 B8D7D500              PUSH PathOfEx.00D5D7B8
	$-210    007E03E0     50                       PUSH EAX
	$-20F    007E03E1     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$-208    007E03E8     83EC 20                  SUB ESP,0x20
	$-205    007E03EB     53                       PUSH EBX
	$-204    007E03EC     8B5D 08                  MOV EBX,DWORD PTR SS:[EBP+0x8]
	$-201    007E03EF     56                       PUSH ESI
	$-200    007E03F0     57                       PUSH EDI
	$-1FF    007E03F1     8BF9                     MOV EDI,ECX
	$-1FD    007E03F3     8B0B                     MOV ECX,DWORD PTR DS:[EBX]
	$-1FB    007E03F5     85C9                     TEST ECX,ECX
	$-1F9    007E03F7     74 05                    JE SHORT PathOfEx.007E03FE
	$-1F7    007E03F9     E8 D2F4E2FF              CALL PathOfEx.0060F8D0
	$-1F2    007E03FE     8B03                     MOV EAX,DWORD PTR DS:[EBX]
	$-1F0    007E0400     C745 08 F48CF500         MOV DWORD PTR SS:[EBP+0x8],PathOfEx.00F58CF4
	$-1E9    007E0407     8B70 30                  MOV ESI,DWORD PTR DS:[EAX+0x30]
	$-1E6    007E040A     8D45 08                  LEA EAX,DWORD PTR SS:[EBP+0x8]
	$-1E3    007E040D     50                       PUSH EAX
	$-1E2    007E040E     8D45 F0                  LEA EAX,DWORD PTR SS:[EBP-0x10]
	$-1DF    007E0411     50                       PUSH EAX
	$-1DE    007E0412     8D4E 14                  LEA ECX,DWORD PTR DS:[ESI+0x14]
	$-1DB    007E0415     E8 6647B4FF              CALL PathOfEx.00324B80
	$-1D6    007E041A     8B45 F0                  MOV EAX,DWORD PTR SS:[EBP-0x10]
	$-1D3    007E041D     3B46 18                  CMP EAX,DWORD PTR DS:[ESI+0x18]
	$-1D0    007E0420     0F84 C2000000            JE PathOfEx.007E04E8
	$-1CA    007E0426     8B48 0C                  MOV ECX,DWORD PTR DS:[EAX+0xC]
	$-1C7    007E0429     83F9 FF                  CMP ECX,-0x1
	$-1C4    007E042C     0F84 B6000000            JE PathOfEx.007E04E8
	$-1BE    007E0432     8B43 04                  MOV EAX,DWORD PTR DS:[EBX+0x4]
	$-1BB    007E0435     8B3488                   MOV ESI,DWORD PTR DS:[EAX+ECX*4]
	$-1B8    007E0438     85F6                     TEST ESI,ESI
	$-1B6    007E043A     0F84 A8000000            JE PathOfEx.007E04E8
	$-1B0    007E0440     8B43 20                  MOV EAX,DWORD PTR DS:[EBX+0x20]
	$-1AD    007E0443     3B87 2C430000            CMP EAX,DWORD PTR DS:[EDI+0x432C]
	$-1A7    007E0449     0F84 D1000000            JE PathOfEx.007E0520
	$-1A1    007E044F     33C9                     XOR ECX,ECX
	$-19F    007E0451     C745 E8 07000000         MOV DWORD PTR SS:[EBP-0x18],0x7
	$-198    007E0458     6A FF                    PUSH -0x1
	$-196    007E045A     51                       PUSH ECX
	$-195    007E045B     8D46 10                  LEA EAX,DWORD PTR DS:[ESI+0x10]
	$-192    007E045E     66:894D D4               MOV WORD PTR SS:[EBP-0x2C],CX
	$-18E    007E0462     50                       PUSH EAX
	$-18D    007E0463     8D4D D4                  LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$-18A    007E0466     C745 E4 00000000         MOV DWORD PTR SS:[EBP-0x1C],0x0
	$-183    007E046D     E8 6E5FB4FF              CALL PathOfEx.003263E0
	$-17E    007E0472     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$-177    007E0479     8D45 D4                  LEA EAX,DWORD PTR SS:[EBP-0x2C]
	$-174    007E047C     8B97 D4420000            MOV EDX,DWORD PTR DS:[EDI+0x42D4]
	$-16E    007E0482     8B8F D0420000            MOV ECX,DWORD PTR DS:[EDI+0x42D0]
	$-168    007E0488     8945 08                  MOV DWORD PTR SS:[EBP+0x8],EAX
	$-165    007E048B     8D45 08                  LEA EAX,DWORD PTR SS:[EBP+0x8]
	$-162    007E048E     50                       PUSH EAX
	$-161    007E048F     E8 4C520000              CALL PathOfEx.007E56E0
	$-15C    007E0494     83C4 04                  ADD ESP,0x4
	$-159    007E0497     8845 F0                  MOV BYTE PTR SS:[EBP-0x10],AL
	$-156    007E049A     8D45 EC                  LEA EAX,DWORD PTR SS:[EBP-0x14]
	$-153    007E049D     8975 EC                  MOV DWORD PTR SS:[EBP-0x14],ESI
	$-150    007E04A0     8D8F 44430000            LEA ECX,DWORD PTR DS:[EDI+0x4344]
	$-14A    007E04A6     50                       PUSH EAX
	$-149    007E04A7     E8 44E2C0FF              CALL PathOfEx.003EE6F0
	$-144    007E04AC     A1 9C233201              MOV EAX,DWORD PTR DS:[0x132239C]
	$-13F    007E04B1     85C0                     TEST EAX,EAX
	$-13D    007E04B3     74 22                    JE SHORT PathOfEx.007E04D7
	$-13B    007E04B5     8B8F 48430000            MOV ECX,DWORD PTR DS:[EDI+0x4348]
	$-135    007E04BB     8D50 18                  LEA EDX,DWORD PTR DS:[EAX+0x18]
	$-132    007E04BE     2B8F 44430000            SUB ECX,DWORD PTR DS:[EDI+0x4344]
	$-12C    007E04C4     8D45 08                  LEA EAX,DWORD PTR SS:[EBP+0x8]
	$-129    007E04C7     C1F9 03                  SAR ECX,0x3
	$-126    007E04CA     41                       INC ECX
	$-125    007E04CB     390A                     CMP DWORD PTR DS:[EDX],ECX
	$-123    007E04CD     894D 08                  MOV DWORD PTR SS:[EBP+0x8],ECX
	$-120    007E04D0     0F43C2                   CMOVNB EAX,EDX
	$-11D    007E04D3     8B00                     MOV EAX,DWORD PTR DS:[EAX]
	$-11B    007E04D5     8902                     MOV DWORD PTR DS:[EDX],EAX
	$-119    007E04D7     8D4D D4                  LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$-116    007E04DA     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-10F    007E04E1     E8 BA5DB4FF              CALL PathOfEx.003262A0
	$-10A    007E04E6     EB 0C                    JMP SHORT PathOfEx.007E04F4
	$-108    007E04E8     53                       PUSH EBX
	$-107    007E04E9     8D8F 50430000            LEA ECX,DWORD PTR DS:[EDI+0x4350]
	$-101    007E04EF     E8 3C270100              CALL PathOfEx.007F2C30
	$-FC     007E04F4     8B03                     MOV EAX,DWORD PTR DS:[EBX]
	$-FA     007E04F6     3B87 34430000            CMP EAX,DWORD PTR DS:[EDI+0x4334]
	$-F4     007E04FC     75 22                    JNZ SHORT PathOfEx.007E0520
	$-F2     007E04FE     8B8F 0C410000            MOV ECX,DWORD PTR DS:[EDI+0x410C]
	$-EC     007E0504     8D97 C8450000            LEA EDX,DWORD PTR DS:[EDI+0x45C8]
	$-E6     007E050A     83C1 10                  ADD ECX,0x10
	$-E3     007E050D     E8 4E24F3FF              CALL PathOfEx.00712960
	$-DE     007E0512     84C0                     TEST AL,AL
	$-DC     007E0514     74 0A                    JE SHORT PathOfEx.007E0520
	$-DA     007E0516     6A 00                    PUSH 0x0
	$-D8     007E0518     8D4F F8                  LEA ECX,DWORD PTR DS:[EDI-0x8]
	$-D5     007E051B     E8 301C0000              CALL PathOfEx.007E2150
	$-D0     007E0520     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-CD     007E0523     5F                       POP EDI
	$-CC     007E0524     5E                       POP ESI
	$-CB     007E0525     5B                       POP EBX
	$-CA     007E0526     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$-C3     007E052D     8BE5                     MOV ESP,EBP
	$-C1     007E052F     5D                       POP EBP
	$-C0     007E0530     C2 0400                  RETN 0x4
	$-BD     007E0533     CC                       INT3
	$-BC     007E0534     CC                       INT3
	$-BB     007E0535     CC                       INT3
	$-BA     007E0536     CC                       INT3
	$-B9     007E0537     CC                       INT3
	$-B8     007E0538     CC                       INT3
	$-B7     007E0539     CC                       INT3
	$-B6     007E053A     CC                       INT3
	$-B5     007E053B     CC                       INT3
	$-B4     007E053C     CC                       INT3
	$-B3     007E053D     CC                       INT3
	$-B2     007E053E     CC                       INT3
	$-B1     007E053F     CC                       INT3
	$-B0     007E0540     83EC 08                  SUB ESP,0x8
	$-AD     007E0543     53                       PUSH EBX
	$-AC     007E0544     56                       PUSH ESI
	$-AB     007E0545     57                       PUSH EDI
	$-AA     007E0546     8B7C24 18                MOV EDI,DWORD PTR SS:[ESP+0x18]
	$-A6     007E054A     8BD9                     MOV EBX,ECX
	$-A4     007E054C     8B0F                     MOV ECX,DWORD PTR DS:[EDI]
	$-A2     007E054E     85C9                     TEST ECX,ECX
	$-A0     007E0550     74 05                    JE SHORT PathOfEx.007E0557
	$-9E     007E0552     E8 79F3E2FF              CALL PathOfEx.0060F8D0
	$-99     007E0557     8B07                     MOV EAX,DWORD PTR DS:[EDI]
	$-97     007E0559     C74424 18 F48CF500       MOV DWORD PTR SS:[ESP+0x18],PathOfEx.00F58CF4
	$-8F     007E0561     8B70 30                  MOV ESI,DWORD PTR DS:[EAX+0x30]
	$-8C     007E0564     8D4424 18                LEA EAX,DWORD PTR SS:[ESP+0x18]
	$-88     007E0568     50                       PUSH EAX
	$-87     007E0569     8D4424 10                LEA EAX,DWORD PTR SS:[ESP+0x10]
	$-83     007E056D     50                       PUSH EAX
	$-82     007E056E     8D4E 14                  LEA ECX,DWORD PTR DS:[ESI+0x14]
	$-7F     007E0571     E8 0A46B4FF              CALL PathOfEx.00324B80
	$-7A     007E0576     8B4424 0C                MOV EAX,DWORD PTR SS:[ESP+0xC]
	$-76     007E057A     3B46 18                  CMP EAX,DWORD PTR DS:[ESI+0x18]
	$-73     007E057D     74 52                    JE SHORT PathOfEx.007E05D1
	$-71     007E057F     8B70 0C                  MOV ESI,DWORD PTR DS:[EAX+0xC]
	$-6E     007E0582     83FE FF                  CMP ESI,-0x1
	$-6B     007E0585     74 4A                    JE SHORT PathOfEx.007E05D1
	$-69     007E0587     8B47 04                  MOV EAX,DWORD PTR DS:[EDI+0x4]
	$-66     007E058A     8B34B0                   MOV ESI,DWORD PTR DS:[EAX+ESI*4]
	$-63     007E058D     897424 10                MOV DWORD PTR SS:[ESP+0x10],ESI
	$-5F     007E0591     85F6                     TEST ESI,ESI
	$-5D     007E0593     74 44                    JE SHORT PathOfEx.007E05D9
	$-5B     007E0595     8B47 20                  MOV EAX,DWORD PTR DS:[EDI+0x20]
	$-58     007E0598     3B83 2C430000            CMP EAX,DWORD PTR DS:[EBX+0x432C]
	$-52     007E059E     74 45                    JE SHORT PathOfEx.007E05E5
	$-50     007E05A0     8B83 48430000            MOV EAX,DWORD PTR DS:[EBX+0x4348]
	$-4A     007E05A6     8D8B 44430000            LEA ECX,DWORD PTR DS:[EBX+0x4344]
	$-44     007E05AC     8B11                     MOV EDX,DWORD PTR DS:[ECX]
	$-42     007E05AE     3BD0                     CMP EDX,EAX
	$-40     007E05B0     74 0B                    JE SHORT PathOfEx.007E05BD
	$-3E     007E05B2     3932                     CMP DWORD PTR DS:[EDX],ESI
	$-3C     007E05B4     74 07                    JE SHORT PathOfEx.007E05BD
	$-3A     007E05B6     83C2 08                  ADD EDX,0x8
	$-37     007E05B9     3BD0                     CMP EDX,EAX
	$-35     007E05BB   ^ 75 F5                    JNZ SHORT PathOfEx.007E05B2
	$-33     007E05BD     52                       PUSH EDX
	$-32     007E05BE     8D4424 1C                LEA EAX,DWORD PTR SS:[ESP+0x1C]
	$-2E     007E05C2     50                       PUSH EAX
	$-2D     007E05C3     E8 E8E0C0FF              CALL PathOfEx.003EE6B0
	$-28     007E05C8     5F                       POP EDI
	$-27     007E05C9     5E                       POP ESI
	$-26     007E05CA     5B                       POP EBX
	$-25     007E05CB     83C4 08                  ADD ESP,0x8
	$-22     007E05CE     C2 0400                  RETN 0x4
	$-1F     007E05D1     C74424 10 00000000       MOV DWORD PTR SS:[ESP+0x10],0x0
	$-17     007E05D9     57                       PUSH EDI
	$-16     007E05DA     8D8B 50430000            LEA ECX,DWORD PTR DS:[EBX+0x4350]
	$-10     007E05E0     E8 0B2A0100              CALL PathOfEx.007F2FF0
	$-B      007E05E5     5F                       POP EDI
	$-A      007E05E6     5E                       POP ESI
	$-9      007E05E7     5B                       POP EBX
	$-8      007E05E8     83C4 08                  ADD ESP,0x8
	$-5      007E05EB     C2 0400                  RETN 0x4
	$-2      007E05EE     CC                       INT3
	$-1      007E05EF     CC                       INT3                                            ; 这个函数一直会到，下层函数是执行当前技能的
	$ ==>    007E05F0     8B81 08410000            MOV EAX,DWORD PTR DS:[ECX+0x4108]               ; tag_一直运行当前技能对象
	$+6      007E05F6     80B8 53010000 02         CMP BYTE PTR DS:[EAX+0x153],0x2                 ; ecx=stCD_GameLogic*+0xc
	$+D      007E05FD     74 11                    JE SHORT PathOfEx.007E0610
	$+F      007E05FF     FFB1 2C430000            PUSH DWORD PTR DS:[ECX+0x432C]
	$+15     007E0605     81C1 AC410000            ADD ECX,0x41AC                                  ; g_o_cur_using_skill_info_
	$+1B     007E060B     E8 B0472A00              CALL PathOfEx.00A84DC0                          ; call tag_运行一次技能
	$+20     007E0610     C3                       RETN
	$+21     007E0611     CC                       INT3
	$+22     007E0612     CC                       INT3
	$+23     007E0613     CC                       INT3
	$+24     007E0614     CC                       INT3
	$+25     007E0615     CC                       INT3
	$+26     007E0616     CC                       INT3
	$+27     007E0617     CC                       INT3
	$+28     007E0618     CC                       INT3
	$+29     007E0619     CC                       INT3
	$+2A     007E061A     CC                       INT3
	$+2B     007E061B     CC                       INT3
	$+2C     007E061C     CC                       INT3
	$+2D     007E061D     CC                       INT3
	$+2E     007E061E     CC                       INT3
	$+2F     007E061F     CC                       INT3
	$+30     007E0620     83B9 E4450000 02         CMP DWORD PTR DS:[ECX+0x45E4],0x2
	$+37     007E0627     75 1D                    JNZ SHORT PathOfEx.007E0646
	$+39     007E0629     0FB681 28450000          MOVZX EAX,BYTE PTR DS:[ECX+0x4528]
	$+40     007E0630     C781 E4450000 00000000   MOV DWORD PTR DS:[ECX+0x45E4],0x0
	$+4A     007E063A     81C1 88170000            ADD ECX,0x1788
	$+50     007E0640     50                       PUSH EAX
	$+51     007E0641     E8 DAF6B8FF              CALL PathOfEx.0036FD20
	$+56     007E0646     C3                       RETN
	$+57     007E0647     CC                       INT3
	$+58     007E0648     CC                       INT3
	$+59     007E0649     CC                       INT3
	$+5A     007E064A     CC                       INT3
	$+5B     007E064B     CC                       INT3
	$+5C     007E064C     CC                       INT3
	$+5D     007E064D     CC                       INT3
	$+5E     007E064E     CC                       INT3
	$+5F     007E064F     CC                       INT3
	$+60     007E0650     55                       PUSH EBP
	$+61     007E0651     8BEC                     MOV EBP,ESP
	$+63     007E0653     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$+69     007E0659     6A FF                    PUSH -0x1
	$+6B     007E065B     68 2074DA00              PUSH PathOfEx.00DA7420
	$+70     007E0660     50                       PUSH EAX
	$+71     007E0661     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$+78     007E0668     83EC 18                  SUB ESP,0x18
	$+7B     007E066B     57                       PUSH EDI
	$+7C     007E066C     8BF9                     MOV EDI,ECX
	$+7E     007E066E     8B87 08410000            MOV EAX,DWORD PTR DS:[EDI+0x4108]
	$+84     007E0674     80B8 53010000 02         CMP BYTE PTR DS:[EAX+0x153],0x2
	$+8B     007E067B     0F84 ED000000            JE PathOfEx.007E076E
	$+91     007E0681     56                       PUSH ESI
	$+92     007E0682     8B75 08                  MOV ESI,DWORD PTR SS:[EBP+0x8]
	$+95     007E0685     8BCE                     MOV ECX,ESI
	$+97     007E0687     E8 14962E00              CALL PathOfEx.00AC9CA0
	$+9C     007E068C     B9 00800000              MOV ECX,0x8000
	$+A1     007E0691     66:85C1                  TEST CX,AX
	$+A4     007E0694     0F85 D3000000            JNZ PathOfEx.007E076D
	$+AA     007E069A     837E 1C 00               CMP DWORD PTR DS:[ESI+0x1C],0x0
	$+AE     007E069E     74 44                    JE SHORT PathOfEx.007E06E4
	$+B0     007E06A0     B8 13000000              MOV EAX,0x13
	$+B5     007E06A5     C745 E0 847FF600         MOV DWORD PTR SS:[EBP-0x20],PathOfEx.00F67F84
	$+BC     007E06AC     66:8945 E4               MOV WORD PTR SS:[EBP-0x1C],AX
	$+C0     007E06B0     8BCE                     MOV ECX,ESI
	$+C2     007E06B2     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$+C9     007E06B9     E8 32992E00              CALL PathOfEx.00AC9FF0
	$+CE     007E06BE     8BCE                     MOV ECX,ESI
	$+D0     007E06C0     66:8945 EC               MOV WORD PTR SS:[EBP-0x14],AX
	$+D4     007E06C4     E8 D7952E00              CALL PathOfEx.00AC9CA0
	$+D9     007E06C9     66:8945 EE               MOV WORD PTR SS:[EBP-0x12],AX
	$+DD     007E06CD     8D4D E0                  LEA ECX,DWORD PTR SS:[EBP-0x20]
	$+E0     007E06D0     8A46 17                  MOV AL,BYTE PTR DS:[ESI+0x17]
	$+E3     007E06D3     8845 F0                  MOV BYTE PTR SS:[EBP-0x10],AL
	$+E6     007E06D6     8B46 1C                  MOV EAX,DWORD PTR DS:[ESI+0x1C]
	$+E9     007E06D9     8B40 20                  MOV EAX,DWORD PTR DS:[EAX+0x20]
	$+EC     007E06DC     8945 E8                  MOV DWORD PTR SS:[EBP-0x18],EAX
	$+EF     007E06DF     8B45 E0                  MOV EAX,DWORD PTR SS:[EBP-0x20]
	$+F2     007E06E2     EB 73                    JMP SHORT PathOfEx.007E0757
	$+F4     007E06E4     F3:0F1087 24430000       MOVSS XMM0,DWORD PTR DS:[EDI+0x4324]
	$+FC     007E06EC     F3:0F5905 1C27ED00       MULSS XMM0,DWORD PTR DS:[0xED271C]
	$+104    007E06F4     C745 DC 787FF600         MOV DWORD PTR SS:[EBP-0x24],PathOfEx.00F67F78
	$+10B    007E06FB     F3:0F2CC8                CVTTSS2SI ECX,XMM0
	$+10F    007E06FF     F3:0F1087 20430000       MOVSS XMM0,DWORD PTR DS:[EDI+0x4320]
	$+117    007E0707     F3:0F5905 1C27ED00       MULSS XMM0,DWORD PTR DS:[0xED271C]
	$+11F    007E070F     894E 24                  MOV DWORD PTR DS:[ESI+0x24],ECX
	$+122    007E0712     F3:0F2CC0                CVTTSS2SI EAX,XMM0
	$+126    007E0716     8946 20                  MOV DWORD PTR DS:[ESI+0x20],EAX
	$+129    007E0719     B8 14000000              MOV EAX,0x14
	$+12E    007E071E     66:8945 E0               MOV WORD PTR SS:[EBP-0x20],AX
	$+132    007E0722     8BCE                     MOV ECX,ESI
	$+134    007E0724     C745 FC 01000000         MOV DWORD PTR SS:[EBP-0x4],0x1
	$+13B    007E072B     E8 C0982E00              CALL PathOfEx.00AC9FF0
	$+140    007E0730     8BCE                     MOV ECX,ESI
	$+142    007E0732     66:8945 EC               MOV WORD PTR SS:[EBP-0x14],AX
	$+146    007E0736     E8 65952E00              CALL PathOfEx.00AC9CA0
	$+14B    007E073B     66:8945 EE               MOV WORD PTR SS:[EBP-0x12],AX
	$+14F    007E073F     8D4D DC                  LEA ECX,DWORD PTR SS:[EBP-0x24]
	$+152    007E0742     8A46 17                  MOV AL,BYTE PTR DS:[ESI+0x17]
	$+155    007E0745     8845 F0                  MOV BYTE PTR SS:[EBP-0x10],AL
	$+158    007E0748     8B46 20                  MOV EAX,DWORD PTR DS:[ESI+0x20]
	$+15B    007E074B     8945 E4                  MOV DWORD PTR SS:[EBP-0x1C],EAX
	$+15E    007E074E     8B46 24                  MOV EAX,DWORD PTR DS:[ESI+0x24]
	$+161    007E0751     8945 E8                  MOV DWORD PTR SS:[EBP-0x18],EAX
	$+164    007E0754     8B45 DC                  MOV EAX,DWORD PTR SS:[EBP-0x24]
	$+167    007E0757     FFB7 0C410000            PUSH DWORD PTR DS:[EDI+0x410C]
	$+16D    007E075D     FF50 04                  CALL DWORD PTR DS:[EAX+0x4]
	$+170    007E0760     8B8F 0C410000            MOV ECX,DWORD PTR DS:[EDI+0x410C]
	$+176    007E0766     6A 00                    PUSH 0x0
	$+178    007E0768     E8 932AEBFF              CALL PathOfEx.00693200
	$+17D    007E076D     5E                       POP ESI
	$+17E    007E076E     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+181    007E0771     5F                       POP EDI
	$+182    007E0772     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+189    007E0779     8BE5                     MOV ESP,EBP
	$+18B    007E077B     5D                       POP EBP
	$+18C    007E077C     C2 0400                  RETN 0x4
	$+18F    007E077F     CC                       INT3
	$+190    007E0780     55                       PUSH EBP
	$+191    007E0781     8BEC                     MOV EBP,ESP
	$+193    007E0783     6A FF                    PUSH -0x1
	$+195    007E0785     68 B866D600              PUSH PathOfEx.00D666B8
	$+19A    007E078A     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$+1A0    007E0790     50                       PUSH EAX
	$+1A1    007E0791     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$+1A8    007E0798     83EC 0C                  SUB ESP,0xC
	$+1AB    007E079B     B8 67000000              MOV EAX,0x67
	$+1B0    007E07A0     C745 E8 3C7FF600         MOV DWORD PTR SS:[EBP-0x18],PathOfEx.00F67F3C
	$+1B7    007E07A7     56                       PUSH ESI
	$+1B8    007E07A8     8BF1                     MOV ESI,ECX
	$+1BA    007E07AA     66:8945 EC               MOV WORD PTR SS:[EBP-0x14],AX
	$+1BE    007E07AE     8A45 08                  MOV AL,BYTE PTR SS:[EBP+0x8]
	$+1C1    007E07B1     8D4D E8                  LEA ECX,DWORD PTR SS:[EBP-0x18]
	$+1C4    007E07B4     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$+1CB    007E07BB     FFB6 18410000            PUSH DWORD PTR DS:[ESI+0x4118]
	$+1D1    007E07C1     8845 F0                  MOV BYTE PTR SS:[EBP-0x10],AL
	$+1D4    007E07C4     E8 271AFFFF              CALL PathOfEx.007D21F0
	$+1D9    007E07C9     8B8E 18410000            MOV ECX,DWORD PTR DS:[ESI+0x4118]
	$+1DF    007E07CF     6A 00                    PUSH 0x0
	$+1E1    007E07D1     E8 2A2AEBFF              CALL PathOfEx.00693200
	$+1E6    007E07D6     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+1E9    007E07D9     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+1F0    007E07E0     5E                       POP ESI
	$+1F1    007E07E1     8BE5                     MOV ESP,EBP
	$+1F3    007E07E3     5D                       POP EBP
	$+1F4    007E07E4     C2 0400                  RETN 0x4
	$+1F7    007E07E7     CC                       INT3
	$+1F8    007E07E8     CC                       INT3
	$+1F9    007E07E9     CC                       INT3
	$+1FA    007E07EA     CC                       INT3
	$+1FB    007E07EB     CC                       INT3
	$+1FC    007E07EC     CC                       INT3
	$+1FD    007E07ED     CC                       INT3
	$+1FE    007E07EE     CC                       INT3
	$+1FF    007E07EF     CC                       INT3
	$+200    007E07F0     55                       PUSH EBP
	$+201    007E07F1     8BEC                     MOV EBP,ESP
	$+203    007E07F3     6A FF                    PUSH -0x1
	$+205    007E07F5     68 3874DA00              PUSH PathOfEx.00DA7438
	$+20A    007E07FA     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$+210    007E0800     50                       PUSH EAX
	$+211    007E0801     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$+218    007E0808     83EC 44                  SUB ESP,0x44
	$+21B    007E080B     B8 8E000000              MOV EAX,0x8E
	$+220    007E0810     C745 F0 07000000         MOV DWORD PTR SS:[EBP-0x10],0x7
	$+227    007E0817     66:8945 B4               MOV WORD PTR SS:[EBP-0x4C],AX
	$+22B    007E081B     BA E07DF600              MOV EDX,PathOfEx.00F67DE0
	$+230    007E0820     33C0                     XOR EAX,EAX
	$+232    007E0822     8955 B0                  MOV DWORD PTR SS:[EBP-0x50],EDX
	$+235    007E0825     56                       PUSH ESI
	$+236    007E0826     8BF1                     MOV ESI,ECX
	$+238    007E0828     C745 EC 00000000         MOV DWORD PTR SS:[EBP-0x14],0x0
	$+23F    007E082F     66:8945 DC               MOV WORD PTR SS:[EBP-0x24],AX
	$+243    007E0833     8945 FC                  MOV DWORD PTR SS:[EBP-0x4],EAX
	$+246    007E0836     8D4D DC                  LEA ECX,DWORD PTR SS:[EBP-0x24]
	$+249    007E0839     8A45 08                  MOV AL,BYTE PTR SS:[EBP+0x8]
	$+24C    007E083C     8845 B8                  MOV BYTE PTR SS:[EBP-0x48],AL
	$+24F    007E083F     8B45 10                  MOV EAX,DWORD PTR SS:[EBP+0x10]
	$+252    007E0842     3BC8                     CMP ECX,EAX
	$+254    007E0844     74 0D                    JE SHORT PathOfEx.007E0853
	$+256    007E0846     6A FF                    PUSH -0x1
	$+258    007E0848     6A 00                    PUSH 0x0
	$+25A    007E084A     50                       PUSH EAX
	$+25B    007E084B     E8 905BB4FF              CALL PathOfEx.003263E0
	$+260    007E0850     8B55 B0                  MOV EDX,DWORD PTR SS:[EBP-0x50]
	$+263    007E0853     8B45 0C                  MOV EAX,DWORD PTR SS:[EBP+0xC]
	$+266    007E0856     8D4D B0                  LEA ECX,DWORD PTR SS:[EBP-0x50]
	$+269    007E0859     FFB6 18410000            PUSH DWORD PTR DS:[ESI+0x4118]
	$+26F    007E085F     0F1000                   MOVUPS XMM0,DQWORD PTR DS:[EAX]
	$+272    007E0862     0F1145 B9                MOVUPS DQWORD PTR SS:[EBP-0x47],XMM0
	$+276    007E0866     0F1040 10                MOVUPS XMM0,DQWORD PTR DS:[EAX+0x10]
	$+27A    007E086A     0F1145 C9                MOVUPS DQWORD PTR SS:[EBP-0x37],XMM0
	$+27E    007E086E     FF52 04                  CALL DWORD PTR DS:[EDX+0x4]
	$+281    007E0871     8B8E 18410000            MOV ECX,DWORD PTR DS:[ESI+0x4118]
	$+287    007E0877     6A 00                    PUSH 0x0
	$+289    007E0879     E8 8229EBFF              CALL PathOfEx.00693200
	$+28E    007E087E     8B45 F0                  MOV EAX,DWORD PTR SS:[EBP-0x10]
	$+291    007E0881     5E                       POP ESI
	$+292    007E0882     83F8 08                  CMP EAX,0x8
	$+295    007E0885     72 0F                    JB SHORT PathOfEx.007E0896
	$+297    007E0887     6A 02                    PUSH 0x2
	$+299    007E0889     40                       INC EAX
	$+29A    007E088A     50                       PUSH EAX
	$+29B    007E088B     FF75 DC                  PUSH DWORD PTR SS:[EBP-0x24]
	$+29E    007E088E     E8 DD4AB4FF              CALL PathOfEx.00325370
	$+2A3    007E0893     83C4 0C                  ADD ESP,0xC
	$+2A6    007E0896     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+2A9    007E0899     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+2B0    007E08A0     8BE5                     MOV ESP,EBP
	$+2B2    007E08A2     5D                       POP EBP
	$+2B3    007E08A3     C2 0C00                  RETN 0xC
	$+2B6    007E08A6     CC                       INT3
	*/
#ifdef VER_GJ3_
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("8b??????????80????????????74??ff??????????81??????????e8????????c3", 0x15 + 2);
#else
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("8b??????????80????????????74??ff??????????81??????????e8????????c3", 0x15 + 2);
#endif
	if (!op_dw_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *op_dw_tmp;
	assert(dw_tmp > 0x3800 && dw_tmp < 0x4b00);
	ofs << "pt_cdword	g_o_cur_using_skill_info_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	//gj gj3
	/*
	$-15E    018455FF     CC                       INT3
	$-15D    01845600     55                       PUSH EBP
	$-15C    01845601     8BEC                     MOV EBP,ESP
	$-15A    01845603     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$-154    01845609     6A FF                    PUSH -0x1
	$-152    0184560B     68 D0C6D101              PUSH PathOfEx.01D1C6D0
	$-14D    01845610     50                       PUSH EAX
	$-14C    01845611     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$-145    01845618     83EC 14                  SUB ESP,0x14
	$-142    0184561B     56                       PUSH ESI
	$-141    0184561C     8BF1                     MOV ESI,ECX
	$-13F    0184561E     8B86 94380000            MOV EAX,DWORD PTR DS:[ESI+0x3894]
	$-139    01845624     83B8 68010000 02         CMP DWORD PTR DS:[EAX+0x168],0x2
	$-132    0184562B     75 51                    JNZ SHORT PathOfEx.0184567E
	$-130    0184562D     B8 CB000000              MOV EAX,0xCB
	$-12B    01845632     C745 E0 3083EB01         MOV DWORD PTR SS:[EBP-0x20],PathOfEx.01EB8330
	$-124    01845639     66:8945 E4               MOV WORD PTR SS:[EBP-0x1C],AX
	$-120    0184563D     8B4D 08                  MOV ECX,DWORD PTR SS:[EBP+0x8]
	$-11D    01845640     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$-116    01845647     FFB6 98380000            PUSH DWORD PTR DS:[ESI+0x3898]
	$-110    0184564D     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$-10E    0184564F     8945 E8                  MOV DWORD PTR SS:[EBP-0x18],EAX
	$-10B    01845652     8B41 04                  MOV EAX,DWORD PTR DS:[ECX+0x4]
	$-108    01845655     8D4D E0                  LEA ECX,DWORD PTR SS:[EBP-0x20]
	$-105    01845658     8945 EC                  MOV DWORD PTR SS:[EBP-0x14],EAX
	$-102    0184565B     E8 C044FFFF              CALL PathOfEx.01839B20
	$-FD     01845660     8B8E 98380000            MOV ECX,DWORD PTR DS:[ESI+0x3898]
	$-F7     01845666     6A 00                    PUSH 0x0
	$-F5     01845668     E8 73D4EBFF              CALL PathOfEx.01702AE0
	$-F0     0184566D     5E                       POP ESI
	$-EF     0184566E     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-EC     01845671     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$-E5     01845678     8BE5                     MOV ESP,EBP
	$-E3     0184567A     5D                       POP EBP
	$-E2     0184567B     C2 0400                  RETN 0x4
	$-DF     0184567E     53                       PUSH EBX
	$-DE     0184567F     8B9E B83A0000            MOV EBX,DWORD PTR DS:[ESI+0x3AB8]
	$-D8     01845685     57                       PUSH EDI
	$-D7     01845686     8B7D 08                  MOV EDI,DWORD PTR SS:[EBP+0x8]
	$-D4     01845689     8B43 70                  MOV EAX,DWORD PTR DS:[EBX+0x70]
	$-D1     0184568C     D1E8                     SHR EAX,1
	$-CF     0184568E     24 01                    AND AL,0x1
	$-CD     01845690     74 64                    JE SHORT PathOfEx.018456F6
	$-CB     01845692     8B4B 30                  MOV ECX,DWORD PTR DS:[EBX+0x30]
	$-C8     01845695     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$-C6     01845697     8B40 54                  MOV EAX,DWORD PTR DS:[EAX+0x54]
	$-C3     0184569A     FFD0                     CALL EAX
	$-C1     0184569C     84C0                     TEST AL,AL
	$-BF     0184569E     74 56                    JE SHORT PathOfEx.018456F6
	$-BD     018456A0     8B43 30                  MOV EAX,DWORD PTR DS:[EBX+0x30]
	$-BA     018456A3     F640 17 20               TEST BYTE PTR DS:[EAX+0x17],0x20
	$-B6     018456A7     75 4D                    JNZ SHORT PathOfEx.018456F6
	$-B4     018456A9     8B8E B83A0000            MOV ECX,DWORD PTR DS:[ESI+0x3AB8]
	$-AE     018456AF     FF77 04                  PUSH DWORD PTR DS:[EDI+0x4]
	$-AB     018456B2     FF37                     PUSH DWORD PTR DS:[EDI]
	$-A9     018456B4     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$-A7     018456B6     FF50 4C                  CALL DWORD PTR DS:[EAX+0x4C]
	$-A4     018456B9     B8 CE000000              MOV EAX,0xCE
	$-9F     018456BE     C745 E0 3083EB01         MOV DWORD PTR SS:[EBP-0x20],PathOfEx.01EB8330
	$-98     018456C5     66:8945 E4               MOV WORD PTR SS:[EBP-0x1C],AX
	$-94     018456C9     C745 FC 01000000         MOV DWORD PTR SS:[EBP-0x4],0x1
	$-8D     018456D0     8D4D E0                  LEA ECX,DWORD PTR SS:[EBP-0x20]
	$-8A     018456D3     8B07                     MOV EAX,DWORD PTR DS:[EDI]
	$-88     018456D5     FFB6 98380000            PUSH DWORD PTR DS:[ESI+0x3898]
	$-82     018456DB     8945 E8                  MOV DWORD PTR SS:[EBP-0x18],EAX
	$-7F     018456DE     8B47 04                  MOV EAX,DWORD PTR DS:[EDI+0x4]
	$-7C     018456E1     8945 EC                  MOV DWORD PTR SS:[EBP-0x14],EAX
	$-79     018456E4     E8 3744FFFF              CALL PathOfEx.01839B20
	$-74     018456E9     8B8E 98380000            MOV ECX,DWORD PTR DS:[ESI+0x3898]
	$-6E     018456EF     6A 00                    PUSH 0x0
	$-6C     018456F1     E8 EAD3EBFF              CALL PathOfEx.01702AE0
	$-67     018456F6     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-64     018456F9     C786 38390000 00000000   MOV DWORD PTR DS:[ESI+0x3938],0x0
	$-5A     01845703     8B07                     MOV EAX,DWORD PTR DS:[EDI]
	$-58     01845705     8986 3C390000            MOV DWORD PTR DS:[ESI+0x393C],EAX
	$-52     0184570B     8B47 04                  MOV EAX,DWORD PTR DS:[EDI+0x4]
	$-4F     0184570E     5F                       POP EDI
	$-4E     0184570F     8986 40390000            MOV DWORD PTR DS:[ESI+0x3940],EAX
	$-48     01845715     5B                       POP EBX
	$-47     01845716     5E                       POP ESI
	$-46     01845717     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$-3F     0184571E     8BE5                     MOV ESP,EBP
	$-3D     01845720     5D                       POP EBP
	$-3C     01845721     C2 0400                  RETN 0x4
	$-39     01845724     CC                       INT3
	$-38     01845725     CC                       INT3
	$-37     01845726     CC                       INT3
	$-36     01845727     CC                       INT3
	$-35     01845728     CC                       INT3
	$-34     01845729     CC                       INT3
	$-33     0184572A     CC                       INT3
	$-32     0184572B     CC                       INT3
	$-31     0184572C     CC                       INT3
	$-30     0184572D     CC                       INT3
	$-2F     0184572E     CC                       INT3
	$-2E     0184572F     CC                       INT3
	$-2D     01845730     55                       PUSH EBP                                        ; tag_中止当前技能
	$-2C     01845731     8BEC                     MOV EBP,ESP                                     ; ecx=stCD_GameLogic*
	$-2A     01845733     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]                        ; 没有参数
	$-24     01845739     6A FF                    PUSH -0x1
	$-22     0184573B     68 F0C6D101              PUSH PathOfEx.01D1C6F0
	$-1D     01845740     50                       PUSH EAX
	$-1C     01845741     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$-15     01845748     83EC 08                  SUB ESP,0x8
	$-12     0184574B     56                       PUSH ESI
	$-11     0184574C     8BF1                     MOV ESI,ECX
	$-F      0184574E     8B86 94380000            MOV EAX,DWORD PTR DS:[ESI+0x3894]
	$-9      01845754     83B8 68010000 02         CMP DWORD PTR DS:[EAX+0x168],0x2
	$-2      0184575B     75 41                    JNZ SHORT PathOfEx.0184579E
	$ ==>    0184575D     B8 CD000000              MOV EAX,0xCD
	$+5      01845762     C745 EC 9483EB01         MOV DWORD PTR SS:[EBP-0x14],PathOfEx.01EB8394
	$+C      01845769     66:8945 F0               MOV WORD PTR SS:[EBP-0x10],AX
	$+10     0184576D     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$+17     01845774     8D4D EC                  LEA ECX,DWORD PTR SS:[EBP-0x14]
	$+1A     01845777     FFB6 98380000            PUSH DWORD PTR DS:[ESI+0x3898]
	$+20     0184577D     E8 9E34FFFF              CALL PathOfEx.01838C20
	$+25     01845782     8B8E 98380000            MOV ECX,DWORD PTR DS:[ESI+0x3898]
	$+2B     01845788     6A 00                    PUSH 0x0
	$+2D     0184578A     E8 51D3EBFF              CALL PathOfEx.01702AE0
	$+32     0184578F     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+35     01845792     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+3C     01845799     5E                       POP ESI
	$+3D     0184579A     8BE5                     MOV ESP,EBP
	$+3F     0184579C     5D                       POP EBP
	$+40     0184579D     C3                       RETN
	$+41     0184579E     8B86 B83A0000            MOV EAX,DWORD PTR DS:[ESI+0x3AB8]
	$+47     018457A4     8B48 30                  MOV ECX,DWORD PTR DS:[EAX+0x30]
	$+4A     018457A7     85C9                     TEST ECX,ECX
	$+4C     018457A9     74 4A                    JE SHORT PathOfEx.018457F5
	$+4E     018457AB     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+50     018457AD     8B40 54                  MOV EAX,DWORD PTR DS:[EAX+0x54]
	$+53     018457B0     FFD0                     CALL EAX
	$+55     018457B2     84C0                     TEST AL,AL
	$+57     018457B4     74 3F                    JE SHORT PathOfEx.018457F5
	$+59     018457B6     B8 CF000000              MOV EAX,0xCF
	$+5E     018457BB     C745 EC 9483EB01         MOV DWORD PTR SS:[EBP-0x14],PathOfEx.01EB8394
	$+65     018457C2     66:8945 F0               MOV WORD PTR SS:[EBP-0x10],AX
	$+69     018457C6     C745 FC 01000000         MOV DWORD PTR SS:[EBP-0x4],0x1
	$+70     018457CD     8D4D EC                  LEA ECX,DWORD PTR SS:[EBP-0x14]
	$+73     018457D0     FFB6 98380000            PUSH DWORD PTR DS:[ESI+0x3898]
	$+79     018457D6     E8 4534FFFF              CALL PathOfEx.01838C20
	$+7E     018457DB     8B8E 98380000            MOV ECX,DWORD PTR DS:[ESI+0x3898]
	$+84     018457E1     6A 00                    PUSH 0x0
	$+86     018457E3     E8 F8D2EBFF              CALL PathOfEx.01702AE0
	$+8B     018457E8     8B8E B83A0000            MOV ECX,DWORD PTR DS:[ESI+0x3AB8]
	$+91     018457EE     6A 00                    PUSH 0x0
	$+93     018457F0     8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+95     018457F2     FF50 50                  CALL DWORD PTR DS:[EAX+0x50]
	$+98     018457F5     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+9B     018457F8     33C0                     XOR EAX,EAX
	$+9D     018457FA     66:8986 44390000         MOV WORD PTR DS:[ESI+0x3944],AX                 ; g_o_cur_using_skill_id_,清空当前使用的技能ID
	$+A4     01845801     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+AB     01845808     5E                       POP ESI
	$+AC     01845809     8BE5                     MOV ESP,EBP
	$+AE     0184580B     5D                       POP EBP
	$+AF     0184580C     C3                       RETN
	$+B0     0184580D     CC                       INT3
	$+B1     0184580E     CC                       INT3
	$+B2     0184580F     CC                       INT3
	$+B3     01845810     55                       PUSH EBP
	$+B4     01845811     8BEC                     MOV EBP,ESP
	$+B6     01845813     6A FF                    PUSH -0x1
	$+B8     01845815     68 08C7D101              PUSH PathOfEx.01D1C708
	$+BD     0184581A     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$+C3     01845820     50                       PUSH EAX
	$+C4     01845821     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$+CB     01845828     83EC 24                  SUB ESP,0x24
	$+CE     0184582B     B8 2F000000              MOV EAX,0x2F
	$+D3     01845830     C745 D0 DC82EB01         MOV DWORD PTR SS:[EBP-0x30],PathOfEx.01EB82DC
	$+DA     01845837     56                       PUSH ESI
	$+DB     01845838     8BF1                     MOV ESI,ECX
	$+DD     0184583A     66:8945 D4               MOV WORD PTR SS:[EBP-0x2C],AX
	$+E1     0184583E     8B45 08                  MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+E4     01845841     8D4D D0                  LEA ECX,DWORD PTR SS:[EBP-0x30]
	$+E7     01845844     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$+EE     0184584B     FFB6 98380000            PUSH DWORD PTR DS:[ESI+0x3898]
	$+F4     01845851     8945 D8                  MOV DWORD PTR SS:[EBP-0x28],EAX
	$+F7     01845854     8B45 14                  MOV EAX,DWORD PTR SS:[EBP+0x14]
	$+FA     01845857     8945 DC                  MOV DWORD PTR SS:[EBP-0x24],EAX
	$+FD     0184585A     8B45 18                  MOV EAX,DWORD PTR SS:[EBP+0x18]
	$+100    0184585D     8945 E0                  MOV DWORD PTR SS:[EBP-0x20],EAX
	$+103    01845860     8A45 0C                  MOV AL,BYTE PTR SS:[EBP+0xC]
	$+106    01845863     8845 EC                  MOV BYTE PTR SS:[EBP-0x14],AL
	$+109    01845866     8A45 10                  MOV AL,BYTE PTR SS:[EBP+0x10]
	$+10C    01845869     F3:0F1155 E8             MOVSS DWORD PTR SS:[EBP-0x18],XMM2
	$+111    0184586E     8845 E4                  MOV BYTE PTR SS:[EBP-0x1C],AL
	$+114    01845871     E8 EA46FFFF              CALL PathOfEx.01839F60
	$+119    01845876     8B8E 98380000            MOV ECX,DWORD PTR DS:[ESI+0x3898]
	$+11F    0184587C     6A 00                    PUSH 0x0
	$+121    0184587E     E8 5DD2EBFF              CALL PathOfEx.01702AE0
	$+126    01845883     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+129    01845886     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+130    0184588D     5E                       POP ESI
	$+131    0184588E     8BE5                     MOV ESP,EBP
	$+133    01845890     5D                       POP EBP
	$+134    01845891     C2 1400                  RETN 0x14
	$+137    01845894     CC                       INT3
	$+138    01845895     CC                       INT3
	$+139    01845896     CC                       INT3
	$+13A    01845897     CC                       INT3
	$+13B    01845898     CC                       INT3
	$+13C    01845899     CC                       INT3
	$+13D    0184589A     CC                       INT3
	$+13E    0184589B     CC                       INT3
	$+13F    0184589C     CC                       INT3
	$+140    0184589D     CC                       INT3
	$+141    0184589E     CC                       INT3
	$+142    0184589F     CC                       INT3
	$+143    018458A0     55                       PUSH EBP
	$+144    018458A1     8BEC                     MOV EBP,ESP
	$+146    018458A3     6A FF                    PUSH -0x1
	$+148    018458A5     68 36C7D101              PUSH PathOfEx.01D1C736
	$+14D    018458AA     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$+153    018458B0     50                       PUSH EAX
	$+154    018458B1     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$+15B    018458B8     83EC 18                  SUB ESP,0x18
	$+15E    018458BB     53                       PUSH EBX
	$+15F    018458BC     56                       PUSH ESI
	$+160    018458BD     57                       PUSH EDI
	$+161    018458BE     8B7D 08                  MOV EDI,DWORD PTR SS:[EBP+0x8]
	$+164    018458C1     8D45 EC                  LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+167    018458C4     8BD9                     MOV EBX,ECX
	$+169    018458C6     57                       PUSH EDI
	$+16A    018458C7     50                       PUSH EAX
	$+16B    018458C8     8D8B EC3A0000            LEA ECX,DWORD PTR DS:[EBX+0x3AEC]
	$+171    018458CE     E8 AD18F2FF              CALL PathOfEx.01767180
	$+176    018458D3     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$+17D    018458DA     B8 2D000000              MOV EAX,0x2D
	$+182    018458DF     66:8945 E0               MOV WORD PTR SS:[EBP-0x20],AX
	$+186    018458E3     C745 DC 3083EB01         MOV DWORD PTR SS:[EBP-0x24],PathOfEx.01EB8330
	$+18D    018458EA     8B75 0C                  MOV ESI,DWORD PTR SS:[EBP+0xC]
	$+190    018458ED     8D4D DC                  LEA ECX,DWORD PTR SS:[EBP-0x24]
	$+193    018458F0     C645 FC 01               MOV BYTE PTR SS:[EBP-0x4],0x1
	$+197    018458F4     FFB3 98380000            PUSH DWORD PTR DS:[EBX+0x3898]
	$+19D    018458FA     897D E4                  MOV DWORD PTR SS:[EBP-0x1C],EDI
	$+1A0    018458FD     8975 E8                  MOV DWORD PTR SS:[EBP-0x18],ESI
	$+1A3    01845900     E8 1B42FFFF              CALL PathOfEx.01839B20
	$+1A8    01845905     8B8B 98380000            MOV ECX,DWORD PTR DS:[EBX+0x3898]
	$+1AE    0184590B     6A 00                    PUSH 0x0
	$+1B0    0184590D     E8 CED1EBFF              CALL PathOfEx.01702AE0
	$+1B5    01845912     56                       PUSH ESI
	$+1B6    01845913     57                       PUSH EDI
	$+1B7    01845914     8D8B 14370000            LEA ECX,DWORD PTR DS:[EBX+0x3714]
	$+1BD    0184591A     E8 8198BCFF              CALL PathOfEx.0140F1A0
	$+1C2    0184591F     C745 DC DC83EB01         MOV DWORD PTR SS:[EBP-0x24],PathOfEx.01EB83DC
	$+1C9    01845926     C745 FC 02000000         MOV DWORD PTR SS:[EBP-0x4],0x2
	$+1D0    0184592D     8B75 F0                  MOV ESI,DWORD PTR SS:[EBP-0x10]
	$+1D3    01845930     85F6                     TEST ESI,ESI
	$+1D5    01845932     74 21                    JE SHORT PathOfEx.01845955
	$+1D7    01845934     83CF FF                  OR EDI,0xFFFFFFFF
	$+1DA    01845937     8BC7                     MOV EAX,EDI
	$+1DC    01845939     F0:0FC146 04             LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
	$+1E1    0184593E     75 15                    JNZ SHORT PathOfEx.01845955
	$+1E3    01845940     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$+1E5    01845942     8BCE                     MOV ECX,ESI
	$+1E7    01845944     FF10                     CALL DWORD PTR DS:[EAX]
	$+1E9    01845946     F0:0FC17E 08             LOCK XADD DWORD PTR DS:[ESI+0x8],EDI
	$+1EE    0184594B     4F                       DEC EDI
	$+1EF    0184594C     75 07                    JNZ SHORT PathOfEx.01845955
	$+1F1    0184594E     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$+1F3    01845950     8BCE                     MOV ECX,ESI
	$+1F5    01845952     FF50 04                  CALL DWORD PTR DS:[EAX+0x4]
	$+1F8    01845955     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+1FB    01845958     B0 01                    MOV AL,0x1
	$+1FD    0184595A     5F                       POP EDI
	$+1FE    0184595B     5E                       POP ESI
	$+1FF    0184595C     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+206    01845963     5B                       POP EBX
	$+207    01845964     8BE5                     MOV ESP,EBP
	$+209    01845966     5D                       POP EBP
	$+20A    01845967     C2 0800                  RETN 0x8
	$+20D    0184596A     CC                       INT3
	$+20E    0184596B     CC                       INT3
	$+20F    0184596C     CC                       INT3
	$+210    0184596D     CC                       INT3
	$+211    0184596E     CC                       INT3
	$+212    0184596F     CC                       INT3
	$+213    01845970     55                       PUSH EBP
	$+214    01845971     8BEC                     MOV EBP,ESP
	$+216    01845973     6A FF                    PUSH -0x1
	$+218    01845975     68 08C7D101              PUSH PathOfEx.01D1C708
	$+21D    0184597A     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$+223    01845980     50                       PUSH EAX
	$+224    01845981     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$+22B    01845988     83EC 24                  SUB ESP,0x24
	$+22E    0184598B     B8 33000000              MOV EAX,0x33
	$+233    01845990     C745 D0 9881EB01         MOV DWORD PTR SS:[EBP-0x30],PathOfEx.01EB8198
	$+23A    01845997     56                       PUSH ESI
	$+23B    01845998     8BF1                     MOV ESI,ECX
	$+23D    0184599A     66:8945 D4               MOV WORD PTR SS:[EBP-0x2C],AX
	$+241    0184599E     8B45 08                  MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+244    018459A1     8D4D D0                  LEA ECX,DWORD PTR SS:[EBP-0x30]
	$+247    018459A4     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$+24E    018459AB     FFB6 98380000            PUSH DWORD PTR DS:[ESI+0x3898]
	$+254    018459B1     8945 D8                  MOV DWORD PTR SS:[EBP-0x28],EAX
	$+257    018459B4     8B45 0C                  MOV EAX,DWORD PTR SS:[EBP+0xC]
	$+25A    018459B7     8945 E8                  MOV DWORD PTR SS:[EBP-0x18],EAX
	$+25D    018459BA     66:8B45 10               MOV AX,WORD PTR SS:[EBP+0x10]
	$+261    018459BE     66:8945 EC               MOV WORD PTR SS:[EBP-0x14],AX
	$+265    018459C2     66:8B45 14               MOV AX,WORD PTR SS:[EBP+0x14]
	$+269    018459C6     66:8945 EE               MOV WORD PTR SS:[EBP-0x12],AX
	$+26D    018459CA     8B45 18                  MOV EAX,DWORD PTR SS:[EBP+0x18]
	$+270    018459CD     8945 DC                  MOV DWORD PTR SS:[EBP-0x24],EAX
	$+273    018459D0     8B45 1C                  MOV EAX,DWORD PTR SS:[EBP+0x1C]
	$+276    018459D3     8945 E0                  MOV DWORD PTR SS:[EBP-0x20],EAX
	$+279    018459D6     8A45 20                  MOV AL,BYTE PTR SS:[EBP+0x20]
	$+27C    018459D9     8845 E4                  MOV BYTE PTR SS:[EBP-0x1C],AL
	$+27F    018459DC     E8 EF60FFFF              CALL PathOfEx.0183BAD0
	$+284    018459E1     8B8E 98380000            MOV ECX,DWORD PTR DS:[ESI+0x3898]
	$+28A    018459E7     6A 00                    PUSH 0x0
	$+28C    018459E9     E8 F2D0EBFF              CALL PathOfEx.01702AE0
	$+291    018459EE     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+294    018459F1     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+29B    018459F8     5E                       POP ESI
	$+29C    018459F9     8BE5                     MOV ESP,EBP
	$+29E    018459FB     5D                       POP EBP
	$+29F    018459FC     C2 1C00                  RETN 0x1C
	$+2A2    018459FF     CC                       INT3
	$+2A3    01845A00     55                       PUSH EBP
	$+2A4    01845A01     8BEC                     MOV EBP,ESP
	$+2A6    01845A03     64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$+2AC    01845A09     6A FF                    PUSH -0x1
	$+2AE    01845A0B     68 7821CE01              PUSH PathOfEx.01CE2178
	$+2B3    01845A10     50                       PUSH EAX
	$+2B4    01845A11     64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$+2BB    01845A18     83EC 0C                  SUB ESP,0xC
	$+2BE    01845A1B     56                       PUSH ESI
	$+2BF    01845A1C     8BF1                     MOV ESI,ECX
	$+2C1    01845A1E     8B8E 94380000            MOV ECX,DWORD PTR DS:[ESI+0x3894]
	$+2C7    01845A24     8B41 08                  MOV EAX,DWORD PTR DS:[ECX+0x8]
	$+2CA    01845A27     8A40 0C                  MOV AL,BYTE PTR DS:[EAX+0xC]
	$+2CD    01845A2A     84C0                     TEST AL,AL
	$+2CF    01845A2C     74 13                    JE SHORT PathOfEx.01845A41
	$+2D1    01845A2E     32C0                     XOR AL,AL
	$+2D3    01845A30     5E                       POP ESI
	$+2D4    01845A31     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+2D7    01845A34     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+2DE    01845A3B     8BE5                     MOV ESP,EBP
	$+2E0    01845A3D     5D                       POP EBP
	$+2E1    01845A3E     C2 0400                  RETN 0x4
	$+2E4    01845A41     8B89 44010000            MOV ECX,DWORD PTR DS:[ECX+0x144]
	$+2EA    01845A47     E8 442CBAFF              CALL PathOfEx.013E8690
	$+2EF    01845A4C     8378 30 00               CMP DWORD PTR DS:[EAX+0x30],0x0
	$+2F3    01845A50   ^ 7E DC                    JLE SHORT PathOfEx.01845A2E
	$+2F5    01845A52     803D 09E0FB01 00         CMP BYTE PTR DS:[0x1FBE009],0x0
	$+2FC    01845A59   ^ 75 D3                    JNZ SHORT PathOfEx.01845A2E
	$+2FE    01845A5B     B8 31000000              MOV EAX,0x31
	$+303    01845A60     C745 E8 C483EB01         MOV DWORD PTR SS:[EBP-0x18],PathOfEx.01EB83C4
	$+30A    01845A67     66:8945 EC               MOV WORD PTR SS:[EBP-0x14],AX
	$+30E    01845A6B     8B45 08                  MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+311    01845A6E     8D4D E8                  LEA ECX,DWORD PTR SS:[EBP-0x18]
	$+314    01845A71     C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$+31B    01845A78     FFB6 98380000            PUSH DWORD PTR DS:[ESI+0x3898]
	$+321    01845A7E     8945 F0                  MOV DWORD PTR SS:[EBP-0x10],EAX
	$+324    01845A81     E8 1A34FFFF              CALL PathOfEx.01838EA0
	$+329    01845A86     8B8E 98380000            MOV ECX,DWORD PTR DS:[ESI+0x3898]
	$+32F    01845A8C     6A 00                    PUSH 0x0
	$+331    01845A8E     E8 4DD0EBFF              CALL PathOfEx.01702AE0
	$+336    01845A93     8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+339    01845A96     B0 01                    MOV AL,0x1
	$+33B    01845A98     5E                       POP ESI
	$+33C    01845A99     64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+343    01845AA0     8BE5                     MOV ESP,EBP
	$+345    01845AA2     5D                       POP EBP
	$+346    01845AA3     C2 0400                  RETN 0x4
	$+349    01845AA6     CC                       INT3
	*/
	//gf
	/*
	$-32E    008D938F    CC                       INT3
	$-32D    008D9390    55                       PUSH EBP
	$-32C    008D9391    8BEC                     MOV EBP,ESP
	$-32A    008D9393    64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$-324    008D9399    6A FF                    PUSH -0x1
	$-322    008D939B    68 287FD600              PUSH PathOfEx.00D67F28
	$-31D    008D93A0    50                       PUSH EAX
	$-31C    008D93A1    64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$-315    008D93A8    83EC 14                  SUB ESP,0x14
	$-312    008D93AB    56                       PUSH ESI
	$-311    008D93AC    8BF1                     MOV ESI,ECX
	$-30F    008D93AE    57                       PUSH EDI
	$-30E    008D93AF    8B8E B83C0000            MOV ECX,DWORD PTR DS:[ESI+0x3CB8]
	$-308    008D93B5    85C9                     TEST ECX,ECX
	$-306    008D93B7    0F84 7F010000            JE PathOfEx.008D953C
	$-300    008D93BD    83BE 683F0000 00         CMP DWORD PTR DS:[ESI+0x3F68],0x0
	$-2F9    008D93C4    0F8F 72010000            JG PathOfEx.008D953C
	$-2F3    008D93CA    8B7D 08                  MOV EDI,DWORD PTR SS:[EBP+0x8]
	$-2F0    008D93CD    66:85FF                  TEST DI,DI
	$-2ED    008D93D0    75 17                    JNZ SHORT PathOfEx.008D93E9
	$-2EB    008D93D2    0FB705 40771301          MOVZX EAX,WORD PTR DS:[0x1137740]
	$-2E4    008D93D9    50                       PUSH EAX
	$-2E3    008D93DA    8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$-2E1    008D93DC    FF50 54                  CALL DWORD PTR DS:[EAX+0x54]
	$-2DE    008D93DF    8BC8                     MOV ECX,EAX
	$-2DC    008D93E1    E8 8AC4F3FF              CALL PathOfEx.00815870
	$-2D7    008D93E6    0FB7F8                   MOVZX EDI,AX
	$-2D4    008D93E9    8D8E 14070000            LEA ECX,DWORD PTR DS:[ESI+0x714]
	$-2CE    008D93EF    E8 1CA7BDFF              CALL PathOfEx.004B3B10
	$-2C9    008D93F4    8B86 943A0000            MOV EAX,DWORD PTR DS:[ESI+0x3A94]
	$-2C3    008D93FA    66:89BE 643F0000         MOV WORD PTR DS:[ESI+0x3F64],DI
	$-2BC    008D9401    83B8 E8010000 02         CMP DWORD PTR DS:[EAX+0x1E8],0x2
	$-2B5    008D9408    0F85 90000000            JNZ PathOfEx.008D949E
	$-2AF    008D940E    8B45 0C                  MOV EAX,DWORD PTR SS:[EBP+0xC]
	$-2AC    008D9411    85C0                     TEST EAX,EAX
	$-2AA    008D9413    74 25                    JE SHORT PathOfEx.008D943A
	$-2A8    008D9415    B9 CC000000              MOV ECX,0xCC
	$-2A3    008D941A    C745 E4 BCCFEF00         MOV DWORD PTR SS:[EBP-0x1C],PathOfEx.00EFCFBC
	$-29C    008D9421    66:894D E8               MOV WORD PTR SS:[EBP-0x18],CX
	$-298    008D9425    C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$-291    008D942C    8945 EC                  MOV DWORD PTR SS:[EBP-0x14],EAX
	$-28E    008D942F    8A45 14                  MOV AL,BYTE PTR SS:[EBP+0x14]
	$-28B    008D9432    8845 F2                  MOV BYTE PTR SS:[EBP-0xE],AL
	$-288    008D9435    8D45 E4                  LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$-285    008D9438    EB 2E                    JMP SHORT PathOfEx.008D9468
	$-283    008D943A    B8 CB000000              MOV EAX,0xCB
	$-27E    008D943F    C745 E0 B0CFEF00         MOV DWORD PTR SS:[EBP-0x20],PathOfEx.00EFCFB0    ; ASCII "p缌"
	$-277    008D9446    66:8945 E4               MOV WORD PTR SS:[EBP-0x1C],AX
	$-273    008D944A    8B4D 10                  MOV ECX,DWORD PTR SS:[EBP+0x10]
	$-270    008D944D    C745 FC 01000000         MOV DWORD PTR SS:[EBP-0x4],0x1
	$-269    008D9454    8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$-267    008D9456    8945 E8                  MOV DWORD PTR SS:[EBP-0x18],EAX
	$-264    008D9459    8B41 04                  MOV EAX,DWORD PTR DS:[ECX+0x4]
	$-261    008D945C    8945 EC                  MOV DWORD PTR SS:[EBP-0x14],EAX
	$-25E    008D945F    8A45 14                  MOV AL,BYTE PTR SS:[EBP+0x14]
	$-25B    008D9462    8845 F2                  MOV BYTE PTR SS:[EBP-0xE],AL
	$-258    008D9465    8D45 E0                  LEA EAX,DWORD PTR SS:[EBP-0x20]
	$-255    008D9468    50                       PUSH EAX
	$-254    008D9469    FFB6 983A0000            PUSH DWORD PTR DS:[ESI+0x3A98]
	$-24E    008D946F    66:897D F0               MOV WORD PTR SS:[EBP-0x10],DI
	$-24A    008D9473    E8 B82EDEFF              CALL PathOfEx.006BC330
	$-245    008D9478    8BC8                     MOV ECX,EAX
	$-243    008D947A    E8 D1310100              CALL PathOfEx.008EC650
	$-23E    008D947F    8B8E 983A0000            MOV ECX,DWORD PTR DS:[ESI+0x3A98]
	$-238    008D9485    6A 00                    PUSH 0x0
	$-236    008D9487    E8 C4DFEBFF              CALL PathOfEx.00797450
	$-231    008D948C    8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-22E    008D948F    64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$-227    008D9496    5F                       POP EDI
	$-226    008D9497    5E                       POP ESI
	$-225    008D9498    8BE5                     MOV ESP,EBP
	$-223    008D949A    5D                       POP EBP
	$-222    008D949B    C2 1000                  RETN 0x10
	$-21F    008D949E    8B8E B83C0000            MOV ECX,DWORD PTR DS:[ESI+0x3CB8]
	$-219    008D94A4    E8 C740F1FF              CALL PathOfEx.007ED570
	$-214    008D94A9    84C0                     TEST AL,AL
	$-212    008D94AB    74 55                    JE SHORT PathOfEx.008D9502
	$-210    008D94AD    B8 D1000000              MOV EAX,0xD1
	$-20B    008D94B2    C745 EC 78D0EF00         MOV DWORD PTR SS:[EBP-0x14],PathOfEx.00EFD078
	$-204    008D94B9    66:8945 F0               MOV WORD PTR SS:[EBP-0x10],AX
	$-200    008D94BD    8D45 EC                  LEA EAX,DWORD PTR SS:[EBP-0x14]
	$-1FD    008D94C0    C745 FC 02000000         MOV DWORD PTR SS:[EBP-0x4],0x2
	$-1F6    008D94C7    50                       PUSH EAX
	$-1F5    008D94C8    FFB6 983A0000            PUSH DWORD PTR DS:[ESI+0x3A98]
	$-1EF    008D94CE    E8 5D2EDEFF              CALL PathOfEx.006BC330
	$-1EA    008D94D3    8BC8                     MOV ECX,EAX
	$-1E8    008D94D5    E8 76310100              CALL PathOfEx.008EC650
	$-1E3    008D94DA    8B8E 983A0000            MOV ECX,DWORD PTR DS:[ESI+0x3A98]
	$-1DD    008D94E0    6A 00                    PUSH 0x0
	$-1DB    008D94E2    E8 69DFEBFF              CALL PathOfEx.00797450
	$-1D6    008D94E7    8B8E B83C0000            MOV ECX,DWORD PTR DS:[ESI+0x3CB8]
	$-1D0    008D94ED    6A 00                    PUSH 0x0
	$-1CE    008D94EF    8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$-1CC    008D94F1    FF50 50                  CALL DWORD PTR DS:[EAX+0x50]
	$-1C9    008D94F4    C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-1C2    008D94FB    C745 EC 60D0EF00         MOV DWORD PTR SS:[EBP-0x14],PathOfEx.00EFD060
	$-1BB    008D9502    8B45 0C                  MOV EAX,DWORD PTR SS:[EBP+0xC]
	$-1B8    008D9505    8D8E 383B0000            LEA ECX,DWORD PTR DS:[ESI+0x3B38]
	$-1B2    008D950B    85C0                     TEST EAX,EAX
	$-1B0    008D950D    74 04                    JE SHORT PathOfEx.008D9513
	$-1AE    008D950F    8901                     MOV DWORD PTR DS:[ECX],EAX
	$-1AC    008D9511    EB 14                    JMP SHORT PathOfEx.008D9527
	$-1AA    008D9513    8B55 10                  MOV EDX,DWORD PTR SS:[EBP+0x10]
	$-1A7    008D9516    C701 00000000            MOV DWORD PTR DS:[ECX],0x0
	$-1A1    008D951C    8B02                     MOV EAX,DWORD PTR DS:[EDX]
	$-19F    008D951E    8941 04                  MOV DWORD PTR DS:[ECX+0x4],EAX
	$-19C    008D9521    8B42 04                  MOV EAX,DWORD PTR DS:[EDX+0x4]
	$-199    008D9524    8941 08                  MOV DWORD PTR DS:[ECX+0x8],EAX
	$-196    008D9527    8A45 14                  MOV AL,BYTE PTR SS:[EBP+0x14]
	$-193    008D952A    8841 0E                  MOV BYTE PTR DS:[ECX+0xE],AL
	$-190    008D952D    66:8979 0C               MOV WORD PTR DS:[ECX+0xC],DI
	$-18C    008D9531    FFB6 B83C0000            PUSH DWORD PTR DS:[ESI+0x3CB8]
	$-186    008D9537    E8 44D81C00              CALL PathOfEx.00AA6D80
	$-181    008D953C    8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-17E    008D953F    5F                       POP EDI
	$-17D    008D9540    64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$-176    008D9547    5E                       POP ESI
	$-175    008D9548    8BE5                     MOV ESP,EBP
	$-173    008D954A    5D                       POP EBP
	$-172    008D954B    C2 1000                  RETN 0x10
	$-16F    008D954E    CC                       INT3
	$-16E    008D954F    CC                       INT3
	$-16D    008D9550    55                       PUSH EBP
	$-16C    008D9551    8BEC                     MOV EBP,ESP
	$-16A    008D9553    64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$-164    008D9559    6A FF                    PUSH -0x1
	$-162    008D955B    68 507FD600              PUSH PathOfEx.00D67F50
	$-15D    008D9560    50                       PUSH EAX
	$-15C    008D9561    64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$-155    008D9568    83EC 14                  SUB ESP,0x14
	$-152    008D956B    56                       PUSH ESI
	$-151    008D956C    8BF1                     MOV ESI,ECX
	$-14F    008D956E    8B86 943A0000            MOV EAX,DWORD PTR DS:[ESI+0x3A94]
	$-149    008D9574    83B8 E8010000 02         CMP DWORD PTR DS:[EAX+0x1E8],0x2
	$-142    008D957B    75 59                    JNZ SHORT PathOfEx.008D95D6
	$-140    008D957D    B8 CD000000              MOV EAX,0xCD
	$-13B    008D9582    C745 E0 C8CFEF00         MOV DWORD PTR SS:[EBP-0x20],PathOfEx.00EFCFC8
	$-134    008D9589    66:8945 E4               MOV WORD PTR SS:[EBP-0x1C],AX
	$-130    008D958D    8B4D 08                  MOV ECX,DWORD PTR SS:[EBP+0x8]
	$-12D    008D9590    C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$-126    008D9597    8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$-124    008D9599    8945 E8                  MOV DWORD PTR SS:[EBP-0x18],EAX
	$-121    008D959C    8B41 04                  MOV EAX,DWORD PTR DS:[ECX+0x4]
	$-11E    008D959F    8945 EC                  MOV DWORD PTR SS:[EBP-0x14],EAX
	$-11B    008D95A2    8D45 E0                  LEA EAX,DWORD PTR SS:[EBP-0x20]
	$-118    008D95A5    50                       PUSH EAX
	$-117    008D95A6    FFB6 983A0000            PUSH DWORD PTR DS:[ESI+0x3A98]
	$-111    008D95AC    E8 7F2DDEFF              CALL PathOfEx.006BC330
	$-10C    008D95B1    8BC8                     MOV ECX,EAX
	$-10A    008D95B3    E8 98300100              CALL PathOfEx.008EC650
	$-105    008D95B8    8B8E 983A0000            MOV ECX,DWORD PTR DS:[ESI+0x3A98]
	$-FF     008D95BE    6A 00                    PUSH 0x0
	$-FD     008D95C0    E8 8BDEEBFF              CALL PathOfEx.00797450
	$-F8     008D95C5    5E                       POP ESI
	$-F7     008D95C6    8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-F4     008D95C9    64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$-ED     008D95D0    8BE5                     MOV ESP,EBP
	$-EB     008D95D2    5D                       POP EBP
	$-EA     008D95D3    C2 0400                  RETN 0x4
	$-E7     008D95D6    53                       PUSH EBX
	$-E6     008D95D7    8B9E B83C0000            MOV EBX,DWORD PTR DS:[ESI+0x3CB8]
	$-E0     008D95DD    57                       PUSH EDI
	$-DF     008D95DE    8B7D 08                  MOV EDI,DWORD PTR SS:[EBP+0x8]
	$-DC     008D95E1    8B43 70                  MOV EAX,DWORD PTR DS:[EBX+0x70]
	$-D9     008D95E4    D1E8                     SHR EAX,1
	$-D7     008D95E6    24 01                    AND AL,0x1
	$-D5     008D95E8    74 6C                    JE SHORT PathOfEx.008D9656
	$-D3     008D95EA    8B4B 30                  MOV ECX,DWORD PTR DS:[EBX+0x30]
	$-D0     008D95ED    8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$-CE     008D95EF    8B40 4C                  MOV EAX,DWORD PTR DS:[EAX+0x4C]
	$-CB     008D95F2    FFD0                     CALL EAX
	$-C9     008D95F4    84C0                     TEST AL,AL
	$-C7     008D95F6    74 5E                    JE SHORT PathOfEx.008D9656
	$-C5     008D95F8    8B43 30                  MOV EAX,DWORD PTR DS:[EBX+0x30]
	$-C2     008D95FB    F640 17 20               TEST BYTE PTR DS:[EAX+0x17],0x20
	$-BE     008D95FF    75 55                    JNZ SHORT PathOfEx.008D9656
	$-BC     008D9601    8B8E B83C0000            MOV ECX,DWORD PTR DS:[ESI+0x3CB8]
	$-B6     008D9607    FF77 04                  PUSH DWORD PTR DS:[EDI+0x4]
	$-B3     008D960A    FF37                     PUSH DWORD PTR DS:[EDI]
	$-B1     008D960C    8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$-AF     008D960E    FF50 4C                  CALL DWORD PTR DS:[EAX+0x4C]
	$-AC     008D9611    B8 D0000000              MOV EAX,0xD0
	$-A7     008D9616    C745 E0 C8CFEF00         MOV DWORD PTR SS:[EBP-0x20],PathOfEx.00EFCFC8
	$-A0     008D961D    66:8945 E4               MOV WORD PTR SS:[EBP-0x1C],AX
	$-9C     008D9621    C745 FC 01000000         MOV DWORD PTR SS:[EBP-0x4],0x1
	$-95     008D9628    8B07                     MOV EAX,DWORD PTR DS:[EDI]
	$-93     008D962A    8945 E8                  MOV DWORD PTR SS:[EBP-0x18],EAX
	$-90     008D962D    8B47 04                  MOV EAX,DWORD PTR DS:[EDI+0x4]
	$-8D     008D9630    8945 EC                  MOV DWORD PTR SS:[EBP-0x14],EAX
	$-8A     008D9633    8D45 E0                  LEA EAX,DWORD PTR SS:[EBP-0x20]
	$-87     008D9636    50                       PUSH EAX
	$-86     008D9637    FFB6 983A0000            PUSH DWORD PTR DS:[ESI+0x3A98]
	$-80     008D963D    E8 EE2CDEFF              CALL PathOfEx.006BC330
	$-7B     008D9642    8BC8                     MOV ECX,EAX
	$-79     008D9644    E8 07300100              CALL PathOfEx.008EC650
	$-74     008D9649    8B8E 983A0000            MOV ECX,DWORD PTR DS:[ESI+0x3A98]
	$-6E     008D964F    6A 00                    PUSH 0x0
	$-6C     008D9651    E8 FADDEBFF              CALL PathOfEx.00797450
	$-67     008D9656    8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-64     008D9659    C786 383B0000 00000000   MOV DWORD PTR DS:[ESI+0x3B38],0x0
	$-5A     008D9663    8B07                     MOV EAX,DWORD PTR DS:[EDI]
	$-58     008D9665    8986 3C3B0000            MOV DWORD PTR DS:[ESI+0x3B3C],EAX
	$-52     008D966B    8B47 04                  MOV EAX,DWORD PTR DS:[EDI+0x4]
	$-4F     008D966E    5F                       POP EDI
	$-4E     008D966F    8986 403B0000            MOV DWORD PTR DS:[ESI+0x3B40],EAX
	$-48     008D9675    5B                       POP EBX
	$-47     008D9676    5E                       POP ESI
	$-46     008D9677    64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$-3F     008D967E    8BE5                     MOV ESP,EBP
	$-3D     008D9680    5D                       POP EBP
	$-3C     008D9681    C2 0400                  RETN 0x4
	$-39     008D9684    CC                       INT3
	$-38     008D9685    CC                       INT3
	$-37     008D9686    CC                       INT3
	$-36     008D9687    CC                       INT3
	$-35     008D9688    CC                       INT3
	$-34     008D9689    CC                       INT3
	$-33     008D968A    CC                       INT3
	$-32     008D968B    CC                       INT3
	$-31     008D968C    CC                       INT3
	$-30     008D968D    CC                       INT3
	$-2F     008D968E    CC                       INT3
	$-2E     008D968F    CC                       INT3
	$-2D     008D9690    55                       PUSH EBP                                         ; tag_中止当前技能
	$-2C     008D9691    8BEC                     MOV EBP,ESP                                      ; ecx=stCD_GameLogic*
	$-2A     008D9693    64:A1 00000000           MOV EAX,DWORD PTR FS:[0]                         ; 没有参数
	$-24     008D9699    6A FF                    PUSH -0x1
	$-22     008D969B    68 707FD600              PUSH PathOfEx.00D67F70
	$-1D     008D96A0    50                       PUSH EAX
	$-1C     008D96A1    64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$-15     008D96A8    83EC 08                  SUB ESP,0x8
	$-12     008D96AB    56                       PUSH ESI
	$-11     008D96AC    8BF1                     MOV ESI,ECX
	$-F      008D96AE    8B86 943A0000            MOV EAX,DWORD PTR DS:[ESI+0x3A94]
	$-9      008D96B4    83B8 E8010000 02         CMP DWORD PTR DS:[EAX+0x1E8],0x2
	$-2      008D96BB    75 49                    JNZ SHORT PathOfEx.008D9706
	$ ==>    008D96BD    B8 CF000000              MOV EAX,0xCF
	$+5      008D96C2    C745 EC 78D0EF00         MOV DWORD PTR SS:[EBP-0x14],PathOfEx.00EFD078
	$+C      008D96C9    66:8945 F0               MOV WORD PTR SS:[EBP-0x10],AX
	$+10     008D96CD    8D45 EC                  LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+13     008D96D0    C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$+1A     008D96D7    50                       PUSH EAX
	$+1B     008D96D8    FFB6 983A0000            PUSH DWORD PTR DS:[ESI+0x3A98]
	$+21     008D96DE    E8 4D2CDEFF              CALL PathOfEx.006BC330
	$+26     008D96E3    8BC8                     MOV ECX,EAX
	$+28     008D96E5    E8 662F0100              CALL PathOfEx.008EC650
	$+2D     008D96EA    8B8E 983A0000            MOV ECX,DWORD PTR DS:[ESI+0x3A98]
	$+33     008D96F0    6A 00                    PUSH 0x0
	$+35     008D96F2    E8 59DDEBFF              CALL PathOfEx.00797450
	$+3A     008D96F7    8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+3D     008D96FA    64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+44     008D9701    5E                       POP ESI
	$+45     008D9702    8BE5                     MOV ESP,EBP
	$+47     008D9704    5D                       POP EBP
	$+48     008D9705    C3                       RETN
	$+49     008D9706    8B86 B83C0000            MOV EAX,DWORD PTR DS:[ESI+0x3CB8]
	$+4F     008D970C    8B48 30                  MOV ECX,DWORD PTR DS:[EAX+0x30]
	$+52     008D970F    85C9                     TEST ECX,ECX
	$+54     008D9711    74 52                    JE SHORT PathOfEx.008D9765
	$+56     008D9713    8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+58     008D9715    8B40 4C                  MOV EAX,DWORD PTR DS:[EAX+0x4C]
	$+5B     008D9718    FFD0                     CALL EAX
	$+5D     008D971A    84C0                     TEST AL,AL
	$+5F     008D971C    74 47                    JE SHORT PathOfEx.008D9765
	$+61     008D971E    B8 D1000000              MOV EAX,0xD1
	$+66     008D9723    C745 EC 78D0EF00         MOV DWORD PTR SS:[EBP-0x14],PathOfEx.00EFD078
	$+6D     008D972A    66:8945 F0               MOV WORD PTR SS:[EBP-0x10],AX
	$+71     008D972E    8D45 EC                  LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+74     008D9731    C745 FC 01000000         MOV DWORD PTR SS:[EBP-0x4],0x1
	$+7B     008D9738    50                       PUSH EAX
	$+7C     008D9739    FFB6 983A0000            PUSH DWORD PTR DS:[ESI+0x3A98]
	$+82     008D973F    E8 EC2BDEFF              CALL PathOfEx.006BC330
	$+87     008D9744    8BC8                     MOV ECX,EAX
	$+89     008D9746    E8 052F0100              CALL PathOfEx.008EC650
	$+8E     008D974B    8B8E 983A0000            MOV ECX,DWORD PTR DS:[ESI+0x3A98]
	$+94     008D9751    6A 00                    PUSH 0x0
	$+96     008D9753    E8 F8DCEBFF              CALL PathOfEx.00797450
	$+9B     008D9758    8B8E B83C0000            MOV ECX,DWORD PTR DS:[ESI+0x3CB8]
	$+A1     008D975E    6A 00                    PUSH 0x0
	$+A3     008D9760    8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+A5     008D9762    FF50 50                  CALL DWORD PTR DS:[EAX+0x50]
	$+A8     008D9765    8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+AB     008D9768    33C0                     XOR EAX,EAX
	$+AD     008D976A    66:8986 443B0000         MOV WORD PTR DS:[ESI+0x3B44],AX                  ; g_o_cur_using_skill_id_,清空当前使用的技能ID
	$+B4     008D9771    64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+BB     008D9778    5E                       POP ESI
	$+BC     008D9779    8BE5                     MOV ESP,EBP
	$+BE     008D977B    5D                       POP EBP
	$+BF     008D977C    C3                       RETN
	$+C0     008D977D    CC                       INT3
	$+C1     008D977E    CC                       INT3
	$+C2     008D977F    CC                       INT3
	$+C3     008D9780    55                       PUSH EBP
	$+C4     008D9781    8BEC                     MOV EBP,ESP
	$+C6     008D9783    6A FF                    PUSH -0x1
	$+C8     008D9785    68 887FD600              PUSH PathOfEx.00D67F88
	$+CD     008D978A    64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$+D3     008D9790    50                       PUSH EAX
	$+D4     008D9791    64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$+DB     008D9798    83EC 24                  SUB ESP,0x24
	$+DE     008D979B    B8 32000000              MOV EAX,0x32
	$+E3     008D97A0    C745 D0 80CFEF00         MOV DWORD PTR SS:[EBP-0x30],PathOfEx.00EFCF80    ; ASCII "p诧"
	$+EA     008D97A7    56                       PUSH ESI
	$+EB     008D97A8    8BF1                     MOV ESI,ECX
	$+ED     008D97AA    66:8945 D4               MOV WORD PTR SS:[EBP-0x2C],AX
	$+F1     008D97AE    8B45 08                  MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+F4     008D97B1    C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$+FB     008D97B8    8945 D8                  MOV DWORD PTR SS:[EBP-0x28],EAX
	$+FE     008D97BB    8B45 14                  MOV EAX,DWORD PTR SS:[EBP+0x14]
	$+101    008D97BE    8945 DC                  MOV DWORD PTR SS:[EBP-0x24],EAX
	$+104    008D97C1    8B45 18                  MOV EAX,DWORD PTR SS:[EBP+0x18]
	$+107    008D97C4    8945 E0                  MOV DWORD PTR SS:[EBP-0x20],EAX
	$+10A    008D97C7    8A45 0C                  MOV AL,BYTE PTR SS:[EBP+0xC]
	$+10D    008D97CA    8845 EC                  MOV BYTE PTR SS:[EBP-0x14],AL
	$+110    008D97CD    8A45 10                  MOV AL,BYTE PTR SS:[EBP+0x10]
	$+113    008D97D0    8845 E4                  MOV BYTE PTR SS:[EBP-0x1C],AL
	$+116    008D97D3    8D45 D0                  LEA EAX,DWORD PTR SS:[EBP-0x30]
	$+119    008D97D6    50                       PUSH EAX
	$+11A    008D97D7    FFB6 983A0000            PUSH DWORD PTR DS:[ESI+0x3A98]
	$+120    008D97DD    F3:0F1155 E8             MOVSS DWORD PTR SS:[EBP-0x18],XMM2
	$+125    008D97E2    E8 492BDEFF              CALL PathOfEx.006BC330
	$+12A    008D97E7    8BC8                     MOV ECX,EAX
	$+12C    008D97E9    E8 622E0100              CALL PathOfEx.008EC650
	$+131    008D97EE    8B8E 983A0000            MOV ECX,DWORD PTR DS:[ESI+0x3A98]
	$+137    008D97F4    6A 00                    PUSH 0x0
	$+139    008D97F6    E8 55DCEBFF              CALL PathOfEx.00797450
	$+13E    008D97FB    8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+141    008D97FE    64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+148    008D9805    5E                       POP ESI
	$+149    008D9806    8BE5                     MOV ESP,EBP
	$+14B    008D9808    5D                       POP EBP
	$+14C    008D9809    C2 1400                  RETN 0x14
	$+14F    008D980C    CC                       INT3
	$+150    008D980D    CC                       INT3
	$+151    008D980E    CC                       INT3
	$+152    008D980F    CC                       INT3
	$+153    008D9810    55                       PUSH EBP
	$+154    008D9811    8BEC                     MOV EBP,ESP
	$+156    008D9813    6A FF                    PUSH -0x1
	$+158    008D9815    68 B8CDD200              PUSH PathOfEx.00D2CDB8
	$+15D    008D981A    64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$+163    008D9820    50                       PUSH EAX
	$+164    008D9821    64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$+16B    008D9828    83EC 0C                  SUB ESP,0xC
	$+16E    008D982B    B8 33000000              MOV EAX,0x33
	$+173    008D9830    C745 E8 48D0EF00         MOV DWORD PTR SS:[EBP-0x18],PathOfEx.00EFD048
	$+17A    008D9837    56                       PUSH ESI
	$+17B    008D9838    8BF1                     MOV ESI,ECX
	$+17D    008D983A    66:8945 EC               MOV WORD PTR SS:[EBP-0x14],AX
	$+181    008D983E    8B45 08                  MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+184    008D9841    C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$+18B    008D9848    8945 F0                  MOV DWORD PTR SS:[EBP-0x10],EAX
	$+18E    008D984B    8D45 E8                  LEA EAX,DWORD PTR SS:[EBP-0x18]
	$+191    008D984E    50                       PUSH EAX
	$+192    008D984F    FFB6 983A0000            PUSH DWORD PTR DS:[ESI+0x3A98]
	$+198    008D9855    E8 D62ADEFF              CALL PathOfEx.006BC330
	$+19D    008D985A    8BC8                     MOV ECX,EAX
	$+19F    008D985C    E8 EF2D0100              CALL PathOfEx.008EC650
	$+1A4    008D9861    8B8E 983A0000            MOV ECX,DWORD PTR DS:[ESI+0x3A98]
	$+1AA    008D9867    6A 00                    PUSH 0x0
	$+1AC    008D9869    E8 E2DBEBFF              CALL PathOfEx.00797450
	$+1B1    008D986E    8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+1B4    008D9871    5E                       POP ESI
	$+1B5    008D9872    64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+1BC    008D9879    8BE5                     MOV ESP,EBP
	$+1BE    008D987B    5D                       POP EBP
	$+1BF    008D987C    C2 0400                  RETN 0x4
	$+1C2    008D987F    CC                       INT3
	$+1C3    008D9880    55                       PUSH EBP
	$+1C4    008D9881    8BEC                     MOV EBP,ESP
	$+1C6    008D9883    6A FF                    PUSH -0x1
	$+1C8    008D9885    68 B67FD600              PUSH PathOfEx.00D67FB6
	$+1CD    008D988A    64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$+1D3    008D9890    50                       PUSH EAX
	$+1D4    008D9891    64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$+1DB    008D9898    83EC 18                  SUB ESP,0x18
	$+1DE    008D989B    53                       PUSH EBX
	$+1DF    008D989C    56                       PUSH ESI
	$+1E0    008D989D    57                       PUSH EDI
	$+1E1    008D989E    8B7D 08                  MOV EDI,DWORD PTR SS:[EBP+0x8]
	$+1E4    008D98A1    8D45 EC                  LEA EAX,DWORD PTR SS:[EBP-0x14]
	$+1E7    008D98A4    8BD9                     MOV EBX,ECX
	$+1E9    008D98A6    57                       PUSH EDI
	$+1EA    008D98A7    50                       PUSH EAX
	$+1EB    008D98A8    8D8B EC3C0000            LEA ECX,DWORD PTR DS:[EBX+0x3CEC]
	$+1F1    008D98AE    E8 DD12F2FF              CALL PathOfEx.007FAB90
	$+1F6    008D98B3    C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$+1FD    008D98BA    B8 30000000              MOV EAX,0x30
	$+202    008D98BF    66:8945 E0               MOV WORD PTR SS:[EBP-0x20],AX
	$+206    008D98C3    C745 DC C8CFEF00         MOV DWORD PTR SS:[EBP-0x24],PathOfEx.00EFCFC8
	$+20D    008D98CA    8B75 0C                  MOV ESI,DWORD PTR SS:[EBP+0xC]
	$+210    008D98CD    8D45 DC                  LEA EAX,DWORD PTR SS:[EBP-0x24]
	$+213    008D98D0    C645 FC 01               MOV BYTE PTR SS:[EBP-0x4],0x1
	$+217    008D98D4    50                       PUSH EAX
	$+218    008D98D5    FFB3 983A0000            PUSH DWORD PTR DS:[EBX+0x3A98]
	$+21E    008D98DB    897D E4                  MOV DWORD PTR SS:[EBP-0x1C],EDI
	$+221    008D98DE    8975 E8                  MOV DWORD PTR SS:[EBP-0x18],ESI
	$+224    008D98E1    E8 4A2ADEFF              CALL PathOfEx.006BC330
	$+229    008D98E6    8BC8                     MOV ECX,EAX
	$+22B    008D98E8    E8 632D0100              CALL PathOfEx.008EC650
	$+230    008D98ED    8B8B 983A0000            MOV ECX,DWORD PTR DS:[EBX+0x3A98]
	$+236    008D98F3    6A 00                    PUSH 0x0
	$+238    008D98F5    E8 56DBEBFF              CALL PathOfEx.00797450
	$+23D    008D98FA    56                       PUSH ESI
	$+23E    008D98FB    57                       PUSH EDI
	$+23F    008D98FC    8D8B 14390000            LEA ECX,DWORD PTR DS:[EBX+0x3914]
	$+245    008D9902    E8 F97EBBFF              CALL PathOfEx.00491800
	$+24A    008D9907    C745 DC 60D0EF00         MOV DWORD PTR SS:[EBP-0x24],PathOfEx.00EFD060
	$+251    008D990E    C745 FC 02000000         MOV DWORD PTR SS:[EBP-0x4],0x2
	$+258    008D9915    8B75 F0                  MOV ESI,DWORD PTR SS:[EBP-0x10]
	$+25B    008D9918    85F6                     TEST ESI,ESI
	$+25D    008D991A    74 21                    JE SHORT PathOfEx.008D993D
	$+25F    008D991C    83CF FF                  OR EDI,0xFFFFFFFF
	$+262    008D991F    8BC7                     MOV EAX,EDI
	$+264    008D9921    F0:0FC146 04             LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
	$+269    008D9926    75 15                    JNZ SHORT PathOfEx.008D993D
	$+26B    008D9928    8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$+26D    008D992A    8BCE                     MOV ECX,ESI
	$+26F    008D992C    FF10                     CALL DWORD PTR DS:[EAX]
	$+271    008D992E    F0:0FC17E 08             LOCK XADD DWORD PTR DS:[ESI+0x8],EDI
	$+276    008D9933    4F                       DEC EDI
	$+277    008D9934    75 07                    JNZ SHORT PathOfEx.008D993D
	$+279    008D9936    8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$+27B    008D9938    8BCE                     MOV ECX,ESI
	$+27D    008D993A    FF50 04                  CALL DWORD PTR DS:[EAX+0x4]
	$+280    008D993D    8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+283    008D9940    B0 01                    MOV AL,0x1
	$+285    008D9942    5F                       POP EDI
	$+286    008D9943    5E                       POP ESI
	$+287    008D9944    64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+28E    008D994B    5B                       POP EBX
	$+28F    008D994C    8BE5                     MOV ESP,EBP
	$+291    008D994E    5D                       POP EBP
	$+292    008D994F    C2 0800                  RETN 0x8
	$+295    008D9952    CC                       INT3
	$+296    008D9953    CC                       INT3
	$+297    008D9954    CC                       INT3
	$+298    008D9955    CC                       INT3
	$+299    008D9956    CC                       INT3
	$+29A    008D9957    CC                       INT3
	$+29B    008D9958    CC                       INT3
	$+29C    008D9959    CC                       INT3
	$+29D    008D995A    CC                       INT3
	$+29E    008D995B    CC                       INT3
	$+29F    008D995C    CC                       INT3
	$+2A0    008D995D    CC                       INT3
	$+2A1    008D995E    CC                       INT3
	$+2A2    008D995F    CC                       INT3
	$+2A3    008D9960    55                       PUSH EBP
	$+2A4    008D9961    8BEC                     MOV EBP,ESP
	$+2A6    008D9963    6A FF                    PUSH -0x1
	$+2A8    008D9965    68 887FD600              PUSH PathOfEx.00D67F88
	$+2AD    008D996A    64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$+2B3    008D9970    50                       PUSH EAX
	$+2B4    008D9971    64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$+2BB    008D9978    83EC 24                  SUB ESP,0x24
	$+2BE    008D997B    B8 36000000              MOV EAX,0x36
	$+2C3    008D9980    C745 D0 3CCEEF00         MOV DWORD PTR SS:[EBP-0x30],PathOfEx.00EFCE3C    ; ASCII "p诧"
	$+2CA    008D9987    56                       PUSH ESI
	$+2CB    008D9988    8BF1                     MOV ESI,ECX
	$+2CD    008D998A    66:8945 D4               MOV WORD PTR SS:[EBP-0x2C],AX
	$+2D1    008D998E    8B45 08                  MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+2D4    008D9991    C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$+2DB    008D9998    8945 D8                  MOV DWORD PTR SS:[EBP-0x28],EAX
	$+2DE    008D999B    8B45 0C                  MOV EAX,DWORD PTR SS:[EBP+0xC]
	$+2E1    008D999E    8945 E8                  MOV DWORD PTR SS:[EBP-0x18],EAX
	$+2E4    008D99A1    66:8B45 10               MOV AX,WORD PTR SS:[EBP+0x10]
	$+2E8    008D99A5    66:8945 EC               MOV WORD PTR SS:[EBP-0x14],AX
	$+2EC    008D99A9    66:8B45 14               MOV AX,WORD PTR SS:[EBP+0x14]
	$+2F0    008D99AD    66:8945 EE               MOV WORD PTR SS:[EBP-0x12],AX
	$+2F4    008D99B1    8B45 18                  MOV EAX,DWORD PTR SS:[EBP+0x18]
	$+2F7    008D99B4    8945 DC                  MOV DWORD PTR SS:[EBP-0x24],EAX
	$+2FA    008D99B7    8B45 1C                  MOV EAX,DWORD PTR SS:[EBP+0x1C]
	$+2FD    008D99BA    8945 E0                  MOV DWORD PTR SS:[EBP-0x20],EAX
	$+300    008D99BD    8A45 20                  MOV AL,BYTE PTR SS:[EBP+0x20]
	$+303    008D99C0    8845 E4                  MOV BYTE PTR SS:[EBP-0x1C],AL
	$+306    008D99C3    8D45 D0                  LEA EAX,DWORD PTR SS:[EBP-0x30]
	$+309    008D99C6    50                       PUSH EAX
	$+30A    008D99C7    FFB6 983A0000            PUSH DWORD PTR DS:[ESI+0x3A98]
	$+310    008D99CD    E8 5E29DEFF              CALL PathOfEx.006BC330
	$+315    008D99D2    8BC8                     MOV ECX,EAX
	$+317    008D99D4    E8 772C0100              CALL PathOfEx.008EC650
	$+31C    008D99D9    8B8E 983A0000            MOV ECX,DWORD PTR DS:[ESI+0x3A98]
	$+322    008D99DF    6A 00                    PUSH 0x0
	$+324    008D99E1    E8 6ADAEBFF              CALL PathOfEx.00797450
	$+329    008D99E6    8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+32C    008D99E9    64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+333    008D99F0    5E                       POP ESI
	$+334    008D99F1    8BE5                     MOV ESP,EBP
	$+336    008D99F3    5D                       POP EBP
	$+337    008D99F4    C2 1C00                  RETN 0x1C
	$+33A    008D99F7    CC                       INT3
	$+33B    008D99F8    CC                       INT3
	$+33C    008D99F9    CC                       INT3
	$+33D    008D99FA    CC                       INT3
	$+33E    008D99FB    CC                       INT3
	$+33F    008D99FC    CC                       INT3
	$+340    008D99FD    CC                       INT3
	$+341    008D99FE    CC                       INT3
	$+342    008D99FF    CC                       INT3
	$+343    008D9A00    55                       PUSH EBP
	$+344    008D9A01    8BEC                     MOV EBP,ESP
	$+346    008D9A03    64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$+34C    008D9A09    6A FF                    PUSH -0x1
	$+34E    008D9A0B    68 B8CDD200              PUSH PathOfEx.00D2CDB8
	$+353    008D9A10    50                       PUSH EAX
	$+354    008D9A11    64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$+35B    008D9A18    83EC 0C                  SUB ESP,0xC
	$+35E    008D9A1B    56                       PUSH ESI
	$+35F    008D9A1C    8BF1                     MOV ESI,ECX
	$+361    008D9A1E    8B8E 943A0000            MOV ECX,DWORD PTR DS:[ESI+0x3A94]
	$+367    008D9A24    8B41 08                  MOV EAX,DWORD PTR DS:[ECX+0x8]
	$+36A    008D9A27    8A40 0C                  MOV AL,BYTE PTR DS:[EAX+0xC]
	$+36D    008D9A2A    84C0                     TEST AL,AL
	$+36F    008D9A2C    74 13                    JE SHORT PathOfEx.008D9A41
	$+371    008D9A2E    32C0                     XOR AL,AL
	$+373    008D9A30    5E                       POP ESI
	$+374    008D9A31    8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+377    008D9A34    64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+37E    008D9A3B    8BE5                     MOV ESP,EBP
	$+380    008D9A3D    5D                       POP EBP
	$+381    008D9A3E    C2 0400                  RETN 0x4
	$+384    008D9A41    8B89 C4010000            MOV ECX,DWORD PTR DS:[ECX+0x1C4]
	$+38A    008D9A47    E8 C423B9FF              CALL PathOfEx.0046BE10
	$+38F    008D9A4C    8378 30 00               CMP DWORD PTR DS:[EAX+0x30],0x0
	$+393    008D9A50  ^ 7E DC                    JLE SHORT PathOfEx.008D9A2E
	$+395    008D9A52    803D 40CDFF00 00         CMP BYTE PTR DS:[0xFFCD40],0x0
	$+39C    008D9A59  ^ 75 D3                    JNZ SHORT PathOfEx.008D9A2E
	$+39E    008D9A5B    B8 34000000              MOV EAX,0x34
	$+3A3    008D9A60    C745 E8 48D0EF00         MOV DWORD PTR SS:[EBP-0x18],PathOfEx.00EFD048
	$+3AA    008D9A67    66:8945 EC               MOV WORD PTR SS:[EBP-0x14],AX
	$+3AE    008D9A6B    8B45 08                  MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+3B1    008D9A6E    C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$+3B8    008D9A75    8945 F0                  MOV DWORD PTR SS:[EBP-0x10],EAX
	$+3BB    008D9A78    8D45 E8                  LEA EAX,DWORD PTR SS:[EBP-0x18]
	$+3BE    008D9A7B    50                       PUSH EAX
	$+3BF    008D9A7C    FFB6 983A0000            PUSH DWORD PTR DS:[ESI+0x3A98]
	$+3C5    008D9A82    E8 A928DEFF              CALL PathOfEx.006BC330
	$+3CA    008D9A87    8BC8                     MOV ECX,EAX
	$+3CC    008D9A89    E8 C22B0100              CALL PathOfEx.008EC650
	$+3D1    008D9A8E    8B8E 983A0000            MOV ECX,DWORD PTR DS:[ESI+0x3A98]
	$+3D7    008D9A94    6A 00                    PUSH 0x0
	$+3D9    008D9A96    E8 B5D9EBFF              CALL PathOfEx.00797450
	$+3DE    008D9A9B    8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+3E1    008D9A9E    B0 01                    MOV AL,0x1
	$+3E3    008D9AA0    5E                       POP ESI
	$+3E4    008D9AA1    64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+3EB    008D9AA8    8BE5                     MOV ESP,EBP
	$+3ED    008D9AAA    5D                       POP EBP
	$+3EE    008D9AAB    C2 0400                  RETN 0x4
	$+3F1    008D9AAE    CC                       INT3
	$+3F2    008D9AAF    CC                       INT3
	$+3F3    008D9AB0    55                       PUSH EBP
	$+3F4    008D9AB1    8BEC                     MOV EBP,ESP
	$+3F6    008D9AB3    6A FF                    PUSH -0x1
	$+3F8    008D9AB5    68 E883D200              PUSH PathOfEx.00D283E8
	$+3FD    008D9ABA    64:A1 00000000           MOV EAX,DWORD PTR FS:[0]
	$+403    008D9AC0    50                       PUSH EAX
	$+404    008D9AC1    64:8925 00000000         MOV DWORD PTR FS:[0],ESP
	$+40B    008D9AC8    83EC 18                  SUB ESP,0x18
	$+40E    008D9ACB    53                       PUSH EBX
	$+40F    008D9ACC    56                       PUSH ESI
	$+410    008D9ACD    B8 35000000              MOV EAX,0x35
	$+415    008D9AD2    C745 DC 20CFEF00         MOV DWORD PTR SS:[EBP-0x24],PathOfEx.00EFCF20
	$+41C    008D9AD9    57                       PUSH EDI
	$+41D    008D9ADA    8BD9                     MOV EBX,ECX
	$+41F    008D9ADC    66:8945 E0               MOV WORD PTR SS:[EBP-0x20],AX
	$+423    008D9AE0    8B45 10                  MOV EAX,DWORD PTR SS:[EBP+0x10]
	$+426    008D9AE3    8B7D 08                  MOV EDI,DWORD PTR SS:[EBP+0x8]
	$+429    008D9AE6    8B75 0C                  MOV ESI,DWORD PTR SS:[EBP+0xC]
	$+42C    008D9AE9    C745 FC 00000000         MOV DWORD PTR SS:[EBP-0x4],0x0
	$+433    008D9AF0    8945 F0                  MOV DWORD PTR SS:[EBP-0x10],EAX
	$+436    008D9AF3    8B45 14                  MOV EAX,DWORD PTR SS:[EBP+0x14]
	$+439    008D9AF6    8945 EC                  MOV DWORD PTR SS:[EBP-0x14],EAX
	$+43C    008D9AF9    8D45 DC                  LEA EAX,DWORD PTR SS:[EBP-0x24]
	$+43F    008D9AFC    50                       PUSH EAX
	$+440    008D9AFD    FFB3 983A0000            PUSH DWORD PTR DS:[EBX+0x3A98]
	$+446    008D9B03    897D E8                  MOV DWORD PTR SS:[EBP-0x18],EDI
	$+449    008D9B06    8975 E4                  MOV DWORD PTR SS:[EBP-0x1C],ESI
	$+44C    008D9B09    E8 2228DEFF              CALL PathOfEx.006BC330
	$+451    008D9B0E    8BC8                     MOV ECX,EAX
	$+453    008D9B10    E8 3B2B0100              CALL PathOfEx.008EC650
	$+458    008D9B15    8B8B 983A0000            MOV ECX,DWORD PTR DS:[EBX+0x3A98]
	$+45E    008D9B1B    6A 00                    PUSH 0x0
	$+460    008D9B1D    E8 2ED9EBFF              CALL PathOfEx.00797450
	$+465    008D9B22    56                       PUSH ESI
	$+466    008D9B23    57                       PUSH EDI
	$+467    008D9B24    8D8B 14390000            LEA ECX,DWORD PTR DS:[EBX+0x3914]
	$+46D    008D9B2A    E8 D17CBBFF              CALL PathOfEx.00491800
	$+472    008D9B2F    8B4D F4                  MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+475    008D9B32    5F                       POP EDI
	$+476    008D9B33    5E                       POP ESI
	$+477    008D9B34    64:890D 00000000         MOV DWORD PTR FS:[0],ECX
	$+47E    008D9B3B    5B                       POP EBX
	$+47F    008D9B3C    8BE5                     MOV ESP,EBP
	$+481    008D9B3E    5D                       POP EBP
	$+482    008D9B3F    C2 1000                  RETN 0x10
	$+485    008D9B42    CC                       INT3
	*/
#ifdef VER_GF_
	addr_tmp = scan_exe_.ScanAddr("b8????????c7????????????66??????8d????c7????????????50ff??????????e8????????8b??e8????????8b??????????6a??e8????????8b????64????????????5e8b??5dc38b??????????8b????85??74??8b??8b????ff??84??74??b8????????c7????????????66??????8d????c7????????????50ff??????????e8????????8b??e8????????8b??????????6a??e8????????8b??????????6a??8b??ff????8b????33??66", 0);
#else
	addr_tmp = scan_exe_.ScanAddr("b8????????c7????????????66??????c7????????????8d????ff??????????e8????????8b??????????6a??e8????????8b????64????????????5e8b??5dc38b??????????8b????85", 0);
#endif
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
#ifdef VER_GF_
	dw_tmp = AsDword(addr_tmp, 0xad + 3);
#else
	dw_tmp = AsDword(addr_tmp, 0x9d + 3);
#endif
	if (dw_tmp < 0x3800 || dw_tmp > 0x4b00)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_o_cur_using_skill_id_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	addr_tmp1 = (char*)addr_tmp - 0x2d;
	if (AsByte(addr_tmp1, 0) != 0x55)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_c_terminate_cur_using_skill_ = " << ExeAddr2Offset(addr_tmp1) << ";" << FC_OFstream_EndlLog;

	//去搜 call tag_查找GrantedEffectsPerLevelPair、g_so_GrantedEffectsPerLevel_require_lvl_
	/*
	$-122    00F1FF8B     8D8D 40ECFFFF            LEA ECX,DWORD PTR SS:[EBP-0x13C0]
	$-11C    00F1FF91     F20F5804C5 A0157901      ADDSD XMM0,QWORD PTR DS:[EAX*8+0x17915A0]
	$-113    00F1FF9A     660F5AD0                 CVTPD2PS XMM2,XMM0
	$-10F    00F1FF9E     66:0F6EC2                MOVD MM0,EDX
	$-10B    00F1FFA2     F30FE6C0                 CVTDQ2PD XMM0,XMM0
	$-107    00F1FFA6     C1EA 1F                  SHR EDX,0x1F
	$-104    00F1FFA9     F20F5804D5 A0157901      ADDSD XMM0,QWORD PTR DS:[EDX*8+0x17915A0]
	$-FB     00F1FFB2     660F5AC0                 CVTPD2PS XMM0,XMM0
	$-F7     00F1FFB6     F3:0F5ED0                DIVSS XMM2,XMM0
	$-F3     00F1FFBA     E8 71CBFFFF              CALL PathOfEx.00F1CB30
	$-EE     00F1FFBF     8BD0                     MOV EDX,EAX
	$-EC     00F1FFC1     8955 6C                  MOV DWORD PTR SS:[EBP+0x6C],EDX
	$-E9     00F1FFC4     8D4F 4C                  LEA ECX,DWORD PTR DS:[EDI+0x4C]
	$-E6     00F1FFC7     C645 FC 29               MOV BYTE PTR SS:[EBP-0x4],0x29
	$-E2     00F1FFCB     8B41 04                  MOV EAX,DWORD PTR DS:[ECX+0x4]
	$-DF     00F1FFCE     3B41 08                  CMP EAX,DWORD PTR DS:[ECX+0x8]
	$-DC     00F1FFD1     75 0C                    JNZ SHORT PathOfEx.00F1FFDF
	$-DA     00F1FFD3     51                       PUSH ECX
	$-D9     00F1FFD4     E8 77670000              CALL PathOfEx.00F26750
	$-D4     00F1FFD9     8B55 6C                  MOV EDX,DWORD PTR SS:[EBP+0x6C]
	$-D1     00F1FFDC     8D4F 4C                  LEA ECX,DWORD PTR DS:[EDI+0x4C]
	$-CE     00F1FFDF     8B41 04                  MOV EAX,DWORD PTR DS:[ECX+0x4]
	$-CB     00F1FFE2     85C0                     TEST EAX,EAX
	$-C9     00F1FFE4     74 0B                    JE SHORT PathOfEx.00F1FFF1
	$-C7     00F1FFE6     52                       PUSH EDX
	$-C6     00F1FFE7     8BC8                     MOV ECX,EAX
	$-C4     00F1FFE9     E8 62CFFFFF              CALL PathOfEx.00F1CF50
	$-BF     00F1FFEE     8D4F 4C                  LEA ECX,DWORD PTR DS:[EDI+0x4C]
	$-BC     00F1FFF1     8341 04 48               ADD DWORD PTR DS:[ECX+0x4],0x48
	$-B8     00F1FFF5     8D8D 40ECFFFF            LEA ECX,DWORD PTR SS:[EBP-0x13C0]
	$-B2     00F1FFFB     E8 50C1E8FF              CALL PathOfEx.00DAC150
	$-AD     00F20000     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-A6     00F20007     8B8D A4FDFFFF            MOV ECX,DWORD PTR SS:[EBP-0x25C]
	$-A0     00F2000D     85C9                     TEST ECX,ECX
	$-9E     00F2000F     74 35                    JE SHORT PathOfEx.00F20046
	$-9C     00F20011     8B85 ACFDFFFF            MOV EAX,DWORD PTR SS:[EBP-0x254]
	$-96     00F20017     2BC1                     SUB EAX,ECX
	$-94     00F20019     C1F8 03                  SAR EAX,0x3
	$-91     00F2001C     6A 08                    PUSH 0x8
	$-8F     00F2001E     50                       PUSH EAX
	$-8E     00F2001F     51                       PUSH ECX
	$-8D     00F20020     E8 8B90D5FF              CALL PathOfEx.00C790B0
	$-88     00F20025     83C4 0C                  ADD ESP,0xC
	$-85     00F20028     C785 A4FDFFFF 00000000   MOV DWORD PTR SS:[EBP-0x25C],0x0
	$-7B     00F20032     C785 A8FDFFFF 00000000   MOV DWORD PTR SS:[EBP-0x258],0x0
	$-71     00F2003C     C785 ACFDFFFF 00000000   MOV DWORD PTR SS:[EBP-0x254],0x0
	$-67     00F20046     8D8D 98FDFFFF            LEA ECX,DWORD PTR SS:[EBP-0x268]
	$-61     00F2004C     E8 9F63D6FF              CALL PathOfEx.00C863F0
	$-5C     00F20051     8B45 28                  MOV EAX,DWORD PTR SS:[EBP+0x28]
	$-59     00F20054     8B50 10                  MOV EDX,DWORD PTR DS:[EAX+0x10]
	$-56     00F20057     8D8D CCFEFFFF            LEA ECX,DWORD PTR SS:[EBP-0x134]
	$-50     00F2005D     FF70 18                  PUSH DWORD PTR DS:[EAX+0x18]
	$-4D     00F20060     83C2 08                  ADD EDX,0x8
	$-4A     00F20063     E8 28700800              CALL PathOfEx.00FA7090                      ; call tag_查找GrantedEffectsPerLevelPair
	$-45     00F20068     8B85 D0FEFFFF            MOV EAX,DWORD PTR SS:[EBP-0x130]
	$-3F     00F2006E     83C4 04                  ADD ESP,0x4
	$-3C     00F20071     85C0                     TEST EAX,EAX
	$-3A     00F20073     75 38                    JNZ SHORT PathOfEx.00F200AD
	$-38     00F20075     C745 FC 2A000000         MOV DWORD PTR SS:[EBP-0x4],0x2A
	$-31     00F2007C     8B85 CCFEFFFF            MOV EAX,DWORD PTR SS:[EBP-0x134]
	$-2B     00F20082     85C0                     TEST EAX,EAX
	$-29     00F20084     74 1B                    JE SHORT PathOfEx.00F200A1
	$-27     00F20086     50                       PUSH EAX
	$-26     00F20087     51                       PUSH ECX
	$-25     00F20088     E8 9349FFFF              CALL PathOfEx.00F14A20
	$-20     00F2008D     83C4 04                  ADD ESP,0x4
	$-1D     00F20090     8BC8                     MOV ECX,EAX
	$-1B     00F20092     E8 1945FFFF              CALL PathOfEx.00F145B0
	$-16     00F20097     C785 CCFEFFFF 00000000   MOV DWORD PTR SS:[EBP-0x134],0x0
	$-C      00F200A1     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-5      00F200A8     E9 16010000              JMP PathOfEx.00F201C3
	$ ==>    00F200AD     8B40 3C                  MOV EAX,DWORD PTR DS:[EAX+0x3C]             ; g_so_GrantedEffectsPerLevel_require_lvl_
	$+3      00F200B0     8945 6C                  MOV DWORD PTR SS:[EBP+0x6C],EAX
	$+6      00F200B3     C745 FC 2B000000         MOV DWORD PTR SS:[EBP-0x4],0x2B
	$+D      00F200BA     8B8D CCFEFFFF            MOV ECX,DWORD PTR SS:[EBP-0x134]
	$+13     00F200C0     85C9                     TEST ECX,ECX
	$+15     00F200C2     74 1E                    JE SHORT PathOfEx.00F200E2
	$+17     00F200C4     51                       PUSH ECX
	$+18     00F200C5     51                       PUSH ECX
	$+19     00F200C6     E8 5549FFFF              CALL PathOfEx.00F14A20
	$+1E     00F200CB     83C4 04                  ADD ESP,0x4
	$+21     00F200CE     8BC8                     MOV ECX,EAX
	$+23     00F200D0     E8 DB44FFFF              CALL PathOfEx.00F145B0
	$+28     00F200D5     8B45 6C                  MOV EAX,DWORD PTR SS:[EBP+0x6C]
	$+2B     00F200D8     C785 CCFEFFFF 00000000   MOV DWORD PTR SS:[EBP-0x134],0x0
	$+35     00F200E2     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+3C     00F200E9     85C0                     TEST EAX,EAX
	$+3E     00F200EB     0F84 D2000000            JE PathOfEx.00F201C3
	$+44     00F200F1     8D8F 28020000            LEA ECX,DWORD PTR DS:[EDI+0x228]
	$+4A     00F200F7     E8 D45ED6FF              CALL PathOfEx.00C85FD0
	$+4F     00F200FC     837E 14 08               CMP DWORD PTR DS:[ESI+0x14],0x8
	$+53     00F20100     72 04                    JB SHORT PathOfEx.00F20106
	$+55     00F20102     8B0E                     MOV ECX,DWORD PTR DS:[ESI]
	$+57     00F20104     EB 02                    JMP SHORT PathOfEx.00F20108
	$+59     00F20106     8BCE                     MOV ECX,ESI
	$+5B     00F20108     FFB7 64020000            PUSH DWORD PTR DS:[EDI+0x264]
	$+61     00F2010E     FF76 10                  PUSH DWORD PTR DS:[ESI+0x10]
	$+64     00F20111     51                       PUSH ECX
	$+65     00F20112     8D8F 28020000            LEA ECX,DWORD PTR DS:[EDI+0x228]
	$+6B     00F20118     E8 A351D6FF              CALL PathOfEx.00C852C0
	$+70     00F2011D     FF75 6C                  PUSH DWORD PTR SS:[EBP+0x6C]
	$+73     00F20120     8D8F 20020000            LEA ECX,DWORD PTR DS:[EDI+0x220]
	$+79     00F20126     E8 A5DDD6FF              CALL PathOfEx.00C8DED0
	$+7E     00F2012B     8B45 58                  MOV EAX,DWORD PTR SS:[EBP+0x58]
	$+81     00F2012E     8D8F 10020000            LEA ECX,DWORD PTR DS:[EDI+0x210]
	$+87     00F20134     3B45 6C                  CMP EAX,DWORD PTR SS:[EBP+0x6C]
	$+8A     00F20137     0F9245 6C                SETB BYTE PTR SS:[EBP+0x6C]
	$+8E     00F2013B     83EC 18                  SUB ESP,0x18
	$+91     00F2013E     8BC4                     MOV EAX,ESP
	$+93     00F20140     8965 50                  MOV DWORD PTR SS:[EBP+0x50],ESP
	$+96     00F20143     50                       PUSH EAX
	$+97     00F20144     E8 675CD7FF              CALL PathOfEx.00C95DB0
	$+9C     00F20149     83EC 18                  SUB ESP,0x18
	$+9F     00F2014C     C745 FC 2C000000         MOV DWORD PTR SS:[EBP-0x4],0x2C
	$+A6     00F20153     BA 97000000              MOV EDX,0x97
	$+AB     00F20158     8BCC                     MOV ECX,ESP
	$+AD     00F2015A     E8 019DFFFF              CALL PathOfEx.00F19E60
	$+B2     00F2015F     FF75 6C                  PUSH DWORD PTR SS:[EBP+0x6C]
	$+B5     00F20162     8D8D F8EBFFFF            LEA ECX,DWORD PTR SS:[EBP-0x1408]
	$+BB     00F20168     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+C2     00F2016F     6A 00                    PUSH 0x0
	$+C4     00F20171     6A 00                    PUSH 0x0
	$+C6     00F20173     E8 78C3FFFF              CALL PathOfEx.00F1C4F0
	$+CB     00F20178     8BD0                     MOV EDX,EAX
	$+CD     00F2017A     8955 6C                  MOV DWORD PTR SS:[EBP+0x6C],EDX
	$+D0     00F2017D     8D4F 40                  LEA ECX,DWORD PTR DS:[EDI+0x40]
	$+D3     00F20180     C745 FC 2D000000         MOV DWORD PTR SS:[EBP-0x4],0x2D
	$+DA     00F20187     8B41 04                  MOV EAX,DWORD PTR DS:[ECX+0x4]
	$+DD     00F2018A     3B41 08                  CMP EAX,DWORD PTR DS:[ECX+0x8]
	$+E0     00F2018D     75 0C                    JNZ SHORT PathOfEx.00F2019B
	$+E2     00F2018F     51                       PUSH ECX
	$+E3     00F20190     E8 BB650000              CALL PathOfEx.00F26750
	$+E8     00F20195     8B55 6C                  MOV EDX,DWORD PTR SS:[EBP+0x6C]
	$+EB     00F20198     8D4F 40                  LEA ECX,DWORD PTR DS:[EDI+0x40]
	$+EE     00F2019B     8B41 04                  MOV EAX,DWORD PTR DS:[ECX+0x4]
	$+F1     00F2019E     85C0                     TEST EAX,EAX
	$+F3     00F201A0     74 0B                    JE SHORT PathOfEx.00F201AD
	$+F5     00F201A2     52                       PUSH EDX
	$+F6     00F201A3     8BC8                     MOV ECX,EAX
	$+F8     00F201A5     E8 A6CDFFFF              CALL PathOfEx.00F1CF50
	$+FD     00F201AA     8D4F 40                  LEA ECX,DWORD PTR DS:[EDI+0x40]
	$+100    00F201AD     8341 04 48               ADD DWORD PTR DS:[ECX+0x4],0x48
	$+104    00F201B1     8D8D F8EBFFFF            LEA ECX,DWORD PTR SS:[EBP-0x1408]
	$+10A    00F201B7     C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+111    00F201BE     E8 8DBFE8FF              CALL PathOfEx.00DAC150
	$+116    00F201C3     33DB                     XOR EBX,EBX
	$+118    00F201C5     8D87 10020000            LEA EAX,DWORD PTR DS:[EDI+0x210]
	$+11E    00F201CB     33C9                     XOR ECX,ECX
	$+120    00F201CD     8D97 20020000            LEA EDX,DWORD PTR DS:[EDI+0x220]
	$+126    00F201D3     85C0                     TEST EAX,EAX
	$+128    00F201D5     894D B8                  MOV DWORD PTR SS:[EBP-0x48],ECX
	$+12B    00F201D8     B8 5C716501              MOV EAX,PathOfEx.0165715C
	$+130    00F201DD     0F44D3                   CMOVE EDX,EBX
	$+133    00F201E0     8D9F 28020000            LEA EBX,DWORD PTR DS:[EDI+0x228]
	$+139    00F201E6     8955 50                  MOV DWORD PTR SS:[EBP+0x50],EDX
	$+13C    00F201E9     8B55 54                  MOV EDX,DWORD PTR SS:[EBP+0x54]
	$+13F    00F201EC     2BC2                     SUB EAX,EDX
	$+141    00F201EE     8955 6C                  MOV DWORD PTR SS:[EBP+0x6C],EDX
	$+144    00F201F1     8945 C8                  MOV DWORD PTR SS:[EBP-0x38],EAX
	$+147    00F201F4     6A 00                    PUSH 0x0
	$+149    00F201F6     51                       PUSH ECX
	$+14A    00F201F7     8B4D 28                  MOV ECX,DWORD PTR SS:[EBP+0x28]
	$+14D    00F201FA     E8 01050300              CALL PathOfEx.00F50700
	$+152    00F201FF     8945 CC                  MOV DWORD PTR SS:[EBP-0x34],EAX
	$+155    00F20202     85C0                     TEST EAX,EAX
	$+157    00F20204     0F84 2E020000            JE PathOfEx.00F20438
	$+15D    00F2020A     8BCB                     MOV ECX,EBX
	$+15F    00F2020C     E8 BF5DD6FF              CALL PathOfEx.00C85FD0
	$+164    00F20211     837E 14 08               CMP DWORD PTR DS:[ESI+0x14],0x8
	$+168    00F20215     72 04                    JB SHORT PathOfEx.00F2021B
	$+16A    00F20217     8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$+16C    00F20219     EB 02                    JMP SHORT PathOfEx.00F2021D
	$+16E    00F2021B     8BC6                     MOV EAX,ESI
	$+170    00F2021D     FF73 3C                  PUSH DWORD PTR DS:[EBX+0x3C]
	$+173    00F20220     8BCB                     MOV ECX,EBX
	$+175    00F20222     FF76 10                  PUSH DWORD PTR DS:[ESI+0x10]
	*/

	//gj3
	/*
	$-2E4    016A7B72      83F8 08                  CMP EAX,0x8
	$-2E1    016A7B75      72 12                    JB SHORT PathOfEx.016A7B89
	$-2DF    016A7B77      40                       INC EAX
	$-2DE    016A7B78      6A 02                    PUSH 0x2
	$-2DC    016A7B7A      50                       PUSH EAX
	$-2DB    016A7B7B      FFB5 BCFBFFFF            PUSH DWORD PTR SS:[EBP-0x444]
	$-2D5    016A7B81      E8 EAD7D0FF              CALL PathOfEx.013B5370
	$-2D0    016A7B86      83C4 0C                  ADD ESP,0xC
	$-2CD    016A7B89      33C0                     XOR EAX,EAX
	$-2CB    016A7B8B      C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-2C4    016A7B92      8B8D 94FDFFFF            MOV ECX,DWORD PTR SS:[EBP-0x26C]
	$-2BE    016A7B98      C785 D0FBFFFF 07000000   MOV DWORD PTR SS:[EBP-0x430],0x7
	$-2B4    016A7BA2      C785 CCFBFFFF 00000000   MOV DWORD PTR SS:[EBP-0x434],0x0
	$-2AA    016A7BAC      66:8985 BCFBFFFF         MOV WORD PTR SS:[EBP-0x444],AX
	$-2A3    016A7BB3      85C9                     TEST ECX,ECX
	$-2A1    016A7BB5      74 35                    JE SHORT PathOfEx.016A7BEC
	$-29F    016A7BB7      8B85 9CFDFFFF            MOV EAX,DWORD PTR SS:[EBP-0x264]
	$-299    016A7BBD      2BC1                     SUB EAX,ECX
	$-297    016A7BBF      C1F8 03                  SAR EAX,0x3
	$-294    016A7BC2      6A 08                    PUSH 0x8
	$-292    016A7BC4      50                       PUSH EAX
	$-291    016A7BC5      51                       PUSH ECX
	$-290    016A7BC6      E8 A5D7D0FF              CALL PathOfEx.013B5370
	$-28B    016A7BCB      83C4 0C                  ADD ESP,0xC
	$-288    016A7BCE      C785 94FDFFFF 00000000   MOV DWORD PTR SS:[EBP-0x26C],0x0
	$-27E    016A7BD8      C785 98FDFFFF 00000000   MOV DWORD PTR SS:[EBP-0x268],0x0
	$-274    016A7BE2      C785 9CFDFFFF 00000000   MOV DWORD PTR SS:[EBP-0x264],0x0
	$-26A    016A7BEC      8D8D 88FDFFFF            LEA ECX,DWORD PTR SS:[EBP-0x278]
	$-264    016A7BF2      E8 A91FD1FF              CALL PathOfEx.013B9BA0
	$-25F    016A7BF7      83EC 18                  SUB ESP,0x18
	$-25C    016A7BFA      8D8F 10020000            LEA ECX,DWORD PTR DS:[EDI+0x210]
	$-256    016A7C00      54                       PUSH ESP
	$-255    016A7C01      E8 8A57D2FF              CALL PathOfEx.013CD390
	$-250    016A7C06      6A 00                    PUSH 0x0
	$-24E    016A7C08      6A 01                    PUSH 0x1
	$-24C    016A7C0A      6A 00                    PUSH 0x0
	$-24A    016A7C0C      8D8D A8ECFFFF            LEA ECX,DWORD PTR SS:[EBP-0x1358]
	$-244    016A7C12      E8 59CAFFFF              CALL PathOfEx.016A4670
	$-23F    016A7C17      8BD0                     MOV EDX,EAX
	$-23D    016A7C19      8955 6C                  MOV DWORD PTR SS:[EBP+0x6C],EDX
	$-23A    016A7C1C      8D47 34                  LEA EAX,DWORD PTR DS:[EDI+0x34]
	$-237    016A7C1F      C745 FC 25000000         MOV DWORD PTR SS:[EBP-0x4],0x25
	$-230    016A7C26      8B48 04                  MOV ECX,DWORD PTR DS:[EAX+0x4]
	$-22D    016A7C29      3B48 08                  CMP ECX,DWORD PTR DS:[EAX+0x8]
	$-22A    016A7C2C      75 0E                    JNZ SHORT PathOfEx.016A7C3C
	$-228    016A7C2E      51                       PUSH ECX
	$-227    016A7C2F      8BC8                     MOV ECX,EAX
	$-225    016A7C31      E8 BA690000              CALL PathOfEx.016AE5F0
	$-220    016A7C36      8B55 6C                  MOV EDX,DWORD PTR SS:[EBP+0x6C]
	$-21D    016A7C39      8D47 34                  LEA EAX,DWORD PTR DS:[EDI+0x34]
	$-21A    016A7C3C      8B48 04                  MOV ECX,DWORD PTR DS:[EAX+0x4]
	$-217    016A7C3F      85C9                     TEST ECX,ECX
	$-215    016A7C41      74 09                    JE SHORT PathOfEx.016A7C4C
	$-213    016A7C43      52                       PUSH EDX
	$-212    016A7C44      E8 97CFFFFF              CALL PathOfEx.016A4BE0
	$-20D    016A7C49      8D47 34                  LEA EAX,DWORD PTR DS:[EDI+0x34]
	$-20A    016A7C4C      8340 04 48               ADD DWORD PTR DS:[EAX+0x4],0x48
	$-206    016A7C50      8D8D A8ECFFFF            LEA ECX,DWORD PTR SS:[EBP-0x1358]
	$-200    016A7C56      C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-1F9    016A7C5D      E8 4EDAE5FF              CALL PathOfEx.015056B0
	$-1F4    016A7C62      8B45 28                  MOV EAX,DWORD PTR SS:[EBP+0x28]
	$-1F1    016A7C65      8B50 20                  MOV EDX,DWORD PTR DS:[EAX+0x20]
	$-1EE    016A7C68      2B50 1C                  SUB EDX,DWORD PTR DS:[EAX+0x1C]
	$-1EB    016A7C6B      0F84 60010000            JE PathOfEx.016A7DD1
	$-1E5    016A7C71      83EC 18                  SUB ESP,0x18
	$-1E2    016A7C74      8BCC                     MOV ECX,ESP
	$-1E0    016A7C76      8965 6C                  MOV DWORD PTR SS:[EBP+0x6C],ESP
	$-1DD    016A7C79      E8 F2D5E7FF              CALL PathOfEx.01525270
	$-1D8    016A7C7E      8B4D 28                  MOV ECX,DWORD PTR SS:[EBP+0x28]
	$-1D5    016A7C81      83EC 18                  SUB ESP,0x18
	$-1D2    016A7C84      C745 FC 26000000         MOV DWORD PTR SS:[EBP-0x4],0x26
	$-1CB    016A7C8B      8B51 14                  MOV EDX,DWORD PTR DS:[ECX+0x14]
	$-1C8    016A7C8E      2B51 1C                  SUB EDX,DWORD PTR DS:[ECX+0x1C]
	$-1C5    016A7C91      8BCC                     MOV ECX,ESP
	$-1C3    016A7C93      8D52 01                  LEA EDX,DWORD PTR DS:[EDX+0x1]
	$-1C0    016A7C96      E8 D5D5E7FF              CALL PathOfEx.01525270
	$-1BB    016A7C9B      8D8D 64FEFFFF            LEA ECX,DWORD PTR SS:[EBP-0x19C]
	$-1B5    016A7CA1      C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-1AE    016A7CA8      E8 43C8DAFF              CALL PathOfEx.014544F0
	$-1A9    016A7CAD      83C4 18                  ADD ESP,0x18
	$-1A6    016A7CB0      C745 FC 27000000         MOV DWORD PTR SS:[EBP-0x4],0x27
	$-19F    016A7CB7      8BCC                     MOV ECX,ESP
	$-19D    016A7CB9      8965 6C                  MOV DWORD PTR SS:[EBP+0x6C],ESP
	$-19A    016A7CBC      BA F4000000              MOV EDX,0xF4
	$-195    016A7CC1      50                       PUSH EAX
	$-194    016A7CC2      E8 E99CFFFF              CALL PathOfEx.016A19B0
	$-18F    016A7CC7      83EC 14                  SUB ESP,0x14
	$-18C    016A7CCA      C645 FC 28               MOV BYTE PTR SS:[EBP-0x4],0x28
	$-188    016A7CCE      BA 9F000000              MOV EDX,0x9F
	$-183    016A7CD3      8BCC                     MOV ECX,ESP
	$-181    016A7CD5      E8 669CFFFF              CALL PathOfEx.016A1940
	$-17C    016A7CDA      8B55 28                  MOV EDX,DWORD PTR SS:[EBP+0x28]
	$-179    016A7CDD      6A 00                    PUSH 0x0
	$-177    016A7CDF      8B42 1C                  MOV EAX,DWORD PTR DS:[EDX+0x1C]
	$-174    016A7CE2      8B4A 14                  MOV ECX,DWORD PTR DS:[EDX+0x14]
	$-171    016A7CE5      8B52 20                  MOV EDX,DWORD PTR DS:[EDX+0x20]
	$-16E    016A7CE8      2BC8                     SUB ECX,EAX
	$-16C    016A7CEA      2BD0                     SUB EDX,EAX
	$-16A    016A7CEC      C645 FC 27               MOV BYTE PTR SS:[EBP-0x4],0x27
	$-166    016A7CF0      3BCA                     CMP ECX,EDX
	$-164    016A7CF2      0F93C0                   SETAE AL
	$-161    016A7CF5      0FB6C0                   MOVZX EAX,AL
	$-15E    016A7CF8      50                       PUSH EAX
	$-15D    016A7CF9      8D41 01                  LEA EAX,DWORD PTR DS:[ECX+0x1]
	$-15A    016A7CFC      66:0F6EC0                MOVD MM0,EAX
	$-156    016A7D00      F30FE6C0                 CVTDQ2PD XMM0,XMM0
	$-152    016A7D04      C1E8 1F                  SHR EAX,0x1F
	$-14F    016A7D07      51                       PUSH ECX
	$-14E    016A7D08      8D8D 60ECFFFF            LEA ECX,DWORD PTR SS:[EBP-0x13A0]
	$-148    016A7D0E      F20F5804C5 40420402      ADDSD XMM0,QWORD PTR DS:[EAX*8+0x2044240]
	$-13F    016A7D17      660F5AD0                 CVTPD2PS XMM2,XMM0
	$-13B    016A7D1B      66:0F6EC2                MOVD MM0,EDX
	$-137    016A7D1F      F30FE6C0                 CVTDQ2PD XMM0,XMM0
	$-133    016A7D23      C1EA 1F                  SHR EDX,0x1F
	$-130    016A7D26      F20F5804D5 40420402      ADDSD XMM0,QWORD PTR DS:[EDX*8+0x2044240]
	$-127    016A7D2F      660F5AC0                 CVTPD2PS XMM0,XMM0
	$-123    016A7D33      F3:0F5ED0                DIVSS XMM2,XMM0
	$-11F    016A7D37      E8 84CAFFFF              CALL PathOfEx.016A47C0
	$-11A    016A7D3C      8BD0                     MOV EDX,EAX
	$-118    016A7D3E      8955 6C                  MOV DWORD PTR SS:[EBP+0x6C],EDX
	$-115    016A7D41      8D4F 4C                  LEA ECX,DWORD PTR DS:[EDI+0x4C]
	$-112    016A7D44      C645 FC 29               MOV BYTE PTR SS:[EBP-0x4],0x29
	$-10E    016A7D48      8B41 04                  MOV EAX,DWORD PTR DS:[ECX+0x4]
	$-10B    016A7D4B      3B41 08                  CMP EAX,DWORD PTR DS:[ECX+0x8]
	$-108    016A7D4E      75 0C                    JNZ SHORT PathOfEx.016A7D5C
	$-106    016A7D50      51                       PUSH ECX
	$-105    016A7D51      E8 9A680000              CALL PathOfEx.016AE5F0
	$-100    016A7D56      8B55 6C                  MOV EDX,DWORD PTR SS:[EBP+0x6C]
	$-FD     016A7D59      8D4F 4C                  LEA ECX,DWORD PTR DS:[EDI+0x4C]
	$-FA     016A7D5C      8B41 04                  MOV EAX,DWORD PTR DS:[ECX+0x4]
	$-F7     016A7D5F      85C0                     TEST EAX,EAX
	$-F5     016A7D61      74 0B                    JE SHORT PathOfEx.016A7D6E
	$-F3     016A7D63      52                       PUSH EDX
	$-F2     016A7D64      8BC8                     MOV ECX,EAX
	$-F0     016A7D66      E8 75CEFFFF              CALL PathOfEx.016A4BE0
	$-EB     016A7D6B      8D4F 4C                  LEA ECX,DWORD PTR DS:[EDI+0x4C]
	$-E8     016A7D6E      8341 04 48               ADD DWORD PTR DS:[ECX+0x4],0x48
	$-E4     016A7D72      8D8D 60ECFFFF            LEA ECX,DWORD PTR SS:[EBP-0x13A0]
	$-DE     016A7D78      E8 33D9E5FF              CALL PathOfEx.015056B0
	$-D9     016A7D7D      C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-D2     016A7D84      8B8D 70FEFFFF            MOV ECX,DWORD PTR SS:[EBP-0x190]
	$-CC     016A7D8A      85C9                     TEST ECX,ECX
	$-CA     016A7D8C      74 35                    JE SHORT PathOfEx.016A7DC3
	$-C8     016A7D8E      8B85 78FEFFFF            MOV EAX,DWORD PTR SS:[EBP-0x188]
	$-C2     016A7D94      2BC1                     SUB EAX,ECX
	$-C0     016A7D96      C1F8 03                  SAR EAX,0x3
	$-BD     016A7D99      6A 08                    PUSH 0x8
	$-BB     016A7D9B      50                       PUSH EAX
	$-BA     016A7D9C      51                       PUSH ECX
	$-B9     016A7D9D      E8 CED5D0FF              CALL PathOfEx.013B5370
	$-B4     016A7DA2      83C4 0C                  ADD ESP,0xC
	$-B1     016A7DA5      C785 70FEFFFF 00000000   MOV DWORD PTR SS:[EBP-0x190],0x0
	$-A7     016A7DAF      C785 74FEFFFF 00000000   MOV DWORD PTR SS:[EBP-0x18C],0x0
	$-9D     016A7DB9      C785 78FEFFFF 00000000   MOV DWORD PTR SS:[EBP-0x188],0x0
	$-93     016A7DC3      8D8D 64FEFFFF            LEA ECX,DWORD PTR SS:[EBP-0x19C]
	$-8D     016A7DC9      E8 D21DD1FF              CALL PathOfEx.013B9BA0
	$-88     016A7DCE      8B45 28                  MOV EAX,DWORD PTR SS:[EBP+0x28]
	$-85     016A7DD1      8B50 10                  MOV EDX,DWORD PTR DS:[EAX+0x10]
	$-82     016A7DD4      8D8D BCFEFFFF            LEA ECX,DWORD PTR SS:[EBP-0x144]
	$-7C     016A7DDA      FF70 18                  PUSH DWORD PTR DS:[EAX+0x18]
	$-79     016A7DDD      83C2 08                  ADD EDX,0x8
	$-76     016A7DE0      E8 8BD30800              CALL PathOfEx.01735170                       ; call tag_查找GrantedEffectsPerLevelPair
	$-71     016A7DE5      8B85 C0FEFFFF            MOV EAX,DWORD PTR SS:[EBP-0x140]
	$-6B     016A7DEB      83C4 04                  ADD ESP,0x4
	$-68     016A7DEE      85C0                     TEST EAX,EAX
	$-66     016A7DF0      75 64                    JNZ SHORT PathOfEx.016A7E56
	$-64     016A7DF2      C745 FC 2A000000         MOV DWORD PTR SS:[EBP-0x4],0x2A
	$-5D     016A7DF9      8B85 C4FEFFFF            MOV EAX,DWORD PTR SS:[EBP-0x13C]
	$-57     016A7DFF      85C0                     TEST EAX,EAX
	$-55     016A7E01      74 1B                    JE SHORT PathOfEx.016A7E1E
	$-53     016A7E03      50                       PUSH EAX
	$-52     016A7E04      51                       PUSH ECX
	$-51     016A7E05      E8 4665FFFF              CALL PathOfEx.0169E350
	$-4C     016A7E0A      83C4 04                  ADD ESP,0x4
	$-49     016A7E0D      8BC8                     MOV ECX,EAX
	$-47     016A7E0F      E8 0C61FFFF              CALL PathOfEx.0169DF20
	$-42     016A7E14      C785 C4FEFFFF 00000000   MOV DWORD PTR SS:[EBP-0x13C],0x0
	$-38     016A7E1E      C745 FC 2B000000         MOV DWORD PTR SS:[EBP-0x4],0x2B
	$-31     016A7E25      8B85 BCFEFFFF            MOV EAX,DWORD PTR SS:[EBP-0x144]
	$-2B     016A7E2B      85C0                     TEST EAX,EAX
	$-29     016A7E2D      74 1B                    JE SHORT PathOfEx.016A7E4A
	$-27     016A7E2F      50                       PUSH EAX
	$-26     016A7E30      51                       PUSH ECX
	$-25     016A7E31      E8 1A65FFFF              CALL PathOfEx.0169E350
	$-20     016A7E36      83C4 04                  ADD ESP,0x4
	$-1D     016A7E39      8BC8                     MOV ECX,EAX
	$-1B     016A7E3B      E8 E060FFFF              CALL PathOfEx.0169DF20
	$-16     016A7E40      C785 BCFEFFFF 00000000   MOV DWORD PTR SS:[EBP-0x144],0x0
	$-C      016A7E4A      C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-5      016A7E51      E9 41010000              JMP PathOfEx.016A7F97
	$ ==>    016A7E56      8B40 64                  MOV EAX,DWORD PTR DS:[EAX+0x64]              ; g_so_GrantedEffectsPerLevel_require_lvl_
	$+3      016A7E59      8945 6C                  MOV DWORD PTR SS:[EBP+0x6C],EAX
	$+6      016A7E5C      C745 FC 2C000000         MOV DWORD PTR SS:[EBP-0x4],0x2C
	$+D      016A7E63      8B85 C4FEFFFF            MOV EAX,DWORD PTR SS:[EBP-0x13C]
	$+13     016A7E69      85C0                     TEST EAX,EAX
	$+15     016A7E6B      74 1B                    JE SHORT PathOfEx.016A7E88
	$+17     016A7E6D      50                       PUSH EAX
	$+18     016A7E6E      51                       PUSH ECX
	$+19     016A7E6F      E8 DC64FFFF              CALL PathOfEx.0169E350
	$+1E     016A7E74      83C4 04                  ADD ESP,0x4
	$+21     016A7E77      8BC8                     MOV ECX,EAX
	$+23     016A7E79      E8 A260FFFF              CALL PathOfEx.0169DF20
	$+28     016A7E7E      C785 C4FEFFFF 00000000   MOV DWORD PTR SS:[EBP-0x13C],0x0
	$+32     016A7E88      C745 FC 2D000000         MOV DWORD PTR SS:[EBP-0x4],0x2D
	$+39     016A7E8F      8B85 BCFEFFFF            MOV EAX,DWORD PTR SS:[EBP-0x144]
	$+3F     016A7E95      85C0                     TEST EAX,EAX
	$+41     016A7E97      74 1B                    JE SHORT PathOfEx.016A7EB4
	$+43     016A7E99      50                       PUSH EAX
	$+44     016A7E9A      51                       PUSH ECX
	$+45     016A7E9B      E8 B064FFFF              CALL PathOfEx.0169E350
	$+4A     016A7EA0      83C4 04                  ADD ESP,0x4
	$+4D     016A7EA3      8BC8                     MOV ECX,EAX
	$+4F     016A7EA5      E8 7660FFFF              CALL PathOfEx.0169DF20
	$+54     016A7EAA      C785 BCFEFFFF 00000000   MOV DWORD PTR SS:[EBP-0x144],0x0
	$+5E     016A7EB4      C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+65     016A7EBB      837D 6C 00               CMP DWORD PTR SS:[EBP+0x6C],0x0
	$+69     016A7EBF      0F84 D2000000            JE PathOfEx.016A7F97
	$+6F     016A7EC5      8D8F 28020000            LEA ECX,DWORD PTR DS:[EDI+0x228]
	$+75     016A7ECB      E8 4071D2FF              CALL PathOfEx.013CF010
	$+7A     016A7ED0      837E 14 08               CMP DWORD PTR DS:[ESI+0x14],0x8
	$+7E     016A7ED4      72 04                    JB SHORT PathOfEx.016A7EDA
	$+80     016A7ED6      8B0E                     MOV ECX,DWORD PTR DS:[ESI]
	$+82     016A7ED8      EB 02                    JMP SHORT PathOfEx.016A7EDC
	$+84     016A7EDA      8BCE                     MOV ECX,ESI
	$+86     016A7EDC      FFB7 64020000            PUSH DWORD PTR DS:[EDI+0x264]
	$+8C     016A7EE2      FF76 10                  PUSH DWORD PTR DS:[ESI+0x10]
	$+8F     016A7EE5      51                       PUSH ECX
	$+90     016A7EE6      8D8F 28020000            LEA ECX,DWORD PTR DS:[EDI+0x228]
	$+96     016A7EEC      E8 9F65D2FF              CALL PathOfEx.013CE490
	$+9B     016A7EF1      FF75 6C                  PUSH DWORD PTR SS:[EBP+0x6C]
	$+9E     016A7EF4      8D8F 20020000            LEA ECX,DWORD PTR DS:[EDI+0x220]
	$+A4     016A7EFA      E8 21B4D1FF              CALL PathOfEx.013C3320
	$+A9     016A7EFF      8B45 58                  MOV EAX,DWORD PTR SS:[EBP+0x58]
	$+AC     016A7F02      8D8F 10020000            LEA ECX,DWORD PTR DS:[EDI+0x210]
	$+B2     016A7F08      3B45 6C                  CMP EAX,DWORD PTR SS:[EBP+0x6C]
	$+B5     016A7F0B      0F9245 6C                SETB BYTE PTR SS:[EBP+0x6C]
	$+B9     016A7F0F      83EC 18                  SUB ESP,0x18
	$+BC     016A7F12      8BC4                     MOV EAX,ESP
	$+BE     016A7F14      8965 50                  MOV DWORD PTR SS:[EBP+0x50],ESP
	$+C1     016A7F17      50                       PUSH EAX
	$+C2     016A7F18      E8 7354D2FF              CALL PathOfEx.013CD390
	$+C7     016A7F1D      83EC 18                  SUB ESP,0x18
	$+CA     016A7F20      C745 FC 2E000000         MOV DWORD PTR SS:[EBP-0x4],0x2E
	$+D1     016A7F27      BA 9E000000              MOV EDX,0x9E
	$+D6     016A7F2C      8BCC                     MOV ECX,ESP
	$+D8     016A7F2E      E8 0D9AFFFF              CALL PathOfEx.016A1940
	$+DD     016A7F33      FF75 6C                  PUSH DWORD PTR SS:[EBP+0x6C]
	$+E0     016A7F36      8D8D 18ECFFFF            LEA ECX,DWORD PTR SS:[EBP-0x13E8]
	$+E6     016A7F3C      C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+ED     016A7F43      6A 00                    PUSH 0x0
	$+EF     016A7F45      6A 00                    PUSH 0x0
	$+F1     016A7F47      E8 34C2FFFF              CALL PathOfEx.016A4180
	$+F6     016A7F4C      8BD0                     MOV EDX,EAX
	$+F8     016A7F4E      8955 6C                  MOV DWORD PTR SS:[EBP+0x6C],EDX
	$+FB     016A7F51      8D4F 40                  LEA ECX,DWORD PTR DS:[EDI+0x40]
	$+FE     016A7F54      C745 FC 2F000000         MOV DWORD PTR SS:[EBP-0x4],0x2F
	$+105    016A7F5B      8B41 04                  MOV EAX,DWORD PTR DS:[ECX+0x4]
	$+108    016A7F5E      3B41 08                  CMP EAX,DWORD PTR DS:[ECX+0x8]
	$+10B    016A7F61      75 0C                    JNZ SHORT PathOfEx.016A7F6F
	$+10D    016A7F63      51                       PUSH ECX
	$+10E    016A7F64      E8 87660000              CALL PathOfEx.016AE5F0
	$+113    016A7F69      8B55 6C                  MOV EDX,DWORD PTR SS:[EBP+0x6C]
	$+116    016A7F6C      8D4F 40                  LEA ECX,DWORD PTR DS:[EDI+0x40]
	$+119    016A7F6F      8B41 04                  MOV EAX,DWORD PTR DS:[ECX+0x4]
	$+11C    016A7F72      85C0                     TEST EAX,EAX
	$+11E    016A7F74      74 0B                    JE SHORT PathOfEx.016A7F81
	$+120    016A7F76      52                       PUSH EDX
	$+121    016A7F77      8BC8                     MOV ECX,EAX
	$+123    016A7F79      E8 62CCFFFF              CALL PathOfEx.016A4BE0
	$+128    016A7F7E      8D4F 40                  LEA ECX,DWORD PTR DS:[EDI+0x40]
	$+12B    016A7F81      8341 04 48               ADD DWORD PTR DS:[ECX+0x4],0x48
	$+12F    016A7F85      8D8D 18ECFFFF            LEA ECX,DWORD PTR SS:[EBP-0x13E8]
	$+135    016A7F8B      C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+13C    016A7F92      E8 19D7E5FF              CALL PathOfEx.015056B0
	$+141    016A7F97      33DB                     XOR EBX,EBX
	$+143    016A7F99      8D87 10020000            LEA EAX,DWORD PTR DS:[EDI+0x210]
	$+149    016A7F9F      33C9                     XOR ECX,ECX
	$+14B    016A7FA1      8D97 20020000            LEA EDX,DWORD PTR DS:[EDI+0x220]
	$+151    016A7FA7      85C0                     TEST EAX,EAX
	$+153    016A7FA9      894D C8                  MOV DWORD PTR SS:[EBP-0x38],ECX
	$+156    016A7FAC      B8 78ACEE01              MOV EAX,PathOfEx.01EEAC78
	$+15B    016A7FB1      0F44D3                   CMOVE EDX,EBX
	$+15E    016A7FB4      8D9F 28020000            LEA EBX,DWORD PTR DS:[EDI+0x228]
	$+164    016A7FBA      8955 50                  MOV DWORD PTR SS:[EBP+0x50],EDX
	$+167    016A7FBD      8B55 54                  MOV EDX,DWORD PTR SS:[EBP+0x54]
	$+16A    016A7FC0      2BC2                     SUB EAX,EDX
	$+16C    016A7FC2      8955 6C                  MOV DWORD PTR SS:[EBP+0x6C],EDX
	$+16F    016A7FC5      8945 C4                  MOV DWORD PTR SS:[EBP-0x3C],EAX
	$+172    016A7FC8      6A 00                    PUSH 0x0
	$+174    016A7FCA      51                       PUSH ECX
	$+175    016A7FCB      8B4D 28                  MOV ECX,DWORD PTR SS:[EBP+0x28]
	$+178    016A7FCE      E8 ADE80200              CALL PathOfEx.016D6880
	$+17D    016A7FD3      8945 34                  MOV DWORD PTR SS:[EBP+0x34],EAX
	$+180    016A7FD6      85C0                     TEST EAX,EAX
	$+182    016A7FD8      0F84 2E020000            JE PathOfEx.016A820C
	$+188    016A7FDE      8BCB                     MOV ECX,EBX
	$+18A    016A7FE0      E8 2B70D2FF              CALL PathOfEx.013CF010
	$+18F    016A7FE5      837E 14 08               CMP DWORD PTR DS:[ESI+0x14],0x8
	$+193    016A7FE9      72 04                    JB SHORT PathOfEx.016A7FEF
	$+195    016A7FEB      8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$+197    016A7FED      EB 02                    JMP SHORT PathOfEx.016A7FF1
	$+199    016A7FEF      8BC6                     MOV EAX,ESI
	$+19B    016A7FF1      FF73 3C                  PUSH DWORD PTR DS:[EBX+0x3C]
	$+19E    016A7FF4      8BCB                     MOV ECX,EBX
	$+1A0    016A7FF6      FF76 10                  PUSH DWORD PTR DS:[ESI+0x10]
	$+1A3    016A7FF9      50                       PUSH EAX
	$+1A4    016A7FFA      E8 9164D2FF              CALL PathOfEx.013CE490
	$+1A9    016A7FFF      8B45 6C                  MOV EAX,DWORD PTR SS:[EBP+0x6C]
	$+1AC    016A8002      8B4D C4                  MOV ECX,DWORD PTR SS:[EBP-0x3C]
	$+1AF    016A8005      8B1401                   MOV EDX,DWORD PTR DS:[ECX+EAX]
	$+1B2    016A8008      8D8D A4FBFFFF            LEA ECX,DWORD PTR SS:[EBP-0x45C]
	$+1B8    016A800E      E8 2D99FFFF              CALL PathOfEx.016A1940
	$+1BD    016A8013      C745 FC 30000000         MOV DWORD PTR SS:[EBP-0x4],0x30
	$+1C4    016A801A      8BD0                     MOV EDX,EAX
	$+1C6    016A801C      8B4D 50                  MOV ECX,DWORD PTR SS:[EBP+0x50]
	$+1C9    016A801F      E8 1C54D2FF              CALL PathOfEx.013CD440
	$+1CE    016A8024      C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+1D5    016A802B      8B85 B8FBFFFF            MOV EAX,DWORD PTR SS:[EBP-0x448]
	$+1DB    016A8031      83F8 08                  CMP EAX,0x8
	$+1DE    016A8034      72 12                    JB SHORT PathOfEx.016A8048
	$+1E0    016A8036      40                       INC EAX
	$+1E1    016A8037      6A 02                    PUSH 0x2
	$+1E3    016A8039      50                       PUSH EAX
	$+1E4    016A803A      FFB5 A4FBFFFF            PUSH DWORD PTR SS:[EBP-0x45C]
	$+1EA    016A8040      E8 2BD3D0FF              CALL PathOfEx.013B5370
	$+1EF    016A8045      83C4 0C                  ADD ESP,0xC
	$+1F2    016A8048      33C0                     XOR EAX,EAX
	$+1F4    016A804A      C785 B8FBFFFF 07000000   MOV DWORD PTR SS:[EBP-0x448],0x7
	$+1FE    016A8054      66:8985 A4FBFFFF         MOV WORD PTR SS:[EBP-0x45C],AX
	$+205    016A805B      8D8F 10020000            LEA ECX,DWORD PTR DS:[EDI+0x210]
	$+20B    016A8061      8D85 04FEFFFF            LEA EAX,DWORD PTR SS:[EBP-0x1FC]
	$+211    016A8067      C785 B4FBFFFF 00000000   MOV DWORD PTR SS:[EBP-0x44C],0x0
	$+21B    016A8071      50                       PUSH EAX
	$+21C    016A8072      E8 1953D2FF              CALL PathOfEx.013CD390
	$+221    016A8077      8BCB                     MOV ECX,EBX
	$+223    016A8079      C745 FC 31000000         MOV DWORD PTR SS:[EBP-0x4],0x31
	$+22A    016A8080      E8 8B6FD2FF              CALL PathOfEx.013CF010
	$+22F    016A8085      837E 14 08               CMP DWORD PTR DS:[ESI+0x14],0x8
	$+233    016A8089      72 04                    JB SHORT PathOfEx.016A808F
	$+235    016A808B      8B06                     MOV EAX,DWORD PTR DS:[ESI]
	$+237    016A808D      EB 02                    JMP SHORT PathOfEx.016A8091
	$+239    016A808F      8BC6                     MOV EAX,ESI
	$+23B    016A8091      FF73 3C                  PUSH DWORD PTR DS:[EBX+0x3C]
	$+23E    016A8094      8BCB                     MOV ECX,EBX
	$+240    016A8096      FF76 10                  PUSH DWORD PTR DS:[ESI+0x10]
	$+243    016A8099      50                       PUSH EAX
	$+244    016A809A      E8 F163D2FF              CALL PathOfEx.013CE490
	$+249    016A809F      FF75 34                  PUSH DWORD PTR SS:[EBP+0x34]
	$+24C    016A80A2      8D8F 20020000            LEA ECX,DWORD PTR DS:[EDI+0x220]
	$+252    016A80A8      E8 73B2D1FF              CALL PathOfEx.013C3320
	$+257    016A80AD      8D85 4CFEFFFF            LEA EAX,DWORD PTR SS:[EBP-0x1B4]
	$+25D    016A80B3      50                       PUSH EAX
	$+25E    016A80B4      8D8F 10020000            LEA ECX,DWORD PTR DS:[EDI+0x210]
	$+264    016A80BA      E8 D152D2FF              CALL PathOfEx.013CD390
	$+269    016A80BF      C645 FC 32               MOV BYTE PTR SS:[EBP-0x4],0x32
	$+26D    016A80C3      8B4D 6C                  MOV ECX,DWORD PTR SS:[EBP+0x6C]
	$+270    016A80C6      8B45 34                  MOV EAX,DWORD PTR SS:[EBP+0x34]
	$+273    016A80C9      3901                     CMP DWORD PTR DS:[ECX],EAX
	$+275    016A80CB      0F9245 E8                SETB BYTE PTR SS:[EBP-0x18]
	$+279    016A80CF      83EC 18                  SUB ESP,0x18
	$+27C    016A80D2      8BCC                     MOV ECX,ESP
	$+27E    016A80D4      C741 14 00000000         MOV DWORD PTR DS:[ECX+0x14],0x0
	$+285    016A80DB      C741 14 07000000         MOV DWORD PTR DS:[ECX+0x14],0x7
	$+28C    016A80E2      C741 10 00000000         MOV DWORD PTR DS:[ECX+0x10],0x0
	$+293    016A80E9      8379 14 08               CMP DWORD PTR DS:[ECX+0x14],0x8
	$+297    016A80ED      72 04                    JB SHORT PathOfEx.016A80F3
	$+299    016A80EF      8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+29B    016A80F1      EB 02                    JMP SHORT PathOfEx.016A80F5
	$+29D    016A80F3      8BC1                     MOV EAX,ECX
	$+29F    016A80F5      33D2                     XOR EDX,EDX
	$+2A1    016A80F7      66:8910                  MOV WORD PTR DS:[EAX],DX
	$+2A4    016A80FA      8D85 4CFEFFFF            LEA EAX,DWORD PTR SS:[EBP-0x1B4]
	$+2AA    016A8100      50                       PUSH EAX
	$+2AB    016A8101      E8 8A05D1FF              CALL PathOfEx.013B8690
	$+2B0    016A8106      83EC 18                  SUB ESP,0x18
	$+2B3    016A8109      8BCC                     MOV ECX,ESP
	$+2B5    016A810B      C741 14 00000000         MOV DWORD PTR DS:[ECX+0x14],0x0
	$+2BC    016A8112      C741 14 07000000         MOV DWORD PTR DS:[ECX+0x14],0x7
	$+2C3    016A8119      C741 10 00000000         MOV DWORD PTR DS:[ECX+0x10],0x0
	$+2CA    016A8120      8379 14 08               CMP DWORD PTR DS:[ECX+0x14],0x8
	$+2CE    016A8124      72 04                    JB SHORT PathOfEx.016A812A
	$+2D0    016A8126      8B01                     MOV EAX,DWORD PTR DS:[ECX]
	$+2D2    016A8128      EB 02                    JMP SHORT PathOfEx.016A812C
	$+2D4    016A812A      8BC1                     MOV EAX,ECX
	$+2D6    016A812C      33D2                     XOR EDX,EDX
	$+2D8    016A812E      66:8910                  MOV WORD PTR DS:[EAX],DX
	$+2DB    016A8131      8D85 04FEFFFF            LEA EAX,DWORD PTR SS:[EBP-0x1FC]
	$+2E1    016A8137      50                       PUSH EAX
	$+2E2    016A8138      E8 5305D1FF              CALL PathOfEx.013B8690
	$+2E7    016A813D      6A 01                    PUSH 0x1
	$+2E9    016A813F      FF75 E8                  PUSH DWORD PTR SS:[EBP-0x18]
	$+2EC    016A8142      6A 00                    PUSH 0x0
	$+2EE    016A8144      51                       PUSH ECX
	$+2EF    016A8145      8D8D D0EBFFFF            LEA ECX,DWORD PTR SS:[EBP-0x1430]
	$+2F5    016A814B      E8 C0C1FFFF              CALL PathOfEx.016A4310
	$+2FA    016A8150      8BD0                     MOV EDX,EAX
	$+2FC    016A8152      8955 34                  MOV DWORD PTR SS:[EBP+0x34],EDX
	$+2FF    016A8155      8D4F 40                  LEA ECX,DWORD PTR DS:[EDI+0x40]
	$+302    016A8158      C645 FC 33               MOV BYTE PTR SS:[EBP-0x4],0x33
	$+306    016A815C      8B41 04                  MOV EAX,DWORD PTR DS:[ECX+0x4]
	$+309    016A815F      3B41 08                  CMP EAX,DWORD PTR DS:[ECX+0x8]
	$+30C    016A8162      75 0C                    JNZ SHORT PathOfEx.016A8170
	$+30E    016A8164      51                       PUSH ECX
	$+30F    016A8165      E8 86640000              CALL PathOfEx.016AE5F0
	$+314    016A816A      8B55 34                  MOV EDX,DWORD PTR SS:[EBP+0x34]
	$+317    016A816D      8D4F 40                  LEA ECX,DWORD PTR DS:[EDI+0x40]
	$+31A    016A8170      8B41 04                  MOV EAX,DWORD PTR DS:[ECX+0x4]
	$+31D    016A8173      85C0                     TEST EAX,EAX
	$+31F    016A8175      74 0B                    JE SHORT PathOfEx.016A8182
	$+321    016A8177      52                       PUSH EDX
	$+322    016A8178      8BC8                     MOV ECX,EAX
	$+324    016A817A      E8 61CAFFFF              CALL PathOfEx.016A4BE0
	$+329    016A817F      8D4F 40                  LEA ECX,DWORD PTR DS:[EDI+0x40]
	$+32C    016A8182      8341 04 48               ADD DWORD PTR DS:[ECX+0x4],0x48
	$+330    016A8186      8D8D D0EBFFFF            LEA ECX,DWORD PTR SS:[EBP-0x1430]
	$+336    016A818C      E8 1FD5E5FF              CALL PathOfEx.015056B0
	$+33B    016A8191      8B85 60FEFFFF            MOV EAX,DWORD PTR SS:[EBP-0x1A0]
	$+341    016A8197      83F8 08                  CMP EAX,0x8
	$+344    016A819A      72 12                    JB SHORT PathOfEx.016A81AE
	$+346    016A819C      40                       INC EAX
	$+347    016A819D      6A 02                    PUSH 0x2
	$+349    016A819F      50                       PUSH EAX
	$+34A    016A81A0      FFB5 4CFEFFFF            PUSH DWORD PTR SS:[EBP-0x1B4]
	$+350    016A81A6      E8 C5D1D0FF              CALL PathOfEx.013B5370
	$+355    016A81AB      83C4 0C                  ADD ESP,0xC
	$+358    016A81AE      33C0                     XOR EAX,EAX
	$+35A    016A81B0      C745 FC FFFFFFFF         MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+361    016A81B7      66:8985 4CFEFFFF         MOV WORD PTR SS:[EBP-0x1B4],AX
	$+368    016A81BE      8B85 18FEFFFF            MOV EAX,DWORD PTR SS:[EBP-0x1E8]
	$+36E    016A81C4      C785 60FEFFFF 07000000   MOV DWORD PTR SS:[EBP-0x1A0],0x7
	$+378    016A81CE      C785 5CFEFFFF 00000000   MOV DWORD PTR SS:[EBP-0x1A4],0x0
	$+382    016A81D8      83F8 08                  CMP EAX,0x8
	$+385    016A81DB      72 12                    JB SHORT PathOfEx.016A81EF
	$+387    016A81DD      40                       INC EAX
	$+388    016A81DE      6A 02                    PUSH 0x2
	$+38A    016A81E0      50                       PUSH EAX
	$+38B    016A81E1      FFB5 04FEFFFF            PUSH DWORD PTR SS:[EBP-0x1FC]
	$+391    016A81E7      E8 84D1D0FF              CALL PathOfEx.013B5370
	$+396    016A81EC      83C4 0C                  ADD ESP,0xC
	$+399    016A81EF      33C0                     XOR EAX,EAX
	$+39B    016A81F1      C785 18FEFFFF 07000000   MOV DWORD PTR SS:[EBP-0x1E8],0x7
	$+3A5    016A81FB      C785 14FEFFFF 00000000   MOV DWORD PTR SS:[EBP-0x1EC],0x0
	$+3AF    016A8205      66:8985 04FEFFFF         MOV WORD PTR SS:[EBP-0x1FC],AX
	$+3B6    016A820C      8B4D C8                  MOV ECX,DWORD PTR SS:[EBP-0x38]
	$+3B9    016A820F      8345 6C 04               ADD DWORD PTR SS:[EBP+0x6C],0x4
	$+3BD    016A8213      41                       INC ECX
	$+3BE    016A8214      894D C8                  MOV DWORD PTR SS:[EBP-0x38],ECX
	$+3C1    016A8217      83F9 03                  CMP ECX,0x3
	$+3C4    016A821A    ^ 0F82 A8FDFFFF            JB PathOfEx.016A7FC8
	$+3CA    016A8220      8B45 28                  MOV EAX,DWORD PTR SS:[EBP+0x28]
	*/
#ifdef VER_GJ3_
	op_by_tmp = scan_exe_.ScanImme<pt_byte>("8b????89????c7????????????8b??????????85??74??5051e8????????83", 2);
#else
	op_by_tmp = scan_exe_.ScanImme<pt_byte>("8b????89????c7????????????8b??????????85??74??5051e8????????83", 2);
#endif
	if (!op_by_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *op_by_tmp;
	assert(dw_tmp > 0x30 && dw_tmp < 0xa0);
	ofs << "pt_cdword	g_o_GrantedEffectsPerLevel_require_lvl_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	/*
	$-97     0158ABCD     CC                   INT3
	$-96     0158ABCE     CC                   INT3                                           ; tag_游戏内得到发包对象holder_13c
	$-95     0158ABCF     CC                   INT3                                           ; 走路发送的第一个包
	$-94     0158ABD0     55                   PUSH EBP
	$-93     0158ABD1     8BEC                 MOV EBP,ESP
	$-91     0158ABD3     6A FF                PUSH -0x1
	$-8F     0158ABD5     68 3089D701          PUSH PathOfEx.01D78930
	$-8A     0158ABDA     64:A1 00000000       MOV EAX,DWORD PTR FS:[0]
	$-84     0158ABE0     50                   PUSH EAX
	$-83     0158ABE1     64:8925 00000000     MOV DWORD PTR FS:[0],ESP
	$-7C     0158ABE8     83EC 30              SUB ESP,0x30
	$-79     0158ABEB     56                   PUSH ESI
	$-78     0158ABEC     57                   PUSH EDI
	$-77     0158ABED     8BF1                 MOV ESI,ECX
	$-75     0158ABEF     E8 BC47FFFF          CALL PathOfEx.0157F3B0
	$-70     0158ABF4     8B8E 6C150000        MOV ECX,DWORD PTR DS:[ESI+0x156C]
	$-6A     0158ABFA     8DBE 6C150000        LEA EDI,DWORD PTR DS:[ESI+0x156C]
	$-64     0158AC00     8845 F2              MOV BYTE PTR SS:[EBP-0xE],AL
	$-61     0158AC03     C745 EC 00000000     MOV DWORD PTR SS:[EBP-0x14],0x0
	$-5A     0158AC0A     3B4F 04              CMP ECX,DWORD PTR DS:[EDI+0x4]
	$-57     0158AC0D     0F84 69010000        JE PathOfEx.0158AD7C
	$-51     0158AC13     8B86 70150000        MOV EAX,DWORD PTR DS:[ESI+0x1570]
	$-4B     0158AC19     8D4D D4              LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$-48     0158AC1C     53                   PUSH EBX
	$-47     0158AC1D     51                   PUSH ECX
	$-46     0158AC1E     8D48 F8              LEA ECX,DWORD PTR DS:[EAX-0x8]
	$-43     0158AC21     E8 5A2AE8FF          CALL PathOfEx.0140D680
	$-3E     0158AC26     C745 FC 00000000     MOV DWORD PTR SS:[EBP-0x4],0x0
	$-37     0158AC2D     8D45 EC              LEA EAX,DWORD PTR SS:[EBP-0x14]
	$-34     0158AC30     8B96 70150000        MOV EDX,DWORD PTR DS:[ESI+0x1570]
	$-2E     0158AC36     8B0F                 MOV ECX,DWORD PTR DS:[EDI]
	$-2C     0158AC38     8945 E0              MOV DWORD PTR SS:[EBP-0x20],EAX
	$-29     0158AC3B     8D45 DC              LEA EAX,DWORD PTR SS:[EBP-0x24]
	$-26     0158AC3E     50                   PUSH EAX
	$-25     0158AC3F     8975 DC              MOV DWORD PTR SS:[EBP-0x24],ESI
	$-22     0158AC42     E8 B9560000          CALL PathOfEx.01590300
	$-1D     0158AC47     83C4 04              ADD ESP,0x4
	$-1A     0158AC4A     8BCF                 MOV ECX,EDI
	$-18     0158AC4C     FF77 04              PUSH DWORD PTR DS:[EDI+0x4]
	$-15     0158AC4F     50                   PUSH EAX
	$-14     0158AC50     8D45 E4              LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$-11     0158AC53     50                   PUSH EAX
	$-10     0158AC54     E8 271C0000          CALL PathOfEx.0158C880
	$-B      0158AC59     8B07                 MOV EAX,DWORD PTR DS:[EDI]
	$-9      0158AC5B     3B47 04              CMP EAX,DWORD PTR DS:[EDI+0x4]
	$-6      0158AC5E     0F85 80000000        JNZ PathOfEx.0158ACE4
	$ ==>    0158AC64     8B8E 44010000        MOV ECX,DWORD PTR DS:[ESI+0x144]               ; 得到游戏内发包对象holder的地址
	$+6      0158AC6A     E8 61482200          CALL PathOfEx.017AF4D0                         ; 走路发的第一个包，会断住。此函数内进行发包的
	$+B      0158AC6F     8D45 E8              LEA EAX,DWORD PTR SS:[EBP-0x18]
	$+E      0158AC72     C745 E8 00000000     MOV DWORD PTR SS:[EBP-0x18],0x0
	$+15     0158AC79     50                   PUSH EAX
	$+16     0158AC7A     8D45 DC              LEA EAX,DWORD PTR SS:[EBP-0x24]
	$+19     0158AC7D     C745 E4 00000000     MOV DWORD PTR SS:[EBP-0x1C],0x0
	$+20     0158AC84     50                   PUSH EAX
	$+21     0158AC85     8D55 E4              LEA EDX,DWORD PTR SS:[EBP-0x1C]
	$+24     0158AC88     C745 DC 00000000     MOV DWORD PTR SS:[EBP-0x24],0x0
	$+2B     0158AC8F     8D4D C4              LEA ECX,DWORD PTR SS:[EBP-0x3C]
	$+2E     0158AC92     C745 E0 00000000     MOV DWORD PTR SS:[EBP-0x20],0x0
	$+35     0158AC99     E8 12360000          CALL PathOfEx.0158E2B0
	$+3A     0158AC9E     83C4 08              ADD ESP,0x8
	$+3D     0158ACA1     8D8E 78150000        LEA ECX,DWORD PTR DS:[ESI+0x1578]
	$+43     0158ACA7     50                   PUSH EAX
	$+44     0158ACA8     E8 233C0000          CALL PathOfEx.0158E8D0
	$+49     0158ACAD     C645 FC 01           MOV BYTE PTR SS:[EBP-0x4],0x1
	$+4D     0158ACB1     83CF FF              OR EDI,0xFFFFFFFF
	$+50     0158ACB4     8B4D CC              MOV ECX,DWORD PTR SS:[EBP-0x34]
	$+53     0158ACB7     85C9                 TEST ECX,ECX
	$+55     0158ACB9     74 0E                JE SHORT PathOfEx.0158ACC9
	$+57     0158ACBB     8BC7                 MOV EAX,EDI
	$+59     0158ACBD     F0:0FC141 08         LOCK XADD DWORD PTR DS:[ECX+0x8],EAX
	$+5E     0158ACC2     75 05                JNZ SHORT PathOfEx.0158ACC9
	$+60     0158ACC4     8B01                 MOV EAX,DWORD PTR DS:[ECX]
	$+62     0158ACC6     FF50 04              CALL DWORD PTR DS:[EAX+0x4]
	$+65     0158ACC9     C645 FC 02           MOV BYTE PTR SS:[EBP-0x4],0x2
	$+69     0158ACCD     8B4D E0              MOV ECX,DWORD PTR SS:[EBP-0x20]
	$+6C     0158ACD0     85C9                 TEST ECX,ECX
	$+6E     0158ACD2     74 6E                JE SHORT PathOfEx.0158AD42
	$+70     0158ACD4     8BC7                 MOV EAX,EDI
	$+72     0158ACD6     F0:0FC141 08         LOCK XADD DWORD PTR DS:[ECX+0x8],EAX
	$+77     0158ACDB     75 65                JNZ SHORT PathOfEx.0158AD42
	$+79     0158ACDD     8B01                 MOV EAX,DWORD PTR DS:[ECX]
	$+7B     0158ACDF     FF50 04              CALL DWORD PTR DS:[EAX+0x4]
	$+7E     0158ACE2     EB 5E                JMP SHORT PathOfEx.0158AD42
	$+80     0158ACE4     8B86 70150000        MOV EAX,DWORD PTR DS:[ESI+0x1570]
	$+86     0158ACEA     8D4D DC              LEA ECX,DWORD PTR SS:[EBP-0x24]
	$+89     0158ACED     51                   PUSH ECX
	$+8A     0158ACEE     8D48 F8              LEA ECX,DWORD PTR DS:[EAX-0x8]
	$+8D     0158ACF1     E8 8A29E8FF          CALL PathOfEx.0140D680
	$+92     0158ACF6     8B00                 MOV EAX,DWORD PTR DS:[EAX]
	$+94     0158ACF8     3B45 D4              CMP EAX,DWORD PTR SS:[EBP-0x2C]
	$+97     0158ACFB     0F94C3               SETE BL
	$+9A     0158ACFE     80F3 01              XOR BL,0x1
	$+9D     0158AD01     885D F3              MOV BYTE PTR SS:[EBP-0xD],BL
	$+A0     0158AD04     C645 FC 03           MOV BYTE PTR SS:[EBP-0x4],0x3
	$+A4     0158AD08     83CF FF              OR EDI,0xFFFFFFFF
	$+A7     0158AD0B     8B5D E0              MOV EBX,DWORD PTR SS:[EBP-0x20]
	$+AA     0158AD0E     85DB                 TEST EBX,EBX
	$+AC     0158AD10     74 1F                JE SHORT PathOfEx.0158AD31
	$+AE     0158AD12     8BC7                 MOV EAX,EDI
	$+B0     0158AD14     F0:0FC143 04         LOCK XADD DWORD PTR DS:[EBX+0x4],EAX
	$+B5     0158AD19     75 16                JNZ SHORT PathOfEx.0158AD31
	$+B7     0158AD1B     8B03                 MOV EAX,DWORD PTR DS:[EBX]
	$+B9     0158AD1D     8BCB                 MOV ECX,EBX
	$+BB     0158AD1F     FF10                 CALL DWORD PTR DS:[EAX]
	$+BD     0158AD21     8BC7                 MOV EAX,EDI
	$+BF     0158AD23     F0:0FC143 08         LOCK XADD DWORD PTR DS:[EBX+0x8],EAX
	$+C4     0158AD28     75 07                JNZ SHORT PathOfEx.0158AD31
	$+C6     0158AD2A     8B03                 MOV EAX,DWORD PTR DS:[EBX]
	$+C8     0158AD2C     8BCB                 MOV ECX,EBX
	$+CA     0158AD2E     FF50 04              CALL DWORD PTR DS:[EAX+0x4]
	$+CD     0158AD31     807D F3 00           CMP BYTE PTR SS:[EBP-0xD],0x0
	$+D1     0158AD35     C645 FC 00           MOV BYTE PTR SS:[EBP-0x4],0x0
	$+D5     0158AD39     74 07                JE SHORT PathOfEx.0158AD42
	$+D7     0158AD3B     8BCE                 MOV ECX,ESI
	$+D9     0158AD3D     E8 9EF7FFFF          CALL PathOfEx.0158A4E0
	$+DE     0158AD42     C745 FC 04000000     MOV DWORD PTR SS:[EBP-0x4],0x4
	$+E5     0158AD49     8B5D D8              MOV EBX,DWORD PTR SS:[EBP-0x28]
	$+E8     0158AD4C     85DB                 TEST EBX,EBX
	$+EA     0158AD4E     74 1E                JE SHORT PathOfEx.0158AD6E
	$+EC     0158AD50     8BC7                 MOV EAX,EDI
	$+EE     0158AD52     F0:0FC143 04         LOCK XADD DWORD PTR DS:[EBX+0x4],EAX
	$+F3     0158AD57     75 15                JNZ SHORT PathOfEx.0158AD6E
	$+F5     0158AD59     8B03                 MOV EAX,DWORD PTR DS:[EBX]
	$+F7     0158AD5B     8BCB                 MOV ECX,EBX
	$+F9     0158AD5D     FF10                 CALL DWORD PTR DS:[EAX]
	$+FB     0158AD5F     F0:0FC17B 08         LOCK XADD DWORD PTR DS:[EBX+0x8],EDI
	$+100    0158AD64     4F                   DEC EDI
	$+101    0158AD65     75 07                JNZ SHORT PathOfEx.0158AD6E
	$+103    0158AD67     8B03                 MOV EAX,DWORD PTR DS:[EBX]
	$+105    0158AD69     8BCB                 MOV ECX,EBX
	$+107    0158AD6B     FF50 04              CALL DWORD PTR DS:[EAX+0x4]
	$+10A    0158AD6E     C745 FC FFFFFFFF     MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+111    0158AD75     837D EC 00           CMP DWORD PTR SS:[EBP-0x14],0x0
	$+115    0158AD79     5B                   POP EBX
	$+116    0158AD7A     75 36                JNZ SHORT PathOfEx.0158ADB2
	$+118    0158AD7C     8B8E DC180000        MOV ECX,DWORD PTR DS:[ESI+0x18DC]
	$+11E    0158AD82     8B86 0C0C0000        MOV EAX,DWORD PTR DS:[ESI+0xC0C]
	$+124    0158AD88     3B81 8C0A0000        CMP EAX,DWORD PTR DS:[ECX+0xA8C]
	$+12A    0158AD8E     75 22                JNZ SHORT PathOfEx.0158ADB2
	$+12C    0158AD90     FF15 C0D4DC01        CALL DWORD PTR DS:[<&USER32.ReleaseCapture>]   ; user32.ReleaseCapture
	$+132    0158AD96     8B8E 0C0C0000        MOV ECX,DWORD PTR DS:[ESI+0xC0C]
	$+138    0158AD9C     85C9                 TEST ECX,ECX
	$+13A    0158AD9E     74 08                JE SHORT PathOfEx.0158ADA8
	$+13C    0158ADA0     8B01                 MOV EAX,DWORD PTR DS:[ECX]
	$+13E    0158ADA2     FF90 04010000        CALL DWORD PTR DS:[EAX+0x104]
	$+144    0158ADA8     C786 0C0C0000 000000>MOV DWORD PTR DS:[ESI+0xC0C],0x0
	$+14E    0158ADB2     8BCE                 MOV ECX,ESI
	$+150    0158ADB4     E8 F745FFFF          CALL PathOfEx.0157F3B0
	$+155    0158ADB9     807D F2 00           CMP BYTE PTR SS:[EBP-0xE],0x0
	$+159    0158ADBD     74 39                JE SHORT PathOfEx.0158ADF8
	$+15B    0158ADBF     84C0                 TEST AL,AL
	*/
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("8b??????????e8????????8d????c7????????????508d????c7", 2);
	if (!op_dw_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *op_dw_tmp;
	assert(dw_tmp > 0x110 && dw_tmp < 0x200);
	ofs << "pt_cdword	g_o_game_logic_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	//gj3
	/*
	$-1F6    014C3D9F     CC                         INT3
	$-1F5    014C3DA0     83EC 0C                    SUB ESP,0xC
	$-1F2    014C3DA3     56                         PUSH ESI
	$-1F1    014C3DA4     6A 01                      PUSH 0x1
	$-1EF    014C3DA6     8BF1                       MOV ESI,ECX
	$-1ED    014C3DA8     E8 93040000                CALL PathOfEx.014C4240
	$-1E8    014C3DAD     80BE 69040000 00           CMP BYTE PTR DS:[ESI+0x469],0x0
	$-1E1    014C3DB4     D95C24 08                  FSTP DWORD PTR SS:[ESP+0x8]
	$-1DD    014C3DB8     F3:0F104424 08             MOVSS XMM0,DWORD PTR SS:[ESP+0x8]
	$-1D7    014C3DBE     6A 00                      PUSH 0x0
	$-1D5    014C3DC0     74 34                      JE SHORT PathOfEx.014C3DF6
	$-1D3    014C3DC2     83EC 28                    SUB ESP,0x28
	$-1D0    014C3DC5     8BC4                       MOV EAX,ESP
	$-1CE    014C3DC7     C700 4035D501              MOV DWORD PTR DS:[EAX],PathOfEx.01D53540
	$-1C8    014C3DCD     8940 24                    MOV DWORD PTR DS:[EAX+0x24],EAX
	$-1C5    014C3DD0     8B46 04                    MOV EAX,DWORD PTR DS:[ESI+0x4]
	$-1C2    014C3DD3     8B48 2C                    MOV ECX,DWORD PTR DS:[EAX+0x2C]
	$-1BF    014C3DD6     8B4424 40                  MOV EAX,DWORD PTR SS:[ESP+0x40]
	$-1BB    014C3DDA     51                         PUSH ECX
	$-1BA    014C3DDB     F3:0F110424                MOVSS DWORD PTR SS:[ESP],XMM0
	$-1B5    014C3DE0     8D04C5 0C000000            LEA EAX,DWORD PTR DS:[EAX*8+0xC]
	$-1AE    014C3DE7     03C6                       ADD EAX,ESI
	$-1AC    014C3DE9     50                         PUSH EAX
	$-1AB    014C3DEA     E8 C13EFDFF                CALL PathOfEx.01497CB0
	$-1A6    014C3DEF     5E                         POP ESI
	$-1A5    014C3DF0     83C4 0C                    ADD ESP,0xC
	$-1A2    014C3DF3     C2 0400                    RETN 0x4
	$-19F    014C3DF6     8B46 04                    MOV EAX,DWORD PTR DS:[ESI+0x4]
	$-19C    014C3DF9     8B48 2C                    MOV ECX,DWORD PTR DS:[EAX+0x2C]
	$-199    014C3DFC     8B41 0C                    MOV EAX,DWORD PTR DS:[ECX+0xC]
	$-196    014C3DFF     FF70 10                    PUSH DWORD PTR DS:[EAX+0x10]
	$-193    014C3E02     83EC 28                    SUB ESP,0x28
	$-190    014C3E05     8BC4                       MOV EAX,ESP
	$-18E    014C3E07     51                         PUSH ECX
	$-18D    014C3E08     C700 70FED401              MOV DWORD PTR DS:[EAX],PathOfEx.01D4FE70
	$-187    014C3E0E     8940 24                    MOV DWORD PTR DS:[EAX+0x24],EAX
	$-184    014C3E11     8B4424 48                  MOV EAX,DWORD PTR SS:[ESP+0x48]
	$-180    014C3E15     F3:0F110424                MOVSS DWORD PTR SS:[ESP],XMM0
	$-17B    014C3E1A     68 FFFFFF7F                PUSH 0x7FFFFFFF
	$-176    014C3E1F     8D04C5 0C000000            LEA EAX,DWORD PTR DS:[EAX*8+0xC]
	$-16F    014C3E26     03C6                       ADD EAX,ESI
	$-16D    014C3E28     50                         PUSH EAX
	$-16C    014C3E29     E8 C23FFDFF                CALL PathOfEx.01497DF0
	$-167    014C3E2E     5E                         POP ESI
	$-166    014C3E2F     83C4 0C                    ADD ESP,0xC
	$-163    014C3E32     C2 0400                    RETN 0x4
	$-160    014C3E35     CC                         INT3
	$-15F    014C3E36     CC                         INT3
	$-15E    014C3E37     CC                         INT3
	$-15D    014C3E38     CC                         INT3
	$-15C    014C3E39     CC                         INT3
	$-15B    014C3E3A     CC                         INT3
	$-15A    014C3E3B     CC                         INT3
	$-159    014C3E3C     CC                         INT3
	$-158    014C3E3D     CC                         INT3
	$-157    014C3E3E     CC                         INT3
	$-156    014C3E3F     CC                         INT3
	$-155    014C3E40     E9 EB010000                JMP PathOfEx.014C4030
	$-150    014C3E45     CC                         INT3
	$-14F    014C3E46     CC                         INT3
	$-14E    014C3E47     CC                         INT3
	$-14D    014C3E48     CC                         INT3
	$-14C    014C3E49     CC                         INT3
	$-14B    014C3E4A     CC                         INT3
	$-14A    014C3E4B     CC                         INT3
	$-149    014C3E4C     CC                         INT3
	$-148    014C3E4D     CC                         INT3
	$-147    014C3E4E     CC                         INT3
	$-146    014C3E4F     CC                         INT3
	$-145    014C3E50     83EC 08                    SUB ESP,0x8
	$-142    014C3E53     53                         PUSH EBX
	$-141    014C3E54     56                         PUSH ESI
	$-140    014C3E55     8BF1                       MOV ESI,ECX
	$-13E    014C3E57     C64424 0E 00               MOV BYTE PTR SS:[ESP+0xE],0x0
	$-139    014C3E5C     57                         PUSH EDI
	$-138    014C3E5D     8D4424 12                  LEA EAX,DWORD PTR SS:[ESP+0x12]
	$-134    014C3E61     50                         PUSH EAX
	$-133    014C3E62     8D9E 54040000              LEA EBX,DWORD PTR DS:[ESI+0x454]
	$-12D    014C3E68     C786 94040000 00000000     MOV DWORD PTR DS:[ESI+0x494],0x0
	$-123    014C3E72     53                         PUSH EBX
	$-122    014C3E73     8D46 14                    LEA EAX,DWORD PTR DS:[ESI+0x14]
	$-11F    014C3E76     50                         PUSH EAX
	$-11E    014C3E77     FFB6 8C040000              PUSH DWORD PTR DS:[ESI+0x48C]
	$-118    014C3E7D     8D4E 10                    LEA ECX,DWORD PTR DS:[ESI+0x10]
	$-115    014C3E80     E8 8B320300                CALL PathOfEx.014F7110
	$-110    014C3E85     884424 13                  MOV BYTE PTR SS:[ESP+0x13],AL
	$-10C    014C3E89     84C0                       TEST AL,AL
	$-10A    014C3E8B     0F85 94000000              JNZ PathOfEx.014C3F25
	$-104    014C3E91     83BE 60040000 01           CMP DWORD PTR DS:[ESI+0x460],0x1
	$-FD     014C3E98     0F86 87000000              JBE PathOfEx.014C3F25
	$-F7     014C3E9E     8BCE                       MOV ECX,ESI
	$-F5     014C3EA0     C786 94040000 00000000     MOV DWORD PTR DS:[ESI+0x494],0x0
	$-EB     014C3EAA     C786 60040000 01000000     MOV DWORD PTR DS:[ESI+0x460],0x1
	$-E1     014C3EB4     E8 D7FCFFFF                CALL PathOfEx.014C3B90
	$-DC     014C3EB9     84C0                       TEST AL,AL
	$-DA     014C3EBB     74 68                      JE SHORT PathOfEx.014C3F25
	$-D8     014C3EBD     80BE 80040000 00           CMP BYTE PTR DS:[ESI+0x480],0x0                 ; g_o_is_moving_
	$-D1     014C3EC4     74 10                      JE SHORT PathOfEx.014C3ED6
	$-CF     014C3EC6     8BCE                       MOV ECX,ESI
	$-CD     014C3EC8     E8 63010000                CALL PathOfEx.014C4030
	$-C8     014C3ECD     B0 01                      MOV AL,0x1
	$-C6     014C3ECF     5F                         POP EDI
	$-C5     014C3ED0     5E                         POP ESI
	$-C4     014C3ED1     5B                         POP EBX
	$-C3     014C3ED2     83C4 08                    ADD ESP,0x8
	$-C0     014C3ED5     C3                         RETN
	$-BF     014C3ED6     8B86 74040000              MOV EAX,DWORD PTR DS:[ESI+0x474]
	$-B9     014C3EDC     33FF                       XOR EDI,EDI
	$-B7     014C3EDE     2B86 70040000              SUB EAX,DWORD PTR DS:[ESI+0x470]
	$-B1     014C3EE4     C1F8 02                    SAR EAX,0x2
	$-AE     014C3EE7     85C0                       TEST EAX,EAX
	$-AC     014C3EE9     74 27                      JE SHORT PathOfEx.014C3F12
	$-AA     014C3EEB     0F1F4400 00                NOP DWORD PTR DS:[EAX+EAX]
	$-A5     014C3EF0     8B86 70040000              MOV EAX,DWORD PTR DS:[ESI+0x470]
	$-9F     014C3EF6     8B0CB8                     MOV ECX,DWORD PTR DS:[EAX+EDI*4]
	$-9C     014C3EF9     8B01                       MOV EAX,DWORD PTR DS:[ECX]
	$-9A     014C3EFB     FF50 08                    CALL DWORD PTR DS:[EAX+0x8]
	$-97     014C3EFE     8B86 74040000              MOV EAX,DWORD PTR DS:[ESI+0x474]
	$-91     014C3F04     47                         INC EDI
	$-90     014C3F05     2B86 70040000              SUB EAX,DWORD PTR DS:[ESI+0x470]
	$-8A     014C3F0B     C1F8 02                    SAR EAX,0x2
	$-87     014C3F0E     3BF8                       CMP EDI,EAX
	$-85     014C3F10   ^ 72 DE                      JB SHORT PathOfEx.014C3EF0
	$-83     014C3F12     C786 8C040000 00000000     MOV DWORD PTR DS:[ESI+0x48C],0x0
	$-79     014C3F1C     B0 01                      MOV AL,0x1
	$-77     014C3F1E     5F                         POP EDI
	$-76     014C3F1F     5E                         POP ESI
	$-75     014C3F20     5B                         POP EBX
	$-74     014C3F21     83C4 08                    ADD ESP,0x8
	$-71     014C3F24     C3                         RETN
	$-70     014C3F25     833B 00                    CMP DWORD PTR DS:[EBX],0x0
	$-6D     014C3F28     0F86 98000000              JBE PathOfEx.014C3FC6
	$-67     014C3F2E     807C24 12 00               CMP BYTE PTR SS:[ESP+0x12],0x0
	$-62     014C3F33     74 0A                      JE SHORT PathOfEx.014C3F3F
	$-60     014C3F35     8B46 04                    MOV EAX,DWORD PTR DS:[ESI+0x4]
	$-5D     014C3F38     8B40 2C                    MOV EAX,DWORD PTR DS:[EAX+0x2C]
	$-5A     014C3F3B     8348 2C 04                 OR DWORD PTR DS:[EAX+0x2C],0x4
	$-56     014C3F3F     80BE 80040000 00           CMP BYTE PTR DS:[ESI+0x480],0x0                 ; g_o_is_moving_
	$-4F     014C3F46     8B8E 8C040000              MOV ECX,DWORD PTR DS:[ESI+0x48C]
	$-49     014C3F4C     8B01                       MOV EAX,DWORD PTR DS:[ECX]
	$-47     014C3F4E     8986 84040000              MOV DWORD PTR DS:[ESI+0x484],EAX
	$-41     014C3F54     8B41 04                    MOV EAX,DWORD PTR DS:[ECX+0x4]
	$-3E     014C3F57     8986 88040000              MOV DWORD PTR DS:[ESI+0x488],EAX
	$-38     014C3F5D     75 3D                      JNZ SHORT PathOfEx.014C3F9C
	$-36     014C3F5F     8B86 74040000              MOV EAX,DWORD PTR DS:[ESI+0x474]
	$-30     014C3F65     33FF                       XOR EDI,EDI
	$-2E     014C3F67     2B86 70040000              SUB EAX,DWORD PTR DS:[ESI+0x470]
	$-28     014C3F6D     C1F8 02                    SAR EAX,0x2
	$-25     014C3F70     85C0                       TEST EAX,EAX
	$-23     014C3F72     74 21                      JE SHORT PathOfEx.014C3F95
	$-21     014C3F74     8B86 70040000              MOV EAX,DWORD PTR DS:[ESI+0x470]
	$-1B     014C3F7A     8B0CB8                     MOV ECX,DWORD PTR DS:[EAX+EDI*4]
	$-18     014C3F7D     8B01                       MOV EAX,DWORD PTR DS:[ECX]
	$-16     014C3F7F     FF10                       CALL DWORD PTR DS:[EAX]
	$-14     014C3F81     8B86 74040000              MOV EAX,DWORD PTR DS:[ESI+0x474]
	$-E      014C3F87     47                         INC EDI
	$-D      014C3F88     2B86 70040000              SUB EAX,DWORD PTR DS:[ESI+0x470]
	$-7      014C3F8E     C1F8 02                    SAR EAX,0x2
	$-4      014C3F91     3BF8                       CMP EDI,EAX
	$-2      014C3F93   ^ 72 DF                      JB SHORT PathOfEx.014C3F74
	$ ==>    014C3F95     C686 80040000 01           MOV BYTE PTR DS:[ESI+0x480],0x1                 ; g_o_is_moving_
	$+7      014C3F9C     8B03                       MOV EAX,DWORD PTR DS:[EBX]
	$+9      014C3F9E     8BCE                       MOV ECX,ESI
	$+B      014C3FA0     48                         DEC EAX
	$+C      014C3FA1     C686 81040000 01           MOV BYTE PTR DS:[ESI+0x481],0x1
	$+13     014C3FA8     50                         PUSH EAX
	$+14     014C3FA9     8986 90040000              MOV DWORD PTR DS:[ESI+0x490],EAX
	$+1A     014C3FAF     E8 ECFDFFFF                CALL PathOfEx.014C3DA0
	$+1F     014C3FB4     8A4424 13                  MOV AL,BYTE PTR SS:[ESP+0x13]
	$+23     014C3FB8     C686 81040000 00           MOV BYTE PTR DS:[ESI+0x481],0x0
	$+2A     014C3FBF     5F                         POP EDI
	$+2B     014C3FC0     5E                         POP ESI
	$+2C     014C3FC1     5B                         POP EBX
	$+2D     014C3FC2     83C4 08                    ADD ESP,0x8
	$+30     014C3FC5     C3                         RETN
	$+31     014C3FC6     80BE 80040000 00           CMP BYTE PTR DS:[ESI+0x480],0x0                 ; g_o_is_moving_
	$+38     014C3FCD     74 10                      JE SHORT PathOfEx.014C3FDF
	$+3A     014C3FCF     8BCE                       MOV ECX,ESI
	$+3C     014C3FD1     E8 5A000000                CALL PathOfEx.014C4030
	$+41     014C3FD6     32C0                       XOR AL,AL
	$+43     014C3FD8     5F                         POP EDI
	$+44     014C3FD9     5E                         POP ESI
	$+45     014C3FDA     5B                         POP EBX
	$+46     014C3FDB     83C4 08                    ADD ESP,0x8
	$+49     014C3FDE     C3                         RETN
	$+4A     014C3FDF     C786 8C040000 00000000     MOV DWORD PTR DS:[ESI+0x48C],0x0
	$+54     014C3FE9     33FF                       XOR EDI,EDI
	$+56     014C3FEB     8B86 74040000              MOV EAX,DWORD PTR DS:[ESI+0x474]
	$+5C     014C3FF1     2B86 70040000              SUB EAX,DWORD PTR DS:[ESI+0x470]
	$+62     014C3FF7     C1F8 02                    SAR EAX,0x2
	$+65     014C3FFA     85C0                       TEST EAX,EAX
	$+67     014C3FFC     74 24                      JE SHORT PathOfEx.014C4022
	$+69     014C3FFE     66:90                      NOP
	$+6B     014C4000     8B86 70040000              MOV EAX,DWORD PTR DS:[ESI+0x470]
	$+71     014C4006     8B0CB8                     MOV ECX,DWORD PTR DS:[EAX+EDI*4]
	$+74     014C4009     8B01                       MOV EAX,DWORD PTR DS:[ECX]
	$+76     014C400B     FF50 04                    CALL DWORD PTR DS:[EAX+0x4]
	$+79     014C400E     8B86 74040000              MOV EAX,DWORD PTR DS:[ESI+0x474]
	$+7F     014C4014     47                         INC EDI
	$+80     014C4015     2B86 70040000              SUB EAX,DWORD PTR DS:[ESI+0x470]
	$+86     014C401B     C1F8 02                    SAR EAX,0x2
	$+89     014C401E     3BF8                       CMP EDI,EAX
	$+8B     014C4020   ^ 72 DE                      JB SHORT PathOfEx.014C4000
	$+8D     014C4022     5F                         POP EDI
	$+8E     014C4023     5E                         POP ESI
	$+8F     014C4024     32C0                       XOR AL,AL
	$+91     014C4026     5B                         POP EBX
	$+92     014C4027     83C4 08                    ADD ESP,0x8
	$+95     014C402A     C3                         RETN
	$+96     014C402B     CC                         INT3
	$+97     014C402C     CC                         INT3
	$+98     014C402D     CC                         INT3
	$+99     014C402E     CC                         INT3
	$+9A     014C402F     CC                         INT3
	$+9B     014C4030     56                         PUSH ESI
	$+9C     014C4031     8BF1                       MOV ESI,ECX
	$+9E     014C4033     57                         PUSH EDI
	$+9F     014C4034     80BE 80040000 00           CMP BYTE PTR DS:[ESI+0x480],0x0                 ; g_o_is_moving_
	$+A6     014C403B     0F84 B1000000              JE PathOfEx.014C40F2
	$+AC     014C4041     8B4E 04                    MOV ECX,DWORD PTR DS:[ESI+0x4]
	$+AF     014C4044     C686 81040000 01           MOV BYTE PTR DS:[ESI+0x481],0x1
	$+B6     014C404B     8B49 2C                    MOV ECX,DWORD PTR DS:[ECX+0x2C]
	$+B9     014C404E     E8 3D40FDFF                CALL PathOfEx.01498090
	$+BE     014C4053     8BCE                       MOV ECX,ESI
	$+C0     014C4055     C686 81040000 00           MOV BYTE PTR DS:[ESI+0x481],0x0
	$+C7     014C405C     E8 2FFBFFFF                CALL PathOfEx.014C3B90
	$+CC     014C4061     33FF                       XOR EDI,EDI
	$+CE     014C4063     C686 80040000 00           MOV BYTE PTR DS:[ESI+0x480],0x0                 ; g_o_is_moving_
	$+D5     014C406A     C786 54040000 00000000     MOV DWORD PTR DS:[ESI+0x454],0x0
	$+DF     014C4074     84C0                       TEST AL,AL
	$+E1     014C4076     C786 8C040000 00000000     MOV DWORD PTR DS:[ESI+0x48C],0x0
	$+EB     014C4080     8B86 74040000              MOV EAX,DWORD PTR DS:[ESI+0x474]
	$+F1     014C4086     74 32                      JE SHORT PathOfEx.014C40BA
	$+F3     014C4088     2B86 70040000              SUB EAX,DWORD PTR DS:[ESI+0x470]
	$+F9     014C408E     C1F8 02                    SAR EAX,0x2
	$+FC     014C4091     85C0                       TEST EAX,EAX
	$+FE     014C4093     74 5D                      JE SHORT PathOfEx.014C40F2
	$+100    014C4095     8B86 70040000              MOV EAX,DWORD PTR DS:[ESI+0x470]
	$+106    014C409B     8B0CB8                     MOV ECX,DWORD PTR DS:[EAX+EDI*4]
	$+109    014C409E     8B01                       MOV EAX,DWORD PTR DS:[ECX]
	$+10B    014C40A0     FF50 08                    CALL DWORD PTR DS:[EAX+0x8]
	$+10E    014C40A3     8B86 74040000              MOV EAX,DWORD PTR DS:[ESI+0x474]
	$+114    014C40A9     47                         INC EDI
	$+115    014C40AA     2B86 70040000              SUB EAX,DWORD PTR DS:[ESI+0x470]
	$+11B    014C40B0     C1F8 02                    SAR EAX,0x2
	$+11E    014C40B3     3BF8                       CMP EDI,EAX
	$+120    014C40B5   ^ 72 DE                      JB SHORT PathOfEx.014C4095
	$+122    014C40B7     5F                         POP EDI
	$+123    014C40B8     5E                         POP ESI
	$+124    014C40B9     C3                         RETN
	$+125    014C40BA     2B86 70040000              SUB EAX,DWORD PTR DS:[ESI+0x470]
	$+12B    014C40C0     C1F8 02                    SAR EAX,0x2
	$+12E    014C40C3     85C0                       TEST EAX,EAX
	$+130    014C40C5     74 2B                      JE SHORT PathOfEx.014C40F2
	$+132    014C40C7     66:0F1F8400 00000000       NOP WORD PTR DS:[EAX+EAX]
	$+13B    014C40D0     8B86 70040000              MOV EAX,DWORD PTR DS:[ESI+0x470]
	$+141    014C40D6     8B0CB8                     MOV ECX,DWORD PTR DS:[EAX+EDI*4]
	$+144    014C40D9     8B01                       MOV EAX,DWORD PTR DS:[ECX]
	$+146    014C40DB     FF50 04                    CALL DWORD PTR DS:[EAX+0x4]
	$+149    014C40DE     8B86 74040000              MOV EAX,DWORD PTR DS:[ESI+0x474]
	$+14F    014C40E4     47                         INC EDI
	$+150    014C40E5     2B86 70040000              SUB EAX,DWORD PTR DS:[ESI+0x470]
	$+156    014C40EB     C1F8 02                    SAR EAX,0x2
	$+159    014C40EE     3BF8                       CMP EDI,EAX
	$+15B    014C40F0   ^ 72 DE                      JB SHORT PathOfEx.014C40D0
	$+15D    014C40F2     5F                         POP EDI
	$+15E    014C40F3     5E                         POP ESI
	$+15F    014C40F4     C3                         RETN
	$+160    014C40F5     CC                         INT3
	$+161    014C40F6     CC                         INT3
	$+162    014C40F7     CC                         INT3
	$+163    014C40F8     CC                         INT3
	$+164    014C40F9     CC                         INT3
	$+165    014C40FA     CC                         INT3
	$+166    014C40FB     CC                         INT3
	$+167    014C40FC     CC                         INT3
	$+168    014C40FD     CC                         INT3
	$+169    014C40FE     CC                         INT3
	$+16A    014C40FF     CC                         INT3
	$+16B    014C4100     56                         PUSH ESI
	$+16C    014C4101     8BF1                       MOV ESI,ECX
	$+16E    014C4103     80BE 74040000 00           CMP BYTE PTR DS:[ESI+0x474],0x0
	$+175    014C410A     74 3B                      JE SHORT PathOfEx.014C4147
	$+177    014C410C     57                         PUSH EDI
	$+178    014C410D     8D4E F4                    LEA ECX,DWORD PTR DS:[ESI-0xC]
	$+17B    014C4110     E8 7BFAFFFF                CALL PathOfEx.014C3B90
	$+180    014C4115     8D4E F4                    LEA ECX,DWORD PTR DS:[ESI-0xC]
	$+183    014C4118     84C0                       TEST AL,AL
	$+185    014C411A     74 0A                      JE SHORT PathOfEx.014C4126
	$+187    014C411C     E8 0FFFFFFF                CALL PathOfEx.014C4030
	$+18C    014C4121     5F                         POP EDI
	$+18D    014C4122     5E                         POP ESI
	$+18E    014C4123     C2 0400                    RETN 0x4
	$+191    014C4126     FF8E 84040000              DEC DWORD PTR DS:[ESI+0x484]
	$+197    014C412C     8B86 84040000              MOV EAX,DWORD PTR DS:[ESI+0x484]
	$+19D    014C4132     85C0                       TEST EAX,EAX
	$+19F    014C4134     7E 0B                      JLE SHORT PathOfEx.014C4141
	$+1A1    014C4136     50                         PUSH EAX
	$+1A2    014C4137     E8 64FCFFFF                CALL PathOfEx.014C3DA0
	$+1A7    014C413C     5F                         POP EDI
	$+1A8    014C413D     5E                         POP ESI
	$+1A9    014C413E     C2 0400                    RETN 0x4
	$+1AC    014C4141     E8 0AFDFFFF                CALL PathOfEx.014C3E50
	$+1B1    014C4146     5F                         POP EDI
	$+1B2    014C4147     5E                         POP ESI
	$+1B3    014C4148     C2 0400                    RETN 0x4
	$+1B6    014C414B     CC                         INT3
	$+1B7    014C414C     CC                         INT3
	$+1B8    014C414D     CC                         INT3
	$+1B9    014C414E     CC                         INT3
	$+1BA    014C414F     CC                         INT3
	$+1BB    014C4150     83C1 F4                    ADD ECX,-0xC
	$+1BE    014C4153     E8 D8FEFFFF                CALL PathOfEx.014C4030
	$+1C3    014C4158     C2 0400                    RETN 0x4
	$+1C6    014C415B     CC                         INT3
	$+1C7    014C415C     CC                         INT3
	$+1C8    014C415D     CC                         INT3
	$+1C9    014C415E     CC                         INT3
	$+1CA    014C415F     CC                         INT3
	$+1CB    014C4160     80B9 75040000 00           CMP BYTE PTR DS:[ECX+0x475],0x0
	$+1D2    014C4167     75 08                      JNZ SHORT PathOfEx.014C4171
	$+1D4    014C4169     83C1 F4                    ADD ECX,-0xC
	$+1D7    014C416C     E8 BFFEFFFF                CALL PathOfEx.014C4030
	$+1DC    014C4171     C2 0400                    RETN 0x4
	$+1DF    014C4174     CC                         INT3
	$+1E0    014C4175     CC                         INT3
	$+1E1    014C4176     CC                         INT3
	$+1E2    014C4177     CC                         INT3
	$+1E3    014C4178     CC                         INT3
	$+1E4    014C4179     CC                         INT3
	$+1E5    014C417A     CC                         INT3
	$+1E6    014C417B     CC                         INT3
	$+1E7    014C417C     CC                         INT3
	$+1E8    014C417D     CC                         INT3
	$+1E9    014C417E     CC                         INT3
	$+1EA    014C417F     CC                         INT3
	$+1EB    014C4180     55                         PUSH EBP
	$+1EC    014C4181     8BEC                       MOV EBP,ESP
	$+1EE    014C4183     83E4 F8                    AND ESP,0xFFFFFFF8
	$+1F1    014C4186     51                         PUSH ECX
	$+1F2    014C4187     F3:0F1045 08               MOVSS XMM0,DWORD PTR SS:[EBP+0x8]
	$+1F7    014C418C     56                         PUSH ESI
	$+1F8    014C418D     8BF1                       MOV ESI,ECX
	$+1FA    014C418F     80BE 80040000 00           CMP BYTE PTR DS:[ESI+0x480],0x0
	$+201    014C4196     F3:0F5886 94040000         ADDSS XMM0,DWORD PTR DS:[ESI+0x494]
	$+209    014C419E     F3:0F1186 94040000         MOVSS DWORD PTR DS:[ESI+0x494],XMM0
	$+211    014C41A6     0F84 82000000              JE PathOfEx.014C422E
	$+217    014C41AC     E8 DFF9FFFF                CALL PathOfEx.014C3B90
	$+21C    014C41B1     84C0                       TEST AL,AL
	$+21E    014C41B3     74 0E                      JE SHORT PathOfEx.014C41C3
	$+220    014C41B5     8BCE                       MOV ECX,ESI
	$+222    014C41B7     E8 74FEFFFF                CALL PathOfEx.014C4030
	$+227    014C41BC     5E                         POP ESI
	$+228    014C41BD     8BE5                       MOV ESP,EBP
	$+22A    014C41BF     5D                         POP EBP
	$+22B    014C41C0     C2 0400                    RETN 0x4
	$+22E    014C41C3     F3:0F1086 94040000         MOVSS XMM0,DWORD PTR DS:[ESI+0x494]
	$+236    014C41CB     0F2F05 A8B1DA01            COMISS XMM0,DWORD PTR DS:[0x1DAB1A8]
	$+23D    014C41D2     72 1B                      JB SHORT PathOfEx.014C41EF
	$+23F    014C41D4     8B8E 8C040000              MOV ECX,DWORD PTR DS:[ESI+0x48C]
	$+245    014C41DA     8B86 84040000              MOV EAX,DWORD PTR DS:[ESI+0x484]
	$+24B    014C41E0     3B01                       CMP EAX,DWORD PTR DS:[ECX]
	$+24D    014C41E2     75 43                      JNZ SHORT PathOfEx.014C4227
	$+24F    014C41E4     8B86 88040000              MOV EAX,DWORD PTR DS:[ESI+0x488]
	$+255    014C41EA     3B41 04                    CMP EAX,DWORD PTR DS:[ECX+0x4]
	$+258    014C41ED     75 38                      JNZ SHORT PathOfEx.014C4227
	$+25A    014C41EF     8B86 8C040000              MOV EAX,DWORD PTR DS:[ESI+0x48C]
	$+260    014C41F5     8B96 84040000              MOV EDX,DWORD PTR DS:[ESI+0x484]
	$+266    014C41FB     8B8E 88040000              MOV ECX,DWORD PTR DS:[ESI+0x488]
	$+26C    014C4201     2B10                       SUB EDX,DWORD PTR DS:[EAX]
	$+26E    014C4203     2B48 04                    SUB ECX,DWORD PTR DS:[EAX+0x4]
	$+271    014C4206     0F2F05 ACB0DA01            COMISS XMM0,DWORD PTR DS:[0x1DAB0AC]
	$+278    014C420D     72 1F                      JB SHORT PathOfEx.014C422E
	$+27A    014C420F     0FAFC9                     IMUL ECX,ECX
	$+27D    014C4212     0FAFD2                     IMUL EDX,EDX
	$+280    014C4215     03CA                       ADD ECX,EDX
	$+282    014C4217     66:0F6EC1                  MOVD MM0,ECX
	$+286    014C421B     0F5BC0                     CVTDQ2PS XMM0,XMM0
	$+289    014C421E     0F2F05 A0B7DA01            COMISS XMM0,DWORD PTR DS:[0x1DAB7A0]
	$+290    014C4225     72 07                      JB SHORT PathOfEx.014C422E
	$+292    014C4227     8BCE                       MOV ECX,ESI
	$+294    014C4229     E8 22FCFFFF                CALL PathOfEx.014C3E50
	$+299    014C422E     5E                         POP ESI
	$+29A    014C422F     8BE5                       MOV ESP,EBP
	$+29C    014C4231     5D                         POP EBP
	$+29D    014C4232     C2 0400                    RETN 0x4
	$+2A0    014C4235     CC                         INT3
	$+2A1    014C4236     CC                         INT3
	$+2A2    014C4237     CC                         INT3
	$+2A3    014C4238     CC                         INT3
	$+2A4    014C4239     CC                         INT3
	$+2A5    014C423A     CC                         INT3
	$+2A6    014C423B     CC                         INT3
	$+2A7    014C423C     CC                         INT3
	$+2A8    014C423D     CC                         INT3
	$+2A9    014C423E     CC                         INT3
	$+2AA    014C423F     CC                         INT3
	$+2AB    014C4240     51                         PUSH ECX
	$+2AC    014C4241     53                         PUSH EBX
	$+2AD    014C4242     56                         PUSH ESI
	$+2AE    014C4243     8BF1                       MOV ESI,ECX
	$+2B0    014C4245     57                         PUSH EDI
	$+2B1    014C4246     68 F6000000                PUSH 0xF6
	$+2B6    014C424B     8B86 6C040000              MOV EAX,DWORD PTR DS:[ESI+0x46C]
	$+2BC    014C4251     8B50 0C                    MOV EDX,DWORD PTR DS:[EAX+0xC]
	$+2BF    014C4254     8B46 04                    MOV EAX,DWORD PTR DS:[ESI+0x4]
	$+2C2    014C4257     8B40 04                    MOV EAX,DWORD PTR DS:[EAX+0x4]
	$+2C5    014C425A     8B3C90                     MOV EDI,DWORD PTR DS:[EAX+EDX*4]
	$+2C8    014C425D     8B4F 10                    MOV ECX,DWORD PTR DS:[EDI+0x10]
	$+2CB    014C4260     8B01                       MOV EAX,DWORD PTR DS:[ECX]
	$+2CD    014C4262     FF10                       CALL DWORD PTR DS:[EAX]
	$+2CF    014C4264     BB 9CFFFFFF                MOV EBX,-0x64
	$+2D4    014C4269     3BC3                       CMP EAX,EBX
	$+2D6    014C426B     0F4CC3                     CMOVL EAX,EBX
	$+2D9    014C426E     807C24 14 00               CMP BYTE PTR SS:[ESP+0x14],0x0
	$+2DE    014C4273     66:0F6EC0                  MOVD MM0,EAX
	$+2E2    014C4277     0F5BC0                     CVTDQ2PS XMM0,XMM0
	$+2E5    014C427A     F3:0F5E05 00B4DA01         DIVSS XMM0,DWORD PTR DS:[0x1DAB400]
	$+2ED    014C4282     F3:0F5805 0CB1DA01         ADDSS XMM0,DWORD PTR DS:[0x1DAB10C]
	$+2F5    014C428A     F3:0F114424 0C             MOVSS DWORD PTR SS:[ESP+0xC],XMM0
	$+2FB    014C4290     74 38                      JE SHORT PathOfEx.014C42CA
	$+2FD    014C4292     8B4F 10                    MOV ECX,DWORD PTR DS:[EDI+0x10]
	$+300    014C4295     68 D10C0000                PUSH 0xCD1
	$+305    014C429A     8B01                       MOV EAX,DWORD PTR DS:[ECX]
	$+307    014C429C     FF10                       CALL DWORD PTR DS:[EAX]
	$+309    014C429E     85C0                       TEST EAX,EAX
	$+30B    014C42A0     74 28                      JE SHORT PathOfEx.014C42CA
	$+30D    014C42A2     3BC3                       CMP EAX,EBX
	$+30F    014C42A4     0F4CC3                     CMOVL EAX,EBX
	$+312    014C42A7     66:0F6EC0                  MOVD MM0,EAX
	$+316    014C42AB     0F5BC0                     CVTDQ2PS XMM0,XMM0
	$+319    014C42AE     F3:0F5E05 00B4DA01         DIVSS XMM0,DWORD PTR DS:[0x1DAB400]
	$+321    014C42B6     F3:0F5805 0CB1DA01         ADDSS XMM0,DWORD PTR DS:[0x1DAB10C]
	$+329    014C42BE     F3:0F594424 0C             MULSS XMM0,DWORD PTR SS:[ESP+0xC]
	$+32F    014C42C4     F3:0F114424 0C             MOVSS DWORD PTR SS:[ESP+0xC],XMM0
	$+335    014C42CA     8B4F 10                    MOV ECX,DWORD PTR DS:[EDI+0x10]
	$+338    014C42CD     68 5D040000                PUSH 0x45D
	$+33D    014C42D2     8B01                       MOV EAX,DWORD PTR DS:[ECX]
	$+33F    014C42D4     FF10                       CALL DWORD PTR DS:[EAX]
	$+341    014C42D6     33C9                       XOR ECX,ECX
	$+343    014C42D8     85C0                       TEST EAX,EAX
	$+345    014C42DA     0F4FC8                     CMOVG ECX,EAX
	$+348    014C42DD     85C9                       TEST ECX,ECX
	$+34A    014C42DF     74 43                      JE SHORT PathOfEx.014C4324
	$+34C    014C42E1     66:0F6EC1                  MOVD MM0,ECX
	$+350    014C42E5     8D5424 14                  LEA EDX,DWORD PTR SS:[ESP+0x14]
	$+354    014C42E9     0F5BC0                     CVTDQ2PS XMM0,XMM0
	$+357    014C42EC     8D4C24 0C                  LEA ECX,DWORD PTR SS:[ESP+0xC]
	$+35B    014C42F0     F3:0F114424 14             MOVSS DWORD PTR SS:[ESP+0x14],XMM0
	$+361    014C42F6     F3:0F1086 7C040000         MOVSS XMM0,DWORD PTR DS:[ESI+0x47C]
	$+369    014C42FE     F3:0F594424 0C             MULSS XMM0,DWORD PTR SS:[ESP+0xC]
	$+36F    014C4304     F3:0F114424 0C             MOVSS DWORD PTR SS:[ESP+0xC],XMM0
	$+375    014C430A     E8 1115C5FF                CALL PathOfEx.01115820
	$+37A    014C430F     5F                         POP EDI
	$+37B    014C4310     5E                         POP ESI
	$+37C    014C4311     5B                         POP EBX
	$+37D    014C4312     F3:0F1000                  MOVSS XMM0,DWORD PTR DS:[EAX]
	$+381    014C4316     F3:0F114424 08             MOVSS DWORD PTR SS:[ESP+0x8],XMM0
	$+387    014C431C     D94424 08                  FLD DWORD PTR SS:[ESP+0x8]
	$+38B    014C4320     59                         POP ECX
	$+38C    014C4321     C2 0400                    RETN 0x4
	$+38F    014C4324     F3:0F1086 7C040000         MOVSS XMM0,DWORD PTR DS:[ESI+0x47C]
	$+397    014C432C     F3:0F594424 0C             MULSS XMM0,DWORD PTR SS:[ESP+0xC]
	$+39D    014C4332     5F                         POP EDI
	$+39E    014C4333     5E                         POP ESI
	$+39F    014C4334     5B                         POP EBX
	$+3A0    014C4335     F3:0F114424 08             MOVSS DWORD PTR SS:[ESP+0x8],XMM0
	$+3A6    014C433B     D94424 08                  FLD DWORD PTR SS:[ESP+0x8]
	$+3AA    014C433F     59                         POP ECX
	$+3AB    014C4340     C2 0400                    RETN 0x4
	$+3AE    014C4343     CC                         INT3
	*/
	//gf
	/*
	$-130    008920AF     CC                        INT3
	$-12F    008920B0     56                        PUSH ESI
	$-12E    008920B1     8BF1                      MOV ESI,ECX
	$-12C    008920B3     57                        PUSH EDI
	$-12B    008920B4     807E 24 00                CMP BYTE PTR DS:[ESI+0x24],0x0
	$-127    008920B8     0F84 81000000             JE PathOfEx.0089213F
	$-121    008920BE     8B4E 04                   MOV ECX,DWORD PTR DS:[ESI+0x4]
	$-11E    008920C1     C646 25 01                MOV BYTE PTR DS:[ESI+0x25],0x1
	$-11A    008920C5     8B49 2C                   MOV ECX,DWORD PTR DS:[ECX+0x2C]
	$-117    008920C8     E8 6345FDFF               CALL PathOfEx.00866630
	$-112    008920CD     8BCE                      MOV ECX,ESI
	$-110    008920CF     C646 25 00                MOV BYTE PTR DS:[ESI+0x25],0x0
	$-10C    008920D3     E8 E8FBFFFF               CALL PathOfEx.00891CC0
	$-107    008920D8     33FF                      XOR EDI,EDI
	$-105    008920DA     C646 24 00                MOV BYTE PTR DS:[ESI+0x24],0x0
	$-101    008920DE     C786 84040000 00000000    MOV DWORD PTR DS:[ESI+0x484],0x0
	$-F7     008920E8     84C0                      TEST AL,AL
	$-F5     008920EA     C746 30 00000000          MOV DWORD PTR DS:[ESI+0x30],0x0
	$-EE     008920F1     8B46 18                   MOV EAX,DWORD PTR DS:[ESI+0x18]
	$-EB     008920F4     74 26                     JE SHORT PathOfEx.0089211C
	$-E9     008920F6     2B46 14                   SUB EAX,DWORD PTR DS:[ESI+0x14]
	$-E6     008920F9     C1F8 02                   SAR EAX,0x2
	$-E3     008920FC     85C0                      TEST EAX,EAX
	$-E1     008920FE     74 3F                     JE SHORT PathOfEx.0089213F
	$-DF     00892100     8B46 14                   MOV EAX,DWORD PTR DS:[ESI+0x14]
	$-DC     00892103     8B0CB8                    MOV ECX,DWORD PTR DS:[EAX+EDI*4]
	$-D9     00892106     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$-D7     00892108     FF50 08                   CALL DWORD PTR DS:[EAX+0x8]
	$-D4     0089210B     8B46 18                   MOV EAX,DWORD PTR DS:[ESI+0x18]
	$-D1     0089210E     47                        INC EDI
	$-D0     0089210F     2B46 14                   SUB EAX,DWORD PTR DS:[ESI+0x14]
	$-CD     00892112     C1F8 02                   SAR EAX,0x2
	$-CA     00892115     3BF8                      CMP EDI,EAX
	$-C8     00892117   ^ 72 E7                     JB SHORT PathOfEx.00892100
	$-C6     00892119     5F                        POP EDI
	$-C5     0089211A     5E                        POP ESI
	$-C4     0089211B     C3                        RETN
	$-C3     0089211C     2B46 14                   SUB EAX,DWORD PTR DS:[ESI+0x14]
	$-C0     0089211F     C1F8 02                   SAR EAX,0x2
	$-BD     00892122     85C0                      TEST EAX,EAX
	$-BB     00892124     74 19                     JE SHORT PathOfEx.0089213F
	$-B9     00892126     8B46 14                   MOV EAX,DWORD PTR DS:[ESI+0x14]
	$-B6     00892129     8B0CB8                    MOV ECX,DWORD PTR DS:[EAX+EDI*4]
	$-B3     0089212C     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$-B1     0089212E     FF50 04                   CALL DWORD PTR DS:[EAX+0x4]
	$-AE     00892131     8B46 18                   MOV EAX,DWORD PTR DS:[ESI+0x18]
	$-AB     00892134     47                        INC EDI
	$-AA     00892135     2B46 14                   SUB EAX,DWORD PTR DS:[ESI+0x14]
	$-A7     00892138     C1F8 02                   SAR EAX,0x2
	$-A4     0089213B     3BF8                      CMP EDI,EAX
	$-A2     0089213D   ^ 72 E7                     JB SHORT PathOfEx.00892126
	$-A0     0089213F     5F                        POP EDI
	$-9F     00892140     5E                        POP ESI
	$-9E     00892141     C3                        RETN
	$-9D     00892142     CC                        INT3
	$-9C     00892143     CC                        INT3
	$-9B     00892144     CC                        INT3
	$-9A     00892145     CC                        INT3
	$-99     00892146     CC                        INT3
	$-98     00892147     CC                        INT3
	$-97     00892148     CC                        INT3
	$-96     00892149     CC                        INT3
	$-95     0089214A     CC                        INT3
	$-94     0089214B     CC                        INT3
	$-93     0089214C     CC                        INT3
	$-92     0089214D     CC                        INT3
	$-91     0089214E     CC                        INT3
	$-90     0089214F     CC                        INT3
	$-8F     00892150     56                        PUSH ESI
	$-8E     00892151     8BF1                      MOV ESI,ECX
	$-8C     00892153     807E 18 00                CMP BYTE PTR DS:[ESI+0x18],0x0
	$-88     00892157     74 3B                     JE SHORT PathOfEx.00892194
	$-86     00892159     57                        PUSH EDI
	$-85     0089215A     8D4E F4                   LEA ECX,DWORD PTR DS:[ESI-0xC]
	$-82     0089215D     E8 5EFBFFFF               CALL PathOfEx.00891CC0
	$-7D     00892162     8D4E F4                   LEA ECX,DWORD PTR DS:[ESI-0xC]
	$-7A     00892165     84C0                      TEST AL,AL
	$-78     00892167     74 0A                     JE SHORT PathOfEx.00892173
	$-76     00892169     E8 42FFFFFF               CALL PathOfEx.008920B0
	$-71     0089216E     5F                        POP EDI
	$-70     0089216F     5E                        POP ESI
	$-6F     00892170     C2 0400                   RETN 0x4
	$-6C     00892173     FF8E 7C040000             DEC DWORD PTR DS:[ESI+0x47C]
	$-66     00892179     8B86 7C040000             MOV EAX,DWORD PTR DS:[ESI+0x47C]
	$-60     0089217F     85C0                      TEST EAX,EAX
	$-5E     00892181     7E 0B                     JLE SHORT PathOfEx.0089218E
	$-5C     00892183     50                        PUSH EAX
	$-5B     00892184     E8 F7FCFFFF               CALL PathOfEx.00891E80
	$-56     00892189     5F                        POP EDI
	$-55     0089218A     5E                        POP ESI
	$-54     0089218B     C2 0400                   RETN 0x4
	$-51     0089218E     E8 9DFDFFFF               CALL PathOfEx.00891F30
	$-4C     00892193     5F                        POP EDI
	$-4B     00892194     5E                        POP ESI
	$-4A     00892195     C2 0400                   RETN 0x4
	$-47     00892198     CC                        INT3
	$-46     00892199     CC                        INT3
	$-45     0089219A     CC                        INT3
	$-44     0089219B     CC                        INT3
	$-43     0089219C     CC                        INT3
	$-42     0089219D     CC                        INT3
	$-41     0089219E     CC                        INT3
	$-40     0089219F     CC                        INT3
	$-3F     008921A0     83C1 F4                   ADD ECX,-0xC
	$-3C     008921A3     E8 08FFFFFF               CALL PathOfEx.008920B0
	$-37     008921A8     C2 0400                   RETN 0x4
	$-34     008921AB     CC                        INT3
	$-33     008921AC     CC                        INT3
	$-32     008921AD     CC                        INT3
	$-31     008921AE     CC                        INT3
	$-30     008921AF     CC                        INT3
	$-2F     008921B0     8079 19 00                CMP BYTE PTR DS:[ECX+0x19],0x0
	$-2B     008921B4     75 08                     JNZ SHORT PathOfEx.008921BE
	$-29     008921B6     83C1 F4                   ADD ECX,-0xC
	$-26     008921B9     E8 F2FEFFFF               CALL PathOfEx.008920B0
	$-21     008921BE     C2 0400                   RETN 0x4
	$-1E     008921C1     CC                        INT3
	$-1D     008921C2     CC                        INT3
	$-1C     008921C3     CC                        INT3
	$-1B     008921C4     CC                        INT3
	$-1A     008921C5     CC                        INT3
	$-19     008921C6     CC                        INT3
	$-18     008921C7     CC                        INT3
	$-17     008921C8     CC                        INT3
	$-16     008921C9     CC                        INT3
	$-15     008921CA     CC                        INT3
	$-14     008921CB     CC                        INT3
	$-13     008921CC     CC                        INT3
	$-12     008921CD     CC                        INT3
	$-11     008921CE     CC                        INT3
	$-10     008921CF     CC                        INT3
	$-F      008921D0     55                        PUSH EBP
	$-E      008921D1     8BEC                      MOV EBP,ESP
	$-C      008921D3     83E4 F8                   AND ESP,0xFFFFFFF8
	$-9      008921D6     51                        PUSH ECX
	$-8      008921D7     F3:0F1045 08              MOVSS XMM0,DWORD PTR SS:[EBP+0x8]
	$-3      008921DC     56                        PUSH ESI
	$-2      008921DD     8BF1                      MOV ESI,ECX
	$ ==>    008921DF     807E 24 00                CMP BYTE PTR DS:[ESI+0x24],0x0                 ; g_o_is_moving_
	$+4      008921E3     F3:0F5886 8C040000        ADDSS XMM0,DWORD PTR DS:[ESI+0x48C]
	$+C      008921EB     F3:0F1186 8C040000        MOVSS DWORD PTR DS:[ESI+0x48C],XMM0
	$+14     008921F3     74 70                     JE SHORT PathOfEx.00892265
	$+16     008921F5     E8 C6FAFFFF               CALL PathOfEx.00891CC0
	$+1B     008921FA     84C0                      TEST AL,AL
	$+1D     008921FC     74 0E                     JE SHORT PathOfEx.0089220C
	$+1F     008921FE     8BCE                      MOV ECX,ESI
	$+21     00892200     E8 ABFEFFFF               CALL PathOfEx.008920B0
	$+26     00892205     5E                        POP ESI
	$+27     00892206     8BE5                      MOV ESP,EBP
	$+29     00892208     5D                        POP EBP
	$+2A     00892209     C2 0400                   RETN 0x4
	$+2D     0089220C     F3:0F1086 8C040000        MOVSS XMM0,DWORD PTR DS:[ESI+0x48C]
	$+35     00892214     0F2F05 A82E1501           COMISS XMM0,DWORD PTR DS:[0x1152EA8]
	$+3C     0089221B     72 12                     JB SHORT PathOfEx.0089222F
	$+3E     0089221D     8B4E 30                   MOV ECX,DWORD PTR DS:[ESI+0x30]
	$+41     00892220     8B46 28                   MOV EAX,DWORD PTR DS:[ESI+0x28]
	$+44     00892223     3B01                      CMP EAX,DWORD PTR DS:[ECX]
	$+46     00892225     75 37                     JNZ SHORT PathOfEx.0089225E
	$+48     00892227     8B46 2C                   MOV EAX,DWORD PTR DS:[ESI+0x2C]
	$+4B     0089222A     3B41 04                   CMP EAX,DWORD PTR DS:[ECX+0x4]
	$+4E     0089222D     75 2F                     JNZ SHORT PathOfEx.0089225E
	$+50     0089222F     8B46 30                   MOV EAX,DWORD PTR DS:[ESI+0x30]
	$+53     00892232     8B56 28                   MOV EDX,DWORD PTR DS:[ESI+0x28]
	$+56     00892235     8B4E 2C                   MOV ECX,DWORD PTR DS:[ESI+0x2C]
	$+59     00892238     2B10                      SUB EDX,DWORD PTR DS:[EAX]
	$+5B     0089223A     2B48 04                   SUB ECX,DWORD PTR DS:[EAX+0x4]
	$+5E     0089223D     0F2F05 9C2D1501           COMISS XMM0,DWORD PTR DS:[0x1152D9C]
	$+65     00892244     72 1F                     JB SHORT PathOfEx.00892265
	$+67     00892246     0FAFC9                    IMUL ECX,ECX
	$+6A     00892249     0FAFD2                    IMUL EDX,EDX
	$+6D     0089224C     03CA                      ADD ECX,EDX
	$+6F     0089224E     66:0F6EC1                 MOVD MM0,ECX
	$+73     00892252     0F5BC0                    CVTDQ2PS XMM0,XMM0
	$+76     00892255     0F2F05 00351501           COMISS XMM0,DWORD PTR DS:[0x1153500]
	$+7D     0089225C     72 07                     JB SHORT PathOfEx.00892265
	$+7F     0089225E     8BCE                      MOV ECX,ESI
	$+81     00892260     E8 CBFCFFFF               CALL PathOfEx.00891F30
	$+86     00892265     5E                        POP ESI
	$+87     00892266     8BE5                      MOV ESP,EBP
	$+89     00892268     5D                        POP EBP
	$+8A     00892269     C2 0400                   RETN 0x4
	$+8D     0089226C     CC                        INT3
	$+8E     0089226D     CC                        INT3
	$+8F     0089226E     CC                        INT3
	$+90     0089226F     CC                        INT3
	$+91     00892270     51                        PUSH ECX
	$+92     00892271     53                        PUSH EBX
	$+93     00892272     56                        PUSH ESI
	$+94     00892273     8BF1                      MOV ESI,ECX
	$+96     00892275     57                        PUSH EDI
	$+97     00892276     68 F6000000               PUSH 0xF6
	$+9C     0089227B     8B46 10                   MOV EAX,DWORD PTR DS:[ESI+0x10]
	$+9F     0089227E     8B50 0C                   MOV EDX,DWORD PTR DS:[EAX+0xC]
	$+A2     00892281     8B46 04                   MOV EAX,DWORD PTR DS:[ESI+0x4]
	$+A5     00892284     8B40 04                   MOV EAX,DWORD PTR DS:[EAX+0x4]
	$+A8     00892287     8B3C90                    MOV EDI,DWORD PTR DS:[EAX+EDX*4]
	$+AB     0089228A     8B4F 10                   MOV ECX,DWORD PTR DS:[EDI+0x10]
	$+AE     0089228D     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$+B0     0089228F     FF10                      CALL DWORD PTR DS:[EAX]
	$+B2     00892291     BB 9CFFFFFF               MOV EBX,-0x64
	$+B7     00892296     3BC3                      CMP EAX,EBX
	$+B9     00892298     0F4CC3                    CMOVL EAX,EBX
	$+BC     0089229B     807C24 14 00              CMP BYTE PTR SS:[ESP+0x14],0x0
	$+C1     008922A0     66:0F6EC0                 MOVD MM0,EAX
	$+C5     008922A4     0F5BC0                    CVTDQ2PS XMM0,XMM0
	$+C8     008922A7     F3:0F5E05 04311501        DIVSS XMM0,DWORD PTR DS:[0x1153104]
	$+D0     008922AF     F3:0F5805 FC2D1501        ADDSS XMM0,DWORD PTR DS:[0x1152DFC]
	$+D8     008922B7     F3:0F114424 0C            MOVSS DWORD PTR SS:[ESP+0xC],XMM0
	$+DE     008922BD     74 38                     JE SHORT PathOfEx.008922F7
	$+E0     008922BF     8B4F 10                   MOV ECX,DWORD PTR DS:[EDI+0x10]
	$+E3     008922C2     68 D10C0000               PUSH 0xCD1
	$+E8     008922C7     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$+EA     008922C9     FF10                      CALL DWORD PTR DS:[EAX]
	$+EC     008922CB     85C0                      TEST EAX,EAX
	$+EE     008922CD     74 28                     JE SHORT PathOfEx.008922F7
	$+F0     008922CF     3BC3                      CMP EAX,EBX
	$+F2     008922D1     0F4CC3                    CMOVL EAX,EBX
	$+F5     008922D4     66:0F6EC0                 MOVD MM0,EAX
	$+F9     008922D8     0F5BC0                    CVTDQ2PS XMM0,XMM0
	$+FC     008922DB     F3:0F5E05 04311501        DIVSS XMM0,DWORD PTR DS:[0x1153104]
	$+104    008922E3     F3:0F5805 FC2D1501        ADDSS XMM0,DWORD PTR DS:[0x1152DFC]
	$+10C    008922EB     F3:0F594424 0C            MULSS XMM0,DWORD PTR SS:[ESP+0xC]
	$+112    008922F1     F3:0F114424 0C            MOVSS DWORD PTR SS:[ESP+0xC],XMM0
	$+118    008922F7     8B4F 10                   MOV ECX,DWORD PTR DS:[EDI+0x10]
	$+11B    008922FA     68 5D040000               PUSH 0x45D
	$+120    008922FF     8B01                      MOV EAX,DWORD PTR DS:[ECX]
	$+122    00892301     FF10                      CALL DWORD PTR DS:[EAX]
	$+124    00892303     33C9                      XOR ECX,ECX
	$+126    00892305     85C0                      TEST EAX,EAX
	$+128    00892307     0F4FC8                    CMOVG ECX,EAX
	$+12B    0089230A     85C9                      TEST ECX,ECX
	$+12D    0089230C     74 40                     JE SHORT PathOfEx.0089234E
	$+12F    0089230E     66:0F6EC1                 MOVD MM0,ECX
	$+133    00892312     8D5424 14                 LEA EDX,DWORD PTR SS:[ESP+0x14]
	$+137    00892316     0F5BC0                    CVTDQ2PS XMM0,XMM0
	$+13A    00892319     8D4C24 0C                 LEA ECX,DWORD PTR SS:[ESP+0xC]
	$+13E    0089231D     F3:0F114424 14            MOVSS DWORD PTR SS:[ESP+0x14],XMM0
	$+144    00892323     F3:0F1046 20              MOVSS XMM0,DWORD PTR DS:[ESI+0x20]
	$+149    00892328     F3:0F594424 0C            MULSS XMM0,DWORD PTR SS:[ESP+0xC]
	$+14F    0089232E     F3:0F114424 0C            MOVSS DWORD PTR SS:[ESP+0xC],XMM0
	$+155    00892334     E8 F761C2FF               CALL PathOfEx.004B8530
	$+15A    00892339     5F                        POP EDI
	$+15B    0089233A     5E                        POP ESI
	$+15C    0089233B     5B                        POP EBX
	$+15D    0089233C     F3:0F1000                 MOVSS XMM0,DWORD PTR DS:[EAX]
	$+161    00892340     F3:0F114424 08            MOVSS DWORD PTR SS:[ESP+0x8],XMM0
	$+167    00892346     D94424 08                 FLD DWORD PTR SS:[ESP+0x8]
	$+16B    0089234A     59                        POP ECX
	$+16C    0089234B     C2 0400                   RETN 0x4
	$+16F    0089234E     F3:0F1046 20              MOVSS XMM0,DWORD PTR DS:[ESI+0x20]
	$+174    00892353     F3:0F594424 0C            MULSS XMM0,DWORD PTR SS:[ESP+0xC]
	$+17A    00892359     5F                        POP EDI
	$+17B    0089235A     5E                        POP ESI
	$+17C    0089235B     5B                        POP EBX
	$+17D    0089235C     F3:0F114424 08            MOVSS DWORD PTR SS:[ESP+0x8],XMM0
	$+183    00892362     D94424 08                 FLD DWORD PTR SS:[ESP+0x8]
	$+187    00892366     59                        POP ECX
	$+188    00892367     C2 0400                   RETN 0x4
	$+18B    0089236A     CC                        INT3
	$+18C    0089236B     CC                        INT3
	$+18D    0089236C     CC                        INT3
	$+18E    0089236D     CC                        INT3
	$+18F    0089236E     CC                        INT3
	$+190    0089236F     CC                        INT3
	$+191    00892370     D941 20                   FLD DWORD PTR DS:[ECX+0x20]
	$+194    00892373     C3                        RETN
	$+195    00892374     CC                        INT3
	$+196    00892375     CC                        INT3
	$+197    00892376     CC                        INT3
	$+198    00892377     CC                        INT3
	$+199    00892378     CC                        INT3
	$+19A    00892379     CC                        INT3
	$+19B    0089237A     CC                        INT3
	$+19C    0089237B     CC                        INT3
	$+19D    0089237C     CC                        INT3
	$+19E    0089237D     CC                        INT3
	$+19F    0089237E     CC                        INT3
	$+1A0    0089237F     CC                        INT3
	$+1A1    00892380     55                        PUSH EBP
	$+1A2    00892381     8BEC                      MOV EBP,ESP
	$+1A4    00892383     6A FF                     PUSH -0x1
	$+1A6    00892385     68 A328F300               PUSH PathOfEx.00F328A3
	$+1AB    0089238A     64:A1 00000000            MOV EAX,DWORD PTR FS:[0]
	$+1B1    00892390     50                        PUSH EAX
	$+1B2    00892391     64:8925 00000000          MOV DWORD PTR FS:[0],ESP
	$+1B9    00892398     51                        PUSH ECX
	$+1BA    00892399     53                        PUSH EBX
	$+1BB    0089239A     57                        PUSH EDI
	$+1BC    0089239B     8BF9                      MOV EDI,ECX
	$+1BE    0089239D     897D F0                   MOV DWORD PTR SS:[EBP-0x10],EDI
	$+1C1    008923A0     8B45 0C                   MOV EAX,DWORD PTR SS:[EBP+0xC]
	$+1C4    008923A3     8947 04                   MOV DWORD PTR DS:[EDI+0x4],EAX
	$+1C7    008923A6     C647 08 00                MOV BYTE PTR DS:[EDI+0x8],0x0
	$+1CB    008923AA     C745 FC 00000000          MOV DWORD PTR SS:[EBP-0x4],0x0
	$+1D2    008923B1     8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+1D5    008923B4     C747 0C E8C10A01          MOV DWORD PTR DS:[EDI+0xC],PathOfEx.010AC1E8
	$+1DC    008923BB     C707 74E80F01             MOV DWORD PTR DS:[EDI],PathOfEx.010FE874
	$+1E2    008923C1     C747 0C A8E80F01          MOV DWORD PTR DS:[EDI+0xC],PathOfEx.010FE8A8
	$+1E9    008923C8     8947 10                   MOV DWORD PTR DS:[EDI+0x10],EAX
	$+1EC    008923CB     C747 14 00000000          MOV DWORD PTR DS:[EDI+0x14],0x0
	$+1F3    008923D2     C747 18 00000000          MOV DWORD PTR DS:[EDI+0x18],0x0
	$+1FA    008923D9     C747 1C 00000000          MOV DWORD PTR DS:[EDI+0x1C],0x0
	$+201    008923E0     C645 FC 01                MOV BYTE PTR SS:[EBP-0x4],0x1
	$+205    008923E4     66:C747 24 0000           MOV WORD PTR DS:[EDI+0x24],0x0
	$+20B    008923EA     C647 26 00                MOV BYTE PTR DS:[EDI+0x26],0x0
	$+20F    008923EE     C747 28 00000000          MOV DWORD PTR DS:[EDI+0x28],0x0
	$+216    008923F5     C747 2C 00000000          MOV DWORD PTR DS:[EDI+0x2C],0x0
	$+21D    008923FC     8B5F 04                   MOV EBX,DWORD PTR DS:[EDI+0x4]
	$+220    008923FF     C747 30 00000000          MOV DWORD PTR DS:[EDI+0x30],0x0
	$+227    00892406     C747 34 00000000          MOV DWORD PTR DS:[EDI+0x34],0x0
	$+22E    0089240D     C747 38 00000000          MOV DWORD PTR DS:[EDI+0x38],0x0
	$+235    00892414     C747 3C 01000000          MOV DWORD PTR DS:[EDI+0x3C],0x1
	$+23C    0089241B     C747 40 01000000          MOV DWORD PTR DS:[EDI+0x40],0x1
	$+243    00892422     C787 84040000 00000000    MOV DWORD PTR DS:[EDI+0x484],0x0
	$+24D    0089242C     C787 88040000 00000000    MOV DWORD PTR DS:[EDI+0x488],0x0
	$+257    00892436     C787 8C040000 00401C46    MOV DWORD PTR DS:[EDI+0x48C],0x461C4000
	$+261    00892440     C687 90040000 00          MOV BYTE PTR DS:[EDI+0x490],0x0
	$+268    00892447     66:0F6E40 10              MOVD MM0,DWORD PTR DS:[EAX+0x10]
	$+26D    0089244C     0F5BC0                    CVTDQ2PS XMM0,XMM0
	$+270    0089244F     F3:0F1147 20              MOVSS DWORD PTR DS:[EDI+0x20],XMM0
	$+275    00892454     8B0B                      MOV ECX,DWORD PTR DS:[EBX]
	$+277    00892456     85C9                      TEST ECX,ECX
	$+279    00892458     74 05                     JE SHORT PathOfEx.0089245F
	$+27B    0089245A     E8 F1ACF0FF               CALL PathOfEx.0079D150
	$+280    0089245F     8B03                      MOV EAX,DWORD PTR DS:[EBX]
	$+282    00892461     56                        PUSH ESI
	$+283    00892462     C745 0C B4200701          MOV DWORD PTR SS:[EBP+0xC],PathOfEx.010720B4   ; ASCII "Positioned"
	$+28A    00892469     8B70 2C                   MOV ESI,DWORD PTR DS:[EAX+0x2C]
	$+28D    0089246C     8D45 0C                   LEA EAX,DWORD PTR SS:[EBP+0xC]
	$+290    0089246F     50                        PUSH EAX
	$+291    00892470     8D45 08                   LEA EAX,DWORD PTR SS:[EBP+0x8]
	$+294    00892473     50                        PUSH EAX
	$+295    00892474     8D4E 14                   LEA ECX,DWORD PTR DS:[ESI+0x14]
	$+298    00892477     E8 9483BEFF               CALL PathOfEx.0047A810
	$+29D    0089247C     8B45 08                   MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+2A0    0089247F     3B46 18                   CMP EAX,DWORD PTR DS:[ESI+0x18]
	$+2A3    00892482     5E                        POP ESI
	$+2A4    00892483     74 10                     JE SHORT PathOfEx.00892495
	$+2A6    00892485     8B48 0C                   MOV ECX,DWORD PTR DS:[EAX+0xC]
	$+2A9    00892488     83F9 FF                   CMP ECX,-0x1
	$+2AC    0089248B     74 08                     JE SHORT PathOfEx.00892495
	$+2AE    0089248D     8B43 04                   MOV EAX,DWORD PTR DS:[EBX+0x4]
	$+2B1    00892490     8B0C88                    MOV ECX,DWORD PTR DS:[EAX+ECX*4]
	$+2B4    00892493     EB 02                     JMP SHORT PathOfEx.00892497
	$+2B6    00892495     33C9                      XOR ECX,ECX
	$+2B8    00892497     8D47 0C                   LEA EAX,DWORD PTR DS:[EDI+0xC]
	$+2BB    0089249A     81C1 EC000000             ADD ECX,0xEC
	$+2C1    008924A0     8945 0C                   MOV DWORD PTR SS:[EBP+0xC],EAX
	$+2C4    008924A3     8D45 0C                   LEA EAX,DWORD PTR SS:[EBP+0xC]
	$+2C7    008924A6     50                        PUSH EAX
	$+2C8    008924A7     E8 8478BEFF               CALL PathOfEx.00479D30
	$+2CD    008924AC     8B4D F4                   MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+2D0    008924AF     8BC7                      MOV EAX,EDI
	$+2D2    008924B1     5F                        POP EDI
	$+2D3    008924B2     5B                        POP EBX
	$+2D4    008924B3     64:890D 00000000          MOV DWORD PTR FS:[0],ECX
	$+2DB    008924BA     8BE5                      MOV ESP,EBP
	$+2DD    008924BC     5D                        POP EBP
	$+2DE    008924BD     C2 0800                   RETN 0x8
	$+2E1    008924C0     56                        PUSH ESI
	$+2E2    008924C1     8B7424 08                 MOV ESI,DWORD PTR SS:[ESP+0x8]
	$+2E6    008924C5     3971 30                   CMP DWORD PTR DS:[ECX+0x30],ESI
	$+2E9    008924C8     75 10                     JNZ SHORT PathOfEx.008924DA
	$+2EB    008924CA     8B06                      MOV EAX,DWORD PTR DS:[ESI]
	$+2ED    008924CC     8D51 28                   LEA EDX,DWORD PTR DS:[ECX+0x28]
	$+2F0    008924CF     8902                      MOV DWORD PTR DS:[EDX],EAX
	$+2F2    008924D1     8B46 04                   MOV EAX,DWORD PTR DS:[ESI+0x4]
	$+2F5    008924D4     8942 04                   MOV DWORD PTR DS:[EDX+0x4],EAX
	$+2F8    008924D7     8951 30                   MOV DWORD PTR DS:[ECX+0x30],EDX
	$+2FB    008924DA     5E                        POP ESI
	$+2FC    008924DB     C2 0400                   RETN 0x4
	$+2FF    008924DE     CC                        INT3
	$+300    008924DF     CC                        INT3
	$+301    008924E0     56                        PUSH ESI
	$+302    008924E1     8BF1                      MOV ESI,ECX
	$+304    008924E3     8B4E 14                   MOV ECX,DWORD PTR DS:[ESI+0x14]
	$+307    008924E6     85C9                      TEST ECX,ECX
	$+309    008924E8     74 29                     JE SHORT PathOfEx.00892513
	$+30B    008924EA     8B46 1C                   MOV EAX,DWORD PTR DS:[ESI+0x1C]
	$+30E    008924ED     2BC1                      SUB EAX,ECX
	$+310    008924EF     6A 04                     PUSH 0x4
	$+312    008924F1     C1F8 02                   SAR EAX,0x2
	$+315    008924F4     50                        PUSH EAX
	$+316    008924F5     51                        PUSH ECX
	$+317    008924F6     E8 058BBEFF               CALL PathOfEx.0047B000
	$+31C    008924FB     C746 14 00000000          MOV DWORD PTR DS:[ESI+0x14],0x0
	$+323    00892502     83C4 0C                   ADD ESP,0xC
	$+326    00892505     C746 18 00000000          MOV DWORD PTR DS:[ESI+0x18],0x0
	$+32D    0089250C     C746 1C 00000000          MOV DWORD PTR DS:[ESI+0x1C],0x0
	$+334    00892513     F64424 08 01              TEST BYTE PTR SS:[ESP+0x8],0x1
	$+339    00892518     C706 BC1C0701             MOV DWORD PTR DS:[ESI],PathOfEx.01071CBC       ; ASCII "疠K"
	$+33F    0089251E     74 0E                     JE SHORT PathOfEx.0089252E
	$+341    00892520     68 94040000               PUSH 0x494
	$+346    00892525     56                        PUSH ESI
	$+347    00892526     E8 DAB65C00               CALL PathOfEx.00E5DC05
	$+34C    0089252B     83C4 08                   ADD ESP,0x8
	$+34F    0089252E     8BC6                      MOV EAX,ESI
	$+351    00892530     5E                        POP ESI
	$+352    00892531     C2 0400                   RETN 0x4
	$+355    00892534     CC                        INT3
	*/
	//gf 2017/8/18 14:59
	/*
	$-160    008AF27F     CC                         INT3
	$-15F    008AF280     56                         PUSH ESI
	$-15E    008AF281     8BF1                       MOV ESI,ECX
	$-15C    008AF283     57                         PUSH EDI
	$-15B    008AF284     80BE 80040000 00           CMP BYTE PTR DS:[ESI+0x480],0x0
	$-154    008AF28B     0F84 B1000000              JE PathOfEx.008AF342
	$-14E    008AF291     8B4E 04                    MOV ECX,DWORD PTR DS:[ESI+0x4]
	$-14B    008AF294     C686 81040000 01           MOV BYTE PTR DS:[ESI+0x481],0x1
	$-144    008AF29B     8B49 2C                    MOV ECX,DWORD PTR DS:[ECX+0x2C]
	$-141    008AF29E     E8 FD32FDFF                CALL PathOfEx.008825A0
	$-13C    008AF2A3     8BCE                       MOV ECX,ESI
	$-13A    008AF2A5     C686 81040000 00           MOV BYTE PTR DS:[ESI+0x481],0x0
	$-133    008AF2AC     E8 2FFBFFFF                CALL PathOfEx.008AEDE0
	$-12E    008AF2B1     33FF                       XOR EDI,EDI
	$-12C    008AF2B3     C686 80040000 00           MOV BYTE PTR DS:[ESI+0x480],0x0
	$-125    008AF2BA     C786 54040000 00000000     MOV DWORD PTR DS:[ESI+0x454],0x0
	$-11B    008AF2C4     84C0                       TEST AL,AL
	$-119    008AF2C6     C786 8C040000 00000000     MOV DWORD PTR DS:[ESI+0x48C],0x0
	$-10F    008AF2D0     8B86 74040000              MOV EAX,DWORD PTR DS:[ESI+0x474]
	$-109    008AF2D6     74 32                      JE SHORT PathOfEx.008AF30A
	$-107    008AF2D8     2B86 70040000              SUB EAX,DWORD PTR DS:[ESI+0x470]
	$-101    008AF2DE     C1F8 02                    SAR EAX,0x2
	$-FE     008AF2E1     85C0                       TEST EAX,EAX
	$-FC     008AF2E3     74 5D                      JE SHORT PathOfEx.008AF342
	$-FA     008AF2E5     8B86 70040000              MOV EAX,DWORD PTR DS:[ESI+0x470]
	$-F4     008AF2EB     8B0CB8                     MOV ECX,DWORD PTR DS:[EAX+EDI*4]
	$-F1     008AF2EE     8B01                       MOV EAX,DWORD PTR DS:[ECX]
	$-EF     008AF2F0     FF50 08                    CALL DWORD PTR DS:[EAX+0x8]
	$-EC     008AF2F3     8B86 74040000              MOV EAX,DWORD PTR DS:[ESI+0x474]
	$-E6     008AF2F9     47                         INC EDI
	$-E5     008AF2FA     2B86 70040000              SUB EAX,DWORD PTR DS:[ESI+0x470]
	$-DF     008AF300     C1F8 02                    SAR EAX,0x2
	$-DC     008AF303     3BF8                       CMP EDI,EAX
	$-DA     008AF305   ^ 72 DE                      JB SHORT PathOfEx.008AF2E5
	$-D8     008AF307     5F                         POP EDI
	$-D7     008AF308     5E                         POP ESI
	$-D6     008AF309     C3                         RETN
	$-D5     008AF30A     2B86 70040000              SUB EAX,DWORD PTR DS:[ESI+0x470]
	$-CF     008AF310     C1F8 02                    SAR EAX,0x2
	$-CC     008AF313     85C0                       TEST EAX,EAX
	$-CA     008AF315     74 2B                      JE SHORT PathOfEx.008AF342
	$-C8     008AF317     66:0F1F8400 00000000       NOP WORD PTR DS:[EAX+EAX]
	$-BF     008AF320     8B86 70040000              MOV EAX,DWORD PTR DS:[ESI+0x470]
	$-B9     008AF326     8B0CB8                     MOV ECX,DWORD PTR DS:[EAX+EDI*4]
	$-B6     008AF329     8B01                       MOV EAX,DWORD PTR DS:[ECX]
	$-B4     008AF32B     FF50 04                    CALL DWORD PTR DS:[EAX+0x4]
	$-B1     008AF32E     8B86 74040000              MOV EAX,DWORD PTR DS:[ESI+0x474]
	$-AB     008AF334     47                         INC EDI
	$-AA     008AF335     2B86 70040000              SUB EAX,DWORD PTR DS:[ESI+0x470]
	$-A4     008AF33B     C1F8 02                    SAR EAX,0x2
	$-A1     008AF33E     3BF8                       CMP EDI,EAX
	$-9F     008AF340   ^ 72 DE                      JB SHORT PathOfEx.008AF320
	$-9D     008AF342     5F                         POP EDI
	$-9C     008AF343     5E                         POP ESI
	$-9B     008AF344     C3                         RETN
	$-9A     008AF345     CC                         INT3
	$-99     008AF346     CC                         INT3
	$-98     008AF347     CC                         INT3
	$-97     008AF348     CC                         INT3
	$-96     008AF349     CC                         INT3
	$-95     008AF34A     CC                         INT3
	$-94     008AF34B     CC                         INT3
	$-93     008AF34C     CC                         INT3
	$-92     008AF34D     CC                         INT3
	$-91     008AF34E     CC                         INT3
	$-90     008AF34F     CC                         INT3
	$-8F     008AF350     56                         PUSH ESI
	$-8E     008AF351     8BF1                       MOV ESI,ECX
	$-8C     008AF353     80BE 74040000 00           CMP BYTE PTR DS:[ESI+0x474],0x0
	$-85     008AF35A     74 3B                      JE SHORT PathOfEx.008AF397
	$-83     008AF35C     57                         PUSH EDI
	$-82     008AF35D     8D4E F4                    LEA ECX,DWORD PTR DS:[ESI-0xC]
	$-7F     008AF360     E8 7BFAFFFF                CALL PathOfEx.008AEDE0
	$-7A     008AF365     8D4E F4                    LEA ECX,DWORD PTR DS:[ESI-0xC]
	$-77     008AF368     84C0                       TEST AL,AL
	$-75     008AF36A     74 0A                      JE SHORT PathOfEx.008AF376
	$-73     008AF36C     E8 0FFFFFFF                CALL PathOfEx.008AF280
	$-6E     008AF371     5F                         POP EDI
	$-6D     008AF372     5E                         POP ESI
	$-6C     008AF373     C2 0400                    RETN 0x4
	$-69     008AF376     FF8E 84040000              DEC DWORD PTR DS:[ESI+0x484]
	$-63     008AF37C     8B86 84040000              MOV EAX,DWORD PTR DS:[ESI+0x484]
	$-5D     008AF382     85C0                       TEST EAX,EAX
	$-5B     008AF384     7E 0B                      JLE SHORT PathOfEx.008AF391
	$-59     008AF386     50                         PUSH EAX
	$-58     008AF387     E8 64FCFFFF                CALL PathOfEx.008AEFF0
	$-53     008AF38C     5F                         POP EDI
	$-52     008AF38D     5E                         POP ESI
	$-51     008AF38E     C2 0400                    RETN 0x4
	$-4E     008AF391     E8 0AFDFFFF                CALL PathOfEx.008AF0A0
	$-49     008AF396     5F                         POP EDI
	$-48     008AF397     5E                         POP ESI
	$-47     008AF398     C2 0400                    RETN 0x4
	$-44     008AF39B     CC                         INT3
	$-43     008AF39C     CC                         INT3
	$-42     008AF39D     CC                         INT3
	$-41     008AF39E     CC                         INT3
	$-40     008AF39F     CC                         INT3
	$-3F     008AF3A0     83C1 F4                    ADD ECX,-0xC
	$-3C     008AF3A3     E8 D8FEFFFF                CALL PathOfEx.008AF280
	$-37     008AF3A8     C2 0400                    RETN 0x4
	$-34     008AF3AB     CC                         INT3
	$-33     008AF3AC     CC                         INT3
	$-32     008AF3AD     CC                         INT3
	$-31     008AF3AE     CC                         INT3
	$-30     008AF3AF     CC                         INT3
	$-2F     008AF3B0     80B9 75040000 00           CMP BYTE PTR DS:[ECX+0x475],0x0
	$-28     008AF3B7     75 08                      JNZ SHORT PathOfEx.008AF3C1
	$-26     008AF3B9     83C1 F4                    ADD ECX,-0xC
	$-23     008AF3BC     E8 BFFEFFFF                CALL PathOfEx.008AF280
	$-1E     008AF3C1     C2 0400                    RETN 0x4
	$-1B     008AF3C4     CC                         INT3
	$-1A     008AF3C5     CC                         INT3
	$-19     008AF3C6     CC                         INT3
	$-18     008AF3C7     CC                         INT3
	$-17     008AF3C8     CC                         INT3
	$-16     008AF3C9     CC                         INT3
	$-15     008AF3CA     CC                         INT3
	$-14     008AF3CB     CC                         INT3
	$-13     008AF3CC     CC                         INT3
	$-12     008AF3CD     CC                         INT3
	$-11     008AF3CE     CC                         INT3
	$-10     008AF3CF     CC                         INT3
	$-F      008AF3D0     55                         PUSH EBP
	$-E      008AF3D1     8BEC                       MOV EBP,ESP
	$-C      008AF3D3     83E4 F8                    AND ESP,0xFFFFFFF8
	$-9      008AF3D6     51                         PUSH ECX
	$-8      008AF3D7     F3:0F1045 08               MOVSS XMM0,DWORD PTR SS:[EBP+0x8]
	$-3      008AF3DC     56                         PUSH ESI
	$-2      008AF3DD     8BF1                       MOV ESI,ECX
	$ ==>    008AF3DF     80BE 80040000 00           CMP BYTE PTR DS:[ESI+0x480],0x0                 ; g_o_is_moving_
	$+7      008AF3E6     F3:0F5886 94040000         ADDSS XMM0,DWORD PTR DS:[ESI+0x494]
	$+F      008AF3EE     F3:0F1186 94040000         MOVSS DWORD PTR DS:[ESI+0x494],XMM0
	$+17     008AF3F6     0F84 82000000              JE PathOfEx.008AF47E
	$+1D     008AF3FC     E8 DFF9FFFF                CALL PathOfEx.008AEDE0
	$+22     008AF401     84C0                       TEST AL,AL
	$+24     008AF403     74 0E                      JE SHORT PathOfEx.008AF413
	$+26     008AF405     8BCE                       MOV ECX,ESI
	$+28     008AF407     E8 74FEFFFF                CALL PathOfEx.008AF280
	$+2D     008AF40C     5E                         POP ESI
	$+2E     008AF40D     8BE5                       MOV ESP,EBP
	$+30     008AF40F     5D                         POP EBP
	$+31     008AF410     C2 0400                    RETN 0x4
	$+34     008AF413     F3:0F1086 94040000         MOVSS XMM0,DWORD PTR DS:[ESI+0x494]
	$+3C     008AF41B     0F2F05 80C81A01            COMISS XMM0,DWORD PTR DS:[0x11AC880]
	$+43     008AF422     72 1B                      JB SHORT PathOfEx.008AF43F
	$+45     008AF424     8B8E 8C040000              MOV ECX,DWORD PTR DS:[ESI+0x48C]
	$+4B     008AF42A     8B86 84040000              MOV EAX,DWORD PTR DS:[ESI+0x484]
	$+51     008AF430     3B01                       CMP EAX,DWORD PTR DS:[ECX]
	$+53     008AF432     75 43                      JNZ SHORT PathOfEx.008AF477
	$+55     008AF434     8B86 88040000              MOV EAX,DWORD PTR DS:[ESI+0x488]
	$+5B     008AF43A     3B41 04                    CMP EAX,DWORD PTR DS:[ECX+0x4]
	$+5E     008AF43D     75 38                      JNZ SHORT PathOfEx.008AF477
	$+60     008AF43F     8B86 8C040000              MOV EAX,DWORD PTR DS:[ESI+0x48C]
	$+66     008AF445     8B96 84040000              MOV EDX,DWORD PTR DS:[ESI+0x484]
	$+6C     008AF44B     8B8E 88040000              MOV ECX,DWORD PTR DS:[ESI+0x488]
	$+72     008AF451     2B10                       SUB EDX,DWORD PTR DS:[EAX]
	$+74     008AF453     2B48 04                    SUB ECX,DWORD PTR DS:[EAX+0x4]
	$+77     008AF456     0F2F05 6CC71A01            COMISS XMM0,DWORD PTR DS:[0x11AC76C]
	$+7E     008AF45D     72 1F                      JB SHORT PathOfEx.008AF47E
	$+80     008AF45F     0FAFC9                     IMUL ECX,ECX
	$+83     008AF462     0FAFD2                     IMUL EDX,EDX
	$+86     008AF465     03CA                       ADD ECX,EDX
	$+88     008AF467     66:0F6EC1                  MOVD MM0,ECX
	$+8C     008AF46B     0F5BC0                     CVTDQ2PS XMM0,XMM0
	$+8F     008AF46E     0F2F05 DCCE1A01            COMISS XMM0,DWORD PTR DS:[0x11ACEDC]
	$+96     008AF475     72 07                      JB SHORT PathOfEx.008AF47E
	$+98     008AF477     8BCE                       MOV ECX,ESI
	$+9A     008AF479     E8 22FCFFFF                CALL PathOfEx.008AF0A0
	$+9F     008AF47E     5E                         POP ESI
	$+A0     008AF47F     8BE5                       MOV ESP,EBP
	$+A2     008AF481     5D                         POP EBP
	$+A3     008AF482     C2 0400                    RETN 0x4
	$+A6     008AF485     CC                         INT3
	$+A7     008AF486     CC                         INT3
	$+A8     008AF487     CC                         INT3
	$+A9     008AF488     CC                         INT3
	$+AA     008AF489     CC                         INT3
	$+AB     008AF48A     CC                         INT3
	$+AC     008AF48B     CC                         INT3
	$+AD     008AF48C     CC                         INT3
	$+AE     008AF48D     CC                         INT3
	$+AF     008AF48E     CC                         INT3
	$+B0     008AF48F     CC                         INT3
	$+B1     008AF490     51                         PUSH ECX
	$+B2     008AF491     53                         PUSH EBX
	$+B3     008AF492     56                         PUSH ESI
	$+B4     008AF493     8BF1                       MOV ESI,ECX
	$+B6     008AF495     57                         PUSH EDI
	$+B7     008AF496     68 F4000000                PUSH 0xF4
	$+BC     008AF49B     8B86 6C040000              MOV EAX,DWORD PTR DS:[ESI+0x46C]
	$+C2     008AF4A1     8B50 0C                    MOV EDX,DWORD PTR DS:[EAX+0xC]
	$+C5     008AF4A4     8B46 04                    MOV EAX,DWORD PTR DS:[ESI+0x4]
	$+C8     008AF4A7     8B40 04                    MOV EAX,DWORD PTR DS:[EAX+0x4]
	$+CB     008AF4AA     8B3C90                     MOV EDI,DWORD PTR DS:[EAX+EDX*4]
	$+CE     008AF4AD     8B4F 10                    MOV ECX,DWORD PTR DS:[EDI+0x10]
	$+D1     008AF4B0     8B01                       MOV EAX,DWORD PTR DS:[ECX]
	$+D3     008AF4B2     FF10                       CALL DWORD PTR DS:[EAX]
	$+D5     008AF4B4     BB 9CFFFFFF                MOV EBX,-0x64
	$+DA     008AF4B9     3BC3                       CMP EAX,EBX
	$+DC     008AF4BB     0F4CC3                     CMOVL EAX,EBX
	$+DF     008AF4BE     807C24 14 00               CMP BYTE PTR SS:[ESP+0x14],0x0
	$+E4     008AF4C3     66:0F6EC0                  MOVD MM0,EAX
	$+E8     008AF4C7     0F5BC0                     CVTDQ2PS XMM0,XMM0
	$+EB     008AF4CA     F3:0F5E05 DCCA1A01         DIVSS XMM0,DWORD PTR DS:[0x11ACADC]
	$+F3     008AF4D2     F3:0F5805 D0C71A01         ADDSS XMM0,DWORD PTR DS:[0x11AC7D0]
	$+FB     008AF4DA     F3:0F114424 0C             MOVSS DWORD PTR SS:[ESP+0xC],XMM0
	$+101    008AF4E0     74 38                      JE SHORT PathOfEx.008AF51A
	$+103    008AF4E2     8B4F 10                    MOV ECX,DWORD PTR DS:[EDI+0x10]
	$+106    008AF4E5     68 CC0C0000                PUSH 0xCCC
	$+10B    008AF4EA     8B01                       MOV EAX,DWORD PTR DS:[ECX]
	$+10D    008AF4EC     FF10                       CALL DWORD PTR DS:[EAX]
	$+10F    008AF4EE     85C0                       TEST EAX,EAX
	$+111    008AF4F0     74 28                      JE SHORT PathOfEx.008AF51A
	$+113    008AF4F2     3BC3                       CMP EAX,EBX
	$+115    008AF4F4     0F4CC3                     CMOVL EAX,EBX
	$+118    008AF4F7     66:0F6EC0                  MOVD MM0,EAX
	$+11C    008AF4FB     0F5BC0                     CVTDQ2PS XMM0,XMM0
	$+11F    008AF4FE     F3:0F5E05 DCCA1A01         DIVSS XMM0,DWORD PTR DS:[0x11ACADC]
	$+127    008AF506     F3:0F5805 D0C71A01         ADDSS XMM0,DWORD PTR DS:[0x11AC7D0]
	$+12F    008AF50E     F3:0F594424 0C             MULSS XMM0,DWORD PTR SS:[ESP+0xC]
	$+135    008AF514     F3:0F114424 0C             MOVSS DWORD PTR SS:[ESP+0xC],XMM0
	$+13B    008AF51A     8B4F 10                    MOV ECX,DWORD PTR DS:[EDI+0x10]
	$+13E    008AF51D     68 59040000                PUSH 0x459
	$+143    008AF522     8B01                       MOV EAX,DWORD PTR DS:[ECX]
	$+145    008AF524     FF10                       CALL DWORD PTR DS:[EAX]
	$+147    008AF526     33C9                       XOR ECX,ECX
	$+149    008AF528     85C0                       TEST EAX,EAX
	$+14B    008AF52A     0F4FC8                     CMOVG ECX,EAX
	$+14E    008AF52D     85C9                       TEST ECX,ECX
	$+150    008AF52F     74 43                      JE SHORT PathOfEx.008AF574
	$+152    008AF531     66:0F6EC1                  MOVD MM0,ECX
	$+156    008AF535     8D5424 14                  LEA EDX,DWORD PTR SS:[ESP+0x14]
	$+15A    008AF539     0F5BC0                     CVTDQ2PS XMM0,XMM0
	$+15D    008AF53C     8D4C24 0C                  LEA ECX,DWORD PTR SS:[ESP+0xC]
	$+161    008AF540     F3:0F114424 14             MOVSS DWORD PTR SS:[ESP+0x14],XMM0
	$+167    008AF546     F3:0F1086 7C040000         MOVSS XMM0,DWORD PTR DS:[ESI+0x47C]
	$+16F    008AF54E     F3:0F594424 0C             MULSS XMM0,DWORD PTR SS:[ESP+0xC]
	$+175    008AF554     F3:0F114424 0C             MOVSS DWORD PTR SS:[ESP+0xC],XMM0
	$+17B    008AF55A     E8 A1CFC0FF                CALL PathOfEx.004BC500
	$+180    008AF55F     5F                         POP EDI
	$+181    008AF560     5E                         POP ESI
	$+182    008AF561     5B                         POP EBX
	$+183    008AF562     F3:0F1000                  MOVSS XMM0,DWORD PTR DS:[EAX]
	$+187    008AF566     F3:0F114424 08             MOVSS DWORD PTR SS:[ESP+0x8],XMM0
	$+18D    008AF56C     D94424 08                  FLD DWORD PTR SS:[ESP+0x8]
	$+191    008AF570     59                         POP ECX
	$+192    008AF571     C2 0400                    RETN 0x4
	$+195    008AF574     F3:0F1086 7C040000         MOVSS XMM0,DWORD PTR DS:[ESI+0x47C]
	$+19D    008AF57C     F3:0F594424 0C             MULSS XMM0,DWORD PTR SS:[ESP+0xC]
	$+1A3    008AF582     5F                         POP EDI
	$+1A4    008AF583     5E                         POP ESI
	$+1A5    008AF584     5B                         POP EBX
	$+1A6    008AF585     F3:0F114424 08             MOVSS DWORD PTR SS:[ESP+0x8],XMM0
	$+1AC    008AF58B     D94424 08                  FLD DWORD PTR SS:[ESP+0x8]
	$+1B0    008AF58F     59                         POP ECX
	$+1B1    008AF590     C2 0400                    RETN 0x4
	$+1B4    008AF593     CC                         INT3
	$+1B5    008AF594     CC                         INT3
	$+1B6    008AF595     CC                         INT3
	$+1B7    008AF596     CC                         INT3
	$+1B8    008AF597     CC                         INT3
	$+1B9    008AF598     CC                         INT3
	$+1BA    008AF599     CC                         INT3
	$+1BB    008AF59A     CC                         INT3
	$+1BC    008AF59B     CC                         INT3
	$+1BD    008AF59C     CC                         INT3
	$+1BE    008AF59D     CC                         INT3
	$+1BF    008AF59E     CC                         INT3
	$+1C0    008AF59F     CC                         INT3
	$+1C1    008AF5A0     D981 7C040000              FLD DWORD PTR DS:[ECX+0x47C]
	$+1C7    008AF5A6     C3                         RETN
	$+1C8    008AF5A7     CC                         INT3
	$+1C9    008AF5A8     CC                         INT3
	$+1CA    008AF5A9     CC                         INT3
	$+1CB    008AF5AA     CC                         INT3
	$+1CC    008AF5AB     CC                         INT3
	$+1CD    008AF5AC     CC                         INT3
	$+1CE    008AF5AD     CC                         INT3
	$+1CF    008AF5AE     CC                         INT3
	$+1D0    008AF5AF     CC                         INT3
	$+1D1    008AF5B0     55                         PUSH EBP
	$+1D2    008AF5B1     8BEC                       MOV EBP,ESP
	$+1D4    008AF5B3     6A FF                      PUSH -0x1
	$+1D6    008AF5B5     68 269AF700                PUSH PathOfEx.00F79A26
	$+1DB    008AF5BA     64:A1 00000000             MOV EAX,DWORD PTR FS:[0]
	$+1E1    008AF5C0     50                         PUSH EAX
	$+1E2    008AF5C1     64:8925 00000000           MOV DWORD PTR FS:[0],ESP
	$+1E9    008AF5C8     51                         PUSH ECX
	$+1EA    008AF5C9     53                         PUSH EBX
	$+1EB    008AF5CA     57                         PUSH EDI
	$+1EC    008AF5CB     8BF9                       MOV EDI,ECX
	$+1EE    008AF5CD     897D F0                    MOV DWORD PTR SS:[EBP-0x10],EDI
	$+1F1    008AF5D0     8B45 0C                    MOV EAX,DWORD PTR SS:[EBP+0xC]
	$+1F4    008AF5D3     8947 04                    MOV DWORD PTR DS:[EDI+0x4],EAX
	$+1F7    008AF5D6     C647 08 00                 MOV BYTE PTR DS:[EDI+0x8],0x0
	$+1FB    008AF5DA     C745 FC 00000000           MOV DWORD PTR SS:[EBP-0x4],0x0
	$+202    008AF5E1     8947 10                    MOV DWORD PTR DS:[EDI+0x10],EAX
	$+205    008AF5E4     8B45 08                    MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+208    008AF5E7     C787 54040000 00000000     MOV DWORD PTR DS:[EDI+0x454],0x0
	$+212    008AF5F1     C787 58040000 00000000     MOV DWORD PTR DS:[EDI+0x458],0x0
	$+21C    008AF5FB     C787 5C040000 00000000     MOV DWORD PTR DS:[EDI+0x45C],0x0
	$+226    008AF605     C787 60040000 01000000     MOV DWORD PTR DS:[EDI+0x460],0x1
	$+230    008AF60F     C787 64040000 01000000     MOV DWORD PTR DS:[EDI+0x464],0x1
	$+23A    008AF619     66:C787 68040000 0000      MOV WORD PTR DS:[EDI+0x468],0x0
	$+243    008AF622     C747 0C 44830F01           MOV DWORD PTR DS:[EDI+0xC],PathOfEx.010F8344
	$+24A    008AF629     C707 847A1501              MOV DWORD PTR DS:[EDI],PathOfEx.01157A84
	$+250    008AF62F     C747 0C B87A1501           MOV DWORD PTR DS:[EDI+0xC],PathOfEx.01157AB8
	$+257    008AF636     8987 6C040000              MOV DWORD PTR DS:[EDI+0x46C],EAX
	$+25D    008AF63C     C787 70040000 00000000     MOV DWORD PTR DS:[EDI+0x470],0x0
	$+267    008AF646     C787 74040000 00000000     MOV DWORD PTR DS:[EDI+0x474],0x0
	$+271    008AF650     C787 78040000 00000000     MOV DWORD PTR DS:[EDI+0x478],0x0
	$+27B    008AF65A     C645 FC 01                 MOV BYTE PTR SS:[EBP-0x4],0x1
	$+27F    008AF65E     66:C787 80040000 0000      MOV WORD PTR DS:[EDI+0x480],0x0
	$+288    008AF667     C787 84040000 00000000     MOV DWORD PTR DS:[EDI+0x484],0x0
	$+292    008AF671     C787 88040000 00000000     MOV DWORD PTR DS:[EDI+0x488],0x0
	$+29C    008AF67B     8B5F 04                    MOV EBX,DWORD PTR DS:[EDI+0x4]
	$+29F    008AF67E     C787 8C040000 00000000     MOV DWORD PTR DS:[EDI+0x48C],0x0
	$+2A9    008AF688     C787 90040000 00000000     MOV DWORD PTR DS:[EDI+0x490],0x0
	$+2B3    008AF692     C787 94040000 00401C46     MOV DWORD PTR DS:[EDI+0x494],0x461C4000
	$+2BD    008AF69C     66:0F6E40 10               MOVD MM0,DWORD PTR DS:[EAX+0x10]
	$+2C2    008AF6A1     0F5BC0                     CVTDQ2PS XMM0,XMM0
	$+2C5    008AF6A4     F3:0F1187 7C040000         MOVSS DWORD PTR DS:[EDI+0x47C],XMM0
	$+2CD    008AF6AC     8B0B                       MOV ECX,DWORD PTR DS:[EBX]
	$+2CF    008AF6AE     85C9                       TEST ECX,ECX
	$+2D1    008AF6B0     74 05                      JE SHORT PathOfEx.008AF6B7
	$+2D3    008AF6B2     E8 B97EF0FF                CALL PathOfEx.007B7570
	$+2D8    008AF6B7     8B03                       MOV EAX,DWORD PTR DS:[EBX]
	$+2DA    008AF6B9     56                         PUSH ESI
	$+2DB    008AF6BA     C745 0C 60E10B01           MOV DWORD PTR SS:[EBP+0xC],PathOfEx.010BE160    ; ASCII "Positioned"
	$+2E2    008AF6C1     8B70 2C                    MOV ESI,DWORD PTR DS:[EAX+0x2C]
	$+2E5    008AF6C4     8D45 0C                    LEA EAX,DWORD PTR SS:[EBP+0xC]
	$+2E8    008AF6C7     50                         PUSH EAX
	$+2E9    008AF6C8     8D45 08                    LEA EAX,DWORD PTR SS:[EBP+0x8]
	$+2EC    008AF6CB     50                         PUSH EAX
	$+2ED    008AF6CC     8D4E 14                    LEA ECX,DWORD PTR DS:[ESI+0x14]
	$+2F0    008AF6CF     E8 DCD3BCFF                CALL PathOfEx.0047CAB0
	$+2F5    008AF6D4     8B45 08                    MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+2F8    008AF6D7     3B46 18                    CMP EAX,DWORD PTR DS:[ESI+0x18]
	$+2FB    008AF6DA     5E                         POP ESI
	$+2FC    008AF6DB     74 10                      JE SHORT PathOfEx.008AF6ED
	$+2FE    008AF6DD     8B48 0C                    MOV ECX,DWORD PTR DS:[EAX+0xC]
	$+301    008AF6E0     83F9 FF                    CMP ECX,-0x1
	$+304    008AF6E3     74 08                      JE SHORT PathOfEx.008AF6ED
	$+306    008AF6E5     8B43 04                    MOV EAX,DWORD PTR DS:[EBX+0x4]
	$+309    008AF6E8     8B0C88                     MOV ECX,DWORD PTR DS:[EAX+ECX*4]
	$+30C    008AF6EB     EB 02                      JMP SHORT PathOfEx.008AF6EF
	$+30E    008AF6ED     33C9                       XOR ECX,ECX
	$+310    008AF6EF     8D47 0C                    LEA EAX,DWORD PTR DS:[EDI+0xC]
	$+313    008AF6F2     81C1 EC000000              ADD ECX,0xEC
	$+319    008AF6F8     8945 0C                    MOV DWORD PTR SS:[EBP+0xC],EAX
	$+31C    008AF6FB     8D45 0C                    LEA EAX,DWORD PTR SS:[EBP+0xC]
	$+31F    008AF6FE     50                         PUSH EAX
	$+320    008AF6FF     E8 CCC8BCFF                CALL PathOfEx.0047BFD0
	$+325    008AF704     8B4D F4                    MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+328    008AF707     8BC7                       MOV EAX,EDI
	$+32A    008AF709     5F                         POP EDI
	$+32B    008AF70A     5B                         POP EBX
	$+32C    008AF70B     64:890D 00000000           MOV DWORD PTR FS:[0],ECX
	$+333    008AF712     8BE5                       MOV ESP,EBP
	$+335    008AF714     5D                         POP EBP
	$+336    008AF715     C2 0800                    RETN 0x8
	$+339    008AF718     CC                         INT3
	*/
#ifdef VER_GJ3_
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("c6????????????8b??8b??48c6????????????5089??????????e8", 2);
	if (!op_dw_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *op_dw_tmp;
#elif VER_GF_
	op_dw_tmp = scan_exe_.ScanImme<pt_dword>("80????????????f3??????????????f3??????????????0f??????????e8????????84??74", 2);
	if (!op_dw_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *op_dw_tmp;
#endif
	assert(dw_tmp < 0x500);
	ofs << "pt_cdword	g_o_is_moving_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;

	/*
	$-3E5    0073AD5F     CC                    INT3
	$-3E4    0073AD60     55                    PUSH EBP
	$-3E3    0073AD61     8BEC                  MOV EBP,ESP
	$-3E1    0073AD63     6A FF                 PUSH -0x1
	$-3DF    0073AD65     68 7268F100           PUSH PathOfEx.00F16872
	$-3DA    0073AD6A     64:A1 00000000        MOV EAX,DWORD PTR FS:[0]
	$-3D4    0073AD70     50                    PUSH EAX
	$-3D3    0073AD71     64:8925 00000000      MOV DWORD PTR FS:[0],ESP
	$-3CC    0073AD78     83EC 38               SUB ESP,0x38
	$-3C9    0073AD7B     53                    PUSH EBX
	$-3C8    0073AD7C     56                    PUSH ESI
	$-3C7    0073AD7D     57                    PUSH EDI
	$-3C6    0073AD7E     8BF9                  MOV EDI,ECX
	$-3C4    0073AD80     8D4D E4               LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$-3C1    0073AD83     E8 C89DD4FF           CALL PathOfEx.00484B50
	$-3BC    0073AD88     51                    PUSH ECX
	$-3BB    0073AD89     8D4D EC               LEA ECX,DWORD PTR SS:[EBP-0x14]
	$-3B8    0073AD8C     51                    PUSH ECX
	$-3B7    0073AD8D     8B08                  MOV ECX,DWORD PTR DS:[EAX]
	$-3B5    0073AD8F     E8 BC0FD5FF           CALL PathOfEx.0048BD50
	$-3B0    0073AD94     C745 FC 01000000      MOV DWORD PTR SS:[EBP-0x4],0x1
	$-3A9    0073AD9B     83CB FF               OR EBX,0xFFFFFFFF
	$-3A6    0073AD9E     8B75 E8               MOV ESI,DWORD PTR SS:[EBP-0x18]
	$-3A3    0073ADA1     85F6                  TEST ESI,ESI
	$-3A1    0073ADA3     74 1F                 JE SHORT PathOfEx.0073ADC4
	$-39F    0073ADA5     8BC3                  MOV EAX,EBX
	$-39D    0073ADA7     F0:0FC146 04          LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
	$-398    0073ADAC     75 16                 JNZ SHORT PathOfEx.0073ADC4
	$-396    0073ADAE     8B06                  MOV EAX,DWORD PTR DS:[ESI]
	$-394    0073ADB0     8BCE                  MOV ECX,ESI
	$-392    0073ADB2     FF10                  CALL DWORD PTR DS:[EAX]
	$-390    0073ADB4     8BC3                  MOV EAX,EBX
	$-38E    0073ADB6     F0:0FC146 08          LOCK XADD DWORD PTR DS:[ESI+0x8],EAX
	$-389    0073ADBB     75 07                 JNZ SHORT PathOfEx.0073ADC4
	$-387    0073ADBD     8B06                  MOV EAX,DWORD PTR DS:[ESI]
	$-385    0073ADBF     8BCE                  MOV ECX,ESI
	$-383    0073ADC1     FF50 04               CALL DWORD PTR DS:[EAX+0x4]
	$-380    0073ADC4     C645 FC 00            MOV BYTE PTR SS:[EBP-0x4],0x0
	$-37C    0073ADC8     8B75 EC               MOV ESI,DWORD PTR SS:[EBP-0x14]
	$-379    0073ADCB     8B8E A00B0000         MOV ECX,DWORD PTR DS:[ESI+0xBA0]
	$-373    0073ADD1     85C9                  TEST ECX,ECX
	$-371    0073ADD3     74 07                 JE SHORT PathOfEx.0073ADDC
	$-36F    0073ADD5     6A 01                 PUSH 0x1
	$-36D    0073ADD7     E8 D4A93700           CALL PathOfEx.00AB57B0
	$-368    0073ADDC     8B4F 20               MOV ECX,DWORD PTR DS:[EDI+0x20]
	$-365    0073ADDF     6A 00                 PUSH 0x0
	$-363    0073ADE1     68 30B27300           PUSH PathOfEx.0073B230
	$-35E    0073ADE6     57                    PUSH EDI
	$-35D    0073ADE7     81C1 80080000         ADD ECX,0x880
	$-357    0073ADED     C647 5E 00            MOV BYTE PTR DS:[EDI+0x5E],0x0
	$-353    0073ADF1     C647 5C 00            MOV BYTE PTR DS:[EDI+0x5C],0x0
	$-34F    0073ADF5     E8 8672D7FF           CALL PathOfEx.004B2080
	$-34A    0073ADFA     8B4F 24               MOV ECX,DWORD PTR DS:[EDI+0x24]
	$-347    0073ADFD     8D45 BC               LEA EAX,DWORD PTR SS:[EBP-0x44]
	$-344    0073AE00     81C1 04070000         ADD ECX,0x704
	$-33E    0073AE06     C745 BC 8CA01401      MOV DWORD PTR SS:[EBP-0x44],PathOfEx.0114A08C   ; ASCII "p臬"
	$-337    0073AE0D     8945 E0               MOV DWORD PTR SS:[EBP-0x20],EAX
	$-334    0073AE10     51                    PUSH ECX
	$-333    0073AE11     50                    PUSH EAX
	$-332    0073AE12     C645 FC 02            MOV BYTE PTR SS:[EBP-0x4],0x2
	$-32E    0073AE16     E8 E53D3800           CALL PathOfEx.00ABEC00
	$-329    0073AE1B     C645 FC 03            MOV BYTE PTR SS:[EBP-0x4],0x3
	$-325    0073AE1F     8B4D E0               MOV ECX,DWORD PTR SS:[EBP-0x20]
	$-322    0073AE22     85C9                  TEST ECX,ECX
	$-320    0073AE24     74 11                 JE SHORT PathOfEx.0073AE37
	$-31E    0073AE26     8B11                  MOV EDX,DWORD PTR DS:[ECX]
	$-31C    0073AE28     8D45 BC               LEA EAX,DWORD PTR SS:[EBP-0x44]
	$-319    0073AE2B     3BC8                  CMP ECX,EAX
	$-317    0073AE2D     0F95C0                SETNE AL
	$-314    0073AE30     0FB6C0                MOVZX EAX,AL
	$-311    0073AE33     50                    PUSH EAX
	$-310    0073AE34     FF52 10               CALL DWORD PTR DS:[EDX+0x10]
	$-30D    0073AE37     C645 FC 00            MOV BYTE PTR SS:[EBP-0x4],0x0
	$-309    0073AE3B     8B57 38               MOV EDX,DWORD PTR DS:[EDI+0x38]
	$-306    0073AE3E     85D2                  TEST EDX,EDX
	$-304    0073AE40     74 1C                 JE SHORT PathOfEx.0073AE5E
	$-302    0073AE42     8B47 20               MOV EAX,DWORD PTR DS:[EDI+0x20]
	$-2FF    0073AE45     52                    PUSH EDX
	$-2FE    0073AE46     8B88 A40A0000         MOV ECX,DWORD PTR DS:[EAX+0xAA4]
	$-2F8    0073AE4C     81C1 04070000         ADD ECX,0x704
	$-2F2    0073AE52     E8 B9393800           CALL PathOfEx.00ABE810
	$-2ED    0073AE57     C747 38 00000000      MOV DWORD PTR DS:[EDI+0x38],0x0
	$-2E6    0073AE5E     8B4F 4C               MOV ECX,DWORD PTR DS:[EDI+0x4C]
	$-2E3    0073AE61     B8 398EE338           MOV EAX,0x38E38E39
	$-2DE    0073AE66     2B4F 48               SUB ECX,DWORD PTR DS:[EDI+0x48]
	$-2DB    0073AE69     8B76 38               MOV ESI,DWORD PTR DS:[ESI+0x38]
	$-2D8    0073AE6C     F7E9                  IMUL ECX
	$-2D6    0073AE6E     8BCE                  MOV ECX,ESI
	$-2D4    0073AE70     C1FA 04               SAR EDX,0x4
	$-2D1    0073AE73     8BC2                  MOV EAX,EDX
	$-2CF    0073AE75     C1E8 1F               SHR EAX,0x1F
	$-2CC    0073AE78     03C2                  ADD EAX,EDX
	$-2CA    0073AE7A     8B16                  MOV EDX,DWORD PTR DS:[ESI]
	$-2C8    0073AE7C     0F95C0                SETNE AL
	$-2C5    0073AE7F     0FB6C0                MOVZX EAX,AL
	$-2C2    0073AE82     50                    PUSH EAX
	$-2C1    0073AE83     FF52 34               CALL DWORD PTR DS:[EDX+0x34]
	$-2BE    0073AE86     83C7 60               ADD EDI,0x60
	$-2BB    0073AE89     C747 10 00000000      MOV DWORD PTR DS:[EDI+0x10],0x0
	$-2B4    0073AE90     837F 14 08            CMP DWORD PTR DS:[EDI+0x14],0x8
	$-2B0    0073AE94     72 02                 JB SHORT PathOfEx.0073AE98
	$-2AE    0073AE96     8B3F                  MOV EDI,DWORD PTR DS:[EDI]
	$-2AC    0073AE98     33C0                  XOR EAX,EAX
	$-2AA    0073AE9A     66:8907               MOV WORD PTR DS:[EDI],AX
	$-2A7    0073AE9D     C745 FC 04000000      MOV DWORD PTR SS:[EBP-0x4],0x4
	$-2A0    0073AEA4     8B75 F0               MOV ESI,DWORD PTR SS:[EBP-0x10]
	$-29D    0073AEA7     85F6                  TEST ESI,ESI
	$-29B    0073AEA9     74 1E                 JE SHORT PathOfEx.0073AEC9
	$-299    0073AEAB     8BC3                  MOV EAX,EBX
	$-297    0073AEAD     F0:0FC146 04          LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
	$-292    0073AEB2     75 15                 JNZ SHORT PathOfEx.0073AEC9
	$-290    0073AEB4     8B06                  MOV EAX,DWORD PTR DS:[ESI]
	$-28E    0073AEB6     8BCE                  MOV ECX,ESI
	$-28C    0073AEB8     FF10                  CALL DWORD PTR DS:[EAX]
	$-28A    0073AEBA     F0:0FC15E 08          LOCK XADD DWORD PTR DS:[ESI+0x8],EBX
	$-285    0073AEBF     4B                    DEC EBX
	$-284    0073AEC0     75 07                 JNZ SHORT PathOfEx.0073AEC9
	$-282    0073AEC2     8B06                  MOV EAX,DWORD PTR DS:[ESI]
	$-280    0073AEC4     8BCE                  MOV ECX,ESI
	$-27E    0073AEC6     FF50 04               CALL DWORD PTR DS:[EAX+0x4]
	$-27B    0073AEC9     8B4D F4               MOV ECX,DWORD PTR SS:[EBP-0xC]
	$-278    0073AECC     5F                    POP EDI
	$-277    0073AECD     5E                    POP ESI
	$-276    0073AECE     5B                    POP EBX
	$-275    0073AECF     64:890D 00000000      MOV DWORD PTR FS:[0],ECX
	$-26E    0073AED6     8BE5                  MOV ESP,EBP
	$-26C    0073AED8     5D                    POP EBP
	$-26B    0073AED9     C3                    RETN
	$-26A    0073AEDA     CC                    INT3
	$-269    0073AEDB     CC                    INT3
	$-268    0073AEDC     CC                    INT3
	$-267    0073AEDD     CC                    INT3
	$-266    0073AEDE     CC                    INT3
	$-265    0073AEDF     CC                    INT3
	$-264    0073AEE0     8B49 34               MOV ECX,DWORD PTR DS:[ECX+0x34]
	$-261    0073AEE3     6A 00                 PUSH 0x0
	$-25F    0073AEE5     E8 C6A83700           CALL PathOfEx.00AB57B0
	$-25A    0073AEEA     C3                    RETN
	$-259    0073AEEB     CC                    INT3
	$-258    0073AEEC     CC                    INT3
	$-257    0073AEED     CC                    INT3
	$-256    0073AEEE     CC                    INT3
	$-255    0073AEEF     CC                    INT3
	$-254    0073AEF0     8079 5D 00            CMP BYTE PTR DS:[ECX+0x5D],0x0
	$-250    0073AEF4     74 38                 JE SHORT PathOfEx.0073AF2E
	$-24E    0073AEF6     8079 5F 00            CMP BYTE PTR DS:[ECX+0x5F],0x0
	$-24A    0073AEFA     74 32                 JE SHORT PathOfEx.0073AF2E
	$-248    0073AEFC     56                    PUSH ESI
	$-247    0073AEFD     8B71 34               MOV ESI,DWORD PTR DS:[ECX+0x34]
	$-244    0073AF00     80BE 54070000 01      CMP BYTE PTR DS:[ESI+0x754],0x1
	$-23D    0073AF07     74 24                 JE SHORT PathOfEx.0073AF2D
	$-23B    0073AF09     8B06                  MOV EAX,DWORD PTR DS:[ESI]
	$-239    0073AF0B     8BCE                  MOV ECX,ESI
	$-237    0073AF0D     C686 54070000 01      MOV BYTE PTR DS:[ESI+0x754],0x1
	$-230    0073AF14     FF90 B0000000         CALL DWORD PTR DS:[EAX+0xB0]
	$-22A    0073AF1A     8B8E 5C070000         MOV ECX,DWORD PTR DS:[ESI+0x75C]
	$-224    0073AF20     85C9                  TEST ECX,ECX
	$-222    0073AF22     74 09                 JE SHORT PathOfEx.0073AF2D
	$-220    0073AF24     8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$-21E    0073AF26     56                    PUSH ESI
	$-21D    0073AF27     FF90 D0000000         CALL DWORD PTR DS:[EAX+0xD0]
	$-217    0073AF2D     5E                    POP ESI
	$-216    0073AF2E     C3                    RETN
	$-215    0073AF2F     CC                    INT3
	$-214    0073AF30     55                    PUSH EBP
	$-213    0073AF31     8BEC                  MOV EBP,ESP
	$-211    0073AF33     64:A1 00000000        MOV EAX,DWORD PTR FS:[0]
	$-20B    0073AF39     6A FF                 PUSH -0x1
	$-209    0073AF3B     68 5CBEF100           PUSH PathOfEx.00F1BE5C
	$-204    0073AF40     50                    PUSH EAX
	$-203    0073AF41     64:8925 00000000      MOV DWORD PTR FS:[0],ESP
	$-1FC    0073AF48     83EC 38               SUB ESP,0x38
	$-1F9    0073AF4B     56                    PUSH ESI
	$-1F8    0073AF4C     57                    PUSH EDI
	$-1F7    0073AF4D     8BF9                  MOV EDI,ECX
	$-1F5    0073AF4F     807F 30 00            CMP BYTE PTR DS:[EDI+0x30],0x0
	$-1F1    0073AF53     0F85 3E020000         JNZ PathOfEx.0073B197
	$-1EB    0073AF59     8B47 28               MOV EAX,DWORD PTR DS:[EDI+0x28]
	$-1E8    0073AF5C     85C0                  TEST EAX,EAX
	$-1E6    0073AF5E     0F84 33020000         JE PathOfEx.0073B197
	$-1E0    0073AF64     83B8 34080000 00      CMP DWORD PTR DS:[EAX+0x834],0x0
	$-1D9    0073AF6B     0F84 26020000         JE PathOfEx.0073B197
	$-1D3    0073AF71     807F 5E 00            CMP BYTE PTR DS:[EDI+0x5E],0x0
	$-1CF    0073AF75     0F84 30010000         JE PathOfEx.0073B0AB
	$-1C9    0073AF7B     8B4F 34               MOV ECX,DWORD PTR DS:[EDI+0x34]
	$-1C6    0073AF7E     85C9                  TEST ECX,ECX
	$-1C4    0073AF80     0F84 25010000         JE PathOfEx.0073B0AB
	$-1BE    0073AF86     53                    PUSH EBX
	$-1BD    0073AF87     8D5F 48               LEA EBX,DWORD PTR DS:[EDI+0x48]
	$-1BA    0073AF8A     66:C747 5E 0001       MOV WORD PTR DS:[EDI+0x5E],0x100
	$-1B4    0073AF90     53                    PUSH EBX
	$-1B3    0073AF91     E8 DA9BEEFF           CALL PathOfEx.00624B70
	$-1AE    0073AF96     8B4B 04               MOV ECX,DWORD PTR DS:[EBX+0x4]
	$-1AB    0073AF99     B8 398EE338           MOV EAX,0x38E38E39
	$-1A6    0073AF9E     2B0B                  SUB ECX,DWORD PTR DS:[EBX]
	$-1A4    0073AFA0     F7E9                  IMUL ECX
	$-1A2    0073AFA2     C1FA 04               SAR EDX,0x4
	$-19F    0073AFA5     8BC2                  MOV EAX,EDX
	$-19D    0073AFA7     C1E8 1F               SHR EAX,0x1F
	$-19A    0073AFAA     03C2                  ADD EAX,EDX
	$-198    0073AFAC     0F85 8E000000         JNZ PathOfEx.0073B040
	$-192    0073AFB2     8D4D DC               LEA ECX,DWORD PTR SS:[EBP-0x24]
	$-18F    0073AFB5     E8 969BD4FF           CALL PathOfEx.00484B50
	$-18A    0073AFBA     51                    PUSH ECX
	$-189    0073AFBB     8D4D EC               LEA ECX,DWORD PTR SS:[EBP-0x14]
	$-186    0073AFBE     51                    PUSH ECX
	$-185    0073AFBF     8B08                  MOV ECX,DWORD PTR DS:[EAX]
	$-183    0073AFC1     E8 5AE8FFFF           CALL PathOfEx.00739820
	$-17E    0073AFC6     8D4D DC               LEA ECX,DWORD PTR SS:[EBP-0x24]
	$-17B    0073AFC9     C745 FC 03000000      MOV DWORD PTR SS:[EBP-0x4],0x3
	$-174    0073AFD0     E8 CBE1D3FF           CALL PathOfEx.004791A0
	$-16F    0073AFD5     8D45 EC               LEA EAX,DWORD PTR SS:[EBP-0x14]
	$-16C    0073AFD8     50                    PUSH EAX
	$-16B    0073AFD9     8D4D E4               LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$-168    0073AFDC     E8 EF18D5FF           CALL PathOfEx.0048C8D0
	$-163    0073AFE1     8D4D D4               LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$-160    0073AFE4     C645 FC 04            MOV BYTE PTR SS:[EBP-0x4],0x4
	$-15C    0073AFE8     E8 639BD4FF           CALL PathOfEx.00484B50
	$-157    0073AFED     8D4D E4               LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$-154    0073AFF0     C645 FC 05            MOV BYTE PTR SS:[EBP-0x4],0x5
	$-150    0073AFF4     51                    PUSH ECX
	$-14F    0073AFF5     8B08                  MOV ECX,DWORD PTR DS:[EAX]
	$-14D    0073AFF7     E8 847EFEFF           CALL PathOfEx.00722E80
	$-148    0073AFFC     8D4D D4               LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$-145    0073AFFF     E8 9CE1D3FF           CALL PathOfEx.004791A0
	$-140    0073B004     8D4D E4               LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$-13D    0073B007     C645 FC 03            MOV BYTE PTR SS:[EBP-0x4],0x3
	$-139    0073B00B     E8 90E1D3FF           CALL PathOfEx.004791A0
	$-134    0073B010     8B4F 34               MOV ECX,DWORD PTR DS:[EDI+0x34]                 ; g_o_ui_select_role_top_
	$-131    0073B013     8D45 BC               LEA EAX,DWORD PTR SS:[EBP-0x44]
	$-12E    0073B016     50                    PUSH EAX
	$-12D    0073B017     E8 14A2EEFF           CALL PathOfEx.00625230
	$-128    0073B01C     C645 FC 06            MOV BYTE PTR SS:[EBP-0x4],0x6
	$-124    0073B020     8B4D EC               MOV ECX,DWORD PTR SS:[EBP-0x14]
	$-121    0073B023     50                    PUSH EAX
	$-120    0073B024     E8 C7FCFEFF           CALL PathOfEx.0072ACF0
	$-11B    0073B029     8D4D BC               LEA ECX,DWORD PTR SS:[EBP-0x44]
	$-118    0073B02C     E8 EF0ED4FF           CALL PathOfEx.0047BF20
	$-113    0073B031     8D4D EC               LEA ECX,DWORD PTR SS:[EBP-0x14]
	$-110    0073B034     C745 FC FFFFFFFF      MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-109    0073B03B     E8 60E1D3FF           CALL PathOfEx.004791A0
	$-104    0073B040     8D4D D4               LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$-101    0073B043     E8 089BD4FF           CALL PathOfEx.00484B50
	$-FC     0073B048     51                    PUSH ECX
	$-FB     0073B049     8D4D E4               LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$-F8     0073B04C     51                    PUSH ECX
	$-F7     0073B04D     8B08                  MOV ECX,DWORD PTR DS:[EAX]
	$-F5     0073B04F     E8 FC0CD5FF           CALL PathOfEx.0048BD50
	$-F0     0073B054     8D4D D4               LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$-ED     0073B057     C745 FC 07000000      MOV DWORD PTR SS:[EBP-0x4],0x7
	$-E6     0073B05E     E8 3DE1D3FF           CALL PathOfEx.004791A0
	$-E1     0073B063     8B75 E4               MOV ESI,DWORD PTR SS:[EBP-0x1C]
	$-DE     0073B066     8B4E 38               MOV ECX,DWORD PTR DS:[ESI+0x38]
	$-DB     0073B069     8B01                  MOV EAX,DWORD PTR DS:[ECX]
	$-D9     0073B06B     8B40 38               MOV EAX,DWORD PTR DS:[EAX+0x38]
	$-D6     0073B06E     FFD0                  CALL EAX
	$-D4     0073B070     84C0                  TEST AL,AL
	$-D2     0073B072     74 27                 JE SHORT PathOfEx.0073B09B
	$-D0     0073B074     8B4B 04               MOV ECX,DWORD PTR DS:[EBX+0x4]
	$-CD     0073B077     B8 398EE338           MOV EAX,0x38E38E39
	$-C8     0073B07C     2B0B                  SUB ECX,DWORD PTR DS:[EBX]
	$-C6     0073B07E     8B76 38               MOV ESI,DWORD PTR DS:[ESI+0x38]
	$-C3     0073B081     F7E9                  IMUL ECX
	$-C1     0073B083     8BCE                  MOV ECX,ESI
	$-BF     0073B085     C1FA 04               SAR EDX,0x4
	$-BC     0073B088     8BC2                  MOV EAX,EDX
	$-BA     0073B08A     C1E8 1F               SHR EAX,0x1F
	$-B7     0073B08D     03C2                  ADD EAX,EDX
	$-B5     0073B08F     8B16                  MOV EDX,DWORD PTR DS:[ESI]
	$-B3     0073B091     0F95C0                SETNE AL
	$-B0     0073B094     0FB6C0                MOVZX EAX,AL
	$-AD     0073B097     50                    PUSH EAX
	$-AC     0073B098     FF52 2C               CALL DWORD PTR DS:[EDX+0x2C]
	$-A9     0073B09B     8D4D E4               LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$-A6     0073B09E     C745 FC FFFFFFFF      MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-9F     0073B0A5     E8 F6E0D3FF           CALL PathOfEx.004791A0
	$-9A     0073B0AA     5B                    POP EBX
	$-99     0073B0AB     807F 5C 00            CMP BYTE PTR DS:[EDI+0x5C],0x0
	$-95     0073B0AF     0F84 8F000000         JE PathOfEx.0073B144
	$-8F     0073B0B5     8B4F 34               MOV ECX,DWORD PTR DS:[EDI+0x34]                 ; g_o_ui_select_role_top_
	$-8C     0073B0B8     85C9                  TEST ECX,ECX
	$-8A     0073B0BA     0F84 84000000         JE PathOfEx.0073B144
	$-84     0073B0C0     8D77 3C               LEA ESI,DWORD PTR DS:[EDI+0x3C]
	$-81     0073B0C3     66:C747 5C 0001       MOV WORD PTR DS:[EDI+0x5C],0x100
	$-7B     0073B0C9     56                    PUSH ESI
	$-7A     0073B0CA     E8 2191EEFF           CALL PathOfEx.006241F0
	$-75     0073B0CF     8D4D D4               LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$-72     0073B0D2     E8 799AD4FF           CALL PathOfEx.00484B50
	$-6D     0073B0D7     51                    PUSH ECX
	$-6C     0073B0D8     8D4D EC               LEA ECX,DWORD PTR SS:[EBP-0x14]
	$-69     0073B0DB     51                    PUSH ECX
	$-68     0073B0DC     8B08                  MOV ECX,DWORD PTR DS:[EAX]
	$-66     0073B0DE     E8 3DE7FFFF           CALL PathOfEx.00739820
	$-61     0073B0E3     8D4D D4               LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$-5E     0073B0E6     C745 FC 08000000      MOV DWORD PTR SS:[EBP-0x4],0x8
	$-57     0073B0ED     E8 AEE0D3FF           CALL PathOfEx.004791A0
	$-52     0073B0F2     8D47 60               LEA EAX,DWORD PTR DS:[EDI+0x60]
	$-4F     0073B0F5     8BCF                  MOV ECX,EDI
	$-4D     0073B0F7     50                    PUSH EAX
	$-4C     0073B0F8     E8 F30F0000           CALL PathOfEx.0073C0F0
	$-47     0073B0FD     8B4E 04               MOV ECX,DWORD PTR DS:[ESI+0x4]
	$-44     0073B100     2B0E                  SUB ECX,DWORD PTR DS:[ESI]
	$-42     0073B102     C1F9 07               SAR ECX,0x7
	$-3F     0073B105     3BC1                  CMP EAX,ECX
	$-3D     0073B107     73 0B                 JNB SHORT PathOfEx.0073B114
	$-3B     0073B109     8B4D EC               MOV ECX,DWORD PTR SS:[EBP-0x14]
	$-38     0073B10C     50                    PUSH EAX
	$-37     0073B10D     E8 DEFCFEFF           CALL PathOfEx.0072ADF0
	$-32     0073B112     EB 21                 JMP SHORT PathOfEx.0073B135
	$-30     0073B114     8B4F 34               MOV ECX,DWORD PTR DS:[EDI+0x34]                 ; g_o_ui_select_role_top_
	$-2D     0073B117     8D45 BC               LEA EAX,DWORD PTR SS:[EBP-0x44]
	$-2A     0073B11A     50                    PUSH EAX
	$-29     0073B11B     E8 10A1EEFF           CALL PathOfEx.00625230
	$-24     0073B120     C645 FC 09            MOV BYTE PTR SS:[EBP-0x4],0x9
	$-20     0073B124     8B4D EC               MOV ECX,DWORD PTR SS:[EBP-0x14]
	$-1D     0073B127     50                    PUSH EAX
	$-1C     0073B128     E8 C3FBFEFF           CALL PathOfEx.0072ACF0
	$-17     0073B12D     8D4D BC               LEA ECX,DWORD PTR SS:[EBP-0x44]
	$-14     0073B130     E8 EB0DD4FF           CALL PathOfEx.0047BF20
	$-F      0073B135     8D4D EC               LEA ECX,DWORD PTR SS:[EBP-0x14]
	$-C      0073B138     C745 FC FFFFFFFF      MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-5      0073B13F     E8 5CE0D3FF           CALL PathOfEx.004791A0
	$ ==>    0073B144     8B4F 34               MOV ECX,DWORD PTR DS:[EDI+0x34]                 ; g_o_ui_select_role_top_
	$+3      0073B147     85C9                  TEST ECX,ECX
	$+5      0073B149     0F84 C4000000         JE PathOfEx.0073B213
	$+B      0073B14F     E8 4CA73700           CALL PathOfEx.00AB58A0
	$+10     0073B154     84C0                  TEST AL,AL
	$+12     0073B156     0F85 B7000000         JNZ PathOfEx.0073B213
	$+18     0073B15C     3847 5D               CMP BYTE PTR DS:[EDI+0x5D],AL
	$+1B     0073B15F     0F84 AE000000         JE PathOfEx.0073B213
	$+21     0073B165     3847 5F               CMP BYTE PTR DS:[EDI+0x5F],AL
	$+24     0073B168     0F84 A5000000         JE PathOfEx.0073B213
	$+2A     0073B16E     8BCF                  MOV ECX,EDI
	$+2C     0073B170     E8 4B7AFEFF           CALL PathOfEx.00722BC0
	$+31     0073B175     84C0                  TEST AL,AL
	$+33     0073B177     0F84 96000000         JE PathOfEx.0073B213
	$+39     0073B17D     8B4F 34               MOV ECX,DWORD PTR DS:[EDI+0x34]                 ; g_o_ui_select_role_top_
	$+3C     0073B180     E8 EB0FDAFF           CALL PathOfEx.004DC170
	$+41     0073B185     5F                    POP EDI
	$+42     0073B186     5E                    POP ESI
	$+43     0073B187     8B4D F4               MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+46     0073B18A     64:890D 00000000      MOV DWORD PTR FS:[0],ECX
	$+4D     0073B191     8BE5                  MOV ESP,EBP
	$+4F     0073B193     5D                    POP EBP
	$+50     0073B194     C2 0C00               RETN 0xC
	$+53     0073B197     8D4D D4               LEA ECX,DWORD PTR SS:[EBP-0x2C]
	$+56     0073B19A     E8 B199D4FF           CALL PathOfEx.00484B50
	$+5B     0073B19F     51                    PUSH ECX
	$+5C     0073B1A0     8D4D E4               LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$+5F     0073B1A3     51                    PUSH ECX
	$+60     0073B1A4     8B08                  MOV ECX,DWORD PTR DS:[EAX]
	$+62     0073B1A6     E8 A50BD5FF           CALL PathOfEx.0048BD50
	$+67     0073B1AB     C745 FC 01000000      MOV DWORD PTR SS:[EBP-0x4],0x1
	$+6E     0073B1B2     83CF FF               OR EDI,0xFFFFFFFF
	$+71     0073B1B5     8B75 D8               MOV ESI,DWORD PTR SS:[EBP-0x28]
	$+74     0073B1B8     85F6                  TEST ESI,ESI
	$+76     0073B1BA     74 1F                 JE SHORT PathOfEx.0073B1DB
	$+78     0073B1BC     8BC7                  MOV EAX,EDI
	$+7A     0073B1BE     F0:0FC146 04          LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
	$+7F     0073B1C3     75 16                 JNZ SHORT PathOfEx.0073B1DB
	$+81     0073B1C5     8B06                  MOV EAX,DWORD PTR DS:[ESI]
	$+83     0073B1C7     8BCE                  MOV ECX,ESI
	$+85     0073B1C9     FF10                  CALL DWORD PTR DS:[EAX]
	$+87     0073B1CB     8BC7                  MOV EAX,EDI
	$+89     0073B1CD     F0:0FC146 08          LOCK XADD DWORD PTR DS:[ESI+0x8],EAX
	$+8E     0073B1D2     75 07                 JNZ SHORT PathOfEx.0073B1DB
	$+90     0073B1D4     8B06                  MOV EAX,DWORD PTR DS:[ESI]
	$+92     0073B1D6     8BCE                  MOV ECX,ESI
	$+94     0073B1D8     FF50 04               CALL DWORD PTR DS:[EAX+0x4]
	$+97     0073B1DB     C645 FC 00            MOV BYTE PTR SS:[EBP-0x4],0x0
	$+9B     0073B1DF     8B4D E4               MOV ECX,DWORD PTR SS:[EBP-0x1C]
	$+9E     0073B1E2     E8 39C6FFFF           CALL PathOfEx.00737820
	$+A3     0073B1E7     C745 FC 02000000      MOV DWORD PTR SS:[EBP-0x4],0x2
	$+AA     0073B1EE     8B75 E8               MOV ESI,DWORD PTR SS:[EBP-0x18]
	$+AD     0073B1F1     85F6                  TEST ESI,ESI
	$+AF     0073B1F3     74 1E                 JE SHORT PathOfEx.0073B213
	$+B1     0073B1F5     8BC7                  MOV EAX,EDI
	$+B3     0073B1F7     F0:0FC146 04          LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
	$+B8     0073B1FC     75 15                 JNZ SHORT PathOfEx.0073B213
	$+BA     0073B1FE     8B06                  MOV EAX,DWORD PTR DS:[ESI]
	$+BC     0073B200     8BCE                  MOV ECX,ESI
	$+BE     0073B202     FF10                  CALL DWORD PTR DS:[EAX]
	$+C0     0073B204     F0:0FC17E 08          LOCK XADD DWORD PTR DS:[ESI+0x8],EDI
	$+C5     0073B209     4F                    DEC EDI
	$+C6     0073B20A     75 07                 JNZ SHORT PathOfEx.0073B213
	$+C8     0073B20C     8B06                  MOV EAX,DWORD PTR DS:[ESI]
	$+CA     0073B20E     8BCE                  MOV ECX,ESI
	$+CC     0073B210     FF50 04               CALL DWORD PTR DS:[EAX+0x4]
	$+CF     0073B213     8B4D F4               MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+D2     0073B216     5F                    POP EDI
	$+D3     0073B217     5E                    POP ESI
	$+D4     0073B218     64:890D 00000000      MOV DWORD PTR FS:[0],ECX
	$+DB     0073B21F     8BE5                  MOV ESP,EBP
	$+DD     0073B221     5D                    POP EBP
	$+DE     0073B222     C2 0C00               RETN 0xC
	$+E1     0073B225     CC                    INT3
	$+E2     0073B226     CC                    INT3
	$+E3     0073B227     CC                    INT3
	$+E4     0073B228     CC                    INT3
	$+E5     0073B229     CC                    INT3
	$+E6     0073B22A     CC                    INT3
	$+E7     0073B22B     CC                    INT3
	$+E8     0073B22C     CC                    INT3
	$+E9     0073B22D     CC                    INT3
	$+EA     0073B22E     CC                    INT3
	$+EB     0073B22F     CC                    INT3
	$+EC     0073B230     55                    PUSH EBP
	$+ED     0073B231     8BEC                  MOV EBP,ESP
	$+EF     0073B233     64:A1 00000000        MOV EAX,DWORD PTR FS:[0]
	$+F5     0073B239     6A FF                 PUSH -0x1
	$+F7     0073B23B     68 68D8EF00           PUSH PathOfEx.00EFD868
	$+FC     0073B240     50                    PUSH EAX
	$+FD     0073B241     64:8925 00000000      MOV DWORD PTR FS:[0],ESP
	$+104    0073B248     83EC 10               SUB ESP,0x10
	$+107    0073B24B     56                    PUSH ESI
	$+108    0073B24C     8B75 0C               MOV ESI,DWORD PTR SS:[EBP+0xC]
	$+10B    0073B24F     57                    PUSH EDI
	$+10C    0073B250     8BF9                  MOV EDI,ECX
	$+10E    0073B252     803E 00               CMP BYTE PTR DS:[ESI],0x0
	$+111    0073B255     0F85 AD000000         JNZ PathOfEx.0073B308
	$+117    0073B25B     807E 08 00            CMP BYTE PTR DS:[ESI+0x8],0x0
	$+11B    0073B25F     0F85 A3000000         JNZ PathOfEx.0073B308
	$+121    0073B265     E8 5679FEFF           CALL PathOfEx.00722BC0
	$+126    0073B26A     84C0                  TEST AL,AL
	$+128    0073B26C     0F84 96000000         JE PathOfEx.0073B308
	$+12E    0073B272     8B46 04               MOV EAX,DWORD PTR DS:[ESI+0x4]
	$+131    0073B275     83F8 0D               CMP EAX,0xD
	$+134    0073B278     74 25                 JE SHORT PathOfEx.0073B29F
	$+136    0073B27A     83F8 1B               CMP EAX,0x1B
	$+139    0073B27D     0F85 85000000         JNZ PathOfEx.0073B308
	$+13F    0073B283     8BCF                  MOV ECX,EDI
	$+141    0073B285     C606 01               MOV BYTE PTR DS:[ESI],0x1
	$+144    0073B288     E8 03070000           CALL PathOfEx.0073B990
	$+149    0073B28D     5F                    POP EDI
	$+14A    0073B28E     5E                    POP ESI
	$+14B    0073B28F     8B4D F4               MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+14E    0073B292     64:890D 00000000      MOV DWORD PTR FS:[0],ECX
	$+155    0073B299     8BE5                  MOV ESP,EBP
	$+157    0073B29B     5D                    POP EBP
	$+158    0073B29C     C2 0800               RETN 0x8
	$+15B    0073B29F     8D4D E4               LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$+15E    0073B2A2     E8 A998D4FF           CALL PathOfEx.00484B50
	$+163    0073B2A7     51                    PUSH ECX
	$+164    0073B2A8     8D4D EC               LEA ECX,DWORD PTR SS:[EBP-0x14]
	$+167    0073B2AB     51                    PUSH ECX
	$+168    0073B2AC     8B08                  MOV ECX,DWORD PTR DS:[EAX]
	$+16A    0073B2AE     E8 9D0AD5FF           CALL PathOfEx.0048BD50
	$+16F    0073B2B3     8D4D E4               LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$+172    0073B2B6     C745 FC 00000000      MOV DWORD PTR SS:[EBP-0x4],0x0
	$+179    0073B2BD     E8 DEDED3FF           CALL PathOfEx.004791A0
	$+17E    0073B2C2     8B45 EC               MOV EAX,DWORD PTR SS:[EBP-0x14]
	$+181    0073B2C5     8B80 740B0000         MOV EAX,DWORD PTR DS:[EAX+0xB74]
	$+187    0073B2CB     85C0                  TEST EAX,EAX
	$+189    0073B2CD     74 09                 JE SHORT PathOfEx.0073B2D8
	$+18B    0073B2CF     80B8 54070000 00      CMP BYTE PTR DS:[EAX+0x754],0x0
	$+192    0073B2D6     75 28                 JNZ SHORT PathOfEx.0073B300
	$+194    0073B2D8     C606 01               MOV BYTE PTR DS:[ESI],0x1
	$+197    0073B2DB     8B4F 34               MOV ECX,DWORD PTR DS:[EDI+0x34]
	$+19A    0073B2DE     E8 EDA1EEFF           CALL PathOfEx.006254D0
	$+19F    0073B2E3     8B4F 34               MOV ECX,DWORD PTR DS:[EDI+0x34]
	$+1A2    0073B2E6     50                    PUSH EAX
	$+1A3    0073B2E7     E8 84A2EEFF           CALL PathOfEx.00625570
	$+1A8    0073B2EC     8B4F 34               MOV ECX,DWORD PTR DS:[EDI+0x34]
	$+1AB    0073B2EF     50                    PUSH EAX
	$+1AC    0073B2F0     E8 3BA4EEFF           CALL PathOfEx.00625730
	$+1B1    0073B2F5     0FB6C0                MOVZX EAX,AL
	$+1B4    0073B2F8     8BCF                  MOV ECX,EDI
	$+1B6    0073B2FA     50                    PUSH EAX
	$+1B7    0073B2FB     E8 40040000           CALL PathOfEx.0073B740
	$+1BC    0073B300     8D4D EC               LEA ECX,DWORD PTR SS:[EBP-0x14]
	$+1BF    0073B303     E8 98DED3FF           CALL PathOfEx.004791A0
	$+1C4    0073B308     8B4D F4               MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+1C7    0073B30B     5F                    POP EDI
	$+1C8    0073B30C     5E                    POP ESI
	$+1C9    0073B30D     64:890D 00000000      MOV DWORD PTR FS:[0],ECX
	$+1D0    0073B314     8BE5                  MOV ESP,EBP
	$+1D2    0073B316     5D                    POP EBP
	$+1D3    0073B317     C2 0800               RETN 0x8
	$+1D6    0073B31A     CC                    INT3
	$+1D7    0073B31B     CC                    INT3
	$+1D8    0073B31C     CC                    INT3
	$+1D9    0073B31D     CC                    INT3
	$+1DA    0073B31E     CC                    INT3
	$+1DB    0073B31F     CC                    INT3
	$+1DC    0073B320     55                    PUSH EBP
	$+1DD    0073B321     8BEC                  MOV EBP,ESP
	$+1DF    0073B323     6A FF                 PUSH -0x1
	$+1E1    0073B325     68 B478EF00           PUSH PathOfEx.00EF78B4
	$+1E6    0073B32A     64:A1 00000000        MOV EAX,DWORD PTR FS:[0]
	$+1EC    0073B330     50                    PUSH EAX
	$+1ED    0073B331     64:8925 00000000      MOV DWORD PTR FS:[0],ESP
	$+1F4    0073B338     83EC 14               SUB ESP,0x14
	$+1F7    0073B33B     56                    PUSH ESI
	$+1F8    0073B33C     57                    PUSH EDI
	$+1F9    0073B33D     8D4D E4               LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$+1FC    0073B340     E8 0B98D4FF           CALL PathOfEx.00484B50
	$+201    0073B345     51                    PUSH ECX
	$+202    0073B346     8D4D EC               LEA ECX,DWORD PTR SS:[EBP-0x14]
	$+205    0073B349     51                    PUSH ECX
	$+206    0073B34A     8B08                  MOV ECX,DWORD PTR DS:[EAX]
	$+208    0073B34C     E8 FF09D5FF           CALL PathOfEx.0048BD50
	$+20D    0073B351     C745 FC 01000000      MOV DWORD PTR SS:[EBP-0x4],0x1
	$+214    0073B358     83CF FF               OR EDI,0xFFFFFFFF
	$+217    0073B35B     8B75 E8               MOV ESI,DWORD PTR SS:[EBP-0x18]
	$+21A    0073B35E     85F6                  TEST ESI,ESI
	$+21C    0073B360     74 1F                 JE SHORT PathOfEx.0073B381
	$+21E    0073B362     8BC7                  MOV EAX,EDI
	$+220    0073B364     F0:0FC146 04          LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
	$+225    0073B369     75 16                 JNZ SHORT PathOfEx.0073B381
	$+227    0073B36B     8B06                  MOV EAX,DWORD PTR DS:[ESI]
	$+229    0073B36D     8BCE                  MOV ECX,ESI
	$+22B    0073B36F     FF10                  CALL DWORD PTR DS:[EAX]
	$+22D    0073B371     8BC7                  MOV EAX,EDI
	$+22F    0073B373     F0:0FC146 08          LOCK XADD DWORD PTR DS:[ESI+0x8],EAX
	$+234    0073B378     75 07                 JNZ SHORT PathOfEx.0073B381
	$+236    0073B37A     8B06                  MOV EAX,DWORD PTR DS:[ESI]
	$+238    0073B37C     8BCE                  MOV ECX,ESI
	$+23A    0073B37E     FF50 04               CALL DWORD PTR DS:[EAX+0x4]
	$+23D    0073B381     C645 FC 00            MOV BYTE PTR SS:[EBP-0x4],0x0
	$+241    0073B385     8B45 EC               MOV EAX,DWORD PTR SS:[EBP-0x14]
	$+244    0073B388     8B88 740B0000         MOV ECX,DWORD PTR DS:[EAX+0xB74]
	$+24A    0073B38E     85C9                  TEST ECX,ECX
	$+24C    0073B390     74 07                 JE SHORT PathOfEx.0073B399
	$+24E    0073B392     6A 00                 PUSH 0x0
	$+250    0073B394     E8 17A43700           CALL PathOfEx.00AB57B0
	$+255    0073B399     C745 FC 02000000      MOV DWORD PTR SS:[EBP-0x4],0x2
	$+25C    0073B3A0     8B75 F0               MOV ESI,DWORD PTR SS:[EBP-0x10]
	$+25F    0073B3A3     85F6                  TEST ESI,ESI
	$+261    0073B3A5     74 1E                 JE SHORT PathOfEx.0073B3C5
	$+263    0073B3A7     8BC7                  MOV EAX,EDI
	$+265    0073B3A9     F0:0FC146 04          LOCK XADD DWORD PTR DS:[ESI+0x4],EAX
	$+26A    0073B3AE     75 15                 JNZ SHORT PathOfEx.0073B3C5
	$+26C    0073B3B0     8B06                  MOV EAX,DWORD PTR DS:[ESI]
	$+26E    0073B3B2     8BCE                  MOV ECX,ESI
	$+270    0073B3B4     FF10                  CALL DWORD PTR DS:[EAX]
	$+272    0073B3B6     F0:0FC17E 08          LOCK XADD DWORD PTR DS:[ESI+0x8],EDI
	$+277    0073B3BB     4F                    DEC EDI
	$+278    0073B3BC     75 07                 JNZ SHORT PathOfEx.0073B3C5
	$+27A    0073B3BE     8B06                  MOV EAX,DWORD PTR DS:[ESI]
	$+27C    0073B3C0     8BCE                  MOV ECX,ESI
	$+27E    0073B3C2     FF50 04               CALL DWORD PTR DS:[EAX+0x4]
	$+281    0073B3C5     8B4D F4               MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+284    0073B3C8     5F                    POP EDI
	$+285    0073B3C9     64:890D 00000000      MOV DWORD PTR FS:[0],ECX
	$+28C    0073B3D0     5E                    POP ESI
	$+28D    0073B3D1     8BE5                  MOV ESP,EBP
	$+28F    0073B3D3     5D                    POP EBP
	$+290    0073B3D4     C3                    RETN
	$+291    0073B3D5     CC                    INT3
	*/
	op_by_tmp = scan_exe_.ScanImme<pt_byte>("8b????85??0f??????????e8????????84??0f??????????38????0f??????????38????0f", 2);
	if (!op_by_tmp)
	{
		assert(false);
		return false;
	}
	dw_tmp = *op_by_tmp;
	assert(dw_tmp < 0xa0);
	ofs << "pt_cdword	g_o_ui_select_role_top_ = " << dw_tmp << ";" << FC_OFstream_EndlLog;
	return true;
}
