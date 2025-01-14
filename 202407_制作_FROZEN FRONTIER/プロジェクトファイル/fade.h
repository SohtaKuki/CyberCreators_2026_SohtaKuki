//=================================================
//
// 画面遷移時のフェード処理 (fade.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _FADE_H_
#define _FADE_H_

#include "scene.h"

//ゲームクラス
class CFade
{
public:
	static const int FADE_TIME = 90;
	static const int FADE_SPEED = 15;

	//フェード処理の列挙型
	typedef enum
	{
		FADE_NONE = 0,
		FADE_OUT,	
		FADE_IN,		
		FADE_MAX,		
	}FADE;

	CFade();	
	~CFade();	
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void SetAlpha(int nAlpha);
	static void SetFade(CScene::MODE mode);
	static int GetFadeState() {return m_FadeState ; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	int m_nAlpha;						
	static FADE m_FadeState;			
	static bool m_bEndFade;				
	static CScene::MODE m_Mode;			
};


#endif