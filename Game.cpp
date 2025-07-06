#include "Game.h"

Game::Game() {
    InitSystem();

    resource = new Resource();
    stage = new Stage(resource);
    player = new Player(resource);
    camera = new Camera(player,resource);
    obstacle = new Obstacle(resource);
    button = new Switch(resource);
    potGimmick = new PotGimmick(resource);

    fontHandle = CreateFontToHandle("Arial Black", 24, 3);
    resultFont1 = CreateFontToHandle("Arial Black", 48, 3);
    resultFont2 = CreateFontToHandle("Arial Black", 24, 3);

    BGM1 = LoadSoundMem("Sound/BGM1.mp3");
    LoadData();
}

void Game::RunLoop() {
    //デルタタイムの計算用
    LONGLONG time = GetNowHiPerformanceCount();
    LONGLONG nowTime = 0;
    float deltaTime = 0;

    ChangeVolumeSoundMem(128, BGM1);
    PlaySoundMem(BGM1, DX_PLAYTYPE_LOOP);
    // while(裏画面を表画面に反映, メッセージ処理, 画面クリア)
    while (ScreenFlip() == 0 && ProcessMessage() == 0
        && ClearDrawScreen() == 0 && gpUpdateKey() == 0) {

        //デルタタイムの計算
        nowTime = GetNowHiPerformanceCount();
        deltaTime = (nowTime - time) / 1000000.0f;

        //当たり判定のコリジョン設定
        for (int i = 0;i < resource->GetCollisionCount();i++) {
            MV1SetupCollInfo(
                resource->GetCollision(i).handle,
                resource->GetCollision(i).frame,
                resource->GetCollision(i).xdiv,
                resource->GetCollision(i).ydiv,
                resource->GetCollision(i).zdiv
            );
        }

        //入力・更新・出力のループ
        ProcessInput();
        UpdateGame(deltaTime);
        GenerateOutput();

        time = nowTime;

        if (Key[KEY_INPUT_ESCAPE] == 1) {
            break;
        }
    }
}

void Game::ProcessInput() { //uint32_t* keyを引数にして、&key[0]を渡すとkeyが使える
    player->ProcessInput((uint32_t*)&Key[0]);
    camera->ProcessInput((uint32_t*)&Key[0]);
}

void Game::UpdateGame(float deltaTime) {
    stage->Update(deltaTime);
    obstacle->Update(deltaTime);
    button->Update(deltaTime);
    player->Update(deltaTime);
    resource->pManager->Update(deltaTime);
    camera->Update(deltaTime);

    if (stageFrame >= 2) {
        potGimmick->Update(deltaTime);
    }

    stageFrame = player->GetStageFrame();
    //DrawFormatString(20, 20, GetColor(0, 255, 0), "%f", deltaTime);
}

