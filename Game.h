#pragma once
#include "DxLib.h"
#include <vector>
#include <string>
#include <random>
#include "Resource.h"
#include "Stage.h"
#include "Player.h"
#include "Camera.h"
#include "Pilmin.h"
#include "Cube.h"
#include "Obstacle.h"
#include "Switch.h"
#include "PotGimmick.h"

#define PI 3.14159265

class Game {
private:
	int Key[256]; // �L�[��������Ă���t���[�������i�[����
	int lightHandle = 0;
	VECTOR lightPos = { 0.0f,100.0f,0.0f };
	Resource* resource;
	Stage* stage;
	Player* player;
	Camera* camera;
	Obstacle* obstacle;
	Switch* button;
	PotGimmick* potGimmick;

	int pilminCount = 20;
	int stageFrame = 0;
	int fontHandle = 0;
	int resultFont1 = 0;
	int resultFont2 = 0;
	int BGM1 = 0;
	vector<string> functionName = { "None","CatchMode","ThrowMode" };

public:
	Game();
	void RunLoop();
	void TerminateGame();
	//Dxlib, ���, �J�����Ȃǂ̏����ݒ�
	void InitSystem();
	//�e��f�[�^�̓ǂݍ���
	void LoadData();
	//�v���C���[�̓��͂��Ǘ�
	void ProcessInput();
	//�Q�[���̍X�V���Ǘ�
	void UpdateGame(float);
	//�o�͂��Ǘ�
	void GenerateOutput();
	int gpUpdateKey();

	float D2R(float deg) {
		return deg * (DX_PI_F / 180.0f);
	}
};