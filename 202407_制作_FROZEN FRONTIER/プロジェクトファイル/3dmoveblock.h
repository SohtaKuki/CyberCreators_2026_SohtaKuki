//=================================================
//
// 3Dモデルのブロックの処理 (3dmoveblock.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _3DMOVEBLOCK_H_
#define _3DMOVEBLOCK_H_

#include "object3D.h"
#include "objectX.h"
#include "3dplayer.h"

#define MAX_CHAR (256) //行の読み込む数
#define NUM_MODEL (16) //モデルの数

//オブジェクト3Dクラス
class C3dmoveblock : public CModel
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

	C3dmoveblock(int nPriority = 3);
	~C3dmoveblock() override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static C3dmoveblock* Create(D3DXVECTOR3 pos,int nType);
	//void BindTexture(LPDIRECT3DTEXTURE9 pTex);
	static HRESULT Load();
	static void Unload();
	void LoadMoveblockData();
	bool Collision3DMoveblock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fHeight);
	static unsigned const int MAX_BLOCK = 100;	//オブジェクト最大数
	D3DXVECTOR3& GetMoveBlock() { return m_nMove; } //Move値取得
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
	int m_nTurnCnt;
	int m_nType;
	bool m_bLanding;
	bool m_bTurn;

protected:
	D3DXVECTOR3 m_nMove;
	D3DXVECTOR3 m_rot;

};

#endif