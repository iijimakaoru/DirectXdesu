#pragma once
#include<map>
#include<list>
struct Beat
{
	int measure = 0;//小節番号
	float beat = 0;//拍数
	float LPB = 4;//1拍の長さ
};

struct Meter
{
	int beatAmount = 4;//1小節に入る拍の数
	int beatLength = 4;//1拍の長さ
};
enum DIRECTION
{
	right, //右
	left,  //左
	up,	   //上
	dawn   //下
};

enum Hand {
	R, //右
	L  //左
};

class Note
{
public:
	Note()=default;
	Beat beat;//何拍目の音
	int lane = 0;//レーン指定
	DIRECTION direction=right;//方向
	bool isHit = false;

	// 移動コンストラクタ
	Note(Note&& other) noexcept
		: beat(std::move(other.beat)), lane(other.lane), direction(other.direction) {
	}

	// 必要に応じてコピーコンストラクタも実装
	Note(const Note& other)
		: beat(other.beat), lane(other.lane), direction(other.direction) {
	}
};

class MusicDesc
{
public:
	MusicDesc();//コンストラクタ
	MusicDesc(float bpm,Meter meter);//コンストラクタ
	MusicDesc(float bpm);//コンストラクタ
	~MusicDesc();//デストラクタ

	float ConvertBeatToMiliSeconds(const Beat& beat);
private:
	float GetNoteLength(const Beat& beat);
	Meter GetMeter(const Beat& beat);

private:
	std::map<Beat,float>bpmChange_;
	std::map<Beat,Meter>meterChange_;
	std::list<Beat>changeList;
	float bpm_;
	float baseBPM = 120.0f;
	Meter meter_;
	Meter baseMeter;



};

