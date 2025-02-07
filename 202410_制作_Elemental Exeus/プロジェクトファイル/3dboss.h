//=================================================
//
// 3Dモデルのプレイヤーの表示処理 (3dplayer.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _3DBOSS_H_
#define _3DBOSS_H_

#include "object3D.h"
#include "objectX.h"

#define MAX_CHAR (256) //行の読み込む数
#define BOSS_MODEL (9) //モデルの数
#define MAX_KEYSET (30) // キーの最大値
#define MAX_MOTION (15) //モーション

static const float BOSS_MOVE_SPD = 0.38f; //プレイヤーの移動速度
static const float BOSS_MOVE_BOOST = 1.3f; //プレイヤーの移動速度上昇の倍率


//オブジェクト3Dクラス
class C3dboss : public CModel
{
public:

	//ボスのモーション
	typedef enum
	{
		MOTION_NORMAL = 0,
		MOTION_WALK,
		MOTION_ACTION,
		MOTION_JUMP,
		MOTION_LAND,
		MOTION_ACTION2,
		MOTION_DEAD,
		MOTION_ACTION_WIRED,
		MOTION_SP_AIRATK,
		MOTION_ENTRY,
		MOTION_MELEEATK,
		MOTION_TORUPATK,
		MOTION_SHORTMELEEATK,
		MOTION_MAX
	}BOSSMOTION;

	//ボスの攻撃
	typedef enum
	{
		ACTION_NONE = 0,
		ACTION_DASHATK,
		ACTION_WIREDATK,
		ACTION_UPPERATK,
		ACTION_MELEE,
		ACTION_SPATK,
		ACTION_UP,
		ACTION_DOWN,
		ACTION_TORATK,
		ACTION_SHORTMELEE,
		ACTION_MAX
	}BOSSACTION;

	//ボスの立ち位置
	typedef enum
	{
		POSITION_NOPOS = 0,
		POSTION_LEFT,
		POSTION_RIGHT,
		POSTION_MAX,
	}BOSS_LRPOSTION;

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
		Key aKey[BOSS_MODEL];
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

	C3dboss(int nPriority = 20);
	~C3dboss() override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	void ModelMotion();
	//void SetPlayerPos();
	static C3dboss* Create(D3DXVECTOR3 pos);
	//void BindTexture(LPDIRECT3DTEXTURE9 pTex);
	static HRESULT Load();
	static void Unload();
	void BossDamage(int nDamage);
	void LoadPlayerData();
	void PlayerWalkSound();
	static int GetBossLife() { return m_nLife; }
	D3DXVECTOR3& GetBossPos() { return m_nOld3DBossPos; }//座標の取得
	static bool GetBossEntry() { return m_bEntry; }
	static D3DXVECTOR3& GetBossPosCamera() { return m_nPos; }//座標の取得
	static const int BOSS_LIFE = 520;
	BOSSMOTION Motion;
	void SetPlayerMotion(BOSSMOTION motion);
	bool Collision3DBoss(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fBlockHeight);
	static D3DXVECTOR3 GetBossRot() {return m_rot ; }
	static bool GetBossDeathState() { return m_bDeathSwitch; } //撃破されたどうか
private:
	D3DXMATRIX m_mtxworld;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	static LPDIRECT3DTEXTURE9 m_pTexBuff;
	LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXVECTOR3 m_nOld3DBossPos;
	static D3DXVECTOR3 m_nPos;
	D3DXMATRIX m_mtxWorld;
	LPD3DXMESH m_pMesh[BOSS_MODEL];
	LPD3DXBUFFER m_pBuffMat[BOSS_MODEL];
	DWORD m_nNumMat[BOSS_MODEL];
	//Enemy m_aEnemy;
	LoadEnemy m_aLoadEnemy[BOSS_MODEL];
	ModelPartsEnemy m_aModel[BOSS_MODEL];
	MotionSet MotionSet[MAX_MOTION];
	static D3DVERTEXELEMENT9 m_aElements[MAX_FVF_DECL_SIZE];
	bool bUse;
	FILE* m_pFile;//ファイルポインタを宣言
	bool m_bWalkSound; //歩行音をならすかどうか
	int m_nJumpCnt; //ジャンプした回数
	int m_nWalkSoundCnt; //歩行SEの再生する間隔
	int m_nMotionCnt; //モーション数
	int m_nFrameCnt; //モーションのフレーム数
	static int m_nLife; //プレイヤーの体力
	int m_nNextMotion;             // 次のモーションID
	int m_nJumpAnimTimer;
	int m_nComboNum; //コンボ数
	int m_nFlyTimer; //空中浮遊時間
	int m_nEndTimer; //非表示になった際の時間計測
	bool m_bWalking; //歩行中かどうか
	bool m_bJumping; //ジャンプ中かどうか
	bool MotionUse; //モーションを使用しているか
	bool m_bIsCollision; //当たり判定
	bool m_bFlying; //空中浮遊しているか
	bool m_bDashAtk; //ダッシュアタック攻撃
	bool m_bWiredAtk; //両端に竜巻を出しているか
	bool m_bFlyAtk; //空中竜巻攻撃
	bool m_bBossUPzone; //ボスが上にいるかどうか
	bool m_bBossDownzone; //ボスが下にいるかどうか
	bool m_bBossSPATK; //特殊攻撃をしているかどうか
	bool m_bDashATKDamage; //突進攻撃のダメージを受けたか
	static bool m_bDeathSwitch; //撃破されたどうか
	static bool m_bEntry; //ボスの登場が終わってるかどうか
	int m_nLRPos; //左右のどちらにいるか
	float m_fRotSpeed; //回転速度
	int m_nACTIONTimer; //各行動ごとの間隔
	int random_number; //ボスの行動変数
	int m_nBossDmgColorTimer; //ダメージ時のマテリアルを変えている時間
	bool m_bBossPowerUP; //ボスが強力な攻撃を打てるかどうか
	bool m_bBossDMGState; //ボスがダメージを受けているか
	bool m_bATKCollisonState; //近接攻撃の当たり判定を作動させるか
	D3DXVECTOR3 m_n3DBossMove;
	static D3DXVECTOR3 m_rot;


};

#endif