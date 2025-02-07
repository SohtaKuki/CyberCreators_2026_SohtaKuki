//=================================================
//
// 2Dポリゴンの描画処理 (object2D.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "fadebefore.h"
#include "manager.h"
#include "endcallui.h"

//======================
// コンストラクタ
//======================
CFadebefore::CFadebefore(int nPriority) : CObject2D(nPriority)
{
	m_nFadeTimer = 0;
}

//======================
// デストラクタ
//======================
CFadebefore::~CFadebefore()
{

}

//======================
// 初期化処理
//======================
HRESULT CFadebefore::Init()
{
	CObject2D::Init();

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	CObject2D::GetBuff()->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_nPos.x - m_nSize.x, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_nPos.x + m_nSize.x, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_nPos.x - m_nSize.x, m_nSize.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_nPos.x + m_nSize.x, m_nSize.y, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = D3DCOLOR_RGBA(0, 0, 0, 0);
	pVtx[1].col = D3DCOLOR_RGBA(0, 0, 0, 0);
	pVtx[2].col = D3DCOLOR_RGBA(0, 0, 0, 0);
	pVtx[3].col = D3DCOLOR_RGBA(0, 0, 0, 0);

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
void CFadebefore::Uninit()
{
	CObject2D::Uninit();
}

//======================
// 更新処理
//======================
void CFadebefore::Update()
{
	if (CEndCallUI::GetEndPhase() == CEndCallUI::END_PHASE_NUM::END_PHASE_1||
		CEndCallUI::GetEndPhase() == CEndCallUI::END_PHASE_NUM::END_PHASE_2)
	{
		VERTEX_2D* pVtx;

		//頂点バッファをロックし、頂点情報へのポインタを取得
		CObject2D::GetBuff()->Lock(0, 0, (void**)&pVtx, 0);

		if (m_nFadeTimer < 255)
		{
			m_nFadeTimer += 5;
		}

		pVtx[0].col = D3DCOLOR_RGBA(0, 0, 0, m_nFadeTimer);
		pVtx[1].col = D3DCOLOR_RGBA(0, 0, 0, m_nFadeTimer);
		pVtx[2].col = D3DCOLOR_RGBA(0, 0, 0, m_nFadeTimer);
		pVtx[3].col = D3DCOLOR_RGBA(0, 0, 0, m_nFadeTimer);

		pVtx += 4;

		CObject2D::GetBuff()->Unlock();
	}

	int nFadeState = CFade::GetFadeState();

	if (nFadeState == CFade::FADE_OUT)
	{
		CFadebefore::Uninit();
	}
}

//======================
// 描画処理
//======================
void CFadebefore::Draw()
{
	CObject2D::Draw();
}

//======================
// オブジェクト生成処理
//======================
CFadebefore* CFadebefore::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CFadebefore* Gameui = new CFadebefore;

	Gameui->m_nPos = pos;

	Gameui->m_nSize = size;

	Gameui->Init();

	//LPDIRECT3DTEXTURE9 pTexture;

	////テクスチャの読み込み
	//D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\timer000.png", &pTexture);

	//Gameui->BindTexture(pTexture);

	return Gameui;
}