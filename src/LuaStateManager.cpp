#include <LuaStateManager.hpp>
#include <fstream>

LuaStateMan::LuaStateMan() : m_state{ luaL_newstate() } {}

LuaStateMan::~LuaStateMan()
{
    lua_close(m_state);
}

lua_State* LuaStateMan::Get() const noexcept
{
    return m_state;
}

void LuaStateMan::LoadGlobal(std::string_view varName) const noexcept
{
    lua_getglobal(m_state, std::data(varName));
}

std::string LuaStateMan::GetString(std::int32_t index) const noexcept
{
    if (lua_isstring(m_state, index))
        return lua_tostring(m_state, index);

    return std::string{};
}

bool LuaStateMan::OpenScript(std::string_view filePath) const noexcept
{
    return CheckError(luaL_dofile(m_state, std::data(filePath)));
}

bool LuaStateMan::CheckError(int errorCode) const noexcept
{
    if (errorCode != LUA_OK)
    {
        // TO DO: Change it into something better.
        static std::ofstream log{
            "ErrorLog.txt", std::ios_base::app | std::ios_base::ate
        };
        log << GetString(-1) << std::endl;

        return false;
    }

    return true;
}

bool LuaStateMan::GetBool(std::int32_t index) const noexcept
{
    if (lua_isboolean(m_state, index))
        return static_cast<bool>(lua_toboolean(m_state, index));
    
    return bool{};
}

void LuaStateMan::PushNull() const noexcept
{
    lua_pushnil(m_state);
}

void LuaStateMan::PushBool(bool value) const noexcept
{
    lua_pushboolean(m_state, static_cast<std::int32_t>(value));
}

void LuaStateMan::PushString(std::string_view str) const noexcept
{
    lua_pushlstring(m_state, std::data(str), std::size(str));
}

void LuaStateMan::CallFunctionV(std::string_view functionName) const noexcept
{
    LoadGlobal(functionName);

    if(lua_isfunction(m_state, -1))
    {
        CheckError(lua_pcall(m_state, 0, 0, 0));
    }
}
