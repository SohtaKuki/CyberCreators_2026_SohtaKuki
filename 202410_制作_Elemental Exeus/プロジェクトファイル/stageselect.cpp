//===================================
//
// ポーズ処理 [pause.cpp]
// Author: Sohta Kuki
//
//===================================

#include "stageselect.h"
#include "input.h"

int CStageSelect::m_nPauseSelect = 0;

//==========================
//コンストラクタ
//==========================
CStageSelect::CStageSelect(int nPriority) : CObject2D(nPriority)
{
	m_nPauseSelect = 0;
}

//==========================
//デストラクタ
//==========================
CStageSelect::~CStageSelect()
{

}

//=============================
//ポーズ画面の初期化処理
//=============================

HRESULT CStageSelect::Init()
{
	CRenderer* Renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\STAGE_SELECT_1.png", &m_pTexturePauseSelect[STAGE_SELECT_1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\STAGE_SELECT_2.png", &m_pTexturePauseSelect[STAGE_SELECT_2]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\STAGE_SELECT_3.png", &m_pTexturePauseSelect[STAGE_SELECT_3]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffPauseSelect,
		NULL);
	VERTEX_2D* pVtx;	//頂点情報のポインタ

	//頂点バッファをロックして、頂点情報へのポインタを取得
	m_pVtxBuffPauseSelect->Lock(0, 0, (void**)&pVtx, 0);
	pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);



	//頂点バッファのアンロック
	m_pVtxBuffPauseSelect->Unlock();

	CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_STAGE_SELECT);

	return S_OK;
}

//=============================
//ポーズ画面の終了処理
//=============================

void CStageSelect::Uninit(void)
{
	for (int nCnt = 0; nCnt < STAGE_SELECT_MAX; nCnt++)
	{
		//テクスチャの破棄
		if (m_pTexturePauseSelect[nCnt] != NULL)
		{
			m_pTexturePauseSelect[nCnt]->Release();
			m_pTexturePauseSelect[nCnt] = NULL;
		}
	}
	//頂点バッファの破棄
	if (m_pVtxBuffPauseSelect != NULL)
	{
		m_pVtxBuffPauseSelect->Release();
		m_pVtxBuffPauseSelect = NULL;
	}
}

//=============================
//ポーズ画面の更新処理
//=============================

void CStageSelect::Update(void)
{
	int nFadeState = CFade::GetFadeState();

	if (nFadeState == CFade::FADE_OUT)
	{
		CStageSelect::Uninit();
		return;
	}

	VERTEX_2D* pVtx;	//頂点情報のポインタ

	//頂点バッファをロックして、頂点情報へのポインタを取得
	m_pVtxBuffPauseSelect->Lock(0, 0, (void**)&pVtx, 0);
	if (CManager::GetKeyboard()->GetTrigger(DIK_W))
	{

		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SELECT);

		if (m_nPauseSelect > 0)
		{
			m_nPauseSelect--;
		}

		else if (m_nPauseSelect <= 0)
		{
			m_nPauseSelect = STAGE_SELECT_3;
		}
	}

	if (CManager::GetKeyboard()->GetTrigger(DIK_LEFT) || CManager::GetJoypad()->GetTrigger(CInputJoypad::JOYKEY_LEFT) || CManager::GetJoypad()->GetStickTrigger(CInputJoypad::STICKTYPE_LEFT, CInputJoypad::STICKANGLE_LEFT) == true)
	{
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SELECT);

		if (m_nPauseSelect > 0)
		{
			m_nPauseSelect--;
		}
		else if (m_nPauseSelect <= 0)
		{
			m_nPauseSelect = STAGE_SELECT_3;
		}
	}

	if (CManager::GetKeyboard()->GetTrigger(DIK_S))
	{
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SELECT);

		if (m_nPauseSelect < 2)
		{
			m_nPauseSelect++;
		}
		else if (m_nPauseSelect >= 2)
		{
			m_nPauseSelect = STAGE_SELECT_1;
		}
	}

	if (CManager::GetKeyboard()->GetTrigger(DIK_RIGHT) || CManager::GetJoypad()->GetTrigger(CInputJoypad::JOYKEY_RIGHT) || CManager::GetJoypad()->GetStickTrigger(CInputJoypad::STICKTYPE_LEFT, CInputJoypad::STICKANGLE_RIGHT) == true)
	{
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SELECT);

		if (m_nPauseSelect < 2)
		{
			m_nPauseSelect++;
		}
		else if (m_nPauseSelect >= 2)
		{
			m_nPauseSelect = STAGE_SELECT_1;
		}
	}
	//頂点バッファのアンロック
	m_pVtxBuffPauseSelect->Unlock();


}
//=============================
//ポーズ画面の描画処理
//=============================

void CStageSelect::Draw(void)
{
	CRenderer* Renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuffPauseSelect, 0, sizeof(VERTEX_2D));
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	//テクスチャの設定

	pDevice->SetTexture(0, m_pTexturePauseSelect[m_nPauseSelect]);
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);
}

//=============================
//ポーズ画面の取得処理
//=============================
int CStageSelect::GetStageSelect(void)
{
	return m_nPauseSelect;
}

//============================
//タイトルの生成処理
//============================
CStageSelect* CStageSelect::Create()
{
	CStageSelect* pStageSelect;

	pStageSelect = new CStageSelect;

	if (FAILED(pStageSelect->Init()))
	{
		delete pStageSelect;
		return nullptr;
	}

	return pStageSelect;
}