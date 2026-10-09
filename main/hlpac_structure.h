// SPDX-License-Identifier: MPL-2.0
#ifndef HLPAC_STRUCTURES_H
#define HLPAC_STRUCTURES_H
#include<stddef.h>
#include<stdint.h>
typedef enum:uint16_t{
    FUNCTION,
    SECTION,
    SCOPE,
    STRUCT_DECL,
    EXTERN,
    CALL,
    OPERATION_FOUR,
    OPERATION_THREE,
    OPERATION_TWO,
    OPERATION_SINGLE,
    LABEL,
    JMP,
    JMP_WHEN,
    JMP_FLAGWITH,
    MOV_WHEN,
    REG_DECL,
    RET,
    LITERAL,
    REG_LABEL
}HLPAASTTypes;
typedef enum:uint16_t{
    UNSETTING,
    VOID,
    BOOLEAN,
    I8,
    U8,
    I16,
    U16,
    I32,
    U32,
    I64,
    U64,
    I128,
    U128,
    F32,
    F64,
    PTR,
    STRUCT_TYPE,
    LOCAL_ELEM=256,
    GLOBAL_ELEM,
    LOCAL_ELEM_DEREF,
    GLOBAL_ELEM_DEREF,
    LOCAL_ADDR,
    GLOBAL_ADDR,
    SIB_EXPR,
    SIB_EXPR_DEREF,
}HLPAElemTypes;
typedef struct{
    HLPAASTTypes typing;
    HLPAElemTypes element_type;
    uint32_t element_index;
}RetNode;
typedef struct{
    HLPAASTTypes typing;
    HLPAElemTypes data_type;
    uint32_t data_index;
    HLPAElemTypes res_elem_type;
    HLPAElemTypes res_explit_type;
    uint32_t res_elem_index;
    uint32_t res_explit_type_extra;
    uint32_t args_count;
    uint32_t args_index;
}CallNode;
typedef enum:uint16_t{
    ADD,
    SUB,
    ADC,
    SBB,
    AND,
    OR,
    XOR,
    SHL,
    SHR,
    MOV=64,
    TEST,
    CMP,
    LEA,
}OperationType;
typedef struct{
    HLPAASTTypes typing;
    OperationType opt_type;
    HLPAElemTypes src1_type;
    HLPAElemTypes src2_type;
    HLPAElemTypes dst_type;
    uint32_t src1_index;
    uint32_t src2_index;
    uint32_t dst_index;
}TripleOptNumNode;
typedef struct{
    HLPAASTTypes typing;
    OperationType opt_type;
    HLPAElemTypes src_type;
    HLPAElemTypes dst_type;
    uint32_t src_index;
    uint32_t dst_index;
}DoubleOptNumNode;
typedef struct{
    uint32_t disp;
    HLPAElemTypes base_type;
    HLPAElemTypes index_type;
    uint32_t base_index;
    uint32_t index_index;
    uint32_t explit_type_extra;
    HLPAElemTypes explit_type;
    uint8_t scale;
}SIBExprNode;
typedef struct{
    uint32_t name_index;
    uint16_t name_size;
    uint16_t bool_field; // LE in file
}VariableNode;
#endif
