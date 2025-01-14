//=================================================
//
// タイトル画面背景の処理 (titlebg.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "titlebg.h"
#include "scene.h"
#include "sound.h"
#include "3dmeshfield.h"

//============================
//コンストラクタ
//============================
CTitleBG::CTitleBG(int nPriority) : CObject2D(nPriority)
{
    m_bAnimSwitch = false;
}

//============================
//デストラクタ
//============================
CTitleBG::~CTitleBG()
{

}

//=====================
// 背景初期化処理
//======================
HRESULT CTitleBG::Init()
{

    CRenderer* Renderer = CManager::GetRenderer();
    LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

    int nCntBG;

    //テクスチャ関連の初期化
    for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
    {
        m_aPosTexV[nCntBG] = 0.0f;
        m_nColTex[nCntBG] = 255;
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
        pVtx[0].col = D3DCOLOR_RGBA(m_nColTex[nCntBG], m_nColTex[nCntBG], m_nColTex[nCntBG], 255);
        pVtx[1].col = D3DCOLOR_RGBA(m_nColTex[nCntBG], m_nColTex[nCntBG], m_nColTex[nCntBG], 255);
        pVtx[2].col = D3DCOLOR_RGBA(m_nColTex[nCntBG], m_nColTex[nCntBG], m_nColTex[nCntBG], 255);
        pVtx[3].col = D3DCOLOR_RGBA(m_nColTex[nCntBG], m_nColTex[nCntBG], m_nColTex[nCntBG], 255);

        //テクスチャ座標の設定
        pVtx[0].tex = D3DXVECTOR2(m_aPosTexXV[nCntBG], m_aPosTexV[nCntBG]);
        pVtx[1].tex = D3DXVECTOR2(m_aPosTexXV[nCntBG] + 1.0f, m_aPosTexV[nCntBG]);
        pVtx[2].tex = D3DXVECTOR2(m_aPosTexXV[nCntBG], m_aPosTexV[nCntBG] + 1.0f);
        pVtx[3].tex = D3DXVECTOR2(m_aPosTexXV[nCntBG] + 1.0f, m_aPosTexV[nCntBG] + 1.0f);

        pVtx += 4;
    }

    //頂点バッファアンロック
    m_pVtxBuff->Unlock();
    CMeshField::Create(D3DXVECTOR3(50.0f, 3000.0f, 0.0f));

    CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_BGM);

    return S_OK;
}

//=================================
// 背景終了処理
//=================================
void CTitleBG::Uninit()
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
void CTitleBG::Update()
{
    int nCntBG;
    VERTEX_2D* pVtx; //頂点情報へのポインタ

    //頂点バッファロック
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    if (m_bAnimSwitch == false && CScene::GetScreenSwitch() == true)
    {
        m_nColTex[2] -= 4;
    }

    if (m_bAnimSwitch == true && CScene::GetScreenSwitch() == true)
    {
        m_nColTex[2] += 4;
    }

    if (m_nColTex[2] == 195)
    {
        m_bAnimSwitch = true;
    }

    if (m_nColTex[2] == 255)
    {
        m_bAnimSwitch = false;
    }

    //各種テクスチャの更新処理
    for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
    {
        if (nCntBG == 0)
        {
            m_aPosTexV[nCntBG] -= 0.00f;
        }



        //==========={ここまで}==================

        //頂点カラー
        pVtx[0].col = D3DCOLOR_RGBA(m_nColTex[nCntBG], m_nColTex[nCntBG], m_nColTex[nCntBG], 255);
        pVtx[1].col = D3DCOLOR_RGBA(m_nColTex[nCntBG], m_nColTex[nCntBG], m_nColTex[nCntBG], 255);
        pVtx[2].col = D3DCOLOR_RGBA(m_nColTex[nCntBG], m_nColTex[nCntBG], m_nColTex[nCntBG], 255);
        pVtx[3].col = D3DCOLOR_RGBA(m_nColTex[nCntBG], m_nColTex[nCntBG], m_nColTex[nCntBG], 255);

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
        
        CTitleBG::Uninit();
        return;
    }

}

//==================================================================================================
// 背景描画処理
//==================================================================================================
void CTitleBG::Draw()
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

void CTitleBG::BindTexture(LPDIRECT3DTEXTURE9 pTex[NUM_BG])
{
    for (int nCnt = 0; nCnt < NUM_BG; nCnt++)
    {
        m_pTexBuff[nCnt] = pTex[nCnt];
    }
}

//============================
//タイトルの生成処理
//============================
CTitleBG* CTitleBG::Create()
{
    CTitleBG* pTitleBG;

    pTitleBG = new CTitleBG;

    if (FAILED(pTitleBG->Init()))
    {
        delete pTitleBG;
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
            D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\EE_Title.png", &pTexture[nCntBG]);
            break;
        case 1:
            D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\PRESS_START_BIGBUT.png", &pTexture[nCntBG]);
            break;
        case 2:
            D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\PRESS_START_BIGBUT_BG.png", &pTexture[nCntBG]);
            break;
        case 3:
            D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\title_exit_button.png", &pTexture[nCntBG]);
            break;

        }

        if (!pTexture[nCntBG])
        {
            pTitleBG->Uninit();
            delete pTitleBG;
            return nullptr;
        }
    }

    pTitleBG->BindTexture(pTexture);

    return pTitleBG;
}
