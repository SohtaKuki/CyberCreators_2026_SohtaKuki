//=================================================
//
// ビルボードのヘッダーファイル (billboard.h)
// Author: Sohta Kuki
//
//=================================================
#ifndef _MESHFIELD_H_	
#define _MESHFIELD_H_	

#include "object3D.h"
#include  "objectX.h"

//オブジェクト2Dクラス
class CMeshField :public CObject3D
{
public:

	const int MESHFIELD_X_BLOCK = 2;
	const int MESHFIELD_Z_BLOCK = 2;

	const int  MESHFIELD_VERTEX_NUM = 14;
	const int  MESHFIELD_INDEX_NUM = 14;
	const int  MESHFIELD_PRIMITIVE_NUM = 12;

	// 定数（ドームの分割数）
	const int LATITUDE_DIVISIONS = 10; // 緯度の分割数
	const int LONGITUDE_DIVISIONS = 20; // 経度の分割数


	typedef struct
	{
		D3DXVECTOR3 pos;	// 座標
		D3DXVECTOR3 rot;	//向き
	}Meshfield;

	CMeshField(int nPriority = 1);
	~CMeshField() override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	Meshfield GetMeshfield(void);
	static CMeshField* Create(D3DXVECTOR3 pos);
private:

	D3DXVECTOR3 m_nPos;
	D3DXVECTOR3 m_nSize;

	const int sphereDiv = 10;  // 球の分割数
	float m_fRadius;
	float m_fTexMoveSPD;
	static LPDIRECT3DTEXTURE9 m_pTextureMeshfield;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffMeshfield;
	LPDIRECT3DINDEXBUFFER9  m_pldxBuffMeshField;
	D3DXMATRIX m_mtxWorldMeshfield;
	Meshfield m_Meshfield;

};

#endif