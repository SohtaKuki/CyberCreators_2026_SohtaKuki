//=================================================
//
// リザルトの処理 (score.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _RESULTSCORE_H_
#define _RESULTSCORE_H_


#include "object.h"
#include "object2D.h"
#include "player.h"

static const int NUM_RESULTSCORE = 8;
static const int TEX_RESULTSCORE_INTERVAL = 75;

//アイテムオブジェクトクラス
class CResultScore : public CObject2D
{
public:
	CResultScore(int nPriority = 6);
	~CResultScore() override;
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void BindTexture(LPDIRECT3DTEXTURE9 pTex);
	static void AddResultScore(int nTime);
	int GetResultScore() { return m_nResultScore; }
	static CResultScore* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

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