//=================================================
//
// 2Dポリゴンの描画処理 (object2D.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _OBJECT_BG_H_
#define _OBJECT_BG_H_

#include "object.h"
#include "object2D.h"

//背景オブジェクトクラス
class CObjectBG :public CObject2D
{
public:
	CObjectBG();
	~CObjectBG()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	static CObjectBG* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	LPDIRECT3DTEXTURE9 m_pTexBuff;
	D3DXVECTOR3 m_nPos;
	D3DXVECTOR3 m_nSize;
	int AnimationPTN;
	bool bUse;
	int m_CurrentFrame = 0;           // 現在のフレーム
	int m_Numframes = 8;        // アニメーションの総フレーム数
	float m_FrameDuration = 0.5f; // フレームの表示時間
	float m_Frametimer = 0.0f;        // フレームの経過時間
};

#endif

