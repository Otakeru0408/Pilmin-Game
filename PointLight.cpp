#include "PointLight.h"

PointLight::PointLight() {
	lightHandle = CreatePointLightHandle(pos, range, 1, 0.0006f, 0);
}

void PointLight::Update(float deltaTime) {
	SetLightPositionHandle(lightHandle, pos);
}

void PointLight::Draw() {

}