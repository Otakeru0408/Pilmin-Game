#pragma once
#include "DxLib.h"
#include "Pilmin.h"

class Pilmin_rock :public Pilmin {
private:
public:
	Pilmin_rock(int handle, int _stageWidth, int _stageDepth)
		:Pilmin(handle, _stageWidth, _stageDepth)
	{
		modelHandle = handle;
		stageWidth = _stageWidth;
		stageDepth_min = -_stageDepth;
		stageDepth_max = _stageDepth;
		MV1SetFrameVisible(modelHandle, 5, FALSE);
	}
};