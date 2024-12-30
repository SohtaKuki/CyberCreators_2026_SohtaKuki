//=================================================
//
// ライト処理 (light.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"


#define MAX_LIGHT (4)

//爆発オブジェクトクラス
class CLight
{
public:
	CLight();
	~CLight();
	HRESULT Init();
	void Uninit();
	void Update();

private:
	D3DLIGHT9 m_alight[MAX_LIGHT];
	D3DXVECTOR3 m_avecDir[MAX_LIGHT];
};

#endif