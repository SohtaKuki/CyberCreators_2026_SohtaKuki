//=================================================
//
// 床の描画処理 (floorBG.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "floorBG.h"

//======================
// コンストラクタ
//======================
CFloorBG::CFloorBG(int nPriority) : CModel(nPriority), m_pTexBuff(nullptr)
{

}

//======================
// デストラクタ
//======================
CFloorBG::~CFloorBG()
{
    CFloorBG::Uninit();
}

//======================
// 初期化処理
//======================
HRESULT CFloorBG::Init()
{
    if (FAILED(CObject3D::Init()))
    {
        return E_FAIL;
    }

    return S_OK;
}

//======================
// 終了処理
//======================
void CFloorBG::Uninit()
{
    if (m_pTexBuff != nullptr)
    {
        m_pTexBuff->Release();
        m_pTexBuff = nullptr;
    }

    CObject3D::Uninit();
}

//======================
// 更新処理
//======================
void CFloorBG::Update()
{

}

//======================
// 描画処理
//======================
void CFloorBG::Draw()
{
    CRenderer* Renderer = CManager::GetRenderer();
    LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

    D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス

    //ワールドマトリックスの初期化
    D3DXMatrixIdentity(&m_mtxWorld);

    //向きを反映
    D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

    D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

    //位置を反映
    D3DXMatrixTranslation(&mtxTrans, m_nPos.x, m_nPos.y, m_nPos.z);

    D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

    //ワールドマトリックスの設定
    pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

    pDevice->SetFVF(FVF_VERTEX_3D);
    pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
    pDevice->SetTexture(0, m_pTexBuff);
    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//======================
// オブジェクト生成処理
//======================
CFloorBG* CFloorBG::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nType)
{
    CFloorBG* pFloorBG = new CFloorBG;

    if (SUCCEEDED(pFloorBG->Init()))
    {
        pFloorBG->m_nType = nType;
        pFloorBG->m_nPos = pos;
        pFloorBG->m_nSize = size;

        // テクスチャのロード
        if (FAILED(pFloorBG->Load()))
        {
            pFloorBG->Uninit();
            delete pFloorBG;
            return nullptr;
        }
    }

    return pFloorBG;
}

//======================
// テクスチャロード処理
//======================
HRESULT CFloorBG::Load()
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    if (m_nType == 0)
    {
        if (FAILED(D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\samplepos.png", &m_pTexBuff)))
        {
            return E_FAIL;
        }
    }
    else if (m_nType == 1)
    {
        if (FAILED(D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\block003.png", &m_pTexBuff)))
        {
            return E_FAIL;
        }
    }

    return S_OK;
}
