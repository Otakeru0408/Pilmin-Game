#pragma once
#include "DxLib.h"
#include <vector>

class Cube {
private:
    VECTOR center;
    float width, height, depth;
    std::vector<VECTOR> vertexPositions; // 24���_�̍��W���X�g
    std::vector<VERTEX3D> vertices;
    std::vector<unsigned short> indices;

    float radius = 10;
    int divNum = 4;
    int difColor = GetColor(0, 0, 0);
    int spcColor = GetColor(0, 0, 0);
    int fillFlag = TRUE;

public:
    Cube(VECTOR v, float w, float h, float d)
        : center(v), width(w), height(h), depth(d) {
        //center.y += height / 2;
        InitializeVertexPositions(); // 24���_���W���v�Z
        InitializeVertices();        // VERTEX3D ���\�z
        InitializeIndices();         // �C���f�b�N�X��ݒ�
    }

    void SetCentorPos(VECTOR v) {
        center = v;
        //center.y += height / 2;
        InitializeVertexPositions(); // 24���_���W���v�Z
        InitializeVertices();        // VERTEX3D ���\�z
        InitializeIndices();         // �C���f�b�N�X��ݒ�
    }

    void InitializeVertexPositions() {
        vertexPositions = {
            // �O��
            VGet(center.x - width / 2, center.y - height / 2, center.z - depth / 2),
            VGet(center.x + width / 2, center.y - height / 2, center.z - depth / 2),
            VGet(center.x + width / 2, center.y + height / 2, center.z - depth / 2),
            VGet(center.x - width / 2, center.y + height / 2, center.z - depth / 2),

            // �w��
            VGet(center.x - width / 2, center.y - height / 2, center.z + depth / 2),
            VGet(center.x + width / 2, center.y - height / 2, center.z + depth / 2),
            VGet(center.x + width / 2, center.y + height / 2, center.z + depth / 2),
            VGet(center.x - width / 2, center.y + height / 2, center.z + depth / 2),

            // ����
            VGet(center.x - width / 2, center.y - height / 2, center.z - depth / 2),
            VGet(center.x - width / 2, center.y + height / 2, center.z - depth / 2),
            VGet(center.x - width / 2, center.y + height / 2, center.z + depth / 2),
            VGet(center.x - width / 2, center.y - height / 2, center.z + depth / 2),

            // �E��
            VGet(center.x + width / 2, center.y - height / 2, center.z - depth / 2),
            VGet(center.x + width / 2, center.y + height / 2, center.z - depth / 2),
            VGet(center.x + width / 2, center.y + height / 2, center.z + depth / 2),
            VGet(center.x + width / 2, center.y - height / 2, center.z + depth / 2),

            // ���
            VGet(center.x - width / 2, center.y + height / 2, center.z - depth / 2),
            VGet(center.x + width / 2, center.y + height / 2, center.z - depth / 2),
            VGet(center.x + width / 2, center.y + height / 2, center.z + depth / 2),
            VGet(center.x - width / 2, center.y + height / 2, center.z + depth / 2),

            // ����
            VGet(center.x - width / 2, center.y - height / 2, center.z - depth / 2),
            VGet(center.x + width / 2, center.y - height / 2, center.z - depth / 2),
            VGet(center.x + width / 2, center.y - height / 2, center.z + depth / 2),
            VGet(center.x - width / 2, center.y - height / 2, center.z + depth / 2)
        };
    }

    void InitializeVertices() {
        vertices.clear();
        for (int i = 0; i < 24; ++i) {
            VECTOR normal;
            if (i < 4)      normal = VGet(0, 0, -1); // �O��
            else if (i < 8) normal = VGet(0, 0, 1);  // �w��
            else if (i < 12) normal = VGet(-1, 0, 0); // ����
            else if (i < 16) normal = VGet(1, 0, 0);  // �E��
            else if (i < 20) normal = VGet(0, 1, 0);  // ���
            else normal = VGet(0, -1, 0); // ����

            float u = (i % 2 == 0) ? 0.0f : 1.0f;
            float v = (i % 4 < 2) ? 0.0f : 1.0f;

            VERTEX3D _v = { vertexPositions[i], normal, GetColor(0,0,0), GetColor(0,0,0), u, v, 0.0f, 0.0f };
            vertices.emplace_back(_v);
        }
    }

    void InitializeIndices() {
        indices = {
            0, 1, 2, 2, 3, 0,   // �O��
            4, 5, 6, 6, 7, 4,   // �w��
            8, 9, 10, 10, 11, 8,  // ����
            12, 13, 14, 14, 15, 12, // �E��
            16, 17, 18, 18, 19, 16, // ���
            20, 21, 22, 22, 23, 20  // ����
        };
    }

    void Draw() const {
        DrawPolygonIndexed3D(vertices.data(), (int)vertices.size(), indices.data(), (int)indices.size(), DX_NONE_GRAPH, TRUE);
    }

    void DrawFrameCapsule() const {
        // �e�ӂ��J�v�Z���ŕ`��
        DrawCapsule3D(vertexPositions[0], vertexPositions[1], radius, divNum, difColor, spcColor, fillFlag); // �O�� ����
        DrawCapsule3D(vertexPositions[1], vertexPositions[2], radius, divNum, difColor, spcColor, fillFlag); // �O�� �E��
        DrawCapsule3D(vertexPositions[2], vertexPositions[3], radius, divNum, difColor, spcColor, fillFlag); // �O�� ���
        DrawCapsule3D(vertexPositions[3], vertexPositions[0], radius, divNum, difColor, spcColor, fillFlag); // �O�� ����

        DrawCapsule3D(vertexPositions[4], vertexPositions[5], radius, divNum, difColor, spcColor, fillFlag); // �w�� ����
        DrawCapsule3D(vertexPositions[5], vertexPositions[6], radius, divNum, difColor, spcColor, fillFlag); // �w�� �E��
        DrawCapsule3D(vertexPositions[6], vertexPositions[7], radius, divNum, difColor, spcColor, fillFlag); // �w�� ���
        DrawCapsule3D(vertexPositions[7], vertexPositions[4], radius, divNum, difColor, spcColor, fillFlag); // �w�� ����

        DrawCapsule3D(vertexPositions[0], vertexPositions[4], radius, divNum, difColor, spcColor, fillFlag); // ������ �O-��
        DrawCapsule3D(vertexPositions[1], vertexPositions[5], radius, divNum, difColor, spcColor, fillFlag); // �E���� �O-��
        DrawCapsule3D(vertexPositions[2], vertexPositions[6], radius, divNum, difColor, spcColor, fillFlag); // �E���� ��-��
        DrawCapsule3D(vertexPositions[3], vertexPositions[7], radius, divNum, difColor, spcColor, fillFlag); // ������ ��-��
    }

};