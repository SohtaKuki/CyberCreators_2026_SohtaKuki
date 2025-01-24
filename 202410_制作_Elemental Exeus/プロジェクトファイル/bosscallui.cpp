//=================================================
//
// スタートコールUI表示処理 (startcallui.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "bosscallui.h"

bool CBossCallUI::m_bUse[NUM_ICON] = {};
bool CBossCallUI::m_bStartComplete = false;
bool CBossCallUI::m_bStartCallFlag = false;
bool CBossCallUI::m_bAnimationEnd = false;
bool CBossCallUI::m_bBeginCallUI = false;

//============================
//コンストラクタ
//============================
CBossCallUI::CBossCallUI(int nPriority) : CObject2D(nPriority)
{
    for (int nCntBG = 0; nCntBG < NUM_ICON; nCntBG++)
    {
        m_pTexBuff[nCntBG] = nullptr;
    }

    m_bUse[0] = false;
    m_bUse[1] = false;
    m_bUse[2] = false;
    m_pVtxBuff = nullptr;
    m_nAlphaCnt = 255;
    m_nMissonAnim = 0;
    m_bAlphaSwitch = false;
    m_bImageSwitch = false;
    m_bStartComplete = false;
    m_bStartCallFlag = false;
    m_bAnimationEnd = false;
}

//============================
//デストラクタ
//============================
CBossCallUI::~CBossCallUI()
{

}

