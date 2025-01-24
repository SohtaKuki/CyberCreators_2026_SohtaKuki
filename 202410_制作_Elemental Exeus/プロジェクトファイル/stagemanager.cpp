//=================================================
//
// ステージマネージャー処理 (texture.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "stagemanager.h"

#include "3denemy.h"
#include "3dblock.h"
#include "startobj.h"
#include "3ditem.h"
#include "objectX.h"
#include "3dplayer.h"
#include "3dgoalobj.h"
#include "billboard.h"
#include "timer.h"
#include "floor.h"
#include "3dmoveblock.h"
#include "blizardfloor.h"
#include "3dbackground.h"
#include "floorBG.h"
#include "3denemy_air.h"
#include "3dboss_switch.h"

//======================
//コンストラクタ
//======================
CStageManager::CStageManager()
{

}

//======================
//デストラクタ
//======================
CStageManager::~CStageManager()
{
    m_pFile = nullptr;
}

//======================
//初期化処理
//======================
HRESULT CStageManager::Init()
{
	return S_OK;
}

//======================
// 終了処理
//======================
void CStageManager::Uninit()
{

}

//======================
// 更新処理
//======================
void CStageManager::Update()
{

}

//======================
// 描画処理
//======================
void CStageManager::Draw()
{

}

//======================
// オブジェクト生成処理
//======================
CStageManager* CStageManager::Create(int nType)
{
    CStageManager* D3DStageManager = nullptr;

    D3DStageManager = new CStageManager;

    //初期化に成功した場合
    if (SUCCEEDED(D3DStageManager->Init()))
    {
        D3DStageManager->m_nType = nType;

        D3DStageManager->LoadStageData();
    }

    return D3DStageManager;
}

