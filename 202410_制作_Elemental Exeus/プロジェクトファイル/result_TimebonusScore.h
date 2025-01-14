//=================================================
//
// リザルトの処理 (score.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _RESULT_TIMEB_SCORE_H_
#define _RESULT_TIMEB_SCORE_H_


#include "object.h"
#include "object2D.h"
#include "player.h"

static const int NUM_RESULT_TIMEB_SCORE = 5;
static const int TEX_RESULT_TIMEB_SCORE_INTERVAL = 40;

//アイテムオブジェクトクラス
class CResultTIMEBScore : public CObject2D
{
public:
	CResultTIMEBScore(int nPriority = 6);
	~CResultTIMEBScore() override;
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void BindTexture(LPDIRECT3DTEXTURE9 pTex);
	static void AddResultScore(int nTime);
	int GetResultScore() { return m_nResultScore; }
	static CResultTIMEBScore* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:
	static int m_nResultScore;
	int m_nResultScoreCnt;
	bool bUpdateTime;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	LPDIRECT3DTEXTURE9 m_pTexBuff;
	D3DXVECTOR3 m_nPos;
	D3DXVECTOR3 m_nSize;
};

#endif