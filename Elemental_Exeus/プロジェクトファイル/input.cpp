//=================================================
//
// 入力処理 (input.cpp)
// Author: Sohta Kuki
//
//=================================================

#include "input.h"

LPDIRECTINPUT8 CInput::m_pInput = nullptr;

//============================
//コンストラクタ
//============================
CInput::CInput()
{

}

//============================
//デストラクタ
//============================
CInput::~CInput()
{

}

//============================
//初期化処理
//============================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
    if (m_pInput == nullptr)
    {
        //DirectInputオブジェクトの生成
        if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
        {
            return E_FAIL;
        }
    }
    return S_OK;
}

//============================
//終了処理
//============================
void CInput::Uninit()
{
    //入力デバイス(キーボード)の破棄
    if (m_pDevKeyboard != NULL)
    {
        m_pDevKeyboard->Unacquire();
        m_pDevKeyboard->Release();
        m_pDevKeyboard = NULL;
    }

    //DirectInputオブジェクトの破棄
    if (m_pInput != NULL)
    {
        m_pInput->Release();
        m_pInput = NULL;
    }
}

//============================
//コンストラクタ
//============================
CInputKeyboard::CInputKeyboard()
{
    //LPDIRECTINPUTDEVICE8 g_pDevKeyboard = nullptr;
}

//============================
//デストラクタ
//============================
CInputKeyboard::~CInputKeyboard()
{

}

