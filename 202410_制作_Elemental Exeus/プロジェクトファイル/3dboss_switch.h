//=================================================
//
// 3Dモデルのブロックの処理 (3dswitchbs.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _3DSWITCH_BS_H_
#define _3DSWITCH_BS_H_

#include "object3D.h"
#include "objectX.h"
#include "3dplayer.h"

#define MAX_CHAR (256) //行の読み込む数
#define SWITCH_BS_MODEL (16) //モデルの数

//オブジェクト3Dクラス
class C3dswitchbs : public CModel
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

	C3dswitchbs(int nPriority = 3);
	~C3dswitchbs() override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static C3dswitchbs* Create(D3DXVECTOR3 pos, int nType);
	void CalculateSwitchbsDimensions(LPD3DXMESH pMesh, float* pWidth, float* pHeight, float* pDepth);
	//void BindTexture(LPDIRECT3DTEXTURE9 pTex);
	static HRESULT Load();
	static void Unload();
	void LoadSwitchbsData();
	bool Collision3DSwitchbs(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fHeight);
	static unsigned const int MAX_SWITCH_BS = 200;	//オブジェクト最大数
	D3DXVECTOR3& GetMoveSwitchbs() { return m_nMove; } //Move値取得
	static int GetMaxSwitchbs() { return m_nMaxSwitchbs; }
private:
	D3DXMATRIX m_mtxworld;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	static LPDIRECT3DTEXTURE9 m_pTexBuff;
	LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXVECTOR3 m_nSize;
	D3DXMATRIX m_mtxWorld;
	LPD3DXMESH m_pMesh[SWITCH_BS_MODEL];
	LPD3DXBUFFER m_pBuffMat[SWITCH_BS_MODEL];
	DWORD m_nNumMat[SWITCH_BS_MODEL];
	//Enemy m_aEnemy;
	LoadEnemy m_aLoadEnemy[SWITCH_BS_MODEL];
	ModelPartsEnemy m_aModel[SWITCH_BS_MODEL];
	bool bUse;
	FILE* m_pFile;//ファイルポインタを宣言
	int m_nTurnCnt;
	int m_nType;
	static int m_nMaxSwitchbs;
	bool m_bLanding;
	bool m_bTurn;

protected:
	D3DXVECTOR3 m_nMove;
	D3DXVECTOR3 m_rot;

};

#endif