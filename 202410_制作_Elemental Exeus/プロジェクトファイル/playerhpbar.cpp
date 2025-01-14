//=================================================
//
// 2Dポリゴンの描画処理 (object2D.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "playerhpbar.h"
#include "manager.h"
#include "3dplayer.h"
#include "3dboss.h"
#include "3dtornado.h"
#include "bosscallui.h"
#include "3dgoalobj.h"

//======================
// コンストラクタ
//======================
CPlayerHpBar::CPlayerHpBar(int nPriority) : CObject2D(nPriority)
{

}

//======================
// デストラクタ
//======================
CPlayerHpBar::~CPlayerHpBar()
{

}

//======================
// 初期化処理
//======================
HRESULT CPlayerHpBar::Init()
{
	CObject2D::Init();

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	CObject2D::GetBuff()->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_nSize.x, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, m_nSize.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_nSize.x, m_nSize.y, 0.0f);

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
void CPlayerHpBar::Uninit()
{
	CObject2D::Uninit();
}

//======================
// 更新処理
//======================
void CPlayerHpBar::Update()
{
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	CObject2D::GetBuff()->Lock(0, 0, (void**)&pVtx, 0);

	if (m_nBarType == 0)
	{
		if (CBossCallUI::GetEndStat() == true && C3dboss::GetBossEntry() == true && C3dgoalobj::GetStageNum() == 2 ||
			CBossCallUI::GetEndStat() == false && C3dboss::GetBossEntry() == false && C3dgoalobj::GetStageNum() == 2
			|| C3dgoalobj::GetStageNum() == 0 || C3dgoalobj::GetStageNum() == 1)
		{
			//頂点座標の更新
			pVtx[0].pos.x = CObject2D::GetPos().x - 50 * 0.5f;
			pVtx[0].pos.y = CObject2D::GetPos().y - 50 * 0.5f;

			pVtx[1].pos.x = (m_nSize.x + CObject2D::GetPos().x) + 50 * 0.4f * (C3dplayer::GetPlayerLife() * 0.4);
			pVtx[1].pos.y = CObject2D::GetPos().y - 50 * 0.5f;

			pVtx[2].pos.x = CObject2D::GetPos().x - 50 * 0.5f;
			pVtx[2].pos.y = (m_nSize.y + CObject2D::GetPos().y) + 50 * 0.5f;

			pVtx[3].pos.x = (m_nSize.x + CObject2D::GetPos().x) + 50 * 0.4f * (C3dplayer::GetPlayerLife() * 0.4);
			pVtx[3].pos.y = (m_nSize.y + CObject2D::GetPos().y) + 50 * 0.5f;
		}

		if (CBossCallUI::GetEndStat() == true && C3dboss::GetBossEntry() == false && C3dgoalobj::GetStageNum() == 2)
		{
			//頂点座標の更新
			for (int nCnt = 0; nCnt < 4; nCnt++)
			{
				pVtx[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}
	}

	if (m_nBarType == 1)
	{
		if (C3dboss::GetBossEntry() == true)
		{
			//頂点座標の更新
			pVtx[0].pos.x = CObject2D::GetPos().x - 50 * 0.5f;
			pVtx[0].pos.y = CObject2D::GetPos().y - 50 * 0.5f;

			pVtx[1].pos.x = (m_nSize.x + CObject2D::GetPos().x) + 50 * 0.158f * (C3dboss::GetBossLife() * 0.158f);
			pVtx[1].pos.y = CObject2D::GetPos().y - 50 * 0.5f;

			pVtx[2].pos.x = CObject2D::GetPos().x - 50 * 0.5f;
			pVtx[2].pos.y = (m_nSize.y + CObject2D::GetPos().y) + 50 * 0.5f;

			pVtx[3].pos.x = (m_nSize.x + CObject2D::GetPos().x) + 50 * 0.158f * (C3dboss::GetBossLife() * 0.158f);
			pVtx[3].pos.y = (m_nSize.y + CObject2D::GetPos().y) + 50 * 0.5f;
		}

		if (C3dboss::GetBossEntry() == false)
		{
			//頂点座標の更新
			for (int nCnt = 0; nCnt < 4; nCnt++)
			{
				pVtx[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f,0.0f);
			}
		}
	}

	if (m_nBarType == 2)
	{
		if (CBossCallUI::GetEndStat() == true && C3dboss::GetBossEntry() == true && C3dgoalobj::GetStageNum() == 2 ||
			CBossCallUI::GetEndStat() == false && C3dboss::GetBossEntry() == false && C3dgoalobj::GetStageNum() == 2
			|| C3dgoalobj::GetStageNum() == 0 || C3dgoalobj::GetStageNum() == 1)
		{
			//頂点座標の更新
			pVtx[0].pos.x = CObject2D::GetPos().x - 50 * 0.5f;
			pVtx[0].pos.y = CObject2D::GetPos().y - 50 * 0.5f;

			pVtx[1].pos.x = (m_nSize.x + CObject2D::GetPos().x) + 50 * 0.4f * (C3dtornado::GetNumTornado() * 5.5);
			pVtx[1].pos.y = CObject2D::GetPos().y - 50 * 0.5f;

			pVtx[2].pos.x = CObject2D::GetPos().x - 50 * 0.5f;
			pVtx[2].pos.y = (m_nSize.y + CObject2D::GetPos().y) + 50 * 0.5f;

			pVtx[3].pos.x = (m_nSize.x + CObject2D::GetPos().x) + 50 * 0.4f * (C3dtornado::GetNumTornado() * 5.5);
			pVtx[3].pos.y = (m_nSize.y + CObject2D::GetPos().y) + 50 * 0.5f;
		}

		if (CBossCallUI::GetEndStat() == true && C3dboss::GetBossEntry() == false && C3dgoalobj::GetStageNum() == 2)
		{
			//頂点座標の更新
			for (int nCnt = 0; nCnt < 4; nCnt++)
			{
				pVtx[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}
	}


	//頂点バッファをアンロックする
	CObject2D::GetBuff()->Unlock();

	int nFadeState = CFade::GetFadeState();

	
	if (nFadeState == CFade::FADE_OUT)
	{
		CPlayerHpBar::Uninit();
	}
}

//======================
// 描画処理
//======================
void CPlayerHpBar::Draw()
{
	CObject2D::Draw();
}

//======================
// オブジェクト生成処理
//======================
CPlayerHpBar* CPlayerHpBar::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size ,int nBarType)
{
	CPlayerHpBar* PlayerHpBar = new CPlayerHpBar;

	PlayerHpBar->CObject2D::SetPos(pos);

	PlayerHpBar->m_nSize = size;

	PlayerHpBar->m_nBarType = nBarType;

	PlayerHpBar->Init();

	if (nBarType == 0)
	{
		LPDIRECT3DTEXTURE9 pTexture;

		//テクスチャの読み込み
		D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\hpbar.png", &pTexture);

		PlayerHpBar->BindTexture(pTexture);
	}

	if (nBarType == 1)
	{
		LPDIRECT3DTEXTURE9 pTexture;

		//テクスチャの読み込み
		D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\boss_hpbar.png", &pTexture);

		PlayerHpBar->BindTexture(pTexture);
	}

	if (nBarType == 2)
	{
		LPDIRECT3DTEXTURE9 pTexture;

		//テクスチャの読み込み
		D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\tornado_Gauge.png", &pTexture);

		PlayerHpBar->BindTexture(pTexture);
	}

	return PlayerHpBar;
}