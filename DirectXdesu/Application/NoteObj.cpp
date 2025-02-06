#include "NoteObj.h"
#include<iostream>
#include<fstream>

NoteObj::NoteObj()
{
}

NoteObj::~NoteObj()
{
}

void NoteObj::Init(MusicDesc* music_)
{
	modelM = ModelManager::GetInstance();
	pipelineM = PipelineManager::GetInstance();
	LoadNote("cyber44");
	music = music_;
	
	for (auto it = notes.begin();it!=notes.end();++it)
	{
		std::unique_ptr<KObject3d> obj_;
		std::unique_ptr<KObject3d> underObj_;
		obj_.reset(KObject3d::Create(modelM->GetModels("S_Cube"),pipelineM->GetPipeline("Obj")));
		obj_->GetTransform().SetScale({ 15.0f,15.0f,5.0f });
		underObj_.reset(KObject3d::Create(modelM->GetModels("S_Cube"), pipelineM->GetPipeline("Obj")));
		underObj_->GetTransform().SetScale({ 15.0f,2.0f,2.0f });
		//色設定
		if (it->second.lane == 0)
		{
			obj_->SetColor({ 0.5f,0.0f,0.0f,1.0f });
			underObj_->SetColor({ 0.5f,0.0f,0.0f,1.0f });
		}
		else
		{
			obj_->SetColor({ 0.0f,0.3f,1.0f,1.0f });
			underObj_->SetColor({ 0.0f,0.3f,1.0f,1.0f });
		}

		//方向設定
		//if (notes[key]->direction == DIRECTION::left)//左
		//{
		//	obj_->GetTransform().SetRot({ 0.0f,180.0f,0.0f });
		//}
		//else if (notes[key]->direction == DIRECTION::up)//上
		//{
		//	obj_->GetTransform().SetRot({ 0.0f,0.0f,-90.0f });
		//}
		//else if (notes[key]->direction == DIRECTION::dawn)//下
		//{
		//	obj_->GetTransform().SetRot({ 0.0f,180.0f,90.0f });
		//}
		//else											  //右
		//{
		//	obj_->GetTransform().SetRot({ 0.0f,0.0f,0.0f });
		//}
		notePosZ = (sec * speed) * music->ConvertBeatToMiliSeconds(it->second.beat);
		
		obj_->GetTransform().SetPos({ -50.0f + (100.0f * it->second.lane),25.0f,notePosZ });
		underObj_->GetTransform().SetPos({ -50.0f + (100.0f * it->second.lane),5.0f,notePosZ });

		obj.emplace(it->first,std::move(obj_));
		underObj.push_back(std::move(underObj_));
	}

}

void NoteObj::Update(Camera* camera_)
{
	for (auto& [key, value] : obj)
	{
		if (!notes[key].isHit)
		{
			KMyMath::Vector3 move;
			move = obj[key]->GetTransform().GetPos();
			move.z -= speed;

			obj[key]->GetTransform().SetPos(move);
			obj[key]->Update(camera_->GetViewPro(), camera_->GetWorldPos());
		}
	}
	//一応後で同じにする今は表示のみ
	for (size_t i = 0; i < underObj.size(); i++)
	{
		KMyMath::Vector3 move;
		move = underObj[i]->GetTransform().GetPos();
		move.z -= speed;

		underObj[i]->GetTransform().SetPos(move);
		underObj[i]->Update(camera_->GetViewPro(), camera_->GetWorldPos());
	}
}

void NoteObj::Draw()
{
	for (auto& [key, value] : obj)
	{
		if (!notes[key].isHit)
		{
			obj[key]->Draw();
		}
	}
	for (size_t i = 0; i < underObj.size(); i++)
	{
		
		underObj[i]->Draw();
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
		//タイトルの代入
		if (line.find("TITLE") != std::string::npos)
		{
			// スペースの位置を見つける
			size_t spacePos = line.find(' ');
			if (spacePos != std::string::npos) {
				// スペースの次の部分を切り出す
				fileName = line.substr(spacePos + 1);
			}
		}
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
					float pos;
					switch (num)
					{
					case 1:
						pos = 0.0f;
						break;
					case 2:
						pos = 2.0f;
						break;
					case 4:
						pos = 1.0f;
						break;
					case 8:
						pos = 0.5f;
						break;
					default:
						pos = 0.5f;
						break;
					}
					for (size_t i = 0; i < num; i++)
					{
						int32_t posBeat = 2 * i;
						std::string a = value.substr(posBeat, 2);
						if (a == "11") {
							Note note;
							//値を代入
							note.beat.measure = measure;
							note.lane = lane;
							note.beat.beat = pos * i;
							note.direction = DIRECTION::left;
							notes.emplace(SetKey(note.beat, lane), std::move(note));
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
			}
		}
	}
	//ファイルを閉じる
	file.close();
}

void NoteObj::OutputNote(const std::string& name)
{
	std::string inputTxt;
	std::ofstream outputfile(fileName);

	inputTxt;

	outputfile << inputTxt;
	outputfile.close();
}
