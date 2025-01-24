//=================================================
//
// 3Dモデルのブロックの処理 (3dbosstornado.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _3DBOSSTORNADO_H_
#define _3DBOSSTORNADO_H_

#include "object3D.h"
#include "3dplayer.h"

#define MAX_CHAR (256) //行の読み込む数
#define BOSSTORNADO_MODEL (1) //モデルの数

//オブジェクト3Dクラス
class C3dbosstornado : public CObject3D
{
public:

	typedef struct
	{
		char filename[MAX_CHAR];
		int index;
		int parent;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;

	}LoadEnemy;

	//モデルの構造体
	typedef struct
	{

		D3DXMATRIX mtxworld;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		int nType;
		int nLife;
		bool bUse;
		int nCounterState;
		int nIdxModelParent;	//親モデルのインデックス

	}ModelPartsEnemy;

	typedef enum
	{
		BOSSTORNADO_SET = 0,
		BOSSTORNADO_ATK,
		BOSSTORNADO_BOSS_ATK,
		BOSSTORNADO_BOSS_LEFTATK,
		BOSSTORNADO_BOSS_RIGHTATK,
		BOSSTORNADO_MAX
	}BOSSTORNADO_STATE;

	C3dbosstornado(int nPriority = 3);
	~C3dbosstornado() override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static C3dbosstornado* Create(D3DXVECTOR3 pos, int nDirection);
	//void BindTexture(LPDIRECT3DTEXTURE9 pTex);
	static HRESULT Load();
	static void Unload();
	void LoadBlockData();
	void BosstornadoAtk(int nDirection);
	static int GetNumBosstornado() { return m_nNumBosstornado; }
	bool Collision3DBosstornado(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fHeight);
	static unsigned const int MAX_BOSSTORNADO = 6;	//オブジェクト最大数
	static unsigned const int BOSSTORNADO_TIMER = 180;	//スラッシュウインド持続時間
	D3DXVECTOR3& GetMoveBlock() { return m_nMove; } //Move値取得
private:
	D3DXMATRIX m_mtxworld;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	static LPDIRECT3DTEXTURE9 m_pTexBuff;
	LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXVECTOR3 m_nSize;
	D3DXMATRIX m_mtxWorld;
	LPD3DXMESH m_pMesh[BOSSTORNADO_MODEL];
	LPD3DXBUFFER m_pBuffMat[BOSSTORNADO_MODEL];
	DWORD m_nNumMat[BOSSTORNADO_MODEL];
	//Enemy m_aEnemy;
	D3DXVECTOR3 m_nPos;
	LoadEnemy m_aLoadEnemy[BOSSTORNADO_MODEL];
	ModelPartsEnemy m_aModel[BOSSTORNADO_MODEL];
	bool bUse;
	FILE* m_pFile;//ファイルポインタを宣言
	int m_nTurnCnt;
	static int m_nMaxBlock;
	int m_nTimer;
	int m_nDirection;
	bool m_bLanding;
	bool m_bTurn;
	bool m_bDisplay;
	bool m_bMoveSwitch;
	bool m_bMoveSwitchL;
	bool m_bMoveSwitchBossR;
	bool m_bMoveSwitchBossL;
	bool m_bMoveBossSwitchLD;
	bool m_bMoveBossSwitchRD;
	bool m_bMoveSwitchUP;
	int m_nATKType;
	int m_nSpawnAtkTimer;
	int m_nHiddenTimer;
	static int m_nNumBosstornado;

protected:
	D3DXVECTOR3 m_nMove;
	D3DXVECTOR3 m_rot;

};

#endif