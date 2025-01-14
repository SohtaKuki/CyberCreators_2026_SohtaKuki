//=================================================
//
// 入力処理に必要なヘッダーファイル (input.h)
// Author: Sohta Kuki
//
//=================================================

#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"


//マクロ定義
#define NUM_KEY_MAX (256) //キーの最大数

//入力クラス
class CInput
{
public:
	CInput();
	~CInput();
	virtual HRESULT Init(HINSTANCE hInstance,HWND hWnd);
	virtual void Uninit();
	virtual void Update() = 0;
protected:
	static LPDIRECTINPUT8 m_pInput; //DirectInputオブジェクト
	LPDIRECTINPUTDEVICE8 m_pDevKeyboard;
};

//キーボード入力処理
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit();
	void Update();

	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);

private:
	BYTE m_aKeyState[NUM_KEY_MAX];
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];
};

//コントローラー入力
class CInputJoypad : public CInput
{
public:

	//キーの種類
	typedef enum
	{
		JOYKEY_UP = 0,
		JOYKEY_DOWN,
		JOYKEY_LEFT,
		JOYKEY_RIGHT,
		JOYKEY_START,
		JOYKEY_BACK,
		JOYKEY_L3,
		JOYKEY_R3,
		JOYKEY_LB,
		JOYKEY_RB,
		JOYKEY_LT,
		JOYKEY_RT,
		JOYKEY_A,
		JOYKEY_B,
		JOYKEY_X,
		JOYKEY_Y,
		JOYKEY_MAX
	}JOYKEY;

	//スティックの番号
	typedef enum
	{
		STICKTYPE_LEFT = 0, // L3スティック
		STICKTYPE_RIGHT,	// R3スティック
		STICKTYPE_MAX
	}STICKTYPE;

	//スティックの方向
	typedef enum
	{
		STICKANGLE_UP = 0, // 上
		STICKANGLE_DOWN,   // 下
		STICKANGLE_LEFT,   // 左
		STICKANGLE_RIGHT,  // 右
		STICKANGLE_MAX,
	}STICKANGLE;

	//スティック関連の入力
	typedef struct
	{
		float afTplDiameter[STICKTYPE_MAX];
		float afAngle[STICKTYPE_MAX];
		bool abAnglePress[STICKTYPE_MAX][STICKANGLE_MAX];
		bool abAngleTrigger[STICKTYPE_MAX][STICKANGLE_MAX];
		bool abAngleRepeat[STICKTYPE_MAX][STICKANGLE_MAX];
		bool abAngleRelease[STICKTYPE_MAX][STICKANGLE_MAX];
	}STICKINPUT;

	//基本処理
	CInputJoypad();
	~CInputJoypad();
	HRESULT Init(void);
	void Uninit() override;
	void Update() override;
	D3DXVECTOR2 GetStickPosition(STICKTYPE stickType);
	D3DXVECTOR2 GetTriggerPosition(JOYKEY TriggerType);
	bool GetStickTrigger(STICKTYPE stickType, STICKANGLE angle);

	//キーの入力状態を返す
	bool GetPress(JOYKEY key);		//プレス状態取得
	bool GetTrigger(JOYKEY key);	//トリガー状態を取得

private:

	//メンバ変数
	BYTE m_aKeyState[NUM_KEY_MAX];				
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];		
	int m_KeyPressCount;						
	int m_JoyKeyPressCount;						
	XINPUT_STATE m_joykeyState;					
	XINPUT_STATE m_joykeyStateTrigger;			
	int m_Time;									
	WORD m_Strength;							
	D3DXVECTOR3 m_joykeyStickPos;				
	bool m_bAngle[STICKTYPE_MAX][STICKANGLE_MAX];
	DWORD m_aStickCurrentTime[STICKTYPE_MAX][STICKANGLE_MAX]; 
	DWORD m_aStickExecLastTime[STICKTYPE_MAX][STICKANGLE_MAX];
	STICKINPUT m_stick;					
	XINPUT_STATE m_XInput;				
	XINPUT_STATE m_joykeyStateRelease;
	XINPUT_STATE m_joykeyCurrentTime;
	XINPUT_STATE m_joykeyExecLastTime;
	XINPUT_STATE m_joykeyInput;			
	XINPUT_VIBRATION m_joykeyMoter;		
	WORD m_Button;						
	WORD m_OldButton;					
};


#endif