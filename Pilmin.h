#pragma once
#include "Dxlib.h"
#include "RandomValue.h"

class Pilmin {
public:
	enum State {
		Free,
		Catched,
		Waiting,
		Working,
		Carrying,
	};
protected:
	VECTOR pos = { 0,150,0 };
	VECTOR addPos = { 0,0,0 };
	VECTOR mainCharaPos = { 0,0,0 };
	VECTOR dir = { 0,0,0 };
	int modelHandle;
	int stageWidth = 0;
	int stageDepth_min = 0;
	int stageDepth_max = 0;
	float moveSpeed = 500.0f;
	int* otherPilmins = 0;
	int sizeOfPilminArray = 0;
	bool isHit = false;
	State nowState = State::Free;
	float distanceFromChara = 500;
	RandomValue randVal;
	float angle = 0;			//角度はラジアン値(SetAngleの引数は弧度法)
	//投げる高さ
	float jumpHeight = 10;
	// maxJumpHeight = 10;
	float subnum = 0;
	//投げにかかる重力
	float gravity = .2f;
	//横方向の移動距離
	float throwSpeed = 5.0f;
	bool isThrow = true;
	MV1_COLL_RESULT_POLY_DIM HitPolyDim;

	VECTOR throwDir = { 0,0,0 };

	float rand_moveTime = 0;
	int rand_moveDirection = 0;

	int pilmin_kind = 0;
public:
	Pilmin(int,int,int);

	int GetHandle() { return modelHandle; }
	
	virtual void SetPos(VECTOR _pos) { pos = _pos; }
	VECTOR GetPos() { return pos; }
	void SetDegAngle(float r) { angle = D2R(r); }		//rは弧度法 angleはラジアン値
	void SetRadAngle(float r) { angle = r; }			//rはラジアン値 angleもラジアン値
	float GetAngle() { return angle; }
	void SetMainCharaPos(VECTOR _pos) { mainCharaPos = _pos; }
	VECTOR GetMainCharaPos() { return mainCharaPos; }

	void SetIsHit(bool b) { isHit = b; }
	void SetNowState(State s) { nowState = s; }
	State GetNowState() { return nowState; }

	void SetPilminKind(int k) { pilmin_kind = k; }
	int GetPilminKind() { return pilmin_kind; }

	virtual void Update(float deltaTime);
	virtual void Draw();

	void SetThrowSetting(VECTOR dir, float speed, float height, float grav) {
		throwDir = dir;
		throwSpeed = speed;
		jumpHeight = height;
		gravity = grav;

		throwDir = VScale(VNorm(throwDir), throwSpeed);
	}
	VECTOR GetThrowDir() { return throwDir; }

	void SetStageDepth_max(int d) {
		stageDepth_max = d;
	}
	void SetStageDepth_min(int d) {
		stageDepth_min = d;
	}
	int GetStageDepth_max() { return stageDepth_max; }
	int GetStageDepth_min() { return stageDepth_min; }


	void StateFree(float);
	void StateCatched(float);
	void StateWaiting(float);
	void StateWorking(float);
	void StateCarrying(float);

	bool isInsideOfStage(VECTOR p) {
		return -stageWidth < p.x && p.x < stageWidth
			&& stageDepth_min < p.z && p.z < stageDepth_max;
	}
	// 2Dベクトルの角度を求める関数
	float GetAngle(VECTOR A, VECTOR B = {0,0,1}) {
		float dotProduct = A.x * B.x + A.y * B.y;
		float magnitudeA = std::sqrt(A.x * A.x + A.y * A.y);
		float magnitudeB = std::sqrt(B.x * B.x + B.y * B.y);

		float cosTheta = dotProduct / (magnitudeA * magnitudeB);
		return std::acos(cosTheta) * (180.0 / DX_PI_F); // ラジアンを度数に変換
	}

	float D2R(float deg) {
		return deg * (DX_PI_F / 180.0f);
	}
};

/*
投げ飛ばす仕組み Y軸編

pos.y += dy*dy;
int dyは初期値5

dy から1ずつ引いていく
 5, 4, 3,2,1,0,-1,-2,-3,-4,-5
25,16, 9,4,1,0, 1, 4, 9,16,25
subNum = dy > 0 ? dy*dy : -dy*dy;



*/