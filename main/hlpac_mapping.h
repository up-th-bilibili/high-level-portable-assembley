// SPDX-License-Identifier: MPL-2.0
#ifndef HLPAC_MAPPING_H
#define HLPAC_MAPPING_H 1
#include<stddef.h>
#include<stdint.h>
#include"hlpac_structure.h"
typedef struct{
    size_t max_count;
    size_t curr_count;
    uint8_t* flags;
    struct element{
        uint64_t key;
        uint8_t datas[];
    }*datas;
}HLPAMapping;
HLPAMapping* hlpac_mapping_create();
HLPAMapping* hlpac_mapping_new();
void hlpac_mapping_init(HLPAMapping* self);
#endif
