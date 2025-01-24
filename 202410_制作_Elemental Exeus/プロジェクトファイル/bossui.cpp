//=================================================
//
// 2Dポリゴンの描画処理 (object2D.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "bossui.h"
#include "manager.h"
#include "3dboss.h"

//======================
// コンストラクタ
//======================
CBossui::CBossui(int nPriority) : CObject2D(nPriority)
{

}

//======================
// デストラクタ
//======================
CBossui::~CBossui()
{

}

//======================
// 初期化処理
//======================
HRESULT CBossui::Init()
{
	CObject2D::Init();

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	CObject2D::GetBuff()->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_nPos.x - m_nSize.x, m_nPos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_nPos.x + m_nSize.x, m_nPos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_nPos.x - m_nSize.x, m_nPos.y + m_nSize.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_nPos.x + m_nSize.x, m_nPos.y + m_nSize.y, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;

	CObject2D::GetBuff()->Unlock();

	return S_OK;
}

//======================
// 終了処理
//======================
void CBossui::Uninit()
{
	CObject2D::Uninit();
}

//======================
// 更新処理
//======================
void CBossui::Update()
{   
	//ゲームが進行可能の時のみ通す
	if (CScene::GetUpdateStat() == true)
	{
		VERTEX_2D* pVtx;


		//頂点バッファをロックし、頂点情報へのポインタを取得
		CObject2D::GetBuff()->Lock(0, 0, (void**)&pVtx, 0);

		if (C3dboss::GetBossEntry() == true)
		{
			pVtx[0].pos = D3DXVECTOR3(m_nPos.x - m_nSize.x, m_nPos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(m_nPos.x + m_nSize.x, m_nPos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(m_nPos.x - m_nSize.x, m_nPos.y + m_nSize.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(m_nPos.x + m_nSize.x, m_nPos.y + m_nSize.y, 0.0f);
		}

		if (C3dboss::GetBossEntry() == false)
		{
			for (int nCnt = 0; nCnt < 4; nCnt++)
			{
				pVtx[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}

		CObject2D::GetBuff()->Unlock();

	}

	int nFadeState = CFade::GetFadeState();

	if (nFadeState == CFade::FADE_OUT)
	{
		CBossui::Uninit();
		return;
	}
}

//======================
// 描画処理
//======================
void CBossui::Draw()
{
	CObject2D::Draw();
}

//======================
// オブジェクト生成処理
//======================
CBossui* CBossui::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CBossui* Bossui = new CBossui;

	Bossui->m_nPos = pos;

	Bossui->m_nSize = size;

	Bossui->Init();

	LPDIRECT3DTEXTURE9 pTexture;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\bossui000.png", &pTexture);

	Bossui->BindTexture(pTexture);

	return Bossui;
}