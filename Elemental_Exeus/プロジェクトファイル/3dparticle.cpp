//=================================================
//
// 3Dパーティクルの処理 (3dmeshfield.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "3dparticle.h"

//======================
// コンストラクタ
//======================
CParticle::CParticle(int nPriority) : CObject3D(nPriority)
{
    // 初期化
    m_vVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_vAcceleration = D3DXVECTOR3(0.0f, -0.001f, 0.0f); // 重力加速度をシミュレーション
    m_fLifeTime = 1.0f; // 寿命を設定（1秒）
    m_fAlphaDecay = 1.0f; // アルファ値の減少量
}

//======================
// デストラクタ
//======================
CParticle::~CParticle()
{
}

//======================
// 初期化処理
//======================
HRESULT CParticle::Init()
{
    CRenderer* Renderer = CManager::GetRenderer();
    LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

    //テクスチャの読込み
    D3DXCreateTextureFromFile(pDevice, "data\\texture\\billboard000.png", &m_pTexture);

    //頂点バッファの生成
    pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * 1, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

    //頂点情報のポインタ
    VERTEX_3D* pVtx;

    //頂点バッファをロックし頂点情報時へのポインタを取得
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    //頂点座標の設定
    pVtx[0].pos = D3DXVECTOR3(-m_nSize.x, m_nSize.y, m_nSize.z);
    pVtx[1].pos = D3DXVECTOR3(m_nSize.x, m_nSize.y, m_nSize.z);
    pVtx[2].pos = D3DXVECTOR3(-m_nSize.x, -m_nSize.y, m_nSize.z);
    pVtx[3].pos = D3DXVECTOR3(m_nSize.x, -m_nSize.y, m_nSize.z);

    //法線ベクトルの設定
    pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

    //頂点カラーの初期設定
    pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

    //テクスチャ座標の初期設定
    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

    pVtx += 4;

    //頂点バッファをアンロック
    m_pVtxBuff->Unlock();

    return S_OK;
}

//======================
// 終了処理
//======================
void CParticle::Uninit()
{
    CObject3D::Uninit();
}

//======================
// 更新処理
//======================
void CParticle::Update()
{

    //// 寿命の減少
    //m_fLifeTime -= 0.01f;

    //// 速度と位置の更新
    //m_vVelocity += m_vAcceleration;
    //m_nPos += m_vVelocity;

    //// アルファ値の減少
    //float alpha = max(0.0f, m_fLifeTime * m_fAlphaDecay);

    //// サイズの減少
    //float sizeFactor = max(0.1f, m_fLifeTime); // 寿命が短くなるほどサイズが小さくなる
    //m_nSize = D3DXVECTOR3(sizeFactor, sizeFactor, sizeFactor);

    // 頂点バッファのロック
    VERTEX_3D* pVtx;
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


    // サイズを考慮した頂点座標の設定
    pVtx[0].pos = D3DXVECTOR3(-m_nSize.x, m_nSize.y, m_nSize.z);
    pVtx[1].pos = D3DXVECTOR3(m_nSize.x, m_nSize.y, m_nSize.z);
    pVtx[2].pos = D3DXVECTOR3(-m_nSize.x, -m_nSize.y, m_nSize.z);
    pVtx[3].pos = D3DXVECTOR3(m_nSize.x, -m_nSize.y, m_nSize.z);

    // 頂点カラーの設定
    pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


    // 頂点バッファをアンロック
    m_pVtxBuff->Unlock();

    // パーティクルの消滅
    if (m_fLifeTime <= 0.0f)
    {
        this->Uninit();
    }
}


//======================
// 描画処理
//======================
void CParticle::Draw()
{
    CRenderer* Renderer = CManager::GetRenderer();
    LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

    D3DXMATRIX mtxTrans;	//計算用マトリックス

    //ビューマトリックス取得
    D3DXMATRIX mtxView;

    ////ライトを無効にする
    //pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    //=========================
    // 壁めり込み防止コード
    //=========================

    ////Zの比較方法変更
    //pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

    ////Zバッファに書き込まない
    //pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

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

    //テクスチャの設定
    pDevice->SetTexture(0, m_pTexture);

    //ビルボードの描画
    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

    //アルファテストを元に戻す
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//======================
// オブジェクト生成処理
//======================
CParticle* CParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    CParticle* Particle = new CParticle;

    // 初期化に成功した場合
    if (SUCCEEDED(Particle->Init()))
    {
        Particle->m_nPos = pos;
        Particle->m_nSize = size;
    }

    return Particle;
}
