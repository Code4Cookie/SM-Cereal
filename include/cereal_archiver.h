#pragma once

#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/variant.hpp>

#include <vector>
#include <variant>
#include <fstream>

#include <filesystem>


#define SM_CEREALCAP 25

using SM_CerealVariant = std::variant<int, float, std::string>;

enum class CerealParamType
{
	CEREAL_PARAM_INT = 0,
	CEREAL_PARAM_STRING
};

enum class CerealArchiveType
{
	CEREAL_JSON = 0,
	CEREAL_XML,
	CEREAL_BINARY
};

template <typename I, typename O, bool B = false>
class CBaseArchiver
{
public:
    virtual bool Serialize(const char* pszFileName);
	virtual void Deserialize(const char* pszFileName);

	CBaseArchiver()
	{
		m_Data.reserve(SM_CEREALCAP);
		m_Data.assign(SM_CEREALCAP, 0);
	}

	void SetData(int index, std::conditional_t<B, int, SM_CerealVariant> data)
	{
		if (index >= m_Data.size() || index < 0)
			return;

		m_Data[index] = data;
	}

	std::conditional_t<B, int, SM_CerealVariant> GetData(int index)
	{
		if (index >= m_Data.size() || index < 0)
			return 0;

		return m_Data[index];
	}

	void ClearData()
	{
		m_Data.clear();
	}

	void Reset()
	{
		m_Data.assign(SM_CEREALCAP, 0);
	}
protected:
	// Can either be a vector of variants or ints
	std::conditional_t<B,
		std::vector<int>,
		std::vector<SM_CerealVariant>
	> m_Data;


	static_assert(std::derived_from<I, cereal::detail::InputArchiveBase>);
	static_assert(std::derived_from<O, cereal::detail::OutputArchiveBase>);
};

class CJSONArchiver : public CBaseArchiver<cereal::JSONInputArchive, cereal::JSONOutputArchive>
{
};

class CXMLArchiver : public CBaseArchiver<cereal::XMLInputArchive, cereal::XMLOutputArchive>
{
};

class CBinaryArchiver : public CBaseArchiver<cereal::PortableBinaryInputArchive, cereal::PortableBinaryOutputArchive, true>
{
};


template<typename I, typename O, bool B>
bool CBaseArchiver<I, O, B>::Serialize(const char* pszFileName)
{
	std::fstream file(pszFileName, B ? std::ios::out | std::ios::binary : std::ios::out);

	if (file.fail())
		return false;

	O* archive = new O(file);

	archive->operator()(m_Data);
	
	delete archive;

	return true;
}

template<typename I, typename O, bool B>
void CBaseArchiver<I, O, B>::Deserialize(const char* pszFileName)
{
	std::fstream file(pszFileName, B ? std::ios::in | std::ios::binary : std::ios::in);

	I* archive = new I(file);

	archive->operator()(m_Data);

	delete archive;
}


extern CJSONArchiver g_JsonArchiver;
extern CXMLArchiver g_XmlArchiver;
extern CBinaryArchiver g_BinaryArchiver;