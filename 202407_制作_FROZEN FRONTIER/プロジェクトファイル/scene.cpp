//=================================================
//
// 画面遷移処理 (scene.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "scene.h"
#include "objbg.h"
#include "stagemanager.h"
#include "floor.h"
#include "timer.h"
#include "playerhpbar.h"
#include "result.h"
#include "object2D.h"
#include "result.h"
#include "titlebg.h"
#include "gameui.h"
#include "score.h"
#include "resultscore.h"
#include "3dchargeshotui.h"
#include "3daddlifeui.h"
#include "3dbuffui.h"
#include "scorewindow.h"
#include "playerwindow.h"
#include "blizardfloor.h"
#include "stageselect.h"
#include "pause.h"
#include "startcallui.h"
#include "endcallui.h"
#include "tutorialui.h"
#include "stageselectbg.h"

bool CScene::bUpdate = {};
bool CScene::m_bScreenSwitch = false;

//======================
//コンストラクタ
//======================
CScene::CScene()
{
	m_nScreenFadeTime = 0;
	m_bScreenSwitch = false;
}

//======================
//デストラクタ
//======================
CScene::~CScene()
{

}

//======================
//初期化処理
//======================
HRESULT CScene::Init()
{
	bUpdate = false;
	return S_OK;
}

//======================
//終了処理
//======================
void CScene::Uninit()
{

	CObject::ReleaseAll();
}

//======================
//更新処理
//======================
void CScene::Update()
{

}

//======================
//描画処理
//======================
void CScene::Draw()
{

}

//======================
// オブジェクト生成処理
//======================
CScene* CScene::Create(MODE mode)
{
	CScene* Scene = nullptr;

	switch (mode)
	{
	case MODE_TITLE:
		Scene = new CTitle();
		break;
	case MODE_STAGESELECT:
		Scene = new CStageSelectSc();
		break;
	case MODE_GAME:
		Scene = new CGame;
		break;
	case MODE_RESULT:
		Scene = new CResult;
		break;
	default:
		break;

	}

	//シーンの初期化
	if (Scene != nullptr)
	{
		Scene->Init();

		return Scene;
	}


	return nullptr;
}

//=====================================
// 一部オブジェクトの更新をするかどうか
//=====================================
void CScene::UpdateSwitch(int nType)
{
	if (nType == 0)
	{
		bUpdate = false;
	}

	if (nType == 1)
	{
		bUpdate = true;
	}
}

//======================
//コンストラクタ
//======================
CGame::CGame()
{

}

//======================
//デストラクタ
//======================
CGame::~CGame()
{

}

//======================
//初期化処理
//======================
HRESULT CGame::Init()
{
	CScene::Init();

	CTimer::Create(D3DXVECTOR3(500.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	CScore::Create(D3DXVECTOR3(1030.0f, 53.0f, 0.0f),D3DXVECTOR3(33.0f, 33.0f, 0.0f));
	CPlayerHpBar::Create(D3DXVECTOR3(110.0f, 55.0f, 0.0f), D3DXVECTOR3(-25.0f, -30.0f, 0.0f));
	CGameui::Create(D3DXVECTOR3(640.0f, 0.0f, 0.0f), D3DXVECTOR3(100.0f, 140.0f, 0.0f));


	//ステージごとに生成する処理を変える
	if (CStageSelect::GetStageSelect() == 0)
	{
		CStageManager::Create(0);
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_STAGE_TUTORIAL);
		CTutorialUI::Create();
	}

	if (CStageSelect::GetStageSelect() == 1)
	{
		CStageManager::Create(1);
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_STAGE_1);
	}

	if (CStageSelect::GetStageSelect() == 2)
	{
		CStageManager::Create(2);
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_STAGE_2);
	}

	CChargeshotUI::Create(D3DXVECTOR3(-100.0f, 120.0f, 0.0f), D3DXVECTOR3(150.0f, 150.0f, 0));
	CAddlifeui::Create(D3DXVECTOR3(-100.0f, 220.0f, 0.0f), D3DXVECTOR3(150.0f, 150.0f, 0));
	CBuffUI::Create(D3DXVECTOR3(70.0f, 90.0f, 0.0f), D3DXVECTOR3(40.0f, 80.0f, 0));
	CPlayerWindow::Create(D3DXVECTOR3(285.0f, -70.0f, 0.0f), D3DXVECTOR3(280.0f, 220.0f, 0.0f));
	CScoreWindow::Create(D3DXVECTOR3(1150.0f, -20.0f, 0.0f), D3DXVECTOR3(180.0f, 180.0f, 0.0f));
	CStartCallUI::Create(D3DXVECTOR3(640.0f, 160.0f, 0.0f), D3DXVECTOR3(180.0f, 150.0f, 0));
	CEndCallUI::Create(D3DXVECTOR3(640.0f, -100.0f, 0.0f), D3DXVECTOR3(180.0f, 150.0f, 0));
	CPauseSelect::Create();

	return S_OK;
}

//======================
//終了処理
//======================
void CGame::Uninit()
{
	CScene::Uninit();
	CManager::GetSound()->Stop();
}

