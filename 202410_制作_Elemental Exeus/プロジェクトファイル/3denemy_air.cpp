//=================================================
//
// 3Dモデルのプレイヤーの表示処理 (3denemyair.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "3dblock.h"
#include "player.h"
#include "block.h"
#include "startobj.h"
#include "3ditem.h"
#include "3denemy_air.h"
#include "3dbullet.h"
#include "3dchargebullet.h"
#include "score.h"
#include "scene.h"
#include "3dexplosion.h"
#include "3dslashwind_enemy.h"
#include "3dexplosion_sub.h"

LPDIRECT3DTEXTURE9 C3denemyair::m_pTexBuff = nullptr;
//======================
// コンストラクタ
//======================
C3denemyair::C3denemyair(int nPriority) : CModel(nPriority)
{
    m_nShotInterval = 0;
    m_nMoveInterval = 0;
    m_nLife = 3;
    m_bMoveSwitch = false;
    m_rot.y = 1.57f;
}

//======================
// デストラクタ
//======================
C3denemyair::~C3denemyair()
{

}

//======================
// 初期化処理
//======================
HRESULT C3denemyair::Init()
{
    //初期化
    if (FAILED(CModel::Init()))
    {
        return E_FAIL;
    }

    //モーションの初期化
    for (int nCount = 0; nCount < MAX_MOTION; nCount++)
    {
        C3denemyair::MotionSet[nCount].Loop = false;
        C3denemyair::MotionSet[nCount].NumKey = 0;
        for (int nKeySetCount = 0; nKeySetCount < MAX_KEYSET; nKeySetCount++)
        {
            MotionSet[nCount].KeySet[nKeySetCount].Frame = 0;

            for (int nKeyCount = 0; nKeyCount < ENEMYAIR_MODEL; nKeyCount++)
            {
                MotionSet[nCount].KeySet[nKeySetCount].aKey[nKeyCount].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                MotionSet[nCount].KeySet[nKeySetCount].aKey[nKeyCount].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            }
        }
    }

    for (int nCnt = 0; nCnt < ENEMYAIR_MODEL; nCnt++)
    {
        m_pBuffMat[nCnt] = 0;
    }

    return S_OK;
}

