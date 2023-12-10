#ifndef LUA_ALLOCATOR_HPP_
#define LUA_ALLOCATOR_HPP_
#include <memory>
#include <new>
#include <type_traits>
#include <string>
#include <array>

template<typename T>
class AllocatorLua {
    template<typename U>
    friend class AllocatorLua;

public:
    typedef T value_type;
    typedef size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef std::true_type propagate_on_container_move_assignment;

    AllocatorLua(void* memory) : m_memory{ memory } {}
    AllocatorLua(const AllocatorLua& alloc) noexcept : m_memory{ alloc.m_memory } {}
    AllocatorLua(AllocatorLua&& alloc) noexcept : m_memory{ alloc.m_memory }
    { alloc.m_memory = nullptr; }

    template<typename U>
    AllocatorLua(const AllocatorLua<U>& alloc) noexcept : m_memory{ alloc.m_memory } {}
    template<typename U>
    AllocatorLua(AllocatorLua<U>&& alloc) noexcept : m_memory{ alloc.m_memory }
    { alloc.m_memory = nullptr; }

    AllocatorLua& operator=(AllocatorLua&& alloc) noexcept
    {
        m_memory = alloc.m_memory;
        alloc.m_memory = nullptr;

        return *this;
    }

    AllocatorLua& operator=(const AllocatorLua& alloc) noexcept
    {
        m_memory = alloc.m_memory;

        return *this;
    }

    [[nodiscard]]
    T* allocate(size_type size)
    {
        return static_cast<T*>(m_memory);
    }

    void deallocate(T* ptr, size_type size) {}

private:
    void* m_memory;
};
#endif