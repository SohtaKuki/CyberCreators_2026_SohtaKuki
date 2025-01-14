//=================================================
//
// pause.cppの処理に必要なヘッダーファイル (pause.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _PAUSESELECT_H_
#define _PAUSESELECT_H_

#include "object2D.h"

#define PAUSE_SELECT_NUM	(3)

class CPauseSelect : public CObject2D
{
public:
	typedef enum
	{
		PAUSE_SELECT_1 = 0,
		PAUSE_SELECT_2,
		PAUSE_SELECT_3,
		PAUSE_SELECT_MAX
	}PAUSE_SELECT;

	//プロトタイプ宣言
	CPauseSelect(int nPriority = 8);
	~CPauseSelect();
	HRESULT Init() override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	static int GetPauseSelect();
	static CPauseSelect* Create();
	static bool bUseSwitch(int nType);
	static bool GetbUseStat() { return m_bUse;}
private:
	//グローバル変数
	LPDIRECT3DTEXTURE9 m_pTexturePauseSelect[PAUSE_SELECT_NUM] = {};
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffPauseSelect = NULL;
	static int m_nPauseSelect;
	static bool m_bUse;
};

#endif

