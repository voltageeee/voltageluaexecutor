#include "Base.h"
#define LUA_MULTRET -1

LuaExecutor* GLuaExecutor = new LuaExecutor();

void err(std::string err)
{
	MessageBoxA(NULL, err.c_str(), "отсосешь)", 0);
}

// ужасно тупая гнилая хуевая паста
void LuaExecutor::InitLuaExecutor()
{
	HMODULE luaShared = GetModuleHandleA("lua_shared.dll");

	if (!luaShared)
	{
		err("пизда luashared");
		return;
	}

	CreateInterface CreateInterface_src = (CreateInterface)GetProcAddress(luaShared, "CreateInterface");
	ILuaShared = (CLuaShared*)CreateInterface_src("LUASHARED003", NULL);
	if (!ILuaShared)
	{
		err("пизда луа интерфейсу");
		return;
	}

	luaL_loadString = (_luaL_loadString)GetProcAddress(luaShared, "luaL_loadstring");
	if (!luaL_loadString)
	{
		err("пизда lua_loadstring");
		return;
	}

	luaL_pCall = (_luaL_pCall)GetProcAddress(luaShared, "lua_pcall");
	if (!luaL_pCall)
	{
		err("пизда lua_pcall");
		return;
	}

	LuaLoaded = true;
}

void LuaExecutor::ExecuteString(char* code)
{
	if (!LuaLoaded)
	{
		err("ахахахахахах)))))))");
		return;
	}

	CLuaInterface = ILuaShared->GetLuaInterface(LUAINTERFACE_CLIENT);
	if (!CLuaInterface)
	{
		err("пизда cluainterface");
		return;
	}

	pGLuaState = *(DWORD*)(CLuaInterface + 0x4);
	if (!pGLuaState)
	{
		err("ооо бляяя luastate");
		return;
	}

	luaL_loadString((void*)pGLuaState, code);
	luaL_pCall((void*)pGLuaState, 0, LUA_MULTRET, 0);
}
