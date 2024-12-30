//=================================================
//
// スコア処理 (score.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _SCORE_H_
#define _SCORE_H_


#include "object.h"
#include "object2D.h"
#include "player.h"

static const int NUM_SCORE = 8;
static const int TEX_SCORE_INTERVAL = 24;

//アイテムオブジェクトクラス
class CScore : public CObject2D
{
public:
	CScore(int nPriority = 73);
	~CScore() override;
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void BindTexture(LPDIRECT3DTEXTURE9 pTex);
	static void AddScore(int nTime);
	static int GetScore() { return m_nScore; }
	static int GetHPBonusScore() { return m_nHPBonusScore; }
	static int GetTimeBonusScore() { return m_nTimeBonusScore; }
	static CScore* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:
	static int m_nScore;
	static int m_nHPBonusScore;
	static int m_nTimeBonusScore;
	int m_nScoreCnt;
	bool bUpdateTime;
	bool m_bUse[NUM_SCORE];
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	LPDIRECT3DTEXTURE9 m_pTexBuff;
	D3DXVECTOR3 m_nPos;
	D3DXVECTOR3 m_nSize;
};

#endif