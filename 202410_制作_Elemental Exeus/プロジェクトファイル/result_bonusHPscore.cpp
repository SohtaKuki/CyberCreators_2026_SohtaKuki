//=================================================
//
// アイテムの処理 (score.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "result_bonusHPscore.h"
#include "score.h"
#include "manager.h"
#include "object2D.h"
#include "player.h"
#include "explosion.h"
#include "enemy.h"
#include "block.h"

int CResultHPBScore::m_nResultHPBScore = 0;

//============================
//コンストラクタ
//============================
CResultHPBScore::CResultHPBScore(int nPriority) : CObject2D(nPriority)
{
	m_nResultHPBScore = 0;
	m_nResultHPBScoreCnt = 0;
	bUpdateTime = false;
}

//============================
//デストラクタ
//============================
CResultHPBScore::~CResultHPBScore()
{

}

//============================
//アイテムの初期化処理
//============================
HRESULT CResultHPBScore::Init()
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

	for (int nCntTime = 0; nCntTime < NUM_RESULT_HPB_SCORE; nCntTime++)
	{
		//頂点座標を設定
		pVtx[0].pos = D3DXVECTOR3(m_nPos.x + (nCntTime * TEX_RESULT_HPB_SCORE_INTERVAL), m_nPos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_nPos.x + m_nSize.x + (nCntTime * TEX_RESULT_HPB_SCORE_INTERVAL), m_nPos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_nPos.x + (nCntTime * TEX_RESULT_HPB_SCORE_INTERVAL), (m_nPos.y + m_nSize.y), 0.0f);
		pVtx[3].pos = D3DXVECTOR3((m_nPos.x + m_nSize.x + (nCntTime * TEX_RESULT_HPB_SCORE_INTERVAL)), (m_nPos.y + m_nSize.y), 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

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
void CResultHPBScore::Uninit()
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
void CResultHPBScore::Update()
{
	VERTEX_2D* pVtx;

	int aPosTexU[NUM_RESULT_HPB_SCORE];

	m_nResultHPBScore = CScore::GetHPBonusScore();


	// 時間の値をコピー
	int CopyTime = m_nResultHPBScore;

	// 各桁の値を計算
	for (int nCntTime = NUM_RESULT_HPB_SCORE - 1; nCntTime >= 0; nCntTime--)
	{
		aPosTexU[nCntTime] = CopyTime % 10;
		CopyTime /= 10;
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の更新
	for (int nCntTime = 0; nCntTime < NUM_RESULT_HPB_SCORE; nCntTime++)
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
		m_nResultHPBScore = 0;
	}

}

//============================
//アイテムの描画処理
//============================
void CResultHPBScore::Draw()
{
	CRenderer* Renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, m_pTexBuff);

	for (int nCntTime = 0; nCntTime < NUM_RESULT_HPB_SCORE; nCntTime++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);
	}
}

//============================
//アイテムの生成処理
//============================
CResultHPBScore* CResultHPBScore::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CResultHPBScore* pResultHPBScore;

	pResultHPBScore = new CResultHPBScore;

	pResultHPBScore->SetType(TYPE::SCORE);
	pResultHPBScore->m_nPos = pos;
	pResultHPBScore->m_nSize = size;

	//アイテムの初期化
	pResultHPBScore->Init();

	LPDIRECT3DTEXTURE9 pTexture;

	//テクスチャの読み込む
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\SCORE_NUMBER_HPB.png", &pTexture);

	pResultHPBScore->BindTexture(pTexture);

	return pResultHPBScore;
}

void CResultHPBScore::AddResultHPBScore(int nTime)
{
	m_nResultHPBScore += nTime;
}

//=======================
//テクスチャの設定
//=======================
void CResultHPBScore::BindTexture(LPDIRECT3DTEXTURE9 pTex)
{
	m_pTexBuff = pTex;
}

