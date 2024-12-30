//=================================================
//
// 2Dポリゴンの描画処理 (object2D.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "enemy.h"
#include "bullet.h"
#include "manager.h"

LPDIRECT3DTEXTURE9 CEnemy::m_pTexTemp = nullptr;

//======================
// コンストラクタ
//======================
CEnemy::CEnemy()
{
	m_nLife = 4;
	m_PolygonMoveSpeed = 1.0f;

}
//======================
// デストラクタ
//======================
CEnemy::~CEnemy()
{

}

//======================
// 初期化処理
//======================
HRESULT CEnemy::Init()
{
	//初期化
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//======================
// 終了処理
//======================
void CEnemy::Uninit()
{
	Unload();
	CObject2D::Uninit();
}

//======================
// 更新処理
//======================
void CEnemy::Update()
{
	int nCntExplosion;
	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロック
	CObject2D::GetBuff()->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 Pos = CObject2D::GetPos();

	pVtx[0].pos = D3DXVECTOR3(Pos.x - m_nEnemySize.x, Pos.y - m_nEnemySize.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(Pos.x + m_nEnemySize.x, Pos.y - m_nEnemySize.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(Pos.x - m_nEnemySize.x, Pos.y + m_nEnemySize.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(Pos.x + m_nEnemySize.x, Pos.y + m_nEnemySize.y, 0.0f);

	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	Pos.x += m_moveEnemy.x;
	Pos.y += m_moveEnemy.y;

	SetPos(Pos);

	//移動量を更新
	m_moveEnemy.x += (Length_value2 - m_moveEnemy.x) * Attenuation_value;
	m_moveEnemy.y += (Length_value2 - m_moveEnemy.y) * Attenuation_value;

}

//======================
// 描画処理
//======================
void CEnemy::Draw()
{
	CObject2D::Draw();
}

//======================
// オブジェクト生成処理
//======================
CEnemy* CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CEnemy* enemy = nullptr;

	enemy = new CEnemy;

	if (SUCCEEDED(enemy->Init()))
	{
		enemy->SetType(TYPE::ENEMY);

		enemy->Load();

		enemy->CObject2D::SetPos(pos);

		enemy->m_nEnemySize = size;

		//テクスチャの設定
		enemy->BindTexture(m_pTexTemp);
	}

	return enemy;
}

//======================
// ダメージ処理
//======================
void CEnemy::Damage()
{
	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		CObject* pObj = CObject::GetObj(3,nCntObj);

		if (pObj != nullptr)
		{
			CObject::TYPE type = pObj->GetType();
			if (type == CObject::TYPE::ENEMY)
			{
				m_nLife -= 1;

				VERTEX_2D* pVtx;

				CObject2D::GetBuff()->Lock(0, 0, (void**)&pVtx, 0);
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				pVtx += 4;

				CObject2D::GetBuff()->Unlock();
			}

			if (m_nLife == 0)
			{
				CObject2D::Uninit();
			}
		}
	}
}

//======================
// テクスチャロード処理
//======================
HRESULT CEnemy::Load()
{
	LPDIRECT3DDEVICE9 pDevice = nullptr;
	pDevice = CManager::GetRenderer()->GetDevice();

	if (FAILED(D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\enemy000.png", &m_pTexTemp)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//======================
// テクスチャアンロード(終了)処理
//======================
void CEnemy::Unload()
{
	if (m_pTexTemp != nullptr)
	{
		m_pTexTemp->Release();
		m_pTexTemp = nullptr;
	}
}

void CEnemy::SetPos(D3DXVECTOR3 pos)
{
	CEnemy* enemy = nullptr;
}
