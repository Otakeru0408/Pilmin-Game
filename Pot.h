#pragma once
#include "Obstacle.h"

class Pot :public Obstacle{
private:
	float angle = 0;
	float modelHeight = 150;
	int kindNumber = 0;

public:
	Pot(Resource* r,int i,int max) :Obstacle(r){
		resource = r;
		modelHeight = modelHeight * scaleRate;
		scaleRate = 5;
		kindNumber = i;
		mustCount = max;
	}

	int GetHandle() { return modelHandle; }
	void SetHandle(int h) { modelHandle = h; }
	void SetPos(VECTOR v) { 
		pos = v; 
		cube = new Cube(pos, 1000, 2000, 1000);
	}
	VECTOR GetPos() { return pos; }
	void SetAngle(float a) { angle = a; }
	int GetNowCount() { return nowCount; }
	int GetMustCount() { return mustCount; }

	void LoadModel(string name, const string& path) {
		modelHandle = resource->LoadModel(name, path);
		MV1SetScale(modelHandle, VGet(scaleRate, scaleRate, scaleRate));
		pos.y += modelHeight/2;
		MV1SetPosition(modelHandle, pos);
		MV1SetWireFrameDrawFlag(modelHandle, TRUE);
	}

	void Update(float deltaTime)override {
		MV1SetPosition(modelHandle, pos);
		MV1SetRotationXYZ(modelHandle, VGet(0, angle, 0));

		nowCount = 0;
		//èâä˙âª
		emplacedNum.clear();
		for (int i = 0;i < resource->pManager->GetPilminCount();i++) {
			if (isArea(resource->pManager->GetPilmin(i)->GetPos())) {
				if (resource->pManager->GetPilmin(i)->GetNowState() == Pilmin::State::Working
					&& resource->pManager->GetPilmin(i)->GetPilminKind()== kindNumber) {
					resource->pManager->GetPilmin(i)->SetNowState(Pilmin::State::Carrying);
				}
				if (resource->pManager->GetPilmin(i)->GetNowState() == Pilmin::State::Carrying) {
					nowCount++;
					emplacedNum.emplace_back(i);
					resource->pManager->GetPilmin(i)->SetPos(pos);
				}
			}
		}
		if (nowCount >= mustCount)isCarrying = true;
		else isCarrying = false;


	}

	void Draw() override{
		cube->DrawFrameCapsule();
		MV1DrawModel(modelHandle);
	}

};