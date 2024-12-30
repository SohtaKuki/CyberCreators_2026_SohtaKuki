//=================================================
//
// ビルボード描画式弾のヘッダーファイル (3dhiteffect.h)
// Author: Sohta Kuki
//
//=================================================
#ifndef _3D_HITEFFECT_H_	
#define _3D_HITEFFECT_H_	

#include "object3D.h"

//オブジェクト2Dクラス
class C3dhiteffect :public CObject3D
{
public:
	static const int BULLET_SPD = 6;
	static const int EXPLOSION_PATTERN_ANIM = 15;  //爆発アニメのパターン数
	static const int DELAY_ANIM = 2;//アニメの待機時間
	C3dhiteffect(int nPriority = 16);
	~C3dhiteffect() override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col);
	static C3dhiteffect* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot);
	D3DXVECTOR3& GetBulletPos() { return m_nPos; }
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	static LPDIRECT3DTEXTURE9 m_pTexBuff;
	LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXVECTOR3 m_nPos;
	D3DXVECTOR3 m_nSize;
	int m_nLife;
	int m_nType;
	int m_nCounterAnim;
	int m_nPatternAnimExpl;
	bool m_bUse;

};

#endif