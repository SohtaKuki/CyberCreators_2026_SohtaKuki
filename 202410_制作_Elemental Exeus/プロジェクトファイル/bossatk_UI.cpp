//=================================================
//
// スタートコールUI表示処理 (startcallui.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "bossatk_UI.h"

bool CBossATKUI::m_bUse[NUM_ICON] = {};
bool CBossATKUI::m_bEndComplete = true;

//============================
//コンストラクタ
//============================
CBossATKUI::CBossATKUI(int nPriority) : CObject2D(nPriority)
{
    for (int nCntBG = 0; nCntBG < NUM_ICON; nCntBG++)
    {
        m_pTexBuff[nCntBG] = nullptr;
        m_bUse[nCntBG] = false;
    }

    m_pVtxBuff = nullptr;
    m_nAlphaCnt = 255;
    m_nMissonAnim = 0;
    m_bAlphaSwitch = false;
    m_bImageSwitch = false;
    m_bEndComplete = true;
}

//============================
//デストラクタ
//============================
CBossATKUI::~CBossATKUI()
{

}

//=====================
// 背景初期化処理
//======================
HRESULT CBossATKUI::Init()
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
            pVtx[0].pos = D3DXVECTOR3(m_nPos[nCntBG].x - m_nSize[nCntBG].x, m_nPos[nCntBG].y, 0.0f);
            pVtx[1].pos = D3DXVECTOR3(m_nPos[nCntBG].x + m_nSize[nCntBG].x, m_nPos[nCntBG].y, 0.0f);
            pVtx[2].pos = D3DXVECTOR3(m_nPos[nCntBG].x - m_nSize[nCntBG].x, m_nPos[nCntBG].y + m_nSize[nCntBG].y, 0.0f);
            pVtx[3].pos = D3DXVECTOR3(m_nPos[nCntBG].x + m_nSize[nCntBG].x, m_nPos[nCntBG].y + m_nSize[nCntBG].y, 0.0f);

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
void CBossATKUI::Uninit()
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
void CBossATKUI::Update()
{

    //フェードの状態を取得
    int nFadeState = CFade::GetFadeState();

    //フェードアウトになった場合、終了処理に移行
    if (nFadeState == CFade::FADE_OUT)
    {
        CBossATKUI::Uninit();
        return;
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

void CBossATKUI::SetAlpha(int nAlpha)
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
void CBossATKUI::Draw()
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

void CBossATKUI::BindTexture(LPDIRECT3DTEXTURE9 pTex[NUM_ICON])
{
    for (int nCnt = 0; nCnt < NUM_ICON; nCnt++)
    {
        m_pTexBuff[nCnt] = pTex[nCnt];
    }
}

//============================
//バフ付与UIの生成処理
//============================
CBossATKUI* CBossATKUI::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    CBossATKUI* pBossATKUI;

    pBossATKUI = new CBossATKUI;

    for (int nCntBG = 0; nCntBG < NUM_ICON; nCntBG++)
    {
        pBossATKUI->m_nPos[nCntBG] = pos;
        pBossATKUI->m_nSize[nCntBG] = size;
    }

    if (FAILED(pBossATKUI->Init()))
    {
        delete pBossATKUI;
        return nullptr;
    }

    int nCntBG;

    LPDIRECT3DTEXTURE9 pTexture[NUM_ICON];

    //テクスチャ読み込み
    for (nCntBG = 0; nCntBG < NUM_ICON; nCntBG++)
    {
        switch (nCntBG)
        {
        case 0: //通常攻撃時、下段左側にボスがいる想定
            D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\BSATKWARNING_UI\\warning_R_lowerDASH.png", &pTexture[nCntBG]);
            break;
        case 1: //通常攻撃時、下段右側にボスがいる想定
            D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\BSATKWARNING_UI\\warning_L_lowerDASH.png", &pTexture[nCntBG]);
            break;
        case 2: //通常攻撃時、上段左側にボスがいる想定
            D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\BSATKWARNING_UI\\warning_R_upperDASH.png", &pTexture[nCntBG]);
            break;
        case 3: //通常攻撃時、上段左側にボスがいる想定
            D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\BSATKWARNING_UI\\warning_L_upperDASH.png", &pTexture[nCntBG]);
            break;
        case 4://空中竜巻投擲攻撃（左側にいる想定）
            D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\BSATKWARNING_UI\\warning_R_AIRATK.png", &pTexture[nCntBG]);
            break;
        case 5: //空中竜巻投擲攻撃（右側にいる想定）
            D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\BSATKWARNING_UI\\warning_L_AIRATK.png", &pTexture[nCntBG]);
            break;
        }

        if (!pTexture[nCntBG])
        {
            pBossATKUI->Uninit();
            delete pBossATKUI;
            return nullptr;
        }
    }

    pBossATKUI->BindTexture(pTexture);

    return pBossATKUI;
}

//============================
//UIを表示させる処理
//============================
bool CBossATKUI::DisplayBossATKUI(int nDisplayID, int DisplayOption)
{
    //表示する場合
    if (DisplayOption == CBossATKUI::UIDISPLAY::UI_DISPLAY)
    {
        m_bUse[nDisplayID] = true;
    }

    //非表示にする場合
    if (DisplayOption == CBossATKUI::UIDISPLAY::UI_HIDDEN)
    {
        m_bUse[nDisplayID] = false;
    }

    //クリア時の音声
    if (m_bUse[CBossATKUI::BSUI_R_LOWERDASHATK] == true)
    {
        CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CLEAR);
    }

    return m_bUse[nDisplayID];
}
