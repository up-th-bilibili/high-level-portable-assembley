// SPDX-License-Identifier: MPL-2.0
#ifndef HLPAC_MAPPING_H
#define HLPAC_MAPPING_H 1
#include<stddef.h>
#include<stdint.h>
#include"hlpac_structure.h"
typedef struct{
    uint64_t key;
    void* datas;
}HLPAMappingElement;
typedef struct{
    size_t max_count;
    size_t curr_count;
    uint8_t* flags;
    HLPAMappingElement* datas;
}HLPAMapping;
HLPAMapping* hlpac_mapping_create();
HLPAMapping* hlpac_mapping_new();
void hlpac_mapping_init(HLPAMapping* self);
void hlpac_mapping_set(HLPAMapping* self,uint64_t hash,void* value);
#endif
