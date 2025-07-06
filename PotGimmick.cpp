#include "PotGimmick.h"

PotGimmick::PotGimmick(Resource* r) {
	resource = r;
	for (int i = 0;i < 3;i++) {
		Pot* p = new Pot(resource,i,10);
		p->SetPos(VGet(-2000, 0, 27000 + i * 3000));
		p->SetAngle(-DX_PI_F / 2);
		pots.emplace_back(p);
	}
	pots[0]->LoadModel("Pot1", "Model/Pot1.mv1");
	pots[1]->LoadModel("Pot2", "Model/Pot2.mv1");
	pots[2]->LoadModel("Pot3", "Model/Pot3.mv1");
	VECTOR v = pots[1]->GetPos();
	pots[1]->SetPos(VGet(v.x, v.y - 200, v.z));

	fence = new Fence(resource);
	fence->LoadModel("Fence2", "Model/Fence.mv1");
	fence->SetPos(VGet(0, 2000, 37000));
	resource->SetupCollision(fence->GetHandle(), -1, 4, 4, 4);
}

void PotGimmick::Update(float deltaTime) {
	isReached = true;
	for (auto p : pots) {
		p->Update(deltaTime);
		if (p->GetNowCount() < p->GetMustCount())isReached = false;
	}

	if (isReached)fence->SetFlag(TRUE);
	fence->Update(deltaTime);
}

void PotGimmick::Draw() {
	for (auto p : pots) {
		p->Draw();
	}
	fence->Draw();
	//DrawFormatString(10, 110, GetColor(255, 0, 0), "num1:%d,num2:%d,num3:%d",
	//	pots[0]->GetNowCount(), pots[1]->GetNowCount(), pots[2]->GetNowCount());
}