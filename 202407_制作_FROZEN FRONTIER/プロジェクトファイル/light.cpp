//=================================================
//
// ライト処理 (light.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "light.h"
#include "main.h"
#include "object.h"


CLight::CLight()
{
	//ライトの情報をクリアする
	ZeroMemory(&m_alight, sizeof(D3DLIGHT9));
}

CLight::~CLight()
{

}

//======================
// 初期化処理
//======================
HRESULT CLight::Init()
{
	CRenderer* Renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	D3DXVECTOR4 vecDir[MAX_LIGHT];

	m_alight->Type = D3DLIGHT_DIRECTIONAL;

	m_alight->Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		switch (nCnt)
		{
		case 0:
			// ライトの方向を設定
			vecDir[nCnt] = D3DXVECTOR4(0.2f, -0.8f, -0.4f, 0.0f); 
			break;
		case 1:
			// ライトの方向を設定
			vecDir[nCnt] = D3DXVECTOR4(0.5f, 0.6f, 0.7f, 0.0f);
			break;
		case 2:
			// ライトの方向を設定
			vecDir[nCnt] = D3DXVECTOR4(0.9f, 0.5f, 0.6f, 0.0f);
			break;
		case 3:
			// ライトの方向を設定
			vecDir[nCnt] = D3DXVECTOR4(-0.9f, 0.5f, 0.5f, 0.0f); 
			break;
		}
	}

	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		// ライトの方向ベクトルを正規化
		D3DXVECTOR3 normalizedDir(vecDir[nCnt].x, vecDir[nCnt].y, vecDir[nCnt].z); // D3DXVECTOR3に変換して正規化
		D3DXVec3Normalize(&normalizedDir, &normalizedDir);

		// 正規化されたベクトルをD3DXVECTOR4に戻す
		vecDir[nCnt] = D3DXVECTOR4(normalizedDir.x, normalizedDir.y, normalizedDir.z, 0.0f); // 再度D3DXVECTOR4に格納

		// ライトの方向を設定
		m_alight->Direction = D3DXVECTOR3(vecDir[nCnt].x, vecDir[nCnt].y, vecDir[nCnt].z);

		// ライトを設定
		pDevice->SetLight(nCnt, m_alight);

		// ライトを有効にする
		pDevice->LightEnable(nCnt, TRUE);
	}

	return S_OK;
}

//======================
// 終了処理
//======================
void CLight::Uninit()
{

}

//======================
// 更新処理
//======================
void CLight::Update()
{

}
