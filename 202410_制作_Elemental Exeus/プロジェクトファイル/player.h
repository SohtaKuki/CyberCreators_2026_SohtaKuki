//=================================================
//
// 2Dポリゴンの描画処理 (object2D.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "object.h"
#include "object2D.h"

static const float Angle_value1(0.25f); //ポリゴン角度調整(値1)
static const float Angle_value2(0.75f); //ポリゴン角度調整(値2)
static const float Angle_value3(0.35f); //ポリゴン角度調整 (値3)
static const float Length_value1(3.0f); //ポリゴン長さ調整 (値1)
static const float Length_value2(0.0f); //ポリゴン長さ調整 (値2)
static const float Attenuation_value(0.10f); //ポリゴン移動量減衰係数

//オブジェクト2Dクラス
class CPlayer :public CObject2D
{
public:
	CPlayer(int nPriority = 3);
	~CPlayer()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static CPlayer* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	static const int CP_IMAGE_PATTERN_ANIM = 8;  //アニメのパターン数
	static const int CP_DELAY_ANIM = 20;//アニメの待機時間
	static const int BLOCK_CLLISION = 550;

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	LPDIRECT3DTEXTURE9 m_pTexBuff;
	int AnimationPTN;
	bool bUse;
	int m_CurrentFrame;           // 現在のフレーム
	int m_Numframes;        // アニメーションの総フレーム数
	float m_FrameDuration; // フレームの表示時間
	float m_Frametimer;        // フレームの経過時間
	float m_fLengthPlayer; //対角線の長さ
	float m_fAnglePlayer; //対角線の角度
	float m_PolygonMoveSpeed;	// ポリゴンの移動速度
	D3DXVECTOR3 m_movePlayer;
	D3DXVECTOR3 m_rotPlayer;
	D3DXVECTOR3 m_nPlayerSize;
	D3DXVECTOR3 m_nOldPlayerPos;
	
};

#endif
