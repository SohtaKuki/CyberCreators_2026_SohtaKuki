//=================================================
//
// 3Dモデルのゴールマーカーの処理 (3dgoalobj.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _3DGOALOBJ_H_
#define _3DGOALOBJ_H_

#include "object3D.h"
#include "objectX.h"
#include "3dplayer.h"

#define MAX_CHAR (256) //行の読み込む数
#define NUM_MODEL (16) //モデルの数

//オブジェクト3Dクラス
class C3dgoalobj : public CModel
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

	C3dgoalobj(int nPriority = 3);
	~C3dgoalobj() override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static C3dgoalobj* Create(D3DXVECTOR3 pos,int nType);
	//void BindTexture(LPDIRECT3DTEXTURE9 pTex);
	static HRESULT Load();
	static void Unload();
	void LoadGoalobjData();
	static void ResultMove();
	static int GetStageNum();
	static int ResetStageNum(){return m_nStageNum = 0; }
	bool Collision3DGoalobj(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fHeight);
	static unsigned const int MAX_BLOCK = 1000;	//オブジェクト最大数
private:
	D3DXMATRIX m_mtxworld;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	static LPDIRECT3DTEXTURE9 m_pTexBuff;
	LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXVECTOR3 m_nSize;
	D3DXMATRIX m_mtxWorld;
	LPD3DXMESH m_pMesh[NUM_MODEL];
	LPD3DXBUFFER m_pBuffMat[NUM_MODEL];
	DWORD m_nNumMat[NUM_MODEL];
	//Enemy m_aEnemy;
	LoadEnemy m_aLoadEnemy[NUM_MODEL];
	ModelPartsEnemy m_aModel[NUM_MODEL];
	bool bUse;
	FILE* m_pFile;//ファイルポインタを宣言
	bool m_bLanding;
	int m_nType;
	int m_nEndTimer;
	bool m_bIsGoal;
	static int m_nStageNum;
protected:
	D3DXVECTOR3 m_nMove;
	D3DXVECTOR3 m_rot;

};

#endif