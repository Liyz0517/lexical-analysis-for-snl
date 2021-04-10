//
//  globals.h
//  SNL语言_语法分析器_LL(1)方法
//
//  Created by 李永桢 on 2021/3/30.
//  Copyright © 2021 李永桢. All rights reserved.
//

using namespace std;
#ifndef globals_h
#define globals_h

#define MAX_NUM 100

// 语法树结点类型
typedef enum {
    ProK, PheadK, TypeK, VarK, ProcDecK, StmlK, DecK, StmtK, ExpK
} NodeKind;

// 语法树声明类型
typedef enum {
    ArrayK, CharK, IntegerK, RecordK, IdK
} DecType;

// 语法树语句类型
typedef enum {
    IfK, WhileK, AssignK, ReadK, WriteK, CallK, ReturnK
} StmtType;

// 语法树表达式类型
typedef enum {
    OpK, ConstK, IdEK
} ExpType;

// 语法树节点其他属性：数组的属性
struct ARRAYATTR {
    int low;           // 数组下界
    int up;            // 数组上界
    string childType;  // 数组的成员类型
};

// 语法树节点其他属性：过程的属性
typedef enum {     // 过程的参数类型
    valparamtype,  // 值参
    varparamtype   // 变参
} Paramt;
struct PROCATTR {
    Paramt paramt;  // 过程的参数类型
};

// 语法树节点其他属性：表达式的属性
//typedef enum {  // 表达式的运算符
//    LT, EQ,         // 关系运算
//    PLUS, MINUS,    // 加法
//    TIMES, OVER     // 乘法
//} Op;
typedef enum {  // 表达式的变量类别
    IdV,          // 标志符变量
    ArrayMembV,   // 数组成员变量
    FieldMembV    // 域成员变量
} VarKind;
typedef enum {  // 表达式的检查类型
    Void,     // 无返回类型
    Integer,  // 整型
    Boolean,  // 布尔型
} ExpCheckType;
struct EXPATTR {
    LexType op;            // 运算符
    int val;               // 数值
    VarKind varkind;       // 变量类型
    ExpCheckType type;     // 检查类型
};

// 语法树节点其他属性
struct Attr {
    ARRAYATTR ArrayAttr;  // 数组的属性
    PROCATTR ProcAttr;    // 过程的属性
    EXPATTR ExpAttr;      // 表达式的属性

};


// 语法树结点
class Node{
private:
    Node* child[3];        // 儿子结点
    Node* sibling;         // 兄弟结点
    int lineNo;            // 行号
    NodeKind nodeKind;     // 结点类型
    int idnum;             // 结点内标志符个数
    string name[MAX_NUM];  // 标志符名字
    // 指针数组table
    string type_name;      // 类型名
    Attr attr;             // 其他属性
};

#endif /* globals_h */
