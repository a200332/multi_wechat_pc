// WeChatResource.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "util.h"

#define WECHATRESOURCE TEXT("WeChatResource.dll.1")
#define WECHATWINDLL TEXT("WeChatWin.dll")

const TCHAR* g_Supported_wx_Version[] = {
	TEXT("2.6.5.38"),
};


bool IsSupportedWxVersion()
{
	TCHAR tszDllPath[MAX_PATH] = { 0 };

	GetModuleFileName(NULL, tszDllPath, MAX_PATH);
	PathRemoveFileSpec(tszDllPath);
	PathAppend(tszDllPath, WECHATWINDLL);

	TCHAR version[100] = { 0 };
	if (!GetFileVersion(tszDllPath, version))
	{
		return false;
	}

	for (int i = 0; i < ARRAYSIZE(g_Supported_wx_Version); i++) {
		if (!_tcsicmp(g_Supported_wx_Version[i], version)) {
			return true;
		}
	}

	return false;
}


/* //2.6.5.38
text:10247EF1 8A 45 F3                                      mov     al, [ebp+var_D]
*/

bool FakeRevokeMsg()
{
	if (!IsSupportedWxVersion()) {
		return false;
	}

	//33 C0                xor eax,eax 
	BYTE code[] = { 0x33, 0xc0, 0x90 };
	HMODULE hMod = GetModuleHandle(WECHATWINDLL);
	DWORD offset = 0x247EF1;
	if (!hMod) {
		return false;
	}

	PVOID addr = (BYTE*)hMod + offset;
	Patch(addr, 3, code);

	return true;
}

void RestoreRevokeMsg()
{
	if (!IsSupportedWxVersion()) {
		return;
	}

	BYTE code[] = { 0x8A, 0x45, 0xF3 };
	HMODULE hMod = GetModuleHandle(WECHATWINDLL);
	DWORD offset = 0x247EF1;
	if (!hMod) {
		return;
	}

	PVOID addr = (BYTE*)hMod + offset;
	Patch(addr, 3, code);
}