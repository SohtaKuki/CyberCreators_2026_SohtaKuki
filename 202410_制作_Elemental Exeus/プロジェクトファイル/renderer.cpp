//=================================================
//
// レンダラーの処理 (renderer.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "renderer.h"
#include "object.h"

//======================
// コンストラクタ
//======================
CRenderer::CRenderer()
{
	m_pD3D = nullptr;
	m_pD3DDevice = nullptr;
}

//======================
// デストラクタ
//======================
CRenderer::~CRenderer()
{

}

//======================
// 初期化処理
//======================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;
	D3DPRESENT_PARAMETERS d3dpp;

	// Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);


	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						
	d3dpp.BackBufferFormat = d3ddm.Format;		
	d3dpp.BackBufferCount = 1;									
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					
	d3dpp.EnableAutoDepthStencil = TRUE;						
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					
	d3dpp.Windowed = bWindow;									
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,D3DCREATE_HARDWARE_VERTEXPROCESSING,&d3dpp,&m_pD3DDevice)))
	{

	}
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return S_OK;
}

//======================
// 終了処理
//======================
void CRenderer::Uninit()
{
	 
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//======================
// 更新処理
//======================
void CRenderer::Update()
{
	CObject::UpdateAll();

	CManager::GetFade()->Update();
}

//======================
// 描画処理
//======================
void CRenderer::Draw()
{
	
	m_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0); //ここでワールドの色を変える

	 
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		CManager::GetCamera()->SetCamera();

		// 全て描画
		CObject::DrawAll();

		CManager::GetFade()->Draw();

		m_pD3DDevice->EndScene();
	}

	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//======================
// デバイス取得
//======================
LPDIRECT3DDEVICE9 CRenderer::GetDevice()
{
	return m_pD3DDevice;
}