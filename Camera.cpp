#include "Camera.h"

Camera::Camera(Player* p,Resource* r) {
	SetCameraPositionAndTarget_UpVecY(camPos,focusPos);
	player = p;
	resource = r;
}

void Camera::ProcessInput(uint32_t* key) {

	focusPos = player->GetPos();
	int mouseX, mouseY;

	// ���{�^���������ꂽ��A�J�n�ʒu���L�^
	if (GetMouseInput() & MOUSE_INPUT_LEFT) {
		if (!isDragging) { // ����̂݋L�^
			GetMousePoint(&startMouseX, &startMouseY);
			isDragging = true;
		}
		else {
			// ���݂̍��W���擾
			GetMousePoint(&mouseX, &mouseY);

			// X���W�̈ړ��ʂ��v�Z
			int deltaX = mouseX - startMouseX;
			int deltaY = mouseY - startMouseY;

			// �ړ��ʂ��J�����̉�]�p�x�ɕϊ��i���x�����j
			angleY -= deltaX * rotateSpeed;
			angleX += deltaY * rotateSpeed;

			startMouseX = mouseX;
			startMouseY = mouseY;
		}
	}
	else {
		isDragging = false; // �N���b�N�𗣂�����I��
	}
	//�p�x����
	if (angleX > R2D(PI / 3))angleX = R2D(PI / 3);
	if (angleX < R2D(-PI / 4))angleX = R2D(-PI / 4);

	
	camPos = {
		focusPos.x + distanceFromPlayer * cosf(D2R(angleY)) * cosf(D2R(angleX)),
		focusPos.y + distanceFromPlayer * sinf(D2R(angleX)),
		focusPos.z + distanceFromPlayer * sinf(D2R(angleY)) * cosf(D2R(angleX))
	};
	player->SetCamAngleX(angleX);
	player->SetCamAngleY(angleY);


	/*���ǂݎ��A�N�Z�X�ᔽ�H
	for (int i = 0;i < resource->GetCollisionCount();i++) {
		result = MV1CollCheck_Line(
			resource->GetCollision(i).handle,
			resource->GetCollision(i).frame,
			camPos, player->GetPos());
		if (result.HitFlag) {
			camPos = result.HitPosition;
			break;
		}
	}
	if (!result.HitFlag) {
		//�}�E�X�X�N���[���ŃY�[��(��Q�����Ȃ��Ƃ������\)
		distanceFromPlayer -= GetMouseWheelRotVol() * 100;
		if (distanceFromPlayer < 500)distanceFromPlayer = 500;
	}*/

	result = MV1CollCheck_Line(
		resource->GetHandle("Stage"),
		resource->GetCollisionByHandle(resource->GetHandle("Stage")).frame,
		camPos,
		player->GetPos()
		);
	if (result.HitFlag) {
		camPos = result.HitPosition;
	}
	else {
		//�}�E�X�X�N���[���ŃY�[��(��Q�����Ȃ��Ƃ������\)
		distanceFromPlayer -= GetMouseWheelRotVol() * 100;
		if (distanceFromPlayer < 500)distanceFromPlayer = 500;
	}
}

void Camera::Update(float deltaTime) {
	
	SetCameraPositionAndTarget_UpVecY(camPos, focusPos);
	
}

void Camera::Draw() {

	//DrawFormatString(10, 30, GetColor(255, 0, 0), "cameraDistance:%f",distanceFromPlayer );

}