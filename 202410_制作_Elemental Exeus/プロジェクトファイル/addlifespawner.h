//=================================================
//
// ステージマネージャー処理 (stagemanager.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _ADDLIFE_SPNER_H_
#define _ADDLIFE_SPNER_H_

#include "main.h"
#include "object3D.h"

class CAddLifeSpawner : public CObject3D
{
public:

	//UI表示構造体
	typedef enum
	{
		ADDLIFE_POS_R_LOWER = 0,
		ADDLIFE_POS_L_LOWER,
		ADDLIFE_POS_R_UPPER,
		ADDLIFE_POS_L_UPPER,
		ADDLIFE_POS_MAX,
	}ADDLIFE_SPAWNER_POS;

	CAddLifeSpawner();
	virtual ~CAddLifeSpawner();
	static HRESULT Init();
	void Uninit();
	void Update() override;
	void Draw();
	static CAddLifeSpawner* Create(int nType);
	const static int MAX_OBJ = 1000;
private:
	FILE* m_pFile;//ファイルポインタを宣言
	int m_nType;
	int m_nTimer;
	int m_nRandSelect;
};

#endif