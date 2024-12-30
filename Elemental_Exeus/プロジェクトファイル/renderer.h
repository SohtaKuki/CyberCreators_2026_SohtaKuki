//=================================================
//
// レンダラーの処理 (renderer.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include"d3dx9.h"
#include <cmath>

//レンダークラス
class CRenderer
{
public:
	CRenderer();
	~CRenderer();
	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit();
	void Update();
	void Draw();
	LPDIRECT3DDEVICE9 GetDevice(); //3Dデバイスの取得
private:
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pD3DDevice;
};

#endif