//=================================================
//
// 2Dポリゴンの描画処理 (object2D.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _PLAYERWINDOW_H_
#define _PLAYERWINDOW_H_

#include "object.h"
#include "object2D.h"

//背景オブジェクトクラス
class CPlayerWindow :public CObject2D
{
public:
	CPlayerWindow(int nPriority = 70);
	~CPlayerWindow()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	static CPlayerWindow* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
private:
	D3DXVECTOR3 m_nPos;
	D3DXVECTOR3 m_nSize;
	bool bUse;
};

#endif

