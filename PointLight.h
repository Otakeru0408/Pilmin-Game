#pragma once
#include "DxLib.h"

class PointLight {
private:
	int lightHandle = 0;
	float range = 2000.0f;
	VECTOR pos = { 0,0,0 };
public:
	PointLight();
	void Update(float);
	void Draw();

	void SetPos(VECTOR v) { pos = v; }
	VECTOR GetPos() { return pos; }
};