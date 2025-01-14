//=================================================
//
// 床の描画処理 (floorBG.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _FLOORBG_H_
#define _FLOORBG_H_

#include "object.h"
#include "objectX.h"


//オブジェクト2Dクラス
class CFloorBG : public CModel
{
public:
	CFloorBG(int nPriority = 9);
	~CFloorBG() override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	LPDIRECT3DVERTEXBUFFER9 GetBuff(void);
	static CFloorBG* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nType);
	HRESULT Load();
	static void Unload();
	bool Collision3DFloorBG(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fHeight);
	void SetPos(D3DXVECTOR3 pos);
	static unsigned const int MAX_TYPE = 2;	//テクスチャタイプ最大数
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	LPDIRECT3DTEXTURE9 m_pTexBuff;
	LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXVECTOR3 m_nPos;
	D3DXVECTOR3 m_nSize;
	D3DXMATRIX m_mtxWorld;
	int m_nType;
	bool bUse;
protected:
	D3DXVECTOR3 m_nMove;
	D3DXVECTOR3 m_rot;
};

#endif
