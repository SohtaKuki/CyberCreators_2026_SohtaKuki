//=================================================
//
// 3Dモデルのブロックの処理 (3dblock.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _3DBLOCK_H_
#define _3DBLOCK_H_

#include "object3D.h"
#include "objectX.h"
#include "3dplayer.h"

#define MAX_CHAR (256) //行の読み込む数
#define BLOCK_MODEL (16) //モデルの数

//オブジェクト3Dクラス
class C3dblock : public CModel
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
		float fHeight;
		float fWidth;
		float fDepth;
	}ModelPartsEnemy;

	C3dblock(int nPriority = 3);
	~C3dblock() override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static C3dblock* Create(D3DXVECTOR3 pos , int nType);
	void CalculateBlockDimensions(LPD3DXMESH pMesh, float* pWidth, float* pHeight, float* pDepth);
	//void BindTexture(LPDIRECT3DTEXTURE9 pTex);
	static HRESULT Load();
	static void Unload();
	void LoadBlockData();
	bool Collision3DBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fHeight);
	static unsigned const int MAX_BLOCK = 200;	//オブジェクト最大数
	D3DXVECTOR3& GetMoveBlock() { return m_nMove; } //Move値取得
	static int GetMaxBlock() { return m_nMaxBlock; } //ブロックの最大数取得
private:
	D3DXMATRIX m_mtxworld;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	static LPDIRECT3DTEXTURE9 m_pTexBuff;
	LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXVECTOR3 m_nSize;
	D3DXMATRIX m_mtxWorld;
	LPD3DXMESH m_pMesh[BLOCK_MODEL];
	LPD3DXBUFFER m_pBuffMat[BLOCK_MODEL];
	DWORD m_nNumMat[BLOCK_MODEL];
	//Enemy m_aEnemy;
	LoadEnemy m_aLoadEnemy[BLOCK_MODEL];
	ModelPartsEnemy m_aModel[BLOCK_MODEL];
	bool bUse;
	FILE* m_pFile;//ファイルポインタを宣言
    int m_nTurnCnt;
	int m_nType;
	static int m_nMaxBlock;
	bool m_bLanding;
	bool m_bTurn;

protected:
	D3DXVECTOR3 m_nMove;
	D3DXVECTOR3 m_rot;

};

#endif