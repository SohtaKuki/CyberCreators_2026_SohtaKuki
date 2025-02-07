//=================================================
//
// 2Dポリゴンの描画処理 (object2D.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "playerhpbar.h"
#include "manager.h"
#include "3dplayer.h"
#include "3dboss.h"
#include "3dtornado.h"
#include "bosscallui.h"
#include "3dgoalobj.h"
#include "tornado_reloadUI.h"

//======================
// コンストラクタ
//======================
CPlayerHpBar::CPlayerHpBar(int nPriority) : CObject2D(nPriority)
{
    m_fCurrentLife = 1.0f;
    m_fTargetLife = 1.0f;
    m_fLifeLerpSpeed = 0.15f;
}

//======================
// デストラクタ
//======================
CPlayerHpBar::~CPlayerHpBar()
{

}

//======================
// 初期化処理
//======================
HRESULT CPlayerHpBar::Init()
{
    CObject2D::Init();

    VERTEX_2D* pVtx;

    //頂点バッファをロックし、頂点情報へのポインタを取得
    CObject2D::GetBuff()->Lock(0, 0, (void**)&pVtx, 0);

    //頂点座標の設定
    pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(m_nSize.x, 0.0f, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(0.0f, m_nSize.y, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(m_nSize.x, m_nSize.y, 0.0f);

    pVtx[0].rhw = 1.0f;
    pVtx[1].rhw = 1.0f;
    pVtx[2].rhw = 1.0f;
    pVtx[3].rhw = 1.0f;

    pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

    //テクスチャ座標の設定
    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

    pVtx += 4;

    CObject2D::GetBuff()->Unlock();

    return S_OK;
}

//======================
// 終了処理
//======================
void CPlayerHpBar::Uninit()
{
    CObject2D::Uninit();
}

//======================
// 更新処理
//======================
void CPlayerHpBar::Update()
{
    // 現在の残量値を補間更新する
    float playerLife = C3dplayer::GetPlayerLife();
    float BossLife = C3dboss::GetBossLife();
    float TornadoLife = C3dplayer::GetTorNum();

    m_fTargetLife = playerLife; // プレイヤー体力の目標値を更新
    m_fTargetBossLife = BossLife;// ボス体力の目標値を更新
    m_fTargetTorLife = TornadoLife;// 竜巻の残量の目標値を更新

    // 線形補間で現在値を滑らかに変更
    m_fCurrentLife += (m_fTargetLife - m_fCurrentLife) * m_fLifeLerpSpeed;
    m_fBossCurrentLife += (m_fTargetBossLife - m_fBossCurrentLife) * m_fLifeLerpSpeed;
    m_fTorCurrentLife += (m_fTargetTorLife - m_fTorCurrentLife) * m_fLifeLerpSpeed;

    VERTEX_2D* pVtx;

    //頂点バッファをロックし、頂点情報へのポインタを取得
    CObject2D::GetBuff()->Lock(0, 0, (void**)&pVtx, 0);

    //ゲージの背後にある半透明赤のやつ
    if (m_nBarType == 0 || m_nBarType == 1)
    {
        pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.8f);
        pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.8f);
        pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.8f);
        pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.8f);
    }

    //通常のゲージ
    if (m_nBarType == 2 || m_nBarType == 3 || m_nBarType == 4 || m_nBarType == 5)
    {
        pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    }

    //プレイヤーの体力値
    if (m_nBarType == 0)
    {
        if (CBossCallUI::GetEndStat() == true && C3dboss::GetBossEntry() == true && C3dgoalobj::GetStageNum() == 2 ||
            CBossCallUI::GetEndStat() == false && C3dboss::GetBossEntry() == false && C3dgoalobj::GetStageNum() == 2
            || C3dgoalobj::GetStageNum() == 0 || C3dgoalobj::GetStageNum() == 1)
        {
            //頂点座標の更新
            float barWidth = 50 * 0.3585f * (m_fCurrentLife * 0.3585f);

            pVtx[0].pos.x = CObject2D::GetPos().x - 50 * 0.5f;
            pVtx[0].pos.y = CObject2D::GetPos().y - 50 * 0.5f;

            pVtx[1].pos.x = (m_nSize.x + 75) + CObject2D::GetPos().x + barWidth;
            pVtx[1].pos.y = CObject2D::GetPos().y - 50 * 0.5f;

            pVtx[2].pos.x = CObject2D::GetPos().x - 50 * 0.5f;
            pVtx[2].pos.y = (m_nSize.y + CObject2D::GetPos().y) + 50 * 0.5f;

            pVtx[3].pos.x = (m_nSize.x + 75) + CObject2D::GetPos().x + barWidth;
            pVtx[3].pos.y = (m_nSize.y + CObject2D::GetPos().y) + 50 * 0.5f;
        }
    }

    //ボスの体力値
    if (m_nBarType == 1)
    {
        if (C3dboss::GetBossEntry() == true)
        {
            float barBossWidth = 49.7 * 0.16f * (m_fBossCurrentLife * 0.16f);

            //頂点座標の更新
            pVtx[0].pos.x = CObject2D::GetPos().x - 50 * 0.5f;
            pVtx[0].pos.y = CObject2D::GetPos().y - 50 * 0.5f;

            pVtx[1].pos.x = (m_nSize.x + CObject2D::GetPos().x) + barBossWidth;
            pVtx[1].pos.y = CObject2D::GetPos().y - 50 * 0.5f;

            pVtx[2].pos.x = CObject2D::GetPos().x - 50 * 0.5f;
            pVtx[2].pos.y = (m_nSize.y + CObject2D::GetPos().y) + 50 * 0.5f;

            pVtx[3].pos.x = (m_nSize.x + CObject2D::GetPos().x) + barBossWidth;
            pVtx[3].pos.y = (m_nSize.y + CObject2D::GetPos().y) + 50 * 0.5f;
        }

        if (C3dboss::GetBossEntry() == false)
        {
            //頂点座標の更新
            for (int nCnt = 0; nCnt < 4; nCnt++)
            {
                pVtx[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            }
        }
    }

    //竜巻の残量値
    if (m_nBarType == 2)
    {
        if (CBossCallUI::GetEndStat() == true && C3dboss::GetBossEntry() == true && C3dgoalobj::GetStageNum() == 2 ||
            CBossCallUI::GetEndStat() == false && C3dboss::GetBossEntry() == false && C3dgoalobj::GetStageNum() == 2
            || C3dgoalobj::GetStageNum() == 0 || C3dgoalobj::GetStageNum() == 1)
        {
            float barTorWidth = 50 * 0.4f * (m_fTorCurrentLife * 5.5);

            //頂点座標の更新
            pVtx[0].pos.x = CObject2D::GetPos().x - 50 * 0.5f;
            pVtx[0].pos.y = CObject2D::GetPos().y - 50 * 0.5f;

            pVtx[1].pos.x = (m_nSize.x + CObject2D::GetPos().x) + barTorWidth;
            pVtx[1].pos.y = CObject2D::GetPos().y - 50 * 0.5f;

            pVtx[2].pos.x = CObject2D::GetPos().x - 50 * 0.5f;
            pVtx[2].pos.y = (m_nSize.y + CObject2D::GetPos().y) + 50 * 0.5f;

            pVtx[3].pos.x = (m_nSize.x + CObject2D::GetPos().x) + barTorWidth;
            pVtx[3].pos.y = (m_nSize.y + CObject2D::GetPos().y) + 50 * 0.5f;
        }


        //竜巻の発動可能数が2つだった場合
        if (C3dplayer::GetTorNum() >= 2)
        {
            CTornadoReloadUI::DisplayTornadoReloadUI(0, CTornadoReloadUI::UIDISPLAY::UI_HIDDEN);
        }

        //竜巻の発動可能数が2つ以下だった場合
        if (C3dplayer::GetTorNum() < 2)
        {
            CTornadoReloadUI::DisplayTornadoReloadUI(0, CTornadoReloadUI::UIDISPLAY::UI_DISPLAY);
        }

        if (CBossCallUI::GetEndStat() == true && C3dboss::GetBossEntry() == false && C3dgoalobj::GetStageNum() == 2)
        {
            //頂点座標の更新
            for (int nCnt = 0; nCnt < 4; nCnt++)
            {
                pVtx[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            }
        }
    }

    if (m_nBarType == 3)
    {
        if (CBossCallUI::GetEndStat() == true && C3dboss::GetBossEntry() == true && C3dgoalobj::GetStageNum() == 2 ||
            CBossCallUI::GetEndStat() == false && C3dboss::GetBossEntry() == false && C3dgoalobj::GetStageNum() == 2
            || C3dgoalobj::GetStageNum() == 0 || C3dgoalobj::GetStageNum() == 1)
        {
            //頂点座標の更新
            pVtx[0].pos.x = CObject2D::GetPos().x - 50 * 0.5f;
            pVtx[0].pos.y = CObject2D::GetPos().y - 50 * 0.5f;

            pVtx[1].pos.x = ((m_nSize.x + 75) + CObject2D::GetPos().x) + 50 * 0.3585f * (C3dplayer::GetPlayerLife() * 0.3585f);
            pVtx[1].pos.y = CObject2D::GetPos().y - 50 * 0.5f;

            pVtx[2].pos.x = CObject2D::GetPos().x - 50 * 0.5f;
            pVtx[2].pos.y = (m_nSize.y + CObject2D::GetPos().y) + 50 * 0.5f;

            pVtx[3].pos.x = ((m_nSize.x + 75) + CObject2D::GetPos().x) + 50 * 0.3585f * (C3dplayer::GetPlayerLife() * 0.3585f);
            pVtx[3].pos.y = (m_nSize.y + CObject2D::GetPos().y) + 50 * 0.5f;
        }

        if (CBossCallUI::GetEndStat() == true && C3dboss::GetBossEntry() == false && C3dgoalobj::GetStageNum() == 2)
        {
            //頂点座標の更新
            for (int nCnt = 0; nCnt < 4; nCnt++)
            {
                pVtx[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            }
        }
    }

    if (m_nBarType == 4)
    {
        if (C3dboss::GetBossEntry() == true)
        {
            //頂点座標の更新
            pVtx[0].pos.x = CObject2D::GetPos().x - 50 * 0.5f;
            pVtx[0].pos.y = CObject2D::GetPos().y - 50 * 0.5f;

            pVtx[1].pos.x = (m_nSize.x + CObject2D::GetPos().x) + 49.7 * 0.16f * (C3dboss::GetBossLife() * 0.16f);
            pVtx[1].pos.y = CObject2D::GetPos().y - 50 * 0.5f;

            pVtx[2].pos.x = CObject2D::GetPos().x - 50 * 0.5f;
            pVtx[2].pos.y = (m_nSize.y + CObject2D::GetPos().y) + 50 * 0.5f;

            pVtx[3].pos.x = (m_nSize.x + CObject2D::GetPos().x) + 49.7 * 0.16f * (C3dboss::GetBossLife() * 0.16f);
            pVtx[3].pos.y = (m_nSize.y + CObject2D::GetPos().y) + 50 * 0.5f;
        }

        if (C3dboss::GetBossEntry() == false)
        {
            //頂点座標の更新
            for (int nCnt = 0; nCnt < 4; nCnt++)
            {
                pVtx[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            }
        }
    }

    if (m_nBarType == 5)
    {
        if (CBossCallUI::GetEndStat() == true && C3dboss::GetBossEntry() == true && C3dgoalobj::GetStageNum() == 2 ||
            CBossCallUI::GetEndStat() == false && C3dboss::GetBossEntry() == false && C3dgoalobj::GetStageNum() == 2
            || C3dgoalobj::GetStageNum() == 0 || C3dgoalobj::GetStageNum() == 1)
        {
            //頂点座標の更新
            pVtx[0].pos.x = CObject2D::GetPos().x - 50 * 0.5f;
            pVtx[0].pos.y = CObject2D::GetPos().y - 50 * 0.5f;

            pVtx[1].pos.x = (m_nSize.x + CObject2D::GetPos().x) + 50 * 0.4f * (C3dplayer::GetTorNum() * 5.5);
            pVtx[1].pos.y = CObject2D::GetPos().y - 50 * 0.5f;

            pVtx[2].pos.x = CObject2D::GetPos().x - 50 * 0.5f;
            pVtx[2].pos.y = (m_nSize.y + CObject2D::GetPos().y) + 50 * 0.5f;

            pVtx[3].pos.x = (m_nSize.x + CObject2D::GetPos().x) + 50 * 0.4f * (C3dplayer::GetTorNum() * 5.5);
            pVtx[3].pos.y = (m_nSize.y + CObject2D::GetPos().y) + 50 * 0.5f;
        }

        //竜巻の発動可能数が2つだった場合
        if (C3dplayer::GetTorNum() >= 2)
        {
            CTornadoReloadUI::DisplayTornadoReloadUI(0, CTornadoReloadUI::UIDISPLAY::UI_HIDDEN);
        }

        //竜巻の発動可能数が2つ以下だった場合
        if (C3dplayer::GetTorNum() < 2)
        {
            CTornadoReloadUI::DisplayTornadoReloadUI(0, CTornadoReloadUI::UIDISPLAY::UI_DISPLAY);
        }

        if (CBossCallUI::GetEndStat() == true && C3dboss::GetBossEntry() == false && C3dgoalobj::GetStageNum() == 2)
        {
            //頂点座標の更新
            for (int nCnt = 0; nCnt < 4; nCnt++)
            {
                pVtx[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            }
        }
    }

    //頂点バッファをアンロックする
    CObject2D::GetBuff()->Unlock();

    int nFadeState = CFade::GetFadeState();

    if (nFadeState == CFade::FADE_OUT)
    {
        CPlayerHpBar::Uninit();
    }
}

//======================
// 描画処理
//======================
void CPlayerHpBar::Draw()
{
    CObject2D::Draw();
}

//======================
// オブジェクト生成処理
//======================
CPlayerHpBar* CPlayerHpBar::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nBarType)
{
    CPlayerHpBar* PlayerHpBar = new CPlayerHpBar;

    PlayerHpBar->CObject2D::SetPos(pos);

    PlayerHpBar->m_nSize = size;

    PlayerHpBar->m_nBarType = nBarType;

    PlayerHpBar->Init();

    //体力ゲージのテクスチャの読み込み
    if (nBarType == 3)
    {
        LPDIRECT3DTEXTURE9 pTexture;

        //テクスチャの読み込み
        D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\hpbar.png", &pTexture);

        PlayerHpBar->BindTexture(pTexture);
    }

    //ボスの体力ゲージのテクスチャの読み込み
    if (nBarType == 4)
    {
        LPDIRECT3DTEXTURE9 pTexture;

        //テクスチャの読み込み
        D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\boss_hpbar.png", &pTexture);

        PlayerHpBar->BindTexture(pTexture);
    }

    //竜巻の残量ゲージの読み込み
    if (nBarType == 2)
    {
        LPDIRECT3DTEXTURE9 pTexture;

        //テクスチャの読み込み
        D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\tornado_Gauge.png", &pTexture);

        PlayerHpBar->BindTexture(pTexture);
    }

    return PlayerHpBar;
}
