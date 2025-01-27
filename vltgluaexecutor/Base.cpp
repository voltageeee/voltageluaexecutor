#include "Base.h"
#define LUA_MULTRET -1

LuaExecutor* GLuaExecutor = new LuaExecutor();

void err(std::string err)
{
	MessageBoxA(NULL, err.c_str(), "Îòñîñåøü", 0);
}

// ëþòî õóåâàÿ óæàñíàÿ òóïàÿ ïàñòà
void LuaExecutor::InitLuaExecutor()
{
	HMODULE luaShared = GetModuleHandleA("lua_shared.dll");

	if (!luaShared)
	{
		err("íå óäàëîñü èíèöèàëèçðîâàòü luashared");
		return;
	}

	CreateInterface CreateInterface_src = (CreateInterface)GetProcAddress(luaShared, "CreateInterface");
	ILuaShared = (CLuaShared*)CreateInterface_src("LUASHARED003", NULL);
	if (!ILuaShared)
	{
		err("íå óäàëîñü èíèöèàëèçèðîâàòü ilua èíòåðôåéñ");
		return;
	}

	luaL_loadString = (_luaL_loadString)GetProcAddress(luaShared, "luaL_loadstring");
	if (!luaL_loadString)
	{
		err("íå óäàëîñü ïîäãðóçèòü lua_loadstring");
		return;
	}

	luaL_pCall = (_luaL_pCall)GetProcAddress(luaShared, "lua_pcall");
	if (!luaL_pCall)
	{
		err("íå óäàëîñü ïîäãðóçèòü lua_pcall");
		return;
	}

	LuaLoaded = true;
}

void LuaExecutor::ExecuteString(char* code)
{
	if (!LuaLoaded)
	{
		err("èíòåðôåéñ ëóà íå ïîäãðóæåí)))))))");
		return;
	}

	CLuaInterface = ILuaShared->GetLuaInterface(LUAINTERFACE_CLIENT);
	if (!CLuaInterface)
	{
		err("íå ïîëó÷èëîñü ïîäãðóçèòü cluainterface");
		return;
	}

	pGLuaState = *(DWORD*)(CLuaInterface + 0x4);
	if (!pGLuaState)
	{
		err("íå ïîëó÷èëîñü ïîäãðóçèòü luastate");
		return;
	}

	luaL_loadString((void*)pGLuaState, code);
	luaL_pCall((void*)pGLuaState, 0, LUA_MULTRET, 0);
}
