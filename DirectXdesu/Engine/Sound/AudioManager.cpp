#include "AudioManager.h"

void AudioManager::Init(const std::string& directoryPath_) {
	directoryPath = directoryPath_;

	HRESULT result;
	IXAudio2MasteringVoice* masterVoice;

	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));

	result = xAudio2->CreateMasteringVoice(&masterVoice);
	assert(SUCCEEDED(result));
}

void AudioManager::Finalize() { 
	xAudio2.Reset();
	// 音声データを解放
	std::map<std::string, SoundData>::iterator it = soundDatas.begin();
	for (; it != soundDatas.end(); ++it) {
		SoundUnLoad(&it->second);
	}
	soundDatas.clear();
}

void AudioManager::SoundLoadWave(const std::string& fileName) {
	if (soundDatas.find(fileName) != soundDatas.end()) {
		// 重複読み込み防止
		return;
	}

	std::string fullPath = directoryPath + fileName;

#pragma region ファイルオープン
	// ファイル入力ストリームのインスタンス
	std::ifstream file;
	// .wavファイルをバイナリモードで開く
	file.open(fullPath, std::ios_base::binary);
	// ファイルオーブン失敗検出
	assert(file.is_open());
#pragma endregion
#pragma region.wavデータ読み込み
	// RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	// ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	// タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}
	// Formatチャンクの読み込み
	FormatChunk format = {};
	// チャンクヘッダーの確認
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}
	// チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);
	// Dataチャンクの読み込み
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	// JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK", 4) == 0) {
		// 読み取り位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		// 再読み込み
		file.read((char*)&data, sizeof(data));
	}

	// LISTチャンクを検出した場合
	if (strncmp(data.id, "LIST", 4) == 0) {
		// 読み取り位置をLISTチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		// 再読み込み
		file.read((char*)&data, sizeof(data));
	}

	// FACTチャンクを検出した場合
	if (strncmp(data.id, "fact", 4) == 0) {
		// 読み取り位置をLISTチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		// 再読み込み
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0) {
		assert(0);
	}
	// Dataチャンクのデータ部(波形データ)の読み込み
	char* pBuffer = new char[static_cast<size_t>(data.size)];
	file.read(pBuffer, data.size);
#pragma endregion
#pragma region ファイルクローズ
	// Waveファイルを閉じる
	file.close();
#pragma endregion
#pragma region 読み込んだ音声データをmapへ
	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = static_cast<unsigned int>(data.size);

	soundDatas.insert(std::make_pair(fullPath, soundData));
#pragma endregion
}

void AudioManager::SoundUnLoad(SoundData* soundData) {
	//	バッファのメモリを解放
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

void AudioManager::SoundPlayWave(std::string fileName, float volume, bool isLoop) {
	HRESULT result;

	std::string fullPath = directoryPath + fileName;

	std::map<std::string, SoundData>::iterator it = soundDatas.find(fullPath);
	// 未読み込みの検出
	assert(it != soundDatas.end());

	SoundData& soundData = it->second;

	// 波形フォーマットを元にSourceVoiceの生成
	// IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&soundData.pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));
	// 再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	if (isLoop) {
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	buf.Flags = XAUDIO2_END_OF_STREAM;
	// 波形データの再生
	result = soundData.pSourceVoice->SetVolume(volume);
	result = soundData.pSourceVoice->SubmitSourceBuffer(&buf);
	result = soundData.pSourceVoice->Start();
}

void AudioManager::SoundStopWave(std::string fileName) {
	std::string fullPath = directoryPath + fileName;

	std::map<std::string, SoundData>::iterator it = soundDatas.find(fullPath);
	// 未読み込みの検出
	assert(it != soundDatas.end());
	// サウンドデータの参照を取得
	SoundData& soundData = it->second;

	HRESULT result;

	// 波形フォーマットをもとにSourceVoiceの生成
	IXAudio2SourceVoice* pSourceVoice = soundData.pSourceVoice;
	result = xAudio2->CreateSourceVoice(&soundData.pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	// 再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;

	buf.Flags = XAUDIO2_END_OF_STREAM;

	if (pSourceVoice != nullptr) {
		result = pSourceVoice->Stop();
		result = pSourceVoice->FlushSourceBuffers();
		result = pSourceVoice->SubmitSourceBuffer(&buf);
	}
}

// AudioManager* AudioManager::GetInstance() {
//	static AudioManager instance;
//	return &instance;
// }
