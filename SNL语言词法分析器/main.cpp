//
//  main.cpp
//  SNL语言词法分析器
//
//  Created by 李永桢 on 2021/3/23.
//  Copyright © 2021 李永桢. All rights reserved.
//

// 库文件
#include "string.h"
#include "stdio.h"
#include <iostream>
#include <list>
using namespace std;

// 定义
#define MAXRESERVED 21  // 定义保留字数量常量 MAXRESERVED 为 21

// Token 种类
typedef enum {
    ID=0,        /* 标识符 */
    CHARC=1,     /* 字符标识符 */
    INTC=2,      /* 无符号整数标识符 */
    UNDERANGE,   /* 数组下标界限符: '..' */
    ASSIGN,      /* 双字符分界符: ':=' */
    ERROR,       /* 错误 */
    /* 保留字 */
    PROGRAM, PROCEDURE, TYPE,   VAR,   IF,   INTEGER,
    THEN,    ELSE,      FI,     WHILE, DO,
    ENDWH,   BEGIN,    END,   READ,  WRITE,
    ARRAY,   RECORD,    OF,     RETURN,
    /* 单字符分界符 */
    EQ, LT, PLUS, MINUS, TIMES, OVER,      // '=', '<', '+', '-', '*', '/',
    LMIDPAREN, RMIDPAREN, LPAREN, RPAREN,  // '[', ']', '(', ')'
    DOT, SEMI, COMMA, SINGLEQUOTE          // '.', ';', ',', '\''
} LexType;


// 函数
bool reservedLookUp( char* s );              // 查询保留字表
bool isLetter( char ch );                    // 判断当前字符是否为字母
bool isDigit( char ch );                     // 判断当前字符是否为数字
bool isSingleDelimiter( char ch );           // 判断当前字符是否为单字符分界符
string separateSingleDelimiter( string s );  // 区分是哪一个单分界符
string delLastChar( string s );              // 字符串去掉最后一个字符
string upperToLower( string s );             // 字符串大写字符转为小写
string lowerToUpper( string s );             // 字符串小写字符转为大写
bool isReserved( string s );                 // 判断字符串是否为保留字
LexType getLexType( string s );              // 获取LexType

/*
fseek(文件指针,偏移量，指定文件指针的位置)
起始点           常量名          常量值
文件开头         SEEK_SET          0
当前位置         SEEK_CUR          1
文件末尾         SEEK_END          2
*/

// 保留字查找表
static struct{
    string str;
    LexType tok;
} reservedWords[MAXRESERVED] = {
    {"program",PROGRAM},{"type",TYPE},{"var",VAR},
    {"procedure",PROCEDURE},{"begin",BEGIN},{"end",END},
    {"array",ARRAY},{"of",OF},{"record",RECORD},{"if",IF},{"then",THEN},
    {"else",ELSE},{"fi",FI},{"while",WHILE},{"do",DO},{"endwh",ENDWH},
    {"read",READ},{"write",WRITE},{"return",RETURN},
    {"integer",INTEGER},{"char",CHARC}
};

// Token实体类
class Token{
public:
    int lineshow;  // 行数
    string lex;    // 词法信息(单词总类)
    string sem;    // 语意信息(单词字符串本身)
public:
    Token(int lineshow, string lex, string sem){this->lineshow = lineshow; this->lex = lex; this->sem = sem;}
};



// 查保留字表
bool reservedLookUp( char* s ){
    for (int i = 0; i < MAXRESERVED; i++) {
        if ( s == reservedWords[i].str ) {
            return true;
        }
    }
    return false;
}

// 判断当前字符是否为字母
bool isLetter( char ch ){
    if ( (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') ) {
        return true;
    }
    return false;
}

// 判断当前字符是否为数字
bool isDigit( char ch ){
    if ( ch >= '0' && ch <='9' ) {
        return true;
    }
    return false;
}

// 判断当前字符是否为单字符分界符
bool isSingleDelimiter( char ch ) {
    if ( ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' || ch == ')'
        || ch == '[' || ch == ']' || ch == ';' || ch == '=' || ch == '<' || ch == ',' ) {
        return true;
    }
    return false;
}

// 区分是哪一个单分界符
string separateSingleDelimiter( string s ) {
    char ch = s[0];
    switch (ch) {
        case '=':
            return "EQ";
        case '<':
            return "LT";
        case '+':
            return "PLUS";
        case '-':
            return "MINUS";
        case '*':
            return "TIMES";
        case '/':
            return "OVER";
        case '(':
            return "LPAREN";
        case ')':
            return "RPAREN";
        case '[':
            return "LMIDPAREN";
        case ']':
            return "RMIDPAREN";
        case '.':
            return "DOT";
        case ';':
            return "SEMI";
        case ',':
            return "COMMA";
        case '\'':
            return "SINGLEQUOTE";
        default:
            return "DOT";
    }
}

