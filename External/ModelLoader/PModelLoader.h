#ifndef	__RENDER_P_MODELLOADER_H__
#define	__RENDER_P_MODELLOADER_H__

#include<P_Defined.h>
#include<P_MODEL_DATA.h>

PHONON_SUPPRESS_WARNINGS_BEGIN

#include<string>

PHONON_SUPPRESS_WARNINGS_END

PHONON_LOADER_NAMESPACE_BEGIN

class PModelLoader
{
public:

	enum class Mode : size_t
	{
		ASSIMPONLY,
		NOASSIMP,
		FORMATMATCH
	};

public:

	static void ModeChange(Mode mode);

	static void Load(P_MODEL_DATA* pData,const std::string& filePath);

private:

	PModelLoader();
	~PModelLoader();

	static PModelLoader* GetInstance();

	void _ModeChange(Mode mode);

	void _Load(P_MODEL_DATA* pData,const std::string& filePath);

private:

	std::string baseDirectory = "Resources/";

	Mode m_mode = Mode::ASSIMPONLY;

};

PHONON_LOADER_NAMESPACE_END

#endif	//__RENDER_P_MODELLOADER_H__