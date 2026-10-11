// SPDX-License-Identifier: MPL-2.0
#include"hlpac_mapping.h"
#include<stdlib.h>
HLPAMapping* hlpac_mapping_new(){
    return malloc(sizeof(HLPAMapping));
}
void hlpac_mapping_init(HLPAMapping* self){
    self->max_count=0;
    self->curr_count=0;
    self->flags=nullptr;
    self->datas=nullptr;
}
HLPAMapping* hlpac_mapping_create(){
    HLPAMapping* self = hlpac_mapping_new();
    if(self)hlpac_mapping_init(self);
    return self;
}
uint32_t hlpac_mapping_string_hash_get_char_utf8(const char* string,uint8_t* char_size){
    if(!(*string&0x80)){
        *char_size=1;
        return *string;
    }else if((*string&0xe0)==0xc0){
        *char_size=2;
        return ((*string&0x1f)<<6)+(string[1]&0x3f);
    }else if((*string&0xf0)==0xe0){
        *char_size=3;
        return ((*string&0x0f)<<12)+((string[1]&0x3f)<<6)+(string[2]&0x3f);
    }else if((*string&0xf8)==0xf0){
        *char_size=4;
        return ((*string&0x07)<<18)+((string[1]&0x3f)<<12)+((string[2]&0x3f)<<6)+(string[3]&0x3f);
    }
    return 0xffffffff;
}
static inline uint64_t hash_finalize(uint64_t x){
    x ^= (x>>33)|(x<<31);
    x *= 0xff51afd7ed558ccdULL;
    x ^= (x>>33)|(x<<31);
    x *= 0xc4ceb9fe1a85ec53ULL;
    x ^= (x>>33)|(x<<31);
    return x;
}
uint64_t hlpac_mapping_string_hash(const char* string,size_t size){
    uint64_t res=14695981039346656037ull;
    for(size_t i=0;i<size;){
        uint8_t char_size;
        auto data=hlpac_mapping_string_hash_get_char_utf8(string+i,&char_size);
        i += char_size;
        uint8_t split[3]={},length;
        if(data<=0x7f){
            length=2;
            split[2]=data;
        }else if(data<=0x407f){
            length=1;
            data-=0x80;
            split[1]=(data>>8)|0x80,split[2]=data;
        }else{
            length=0;
            data-=0x4080;
            split[0]=(data>>16)|0xc0,split[1]=data>>8,split[2]=data;
        }
        for(int j=length;j<3;j++){
            res^=split[j];
            res*=1099511628211ull;
        }
    }
    return hash_finalize(res);
}
void hlpac_mapping_set(HLPAMapping* self,uint64_t hash,void* value){
    // TODO:扩容逻辑
    auto i=hash;
    auto curr=hash;
    for(;i&=(self->max_count-1),self->datas[i].key==hash||((self->flags[i>>2])>>((i&3)<<1))&1;){
        i*=5;
        i+=(curr>>=5,curr)+1;
    }
    if(self->datas[i].key!=hash)self->datas[i].key=hash;
    self->datas[i].datas=value;
    self->flags[i>>2]&=~(3<<((i&3)<<1));
}
