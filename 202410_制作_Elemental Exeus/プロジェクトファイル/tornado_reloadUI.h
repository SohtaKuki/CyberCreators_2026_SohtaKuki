//=================================================
//
// プレイヤーバフ付与状態UI表示処理 (tornado_reloadUI.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _TORNADORELOADUI_H_
#define _TORNADORELOADUI_H_

#include "object2D.h"

//マクロ定義
#define NUM_ICON	(2)	//アイコンの数

class CTornadoReloadUI : public CObject2D
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

	CTornadoReloadUI(int nPriority = 73);
	~CTornadoReloadUI() override;
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void BindTexture(LPDIRECT3DTEXTURE9 pTex[NUM_ICON]);
	void SetAlpha(int nAlpha);
	static CTornadoReloadUI* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	static bool DisplayTornadoReloadUI(int nDisplayID, int DisplayOption);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	LPDIRECT3DTEXTURE9 m_pTexBuff[NUM_ICON];
	D3DXVECTOR3 m_nPos[NUM_ICON];
	D3DXVECTOR3 m_nSize[NUM_ICON];
	D3DXVECTOR3 m_nRot[NUM_ICON];
	float m_aPosTexV[NUM_ICON];
	float m_aPosTexXV[NUM_ICON];
	static bool m_bUse[NUM_ICON];
	int m_nAlphaCnt;
	bool m_bAlphaSwitch;
};

#endif

