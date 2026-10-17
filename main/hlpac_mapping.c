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
    if(!self)return;
    if(!self->datas){
        self->datas = calloc(32,sizeof(HLPAMappingElement));
        if(!self->datas)return;
        self->flags = calloc(8,1);
        if(!self->flags){
            free(self->datas);
            self->datas=nullptr;
            return;
        }
        self->max_count=32;
    }
    if(self->curr_count==self->max_count){
        HLPAMappingElement* new_datas = calloc(self->max_count*2,sizeof(HLPAMappingElement));
        if(!new_datas){
            free(self->datas);
            free(self->flags);
            self->max_count=self->curr_count=0;
            self->flags=self->datas=nullptr;
            return;
        }
        uint8_t* new_flags = calloc(self->max_count>>1,1);
        if(!new_flags){
            free(new_datas);
            free(self->datas);
            free(self->flags);
            self->max_count=self->curr_count=0;
            self->flags=self->datas=nullptr;
            return;
        }
        HLPAMappingElement* old_datas=self->datas;
        uint8_t* old_flags=self->flags;
        size_t old_max_count=self->max_count;
        self->datas=new_datas;
        self->flags=new_flags;
        self->max_count=old_max_count<<1;
        self->curr_count=0;
        for(size_t j=0;j<old_max_count;j++){
            if(((old_flags[j>>2])>>((j&3)<<1))&1){
                hlpac_mapping_set(self,old_datas[j].key,old_datas[j].datas);
            }
        }
        free(old_datas);
        free(old_flags);
    }
    auto i=hash;
    auto curr=hash;
    for(;i&=(self->max_count-1),self->datas[i].key!=hash&&(((self->flags[i>>2])>>((i&3)<<1))&1);){
        i*=5;
        i+=(curr>>=5,curr)+1;
    }
    if(self->datas[i].key!=hash){
        self->datas[i].key=hash;
        self->curr_count+=1;
    }
    self->datas[i].datas=value;
    self->flags[i>>2]=(self->flags[i>>2]&~(3<<((i&3)<<1)))|(1<<((i&3)<<1));
}
void* hlpac_mapping_get(HLPAMapping* self,uint64_t hash){
    if(!self->max_count){
        errno = ENOENT;
        return nullptr;
    }
    auto i=hash;
    auto curr=hash;
    for(;i&=(self->max_count-1),self->datas[i].key!=hash&&(((self->flags[i>>2])>>((i&3)<<1))&3);){
        i*=5;
        i+=(curr>>=5,curr)+1;
    }
    if(self->datas[i].key!=hash){
        errno = ENOENT;
        return nullptr;
    }
    errno = 0;
    return self->datas[i].datas;
}