//======================
//更新処理
//======================
void CGame::Update()
{
	//ポーズ画面にてゲーム画面に戻る場合
	if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) && CPauseSelect::GetPauseSelect() == 0 || CManager::GetJoypad()->GetTrigger(CInputJoypad::JOYKEY_A) && CPauseSelect::GetPauseSelect() == 0)
	{
		CPauseSelect::bUseSwitch(0);
	}

	//ポーズ画面にてステージセレクトに戻る場合
	else if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) && CPauseSelect::GetPauseSelect() == 1 || CManager::GetJoypad()->GetTrigger(CInputJoypad::JOYKEY_A) && CPauseSelect::GetPauseSelect() == 1 )
	{
		CManager::GetFade()->SetFade(CScene::MODE_STAGESELECT);
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
	}

	//ポーズ画面にてタイトル画面に戻る場合
	else if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) && CPauseSelect::GetPauseSelect() == 2 || CManager::GetJoypad()->GetTrigger(CInputJoypad::JOYKEY_A) && CPauseSelect::GetPauseSelect() == 2 && CPauseSelect::GetbUseStat() == true)
	{
		CManager::GetFade()->SetFade(CScene::MODE_TITLE);
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
	}
}

//======================
//描画処理
//======================
void CGame::Draw()
{

}

//======================
//コンストラクタ
//======================
CResult::CResult()
{

}

//======================
//デストラクタ
//======================
CResult::~CResult()
{

}

//======================
//初期化処理
//======================
HRESULT CResult::Init()
{
	CResultBG::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));
	CResultScore::Create(D3DXVECTOR3(150.0f, 378.0f, 0.0f), D3DXVECTOR3(100.0f, 100.0f, 0.0f));
	CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_RESULT);

	return S_OK;
}

//======================
//終了処理
//======================
void CResult::Uninit()
{
	
	CScene::Uninit();
}

//======================
//更新処理
//======================
void CResult::Update()
{
	//エンターキーが押されたとき
	if (CManager::GetKeyboard()->GetTrigger(DIK_Q) || CManager::GetJoypad()->GetTrigger(CInputJoypad::JOYKEY_B))
	{
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
		CManager::GetFade()->SetFade(CScene::MODE_GAME);
		CManager::GetSound()->Stop(CSound::SOUND_LABEL_BGM_RESULT);
	}

	//エンターキーが押されたとき
	if (CManager::GetKeyboard()->GetTrigger(DIK_P) || CManager::GetJoypad()->GetTrigger(CInputJoypad::JOYKEY_RB))
	{
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
		CManager::GetFade()->SetFade(CScene::MODE_STAGESELECT);
		CManager::GetSound()->Stop(CSound::SOUND_LABEL_BGM_RESULT);
	}

	//エンターキーが押されたとき
	if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) || CManager::GetJoypad()->GetTrigger(CInputJoypad::JOYKEY_A))
	{
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
		CManager::GetFade()->SetFade(CScene::MODE_TITLE);
		CManager::GetSound()->Stop(CSound::SOUND_LABEL_BGM_RESULT);
	}
}

//======================
//描画処理
//======================
void CResult::Draw()
{

}

//======================
//コンストラクタ
//======================
CTitle::CTitle()
{

}

//======================
//デストラクタ
//======================
CTitle::~CTitle()
{

}

//======================
//初期化処理
//======================
HRESULT CTitle::Init()
{
	CTitleBG::Create();
	//CObjectBG::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));

	return S_OK;
}

//======================
//終了処理
//======================
void CTitle::Uninit()
{
	CManager::GetSound()->Stop(CSound::SOUND_LABEL_BGM);
	CScene::Uninit();
}

//======================
//更新処理
//======================
void CTitle::Update()
{
	if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) || CManager::GetJoypad()->GetTrigger(CInputJoypad::JOYKEY_A))
	{
		if (m_bScreenSwitch == false)
		{
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_STARTBTN);
			m_bScreenSwitch = true;
		}
	}

	if (m_bScreenSwitch == true)
	{
		m_nScreenFadeTime++;
	}

	if (m_nScreenFadeTime == 60)
	{
		CManager::GetFade()->SetFade(CScene::MODE_STAGESELECT);
		m_bScreenSwitch = false;
		m_nScreenFadeTime = 0;
	}

}

//======================
//描画処理
//======================
void CTitle::Draw()
{

}

//======================
//コンストラクタ
//======================
CStageSelectSc::CStageSelectSc()
{

}

//======================
//デストラクタ
//======================
CStageSelectSc::~CStageSelectSc()
{

}

//======================
//初期化処理
//======================
HRESULT CStageSelectSc::Init()
{
	CStageSelectBG::Create();
	CStageSelect::Create();

	return S_OK;
}

//======================
//終了処理
//======================
void CStageSelectSc::Uninit()
{

	CScene::Uninit();
}

//======================
//更新処理
//======================
void CStageSelectSc::Update()
{

	if (CManager::GetKeyboard()->GetTrigger(DIK_BACKSPACE) || CManager::GetJoypad()->GetTrigger(CInputJoypad::JOYKEY_B))
	{
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_STARTBTN);
		CManager::GetFade()->SetFade(CScene::MODE_TITLE);
	}

	//エンターキーが押されたとき
	if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) || CManager::GetJoypad()->GetTrigger(CInputJoypad::JOYKEY_A))
	{
		//if (m_bScreenSwitch == false)
		//{
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);

	//		m_bScreenSwitch = true;
	//	}
	//}

	//if (m_bScreenSwitch == true)
	//{
	//	m_nScreenFadeTime++;
	//}

	//if (m_nScreenFadeTime == 40)
	//{
		CManager::GetSound()->Stop(CSound::SOUND_LABEL_BGM_STAGE_SELECT);
		CManager::GetFade()->SetFade(CScene::MODE_GAME);
	//	m_bScreenSwitch = false;
	//	m_nScreenFadeTime = 0;
	}
}

//======================
//描画処理
//======================
void CStageSelectSc::Draw()
{

}
