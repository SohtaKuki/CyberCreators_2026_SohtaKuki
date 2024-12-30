//=================================================
//
// ビルボード描画式弾のヘッダーファイル (3dbullet.h)
// Author: Sohta Kuki
//
//=================================================
#ifndef _BULLET_H_	
#define _BULLET_H_	

#include "object3D.h"

//オブジェクト2Dクラス
class C3dbullet :public CObject3D
{
public:
	static const int BULLET_SPD = 6;
	C3dbullet(int nPriority = 3);
	~C3dbullet() override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static C3dbullet* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot,int type);
	D3DXVECTOR3& GetBulletPos() { return m_nPos; }
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	static LPDIRECT3DTEXTURE9 m_pTexBuff;
	LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXVECTOR3 m_nPos;
	D3DXVECTOR3 m_nSize;
	int m_nLife;
	int m_nType;

};

#endif