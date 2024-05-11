#include "includes.h";
#include "object.hpp"
#include "offsets.h"
#include <vector>
#include "funcs.hpp"

#include "spacegliding.h"
#include <iostream>



void __stdcall SpaceGlide() {
		//std::cout << spaceglide << std::endl;
		DWORD64 BaseAddress = (DWORD64)GetModuleHandle(NULL);
		GameObject* oLocalPlayer = (GameObject*)(*(DWORD64*)(BaseAddress + LocalPlayer));
		std::vector<GameObject*> Champions;

		GameObject* target = new GameObject();

		typedef bool(__thiscall* fnIsAlive)(GameObject* obj);
		typedef float(__cdecl* fnGetAttackDelay)(GameObject* obj);
		typedef float(__cdecl* fnGetAttackCastDelay)(GameObject* obj);
		typedef void(__thiscall* fnPrintChat)(DWORD64, const char*, int);
		typedef int(__thiscall* fnNewIssueOrder)(__int64* locplayer,
			unsigned __int8 a2,
			float* a3,
			__int64 a4,
			char a5,
			char a6,
			char a7);

		fnGetAttackDelay GetAttackDelay = (fnGetAttackDelay)(BaseAddress + fGetAttackDelay);
		fnGetAttackCastDelay GetAttackCastDelay = (fnGetAttackCastDelay)(BaseAddress + fGetAttackCastDelay);
		fnPrintChat PrintChat = (fnPrintChat)(BaseAddress + fPrintChat);
		fnIsAlive IsAlive = (fnIsAlive)(BaseAddress + fIsAlive);

		while (true) {
			if (spaceglide == 0)
				continue;
			if (GetAsyncKeyState(VK_SPACE) && GetTickCount64() - lastLoop > 3) {
				Champions.clear();
				int vectorSize = (int)(*(DWORD64*)(*(DWORD64*)(BaseAddress + HeroList) + 0x10));

				for (int i = 0; i < vectorSize; i++) {
					GameObject* heroObject = (GameObject*)(*(DWORD64*)(*(DWORD64*)(*(DWORD64*)(BaseAddress + HeroList) + 0x8) + 0x8 * i));
					//std::cout << (heroObject->position.distance(oLocalPlayer->position) < 900) << " <- distance<900 | -> loc distance < targetdistance " << (heroObject->position.distance(oLocalPlayer->position) < target->position.distance(oLocalPlayer->position)) << std::endl;
					//std::cout << (IsAlive(heroObject)) << " <- isAlive | -> targetable " << (heroObject->targetable) << std::endl << (heroObject->visible) << " <- visible | -> team ar udris chven temas " << (heroObject->team != oLocalPlayer->team) << std::endl;
					//printf("Hero Object: %f %f  ==== LocalPlayer: %f %f", heroObject->position.x, heroObject->position.y, oLocalPlayer->position.x, oLocalPlayer->position.y);
					/*if (IsAlive(heroObject) &&
						heroObject->targetable && 
						heroObject->visible && 
						heroObject->team != oLocalPlayer->team && 
						heroObject->position.distance(oLocalPlayer->position) < 900 && 
						heroObject->position.distance(oLocalPlayer->position) < target->position.distance(oLocalPlayer->position)
						)
						*/
					//std::cout << oLocalPlayer->GetBoundingRadius() << std::endl;

					if (
						(*(float*)(Core::GetLocalObject() + AttackRange)) + oLocalPlayer->GetBoundingRadius() >= oLocalPlayer->position.distance(heroObject->position) &&// vamowmebt tu championiari chvens rangeshi
						IsAlive(heroObject) && // vamowmebt tu cocxali ari chempioni
						heroObject->targetable && // vamowmebt tu shegvidzlia chempionis sheteva
						heroObject->visible && // vamowmebt tu chempioni gamqralia
						heroObject->team != oLocalPlayer->team // vamowmebt imkofeba tu ara chempioni chven teamshi
						
						 // vedzebt uketess targets radganac dzveli agar gamogvadgeba
						)

						target = heroObject;
					//std::cout << target << std::endl;

					Champions.push_back(heroObject);
				}
				
				using fnIssueOrder = bool(__fastcall*)(uintptr_t HudInput, int, int, int, int x, int y, bool);

				fnIssueOrder IssueOrder = (fnIssueOrder)(BaseAddress + fIssueOrder);
				bool isInRange = ((*(float*)(Core::GetLocalObject() + AttackRange)) + oLocalPlayer->GetBoundingRadius() >= oLocalPlayer->position.distance(target->position));
				if (isInRange && target->position.x != 0 && GetTickCount64() >= lastAttack + GetAttackDelay(oLocalPlayer) * 1000.f) {
					Core::LoadW2S();

					Vector2 AttackPosition = Core::fWorldToScreen(target->position);
					//std::cout << "Target Position " << target->position.x << " " << target->position.y << " "<< target->position.z << std::endl;
					//std::cout << "Attack Position " << AttackPosition.x << " " << AttackPosition.y << std::endl;
					/*
					DWORD64 HUDInput = *(DWORD64*)(*(DWORD64*)(BaseAddress + oHudInstance) + 0x28);
					NewIssueOrder(HUDInput, 0, 0, true, AttackPosition.x, AttackPosition.y, 0);
					NewIssueOrder(HUDInput, 1, 0, true, AttackPosition.x, AttackPosition.y, 0);
					*/
					
					
					auto hudInput = *(uintptr_t*)(*(uintptr_t*)(BaseAddress + oHudInstance) + 0x48);
					std::cout << hudInput << std::endl;
					
					IssueOrder(hudInput, 0, 0, 1, AttackPosition.x, AttackPosition.y, false);
					IssueOrder(hudInput, 1, 0, 1, AttackPosition.x, AttackPosition.y, false);
					//ImGui::Text("Attack Position (%f, %f)", AttackPosition.x, AttackPosition.y);
					//NewIssueOrder(HUDInput, 0, 0, true, AttackPosition.x, AttackPosition.y, 0);
					//NewIssueOrder(HUDInput, 1, 0, true, AttackPosition.x, AttackPosition.y, 0);
					//std::cout << "Attack Position " << AttackPosition.x << " " << AttackPosition.y << std::endl;
					//NewIssueOrder(HUDInput, 1, 0, true, AttackPosition.x, AttackPosition.y, 0);

					lastAttack = GetTickCount64();
				}
				
				else if (GetTickCount64() > lastMove + 30 && GetTickCount64() >= lastAttack + GetAttackCastDelay(oLocalPlayer) * 1000.f + Ping + 10) {

					// Load WorldToScreen
					Core::LoadW2S();

					DWORD64 MousePtr = *(DWORD64*)(*(DWORD64*)(BaseAddress + oHudInstance) + 0x28);

					Vector3 mousePos;

					mousePos.x = *(float*)(MousePtr + 0x20);
					mousePos.y = *(float*)(MousePtr + 0x30);
					mousePos.z = *(float*)(MousePtr + 0x28);
					//std::cout << "Mouse Position " << mousePos.x << " " << mousePos.y << " " << mousePos.z << std::endl;

					Vector3 newpos;
					newpos.x = (int)(mousePos.x);
					newpos.y = (int)(mousePos.y);
					newpos.z = (int)(mousePos.z);
					Vector2 MovePosition = Core::fWorldToScreen(newpos);
					auto hudInput = *(uintptr_t*)(*(uintptr_t*)(BaseAddress + oHudInstance) + 0x48);
					IssueOrder(hudInput, 0, 0, 1, MovePosition.x, MovePosition.y, false);
					IssueOrder(hudInput, 1, 0, 1, MovePosition.x, MovePosition.y, false);
					//std::cout << "Move Position " << MovePosition.x << " " << MovePosition.y << std::endl;
					//std::cout << "WOT " << MovePosition.x << " " << MovePosition.y << std::endl;
					lastMove = GetTickCount64();
				}
				

				lastLoop = GetTickCount64();
			}
	}
}