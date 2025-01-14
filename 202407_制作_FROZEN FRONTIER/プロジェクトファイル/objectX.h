//=================================================
//
// モデルの描画処理 (model.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _MODEL_H_
#define _MODEL_H_

#include "object3D.h"

#define MAX_CHAR (256) //行の読み込む数
#define NUM_MODEL (16) //モデルの数

//オブジェクト3Dクラス
class CModel : public CObject3D
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
		int nLife;
		bool bUse;
		int nCounterState;
		int nIdxModelParent;	//親モデルのインデックス

	}ModelPartsEnemy;

	CModel(int nPriority = 3);
	~CModel() override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static CModel* Create(D3DXVECTOR3 pos);
	//void BindTexture(LPDIRECT3DTEXTURE9 pTex);
	static HRESULT Load();
	static void Unload();
	virtual void LoadModelData(const char* filename);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;

	LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXVECTOR3 m_nPos;
	D3DXVECTOR3 m_nSize;
	//Enemy m_aEnemy;
	bool bUse;
protected:
	FILE* m_pFile;//ファイルポインタを宣言
	LoadEnemy m_aLoadEnemy[NUM_MODEL];
	ModelPartsEnemy m_aModel[NUM_MODEL];
	DWORD m_nNumMat[NUM_MODEL];
	D3DXMATRIX m_mtxWorld;
	LPD3DXMESH m_pMesh[NUM_MODEL];
	LPD3DXBUFFER m_pBuffMat[NUM_MODEL];
	D3DXMATRIX m_mtxworld;
	D3DXVECTOR3 m_nMove;
	D3DXVECTOR3 m_rot;
};

#endif