//=================================================
//
// ビルボードの処理 (billboard.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "3dtutorialui.h"
#include "3dboss.h"

bool C3dTutorialUI::m_bUse[NUM_UI] = {};
int C3dTutorialUI::m_nType = {};


//======================
// コンストラクタ
//======================
C3dTutorialUI::C3dTutorialUI(int nPriority) : CObject3D(nPriority)
{
	m_pVtxBuff = nullptr;
}

//======================
// デストラクタ
//======================
C3dTutorialUI::~C3dTutorialUI()
{
	m_pVtxBuff = nullptr;
	m_pTexBuff = nullptr;
}

//======================
// 初期化処理
//======================
HRESULT C3dTutorialUI::Init()
{
	CRenderer* Renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	for (int nCnt = 0; nCnt < NUM_UI; nCnt++)
	{
		m_bUse[nCnt] = true;


	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * NUM_UI, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	//頂点情報のポインタ
	VERTEX_3D* pVtx;

	//頂点バッファをロックし頂点情報時へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < NUM_UI; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-m_nSize[nCnt].x, m_nSize[nCnt].y, m_nSize[nCnt].z);
		pVtx[1].pos = D3DXVECTOR3(m_nSize[nCnt].x, m_nSize[nCnt].y, m_nSize[nCnt].z);
		pVtx[2].pos = D3DXVECTOR3(-m_nSize[nCnt].x, -m_nSize[nCnt].y, m_nSize[nCnt].z);
		pVtx[3].pos = D3DXVECTOR3(m_nSize[nCnt].x, -m_nSize[nCnt].y, m_nSize[nCnt].z);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの初期設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の初期設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//======================
//終了処理
//======================
void C3dTutorialUI::Uninit()
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

	CObject3D::Uninit();
}

//======================
//終了処理
//======================
void C3dTutorialUI::Update()
{
	//頂点情報のポインタ
	VERTEX_3D* pVtx;

	//頂点バッファをロックし頂点情報時へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCnt = 0; nCnt < NUM_UI; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(m_nPos[nCnt].x - m_nSize[nCnt].x, m_nPos[nCnt].y + m_nSize[nCnt].y, m_nPos[nCnt].z);
		pVtx[1].pos = D3DXVECTOR3(m_nPos[nCnt].x + m_nSize[nCnt].x * 2, m_nPos[nCnt].y + m_nSize[nCnt].y, m_nPos[nCnt].z);
		pVtx[2].pos = D3DXVECTOR3(m_nPos[nCnt].x - m_nSize[nCnt].x, m_nPos[nCnt].y - m_nSize[nCnt].y, m_nPos[nCnt].z);
		pVtx[3].pos = D3DXVECTOR3(m_nPos[nCnt].x + m_nSize[nCnt].x * 2, m_nPos[nCnt].y - m_nSize[nCnt].y, m_nPos[nCnt].z);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの初期設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の初期設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//if (m_nType == C3dTutorialUI::TUTORIALUI_DISPLAY::BSUI_R_DIAGATK)
	//{
	//	m_nPos[m_nType].x = 110.0f;
	//	m_nPos[m_nType].y = 130.0f;

	//	m_nSize[m_nType].x = 400.0f;
	//	m_nSize[m_nType].y = 320.0f;
	//}

	//if (m_nType == C3dTutorialUI::TUTORIALUI_DISPLAY::BSUI_L_DIAGATK)
	//{
	//	m_nPos[m_nType].x = 0.0f;
	//	m_nPos[m_nType].y = 130.0f;

	//	m_nSize[m_nType].x = 400.0f;
	//	m_nSize[m_nType].y = 320.0f;
	//}


	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	//フェードの状態を取得
	int nFadeState = CFade::GetFadeState();

	//フェードアウトになった場合、終了処理に移行
	if (nFadeState == CFade::FADE_OUT)
	{
		C3dTutorialUI::Uninit();
	}
}

//======================
// 描画処理
//======================
void C3dTutorialUI::Draw()
{
	CRenderer* Renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	D3DXMATRIX mtxTrans;	//計算用マトリックス

	//ビューマトリックス取得
	D3DXMATRIX mtxView;

	////ライトを無効にする
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//=========================
	// 壁めり込み防止コード
	//=========================

	////Zの比較方法変更
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	////Zバッファに書き込まない
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//アルファテストを有効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//ビューマトリックス取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ポリゴンをカメラの正面に向け続ける
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);	//逆行列を求める

	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	for (int nCnt = 0; nCnt < NUM_UI; nCnt++)
	{
		D3DXMatrixTranslation(&mtxTrans, m_nPos[nCnt].x, m_nPos[nCnt].y, m_nPos[nCnt].z);
	}

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int nCnt = 0; nCnt < NUM_UI; nCnt++)
	{
		if (m_bUse[nCnt] == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, m_pTexBuff);

			//ビルボードの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);

		}
	}



	//アルファテストを元に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


	//=========================
	// 壁めり込み防止コード
	//=========================

	////Zの比較方法変更
	//pDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESSEQUAL);

	////Zバッファに書き込む
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	////ライトを無効にする
	//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//======================
