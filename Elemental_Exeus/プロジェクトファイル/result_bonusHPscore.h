//=================================================
//
// リザルトの処理 (score.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _RESULTHPBOUNS_SCORE_H_
#define _RESULTHPBOUNS_SCORE_H_


#include "object.h"
#include "object2D.h"
#include "player.h"

static const int NUM_RESULT_HPB_SCORE = 4;
static const int TEX_RESULT_HPB_SCORE_INTERVAL = 40;

//アイテムオブジェクトクラス
class CResultHPBScore : public CObject2D
{
public:
	CResultHPBScore(int nPriority = 6);
	~CResultHPBScore() override;
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void BindTexture(LPDIRECT3DTEXTURE9 pTex);
	static void AddResultHPBScore(int nTime);
	int GetResultHPBScore() { return m_nResultHPBScore; }
	static CResultHPBScore* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:
	static int m_nResultHPBScore;
	int m_nResultHPBScoreCnt;
	bool bUpdateTime;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	LPDIRECT3DTEXTURE9 m_pTexBuff;
	D3DXVECTOR3 m_nPos;
	D3DXVECTOR3 m_nSize;
};

#endif