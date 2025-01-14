//=================================================
//
// 3Dポリゴンの描画処理 (object3D.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "object3D.h"

LPDIRECT3DTEXTURE9 CObject3D::m_pTexBuff = nullptr;

//======================
// コンストラクタ
//======================
CObject3D::CObject3D(int nPriority) : CObject(nPriority)
{

}

//======================
// デストラクタ
//======================
CObject3D::~CObject3D()
{

}

//======================
// 初期化処理
//======================
HRESULT CObject3D::Init()
{
	CRenderer* Renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_OBJECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr)))
	{
		return E_FAIL;
	}


	VERTEX_3D* pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    pVtx[0].pos = D3DXVECTOR3(-m_nPos.x, 0.0f, m_nPos.z);
    pVtx[1].pos = D3DXVECTOR3(m_nPos.x, 0.0f, m_nPos.z);
    pVtx[2].pos = D3DXVECTOR3(-m_nPos.x, 0.0f, -m_nPos.z);
    pVtx[3].pos = D3DXVECTOR3(m_nPos.x, 0.0f, -m_nPos.z);

    // 法線ベクトルの設定
    pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

    // 頂点カラーの設定
    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

    // テクスチャ座標の設定
    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pVtxBuff->Unlock();

	return S_OK;
}

//======================
// 終了処理
//======================
void CObject3D::Uninit()
{
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	//CObject3D::Unload();
	CObject::Release();

}

//=======================
//テクスチャの設定
//=======================
void CObject3D::BindTexture(LPDIRECT3DTEXTURE9 pTex)
{
	m_pTexBuff = pTex;
}

//======================
// 更新処理
//======================
void CObject3D::Update()
{
	int nFadeState = CFade::GetFadeState();

	if (nFadeState == CFade::FADE_OUT)
	{
		if (m_pTexBuff != nullptr)
		{
			m_pTexBuff->Release();
			m_pTexBuff = nullptr;
		}
	}
}

//======================
// 描画処理
//======================
void CObject3D::Draw()
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
CObject3D* CObject3D::Create(D3DXVECTOR3 pos)
{
	CObject3D* Object3D = new CObject3D;

	//初期化に成功した場合
	if (SUCCEEDED(Object3D->Init()))
	{
		Object3D->Load();

		Object3D->m_nPos = pos;
	}

	return Object3D;
}

//======================
// テクスチャロード処理
//======================
HRESULT CObject3D::Load()
{
	LPDIRECT3DDEVICE9 pDevice = nullptr;
	pDevice = CManager::GetRenderer()->GetDevice();

	if (FAILED(D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\samplepos.png", &m_pTexBuff)))
	//if (FAILED(D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\icetexture000.png", &m_pTexBuff)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//======================
// テクスチャアンロード(終了)処理
//======================
void CObject3D::Unload()
{
	if (m_pTexBuff != nullptr)
	{
		m_pTexBuff->Release();
		m_pTexBuff = nullptr;
	}
}


//============================
//頂点バッファの取得
//============================
LPDIRECT3DVERTEXBUFFER9 CObject3D::GetBuff(void)
{
	return m_pVtxBuff;
}