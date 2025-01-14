//=================================================
//
// 2Dポリゴンの描画処理 (object2D.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "block.h"
#include "bullet.h"
#include "manager.h"

LPDIRECT3DTEXTURE9 CBlock::m_pTexTemp = nullptr;

//======================
// コンストラクタ
//======================
CBlock::CBlock(int nPriority) : CObject2D(nPriority)
{
	m_nLife = 4;
	m_PolygonMoveSpeed = 1.0f;

}
//======================
// デストラクタ
//======================
CBlock::~CBlock()
{

}

//======================
// 初期化処理
//======================
HRESULT CBlock::Init()
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
void CBlock::Uninit()
{
	Unload();
	CObject2D::Uninit();
}

//======================
// 更新処理
//======================
void CBlock::Update()
{
	int nCntExplosion;
	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロック
	CObject2D::GetBuff()->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 BlockPos = CObject2D::GetPos();

	pVtx[0].pos = D3DXVECTOR3(BlockPos.x - m_nBlockSize.x, BlockPos.y - m_nBlockSize.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(BlockPos.x + m_nBlockSize.x, BlockPos.y - m_nBlockSize.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(BlockPos.x - m_nBlockSize.x, BlockPos.y + m_nBlockSize.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(BlockPos.x + m_nBlockSize.x, BlockPos.y + m_nBlockSize.y, 0.0f);

	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	BlockPos.x += m_moveBlock.x;
	BlockPos.y += m_moveBlock.y;

	//移動量を更新
	m_moveBlock.x += (Length_value2 - m_moveBlock.x) * Attenuation_value;
	m_moveBlock.y += (Length_value2 - m_moveBlock.y) * Attenuation_value;

}

//======================
// 描画処理
//======================
void CBlock::Draw()
{
	CObject2D::Draw();
}

//======================
// オブジェクト生成処理
//======================
CBlock* CBlock::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size , int nBlockType)
{
	CBlock* block = nullptr;

	block = new CBlock;

	if (SUCCEEDED(block->Init()))
	{
		block->SetType(TYPE::BLOCK);

		block->Load();

		block->CObject2D::SetPos(pos);

		block->m_nBlockSize = size;

		block->m_nBlockType = nBlockType;

		//テクスチャの設定
		block->BindTexture(m_pTexTemp);
	}

	return block;
}

//======================
// ダメージ処理
//======================
void CBlock::Damage()
{
	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		CObject* pObj = CObject::GetObj(3, nCntObj);

		if (pObj != nullptr)
		{
			CObject::TYPE type = pObj->GetType();

			if (type == CObject::TYPE::BLOCK)
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
HRESULT CBlock::Load()
{
	LPDIRECT3DDEVICE9 pDevice = nullptr;
	pDevice = CManager::GetRenderer()->GetDevice();


	if (FAILED(D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\block003.png", &m_pTexTemp)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//======================
// テクスチャアンロード(終了)処理
//======================
void CBlock::Unload()
{
	if (m_pTexTemp != nullptr)
	{
		m_pTexTemp->Release();
		m_pTexTemp = nullptr;
	}
}

void CBlock::SetPos(D3DXVECTOR3 pos)
{
	CBlock* block = nullptr;
}

//===============================
// ブロックの衝突判定
//===============================
bool CBlock::CollisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fHeight)
{

	float fBlockWidth = m_nBlockSize.x;
	float fBlockHeight = m_nBlockSize.y + 50.0f;

	bool bLanding = false;

	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY; nCntPriority++)
	{
		for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
		{
			CObject* pObj = CObject::GetObj(nCntPriority, nCntBlock);

			if (pObj != nullptr)
			{
				CObject::TYPE type = pObj->GetType();

				//オブジェクトタイプがブロックの場合
				if (type == CObject::TYPE::BLOCK)
				{
					//ブロックの種類が普通だった場合
					if (BLOCKTYPE::NORMAL)
					{
						CBlock* pBlock = (CBlock*)pObj;

						D3DXVECTOR3 BlockPos = pBlock->GetPos();

						//右側当たり判定
						if (pPos->x - fWidth <= BlockPos.x + fBlockWidth && pPosOld->x - fWidth >= BlockPos.x + fBlockWidth && pPos->y - fHeight < BlockPos.y + fBlockHeight && pPos->y  > BlockPos.y - fBlockHeight)
						{
							pPos->x = BlockPos.x + fBlockWidth + fWidth;
						}

						//左側当たり判定
						else if (pPos->x + fWidth >= BlockPos.x - fBlockWidth && pPosOld->x + fWidth <= BlockPos.x - fBlockWidth && pPos->y - fHeight < BlockPos.y + fBlockHeight && pPos->y > BlockPos.y - fBlockHeight)
						{
							pPos->x = BlockPos.x - fBlockWidth - fWidth;
						}

						//下側当たり判定
						if (pPos->x - fWidth < BlockPos.x + fBlockWidth && pPos->x + fWidth > BlockPos.x - fBlockWidth && pPos->y - fHeight <= BlockPos.y + fBlockHeight && pPosOld->y - fHeight >= BlockPos.y + fBlockHeight)
						{
							pPos->y = BlockPos.y + fBlockHeight + fHeight;
						}

						//上側当たり判定
						else if (pPos->x - fWidth < BlockPos.x + fBlockWidth && pPos->x + fWidth > BlockPos.x - fBlockWidth && pPos->y >= BlockPos.y - fBlockHeight && pPosOld->y <= BlockPos.y - fBlockHeight)
						{
							pPos->y = BlockPos.y - fBlockHeight;
							bLanding = true;
						}
					}
				}
			}
		}
	}
	return bLanding;
}