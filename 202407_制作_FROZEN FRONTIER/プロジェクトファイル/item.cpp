//=================================================
//
// アイテムの処理 (item.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "item.h"
#include "manager.h"
#include "object2D.h"
#include "player.h"
#include "explosion.h"
#include "enemy.h"
#include "block.h"

//============================
//コンストラクタ
//============================
CItem::CItem(int nPriority) : CPlayer(nPriority)
{

}

//============================
//デストラクタ
//============================
CItem::~CItem()
{

}

//============================
//アイテムの初期化処理
//============================
HRESULT CItem::Init()
{
	//初期化
	CObject2D::Init();

	return S_OK;
}

//============================
//アイテムの初期化処理
//============================
void CItem::Uninit()
{
	//初期化
	CObject2D::Uninit();
}

//============================
//アイテムの更新処理
//============================
void CItem::Update()
{
	//弾の移動処理
	CObject2D::GetPos() += m_nMove;

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	CObject2D::GetBuff()->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の更新
	pVtx[0].pos.x = CObject2D::GetPos().x - 50 * 0.5f;
	pVtx[0].pos.y = CObject2D::GetPos().y - 50 * 0.5f;

	pVtx[1].pos.x = CObject2D::GetPos().x + 50 * 0.5f;
	pVtx[1].pos.y = CObject2D::GetPos().y - 50 * 0.5f;

	pVtx[2].pos.x = CObject2D::GetPos().x - 50 * 0.5f;
	pVtx[2].pos.y = CObject2D::GetPos().y + 50 * 0.5f;

	pVtx[3].pos.x = CObject2D::GetPos().x + 50 * 0.5f;
	pVtx[3].pos.y = CObject2D::GetPos().y + 50 * 0.5f;

	//頂点バッファをアンロックする
	CObject2D::GetBuff()->Unlock();

	//アイテムの当たり判定
	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		CObject* pObj = CObject::GetObj(3, nCntObj);

		if (pObj != nullptr)
		{
			CObject::TYPE type = pObj->GetType();

			CPlayer* pPlayer = (CPlayer*)pObj;

			D3DXVECTOR3 PlayerPos = pPlayer->GetPos();

			if (type == CObject::TYPE::PLAYER)
			{
				if (CObject2D::GetPos().x >= PlayerPos.x - PLAYER_HITPOINT
					&& CObject2D::GetPos().x <= PlayerPos.x + PLAYER_HITPOINT
					&& CObject2D::GetPos().y >= PlayerPos.y - PLAYER_HITPOINT
					&& CObject2D::GetPos().y <= PlayerPos.y + PLAYER_HITPOINT)
				{
					CObject2D::Uninit();
					return;
				}
			}

		}
	}
}

//============================
//アイテムの描画処理
//============================
void CItem::Draw()
{
	CObject2D::Draw();
}

//============================
//アイテムの生成処理
//============================
CItem* CItem::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CItem* pItem;

	pItem = new CItem;

	pItem->SetType(TYPE::D2DITEM);
	pItem->CObject2D::SetPos(pos);

	pItem->m_rot = rot;


	//アイテムの初期化
	pItem->Init();

	LPDIRECT3DTEXTURE9 pTexture;

	//テクスチャの読み込む
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "data\\TEXTURE\\item001.png", &pTexture);

	//テクスチャの設定
	pItem->BindTexture(pTexture);

	return pItem;
}