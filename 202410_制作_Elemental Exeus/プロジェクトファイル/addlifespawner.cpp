//=================================================
//
// ステージマネージャー処理 (texture.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "addlifespawner.h"

#include "3ditem.h"
#include "3dhiteffect.h"
#include "ItemdropUI.h"


//======================
//コンストラクタ
//======================
CAddLifeSpawner::CAddLifeSpawner()
{

}

//======================
//デストラクタ
//======================
CAddLifeSpawner::~CAddLifeSpawner()
{

}

//======================
//初期化処理
//======================
HRESULT CAddLifeSpawner::Init()
{
    return S_OK;
}

//======================
// 終了処理
//======================
void CAddLifeSpawner::Uninit()
{

}

//======================
// 更新処理
//======================
void CAddLifeSpawner::Update()
{
    if (C3dplayer::GetPlayerLife() <= 24 && C3ditem::GetItemNum() == 1)
    {
        m_nTimer++;


        if (m_nTimer == 120)
        {
            CItemDropUI::DisplayAddLifeUI(); //アイテムが出現したUIを表示する

            int randoms[] = { ADDLIFE_SPAWNER_POS::ADDLIFE_POS_R_LOWER , ADDLIFE_SPAWNER_POS::ADDLIFE_POS_L_LOWER,
                             ADDLIFE_SPAWNER_POS::ADDLIFE_POS_R_UPPER,ADDLIFE_SPAWNER_POS::ADDLIFE_POS_L_UPPER };

            // 0から6までの乱数を生成
            m_nRandSelect = randoms[rand() % 3];

            CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_ITEM_SPAWN);

            if (m_nRandSelect == ADDLIFE_SPAWNER_POS::ADDLIFE_POS_R_LOWER)
            {
                C3ditem::Create(D3DXVECTOR3(700.0f ,50.0f ,- 10.0f), 3);
                C3dhiteffect::Create(D3DXVECTOR3(700.0f, 50.0f, -10.0f), D3DXVECTOR3(90.0f, 90.0f, 0.0f), m_rot);
            }

            if (m_nRandSelect == ADDLIFE_SPAWNER_POS::ADDLIFE_POS_R_UPPER)
            {
                C3ditem::Create(D3DXVECTOR3(700.0f, 280.0f, -10.0f), 3);
                C3dhiteffect::Create(D3DXVECTOR3(700.0f, 280.0f, -10.0f), D3DXVECTOR3(90.0f, 90.0f, 0.0f), m_rot);
            }

            if (m_nRandSelect == ADDLIFE_SPAWNER_POS::ADDLIFE_POS_L_LOWER)
            {
                C3ditem::Create(D3DXVECTOR3(-150.0f ,50.0f ,-10.0f), 3);
                C3dhiteffect::Create(D3DXVECTOR3(-150.0f, 50.0f, -10.0f), D3DXVECTOR3(90.0f, 90.0f, 0.0f), m_rot);
            }

            if (m_nRandSelect == ADDLIFE_SPAWNER_POS::ADDLIFE_POS_L_UPPER)
            {
                C3ditem::Create(D3DXVECTOR3(-150.0f, 280.0f, -10.0f), 3);
                C3dhiteffect::Create(D3DXVECTOR3(-150.0f, 280.0f, -10.0f), D3DXVECTOR3(90.0f, 90.0f, 0.0f), m_rot);
            }

            m_nTimer = 0;
        }
    }
}

//======================
// 描画処理
//======================
void CAddLifeSpawner::Draw()
{

}

//======================
// オブジェクト生成処理
//======================
CAddLifeSpawner* CAddLifeSpawner::Create(int nType)
{
    CAddLifeSpawner* D3DAddLifeSpawner = nullptr;

    D3DAddLifeSpawner = new CAddLifeSpawner;

    //初期化に成功した場合
    if (SUCCEEDED(D3DAddLifeSpawner->Init()))
    {
        
    }

    return D3DAddLifeSpawner;
}
