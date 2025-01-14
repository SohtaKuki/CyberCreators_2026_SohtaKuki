//=================================================
//
// 3Dモデルのプレイヤーの表示処理 (3dplayer.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _3DPLAYER_H_
#define _3DPLAYER_H_

#include "object3D.h"
#include "objectX.h"

#define MAX_CHAR (256) //行の読み込む数
#define NUM_MODEL (16) //モデルの数

static const float PLAYER_MOVE_SPD = 0.4f; //プレイヤーの移動速度
static const float PLAYER_MOVE_BOOST = 1.2f; //プレイヤーの移動速度上昇の倍率


//オブジェクト3Dクラス
class C3dplayer : public CModel
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

	C3dplayer(int nPriority = 3);
	~C3dplayer() override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	//void SetPlayerPos();
	static C3dplayer* Create(D3DXVECTOR3 pos);
	//void BindTexture(LPDIRECT3DTEXTURE9 pTex);
	static HRESULT Load();
	static void Unload();
	void PlayerDamage(int nDamage);
	void LoadPlayerData();
	void PlayerWalkSound();
	static int GetPlayerLife() {return m_nLife ; }
	D3DXVECTOR3& GetPlayerPos() { return m_nOld3DPlayerPos; }//座標の取得
	static bool GetShotButtonPreesed() { return m_bAButtonPressed; }//射撃ボタンの押下状態を取得
	static const int PLAYER_LIFE = 48;
private:
	D3DXMATRIX m_mtxworld;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	static LPDIRECT3DTEXTURE9 m_pTexBuff;
	LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXVECTOR3 m_nOld3DPlayerPos;
	D3DXVECTOR3 m_nSize;
	D3DXMATRIX m_mtxWorld;
	LPD3DXMESH m_pMesh[NUM_MODEL];
	LPD3DXBUFFER m_pBuffMat[NUM_MODEL];
	DWORD m_nNumMat[NUM_MODEL];
	//Enemy m_aEnemy;
	LoadEnemy m_aLoadEnemy[NUM_MODEL];
	ModelPartsEnemy m_aModel[NUM_MODEL];
	bool bUse;
	DWORD m_bAButtonPressStartTime; //ボタンを押した時刻
	static bool m_bAButtonPressed; //ボタンを押したかどうか(長押し処理)
	FILE* m_pFile;//ファイルポインタを宣言
	bool m_bPlayerBuff; //プレイヤーのバフが付与されているか
	bool m_bInstantShot; //チャージショットが有効かどうか
	bool m_bWalkSound;
	int m_nBuffTime; //プレイヤーのバフ付与時間
	int m_nInstantShotTime; //チャージショットを打った時間
	int m_nJumpCnt; //ジャンプした回数
	int m_nWalkSoundCnt; //歩行SEの再生する間隔
	
	static int m_nLife;
protected:
	D3DXVECTOR3 m_n3DPlayerMove;
	D3DXVECTOR3 m_rot;

};

#endif