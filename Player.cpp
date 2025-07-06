#include "Player.h"
void Player::LoadModel(string name, const string& path) {
	modelHandle = resource->LoadModel(name, path);
	MV1SetFrameVisible(modelHandle, 0, FALSE);
	MV1SetScale(modelHandle, VGet(scaleRate,scaleRate,scaleRate));
}

void Player::ProcessInput(uint32_t* key) {

	//真ん前ベクトルと右ベクトルを計算
	forward = { cosf(D2R(camAngleY)),0,sinf(D2R(camAngleY)) };
	backward = VScale(forward, -1);
	right = { sinf(D2R(camAngleY)),0,-cosf(D2R(camAngleY)) };
	left = VScale(right, -1);
	//移動分のベクトルを計算
	vec = { 0,0,0 };
	if (key[KEY_INPUT_A] > 0) {
		vec = VAdd(vec ,VScale(right, moveSpeed));
	}
	if (key[KEY_INPUT_D] > 0) {
		vec = VAdd(vec , VScale(right, -moveSpeed));
	}
	if (key[KEY_INPUT_W] > 0) {
		vec = VAdd(vec , VScale(forward, -moveSpeed));
	}
	if (key[KEY_INPUT_S] > 0) {
		vec = VAdd(vec , VScale(forward, moveSpeed));
	}

	if (key[KEY_INPUT_SPACE] == 1) {
		ChangeVolumeSoundMem(256, resource->sound_change);
		PlaySoundMem(resource->sound_change, DX_PLAYTYPE_BACK);
		catchMode = (++catchMode) % maxCatchMode;
		if (catchMode == 2) {
			//vector<int>().swap(catchedNum);	//スローモードのために初期化
			catchedNum.clear();
			//スローモードに変わった時点での捕まえたpilminの配列
			for (int i = 0;i < resource->pManager->GetPilminCount();i++) {
				if (resource->pManager->GetPilmin(i)->GetNowState() == Pilmin::State::Catched) {
					catchedNum.emplace_back(i);
				}
			}
		}
	}

	//pilminを捕まえる関数
	CatchPilmin(&key[0]);

	//当たり判定のチェックをすべての対象で
	for (int i = 0;i < resource->GetCollisionCount();i++) {
		HitPolyDim = MV1CollCheck_Sphere(resource->GetCollision(i).handle,
			resource->GetCollision(i).frame,
			VAdd(pos, vec), 140);
		if (HitPolyDim.HitNum > 0) {
			vec = { 0,0,0 };
			break;
		}
	}
	
	//デバッグ用
	//DrawFormatString(100, 0, GetColor(255, 0, 0), "%d", HitPolyDim.HitNum);
}

