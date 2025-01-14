//=================================================
//
// モデルの表示処理 (model.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "objectX.h"


//======================
// コンストラクタ
//======================
CModel::CModel(int nPriority) : CObject3D(nPriority)
{

}

//======================
// デストラクタ
//======================
CModel::~CModel()
{

}

//======================
// 初期化処理
//======================
HRESULT CModel::Init()
{
	//初期化
	if (FAILED(CObject3D::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//======================
// 終了処理
//======================
void CModel::Uninit()
{
	CObject3D::Uninit();
}

//======================
// 更新処理
//======================
void CModel::Update()
{

}

//======================
// 描画処理
//======================
void CModel::Draw()
{
    LPDIRECT3DDEVICE9 pDevice = nullptr;
    pDevice = CManager::GetRenderer()->GetDevice();

    D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス
    D3DMATERIAL9 matDef; //現在のマテリアル保存用
    D3DXMATERIAL* pMat; //マテリアルデータへのポインタ

    //ワールドマトリックスの初期化
    D3DXMatrixIdentity(&m_mtxworld);

    //向きを反映
    D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
    D3DXMatrixMultiply(&m_mtxworld, &m_mtxworld, &mtxRot);

    //位置を反映
    D3DXMatrixTranslation(&mtxTrans, m_nPos.x, m_nPos.y, m_nPos.z);
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
                pDevice->SetTexture(0, nullptr);

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
CModel* CModel::Create(D3DXVECTOR3 pos)
{
	CModel* Model = nullptr;

	Model = new CModel;

	//初期化に成功した場合
	if (SUCCEEDED(Model->Init()))
	{

		//Model->Load();

		Model->m_nPos = pos;

		////テクスチャの設定
		//Model->BindTexture(m_pTexBuff);
	}

	return Model;
}

//======================
// テクスチャロード処理
//======================
HRESULT CModel::Load()
{


	return S_OK;
}

//======================
// テクスチャアンロード(終了)処理
//======================
void CModel::Unload()
{

}


//===========================
// 外部ファイル読み込み処理
//===========================
void CModel::LoadModelData(const char* filename)
{
    char Datacheck[MAX_CHAR];
    int nCntEnemyData = 0;
    int EnemyModelSave = 0;


    FILE* pFile;//ファイルポインタを宣言
    pFile = fopen(filename, "r");//ファイルを開く

    //ファイルが存在しない場合
    if (pFile == NULL)
    {
        return;
    }

    //外部ファイル文字列読み取り
    while (1)
    {
        fscanf(pFile, "%s", Datacheck);

        if (!strcmp(Datacheck, "END_CHARACTERSET"))
        {
            fclose(pFile);
            break;
        }

        if (Datacheck[0] == '#')
        {
            continue;
        }

        //パーツ数
        if (!strcmp(Datacheck, "NUM_PARTS"))
        {
            fscanf(pFile, "%s", Datacheck);
            fscanf(pFile, "%d", &EnemyModelSave);
        }

        //ファイル名
        if (!strcmp(Datacheck, "MODEL_FILENAME"))
        {
            fscanf(pFile, "%s", Datacheck);
            fscanf(pFile, "%s", &m_aLoadEnemy[EnemyModelSave].filename[0]);
            EnemyModelSave++;
        }

        //パーツの詳細
        if (!strcmp(Datacheck, "PARTSSET"))
        {
            while (1)
            {
                fscanf(pFile, "%s", Datacheck);

                //パーツ詳細終わり検知
                if (!strcmp(Datacheck, "END_PARTSSET"))
                {
                    break;
                }

                //インデックス
                else if (!strcmp(Datacheck, "INDEX"))
                {
                    fscanf(pFile, "%s", Datacheck);
                    fscanf(pFile, "%d", &m_aLoadEnemy[nCntEnemyData].index);
                }

                //親設定
                else if (!strcmp(Datacheck, "PARENT"))
                {
                    fscanf(pFile, "%s", Datacheck);
                    fscanf(pFile, "%d", &m_aLoadEnemy[nCntEnemyData].parent);
                }

                //立ち位置
                else if (!strcmp(Datacheck, "POS"))
                {
                    fscanf(pFile, "%s", Datacheck);
                    fscanf(pFile, "%f", &m_aLoadEnemy[nCntEnemyData].pos.x);
                    fscanf(pFile, "%f", &m_aLoadEnemy[nCntEnemyData].pos.y);
                    fscanf(pFile, "%f", &m_aLoadEnemy[nCntEnemyData].pos.z);
                }

                //向き
                else if (!strcmp(Datacheck, "ROT"))
                {
                    fscanf(pFile, "%s", Datacheck);
                    fscanf(pFile, "%f", &m_aLoadEnemy[nCntEnemyData].rot.x);
                    fscanf(pFile, "%f", &m_aLoadEnemy[nCntEnemyData].rot.y);
                    fscanf(pFile, "%f", &m_aLoadEnemy[nCntEnemyData].rot.z);
                }
            }
            nCntEnemyData++;
        }
    }

    LPDIRECT3DDEVICE9 pDevice = nullptr;
    pDevice = CManager::GetRenderer()->GetDevice();

    //ここで上記の処理で得た数値を代入
    for (int nCnt = 0; nCnt < EnemyModelSave; nCnt++)
    {
        D3DXLoadMeshFromX(&m_aLoadEnemy[nCnt].filename[0], D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[nCnt], NULL, &m_nNumMat[nCnt], &m_pMesh[nCnt]);
        //D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\golden000.jpg", &g_pTextureEnemy);
        m_aModel[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        m_aModel[nCnt].nLife = 4;
        m_aModel[nCnt].nIdxModelParent = m_aLoadEnemy[nCnt].parent;
        m_aModel[nCnt].bUse = true;

        //敵の立ち位置初期化
        //m_nPos = D3DXVECTOR3(-0.0f, -0.0f, 0.0f);
        //m_aEnemy.rot = D3DXVECTOR3(-0.0f, 0.0f, 0.0f);

        m_aModel[nCnt].pos = D3DXVECTOR3(m_aLoadEnemy[nCnt].pos.x, m_aLoadEnemy[nCnt].pos.y, m_aLoadEnemy[nCnt].pos.z);
        m_aModel[nCnt].rot = D3DXVECTOR3(m_aLoadEnemy[nCnt].rot.x, m_aLoadEnemy[nCnt].rot.y, m_aLoadEnemy[nCnt].rot.z);
    }
}