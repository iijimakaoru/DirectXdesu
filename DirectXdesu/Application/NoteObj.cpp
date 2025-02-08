#include "NoteObj.h"
#include<iostream>
#include<fstream>
#include <format>
NoteObj::NoteObj()
{
}

NoteObj::~NoteObj()
{
}

void NoteObj::Init(bool test_,MusicDesc* music_)
{
	test = test_;
	modelM = ModelManager::GetInstance();
	pipelineM = PipelineManager::GetInstance();
	music = music_;
	if (test)
	{
		LoadNoteSUS("cyber44");
		for (auto it = notesMap.begin(); it != notesMap.end(); ++it)
		{
			std::unique_ptr<KObject3d> obj_;
			std::unique_ptr<KObject3d> underObj_;
			obj_.reset(KObject3d::Create(modelM->GetModels("S_Cube"), pipelineM->GetPipeline("Obj")));
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
			notePosZ = (sec * speed) * music->ConvertBeatToMiliSeconds(it->second.beat);

			obj_->GetTransform().SetPos({ -50.0f + (100.0f * it->second.lane),25.0f,notePosZ });
			underObj_->GetTransform().SetPos({ -50.0f + (100.0f * it->second.lane),5.0f,notePosZ });

			objMap.emplace(it->first, std::move(obj_));
			underObj.push_back(std::move(underObj_));
		}
	}
	else
	{
		LoadNote("cyber44");
		for (size_t i = 0; i < notes.size(); i++)
		{
			std::unique_ptr<KObject3d> obj_;
			std::unique_ptr<KObject3d> underObj_;
			obj_.reset(KObject3d::Create(modelM->GetModels("S_Arrow"),pipelineM->GetPipeline("Obj")));
			obj_->GetTransform().SetScale({ 15.0f,15.0f,5.0f });
			underObj_.reset(KObject3d::Create(modelM->GetModels("S_Cube"), pipelineM->GetPipeline("Obj")));
			underObj_->GetTransform().SetScale({ 15.0f,2.0f,2.0f });

			//色設定
			if (notes[i].lane == 0)
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
			if (notes[i].direction == DIRECTION::left)//左
			{
				obj_->GetTransform().SetRot({ 0.0f,180.0f,0.0f });
			}
			else if (notes[i].direction == DIRECTION::up)//上
			{
				obj_->GetTransform().SetRot({ 0.0f,0.0f,-90.0f });
			}
			else if (notes[i].direction == DIRECTION::dawn)//下
			{
				obj_->GetTransform().SetRot({ 0.0f,180.0f,90.0f });
			}
			else											  //右
			{
				obj_->GetTransform().SetRot({ 0.0f,0.0f,0.0f });
			}
			notePosZ = (sec * speed) * music->ConvertBeatToMiliSeconds(notes[i].beat);
			obj_->GetTransform().SetPos({ -50.0f + (100.0f * notes[i].lane),25.0f,notePosZ });
			underObj_->GetTransform().SetPos({ -50.0f + (100.0f * i),5.0f,notePosZ });

			obj.push_back(std::move(obj_));
			underObj.push_back(std::move(underObj_));
		}
	}
	music = music_;
}

void NoteObj::Update(Camera* camera_)
{
	if (test)
	{
		for (auto& [key, value] : objMap)
		{
			if (!notesMap[key].isHit)
			{
				KMyMath::Vector3 move;
				move = objMap[key]->GetTransform().GetPos();
				move.z -= speed;

				objMap[key]->GetTransform().SetPos(move);
				objMap[key]->Update(camera_->GetViewPro(), camera_->GetWorldPos());
			}
		}
	}
	else
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
				move.y = underObj[i]->GetTransform().GetPos().y;
				underObj[i]->GetTransform().SetPos(move);
				underObj[i]->Update(camera_->GetViewPro(), camera_->GetWorldPos());
			}
		}
	}
}

