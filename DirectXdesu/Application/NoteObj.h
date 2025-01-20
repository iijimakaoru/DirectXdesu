#pragma once
#include<vector>
#include"MusicDesc.h"
#include"KObject3d.h"
#include"ModelManager.h"
#include"PipelineManager.h"
#include"Camera.h"

class NoteObj
{
public:
	void Init(const std::vector<Note>& notes_, MusicDesc*music_);
	void Update(Camera*camera_);
	void Draw();
	std::vector<std::unique_ptr<KObject3d>>& Obj(){
		return obj;
	}
private:
	std::vector<std::unique_ptr<KObject3d>>obj;
	ModelManager* modelM;
	PipelineManager* pipelineM;
	float speed = 3.0f;
	float posZ = 0.1f;
	float playTime;
	float notePosZ;
	float sec = 0.1f;
	std::vector<Note> notes;
	MusicDesc* music;
};