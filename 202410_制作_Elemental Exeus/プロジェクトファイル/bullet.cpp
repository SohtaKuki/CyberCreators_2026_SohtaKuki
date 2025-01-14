//=================================================
//
// 弾の処理 (bullet.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "bullet.h"
#include "manager.h"
#include "object2D.h"
#include "player.h"
#include "explosion.h"
#include "enemy.h"
#include "block.h"

//============================
//コンストラクタ
//============================
CBullet::CBullet(int nPriority) : CPlayer(nPriority)
{
	m_nLife = 0;
}

//============================
//デストラクタ
//============================
CBullet::~CBullet()
{

}

//============================
//弾の初期化処理
//============================
HRESULT CBullet::Init()
{
	//初期化
	CObject2D::Init();

	return S_OK;
}

//============================
//弾の初期化処理
//============================
void CBullet::Uninit()
{
	//初期化
	CObject2D::Uninit();
}

//============================
//弾の更新処理
//============================
void CBullet::Update()
{
	//弾の移動処理
	CObject2D::GetPos() += m_nMove;

	//寿命を減らす
	m_nLife--;

	//寿命が尽きたときの処理
	if (m_nLife <= 0)
	{
		CExplosion::Create(CObject2D::GetPos());
		CObject2D::Uninit();
		return;
	}

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	CObject2D::GetBuff()->Lock(0, 0, (void**)&pVtx, 0);
	 
	//頂点座標の更新
	pVtx[0].pos.x = CObject2D::GetPos().x - 35 * 0.5f;
	pVtx[0].pos.y = CObject2D::GetPos().y - 35 * 0.5f;

	pVtx[1].pos.x = CObject2D::GetPos().x + 35 * 0.5f;
 	pVtx[1].pos.y = CObject2D::GetPos().y - 35 * 0.5f;

	pVtx[2].pos.x = CObject2D::GetPos().x - 35 * 0.5f;
	pVtx[2].pos.y = CObject2D::GetPos().y + 35 * 0.5f;

	pVtx[3].pos.x = CObject2D::GetPos().x + 35 * 0.5f;
	pVtx[3].pos.y = CObject2D::GetPos().y + 35 * 0.5f;

	//頂点バッファをアンロックする
	CObject2D::GetBuff()->Unlock();

	//弾の当たり判定
	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		CObject* pObj = CObject::GetObj(3,nCntObj);

		if (pObj != nullptr)
		{
			CObject::TYPE type = pObj->GetType();

			CEnemy* pEnemy = (CEnemy*)pObj;

			D3DXVECTOR3 EnemyPos = pEnemy->GetPos();

			if (type == CObject::TYPE::ENEMY)
			{
				if (CObject2D::GetPos().x >= EnemyPos.x - ENEMY_HITPOINT
					&& CObject2D::GetPos().x <= EnemyPos.x + ENEMY_HITPOINT
					&& CObject2D::GetPos().y >= EnemyPos.y - ENEMY_HITPOINT
					&& CObject2D::GetPos().y <= EnemyPos.y + ENEMY_HITPOINT)
				{
					CExplosion::Create(CObject2D::GetPos());
					pEnemy->Damage();
					CObject2D::Uninit();
					return;
				}
			}

			CBlock* pBlock = (CBlock*)pObj;

			D3DXVECTOR3 BlockPos = pBlock->GetPos();

			if (type == CObject::TYPE::BLOCK)
			{
				if (CObject2D::GetPos().x >= BlockPos.x - ENEMY_HITPOINT
					&& CObject2D::GetPos().x <= BlockPos.x + ENEMY_HITPOINT
					&& CObject2D::GetPos().y >= BlockPos.y - ENEMY_HITPOINT
					&& CObject2D::GetPos().y <= BlockPos.y + ENEMY_HITPOINT)
				{
					CExplosion::Create(CObject2D::GetPos());
					pEnemy->Damage();
					CObject2D::Uninit();
					return;
				}
			}
		}
	}
}

//============================
//弾の描画処理
//============================
void CBullet::Draw()
{
	CObject2D::Draw();
}

//============================
//弾の生成処理
//============================
CBullet* CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CBullet* pBullet;

	pBullet = new CBullet;

	pBullet->SetType(TYPE::D2DBULLET);
	pBullet->CObject2D::SetPos(pos);
	pBullet->m_rot = rot;
	pBullet->m_nLife = BULLET_LIFE;

	//移動量を設定
	pBullet->m_nMove.x = sinf(D3DX_PI * (pBullet->m_rot.z / D3DX_PI) - D3DX_PI * 0.5f) * BULLET_SPD;
	pBullet->m_nMove.y = cosf(D3DX_PI * (pBullet->m_rot.z / D3DX_PI) - D3DX_PI * 0.5f) * BULLET_SPD;

	//弾の初期化
	pBullet->Init();

	LPDIRECT3DTEXTURE9 pTexture;

	//テクスチャの読み込む
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(),"data\\TEXTURE\\bullet_yellow.png",&pTexture);

	//テクスチャの設定
	pBullet->BindTexture(pTexture);

	return pBullet;
}