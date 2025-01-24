//=================================================
//
// チャージショット充填完了UI表示処理 (3dchargeshotui.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "ItemdropUI.h"
#include "manager.h"

bool CItemDropUI::m_bUse = false;

//======================
// コンストラクタ
//======================
CItemDropUI::CItemDropUI(int nPriority) : CObject2D(nPriority)
{
	m_bUse = false;
	m_nDisplaytime = 0;
	m_nAlpha = 0;
}

//======================
// デストラクタ
//======================
CItemDropUI::~CItemDropUI()
{

}

//======================
// 初期化処理
//======================
HRESULT CItemDropUI::Init()
{
	m_nAlpha = 0;

	CRenderer* Renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_OBJECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr)))
	{
		return E_FAIL;
	}

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点情報の設定
	pVtx[0].pos = D3DXVECTOR3(m_nPos.x - m_nSize.x, m_nPos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_nPos.x + m_nSize.x, m_nPos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_nPos.x - m_nSize.x, m_nPos.y + m_nSize.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_nPos.x + m_nSize.x, m_nPos.y + m_nSize.y, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;

	m_pVtxBuff->Unlock();

	return S_OK;
}

//======================
// 終了処理
//======================
void CItemDropUI::Uninit()
{
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	if (m_pTexBuff != nullptr)
	{
		m_pTexBuff->Release();
		m_pTexBuff = nullptr;
	}


	CObject::Release();
}

//======================
// 更新処理
//======================
void CItemDropUI::Update()
{
	//テクスチャ表示済みだったら
	if (m_bUse == true)
	{
		m_nDisplaytime++;
		m_nAlpha += 5;

		if (m_nPos.y <= -5.0f && m_nDisplaytime <= 120)
		{
			m_nPos.y += 15.0f;
		}

		if (m_nPos.y >= -5.0f && m_nDisplaytime <= 120)
		{
			m_nPos.y = -5.0f;
		}

		//既定の時間になったら非表示＆リセット
		if (m_nDisplaytime >= 120)
		{
			m_nPos.y -= 8.0f;

			if (m_nPos.y <= -55.0f)
			{
				m_nDisplaytime = 0;
				m_bUse = false;
			}
		}
	}

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点情報の設定
	pVtx[0].pos = D3DXVECTOR3(m_nPos.x - m_nSize.x, m_nPos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_nPos.x + m_nSize.x, m_nPos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_nPos.x - m_nSize.x, m_nPos.y + m_nSize.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_nPos.x + m_nSize.x, m_nPos.y + m_nSize.y, 0.0f);

	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, m_nAlpha);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, m_nAlpha);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, m_nAlpha);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, m_nAlpha);

	pVtx += 4;

	m_pVtxBuff->Unlock();

	//フェードの状態を取得
	int nFadeState = CFade::GetFadeState();

	//フェードアウトしたら強制削除
	if (nFadeState == CFade::FADE_OUT)
	{
		CItemDropUI::Uninit();
	}
}

//======================
// 描画処理
//======================
void CItemDropUI::Draw()
{
	CRenderer* Renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);


	if (m_bUse == true)
	{
		pDevice->SetTexture(0, m_pTexBuff);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//======================
// オブジェクト生成処理
//======================
CItemDropUI* CItemDropUI::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CItemDropUI* ItemDropUI = new CItemDropUI;

	ItemDropUI->m_nPos = pos;

	ItemDropUI->m_nSize = size;

	ItemDropUI->Init();

	LPDIRECT3DTEXTURE9 pTexture;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\addlife_itemdrop000.png", &pTexture);

	ItemDropUI->BindTexture(pTexture);

	return ItemDropUI;
}

//=======================
//テクスチャの設定
//=======================
void CItemDropUI::BindTexture(LPDIRECT3DTEXTURE9 pTex)
{
	m_pTexBuff = pTex;
}


//======================
//テクスチャを表示させる
//======================
bool CItemDropUI::DisplayAddLifeUI()
{
	m_bUse = true;

	return m_bUse;
}