//=================================================
//
// スタートコールUI表示処理 (startcallui.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _BOSSCALLUI_H_
#define _BOSSCALLUI_H_

#include "object2D.h"

//マクロ定義
#define NUM_ICON	(3)	//アイコンの数

class CBossCallUI : public CObject2D
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

	CBossCallUI(int nPriority = 75);
	~CBossCallUI() override;
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void BindTexture(LPDIRECT3DTEXTURE9 pTex[NUM_ICON]);
	void SetAlpha(int nAlpha);
	static void StartCallUIBossFlag();
	static CBossCallUI* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	static bool DisplayBossCallUI(int nDisplayID, int DisplayOption);
	static bool GetStartStat() { return m_bStartComplete; }
	static bool GetEndStat() { return m_bAnimationEnd; }
	static bool GetBeginCallUI() { return m_bBeginCallUI; }
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	LPDIRECT3DTEXTURE9 m_pTexBuff[NUM_ICON];
	D3DXVECTOR3 m_nPos[NUM_ICON];
	D3DXVECTOR3 m_nSize[NUM_ICON];
	float m_aPosTexV[NUM_ICON];
	float m_aPosTexXV[NUM_ICON];
	static bool m_bUse[NUM_ICON];
	int m_nAlphaCnt;
	int m_nMissonAnim;
	bool m_bAlphaSwitch;
	bool m_bImageSwitch;
	static bool m_bAnimationEnd;
	static bool m_bStartCallFlag;
	static bool m_bStartComplete;
	static bool m_bBeginCallUI;
};

#endif

