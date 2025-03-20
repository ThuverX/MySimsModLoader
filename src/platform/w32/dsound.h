//
// Created by exozg on 24/01/2025.
//
#pragma once

#pragma pack(1)

// taken from https://gitlab.com/znixian/payday2-superblt/-/blob/master/platforms/w32/loader/wsock.cpp?ref_type=heads

#pragma region ALLFUNC(FUNC)

#define ALLFUNC(FUNC) \
	FUNC(0,DirectSoundCaptureCreate8,12) \
	FUNC(1,DirectSoundCreate8,11) \
	FUNC(2,DirectSoundFullDuplexCreate,10) \
	FUNC(3,GetDeviceID,9) \
	FUNC(4,DirectSoundCaptureEnumerateW,8) \
	FUNC(5,DirectSoundCaptureEnumerateA,7) \
	FUNC(6,DirectSoundCaptureCreate,6) \
	FUNC(7,DirectSoundEnumerateW,3) \
	FUNC(8,DirectSoundEnumerateA,2) \
	FUNC(9,DirectSoundCreate,1) \

#define ALLFUNC_COUNT 10

#pragma endregion

FARPROC p[ALLFUNC_COUNT] = {nullptr};

void exports(HMODULE hL){
#define REGISTER(num, name, ordinal) p[num] = GetProcAddress(hL, #name);
	ALLFUNC(REGISTER);
#undef REGISTER
}

#define DEF_STUB(num, name, ordinal) \
extern "C" __declspec(naked) void __stdcall _DSOUND_EXPORT_##name(){__asm {jmp p[num * 4]}};

ALLFUNC(DEF_STUB)
#undef DEF_STUB