//=====================
// 背景初期化処理
//======================
HRESULT CBossCallUI::Init()
{

    CRenderer* Renderer = CManager::GetRenderer();
    LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

    int nCntBG;

    //テクスチャ座標の開始位置（V値）の初期化
    for (nCntBG = 0; nCntBG < NUM_ICON; nCntBG++)
    {
        m_aPosTexV[nCntBG] = 0.0f;
    }

    if (FAILED(pDevice->CreateVertexBuffer(
        sizeof(VERTEX_2D) * 4 * NUM_ICON,
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

    for (nCntBG = 0; nCntBG < NUM_ICON; nCntBG++)
    {

        //頂点座標の設定
        pVtx[0].pos = D3DXVECTOR3((m_nPos[nCntBG].x - m_nSize[nCntBG].x), m_nPos[nCntBG].y, 0.0f);
        pVtx[1].pos = D3DXVECTOR3((m_nPos[nCntBG].x + m_nSize[nCntBG].x), m_nPos[nCntBG].y, 0.0f);
        pVtx[2].pos = D3DXVECTOR3((m_nPos[nCntBG].x - m_nSize[nCntBG].x), m_nPos[nCntBG].y + m_nSize[nCntBG].y, 0.0f);
        pVtx[3].pos = D3DXVECTOR3((m_nPos[nCntBG].x + m_nSize[nCntBG].x), m_nPos[nCntBG].y + m_nSize[nCntBG].y, 0.0f);

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
void CBossCallUI::Uninit()
{
    //テクスチャの破棄
    for (int nCntBG = 0; nCntBG < NUM_ICON; nCntBG++)
    {
        if (m_pTexBuff[nCntBG] != nullptr)
        {
            m_pTexBuff[nCntBG]->Release();
            m_pTexBuff[nCntBG] = nullptr;
        }
    }

    //頂点バッファの破棄
    if (m_pVtxBuff != nullptr)
    {
        m_pVtxBuff->Release();
        m_pVtxBuff = nullptr;
    }

    CObject::Release();
}


//=========================
// 背景更新処理
//=========================
void CBossCallUI::Update()
{
    //ゲームが進行可能の時のみ通す
    if (CScene::GetUpdateStat() == true)
    {

        if (m_bStartCallFlag == true && m_bUse[0] == false && m_bImageSwitch == false)
        {

            m_bUse[0] = true;
        }

        if (m_bStartCallFlag == true && m_bUse[0] == true)
        {
            if (m_nPos[0].y < 160.0f)
            {
                m_nPos[0].y += 4.0f;
            }

            if (m_nPos[0].y >= 160.0f)
            {
                m_nPos[0].y = 160.0f;
                m_nMissonAnim++;
            }


            if (m_nMissonAnim == 30)
            {
                m_nMissonAnim = 0;
                m_bImageSwitch = true;

            }
        }

        //=================
        //別テクスチャを表示させる
        //================
        if (m_bImageSwitch == true)
        {
            if (m_bUse[1] == false && m_bUse[2] == false && m_bAnimationEnd == false)
            {
                CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_ENTRY_WARNING);
                m_bUse[1] = true;
                m_bUse[2] = true;
                m_bBeginCallUI = true;
            }

            //ここで減速
            if (m_nPos[1].x >= 600.0f && m_nPos[1].x < 800.0f)
            {
                m_nPos[1].x -= 1.5f;
                m_nPos[2].x += 1.5f;
                m_bStartComplete = true;
            }

            //減速後の加速
            if (m_nPos[1].x <= 600.0f)
            {
                m_nPos[1].x -= 30.0f;
                m_nPos[2].x += 30.0f;
            }

            //初動
            if (m_nPos[1].x >= 800.0f)
            {
                m_nPos[1].x -= 15.0f;
                m_nPos[2].x += 15.0f;
            }

            if (m_nPos[1].x < -1500.0f && m_nPos[2].x > -1500.0f)
            {
                m_bUse[0] = false;
                m_bUse[1] = false;
                m_bUse[2] = false;
                m_bAnimationEnd = true;
                m_bBeginCallUI = false;
            }
        }


        if (m_bAlphaSwitch == false)
        {
            m_nAlphaCnt--;
        }

        if (m_bAlphaSwitch == true)
        {
            m_nAlphaCnt++;
        }

        if (m_nAlphaCnt == 205)
        {
            m_bAlphaSwitch = true;
        }

        if (m_nAlphaCnt == 255)
        {
            m_bAlphaSwitch = false;
        }

        SetAlpha(m_nAlphaCnt);

    }

    //フェードの状態を取得
    int nFadeState = CFade::GetFadeState();

    //フェードアウトになった場合、終了処理に移行
    if (nFadeState == CFade::FADE_OUT)
    {
        CBossCallUI::Uninit();
    }
}

void CBossCallUI::SetAlpha(int nAlpha)
{
    int nCntBG;
    VERTEX_2D* pVtx; //頂点情報へのポインタ

    //頂点バッファロック
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    for (nCntBG = 0; nCntBG < NUM_ICON; nCntBG++)
    {

        //if (nCntBG == 0)
        //{
        //    m_aPosTexV[nCntBG] -= 0.00f;
        //}
        //else if (nCntBG == 1)
        //{
        //    m_aPosTexV[nCntBG] -= 0.003f;
        //}
        //else if (nCntBG == 2)
        //{
        //    m_aPosTexV[nCntBG] -= 0.0020f;
        //}

        ////テクスチャ座標の設定
        //pVtx[0].tex = D3DXVECTOR2(m_aPosTexXV[nCntBG], m_aPosTexV[nCntBG]);
        //pVtx[1].tex = D3DXVECTOR2(m_aPosTexXV[nCntBG] + 1.0f, m_aPosTexV[nCntBG]);
        //pVtx[2].tex = D3DXVECTOR2(m_aPosTexXV[nCntBG], m_aPosTexV[nCntBG] + 1.0f);
        //pVtx[3].tex = D3DXVECTOR2(m_aPosTexXV[nCntBG] + 1.0f, m_aPosTexV[nCntBG] + 1.0f);

        pVtx[0].pos = D3DXVECTOR3((m_nPos[nCntBG].x - m_nSize[nCntBG].x), m_nPos[nCntBG].y, 0.0f);
        pVtx[1].pos = D3DXVECTOR3((m_nPos[nCntBG].x + m_nSize[nCntBG].x), m_nPos[nCntBG].y, 0.0f);
        pVtx[2].pos = D3DXVECTOR3((m_nPos[nCntBG].x - m_nSize[nCntBG].x), m_nPos[nCntBG].y + m_nSize[nCntBG].y, 0.0f);
        pVtx[3].pos = D3DXVECTOR3((m_nPos[nCntBG].x + m_nSize[nCntBG].x), m_nPos[nCntBG].y + m_nSize[nCntBG].y, 0.0f);

        pVtx[0].col = D3DCOLOR_RGBA(m_nAlphaCnt, m_nAlphaCnt, m_nAlphaCnt, m_nAlphaCnt);
        pVtx[1].col = D3DCOLOR_RGBA(m_nAlphaCnt, m_nAlphaCnt, m_nAlphaCnt, m_nAlphaCnt);
        pVtx[2].col = D3DCOLOR_RGBA(m_nAlphaCnt, m_nAlphaCnt, m_nAlphaCnt, m_nAlphaCnt);
        pVtx[3].col = D3DCOLOR_RGBA(m_nAlphaCnt, m_nAlphaCnt, m_nAlphaCnt, m_nAlphaCnt);



        pVtx += 4;
    }


    //頂点バッファをアンロックする
    m_pVtxBuff->Unlock();
}

//=========================
// 背景描画処理
//=========================
void CBossCallUI::Draw()
{
    int nCntBG;

    CRenderer* Renderer = CManager::GetRenderer();
    LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

    //頂点バッファをデータストリームに設定
    pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(CObject::VERTEX_2D));

    //頂点フォーマットの設定
    pDevice->SetFVF(FVF_VERTEX_2D);

    for (nCntBG = 0; nCntBG < NUM_ICON; nCntBG++)
    {
        if (m_bUse[nCntBG] == true)
        {
            //テクスチャの設定
            pDevice->SetTexture(0, m_pTexBuff[nCntBG]);

            //ポリゴンの描画
            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBG * 4, 2);
        }
    }
}

void CBossCallUI::BindTexture(LPDIRECT3DTEXTURE9 pTex[NUM_ICON])
{
    for (int nCnt = 0; nCnt < NUM_ICON; nCnt++)
    {
        m_pTexBuff[nCnt] = pTex[nCnt];
    }
}

//============================
//バフ付与UIの生成処理
//============================
CBossCallUI* CBossCallUI::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    CBossCallUI* pBossCallUI;

    pBossCallUI = new CBossCallUI;

    for (int nCntBG = 0; nCntBG < NUM_ICON; nCntBG++)
    {
        pBossCallUI->m_nPos[nCntBG] = pos;
        pBossCallUI->m_nSize[nCntBG] = size;
    }

    pBossCallUI->m_nPos[1].y = 520.0f;
    pBossCallUI->m_nPos[1].x = 2200.0f;
    pBossCallUI->m_nSize[1].x = 1000.0f;
    pBossCallUI->m_nSize[1].y = 300.0f;

    pBossCallUI->m_nPos[2].y = -100.0f;
    pBossCallUI->m_nPos[2].x = -1200.0f;
    pBossCallUI->m_nSize[2].x = 1000.0f;
    pBossCallUI->m_nSize[2].y = 300.0f;

    if (FAILED(pBossCallUI->Init()))
    {
        delete pBossCallUI;
        return nullptr;
    }

    int nCntBG;

    LPDIRECT3DTEXTURE9 pTexture[NUM_ICON];

    //テクスチャ読み込み
    for (nCntBG = 0; nCntBG < NUM_ICON; nCntBG++)
    {
        switch (nCntBG)
        {
        case 0:
            D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\bosscallui_text.png", &pTexture[nCntBG]);
            break;
        case 1:
            D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\boss_warning.png", &pTexture[nCntBG]);
            break;
        case 2:
            D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\boss_warning.png", &pTexture[nCntBG]);
            break;
        }

        if (!pTexture[nCntBG])
        {
            pBossCallUI->Uninit();
            delete pBossCallUI;
            return nullptr;
        }
    }

    pBossCallUI->BindTexture(pTexture);

    return pBossCallUI;
}

void CBossCallUI::StartCallUIBossFlag()
{
    m_bStartCallFlag = true;
}

//============================
//UIを表示させる処理
//============================
bool CBossCallUI::DisplayBossCallUI(int nDisplayID, int DisplayOption)
{
    //表示する場合
    if (DisplayOption == CBossCallUI::UIDISPLAY::UI_DISPLAY)
    {
        m_bUse[nDisplayID] = true;
    }

    //非表示にする場合
    if (DisplayOption == CBossCallUI::UIDISPLAY::UI_HIDDEN)
    {
        m_bUse[nDisplayID] = false;
    }

    return m_bUse[nDisplayID];
}


