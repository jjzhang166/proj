#include "jfzrstruct.h"

extern CString	g_csPrintFbVillage;

void FbLevelPrint(PFB_LEVEL ptheFbObj,DWORD lpSubTreeRoot);
void FbLevelTraverse( DWORD lpSubTreeRoot);
void ToFbVillageTraverse();


// 当前角色所在地图id和等级
extern CString	g_csPrintMapData;
bool UpDataMap();
