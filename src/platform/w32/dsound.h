//
// Created by exozg on 24/01/2025.
//
#pragma once

#pragma pack(1)

FARPROC p[12] = {nullptr};

void exports(HMODULE hL){
    p[0] = GetProcAddress(hL, "DirectSoundCreate");
    p[1] = GetProcAddress(hL, "DirectSoundEnumerateA");
    p[2] = GetProcAddress(hL, "DirectSoundEnumerateW");
    p[3] = GetProcAddress(hL, "DllCanUnloadNow");
    p[4] = GetProcAddress(hL, "DllGetClassObject");
    p[5] = GetProcAddress(hL, "DirectSoundCaptureCreate");
    p[6] = GetProcAddress(hL, "DirectSoundCaptureEnumerateA");
    p[7] = GetProcAddress(hL, "DirectSoundCaptureEnumerateW");
    p[8] = GetProcAddress(hL, "GetDeviceID");
    p[9] = GetProcAddress(hL, "DirectSoundFullDuplexCreate");
    p[11] = GetProcAddress(hL, "DirectSoundCaptureCreate8");
    p[10] = GetProcAddress(hL, "DirectSoundCreate8");
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall __E__0__()
{
	__asm
	{
		jmp p[0*4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall __E__1__()
{
	__asm
	{
		jmp p[1*4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall __E__2__()
{
	__asm
	{
		jmp p[2*4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall __E__3__()
{
	__asm
	{
		jmp p[3*4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall __E__4__()
{
	__asm
	{
		jmp p[4*4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall __E__5__()
{
	__asm
	{
		jmp p[5*4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall __E__6__()
{
	__asm
	{
		jmp p[6*4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall __E__7__()
{
	__asm
	{
		jmp p[7*4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall __E__8__()
{
	__asm
	{
		jmp p[8*4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall __E__9__()
{
	__asm
	{
		jmp p[9*4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall __E__10__()
{
	__asm
	{
		jmp p[10*4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall __E__11__()
{
	__asm
	{
		jmp p[11*4];
	}
}
