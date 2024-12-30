//=================================================
//
// pause.cppの処理に必要なヘッダーファイル (pause.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _STAGESELECT_H_
#define _STAGESELECT_H_

#include "object2D.h"

#define PAUSE_SELECT_NUM	(3)

class CStageSelect : public CObject2D
{
public:
	typedef enum
	{
		STAGE_SELECT_1 = 0,
		STAGE_SELECT_2,
		STAGE_SELECT_3,
		STAGE_SELECT_MAX
	}PAUSE_SELECT;

	//プロトタイプ宣言
	CStageSelect(int nPriority = 3);
	~CStageSelect();
	HRESULT Init() override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	static int GetStageSelect();
	static CStageSelect* Create();
private:
	//グローバル変数
	LPDIRECT3DTEXTURE9 m_pTexturePauseSelect[PAUSE_SELECT_NUM] = {};
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffPauseSelect = NULL;

	static int m_nPauseSelect;
}; 

#endif

