#include <Windows.h>
#include <psapi.h>
#include "Vector.h"
#include <string>;

#include "includes.h";
#include "funcs.hpp";
#include <vector>
#include <iostream>
//#include "spoof/spoof_call.h"
//#include "spoof/mem.h"

#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3d11.lib")

#include "object.hpp";
#include "drawings.hpp"

#include "spacegliding.h"

bool drawings = false;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;



void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}
	DWORD64 handle = (DWORD64)GetModuleHandle(NULL);
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("ZeruaS");
	ImGui::Text("Funqciebi");
	ImGui::Checkbox("SpaceGlide", &spaceglide);
	ImGui::Checkbox("Drawings", &drawings);
	GameObject* oLocalPlayer = (GameObject*)(*(DWORD64*)(baseAddr + LocalPlayer));

	float player_x = *(float*)(Core::GetLocalObject() + 0x220);
	float player_y = *(float*)(Core::GetLocalObject() + 0x224);
	float player_z = *(float*)(Core::GetLocalObject() + 0x228);

	float attackrange = *(float*)(Core::GetLocalObject() + AttackRange);
	Vector2 WoT = Core::fWorldToScreen(Vector3(player_x, player_y,player_z));
	//ImGui::GetBackgroundDrawList()->AddCircle(ImVec2::ImVec2(960, 540), attackrange, ImColor(255, 0, 0), 100, 1.0f);
	if (drawings) {
		Drawings::DrawRange(Vector3(player_x, player_y, player_z), attackrange + oLocalPlayer->GetBoundingRadius(), ImColor(255, 255, 255));
		Drawings::DrawTowerRanges();
	}
	// debug ragaceebi
	ImGui::Text("Ameebs Ar Miakciot Kuradgeba [DEBUG]");
	float game_time = Core::GetGameTime();
	ImGui::SliderFloat("Game Time", &game_time, 0, 10000);
	//float fov = ImGui::DragFloat("Field of View", &fov, 10, 500);
	//*(float*)(*(DWORD64*)(baseAddr + 0x21F88C8)+ 0x1B4) = 500;
	
	int health = *(float*)(Core::GetLocalObject() + ObjHealth); // 0x1088 health ari
	ImGui::InputInt("LocalPlayer Health", &health);
	ImGui::InputFloat("Attack Range", &attackrange);
	Ping = Core::GetPing();
	ImGui::InputDouble("ping", &Ping);
	if (ImGui::Button("daprinte ragac"))
		Core::PrintChat("ragac lol?");
	ImGui::End();

	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)&oPresent, hkPresent);
			init_hook = true;
		}
	} while (!init_hook);
	return TRUE;
}


// reference: https://learn.microsoft.com/en-us/windows/win32/dlls/dllmain
// egi ari sadac injection xdeba karoche ra
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    DWORD64 handle = (DWORD64)GetModuleHandle(NULL);
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        /*
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		std::cout << "debug" << std::endl;
		*/
		DisableThreadLibraryCalls(hModule);
		CreateThread(nullptr, 0, MainThread, hModule, 0, nullptr);

		//features functions
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SpaceGlide, 0, 0, 0);

		Core::PrintChat("ZeruaS Chatvirtul, Lolis AntiCheat gadatvirtul (vegar gavritme)");
		Core::PrintChat("ZeruaS CHEAT WIP");

        
        //return TRUE;
    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
		kiero::shutdown();
    }

    return TRUE;
}