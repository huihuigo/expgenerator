#ifndef MYAPP_H_INCLUDED
#define MYAPP_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#define TRUE 1
#define FALSE 0
typedef int Status;

//操作数结构体定义
typedef struct NumInfo{
    //denominator 分母, numerator 分子
    int numerator;
    int denominator;
} NumInfo;

//表达式树结点定义
typedef struct BiTNode{
    union {
        char opSym;         //运算符
        NumInfo numForm;    //操作数
    } value;

    int flag;   //0:value = opSym   1:value = num    -1:undefined
    struct BiTNode *lchild, *rchild;
} BiTNode, *BiTree;

//表达式基本信息结构体定义
typedef struct {
    BiTree ExpTree;     //表达式树
    int symCount;       //运算符个数
    char* symArr;       //运算符数组

    NumInfo* opNumArr;  //操作数数组
    NumInfo result;     //表达式结果
} ExpInfo, *ExpInfoPtr;


//生成运算符树部分的函数声明
char* chooseSym(int symNum);
BiTree createSymTree(int sym, char* symArr);
BiTNode* createSymNode(int& symArrIndex, char* symArr);


//插入操作数部分的函数声明
void addOpNumInSymTree(int numRange, BiTree symTree, int& flag);
void setNumForSubtraction(int &num1_numerator, int &num1_denominator, int &num2_numerator, int &num2_denominator, int numRange);
void setNumForDivision(int &num1_numerator, int &num1_denominator, int &num2_numerator, int &num2_denominator, int numRange);
void getRandomNum(int &numerator_, int &denominator_, int numRange);
BiTNode* createNumNode(int numerator, int denominator);


//检查表达式是否合法的函数声明，包含计算表达式的函数
Status isLegalExp(BiTree ExpTree, NumInfo& result);
NumInfo getValue(BiTree ExpTree, int&flag, int model);
int findGCD(int a, int b);
int findLCM(int a, int b);
NumInfo simplify(NumInfo numForm);


//检测是否为重复的表达式的函数声明
Status isRepetitive(ExpInfoPtr ExpArr, ExpInfo tempExpInfo, int ExpIndex);
Status ExpTreeCmp(BiTree T1, BiTree T2);
Status symArrCmp(char* arr1, char* arr2, int length);
Status opNumArrCmp(NumInfo* arr1, NumInfo* arr2, int length);
void addInExpArr(ExpInfoPtr ExpArr, ExpInfo tempExpInfo, int ExpIndex);
void getOpNumArr(BiTree ExpTree, NumInfo* OpNumArr, int& OpNumArrIndex);


//题目和答案文件读写函数的声明
Status Priority_com(char c1, char c2);
void writeExp(BiTree T, FILE* file);
void writeAnswer(NumInfo result, FILE* file);

#endif // MYAPP_H_INCLUDED
