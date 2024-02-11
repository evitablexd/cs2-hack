#pragma once

#define CALL_VIRTUAL(retType, idx, ...) \
  Virtual::CallVirtual<retType>(idx, __VA_ARGS__)

namespace Virtual {
    template <typename T>
    __forceinline T Get(void* instance, const unsigned int index) {
        return (*static_cast<T**>(instance))[index];
    }

    template <typename T>
    inline T Read(const std::uintptr_t location) {
        return *reinterpret_cast<T*>(location);
    }

    template <typename T>
    inline void Write(const std::uintptr_t location, const T& data) {
        *reinterpret_cast<T*>(location) = data;
    }

    template <typename T = void*>
    inline T GetVMethod(uint32_t uIndex, void* pClass) {
        if (!pClass) {
            return T{};
        }

        void** pVTable = *static_cast<void***>(pClass);
        if (!pVTable) {
            return T{};
        }

        return reinterpret_cast<T>(pVTable[uIndex]);
    }

    template <typename T, typename... Args>
    inline T CallVirtual(uint32_t uIndex, void* pClass, Args... args) {
        auto pFunc = GetVMethod<T(__thiscall*)(void*, Args...)>(uIndex, pClass);
        if (!pFunc) {
            return T{};
        }

        return pFunc(pClass, args...);
    }
}
