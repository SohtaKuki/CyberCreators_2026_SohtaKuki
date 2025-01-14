//=================================================
//
// 3Dモデルのブロックの表示処理 (3dblock.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "3dblock.h"
#include "3dplayer.h"

LPDIRECT3DTEXTURE9 C3dblock::m_pTexBuff = nullptr;
int C3dblock::m_nMaxBlock = 0;

//======================
// コンストラクタ
//======================
C3dblock::C3dblock(int nPriority) : CModel(nPriority)
{
    m_nTurnCnt = 0;
    m_bTurn = false;
}

//======================
// デストラクタ
//======================
C3dblock::~C3dblock()
{

}

//======================
// 初期化処理
//======================
HRESULT C3dblock::Init()
{
    //初期化
    if (FAILED(CModel::Init()))
    {
        return E_FAIL;
    }

    //ブロック数をカウント
    m_nMaxBlock++;

    return S_OK;
}

void C3dblock::Uninit()
{
    // すでに解放されている場合は処理をスキップ
    if (m_pTexBuff != nullptr) {
        m_pTexBuff->Release();
        m_pTexBuff = nullptr;
    }

    // メッシュやマテリアルの解放
    for (int i = 0; i < NUM_MODEL; i++) 
    {
        if (m_pMesh[i] != nullptr) 
        {
            m_pMesh[i]->Release();
            m_pMesh[i] = nullptr;
        }
        if (m_pBuffMat[i] != nullptr) 
        {
            m_pBuffMat[i]->Release();
            m_pBuffMat[i] = nullptr;
        }
    }

    // ブロック数を減らす
    m_nMaxBlock--;

    // 基底クラスの終了処理を呼び出す
    CModel::Uninit();
}

//======================
// 更新処理
//======================
void C3dblock::Update()
{
    for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY; nCntPriority++)
    {
        //ブロックの当たり判定
        for (int nCntObj = 0; nCntObj < C3dblock::MAX_BLOCK; nCntObj++)
        {
            CObject* pObj = CObject::GetObj(nCntPriority, nCntObj);

            if (pObj != nullptr)
            {
                CObject::TYPE type = pObj->GetType();

                //ブロックだった場合
                if (type == CObject::TYPE::BLOCK)
                {
                    D3DXVECTOR3 Pos = CObject3D::GetPos();


                    SetPos(Pos);

                    //X座標の移動量を更新
                    m_nMove.x += (0.0f - m_nMove.x) * 0.1f;

                    //Z座標の移動量を更新
                    m_nMove.z += (0.0f - m_nMove.z) * 0.1f;

                }

            }
        }
    }

    int nFadeState = CFade::GetFadeState();

    if (nFadeState == CFade::FADE_OUT)
    {

        C3dblock::Uninit();
    }
}

//======================
// 描画処理
//======================
void C3dblock::Draw()
{
    LPDIRECT3DDEVICE9 pDevice = nullptr;
    pDevice = CManager::GetRenderer()->GetDevice();

    D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス
    D3DMATERIAL9 matDef; //現在のマテリアル保存用
    D3DXMATERIAL* pMat; //マテリアルデータへのポインタ
    D3DXVECTOR3 Pos = CObject3D::GetPos();

    //ワールドマトリックスの初期化
    D3DXMatrixIdentity(&m_mtxworld);

    //向きを反映
    D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
    D3DXMatrixMultiply(&m_mtxworld, &m_mtxworld, &mtxRot);

    //位置を反映
    D3DXMatrixTranslation(&mtxTrans, Pos.x, Pos.y, Pos.z);
    D3DXMatrixMultiply(&m_mtxworld, &m_mtxworld, &mtxTrans);

    //ワールドマトリックスの設定
    pDevice->SetTransform(D3DTS_WORLD, &m_mtxworld);

    //マテリアルを取得
    pDevice->GetMaterial(&matDef);

    for (int nCntParts = 0; nCntParts < NUM_MODEL; nCntParts++)
    {
        //ワールドマトリックスの初期化
        D3DXMatrixIdentity(&m_aModel[nCntParts].mtxworld);

        //向きを反映
        D3DXMatrixRotationYawPitchRoll(&mtxRot, m_aModel[nCntParts].rot.y, m_aModel[nCntParts].rot.x, m_aModel[nCntParts].rot.z);
        D3DXMatrixMultiply(&m_aModel[nCntParts].mtxworld, &m_aModel[nCntParts].mtxworld, &mtxRot);

        //位置を反映
        D3DXMatrixTranslation(&mtxTrans, m_aModel[nCntParts].pos.x, m_aModel[nCntParts].pos.y, m_aModel[nCntParts].pos.z);
        D3DXMatrixMultiply(&m_aModel[nCntParts].mtxworld, &m_aModel[nCntParts].mtxworld, &mtxTrans);

        //親子関係
        D3DXMATRIX mtxParent;

        if (m_aModel[nCntParts].nIdxModelParent == -1)
        {
            mtxParent = m_mtxworld;
        }

        else
        {
            mtxParent = m_aModel[m_aModel[nCntParts].nIdxModelParent].mtxworld;
        }

        D3DXMatrixMultiply(&m_aModel[nCntParts].mtxworld, &m_aModel[nCntParts].mtxworld, &mtxParent);

        if (m_aModel[nCntParts].bUse == true)
        {
            //マトリックスの設定
            pDevice->SetTransform(D3DTS_WORLD, &m_aModel[nCntParts].mtxworld);

            pMat = (D3DXMATERIAL*)m_pBuffMat[nCntParts]->GetBufferPointer();

            for (int nCntMat = 0; nCntMat < (int)m_nNumMat[nCntParts]; nCntMat++)
            {
                //マテリアルの設定
                pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

                //テクスチャの設定
                pDevice->SetTexture(0, m_pTexBuff);

                //モデル(パーツ)の描画
                m_pMesh[nCntParts]->DrawSubset(nCntMat);
            }
        }

    }

    pDevice->SetMaterial(&matDef);
}

