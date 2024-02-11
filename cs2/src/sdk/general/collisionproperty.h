#pragma once

class CCollisionProperty {
public:
    SCHEMA_FIELD(m_vecMins, "CCollisionProperty", "m_vecMins", Vector_t);
    SCHEMA_FIELD(m_vecMaxs, "CCollisionProperty", "m_vecMaxs", Vector_t);
    SCHEMA_FIELD(m_solidFlags, "CCollisionProperty", "m_usSolidFlags",
        std::uint16_t);

    auto CollisionMask() {
        return *reinterpret_cast<std::uint16_t*>((uintptr_t)(this) + 0x38);
    }
};