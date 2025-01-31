#pragma once
#include<vector>
#include<string>
#include<MusicDesc.h>
#include"KObject3d.h"
#include"ModelManager.h"
#include"PipelineManager.h"
#include"Camera.h"

class NoteObj
{
public:
	void Init(MusicDesc* music_);
	void Update(Camera* camera_);
	void Draw();
	std::map<Beat, std::unique_ptr<KObject3d>>& Obj() {
		return obj;
	}
	std::map<Beat, std::unique_ptr<Note>>& Notes() {
		return notes;
	}
	void LoadNote(const std::string& name);
private:
	std::map<Beat, std::unique_ptr<KObject3d>>obj;
	std::map<Beat, std::unique_ptr<Note>>notes;
	ModelManager* modelM;
	PipelineManager* pipelineM;
	float speed = 3.0f;
	float posZ = 0.1f;
	float playTime;
	float notePosZ;
	float sec = 0.1f;
	MusicDesc* music;
};