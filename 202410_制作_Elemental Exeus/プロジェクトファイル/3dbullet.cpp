//=================================================
//
// ビルボード描画式弾の処理 (3dbullet.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "3dbullet.h"
#include "3dblock.h"
#include "3denemy.h"
#include "3dexplosion.h"
#include "score.h"
#include "camera.h"
#include "scene.h"

//======================
// コンストラクタ
//======================
C3dbullet::C3dbullet(int nPriority) : CObject3D(nPriority)
{
	
}

//======================
// デストラクタ
//======================
C3dbullet::~C3dbullet()
{

}

//======================
// 初期化処理
//======================
HRESULT C3dbullet::Init()
{
	CRenderer* Renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	//テクスチャの読込み
	D3DXCreateTextureFromFile(pDevice, "data\\texture\\bullet_RGB.png", &m_pTexture);

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
	pVtx[0].col = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[1].col = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[2].col = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[3].col = D3DCOLOR_RGBA(255,255,255,255);

	//テクスチャ座標の初期設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//======================
//終了処理
//======================
void C3dbullet::Uninit()
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
void C3dbullet::Update()
{
	if (CScene::GetUpdateStat() == true)
	{
		//頂点情報のポインタ
		VERTEX_3D* pVtx;

		//頂点バッファをロックし頂点情報時へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		m_nPos += m_nMove;

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

		//テクスチャ座標の初期設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;

		//頂点バッファをアンロック
		m_pVtxBuff->Unlock();

		//弾とブロックの当たり判定
		for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
		{
			CObject* pObj = CObject::GetObj(3, nCntObj);

			if (pObj != nullptr)
			{
				CObject::TYPE type = pObj->GetType();

				C3dblock* p3dblock = (C3dblock*)pObj;

				D3DXVECTOR3 BlockPos = p3dblock->GetPos();

				//ブロックの場合
				if (type == CObject::TYPE::BLOCK)
				{
					if (m_nPos.x >= BlockPos.x - 30
						&& m_nPos.x <= BlockPos.x + 30
						&& m_nPos.z >= BlockPos.z - 30
						&& m_nPos.z <= BlockPos.z + 30)
					{
						C3dexplosion::Create(m_nPos, D3DXVECTOR3(40.0f, 40.0f, 0.0f), m_rot, 0);
						Uninit();
						return;
					}
				}

			}
		}

		

		

		//弾と敵の当たり判定
		for (int nCntObj = 0; nCntObj < C3denemy::MAX_ENEMY; nCntObj++)
		{
			CObject* pObj = CObject::GetObj(3, nCntObj);

			if (pObj != nullptr)
			{
				CObject::TYPE type = pObj->GetType();

				C3denemy* p3denemy = (C3denemy*)pObj;

				D3DXVECTOR3 EnemyPos = p3denemy->GetPos();

				//弾の種別がプレイヤーだった場合
				if (m_nType == 0)
				{
					//頂点情報のポインタ
					VERTEX_3D* pVtx;

					//頂点バッファをロックし頂点情報時へのポインタを取得
					m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

					//頂点カラーの初期設定
					pVtx[0].col = D3DCOLOR_RGBA(255, 46, 46, 255);
					pVtx[1].col = D3DCOLOR_RGBA(255, 46, 46, 255);
					pVtx[2].col = D3DCOLOR_RGBA(255, 46, 46, 255);
					pVtx[3].col = D3DCOLOR_RGBA(255, 46, 46, 255);

					pVtx += 4;

					//頂点バッファをアンロック金玉
					m_pVtxBuff->Unlock();


					//敵の場合
					if (type == CObject::TYPE::ENEMY_NORMALMOVE || type == CObject::TYPE::ENEMY_FLYMOVE || type == CObject::TYPE::ENEMY_NORMAL)
					{
						if (m_nPos.x >= EnemyPos.x - 30
							&& m_nPos.x <= EnemyPos.x + 30
							&& m_nPos.y >= EnemyPos.y - 15
							&& m_nPos.y <= EnemyPos.y + 15
							&& m_nPos.z >= EnemyPos.z - 30
							&& m_nPos.z <= EnemyPos.z + 30)
						{
							Uninit();
							CScore::AddScore(300);
							p3denemy->EnemyDamage(1);
							return;
						}
					}
				}
			}
		}

		//弾とプレイヤーの当たり判定
		for (int nCntObj = 0; nCntObj < 4; nCntObj++)
		{
			CObject* pObj = CObject::GetObj(3, nCntObj);

			if (pObj != nullptr)
			{
				CObject::TYPE type = pObj->GetType();

				C3dplayer* p3dplayer = (C3dplayer*)pObj;

				D3DXVECTOR3 PlayerPos = p3dplayer->GetPos();

				//弾の種別が敵だった場合
				if (m_nType == 1)
				{
					//頂点情報のポインタ
					VERTEX_3D* pVtx;

					//頂点バッファをロックし頂点情報時へのポインタを取得
					m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

					//頂点カラーの初期設定
					pVtx[0].col = D3DCOLOR_RGBA(0, 90, 255, 255);
					pVtx[1].col = D3DCOLOR_RGBA(0, 90, 255, 255);
					pVtx[2].col = D3DCOLOR_RGBA(0, 90, 255, 255);
					pVtx[3].col = D3DCOLOR_RGBA(0, 90, 255, 255);

					pVtx += 4;

					//頂点バッファをアンロック
					m_pVtxBuff->Unlock();

					//プレイヤーの場合
					if (type == CObject::TYPE::PLAYER)
					{
						if (m_nPos.x >= PlayerPos.x - 30
							&& m_nPos.x <= PlayerPos.x + 30
							&& m_nPos.y >= PlayerPos.y - 30
							&& m_nPos.y <= PlayerPos.y + 30
							&& m_nPos.z >= PlayerPos.z - 30
							&& m_nPos.z <= PlayerPos.z + 30)
						{
							CCamera::SetShake(3, 8.0f);
							C3dexplosion::Create(m_nPos, D3DXVECTOR3(40.0f, 40.0f, 0.0f), m_rot, 0);
							p3dplayer->PlayerDamage(10);
							Uninit();
							return;
						}
					}
				}
			}
		}
	}
}

//======================
// 描画処理
//======================
void C3dbullet::Draw()
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

	D3DXMatrixTranslation(&mtxTrans, m_nPos.x, m_nPos.y, m_nPos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//ビルボードの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//アルファテストを元に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


	//=========================
	// 壁めり込み防止コード
	//=========================

	////Zの比較方法変更
	//pDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESSEQUAL);

	////Zバッファに書き込む
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//======================
// オブジェクト生成処理
//======================
C3dbullet* C3dbullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size,D3DXVECTOR3 rot,int type)
{
	C3dbullet* bullet3d = new C3dbullet;

	//初期化に成功した場合
	if (SUCCEEDED(bullet3d->Init()))
	{
		bullet3d->m_nType = type;

		if (type == 0)
		{
			bullet3d->SetType(TYPE::PLAYER_BULLET);
		}

		if (type == 1)
		{
			bullet3d->SetType(TYPE::ENEMY_BULLET);
		}

		bullet3d->m_nPos = pos;

		//弾の高さを少しだけ補正する
		bullet3d->m_nPos.y += 10.0f;

		bullet3d->m_rot = rot;

		bullet3d->m_nSize = size;

		bullet3d->m_nLife = 60;

		//弾の発射方向
 		bullet3d->m_nMove.x = sinf(bullet3d->m_rot.y + D3DX_PI) * BULLET_SPD;
		bullet3d->m_nMove.z = cosf(bullet3d->m_rot.y + D3DX_PI) * BULLET_SPD;

	}

	return bullet3d;
}