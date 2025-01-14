//=================================================
//
// 3Dモデルのプレイヤーの表示処理 (3denemy.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _3DENEMY_H_
#define _3DENEMY_H_

#include "object3D.h"
#include "objectX.h"

#define MAX_CHAR (256) //行の読み込む数
#define NUM_MODEL (16) //モデルの数

//オブジェクト3Dクラス
class C3denemy : public CModel
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
		LPDIRECT3DTEXTURE9 m_pTexture;
		int nLife;
		bool bUse;
		int nCounterState;
		int nIdxModelParent;	//親モデルのインデックス

	}ModelPartsEnemy;

	C3denemy(int nPriority = 3);
	~C3denemy() override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	//void SetPlayerPos();
	static C3denemy* Create(D3DXVECTOR3 pos,int MoveType);
	//void BindTexture(LPDIRECT3DTEXTURE9 pTex);
	static HRESULT Load();
	static void Unload();
	void LoadPlayerData();
	void EnemyDamage();
	//void OldPlayerPos() { m_nPos = m_nOld3DPlayerPos; }
	//void OldPlayerPosZ() { m_nPos.z = m_nOld3DPlayerPos.z; }
	D3DXVECTOR3& GetEnemyPos() { return m_nOld3DEnemyPos; }//座標の取得
	static unsigned const int MAX_ENEMY = 300;	//オブジェクト最大数
	static const int SHOT_INTERVAL = 120;
	static const int MOVE_INTERVAL = 90;
private:
	D3DXMATRIX m_mtxworld;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	static LPDIRECT3DTEXTURE9 m_pTexBuff;
	LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXVECTOR3 m_nOld3DEnemyPos;
	D3DXVECTOR3 m_nSize;
	D3DXMATRIX m_mtxWorld;
	LPD3DXMESH m_pMesh[NUM_MODEL];
	LPD3DXBUFFER m_pBuffMat[NUM_MODEL];
	DWORD m_nNumMat[NUM_MODEL];
	//Enemy m_aEnemy;
	LoadEnemy m_aLoadEnemy[NUM_MODEL];
	ModelPartsEnemy m_aModel[NUM_MODEL];
	bool bUse;
	bool m_bMoveSwitch;
	FILE* m_pFile;//ファイルポインタを宣言
	int m_nShotInterval; //弾の打つ間隔
	int m_nMoveInterval; //方向転換をする間隔
	int m_nLife;
protected:
	D3DXVECTOR3 m_n3DEnemyMove;
	D3DXVECTOR3 m_rot;

};

#endif