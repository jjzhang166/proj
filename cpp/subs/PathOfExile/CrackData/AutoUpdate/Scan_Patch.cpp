#include "stdafx.h"
#include "AutoUpdate.h"

bool CAutoUpdateApp::Scan_Patch(Cmn_FC_OFstream& ofs)
{
	//boost::optional<pt_dword> op_dw_tmp;
	//pt_dword dw_tmp;
	pt_pcvoid addr_tmp;
	//pt_pcvoid addr_tmp1;
	/*
	$-421    008CEF9F     CC                          INT3
	$-420    008CEFA0     55                          PUSH EBP
	$-41F    008CEFA1     8D6C24 C0                   LEA EBP,DWORD PTR SS:[ESP-0x40]
	$-41B    008CEFA5     83EC 40                     SUB ESP,0x40
	$-418    008CEFA8     6A FF                       PUSH -0x1
	$-416    008CEFAA     68 1552D600                 PUSH PathOfEx.00D65215
	$-411    008CEFAF     64:A1 00000000              MOV EAX,DWORD PTR FS:[0]
	$-40B    008CEFB5     50                          PUSH EAX
	$-40A    008CEFB6     64:8925 00000000            MOV DWORD PTR FS:[0],ESP
	$-403    008CEFBD     81EC B0000000               SUB ESP,0xB0
	$-3FD    008CEFC3     53                          PUSH EBX
	$-3FC    008CEFC4     8BD9                        MOV EBX,ECX
	$-3FA    008CEFC6     C745 38 00000000            MOV DWORD PTR SS:[EBP+0x38],0x0
	$-3F3    008CEFCD     56                          PUSH ESI
	$-3F2    008CEFCE     57                          PUSH EDI
	$-3F1    008CEFCF     895D 1C                     MOV DWORD PTR SS:[EBP+0x1C],EBX
	$-3EE    008CEFD2     8B8B 9C3A0000               MOV ECX,DWORD PTR DS:[EBX+0x3A9C]
	$-3E8    008CEFD8     85C9                        TEST ECX,ECX
	$-3E6    008CEFDA     0F84 1C020000               JE PathOfEx.008CF1FC
	$-3E0    008CEFE0     8079 38 00                  CMP BYTE PTR DS:[ECX+0x38],0x0
	$-3DC    008CEFE4     75 10                       JNZ SHORT PathOfEx.008CEFF6
	$-3DA    008CEFE6     83C1 3C                     ADD ECX,0x3C
	$-3D7    008CEFE9     E8 62AA1800                 CALL PathOfEx.00A59A50
	$-3D2    008CEFEE     84C0                        TEST AL,AL
	$-3D0    008CEFF0     0F84 06020000               JE PathOfEx.008CF1FC
	$-3CA    008CEFF6     8BB3 9C3A0000               MOV ESI,DWORD PTR DS:[EBX+0x3A9C]
	$-3C4    008CEFFC     807E 38 00                  CMP BYTE PTR DS:[ESI+0x38],0x0
	$-3C0    008CF000     75 24                       JNZ SHORT PathOfEx.008CF026
	$-3BE    008CF002     8B46 40                     MOV EAX,DWORD PTR DS:[ESI+0x40]
	$-3BB    008CF005     85C0                        TEST EAX,EAX
	$-3B9    008CF007     74 19                       JE SHORT PathOfEx.008CF022
	$-3B7    008CF009     6A FF                       PUSH -0x1
	$-3B5    008CF00B     50                          PUSH EAX
	$-3B4    008CF00C     FF15 B8E0D800               CALL DWORD PTR DS:[0xD8E0B8]
	$-3AE    008CF012     FF76 40                     PUSH DWORD PTR DS:[ESI+0x40]
	$-3AB    008CF015     FF15 A8E0D800               CALL DWORD PTR DS:[0xD8E0A8]
	$-3A5    008CF01B     C746 40 00000000            MOV DWORD PTR DS:[ESI+0x40],0x0
	$-39E    008CF022     C646 38 01                  MOV BYTE PTR DS:[ESI+0x38],0x1
	$-39A    008CF026     8B46 30                     MOV EAX,DWORD PTR DS:[ESI+0x30]
	$-397    008CF029     8945 2C                     MOV DWORD PTR SS:[EBP+0x2C],EAX
	$-394    008CF02C     8B83 9C3A0000               MOV EAX,DWORD PTR DS:[EBX+0x3A9C]
	$-38E    008CF032     C785 44FFFFFF DCA8E700      MOV DWORD PTR SS:[EBP-0xBC],PathOfEx.00E7A8DC
	$-384    008CF03C     C785 54FFFFFF A4E8EA00      MOV DWORD PTR SS:[EBP-0xAC],PathOfEx.00EAE8A4
	$-37A    008CF046     C745 AC AC53DE00            MOV DWORD PTR SS:[EBP-0x54],PathOfEx.00DE53AC
	$-373    008CF04D     8B70 34                     MOV ESI,DWORD PTR DS:[EAX+0x34]
	$-370    008CF050     8975 3C                     MOV DWORD PTR SS:[EBP+0x3C],ESI
	$-36D    008CF053     51                          PUSH ECX
	$-36C    008CF054     8D85 5CFFFFFF               LEA EAX,DWORD PTR SS:[EBP-0xA4]
	$-366    008CF05A     C745 FC 00000000            MOV DWORD PTR SS:[EBP-0x4],0x0
	$-35F    008CF061     50                          PUSH EAX
	$-35E    008CF062     8D8D 44FFFFFF               LEA ECX,DWORD PTR SS:[EBP-0xBC]
	$-358    008CF068     C745 38 01000000            MOV DWORD PTR SS:[EBP+0x38],0x1
	$-351    008CF06F     E8 3C94BAFF                 CALL PathOfEx.004784B0
	$-34C    008CF074     C745 FC 01000000            MOV DWORD PTR SS:[EBP-0x4],0x1
	$-345    008CF07B     8B85 44FFFFFF               MOV EAX,DWORD PTR SS:[EBP-0xBC]
	$-33F    008CF081     6A 03                       PUSH 0x3
	$-33D    008CF083     8B40 04                     MOV EAX,DWORD PTR DS:[EAX+0x4]
	$-33A    008CF086     C78405 44FFFFFF FCFBE600    MOV DWORD PTR SS:[EBP+EAX-0xBC],PathOfEx.00E6FBFC
	$-32F    008CF091     8B85 44FFFFFF               MOV EAX,DWORD PTR SS:[EBP-0xBC]
	$-329    008CF097     8B48 04                     MOV ECX,DWORD PTR DS:[EAX+0x4]
	$-326    008CF09A     8D41 98                     LEA EAX,DWORD PTR DS:[ECX-0x68]
	$-323    008CF09D     89840D 40FFFFFF             MOV DWORD PTR SS:[EBP+ECX-0xC0],EAX
	$-31C    008CF0A4     8D8D 5CFFFFFF               LEA ECX,DWORD PTR SS:[EBP-0xA4]
	$-316    008CF0AA     E8 E199BAFF                 CALL PathOfEx.00478A90
	$-311    008CF0AF     68 2491EF00                 PUSH PathOfEx.00EF9124                                 ; UNICODE "ms"
	$-30C    008CF0B4     56                          PUSH ESI
	$-30B    008CF0B5     8D85 54FFFFFF               LEA EAX,DWORD PTR SS:[EBP-0xAC]
	$-305    008CF0BB     C745 FC 02000000            MOV DWORD PTR SS:[EBP-0x4],0x2
	$-2FE    008CF0C2     68 D890EF00                 PUSH PathOfEx.00EF90D8                                 ; UNICODE "Connect time to instance server was "
	$-2F9    008CF0C7     50                          PUSH EAX
	$-2F8    008CF0C8     E8 4393BBFF                 CALL PathOfEx.00488410
	$-2F3    008CF0CD     83C4 08                     ADD ESP,0x8
	$-2F0    008CF0D0     8BC8                        MOV ECX,EAX
	$-2EE    008CF0D2     E8 D919BBFF                 CALL PathOfEx.00480AB0
	$-2E9    008CF0D7     50                          PUSH EAX
	$-2E8    008CF0D8     E8 3393BBFF                 CALL PathOfEx.00488410
	$-2E3    008CF0DD     83C4 08                     ADD ESP,0x8
	$-2E0    008CF0E0     8D45 00                     LEA EAX,DWORD PTR SS:[EBP]
	$-2DD    008CF0E3     8D8D 44FFFFFF               LEA ECX,DWORD PTR SS:[EBP-0xBC]
	$-2D7    008CF0E9     50                          PUSH EAX
	$-2D6    008CF0EA     E8 0193BBFF                 CALL PathOfEx.004883F0
	$-2D1    008CF0EF     8BF0                        MOV ESI,EAX
	$-2CF    008CF0F1     C645 FC 03                  MOV BYTE PTR SS:[EBP-0x4],0x3
	$-2CB    008CF0F5     FF15 C8E0D800               CALL DWORD PTR DS:[0xD8E0C8]                           ; kernel32.GetTickCount
	$-2C5    008CF0FB     8BF8                        MOV EDI,EAX
	$-2C3    008CF0FD     8D45 34                     LEA EAX,DWORD PTR SS:[EBP+0x34]
	$-2C0    008CF100     50                          PUSH EAX
	$-2BF    008CF101     E8 11B93D00                 CALL PathOfEx.00CAAA17
	$-2BA    008CF106     64:8B0D 2C000000            MOV ECX,DWORD PTR FS:[0x2C]
	$-2B3    008CF10D     83C4 04                     ADD ESP,0x4
	$-2B0    008CF110     8B09                        MOV ECX,DWORD PTR DS:[ECX]
	$-2AE    008CF112     56                          PUSH ESI
	$-2AD    008CF113     83EC 0C                     SUB ESP,0xC
	$-2AA    008CF116     8B49 18                     MOV ECX,DWORD PTR DS:[ECX+0x18]
	$-2A7    008CF119     6A 03                       PUSH 0x3
	$-2A5    008CF11B     57                          PUSH EDI
	$-2A4    008CF11C     FF75 38                     PUSH DWORD PTR SS:[EBP+0x38]
	$-2A1    008CF11F     FF75 34                     PUSH DWORD PTR SS:[EBP+0x34]
	$-29E    008CF122     68 19010000                 PUSH 0x119
	$-299    008CF127     E8 648FBBFF                 CALL PathOfEx.00488090
	$-294    008CF12C     8B45 14                     MOV EAX,DWORD PTR SS:[EBP+0x14]
	$-291    008CF12F     83F8 08                     CMP EAX,0x8
	$-28E    008CF132     72 0F                       JB SHORT PathOfEx.008CF143
	$-28C    008CF134     40                          INC EAX
	$-28B    008CF135     6A 02                       PUSH 0x2
	$-289    008CF137     50                          PUSH EAX
	$-288    008CF138     FF75 00                     PUSH DWORD PTR SS:[EBP]
	$-285    008CF13B     E8 F0D6B9FF                 CALL PathOfEx.0046C830
	$-280    008CF140     83C4 0C                     ADD ESP,0xC
	$-27D    008CF143     8D4D AC                     LEA ECX,DWORD PTR SS:[EBP-0x54]
	$-27A    008CF146     E8 257CBAFF                 CALL PathOfEx.00476D70
	$-275    008CF14B     8D45 AC                     LEA EAX,DWORD PTR SS:[EBP-0x54]
	$-272    008CF14E     C745 FC 04000000            MOV DWORD PTR SS:[EBP-0x4],0x4
	$-26B    008CF155     50                          PUSH EAX
	$-26A    008CF156     C745 AC 2453DE00            MOV DWORD PTR SS:[EBP-0x54],PathOfEx.00DE5324
	$-263    008CF15D     E8 0FB03900                 CALL PathOfEx.00C6A171
	$-25E    008CF162     C745 FC FFFFFFFF            MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-257    008CF169     83C4 04                     ADD ESP,0x4
	$-254    008CF16C     8B8B 943A0000               MOV ECX,DWORD PTR DS:[EBX+0x3A94]
	$-24E    008CF172     83B9 E8010000 03            CMP DWORD PTR DS:[ECX+0x1E8],0x3
	$-247    008CF179     75 2F                       JNZ SHORT PathOfEx.008CF1AA
	$-245    008CF17B     A1 205FFE00                 MOV EAX,DWORD PTR DS:[0xFE5F20]
	$-240    008CF180     83F8 02                     CMP EAX,0x2
	$-23D    008CF183     75 0A                       JNZ SHORT PathOfEx.008CF18F
	$-23B    008CF185     837D 3C 5A                  CMP DWORD PTR SS:[EBP+0x3C],0x5A
	$-237    008CF189     76 19                       JBE SHORT PathOfEx.008CF1A4
	$-235    008CF18B     33C0                        XOR EAX,EAX
	$-233    008CF18D     EB 10                       JMP SHORT PathOfEx.008CF19F
	$-231    008CF18F     85C0                        TEST EAX,EAX
	$-22F    008CF191     75 11                       JNZ SHORT PathOfEx.008CF1A4
	$-22D    008CF193     837D 3C 46                  CMP DWORD PTR SS:[EBP+0x3C],0x46
	$-229    008CF197     BA 02000000                 MOV EDX,0x2
	$-224    008CF19C     0F42C2                      CMOVB EAX,EDX
	$-221    008CF19F     A3 205FFE00                 MOV DWORD PTR DS:[0xFE5F20],EAX
	$-21C    008CF1A4     8981 E8010000               MOV DWORD PTR DS:[ECX+0x1E8],EAX
	$-216    008CF1AA     8B45 2C                     MOV EAX,DWORD PTR SS:[EBP+0x2C]
	$-213    008CF1AD     85C0                        TEST EAX,EAX
	$-211    008CF1AF     74 0F                       JE SHORT PathOfEx.008CF1C0
	$-20F    008CF1B1     8BCB                        MOV ECX,EBX
	$-20D    008CF1B3     8983 983A0000               MOV DWORD PTR DS:[EBX+0x3A98],EAX
	$-207    008CF1B9     E8 D2FCFFFF                 CALL PathOfEx.008CEE90
	$-202    008CF1BE     EB 16                       JMP SHORT PathOfEx.008CF1D6
	$-200    008CF1C0     B8 83000000                 MOV EAX,0x83
	$-1FB    008CF1C5     C783 343B0000 01000000      MOV DWORD PTR DS:[EBX+0x3B34],0x1
	$-1F1    008CF1CF     66:8983 803B0000            MOV WORD PTR DS:[EBX+0x3B80],AX
	$-1EA    008CF1D6     8BB3 9C3A0000               MOV ESI,DWORD PTR DS:[EBX+0x3A9C]
	$-1E4    008CF1DC     C783 9C3A0000 00000000      MOV DWORD PTR DS:[EBX+0x3A9C],0x0
	$-1DA    008CF1E6     85F6                        TEST ESI,ESI
	$-1D8    008CF1E8     74 12                       JE SHORT PathOfEx.008CF1FC
	$-1D6    008CF1EA     8BCE                        MOV ECX,ESI
	$-1D4    008CF1EC     E8 0FA71800                 CALL PathOfEx.00A59900
	$-1CF    008CF1F1     6A 50                       PUSH 0x50
	$-1CD    008CF1F3     56                          PUSH ESI
	$-1CC    008CF1F4     E8 8FD13A00                 CALL PathOfEx.00C7C388
	$-1C7    008CF1F9     83C4 08                     ADD ESP,0x8
	$-1C4    008CF1FC     8B83 CC3E0000               MOV EAX,DWORD PTR DS:[EBX+0x3ECC]
	$-1BE    008CF202     8B75 50                     MOV ESI,DWORD PTR SS:[EBP+0x50]
	$-1BB    008CF205     85C0                        TEST EAX,EAX
	$-1B9    008CF207     74 18                       JE SHORT PathOfEx.008CF221
	$-1B7    008CF209     3BF0                        CMP ESI,EAX
	$-1B5    008CF20B     76 0C                       JBE SHORT PathOfEx.008CF219
	$-1B3    008CF20D     C783 CC3E0000 00000000      MOV DWORD PTR DS:[EBX+0x3ECC],0x0
	$-1A9    008CF217     EB 08                       JMP SHORT PathOfEx.008CF221
	$-1A7    008CF219     2BC6                        SUB EAX,ESI
	$-1A5    008CF21B     8983 CC3E0000               MOV DWORD PTR DS:[EBX+0x3ECC],EAX
	$-19F    008CF221     8B83 343B0000               MOV EAX,DWORD PTR DS:[EBX+0x3B34]
	$-199    008CF227     85C0                        TEST EAX,EAX
	$-197    008CF229     0F84 5A050000               JE PathOfEx.008CF789
	$-191    008CF22F     83F8 01                     CMP EAX,0x1
	$-18E    008CF232     0F84 51050000               JE PathOfEx.008CF789
	$-188    008CF238     8B8B 983A0000               MOV ECX,DWORD PTR DS:[EBX+0x3A98]
	$-182    008CF23E     E8 AD5CECFF                 CALL PathOfEx.00794EF0
	$-17D    008CF243     FFB3 983A0000               PUSH DWORD PTR DS:[EBX+0x3A98]
	$-177    008CF249     8D8B AC3A0000               LEA ECX,DWORD PTR DS:[EBX+0x3AAC]
	$-171    008CF24F     E8 CC5B1900                 CALL PathOfEx.00A64E20
	$-16C    008CF254     83BB 343B0000 03            CMP DWORD PTR DS:[EBX+0x3B34],0x3
	$-165    008CF25B     0F85 FD000000               JNZ PathOfEx.008CF35E
	$-15F    008CF261     80BB 12400000 00            CMP BYTE PTR DS:[EBX+0x4012],0x0
	$-158    008CF268     74 13                       JE SHORT PathOfEx.008CF27D
	$-156    008CF26A     8B83 943A0000               MOV EAX,DWORD PTR DS:[EBX+0x3A94]
	$-150    008CF270     83B8 E8010000 02            CMP DWORD PTR DS:[EAX+0x1E8],0x2
	$-149    008CF277     0F85 E1000000               JNZ PathOfEx.008CF35E
	$-143    008CF27D     66:0F6EC6                   MOVD MM0,ESI
	$-13F    008CF281     8BC6                        MOV EAX,ESI
	$-13D    008CF283     F30FE6C0                    CVTDQ2PD XMM0,XMM0
	$-139    008CF287     C1E8 1F                     SHR EAX,0x1F
	$-136    008CF28A     8B8B 943A0000               MOV ECX,DWORD PTR DS:[EBX+0x3A94]
	$-130    008CF290     51                          PUSH ECX
	$-12F    008CF291     8B11                        MOV EDX,DWORD PTR DS:[ECX]
	$-12D    008CF293     F20F5804C5 40FCF300         ADDSD XMM0,QWORD PTR DS:[EAX*8+0xF3FC40]
	$-124    008CF29C     660F5AC0                    CVTPD2PS XMM0,XMM0
	$-120    008CF2A0     F3:0F5E05 F0F2F300          DIVSS XMM0,DWORD PTR DS:[0xF3F2F0]
	$-118    008CF2A8     F3:0F110424                 MOVSS DWORD PTR SS:[ESP],XMM0
	$-113    008CF2AD     FF52 18                     CALL DWORD PTR DS:[EDX+0x18]
	$-110    008CF2B0     8BB3 B43E0000               MOV ESI,DWORD PTR DS:[EBX+0x3EB4]
	$-10A    008CF2B6     8975 38                     MOV DWORD PTR SS:[EBP+0x38],ESI
	$-107    008CF2B9     3BB3 B83E0000               CMP ESI,DWORD PTR DS:[EBX+0x3EB8]
	$-101    008CF2BF     0F84 99000000               JE PathOfEx.008CF35E
	$-FB     008CF2C5     8B4D 50                     MOV ECX,DWORD PTR SS:[EBP+0x50]
	$-F8     008CF2C8     8D7E 30                     LEA EDI,DWORD PTR DS:[ESI+0x30]
	$-F5     008CF2CB     897D 3C                     MOV DWORD PTR SS:[EBP+0x3C],EDI
	$-F2     008CF2CE     66:90                       NOP
	$-F0     008CF2D0     8B06                        MOV EAX,DWORD PTR DS:[ESI]
	$-EE     008CF2D2     3BC1                        CMP EAX,ECX
	$-EC     008CF2D4     77 6C                       JA SHORT PathOfEx.008CF342
	$-EA     008CF2D6     8B4F FC                     MOV ECX,DWORD PTR DS:[EDI-0x4]
	$-E7     008CF2D9     85C9                        TEST ECX,ECX
	$-E5     008CF2DB     0F84 30010000               JE PathOfEx.008CF411
	$-DF     008CF2E1     8B01                        MOV EAX,DWORD PTR DS:[ECX]
	$-DD     008CF2E3     FF50 08                     CALL DWORD PTR DS:[EAX+0x8]
	$-DA     008CF2E6     8B8B B83E0000               MOV ECX,DWORD PTR DS:[EBX+0x3EB8]
	$-D4     008CF2EC     8B45 3C                     MOV EAX,DWORD PTR SS:[EBP+0x3C]
	$-D1     008CF2EF     894D 2C                     MOV DWORD PTR SS:[EBP+0x2C],ECX
	$-CE     008CF2F2     3BC1                        CMP EAX,ECX
	$-CC     008CF2F4     74 28                       JE SHORT PathOfEx.008CF31E
	$-CA     008CF2F6     8BDE                        MOV EBX,ESI
	$-C8     008CF2F8     8BF1                        MOV ESI,ECX
	$-C6     008CF2FA     2BD8                        SUB EBX,EAX
	$-C4     008CF2FC     0F1F40 00                   NOP DWORD PTR DS:[EAX]
	$-C0     008CF300     8B07                        MOV EAX,DWORD PTR DS:[EDI]
	$-BE     008CF302     89041F                      MOV DWORD PTR DS:[EDI+EBX],EAX
	$-BB     008CF305     8D47 08                     LEA EAX,DWORD PTR DS:[EDI+0x8]
	$-B8     008CF308     50                          PUSH EAX
	$-B7     008CF309     8D0C18                      LEA ECX,DWORD PTR DS:[EAX+EBX]
	$-B4     008CF30C     E8 6F10BAFF                 CALL PathOfEx.00470380
	$-AF     008CF311     83C7 30                     ADD EDI,0x30
	$-AC     008CF314     3BFE                        CMP EDI,ESI
	$-AA     008CF316   ^ 75 E8                       JNZ SHORT PathOfEx.008CF300
	$-A8     008CF318     8B75 38                     MOV ESI,DWORD PTR SS:[EBP+0x38]
	$-A5     008CF31B     8B5D 1C                     MOV EBX,DWORD PTR SS:[EBP+0x1C]
	$-A2     008CF31E     8B93 B83E0000               MOV EDX,DWORD PTR DS:[EBX+0x3EB8]
	$-9C     008CF324     FF75 50                     PUSH DWORD PTR SS:[EBP+0x50]
	$-99     008CF327     8D4A D0                     LEA ECX,DWORD PTR DS:[EDX-0x30]
	$-96     008CF32A     51                          PUSH ECX
	$-95     008CF32B     E8 E0B0BDFF                 CALL PathOfEx.004AA410
	$-90     008CF330     8B7D 3C                     MOV EDI,DWORD PTR SS:[EBP+0x3C]
	$-8D     008CF333     83C4 08                     ADD ESP,0x8
	$-8A     008CF336     8383 B83E0000 D0            ADD DWORD PTR DS:[EBX+0x3EB8],-0x30
	$-83     008CF33D     8B4D 50                     MOV ECX,DWORD PTR SS:[EBP+0x50]
	$-80     008CF340     EB 10                       JMP SHORT PathOfEx.008CF352
	$-7E     008CF342     2BC1                        SUB EAX,ECX
	$-7C     008CF344     8906                        MOV DWORD PTR DS:[ESI],EAX
	$-7A     008CF346     83C6 30                     ADD ESI,0x30
	$-77     008CF349     83C7 30                     ADD EDI,0x30
	$-74     008CF34C     8975 38                     MOV DWORD PTR SS:[EBP+0x38],ESI
	$-71     008CF34F     897D 3C                     MOV DWORD PTR SS:[EBP+0x3C],EDI
	$-6E     008CF352     3BB3 B83E0000               CMP ESI,DWORD PTR DS:[EBX+0x3EB8]
	$-68     008CF358   ^ 0F85 72FFFFFF               JNZ PathOfEx.008CF2D0
	$-62     008CF35E     8D45 18                     LEA EAX,DWORD PTR SS:[EBP+0x18]
	$-5F     008CF361     50                          PUSH EAX
	$-5E     008CF362     FF15 DCE2D800               CALL DWORD PTR DS:[0xD8E2DC]
	$-58     008CF368     FF35 E4F70001               PUSH DWORD PTR DS:[0x100F7E4]
	$-52     008CF36E     FF35 E0F70001               PUSH DWORD PTR DS:[0x100F7E0]
	$-4C     008CF374     FF75 1C                     PUSH DWORD PTR SS:[EBP+0x1C]
	$-49     008CF377     FF75 18                     PUSH DWORD PTR SS:[EBP+0x18]
	$-46     008CF37A     E8 31DA3A00                 CALL PathOfEx.00C7CDB0
	$-41     008CF37F     83BB 343B0000 03            CMP DWORD PTR DS:[EBX+0x3B34],0x3
	$-3A     008CF386     8BF0                        MOV ESI,EAX
	$-38     008CF388     8BFA                        MOV EDI,EDX
	$-36     008CF38A     0F85 94010000               JNZ PathOfEx.008CF524
	$-30     008CF390     80BB 903B0000 00            CMP BYTE PTR DS:[EBX+0x3B90],0x0
	$-29     008CF397     8B8B 883B0000               MOV ECX,DWORD PTR DS:[EBX+0x3B88]
	$-23     008CF39D     8B83 8C3B0000               MOV EAX,DWORD PTR DS:[EBX+0x3B8C]
	$-1D     008CF3A3     74 71                       JE SHORT PathOfEx.008CF416
	$-1B     008CF3A5     81C1 D0070000               ADD ECX,0x7D0
	$-15     008CF3AB     83D0 00                     ADC EAX,0x0
	$-12     008CF3AE     3BF8                        CMP EDI,EAX
	$-10     008CF3B0     0F8C D5000000               JL PathOfEx.008CF48B
	$-A      008CF3B6     7F 08                       JG SHORT PathOfEx.008CF3C0
	$-8      008CF3B8     3BF1                        CMP ESI,ECX
	$-6      008CF3BA     0F82 CB000000               JB PathOfEx.008CF48B                                   ; 这个函数估计是个处理timer的函数
	$ ==>    008CF3C0     B8 0F000000                 MOV EAX,0xF                                            ; 此处会发0x1和0xf的包，此处ret掉，调试断住再久都不掉线了，0x1这个包很可能有检测之类的东西
	$+5      008CF3C5     C745 30 BC9BEF00            MOV DWORD PTR SS:[EBP+0x30],PathOfEx.00EF9BBC
	$+C      008CF3CC     66:8945 34                  MOV WORD PTR SS:[EBP+0x34],AX
	$+10     008CF3D0     8D45 30                     LEA EAX,DWORD PTR SS:[EBP+0x30]
	$+13     008CF3D3     C745 FC 05000000            MOV DWORD PTR SS:[EBP-0x4],0x5
	$+1A     008CF3DA     50                          PUSH EAX
	$+1B     008CF3DB     FFB3 983A0000               PUSH DWORD PTR DS:[EBX+0x3A98]
	$+21     008CF3E1     C745 38 00000000            MOV DWORD PTR SS:[EBP+0x38],0x0
	$+28     008CF3E8     E8 13AFDEFF                 CALL PathOfEx.006BA300
	$+2D     008CF3ED     8BC8                        MOV ECX,EAX
	$+2F     008CF3EF     E8 CCAF0100                 CALL PathOfEx.008EA3C0
	$+34     008CF3F4     8B8B 983A0000               MOV ECX,DWORD PTR DS:[EBX+0x3A98]
	$+3A     008CF3FA     6A 00                       PUSH 0x0
	$+3C     008CF3FC     E8 7F5EECFF                 CALL PathOfEx.00795280
	$+41     008CF401     C745 FC FFFFFFFF            MOV DWORD PTR SS:[EBP-0x4],-0x1					;return到此处
	$+48     008CF408     C745 30 489CEF00            MOV DWORD PTR SS:[EBP+0x30],PathOfEx.00EF9C48          ; ASCII " 殒"
	$+4F     008CF40F     EB 6E                       JMP SHORT PathOfEx.008CF47F
	$+51     008CF411     E8 47AC3900                 CALL PathOfEx.00C6A05D
	$+56     008CF416     81C1 F4010000               ADD ECX,0x1F4
	$+5C     008CF41C     83D0 00                     ADC EAX,0x0
	$+5F     008CF41F     3BF8                        CMP EDI,EAX
	$+61     008CF421     7C 68                       JL SHORT PathOfEx.008CF48B
	$+63     008CF423     7F 04                       JG SHORT PathOfEx.008CF429
	$+65     008CF425     3BF1                        CMP ESI,ECX
	$+67     008CF427     72 62                       JB SHORT PathOfEx.008CF48B
	$+69     008CF429     B8 0F000000                 MOV EAX,0xF                                            ; 此处只发0xf的包，估计是个心跳包，只有一个dword字段，还一直为0，但是经过两个函数的加密
	$+6E     008CF42E     C745 20 BC9BEF00            MOV DWORD PTR SS:[EBP+0x20],PathOfEx.00EF9BBC
	$+75     008CF435     66:8945 24                  MOV WORD PTR SS:[EBP+0x24],AX
	$+79     008CF439     8D45 20                     LEA EAX,DWORD PTR SS:[EBP+0x20]
	$+7C     008CF43C     C745 FC 06000000            MOV DWORD PTR SS:[EBP-0x4],0x6
	$+83     008CF443     50                          PUSH EAX
	$+84     008CF444     FFB3 983A0000               PUSH DWORD PTR DS:[EBX+0x3A98]
	$+8A     008CF44A     C745 28 00000000            MOV DWORD PTR SS:[EBP+0x28],0x0
	$+91     008CF451     E8 AAAEDEFF                 CALL PathOfEx.006BA300
	$+96     008CF456     8BC8                        MOV ECX,EAX
	$+98     008CF458     E8 63AF0100                 CALL PathOfEx.008EA3C0
	$+9D     008CF45D     8B8B 983A0000               MOV ECX,DWORD PTR DS:[EBX+0x3A98]
	$+A3     008CF463     6A 00                       PUSH 0x0
	$+A5     008CF465     E8 165EECFF                 CALL PathOfEx.00795280
	$+AA     008CF46A     C745 FC FFFFFFFF            MOV DWORD PTR SS:[EBP-0x4],-0x1				;return到此处
	$+B1     008CF471     C683 903B0000 01            MOV BYTE PTR DS:[EBX+0x3B90],0x1
	$+B8     008CF478     C745 20 489CEF00            MOV DWORD PTR SS:[EBP+0x20],PathOfEx.00EF9C48          ; ASCII " 殒"
	$+BF     008CF47F     89BB 8C3B0000               MOV DWORD PTR DS:[EBX+0x3B8C],EDI
	$+C5     008CF485     89B3 883B0000               MOV DWORD PTR DS:[EBX+0x3B88],ESI
	$+CB     008CF48B     8B93 983A0000               MOV EDX,DWORD PTR DS:[EBX+0x3A98]
	$+D1     008CF491     837A 58 00                  CMP DWORD PTR DS:[EDX+0x58],0x0
	$+D5     008CF495     74 44                       JE SHORT PathOfEx.008CF4DB
	$+D7     008CF497     8B4A 58                     MOV ECX,DWORD PTR DS:[EDX+0x58]
	$+DA     008CF49A     8B81 30010000               MOV EAX,DWORD PTR DS:[ECX+0x130]
	$+E0     008CF4A0     3B81 34010000               CMP EAX,DWORD PTR DS:[ECX+0x134]
	$+E6     008CF4A6     74 33                       JE SHORT PathOfEx.008CF4DB
	$+E8     008CF4A8     8D45 0C                     LEA EAX,DWORD PTR SS:[EBP+0xC]
	$+EB     008CF4AB     50                          PUSH EAX
	$+EC     008CF4AC     E8 3F5F1900                 CALL PathOfEx.00A653F0
	$+F1     008CF4B1     8B4D 0C                     MOV ECX,DWORD PTR SS:[EBP+0xC]
	$+F4     008CF4B4     FF83 F83F0000               INC DWORD PTR DS:[EBX+0x3FF8]
	$+FA     008CF4BA     8B55 10                     MOV EDX,DWORD PTR SS:[EBP+0x10]
	$+FD     008CF4BD     8B83 F83F0000               MOV EAX,DWORD PTR DS:[EBX+0x3FF8]
	$+103    008CF4C3     2BD1                        SUB EDX,ECX
	$+105    008CF4C5     51                          PUSH ECX
	$+106    008CF4C6     A3 648CFF00                 MOV DWORD PTR DS:[0xFF8C64],EAX
	$+10B    008CF4CB     E8 8067E3FF                 CALL PathOfEx.00705C50
	$+110    008CF4D0     83C4 04                     ADD ESP,0x4
	$+113    008CF4D3     8D4D 0C                     LEA ECX,DWORD PTR SS:[EBP+0xC]
	$+116    008CF4D6     E8 55ECDEFF                 CALL PathOfEx.006BE130
	$+11B    008CF4DB     833D 608CFF00 00            CMP DWORD PTR DS:[0xFF8C60],0x0
	$+122    008CF4E2     74 40                       JE SHORT PathOfEx.008CF524
	$+124    008CF4E4     8745 50                     XCHG DWORD PTR SS:[EBP+0x50],EAX
	$+127    008CF4E7     8B83 F83F0000               MOV EAX,DWORD PTR DS:[EBX+0x3FF8]
	$+12D    008CF4ED     C705 608CFF00 00000000      MOV DWORD PTR DS:[0xFF8C60],0x0
	$+137    008CF4F7     3B05 648CFF00               CMP EAX,DWORD PTR DS:[0xFF8C64]
	$+13D    008CF4FD     75 17                       JNZ SHORT PathOfEx.008CF516
	$+13F    008CF4FF     8B83 983A0000               MOV EAX,DWORD PTR DS:[EBX+0x3A98]
	$+145    008CF505     8B48 58                     MOV ECX,DWORD PTR DS:[EAX+0x58]
	$+148    008CF508     0FB605 5C8CFF00             MOVZX EAX,BYTE PTR DS:[0xFF8C5C]
	$+14F    008CF50F     50                          PUSH EAX
	$+150    008CF510     51                          PUSH ECX
	$+151    008CF511     E8 3A5E1900                 CALL PathOfEx.00A65350
	$+156    008CF516     FF35 588CFF00               PUSH DWORD PTR DS:[0xFF8C58]
	$+15C    008CF51C     E8 3FF13C00                 CALL PathOfEx.00C9E660
	$+161    008CF521     83C4 04                     ADD ESP,0x4
	$+164    008CF524     8B83 B83C0000               MOV EAX,DWORD PTR DS:[EBX+0x3CB8]
	$+16A    008CF52A     85C0                        TEST EAX,EAX
	$+16C    008CF52C     0F84 8B000000               JE PathOfEx.008CF5BD
	$+172    008CF532     8B40 04                     MOV EAX,DWORD PTR DS:[EAX+0x4]
	$+175    008CF535     8D8B D83C0000               LEA ECX,DWORD PTR DS:[EBX+0x3CD8]
	$+17B    008CF53B     8B40 20                     MOV EAX,DWORD PTR DS:[EAX+0x20]
	$+17E    008CF53E     83C0 10                     ADD EAX,0x10
	$+181    008CF541     50                          PUSH EAX
	$+182    008CF542     E8 E9780100                 CALL PathOfEx.008E6E30
	$+187    008CF547     8B83 B83C0000               MOV EAX,DWORD PTR DS:[EBX+0x3CB8]
	$+18D    008CF54D     66:83B8 80000000 04         CMP WORD PTR DS:[EAX+0x80],0x4
	$+195    008CF555     72 21                       JB SHORT PathOfEx.008CF578
	$+197    008CF557     83EC 18                     SUB ESP,0x18
	$+19A    008CF55A     33C9                        XOR ECX,ECX
	$+19C    008CF55C     66:8988 80000000            MOV WORD PTR DS:[EAX+0x80],CX
	$+1A3    008CF563     8BCC                        MOV ECX,ESP
	$+1A5    008CF565     68 4CFCE600                 PUSH PathOfEx.00E6FC4C
	$+1AA    008CF56A     E8 81E8B9FF                 CALL PathOfEx.0046DDF0
	$+1AF    008CF56F     6A 08                       PUSH 0x8
	$+1B1    008CF571     8BCB                        MOV ECX,EBX
	$+1B3    008CF573     E8 88730000                 CALL PathOfEx.008D6900
	$+1B8    008CF578     8B8B 983B0000               MOV ECX,DWORD PTR DS:[EBX+0x3B98]
	$+1BE    008CF57E     8B83 9C3B0000               MOV EAX,DWORD PTR DS:[EBX+0x3B9C]
	$+1C4    008CF584     81C1 88130000               ADD ECX,0x1388
	$+1CA    008CF58A     83D0 00                     ADC EAX,0x0
	$+1CD    008CF58D     3BF8                        CMP EDI,EAX
	$+1CF    008CF58F     7C 2C                       JL SHORT PathOfEx.008CF5BD
	$+1D1    008CF591     7F 04                       JG SHORT PathOfEx.008CF597
	$+1D3    008CF593     3BF1                        CMP ESI,ECX
	$+1D5    008CF595     72 26                       JB SHORT PathOfEx.008CF5BD
	$+1D7    008CF597     8B8B B83C0000               MOV ECX,DWORD PTR DS:[EBX+0x3CB8]
	$+1DD    008CF59D     89B3 983B0000               MOV DWORD PTR DS:[EBX+0x3B98],ESI
	$+1E3    008CF5A3     89BB 9C3B0000               MOV DWORD PTR DS:[EBX+0x3B9C],EDI
	$+1E9    008CF5A9     0FB781 80000000             MOVZX EAX,WORD PTR DS:[ECX+0x80]
	$+1F0    008CF5B0     66:85C0                     TEST AX,AX
	$+1F3    008CF5B3     74 08                       JE SHORT PathOfEx.008CF5BD
	$+1F5    008CF5B5     48                          DEC EAX
	$+1F6    008CF5B6     66:8981 80000000            MOV WORD PTR DS:[ECX+0x80],AX
	$+1FD    008CF5BD     83BB 343B0000 01            CMP DWORD PTR DS:[EBX+0x3B34],0x1
	$+204    008CF5C4     75 10                       JNZ SHORT PathOfEx.008CF5D6
	$+206    008CF5C6     0FB783 803B0000             MOVZX EAX,WORD PTR DS:[EBX+0x3B80]
	$+20D    008CF5CD     8D4B 14                     LEA ECX,DWORD PTR DS:[EBX+0x14]
	$+210    008CF5D0     50                          PUSH EAX
	$+211    008CF5D1     E8 FADDC2FF                 CALL PathOfEx.004FD3D0
	$+216    008CF5D6     83BB 343B0000 03            CMP DWORD PTR DS:[EBX+0x3B34],0x3
	$+21D    008CF5DD     0F85 9C000000               JNZ PathOfEx.008CF67F
	$+223    008CF5E3     0FB783 BA3B0000             MOVZX EAX,WORD PTR DS:[EBX+0x3BBA]
	$+22A    008CF5EA     B9 FFFF0000                 MOV ECX,0xFFFF
	$+22F    008CF5EF     66:3BC1                     CMP AX,CX
	$+232    008CF5F2     0F84 87000000               JE PathOfEx.008CF67F
	$+238    008CF5F8     8B7D 4C                     MOV EDI,DWORD PTR SS:[EBP+0x4C]
	$+23B    008CF5FB     3BBB C43B0000               CMP EDI,DWORD PTR DS:[EBX+0x3BC4]
	$+241    008CF601     7C 7C                       JL SHORT PathOfEx.008CF67F
	$+243    008CF603     8B75 48                     MOV ESI,DWORD PTR SS:[EBP+0x48]
	$+246    008CF606     7F 08                       JG SHORT PathOfEx.008CF610
	$+248    008CF608     3BB3 C03B0000               CMP ESI,DWORD PTR DS:[EBX+0x3BC0]
	$+24E    008CF60E     72 6F                       JB SHORT PathOfEx.008CF67F
	$+250    008CF610     B9 6E000000                 MOV ECX,0x6E
	$+255    008CF615     C745 20 8C9BEF00            MOV DWORD PTR SS:[EBP+0x20],PathOfEx.00EF9B8C
	$+25C    008CF61C     66:894D 24                  MOV WORD PTR SS:[EBP+0x24],CX
	$+260    008CF620     C745 FC 07000000            MOV DWORD PTR SS:[EBP-0x4],0x7
	$+267    008CF627     66:8945 28                  MOV WORD PTR SS:[EBP+0x28],AX
	$+26B    008CF62B     8A83 B83B0000               MOV AL,BYTE PTR DS:[EBX+0x3BB8]
	$+271    008CF631     8845 2A                     MOV BYTE PTR SS:[EBP+0x2A],AL
	$+274    008CF634     8D45 20                     LEA EAX,DWORD PTR SS:[EBP+0x20]
	$+277    008CF637     50                          PUSH EAX
	$+278    008CF638     FFB3 983A0000               PUSH DWORD PTR DS:[EBX+0x3A98]
	$+27E    008CF63E     E8 BDACDEFF                 CALL PathOfEx.006BA300
	$+283    008CF643     8BC8                        MOV ECX,EAX
	$+285    008CF645     E8 76AD0100                 CALL PathOfEx.008EA3C0
	$+28A    008CF64A     8B8B 983A0000               MOV ECX,DWORD PTR DS:[EBX+0x3A98]
	$+290    008CF650     6A 00                       PUSH 0x0
	$+292    008CF652     E8 295CECFF                 CALL PathOfEx.00795280
	$+297    008CF657     81C6 C8000000               ADD ESI,0xC8
	$+29D    008CF65D     C745 FC FFFFFFFF            MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+2A4    008CF664     B8 FFFF0000                 MOV EAX,0xFFFF
	$+2A9    008CF669     89B3 C03B0000               MOV DWORD PTR DS:[EBX+0x3BC0],ESI
	$+2AF    008CF66F     83D7 00                     ADC EDI,0x0
	$+2B2    008CF672     66:8983 BA3B0000            MOV WORD PTR DS:[EBX+0x3BBA],AX
	$+2B9    008CF679     89BB C43B0000               MOV DWORD PTR DS:[EBX+0x3BC4],EDI
	$+2BF    008CF67F     8B83 183F0000               MOV EAX,DWORD PTR DS:[EBX+0x3F18]
	$+2C5    008CF685     2383 1C3F0000               AND EAX,DWORD PTR DS:[EBX+0x3F1C]
	$+2CB    008CF68B     8B35 DCE2D800               MOV ESI,DWORD PTR DS:[0xD8E2DC]
	$+2D1    008CF691     83F8 FF                     CMP EAX,-0x1
	$+2D4    008CF694     74 71                       JE SHORT PathOfEx.008CF707
	$+2D6    008CF696     8D45 48                     LEA EAX,DWORD PTR SS:[EBP+0x48]
	$+2D9    008CF699     50                          PUSH EAX
	$+2DA    008CF69A     FFD6                        CALL ESI
	$+2DC    008CF69C     FF35 E4F70001               PUSH DWORD PTR DS:[0x100F7E4]
	$+2E2    008CF6A2     FF35 E0F70001               PUSH DWORD PTR DS:[0x100F7E0]
	$+2E8    008CF6A8     FF75 4C                     PUSH DWORD PTR SS:[EBP+0x4C]
	$+2EB    008CF6AB     FF75 48                     PUSH DWORD PTR SS:[EBP+0x48]
	$+2EE    008CF6AE     E8 FDD63A00                 CALL PathOfEx.00C7CDB0
	$+2F3    008CF6B3     3B93 1C3F0000               CMP EDX,DWORD PTR DS:[EBX+0x3F1C]
	$+2F9    008CF6B9     7C 4C                       JL SHORT PathOfEx.008CF707
	$+2FB    008CF6BB     7F 08                       JG SHORT PathOfEx.008CF6C5
	$+2FD    008CF6BD     3B83 183F0000               CMP EAX,DWORD PTR DS:[EBX+0x3F18]
	$+303    008CF6C3     72 42                       JB SHORT PathOfEx.008CF707
	$+305    008CF6C5     FFB3 103F0000               PUSH DWORD PTR DS:[EBX+0x3F10]
	$+30B    008CF6CB     0FB783 0C3F0000             MOVZX EAX,WORD PTR DS:[EBX+0x3F0C]
	$+312    008CF6D2     8D8B 14050000               LEA ECX,DWORD PTR DS:[EBX+0x514]
	$+318    008CF6D8     50                          PUSH EAX
	$+319    008CF6D9     6A 01                       PUSH 0x1
	$+31B    008CF6DB     E8 20C40000                 CALL PathOfEx.008DBB00
	$+320    008CF6E0     33C0                        XOR EAX,EAX
	$+322    008CF6E2     C783 183F0000 FFFFFFFF      MOV DWORD PTR DS:[EBX+0x3F18],-0x1
	$+32C    008CF6EC     C783 1C3F0000 FFFFFFFF      MOV DWORD PTR DS:[EBX+0x3F1C],-0x1
	$+336    008CF6F6     66:8983 0C3F0000            MOV WORD PTR DS:[EBX+0x3F0C],AX
	$+33D    008CF6FD     C783 103F0000 09000000      MOV DWORD PTR DS:[EBX+0x3F10],0x9
	$+347    008CF707     8B83 283F0000               MOV EAX,DWORD PTR DS:[EBX+0x3F28]
	$+34D    008CF70D     2383 2C3F0000               AND EAX,DWORD PTR DS:[EBX+0x3F2C]
	$+353    008CF713     83F8 FF                     CMP EAX,-0x1
	$+356    008CF716     74 71                       JE SHORT PathOfEx.008CF789
	$+358    008CF718     8D45 48                     LEA EAX,DWORD PTR SS:[EBP+0x48]
	$+35B    008CF71B     50                          PUSH EAX
	$+35C    008CF71C     FFD6                        CALL ESI
	$+35E    008CF71E     FF35 E4F70001               PUSH DWORD PTR DS:[0x100F7E4]
	$+364    008CF724     FF35 E0F70001               PUSH DWORD PTR DS:[0x100F7E0]
	$+36A    008CF72A     FF75 4C                     PUSH DWORD PTR SS:[EBP+0x4C]
	$+36D    008CF72D     FF75 48                     PUSH DWORD PTR SS:[EBP+0x48]
	$+370    008CF730     E8 7BD63A00                 CALL PathOfEx.00C7CDB0
	$+375    008CF735     3B93 2C3F0000               CMP EDX,DWORD PTR DS:[EBX+0x3F2C]
	$+37B    008CF73B     7C 4C                       JL SHORT PathOfEx.008CF789
	$+37D    008CF73D     7F 08                       JG SHORT PathOfEx.008CF747
	$+37F    008CF73F     3B83 283F0000               CMP EAX,DWORD PTR DS:[EBX+0x3F28]
	$+385    008CF745     72 42                       JB SHORT PathOfEx.008CF789
	$+387    008CF747     FFB3 243F0000               PUSH DWORD PTR DS:[EBX+0x3F24]
	$+38D    008CF74D     0FB783 203F0000             MOVZX EAX,WORD PTR DS:[EBX+0x3F20]
	$+394    008CF754     8D8B 14050000               LEA ECX,DWORD PTR DS:[EBX+0x514]
	$+39A    008CF75A     50                          PUSH EAX
	$+39B    008CF75B     6A 00                       PUSH 0x0
	$+39D    008CF75D     E8 9EC30000                 CALL PathOfEx.008DBB00
	$+3A2    008CF762     33C0                        XOR EAX,EAX
	$+3A4    008CF764     C783 283F0000 FFFFFFFF      MOV DWORD PTR DS:[EBX+0x3F28],-0x1
	$+3AE    008CF76E     C783 2C3F0000 FFFFFFFF      MOV DWORD PTR DS:[EBX+0x3F2C],-0x1
	$+3B8    008CF778     66:8983 203F0000            MOV WORD PTR DS:[EBX+0x3F20],AX
	$+3BF    008CF77F     C783 243F0000 09000000      MOV DWORD PTR DS:[EBX+0x3F24],0x9
	$+3C9    008CF789     8B4D F4                     MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+3CC    008CF78C     5F                          POP EDI
	$+3CD    008CF78D     5E                          POP ESI
	$+3CE    008CF78E     64:890D 00000000            MOV DWORD PTR FS:[0],ECX
	$+3D5    008CF795     5B                          POP EBX
	$+3D6    008CF796     8D65 40                     LEA ESP,DWORD PTR SS:[EBP+0x40]
	$+3D9    008CF799     5D                          POP EBP
	$+3DA    008CF79A     C2 0C00                     RETN 0xC
	$+3DD    008CF79D     CC                          INT3
	$+3DE    008CF79E     CC                          INT3
	$+3DF    008CF79F     CC                          INT3
	$+3E0    008CF7A0     66:8B4424 04                MOV AX,WORD PTR SS:[ESP+0x4]
	$+3E5    008CF7A5     56                          PUSH ESI
	$+3E6    008CF7A6     8BB1 983A0000               MOV ESI,DWORD PTR DS:[ECX+0x3A98]
	$+3EC    008CF7AC     6A 01                       PUSH 0x1
	$+3EE    008CF7AE     66:8981 803B0000            MOV WORD PTR DS:[ECX+0x3B80],AX
	$+3F5    008CF7B5     C781 343B0000 01000000      MOV DWORD PTR DS:[ECX+0x3B34],0x1
	$+3FF    008CF7BF     FF36                        PUSH DWORD PTR DS:[ESI]
	$+401    008CF7C1     FF15 04E5D800               CALL DWORD PTR DS:[0xD8E504]
	$+407    008CF7C7     C686 B0000000 00            MOV BYTE PTR DS:[ESI+0xB0],0x0
	$+40E    008CF7CE     E8 2DABDEFF                 CALL PathOfEx.006BA300
	$+413    008CF7D3     5E                          POP ESI
	$+414    008CF7D4     C740 08 00000000            MOV DWORD PTR DS:[EAX+0x8],0x0
	$+41B    008CF7DB     8B40 04                     MOV EAX,DWORD PTR DS:[EAX+0x4]
	$+41E    008CF7DE     C740 04 00000000            MOV DWORD PTR DS:[EAX+0x4],0x0
	$+425    008CF7E5     C2 0400                     RETN 0x4
	$+428    008CF7E8     CC                          INT3
	$+429    008CF7E9     CC                          INT3
	$+42A    008CF7EA     CC                          INT3
	$+42B    008CF7EB     CC                          INT3
	$+42C    008CF7EC     CC                          INT3
	$+42D    008CF7ED     CC                          INT3
	$+42E    008CF7EE     CC                          INT3
	$+42F    008CF7EF     CC                          INT3
	$+430    008CF7F0     83E9 00                     SUB ECX,0x0
	$+433    008CF7F3     74 19                       JE SHORT PathOfEx.008CF80E
	$+435    008CF7F5     83E9 01                     SUB ECX,0x1
	$+438    008CF7F8     74 0E                       JE SHORT PathOfEx.008CF808
	$+43A    008CF7FA     83E9 01                     SUB ECX,0x1
	$+43D    008CF7FD     74 06                       JE SHORT PathOfEx.008CF805
	$+43F    008CF7FF     B8 03000000                 MOV EAX,0x3
	$+444    008CF804     C3                          RETN
	$+445    008CF805     33C0                        XOR EAX,EAX
	$+447    008CF807     C3                          RETN
	$+448    008CF808     B8 02000000                 MOV EAX,0x2
	$+44D    008CF80D     C3                          RETN
	$+44E    008CF80E     B8 01000000                 MOV EAX,0x1
	$+453    008CF813     C3                          RETN
	$+454    008CF814     CC                          INT3
	$+455    008CF815     CC                          INT3
	$+456    008CF816     CC                          INT3
	$+457    008CF817     CC                          INT3
	$+458    008CF818     CC                          INT3
	$+459    008CF819     CC                          INT3
	$+45A    008CF81A     CC                          INT3
	$+45B    008CF81B     CC                          INT3
	$+45C    008CF81C     CC                          INT3
	$+45D    008CF81D     CC                          INT3
	$+45E    008CF81E     CC                          INT3
	$+45F    008CF81F     CC                          INT3
	$+460    008CF820     55                          PUSH EBP
	$+461    008CF821     8BEC                        MOV EBP,ESP
	$+463    008CF823     64:A1 00000000              MOV EAX,DWORD PTR FS:[0]
	$+469    008CF829     6A FF                       PUSH -0x1
	$+46B    008CF82B     68 6652D600                 PUSH PathOfEx.00D65266
	$+470    008CF830     50                          PUSH EAX
	$+471    008CF831     64:8925 00000000            MOV DWORD PTR FS:[0],ESP
	$+478    008CF838     83EC 28                     SUB ESP,0x28
	$+47B    008CF83B     8B45 08                     MOV EAX,DWORD PTR SS:[EBP+0x8]
	$+47E    008CF83E     53                          PUSH EBX
	$+47F    008CF83F     8BD9                        MOV EBX,ECX
	$+481    008CF841     56                          PUSH ESI
	$+482    008CF842     57                          PUSH EDI
	$+483    008CF843     85C0                        TEST EAX,EAX
	$+485    008CF845     75 24                       JNZ SHORT PathOfEx.008CF86B
	$+487    008CF847     8D8B 94090000               LEA ECX,DWORD PTR DS:[EBX+0x994]
	$+48D    008CF84D     8983 0C400000               MOV DWORD PTR DS:[EBX+0x400C],EAX
	$+493    008CF853     E8 A842BEFF                 CALL PathOfEx.004B3B00
	$+498    008CF858     8B4D F4                     MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+49B    008CF85B     64:890D 00000000            MOV DWORD PTR FS:[0],ECX
	$+4A2    008CF862     5F                          POP EDI
	$+4A3    008CF863     5E                          POP ESI
	$+4A4    008CF864     5B                          POP EBX
	$+4A5    008CF865     8BE5                        MOV ESP,EBP
	$+4A7    008CF867     5D                          POP EBP
	$+4A8    008CF868     C2 0800                     RETN 0x8
	$+4AB    008CF86B     8B8B 943A0000               MOV ECX,DWORD PTR DS:[EBX+0x3A94]
	$+4B1    008CF871     8945 08                     MOV DWORD PTR SS:[EBP+0x8],EAX
	$+4B4    008CF874     8B01                        MOV EAX,DWORD PTR DS:[ECX]
	$+4B6    008CF876     FF50 14                     CALL DWORD PTR DS:[EAX+0x14]
	$+4B9    008CF879     8BF8                        MOV EDI,EAX
	$+4BB    008CF87B     8D45 08                     LEA EAX,DWORD PTR SS:[EBP+0x8]
	$+4BE    008CF87E     50                          PUSH EAX
	$+4BF    008CF87F     8D45 F0                     LEA EAX,DWORD PTR SS:[EBP-0x10]
	$+4C2    008CF882     50                          PUSH EAX
	$+4C3    008CF883     8D4F 0C                     LEA ECX,DWORD PTR DS:[EDI+0xC]
	$+4C6    008CF886     E8 45E7CDFF                 CALL PathOfEx.005ADFD0
	$+4CB    008CF88B     8B45 F0                     MOV EAX,DWORD PTR SS:[EBP-0x10]
	$+4CE    008CF88E     3B47 0C                     CMP EAX,DWORD PTR DS:[EDI+0xC]
	$+4D1    008CF891     75 18                       JNZ SHORT PathOfEx.008CF8AB
	$+4D3    008CF893     8D45 08                     LEA EAX,DWORD PTR SS:[EBP+0x8]
	$+4D6    008CF896     50                          PUSH EAX
	$+4D7    008CF897     8D45 F0                     LEA EAX,DWORD PTR SS:[EBP-0x10]
	$+4DA    008CF89A     50                          PUSH EAX
	$+4DB    008CF89B     8D4F 14                     LEA ECX,DWORD PTR DS:[EDI+0x14]
	$+4DE    008CF89E     E8 2DE7CDFF                 CALL PathOfEx.005ADFD0
	$+4E3    008CF8A3     8B45 F0                     MOV EAX,DWORD PTR SS:[EBP-0x10]
	$+4E6    008CF8A6     3B47 14                     CMP EAX,DWORD PTR DS:[EDI+0x14]
	$+4E9    008CF8A9     74 0A                       JE SHORT PathOfEx.008CF8B5
	$+4EB    008CF8AB     8B40 14                     MOV EAX,DWORD PTR DS:[EAX+0x14]
	$+4EE    008CF8AE     8945 08                     MOV DWORD PTR SS:[EBP+0x8],EAX
	$+4F1    008CF8B1     85C0                        TEST EAX,EAX
	$+4F3    008CF8B3     75 1A                       JNZ SHORT PathOfEx.008CF8CF
	$+4F5    008CF8B5     8BCB                        MOV ECX,EBX
	$+4F7    008CF8B7     E8 E4920000                 CALL PathOfEx.008D8BA0
	$+4FC    008CF8BC     8B4D F4                     MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+4FF    008CF8BF     64:890D 00000000            MOV DWORD PTR FS:[0],ECX
	$+506    008CF8C6     5F                          POP EDI
	$+507    008CF8C7     5E                          POP ESI
	$+508    008CF8C8     5B                          POP EBX
	$+509    008CF8C9     8BE5                        MOV ESP,EBP
	$+50B    008CF8CB     5D                          POP EBP
	$+50C    008CF8CC     C2 0800                     RETN 0x8
	$+50F    008CF8CF     807D 0C FF                  CMP BYTE PTR SS:[EBP+0xC],0xFF
	$+513    008CF8D3     75 55                       JNZ SHORT PathOfEx.008CF92A
	$+515    008CF8D5     83BB 0C400000 00            CMP DWORD PTR DS:[EBX+0x400C],0x0
	$+51C    008CF8DC     C783 0C400000 01000000      MOV DWORD PTR DS:[EBX+0x400C],0x1
	$+526    008CF8E6     0F9445 0C                   SETE BYTE PTR SS:[EBP+0xC]
	$+52A    008CF8EA     C745 EC 00000000            MOV DWORD PTR SS:[EBP-0x14],0x0
	$+531    008CF8F1     C745 F0 00000000            MOV DWORD PTR SS:[EBP-0x10],0x0
	$+538    008CF8F8     C745 FC 00000000            MOV DWORD PTR SS:[EBP-0x4],0x0
	$+53F    008CF8FF     8D4D EC                     LEA ECX,DWORD PTR SS:[EBP-0x14]
	$+542    008CF902     FF75 0C                     PUSH DWORD PTR SS:[EBP+0xC]
	$+545    008CF905     51                          PUSH ECX
	$+546    008CF906     50                          PUSH EAX
	$+547    008CF907     8D8B 14090000               LEA ECX,DWORD PTR DS:[EBX+0x914]
	$+54D    008CF90D     E8 AE2AD6FF                 CALL PathOfEx.006323C0
	$+552    008CF912     C745 FC 01000000            MOV DWORD PTR SS:[EBP-0x4],0x1
	$+559    008CF919     8B45 EC                     MOV EAX,DWORD PTR SS:[EBP-0x14]
	$+55C    008CF91C     85C0                        TEST EAX,EAX
	$+55E    008CF91E     0F84 0D010000               JE PathOfEx.008CFA31
	$+564    008CF924     50                          PUSH EAX
	$+565    008CF925     E9 F7000000                 JMP PathOfEx.008CFA21
	$+56A    008CF92A     8BC8                        MOV ECX,EAX
	$+56C    008CF92C     E8 2FF2BCFF                 CALL PathOfEx.0049EB60
	$+571    008CF931     83EC 10                     SUB ESP,0x10
	$+574    008CF934     8BF8                        MOV EDI,EAX
	$+576    008CF936     E8 C586E4FF                 CALL PathOfEx.00718000
	$+57B    008CF93B     8BC8                        MOV ECX,EAX
	$+57D    008CF93D     E8 7ED3E9FF                 CALL PathOfEx.0076CCC0
	$+582    008CF942     8BF0                        MOV ESI,EAX
	$+584    008CF944     8975 F0                     MOV DWORD PTR SS:[EBP-0x10],ESI
	$+587    008CF947     C745 FC 02000000            MOV DWORD PTR SS:[EBP-0x4],0x2
	$+58E    008CF94E     8D45 0C                     LEA EAX,DWORD PTR SS:[EBP+0xC]
	$+591    008CF951     8B57 0C                     MOV EDX,DWORD PTR DS:[EDI+0xC]
	$+594    008CF954     8D4D CC                     LEA ECX,DWORD PTR SS:[EBP-0x34]
	$+597    008CF957     50                          PUSH EAX
	$+598    008CF958     83C2 08                     ADD EDX,0x8
	$+59B    008CF95B     E8 E0DC0000                 CALL PathOfEx.008DD640
	$+5A0    008CF960     83C4 04                     ADD ESP,0x4
	$+5A3    008CF963     0FB648 08                   MOVZX ECX,BYTE PTR DS:[EAX+0x8]
	$+5A7    008CF967     894D D8                     MOV DWORD PTR SS:[EBP-0x28],ECX
	$+5AA    008CF96A     8B08                        MOV ECX,DWORD PTR DS:[EAX]
	$+5AC    008CF96C     C700 00000000               MOV DWORD PTR DS:[EAX],0x0
	$+5B2    008CF972     8B40 04                     MOV EAX,DWORD PTR DS:[EAX+0x4]
	$+5B5    008CF975     894D DC                     MOV DWORD PTR SS:[EBP-0x24],ECX
	$+5B8    008CF978     8945 E0                     MOV DWORD PTR SS:[EBP-0x20],EAX
	$+5BB    008CF97B     8D45 D8                     LEA EAX,DWORD PTR SS:[EBP-0x28]
	$+5BE    008CF97E     C645 FC 04                  MOV BYTE PTR SS:[EBP-0x4],0x4
	$+5C2    008CF982     50                          PUSH EAX
	$+5C3    008CF983     8D55 F0                     LEA EDX,DWORD PTR SS:[EBP-0x10]
	$+5C6    008CF986     8D4D E4                     LEA ECX,DWORD PTR SS:[EBP-0x1C]
	$+5C9    008CF989     E8 3257EBFF                 CALL PathOfEx.007850C0
	$+5CE    008CF98E     83C4 04                     ADD ESP,0x4
	$+5D1    008CF991     C645 FC 07                  MOV BYTE PTR SS:[EBP-0x4],0x7
	$+5D5    008CF995     8B45 DC                     MOV EAX,DWORD PTR SS:[EBP-0x24]
	$+5D8    008CF998     85C0                        TEST EAX,EAX
	$+5DA    008CF99A     74 11                       JE SHORT PathOfEx.008CF9AD
	$+5DC    008CF99C     50                          PUSH EAX
	$+5DD    008CF99D     51                          PUSH ECX
	$+5DE    008CF99E     E8 5D86E4FF                 CALL PathOfEx.00718000
	$+5E3    008CF9A3     83C4 04                     ADD ESP,0x4
	$+5E6    008CF9A6     8BC8                        MOV ECX,EAX
	$+5E8    008CF9A8     E8 E381E4FF                 CALL PathOfEx.00717B90
	$+5ED    008CF9AD     C645 FC 09                  MOV BYTE PTR SS:[EBP-0x4],0x9
	$+5F1    008CF9B1     8B45 CC                     MOV EAX,DWORD PTR SS:[EBP-0x34]
	$+5F4    008CF9B4     85C0                        TEST EAX,EAX
	$+5F6    008CF9B6     74 18                       JE SHORT PathOfEx.008CF9D0
	$+5F8    008CF9B8     50                          PUSH EAX
	$+5F9    008CF9B9     51                          PUSH ECX
	$+5FA    008CF9BA     E8 4186E4FF                 CALL PathOfEx.00718000
	$+5FF    008CF9BF     83C4 04                     ADD ESP,0x4
	$+602    008CF9C2     8BC8                        MOV ECX,EAX
	$+604    008CF9C4     E8 C781E4FF                 CALL PathOfEx.00717B90
	$+609    008CF9C9     C745 CC 00000000            MOV DWORD PTR SS:[EBP-0x34],0x0
	$+610    008CF9D0     6A 00                       PUSH 0x0
	$+612    008CF9D2     8D45 E4                     LEA EAX,DWORD PTR SS:[EBP-0x1C]
	$+615    008CF9D5     C645 FC 08                  MOV BYTE PTR SS:[EBP-0x4],0x8
	$+619    008CF9D9     50                          PUSH EAX
	$+61A    008CF9DA     FF75 08                     PUSH DWORD PTR SS:[EBP+0x8]
	$+61D    008CF9DD     8D8B 14090000               LEA ECX,DWORD PTR DS:[EBX+0x914]
	$+623    008CF9E3     C783 0C400000 02000000      MOV DWORD PTR DS:[EBX+0x400C],0x2
	$+62D    008CF9ED     E8 CE29D6FF                 CALL PathOfEx.006323C0
	$+632    008CF9F2     C645 FC 0A                  MOV BYTE PTR SS:[EBP-0x4],0xA
	$+636    008CF9F6     8B45 E4                     MOV EAX,DWORD PTR SS:[EBP-0x1C]
	$+639    008CF9F9     85C0                        TEST EAX,EAX
	$+63B    008CF9FB     74 18                       JE SHORT PathOfEx.008CFA15
	$+63D    008CF9FD     50                          PUSH EAX
	$+63E    008CF9FE     51                          PUSH ECX
	$+63F    008CF9FF     E8 FC85E4FF                 CALL PathOfEx.00718000
	$+644    008CFA04     83C4 04                     ADD ESP,0x4
	$+647    008CFA07     8BC8                        MOV ECX,EAX
	$+649    008CFA09     E8 8281E4FF                 CALL PathOfEx.00717B90
	$+64E    008CFA0E     C745 E4 00000000            MOV DWORD PTR SS:[EBP-0x1C],0x0
	$+655    008CFA15     C745 FC 0B000000            MOV DWORD PTR SS:[EBP-0x4],0xB
	$+65C    008CFA1C     85F6                        TEST ESI,ESI
	$+65E    008CFA1E     74 11                       JE SHORT PathOfEx.008CFA31
	$+660    008CFA20     56                          PUSH ESI
	$+661    008CFA21     51                          PUSH ECX
	$+662    008CFA22     E8 D985E4FF                 CALL PathOfEx.00718000
	$+667    008CFA27     83C4 04                     ADD ESP,0x4
	$+66A    008CFA2A     8BC8                        MOV ECX,EAX
	$+66C    008CFA2C     E8 5F81E4FF                 CALL PathOfEx.00717B90
	$+671    008CFA31     8B4D F4                     MOV ECX,DWORD PTR SS:[EBP-0xC]
	$+674    008CFA34     5F                          POP EDI
	$+675    008CFA35     5E                          POP ESI
	$+676    008CFA36     64:890D 00000000            MOV DWORD PTR FS:[0],ECX
	$+67D    008CFA3D     5B                          POP EBX
	$+67E    008CFA3E     8BE5                        MOV ESP,EBP
	$+680    008CFA40     5D                          POP EBP
	$+681    008CFA41     C2 0800                     RETN 0x8
	$+684    008CFA44     CC                          INT3
	*/

	//gj
	/*
	$-28E    0140DD72     /72 0F                       JB SHORT PathOfEx.0140DD83
	$-28C    0140DD74     |40                          INC EAX
	$-28B    0140DD75     |6A 02                       PUSH 0x2
	$-289    0140DD77     |50                          PUSH EAX
	$-288    0140DD78     |FF75 00                     PUSH DWORD PTR SS:[EBP]
	$-285    0140DD7B     |E8 30B3BAFF                 CALL PathOfEx.00FB90B0
	$-280    0140DD80     |83C4 0C                     ADD ESP,0xC
	$-27D    0140DD83     \8D4D AC                     LEA ECX,DWORD PTR SS:[EBP-0x54]
	$-27A    0140DD86      E8 B558BBFF                 CALL PathOfEx.00FC3640
	$-275    0140DD8B      8D45 AC                     LEA EAX,DWORD PTR SS:[EBP-0x54]
	$-272    0140DD8E      C745 FC 04000000            MOV DWORD PTR SS:[EBP-0x4],0x4
	$-26B    0140DD95      50                          PUSH EAX
	$-26A    0140DD96      C745 AC 440B9701            MOV DWORD PTR SS:[EBP-0x54],PathOfEx.01970B44
	$-263    0140DD9D      E8 D21A3E00                 CALL PathOfEx.017EF874
	$-25E    0140DDA2      C745 FC FFFFFFFF            MOV DWORD PTR SS:[EBP-0x4],-0x1
	$-257    0140DDA9      83C4 04                     ADD ESP,0x4
	$-254    0140DDAC      8B8B 94380000               MOV ECX,DWORD PTR DS:[EBX+0x3894]
	$-24E    0140DDB2      83B9 68010000 03            CMP DWORD PTR DS:[ECX+0x168],0x3
	$-247    0140DDB9      75 2F                       JNZ SHORT PathOfEx.0140DDEA
	$-245    0140DDBB      A1 10A4B701                 MOV EAX,DWORD PTR DS:[0x1B7A410]
	$-240    0140DDC0      83F8 02                     CMP EAX,0x2
	$-23D    0140DDC3      75 0A                       JNZ SHORT PathOfEx.0140DDCF
	$-23B    0140DDC5      837D 3C 5A                  CMP DWORD PTR SS:[EBP+0x3C],0x5A
	$-237    0140DDC9      76 19                       JBE SHORT PathOfEx.0140DDE4
	$-235    0140DDCB      33C0                        XOR EAX,EAX
	$-233    0140DDCD      EB 10                       JMP SHORT PathOfEx.0140DDDF
	$-231    0140DDCF      85C0                        TEST EAX,EAX
	$-22F    0140DDD1      75 11                       JNZ SHORT PathOfEx.0140DDE4
	$-22D    0140DDD3      837D 3C 46                  CMP DWORD PTR SS:[EBP+0x3C],0x46
	$-229    0140DDD7      BA 02000000                 MOV EDX,0x2
	$-224    0140DDDC      0F42C2                      CMOVB EAX,EDX
	$-221    0140DDDF      A3 10A4B701                 MOV DWORD PTR DS:[0x1B7A410],EAX
	$-21C    0140DDE4      8981 68010000               MOV DWORD PTR DS:[ECX+0x168],EAX
	$-216    0140DDEA      8B45 2C                     MOV EAX,DWORD PTR SS:[EBP+0x2C]
	$-213    0140DDED      85C0                        TEST EAX,EAX
	$-211    0140DDEF      74 0F                       JE SHORT PathOfEx.0140DE00
	$-20F    0140DDF1      8BCB                        MOV ECX,EBX
	$-20D    0140DDF3      8983 98380000               MOV DWORD PTR DS:[EBX+0x3898],EAX
	$-207    0140DDF9      E8 F2FCFFFF                 CALL PathOfEx.0140DAF0
	$-202    0140DDFE      EB 16                       JMP SHORT PathOfEx.0140DE16
	$-200    0140DE00      B8 83000000                 MOV EAX,0x83
	$-1FB    0140DE05      C783 34390000 01000000      MOV DWORD PTR DS:[EBX+0x3934],0x1
	$-1F1    0140DE0F      66:8983 80390000            MOV WORD PTR DS:[EBX+0x3980],AX
	$-1EA    0140DE16      8BB3 9C380000               MOV ESI,DWORD PTR DS:[EBX+0x389C]
	$-1E4    0140DE1C      C783 9C380000 00000000      MOV DWORD PTR DS:[EBX+0x389C],0x0
	$-1DA    0140DE26      85F6                        TEST ESI,ESI
	$-1D8    0140DE28      74 12                       JE SHORT PathOfEx.0140DE3C
	$-1D6    0140DE2A      8BCE                        MOV ECX,ESI
	$-1D4    0140DE2C      E8 BF901900                 CALL PathOfEx.015A6EF0
	$-1CF    0140DE31      6A 50                       PUSH 0x50
	$-1CD    0140DE33      56                          PUSH ESI
	$-1CC    0140DE34      E8 783C3F00                 CALL PathOfEx.01801AB1
	$-1C7    0140DE39      83C4 08                     ADD ESP,0x8
	$-1C4    0140DE3C      8B83 D83C0000               MOV EAX,DWORD PTR DS:[EBX+0x3CD8]
	$-1BE    0140DE42      8B75 50                     MOV ESI,DWORD PTR SS:[EBP+0x50]
	$-1BB    0140DE45      85C0                        TEST EAX,EAX
	$-1B9    0140DE47      74 18                       JE SHORT PathOfEx.0140DE61
	$-1B7    0140DE49      3BF0                        CMP ESI,EAX
	$-1B5    0140DE4B      76 0C                       JBE SHORT PathOfEx.0140DE59
	$-1B3    0140DE4D      C783 D83C0000 00000000      MOV DWORD PTR DS:[EBX+0x3CD8],0x0
	$-1A9    0140DE57      EB 08                       JMP SHORT PathOfEx.0140DE61
	$-1A7    0140DE59      2BC6                        SUB EAX,ESI
	$-1A5    0140DE5B      8983 D83C0000               MOV DWORD PTR DS:[EBX+0x3CD8],EAX
	$-19F    0140DE61      8B83 34390000               MOV EAX,DWORD PTR DS:[EBX+0x3934]
	$-199    0140DE67      85C0                        TEST EAX,EAX
	$-197    0140DE69      0F84 3E050000               JE PathOfEx.0140E3AD
	$-191    0140DE6F      83F8 01                     CMP EAX,0x1
	$-18E    0140DE72      0F84 35050000               JE PathOfEx.0140E3AD
	$-188    0140DE78      8B8B 98380000               MOV ECX,DWORD PTR DS:[EBX+0x3898]
	$-182    0140DE7E      E8 CD48ECFF                 CALL PathOfEx.012D2750
	$-17D    0140DE83      FFB3 98380000               PUSH DWORD PTR DS:[EBX+0x3898]
	$-177    0140DE89      8D8B AC380000               LEA ECX,DWORD PTR DS:[EBX+0x38AC]
	$-171    0140DE8F      E8 CC4B1A00                 CALL PathOfEx.015B2A60
	$-16C    0140DE94      83BB 34390000 03            CMP DWORD PTR DS:[EBX+0x3934],0x3
	$-165    0140DE9B      0F85 FD000000               JNZ PathOfEx.0140DF9E
	$-15F    0140DEA1      80BB 0E3E0000 00            CMP BYTE PTR DS:[EBX+0x3E0E],0x0
	$-158    0140DEA8      74 13                       JE SHORT PathOfEx.0140DEBD
	$-156    0140DEAA      8B83 94380000               MOV EAX,DWORD PTR DS:[EBX+0x3894]
	$-150    0140DEB0      83B8 68010000 02            CMP DWORD PTR DS:[EAX+0x168],0x2
	$-149    0140DEB7      0F85 E1000000               JNZ PathOfEx.0140DF9E
	$-143    0140DEBD      66:0F6EC6                   MOVD MM0,ESI
	$-13F    0140DEC1      8BC6                        MOV EAX,ESI
	$-13D    0140DEC3      F30FE6C0                    CVTDQ2PD XMM0,XMM0
	$-139    0140DEC7      C1E8 1F                     SHR EAX,0x1F
	$-136    0140DECA      8B8B 94380000               MOV ECX,DWORD PTR DS:[EBX+0x3894]
	$-130    0140DED0      51                          PUSH ECX
	$-12F    0140DED1      8B11                        MOV EDX,DWORD PTR DS:[ECX]
	$-12D    0140DED3      F20F5804C5 5015AD01         ADDSD XMM0,QWORD PTR DS:[EAX*8+0x1AD1550]
	$-124    0140DEDC      660F5AC0                    CVTPD2PS XMM0,XMM0
	$-120    0140DEE0      F3:0F5E05 CC0BAD01          DIVSS XMM0,DWORD PTR DS:[0x1AD0BCC]
	$-118    0140DEE8      F3:0F110424                 MOVSS DWORD PTR SS:[ESP],XMM0
	$-113    0140DEED      FF52 18                     CALL DWORD PTR DS:[EDX+0x18]
	$-110    0140DEF0      8BB3 C03C0000               MOV ESI,DWORD PTR DS:[EBX+0x3CC0]
	$-10A    0140DEF6      8975 38                     MOV DWORD PTR SS:[EBP+0x38],ESI
	$-107    0140DEF9      3BB3 C43C0000               CMP ESI,DWORD PTR DS:[EBX+0x3CC4]
	$-101    0140DEFF      0F84 99000000               JE PathOfEx.0140DF9E
	$-FB     0140DF05      8B4D 50                     MOV ECX,DWORD PTR SS:[EBP+0x50]
	$-F8     0140DF08      8D7E 30                     LEA EDI,DWORD PTR DS:[ESI+0x30]
	$-F5     0140DF0B      897D 3C                     MOV DWORD PTR SS:[EBP+0x3C],EDI
	$-F2     0140DF0E      66:90                       NOP
	$-F0     0140DF10      8B06                        MOV EAX,DWORD PTR DS:[ESI]
	$-EE     0140DF12      3BC1                        CMP EAX,ECX
	$-EC     0140DF14      77 6C                       JA SHORT PathOfEx.0140DF82
	$-EA     0140DF16      8B4F FC                     MOV ECX,DWORD PTR DS:[EDI-0x4]
	$-E7     0140DF19      85C9                        TEST ECX,ECX
	$-E5     0140DF1B      0F84 28010000               JE PathOfEx.0140E049
	$-DF     0140DF21      8B01                        MOV EAX,DWORD PTR DS:[ECX]
	$-DD     0140DF23      FF50 08                     CALL DWORD PTR DS:[EAX+0x8]
	$-DA     0140DF26      8B8B C43C0000               MOV ECX,DWORD PTR DS:[EBX+0x3CC4]
	$-D4     0140DF2C      8B45 3C                     MOV EAX,DWORD PTR SS:[EBP+0x3C]
	$-D1     0140DF2F      894D 2C                     MOV DWORD PTR SS:[EBP+0x2C],ECX
	$-CE     0140DF32      3BC1                        CMP EAX,ECX
	$-CC     0140DF34      74 28                       JE SHORT PathOfEx.0140DF5E
	$-CA     0140DF36      8BDE                        MOV EBX,ESI
	$-C8     0140DF38      8BF1                        MOV ESI,ECX
	$-C6     0140DF3A      2BD8                        SUB EBX,EAX
	$-C4     0140DF3C      0F1F40 00                   NOP DWORD PTR DS:[EAX]
	$-C0     0140DF40      8B07                        MOV EAX,DWORD PTR DS:[EDI]
	$-BE     0140DF42      89041F                      MOV DWORD PTR DS:[EDI+EBX],EAX
	$-BB     0140DF45      8D47 08                     LEA EAX,DWORD PTR DS:[EDI+0x8]
	$-B8     0140DF48      50                          PUSH EAX
	$-B7     0140DF49      8D0C18                      LEA ECX,DWORD PTR DS:[EAX+EBX]
	$-B4     0140DF4C      E8 FFECBAFF                 CALL PathOfEx.00FBCC50
	$-AF     0140DF51      83C7 30                     ADD EDI,0x30
	$-AC     0140DF54      3BFE                        CMP EDI,ESI
	$-AA     0140DF56    ^ 75 E8                       JNZ SHORT PathOfEx.0140DF40
	$-A8     0140DF58      8B75 38                     MOV ESI,DWORD PTR SS:[EBP+0x38]
	$-A5     0140DF5B      8B5D 1C                     MOV EBX,DWORD PTR SS:[EBP+0x1C]
	$-A2     0140DF5E      8B93 C43C0000               MOV EDX,DWORD PTR DS:[EBX+0x3CC4]
	$-9C     0140DF64      FF75 50                     PUSH DWORD PTR SS:[EBP+0x50]
	$-99     0140DF67      8D4A D0                     LEA ECX,DWORD PTR DS:[EDX-0x30]
	$-96     0140DF6A      51                          PUSH ECX
	$-95     0140DF6B      E8 F0A1BEFF                 CALL PathOfEx.00FF8160
	$-90     0140DF70      8B7D 3C                     MOV EDI,DWORD PTR SS:[EBP+0x3C]
	$-8D     0140DF73      83C4 08                     ADD ESP,0x8
	$-8A     0140DF76      8383 C43C0000 D0            ADD DWORD PTR DS:[EBX+0x3CC4],-0x30
	$-83     0140DF7D      8B4D 50                     MOV ECX,DWORD PTR SS:[EBP+0x50]
	$-80     0140DF80      EB 10                       JMP SHORT PathOfEx.0140DF92
	$-7E     0140DF82      2BC1                        SUB EAX,ECX
	$-7C     0140DF84      8906                        MOV DWORD PTR DS:[ESI],EAX
	$-7A     0140DF86      83C6 30                     ADD ESI,0x30
	$-77     0140DF89      83C7 30                     ADD EDI,0x30
	$-74     0140DF8C      8975 38                     MOV DWORD PTR SS:[EBP+0x38],ESI
	$-71     0140DF8F      897D 3C                     MOV DWORD PTR SS:[EBP+0x3C],EDI
	$-6E     0140DF92      3BB3 C43C0000               CMP ESI,DWORD PTR DS:[EBX+0x3CC4]
	$-68     0140DF98    ^ 0F85 72FFFFFF               JNZ PathOfEx.0140DF10
	$-62     0140DF9E      8D45 18                     LEA EAX,DWORD PTR SS:[EBP+0x18]
	$-5F     0140DFA1      50                          PUSH EAX
	$-5E     0140DFA2      FF15 EC729101               CALL DWORD PTR DS:[<&KERNEL32.QueryPerformanceCounter>]    ; kernel32.QueryPerformanceCounter
	$-58     0140DFA8      FF35 3403BB01               PUSH DWORD PTR DS:[0x1BB0334]
	$-52     0140DFAE      FF35 3003BB01               PUSH DWORD PTR DS:[0x1BB0330]
	$-4C     0140DFB4      FF75 1C                     PUSH DWORD PTR SS:[EBP+0x1C]
	$-49     0140DFB7      FF75 18                     PUSH DWORD PTR SS:[EBP+0x18]
	$-46     0140DFBA      E8 01453F00                 CALL PathOfEx.018024C0
	$-41     0140DFBF      83BB 34390000 03            CMP DWORD PTR DS:[EBX+0x3934],0x3
	$-3A     0140DFC6      8BF0                        MOV ESI,EAX
	$-38     0140DFC8      8BFA                        MOV EDI,EDX
	$-36     0140DFCA      0F85 84010000               JNZ PathOfEx.0140E154
	$-30     0140DFD0      80BB 90390000 00            CMP BYTE PTR DS:[EBX+0x3990],0x0
	$-29     0140DFD7      8B8B 88390000               MOV ECX,DWORD PTR DS:[EBX+0x3988]
	$-23     0140DFDD      8B83 8C390000               MOV EAX,DWORD PTR DS:[EBX+0x398C]
	$-1D     0140DFE3      74 69                       JE SHORT PathOfEx.0140E04E
	$-1B     0140DFE5      81C1 D0070000               ADD ECX,0x7D0
	$-15     0140DFEB      83D0 00                     ADC EAX,0x0
	$-12     0140DFEE      3BF8                        CMP EDI,EAX
	$-10     0140DFF0      0F8C C5000000               JL PathOfEx.0140E0BB
	$-A      0140DFF6      7F 08                       JG SHORT PathOfEx.0140E000
	$-8      0140DFF8      3BF1                        CMP ESI,ECX
	$-6      0140DFFA      0F82 BB000000               JB PathOfEx.0140E0BB                                       ; 这个函数估计是个处理timer的函数
	$ ==>    0140E000      B8 0C000000                 MOV EAX,0xC                                                ; 此处会发0x1和0xc的包，此处ret掉，调试断住再久都不掉线了，0x1这个包很可能有检测之类的东西
	$+5      0140E005      C745 30 4083A801            MOV DWORD PTR SS:[EBP+0x30],PathOfEx.01A88340
	$+C      0140E00C      66:8945 34                  MOV WORD PTR SS:[EBP+0x34],AX
	$+10     0140E010      C745 FC 05000000            MOV DWORD PTR SS:[EBP-0x4],0x5
	$+17     0140E017      8D4D 30                     LEA ECX,DWORD PTR SS:[EBP+0x30]
	$+1A     0140E01A      FFB3 98380000               PUSH DWORD PTR DS:[EBX+0x3898]
	$+20     0140E020      C745 38 00000000            MOV DWORD PTR SS:[EBP+0x38],0x0
	$+27     0140E027      E8 34872100                 CALL PathOfEx.01626760
	$+2C     0140E02C      8B8B 98380000               MOV ECX,DWORD PTR DS:[EBX+0x3898]
	$+32     0140E032      6A 00                       PUSH 0x0
	$+34     0140E034      E8 A74AECFF                 CALL PathOfEx.012D2AE0
	$+39     0140E039      C745 FC FFFFFFFF            MOV DWORD PTR SS:[EBP-0x4],-0x1                            ; return到此处
	$+40     0140E040      C745 30 B883A801            MOV DWORD PTR SS:[EBP+0x30],PathOfEx.01A883B8
	$+47     0140E047      EB 66                       JMP SHORT PathOfEx.0140E0AF
	$+49     0140E049      E8 12173E00                 CALL PathOfEx.017EF760
	$+4E     0140E04E      81C1 F4010000               ADD ECX,0x1F4
	$+54     0140E054      83D0 00                     ADC EAX,0x0
	$+57     0140E057      3BF8                        CMP EDI,EAX
	$+59     0140E059      7C 60                       JL SHORT PathOfEx.0140E0BB
	$+5B     0140E05B      7F 04                       JG SHORT PathOfEx.0140E061
	$+5D     0140E05D      3BF1                        CMP ESI,ECX
	$+5F     0140E05F      72 5A                       JB SHORT PathOfEx.0140E0BB
	$+61     0140E061      B8 0C000000                 MOV EAX,0xC                                                ; 此处只发0xc的包，估计是个心跳包，只有一个dword字段，还一直为0，但是经过两个函数的加密
	$+66     0140E066      C745 20 4083A801            MOV DWORD PTR SS:[EBP+0x20],PathOfEx.01A88340
	$+6D     0140E06D      66:8945 24                  MOV WORD PTR SS:[EBP+0x24],AX
	$+71     0140E071      C745 FC 06000000            MOV DWORD PTR SS:[EBP-0x4],0x6
	$+78     0140E078      8D4D 20                     LEA ECX,DWORD PTR SS:[EBP+0x20]
	$+7B     0140E07B      FFB3 98380000               PUSH DWORD PTR DS:[EBX+0x3898]
	$+81     0140E081      C745 28 00000000            MOV DWORD PTR SS:[EBP+0x28],0x0
	$+88     0140E088      E8 D3862100                 CALL PathOfEx.01626760
	$+8D     0140E08D      8B8B 98380000               MOV ECX,DWORD PTR DS:[EBX+0x3898]
	$+93     0140E093      6A 00                       PUSH 0x0
	$+95     0140E095      E8 464AECFF                 CALL PathOfEx.012D2AE0
	$+9A     0140E09A      C745 FC FFFFFFFF            MOV DWORD PTR SS:[EBP-0x4],-0x1                            ; return到此处
	$+A1     0140E0A1      C683 90390000 01            MOV BYTE PTR DS:[EBX+0x3990],0x1
	$+A8     0140E0A8      C745 20 B883A801            MOV DWORD PTR SS:[EBP+0x20],PathOfEx.01A883B8
	$+AF     0140E0AF      89BB 8C390000               MOV DWORD PTR DS:[EBX+0x398C],EDI
	$+B5     0140E0B5      89B3 88390000               MOV DWORD PTR DS:[EBX+0x3988],ESI
	$+BB     0140E0BB      8B93 98380000               MOV EDX,DWORD PTR DS:[EBX+0x3898]
	$+C1     0140E0C1      837A 58 00                  CMP DWORD PTR DS:[EDX+0x58],0x0
	$+C5     0140E0C5      74 44                       JE SHORT PathOfEx.0140E10B
	$+C7     0140E0C7      8B4A 58                     MOV ECX,DWORD PTR DS:[EDX+0x58]
	$+CA     0140E0CA      8B81 30010000               MOV EAX,DWORD PTR DS:[ECX+0x130]
	$+D0     0140E0D0      3B81 34010000               CMP EAX,DWORD PTR DS:[ECX+0x134]
	$+D6     0140E0D6      74 33                       JE SHORT PathOfEx.0140E10B
	$+D8     0140E0D8      8D45 0C                     LEA EAX,DWORD PTR SS:[EBP+0xC]
	$+DB     0140E0DB      50                          PUSH EAX
	$+DC     0140E0DC      E8 5F4F1A00                 CALL PathOfEx.015B3040
	$+E1     0140E0E1      8B4D 0C                     MOV ECX,DWORD PTR SS:[EBP+0xC]
	$+E4     0140E0E4      FF83 003E0000               INC DWORD PTR DS:[EBX+0x3E00]
	$+EA     0140E0EA      8B55 10                     MOV EDX,DWORD PTR SS:[EBP+0x10]
	$+ED     0140E0ED      8B83 003E0000               MOV EAX,DWORD PTR DS:[EBX+0x3E00]
	$+F3     0140E0F3      2BD1                        SUB EDX,ECX
	$+F5     0140E0F5      51                          PUSH ECX
	$+F6     0140E0F6      A3 00E0B801                 MOV DWORD PTR DS:[0x1B8E000],EAX
	$+FB     0140E0FB      E8 402BE3FF                 CALL PathOfEx.01240C40
	$+100    0140E100      83C4 04                     ADD ESP,0x4
	$+103    0140E103      8D4D 0C                     LEA ECX,DWORD PTR SS:[EBP+0xC]
	$+106    0140E106      E8 C5E0E0FF                 CALL PathOfEx.0121C1D0
	$+10B    0140E10B      833D 04E0B801 00            CMP DWORD PTR DS:[0x1B8E004],0x0
	$+112    0140E112      74 40                       JE SHORT PathOfEx.0140E154
	$+114    0140E114      8745 50                     XCHG DWORD PTR SS:[EBP+0x50],EAX
	$+117    0140E117      8B83 003E0000               MOV EAX,DWORD PTR DS:[EBX+0x3E00]
	$+11D    0140E11D      C705 04E0B801 00000000      MOV DWORD PTR DS:[0x1B8E004],0x0
	$+127    0140E127      3B05 00E0B801               CMP EAX,DWORD PTR DS:[0x1B8E000]
	$+12D    0140E12D      75 17                       JNZ SHORT PathOfEx.0140E146
	$+12F    0140E12F      8B83 98380000               MOV EAX,DWORD PTR DS:[EBX+0x3898]
	$+135    0140E135      8B48 58                     MOV ECX,DWORD PTR DS:[EAX+0x58]
	$+138    0140E138      0FB605 F8DFB801             MOVZX EAX,BYTE PTR DS:[0x1B8DFF8]
	$+13F    0140E13F      50                          PUSH EAX
	$+140    0140E140      51                          PUSH ECX
	$+141    0140E141      E8 5A4E1A00                 CALL PathOfEx.015B2FA0
	$+146    0140E146      FF35 FCDFB801               PUSH DWORD PTR DS:[0x1B8DFFC]
	$+14C    0140E14C      E8 6F5C4100                 CALL PathOfEx.01823DC0
	$+151    0140E151      83C4 04                     ADD ESP,0x4
	$+154    0140E154      8B83 B83A0000               MOV EAX,DWORD PTR DS:[EBX+0x3AB8]
	$+15A    0140E15A      85C0                        TEST EAX,EAX
	$+15C    0140E15C      0F84 8B000000               JE PathOfEx.0140E1ED
	$+162    0140E162      8B40 04                     MOV EAX,DWORD PTR DS:[EAX+0x4]
	$+165    0140E165      8D8B D83A0000               LEA ECX,DWORD PTR DS:[EBX+0x3AD8]
	$+16B    0140E16B      8B40 20                     MOV EAX,DWORD PTR DS:[EAX+0x20]
	$+16E    0140E16E      83C0 10                     ADD EAX,0x10
	$+171    0140E171      50                          PUSH EAX
	$+172    0140E172      E8 39AB0100                 CALL PathOfEx.01428CB0
	$+177    0140E177      8B83 B83A0000               MOV EAX,DWORD PTR DS:[EBX+0x3AB8]
	$+17D    0140E17D      66:83B8 80000000 04         CMP WORD PTR DS:[EAX+0x80],0x4
	$+185    0140E185      72 21                       JB SHORT PathOfEx.0140E1A8
	$+187    0140E187      83EC 18                     SUB ESP,0x18
	$+18A    0140E18A      33C9                        XOR ECX,ECX
	$+18C    0140E18C      66:8988 80000000            MOV WORD PTR DS:[EAX+0x80],CX
	$+193    0140E193      8BCC                        MOV ECX,ESP
	$+195    0140E195      68 B80DA001                 PUSH PathOfEx.01A00DB8
	$+19A    0140E19A      E8 F1C4BAFF                 CALL PathOfEx.00FBA690
	$+19F    0140E19F      6A 08                       PUSH 0x8
	$+1A1    0140E1A1      8BCB                        MOV ECX,EBX
	$+1A3    0140E1A3      E8 286D0000                 CALL PathOfEx.01414ED0
	$+1A8    0140E1A8      8B8B 98390000               MOV ECX,DWORD PTR DS:[EBX+0x3998]
	$+1AE    0140E1AE      8B83 9C390000               MOV EAX,DWORD PTR DS:[EBX+0x399C]
	$+1B4    0140E1B4      81C1 88130000               ADD ECX,0x1388
	$+1BA    0140E1BA      83D0 00                     ADC EAX,0x0
	$+1BD    0140E1BD      3BF8                        CMP EDI,EAX
	$+1BF    0140E1BF      7C 2C                       JL SHORT PathOfEx.0140E1ED
	$+1C1    0140E1C1      7F 04                       JG SHORT PathOfEx.0140E1C7
	$+1C3    0140E1C3      3BF1                        CMP ESI,ECX
	$+1C5    0140E1C5      72 26                       JB SHORT PathOfEx.0140E1ED
	$+1C7    0140E1C7      8B8B B83A0000               MOV ECX,DWORD PTR DS:[EBX+0x3AB8]
	$+1CD    0140E1CD      89B3 98390000               MOV DWORD PTR DS:[EBX+0x3998],ESI
	$+1D3    0140E1D3      89BB 9C390000               MOV DWORD PTR DS:[EBX+0x399C],EDI
	$+1D9    0140E1D9      0FB781 80000000             MOVZX EAX,WORD PTR DS:[ECX+0x80]
	$+1E0    0140E1E0      66:85C0                     TEST AX,AX
	$+1E3    0140E1E3      74 08                       JE SHORT PathOfEx.0140E1ED
	$+1E5    0140E1E5      48                          DEC EAX
	$+1E6    0140E1E6      66:8981 80000000            MOV WORD PTR DS:[ECX+0x80],AX
	$+1ED    0140E1ED      83BB 34390000 01            CMP DWORD PTR DS:[EBX+0x3934],0x1
	$+1F4    0140E1F4      75 10                       JNZ SHORT PathOfEx.0140E206
	$+1F6    0140E1F6      0FB783 80390000             MOVZX EAX,WORD PTR DS:[EBX+0x3980]
	$+1FD    0140E1FD      8D4B 14                     LEA ECX,DWORD PTR DS:[EBX+0x14]
	$+200    0140E200      50                          PUSH EAX
	$+201    0140E201      E8 7ACEC3FF                 CALL PathOfEx.0104B080
	$+206    0140E206      83BB 34390000 03            CMP DWORD PTR DS:[EBX+0x3934],0x3
	$+20D    0140E20D      0F85 90000000               JNZ PathOfEx.0140E2A3
	$+213    0140E213      0FB783 BA390000             MOVZX EAX,WORD PTR DS:[EBX+0x39BA]
	$+21A    0140E21A      B9 FFFF0000                 MOV ECX,0xFFFF
	$+21F    0140E21F      66:3BC1                     CMP AX,CX
	$+222    0140E222      74 7F                       JE SHORT PathOfEx.0140E2A3
	$+224    0140E224      8B7D 4C                     MOV EDI,DWORD PTR SS:[EBP+0x4C]
	$+227    0140E227      3BBB C4390000               CMP EDI,DWORD PTR DS:[EBX+0x39C4]
	$+22D    0140E22D      7C 74                       JL SHORT PathOfEx.0140E2A3
	$+22F    0140E22F      8B75 48                     MOV ESI,DWORD PTR SS:[EBP+0x48]
	$+232    0140E232      7F 08                       JG SHORT PathOfEx.0140E23C
	$+234    0140E234      3BB3 C0390000               CMP ESI,DWORD PTR DS:[EBX+0x39C0]
	$+23A    0140E23A      72 67                       JB SHORT PathOfEx.0140E2A3
	$+23C    0140E23C      B9 6B000000                 MOV ECX,0x6B
	$+241    0140E241      C745 20 DC82A801            MOV DWORD PTR SS:[EBP+0x20],PathOfEx.01A882DC
	$+248    0140E248      66:894D 24                  MOV WORD PTR SS:[EBP+0x24],CX
	$+24C    0140E24C      C745 FC 07000000            MOV DWORD PTR SS:[EBP-0x4],0x7
	$+253    0140E253      8D4D 20                     LEA ECX,DWORD PTR SS:[EBP+0x20]
	$+256    0140E256      FFB3 98380000               PUSH DWORD PTR DS:[EBX+0x3898]
	$+25C    0140E25C      66:8945 28                  MOV WORD PTR SS:[EBP+0x28],AX
	$+260    0140E260      8A83 B8390000               MOV AL,BYTE PTR DS:[EBX+0x39B8]
	$+266    0140E266      8845 2A                     MOV BYTE PTR SS:[EBP+0x2A],AL
	$+269    0140E269      E8 A2B8FFFF                 CALL PathOfEx.01409B10
	$+26E    0140E26E      8B8B 98380000               MOV ECX,DWORD PTR DS:[EBX+0x3898]
	$+274    0140E274      6A 00                       PUSH 0x0
	$+276    0140E276      E8 6548ECFF                 CALL PathOfEx.012D2AE0
	$+27B    0140E27B      81C6 C8000000               ADD ESI,0xC8
	$+281    0140E281      C745 FC FFFFFFFF            MOV DWORD PTR SS:[EBP-0x4],-0x1
	$+288    0140E288      B8 FFFF0000                 MOV EAX,0xFFFF
	$+28D    0140E28D      89B3 C0390000               MOV DWORD PTR DS:[EBX+0x39C0],ESI
	$+293    0140E293      83D7 00                     ADC EDI,0x0
	$+296    0140E296      66:8983 BA390000            MOV WORD PTR DS:[EBX+0x39BA],AX
	$+29D    0140E29D      89BB C4390000               MOV DWORD PTR DS:[EBX+0x39C4],EDI
	$+2A3    0140E2A3      8B83 203D0000               MOV EAX,DWORD PTR DS:[EBX+0x3D20]
	$+2A9    0140E2A9      2383 243D0000               AND EAX,DWORD PTR DS:[EBX+0x3D24]
	$+2AF    0140E2AF      8B35 EC729101               MOV ESI,DWORD PTR DS:[<&KERNEL32.QueryPerformanceCounter>] ; kernel32.QueryPerformanceCounter
	$+2B5    0140E2B5      83F8 FF                     CMP EAX,-0x1
	$+2B8    0140E2B8      74 71                       JE SHORT PathOfEx.0140E32B
	$+2BA    0140E2BA      8D45 48                     LEA EAX,DWORD PTR SS:[EBP+0x48]
	$+2BD    0140E2BD      50                          PUSH EAX
	$+2BE    0140E2BE      FFD6                        CALL ESI
	$+2C0    0140E2C0      FF35 3403BB01               PUSH DWORD PTR DS:[0x1BB0334]
	$+2C6    0140E2C6      FF35 3003BB01               PUSH DWORD PTR DS:[0x1BB0330]
	$+2CC    0140E2CC      FF75 4C                     PUSH DWORD PTR SS:[EBP+0x4C]
	$+2CF    0140E2CF      FF75 48                     PUSH DWORD PTR SS:[EBP+0x48]
	$+2D2    0140E2D2      E8 E9413F00                 CALL PathOfEx.018024C0
	$+2D7    0140E2D7      3B93 243D0000               CMP EDX,DWORD PTR DS:[EBX+0x3D24]
	$+2DD    0140E2DD      7C 4C                       JL SHORT PathOfEx.0140E32B
	$+2DF    0140E2DF      7F 08                       JG SHORT PathOfEx.0140E2E9
	$+2E1    0140E2E1      3B83 203D0000               CMP EAX,DWORD PTR DS:[EBX+0x3D20]
	$+2E7    0140E2E7      72 42                       JB SHORT PathOfEx.0140E32B
	$+2E9    0140E2E9      FFB3 1C3D0000               PUSH DWORD PTR DS:[EBX+0x3D1C]
	$+2EF    0140E2EF      0FB783 183D0000             MOVZX EAX,WORD PTR DS:[EBX+0x3D18]
	$+2F6    0140E2F6      8D8B 94040000               LEA ECX,DWORD PTR DS:[EBX+0x494]
	$+2FC    0140E2FC      50                          PUSH EAX
	$+2FD    0140E2FD      6A 01                       PUSH 0x1
	$+2FF    0140E2FF      E8 7CB50000                 CALL PathOfEx.01419880
	$+304    0140E304      33C0                        XOR EAX,EAX
	*/
#ifdef VER_GJ_
	addr_tmp = scan_exe_.ScanAddr("b8????????c7????????????66??????c7????????????8d????ff??????????c7????????????e8????????8b??????????6a??e8????????c7????????????c7????????????eb??e8????????81??????????83????3b??7c??7f??3b??72??b8????????c7????????????66??????c7????????????8d????ff??????????c7????????????e8????????8b??????????6a??e8????????c7", 0);
#else
	addr_tmp = scan_exe_.ScanAddr("b8????????c7????????????66??????8d????c7????????????50ff??????????c7????????????e8????????8b??e8????????8b??????????6a??e8????????c7????????????c7????????????eb??e8????????81??????????83????3b??7c??7f??3b??72??b8????????c7????????????66??????8d????c7????????????50ff??????????c7????????????e8????????8b??e8????????8b??????????6a??e8????????c7????????????c6", 0);
#endif
	if (!addr_tmp)
	{
		assert(false);
		return false;
	}
	ofs << "pt_cdword	g_p_ret_0x1_0xf_check_packet_ = " << ExeAddr2Offset(addr_tmp) << ";" << FC_OFstream_EndlLog;
	return true;
}
