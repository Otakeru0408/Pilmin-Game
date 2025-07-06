#pragma once
#include "Resource.h"

using namespace std;

class Stage {
private:
	int modelHandle = 0;
	Resource* resourceInfo;
public:
	Stage(Resource* r) {
		resourceInfo = r;
	}
	void LoadModel(string name, const string& path) {
		modelHandle = resourceInfo->LoadModel(name, path);
	}

	int GetHandle() {
		return modelHandle;
	}

	void Update(float) {
		MV1SetupCollInfo(modelHandle, -1, 0, 0, 0);
	}
	void Draw() {
		MV1SetPosition(modelHandle, VGet(0.0f, -100.0f, 0.0f));
		MV1DrawModel(modelHandle);
	}

	
};