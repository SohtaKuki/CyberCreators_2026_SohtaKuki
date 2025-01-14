//=================================================
//
// pause.cppの処理に必要なヘッダーファイル (pause.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _TUTORIALUI_H_
#define _TUTORIALUI_H_

#include "object2D.h"

#define PAUSE_SELECT_NUM	(5)

class CTutorialUI : public CObject2D
{
public:
	typedef enum
	{
		TUTORIAL_UI_1 = 0,
		TUTORIAL_UI_2,
		TUTORIAL_UI_3,
		TUTORIAL_UI_4,
		TUTORIAL_UI_5,
		TUTORIAL_UI_MAX,
	}TUTORIAL_UI;

	//プロトタイプ宣言
	CTutorialUI(int nPriority = 6);
	~CTutorialUI();
	HRESULT Init() override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	static int GetTutorialUI();
	static CTutorialUI* Create();
private:
	//グローバル変数
	LPDIRECT3DTEXTURE9 m_pTexturePauseSelect[PAUSE_SELECT_NUM] = {};
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffPauseSelect = NULL;

	static int m_nPauseSelect;
};

#endif

