#pragma once
#include <string>
#include <cstdint>
#include <vector>

#include "DxLib.h"
#include "Resource.h"
#include "PointLight.h"
#include "Pilmin.h"
#include "DrawArcLine.h"

class Pilmin;
class DrawArcLine;

using namespace std;

class Player {
protected:
	VECTOR pos = { 0,150,0 };
	VECTOR rot = { 0,0,0 };
	VECTOR forward = { 0,0,0 };
	VECTOR backward = { 0,0,0 };
	VECTOR right = { 0,0,0 };
	VECTOR left = { 0,0,0 };
	int modelHandle = 0;
	float scaleRate = 0.5f;
	VECTOR vec = { 0,0,0 };
	float rotSpeed = 0.1f;
	float addRot = 0;
	float moveSpeed = 50.0f;
	string name;
	Resource* resource;
	PointLight* pLight;
	float camAngleY = 0;		//角度は弧度法
	float camAngleX =0;
	MV1_COLL_RESULT_POLY_DIM HitPolyDim;
	int catchMode = 0;
	int maxCatchMode = 3;
	int indexOfThrow = 0;
	vector<int> catchedNum;
	int nowWaiting = 0;
	bool readyToThrow = false;
	int prevMouseInput = 0;

	//pilmin投げ関係
	float throwSpeed = 50.0f;
	float gravity = .3f;
	float throwHeight = 10.0f;
	DrawArcLine arc;

	//stage取り扱い
	int stageFrame = 0;
	int prevStageFrame = 0;
	vector<int> stageDepths = { -5800,9000,24000,39000, 54000 };

public:
	Player(Resource* r) {
		name = "Player";
		resource = r;
		pLight = new PointLight();
	}
	void LoadModel(string, const string&);

	int GetCatchMode() { return catchMode; }

	float D2R(float deg) {
		return deg * (DX_PI_F / 180.0f);
	}

	void CatchPilmin(uint32_t* key);

	VECTOR GetPos()const { return pos; }

	void SetPosition(VECTOR v) {
		pos = v;
	}

	string GetName() { return name; }
	void SetName(string _name) {
		name = _name;
	}

	int GetHandle() {
		return modelHandle;
	}

	int GetStageFrame() {
		return stageFrame;
	}

	//移動の際に必要になるカメラ角度をカメラ側からもらう
	void SetCamAngleY(float y) { camAngleY = y; }
	void SetCamAngleX(float x) { camAngleX = x; }

	void ProcessInput(uint32_t* key);
	void Update(float);
	virtual void Draw();


};