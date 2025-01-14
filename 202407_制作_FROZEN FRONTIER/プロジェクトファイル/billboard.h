//=================================================
//
// ビルボードのヘッダーファイル (billboard.h)
// Author: Sohta Kuki
//
//=================================================
#ifndef _BILLBOARD_H_	
#define _BILLBOARD_H_	

#include "object3D.h"

//オブジェクト2Dクラス
class CBillboard :public CObject3D
{
public:
	CBillboard(int nPriority = 3);
	~CBillboard() override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static CBillboard * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	static LPDIRECT3DTEXTURE9 m_pTexBuff;
	LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXVECTOR3 m_nPos;
	D3DXVECTOR3 m_nSize;

};

#endif