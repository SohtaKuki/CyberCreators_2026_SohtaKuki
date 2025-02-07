//=================================================
//
// アイテムの処理 (score.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "score.h"
#include "manager.h"
#include "object2D.h"
#include "player.h"
#include "explosion.h"
#include "enemy.h"
#include "block.h"
#include "3dboss.h"
#include "bosscallui.h"
#include "3dgoalobj.h"
#include "timer.h"

int CScore::m_nScore = 0;
int CScore::m_nHPBonusScore = 0;
int CScore::m_nTimeBonusScore = 0;

//============================
//コンストラクタ
//============================
CScore::CScore(int nPriority) : CObject2D(nPriority)
{
	m_nScoreCnt = 0;
	bUpdateTime = false;
	m_bUse[NUM_SCORE] = false;
}

//============================
//デストラクタ
//============================
CScore::~CScore()
{

}

//============================
//スコアの初期化処理
//============================
HRESULT CScore::Init()
{
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

#if _DEBUG
	m_bUse[NUM_SCORE] = true;
#endif

	VERTEX_2D* pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < NUM_SCORE; nCntTime++)
	{
		//頂点座標を設定
		pVtx[0].pos = D3DXVECTOR3(m_nPos.x + (nCntTime * TEX_SCORE_INTERVAL), m_nPos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_nPos.x + m_nSize.x + (nCntTime * TEX_SCORE_INTERVAL), m_nPos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_nPos.x + (nCntTime * TEX_SCORE_INTERVAL), (m_nPos.y + m_nSize.y), 0.0f);
		pVtx[3].pos = D3DXVECTOR3((m_nPos.x + m_nSize.x + (nCntTime * TEX_SCORE_INTERVAL)), (m_nPos.y + m_nSize.y), 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255,255,255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255,255,255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255,255,255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255,255,255, 255);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//============================
//スコアの終了処理
//============================
void CScore::Uninit()
{

	AddScore(C3dplayer::GetPlayerLife() * 100); //プレイヤーHPボーナスのスコア加算

	m_nHPBonusScore += C3dplayer::GetPlayerLife() * 100; //プレイヤーHPボーナスのスコア加算差分もここで保存しとく

	if (C3dgoalobj::GetStageNum() == 1 && CTimer::GetTimer() <= 40)
	{
		AddScore(14000); //タイムボーナスのスコア加算
		m_nTimeBonusScore += 14000;
	}

	if (C3dgoalobj::GetStageNum() == 1 && CTimer::GetTimer() >= 40)
	{
		AddScore(5000); //タイムボーナスのスコア加算
		m_nTimeBonusScore += 5000;
	}

	if (C3dgoalobj::GetStageNum() == 2 && CTimer::GetTimer() <= 105)
	{
		AddScore(18000); //タイムボーナスのスコア加算
		m_nTimeBonusScore += 14000;
	}

	if (C3dgoalobj::GetStageNum() == 2 && CTimer::GetTimer() >= 105)
	{
		AddScore(6000); //タイムボーナスのスコア加算
		m_nTimeBonusScore += 5000;
	}

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

//============================
//スコアの更新処理
//============================
void CScore::Update()
{
	VERTEX_2D* pVtx;

	int aPosTexU[NUM_SCORE];

	// 時間の値をコピー
	int CopyTime = m_nScore;

	// 各桁の値を計算
	for (int nCntTime = NUM_SCORE - 1; nCntTime >= 0; nCntTime--)
	{
		aPosTexU[nCntTime] = CopyTime % 10;
		CopyTime /= 10;
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の更新
	for (int nCntTime = 0; nCntTime < NUM_SCORE; nCntTime++)
	{
		pVtx[0].tex = D3DXVECTOR2(aPosTexU[nCntTime] / 10.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((aPosTexU[nCntTime] + 1) / 10.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aPosTexU[nCntTime] / 10.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((aPosTexU[nCntTime] + 1) / 10.0f, 1.0f);

		pVtx += 4;
	}




	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	int nFadeState = CFade::GetFadeState();

	if (nFadeState == CFade::FADE_OUT)
	{
		Uninit();
	}

}

//============================
//スコアの描画処理
//============================
void CScore::Draw()
{
	CRenderer* Renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, m_pTexBuff);

	for (int nCntTime = 0; nCntTime < NUM_SCORE; nCntTime++)
	{
		if (m_bUse[NUM_SCORE] == true) 
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);
		}
	}
}

//============================
//スコアの生成処理
//============================
CScore* CScore::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CScore* pScore;

	pScore = new CScore;

	pScore->SetType(TYPE::SCORE);
	pScore->m_nPos = pos;
	pScore->m_nSize = size;

	if (C3dgoalobj::GetStageNum() == 0 || C3dgoalobj::GetStageNum() == 1)
	{
		m_nHPBonusScore = 0;
		m_nTimeBonusScore = 0;
		m_nScore = 0;
	}

	//アイテムの初期化
	pScore->Init();

	LPDIRECT3DTEXTURE9 pTexture;

	//テクスチャの読み込む
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\SCORE_NUMBER_type3.png", &pTexture);

	pScore->BindTexture(pTexture);

	return pScore;
}

void CScore::AddScore(int nTime)
{
	m_nScore += nTime;
}

//=======================
//テクスチャの設定
//=======================
void CScore::BindTexture(LPDIRECT3DTEXTURE9 pTex)
{
	m_pTexBuff = pTex;
}

