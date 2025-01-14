//=================================================
//
// ビルボードのヘッダーファイル (billboard.h)
// Author: Sohta Kuki
//
//=================================================
#ifndef _3DBOSSATKUI_DIAG_H_	
#define _3DBOSSATKUI_DIAG_H_	

#include "object3D.h"

#define NUM_UI (2) //UIの数

//オブジェクト2Dクラス
class C3dBossATKUIDiag :public CObject3D
{
public:

	//UI表示構造体
	typedef enum
	{
		BSUI_R_DIAGATK = 0,
		BSUI_L_DIAGATK,
		BSUI_MAX,
	}ATKUI_DISPLAY;

	//UI表示構造体
	typedef enum
	{
		UI_DISPLAY = 0,
		UI_HIDDEN,
		UI_MAX,
	}UIDISPLAY;


	C3dBossATKUIDiag(int nPriority = 20);
	~C3dBossATKUIDiag() override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static C3dBossATKUIDiag* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	static bool DisplayBossATKUI(int nDisplayID, int DisplayOption);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	static LPDIRECT3DTEXTURE9 m_pTexBuff;
	LPDIRECT3DTEXTURE9 m_pTexture[NUM_UI];
	D3DXVECTOR3 m_nPos[NUM_UI];
	D3DXVECTOR3 m_nSize[NUM_UI];
	static bool m_bUse[NUM_UI];
	static int m_nType;
};

#endif