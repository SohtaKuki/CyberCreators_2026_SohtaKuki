//=================================================
//
// ビルボード描画式弾の処理 (3dtornadoeffect.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "3dfireeffect.h"
#include "3dblock.h"
#include "3denemy.h"
#include "score.h"
#include "camera.h"
#include "scene.h"
#include "bosscallui.h"
#include "3dboss.h"
#include "3dgoalobj.h"

//======================
// コンストラクタ
//======================
C3dfireeffect::C3dfireeffect(int nPriority) : CObject3D(nPriority)
{
	m_bUse = true;
	m_nPatternAnimExpl = 1;
}

//======================
// デストラクタ
//======================
C3dfireeffect::~C3dfireeffect()
{

}

//======================
// 初期化処理
//======================
HRESULT C3dfireeffect::Init()
{
	CRenderer* Renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	//テクスチャの読込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\fire000.png", &m_pTexture);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * 1, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	//頂点情報のポインタ
	VERTEX_3D* pVtx;

	//頂点バッファをロックし頂点情報時へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3((-m_nSize.x), (m_nSize.y), (m_nSize.z));
	pVtx[1].pos = D3DXVECTOR3((m_nSize.x), (m_nSize.y), (m_nSize.z));
	pVtx[2].pos = D3DXVECTOR3((-m_nSize.x), (-m_nSize.y), (m_nSize.z));
	pVtx[3].pos = D3DXVECTOR3((m_nSize.x), (-m_nSize.y), (m_nSize.z));

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラーの初期設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);



	pVtx += 4;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();


	// 0から6までの乱数を生成
	m_nPatternAnimExpl = rand() % 21;

	return S_OK;
}

//======================
//終了処理
//======================
void C3dfireeffect::Uninit()
{
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}

	CObject3D::Uninit();
}

//======================
//更新処理
//======================
void C3dfireeffect::Update()
{
	//フェードの状態を取得
	int nFadeState = CFade::GetFadeState();

	//フェードアウトしたら強制削除
	if (nFadeState == CFade::FADE_OUT)
	{
		C3dfireeffect::Uninit();
		return;
	}

	if (CScene::GetUpdateStat() == true)
	{
		//頂点情報のポインタ
		VERTEX_3D* pVtx;

		//頂点バッファをロックし頂点情報時へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//ボス演出時の処理
		if (CBossCallUI::GetEndStat() == true && C3dboss::GetBossEntry() == false && C3dgoalobj::GetStageNum() == 2)
		{
			m_bUse = false;
		}

		//ボス演出が終了した際の処理
		if (CBossCallUI::GetEndStat() == true && C3dboss::GetBossEntry() == true && C3dgoalobj::GetStageNum() == 2)
		{
			m_bUse = true;
		}


		if (m_bUse == true)
		{

			m_nCounterAnim++;

			if (m_nCounterAnim % C3dfireeffect::DELAY_ANIM == 0)
			{
				m_nCounterAnim = 0;

				//アニメ切り替え処理
				m_nPatternAnimExpl++;


				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2((1.0f / EFCT_TORNADO_PATTERN_ANIM) * m_nPatternAnimExpl - (1.0f / EFCT_TORNADO_PATTERN_ANIM), 0.0f);
				pVtx[1].tex = D3DXVECTOR2((1.0f / EFCT_TORNADO_PATTERN_ANIM) * m_nPatternAnimExpl, 0.0f);
				pVtx[2].tex = D3DXVECTOR2((1.0f / EFCT_TORNADO_PATTERN_ANIM) * m_nPatternAnimExpl - (1.0f / EFCT_TORNADO_PATTERN_ANIM), 1.0f);
				pVtx[3].tex = D3DXVECTOR2((1.0f / EFCT_TORNADO_PATTERN_ANIM) * m_nPatternAnimExpl, 1.0f);
			}

			if (m_nPatternAnimExpl >= 17)
			{
				m_nPatternAnimExpl = 1;
			}


		}

		pVtx += 4;

		////エフェクトを竜巻に追従する
		//for (int nCntObj = 0; nCntObj < 80; nCntObj++)
		//{
		//	CObject* pObj = CObject::GetObj(3, nCntObj);

		//	if (pObj != nullptr)
		//	{
		//		CObject::TYPE type = pObj->GetType();

		//		m_p3dTornado = (C3dtornado*)pObj;

		//		if (type == CObject::TYPE::SLASHTORNADO)
		//		{
		//			D3DXVECTOR3 EnemyPos = m_p3dTornado->GetPos();

		//			m_nPos.x = EnemyPos.x;
		//		}
		//	}
		//}

		//頂点バッファをアンロック
		m_pVtxBuff->Unlock();
	}
}

//======================
// 描画処理
//======================
void C3dfireeffect::Draw()
{
	CRenderer* Renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	D3DXMATRIX mtxTrans;	//計算用マトリックス

	//ビューマトリックス取得
	D3DXMATRIX mtxView;

	//ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//=========================
	// 壁めり込み防止コード
	//=========================

	//Zの比較方法変更
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	//Zバッファに書き込まない
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//アルファテストを有効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//ビューマトリックス取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ポリゴンをカメラの正面に向け続ける
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);	//逆行列を求める

	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	D3DXMatrixTranslation(&mtxTrans, m_nPos.x, m_nPos.y, m_nPos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//使用済みの場合のみ通す
	if (m_bUse == true)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, m_pTexture);

		//ビルボードの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}


	//アルファテストを元に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


	//=========================
	// 壁めり込み防止コード
	//=========================

	//Zの比較方法変更
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	//Zバッファに書き込む
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//======================
// オブジェクト生成処理
//======================
C3dfireeffect* C3dfireeffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, int type)
{
	C3dfireeffect* fireeffect3d = new C3dfireeffect;

	fireeffect3d->m_nPos = pos;

	fireeffect3d->m_rot = rot;

	fireeffect3d->m_nSize = size;

	//初期化に成功した場合
	if (SUCCEEDED(fireeffect3d->Init()))
	{
		fireeffect3d->m_nType = type;

		if (type == 0)
		{
			fireeffect3d->SetType(TYPE::PLAYER_EFCT_TORNADO);
		}

		if (type == 1)
		{
			fireeffect3d->SetType(TYPE::ENEMY_EFCT_TORNADO);
		}

		fireeffect3d->m_nLife = 180;


	}

	return fireeffect3d;
}

void C3dfireeffect::SetExplosionSub(D3DXVECTOR3 pos, D3DXCOLOR col)
{

}