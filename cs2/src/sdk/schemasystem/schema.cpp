#include "../../includes.h"

using SchemaKeyValueMap_t = std::unordered_map<uint32_t, int16_t>;
using SchemaTableMap_t = std::unordered_map<uint32_t, SchemaKeyValueMap_t>;

static bool InitSchemaFieldsForClass(SchemaTableMap_t& tableMap,
    const char* className, uint32_t classKey) {
    CSchemaSystemTypeScope* pType = interfaces::g_schema->FindTypeScopeForModule("client.dll");

    if (!pType) return false;

    SchemaClassInfoData_t* pClassInfo = pType->FindDeclaredClass(className);
    if (!pClassInfo) {
        tableMap.emplace(classKey, SchemaKeyValueMap_t{});
        return false;
    }

    short fieldsSize = pClassInfo->m_fields_size;
    SchemaClassFieldData_t* pFields = pClassInfo->m_fields;

    auto& keyValueMap = tableMap[classKey];
    keyValueMap.reserve(fieldsSize);

    for (int i = 0; i < fieldsSize; ++i) {
        SchemaClassFieldData_t& field = pFields[i];

        keyValueMap.emplace(FNV1A::hash_32_fnv1a_const(field.m_name), field.m_offset);
    }

    return true;
}

int16_t schema::GetOffset(const char* className, uint32_t classKey, const char* memberName, uint32_t memberKey)
{
    static SchemaTableMap_t schemaTableMap;
    const auto& tableMapIt = schemaTableMap.find(classKey);
    if (tableMapIt == schemaTableMap.cend()) {
        InitSchemaFieldsForClass(schemaTableMap, className, classKey);
        return GetOffset(className, classKey, memberName, memberKey);
    }

    const SchemaKeyValueMap_t& tableMap = tableMapIt->second;
    if (tableMap.find(memberKey) == tableMap.cend()) {
        return 0;
    }

    return tableMap.at(memberKey);
}
