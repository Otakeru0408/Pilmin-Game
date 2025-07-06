#pragma once
#include <map>
#include <vector>
#include <string>
#include "DxLib.h"
#include "PilminManager.h"

class Resource {
public:
    //当たり判定の設定用構造体
    struct collisionInfo {
        int handle;
        int frame;
        int xdiv;
        int ydiv;
        int zdiv;
    };

    PilminManager* pManager;
private:
    //モデルを格納し名前で取得できるようにした
	std::map<std::string, int> modelHandles;
    //ロードしたモデルのうち当たり判定のあるもの
    std::vector<collisionInfo> colliderObjects;
public:

    int sound_throw = 0;
    int sound_change = 0;
    int sound_clear = 0;
    int sound_switch = 0;

    Resource() {
        pManager = new PilminManager();

        sound_throw = LoadSoundMem("Sound/sound_throw.mp3");
        sound_change = LoadSoundMem("Sound/sound_change.mp3");
        sound_clear = LoadSoundMem("Sound/sound_clear.mp3");
        sound_switch = LoadSoundMem("Sound/sound_switch.mp3");
    }
    //各クラスからこの関数を使うことでロードしたモデルを一元管理
    int LoadModel(const std::string& name, const std::string& filePath) {
        int handle = MV1LoadModel(filePath.c_str()); // モデルをロード
        if (handle != -1) {
            modelHandles[name] = handle;
        }

        return handle;
    }

    //当たり判定をせっていするための関数
    void SetupCollision(int handle, int frame, int xdiv,int ydiv,int zdiv) {
        collisionInfo c;
        c.handle = handle;
        c.frame = frame;
        c.xdiv = xdiv;
        c.ydiv = ydiv;
        c.zdiv = zdiv;
        colliderObjects.emplace_back(c);
    }

    void SetCollFrame(int _handle, int _frame) {
        
        for (auto& c : colliderObjects) {
            if (c.handle == _handle) {
                c.frame = _frame;
            }
        }
    }

    collisionInfo GetCollision(int n) {
        return colliderObjects[n];
    }

    collisionInfo GetCollisionByHandle(int h) {
        for (auto c : colliderObjects) {
            if (c.handle == h)
                return c;
        }
    }


    int GetCollisionCount() {
        return colliderObjects.size();
    }

    int GetHandle(std::string name) {
        return modelHandles[name];
    }
};