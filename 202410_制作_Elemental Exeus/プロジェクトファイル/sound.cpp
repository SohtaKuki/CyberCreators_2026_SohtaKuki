//=============================================================================
//
// サウンド処理 [sound.cpp]
// Author : AKIRA TANAKA
//
//=============================================================================
#include "sound.h"

//*****************************************************************************
// サウンド情報の構造体定義
//*****************************************************************************
typedef struct
{
	const char* pFilename;	// 音源のファイル名
	int nCntLoop;			// 音源のループする回数
	float Volume;			// 音源ボリューム
} SOUNDINFO;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

// サウンドの情報
SOUNDINFO g_aSoundInfo[CSound::SOUND_LABEL_MAX] =
{
	{"data\\SOUND\\BGM\\title000.wav", -1, 0.5f}, //タイトル画面BGM
	{"data\\SOUND\\BGM\\stageselect000.wav", -1, 0.3f}, //ステージセレクト画面BGM
	{"data\\SOUND\\BGM\\stage000.wav", -1, 0.3f}, //ステージ1BGM
	{"data\\SOUND\\BGM\\stage001.wav", -1, 0.3f}, //ステージ2BGM
	{"data\\SOUND\\BGM\\Stage_BOSS.wav", -1, 0.3f}, //ステージボスBGM
	{"data\\SOUND\\BGM\\stage_tutorial.wav", -1, 0.3f}, //チュートリアルステージBGM
	{"data\\SOUND\\BGM\\result000.wav", -1, 0.3f}, //リザルト画面BGM
	{"data\\SOUND\\BGM\\ranking000.wav", -1, 0.3f}, //リザルト画面BGM
	{"data\\SOUND\\SE\\select000.wav", 0, 0.8f}, //選択SE
	{"data\\SOUND\\SE\\decide000.wav", 0, 0.8f}, //決定SE
	{"data\\SOUND\\SE\\start000.wav", 0, 1.0f}, //スタートコールSE
	{"data\\SOUND\\SE\\TornadoShot000.wav", 0, 0.5f}, //竜巻攻撃SE
	{"data\\SOUND\\SE\\TornadoJump000.wav", 0, 1.0f}, //竜巻ジャンプSE
	{"data\\SOUND\\SE\\playerdamage000.wav", 0, 0.5f}, //プレイヤーダメージSE
	{"data\\SOUND\\SE\\enemydamage000.wav", 0, 0.5f}, //敵ダメージSE
	{"data\\SOUND\\SE\\walk000.wav", 0, 0.7f}, //プレイヤー歩行SE
	{"data\\SOUND\\SE\\TornadoStart000.wav", 0, 0.5f}, //竜巻出現SE
	{"data\\SOUND\\SE\\SlashWind_1stCmb.wav", 0, 1.0f}, //スラッシュウィンド1コンボ目
	{"data\\SOUND\\SE\\SlashWind_2ndCmb.wav", 0, 1.0f}, //スラッシュウィンド2コンボ目
	{"data\\SOUND\\SE\\SlashWind_3rdCmb.wav", 0, 1.0f}, //スラッシュウィンド3コンボ目
	{"data\\SOUND\\SE\\BrokenBlock_Death.wav", 0, 0.7f}, //破壊可能ブロック破壊SE
	{"data\\SOUND\\SE\\jump000.wav", 0, 0.3f}, //プレイヤージャンプSE
	{"data\\SOUND\\SE\\startbtn000.wav", 0, 0.7f}, //スタートボタンSE
	{"data\\SOUND\\SE\\BOSS_ELIMITED.wav", 0, 0.7f}, //クリア成功SE
	{"data\\SOUND\\SE\\failed000.wav", 0, 0.7f}, //クリア失敗SE
	{"data\\SOUND\\SE\\BossMove000.wav", 0, 0.7f}, //ボス移動SE
	{"data\\SOUND\\SE\\BossTornadoSTARTUP.wav", 0, 0.7f}, //ボス竜巻攻撃準備SE
	{"data\\SOUND\\SE\\BossWideTornado_STRUP.wav", 0, 1.0f}, //ボス両端竜巻攻撃準備SE
	{"data\\SOUND\\SE\\BossTornadoWIDE_SHOT.wav", 0, 1.0f}, //ボス両端竜巻攻撃開始SE
	{"data\\SOUND\\SE\\SlashWind_3rdCmb.wav", 0, 1.0f}, //ボス両端竜巻攻撃開始SE
	{"data\\SOUND\\SE\\MELEEATK000.wav", 0, 1.0f}, //ボス近接攻撃SE
	{"data\\SOUND\\SE\\DASHATK000.wav", 0, 1.0f}, //ボス突進攻撃SE
	{"data\\SOUND\\SE\\Boss_EntryMove.wav", 0, 0.4f}, //ボス入場時移動SE
	{"data\\SOUND\\SE\\BossnameDisplay.wav", 0, 0.6f}, //ボス名前表示時SE
	{"data\\SOUND\\SE\\BOSS_ENTRY_MOTION.wav", 0, 0.6f}, //ボス入場モーションSE
	{"data\\SOUND\\SE\\BossTornadoShot000.wav", 0, 0.5f}, //竜巻攻撃SE
	{"data\\SOUND\\SE\\Warning000.wav", -1, 1.0f}, //竜巻攻撃SE
	{"data\\SOUND\\SE\\BossExplosion000.wav", 0, 0.8f}, //ボス爆発SE
};

