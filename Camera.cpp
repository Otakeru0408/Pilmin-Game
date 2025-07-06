#include "Camera.h"

Camera::Camera(Player* p,Resource* r) {
	SetCameraPositionAndTarget_UpVecY(camPos,focusPos);
	player = p;
	resource = r;
}

void Camera::ProcessInput(uint32_t* key) {

	focusPos = player->GetPos();
	int mouseX, mouseY;

	// 左ボタンが押されたら、開始位置を記録
	if (GetMouseInput() & MOUSE_INPUT_LEFT) {
		if (!isDragging) { // 初回のみ記録
			GetMousePoint(&startMouseX, &startMouseY);
			isDragging = true;
		}
		else {
			// 現在の座標を取得
			GetMousePoint(&mouseX, &mouseY);

			// X座標の移動量を計算
			int deltaX = mouseX - startMouseX;
			int deltaY = mouseY - startMouseY;

			// 移動量をカメラの回転角度に変換（感度調整）
			angleY -= deltaX * rotateSpeed;
			angleX += deltaY * rotateSpeed;

			startMouseX = mouseX;
			startMouseY = mouseY;
		}
	}
	else {
		isDragging = false; // クリックを離したら終了
	}
	//角度制限
	if (angleX > R2D(PI / 3))angleX = R2D(PI / 3);
	if (angleX < R2D(-PI / 4))angleX = R2D(-PI / 4);

	
	camPos = {
		focusPos.x + distanceFromPlayer * cosf(D2R(angleY)) * cosf(D2R(angleX)),
		focusPos.y + distanceFromPlayer * sinf(D2R(angleX)),
		focusPos.z + distanceFromPlayer * sinf(D2R(angleY)) * cosf(D2R(angleX))
	};
	player->SetCamAngleX(angleX);
	player->SetCamAngleY(angleY);


	/*↓読み取りアクセス違反？
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
		//マウススクロールでズーム(障害物がないときだけ可能)
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
		//マウススクロールでズーム(障害物がないときだけ可能)
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