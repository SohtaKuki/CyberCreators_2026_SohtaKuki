//=================================================
//
// 床の描画処理 (floor.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "blizardfloor.h"

LPDIRECT3DTEXTURE9 CBlizardfloor::m_pTexBuff = nullptr;

//======================
// コンストラクタ
//======================
CBlizardfloor::CBlizardfloor(int nPriority) : CObject3D(nPriority)
{

}

//======================
// デストラクタ
//======================
CBlizardfloor::~CBlizardfloor()
{

}

//======================
// 初期化処理
//======================
HRESULT CBlizardfloor::Init()
{
	//初期化
	if (FAILED(CObject3D::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//======================
// 終了処理
//======================
void CBlizardfloor::Uninit()
{
	CObject3D::Uninit();
}

//======================
// 更新処理
//======================
void CBlizardfloor::Update()
{
	VERTEX_3D* pVtx;

	CObject3D::GetBuff()->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(-m_nSize.x,m_nPos.y, m_nSize.z);
	pVtx[1].pos = D3DXVECTOR3(m_nSize.x, m_nPos.y, m_nSize.z);
	pVtx[2].pos = D3DXVECTOR3(-m_nSize.x, m_nPos.y, -m_nSize.z);
	pVtx[3].pos = D3DXVECTOR3(m_nSize.x, m_nPos.y, -m_nSize.z);

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

	CObject3D::GetBuff()->Unlock();

	int nFadeState = CFade::GetFadeState();

	if (nFadeState == CFade::FADE_OUT)
	{
		CBlizardfloor::Uninit();
	}
}

//======================
// 描画処理
//======================
void CBlizardfloor::Draw()
{
	CObject3D::Draw();
}

//======================
// オブジェクト生成処理
//======================
CBlizardfloor* CBlizardfloor::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CBlizardfloor* Blizardfloor = nullptr;

	Blizardfloor = new CBlizardfloor;

	//初期化に成功した場合
	if (SUCCEEDED(Blizardfloor->Init()))
	{
		Blizardfloor->Load();

		Blizardfloor->m_nPos = pos;

		Blizardfloor->m_nSize = size;

		Blizardfloor->Init();

		////テクスチャの設定
		//Blizardfloor->BindTexture(m_pTexBuff);
	}

	return Blizardfloor;
}

//======================
// テクスチャロード処理
//======================
HRESULT CBlizardfloor::Load()
{
	LPDIRECT3DDEVICE9 pDevice = nullptr;
	pDevice = CManager::GetRenderer()->GetDevice();

	if (FAILED(D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\samplepos.png", &m_pTexBuff)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//======================
// テクスチャアンロード(終了)処理
//======================
void CBlizardfloor::Unload()
{
	if (m_pTexBuff != nullptr)
	{
		m_pTexBuff->Release();
		m_pTexBuff = nullptr;
	}
}
