#pragma once
#include "Obstacle.h"
#include "RandomValue.h"
#include "Fence.h"

class Switch : public Obstacle{
private:
	int nowHandle = 0;
	int modelHandle2 = 0;
	int ridingHeight = 800;
	int pushingHeight = 500;
	RandomValue randVal;
	Fence* fence;
public:
	Switch(Resource* r)
		:Obstacle(r) {
		pos = { 2500,150,18700 };
		mustCount = 10;
		radius = 500;
		scaleRate = 9;
		nowHandle = modelHandle;
		cube = new Cube(pos, frameWidth, frameHeight, frameDepth);
		fence = new Fence(r);
		
		fence->LoadModel("Fence", "Model/Fence.mv1");
		resource->SetupCollision(fence->GetHandle(), -1, 4, 4, 4);
	}

	int GetHandle1() { return modelHandle; }
	int GetHandle2() { return modelHandle2; }

	void LoadSecondModel(string name, const string& path) {
		modelHandle2 = resource->LoadModel(name, path);
		MV1SetScale(modelHandle2, VGet(scaleRate, scaleRate, scaleRate));
		MV1SetPosition(modelHandle2, pos);
	}

	void Update(float deltaTime)override {

		nowCount = 0;
		//èâä˙âª
		//std::vector<int>().swap(emplacedNum);
		emplacedNum.clear();
		for (int i = 0;i < resource->pManager->GetPilminCount();i++) {
			if (isArea(resource->pManager->GetPilmin(i)->GetPos())) {
				if (resource->pManager->GetPilmin(i)->GetNowState() == Pilmin::State::Working) {
					resource->pManager->GetPilmin(i)->SetNowState(Pilmin::State::Carrying);
					float angle = randVal.GetRandomVal(-2 * DX_PI_F, 2 * DX_PI_F);
					radius = randVal.GetRandomVal(0, 500);
					float x = radius * cosf(angle);
					float z = radius * sinf(angle);
					float y = ridingHeight;
					VECTOR p = { x, y, z };
					resource->pManager->GetPilmin(i)->SetPos(VAdd(pos,p));
				}
				if (resource->pManager->GetPilmin(i)->GetNowState() == Pilmin::State::Carrying) {
					nowCount++;
					emplacedNum.emplace_back(i);
				}
			}
		}
		if (nowCount >= mustCount)isCarrying = true;
		//else isCarrying = false;


		if (isCarrying)EnCarry_Pilmins();
		Emplace_Pilmins();

		if (isCarrying) {
			if (!prevIsCarrying)PlaySoundMem(resource->sound_switch, DX_PLAYTYPE_BACK);
			MV1SetPosition(modelHandle2, pos);
		}
		else {
			MV1SetPosition(modelHandle, pos);
		}
		
		cube->SetCentorPos(pos);
		fence->SetFlag(isCarrying);
		fence->Update(deltaTime);
		prevIsCarrying = isCarrying;
	}

	void Emplace_Pilmins()override {
		
	}

	void EnCarry_Pilmins()override {
		for (int i = 0;i < resource->pManager->GetPilminCount();i++) {
			if (resource->pManager->GetPilmin(i)->GetNowState() == Pilmin::State::Carrying) {
				VECTOR p = resource->pManager->GetPilmin(i)->GetPos();
				p.y = pushingHeight;
				resource->pManager->GetPilmin(i)->SetPos(p);
			}
		}
	}

	void Draw() override{
		fence->Draw();
		cube->DrawFrameCapsule();
		if (isCarrying)MV1DrawModel(modelHandle2);
		else MV1DrawModel(modelHandle);
	}
};