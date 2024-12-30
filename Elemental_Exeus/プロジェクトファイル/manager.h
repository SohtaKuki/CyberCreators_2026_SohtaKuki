//=================================================
//
// ポリゴン描画のマネージャー管理 (manager.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "scene.h"
#include "fade.h"
#include "sound.h"

//マネージャクラス
class CManager
{
public:
	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit();
	void Update();
	void Draw();
	static CRenderer* GetRenderer();
	static CInputKeyboard* GetKeyboard();
	static CInputJoypad* GetJoypad();
	static CCamera* GetCamera();
	static CLight* GetLight();
	static CFade* GetFade();
	static CSound* GetSound();
	static void SetMode(CScene::MODE mode);
private:
	static CRenderer* m_pRenderer;
	static CInputKeyboard* m_pKeyboard;
	static CInputJoypad* m_pJoypad;
	static CCamera* m_pCamera;
	static CSound* m_pSound;
	static CLight* m_pLight;
	static CScene* m_pScene;
	static CFade* m_pFade;
};

#endif