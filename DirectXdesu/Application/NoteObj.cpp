#include "NoteObj.h"
#include<iostream>

void NoteObj::Init(MusicDesc* music_)
{
	modelM = ModelManager::GetInstance();
	pipelineM = PipelineManager::GetInstance();
	LoadNote("maou_bgm_cyber44");
	music = music_;
	for (size_t i = 0; i < notes.size(); i++)
	{
		std::unique_ptr<KObject3d> obj_;
		obj_.reset(KObject3d::Create(modelM->GetModels("S_Arrow"),
			pipelineM->GetPipeline("Obj")));
		obj_->GetTransform().SetScale({ 15.0f,15.0f,5.0f });

		//色設定
		if (notes[i]->lane == 0)
		{
			obj_->SetColor({ 0.5f,0.0f,0.0f,1.0f });
		}
		else
		{
			obj_->SetColor({ 0.0f,0.3f,1.0f,1.0f });
		}

		//方向設定
		if (notes[i]->direction == DIRECTION::left)//左
		{
			obj_->GetTransform().SetRot({ 0.0f,180.0f,0.0f });
		}
		else if (notes[i]->direction == DIRECTION::up)//上
		{
			obj_->GetTransform().SetRot({ 0.0f,0.0f,-90.0f });
		}
		else if (notes[i]->direction == DIRECTION::dawn)//下
		{
			obj_->GetTransform().SetRot({ 0.0f,180.0f,90.0f });
		}
		else											  //右
		{
			obj_->GetTransform().SetRot({ 0.0f,0.0f,0.0f });
		}
		notePosZ = (sec * speed) * music->ConvertBeatToMiliSeconds(notes[i]->beat);
		obj_->GetTransform().SetPos({ -50.0f + (100.0f * notes[i]->lane),25.0f,notePosZ });

		obj.push_back(std::move(obj_));
	}

}

void NoteObj::Update(Camera* camera_)
{
	for (size_t i = 0; i < obj.size(); i++)
	{
		if (!notes[i]->isHit)
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
		if (!notes[i]->isHit)
		{
			obj[i]->Draw();
		}
	}
}

void NoteObj::LoadNote(const std::string& name)
{
	int bpm;
	//ファイルを開く
	const std::string filename = "Resources/csv/note/" + name + ".sus";
	std::ifstream file;
	file.open(filename);
	assert(file.is_open());


	//1行分の文字列を入れる変数
	std::string line;

	for (size_t i = 0; i < 10; i++)
	{
		std::getline(file, line);
	}
	while (std::getline(file, line))
	{
		std::istringstream line_stream(line);
		//,区切りで行の先頭文字列を取得
		std::string key;
		// 空行をスキップ
		if (line.empty()) continue;
		//文字列にBPMがあるか
		if (line[0] == '#')
		{
			//BPMの代入
			if (line.find('BPM')!= std::string::npos)
			{
				// スペースの位置を見つける
				size_t spacePos = line.find(':');
				if (spacePos != std::string::npos) {
					// スペースの次の部分を切り出す
					std::string value = line.substr(spacePos + 1);

					bpm = std::stoi(value);
				}
			}
			//BPMの次の行をスキップ
			else if (line.find('8') != std::string::npos)
			{
				continue;
			}
			//ノーツの算出
			else
			{
				// スペースの次の部分を切り出す
				std::string value = line.substr(2, 2);//小節番号
				int32_t measure = std::stoi(value);
				value = line.substr(5,1);//レーン
				int32_t lane = std::stoi(value) - 2;//レーンの始まりを0にするため値を-2する(文字を使うようになったら改良必須)
				//拍数
				// スペースの位置を見つける
				size_t spacePos = line.find(':');
				if (spacePos != std::string::npos) {
					// スペースの次の部分を切り出す
					std::string value = line.substr(spacePos + 1);
					int32_t num = value.size() / 2;
					for (size_t i = 0; i < num; i++)
					{
						int32_t pos = 2 * i;
						std::string a = value.substr(pos, 2);
						if (a == "11") {
							std::unique_ptr<Note> note = std::make_unique<Note>();
							//値を代入
							note->beat.measure = measure;
							note->lane = lane;
							note->direction = DIRECTION::right;
							note->beat.beat = 0.5 * pos;
							notes.push_back(std::move(note));
						}
					}
					////方向設定
					//if (key == "left")//左
					//{
					//	note->direction = DIRECTION::left;
					//}
					//else if (key == "up")//上
					//{
					//	note->direction = DIRECTION::up;
					//}
					//else if (key == "dawn")//下
					//{
					//	note->direction = DIRECTION::dawn;
					//}
					//else //右
					//{
					//	note->direction = DIRECTION::right;
					//}
					//notes.push_back(std::move(note));
				}
			}
		
			continue;
		}
		/*getline(line_stream, key,',');
		note->beat.measure=std::stoi(key);
		getline(line_stream, key, ',');
		note->beat.beat = std::stof(key);
		getline(line_stream, key, ',');
		note->beat.LPB = std::stoi(key);
		getline(line_stream, key, ',');
		note->lane = std::stoi(key);
		getline(line_stream, key, ',');*/
		
	}
	//ファイルを閉じる
	file.close();
}
