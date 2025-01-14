//=================================================
//
// ビルボード描画式チャージ弾の処理 (3dchargebullet.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "3dchargebullet.h"
#include "3dblock.h"
#include "3denemy.h"
#include "3dbrokenblock.h"
#include "3dwall.h"
#include "score.h"
#include "scene.h"
#include "3dexplosion.h"

//======================
// コンストラクタ
//======================
C3dchargebullet::C3dchargebullet(int nPriority) : CObject3D(nPriority)
{

}

//======================
// デストラクタ
//======================
C3dchargebullet::~C3dchargebullet()
{

}

//======================
// 初期化処理
//======================
HRESULT C3dchargebullet::Init()
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
	pVtx[0].col = D3DCOLOR_RGBA(255, 46, 46, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 46, 46, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 46, 46, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 46, 46, 255);

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
void C3dchargebullet::Uninit()
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
void C3dchargebullet::Update()
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
		pVtx[0].col = D3DCOLOR_RGBA(255, 46, 46, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 46, 46, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 46, 46, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 46, 46, 255);

		//テクスチャ座標の初期設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;

		//頂点バッファをアンロック
		m_pVtxBuff->Unlock();

		//弾とブロックの当たり判定
		for (int nCntObj = 0; nCntObj < C3dblock::MAX_BLOCK; nCntObj++)
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
					if (m_nPos.x >= BlockPos.x - NORMAL_BLOCK_COLISION
						&& m_nPos.x <= BlockPos.x + NORMAL_BLOCK_COLISION
						&& m_nPos.z >= BlockPos.z - NORMAL_BLOCK_COLISION
						&& m_nPos.z <= BlockPos.z + NORMAL_BLOCK_COLISION)
					{
						C3dexplosion::Create(m_nPos, D3DXVECTOR3(40.0f, 40.0f, 0.0f), m_rot, 0);
						Uninit();
						return;
					}
				}

			}

		}

		//弾とブロックの当たり判定
		for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
		{
			CObject* pObj = CObject::GetObj(3, nCntObj);

			if (pObj != nullptr)
			{
				CObject::TYPE type = pObj->GetType();

				C3dwall* p3dwall = (C3dwall*)pObj;

				D3DXVECTOR3 WallPos = p3dwall->GetPos();

				//ブロックの場合
				if (type == CObject::TYPE::WALL_WIDTH)
				{
					if (m_nPos.x >= WallPos.x - 120
						&& m_nPos.x <= WallPos.x + 120
						&& m_nPos.z >= WallPos.z - 10
						&& m_nPos.z <= WallPos.z + 10)
					{

						Uninit();
						return;
					}
				}

				//ブロックの場合
				if (type == CObject::TYPE::WALL_WIDTH_SHORT)
				{
					if (m_nPos.x >= WallPos.x - 60
						&& m_nPos.x <= WallPos.x + 60
						&& m_nPos.z >= WallPos.z - 10
						&& m_nPos.z <= WallPos.z + 10)
					{

						Uninit();
						return;
					}
				}

				//ブロックの場合
				if (type == CObject::TYPE::WALL_HEIGHT)
				{
					if (m_nPos.x >= WallPos.x - 10
						&& m_nPos.x <= WallPos.x + 10
						&& m_nPos.z >= WallPos.z - 120
						&& m_nPos.z <= WallPos.z + 120)
					{

						Uninit();
						return;
					}
				}

				//ブロックの場合
				if (type == CObject::TYPE::WALL_HEIGHT_SHORT)
				{
					if (m_nPos.x >= WallPos.x - 10
						&& m_nPos.x <= WallPos.x + 10
						&& m_nPos.z >= WallPos.z - 60
						&& m_nPos.z <= WallPos.z + 60)
					{

						Uninit();
						return;
					}
				}
			}
		}

		//弾とブロックの当たり判定
		for (int nCntObj = 0; nCntObj < C3dbrokenblock::MAX_BLOCK; nCntObj++)
		{
			CObject* pObj = CObject::GetObj(3, nCntObj);

			if (pObj != nullptr)
			{
				CObject::TYPE type = pObj->GetType();

				C3dbrokenblock* p3dbrokenblock = (C3dbrokenblock*)pObj;

				D3DXVECTOR3 BlockPos = p3dbrokenblock->GetPos();

				//破壊可能ブロックの場合
				if (type == CObject::TYPE::BROKENBLOCK)
				{
					if (m_nPos.x >= BlockPos.x - NORMAL_BLOCK_COLISION
						&& m_nPos.x <= BlockPos.x + NORMAL_BLOCK_COLISION
						&& m_nPos.y >= BlockPos.y - NORMAL_BLOCK_COLISION
						&& m_nPos.y <= BlockPos.y + NORMAL_BLOCK_COLISION
						&& m_nPos.z >= BlockPos.z - NORMAL_BLOCK_COLISION
						&& m_nPos.z <= BlockPos.z + NORMAL_BLOCK_COLISION)
					{
						C3dexplosion::Create(m_nPos, D3DXVECTOR3(40.0f, 40.0f, 0.0f), m_rot, 0);
						Uninit();
						CCamera::SetShake(3, 4.0f);
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_BRBLOCK_DEATH);
						p3dbrokenblock->Uninit();
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

				//敵の場合
				if (type == CObject::TYPE::ENEMY_XMOVE || type == CObject::TYPE::ENEMY_ZMOVE || type == CObject::TYPE::ENEMY_NORMAL)
				{
					if (m_nPos.x >= EnemyPos.x - NORMAL_BLOCK_COLISION
						&& m_nPos.x <= EnemyPos.x + NORMAL_BLOCK_COLISION
						&& m_nPos.y >= EnemyPos.y - NORMAL_BLOCK_COLISION
						&& m_nPos.y <= EnemyPos.y + NORMAL_BLOCK_COLISION
						&& m_nPos.z >= EnemyPos.z - NORMAL_BLOCK_COLISION
						&& m_nPos.z <= EnemyPos.z + NORMAL_BLOCK_COLISION)
					{
						CScore::AddScore(17000);
						C3dexplosion::Create(m_nPos, D3DXVECTOR3(50.0f, 50.0f, 0.0f), m_rot, 0);
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_ENEMY_DAMAGE);
						p3denemy->Uninit();
						return;
					}
				}

			}
		}
	}
}

//======================
// 描画処理
//======================
void C3dchargebullet::Draw()
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
C3dchargebullet* C3dchargebullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot)
{
	C3dchargebullet* chbullet3d = new C3dchargebullet;

	//初期化に成功した場合
	if (SUCCEEDED(chbullet3d->Init()))
	{
		chbullet3d->SetType(TYPE::CHBULLET);

		chbullet3d->m_nPos = pos;

		//弾の高さを少しだけ補正する
		chbullet3d->m_nPos.y += 10.0f;

		chbullet3d->m_rot = rot;

		chbullet3d->m_nSize = size;

		chbullet3d->m_nLife = 80;

		//弾の発射方向
		chbullet3d->m_nMove.x = sinf(chbullet3d->m_rot.y + D3DX_PI) * BULLET_SPD;
		chbullet3d->m_nMove.z = cosf(chbullet3d->m_rot.y + D3DX_PI) * BULLET_SPD;

	}

	return chbullet3d;
}