#pragma once
#include "DxLib.h"
#include "Resource.h"

class Fence {
private:
	int modelHandle = 0;
	VECTOR pos = { 0,2000,22500 };
	VECTOR pos_under = { 0,4000,22500 };
	float scaleRate = 8;
	Resource* resource;
	bool isClosed = false;
	
public:
	Fence(Resource* r) {
		resource = r;
	}

	int GetHandle() { return modelHandle; }
	void SetFlag(bool b) { isClosed = b; }
	void SetPos(VECTOR v) { 
		pos = v;pos_under = VGet(v.x, v.y + 2000, v.z); 
		MV1SetPosition(modelHandle, pos);
	}

	void LoadModel(string name, const string& path) {
		modelHandle = resource->LoadModel(name, path);
		MV1SetScale(modelHandle, VGet(scaleRate, scaleRate, scaleRate));
		MV1SetPosition(modelHandle, pos);
	}

	void Update(float deltaTime) {
		if (!isClosed)MV1SetPosition(modelHandle, pos);
		else MV1SetPosition(modelHandle, pos_under);
	}

	void Draw() {
		MV1DrawModel(modelHandle);
	}
};