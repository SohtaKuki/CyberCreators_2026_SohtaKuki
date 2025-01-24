//=================================================
//
// カメラ処理 (camera.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "camera.h"
#include "manager.h"
#include "object.h"
#include "3dplayer.h"
#include "3dgoalobj.h"
#include "bosscallui.h"
#include "3dboss.h"

int CCamera::m_nShakeframe = 0;
float CCamera::m_fShake = 0.0f;

//======================
// コンストラクタ
//======================
CCamera::CCamera()
{

}

//======================
// デストラクタ
//======================
CCamera::~CCamera()
{

}

//======================
// 初期化処理
//======================
HRESULT CCamera::Init()
{
	m_posV = D3DXVECTOR3(220.0f, 10.0f, 1250.0f);
	m_posR = D3DXVECTOR3(220.0f ,110.0f, -680.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fDistance = sqrtf((0.0f * 0.0f * 100.0f) + (m_posV.z * m_posV.z));
	m_fStartPos = 10.0f;
	m_fEndPos = 10000.0f;
	m_fFogDisity = 0.0000f;
	m_fBlizCnt = 0.00055f;
	m_bBlizSwitch = false;

	return S_OK;
}

//======================
// 終了処理
//======================
void CCamera::Uninit()
{

}

//======================
// 更新処理
//======================
void CCamera::Update()
{
	if (C3dgoalobj::GetStageNum() == 0 || C3dgoalobj::GetStageNum() == 1)
	{
		CObject* pObj = CObject::GetObj(3, 1);

		if (pObj != nullptr)
		{
			CObject::TYPE type = pObj->GetType();

			C3dplayer* p3dPlayer = (C3dplayer*)pObj;

			//D3DXVECTOR3 PlayerPos = p3dPlayer->GetPos();

			if (C3dgoalobj::GetStageNum() == 0)
			{
				//プレイヤーにカメラを追従する
				m_posV.x = p3dPlayer->GetPos().x + 220.0f;
				m_posR.x = p3dPlayer->GetPos().x + 220.0f;

				//プレイヤーのy座標に合わせてカメラを追従する
				if (p3dPlayer->GetPos().y >= 320.0f)
				{
					m_posV.y = p3dPlayer->GetPos().y - 155.0f;
					m_posR.y = p3dPlayer->GetPos().y - 150.0f;
					m_posV.z = p3dPlayer->GetPos().z + 1250.0f;
					m_posR.z = p3dPlayer->GetPos().z - 400.0f;
				}

				//既定の高さ以下の場所にいた場合
				else
				{
					m_posV.y = 195.0f;
					m_posR.y = 190.0f;
					m_posV.z = p3dPlayer->GetPos().z + 1250.0f;
					m_posR.z = p3dPlayer->GetPos().z - 400.0f;
				}
			}

			//ステージ1の場合
			if (C3dgoalobj::GetStageNum() == 1)
			{
				//プレイヤーにカメラを追従する
				m_posV.x = p3dPlayer->GetPos().x + 220.0f;
				m_posR.x = p3dPlayer->GetPos().x + 220.0f;

				//プレイヤーのy座標に合わせてカメラを追従する
				if (p3dPlayer->GetPos().y >= 410.0f)
				{
					m_posV.y = p3dPlayer->GetPos().y - 15.0f;
					m_posR.y = p3dPlayer->GetPos().y - 0.0f;
					m_posV.z = p3dPlayer->GetPos().z + 1250.0f;
					m_posR.z = p3dPlayer->GetPos().z - 400.0f;
				}

				//既定の高さ以下の場所にいた場合
				else
				{
					m_posV.y = 195.0f;
					m_posR.y = 190.0f;
					m_posV.z = p3dPlayer->GetPos().z + 1250.0f;
					m_posR.z = p3dPlayer->GetPos().z - 400.0f;
				}
			}
		}
	}

	if (C3dgoalobj::GetStageNum() == 2)
	{
		//ボス出現前、出現後のカメラ視点
		if (C3dgoalobj::GetStageNum() == 2 && CBossCallUI::GetEndStat() == false && C3dboss::GetBossEntry() == false || 
			C3dgoalobj::GetStageNum() == 2 && CBossCallUI::GetEndStat() == true && C3dboss::GetBossEntry() == true)
		{

			m_posV = D3DXVECTOR3(270.0f, 280.0f, 1400.0f);
			m_posR = D3DXVECTOR3(270.0f, 275.0f, -680.0f);

			m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		//出現中のカメラ視点
		if (C3dgoalobj::GetStageNum() == 2 && CBossCallUI::GetEndStat() == true && C3dboss::GetBossEntry() == false)
		{
			m_posV = D3DXVECTOR3(338.0f, 80.0f, -13.0f);
			m_posR = D3DXVECTOR3(368.0f, C3dboss::GetBossPosCamera().y + 67.5f, -13.0f);

			m_rot.y = 1.57f;

		}
	}









	//if (CManager::GetKeyboard()->GetPress(DIK_C))
	//{
	//	m_rot.y += 6.0f;
	//}

	//if (m_fFogDisity > 0.001f)
	//{
	//	m_bBlizSwitch = true;
	//}

	//if (m_fFogDisity < 0.00075f)
	//{
	//	m_bBlizSwitch = false;
	//}
	//
	//if (m_bBlizSwitch == false)
	//{
	//	m_fBlizCnt += 0.000005f;
	//}

	//if (m_bBlizSwitch == true)
	//{
	//	m_fBlizCnt -= 0.000005f;
	//}

	//m_fFogDisity += m_fBlizCnt;

	m_posV.x = m_posR.x - sinf(m_rot.y) * 500;
	m_posV.z = m_posR.z - cosf(m_rot.y) * 500;
}

//======================
// カメラの振動処理
//======================
void CCamera::SetShake(int shakeframe, float shake)
{
	m_nShakeframe = shakeframe;
	m_fShake = shake;
}


//======================
// カメラの設定処理
//======================
void CCamera::SetCamera()
{
	CRenderer* Renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	//プロジェクションマトリックスを作成
	//D3DXMatrixOrthoLH(&m_mtxProjection, (float)SCREEN_WIDTH ,(float)SCREEN_HEIGHT, 10.0f, 1800.0f);

	D3DXMatrixPerspectiveFovLH(&m_mtxProjection, D3DXToRadian(30.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, 12000.0f);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	D3DXVECTOR3 adjust = D3DXVECTOR3(0, 0, 0);

	D3DXVECTOR3 PosV = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 PosR = D3DXVECTOR3(0, 0, 0);

	if (m_nShakeframe > 0)
	{
		m_nShakeframe--;
		adjust = D3DXVECTOR3(rand() & (int)m_fShake,rand() & (int)m_fShake, rand() & (int)m_fShake);
	}

	PosV = m_posV + adjust;
	PosR = m_posR + adjust;

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView, &PosV, &PosR, &m_vecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	////フォグ関係の処理
	//pDevice->SetRenderState(D3DRS_FOGENABLE,TRUE);

	//pDevice->SetRenderState(D3DRS_FOGCOLOR, 0x00FFFFFF);

	//pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_EXP);

	//pDevice->SetRenderState(D3DRS_FOGDENSITY,*(DWORD*)(&m_fBlizCnt));

	//pDevice->SetRenderState(D3DRS_SHADEMODE, 2);

	//pDevice->SetRenderState(D3DRS_DEPTHBIAS, 1);

	//pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	//フォグ関係の処理(距離計算式　今は使わない)
	//pDevice->SetRenderState(D3DRS_FOGSTART,*(DWORD*)(&m_fStartPos));
	//pDevice->SetRenderState(D3DRS_FOGEND,*(DWORD*)(&m_fEndPos));


}


