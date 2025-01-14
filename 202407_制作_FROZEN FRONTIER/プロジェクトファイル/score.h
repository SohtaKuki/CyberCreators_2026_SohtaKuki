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
static const int TEX_SCORE_INTERVAL = 28;

//アイテムオブジェクトクラス
class CScore : public CObject2D
{
public:
	CScore(int nPriority = 6);
	~CScore() override;
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void BindTexture(LPDIRECT3DTEXTURE9 pTex);
	static void AddScore(int nTime);
	static int GetScore() { return m_nScore; }
	static CScore* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:
	static int m_nScore;
	int m_nScoreCnt;
	bool bUpdateTime;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	LPDIRECT3DTEXTURE9 m_pTexBuff;
	D3DXVECTOR3 m_nPos;
	D3DXVECTOR3 m_nSize;
};

#endif