//=================================================
//
// ビルボード描画式チャージ弾のヘッダーファイル (3dbullet.h)
// Author: Sohta Kuki
//
//=================================================
#ifndef _CHBULLET_H_	
#define _CHBULLET_H_	

#include "object3D.h"

//オブジェクト2Dクラス
class C3dchargebullet :public CObject3D
{
public:
	static const int BULLET_SPD = 5;
	static const int NORMAL_BLOCK_COLISION = 35;
	static const int COLISION_X = 60;
	static const int COLISION_Z = 30;
	C3dchargebullet(int nPriority = 3);
	~C3dchargebullet() override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static C3dchargebullet* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	static LPDIRECT3DTEXTURE9 m_pTexBuff;
	LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXVECTOR3 m_nPos;
	D3DXVECTOR3 m_nSize;
	int m_nLife;

};

#endif