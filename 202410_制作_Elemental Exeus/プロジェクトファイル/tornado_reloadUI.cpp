//=================================================
//
// タイトル画面背景の処理 (titlebg.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "tornado_reloadUI.h"

bool CTornadoReloadUI::m_bUse[NUM_ICON] = {};

//============================
//コンストラクタ
//============================
CTornadoReloadUI::CTornadoReloadUI(int nPriority) : CObject2D(nPriority)
{
    for (int nCntBG = 0; nCntBG < NUM_ICON; nCntBG++)
    {
        m_bUse[nCntBG] = false;
        m_pTexBuff[nCntBG] = nullptr;
    }

    m_pVtxBuff = nullptr;
    m_nAlphaCnt = 255;
    m_bAlphaSwitch = false;
}

//============================
//デストラクタ
//============================
CTornadoReloadUI::~CTornadoReloadUI()
{

}

//=====================
// 背景初期化処理
//======================
HRESULT CTornadoReloadUI::Init()
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
        switch (nCntBG)
        {
        case 0:
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
            break;
        case 1:
            //頂点座標の設定
            pVtx[0].pos = D3DXVECTOR3((m_nPos[nCntBG].x - m_nSize[nCntBG].x) + 114.0f, m_nPos[nCntBG].y  - 5.0f, 0.0f);
            pVtx[1].pos = D3DXVECTOR3((m_nPos[nCntBG].x + m_nSize[nCntBG].x) + 114.0f, m_nPos[nCntBG].y  - 5.0f, 0.0f);
            pVtx[2].pos = D3DXVECTOR3((m_nPos[nCntBG].x - m_nSize[nCntBG].x) + 114.0f, m_nPos[nCntBG].y  - 5.0f + m_nSize[nCntBG].y - 25.0f, 0.0f);
            pVtx[3].pos = D3DXVECTOR3((m_nPos[nCntBG].x + m_nSize[nCntBG].x) + 114.0f, m_nPos[nCntBG].y  - 5.0f + m_nSize[nCntBG].y - 25.0f, 0.0f);

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
            break;
        }

    }

    DisplayTornadoReloadUI(1, CTornadoReloadUI::UIDISPLAY::UI_DISPLAY);

    //頂点バッファアンロック
    m_pVtxBuff->Unlock();

    return S_OK;
}

//=================================
// 背景終了処理
//=================================
void CTornadoReloadUI::Uninit()
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
void CTornadoReloadUI::Update()
{
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

    m_nRot[0].z += 0.1f;

    SetAlpha(m_nAlphaCnt);


    //フェードの状態を取得
    int nFadeState = CFade::GetFadeState();

    //フェードアウトになった場合、終了処理に移行
    if (nFadeState == CFade::FADE_OUT)
    {
        CTornadoReloadUI::Uninit();
    }

}

void CTornadoReloadUI::SetAlpha(int nAlpha)
{
    int nCntBG;
    VERTEX_2D* pVtx; //頂点情報へのポインタ

    //頂点バッファロック
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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

        float fLength = sqrtf(m_nSize[0].x * m_nSize[0].x  + m_nSize[0].y * m_nSize[0].y ) / 1.5f;

        pVtx[0].pos.x = m_nPos[0].x + sinf(m_nRot[0].z - D3DX_PI * 0.75f) * fLength;
        pVtx[0].pos.y = m_nPos[0].y + cosf(m_nRot[0].z - D3DX_PI * 0.75f) * fLength;
        pVtx[0].pos.z = 0.0f;

        pVtx[1].pos.x = m_nPos[0].x + sinf(m_nRot[0].z + D3DX_PI * 0.75f) * fLength;
        pVtx[1].pos.y = m_nPos[0].y + cosf(m_nRot[0].z + D3DX_PI * 0.75f) * fLength;
        pVtx[1].pos.z = 0.0f;

        pVtx[2].pos.x = m_nPos[0].x + sinf(m_nRot[0].z - D3DX_PI * 0.25f)* fLength;
        pVtx[2].pos.y = m_nPos[0].y + cosf(m_nRot[0].z - D3DX_PI * 0.25f) * fLength;
        pVtx[2].pos.z = 0.0f;

        pVtx[3].pos.x = m_nPos[0].x + sinf(m_nRot[0].z + D3DX_PI * 0.25f) * fLength;
        pVtx[3].pos.y = m_nPos[0].y + cosf(m_nRot[0].z + D3DX_PI * 0.25f) * fLength;
        pVtx[3].pos.z = 0.0f;

        pVtx[0].col = D3DCOLOR_RGBA(m_nAlphaCnt, m_nAlphaCnt, m_nAlphaCnt, m_nAlphaCnt);
        pVtx[1].col = D3DCOLOR_RGBA(m_nAlphaCnt, m_nAlphaCnt, m_nAlphaCnt, m_nAlphaCnt);
        pVtx[2].col = D3DCOLOR_RGBA(m_nAlphaCnt, m_nAlphaCnt, m_nAlphaCnt, m_nAlphaCnt);
        pVtx[3].col = D3DCOLOR_RGBA(m_nAlphaCnt, m_nAlphaCnt, m_nAlphaCnt, m_nAlphaCnt);



        pVtx += 4;


    //頂点バッファをアンロックする
    m_pVtxBuff->Unlock();
}

//=========================
// 背景描画処理
//=========================
void CTornadoReloadUI::Draw()
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

void CTornadoReloadUI::BindTexture(LPDIRECT3DTEXTURE9 pTex[NUM_ICON])
{
    for (int nCnt = 0; nCnt < NUM_ICON; nCnt++)
    {
        m_pTexBuff[nCnt] = pTex[nCnt];
    }
}

//============================
//バフ付与UIの生成処理
//============================
CTornadoReloadUI* CTornadoReloadUI::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    CTornadoReloadUI* pTornadoReloadUI;

    pTornadoReloadUI = new CTornadoReloadUI;

    for (int nCntBG = 0; nCntBG < NUM_ICON; nCntBG++)
    {
        pTornadoReloadUI->m_nPos[nCntBG] = pos;
        pTornadoReloadUI->m_nSize[nCntBG] = size;
    }

    if (FAILED(pTornadoReloadUI->Init()))
    {
        delete pTornadoReloadUI;
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
            D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\TornadoReloadUI_GRN.png", &pTexture[nCntBG]);
            break;
        case 1:
            D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\tornado_midLine.png", &pTexture[nCntBG]);
            break;
        }

        if (!pTexture[nCntBG])
        {
            pTornadoReloadUI->Uninit();
            delete pTornadoReloadUI;
            return nullptr;
        }
    }

    pTornadoReloadUI->BindTexture(pTexture);

    return pTornadoReloadUI;
}

//============================
//UIを表示させる処理
//============================
bool CTornadoReloadUI::DisplayTornadoReloadUI(int nDisplayID, int DisplayOption)
{
    //表示する場合
    if (DisplayOption == CTornadoReloadUI::UIDISPLAY::UI_DISPLAY)
    {
        m_bUse[nDisplayID] = true;
    }

    //非表示にする場合
    if (DisplayOption == CTornadoReloadUI::UIDISPLAY::UI_HIDDEN)
    {
        m_bUse[nDisplayID] = false;
    }

    return m_bUse[nDisplayID];
}
