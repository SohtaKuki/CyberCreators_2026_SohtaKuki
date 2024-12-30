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
#define PLAYER_MODEL (15) //モデルの数
#define MAX_KEYSET (20) // キーの最大値
#define MAX_MOTION (20) //モーション

static const float PLAYER_MOVE_SPD = 0.4f; //プレイヤーの移動速度
static const float PLAYER_MOVE_BOOST = 1.3f; //プレイヤーの移動速度上昇の倍率


//オブジェクト3Dクラス
class C3dplayer : public CModel
{
public:

	typedef enum
	{
		MOTION_NORMAL = 0,
		MOTION_WALK,
		MOTION_ACTION,
		MOTION_JUMP,
		MOTION_LAND,
		MOTION_ACTION2,
		MOTION_ACTION3,
		MOTION_TORNADO,
		MOTION_MAX
	}PLAYERMOTION;

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
		D3DXVECTOR3 targetPos;
		D3DXVECTOR3 targetRot;
		D3DXVECTOR3 rot;
		LPDIRECT3DTEXTURE9 m_pTexture;
		int nLife;
		bool bUse;
		int nCounterState;
		int nIdxModelParent;	//親モデルのインデックス

	}ModelPartsEnemy;

	//キーの構造体
	typedef struct {
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
	}Key;

	//キーセットの構造体
	typedef struct {
		int Frame;
		Key aKey[PLAYER_MODEL];
	}KeySet;

	//モーションの構造体
	typedef struct {
		bool Loop;
		int NumKey;
		KeySet KeySet[MAX_KEYSET];
	}MotionSet;

	typedef enum
	{
		DOUBLE_JUMP = 0,
		FREE_FLY,
		JUMP_TYPE_MAX
	}JUMP_TYPE;

	C3dplayer(int nPriority = 3);
	~C3dplayer() override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	void ModelMotion();
	//void SetPlayerPos();
	static C3dplayer* Create(D3DXVECTOR3 pos);
	//void BindTexture(LPDIRECT3DTEXTURE9 pTex);
	static HRESULT Load();
	static void Unload();
	void PlayerDamage(int nDamage);
	void LoadPlayerData();
	void PlayerWalkSound();
	static int GetPlayerLife() {return m_nLife ; }
	static int GetTorNum() { return m_nDisplayTorNum; }
	D3DXVECTOR3& GetPlayerPos() { return m_nOld3DPlayerPos; }//座標の取得
	static D3DXVECTOR3& GetPlayerRot() { return m_rot; }//座標の取得
	static bool GetShotButtonPreesed() { return m_bAButtonPressed; }//射撃ボタンの押下状態を取得
	static const int PLAYER_LIFE = 48;
	static const int JUMP_TIMER = 7;
	PLAYERMOTION Motion;
	void SetPlayerMotion(PLAYERMOTION motion);
private:
	D3DXMATRIX m_mtxworld;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	static LPDIRECT3DTEXTURE9 m_pTexBuff;
	LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXVECTOR3 m_nOld3DPlayerPos;
	D3DXVECTOR3 m_nSize;
	D3DXMATRIX m_mtxWorld;
	LPD3DXMESH m_pMesh[PLAYER_MODEL];
	LPD3DXBUFFER m_pBuffMat[PLAYER_MODEL];
	DWORD m_nNumMat[PLAYER_MODEL];
	static D3DVERTEXELEMENT9 m_aElements[MAX_FVF_DECL_SIZE];
	//Enemy m_aEnemy;
	LoadEnemy m_aLoadEnemy[PLAYER_MODEL];
	ModelPartsEnemy m_aModel[PLAYER_MODEL];
	MotionSet MotionSet[MAX_MOTION];
	bool bUse;
	DWORD m_bAButtonPressStartTime; //ボタンを押した時刻
	static bool m_bAButtonPressed; //ボタンを押したかどうか(長押し処理)
	FILE* m_pFile;//ファイルポインタを宣言
	bool m_bPlayerBuff; //プレイヤーのバフが付与されているか
	bool m_bInstantShot; //チャージショットが有効かどうか
	bool m_bWalkSound; //歩行音をならすかどうか
	bool m_bSuperJump; //大ジャンプが可能かどうか
	int m_nBuffTime; //プレイヤーのバフ付与時間
	int m_nInstantShotTime; //チャージショットを打った時間
	int m_nJumpCnt; //ジャンプした回数
	int m_nWalkSoundCnt; //歩行SEの再生する間隔
	int m_nAtkCombo; //攻撃ボタンを押した回数
	int m_nAirStepTimer;
	float m_fAtkTimer; //コンボ攻撃のタイマー
	int m_nFreeFlyTimer; //空中浮遊のタイマー
	int m_nJumpType; //ジャンプ種別
	int m_nMotionCnt; //モーション数
	int m_nFrameCnt; //モーションのフレーム数
	static int m_nLife; //プレイヤーの体力
	bool MotionUse; //モーションを使用しているか
	bool m_bIsCollision;
	bool m_bIsTorCollision;
	bool m_bAirStep;
	int m_nNextMotion;             // 次のモーションID
	int m_nJumpAnimTimer;
	bool m_bWalking; //歩行中かどうか
	bool m_bJumping; //ジャンプ中かどうか
	bool m_bTorColision;
	bool m_bUse[PLAYER_MODEL];
	int m_nComboNum; //コンボ数
	bool m_bTorUPPER;
	int m_nJumpTimer; //ジャンプ中の時間
	static int m_nDisplayTorNum;

protected:
	D3DXVECTOR3 m_n3DPlayerMove;
	static D3DXVECTOR3 m_rot;

};

#endif