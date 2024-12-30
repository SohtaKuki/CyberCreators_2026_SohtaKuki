//=================================================
//
// 床の描画処理 (floor.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _FLOOR_H_
#define _FLOOR_H_

#include "object.h"
#include "objectX.h"


//オブジェクト2Dクラス
class CFloor : public CModel
{
public:
	CFloor(int nPriority = 9);
	~CFloor() override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	LPDIRECT3DVERTEXBUFFER9 GetBuff(void);
	static CFloor* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size,int nType);
	HRESULT Load(LPCSTR textureFileName);
	static void Unload();
	bool Collision3DFloor(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fHeight);
	void SetPos(D3DXVECTOR3 pos);
	static unsigned const int MAX_TYPE = 2;	//テクスチャタイプ最大数
	static unsigned const int DIVISION = 10; // テクスチャの分割数

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	LPDIRECT3DTEXTURE9 m_pTexBuff;
	LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;
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
