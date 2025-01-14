//=================================================
//
// タイトル画面背景の処理 (titlebg.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "stageselectbg.h"
#include "sound.h"

//============================
//コンストラクタ
//============================
CStageSelectBG::CStageSelectBG(int nPriority) : CObject2D(nPriority)
{

}

//============================
//デストラクタ
//============================
CStageSelectBG::~CStageSelectBG()
{

}

//=====================
// 背景初期化処理
//======================
HRESULT CStageSelectBG::Init()
{

    CRenderer* Renderer = CManager::GetRenderer();
    LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

    int nCntBG;

    //テクスチャ座標の開始位置（V値）の初期化
    for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
    {
        m_aPosTexV[nCntBG] = 0.0f;
    }

    if (FAILED(pDevice->CreateVertexBuffer(
        sizeof(VERTEX_2D) * 4 * NUM_BG,
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_2D,
        D3DPOOL_MANAGED,
        &m_pVtxBuff,
        nullptr)))
    {
        return E_FAIL;
    }

    VERTEX_2D* pVtx; //頂点情報へのポインタ

    //頂点バッファロック
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
    {
        //頂点座標の設定
        pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
        pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
        pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

        //rhwの設定
        pVtx[0].rhw = 1.0f;
        pVtx[1].rhw = 1.0f;
        pVtx[2].rhw = 1.0f;
        pVtx[3].rhw = 1.0f;

        //頂点カラー
        pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

        //テクスチャ座標の設定
        pVtx[0].tex = D3DXVECTOR2(m_aPosTexXV[nCntBG], m_aPosTexV[nCntBG]);
        pVtx[1].tex = D3DXVECTOR2(m_aPosTexXV[nCntBG] + 1.0f, m_aPosTexV[nCntBG]);
        pVtx[2].tex = D3DXVECTOR2(m_aPosTexXV[nCntBG], m_aPosTexV[nCntBG] + 1.0f);
        pVtx[3].tex = D3DXVECTOR2(m_aPosTexXV[nCntBG] + 1.0f, m_aPosTexV[nCntBG] + 1.0f);

        pVtx += 4;
    }

    //頂点バッファアンロック
    m_pVtxBuff->Unlock();

    return S_OK;
}

//=================================
// 背景終了処理
//=================================
void CStageSelectBG::Uninit()
{
    //テクスチャの破棄
    for (int nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
    {
        if (m_pTexBuff[nCntBG] != NULL)
        {
            m_pTexBuff[nCntBG]->Release();
            m_pTexBuff[nCntBG] = NULL;
        }
    }

    //頂点バッファの破棄
    if (m_pVtxBuff != NULL)
    {
        m_pVtxBuff->Release();
        m_pVtxBuff = NULL;
    }

    CObject::Release();
}

//=========================
// 背景更新処理
//=========================
void CStageSelectBG::Update()
{
    int nCntBG;
    VERTEX_2D* pVtx; //頂点情報へのポインタ

    //頂点バッファロック
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
    {
        if (nCntBG == 0)
        {
            m_aPosTexV[nCntBG] -= 0.00f;
        }
        //==========={ここから背景画像関連}==================
        else if (nCntBG == 1)
        {
            m_aPosTexV[nCntBG] -= 0.003f;
        }
        else if (nCntBG == 2)
        {
            m_aPosTexV[nCntBG] -= 0.0020f;
        }
        else if (nCntBG == 3)
        {
            m_aPosTexV[nCntBG] -= 0.0040f;
        }

        //==========={ここまで}==================

        //テクスチャ座標の設定
        pVtx[0].tex = D3DXVECTOR2(m_aPosTexXV[nCntBG], m_aPosTexV[nCntBG]);
        pVtx[1].tex = D3DXVECTOR2(m_aPosTexXV[nCntBG] + 1.0f, m_aPosTexV[nCntBG]);
        pVtx[2].tex = D3DXVECTOR2(m_aPosTexXV[nCntBG], m_aPosTexV[nCntBG] + 1.0f);
        pVtx[3].tex = D3DXVECTOR2(m_aPosTexXV[nCntBG] + 1.0f, m_aPosTexV[nCntBG] + 1.0f);

        pVtx += 4;
    }


    //頂点バッファをアンロックする
    m_pVtxBuff->Unlock();

    int nFadeState = CFade::GetFadeState();

    if (nFadeState == CFade::FADE_OUT)
    {
        CManager::GetSound()->Stop();
        CStageSelectBG::Uninit();
        return;
    }

}

//==================================================================================================
// 背景描画処理
//==================================================================================================
void CStageSelectBG::Draw()
{
    int nCntBG;

    CRenderer* Renderer = CManager::GetRenderer();
    LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

    //頂点バッファをデータストリームに設定
    pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

    //頂点フォーマットの設定
    pDevice->SetFVF(FVF_VERTEX_2D);

    for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
    {
        //テクスチャの設定
        pDevice->SetTexture(0, m_pTexBuff[nCntBG]);

        //ポリゴンの描画
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBG * 4, 2);
    }
}

void CStageSelectBG::BindTexture(LPDIRECT3DTEXTURE9 pTex[NUM_BG])
{
    for (int nCnt = 0; nCnt < NUM_BG; nCnt++)
    {
        m_pTexBuff[nCnt] = pTex[nCnt];
    }
}

//============================
//タイトルの生成処理
//============================
CStageSelectBG* CStageSelectBG::Create()
{
    CStageSelectBG* pStageSelectBG;

    pStageSelectBG = new CStageSelectBG;

    if (FAILED(pStageSelectBG->Init()))
    {
        delete pStageSelectBG;
        return nullptr;
    }

    int nCntBG;

    LPDIRECT3DTEXTURE9 pTexture[NUM_BG];

    //テクスチャ読み込み
    for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
    {
        switch (nCntBG)
        {
        case 0:
            D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\stageselect_bg.png", &pTexture[nCntBG]);
            break;
        case 1:
            D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\frozen_frontier_title_bg1.png", &pTexture[nCntBG]);
            break;
        case 2:
            D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\frozen_frontier_title_bg2.png", &pTexture[nCntBG]);
            break;
        case 3:
            D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\frozen_frontier_title_bg3.png", &pTexture[nCntBG]);
            break;
        }

        if (!pTexture[nCntBG])
        {
            pStageSelectBG->Uninit();
            delete pStageSelectBG;
            return nullptr;
        }
    }

    pStageSelectBG->BindTexture(pTexture);

    return pStageSelectBG;
}
