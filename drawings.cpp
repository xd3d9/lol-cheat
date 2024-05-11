#include <imgui_impl_dx11.h>
#include "Vector.h"
#include "funcs.hpp"
#include "drawings.hpp"
#include "object.hpp"

void DrawCircleAt(ImDrawList* canvas, const Vector3& worldPos, float radius, bool filled, int numPoints, ImColor color, float thickness) {
	Core::LoadW2S();
	if (numPoints >= 200)
		return;
	static ImVec2 points[200];

	float step = 6.2831f / numPoints;
	float theta = 0.f;
	for (int i = 0; i < numPoints; i++, theta += step) {
		Vector3 worldSpace = { worldPos.x + radius * cos(theta), worldPos.y, worldPos.z - radius * sin(theta) };
		Vector2 screenSpace = Core::fWorldToScreen(worldSpace);

		points[i].x = screenSpace.x;
		points[i].y = screenSpace.y;
	}

	if (filled)
		canvas->AddConvexPolyFilled(points, numPoints, color);
	else
		canvas->AddPolyline(points, numPoints, color, true, thickness);
}

/*
* Funqcia Romelic Xatavs Wres Ekranze Umetesad Gamoikeneba Rangebis Daxatvashi
* @param position: Atvlis wertilis pozicia, gamoikeneba umetesad WorldToScreen ti
* @param attackrange: Obiektis shetevis/mokmedebis areali
* @param color: R,G,B ferta informacia romelic sheexeba wris fers, gamoikeneba rogorc ImColor(255, 255, 255)
* @returns araferi voidia
*/
void Drawings::DrawRange(Vector3 position, float attackrange, ImColor color) {
	DrawCircleAt(ImGui::GetBackgroundDrawList(), position, attackrange, false, 100, color, 1);
}

/*
Vxatavt Kvela Toweris Shetevis Areals Rom Shemtxvevit Ar Shevakvdet
Dasafixia
*/
void Drawings::DrawTowerRanges() {
	int vectorSize = (int)(*(DWORD64*)(*(DWORD64*)(baseAddr + TurretList) + 0x10)); // vigebt towerebis raodenobas tamashshi

	for (int i = 0; i < vectorSize; i++) {
		GameObject* turretObject = (GameObject*)(*(DWORD64*)(*(DWORD64*)(*(DWORD64*)(baseAddr + TurretList) + 0x8) + 0x8 * i)); // vigebt sias turretlistidan rom gamovitanot titeuli tower rogorc obiektad
		float attackrange = 94 + 750; // toweris shetevis areali (HARDCODED) (memoryshi ver vipove :skull:)
		DrawCircleAt(ImGui::GetBackgroundDrawList(), turretObject->position, attackrange, false, 100, ImColor(255, 0, 0), 1);
	}
}