//=================================================
//
// 3Dモデルのプレイヤーの表示処理 (3denemyair.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _3DENEMYAIR_H_
#define _3DENEMYAIR_H_

#include "object3D.h"
#include "objectX.h"

#define MAX_CHAR (256) //行の読み込む数
#define ENEMYAIR_MODEL (7) //モデルの数
#define MAX_KEYSET (20) // キーの最大値
#define MAX_MOTION (10) //モーション

//オブジェクト3Dクラス
class C3denemyair : public CModel
{
public:

	typedef enum
	{
		MOTION_NORMAL = 0,
		MOTION_ACTION,
		MOTION_MAX
	}ENEMYAIRMOTION;


	typedef struct
	{
		char filename[MAX_CHAR];
		int index;
		int parent;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;

	}LoadEnemy;

	//キーの構造体
	typedef struct {
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
	}Key;

	//キーセットの構造体
	typedef struct {
		int Frame;
		Key aKey[ENEMYAIR_MODEL];
	}KeySet;

	//モーションの構造体
	typedef struct {
		bool Loop;
		int NumKey;
		KeySet KeySet[MAX_KEYSET];
	}MotionSet;

	//モデルの構造体
	typedef struct
	{

		D3DXMATRIX mtxworld;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		D3DXVECTOR3 targetPos;
		D3DXVECTOR3 targetRot;
		LPDIRECT3DTEXTURE9 m_pTexture;
		int nLife;
		bool bUse;
		int nCounterState;
		int nIdxModelParent;	//親モデルのインデックス

	}ModelPartsEnemy;

	C3denemyair(int nPriority = 3);
	~C3denemyair() override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	void ModelMotion();
	//void SetPlayerPos();
	static C3denemyair* Create(D3DXVECTOR3 pos, int MoveType);
	//void BindTexture(LPDIRECT3DTEXTURE9 pTex);
	static HRESULT Load();
	static void Unload();
	void LoadPlayerData();
	void EnemyDamage();
	//void OldPlayerPos() { m_nPos = m_nOld3DPlayerPos; }
	//void OldPlayerPosZ() { m_nPos.z = m_nOld3DPlayerPos.z; }
	D3DXVECTOR3& GetEnemyPos() { return m_nOld3DEnemyPos; }//座標の取得
	static unsigned const int MAX_ENEMYAIR = 300;	//オブジェクト最大数
	static const int SHOT_INTERVAL = 120;
	static const int MOVE_INTERVAL = 100;
	ENEMYAIRMOTION Motion;
	void SetEnemyMotion(ENEMYAIRMOTION motion);
	bool Collision3DEnemyAir(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fBlockHeight);
private:
	D3DXMATRIX m_mtxworld;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	static LPDIRECT3DTEXTURE9 m_pTexBuff;
	LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXVECTOR3 m_nOld3DEnemyPos;
	D3DXVECTOR3 m_nSize;
	D3DXMATRIX m_mtxWorld;
	LPD3DXMESH m_pMesh[ENEMYAIR_MODEL];
	LPD3DXBUFFER m_pBuffMat[ENEMYAIR_MODEL];
	DWORD m_nNumMat[ENEMYAIR_MODEL];
	//Enemy m_aEnemy;
	LoadEnemy m_aLoadEnemy[ENEMYAIR_MODEL];
	ModelPartsEnemy m_aModel[ENEMYAIR_MODEL];
	MotionSet MotionSet[MAX_MOTION];
	bool bUse;
	bool m_bMoveSwitch;
	FILE* m_pFile;//ファイルポインタを宣言
	int m_nShotInterval; //弾の打つ間隔
	int m_nMoveInterval; //方向転換をする間隔
	int m_nLife; // 体力
	int m_nType; //敵タイプ
	int m_nMotionCnt; //モーション数
	int m_nFrameCnt; //モーションのフレーム数
	int m_nAirEnemyDmgColorTimer; //ダメージ時のマテリアルを変えている時間
	bool MotionUse; //モーションを使用しているか
	bool m_bAirEnemyDMGState;
protected:
	D3DXVECTOR3 m_n3DEnemyMove;
	D3DXVECTOR3 m_rot;

};

#endif