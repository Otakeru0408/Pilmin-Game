#include "Pilmin.h"

Pilmin::Pilmin(int handle,int _stageWidth,int _stageDepth) {
	modelHandle = handle;
	stageWidth = _stageWidth;
	stageDepth_min = -_stageDepth;
	stageDepth_max = _stageDepth;
	MV1SetFrameVisible(modelHandle, 5, FALSE);
}

void Pilmin::Update(float deltaTime) {
	addPos = pos;
	switch (nowState) {
	case Free:
		StateFree(deltaTime);
		break;
	case Catched:
		StateCatched(deltaTime);
		break;
	case Waiting:
		StateWaiting(deltaTime);
		break;
	case Working:
		StateWorking(deltaTime);
		break;
	case Carrying:
		StateCarrying(deltaTime);
	default:
		break;
	}

	/*VECTOR top = VGet(pos.x, pos.y + 200, pos.z);
	VECTOR bottom = VGet(pos.x, pos.y-200, pos.z);
	DrawCylinder3D(top, bottom, 100, 16, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);
	HitPolyDim = MV1CollCheck_Sphere(stageHandle,-1,top,100);*/
	if (isInsideOfStage(addPos)) {
		pos = addPos;
	}
	
	MV1SetPosition(modelHandle, pos);
}

void Pilmin::Draw() {
	MV1DrawModel(modelHandle);
}

void Pilmin::StateFree(float deltaTime) {
	if (rand_moveTime <= 0) {
		rand_moveTime = (float)randVal.GetRandomVal(1, 10);
		rand_moveDirection = randVal.GetRandomVal(0, 4);
	}
	rand_moveTime -= deltaTime;
	//DrawFormatString(10, 10, GetColor(255, 0, 0), "%f", rand_moveTime);
	switch (rand_moveDirection) {
	case 0:			//‚È‚É‚àˆÚ“®‚µ‚È‚¢
		break;
	case 1:		//+X•ûŒü
		addPos = VAdd(pos, VGet(moveSpeed * deltaTime / 2, 0, 0));
		angle = atan2(moveSpeed * deltaTime / 2,0);
		break;
	case 2:		//-X•ûŒü
		addPos = VAdd(pos, VGet(-moveSpeed * deltaTime / 2, 0, 0));
		angle = atan2(-moveSpeed * deltaTime / 2, 0);
		break;
	case 3:		//+Z•ûŒü
		addPos = VAdd(pos, VGet(0, 0, moveSpeed * deltaTime / 2));
		angle = atan2(0, moveSpeed * deltaTime / 2);
		break;
	case 4:		//-Z•ûŒü
		addPos = VAdd(pos, VGet(0, 0, -moveSpeed * deltaTime / 2));
		angle = atan2(0, -moveSpeed * deltaTime / 2);
		break;
	default:break;
	}
	MV1SetRotationXYZ(modelHandle, VGet(0, angle+DX_PI_F, 0));
}

void Pilmin::StateCatched(float deltaTime) {
	VECTOR dir = VSub(mainCharaPos, pos);
	angle = atan2(dir.x, dir.z);
	MV1SetRotationXYZ(modelHandle, VGet(0, angle+DX_PI_F, 0));
	if (VSize(dir) > distanceFromChara) {

		addPos = VAdd(pos, VScale(VNorm(dir), moveSpeed * deltaTime));
	}
}

void Pilmin::StateWaiting(float deltaTime) {
}

void Pilmin::StateWorking(float deltaTime) {
	if (pos.y + subnum > 150) {
		//pos = VAdd(pos, VScale(throwDir,deltaTime));
		subnum = jumpHeight > 0 ? jumpHeight * jumpHeight : -jumpHeight * jumpHeight;

		addPos = VAdd(pos, VGet(throwDir.x, subnum, throwDir.z));
		jumpHeight -= gravity;
	}
	else {
		pos.y = 150;
		nowState = State::Catched;
		//jumpHeight = maxJumpHeight;
	}
}

void Pilmin::StateCarrying(float deltaTime) {
	MV1SetRotationXYZ(modelHandle, VGet(0,angle,0));
}