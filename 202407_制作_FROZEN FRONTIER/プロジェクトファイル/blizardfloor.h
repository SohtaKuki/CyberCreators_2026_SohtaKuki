//=================================================
//
// 床の描画処理 (floor.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _BLIZARDFLOOR_H_
#define _BLIZARDFLOOR_H_

#include "object.h"
#include "object3D.h"


//オブジェクト2Dクラス
class CBlizardfloor : public CObject3D
{
public:
	CBlizardfloor(int nPriority = 2);
	~CBlizardfloor() override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	LPDIRECT3DVERTEXBUFFER9 GetBuff(void);
	static CBlizardfloor* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	static HRESULT Load();
	static void Unload();
	void SetPos(D3DXVECTOR3 pos);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	static LPDIRECT3DTEXTURE9 m_pTexBuff;
	LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXVECTOR3 m_nPos;
	D3DXVECTOR3 m_nSize;
	D3DXMATRIX m_mtxWorld;
	bool bUse;
protected:
	D3DXVECTOR3 m_nMove;
	D3DXVECTOR3 m_rot;
};

#endif
