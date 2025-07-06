#include "Obstacle.h"

Obstacle::Obstacle(Resource* r) {
	cube = new Cube(pos,frameWidth,frameHeight,frameDepth);
	resource = r;
}

void Obstacle::LoadModel(string name, const string& path) {
	modelHandle = resource->LoadModel(name, path);
	//MV1SetFrameVisible(modelHandle, 0, FALSE);
	MV1SetScale(modelHandle, VGet(scaleRate, scaleRate, scaleRate));
}

void Obstacle::Update(float deltaTime) {

	nowCount = 0;
	//������
	//std::vector<int>().swap(emplacedNum);
	emplacedNum.clear();
	for (int i = 0;i < resource->pManager->GetPilminCount();i++) {
		if (isArea(resource->pManager->GetPilmin(i)->GetPos())) {
			if( resource->pManager->GetPilmin(i)->GetNowState() == Pilmin::State::Working) {
				resource->pManager->GetPilmin(i)->SetNowState(Pilmin::State::Carrying);
			}
			if (resource->pManager->GetPilmin(i)->GetNowState() == Pilmin::State::Carrying) {
				nowCount++;
				emplacedNum.emplace_back(i);
			}
		}
	}
	if (nowCount >= mustCount)isCarrying = true;
	else isCarrying = false;

	//�����Ԃ��厖�B�t�ɂ���ƊO����pilmin���u���Ă�����Ă��܂�
	if (isCarrying)EnCarry_Pilmins();
	Emplace_Pilmins();

	MV1SetPosition(modelHandle, pos);
	cube->SetCentorPos(pos);
}

void Obstacle::Emplace_Pilmins() {
	for (int i = 0;i < nowCount;i++) {
		float angle = 2.0f * DX_PI * i / nowCount; // �ϓ��Ɋp�x�𕪂���
		float x = pos.x + radius * cosf(angle);
		float z = pos.z + radius * sinf(angle);
		float y = 150;
		VECTOR p = { x, y, z };
		resource->pManager->GetPilmin(emplacedNum[i])->SetPos(p);
		VECTOR dir = VSub(p,pos);
		resource->pManager->GetPilmin(emplacedNum[i])->SetRadAngle(atan2(dir.x,dir.z));

	}
}

void Obstacle::EnCarry_Pilmins() {
	VECTOR subdir = VSub(resource->pManager->GetPilmin(emplacedNum[0])->GetMainCharaPos(), pos);
	if (VSize(subdir) > radius) {
		pos = VAdd(pos, VScale(VNorm(subdir), moveSpeed));
	}
	
}

void Obstacle::Draw() {
	cube->DrawFrameCapsule();
	MV1DrawModel(modelHandle);
	if (isCarrying) {
		DrawCapsule3D(pos, resource->pManager->GetPilmin(emplacedNum[0])->GetMainCharaPos(),
			10,4,GetColor(0,0,0),GetColor(0,0,0),TRUE);
	}
}