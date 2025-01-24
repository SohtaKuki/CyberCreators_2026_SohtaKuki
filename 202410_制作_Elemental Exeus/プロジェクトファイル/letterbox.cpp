//=================================================
//
// スタートコールUI表示処理 (startcallui.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "letterbox.h"
#include "3dboss.h"
#include "bosscallui.h"

bool CLetterbox::m_bStartComplete {};
bool CLetterbox::m_bStartCallFlag = false;
bool CLetterbox::m_bAnimationEnd = false;
//============================
//コンストラクタ
//============================
CLetterbox::CLetterbox(int nPriority) : CObject2D(nPriority)
{
    for (int nCntBG = 0; nCntBG < NUM_LTRBOX; nCntBG++)
    {
        m_pTexBuff[nCntBG] = nullptr;
    }

    m_bUIUse[0] = false;
    m_bUIUse[1] = false;
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
CLetterbox::~CLetterbox()
{

}

//=====================
// 背景初期化処理
//======================
HRESULT CLetterbox::Init()
{

    CRenderer* Renderer = CManager::GetRenderer();
    LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

    int nCntBG;

    //テクスチャ座標の開始位置（V値）の初期化
    for (nCntBG = 0; nCntBG < NUM_LTRBOX; nCntBG++)
    {
        m_aPosTexV[nCntBG] = 0.0f;
    }

    if (FAILED(pDevice->CreateVertexBuffer(
        sizeof(VERTEX_2D) * 4 * NUM_LTRBOX,
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

    for (nCntBG = 0; nCntBG < NUM_LTRBOX; nCntBG++)
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
void CLetterbox::Uninit()
{
    //テクスチャの破棄
    for (int nCntBG = 0; nCntBG < NUM_LTRBOX; nCntBG++)
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
void CLetterbox::Update()
{

    if (CBossCallUI::GetEndStat() == true && m_bUIUse[0] == false)
    {
        
        m_bUIUse[0] = true;
        m_bUIUse[1] = true;
        CManager::GetSound()->Stop(CSound::SOUND_LABEL_SE_ENTRY_WARNING);
        CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_BOSS_ENTRY_MOVE);
        m_bStartComplete = true;
    }

    if (CBossCallUI::GetEndStat() == true && m_bUIUse[0] == true)
    {
        if (m_nPos[0].y < 160.0f)
        {
            m_nPos[0].y += 4.0f;
        }

        if (m_nPos[1].y > 650.0f)
        {
            m_nPos[1].y -= 4.0f;
        }

        if (m_nPos[0].y >= -70.0f)
        {
            m_nPos[0].y = -70.0f;
        }


        if (m_nPos[1].y <= 650.0f)
        {
            m_nPos[1].y = 650.0f;
        }

        //if (m_nMissonAnim == 30)
        //{
        //    m_nMissonAnim = 0;
        //    m_bImageSwitch = true;
        //    CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_STARTCALL2);
        //}

        if (C3dboss::GetBossEntry() == true)
        {
            m_bUIUse[0] = false;
            m_bUIUse[1] = false;
            m_bAnimationEnd = true;
            m_bStartComplete = false;
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


    //フェードの状態を取得
    int nFadeState = CFade::GetFadeState();

    //フェードアウトになった場合、終了処理に移行
    if (nFadeState == CFade::FADE_OUT)
    {
        CLetterbox::Uninit();
    }

}

void CLetterbox::SetAlpha(int nAlpha)
{
    int nCntBG;
    VERTEX_2D* pVtx; //頂点情報へのポインタ

    //頂点バッファロック
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    for (nCntBG = 0; nCntBG < NUM_LTRBOX; nCntBG++)
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

        pVtx[0].col = D3DCOLOR_RGBA(0, 0, 0, 255);
        pVtx[1].col = D3DCOLOR_RGBA(0, 0, 0, 255);
        pVtx[2].col = D3DCOLOR_RGBA(0, 0, 0, 255);
        pVtx[3].col = D3DCOLOR_RGBA(0, 0, 0, 255);



        pVtx += 4;
    }


    //頂点バッファをアンロックする
    m_pVtxBuff->Unlock();
}

//=========================
// 背景描画処理
//=========================
void CLetterbox::Draw()
{
    int nCntBG;

    CRenderer* Renderer = CManager::GetRenderer();
    LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

    //頂点バッファをデータストリームに設定
    pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(CObject::VERTEX_2D));

    //頂点フォーマットの設定
    pDevice->SetFVF(FVF_VERTEX_2D);

    for (nCntBG = 0; nCntBG < NUM_LTRBOX; nCntBG++)
    {
        if (m_bUIUse[nCntBG] == true)
        {
            //テクスチャの設定
            pDevice->SetTexture(0, m_pTexBuff[nCntBG]);

            //ポリゴンの描画
            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBG * 4, 2);
        }
    }
}

void CLetterbox::BindTexture(LPDIRECT3DTEXTURE9 pTex[NUM_LTRBOX])
{
    for (int nCnt = 0; nCnt < NUM_LTRBOX; nCnt++)
    {
        m_pTexBuff[nCnt] = pTex[nCnt];
    }
}

//============================
//バフ付与UIの生成処理
//============================
CLetterbox* CLetterbox::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    CLetterbox* pLetterbox;

    pLetterbox = new CLetterbox;

    for (int nCntBG = 0; nCntBG < NUM_LTRBOX; nCntBG++)
    {
        pLetterbox->m_nPos[nCntBG] = pos;
        pLetterbox->m_nSize[nCntBG] = size;
    }

    pLetterbox->m_nPos[0].y = -100.0f;
    pLetterbox->m_nPos[0].x = 0.0f;
    pLetterbox->m_nSize[0].x = 2000.0f;
    pLetterbox->m_nSize[0].y = 150.0f;

    pLetterbox->m_nPos[1].y = 680.0f;
    pLetterbox->m_nPos[1].x = 0.0f;
    pLetterbox->m_nSize[1].x = 2000.0f;
    pLetterbox->m_nSize[1].y = 150.0f;

    if (FAILED(pLetterbox->Init()))
    {
        delete pLetterbox;
        return nullptr;
    }





    return pLetterbox;
}

void CLetterbox::StartCallUIBossFlag()
{
    m_bStartCallFlag = true;
}




