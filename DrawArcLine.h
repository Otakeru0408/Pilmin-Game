#pragma once
#include "DxLib.h"

#include <vector>

class DrawArcLine {
private:
	VECTOR startPos = { 0,0,0 };
	VECTOR dir = { 0,0,0 };
	float throwHeight = 0;
	float throwSpeed = 0;
	float gravity = 0;
	float posY = 0;

	std::vector<VECTOR> trajectory;
	VECTOR previousPos = { 0,0,0 };

public:
	DrawArcLine() {
		
	}

	void ArcLine(VECTOR p, VECTOR v, float h, float s, float g) {
		//èâä˙âª
		startPos = p;
		dir = v;
		throwHeight = h;
		throwSpeed = s;
		gravity = g;
		posY = startPos.y;
		dir = VScale(VNorm(dir), throwSpeed);
		std::vector<VECTOR>().swap(trajectory);

		trajectory.emplace_back(startPos);
		previousPos = startPos;

		for (int i = 0;i<60;i++) {
			float subnum= throwHeight > 0 ? throwHeight * throwHeight : -throwHeight * throwHeight;
			VECTOR newPos = VAdd(previousPos, VGet(dir.x, subnum, dir.z));
			trajectory.emplace_back(newPos);
			previousPos = newPos;
			throwHeight -= gravity;
		}

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 50);
		for (int i = 0;i < trajectory.size()-1;i++) {
			if (i % 3 == 0) {
				DrawCapsule3D(trajectory[i], trajectory[i + 1], 5, 3,
					GetColor(0, 255, 255), GetColor(0, 255, 255), FALSE);
			}
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
};