//======================
// 終了処理
//======================
void C3denemyair::Uninit()
{
    // すでに解放されている場合は処理をスキップ
    if (m_pTexBuff != nullptr) {
        m_pTexBuff->Release();
        m_pTexBuff = nullptr;
    }

    // メッシュやマテリアルの解放
    for (int i = 0; i < ENEMYAIR_MODEL; i++)
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
void C3denemyair::Update()
{
    if (CScene::GetUpdateStat() == true)
    {
        D3DXVECTOR3 Pos = CObject3D::GetPos();

        int nCnt = 0;

        C3denemyair::ModelMotion();

        //プレイヤーのHPが0以上の場合のみ通す
        if (m_nLife > 0)
        {
            m_nShotInterval++;
        }

        //球発射(1秒半の間隔で発射)
        if (m_nShotInterval == SHOT_INTERVAL)
        {
            //C3dbullet::Create(Pos, D3DXVECTOR3(7.0f, 7.0f, 0.0f), m_rot, 1);
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

        if (m_nShotInterval == 100)
        {

            MotionUse = false;
            SetEnemyMotion(MOTION_ACTION);
            MotionUse = true;
        }


        if (Motion == C3denemyair::MOTION_ACTION)
        {
            if (m_nMotionCnt == 2 && m_nFrameCnt == 0)
            {
                C3dslashwindEnemy::Create(D3DXVECTOR3(Pos.x - 80.0f, Pos.y, Pos.z));
            }
        }

        //逆へ方向転換するフラグが無効の場合
        if (m_bMoveSwitch == false)
        {
            m_nMoveInterval++;

            if (GetType() == TYPE::ENEMY_NORMALMOVE)
            {
                m_n3DEnemyMove.x += 0.4f;
                m_rot.y = D3DX_PI * -0.5f;
            }

            if (GetType() == TYPE::ENEMY_FLYMOVE)
            {
                m_n3DEnemyMove.y += 0.2f;
            }
        }

        //逆へ方向転換するフラグが有効の場合
        if (m_bMoveSwitch == true)
        {
            m_nMoveInterval--;

            if (GetType() == TYPE::ENEMY_NORMALMOVE)
            {
                m_n3DEnemyMove.x -= 0.4f;
                m_rot.y = D3DX_PI * 0.5f;
            }

            if (GetType() == TYPE::ENEMY_FLYMOVE)
            {
                m_n3DEnemyMove.y -= 0.2f;
            }
        }


        if (m_nLife <= 0)
        {
            C3dexplosionsub::Create(D3DXVECTOR3(CObject3D::GetPos().x, CObject3D::GetPos().y - 35.0f, CObject3D::GetPos().z), D3DXVECTOR3(100.0f, 140.0f, 0.0f), m_rot, 0);
            CScore::AddScore(17000);
            Uninit();
            return;
        }

        //特別なモーション中ではない場合
        if (MotionUse == false)
        {
            SetEnemyMotion(MOTION_NORMAL);
        }


        //過去座標を保存
        m_nOld3DEnemyPos = Pos;

        Pos.x += m_n3DEnemyMove.x;
        Pos.y += m_n3DEnemyMove.y;
        Pos.z += m_n3DEnemyMove.z;

        //ブロックとの当たり判定の補正
        for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY; nCntPriority++)
        {
            for (int nCntObj = 0; nCntObj < MAX_ENEMYAIR; nCntObj++)
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



        int nFadeState = CFade::GetFadeState();

        if (nFadeState == CFade::FADE_OUT)
        {
            C3denemyair::Uninit();
            return;
        }

        //座標を設定
        SetPos(Pos);

        //X座標の移動量を更新
        m_n3DEnemyMove.x += (Length_value2 - m_n3DEnemyMove.x) * 0.18f;

        //Y座標の移動量を更新
        m_n3DEnemyMove.y += (Length_value2 - m_n3DEnemyMove.y) * 0.18f;

        //Z座標の移動量を更新
        m_n3DEnemyMove.z += (Length_value2 - m_n3DEnemyMove.z) * 0.18f;

    }
}

//======================
// 描画処理
//======================
void C3denemyair::Draw()
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

    ////モデルの影の設定
    //D3DXMATRIX mtxshadow , mtxshadowTrans;
    //D3DXPLANE plane;
    //D3DXVECTOR4 vecLight;
    //D3DXVECTOR3 pos, nor;

    //D3DXMatrixIdentity(&mtxshadow);

    ////ライトの逆方向ベクトルを設定
    //vecLight = D3DXVECTOR4(0.5f, 0.6f, 0.7f,1.0f);
    //pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
    //nor = D3DXVECTOR3(0.0f,1.0f,0.0f);

    //D3DXPlaneFromPointNormal(&plane, &pos, &nor);
    //D3DXMatrixShadow(&mtxshadow,&vecLight,&plane);

    //m_mtxworld * mtxshadow;

    //ワールドマトリックスの設定
    pDevice->SetTransform(D3DTS_WORLD, &m_mtxworld);

    //マテリアルを取得
    pDevice->GetMaterial(&matDef);

    for (int nCntParts = 0; nCntParts < ENEMYAIR_MODEL; nCntParts++)
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

        //マトリックスの設定
        pDevice->SetTransform(D3DTS_WORLD, &m_aModel[nCntParts].mtxworld);

        pMat = (D3DXMATERIAL*)m_pBuffMat[nCntParts]->GetBufferPointer();



        for (int nCntMat = 0; nCntMat < (int)m_nNumMat[nCntParts]; nCntMat++)
        {
            //マテリアルの設定
            pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

            //テクスチャの設定
            pDevice->SetTexture(0, NULL);

            //テクスチャが存在する
            if (pMat[nCntMat].pTextureFilename != NULL)
            {
                //テクスチャの設定
                pDevice->SetTexture(0, &m_aModel[0].m_pTexture[nCntMat]);
            }

            else
            {
                //テクスチャの設定
                pDevice->SetTexture(0, NULL);
            }

            if (m_nLife > 0)
            {
                //モデル(パーツ)の描画
                m_pMesh[nCntParts]->DrawSubset(nCntMat);
            }

        }
    }
}

//======================
// 敵のダメージ処理
//======================
void C3denemyair::EnemyDamage()
{
    m_nLife--;
    CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_ENEMY_DAMAGE);
    C3dexplosion::Create(CObject3D::GetPos(), D3DXVECTOR3(35.0f, 35.0f, 0.0f), m_rot, 0);
}

