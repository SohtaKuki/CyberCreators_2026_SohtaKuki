//=================================================
//
// ビルボードのヘッダーファイル (billboard.h)
// Author: Sohta Kuki
//
//=================================================
#ifndef _PARTICLE_H_	
#define _PARTICLE_H_	

#include "object3D.h"

//オブジェクト2Dクラス
class CParticle :public CObject3D
{
public:


	CParticle(int nPriority = 6);
	~CParticle() override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static CParticle* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	static LPDIRECT3DTEXTURE9 m_pTexBuff;
	LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXVECTOR3 m_nPos;
	D3DXVECTOR3 m_nSize;
	D3DXVECTOR3 m_vVelocity;
	D3DXVECTOR3 m_vAcceleration;
	float m_fLifeTime; // 寿命を設定（1秒）
	float m_fAlphaDecay; // アルファ値の減少量
	float m_fCurrentTime;

};

#endif