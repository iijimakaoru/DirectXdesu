#include "NoteObj.h"

void NoteObj::Init(const std::vector<Note>& notes_,MusicDesc* music_)
{
	modelM = ModelManager::GetInstance();
	pipelineM = PipelineManager::GetInstance();

	notes = notes_;
	music = music_;
	for (size_t i = 0; i < notes_.size(); i++)
	{
		std::unique_ptr<KObject3d> obj_;
		obj_.reset(KObject3d::Create(modelM->GetModels("S_Arrow"),
			pipelineM->GetPipeline("Obj")));
		obj_->GetTransform().SetScale({ 15.0f,15.0f,5.0f });

		//色設定
		if (notes_[i].lane == 0)
		{
			obj_->SetColor({ 0.5f,0.0f,0.0f,1.0f });
		}
		else
		{
			obj_->SetColor({ 0.0f,0.3f,1.0f,1.0f });
		}

		//方向設定
		if (notes_[i].direction == DIRECTION::left)//左
		{
			obj_->GetTransform().SetRot({ 0.0f,180.0f,0.0f });
		}
		else if (notes_[i].direction == DIRECTION::up)//上
		{
			obj_->GetTransform().SetRot({ 0.0f,0.0f,-90.0f });
		}
		else if (notes_[i].direction == DIRECTION::dawn)//下
		{
			obj_->GetTransform().SetRot({ 0.0f,180.0f,90.0f });
		}
		else											  //右
		{
			obj_->GetTransform().SetRot({ 0.0f,0.0f,0.0f });
		}
		notePosZ = (sec * speed) * music->ConvertBeatToMiliSeconds(notes_[i].beat);
		obj_->GetTransform().SetPos({ -50.0f + (100.0f * notes_[i].lane),25.0f,notePosZ });

		obj.push_back(std::move(obj_));
	}

}

void NoteObj::Update(Camera* camera_)
{
	for (size_t i = 0; i < obj.size(); i++)
	{
		if (!notes[i].isHit)
		{
			KMyMath::Vector3 move;
			move = obj[i]->GetTransform().GetPos();
			move.z -= speed;

			obj[i]->GetTransform().SetPos(move);
			obj[i]->Update(camera_->GetViewPro(), camera_->GetWorldPos());
		}
	}
}

void NoteObj::Draw()
{
	for (size_t i = 0; i < obj.size(); i++)
	{
		if (!notes[i].isHit)
		{
			obj[i]->Draw();
		}
	}
}
