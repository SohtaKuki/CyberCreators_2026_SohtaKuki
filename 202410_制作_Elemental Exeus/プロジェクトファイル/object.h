//=================================================
//
// オブジェクトの処理 (object.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"
#include "manager.h"

//オブジェクトクラス
class CObject
{
public:
	typedef struct
	{
		D3DXVECTOR3 pos;
		float rhw;
		D3DCOLOR col;
		D3DXVECTOR2 tex;
	}VERTEX_2D;

	//頂点情報[3D]の構造体を定義
	typedef struct
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 nor;
		D3DCOLOR col;
		D3DXVECTOR2 tex;

	}VERTEX_3D;

	typedef enum
	{
		NONE = 0,
		ENEMY, //敵(2DObject用)
		ENEMY_NORMAL, //敵(3DObject用 ノーマル)
		ENEMY_NORMALMOVE, //敵(3DObject用 ノーマル)
		ENEMY_FLY, //敵(X座標移動)
		ENEMY_FLYMOVE, //敵(Z座標移動)
		START, //スタートマーカー
		GOAL, //ゴールマーカー
		PLAYER, //プレイヤー
		BLOCK, //ブロック
		BROKENBLOCK, //破壊可能ブロック
		MOVEBLOCK_X, //移動ブロック（X軸）
		MOVEBLOCK_X_REV, //移動ブロック (X軸反転)
		MOVEBLOCK_Z, //移動ブロック（Z軸）
		MOVEBLOCK_Z_REV, //移動ブロック（Z軸）
		WALL_WIDTH, //壁 (横長)
		WALL_HEIGHT, //壁 (縦長)
		WALL_WIDTH_SHORT,//壁 (横長ショート)
		WALL_HEIGHT_SHORT, //壁(縦長ショート)
		D2DITEM, //2DObject用アイテム
		TIMER, //タイマー
		FADE, //フェード
		D2DBULLET, //2D用プレイヤー弾
		PLAYER_BULLET, //プレイヤー通常弾
		CHBULLET, //プレイヤー特殊弾(チャージショット)
		TORNADO_ATK, //トルネードを前方に出す
		ENEMY_BULLET, //敵通常弾
		SLASHWIND, //プレイヤー近接攻撃
		SLASHWINDENEMY, //敵近接攻撃
		SLASHWINDBOSS, //ボス近接攻撃
		ITEM_ADDLIFE, //アイテム(体力回復)
		FLOOR, //床
		FLOOR_BG, //床2層目
		SCORE, //スコア
		PLAYER_EXPLOSION, //プレイヤー爆発
		ENEMY_EXPLOSION, //敵爆発
		SLASHTORNADO, //設置できる竜巻
		BOSS,//ボス
		BACKGROUND,//背景オブジェクト
		SWITCH_BS,//ボス用の入場検知オブジェクト
		BOSS_ATKUI, //ボスの攻撃マーカー
		PLAYER_EXPLOSION_SUB, //プレイヤーの爆発(サブ)
		ENEMY_EXPLOSION_SUB, //敵の爆発(サブ)
		PLAYER_EFCT_TORNADO, //プレイヤーの竜巻エフェクト
		ENEMY_EFCT_TORNADO, //敵の竜巻エフェクト
		BOSS_TORNADO, //ボス用の竜巻
		HIT_EFCT, //ヒット時のエフェクト
		RANKING, //ランキング
		MAX_TYPE,
	}TYPE;

	CObject(int nPriority = 3);
	virtual ~CObject();
	virtual HRESULT Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	static void ReleaseAll();
	static void UpdateAll();
	static void DrawAll();
	static const int MAX_OBJECT = 1024;	//オブジェクト最大数
	static CObject* GetObj(int nPriority ,int nIdx);
	TYPE GetType() { return m_type; }
	void SetType(TYPE type);
	void Release();
private:
	static CObject* m_apObject[MAX_PRIORITY][MAX_OBJECT];	//オブジェクト管理
	TYPE m_type;
	static int m_nNumAll;	//オブジェクトの総数
	int m_nID;	// 自分自身のID
	int m_nPriority;
protected:
};

#endif
