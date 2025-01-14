//=================================================
//
// タイマーの処理 (timer.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _TIMER_H_
#define _TIMER_H_


#include "object.h"
#include "object2D.h"
#include "player.h"

static const int NUM_TIME = 3;
static const int MAX_TIMESPEED = 60;
static const int TEX_TIME_INTERVAL = 25;

//アイテムオブジェクトクラス
class CTimer : public CObject2D
{
public:
	CTimer(int nPriority = 6);
	~CTimer() override;
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void BindTexture(LPDIRECT3DTEXTURE9 pTex);
	static void AddTimer(int nTime);
	static int GetTimer() { return m_nTime; }
	static CTimer* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:
	static int m_nTime;
	int m_nTimerCnt;
	bool bUpdateTime;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	LPDIRECT3DTEXTURE9 m_pTexBuff;
	D3DXVECTOR3 m_nPos;
};

#endif