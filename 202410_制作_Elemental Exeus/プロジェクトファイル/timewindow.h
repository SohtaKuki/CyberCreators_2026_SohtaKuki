//=================================================
//
// 2Dポリゴンの描画処理 (object2D.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _TIMEWINDOW_H_
#define _TIMEWINDOW_H_

#include "object.h"
#include "object2D.h"

//背景オブジェクトクラス
class CTimeWindow :public CObject2D
{
public:
	CTimeWindow(int nPriority = 70);
	~CTimeWindow()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	static CTimeWindow* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
private:
	D3DXVECTOR3 m_nPos;
	D3DXVECTOR3 m_nSize;
	bool bUse;
};

#endif

