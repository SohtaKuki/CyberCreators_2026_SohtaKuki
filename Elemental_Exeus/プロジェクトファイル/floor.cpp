//=================================================
//
// 床の描画処理 (floor.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "floor.h"

//======================
// コンストラクタ
//======================
CFloor::CFloor(int nPriority) : CModel(nPriority)
{

}

//======================
// デストラクタ
//======================
CFloor::~CFloor()
{

}

HRESULT CFloor::Init()
{
	CRenderer* Renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	// 頂点バッファの作成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_OBJECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr)))
	{
		return E_FAIL;
	}

	VERTEX_3D* pVtx;

	// 頂点データの設定
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int row = static_cast<int>(sqrt(MAX_OBJECT)); // 四角形ポリゴンの行数
	int col = row; // 列数
	float spacing = m_nSize.x * 1.0f / 10; // ポリゴン間の距離

	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			int index = (i * col + j) * 4;

			// 各四角形の中心位置を計算
			float centerX = j * spacing - (col / 2.0f) * spacing;
			float centerZ = i * spacing - (row / 2.0f) * spacing;

			// 頂点座標
			pVtx[index + 0].pos = D3DXVECTOR3(centerX - (m_nSize.x / 10), 0.0f, centerZ + (m_nSize.z / 10));
			pVtx[index + 1].pos = D3DXVECTOR3(centerX + (m_nSize.x / 10), 0.0f, centerZ + (m_nSize.z / 10));
			pVtx[index + 2].pos = D3DXVECTOR3(centerX - (m_nSize.x / 10), 0.0f, centerZ - (m_nSize.z / 10));
			pVtx[index + 3].pos = D3DXVECTOR3(centerX + (m_nSize.x / 10), 0.0f, centerZ - (m_nSize.z / 10));

			// 法線ベクトル
			for (int k = 0; k < 4; ++k)
			{
				pVtx[index + k].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				pVtx[index + k].col = D3DCOLOR_RGBA(255, 255, 255, 255); // 白
			}

			// テクスチャ座標
			pVtx[index + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[index + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[index + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[index + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}

	m_pVtxBuff->Unlock();

	return S_OK;
}




//======================
// 終了処理
//======================
void CFloor::Uninit()
{
	CObject3D::Uninit();
}

//======================
// 更新処理
//======================
void CFloor::Update()
{



	int nFadeState = CFade::GetFadeState();

	if (nFadeState == CFade::FADE_OUT)
	{
		CFloor::Uninit();
	}
}

void CFloor::Draw()
{
	CRenderer* Renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	// ワールドマトリックス設定
	D3DXMATRIX mtxRot, mtxTrans;
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転と位置の適用
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	D3DXMatrixTranslation(&mtxTrans, m_nPos.x, m_nPos.y, m_nPos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	pDevice->SetFVF(FVF_VERTEX_3D);
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
	pDevice->SetTexture(0, m_pTexBuff);

	// ポリゴンごとに描画
	for (int i = 0; i < MAX_OBJECT; ++i)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	}
}

//======================
// オブジェクト生成処理
//======================
CFloor* CFloor::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size,int nType)
{
	CFloor* Floor = nullptr;

	Floor = new CFloor;

	Floor->m_nPos = pos;

	Floor->m_nSize = size;

	//初期化に成功した場合
	if (SUCCEEDED(Floor->Init()))
	{
		//テクスチャ読み込み選別
		if (nType == 0)
		{
			Floor->SetType(TYPE::FLOOR);
			Floor->Load("data\\TEXTURE\\dark_tile.png");
		}

		else if (nType == 1)
		{
			Floor->SetType(TYPE::FLOOR);
			Floor->Load("data\\TEXTURE\\stone_road.jpg");
		}


		//テクスチャの設定
		Floor->BindTexture(Floor->m_pTexBuff);
	}

	return Floor;
}

//======================
// テクスチャロード処理
//======================
HRESULT CFloor::Load(LPCSTR textureFileName)
{
	LPDIRECT3DDEVICE9 pDevice = nullptr;
	pDevice = CManager::GetRenderer()->GetDevice();

	if (FAILED(D3DXCreateTextureFromFile(pDevice, textureFileName, &m_pTexBuff)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//======================
// テクスチャアンロード(終了)処理
//======================
void CFloor::Unload()
{
	//if (m_pTexBuff != nullptr)
	//{
	//	m_pTexBuff->Release();
	//	m_pTexBuff = nullptr;
	//}
}

//===========================
// 床の当たり判定
//===========================
bool CFloor::Collision3DFloor(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fHeight)
{
	bool bLanding = false; // 重力を適応した場合のみ使用

	D3DXVECTOR3 Pos = CObject3D::GetPos();

	// 右側当たり判定
	if (pPos->x - fWidth <= Pos.x + m_nSize.x && pPosOld->x - fWidth >= Pos.x + m_nSize.x && pPos->z - fHeight < Pos.z + m_nSize.z && pPos->z > Pos.z - m_nSize.z && pPos->y < Pos.y+ m_nSize.y && pPos->y > Pos.y- m_nSize.y)
	{
		pPos->x = Pos.x + m_nSize.x + fWidth;
	}
	// 左側当たり判定
	else if (pPos->x + fWidth >= Pos.x - m_nSize.x && pPosOld->x + fWidth <= Pos.x - m_nSize.x && pPos->z - fHeight < Pos.z + m_nSize.z && pPos->z > Pos.z - m_nSize.z && pPos->y < Pos.y+ m_nSize.y && pPos->y > Pos.y- m_nSize.y)
	{
		pPos->x = Pos.x - m_nSize.x - fWidth;
	}
	// 前側当たり判定
	if (pPos->x - fWidth < Pos.x + m_nSize.x && pPos->x + fWidth > Pos.x - m_nSize.x && pPos->z - fHeight <= Pos.z + m_nSize.z && pPosOld->z - fHeight >= Pos.z + m_nSize.z && pPos->y < Pos.y+ m_nSize.y && pPos->y > Pos.y- m_nSize.y)
	{
		pPos->z = Pos.z + m_nSize.z + fHeight;
	}
	// 後側当たり判定
	else if (pPos->x - fWidth < Pos.x + m_nSize.x && pPos->x + fWidth > Pos.x - m_nSize.x && pPos->z >= Pos.z - m_nSize.z && pPosOld->z <= Pos.z - m_nSize.z && pPos->y < Pos.y+ m_nSize.y && pPos->y > Pos.y- m_nSize.y)
	{
		pPos->z = Pos.z - m_nSize.z - fHeight;

	}

	// y座標上側当たり判定
	if (pPos->x - fWidth < Pos.x + m_nSize.x && pPos->x + fWidth > Pos.x - m_nSize.x && pPos->y - fHeight <= Pos.y+ m_nSize.y && pPosOld->y - fHeight >= Pos.y+ m_nSize.y && pPos->z < Pos.z + m_nSize.z && pPos->z > Pos.z - m_nSize.z)
	{
		pPos->y = Pos.y+ m_nSize.y + fHeight;
		bLanding = true;
	}

	return bLanding;
}
