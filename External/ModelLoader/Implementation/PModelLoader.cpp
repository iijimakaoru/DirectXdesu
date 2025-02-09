#include <PModelLoader.h>

#include<PAssimpLoader.h>

PHONON_LOADER_NAMESPACE_BEGIN

void PModelLoader::ModeChange(Mode mode)
{
	GetInstance()->_ModeChange(mode);
}

void PModelLoader::Load(P_MODEL_DATA* pData, const std::string& filePath)
{
	GetInstance()->_Load(pData,filePath);
}

void PModelLoader::SmoothingLoad(P_MODEL_DATA* pData,const std::string& filePath)
{
	GetInstance()->_SmoothingLoad(pData,filePath);
}

PModelLoader::PModelLoader()
{
}

PModelLoader::~PModelLoader()
{
}

PModelLoader* PModelLoader::GetInstance()
{
	static PModelLoader instance;
	return &instance;
}

void PModelLoader::_ModeChange(Mode mode)
{
	m_mode = mode;
}

void PModelLoader::_Load(P_MODEL_DATA* pData, const std::string& filePath)
{
	switch (m_mode)
	{
	case PModelLoader::Mode::ASSIMPONLY:
		PAssimpLoader::Load(pData,std::string(baseDirectory + filePath));
		break;
	case PModelLoader::Mode::NOASSIMP:
		break;
	case PModelLoader::Mode::FORMATMATCH:
		break;
	default:
		break;
	}
}

void PModelLoader::_SmoothingLoad(P_MODEL_DATA* pData,const std::string& filePath)
{
	switch ( m_mode )
	{
	case PModelLoader::Mode::ASSIMPONLY:
		PAssimpLoader::SmoothingLoad(pData,std::string(baseDirectory + filePath));
		break;
	case PModelLoader::Mode::NOASSIMP:
		break;
	case PModelLoader::Mode::FORMATMATCH:
		break;
	default:
		break;
	}
}

PHONON_LOADER_NAMESPACE_END