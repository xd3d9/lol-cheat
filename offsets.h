#pragma once
/*
CHATS RAC EXEBA
*/
#define oChatClient 0x226B168 //		14.2 		48 8B 05 ? ? ? ? 88 90 ? ? ? ? C3
#define oChatOpen 0xCA0 //				13.22
#define fPrintChat 0x890D20 //			14.2 		E8 ? ? ? ? 4C 8B C3 B2 01

// GAME DATA
#define GameTime 0x2257158 //			14.2		F3 0F 5C 35 ? ? ? ? 0F 28 F8
#define NetClient 0x224C650 //			14.2 		FF 50 30 4C 8B 15 ? ? ? ? 

//OBJECT POINTERS
#define LocalPlayer 0x226B000 //		14.2 		48 8B 05 ? ? ? ? 4C 8B D2 4C 8B C1
#define HeroList 0x224C788 //			14.2		48 8B 05 ? ? ? ? 45 33 E4 0F 57 C0
#define TurretList 0x22563D0 //			14.2		48 8B 1D ? ? ? ? 48 8B 5B 28

//Object Offsets
#define ObjHealth 0x1090 //				14.2 
#define ObjName 0x60 //					14.2 
#define fIsAlive 0x225410 //			14.2 		E8 ? ? ? ? 84 C0 74 35 48 8D 8F ? ? ? ?

#define fGetAttackDelay 0x3C6920 //		14.2 		F3 0F 10 89 ? ? ? ? E9 ? ? ? ?
#define fGetAttackCastDelay 0x3C6820 //	14.2 		E8 ? ? ? ? 48 8B CE F3 0F 11 83 ? ? ? ?
#define BonusRadius 0x2134D0
#define fIssueOrder 0x8EA8D0 //			14.2 		45 33 C0 E8 ? ? ? ? 48 83 C4 48
#define fIssueMove 0x8E0ED0 //			14.2 		48 89 5C 24 ? 48 89 74 24 ? 57 48 81 EC ? ? ? ? 48 8B F1 41 0F B6 F9
#define AttackRange 0x1704 //			14.2 		F3 0F 10 B6 ? ? ? ? E8 ? ? ? ? 48 8B C8 48 8B D8 E8 ? ? ? ? 0F 5A C6 45 33 C0 C6 43 10 04 33 D2

#define oHudInstance 0x224C670 //		14.2 		48 8B 0D ? ? ? ? 8B 57 10
#define oWorldToScreen 0xE9AC90 //		14.2 		E8 ? ? ? ? 49 8D 97 ? ? ? ? 4C 8D 45 D8
#define oViewport 0x224FAC0 //			14.2 		48 8B 3D ? ? ? ? FF 90 ? ? ? ?

#define oRenderer 0x2263D70 //			13.23		48 8B 0D ? ? ? ? 4C 8D 44 24 ? 48 8B D0
#define oRendererWidth 0xC //			13.22
#define oRendererHeight 0x10 //			13.22
#define oViewProjMatrices 0x22B7010 //	13.23		48 83 EC 28 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8D 0D ? ? ? ? 48 83 C4 28 