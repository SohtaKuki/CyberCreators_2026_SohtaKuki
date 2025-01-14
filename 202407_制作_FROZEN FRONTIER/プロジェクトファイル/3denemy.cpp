//=================================================
//
// 3Dモデルのプレイヤーの表示処理 (3denemy.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "3dblock.h"
#include "player.h"
#include "block.h"
#include "startobj.h"
#include "3ditem.h"
#include "3denemy.h"
#include "3dbullet.h"
#include "3dchargebullet.h"
#include "3dbrokenblock.h"
#include "3dwall.h"
#include "score.h"
#include "scene.h"
#include "3dexplosion.h"

LPDIRECT3DTEXTURE9 C3denemy::m_pTexBuff = nullptr;
//======================
// コンストラクタ
//======================
C3denemy::C3denemy(int nPriority) : CModel(nPriority)
{
    m_nShotInterval = 0;
    m_nMoveInterval = 0;
    m_nLife = 3;
    m_bMoveSwitch = false;
}

//======================
// デストラクタ
//======================
C3denemy::~C3denemy()
{

}

//======================
// 初期化処理
//======================
HRESULT C3denemy::Init()
{
    //初期化
    if (FAILED(CModel::Init()))
    {
        return E_FAIL;
    }

    return S_OK;
}

//======================
// 終了処理
//======================
void C3denemy::Uninit()
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

    CModel::Uninit();
}

