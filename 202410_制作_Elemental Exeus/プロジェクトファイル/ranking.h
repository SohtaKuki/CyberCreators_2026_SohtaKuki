//================================
//
// ランキング処理 (ranking.h)
// Author: Sohta Kuki
//
//================================

#ifndef _RANKING_H_
#define _RANKING_H_

#include "object.h"
#include "object2D.h"

//マクロ定義
#define MAX_RANK (5)//表示する順位数 (スコア数字)
#define MAX_RANKINGBG (2)//ランキング画面の背景
#define SCORE_NUM_SPACE (70) //スコア数字間隔
#define SCORE_NUM_SCALEX (45) //スコア数字大きさ X
#define SCORE_NUM_SCALEY (40) //スコア数字大きさ Y
#define MAX_SCORE (30)//スコアの最大表示数

class CRanking :public CObject2D
{
public:

	//ランキングスコア構造体
	typedef struct
	{
		D3DXVECTOR3 pos;
		int nScore;
	}RankScore;

	//プロトタイプ宣言
	CRanking(int nPriority = 10);
	~CRanking();
	HRESULT Init(void)override;
	void Uninit(void)override;
	void Update(void)override;
	void Draw(void)override;
	void ResetRanking(void);
	void SetRanking(int nScore);
	void WriteRankingData(void);
	void SetRankingPos(D3DXVECTOR3 pos, int rank);
	static CRanking* Create();
private:
	LPDIRECT3DTEXTURE9 m_pTextureRanking[MAX_RANKINGBG] = {};
	LPDIRECT3DTEXTURE9 m_pTextureRankingscore = NULL;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffRanking = NULL;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffRankingscore = NULL;
	RankScore m_aRankScore[MAX_RANK];
	int m_nRankingTimer;//ランキング画面表示用タイマー
	int m_nRankUpdate = -1;
	int m_nCounterRankingState = 0;//状態管理カウンター
	int m_aPosTexU[MAX_RANK][8];
};

#endif