void Player::CatchPilmin(uint32_t* key) {
	PilminManager* pManager = resource->pManager;

	VECTOR top = VScale(backward, 1000);
	VECTOR bottom = VScale(backward, 1000);
	VECTOR bottom2 = VScale(backward, 500);
	switch (catchMode) {
	case 0:	//未選択モード
		for (int i = 0;i < pManager->GetPilminCount();i++) {
			if (pManager->GetPilmin(i)->GetNowState() == Pilmin::State::Waiting) {
				pManager->GetPilmin(i)->SetNowState(Pilmin::State::Catched);
			}
		}
		break;
	case 1:			//キャッチモード
		
		DrawCone3D(VAdd(pos, VGet(top.x, top.y + 500, top.z)),
			VAdd(pos, bottom), 500, 16,	//r=500
			GetColor(0, 0, 0), GetColor(0, 0, 0), FALSE);

		for (int i = 0;i < pManager->GetPilminCount();i++) {
			//すべてのpilminに対して
			//ゾーンの中心地点との距離を測り、ゾーン内にいるなら
			float dist = VSize(VSub(pManager->GetPilmin(i)->GetPos(), VAdd(pos, bottom)));
			if (dist <= 500) {
				if (pManager->GetPilmin(i)->GetNowState() == Pilmin::State::Free
					|| pManager->GetPilmin(i)->GetNowState() == Pilmin::State::Carrying) {
					//捕まえた判定にする
					pManager->GetPilmin(i)->SetNowState(Pilmin::State::Catched);
					VECTOR _pos = pManager->GetPilmin(i)->GetPos();
					_pos.y += 100;		//捕まえた時ちょっと跳ねるように
					pManager->GetPilmin(i)->SetPos(_pos);
				}
				
			}

		}
		break;
	case 2: {				//スローモード(投げる)
		//投げるためのガイドを表示
		arc.ArcLine(VGet(pos.x, pos.y + 500, pos.z), backward, throwHeight, throwSpeed, gravity);
		//Spaceを押したときに、捕まえているpilminの配列を作成した。
		if (catchedNum.size() > 0) {
			if (nowWaiting >= catchedNum.size())nowWaiting = catchedNum.size() - 1;
			//Catched→waitingにする
			if (pManager->GetPilmin(catchedNum[nowWaiting])->GetNowState()
				== Pilmin::State::Catched) {
				pManager->GetPilmin(catchedNum[nowWaiting])->SetNowState(Pilmin::State::Waiting);
				pManager->GetPilmin(catchedNum[nowWaiting])->SetThrowSetting(backward, throwSpeed, throwHeight, gravity);
			}
			//waiting→positionを頭上に固定
			else if (pManager->GetPilmin(catchedNum[nowWaiting])->GetNowState()
				== Pilmin::State::Waiting) {
				pManager->GetPilmin(catchedNum[nowWaiting])->SetPos(VGet(pos.x, pos.y + 500, pos.z));
			}
		}

		int mouseInput = GetMouseInput();
		if (mouseInput & MOUSE_INPUT_RIGHT && prevMouseInput == 0) {
			ChangeVolumeSoundMem(128, resource->sound_throw);
			PlaySoundMem(resource->sound_throw,DX_PLAYTYPE_BACK);
			for (int i = 0;i < pManager->GetPilminCount();i++) {
				if (pManager->GetPilmin(i)->GetNowState()
					== Pilmin::State::Waiting) {
					pManager->GetPilmin(i)->SetNowState(Pilmin::State::Working);
					pManager->GetPilmin(catchedNum[nowWaiting])->SetThrowSetting(backward, throwSpeed, throwHeight, gravity);
				}
			}

			if (catchedNum.size() > 0)	nowWaiting = (++nowWaiting) % catchedNum.size();
		}
		prevMouseInput = mouseInput;

		break;
	}
	default:break;
	}

	//StateがCatchedなら主人公に追従するようにする
	for (int i = 0;i < pManager->GetPilminCount();i++) {
		if (pManager->GetPilmin(i)->GetNowState() == Pilmin::State::Catched
			|| pManager->GetPilmin(i)->GetNowState() == Pilmin::State::Carrying) {
			pManager->GetPilmin(i)->SetMainCharaPos(pos);
		}
	}
}

void Player::Update(float deltaTime) {
	//ライト位置設定
	pLight->SetPos(VGet(pos.x,pos.y-150,pos.z));
	pLight->Update(deltaTime);

	//ベクトルの移動分を追加
	pos = VAdd(pos, vec);

	//ステージの進度によって当たり判定のエリアを変える
	if (pos.z > stageDepths[0]) stageFrame = 0;
	if (pos.z > stageDepths[1])  stageFrame = 1;
	if (pos.z > stageDepths[2]) stageFrame = 2;
	if (pos.z > stageDepths[3]) stageFrame = 3;
	if (prevStageFrame < stageFrame) {		//次のステージに進んだ瞬間
		for (int i = 0;i < resource->pManager->GetPilminCount();i++) {
			PlaySoundMem(resource->sound_clear, DX_PLAYTYPE_BACK);
			if (resource->pManager->GetPilmin(i)->GetNowState() == Pilmin::State::Carrying) {
				resource->pManager->GetPilmin(i)->SetNowState(Pilmin::State::Catched);
			}
		}
		resource->pManager->InitPilmins(stageFrame);
	}
	prevStageFrame = stageFrame;
	resource->SetCollFrame(resource->GetHandle("Stage"), stageFrame);
	for (int i = 0;i < resource->pManager->GetPilminCount();i++) {
		/*int nowStageDepth = resource->pManager->GetPilmin(i)->GetStageDepth_max();
		if (nowStageDepth < stageDepths[stageFrame]) {
			resource->pManager->GetPilmin(i)->SetStageDepth_max(stageDepths[stageFrame+1]);
		}*/
		resource->pManager->GetPilmin(i)->SetStageDepth_max(stageDepths[stageFrame + 1]);
	}

	MV1SetPosition(modelHandle, pos);
	MV1SetRotationXYZ(modelHandle, VGet(0,D2R(-camAngleY) + DX_PI_F / 2, 0));

}

void Player::Draw() {
	MV1DrawModel(modelHandle);

	int num = 0;
	for (int i = 0;i < resource->pManager->GetPilminCount();i++) {
		if (resource->pManager->GetPilmin(i)->GetNowState() == Pilmin::State::Catched) {
			num++;
		}
	}
	resource->SetCollFrame(resource->GetHandle("Stage"), stageFrame);
	//DrawFormatString(10, 10, GetColor(255, 0, 0), "pos: x,%f y,%f z,%f,frame:%d", 
	//	pos.x, pos.y, pos.z, resource->GetCollisionByHandle(resource->GetHandle("Stage")).frame);

}
