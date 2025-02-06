#pragma once
#include<vector>
#include<string>
#include<MusicDesc.h>
#include"KObject3d.h"
#include"ModelManager.h"
#include"PipelineManager.h"
#include"Camera.h"
#include<memory>

struct Key {
	int value;
	bool operator<(const Key& other) const {
		return value < other.value;
	}
};

class NoteObj
{
public:
	NoteObj() ;
	~NoteObj() ;
	void Init(MusicDesc* music_);
	void Update(Camera* camera_);
	void Draw();
	std::map<Key, std::unique_ptr<KObject3d>>& Obj() {
		return obj;
	}
	std::map<Key, Note>& Notes() {
		return notes;
	}
	void LoadNote(const std::string& name);
	void OutputNote(const std::string& name);

	//小節　サイズの限界まで
	//拍数　99.5まで
	//レーン　9まで
	int32_t SetKey(const Beat& beat, const int lane) {
		int num = 0;
		num += beat.measure * 1000 + beat.beat * 100 + lane;

		return num;
	}
private:
	std::map<Key, std::unique_ptr<KObject3d>>obj;
	std::map<Key, Note>notes;

	std::vector<std::unique_ptr<KObject3d>>underObj;
	ModelManager* modelM;
	PipelineManager* pipelineM;
	std::string fileName;
	float speed = 3.0f;
	float posZ = 0.1f;
	float playTime;
	float notePosZ;
	float sec = 0.1f;
	MusicDesc* music;
};