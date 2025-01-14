//=================================================
//
// 2Dポリゴンの描画処理 (object2D.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "scorewindow.h"
#include "manager.h"
#include "3dboss.h"
#include "bosscallui.h"
#include "3dgoalobj.h"

//======================
// コンストラクタ
//======================
CScoreWindow::CScoreWindow(int nPriority) : CObject2D(nPriority)
{

}

//======================
// デストラクタ
//======================
CScoreWindow::~CScoreWindow()
{

}

//======================
// 初期化処理
//======================
HRESULT CScoreWindow::Init()
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
void CScoreWindow::Uninit()
{
	CObject2D::Uninit();
}

//======================
// 更新処理
//======================
void CScoreWindow::Update()
{
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	CObject2D::GetBuff()->Lock(0, 0, (void**)&pVtx, 0);

	if (CBossCallUI::GetEndStat() == true && C3dboss::GetBossEntry() == true && C3dgoalobj::GetStageNum() == 2 ||
		CBossCallUI::GetEndStat() == false && C3dboss::GetBossEntry() == false && C3dgoalobj::GetStageNum() == 2
		|| C3dgoalobj::GetStageNum() == 0 || C3dgoalobj::GetStageNum() == 1)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(m_nPos.x - m_nSize.x, m_nPos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_nPos.x + m_nSize.x, m_nPos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_nPos.x - m_nSize.x, m_nPos.y + m_nSize.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_nPos.x + m_nSize.x, m_nPos.y + m_nSize.y, 0.0f);
	}

	if (CBossCallUI::GetEndStat() == true && C3dboss::GetBossEntry() == false && C3dgoalobj::GetStageNum() == 2)
	{
		//頂点座標の更新
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			pVtx[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}

	CObject2D::GetBuff()->Unlock();

	int nFadeState = CFade::GetFadeState();

	if (nFadeState == CFade::FADE_OUT)
	{
		CScoreWindow::Uninit();
	}
}

//======================
// 描画処理
//======================
void CScoreWindow::Draw()
{
	CObject2D::Draw();
}

//======================
// オブジェクト生成処理
//======================
CScoreWindow* CScoreWindow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CScoreWindow* ScoreWindow = new CScoreWindow;

	ScoreWindow->m_nPos = pos;

	ScoreWindow->m_nSize = size;

	ScoreWindow->Init();

	LPDIRECT3DTEXTURE9 pTexture;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\score_window.png", &pTexture);

	ScoreWindow->BindTexture(pTexture);

	return ScoreWindow;
}