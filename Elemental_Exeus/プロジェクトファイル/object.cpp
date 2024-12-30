//=================================================
//
// オブジェクトの処理 (object.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "object.h"

unsigned const int MAX_OBJECT = 1028;	// オブジェクトの最大数
int CObject::m_nNumAll = 0;				// オブジェクト総数
CObject* CObject::m_apObject[MAX_PRIORITY][MAX_OBJECT] = {};	// オブジェクト管理

//======================
// コンストラクタ
//======================
CObject::CObject(int nPriority)
{
	m_nPriority = nPriority;

	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	{
		if (m_apObject[m_nPriority][nCnt] == nullptr)
		{
			m_apObject[m_nPriority][nCnt] = this;	// 自身を代入
			m_nID = nCnt;	// IDを代入
			m_nNumAll++; // オブジェクト総数を増やす

			break;
		}
	}
}

//======================
// デストラクタ
//======================
CObject::~CObject()
{

}

//======================
// 初期化処理
//======================
HRESULT CObject::Init()
{
	return S_OK;
}

//======================
// 終了処理
//======================
void CObject::Uninit()
{

}

//======================
// 更新処理
//======================
void CObject::Update()
{

}

//======================
// 描画処理
//======================
void CObject::Draw()
{
	
}

//======================
// 解放処理
//======================
void CObject::Release()
{
	int nID = m_nID;

	int nPriority = m_nPriority;
	
	if (m_apObject[nID] != nullptr)
	{
		delete m_apObject[nPriority][nID];
		m_apObject[nPriority][nID] = nullptr;
		m_nNumAll--;
	}

	
}

//======================
// 全開放処理
//======================
void CObject::ReleaseAll()
{
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY; nCntPriority++)
	{

		for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
		{
			if (m_apObject[nCntPriority][nCnt] != nullptr)
			{
				m_apObject[nCntPriority][nCnt]->Release();
				m_apObject[nCntPriority][nCnt] = nullptr;
			}
		}
	}
}

//======================
// 全更新処理
//======================
void CObject::UpdateAll()
{
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY; nCntPriority++)
	{
		for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
		{
			if (m_apObject[nCntPriority][nCnt] != nullptr)
			{
				m_apObject[nCntPriority][nCnt]->Update();
			}
		}
	}
}

//======================
// 全描画処理
//======================
void CObject::DrawAll()
{
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY; nCntPriority++)
	{
		for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
		{
			if (m_apObject[nCntPriority][nCnt] != nullptr)
			{
				m_apObject[nCntPriority][nCnt]->Draw();
			}
		}
	}
}

//=====================
// オブジェクト取得
//=====================
CObject * CObject::GetObj(int nPriority ,int nIdx)
{
	return m_apObject[nPriority][nIdx];
}

//=====================
// タイプを代入する処理
//=====================
void CObject::SetType(TYPE type)
{
	m_type = type;
}
