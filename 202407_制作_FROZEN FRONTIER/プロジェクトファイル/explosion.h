//=================================================
//
// 爆発の処理 (explosion.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "main.h"
#include "object.h"
#include "object2D.h"

//爆発オブジェクトクラス
class CExplosion : public CObject2D
{
public:
	static const int EXPL_TEX_SIZE = 50;		
	static const int UPDATE_TEX = 3;
	static const int EXPLOSION_PATTERN_ANIM = 8;

	CExplosion();			
	~CExplosion() override;	
	HRESULT Init() override;
	void Uninit() override;	
	void Update() override;	
	void Draw() override;	
	static CExplosion* Create(D3DXVECTOR3 pos);	

private:
	int m_nPatternAnimExpl;
	int m_nCounterAnimExpl;
};

#endif