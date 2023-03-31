#include "extension.h"
#include "cereal_archiver.h"


template <typename T>
void SaveArchiveFile(T& archive, const char* pszFileName)
{
	std::string strFullpath{};
	std::string strFileName(pszFileName); 

	const char* pszModPath = g_pSM->GetGamePath();

	// strcat not work
	strFullpath += pszModPath;
	strFullpath += "\\";
	strFullpath += strFileName;

	bool bSerialized = archive.Serialize(strFullpath.c_str());

	//if (!bSerialized)
		//rootconsole->ConsolePrint("Serialization failed!");
}

/*
		   _  _____  ____  _   _
		  | |/ ____|/ __ \| \ | |
		  | | (___ | |  | |  \| |
	  _   | |\___ \| |  | | . ` |
	 | |__| |____) | |__| | |\  |
	  \____/|_____/ \____/|_| \_|                     
 */                          

cell_t JsonArchive_Reset(IPluginContext* pContext, const cell_t* params)
{
	g_JsonArchiver.Reset();

	return 0;
}

cell_t JsonArchive_SetString(IPluginContext* pContext, const cell_t* params)
{
	if (params[0] < 2)
		return 0;

	char* pszString;
	pContext->LocalToString(params[2], &pszString);

	g_JsonArchiver.SetData(params[1], pszString);

	return 0;
}

cell_t JsonArchive_SetNumber(IPluginContext* pContext, const cell_t* params)
{
	if (params[0] < 2)
		return 0;

	g_JsonArchiver.SetData(params[1], params[2]);

	return 0;
}

cell_t JsonArchive_SetFloat(IPluginContext* pContext, const cell_t* params)
{
	if (params[0] < 2)
		return 0;

	g_JsonArchiver.SetData(params[1], sp_ctof(params[2]));

	return 0;
}

cell_t JsonArchive_GetString(IPluginContext* pContext, const cell_t* params)
{
	/*
		1. index
		2. char array
		3. size of char array
	*/
	if (params[0] < 3)
		return 0;

	SM_CerealVariant var = g_JsonArchiver.GetData(params[1]);

	std::string str;
	try
	{
		str = std::get<std::string>(var);
	}
	catch (const std::bad_variant_access& ex)
	{
		return 0;
	}

	pContext->StringToLocal(params[2], params[3], str.c_str());

	return 0;
}

cell_t JsonArchive_GetNumber(IPluginContext* pContext, const cell_t* params)
{
	if (params[0] < 1)
		return 0;

	SM_CerealVariant var = g_JsonArchiver.GetData(params[1]);

	int num;

	try
	{
		num = std::get<int>(var);
	}
	catch (const std::bad_variant_access& ex)
	{
		return 0;
	}
	
	return num;
}

cell_t JsonArchive_GetFloat(IPluginContext* pContext, const cell_t* params)
{
	if (params[0] < 1)
		return 0;

	SM_CerealVariant var = g_JsonArchiver.GetData(params[1]);

	float fl;
	try
	{
		fl = std::get<float>(var);
	}
	catch (const std::bad_variant_access& ex)
	{
		return 0;
	}

	return sp_ftoc(fl);
}

cell_t JsonArchive_Serialize(IPluginContext* pContext, const cell_t* params)
{
	if (params[0] < 1)
		return 0;

	char* pszFileName;
	
	pContext->LocalToString(params[1], &pszFileName);

	SaveArchiveFile(g_JsonArchiver, pszFileName);
	g_JsonArchiver.Reset();
	
	return 0;
}

cell_t JsonArchive_Deserialize(IPluginContext* pContext, const cell_t* params)
{
	if (params[0] < 1)
		return 0;

	char* pszFileName;
	pContext->LocalToString(params[1], &pszFileName);

	g_JsonArchiver.Deserialize(pszFileName);

	return 0;
}

/*
	 __   ____  __ _
	 \ \ / /  \/  | |
	  \ V /| \  / | |
	   > < | |\/| | |
	  / . \| |  | | |____
	 /_/ \_\_|  |_|______|
*/

cell_t XmlArchive_Reset(IPluginContext* pContext, const cell_t* params)
{
	g_XmlArchiver.Reset();

	return 0;
}

cell_t XmlArchive_SetString(IPluginContext* pContext, const cell_t* params)
{
	if (params[0] < 2)
		return 0;

	char* pszString;
	pContext->LocalToString(params[2], &pszString);

	g_XmlArchiver.SetData(params[1], pszString);

	return 0;
}

cell_t XmlArchive_SetNumber(IPluginContext* pContext, const cell_t* params)
{
	if (params[0] < 2)
		return 0;

	g_XmlArchiver.SetData(params[1], params[2]);

	return 0;
}

cell_t XmlArchive_SetFloat(IPluginContext* pContext, const cell_t* params)
{
	if (params[0] < 2)
		return 0;

	g_XmlArchiver.SetData(params[1], sp_ctof(params[2]));

	return 0;
}

cell_t XmlArchive_GetString(IPluginContext* pContext, const cell_t* params)
{
	/*
		1. index
		2. char array
		3. size of char array
	*/
	if (params[0] < 3)
		return 0;

	SM_CerealVariant var = g_XmlArchiver.GetData(params[1]);

	std::string str;
	try
	{
		str = std::get<std::string>(var);
	}
	catch (const std::bad_variant_access& ex)
	{
		return 0;
	}

	pContext->StringToLocal(params[2], params[3], str.c_str());

	return 0;
}