// オブジェクト生成処理
//======================
C3dTutorialUI* C3dTutorialUI::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size , int nType)
{
	C3dTutorialUI* p3dBossATKUI = nullptr;

	p3dBossATKUI = new C3dTutorialUI;

	for (int nCnt = 0; nCnt < NUM_UI; nCnt++)
	{
		p3dBossATKUI->m_nPos[nCnt] = pos;

		p3dBossATKUI->m_nSize[nCnt] = size;
	}

	//テクスチャ読み込み選別
	if (nType == 0)
	{
		p3dBossATKUI->Load("data\\TEXTURE\\TUTORIAL_UI\\howtoplayer000.png");
	}

	else if (nType == 1)
	{
		p3dBossATKUI->Load("data\\TEXTURE\\TUTORIAL_UI\\howtoplayer001.png");
	}

	else if (nType == 2)
	{
		p3dBossATKUI->Load("data\\TEXTURE\\TUTORIAL_UI\\howtoplayer002.png");
	}

	else if (nType == 3)
	{
		p3dBossATKUI->Load("data\\TEXTURE\\TUTORIAL_UI\\howtoplayer003.png");
	}

	else if (nType == 4)
	{
		p3dBossATKUI->Load("data\\TEXTURE\\TUTORIAL_UI\\howtoplayer004.png");
	}

	//テクスチャの設定
	p3dBossATKUI->BindTexture(p3dBossATKUI->m_pTexBuff);

	p3dBossATKUI->m_nType = nType;


	//初期化に成功した場合
	if (SUCCEEDED(p3dBossATKUI->Init()))
	{
		p3dBossATKUI->SetType(TYPE::BOSS_ATKUI);
	}

	return p3dBossATKUI;
}

//======================
// テクスチャロード処理
//======================
HRESULT C3dTutorialUI::Load(LPCSTR textureFileName)
{
	LPDIRECT3DDEVICE9 pDevice = nullptr;
	pDevice = CManager::GetRenderer()->GetDevice();

	if (FAILED(D3DXCreateTextureFromFile(pDevice, textureFileName, &m_pTexBuff)))
	{
		return E_FAIL;
	}

	return S_OK;
}

bool C3dTutorialUI::DisplayBossATKUI(int nDisplayID, int DisplayOption)
{
	//表示する場合
	if (DisplayOption == C3dTutorialUI::UIDISPLAY::UI_DISPLAY)
	{
		m_bUse[nDisplayID] = true;
	}

	//非表示にする場合
	if (DisplayOption == C3dTutorialUI::UIDISPLAY::UI_HIDDEN)
	{
		m_bUse[nDisplayID] = false;
	}

	m_nType = nDisplayID;

	return m_bUse[nDisplayID];
}