void Game::GenerateOutput() {
    stage->Draw();
    obstacle->Draw();
    if (stageFrame >= 2) {
        potGimmick->Draw();
    }
    button->Draw();
    player->Draw();
    resource->pManager->Draw();

    camera->Draw();

    // 半透明描画モードを設定（アルファ値128で50%透明）
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    // 長方形を描画（左上(50,50)から右下(300,200)まで）
    DrawBox(10, 20, 500, 160, GetColor(255, 255, 255), TRUE);
    // 描画モードを元に戻す
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    int posY = 0;
    DrawFormatString(0,0,GetColor(0,0,0),"stageFrame:%d",stageFrame);
    DrawFormatStringToHandle(30, posY+=30, GetColor(0, 255, 0), fontHandle, "Stage : %d", stageFrame+1);
    DrawFormatStringToHandle(30, posY += 30, GetColor(0, 255, 0), fontHandle, "Number of captured pilmin : %d",
        resource->pManager->GetPilminStateCount(Pilmin::State::Catched));
    DrawFormatStringToHandle(30, posY += 30, GetColor(0, 255, 0), fontHandle, "Distance from character : %.0f",
        camera->GetDistance());
    DrawFormatStringToHandle(30, posY += 30, GetColor(0, 255, 0), fontHandle, "SpaceFunction : %s",
        functionName[player->GetCatchMode()].c_str());
    
    posY = 600;
    if (stageFrame < 3) {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
        if (stageFrame == 2)DrawBox(40, posY += 30, 700, posY + 70, GetColor(255, 255, 0), TRUE);
        else DrawBox(40, posY += 30, 540, posY + 70, GetColor(255, 255, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
    
    switch (stageFrame) {
    case 0: {
        int c = obstacle->GetNowCount() >= obstacle->GetMustCount() ? GetColor(0, 220, 0) : GetColor(255, 0, 0);
        DrawFormatStringToHandle(50, posY += 20, c, fontHandle, "Gimmick -> %d / %d",
            obstacle->GetNowCount(), obstacle->GetMustCount());
        break;
    }
    case 1: {
        int c = button->GetNowCount() >= button->GetMustCount() ? GetColor(0, 255, 0) : GetColor(255, 0, 0);
        DrawFormatStringToHandle(50, posY += 20, c, fontHandle, "Gimmick -> %d / %d",
            button->GetNowCount(), button->GetMustCount());
        break;
    }
    case 2: {
        int c = potGimmick->GetIsReached() ? GetColor(0, 255, 0) : GetColor(255, 0, 0);
        DrawFormatStringToHandle(50, posY += 20, c, fontHandle, "Gimmick -> pot1: %d / %d, pot2: %d / %d, pot3: %d / %d",
            potGimmick->GetPot(0)->GetNowCount(), potGimmick->GetPot(0)->GetMustCount(),
            potGimmick->GetPot(1)->GetNowCount(), potGimmick->GetPot(1)->GetMustCount(),
            potGimmick->GetPot(2)->GetNowCount(), potGimmick->GetPot(2)->GetMustCount()
        );
        break;
    }
    case 3:
        DrawBox(0, 300, 450, 470, GetColor(0, 0, 0), TRUE);
        DrawStringToHandle(10, 350,  "Congratulation!",GetColor(255, 255, 0), resultFont1);
        DrawStringToHandle(10, 400,  "Game Clear",GetColor(255, 200, 0), resultFont2);
        break;
    default:
        break;
    }
    
}

void Game::InitSystem() {
    ChangeWindowMode(TRUE);
    //画面サイズの設定
    SetGraphMode(1280, 720, 32);
    SetBackgroundColor(0,0,0);

    DxLib_Init();
    SetDrawScreen(DX_SCREEN_BACK);
    //ウィンドウモード変更と初期化と裏画面設定

    SetUseLighting(TRUE);
    SetLightDirection(VGet(0.0f, 1.0f, 0.0f));
    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);
}

void Game::LoadData() {
    player->LoadModel(player->GetName(), "Model/Player.mv1");
    resource->SetupCollision(player->GetHandle(), -1, 0, 0, 0);
    stage->LoadModel("Stage", "Model/Stage_All.mv1");
    resource->SetupCollision(stage->GetHandle(), 0, 8,8,8);
    obstacle->LoadModel("Obstacle", "Model/Obstacle.mv1");
    resource->SetupCollision(obstacle->GetHandle(), -1, 2, 2, 2);
    button->LoadModel("Switch_OFF", "Model/Switch_OFF.mv1");
    button->LoadSecondModel("Switch_ON", "Model/Switch_ON.mv1");
    resource->SetupCollision(button->GetHandle1(), 1, 2, 2, 2);
    resource->SetupCollision(button->GetHandle2(), 1, 2, 2, 2);

    camera->SetStageHandle(stage->GetHandle());

    resource->pManager->InitPilmins(0);
}

int Game::gpUpdateKey() {
    char tmpKey[256]; // 現在のキーの入力状態を格納する
    GetHitKeyStateAll(tmpKey); // 全てのキーの入力状態を得る
    for (int i = 0; i < 256; i++) {
        if (tmpKey[i] != 0) { // i番のキーコードに対応するキーが押されていたら
            Key[i]++;     // 加算
        }
        else {              // 押されていなければ
            Key[i] = 0;   // 0にする
        }
    }
    return 0;
}


void Game::TerminateGame() {
    DxLib_End(); // DXライブラリ終了処理
}