//============================
//キーボードの初期化処理
//============================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
    CInput::Init(hInstance, hWnd);

    //入力デバイス(キーボード)の生成
    if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevKeyboard, nullptr)))
    {
        return E_FAIL;
    }

    //データフォーマットを設定
    if (FAILED(m_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
    {
        return E_FAIL;
    }

    //協調レベルを設定
    if (FAILED(m_pDevKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
    {
        return E_FAIL;
    }

    //キーボードへのアクセス権取得
    m_pDevKeyboard->Acquire();

    return S_OK;
}

//============================
//キーボードの終了処理
//============================
void CInputKeyboard::Uninit(void)
{
    CInput::Uninit();
}

//============================
//キーボードの更新処理
//============================
void CInputKeyboard::Update(void)
{

    BYTE aKeyState[NUM_KEY_MAX];
    int nCntKey;

    if (SUCCEEDED(m_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
    {
        for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
        {
            m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
            m_aKeyState[nCntKey] = aKeyState[nCntKey];
            m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
        }
    }
    else
    {
        m_pDevKeyboard->Acquire();
    }
}

//============================
//キーボードのプレス情報を取得
//============================
bool CInputKeyboard::GetPress(int nKey)
{
    return ((m_aKeyState[nKey] & 0x80) != 0) ? true : false;
}

//============================
//キーボードのプレス情報を取得
//============================
bool CInputKeyboard::GetTrigger(int nKey)
{
    return ((m_aKeyStateTrigger[nKey] & 0x80) != 0) ? true : false;
}

//============================
//キーボードのプレス情報を取得
//============================
bool CInputKeyboard::GetRelease(int nKey)
{
    return ((m_aKeyState[nKey] & 0x80) != 0) ? false : true;
}

//============================
//コントローラーのコンストラクタ
//============================
CInputJoypad::CInputJoypad()
{

}

//============================
//コントローラーのデストラクタ
//============================
CInputJoypad::~CInputJoypad()
{

}

//============================
//コントローラーのデストラクタ
//============================
HRESULT CInputJoypad::Init()
{
    //メモリのクリア
    memset(&m_aKeyState, 0, sizeof(XINPUT_STATE));
    memset(&m_aKeyStateTrigger, 0, sizeof(XINPUT_STATE));

    //XInputのステート設定(有効にする)
    XInputEnable(true);

    // コントローラーの振動制御の0クリア
    ZeroMemory(&m_joykeyMoter, sizeof(XINPUT_VIBRATION));

    // 振動制御用の初期化
    m_Time = 0;
    m_Strength = 0;

    return S_OK;
}

//============================
//コントローラーの終了処理
//============================
void CInputJoypad::Uninit()
{
    //XInputのステートを設定
    XInputEnable(false);
}

//============================
//コントローラーの更新処理
//============================
void CInputJoypad::Update()
{
    XINPUT_STATE joykeystate; //コントローラーの入力情報

    //コントローラーの情報を取得
    if (XInputGetState(0, &joykeystate) == ERROR_SUCCESS)
    {
        m_Button = m_joykeyState.Gamepad.wButtons;							//現在のボタンの情報を設定
        m_joykeyStateTrigger.Gamepad.wButtons = m_Button & ~m_OldButton;	//トリガーの情報を設定
        m_OldButton = m_joykeyState.Gamepad.wButtons;						//前回のボタンの情報を更新

        m_joykeyState = joykeystate; //コントローラーのプレス情報を保存

        // 振動
        m_joykeyMoter.wLeftMotorSpeed = m_Strength;		// 左側振動
        m_joykeyMoter.wRightMotorSpeed = m_Strength;	// 右側振動
        XInputSetState(0, &m_joykeyMoter);				// 振動情報送信

        // 振動時間の減算
        if (m_Time > 0)
        {
            m_Time--;
        }
        else
        {
            m_Time = 0;
            m_Strength = 0;
        }
    }
}

//============================
//コントローラーのプレス情報を取得
//============================
bool CInputJoypad::GetPress(JOYKEY key)
{
    return m_joykeyState.Gamepad.wButtons & (0x01 << key);
}

//============================
//コントローラーのトリガー情報を取得
//============================
bool CInputJoypad::GetTrigger(JOYKEY key)
{
    return m_joykeyStateTrigger.Gamepad.wButtons & (0x01 << key);
}

//============================
// スティックのX軸、Y軸の値を取得（範囲は-32768～32767）
//============================
D3DXVECTOR2 CInputJoypad::GetStickPosition(STICKTYPE stickType)
{
    D3DXVECTOR2 stickPos = { 0.0f, 0.0f };

    if (stickType == STICKTYPE_LEFT)
    {
        stickPos.x = m_joykeyState.Gamepad.sThumbLX / 32768.0f; // -1.0～1.0に正規化
        stickPos.y = m_joykeyState.Gamepad.sThumbLY / 32768.0f; // -1.0～1.0に正規化
    }
    else if (stickType == STICKTYPE_RIGHT)
    {
        stickPos.x = m_joykeyState.Gamepad.sThumbRX / 32768.0f; // -1.0～1.0に正規化
        stickPos.y = m_joykeyState.Gamepad.sThumbRY / 32768.0f; // -1.0～1.0に正規化
    }



    // スティックのデッドゾーンの設定
    const float deadZone = 0.2f;
    if (fabs(stickPos.x) < deadZone) stickPos.x = 0.0f;
    if (fabs(stickPos.y) < deadZone) stickPos.y = 0.0f;

    return stickPos;
}



//============================
// トリガーY軸の値を取得（範囲は-1.0～1.0）
//============================
D3DXVECTOR2 CInputJoypad::GetTriggerPosition(JOYKEY TriggerType)
{
    D3DXVECTOR2 TriggerPos = { 0.0f, 0.0f };

    if (TriggerType == JOYKEY_LT)
    {
        TriggerPos.y = (m_joykeyState.Gamepad.bLeftTrigger / 255.0f) * 2.0f - 1.0f; // -1.0～1.0に正規化
    }
    else if (TriggerType == JOYKEY_RT)
    {
        TriggerPos.y = (m_joykeyState.Gamepad.bRightTrigger / 255.0f) * 2.0f - 1.0f; // -1.0～1.0に正規化
    }

    // デッドゾーンの設定
    const float deadZone = 0.2f;
    if (fabs(TriggerPos.y) < deadZone) TriggerPos.y = 0.0f;

    return TriggerPos;
}


//============================
// スティックのトリガー動作を取得
//============================
bool CInputJoypad::GetStickTrigger(STICKTYPE stickType, STICKANGLE angle)
{
    D3DXVECTOR2 stickPos = GetStickPosition(stickType); // スティックの現在の位置を取得

    bool triggered = false;
    const float threshold = 0.5f; // スティックの入力が有効と見なす閾値

    // スティックの方向を判定
    if (stickType == STICKTYPE_LEFT)
    {
        // 上方向のトリガー判定
        if (angle == STICKANGLE_UP && stickPos.y > threshold && !m_stick.abAnglePress[STICKTYPE_LEFT][STICKANGLE_UP])
        {
            triggered = true;
            m_stick.abAnglePress[STICKTYPE_LEFT][STICKANGLE_UP] = true;
        }
        // 下方向のトリガー判定
        else if (angle == STICKANGLE_DOWN && stickPos.y < -threshold && !m_stick.abAnglePress[STICKTYPE_LEFT][STICKANGLE_DOWN])
        {
            triggered = true;
            m_stick.abAnglePress[STICKTYPE_LEFT][STICKANGLE_DOWN] = true;
        }
        // 左方向のトリガー判定
        else if (angle == STICKANGLE_LEFT && stickPos.x < -threshold && !m_stick.abAnglePress[STICKTYPE_LEFT][STICKANGLE_LEFT])
        {
            triggered = true;
            m_stick.abAnglePress[STICKTYPE_LEFT][STICKANGLE_LEFT] = true;
        }
        // 右方向のトリガー判定
        else if (angle == STICKANGLE_RIGHT && stickPos.x > threshold && !m_stick.abAnglePress[STICKTYPE_LEFT][STICKANGLE_RIGHT])
        {
            triggered = true;
            m_stick.abAnglePress[STICKTYPE_LEFT][STICKANGLE_RIGHT] = true;
        }
    }
    else if (stickType == STICKTYPE_RIGHT)
    {
        // 同様に右スティックの方向判定を実装
        if (angle == STICKANGLE_UP && stickPos.y > threshold && !m_stick.abAnglePress[STICKTYPE_RIGHT][STICKANGLE_UP])
        {
            triggered = true;
            m_stick.abAnglePress[STICKTYPE_RIGHT][STICKANGLE_UP] = true;
        }
        else if (angle == STICKANGLE_DOWN && stickPos.y < -threshold && !m_stick.abAnglePress[STICKTYPE_RIGHT][STICKANGLE_DOWN])
        {
            triggered = true;
            m_stick.abAnglePress[STICKTYPE_RIGHT][STICKANGLE_DOWN] = true;
        }
        else if (angle == STICKANGLE_LEFT && stickPos.x < -threshold && !m_stick.abAnglePress[STICKTYPE_RIGHT][STICKANGLE_LEFT])
        {
            triggered = true;
            m_stick.abAnglePress[STICKTYPE_RIGHT][STICKANGLE_LEFT] = true;
        }
        else if (angle == STICKANGLE_RIGHT && stickPos.x > threshold && !m_stick.abAnglePress[STICKTYPE_RIGHT][STICKANGLE_RIGHT])
        {
            triggered = true;
            m_stick.abAnglePress[STICKTYPE_RIGHT][STICKANGLE_RIGHT] = true;
        }
    }

    // トリガーが発生しなかった場合、リセット処理
    if (fabs(stickPos.x) < threshold && fabs(stickPos.y) < threshold)
    {
        m_stick.abAnglePress[stickType][angle] = false;
    }

    return triggered;
}
