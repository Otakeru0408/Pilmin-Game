#pragma once
#include "DxLib.h"
#include "Cube.h"
#include "Resource.h"

#include <vector>

class Obstacle {
protected:
	int modelHandle = 0;
	VECTOR pos = { 0,500,7000 };	//変数
	VECTOR framePos = { 0,0,0 };
	float frameWidth =  2000;		//変数
	float frameHeight = 2000;		//変数
	float frameDepth =  2000;		//変数
	float scaleRate = 9;			//変数
	int mustCount = 5;				//変数
	int nowCount = 10;
	float radius = 1000;			//変数
	float moveSpeed = 10.0f;
	bool isCarrying = false;
	bool prevIsCarrying = false;
	Cube* cube;
	Resource* resource;
	std::vector<int> emplacedNum;
public:
	Obstacle(Resource* r);
	void LoadModel(string, const string&);

	int GetHandle() { return modelHandle; }

	int GetMustCount() { return mustCount; }
	int GetNowCount() { return nowCount; }

	bool isArea(VECTOR v) {
		return pos.x - frameWidth / 2 <= v.x && v.x <= pos.x + frameWidth / 2
			&& pos.y - frameHeight / 2 <= v.y && v.y <= pos.y + frameHeight / 2
			&& pos.z - frameDepth / 2 <= v.z && v.z <= pos.z + frameDepth / 2;
	}

	virtual void Update(float);
	virtual void Draw();

	virtual void Emplace_Pilmins();
	virtual void EnCarry_Pilmins();
};