//=================================================
//
// 3Dモデルのブロックの表示処理 (3dwall.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "3dwall.h"
#include "3dplayer.h"


//======================
// コンストラクタ
//======================
C3dwall::C3dwall(int nPriority) : CModel(nPriority)
{

}

//======================
// デストラクタ
//======================
C3dwall::~C3dwall()
{

}

//======================
// 初期化処理
//======================
HRESULT C3dwall::Init()
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
void C3dwall::Uninit()
{
    if (m_pTexBuff != nullptr)
    {
        m_pTexBuff->Release();
        m_pTexBuff = nullptr;
    }

    CModel::Uninit();
}

//======================
// 更新処理
//======================
void C3dwall::Update()
{
    int nFadeState = CFade::GetFadeState();

    //不具合が起きた場合消しましょう
    if (nFadeState == CFade::FADE_OUT)
    {

        C3dwall::Uninit();
    }
}

//======================
// 描画処理
//======================
void C3dwall::Draw()
{
    LPDIRECT3DDEVICE9 pDevice = nullptr;
    pDevice = CManager::GetRenderer()->GetDevice();

    D3DXMATRIX mtxRot, mtxTrans;
    D3DMATERIAL9 matDef;
    D3DXMATERIAL* pMat;
    D3DXVECTOR3 Pos = CObject3D::GetPos();

    D3DXMatrixIdentity(&m_mtxworld);
    D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
    D3DXMatrixMultiply(&m_mtxworld, &m_mtxworld, &mtxRot);
    D3DXMatrixTranslation(&mtxTrans, Pos.x, Pos.y, Pos.z);
    D3DXMatrixMultiply(&m_mtxworld, &m_mtxworld, &mtxTrans);

    pDevice->SetTransform(D3DTS_WORLD, &m_mtxworld);
    pDevice->GetMaterial(&matDef);

    for (int nCntParts = 0; nCntParts < NUM_MODEL; nCntParts++)
    {
        D3DXMatrixIdentity(&m_aModel[nCntParts].mtxworld);
        D3DXMatrixRotationYawPitchRoll(&mtxRot, m_aModel[nCntParts].rot.y, m_aModel[nCntParts].rot.x, m_aModel[nCntParts].rot.z);
        D3DXMatrixMultiply(&m_aModel[nCntParts].mtxworld, &m_aModel[nCntParts].mtxworld, &mtxRot);
        D3DXMatrixTranslation(&mtxTrans, m_aModel[nCntParts].pos.x, m_aModel[nCntParts].pos.y, m_aModel[nCntParts].pos.z);
        D3DXMatrixMultiply(&m_aModel[nCntParts].mtxworld, &m_aModel[nCntParts].mtxworld, &mtxTrans);

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
            pDevice->SetTransform(D3DTS_WORLD, &m_aModel[nCntParts].mtxworld);

            pMat = (D3DXMATERIAL*)m_pBuffMat[nCntParts]->GetBufferPointer();

            for (int nCntMat = 0; nCntMat < (int)m_nNumMat[nCntParts]; nCntMat++)
            {
                pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
                pDevice->SetTexture(0, m_pTexBuff);  //インスタンスのテクスチャを使用
                m_pMesh[nCntParts]->DrawSubset(nCntMat);
            }
        }
    }

    pDevice->SetMaterial(&matDef);
}


//======================
// オブジェクト生成処理
//======================
C3dwall* C3dwall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
    C3dwall* D3DWall = nullptr;

    D3DWall = new C3dwall;

    //初期化に成功した場合
    if (SUCCEEDED(D3DWall->Init()))
    {
        if (nType == 0)
        {
            D3DWall->SetType(TYPE::WALL_WIDTH);
            D3DWall->Load("data\\TEXTURE\\icetexture003.png");
        }

        if (nType == 1)
        {
            D3DWall->SetType(TYPE::WALL_HEIGHT);
            D3DWall->Load("data\\TEXTURE\\icetexture000.png");
        }

        if (nType == 2)
        {
            D3DWall->SetType(TYPE::WALL_WIDTH_SHORT);
            D3DWall->Load("data\\TEXTURE\\icetexture000.png");
        }

        if (nType == 3)
        {
            D3DWall->SetType(TYPE::WALL_HEIGHT_SHORT);
            D3DWall->Load("data\\TEXTURE\\icetexture003.png");
        }

        D3DWall->LoadWallData();


        D3DWall->CObject3D::SetPos(pos);

        D3DWall->m_rot = rot;

        ////テクスチャの設定
        //Model->BindTexture(m_pTexBuff);
    }

    return D3DWall;
}

