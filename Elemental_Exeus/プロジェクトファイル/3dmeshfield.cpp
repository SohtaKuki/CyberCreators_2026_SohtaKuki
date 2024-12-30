//=================================================
//
// 3Dメッシュフィールド処理 (3dmeshfield.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "3dmeshfield.h"


LPDIRECT3DTEXTURE9 CMeshField::m_pTextureMeshfield = nullptr;

//======================
// コンストラクタ
//======================
CMeshField::CMeshField(int nPriority) : CObject3D(nPriority)
{
    m_fRadius = 8000.0f;
    m_fTexMoveSPD = 0.0f;
}

//======================
// デストラクタ
//======================
CMeshField::~CMeshField()
{
    CMeshField::Uninit();
}

//=============================
// メッシュフィールドの初期化処理
//=============================
HRESULT CMeshField::Init(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
    if (FAILED(D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\nightsky000.png", &m_pTextureMeshfield)))
    {
        return E_FAIL;
    }

    const int numVertices = sphereDiv * (sphereDiv / 2 + 1);
    const int numIndices = (sphereDiv * (sphereDiv / 2)) * 12;

    // 頂点バッファの作成
    if (FAILED(pDevice->CreateVertexBuffer(numVertices * sizeof(VERTEX_3D), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffMeshfield, NULL)))
    {
        return E_FAIL;
    }

    // 頂点データのロック
    VERTEX_3D* pVtx;
    m_pVtxBuffMeshfield->Lock(0, 0, (void**)&pVtx, 0);

    for (int lat = 0; lat <= sphereDiv / 2; ++lat) 
    {
        float theta = (float)lat / (sphereDiv / 2) * D3DX_PI;

        for (int lon = 0; lon < sphereDiv; ++lon) 
        {
            float phi = (float)lon / sphereDiv * (D3DX_PI * 2);

            // 頂点位置
            int idx = lat * sphereDiv + lon;
            pVtx[idx].pos = D3DXVECTOR3(m_fRadius * sinf(theta) * cosf(phi),(m_fRadius  * cosf(theta) - 3000.0f),m_fRadius * sinf(theta) * sinf(phi));

            // 法線
            D3DXVec3Normalize(&pVtx[idx].nor, &pVtx[idx].pos);

            // テクスチャ座標
            pVtx[idx].tex = D3DXVECTOR2(1.0f - (float)lon / sphereDiv, (float)lat / (sphereDiv / 2));


            // 頂点カラー
            pVtx[idx].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        }
    }

    // 頂点バッファのアンロック
    m_pVtxBuffMeshfield->Unlock();

    // インデックスバッファの作成
    if (FAILED(pDevice->CreateIndexBuffer(numIndices * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pldxBuffMeshField, NULL)))
    {
        return E_FAIL;
    }

    // インデックスデータのロック
    WORD* pIdx;
    m_pldxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

    // インデックス設定
    int index = 0;
    for (int lat = 0; lat < sphereDiv / 2; ++lat) 
    {
        for (int lon = 0; lon < sphereDiv; ++lon) 
        {
            int current = lat * sphereDiv + lon;
            int next = current + sphereDiv;

            pIdx[index++] = current;
            pIdx[index++] = next;
            pIdx[index++] = current + 1;

            pIdx[index++] = current + 1;
            pIdx[index++] = next;
            pIdx[index++] = next + 1;
        }
    }

    // インデックスバッファのアンロック
    m_pldxBuffMeshField->Unlock();

    return S_OK;
}


//===========================
// メッシュフィールドの終了処理
//===========================
void CMeshField::Uninit(void)
{

    if (m_pldxBuffMeshField != nullptr)
    {
        m_pldxBuffMeshField->Release();
        m_pldxBuffMeshField = nullptr;
    }


    // 頂点バッファの破棄
    if (m_pVtxBuffMeshfield != nullptr)
    {
        m_pVtxBuffMeshfield->Release();
        m_pVtxBuffMeshfield = nullptr;
    }

    // テクスチャの破棄
    if (m_pTextureMeshfield != nullptr)
    {
        m_pTextureMeshfield->Release();
        m_pTextureMeshfield = nullptr;
    }

}

//===========================
// メッシュフィールドの更新処理
//===========================
void CMeshField::Update(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // 頂点データのロック
    VERTEX_3D* pVtx;
    m_pVtxBuffMeshfield->Lock(0, 0, (void**)&pVtx, 0);

    m_fTexMoveSPD += 0.00015f;

    for (int lat = 0; lat <= sphereDiv / 2; ++lat)
    {
        float theta = (float)lat / (sphereDiv / 2) * D3DX_PI;

        for (int lon = 0; lon < sphereDiv; ++lon)
        {
            float phi = (float)lon / sphereDiv * (D3DX_PI * 2);

            // 頂点位置
            int idx = lat * sphereDiv + lon;
            pVtx[idx].pos = D3DXVECTOR3(m_fRadius * sinf(theta) * cosf(phi), (m_fRadius * cosf(theta) - 3000.0f), m_fRadius * sinf(theta) * sinf(phi));

            // 法線
            D3DXVec3Normalize(&pVtx[idx].nor, &pVtx[idx].pos);

            // テクスチャ座標
            pVtx[idx].tex = D3DXVECTOR2((1.0f - (float)lon / sphereDiv) + m_fTexMoveSPD, (float)lat / (sphereDiv / 2));


            // 頂点カラー
            pVtx[idx].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        }
    }

    // 頂点バッファのアンロック
    m_pVtxBuffMeshfield->Unlock();

}

//===========================
// メッシュフィールドの描画処理
//===========================
void CMeshField::Draw(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    D3DXMATRIX mtxRot, mtxTrans;
    D3DXMatrixIdentity(&m_mtxWorldMeshfield);

    D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Meshfield.rot.y, m_Meshfield.rot.x, m_Meshfield.rot.z);
    D3DXMatrixMultiply(&m_mtxWorldMeshfield, &m_mtxWorldMeshfield, &mtxRot);

    D3DXMatrixTranslation(&mtxTrans, m_nPos.x, m_nPos.y, m_nPos.z);
    D3DXMatrixMultiply(&m_mtxWorldMeshfield, &m_mtxWorldMeshfield, &mtxTrans);

    pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorldMeshfield);
    pDevice->SetFVF(FVF_VERTEX_3D);
    pDevice->SetStreamSource(0, m_pVtxBuffMeshfield, 0, sizeof(VERTEX_3D));
    pDevice->SetIndices(m_pldxBuffMeshField);
    pDevice->SetTexture(0, m_pTextureMeshfield);

    pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, sphereDiv * (sphereDiv / 2 + 1), 0, (sphereDiv * (sphereDiv / 2)) * 2);
}
CMeshField::Meshfield CMeshField::GetMeshfield(void)
{
    return m_Meshfield;
}

CMeshField* CMeshField::Create(D3DXVECTOR3 pos)
{
    CMeshField* MeshField = new CMeshField;

    //初期化に成功した場合
    if (SUCCEEDED(MeshField->Init()))
    {
        MeshField->m_nPos = pos;

    }

    return MeshField;
}
