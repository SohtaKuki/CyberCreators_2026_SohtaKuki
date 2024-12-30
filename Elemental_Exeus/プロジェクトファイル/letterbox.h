//=================================================
//
// スタートコールUI表示処理 (startcallui.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _LETTERBOX_H_
#define _LETTERBOX_H_

#include "object2D.h"

//マクロ定義
#define NUM_LTRBOX	(2)	//アイコンの数

class CLetterbox : public CObject2D
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

	CLetterbox(int nPriority = 75);
	~CLetterbox() override;
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void BindTexture(LPDIRECT3DTEXTURE9 pTex[NUM_LTRBOX]);
	void SetAlpha(int nAlpha);
	static void StartCallUIBossFlag();
	static CLetterbox* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	static bool DisplayLetterbox(int nDisplayID, int DisplayOption);
	static bool GetStartStat() { return m_bStartComplete; }
	static bool GetEndStat() { return m_bAnimationEnd; }
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	LPDIRECT3DTEXTURE9 m_pTexBuff[NUM_LTRBOX];
	D3DXVECTOR3 m_nPos[NUM_LTRBOX];
	D3DXVECTOR3 m_nSize[NUM_LTRBOX];
	float m_aPosTexV[NUM_LTRBOX];
	float m_aPosTexXV[NUM_LTRBOX];
	bool m_bUIUse[NUM_LTRBOX];
	int m_nAlphaCnt;
	int m_nMissonAnim;
	bool m_bAlphaSwitch;
	bool m_bImageSwitch;
	static bool m_bAnimationEnd;
	static bool m_bStartCallFlag;
	static bool m_bStartComplete;
};

#endif