//======================
// テクスチャロード処理
//======================
HRESULT C3dwall::Load(LPCSTR textureFileName)
{
    LPDIRECT3DDEVICE9 pDevice = nullptr;
    pDevice = CManager::GetRenderer()->GetDevice();

    if (FAILED(D3DXCreateTextureFromFile(pDevice, textureFileName, &m_pTexBuff)))
    {
        return E_FAIL;
    }

    return S_OK;
}

//======================
// テクスチャアンロード(終了)処理
//======================
void C3dwall::Unload()
{
    CModel::Unload();
}



//===========================
// 外部ファイル読み込み処理
//===========================
void C3dwall::LoadWallData(void)
{
    char Datacheck[MAX_CHAR];
    int nCntEnemyData = 0;
    int EnemyModelSave = 0;

    if (GetType() == TYPE::WALL_WIDTH)
    {
        m_pFile = fopen("data\\MODEL\\MODEL_Wall\\icewall_profile.txt", "r");//ファイルを開く
    }

    if (GetType() == TYPE::WALL_HEIGHT)
    {
        m_pFile = fopen("data\\MODEL\\MODEL_Wall\\icewall_profile_yrot.txt", "r");//ファイルを開く
    }

    if (GetType() == TYPE::WALL_WIDTH_SHORT)
    {
        m_pFile = fopen("data\\MODEL\\MODEL_Wall\\icewall_profile_short.txt", "r");//ファイルを開く
    }

    if (GetType() == TYPE::WALL_HEIGHT_SHORT)
    {
        m_pFile = fopen("data\\MODEL\\MODEL_Wall\\icewall_profile_yrot_short.txt", "r");//ファイルを開く
    }

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
bool C3dwall::Collision3DWall(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fHeight)
{
    if (GetType() == TYPE::WALL_WIDTH)
    {
        bool bLanding = false; //重力を適応した場合のみ使用
        float fBlockWidth = 85.0f;
        float fBlockDepth = 0.0f;

        D3DXVECTOR3 Pos = CObject3D::GetPos();


        //右側当たり判定
        if (pPos->x - fWidth <= Pos.x + fBlockWidth && pPosOld->x - fWidth >= Pos.x + fBlockWidth && pPos->z - fHeight < Pos.z + fBlockDepth - 33.0f && pPos->z  > Pos.z - fBlockDepth - 24.0f)
        {
            pPos->x = Pos.x + fBlockWidth + fWidth;
        }

        //左側当たり判定
        else if (pPos->x + fWidth >= Pos.x - fBlockWidth && pPosOld->x + fWidth <= Pos.x - fBlockWidth && pPos->z - fHeight < Pos.z + fBlockDepth - 33.0f && pPos->z > Pos.z - fBlockDepth - 24.0f)
        {
            pPos->x = Pos.x - fBlockWidth - fWidth;
        }

        //上側当たり判定
        if (pPos->x - fWidth < Pos.x + fBlockWidth && pPos->x + fWidth > Pos.x - fBlockWidth && pPos->z - fHeight <= Pos.z + fBlockDepth - 16.0f && pPosOld->z - fHeight >= Pos.z + fBlockDepth - 24.0f)
        {
            pPos->z = Pos.z + fBlockDepth - 16.0f + fHeight;
        }

        //下側当たり判定
        else if (pPos->x - fWidth < Pos.x + fBlockWidth && pPos->x + fWidth > Pos.x - fBlockWidth && pPos->z >= Pos.z - fBlockDepth - 24.0f && pPosOld->z <= Pos.z - fBlockDepth - 16.0f)
        {
            pPos->z = Pos.z - fBlockDepth - 24.0f;

        }

        return bLanding;
    }


}

//===========================
// ブロックの当たり判定
//===========================
bool C3dwall::Collision3DHeightWall(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fHeight)
{
    if (GetType() == TYPE::WALL_HEIGHT)
    {
        bool bLanding = false; //重力を適応した場合のみ使用
        float fBlockWidth = -20.0f;
        float fBlockDepth = 105.0f;

        D3DXVECTOR3 Pos = CObject3D::GetPos();


        //右側当たり判定
        if (pPos->x - fWidth <= Pos.x + fBlockWidth && pPosOld->x - fWidth >= Pos.x + fBlockWidth && pPos->z - fHeight < Pos.z + fBlockDepth - 20.0f && pPos->z  > Pos.z - fBlockDepth - 20.0f)
        {
            pPos->x = Pos.x + fBlockWidth + fWidth;
        }

        //左側当たり判定
        else if (pPos->x + fWidth >= Pos.x - fBlockWidth && pPosOld->x + fWidth <= Pos.x - fBlockWidth && pPos->z - fHeight < Pos.z + fBlockDepth - 20.0f && pPos->z > Pos.z - fBlockDepth - 20.0f)
        {
            pPos->x = Pos.x - fBlockWidth - fWidth;
        }

        //上側当たり判定
        if (pPos->x - fWidth < Pos.x + fBlockWidth && pPos->x + fWidth > Pos.x - fBlockWidth && pPos->z - fHeight <= Pos.z + fBlockDepth - 20.0f && pPosOld->z - fHeight >= Pos.z + fBlockDepth - 20.0f)
        {
            pPos->z = Pos.z + fBlockDepth - 20.0f + fHeight;
        }

        //下側当たり判定
        else if (pPos->x - fWidth < Pos.x + fBlockWidth && pPos->x + fWidth > Pos.x - fBlockWidth && pPos->z >= Pos.z - fBlockDepth - 20.0f && pPosOld->z <= Pos.z - fBlockDepth - 20.0f)
        {
            pPos->z = Pos.z - fBlockDepth - 20.0f;

        }

        return bLanding;
    }


}

bool C3dwall::Collision3DWallShort(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fHeight)
{
    if (GetType() == TYPE::WALL_WIDTH_SHORT)
    {
        bool bLanding = false; //重力を適応した場合のみ使用
        float fBlockWidth = 26.0f;
        float fBlockDepth = 0.0f;

        D3DXVECTOR3 Pos = CObject3D::GetPos();


        //右側当たり判定
        if (pPos->x - fWidth <= Pos.x + fBlockWidth && pPosOld->x - fWidth >= Pos.x + fBlockWidth && pPos->z - fHeight < Pos.z + fBlockDepth - 25.0f && pPos->z  > Pos.z - fBlockDepth - 16.0f)
        {
            pPos->x = Pos.x + fBlockWidth + fWidth;
        }

        //左側当たり判定
        else if (pPos->x + fWidth >= Pos.x - fBlockWidth && pPosOld->x + fWidth <= Pos.x - fBlockWidth && pPos->z - fHeight < Pos.z + fBlockDepth - 25.0f && pPos->z > Pos.z - fBlockDepth - 16.0f)
        {
            pPos->x = Pos.x - fBlockWidth - fWidth;
        }

        //上側当たり判定
        if (pPos->x - fWidth < Pos.x + fBlockWidth && pPos->x + fWidth > Pos.x - fBlockWidth && pPos->z - fHeight <= Pos.z + fBlockDepth - 16.0f && pPosOld->z - fHeight >= Pos.z + fBlockDepth - 20.0f)
        {
            pPos->z = Pos.z + fBlockDepth - 16.0f + fHeight;
        }

        //下側当たり判定
        else if (pPos->x - fWidth < Pos.x + fBlockWidth && pPos->x + fWidth > Pos.x - fBlockWidth && pPos->z >= Pos.z - fBlockDepth - 20.0f && pPosOld->z <= Pos.z - fBlockDepth - 16.0f)
        {
            pPos->z = Pos.z - fBlockDepth - 20.0f;

        }

        return bLanding;
    }
}

bool C3dwall::Collision3DHeightWallShort(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fHeight)
{
    if (GetType() == TYPE::WALL_HEIGHT_SHORT)
    {
        bool bLanding = false; //重力を適応した場合のみ使用
        float fBlockWidth = -20.0f;
        float fBlockDepth = 52.0f;

        D3DXVECTOR3 Pos = CObject3D::GetPos();


        //右側当たり判定
        if (pPos->x - fWidth <= Pos.x + fBlockWidth && pPosOld->x - fWidth >= Pos.x + fBlockWidth && pPos->z - fHeight < Pos.z + fBlockDepth - 20.0f && pPos->z  > Pos.z - fBlockDepth - 20.0f)
        {
            pPos->x = Pos.x + fBlockWidth + fWidth;
        }

        //左側当たり判定
        else if (pPos->x + fWidth >= Pos.x - fBlockWidth && pPosOld->x + fWidth <= Pos.x - fBlockWidth && pPos->z - fHeight < Pos.z + fBlockDepth - 20.0f && pPos->z > Pos.z - fBlockDepth - 20.0f)
        {
            pPos->x = Pos.x - fBlockWidth - fWidth;
        }

        //上側当たり判定
        if (pPos->x - fWidth < Pos.x + fBlockWidth && pPos->x + fWidth > Pos.x - fBlockWidth && pPos->z - fHeight <= Pos.z + fBlockDepth - 20.0f && pPosOld->z - fHeight >= Pos.z + fBlockDepth - 20.0f)
        {
            pPos->z = Pos.z + fBlockDepth - 20.0f + fHeight;
        }

        //下側当たり判定
        else if (pPos->x - fWidth < Pos.x + fBlockWidth && pPos->x + fWidth > Pos.x - fBlockWidth && pPos->z >= Pos.z - fBlockDepth - 20.0f && pPosOld->z <= Pos.z - fBlockDepth - 20.0f)
        {
            pPos->z = Pos.z - fBlockDepth - 20.0f;

        }

        return bLanding;
    }
}