//======================
// ステージ配置情報読み込み処理
//======================
void CStageManager::LoadStageData()
{
    char Datacheck[MAX_CHAR];
    int nCntObjectData = 0;
    int CreateObjType[MAX_OBJ];
    int CreateObjType2[MAX_OBJ];

    //ステージを生成するデータを読み込む先を識別
    if (m_nType == 0)
    {
        m_pFile = fopen("data\\LOADSTAGE\\loadstage000.txt", "r");//ファイルを開く
    }

    if (m_nType == 1)
    {
        m_pFile = fopen("data\\LOADSTAGE\\loadstage001.txt", "r");//ファイルを開く
    }

    if (m_nType == 2)
    {
        m_pFile = fopen("data\\LOADSTAGE\\loadstage002.txt", "r");//ファイルを開く
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


            if (Datacheck[0] == '#')
            {
                continue;
            }

            //ファイルを閉じる
            if (!strcmp(Datacheck, "END_STAGESET"))
            {
                fclose(m_pFile);
                break;
            }

            //制限時間の設定
            if (!strcmp(Datacheck, "TIMERSET"))
            {
                fscanf(m_pFile, "%s", Datacheck);
                fscanf(m_pFile, "%d", &m_nTimer);
                continue;
            }

            //オブジェクトの各種設定
            if (!strcmp(Datacheck, "OBJECTSET"))
            {
                while (1)
                {
                    fscanf(m_pFile, "%s", Datacheck);

                    //オブジェクト情報読み込みの終了
                    if (!strcmp(Datacheck, "END_OBJECTSET"))
                    {
                        break;
                    }

                    //生成するオブジェクトの種類
                    if (!strcmp(Datacheck, "OBJ_TYPE"))
                    {
                        fscanf(m_pFile, "%s", Datacheck);
                        fscanf(m_pFile, "%d", &CreateObjType[nCntObjectData]);
                    }

                    //オブジェクトの座標
                    if (!strcmp(Datacheck, "POS"))
                    {
                        fscanf(m_pFile, "%s", Datacheck);
                        fscanf(m_pFile, "%f", &m_nPos[nCntObjectData].x);
                        fscanf(m_pFile, "%f", &m_nPos[nCntObjectData].y);
                        fscanf(m_pFile, "%f", &m_nPos[nCntObjectData].z);
                    }

                    //オブジェクトの向き
                    if (!strcmp(Datacheck, "ROT"))
                    {
                        fscanf(m_pFile, "%s", Datacheck);
                        fscanf(m_pFile, "%f", &m_nRot[nCntObjectData].x);
                        fscanf(m_pFile, "%f", &m_nRot[nCntObjectData].y);
                        fscanf(m_pFile, "%f", &m_nRot[nCntObjectData].z);
                    }

                    //オブジェクトの向き
                    if (!strcmp(Datacheck, "SIZE"))
                    {
                        fscanf(m_pFile, "%s", Datacheck);
                        fscanf(m_pFile, "%f", &m_nSize[nCntObjectData].x);
                        fscanf(m_pFile, "%f", &m_nSize[nCntObjectData].y);
                        fscanf(m_pFile, "%f", &m_nSize[nCntObjectData].z);
                    }

                    //オブジェクトの詳細設定 (一部のオブジェクトでしか使用しない)
                    if (!strcmp(Datacheck, "OBJ_TYPE2"))
                    {
                        fscanf(m_pFile, "%s", Datacheck);
                        fscanf(m_pFile, "%d", &CreateObjType2[nCntObjectData]);
                    }
                }

                nCntObjectData++;
            }

    }

    //デバイスの取得
    LPDIRECT3DDEVICE9 pDevice = nullptr;
    pDevice = CManager::GetRenderer()->GetDevice();

    //制限時間を代入
    CTimer::AddTimer(m_nTimer);

    //上記で得た情報を代入
    for (int nCnt = 0; nCnt < nCntObjectData; nCnt++)
    {
        //プレイヤー生成の場合
        if (CreateObjType[nCnt] == 0)
        {
            C3dplayer::Create(D3DXVECTOR3(m_nPos[nCnt].x, m_nPos[nCnt].y, m_nPos[nCnt].z));
            C3dstartobj::Create();
        }

        //敵生成の場合
        if (CreateObjType[nCnt] == 1)
        {
            C3denemy::Create(D3DXVECTOR3(m_nPos[nCnt].x, m_nPos[nCnt].y, m_nPos[nCnt].z), CreateObjType2[nCnt]);
        }

        //ブロック生成の場合
        if (CreateObjType[nCnt] == 2)
        {
            C3dblock::Create(D3DXVECTOR3(m_nPos[nCnt].x, m_nPos[nCnt].y, m_nPos[nCnt].z),(CreateObjType2[nCnt]));
        }

        //アイテム生成の場合
        if (CreateObjType[nCnt] == 4)
        {
            C3ditem::Create(D3DXVECTOR3(m_nPos[nCnt].x, m_nPos[nCnt].y, m_nPos[nCnt].z), CreateObjType2[nCnt]);
        }

        //ゴールマーカーの場合
        if (CreateObjType[nCnt] == 6)
        {
            C3dgoalobj::Create(D3DXVECTOR3(m_nPos[nCnt].x, m_nPos[nCnt].y, m_nPos[nCnt].z), CreateObjType2[nCnt]);
        }

        //床の生成
        if (CreateObjType[nCnt] == 7)
        {
            CFloor::Create(D3DXVECTOR3(m_nPos[nCnt].x, m_nPos[nCnt].y, m_nPos[nCnt].z) ,D3DXVECTOR3(m_nSize[nCnt].x, m_nSize[nCnt].y, m_nSize[nCnt].z), CreateObjType2[nCnt]);
        }

        //移動床ブロック生成の場合
        if (CreateObjType[nCnt] == 8)
        {
            C3dmoveblock::Create(D3DXVECTOR3(m_nPos[nCnt].x, m_nPos[nCnt].y, m_nPos[nCnt].z),CreateObjType2[nCnt]);
        }

        //吹雪床ブロックの場合
        if (CreateObjType[nCnt] == 9)
        {
            CBlizardfloor ::Create(D3DXVECTOR3(m_nPos[nCnt].x, m_nPos[nCnt].y, m_nPos[nCnt].z), D3DXVECTOR3(m_nSize[nCnt].x, m_nSize[nCnt].y, m_nSize[nCnt].z));
        }

        //床の生成
        if (CreateObjType[nCnt] == 10)
        {
            CFloorBG::Create(D3DXVECTOR3(m_nPos[nCnt].x, m_nPos[nCnt].y, m_nPos[nCnt].z), D3DXVECTOR3(m_nSize[nCnt].x, m_nSize[nCnt].y, m_nSize[nCnt].z), CreateObjType2[nCnt]);
        }

        //敵生成の場合
        if (CreateObjType[nCnt] == 11)
        {
            C3denemyair::Create(D3DXVECTOR3(m_nPos[nCnt].x, m_nPos[nCnt].y, m_nPos[nCnt].z), CreateObjType2[nCnt]);
        }

        //ブロック生成の場合
        if (CreateObjType[nCnt] == 12)
        {
            C3dswitchbs::Create(D3DXVECTOR3(m_nPos[nCnt].x, m_nPos[nCnt].y, m_nPos[nCnt].z), (CreateObjType2[nCnt]));
        }
    }
}
