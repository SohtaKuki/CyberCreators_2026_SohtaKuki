//=================================================
//
// サウンド関係処理 (sound.h)
// Author: Sohta Kuki (Base code by AKIRA TANAKA)
//
//=================================================

#ifndef _SOUND_H_ 
#define _SOUND_H_


#include "main.h"

//サウンドクラス
class CSound
{
public:
	//サウンド列挙型
	typedef enum
	{
		SOUND_LABEL_BGM = 0,
		SOUND_LABEL_BGM_STAGE_SELECT,
		SOUND_LABEL_BGM_STAGE_1,
		SOUND_LABEL_BGM_STAGE_2,
		SOUND_LABEL_BGM_STAGE_TUTORIAL,
		SOUND_LABEL_BGM_RESULT,
		SOUND_LABEL_SE_SELECT,
		SOUND_LABEL_SE_DECIDE,
		SOUND_LABEL_SE_STARTCALL2,
		SOUND_LABEL_SE_ADDLIFE,
		SOUND_LABEL_SE_SHOT,
		SOUND_LABEL_SE_INSTANTSHOT_UI,
		SOUND_LABEL_SE_INSTANTSHOT_FIRE,
		SOUND_LABEL_SE_PLAYER_DAMAGE,
		SOUND_LABEL_SE_ENEMY_DAMAGE,
		SOUND_LABEL_SE_PLAYER_WALK,
		SOUND_LABEL_SE_CHARGE_UP,
		SOUND_LABEL_SE_CHARGE_OK,
		SOUND_LABEL_SE_ADD_SPDUP,
		SOUND_LABEL_SE_BRBLOCK_DAMAGE,
		SOUND_LABEL_SE_BRBLOCK_DEATH,
		SOUND_LABEL_SE_PLAYER_JUMP,
		SOUND_LABEL_SE_STARTBTN,
		SOUND_LABEL_SE_CLEAR,
		SOUND_LABEL_SE_FAILED,
		SOUND_LABEL_MAX,
	}SOUND_LABEL;

	//メンバ関数
	CSound();							
	~CSound();							
	HRESULT Init(HWND hWnd);			
	void Uninit(void);					
	HRESULT PlaySound(SOUND_LABEL label);
	void Stop(SOUND_LABEL label);		
	void Stop(void);					
private:
	IXAudio2* m_pXAudio2 = NULL;							
	IXAudio2MasteringVoice* m_pMasteringVoice = NULL;		
	IXAudio2SourceVoice* m_apSourceVoice[SOUND_LABEL_MAX] = {};
	BYTE* m_apDataAudio[SOUND_LABEL_MAX] = {};				
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};				
};

#endif