//======================
// オブジェクト生成処理
//======================
C3denemyair* C3denemyair::Create(D3DXVECTOR3 pos, int MoveType)
{
    C3denemyair* D3Dplayer = nullptr;

    D3Dplayer = new C3denemyair;

    //初期化に成功した場合
    if (SUCCEEDED(D3Dplayer->Init()))
    {
        if (MoveType == 0)
        {
            D3Dplayer->SetType(TYPE::ENEMY_FLYMOVE);
            D3Dplayer->m_nMoveInterval = MOVE_INTERVAL;
        }

        D3Dplayer->m_nType = MoveType;

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
HRESULT C3denemyair::Load()
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
void C3denemyair::Unload()
{
    CModel::Unload();
}



//===========================
// 外部ファイル読み込み処理
//===========================
void C3denemyair::LoadPlayerData(void)
{
    char Datacheck[MAX_CHAR];
    int nCnt = 0;
    int nCntEnemyData = 0;
    int EnemyModelSave = 0;
    int nMotionCount = 0;
    int nKeyCount = 0;
    int nKeySetCount = 0;


    //m_pFile = fopen("data\\MODEL\\MODEL_golden_man\\motion.txt", "r");//ファイルを開く

    m_pFile = fopen("data\\MODEL\\MODEL_ENEMY_AIR\\Charamotion.txt", "r");//ファイルを開く


    //ファイルが存在しない場合
    if (m_pFile == NULL)
    {
        return;
    }

    // ENDが見つかるまで読み込みを繰り返す
    while (1)
    {
        fscanf(m_pFile, "%s", Datacheck);	// 検索

        if (!strcmp(Datacheck, "SCRIPT"))
        {
            // ENDが見つかるまで読み込みを繰り返す
            while (1)
            {
                fscanf(m_pFile, "%s", Datacheck);	// 検索

                if (!strcmp(Datacheck, "MODEL_FILENAME"))
                {
                    fscanf(m_pFile, "%s", Datacheck);
                    fscanf(m_pFile, "%s", &C3denemyair::m_aLoadEnemy[EnemyModelSave].filename[0]);
                    EnemyModelSave++;
                }

                if (!strcmp(Datacheck, "END_SCRIPT"))
                {// 読み込みを終了
                    break;
                }

                if (!strcmp(Datacheck, "CHARACTERSET"))
                {
                    // ENDが見つかるまで読み込みを繰り返す
                    while (1)
                    {
                        fscanf(m_pFile, "%s", Datacheck);	// 検索

                        if (!strcmp(Datacheck, "END_CHARACTERSET"))
                        {// 読み込みを終了
                            //fclose(m_pFile);
                            break;
                        }
                        if (Datacheck[0] == '#')
                        {// 折り返す
                            continue;
                        }
                        if (!strcmp(Datacheck, "NUM_PARTS"))
                        {
                            fscanf(m_pFile, "%s", Datacheck);
                            fscanf(m_pFile, "%d", &EnemyModelSave);
                        }


                        if (!strcmp(Datacheck, "PARTSSET"))
                        {
                            // 項目ごとにデータを代入
                            while (1)
                            {
                                fscanf(m_pFile, "%s", Datacheck); // 検索

                                if (!strcmp(Datacheck, "END_PARTSSET"))
                                {
                                    break;
                                }			// 読み込みを終了
                                else if (!strcmp(Datacheck, "INDEX"))
                                {
                                    fscanf(m_pFile, "%s", Datacheck);
                                    fscanf(m_pFile, "%d", &C3denemyair::m_aLoadEnemy[nCnt].index);
                                }		// 種類
                                else if (!strcmp(Datacheck, "PARENT"))
                                {
                                    fscanf(m_pFile, "%s", Datacheck);
                                    fscanf(m_pFile, "%d", &C3denemyair::m_aLoadEnemy[nCnt].parent);
                                }		// 幅
                                else if (!strcmp(Datacheck, "POS"))
                                {
                                    fscanf(m_pFile, "%s", Datacheck);
                                    fscanf(m_pFile, "%f", &C3denemyair::m_aLoadEnemy[nCnt].pos.x);
                                    fscanf(m_pFile, "%f", &C3denemyair::m_aLoadEnemy[nCnt].pos.y);
                                    fscanf(m_pFile, "%f", &C3denemyair::m_aLoadEnemy[nCnt].pos.z);
                                }	// 高さ
                                else if (!strcmp(Datacheck, "ROT"))
                                {
                                    fscanf(m_pFile, "%s", Datacheck);
                                    fscanf(m_pFile, "%f", &C3denemyair::m_aLoadEnemy[nCnt].rot.x);
                                    fscanf(m_pFile, "%f", &C3denemyair::m_aLoadEnemy[nCnt].rot.y);
                                    fscanf(m_pFile, "%f", &C3denemyair::m_aLoadEnemy[nCnt].rot.z);
                                }		// 奥行き

                            }
                            nCnt++;	// データ数加算
                        }
                    }
                }

                //============モーションの読み込み==============================================
                if (!strcmp(Datacheck, "MOTIONSET"))
                {
                    int LoopType = 0;

                    // 項目ごとにデータを代入
                    while (1)
                    {
                        fscanf(m_pFile, "%s", Datacheck); // 検索

                        if (Datacheck[0] == '#')
                        {// 折り返す
                            continue;
                            fgets(Datacheck, 40, m_pFile);
                        }
                        if (!strcmp(Datacheck, "END_MOTIONSET"))
                        {
                            break;
                        }			// 読み込みを終了
                        else if (!strcmp(Datacheck, "LOOP")) //ループの読み込み
                        {
                            fscanf(m_pFile, "%s", Datacheck);
                            fscanf(m_pFile, "%d", &LoopType);

                            if (LoopType == 0)
                            {
                                C3denemyair::MotionSet[nMotionCount].Loop = false;
                            }
                            else
                            {
                                C3denemyair::MotionSet[nMotionCount].Loop = true;
                            }
                        }		// 種類
                        else if (!strcmp(Datacheck, "NUM_KEY")) //キー数の読み込み
                        {
                            fscanf(m_pFile, "%s", Datacheck);
                            fscanf(m_pFile, "%d", &C3denemyair::MotionSet[nMotionCount].NumKey);
                        }		// 幅

                        if (!strcmp(Datacheck, "KEYSET")) //キーセットの読み込み
                        {
                            // 項目ごとにデータを代入
                            while (1)
                            {
                                fscanf(m_pFile, "%s", Datacheck); // 検索

                                if (Datacheck[0] == '#')
                                {// 折り返す
                                    fgets(Datacheck, 20, m_pFile);
                                    continue;
                                }
                                if (!strcmp(Datacheck, "END_KEYSET"))
                                {
                                    break;
                                }			// 読み込みを終了
                                //フレーム数の読み込み
                                if (!strcmp(Datacheck, "FRAME"))
                                {
                                    fscanf(m_pFile, "%s", Datacheck);
                                    fscanf(m_pFile, "%d", &C3denemyair::MotionSet[nMotionCount].KeySet[nKeySetCount].Frame);
                                }

                                if (!strcmp(Datacheck, "KEY")) //キーの読み込み
                                {
                                    // 項目ごとにデータを代入
                                    while (1)
                                    {
                                        fscanf(m_pFile, "%s", Datacheck); // 検索

                                        if (Datacheck[0] == '#')
                                        {// 折り返す
                                            continue;
                                            fgets(Datacheck, 40, m_pFile);
                                        }
                                        if (!strcmp(Datacheck, "POS"))
                                        {
                                            fscanf(m_pFile, "%s", Datacheck);
                                            fscanf(m_pFile, "%f", &C3denemyair::MotionSet[nMotionCount].KeySet[nKeySetCount].aKey[nKeyCount].pos.x);
                                            fscanf(m_pFile, "%f", &C3denemyair::MotionSet[nMotionCount].KeySet[nKeySetCount].aKey[nKeyCount].pos.y);
                                            fscanf(m_pFile, "%f", &C3denemyair::MotionSet[nMotionCount].KeySet[nKeySetCount].aKey[nKeyCount].pos.z);
                                        }
                                        else if (!strcmp(Datacheck, "ROT"))
                                        {
                                            fscanf(m_pFile, "%s", Datacheck);
                                            fscanf(m_pFile, "%f", &C3denemyair::MotionSet[nMotionCount].KeySet[nKeySetCount].aKey[nKeyCount].rot.x);
                                            fscanf(m_pFile, "%f", &C3denemyair::MotionSet[nMotionCount].KeySet[nKeySetCount].aKey[nKeyCount].rot.y);
                                            fscanf(m_pFile, "%f", &C3denemyair::MotionSet[nMotionCount].KeySet[nKeySetCount].aKey[nKeyCount].rot.z);
                                        }

                                        if (!strcmp(Datacheck, "END_KEY"))
                                        {
                                            break;
                                        }			// 読み込みを終了

                                    }
                                    nKeyCount++; //データの加算
                                }
                            }
                            nKeyCount = 0;
                            nKeySetCount++; //データの加算
                        }
                    }
                    nKeySetCount = 0; //カウントの初期化
                    nMotionCount++; //データの加算
                }
            }
            fclose(m_pFile);
            break;
        }
    }

    //デバイスの取得
    LPDIRECT3DDEVICE9 pDevice = nullptr;
    pDevice = CManager::GetRenderer()->GetDevice();


    for (int nInfoCount = 0; nInfoCount < EnemyModelSave; nInfoCount++)
    {
        //Xファイルの読み込み
        D3DXLoadMeshFromX(&C3denemyair::m_aLoadEnemy[nInfoCount].filename[0], D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[nInfoCount], NULL, &m_nNumMat[nInfoCount], &m_pMesh[nInfoCount]);

        //位置の初期化
        m_aModel[nInfoCount].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        m_aModel[nInfoCount].nIdxModelParent = C3denemyair::m_aLoadEnemy[nInfoCount].parent;
        m_aModel[nInfoCount].pos = D3DXVECTOR3(C3denemyair::m_aLoadEnemy[nInfoCount].pos.x, C3denemyair::m_aLoadEnemy[nInfoCount].pos.y, C3denemyair::m_aLoadEnemy[nInfoCount].pos.z);
        m_aModel[nInfoCount].rot = D3DXVECTOR3(C3denemyair::m_aLoadEnemy[nInfoCount].rot.x, C3denemyair::m_aLoadEnemy[nInfoCount].rot.y, C3denemyair::m_aLoadEnemy[nInfoCount].rot.z);


        m_aModel[nInfoCount].pos += C3denemyair::MotionSet[0].KeySet[0].aKey[nInfoCount].pos;
        m_aModel[nInfoCount].rot += C3denemyair::MotionSet[0].KeySet[0].aKey[nInfoCount].rot;
    }
}

//======================
// モデルのモーション処理
//======================
void C3denemyair::ModelMotion()
{
    // モデルのパーツ分繰り返す
    for (int nModelCount = 0; nModelCount < ENEMYAIR_MODEL; nModelCount++)
    {
        // 現在のモーションのキーセットを取得
        const auto& currentKey = C3denemyair::MotionSet[Motion].KeySet[m_nMotionCnt];
        const auto& nextKey = C3denemyair::MotionSet[Motion].KeySet[(m_nMotionCnt + 1) % C3denemyair::MotionSet[Motion].NumKey];

        // 線形補間を用いて位置と回転の差分を計算
        D3DXVECTOR3 Movepos = (nextKey.aKey[nModelCount].pos - currentKey.aKey[nModelCount].pos) / currentKey.Frame;
        D3DXVECTOR3 Moverot = (nextKey.aKey[nModelCount].rot - currentKey.aKey[nModelCount].rot) / currentKey.Frame;

        // 差分を加算
        m_aModel[nModelCount].pos += Movepos;
        m_aModel[nModelCount].rot += Moverot;
    }

    // フレームの加算
    m_nFrameCnt++;

    // キーフレーム終了時の処理
    if (m_nFrameCnt >= C3denemyair::MotionSet[C3denemyair::Motion].KeySet[m_nMotionCnt].Frame)
    {
        m_nFrameCnt = 0;           // カウントを初期化
        m_nMotionCnt++;            // モーションのカウントを増加

        // モーションの終了条件をチェック
        if (m_nMotionCnt >= C3denemyair::MotionSet[C3denemyair::Motion].NumKey)
        {
            m_nMotionCnt = 0; // カウントを0にする

            // ループしない場合、モーションを停止
            if (!C3denemyair::MotionSet[C3denemyair::Motion].Loop)
            {
                C3denemyair::MotionUse = false;    // モーションの判定をoffにする
                SetEnemyMotion(MOTION_NORMAL);  // モーションの情報の切り替え
            }
        }
    }
}

//======================
// モーションの指定
//======================
void C3denemyair::SetEnemyMotion(ENEMYAIRMOTION motion)
{
    if (C3denemyair::Motion != motion && C3denemyair::MotionUse == false) //現在のモーションと違ったら
    {
        C3denemyair::Motion = motion; //モーションを設定
        C3denemyair::m_nMotionCnt = 0; //モーションのカウントを初期化
        C3denemyair::m_nFrameCnt = 0;  //フレームのカウントの初期化

        // モデルのパーツ分繰り返して位置と回転を初期化
        for (int nModelCount = 0; nModelCount < ENEMYAIR_MODEL; nModelCount++)
        {
            m_aModel[nModelCount].pos = m_aLoadEnemy[nModelCount].pos + C3denemyair::MotionSet[motion].KeySet[0].aKey[nModelCount].pos;
            m_aModel[nModelCount].rot = m_aLoadEnemy[nModelCount].rot + C3denemyair::MotionSet[motion].KeySet[0].aKey[nModelCount].rot;
        }
    }
}

//===========================
// 敵の当たり判定
//===========================
bool C3denemyair::Collision3DEnemyAir(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fBlockHeight)
{
    bool bLanding = false; // 重力を適応した場合のみ使用
    float fBlockWidth = 18.0f;
    float fBlockDepth = 30.0f;
    float fHeight = 0.0f;

    D3DXVECTOR3 Pos = CObject3D::GetPos();

    // 右側当たり判定
    if (pPos->x - fWidth <= Pos.x + fBlockWidth && pPosOld->x - fWidth >= Pos.x + fBlockWidth && pPos->z - fHeight < Pos.z + fBlockDepth && pPos->z > Pos.z - fBlockDepth && pPos->y < Pos.y + fBlockHeight - 5.0f && pPos->y > Pos.y - fBlockHeight - 0.0f)
    {
        pPos->x = Pos.x + fBlockWidth + fWidth;
    }
    // 左側当たり判定
    else if (pPos->x + fWidth >= Pos.x - fBlockWidth && pPosOld->x + fWidth <= Pos.x - fBlockWidth && pPos->z - fHeight < Pos.z + fBlockDepth && pPos->z > Pos.z - fBlockDepth && pPos->y < Pos.y + fBlockHeight - 5.0f && pPos->y > Pos.y - fBlockHeight - 10.0f)
    {
        pPos->x = Pos.x - fBlockWidth - fWidth;
    }
    // 前側当たり判定
    if (pPos->x - fWidth < Pos.x + fBlockWidth && pPos->x + fWidth > Pos.x - fBlockWidth && pPos->z - fHeight <= Pos.z + fBlockDepth - 30.0f && pPosOld->z - fHeight >= Pos.z + fBlockDepth - 30.0f && pPos->y < Pos.y + fBlockHeight && pPos->y > Pos.y - fBlockHeight)
    {
        pPos->z = Pos.z + fBlockDepth - 40.0f + fHeight;
    }

    // 後側当たり判定
    else if (pPos->x - fWidth < Pos.x + fBlockWidth && pPos->x + fWidth > Pos.x - fBlockWidth && pPos->z >= Pos.z - fBlockDepth && pPosOld->z <= Pos.z - fBlockDepth && pPos->y < Pos.y + fBlockHeight && pPos->y > Pos.y - fBlockHeight)
    {
        pPos->z = Pos.z - fBlockDepth + 50.0f - fHeight;
        bLanding = true;
    }
    // 上側当たり判定
    if (pPos->x - fWidth < Pos.x + fBlockWidth && pPos->x + fWidth > Pos.x - fBlockWidth && pPos->y - fHeight <= Pos.y + fBlockHeight && pPosOld->y - fHeight >= Pos.y + fBlockHeight && pPos->z < Pos.z + fBlockDepth && pPos->z > Pos.z - fBlockDepth)
    {
        pPos->y = Pos.y + fBlockHeight + fHeight;
        bLanding = true;
    }

    // 下側当たり判定
    else if (pPos->x - fWidth < Pos.x + fBlockWidth && pPos->x + fWidth > Pos.x - fBlockWidth && pPos->y + fHeight >= Pos.y - (fBlockHeight + 80.0f) && pPosOld->y + fHeight <= Pos.y - (fBlockHeight + 80.0f) && pPos->z < Pos.z + fBlockDepth && pPos->z > Pos.z - fBlockDepth)
    {
        pPos->y = Pos.y - (fBlockHeight + 80.0f) - fHeight;
    }


    return bLanding;
}