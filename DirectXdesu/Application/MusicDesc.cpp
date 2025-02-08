#include "MusicDesc.h"

bool Comparison(const Beat& now,const Beat& change) {
	if ( now.measure < change.measure )
	{
		return true;
	}
	if ( now.beat<change.beat )
	{
		return true;
	}
	return false;
}

MusicDesc::MusicDesc()
{
	bpm_ = 120;
	meter_.beatAmount = 4;
	meter_.beatLength = 4;
}

MusicDesc::MusicDesc(float bpm,Meter meter)
{
	bpm_ = bpm;
	meter_ = meter;
}

MusicDesc::MusicDesc(float bpm)
{
	bpm_ = bpm;
}

MusicDesc::~MusicDesc()
{
}

float MusicDesc::ConvertBeatToMiliSeconds(const Beat& beat)
{
	float beatTime = 0;
	
	//全音符の長さを得る（ミリ秒）
	float whole = ( ( 60 / bpm_ ) * 1000.0f ) * 4.0f;
	//1小節の長さを得る
	float measure = ( whole / meter_.beatLength ) * meter_.beatAmount;
	//小節分の時間を加算する
	beatTime += measure * beat.measure;
	//拍数分の時間を加算する
	beatTime += whole * ( static_cast< float >( beat.beat ) / beat.LPB );

	return beatTime;
}

float MusicDesc::GetNoteLength(const Beat& beat)
{
	beat.beat;
	float whole = ( ( 60 / bpm_ ) * 1000.0f ) * 4.0f;
	return whole;
}

Meter MusicDesc::GetMeter(const Beat& beat)
{
	beat;
	return Meter();
}
