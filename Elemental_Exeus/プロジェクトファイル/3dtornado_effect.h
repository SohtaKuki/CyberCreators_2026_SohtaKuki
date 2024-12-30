//=================================================
//
// ビルボード描画式弾のヘッダーファイル (3dtornado_effect.h)
// Author: Sohta Kuki
//
//=================================================
#ifndef _3D_EFCT_TORNADO_H_	
#define _3D_EFCT_TORNADO_H_	

#include "object3D.h"
#include "3dtornado.h"

//オブジェクト2Dクラス
class C3dtornadoeffect :public CObject3D
{
public:
	C3dtornadoeffect(int nPriority = 15);
	~C3dtornadoeffect() override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	void SetExplosionSub(D3DXVECTOR3 pos, D3DXCOLOR col);
	static C3dtornadoeffect* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, int type);
	D3DXVECTOR3& GetBulletPos() { return m_nPos; }
	static const int BULLET_SPD = 6;
	static const int EFCT_TORNADO_PATTERN_ANIM = 26;  //爆発アニメのパターン数
	static const int DELAY_ANIM = 2.5;//アニメの待機時間
	static const int NUM_TORECT = 2;//アニメの待機時間
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
	C3dtornado* m_p3dTornado;

};

#endif