#include <vector>
#include <LuaAllocator.hpp>
#include <LuaLibraries.hpp>

struct ArrayData
{
    size_t size;
    lua_Number data[1];
};

static constexpr const char* arrayLibraryName = "Selene.Array";

static int NewArray(lua_State* L)
{
    int elementCount = luaL_checkinteger(L, 1);

    luaL_argcheck(L, elementCount >= 1, 1, "Invalid Size.");

    // Since ArrayData already has 1 number, so subtract it.
    const size_t dataSize = (elementCount - 1u) * sizeof(lua_Number);
    const size_t arrayDataSize = sizeof(ArrayData);
    const size_t totalSize = elementCount + arrayDataSize;
    auto data = static_cast<ArrayData*>(lua_newuserdata(L, totalSize));
    data->size = elementCount;

    lua_getmetatable(L, 1); // Loads the meta table at the top of the stack
    lua_setmetatable(L, -2); // Registers the userdata at the index -2 with the metatable

    return 1;
}

[[nodiscard]]
ArrayData* GetArrayData(lua_State* L)
{
    return static_cast<ArrayData*>(luaL_checkudata(L, 1, arrayLibraryName));
}

static int SetArray(lua_State* L)
{
    using NumberAllocator = AllocatorLua<lua_Number>;

    auto arrayData = GetArrayData(L);
    auto index = static_cast<std::int32_t>(luaL_checkinteger(L, 2) - 1); // Lua's index start at 1.

    luaL_argcheck(L, 0 <= index && index < arrayData->size, 2, "Index is out of bound");

    std::vector<lua_Number, NumberAllocator> array{
        arrayData->size, NumberAllocator{arrayData->data}
    };

    if (lua_isnumber(L, 3))
        array.at(index) = lua_tonumber(L, 3);

    return 0;
}

static int GetArray(lua_State* L)
{
    using NumberAllocator = AllocatorLua<lua_Number>;

    auto arrayData = GetArrayData(L);
    auto index = static_cast<std::int32_t>(luaL_checkinteger(L, 2) - 1); // Lua's index start at 1.

    luaL_argcheck(L, 0 <= index && index < arrayData->size, 2, "Index is out of bound");

    std::vector<lua_Number, NumberAllocator> array{
        arrayData->size, NumberAllocator{arrayData->data}
    };

    lua_pushnumber(L, array.at(index));

    return 1;
}

static int GetSize(lua_State* L)
{
    auto arrayData = GetArrayData(L);

    lua_pushinteger(L, static_cast<lua_Integer>(arrayData->size));

    return 1;
}

static const struct luaL_Reg arrayLib[]
{
    {"new", NewArray},
    {"set", SetArray},
    {"get", GetArray},
    {"size", GetSize},
    {NULL, NULL}
};

int luaopen_array(lua_State* L)
{
    luaL_newmetatable(L, arrayLibraryName);
    luaL_newlib(L, arrayLib);

    return 1;
}
