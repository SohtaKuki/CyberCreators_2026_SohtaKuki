//=================================================
//
// スタートコールUI表示処理 (startcallui.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _BOSSATKUI_H_
#define _BOSSATKUI_H_

#include "object2D.h"

//マクロ定義
#define NUM_ICON	(6)	//アイコンの数

class CBossATKUI : public CObject2D
{
public:
	//UI表示構造体
	typedef enum
	{
		BSUI_R_LOWERDASHATK = 0,
		BSUI_L_LOWERDASHATK,
		BSUI_R_UPPERDASHATK,
		BSUI_L_UPPERDASHATK,
		BSUI_R_AIRATK,
		BSUI_L_AIRATK,
		BSUI_MAX,
	}ATKUI_DISPLAY;

	//UI表示構造体
	typedef enum
	{
		UI_DISPLAY = 0,
		UI_HIDDEN,
		UI_MAX,
	}UIDISPLAY;

	CBossATKUI(int nPriority = 80);
	~CBossATKUI() override;
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void BindTexture(LPDIRECT3DTEXTURE9 pTex[NUM_ICON]);
	void SetAlpha(int nAlpha);
	static CBossATKUI* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	static bool DisplayBossATKUI(int nDisplayID, int DisplayOption);
	static bool GetEndStat() { return m_bEndComplete; }
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
	static bool m_bEndComplete;
};

#endif

