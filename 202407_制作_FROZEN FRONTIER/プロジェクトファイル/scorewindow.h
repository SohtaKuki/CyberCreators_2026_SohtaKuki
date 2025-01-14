//=================================================
//
// 2Dポリゴンの描画処理 (object2D.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _SCOREWINDOW_H_
#define _SCOREWINDOW_H_

#include "object.h"
#include "object2D.h"

//背景オブジェクトクラス
class CScoreWindow :public CObject2D
{
public:
	CScoreWindow(int nPriority = 4);
	~CScoreWindow()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	static CScoreWindow* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
private:
	D3DXVECTOR3 m_nPos;
	D3DXVECTOR3 m_nSize;
	bool bUse;
};

#endif

