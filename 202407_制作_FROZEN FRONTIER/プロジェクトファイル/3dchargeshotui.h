//=================================================
//
// プレイヤーバフ付与状態UI表示処理 (3dbuffui.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _CHARGESHOTUI_H_
#define _CHARGESHOTUI_H_

#include "object2D.h"

//マクロ定義
#define NUM_ICON	(1)	//アイコンの数

class CChargeshotUI : public CObject2D
{
public:
	//UI表示構造体
	typedef enum
	{
		ICON_SPDUP = 0,
		ICON_INSTANTSHOT,
		ICON_MAX,
	}ICONDISPLAY;

	//UI表示構造体
	typedef enum
	{
		UI_DISPLAY = 0,
		UI_HIDDEN,
		UI_MAX,
	}UIDISPLAY;

	CChargeshotUI(int nPriority = 6);
	~CChargeshotUI() override;
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void BindTexture(LPDIRECT3DTEXTURE9 pTex[NUM_ICON]);
	static CChargeshotUI* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	static bool DisplayChargeshotUI(int nDisplayID, int DisplayOption);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	LPDIRECT3DTEXTURE9 m_pTexBuff[NUM_ICON];
	D3DXVECTOR3 m_nPos[NUM_ICON];
	D3DXVECTOR3 m_nSize[NUM_ICON];
	float m_aPosTexV[NUM_ICON];
	float m_aPosTexXV[NUM_ICON];
	static bool m_bUse[NUM_ICON];
	int m_nAlphaCnt;
	int m_nMoveCnt;
	bool m_bAlphaSwitch;
};

#endif

