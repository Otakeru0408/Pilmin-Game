#pragma once
#include "Dxlib.h"
#include "Resource.h"
#include "Pot.h"
#include "Fence.h"

#include <vector>

class PotGimmick {
private:
	Resource* resource;
	std::vector<Pot*> pots;
	Fence* fence;
	bool isReached = true;
public:
	PotGimmick(Resource* r);

	void Update(float);
	void Draw();

	Pot* GetPot(int i) { return pots[i]; }

	bool GetIsReached() { return isReached; }
};