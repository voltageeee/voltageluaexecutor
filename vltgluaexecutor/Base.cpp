#include "Base.h"
#define LUA_MULTRET -1

LuaExecutor* GLuaExecutor = new LuaExecutor();

void err(std::string err)
{
	MessageBoxA(NULL, err.c_str(), "Отсосешь", 0);
}

// люто хуевая ужасная тупая паста
void LuaExecutor::InitLuaExecutor()
{
	HMODULE luaShared = GetModuleHandleA("lua_shared.dll");

	if (!luaShared)
	{
		err("не удалось инициализровать luashared");
		return;
	}

	CreateInterface CreateInterface_src = (CreateInterface)GetProcAddress(luaShared, "CreateInterface");
	ILuaShared = (CLuaShared*)CreateInterface_src("LUASHARED003", NULL);
	if (!ILuaShared)
	{
		err("не удалось инициализировать ilua интерфейс");
		return;
	}

	luaL_loadString = (_luaL_loadString)GetProcAddress(luaShared, "luaL_loadstring");
	if (!luaL_loadString)
	{
		err("не удалось подгрузить lua_loadstring");
		return;
	}

	luaL_pCall = (_luaL_pCall)GetProcAddress(luaShared, "lua_pcall");
	if (!luaL_pCall)
	{
		err("не удалось подгрузить lua_pcall");
		return;
	}

	LuaLoaded = true;
}

void LuaExecutor::ExecuteString(char* code)
{
	if (!LuaLoaded)
	{
		err("интерфейс луа не подгружен)))))))");
		return;
	}

	CLuaInterface = ILuaShared->GetLuaInterface(LUAINTERFACE_CLIENT);
	if (!CLuaInterface)
	{
		err("не получилось подгрузить cluainterface");
		return;
	}

	pGLuaState = *(DWORD*)(CLuaInterface + 0x4);
	if (!pGLuaState)
	{
		err("не получилось подгрузить luastate");
		return;
	}

	luaL_loadString((void*)pGLuaState, code);
	luaL_pCall((void*)pGLuaState, 0, LUA_MULTRET, 0);
}