//=================================================
//
// プレイヤーバフ付与状態UI表示処理 (3dbuffui.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _RESULTRANK_H_
#define _RESULTRANK_H_

#include "object2D.h"

//マクロ定義
#define NUM_RANK	(5)	//アイコンの数

class CResultRank : public CObject2D
{
public:
	//UI表示構造体
	typedef enum
	{
		RANK_D = 0,
		RANK_C,
		RANK_B,
		RANK_A,
		RANK_S,
		RANK_MAX,
	}RANKDISPLAY;

	//UI表示構造体
	typedef enum
	{
		UI_DISPLAY = 0,
		UI_HIDDEN,
		UI_MAX,
	}UIDISPLAY;

	CResultRank(int nPriority = 6);
	~CResultRank() override;
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void BindTexture(LPDIRECT3DTEXTURE9 pTex[NUM_RANK]);
	void SetAlpha(int nAlpha);
	static CResultRank* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	static bool DisplayResultRank(int nDisplayID, int DisplayOption);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	LPDIRECT3DTEXTURE9 m_pTexBuff[NUM_RANK];
	D3DXVECTOR3 m_nPos[NUM_RANK];
	D3DXVECTOR3 m_nSize[NUM_RANK];
	float m_aPosTexV[NUM_RANK];
	float m_aPosTexXV[NUM_RANK];
	static bool m_bUse[NUM_RANK];
	int m_nAlphaCnt;
	bool m_bAlphaSwitch;
};

#endif

