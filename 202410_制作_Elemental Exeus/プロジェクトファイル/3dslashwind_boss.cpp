//=================================================
//
// 3Dモデルのブロックの表示処理 (3dslashwindBoss.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "3dslashwind_boss.h"
#include "3dplayer.h"
#include "3denemy.h"
#include "3dtornado.h"
#include "3dboss.h"
#include "3dhiteffect.h"

LPDIRECT3DTEXTURE9 C3dslashwindBoss::m_pTexBuff = nullptr;
int C3dslashwindBoss::m_nMaxBlock = 0;
bool C3dslashwindBoss::m_bComboDest = false;

//======================
// コンストラクタ
//======================
C3dslashwindBoss::C3dslashwindBoss(int nPriority) : CModel(nPriority)
{
    m_nTurnCnt = 0;
    m_bTurn = false;
}

//======================
// デストラクタ
//======================
C3dslashwindBoss::~C3dslashwindBoss()
{

}

//======================
// 初期化処理
//======================
HRESULT C3dslashwindBoss::Init()
{
    //初期化
    if (FAILED(CModel::Init()))
    {
        return E_FAIL;
    }

    m_nTimer = 0;
    m_bDisplay = true;

    return S_OK;
}

void C3dslashwindBoss::Uninit()
{
    // すでに解放されている場合は処理をスキップ
    if (m_pTexBuff != nullptr)
    {
        m_pTexBuff->Release();
        m_pTexBuff = nullptr;
    }

    // メッシュやマテリアルの解放
    for (int i = 0; i < SLASHWINDBOSS_MODEL; i++)
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
void C3dslashwindBoss::Update()
{
    D3DXVECTOR3 Pos = CObject3D::GetPos();
    D3DXVECTOR3 PlayerRot = C3dplayer::GetPlayerRot();


    SetPos(Pos);

    //自然消滅の処理
    if (m_bDisplay == true)
    {
        m_nTimer++;

        m_rot.y += 0.3f;

        if (m_nTimer == SLASHWINDBOSS_TIMER)
        {
            m_bDisplay = false;
            Uninit();
        }

    }

    //コンボ攻撃の前回出した竜巻の削除
    if (m_bComboDest == true)
    {
        m_bDisplay = false;

        m_bComboDest = false;
    }

    //弾と敵の当たり判定
    for (int nCntObj = 0; nCntObj < C3dslashwindBoss::MAX_SLASHWINDBOSS; nCntObj++)
    {
        CObject* pObj = CObject::GetObj(3, nCntObj);

        if (pObj != nullptr)
        {
            CObject::TYPE type = pObj->GetType();

            C3dplayer* p3dplayer = (C3dplayer*)pObj;

            D3DXVECTOR3 PlayerPos = p3dplayer->GetPos();

            //敵の場合
            if (type == CObject::TYPE::PLAYER)
            {
                if (Pos.x >= PlayerPos.x - 75
                    && Pos.x <= PlayerPos.x + 75
                    && Pos.y >= PlayerPos.y - 100
                    && Pos.y <= PlayerPos.y + 100
                    && Pos.z >= PlayerPos.z - 10
                    && Pos.z <= PlayerPos.z + 10)
                {
                    m_bDisplay = false;
                    p3dplayer->PlayerDamage(10);
                    C3dhiteffect::Create(CObject3D::GetPos(), D3DXVECTOR3(70.0f, 70.0f, 0.0f), m_rot);
                    CCamera::SetShake((2), 7.0f);
                    m_nTimer = 0;
                    C3dslashwindBoss::Uninit();
                    return;
                }
            }
        }
    }


    //フェードの状態を取得
    int nFadeState = CFade::GetFadeState();

    if (nFadeState == CFade::FADE_OUT)
    {
        m_bComboDest = true;
        m_nTimer = 0;

    }
}

//======================
// 描画処理
//======================
void C3dslashwindBoss::Draw()
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

    for (int nCntParts = 0; nCntParts < SLASHWINDBOSS_MODEL; nCntParts++)
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

//コンボ攻撃時の風削除処理
void C3dslashwindBoss::Reset()
{
    m_bComboDest = true;
}


//======================
// オブジェクト生成処理
//======================
C3dslashwindBoss* C3dslashwindBoss::Create(D3DXVECTOR3 pos)
{
    C3dslashwindBoss* D3DSlashWind = nullptr;

    D3DSlashWind = new C3dslashwindBoss;

    //初期化に成功した場合
    if (SUCCEEDED(D3DSlashWind->Init()))
    {
        D3DSlashWind->SetType(TYPE::SLASHWINDBOSS);

        D3DSlashWind->LoadBlockData();

        //D3DBlock->Load();//テクスチャを設定(仮)

        D3DSlashWind->CObject3D::SetPos(pos);

        ////テクスチャの設定
        //Model->BindTexture(m_pTexBuff);
    }

    return D3DSlashWind;
}

//======================
// テクスチャロード処理
//======================
HRESULT C3dslashwindBoss::Load()
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
void C3dslashwindBoss::Unload()
{
    CModel::Unload();
}



//===========================
// 外部ファイル読み込み処理
//===========================
void C3dslashwindBoss::LoadBlockData(void)
{
    char Datacheck[MAX_CHAR];
    int nCntEnemyData = 0;
    int EnemyModelSave = 0;

    m_pFile = fopen("data\\MODEL\\MODEL_TORNADO\\bosstornado_profile.txt", "r");//ファイルを開く

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
bool C3dslashwindBoss::Collision3DBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fHeight)
{
    bool bLanding = false; // 重力が適用された場合のみ使用
    float fMoveslashwindWidth = 25.0f;
    float fMoveslashwindDepth = 70.0f;
    float fMoveslashwindHeight = -15.0f;

    D3DXVECTOR3 Pos = CObject3D::GetPos();

    // 右側当たり判定
    if (pPos->x - fWidth <= Pos.x + fMoveslashwindWidth && pPosOld->x - fWidth >= Pos.x + fMoveslashwindWidth && pPos->z - fHeight < Pos.z + fMoveslashwindDepth && pPos->z > Pos.z - fMoveslashwindDepth && pPos->y < Pos.y + fMoveslashwindHeight && pPos->y > Pos.y - fMoveslashwindHeight)
    {
        pPos->x = Pos.x + fMoveslashwindWidth + fWidth;
    }
    // 左側当たり判定
    else if (pPos->x + fWidth >= Pos.x - fMoveslashwindWidth && pPosOld->x + fWidth <= Pos.x - fMoveslashwindWidth && pPos->z - fHeight < Pos.z + fMoveslashwindDepth && pPos->z > Pos.z - fMoveslashwindDepth && pPos->y < Pos.y + fMoveslashwindHeight && pPos->y > Pos.y - fMoveslashwindHeight)
    {
        pPos->x = Pos.x - fMoveslashwindWidth - fWidth;
    }
    // 前側当たり判定
    if (pPos->x - fWidth < Pos.x + fMoveslashwindWidth && pPos->x + fWidth > Pos.x - fMoveslashwindWidth && pPos->z - fHeight <= Pos.z + fMoveslashwindDepth && pPosOld->z - fHeight >= Pos.z + fMoveslashwindDepth && pPos->y < Pos.y + fMoveslashwindHeight && pPos->y > Pos.y - fMoveslashwindHeight)
    {
        pPos->z = Pos.z + fMoveslashwindDepth + fHeight;
    }
    // 後側当たり判定
    else if (pPos->x - fWidth < Pos.x + fMoveslashwindWidth && pPos->x + fWidth > Pos.x - fMoveslashwindWidth && pPos->z >= Pos.z - fMoveslashwindDepth && pPosOld->z <= Pos.z - fMoveslashwindDepth && pPos->y < Pos.y + fMoveslashwindHeight && pPos->y > Pos.y - fMoveslashwindHeight)
    {
        pPos->z = Pos.z - fMoveslashwindDepth - fHeight;
    }
    // 上側当たり判定
    if (pPos->x - fWidth < Pos.x + fMoveslashwindWidth && pPos->x + fWidth > Pos.x - fMoveslashwindWidth && pPos->y - fHeight <= Pos.y + fMoveslashwindHeight && pPosOld->y - fHeight >= Pos.y + fMoveslashwindHeight && pPos->z < Pos.z + fMoveslashwindDepth && pPos->z > Pos.z - fMoveslashwindDepth)
    {
        pPos->y = Pos.y + fMoveslashwindHeight + fHeight;
        bLanding = true;
    }
    // 下側当たり判定
    else if (pPos->x - fWidth < Pos.x + fMoveslashwindWidth && pPos->x + fWidth > Pos.x - fMoveslashwindWidth && pPos->y >= Pos.y - fMoveslashwindHeight && pPosOld->y <= Pos.y - fMoveslashwindHeight && pPos->z < Pos.z + fMoveslashwindDepth && pPos->z > Pos.z - fMoveslashwindDepth)
    {
        pPos->y = Pos.y - fMoveslashwindHeight - fHeight;
    }

    return bLanding;
}
