//=================================================
//
// アイテムの処理 (timer.cpp)
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

int CTimer::m_nTime = 0;

//============================
//コンストラクタ
//============================
CTimer::CTimer(int nPriority) : CObject2D(nPriority)
{
	m_nTime = 0;
	m_nTimerCnt = 0;
	bUpdateTime = false;
	for (int nCntTime = 0; nCntTime < 3; nCntTime++)
	{
		bUse[nCntTime] = false;
	}
}

//============================
//デストラクタ
//============================
CTimer::~CTimer()
{

}

//============================
//アイテムの初期化処理
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
		//頂点座標を設定
		pVtx[0].pos = D3DXVECTOR3(600.0f + (nCntTime * TEX_TIME_INTERVAL), 70.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(630.0f + (nCntTime * TEX_TIME_INTERVAL), 70.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(600.0f + (nCntTime * TEX_TIME_INTERVAL), 100.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(630.0f + (nCntTime * TEX_TIME_INTERVAL), 100.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(0, 0, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(0, 0, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(0, 0, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(0, 0, 0, 255);

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
//アイテムの初期化処理
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
//アイテムの更新処理
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
				m_nTime++; //時間を1秒減らす
				m_nTimerCnt = 0; //フレームカウントリセット
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
		int CopyTime = m_nTime;

		// 各桁の値を計算
		for (int nCntTime = NUM_TIME - 1; nCntTime >= 0; nCntTime--)
		{
			aPosTexU[nCntTime] = CopyTime % 10;
			CopyTime /= 10;
		}

		//頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//テクスチャ座標の更新
		for (int nCntTime = 0; nCntTime < NUM_TIME; nCntTime++)
		{
			pVtx[0].tex = D3DXVECTOR2(aPosTexU[nCntTime] / 10.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((aPosTexU[nCntTime] + 1) / 10.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(aPosTexU[nCntTime] / 10.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((aPosTexU[nCntTime] + 1) / 10.0f, 1.0f);

			////残り時間が少ない状態になったときに数字を赤くする
			//if (m_nTime < 20)
			//{
			//	pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			//	pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			//	pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			//	pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			//}
			pVtx += 4;
		}

		//頂点バッファをアンロックする
		m_pVtxBuff->Unlock();

		//フェード状態取得
		int nFadeState = CFade::GetFadeState();

		//フェードアウト時に終了処理を入れる
		if (nFadeState == CFade::FADE_OUT)
		{
			Uninit();
			m_nTime = 0;
		}

		////残り時間が0以下になった場合、終了演出を入れる
		//if (m_nTime < 0)
		//{
		//	m_nTime = 0;
		//	CEndCallUI::DisplayEndCallUI(CEndCallUI::ICONDISPLAY::ICON_FAILED, CEndCallUI::UIDISPLAY::UI_DISPLAY);
		//}


		//if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN))
		//{
		//	Uninit();
		//	CManager::GetFade()->SetFade(CScene::MODE_RESULT);
		//	m_nTime = 0;
		//}
	}
}

//============================
//アイテムの描画処理
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
//アイテムの生成処理
//============================
CTimer* CTimer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CTimer* pTimer;

	pTimer = new CTimer;

	pTimer->SetType(TYPE::TIMER);
	pTimer->m_nPos = pos;
	pTimer->m_rot = rot;

	//アイテムの初期化
	pTimer->Init();

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

