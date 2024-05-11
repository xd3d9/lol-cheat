#include "Vector.h"
#include "funcs.hpp"

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;};


// GameObject class
class GameObject {
public:
	// Constructor
	GameObject() { position = Vector3(0, 0, 0); team = 100; visible = false; };

	union {
		DEFINE_MEMBER_N(short, team, 0x3C);
		DEFINE_MEMBER_N(bool, visible, 0x340);
		DEFINE_MEMBER_N(Vector3, position, 0x220);
		DEFINE_MEMBER_N(bool, targetable, 0xEE0);
	};
	float GetBoundingRadius()
	{
		typedef float(__fastcall* fnGetBoundingRadius)(GameObject* obj);
		fnGetBoundingRadius _fnGetBoundingRadius = (fnGetBoundingRadius)(baseAddr + BonusRadius);
		return _fnGetBoundingRadius(this);
	}
};