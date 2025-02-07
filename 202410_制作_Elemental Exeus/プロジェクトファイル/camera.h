//=================================================
//
// カメラ処理 (camera.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//爆発オブジェクトクラス
class CCamera
{
public:
	CCamera();
	~CCamera();
	HRESULT Init();
	void Uninit();
	void Update();
	void SetCamera();
	static void SetShake(int shakeframe,float shake);
private:
	D3DXVECTOR3 m_posV;	//視点
	D3DXVECTOR3 m_posR;	//注視点
	D3DXVECTOR3 m_vecU;	//上方向ベクトル
	D3DXVECTOR3 m_rot;	//回転
	D3DXMATRIX m_mtxProjection;	//プロジェクションマトリックス
	D3DXMATRIX m_mtxView;		//ビューマトリックス
	static int m_nShakeframe;
	static float m_fShake;
	bool m_bBlizSwitch;
	float m_fBlizCnt; //吹雪表現再現アニメカウント
	float m_fStartPos; //フォグの開始地点
	float m_fEndPos; //フォグの終了地点
	float m_fFogDisity; //フォグの密度
	float m_fDistance;
	D3DXVECTOR3 m_targetPosV;
	D3DXVECTOR3 m_targetPosR;
	float m_followSpeed;
	
};

#endif
