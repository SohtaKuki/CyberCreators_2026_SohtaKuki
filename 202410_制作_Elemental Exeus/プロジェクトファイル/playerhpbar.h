//=================================================
//
// 2Dポリゴンの描画処理 (object2D.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _PLAYER_HPBAR_H_
#define _PLAYER_HPBAR_H_

#include "object.h"
#include "object2D.h"

//背景オブジェクトクラス
class CPlayerHpBar :public CObject2D
{
public:
	CPlayerHpBar(int nPriority = 71);
	~CPlayerHpBar()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	static CPlayerHpBar* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size,int nBarType);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	LPDIRECT3DTEXTURE9 m_pTexBuff;
	D3DXVECTOR3 m_nPos;
	D3DXVECTOR3 m_nSize;
	bool bUse;
	int m_CurrentFrame = 0;           // 現在のフレーム
	int m_Numframes = 8;        // アニメーションの総フレーム数
	int m_nBarType = 0; //HPゲージの種別
	float m_FrameDuration = 0.5f; // フレームの表示時間
	float m_Frametimer = 0.0f;        // フレームの経過時間
	float m_fCurrentLife; //プレイヤー体力の現在値
	float m_fBossCurrentLife; //ボスの体力の現在値
	float m_fTorCurrentLife; //竜巻の残量の現在値
	float m_fTargetLife; //プレイヤー体力の目標値
	float m_fTargetBossLife; //ボス体力の目標値
	float m_fTargetTorLife; //竜巻の残量の目標値
	float m_fLifeLerpSpeed; //ゲージの減少速度
};

#endif

