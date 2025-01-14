//=================================================
//
// 2Dポリゴンの描画処理 (object2D.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _BOSSUI_H_
#define _BOSSUI_H_

#include "object.h"
#include "object2D.h"

//背景オブジェクトクラス
class CBossui :public CObject2D
{
public:
	CBossui(int nPriority = 68);
	~CBossui()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	static CBossui* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
private:
	D3DXVECTOR3 m_nPos;
	D3DXVECTOR3 m_nSize;
	bool bUse;
};

#endif

