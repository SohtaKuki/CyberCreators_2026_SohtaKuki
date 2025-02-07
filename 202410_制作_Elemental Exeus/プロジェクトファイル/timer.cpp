//=================================================
//
// タイマー処理 (timer.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "timer.h"
#include "manager.h"
#include "object2D.h"
#include "player.h"
#include "explosion.h"
#include "enemy.h"
#include "block.h"
#include "scene.h"
#include "endcallui.h"
#include "3dgoalobj.h"

int CTimer::m_nTime = {};
int CTimer::m_nDisplayTime = {};
int CTimer::m_nTimerCnt = {};

//============================
//コンストラクタ
//============================
CTimer::CTimer(int nPriority) : CObject2D(nPriority)
{

	bUpdateTime = false;
	for (int nCntTime = 0; nCntTime < 3; nCntTime++)
	{
		bUse[nCntTime] = true;
	}
}

//============================
//デストラクタ
//============================
CTimer::~CTimer()
{

}


//============================
//タイマーの初期化処理
//============================
HRESULT CTimer::Init()
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


	VERTEX_2D* pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntTime = 0; nCntTime < NUM_TIME; nCntTime++)
	{
		float offset = (nCntTime == 0) ? -10.0f : 0.0f; // 3桁目だけ左に10ピクセルずらす

		// 頂点座標を設定
		pVtx[0].pos = D3DXVECTOR3(m_nPos.x + (nCntTime * TEX_TIME_INTERVAL) + offset, m_nPos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_nPos.x + m_nSize.x + (nCntTime * TEX_TIME_INTERVAL) + offset, m_nPos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_nPos.x + (nCntTime * TEX_TIME_INTERVAL) + offset, m_nPos.y + m_nSize.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_nPos.x + m_nSize.x + (nCntTime * TEX_TIME_INTERVAL) + offset, m_nPos.y + m_nSize.y, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
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
//タイマーの初期化処理
//============================
void CTimer::Uninit()
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

//============================
//タイマーの更新処理
//============================
void CTimer::Update()
{
	if (CScene::GetUpdateStat() == true)
	{
		VERTEX_2D* pVtx;

		int aPosTexU[NUM_TIME];

		if (bUpdateTime == false)
		{
			m_nTimerCnt++;

			if (m_nTimerCnt == MAX_TIMESPEED)
			{
				m_nTime++; // 時間を1秒加算
				m_nTimerCnt = 0; // フレームカウントリセット

				// 分単位表示の処理を修正
				int minutes = m_nTime / 60; // 分を計算
				int seconds = m_nTime % 60; // 秒を計算
				m_nDisplayTime = minutes * 100 + seconds; // 分と秒を組み合わせた表示形式に変更
			}
		}

		if (CManager::GetKeyboard()->GetTrigger(DIK_F1))
		{
			if (bUpdateTime == false)
			{
				bUpdateTime = true;
			}
		}

		if (CManager::GetKeyboard()->GetTrigger(DIK_F2))
		{
			if (bUpdateTime == true)
			{
				bUpdateTime = false;
			}
		}

		// 時間の値をコピー
		int CopyTime = m_nDisplayTime;

		// 各桁の値を計算
		for (int nCntTime = NUM_TIME - 1; nCntTime >= 0; nCntTime--)
		{
			aPosTexU[nCntTime] = CopyTime % 10;
			CopyTime /= 10;
		}

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// テクスチャ座標の更新
		for (int nCntTime = 0; nCntTime < NUM_TIME; nCntTime++)
		{
			pVtx[0].tex = D3DXVECTOR2(aPosTexU[nCntTime] / 10.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((aPosTexU[nCntTime] + 1) / 10.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(aPosTexU[nCntTime] / 10.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((aPosTexU[nCntTime] + 1) / 10.0f, 1.0f);

			pVtx += 4;
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();

		// フェード状態取得
		int nFadeState = CFade::GetFadeState();

		// フェードアウト時に終了処理を入れる
		if (nFadeState == CFade::FADE_OUT)
		{
			Uninit();
		}
	}
}



//============================
//タイマーの描画処理
//============================
void CTimer::Draw()
{
	CRenderer* Renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, m_pTexBuff);

	for (int nCntTime = 0; nCntTime < 3; nCntTime++)
	{
		if (bUse[nCntTime] == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);
		}
	}
}

//============================
//タイマーの生成処理
//============================
CTimer* CTimer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CTimer* pTimer;

	pTimer = new CTimer;

	pTimer->SetType(TYPE::TIMER);
	pTimer->m_nPos = pos;
	pTimer->m_nSize = size;

	//タイマーの初期化
	pTimer->Init();


	if (C3dgoalobj::GetStageNum() == 0 || C3dgoalobj::GetStageNum() == 1)
	{
		m_nTimerCnt = 0;
		m_nTime = 0;
		m_nDisplayTime = 0;
	}

	LPDIRECT3DTEXTURE9 pTexture;

	//テクスチャの読み込む
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\SCORE_NUMBER.png", &pTexture);

	pTimer->BindTexture(pTexture);

	return pTimer;
}

void CTimer::AddTimer(int nTime)
{
	m_nTime += nTime;
}

//=======================
//テクスチャの設定
//=======================
void CTimer::BindTexture(LPDIRECT3DTEXTURE9 pTex)
{
	m_pTexBuff = pTex;
}

