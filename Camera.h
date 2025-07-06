#pragma once
#include "DxLib.h"
#include "Player.h"
#include "Resource.h"
#include<cstdint>
#define PI 3.14159265

class Camera {
private:
	VECTOR camPos = { -400.0f, 500.0f, 0 };
	VECTOR focusPos = { 0.0f,0.0f,0.0f };
	Player* player;
	Resource* resource;
	float angleY = -90.0f;		//角度は弧度法
	float angleX = 45.0f;
	float distanceFromPlayer = 4000.0f;
	float distanceFromPlayer_whenHit = 0.0f; //distanceFromPlayerで障害物に当たってる時の距離
	bool isDragging = false;
	int startMouseX = 0.0f;
	int startMouseY = 0.0f;
	float rotateSpeed = 1.0f;
	int stageHandle = 0;
	MV1_COLL_RESULT_POLY result;

public:
	Camera(Player*,Resource*);
	void ProcessInput(uint32_t* key);
	void Update(float);
	void Draw(); //本来は必要ないがデバッグ用

	void SetStageHandle(int i) { stageHandle = i; }
	float GetDistance() { return distanceFromPlayer; }

	float D2R(float deg) {
		return deg * (DX_PI_F / 180.0f);
	}
	float R2D(float rad) {
		return rad * (180.0f / DX_PI_F);
	}
};