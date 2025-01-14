//=================================================
//
// ビルボードのヘッダーファイル (billboard.h)
// Author: Sohta Kuki
//
//=================================================
#ifndef _3DBOSSATKUI_H_	
#define _3DBOSSATKUI_H_	

#include "object3D.h"

//オブジェクト2Dクラス
class C3dBossATKUI :public CObject3D
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
		BSUI_R_MELEEATK,
		BSUI_L_MELEEATK,
		BSUI_WIREDATK,
		BSUI_MAX,
	}ATKUI_DISPLAY;

	//UI表示構造体
	typedef enum
	{
		UI_DISPLAY = 0,
		UI_HIDDEN,
		UI_MAX,
	}UIDISPLAY;


	C3dBossATKUI(int nPriority = 20);
	~C3dBossATKUI() override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static C3dBossATKUI* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	static bool DisplayBossATKUI(int nDisplayID, int DisplayOption);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	static LPDIRECT3DTEXTURE9 m_pTexBuff;
	LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXVECTOR3 m_nPos;
	D3DXVECTOR3 m_nSize;
	static bool m_bUse;
	static int m_nType;
};

#endif