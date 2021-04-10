//
//  grammar.h
//  SNL语言_语法分析器_LL(1)方法
//
//  Created by 李永桢 on 2021/3/30.
//  Copyright © 2021 李永桢. All rights reserved.
//

#ifndef syntax_h
#define syntax_h

#include <iostream>
#include "string"
using namespace std;


// 定义
#define NonTerType_Num  67
#define LexType_Num     41


// 非终极符
typedef enum {
    Program,  // 总程序
    ProgramHead, ProgramName,  // 程序头
    DeclarePart,               // 程序声明 4
    TypeDecpart, TypeDec, TypeDecList, TypeDecMore, TypeId, // 类型声明 9
    TypeDef, BaseType, StructureType, ArrayType, Low, Top, RecType, FieldDecList, FieldDecMore, IdList, IdMore, // 类型 20
    VarDecpart, VarDec, VarDecList, VarDecMore, VarIdList, VarIdMore,  // 变量声明 26
    ProcDecpart, ProcDec, ProcDecMore, ProcName,                       // 过程声明 30
    ParamList, ParamDecList, ParamMore, Param, FormList, FidMore,      // 参数声明 36
    ProcDecPart,       // 过程中的声明部分
    ProcBody,          // 过程体
    ProgramBody,       // 主程序体
    StmList, StmMore,  // 语句序列
    Stm, AssCall,      // 语句
    AssignmentRest,    // 赋值语句
    ConditionalStm,    // 条件语句
    LoopStm,           // 循环语句
    InputStm, Invar,   // 输入语句
    OutputStm,         // 输出语句
    ReturnStm,         // 返回语句
    CallStmRest, ActParamList, ActParamMore,  // 过程调用语句 54
    RelExp, OtherRelE,  // 条件表达式
    Exp, OtherTerm,     // 算术表达式
    Term, OtherFactor,  // 项 60
    Factor, Variable, VariMore, FieldVar, FieldVarMore, CmpOp, AddOp, MultOp  // 因子 67
} NonTerType;

// 非终极符查找表
static struct{
    string str;
    NonTerType tok;
}
nonTerTypeWords[NonTerType_Num] = {
    {"Program",Program}, {"ProgramHead",ProgramHead}, {"ProgramName",ProgramName}, {"DeclarePart",DeclarePart},
    {"TypeDecpart",TypeDecpart}, {"TypeDec",TypeDec},
    {"TypeDecList",TypeDecList}, {"TypeDecMore",TypeDecMore}, {"TypeId",TypeId}, {"TypeDef",TypeDef},
    {"BaseType",BaseType}, {"StructureType",StructureType}, {"ArrayType",ArrayType}, {"Low",Low},
    {"Top",Top}, {"RecType",RecType}, {"FieldDecList",FieldDecList}, {"FieldDecMore",FieldDecMore},
    {"IdList",IdList}, {"IdMore",IdMore}, {"VarDecpart",VarDecpart}, {"VarDec",VarDec},
    {"VarDecList",VarDecList}, {"VarDecMore",VarDecMore}, {"VarIdList",VarIdList}, {"VarIdMore",VarIdMore},
    {"ProcDecpart",ProcDecpart}, {"ProcDec",ProcDec}, {"ProcDecMore",ProcDecMore}, {"ProcName",ProcName},
    {"ParamList",ParamList}, {"ParamDecList",ParamDecList}, {"ParamMore",ParamMore}, {"Param",Param},
    {"FormList",FormList}, {"FidMore",FidMore}, {"ProcDecPart",ProcDecPart}, {"ProcBody",ProcBody},
    {"ProgramBody",ProgramBody}, {"StmList",StmList}, {"StmMore",StmMore}, {"Stm",Stm},
    {"AssCall",AssCall}, {"AssignmentRest",AssignmentRest}, {"ConditionalStm",ConditionalStm}, {"LoopStm",LoopStm},
    {"InputStm",InputStm}, {"Invar",Invar}, {"OutputStm",OutputStm}, {"ReturnStm",ReturnStm},
    {"CallStmRest",CallStmRest}, {"ActParamList",ActParamList}, {"ActParamMore",ActParamMore}, {"RelExp",RelExp},
    {"OtherRelE",OtherRelE}, {"Exp",Exp}, {"OtherTerm",OtherTerm}, {"Term",Term},
    {"OtherFactor",OtherFactor}, {"Factor",Factor}, {"Variable",Variable}, {"VariMore",VariMore},
    {"FieldVar",FieldVar}, {"FieldVarMore",FieldVarMore}, {"CmpOp",CmpOp}, {"AddOp",AddOp}, {"MultOp",MultOp}
};


