#pragma once
#include <windows.h>
#include <winDNS.h>
#include "offsets.h"
#define baseAddr (DWORD64)GetModuleHandle(NULL)

inline double Ping = 0;

inline double lastAttack = 0;
inline double lastMove = 0;
inline double lastLoop = 0;

//inline void* spoof_trampoline = 0x0;

inline int width = 0;
inline int height = 0;

inline float viewMatrix[16];
inline float projMatrix[16];
inline float viewProjMatrix[16];

class Core {
public:

	static float GetGameTime() {
		return *(float*)(baseAddr + GameTime);
	}

	static void PrintChat(const char* Message) {
		typedef void(__thiscall* tPrintChat)(DWORD64 ChatClient, const char* Message, int Color);
		tPrintChat fnPrintChat = (tPrintChat)(baseAddr + fPrintChat);
		fnPrintChat(*(DWORD64*)(baseAddr + oChatClient), Message, 1);
	}

	static DWORD64 GetLocalObject() {
		DWORD64 retaddr = *(DWORD64*)(baseAddr + LocalPlayer);
		if (retaddr == NULL)
			return NULL;

		return retaddr;
	}

	static Vector2 fWorldToScreen(Vector3 in)
	{
		typedef bool(__fastcall* fnWorldToScreen)(QWORD* viewport, Vector3* in, Vector3* out);
		fnWorldToScreen _fnWorldToScreen = (fnWorldToScreen)(baseAddr + oWorldToScreen);
		QWORD* viewport = *(QWORD**)(baseAddr + oViewport);
		Vector3 out;
		_fnWorldToScreen((QWORD*)((QWORD)viewport + 0x270), &in, &out);
		return Vector2(out.x, out.y);
	}

	static double GetPing() {

		auto NetClientPrt = *(uintptr_t*)(baseAddr + NetClient);
		auto v1 = *(uintptr_t*)(NetClientPrt + 0x88);
		auto v2 = *(uintptr_t*)(v1 + 0x0);
		auto v3 = *(uintptr_t*)(v2 + 0x8);
		auto v4 = *(uintptr_t*)(v3 + 0x10);
		auto v5 = *(double*)(v4 + 0x150);

		return v5 * 1000;

	}

	static void MultiplyMatrices(float* out, float* a, int row1, int col1, float* b, int row2, int col2) {
		int size = row1 * col2;
		for (int i = 0; i < row1; i++) {
			for (int j = 0; j < col2; j++) {
				float sum = 0.f;
				for (int k = 0; k < col1; k++)
					sum = sum + a[i * col1 + k] * b[k * col2 + j];
				out[i * col2 + j] = sum;
			}
		}
	}

	static void LoadW2S() {
		char buff[128];

		DWORD_PTR renderBase = (*(DWORD64*)((DWORD64)GetModuleHandle(NULL) + oRenderer));

		memcpy(buff, (void*)(renderBase), 128);
		width = *reinterpret_cast<int*>(buff + oRendererWidth);
		height = *reinterpret_cast<int*>(buff + oRendererHeight);

		memcpy(buff, (void*)((DWORD64)GetModuleHandle(NULL) + oViewProjMatrices), 128);
		memcpy(viewMatrix, buff, 16 * sizeof(float));
		memcpy(projMatrix, &buff[16 * sizeof(float)], 16 * sizeof(float));

		MultiplyMatrices(viewProjMatrix, viewMatrix, 4, 4, projMatrix, 4, 4);
	}

	//egi shegvidzlia gamovikenot externalisaa mara internalis zevitaa dakomentarebuli i guess
	static Vector2 WorldToScreen(const Vector3& pos) {

		Vector2 out = { 0.f, 0.f };
		Vector2 screen = { (float)width, (float)height };

		Vector4 clipCoords;
		clipCoords.x = pos.x * viewProjMatrix[0] + pos.y * viewProjMatrix[4] + pos.z * viewProjMatrix[8] + viewProjMatrix[12];
		clipCoords.y = pos.x * viewProjMatrix[1] + pos.y * viewProjMatrix[5] + pos.z * viewProjMatrix[9] + viewProjMatrix[13];
		clipCoords.z = pos.x * viewProjMatrix[2] + pos.y * viewProjMatrix[6] + pos.z * viewProjMatrix[10] + viewProjMatrix[14];
		clipCoords.w = pos.x * viewProjMatrix[3] + pos.y * viewProjMatrix[7] + pos.z * viewProjMatrix[11] + viewProjMatrix[15];
		if (clipCoords.w < 1.0f)
			clipCoords.w = 1.f;

		Vector2 M;
		M.x = clipCoords.x / clipCoords.w;
		M.y = clipCoords.y / clipCoords.w;

		out.x = (screen.x / 2.f * M.x) + (M.x + screen.x / 2.f);
		out.y = -(screen.y / 2.f * M.y) + (M.y + screen.y / 2.f);
		//std::cout << "World To Screen Pos" << out.x << " " << out.y << std::endl;
		return out;
	}

	static Vector3 GetMousePos()
	{
		DWORD64 MousePtr = (*(DWORD64*)((DWORD64)GetModuleHandle(NULL) + oHudInstance) + 0x28);

		Vector3 mousePos;

		mousePos.x = *(float*)(MousePtr + 0x20);
		mousePos.y = *(float*)(MousePtr + 0x28);
		mousePos.z = *(float*)(MousePtr + 0x30); // arvici es rato ari 0x30 da ara 0x28 mara rogorc chans lolis developerebma ase gadackvites
		//std::cout << "Mouse Pos" << mousePos.x << " " << mousePos.y << " " << mousePos.z << std::endl;
		return mousePos;
	}

};
