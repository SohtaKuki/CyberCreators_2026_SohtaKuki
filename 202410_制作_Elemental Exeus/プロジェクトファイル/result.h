//=================================================
//
// 2Dポリゴンの描画処理 (object2D.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _RESULT_H_
#define _RESULT_H_

#include "object.h"
#include "object2D.h"

//背景オブジェクトクラス
class CResultBG :public CObject2D
{
public:
	CResultBG(int nPriority = 50);
	~CResultBG()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	static CResultBG* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nType);
	void BindTexture(LPDIRECT3DTEXTURE9 pTex);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	LPDIRECT3DTEXTURE9 m_pTexBuff;
	D3DXVECTOR3 m_nPos;
	D3DXVECTOR3 m_nSize;
	bool bUse;
	int m_CurrentFrame = 0;           // 現在のフレーム
	int m_Numframes = 8;        // アニメーションの総フレーム数
	float m_FrameDuration = 0.5f; // フレームの表示時間
	float m_Frametimer = 0.0f;        // フレームの経過時間
};

#endif