//======================
// 更新処理
//======================
void C3denemy::Update()
{
    if (CScene::GetUpdateStat() == true)
    {
        D3DXVECTOR3 Pos = CObject3D::GetPos();

        int nCnt = 0;

        //プレイヤーのHPが0以上の場合のみ通す
        if (m_nLife > 0)
        {
            m_nShotInterval++;
        }

        //球発射(1秒半の間隔で発射)
        if (m_nShotInterval == SHOT_INTERVAL)
        {
            C3dbullet::Create(Pos, D3DXVECTOR3(7.0f, 7.0f, 0.0f), m_rot, 1);
            m_nShotInterval = 0;
        }


        if (m_nMoveInterval == 0)
        {
            m_bMoveSwitch = false;

        }

        if (m_nMoveInterval == MOVE_INTERVAL)
        {
            m_bMoveSwitch = true;
        }

        //逆へ方向転換するフラグが無効の場合
        if (m_bMoveSwitch == false)
        {
            m_nMoveInterval++;

            if (GetType() == TYPE::ENEMY_XMOVE)
            {
                m_n3DEnemyMove.x += 0.4f;
                m_rot.y = D3DX_PI * -0.5f;
            }

            if (GetType() == TYPE::ENEMY_ZMOVE)
            {
                m_n3DEnemyMove.z += 0.4f;
                m_rot.y = D3DX_PI;
            }
        }

        //逆へ方向転換するフラグが有効の場合
        if (m_bMoveSwitch == true)
        {
            m_nMoveInterval--;

            if (GetType() == TYPE::ENEMY_XMOVE)
            {
                m_n3DEnemyMove.x -= 0.4f;
                m_rot.y = D3DX_PI * 0.5f;
            }

            if (GetType() == TYPE::ENEMY_ZMOVE)
            {
                m_n3DEnemyMove.z -= 0.4f;
                m_rot.y = D3DX_PI * -0.0f;
            }
        }


        if (m_nLife <= 0)
        {
            C3dexplosion::Create(CObject3D::GetPos(), D3DXVECTOR3(70.0f, 70.0f, 0.0f), m_rot, 0);
            CScore::AddScore(17000);
            Uninit();
            return;
        }


        //過去座標を保存
        m_nOld3DEnemyPos = Pos;

        Pos.x += m_n3DEnemyMove.x;
        Pos.z += m_n3DEnemyMove.z;

        //ブロックとの当たり判定の補正
        for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY; nCntPriority++)
        {
            for (int nCntObj = 0; nCntObj < MAX_ENEMY; nCntObj++)
            {
                CObject* pObj = CObject::GetObj(nCntPriority, nCntObj);

                if (pObj != nullptr)
                {
                    CObject::TYPE type = pObj->GetType();

                    if (type == CObject::TYPE::BLOCK)
                    {
                        C3dblock* p3dblock = (C3dblock*)pObj;

                        bool bIsCollision = p3dblock->Collision3DBlock(&Pos, &m_nOld3DEnemyPos, &m_n3DEnemyMove, 50.0f, 50.0f);

                        if (bIsCollision == true)
                        {
                            m_bMoveSwitch = true;
                        }
                        //    if (m_n3DEnemyMove.z >= 0.1f && p3dblock->GetMoveBlock().z >= 0.1f || m_n3DEnemyMove.z <= 0.1f && p3dblock->GetMoveBlock().z <= -0.1f)
                        //    {
                        //        Pos.z += p3dblock->GetMoveBlock().z;
                        //    }


                        //    if (m_n3DEnemyMove.z >= 0.1f && p3dblock->GetMoveBlock().z <= -0.1f || m_n3DEnemyMove.z <= -0.1f && p3dblock->GetMoveBlock().z >= 0.1f)
                        //    {
                        //        Pos.z += (p3dblock->GetMoveBlock().z * 2);
                        //    }
                        //}

                    }

                }
            }
        }

        //壁ブロックとの当たり判定
        for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY; nCntPriority++)
        {
            for (int nCntObj = 0; nCntObj < 200; nCntObj++)
            {
                CObject* pObj = CObject::GetObj(nCntPriority, nCntObj);

                if (pObj != nullptr)
                {
                    CObject::TYPE type = pObj->GetType();

                    if (type == CObject::TYPE::WALL_WIDTH)
                    {
                        C3dwall* p3dwall = (C3dwall*)pObj;

                        bool bIsCollision = p3dwall->Collision3DWall(&Pos, &m_nOld3DEnemyPos, &m_n3DEnemyMove, 50.0f, 50.0f);

                        if (bIsCollision == true)
                        {
                            if (m_n3DEnemyMove.z >= 0.1f && p3dwall->GetMoveBlock().z >= 0.1f || m_n3DEnemyMove.z <= 0.1f && p3dwall->GetMoveBlock().z <= -0.1f)
                            {
                                Pos.z += p3dwall->GetMoveBlock().z;
                            }

                            if (m_n3DEnemyMove.z >= 0.1f && p3dwall->GetMoveBlock().z <= -0.1f || m_n3DEnemyMove.z <= -0.1f && p3dwall->GetMoveBlock().z >= 0.1f)
                            {
                                Pos.z += (p3dwall->GetMoveBlock().z * 2);
                            }
                        }

                    }

                    if (type == CObject::TYPE::WALL_WIDTH_SHORT)
                    {
                        C3dwall* p3dwall = (C3dwall*)pObj;

                        bool bIsCollision = p3dwall->Collision3DWallShort(&Pos, &m_nOld3DEnemyPos, &m_n3DEnemyMove, 50.0f, 50.0f);

                        if (bIsCollision == true)
                        {
                            if (m_n3DEnemyMove.z >= 0.1f && p3dwall->GetMoveBlock().z >= 0.1f || m_n3DEnemyMove.z <= 0.1f && p3dwall->GetMoveBlock().z <= -0.1f)
                            {
                                Pos.z += p3dwall->GetMoveBlock().z;
                            }

                            if (m_n3DEnemyMove.z >= 0.1f && p3dwall->GetMoveBlock().z <= -0.1f || m_n3DEnemyMove.z <= -0.1f && p3dwall->GetMoveBlock().z >= 0.1f)
                            {
                                Pos.z += (p3dwall->GetMoveBlock().z * PLAYER_MOVE_BOOST);
                            }
                        }

                    }

                    if (type == CObject::TYPE::WALL_HEIGHT)
                    {

                        C3dwall* p3dwall = (C3dwall*)pObj;


                        bool bIsCollision = p3dwall->Collision3DHeightWall(&Pos, &m_nOld3DEnemyPos, &m_n3DEnemyMove, 50.0f, 50.0f);

                        if (bIsCollision == true)
                        {
                            if (m_n3DEnemyMove.z >= 0.1f && p3dwall->GetMoveBlock().z >= 0.1f || m_n3DEnemyMove.z <= 0.1f && p3dwall->GetMoveBlock().z <= -0.1f)
                            {
                                Pos.z += p3dwall->GetMoveBlock().z;
                            }


                            if (m_n3DEnemyMove.z >= 0.1f && p3dwall->GetMoveBlock().z <= -0.1f || m_n3DEnemyMove.z <= -0.1f && p3dwall->GetMoveBlock().z >= 0.1f)
                            {
                                Pos.z += (p3dwall->GetMoveBlock().z * 2);
                            }
                        }

                    }


                    if (type == CObject::TYPE::WALL_HEIGHT_SHORT)
                    {
                        C3dwall* p3dwall = (C3dwall*)pObj;

                        bool bIsCollision = p3dwall->Collision3DHeightWallShort(&Pos, &m_nOld3DEnemyPos, &m_n3DEnemyMove, 50.0f, 50.0f);

                        if (bIsCollision == true)
                        {
                            if (m_n3DEnemyMove.z >= 0.1f && p3dwall->GetMoveBlock().z >= 0.1f || m_n3DEnemyMove.z <= 0.1f && p3dwall->GetMoveBlock().z <= -0.1f)
                            {
                                Pos.z += p3dwall->GetMoveBlock().z;
                            }

                            if (m_n3DEnemyMove.z >= 0.1f && p3dwall->GetMoveBlock().z <= -0.1f || m_n3DEnemyMove.z <= -0.1f && p3dwall->GetMoveBlock().z >= 0.1f)
                            {
                                Pos.z += (p3dwall->GetMoveBlock().z * 2);
                            }
                        }

                    }

                }
            }
        }

        int nFadeState = CFade::GetFadeState();

        if (nFadeState == CFade::FADE_OUT)
        {

            C3denemy::Uninit();
            return;
        }

        //座標を設定
        SetPos(Pos);

        //X座標の移動量を更新
        m_n3DEnemyMove.x += (Length_value2 - m_n3DEnemyMove.x) * 0.18f;

        //Z座標の移動量を更新
        m_n3DEnemyMove.z += (Length_value2 - m_n3DEnemyMove.z) * 0.18f;

    }
}