//======================
// オブジェクト生成処理
//======================
C3dblock* C3dblock::Create(D3DXVECTOR3 pos)
{
    C3dblock* D3DBlock = nullptr;

    D3DBlock = new C3dblock;

    //初期化に成功した場合
    if (SUCCEEDED(D3DBlock->Init()))
    {
        D3DBlock->SetType(TYPE::BLOCK);

        D3DBlock->LoadBlockData();

        //D3DBlock->Load();//テクスチャを設定(仮)

        D3DBlock->CObject3D::SetPos(pos);

        ////テクスチャの設定
        //Model->BindTexture(m_pTexBuff);


    }

    return D3DBlock;
}

//======================
// テクスチャロード処理
//======================
HRESULT C3dblock::Load()
{
    LPDIRECT3DDEVICE9 pDevice = nullptr;
    pDevice = CManager::GetRenderer()->GetDevice();


    if (FAILED(D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\samplepos.png", &m_pTexBuff)))
    {
        return E_FAIL;
    }

    return S_OK;
}

//======================
// テクスチャアンロード(終了)処理
//======================
void C3dblock::Unload()
{
    CModel::Unload();
}



//===========================
// 外部ファイル読み込み処理
//===========================
void C3dblock::LoadBlockData(void)
{
    char Datacheck[MAX_CHAR];
    int nCntEnemyData = 0;
    int EnemyModelSave = 0;

    m_pFile = fopen("data\\MODEL\\MODEL_Block\\block.txt", "r");//ファイルを開く

    //ファイルが存在しない場合
    if (m_pFile == NULL)
    {
        return;
    }

    //外部ファイル文字列読み取り
    while (1)
    {
        fscanf(m_pFile, "%s", Datacheck);

        if (!strcmp(Datacheck, "END_CHARACTERSET"))
        {
            fclose(m_pFile);
            break;
        }

        if (Datacheck[0] == '#')
        {
            continue;
        }

        //パーツ数
        if (!strcmp(Datacheck, "NUM_PARTS"))
        {
            fscanf(m_pFile, "%s", Datacheck);
            fscanf(m_pFile, "%d", &EnemyModelSave);
        }

        //ファイル名
        if (!strcmp(Datacheck, "MODEL_FILENAME"))
        {
            fscanf(m_pFile, "%s", Datacheck);
            fscanf(m_pFile, "%s", &CModel::m_aLoadEnemy[EnemyModelSave].filename[0]);
            EnemyModelSave++;
        }

        //パーツの詳細
        if (!strcmp(Datacheck, "PARTSSET"))
        {
            while (1)
            {
                fscanf(m_pFile, "%s", Datacheck);

                //パーツ詳細終わり検知
                if (!strcmp(Datacheck, "END_PARTSSET"))
                {
                    break;
                }

                //インデックス
                else if (!strcmp(Datacheck, "INDEX"))
                {
                    fscanf(m_pFile, "%s", Datacheck);
                    fscanf(m_pFile, "%d", &CModel::m_aLoadEnemy[nCntEnemyData].index);
                }

                //親設定
                else if (!strcmp(Datacheck, "PARENT"))
                {
                    fscanf(m_pFile, "%s", Datacheck);
                    fscanf(m_pFile, "%d", &CModel::m_aLoadEnemy[nCntEnemyData].parent);
                }

                //立ち位置
                else if (!strcmp(Datacheck, "POS"))
                {
                    fscanf(m_pFile, "%s", Datacheck);
                    fscanf(m_pFile, "%f", &CModel::m_aLoadEnemy[nCntEnemyData].pos.x);
                    fscanf(m_pFile, "%f", &CModel::m_aLoadEnemy[nCntEnemyData].pos.y);
                    fscanf(m_pFile, "%f", &CModel::m_aLoadEnemy[nCntEnemyData].pos.z);
                }

                //向き
                else if (!strcmp(Datacheck, "ROT"))
                {
                    fscanf(m_pFile, "%s", Datacheck);
                    fscanf(m_pFile, "%f", &CModel::m_aLoadEnemy[nCntEnemyData].rot.x);
                    fscanf(m_pFile, "%f", &CModel::m_aLoadEnemy[nCntEnemyData].rot.y);
                    fscanf(m_pFile, "%f", &CModel::m_aLoadEnemy[nCntEnemyData].rot.z);
                }
            }
            nCntEnemyData++;
        }
    }

    //デバイスの取得
    LPDIRECT3DDEVICE9 pDevice = nullptr;
    pDevice = CManager::GetRenderer()->GetDevice();

    //ここで上記の処理で得た数値を代入
    for (int nCnt = 0; nCnt < EnemyModelSave; nCnt++)
    {
        D3DXLoadMeshFromX(&CModel::m_aLoadEnemy[nCnt].filename[0], D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[nCnt], NULL, &m_nNumMat[nCnt], &m_pMesh[nCnt]);
        //D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\golden000.jpg", &g_pTextureEnemy);
        m_aModel[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        m_aModel[nCnt].nLife = 4;
        m_aModel[nCnt].nIdxModelParent = CModel::m_aLoadEnemy[nCnt].parent;
        m_aModel[nCnt].bUse = true;

        //敵の立ち位置初期化
        //m_nPos = D3DXVECTOR3(-0.0f, -0.0f, 0.0f);
        //m_aEnemy.rot = D3DXVECTOR3(-0.0f, 0.0f, 0.0f);

        m_aModel[nCnt].pos = D3DXVECTOR3(CModel::m_aLoadEnemy[nCnt].pos.x, CModel::m_aLoadEnemy[nCnt].pos.y, CModel::m_aLoadEnemy[nCnt].pos.z);
        m_aModel[nCnt].rot = D3DXVECTOR3(CModel::m_aLoadEnemy[nCnt].rot.x, CModel::m_aLoadEnemy[nCnt].rot.y, CModel::m_aLoadEnemy[nCnt].rot.z);
    }
}


//===========================
// ブロックの当たり判定
//===========================
bool C3dblock::Collision3DBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fHeight)
{
    bool bLanding = false; // 重力が適用された場合のみ使用
    float fMoveblockWidth = 25.0f;
    float fMoveblockDepth = 70.0f;
    float fMoveblockHeight = -15.0f;

    D3DXVECTOR3 Pos = CObject3D::GetPos();

    // 右側当たり判定
    if (pPos->x - fWidth <= Pos.x + fMoveblockWidth && pPosOld->x - fWidth >= Pos.x + fMoveblockWidth && pPos->z - fHeight < Pos.z + fMoveblockDepth && pPos->z > Pos.z - fMoveblockDepth && pPos->y < Pos.y + fMoveblockHeight && pPos->y > Pos.y - fMoveblockHeight)
    {
        pPos->x = Pos.x + fMoveblockWidth + fWidth;
    }
    // 左側当たり判定
    else if (pPos->x + fWidth >= Pos.x - fMoveblockWidth && pPosOld->x + fWidth <= Pos.x - fMoveblockWidth && pPos->z - fHeight < Pos.z + fMoveblockDepth && pPos->z > Pos.z - fMoveblockDepth && pPos->y < Pos.y + fMoveblockHeight && pPos->y > Pos.y - fMoveblockHeight)
    {
        pPos->x = Pos.x - fMoveblockWidth - fWidth;
    }
    // 前側当たり判定
    if (pPos->x - fWidth < Pos.x + fMoveblockWidth && pPos->x + fWidth > Pos.x - fMoveblockWidth && pPos->z - fHeight <= Pos.z + fMoveblockDepth && pPosOld->z - fHeight >= Pos.z + fMoveblockDepth && pPos->y < Pos.y + fMoveblockHeight && pPos->y > Pos.y - fMoveblockHeight)
    {
        pPos->z = Pos.z + fMoveblockDepth + fHeight;
    }
    // 後側当たり判定
    else if (pPos->x - fWidth < Pos.x + fMoveblockWidth && pPos->x + fWidth > Pos.x - fMoveblockWidth && pPos->z >= Pos.z - fMoveblockDepth && pPosOld->z <= Pos.z - fMoveblockDepth && pPos->y < Pos.y + fMoveblockHeight && pPos->y > Pos.y - fMoveblockHeight)
    {
        pPos->z = Pos.z - fMoveblockDepth - fHeight;
    }
    // 上側当たり判定
    if (pPos->x - fWidth < Pos.x + fMoveblockWidth && pPos->x + fWidth > Pos.x - fMoveblockWidth && pPos->y - fHeight <= Pos.y + fMoveblockHeight && pPosOld->y - fHeight >= Pos.y + fMoveblockHeight && pPos->z < Pos.z + fMoveblockDepth && pPos->z > Pos.z - fMoveblockDepth)
    {
        pPos->y = Pos.y + fMoveblockHeight + fHeight;
        bLanding = true;
    }
    // 下側当たり判定
    else if (pPos->x - fWidth < Pos.x + fMoveblockWidth && pPos->x + fWidth > Pos.x - fMoveblockWidth && pPos->y >= Pos.y - fMoveblockHeight && pPosOld->y <= Pos.y - fMoveblockHeight && pPos->z < Pos.z + fMoveblockDepth && pPos->z > Pos.z - fMoveblockDepth)
    {
        pPos->y = Pos.y - fMoveblockHeight - fHeight;
    }

    return bLanding;
}