void NoteObj::Draw()
{
	if (test)
	{
		for (auto& [key, value] : objMap)
		{
			if (!notesMap[key].isHit)
			{
				objMap[key]->Draw();
			}
		}
	}
	else
	{
		for (size_t i = 0; i < obj.size(); i++)
		{
			if (!notes[i].isHit)
			{
				obj[i]->Draw();
				underObj[i]->Draw();
			}
		}
	}
}

void NoteObj::LoadNote(const std::string& name)
{
	int bpm;
	//ファイルを開く
	const std::string filename = "Resources/csv/note/" + name + ".txt";
	fileName = name;
	std::ifstream file;
	file.open(filename);
	assert(file.is_open());


	//1行分の文字列を入れる変数
	std::string line;
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
			if (line.find("BPM") != std::string::npos)
			{
				// スペースの位置を見つける
				size_t spacePos = line.find(':');
				if (spacePos != std::string::npos) {
					// スペースの次の部分を切り出す
					std::string value = line.substr(spacePos + 1);

					bpm = std::stoi(value);
				}
			}
			//ノーツの算出
			else
			{
				// スペースの次の部分を切り出す
				std::string value = line.substr(2, 2);//小節番号
				int32_t measure = std::stoi(value);
				value = line.substr(5, 1);//レーン
				int32_t lane = std::stoi(value) - 2;//レーンの始まりを0にするため値を-2する(文字を使うようになったら改良必須)
				//拍数
				// スペースの位置を見つける
				size_t spacePos = line.find(':');
				if (spacePos != std::string::npos) {
					// スペースの次の部分を切り出す
					std::string value = line.substr(spacePos + 1, 16);
					int32_t num = static_cast<int32_t>(value.size() / 2);
					float pos = 0.5f;
					for (int32_t i = 0; i < num; i++)
					{
						int32_t posBeat = 2 * i;
						std::string beat = value.substr(posBeat, 2);
						spacePos = line.find('/');
						std::string course = line.substr(spacePos + 1);
						if (beat == "11") {
							Note note;
							//値を代入
							note.beat.measure = measure;
							note.lane = lane;
							note.beat.beat = pos * i;
							//方向設定
							if (course[i] == 'L')//左
							{
								note.direction = DIRECTION::left;
							}
							else if (course[i] == 'U')//上
							{
								note.direction = DIRECTION::up;
							}
							else if (course[i] == 'D')//下
							{
								note.direction = DIRECTION::dawn;
							}
							else //右
							{
								note.direction = DIRECTION::right;
							}
							notes.push_back(std::move(note));
						}						
					}
				}
			}
		}
	}
	//ファイルを閉じる
	file.close();
}

void NoteObj::LoadNoteSUS(const std::string& name)
{
	
	//ファイルを開く
	const std::string filename = "Resources/csv/note/" + name + ".sus";
	fileName = name;
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
			if (line.find("BPM")!= std::string::npos)
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
				value = line.substr(5, 1);//レーン
				int32_t lane = std::stoi(value) - 2;//レーンの始まりを0にするため値を-2する(文字を使うようになったら改良必須)
				//拍数
				// スペースの位置を見つける
				size_t spacePos = line.find(':');
				if (spacePos != std::string::npos) {
					// スペースの次の部分を切り出す
					std::string value = line.substr(spacePos + 1);
					int32_t num = static_cast<int32_t>( value.size() / 2);
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
					for (int32_t i = 0; i < num; i++)
					{
						int32_t posBeat = 2 * i;
						std::string beat = value.substr(posBeat, 2);
						if (beat == "11") {
							Note note;
							//値を代入
							note.beat.measure = measure;
							note.lane = lane;
							note.beat.beat = pos * i;
							note.direction = DIRECTION::right;//一旦代入
							notesMap.emplace(SetKey(note.beat, lane), std::move(note));
						}
					}
				}
			}
		}
	}
	//ファイルを閉じる
	file.close();
}

