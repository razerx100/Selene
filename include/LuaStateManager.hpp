#ifndef LUA_STATE_MANAGER_HPP_
#define LUA_STATE_MANAGER_HPP_
#include <cstdint>
#include <string>
#include <concepts>

#include <lua.hpp>

template<typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

class LuaStateMan
{
public:
    LuaStateMan();
    ~LuaStateMan();

    [[nodiscard]]
    lua_State* Get() const noexcept;

    void LoadGlobal(std::string_view varName) const noexcept;

    [[nodiscard]]
    std::string GetString(std::int32_t index = -1) const noexcept;

    template<Arithmetic T>
    [[nodiscard]]
    T GetNumber(std::int32_t index = -1) const noexcept
    {
        if (lua_isnumber(m_state, index))
            return static_cast<T>(lua_tonumber(m_state, index));

        return T{};
    }

    [[nodiscard]]
    bool GetBool(std::int32_t index = -1) const noexcept;

    template<Arithmetic T>
    void PushNumber(T number) const noexcept
    {
        lua_pushnumber(m_state, static_cast<lua_Number>(number));
    }

    void PushNull() const noexcept;
    void PushBool(bool value) const noexcept;
    void PushString(std::string_view str) const noexcept;

    bool OpenScript(std::string_view filePath) const noexcept;

    template<typename ReturnType>
    [[nodiscard]]
    ReturnType GetReturn() const noexcept // Must be specilised for each type of calls
    {
        return ReturnType{};
    }

    template<Arithmetic ReturnType>
    [[nodiscard]]
    ReturnType GetReturn() const noexcept
    {
        return GetNumber<ReturnType>();
    }

    template<>
    [[nodiscard]]
    std::string GetReturn() const noexcept
    {
        return GetString();
    }

    template<>
    [[nodiscard]]
    bool GetReturn() const noexcept
    {
        return GetBool();
    }

    template<typename T>
    void SetArgument(T arg) const noexcept {} // Must be specilised for each type of calls

    template<Arithmetic T>
    void SetArgument(T arg) const noexcept
    {
        PushNumber(arg);
    }

    template<>
    void SetArgument(std::nullptr_t null) const noexcept
    {
        PushNull();
    }

    template<>
    void SetArgument(bool arg) const noexcept
    {
        PushBool(arg);
    }

    template<>
    void SetArgument(const char* arg) const noexcept
    {
        PushString(arg);
    }

    template<>
    void SetArgument(const std::string& arg) const noexcept
    {
        PushString(arg);
    }

    template<>
    void SetArgument(std::string_view arg) const noexcept
    {
        PushString(arg);
    }

    template<typename Arg, typename... Args>
    void SetArgument(Arg argument, Args... arguments) const noexcept
    {
        SetArgument(argument);
        SetArgument(std::forward<Args>(arguments)...);
    }

    template<typename ReturnType>
    [[nodiscard]]
    ReturnType CallFunction(std::string_view functionName) const noexcept
    {
        LoadGlobal(functionName);

        if(lua_isfunction(m_state, -1))
        {
            if(CheckError(lua_pcall(m_state, 0, 1, 0)))
            {
                return GetReturn<ReturnType>();              
            }
        }

        return ReturnType{};
    }

    template<typename ReturnType, typename Arg>
    [[nodiscard]]
    ReturnType CallFunction(std::string_view functionName, Arg argument) const noexcept
    {
        LoadGlobal(functionName);

        if(lua_isfunction(m_state, -1))
        {
            SetArgument(argument);

            if(CheckError(lua_pcall(m_state, 1, 1, 0)))
            {
                return GetReturn<ReturnType>();              
            }
        }

        return ReturnType{};
    }

    template<typename ReturnType, typename... Args>
    [[nodiscard]]
    ReturnType CallFunction(std::string_view functionName, Args... arguments) const noexcept
    {
        LoadGlobal(functionName);

        if(lua_isfunction(m_state, -1))
        {
            SetArgument(std::forward<Args>(arguments)...);

            if(CheckError(lua_pcall(m_state, sizeof...(arguments), 1, 0)))
            {
                return GetReturn<ReturnType>();              
            }
        }

        return ReturnType{};       
    }

    void CallFunctionV(std::string_view functionName) const noexcept;

    template<typename Arg>
    void CallFunctionV(std::string_view functionName, Arg argument) const noexcept
    {
        LoadGlobal(functionName);

        if(lua_isfunction(m_state, -1))
        {
            SetArgument(argument);

            CheckError(lua_pcall(m_state, 1, 0, 0));
        }
    }

    template<typename... Args>
    void CallFunctionV(std::string_view functionName, Args... arguments) const noexcept
    {
        LoadGlobal(functionName);

        if(lua_isfunction(m_state, -1))
        {
            SetArgument(std::forward<Args>(arguments)...);

            CheckError(lua_pcall(m_state, sizeof...(arguments), 0, 0));
        }
    }

private:
    bool CheckError(int errorCode) const noexcept;

private:
    lua_State* m_state;
};
#endif