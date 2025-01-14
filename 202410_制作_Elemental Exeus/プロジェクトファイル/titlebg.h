//=================================================
//
// タイトル画面の背景処理 (titlebg.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _TITLEBG_H_
#define _TITLEBG_H_

#include "object2D.h"

//マクロ定義
#define NUM_BG	(4)	//背景の数

class CTitleBG : public CObject2D
{
public:
	CTitleBG(int nPriority = 50);
	~CTitleBG() override;
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void BindTexture(LPDIRECT3DTEXTURE9 pTex[NUM_BG]);
	static CTitleBG* Create();
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	LPDIRECT3DTEXTURE9 m_pTexBuff[NUM_BG];
	D3DXVECTOR3 m_nPos[NUM_BG];
	float m_aPosTexV[NUM_BG];
	float m_aPosTexXV[NUM_BG];
	int m_nColTex[NUM_BG];
	int m_nAnimCnt;
	bool m_bAnimSwitch;
};

#endif