void NoteObj::OutputNote()
{
	std::string inputTxtL,inputTxtR;
	std::string newLine="\n";
	std::string name ="Resources/csv/note/"+ fileName + ".txt";
	std::string sharp = "#";
	bool measureL = false;
	bool measureR = false;

	std::ofstream outputfile(name);
	//最初にBPMを入れる
	outputfile << "#BPM:" + std::to_string(bpm)+newLine;

	int32_t step = 1000; // 1000刻みで分類
	for (int32_t range_start = 0; range_start <= notesMap.rbegin()->first+step; range_start += step) {
		int32_t range_end = range_start + step - 1; // 例: 0~99, 100~199

		auto it_low = notesMap.lower_bound(range_start);
		auto it_up = notesMap.upper_bound(range_end);
		//左右用マップ
		std::map<int32_t, Note> laneLeft;
		std::map<int32_t, Note> laneRight;
		//クリアしとく
		laneLeft.clear();
		laneRight.clear();
		inputTxtL.clear();
		inputTxtR.clear();
		//最初の一回だけ小節を入れるためのフラグ
		measureL = false;
		measureR = false;
		//レーンごとのマップへ代入
		for (auto it = it_low; it != it_up; ++it) {
			//左
			if (it->second.lane == 0) {
				laneLeft.emplace(it->first, it->second);
				if (!measureL)
				{
					//#から:の値を代入　+2してるのはsus形式にするため
					inputTxtL = sharp + std::format("{:03}", it->second.beat.measure) + "1" + std::to_string(it->second.lane + 2) + ":";
					measureL = true;
				}
			}
			//右
			else if (it->second.lane == 1)
			{
				laneRight.emplace(it->first, it->second);
				if (!measureR)
				{
					inputTxtR = sharp + std::format("{:03}", it->second.beat.measure) + "1" + std::to_string(it->second.lane + 2) + ":";
					measureR = true;
					

				}
			}
		}
		//beatを入れる処理（ごり押しですまぬ）
		size_t bitL = 0;
		size_t bitR = 0;
		std::string inputBitL, inputBitR, courseL, courseR;
		for (auto it = laneLeft.begin(); it != laneLeft.end(); ++it) {
			
			//16ビットで回す
			for (bitL; bitL < 8; bitL++)
			{
				float beat = static_cast<float>(bitL) * 0.5f;
				if (beat==it->second.beat.beat)
				{
					inputBitL += "11";
					bitL++;
					DIRECTION d = it->second.direction;
					switch (d)
					{
					case DIRECTION::right:

						courseL += "R";
						break;
					case DIRECTION::left:
						courseL += "L";
						break;
					case DIRECTION::up:
						courseL += "U";
						break;
					case DIRECTION::dawn:
						courseL += "D";
						break;
					default:
						break;
					}
					break;
				}
				else
				{
					inputBitL += "00";
					courseL += "!";
				}
			}
		}
		//ビットが16に行かなかった場合
		if (bitL<8)
		{
			for (bitL; bitL < 8; bitL++)
			{
				
				inputBitL += "00";
				courseL += "!";
			}
		}
		for (auto it = laneRight.begin(); it != laneRight.end(); ++it) {
			//16ビットで回す
			for (bitR; bitR < 8; bitR++)
			{
				float beat = static_cast<float>(bitR) * 0.5f;
				if (beat == it->second.beat.beat)
				{
					inputBitR += "11";
					bitR++;
					DIRECTION d = it->second.direction;
					switch (d)
					{
					case DIRECTION::right:

						courseR += "R";
						break;
					case DIRECTION::left:
						courseR += "L";
						break;
					case DIRECTION::up:
						courseR += "U";
						break;
					case DIRECTION::dawn:
						courseR += "D";
						break;
					default:
						break;
					}
					break;
				}
				else
				{
					inputBitR += "00";
					courseR += "!";
				}
			}
		}
		//ビットが16に行かなかった場合
		if (bitR < 8)
		{
			for (bitR; bitR < 8; bitR++)
			{

				inputBitR += "00";
				courseR += "!";
			}
		}
		if (inputTxtL[0]=='#')
		{
			inputTxtL += inputBitL + "/" + courseL + newLine;
		}
		if (inputTxtR[0] == '#')
		{
			inputTxtR += inputBitR + "/" + courseR + newLine;
		}
		outputfile << inputTxtL;
		outputfile << inputTxtR;
	}

	//outputfile << ;
	outputfile.close();
}