//======================
// 描画処理
//======================
void C3denemy::Draw()
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
// 敵のダメージ処理
//======================
void C3denemy::EnemyDamage()
{
    CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_ENEMY_DAMAGE);
    C3dexplosion::Create(CObject3D::GetPos(), D3DXVECTOR3(35.0f, 35.0f, 0.0f), m_rot, 0);
    m_nLife--;
}

//======================
// オブジェクト生成処理
//======================
C3denemy* C3denemy::Create(D3DXVECTOR3 pos,int MoveType)
{
    C3denemy* D3Dplayer = nullptr;

    D3Dplayer = new C3denemy;

    //初期化に成功した場合
    if (SUCCEEDED(D3Dplayer->Init()))
    {
        if (MoveType == 0)
        {
            D3Dplayer->SetType(TYPE::ENEMY_NORMAL);
        }

        if (MoveType == 1)
        {
            D3Dplayer->SetType(TYPE::ENEMY_XMOVE);
        }

        if (MoveType == 2)
        {
            D3Dplayer->SetType(TYPE::ENEMY_ZMOVE);
        }

        if (MoveType == 3)
        {
            D3Dplayer->SetType(TYPE::ENEMY_XMOVE);
            D3Dplayer->m_nMoveInterval = MOVE_INTERVAL;
        }

        if (MoveType == 4)
        {
            D3Dplayer->SetType(TYPE::ENEMY_ZMOVE);
            D3Dplayer->m_nMoveInterval = MOVE_INTERVAL;
        }

        D3Dplayer->LoadPlayerData();

        D3Dplayer->CObject3D::SetPos(pos);

        //D3Dplayer->Load();//テクスチャを設定(仮)


        ////テクスチャの設定
        //Model->BindTexture(m_pTexBuff);
    }

    return D3Dplayer;
}



//======================
// テクスチャロード処理
//======================
HRESULT C3denemy::Load()
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
void C3denemy::Unload()
{
    CModel::Unload();
}



//===========================
// 外部ファイル読み込み処理
//===========================
void C3denemy::LoadPlayerData(void)
{
    char Datacheck[MAX_CHAR];
    int nCntEnemyData = 0;
    int EnemyModelSave = 0;

    //m_pFile = fopen("data\\MODEL\\MODEL_golden_man\\motion.txt", "r");//ファイルを開く
    m_pFile = fopen("data\\MODEL_Crystal\\motion_crs3.txt", "r");//ファイルを開く

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
        //D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\golden000.jpg", &m_pTexture);
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