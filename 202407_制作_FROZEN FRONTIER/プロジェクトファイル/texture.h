//=================================================
//
// テクスチャを一括管理する処理 (texture.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "main.h"

class CTexture
{
public:
	CTexture();
	~CTexture();
	void Unload();
	LPDIRECT3DTEXTURE9 GetAdress(int nIdx);
private:

};

#endif