// 字符串去掉最后一个字符
string delLastChar( string s ) {
    string str;
    for ( int i = 0; i < (int)s.length() - 1; i++) {
        str += s[i];
    }
    return str;
}

// 字符串大写字符转为小写
string upperToLower( string s ) {
    for ( int i = 0; i < (int)s.length(); i++) {
        if ( s[i] >= 'A' && s[i] <= 'Z' ) {
            s[i] += 32;
        }
    }
    return s;
}

// 字符串小写字符转为大写
string lowerToUpper( string s ) {
    for ( int i = 0; i < (int)s.length(); i++) {
        if ( s[i] >= 'a' && s[i] <= 'z' ) {
            s[i] -= 32;
        }
    }
    return s;
}

// 判断字符串是否为保留字
bool isReserved( string s ) {
    s = upperToLower(s);  // 转为小写
    for ( int i = 0; i < MAXRESERVED; i++) {
        if ( s == reservedWords[i].str ) {
            return true;
        }
    }
    return false;
}

// 获取LexType
LexType getLexType( string s ) {
    for ( int i = 0; i < MAXRESERVED; i++) {
        if ( s == reservedWords[i].str ) {
            return reservedWords[i].tok;
        }
    }
    return ERROR;
}

// 获取token序列
list<Token> getTokenList(){
    
    // 1.变量
    enum State{s0, s1, s2, s3, s4, s5, s6, s7, s8, s9};  // DFA的所有状态：开始，保留字，数字，完成，赋值，注释，数组下标，标识符，字符，错误
    int buffer;                                  // 用于读入字符
    State state = s0;                            // 初始状态为START
    string str;                                  // 用于接单词
    int line = 1, row = 1;                       // 行数，列数
    Token *token;                                // token
    list<Token> tokenList;                       // token链表
    
    
    // 2.打开文件：需要解析的源代码.txt
    int flag = 1;  // 判断是否成功打开文件
    FILE *fp;
    if ( !(fp = fopen("/Users/liyongzhen/Desktop/编译原理课程设计/词法分析/snl代码/code5.txt","r")) ) {
        //ifstream a("需要解析的源代码.txt");
        cout << "not found the file or other error " << endl;
        flag = 0;
    }
    
    // 3.读取文件
    int i = 0;
    while( !feof(fp) && flag ){
        i++;
        switch (state) {
            case s0:  // 开始状态
                string().swap(str);  // 每次回到初始状态，字符串都 置空 准备重新接受字符
                buffer = fgetc(fp);  // 获取一个字符
                if ( (char)buffer == '\n' ) {  // 如果是'\n'行数+1
                    line++;
                    row = 1;
                    break;
                } else if ( (char)buffer == ' ' || buffer == 9 ) {  // 如果是' '或者是tab键跳过
                    row++;
                    break;
                } else if ( isLetter((char)buffer) ) {  // 如果是字母跳转s1状态
                    str += (char)buffer;  // 接单词
                    row++;
                    state = s1;
                    break;
                } else if ( isDigit((char)buffer) ) {  // 如果是数字跳转s2状态
                    str += (char)buffer;  // 接数字
                    row++;
                    state = s2;
                    break;
                } else if ( isSingleDelimiter((char)buffer) ) {  // 如果是单字符分界符跳转s3状态
                    str = (char)buffer;  // 接单字符分界符
                    row++;
                    state = s3;
                    break;
                } else if ( (char)buffer == ':' ) {  // 如果是':'跳转s4状态
                    str = (char)buffer;  // 接':'
                    row++;
                    state = s4;
                    break;
                } else if ( (char)buffer == '{' ) {  // 如果是'{'跳转s5状态
                    state = s5;
                    break;
                } else if ( (char)buffer == '.' ) {  // 如果是'.'跳转s6状态
                    str = (char)buffer;  // 接'.'
                    row++;
                    state = s6;
                    break;
                } else if ( (char)buffer == '\'' ) {  // 如果是'单引号'跳转s8状态
                    row++;
                    state = s8;
                } else {  // 错误状态
                    state = s9;
                    break;
                }
                break;
            case s1:  // 保留字状态
                // 接单词
                while ( (buffer = fgetc(fp)) != -1 && (isLetter((char)buffer) || isDigit((char)buffer)) && !feof(fp) ) {
                    str += (char)buffer;  // 接单词
                    row++;                // 列数+1
                }
                // 退出循环一定是因为读到一个非字母或数字的字符，需要回退一个字符
                ungetc(buffer, fp);
                // 判断此标识符是否为非关键字
                if ( !isReserved(str) ) { // 标识符状态
                    state = s7;
                    break;
                } else {
                    // 保留字生成token链入链表
                    token = new Token(line, lowerToUpper(str), upperToLower(str));
                    tokenList.push_back(*token);
                    // 回到初始状态继续读下一个单词
                    state = s0;
                    break;
                }
            case s2:  // 数字状态
//                buffer = fgetc(fp);   // 获取一个字符
//                str += (char)buffer;  // 接数字
//                row++;                // 列数+1
                while ( (buffer = fgetc(fp)) != -1 && !feof(fp) && isDigit((char)buffer) ) {
                    str += (char)buffer;
                    row++;
                }
                // 退出循环一定是因为读到一个非数字的字符，需要回退一个字符
                //fseek(fp,-1,SEEK_CUR);
                ungetc(buffer, fp);
                //str = delLastChar(str);
                //row--;
                // 保留字生成token链入链表
                token = new Token(line, "INTC", str);
                tokenList.push_back(*token);
                // 回到初始状态继续读下一个单词
                state = s0;
                break;
            case s3:  // 完成状态(单字符分界符)
                // 保留字生成token链入链表
                token = new Token(line, separateSingleDelimiter(str), str);
                tokenList.push_back(*token);
                // 回到初始状态继续读下一个单词
                state = s0;
                break;
            case s4:  // 赋值状态(:=)
                // 获取一个字符
                buffer = fgetc(fp);
                str += (char)buffer;
                row++;
                // 判断是否是赋值符号，不是则跳转错误状态
                if ( (char)buffer == '=' ) {
                    // 保留字生成token链入链表
                    token = new Token(line, "ASSIGN", str);
                    tokenList.push_back(*token);
                    // 回到初始状态继续读下一个单词
                    state = s0;
                } else {
                    state = s8;  // 跳转错误状态
                }
                break;
            case s5:  // 注释状态({)
                buffer = fgetc(fp);  // 获取一个字符
                while ( !feof(fp) && (char)buffer != '}' ) {
                    buffer = fgetc(fp);
                }
                state = s0;  // 回到初始状态继续读下一个单词
                break;
            case s6:  // 点状态(..)
                if ( feof(fp) || ( buffer = fgetc(fp) ) != '.' ) {
                    token = new Token(line, "DOT", ".");
                    tokenList.push_back(*token);
                    return tokenList;  // 程序结束
                } else {
                    str += (char)buffer;  // if已经读了一个buffer
                    row++;
                    // 保留字生成token链入链表
                    token = new Token(line, "UNDERANGE", str);
                    tokenList.push_back(*token);
                    // 回到初始状态继续读下一个单词
                    state = s0;
                }
                break;
            case s7:  // 标识符状态
                // 保留字生成token链入链表
                token = new Token(line, "ID", str);
                tokenList.push_back(*token);
                // 回到初始状态继续读下一个单词
                state = s0;
                break;
            case s8:  // 字符状态
                // 接单词
                // 非字母或数字则出错
                if ( (buffer = fgetc(fp)) != -1 && !( isLetter((char)buffer) || isDigit((char)buffer) ) ) {
                    state = s9;
                    cout << "进" << endl;
                    break;
                }
                str += (char)buffer;  // 接单词
                row++;                // 列数+1
                // 保留字生成token链入链表
                token = new Token(line, "CHARC", str);
                tokenList.push_back(*token);
//                while ( (buffer = fgetc(fp)) != -1 && (isLetter((char)buffer) || isDigit((char)buffer)) && !feof(fp) ) {
//                    str += (char)buffer;  // 接单词
//                    row++;                // 列数+1
//                }
                // 判断退出字符不是'单引号'，则出错
                buffer = fgetc(fp);
                if ( (char)buffer != '\'' ) {
                    state = s9;
                    break;
                }
                // 回到初始状态继续读下一个单词
                state = s0;
                break;
            case s9:  // 错误状态
                // 保留字生成token链入链表
                token = new Token(line, "ERROR", str);
                tokenList.push_back(*token);
                // 回到初始状态继续读下一个单词
                state = s0;
                break;
            default:
                break;
        }
    }
    
    
    return tokenList;
}


int main(){
    
    // 1.获取token列表
    list<Token> l = getTokenList();

    // 2.迭代器遍历
    list<Token>::iterator it;
    for( it = l.begin(); it != l.end(); it++ ) {
        cout << "<" << it->lineshow << "," << it->lex << "," << it->sem << ">" << endl;
    }
    
}
