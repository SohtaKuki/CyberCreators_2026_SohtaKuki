//=================================================
//
// タイトル画面の背景処理 (titlebg.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _STAGESELECTBG_H_
#define _STAGESELECTBG_H_

#include "object2D.h"

//マクロ定義
#define NUM_BG	(4)	//背景の数

class CStageSelectBG : public CObject2D
{
public:
	CStageSelectBG(int nPriority = 3);
	~CStageSelectBG() override;
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void BindTexture(LPDIRECT3DTEXTURE9 pTex[NUM_BG]);
	static CStageSelectBG* Create();
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	LPDIRECT3DTEXTURE9 m_pTexBuff[NUM_BG];
	D3DXVECTOR3 m_nPos[NUM_BG];
	float m_aPosTexV[NUM_BG];
	float m_aPosTexXV[NUM_BG];
};

#endif

