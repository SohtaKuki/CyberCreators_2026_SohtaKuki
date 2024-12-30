//=================================================
//
// ビルボードのヘッダーファイル (billboard.h)
// Author: Sohta Kuki
//
//=================================================
#ifndef _3DTUTORIALUI_H_	
#define _3DTUTORIALUI_H_	

#include "object3D.h"

#define NUM_UI (2) //UIの数

//オブジェクト2Dクラス
class C3dTutorialUI :public CObject3D
{
public:

	//UI表示構造体
	typedef enum
	{
		TUTORIAL_UI_000 = 0,
		TUTORIAL_UI_001,
		TUTORIAL_UI_002,
		TUTORIAL_UI_003,
		TUTORIAL_UI_004,
		TUTORIAL_UI_MAX,
	}TUTORIALUI_DISPLAY;

	//UI表示構造体
	typedef enum
	{
		UI_DISPLAY = 0,
		UI_HIDDEN,
		UI_MAX,
	}UIDISPLAY;


	C3dTutorialUI(int nPriority = 10);
	~C3dTutorialUI() override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static C3dTutorialUI* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nType);
	static bool DisplayBossATKUI(int nDisplayID, int DisplayOption);
	HRESULT Load(LPCSTR textureFileName);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	LPDIRECT3DTEXTURE9 m_pTexBuff;
	LPDIRECT3DTEXTURE9 m_pTexture[NUM_UI];
	D3DXVECTOR3 m_nPos[NUM_UI];
	D3DXVECTOR3 m_nSize[NUM_UI];
	static bool m_bUse[NUM_UI];
	static int m_nType;
};

#endif