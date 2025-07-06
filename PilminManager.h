#pragma once
#include "DxLib.h"
#include "Pilmin.h"
#include "Pilmin_rock.h"
#include "Pilmin_paper.h"
#include "RandomValue.h"
#include <vector>

using namespace std;

class PilminManager {
private:
	vector<Pilmin*> pilmins;
	int pilminCount = 0;
	RandomValue randVal;

	int stageWidth = 3500;
	int stageDepth = 5500;
	vector<int> roomMidpointZ = { 0,15000, 29900, 45000 };
	vector<int> pilminPerRoom = {10,20,30,40};
public:
	void InitPilmins(int stageNum) {
		//各部屋の中心座標
		VECTOR center = VGet(0, 0, roomMidpointZ[stageNum]);
		//新しい部屋にいくたびpilminが増える
		pilminCount += pilminPerRoom[stageNum];
		for (int i = 0;i < pilminPerRoom[stageNum];i++) {
				int whichPilmin = 0;
				if (i < 10)whichPilmin = stageNum;
				else whichPilmin = randVal.GetRandomVal(0, stageNum);

			int modelHandle = 0;

			switch (whichPilmin) {
			case 0:
			{
				modelHandle = MV1LoadModel("Model/Pilmin.mv1");
				Pilmin* pilmin = new Pilmin(modelHandle,
					stageWidth, stageDepth);
				pilmin->SetPos(VGet(
					center.x + randVal.GetRandomVal(-stageWidth, stageWidth),
					pilmin->GetPos().y,
					center.z + randVal.GetRandomVal(-stageDepth, stageDepth)));
				pilmin->SetPilminKind(whichPilmin);
				AddPilmins(pilmin);
				break;
			}
			case 1:
			{
				modelHandle = MV1LoadModel("Model/Pilmin_rock.mv1");
				Pilmin_rock* pilmin = new Pilmin_rock(modelHandle,
					stageWidth, stageDepth);
				pilmin->SetPos(VGet(
					center.x + randVal.GetRandomVal(-stageWidth, stageWidth),
					pilmin->GetPos().y,
					center.z + randVal.GetRandomVal(-stageDepth, stageDepth)));
				pilmin->SetPilminKind(whichPilmin);
				AddPilmins(pilmin);
				break;
			}
			case 2:
			{
				modelHandle = MV1LoadModel("Model/Pilmin_paper.mv1");
				Pilmin_paper* pilmin = new Pilmin_paper(modelHandle, 
					stageWidth, stageDepth);
				pilmin->SetPos(VGet(
					center.x + randVal.GetRandomVal(-stageWidth, stageWidth),
					pilmin->GetPos().y,
					center.z + randVal.GetRandomVal(-stageDepth, stageDepth)));
				pilmin->SetPilminKind(whichPilmin);
				AddPilmins(pilmin);
				break;
			}
			default:
				break;
			}
			
		}
	}

	void AddPilmins(Pilmin* p) {
		pilmins.emplace_back(p);
	}

	void Update(float deltaTime) {
		for (auto p : pilmins) {
			p->Update(deltaTime);
		}
	}

	void Draw() {
		for (auto p : pilmins) {
			p->Draw();
		}
		//DrawFormatString(10, 50, GetColor(255, 0, 0), "NowCatchPilmin:%d",
		//	GetPilminStateCount(Pilmin::State::Catched));
	}

	Pilmin* GetPilmin(int i) {
		return pilmins[i];
	}

	int GetPilminCount() { return pilmins.size(); }

	int GetPilminStateCount(Pilmin::State s) {
		int num = 0;
		for (auto p : pilmins) {
			if (p->GetNowState() == s)num++;
		}
		return num;
	}

};