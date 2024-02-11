#pragma once

#define MEM_STACKALLOC(SIZE) _malloca(SIZE)

struct address {
    std::uint8_t* m_addr;

public:
    inline address(std::uint8_t* addr) {
        this->m_addr = addr;
    }

    template< typename T >
    inline T get() {
        return T(m_addr);
    }

    inline address to_absolute(int pre, int post)
    {
        this->add(pre);

        this->m_addr = this->m_addr + sizeof(int) + *(int*)(this->m_addr);

        this->add(post);

        return address(this->m_addr);
    }
    
    inline address self_rva(uintptr_t rva, uintptr_t rip = 0x0)
    {
        std::uint32_t nRVA = *reinterpret_cast<std::uint32_t*>(this->m_addr + rva);
        std::uint64_t nRIP = reinterpret_cast<std::uint64_t>(this->m_addr) + rip;

        this->m_addr = reinterpret_cast<std::uint8_t*>(nRVA + nRIP);

        return address(this->m_addr);
    }

    inline address deref() {
        return address(*reinterpret_cast<std::uint8_t**>(this->m_addr));
    }

    inline address rel32() {
        return address(this->m_addr + *reinterpret_cast<std::int32_t*>(this->m_addr) + sizeof(std::int32_t));
    }

    inline address sub(std::uintptr_t bytes) {
        return address(m_addr - bytes);
    }

    inline address add(std::uintptr_t bytes) {
        return address(m_addr + bytes);
    }

    inline address abs(std::size_t length = 7) {
        auto displacement = *reinterpret_cast<std::int32_t*>(this->m_addr + 0x3);

        this->add(displacement);
        this->add(length);

        return address(this->m_addr);
    }
};

namespace signature
{
    address find(const char* module_name, const char* pattern_bytes);

}// namespace signature