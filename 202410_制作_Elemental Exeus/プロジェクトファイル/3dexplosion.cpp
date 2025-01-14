//=================================================
//
// ビルボード描画式弾の処理 (3dexplosion.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "3dexplosion.h"
#include "3dblock.h"
#include "3denemy.h"
#include "score.h"
#include "camera.h"
#include "scene.h"

//======================
// コンストラクタ
//======================
C3dexplosion::C3dexplosion(int nPriority) : CObject3D(nPriority)
{
	m_bUse = true;
}

//======================
// デストラクタ
//======================
C3dexplosion::~C3dexplosion()
{

}

//======================
// 初期化処理
//======================
HRESULT C3dexplosion::Init()
{
	CRenderer* Renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	//テクスチャの読込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\tornado_wi.png", &m_pTexture);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * 1, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	//頂点情報のポインタ
	VERTEX_3D* pVtx;

	//頂点バッファをロックし頂点情報時へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_nSize.x, m_nSize.y, m_nSize.z);
	pVtx[1].pos = D3DXVECTOR3(m_nSize.x, m_nSize.y, m_nSize.z);
	pVtx[2].pos = D3DXVECTOR3(-m_nSize.x, -m_nSize.y, m_nSize.z);
	pVtx[3].pos = D3DXVECTOR3(m_nSize.x, -m_nSize.y, m_nSize.z);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラーの初期設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);



	pVtx += 4;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//======================
//終了処理
//======================
void C3dexplosion::Uninit()
{
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}

	CObject3D::Uninit();
}

//======================
//終了処理
//======================
void C3dexplosion::Update()
{
	//フェードの状態を取得
	int nFadeState = CFade::GetFadeState();

	//フェードアウトしたら強制削除
	if (nFadeState == CFade::FADE_OUT)
	{
		C3dexplosion::Uninit();
		return;
	}

	if (CScene::GetUpdateStat() == true)
	{
		//頂点情報のポインタ
		VERTEX_3D* pVtx;

		//頂点バッファをロックし頂点情報時へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


		m_nLife--;

		//弾寿命が0の場合
		if (m_nLife <= 0)
		{
			Uninit();
			return;
		}

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3((-m_nSize.x), (m_nSize.y), (m_nSize.z));
		pVtx[1].pos = D3DXVECTOR3((m_nSize.x), (m_nSize.y), (m_nSize.z));
		pVtx[2].pos = D3DXVECTOR3((-m_nSize.x), (-m_nSize.y), (m_nSize.z));
		pVtx[3].pos = D3DXVECTOR3((m_nSize.x), (-m_nSize.y), (m_nSize.z));

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの初期設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		if (m_bUse == true)
		{
			m_nCounterAnim++;

			if (m_nCounterAnim % C3dexplosion::DELAY_ANIM == 0)
			{
				m_nCounterAnim = 0;

				//アニメ切り替え処理
				m_nPatternAnimExpl++;

				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2((1.0f / EXPLOSION_PATTERN_ANIM) * m_nPatternAnimExpl - (1.0f / EXPLOSION_PATTERN_ANIM), 0.0f);
				pVtx[1].tex = D3DXVECTOR2((1.0f / EXPLOSION_PATTERN_ANIM) * m_nPatternAnimExpl, 0.0f);
				pVtx[2].tex = D3DXVECTOR2((1.0f / EXPLOSION_PATTERN_ANIM) * m_nPatternAnimExpl - (1.0f / EXPLOSION_PATTERN_ANIM), 1.0f);
				pVtx[3].tex = D3DXVECTOR2((1.0f / EXPLOSION_PATTERN_ANIM) * m_nPatternAnimExpl, 1.0f);
			}


			if (m_nPatternAnimExpl > EXPLOSION_PATTERN_ANIM)
			{
				Uninit();
				return;
			}
		}

		pVtx += 4;

		//頂点バッファをアンロック
		m_pVtxBuff->Unlock();
	}
}

//======================
// 描画処理
//======================
void C3dexplosion::Draw()
{
	CRenderer* Renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	D3DXMATRIX mtxTrans;	//計算用マトリックス

	//ビューマトリックス取得
	D3DXMATRIX mtxView;

	//ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//=========================
	// 壁めり込み防止コード
	//=========================

	//Zの比較方法変更
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	//Zバッファに書き込まない
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

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

	D3DXMatrixTranslation(&mtxTrans, m_nPos.x, m_nPos.y, m_nPos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//使用済みの場合のみ通す
	if (m_bUse == true)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, m_pTexture);

		//ビルボードの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}


	//アルファテストを元に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


	//=========================
	// 壁めり込み防止コード
	//=========================

	//Zの比較方法変更
	pDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESSEQUAL);

	//Zバッファに書き込む
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//======================
// オブジェクト生成処理
//======================
C3dexplosion* C3dexplosion::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, int type)
{
	C3dexplosion* explosion3d = new C3dexplosion;

	//初期化に成功した場合
	if (SUCCEEDED(explosion3d->Init()))
	{
		explosion3d->m_nType = type;

		if (type == 0)
		{
			explosion3d->SetType(TYPE::PLAYER_EXPLOSION);
		}

		if (type == 1)
		{
			explosion3d->SetType(TYPE::ENEMY_EXPLOSION);
		}

		explosion3d->m_nPos = pos;

		//弾の高さを少しだけ補正する
		explosion3d->m_nPos.y += 5.0f;

		explosion3d->m_rot = rot;

		explosion3d->m_nSize = size;

		explosion3d->m_nLife = 80;

		//弾の発射方向
		explosion3d->m_nMove.x = sinf(explosion3d->m_rot.y + D3DX_PI) * BULLET_SPD;
		explosion3d->m_nMove.z = cosf(explosion3d->m_rot.y + D3DX_PI) * BULLET_SPD;

	}

	return explosion3d;
}

void C3dexplosion::SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col)
{

}