//=============================================================================
// コンストラクタ
//=============================================================================
CSound::CSound()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CSound::~CSound()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSound::Init(HWND hWnd)
{
	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&m_pXAudio2, 0);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// マスターボイスの生成
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if (m_pXAudio2 != NULL)
		{
			// XAudio2オブジェクトの開放
			m_pXAudio2->Release();
			m_pXAudio2 = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// サウンドデータの初期化
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		hFile = CreateFile(g_aSoundInfo[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		if (dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// オーディオデータ読み込み
		hr = CheckChunk(hFile, 'atad', &m_aSizeAudio[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		m_apDataAudio[nCntSound] = (BYTE*)malloc(m_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, m_apDataAudio[nCntSound], m_aSizeAudio[nCntSound], dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// ソースボイスの生成
		hr = m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[nCntSound], &(wfx.Format));
		if (FAILED(hr))
		{
			MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// バッファの値設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = m_aSizeAudio[nCntSound];
		buffer.pAudioData = m_apDataAudio[nCntSound];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = g_aSoundInfo[nCntSound].nCntLoop;

		// オーディオバッファの登録
		m_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

		// ファイルをクローズ
		CloseHandle(hFile);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSound::Uninit(void)
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if (m_apSourceVoice[nCntSound] != NULL)
		{
			// 一時停止
			m_apSourceVoice[nCntSound]->Stop(0);

			// ソースボイスの破棄
			m_apSourceVoice[nCntSound]->DestroyVoice();
			m_apSourceVoice[nCntSound] = NULL;

			// オーディオデータの開放
			free(m_apDataAudio[nCntSound]);
			m_apDataAudio[nCntSound] = NULL;
		}
	}

	// マスターボイスの破棄
	m_pMasteringVoice->DestroyVoice();
	m_pMasteringVoice = NULL;

	if (m_pXAudio2 != NULL)
	{
		// XAudio2オブジェクトの開放
		m_pXAudio2->Release();
		m_pXAudio2 = NULL;
	}

	// COMライブラリの終了処理
	CoUninitialize();
}

//=============================================================================
// セグメント再生(再生中なら停止)
//=============================================================================
HRESULT CSound::PlaySound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_aSizeAudio[label];
	buffer.pAudioData = m_apDataAudio[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = g_aSoundInfo[label].nCntLoop;

	// 状態取得
	m_apSourceVoice[label]->GetState(&xa2state);

	// 音量取得
	m_apSourceVoice[label]->SetVolume(g_aSoundInfo[label].Volume);

	if (xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		m_apSourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		m_apSourceVoice[label]->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	m_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// 再生
	m_apSourceVoice[label]->Start(0);

	return S_OK;
}

//=============================================================================
// セグメント停止(ラベル指定)
//=============================================================================
void CSound::Stop(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	m_apSourceVoice[label]->GetState(&xa2state);

	// 音量取得
	m_apSourceVoice[label]->SetVolume(g_aSoundInfo[label].Volume);

	if (xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		m_apSourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		m_apSourceVoice[label]->FlushSourceBuffers();
	}
}

//=============================================================================
// 全てのセグメント停止
//=============================================================================
void CSound::Stop(void)
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if (m_apSourceVoice[nCntSound] != NULL)
		{
			// 一時停止
			m_apSourceVoice[nCntSound]->Stop(0);
		}
	}
}

//=============================================================================
// チャンクのチェック
//=============================================================================
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

//=============================================================================
// チャンクデータの読み込み
//=============================================================================
HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}