cell_t XmlArchive_GetNumber(IPluginContext* pContext, const cell_t* params)
{
	if (params[0] < 1)
		return 0;

	SM_CerealVariant var = g_XmlArchiver.GetData(params[1]);

	int num;
	try
	{
		num = std::get<int>(var);
	}
	catch (const std::bad_variant_access& ex)
	{
		return 0;
	}

	return num;
}

cell_t XmlArchive_GetFloat(IPluginContext* pContext, const cell_t* params)
{
	if (params[0] < 1)
		return 0;

	SM_CerealVariant var = g_XmlArchiver.GetData(params[1]);

	float fl;
	try
	{
		fl = std::get<float>(var);
	}
	catch (const std::bad_variant_access& ex)
	{
		return 0;
	}

	return sp_ftoc(fl);
}

cell_t XmlArchive_Serialize(IPluginContext* pContext, const cell_t* params)
{
	if (params[0] < 1)
		return 0;

	char* pszFileName;

	pContext->LocalToString(params[1], &pszFileName);

	SaveArchiveFile(g_XmlArchiver, pszFileName);
	g_XmlArchiver.Reset();

	return 0;
}

cell_t XmlArchive_Deserialize(IPluginContext* pContext, const cell_t* params)
{
	if (params[0] < 1)
		return 0;

	char* pszFileName;
	pContext->LocalToString(params[1], &pszFileName);

	g_XmlArchiver.Deserialize(pszFileName);

	return 0;
}

/*
	 ____ _____ _   _          _______     __
	|  _ \_   _| \ | |   /\   |  __ \ \   / /
	| |_) || | |  \| |  /  \  | |__) \ \_/ /
	|  _ < | | | . ` | / /\ \ |  _  / \   /
	| |_) || |_| |\  |/ ____ \| | \ \  | |
	|____/_____|_| \_/_/    \_\_|  \_\ |_|
*/

cell_t BinaryArchive_Reset(IPluginContext* pContext, const cell_t* params)
{
	g_BinaryArchiver.Reset();

	return 0;
}

cell_t BinaryArchive_SetNumber(IPluginContext* pContext, const cell_t* params)
{
	if (params[0] < 2)
		return 0;

	g_BinaryArchiver.SetData(params[1], params[2]);

	return 0;
}

cell_t BinaryArchive_GetNumber(IPluginContext* pContext, const cell_t* params)
{
	if (params[0] < 1)
		return 0;

	return g_BinaryArchiver.GetData(params[1]);
}

cell_t BinaryArchive_Serialize(IPluginContext* pContext, const cell_t* params)
{
	if (params[0] < 1)
		return 0;

	char* pszFileName;

	pContext->LocalToString(params[1], &pszFileName);

	SaveArchiveFile(g_BinaryArchiver, pszFileName);
	g_BinaryArchiver.Reset();

	return 0;
}

cell_t BinaryArchive_Deserialize(IPluginContext* pContext, const cell_t* params)
{
	if (params[0] < 1)
		return 0;

	char* pszFileName;
	pContext->LocalToString(params[1], &pszFileName);

	g_BinaryArchiver.Deserialize(pszFileName);

	return 0;
}


const sp_nativeinfo_t CerealNatives[] =
{
	{"JsonArchive_Reset",		JsonArchive_Reset},

	{"JsonArchive_SetFloat",		JsonArchive_SetFloat},
	{"JsonArchive_SetNumber",		JsonArchive_SetNumber},
	{"JsonArchive_SetString",		JsonArchive_SetString},

	{"JsonArchive_GetFloat",		JsonArchive_GetFloat},
	{"JsonArchive_GetNumber",		JsonArchive_GetNumber},
	{"JsonArchive_GetString",		JsonArchive_GetString},

	{"JsonArchive_Serialize",		JsonArchive_Serialize},
	{"JsonArchive_Deserialize ",	JsonArchive_Deserialize },


	{"XmlArchive_Reset",			XmlArchive_Reset},

	{"XmlArchive_SetFloat",			XmlArchive_SetFloat},
	{"XmlArchive_SetNumber",		XmlArchive_SetNumber},
	{"XmlArchive_SetString",		XmlArchive_SetString},

	{"XmlArchive_GetFloat",			XmlArchive_GetFloat},
	{"XmlArchive_GetNumber",		XmlArchive_GetNumber},
	{"XmlArchive_GetString",		XmlArchive_GetString},

	{"XmlArchive_Serialize",		XmlArchive_Serialize},
	{"XmlArchive_Deserialize ",		XmlArchive_Deserialize },


	{"BinaryArchive_Reset",			BinaryArchive_Reset},

	{"BinaryArchive_SetNumber",		BinaryArchive_SetNumber},

	{"BinaryArchive_GetNumber",		BinaryArchive_GetNumber},

	{"BinaryArchive_Serialize",		BinaryArchive_Serialize},
	{"BinaryArchive_Deserialize ",	BinaryArchive_Deserialize},

	{NULL,			NULL},
};

void Cereal_AddNatives()
{
	sharesys->AddNatives(myself, CerealNatives);
}