// 终极符
typedef enum {
    ID,          /* 标识符 */
    CHARC,        /* 字符标识符 */
    INTC,        /* 无符号整数标识符 */
    UNDERANGE,   /* 数组下标界限符: '..' */
    ASSIGN,      /* 双字符分界符: ':=' */
    ERROR,       /* 错误 */ // 6
    /* 保留字 */
    PROGRAM, PROCEDURE, TYPE,   VAR,   IF,   INTEGER,
    THEN,    ELSE,      FI,     WHILE, DO,   CHAR,
    ENDWH,   BEGIN,    END,   READ,  WRITE,
    ARRAY,   RECORD,    OF,     RETURN, // 26
    /* 单字符分界符 */
    EQ, LT, PLUS, MINUS, TIMES, OVER,      // '=', '<', '+', '-', '*', '/',
    LMIDPAREN, RMIDPAREN, LPAREN, RPAREN,  // '[', ']', '(', ')'
    DOT, SEMI, COMMA, SINGLEQUOTE          // '.', ';', ',', '\'' // 40
} LexType;

// 终极符查找表
static struct{
    string str;
    LexType tok;
}
lexTypeWords[LexType_Num] = {
    {"ID",ID}, {"CHARC",CHARC}, {"INTC",INTC}, {"UNDERANGE",UNDERANGE}, {"ASSIGN",ASSIGN}, {"ERROR",ERROR},
    /* 保留字 */
    {"PROGRAM",PROGRAM}, {"PROCEDURE",PROCEDURE}, {"TYPE",TYPE}, {"VAR",VAR}, {"IF",IF}, {"INTEGER",INTEGER},{"CHAR",CHAR},
    {"THEN",THEN}, {"ELSE",ELSE}, {"FI",FI}, {"WHILE",WHILE}, {"DO",DO},
    {"ENDWH",ENDWH}, {"BEGIN",BEGIN}, {"END",END}, {"READ",READ}, {"WRITE",WRITE},
    {"ARRAY",ARRAY}, {"RECORD",RECORD}, {"OF",OF}, {"RETURN",RETURN},
    /* 单字符分界符 */
    {"=",EQ}, {"<",LT}, {"+",PLUS}, {"-",MINUS}, {"*",TIMES}, {"/",OVER},
    {"(",LPAREN}, {")",RPAREN}, {"[",LMIDPAREN}, {"]",RMIDPAREN},
    {".",DOT}, {";",SEMI}, {",",COMMA}, {"\'",SINGLEQUOTE}
};

// 非终极符string类型转enum
NonTerType nttStrToEnum( string s ) {
    for (int i = 0; i < NonTerType_Num; i++) {
        if ( s == nonTerTypeWords[i].str ) {
            return nonTerTypeWords[i].tok;
        }
    }
    return Program;
}

// 非终极符enum转string类型
string enumToNttStr( NonTerType s ) {
    for (int i = 0; i < NonTerType_Num; i++) {
        if ( s == nonTerTypeWords[i].tok ) {
            return nonTerTypeWords[i].str;
        }
    }
    return "Program";
}

// 终极符string类型转enum
LexType ltStrToEnum( string s ) {
    for (int i = 0; i < LexType_Num; i++) {
        if ( s == lexTypeWords[i].str ) {
            return lexTypeWords[i].tok;
        }
    }
    return PROGRAM;
}

// 终极符enum转string类型
string enumToltStr( LexType s ) {
    for (int i = 0; i < LexType_Num; i++) {
        if ( s == lexTypeWords[i].tok ) {
            return lexTypeWords[i].str;
        }
    }
    return "Program";
}



#endif /* grammar_h */
