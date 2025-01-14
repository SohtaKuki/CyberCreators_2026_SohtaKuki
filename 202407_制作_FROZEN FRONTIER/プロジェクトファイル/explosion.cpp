//=================================================
//
// 爆発の処理 (explosion.cpp)
// Author: Sohta Kuki
//
//=================================================


#include "explosion.h"
#include "manager.h"

//============================
//コンストラクタ
//============================
CExplosion::CExplosion()
{
	m_nMove = { 0, 0, 0 };
	m_nPatternAnimExpl = 0;
	m_nCounterAnimExpl = 0;
}

//============================
//デストラクタ
//============================
CExplosion::~CExplosion()
{

}

//============================
//爆発アニメーションの初期化処理
//============================
HRESULT CExplosion::Init()
{
	//2Dオブジェクトの初期化
	CObject2D::Init();

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	CObject2D::GetBuff()->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = CObject2D::GetPos().x - EXPL_TEX_SIZE * 0.5f;
	pVtx[0].pos.y = CObject2D::GetPos().y - EXPL_TEX_SIZE * 0.5f;

	pVtx[1].pos.x = CObject2D::GetPos().x + EXPL_TEX_SIZE * 0.5f;
	pVtx[1].pos.y = CObject2D::GetPos().y - EXPL_TEX_SIZE * 0.5f;

	pVtx[2].pos.x = CObject2D::GetPos().x - EXPL_TEX_SIZE * 0.5f;
	pVtx[2].pos.y = CObject2D::GetPos().y + EXPL_TEX_SIZE * 0.5f;

	pVtx[3].pos.x = CObject2D::GetPos().x + EXPL_TEX_SIZE * 0.5f;
	pVtx[3].pos.y = CObject2D::GetPos().y + EXPL_TEX_SIZE * 0.5f;

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f / EXPLOSION_PATTERN_ANIM, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f / EXPLOSION_PATTERN_ANIM, 1.0f);

	//頂点バッファをアンロックする
	CObject2D::GetBuff()->Unlock();

	return S_OK;
}

//============================
//爆発アニメーションの終了処理
//============================
void CExplosion::Uninit()
{
	//2Dオブジェクトの終了
	CObject2D::Uninit();
}

//============================
//爆発アニメーションの更新処理
//============================
void CExplosion::Update()
{

	m_nPatternAnimExpl++;

	if (m_nPatternAnimExpl > UPDATE_TEX)
	{
		m_nCounterAnimExpl++;

		if (m_nCounterAnimExpl > EXPLOSION_PATTERN_ANIM)
		{
			CObject2D::Uninit();
			return;
		}

		m_nPatternAnimExpl = 0;
	}

	VERTEX_2D* pVtx;

	CObject2D::GetBuff()->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2((1.0f / EXPLOSION_PATTERN_ANIM) * m_nCounterAnimExpl - (1.0f / EXPLOSION_PATTERN_ANIM), 0.0f);
	pVtx[1].tex = D3DXVECTOR2((1.0f / EXPLOSION_PATTERN_ANIM) * m_nCounterAnimExpl, 0.0f);
	pVtx[2].tex = D3DXVECTOR2((1.0f / EXPLOSION_PATTERN_ANIM) * m_nCounterAnimExpl - (1.0f / EXPLOSION_PATTERN_ANIM), 1.0f);
	pVtx[3].tex = D3DXVECTOR2((1.0f / EXPLOSION_PATTERN_ANIM) * m_nCounterAnimExpl, 1.0f);


	CObject2D::GetBuff()->Unlock();
}

//============================
//爆発アニメーションの描画処理
//============================
void CExplosion::Draw()
{
	//2Dオブジェクトの描画
	CObject2D::Draw();
}

//============================
//爆発アニメーションの生成処理
//============================
CExplosion* CExplosion::Create(D3DXVECTOR3 pos)
{
	CExplosion* pExplosion;

	pExplosion = new CExplosion;

	pExplosion->CObject2D::GetPos() = pos;

	//爆発の初期化
	pExplosion->Init();

	LPDIRECT3DTEXTURE9 pTexture;

	//テクスチャの設定
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(),"data\\TEXTURE\\explosion000.png",&pTexture);

	pExplosion->BindTexture(pTexture);

	return pExplosion;
}