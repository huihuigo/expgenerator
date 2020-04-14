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

//�������ṹ�嶨��
typedef struct NumInfo{
    //denominator ��ĸ, numerator ����
    int numerator;
    int denominator;
} NumInfo;

//���ʽ����㶨��
typedef struct BiTNode{
    union {
        char opSym;         //�����
        NumInfo numForm;    //������
    } value;

    int flag;   //0:value = opSym   1:value = num    -1:undefined
    struct BiTNode *lchild, *rchild;
} BiTNode, *BiTree;

//���ʽ������Ϣ�ṹ�嶨��
typedef struct {
    BiTree ExpTree;     //���ʽ��
    int symCount;       //���������
    char* symArr;       //���������

    NumInfo* opNumArr;  //����������
    NumInfo result;     //���ʽ���
} ExpInfo, *ExpInfoPtr;


//��������������ֵĺ�������
char* chooseSym(int symNum);
BiTree createSymTree(int sym, char* symArr);
BiTNode* createSymNode(int& symArrIndex, char* symArr);


//������������ֵĺ�������
void addOpNumInSymTree(int numRange, BiTree symTree, int& flag);
void setNumForSubtraction(int &num1_numerator, int &num1_denominator, int &num2_numerator, int &num2_denominator, int numRange);
void setNumForDivision(int &num1_numerator, int &num1_denominator, int &num2_numerator, int &num2_denominator, int numRange);
void getRandomNum(int &numerator_, int &denominator_, int numRange);
BiTNode* createNumNode(int numerator, int denominator);


//�����ʽ�Ƿ�Ϸ��ĺ�������������������ʽ�ĺ���
Status isLegalExp(BiTree ExpTree, NumInfo& result);
NumInfo getValue(BiTree ExpTree, int&flag, int model);
int findGCD(int a, int b);
int findLCM(int a, int b);
NumInfo simplify(NumInfo numForm);


//����Ƿ�Ϊ�ظ��ı��ʽ�ĺ�������
Status isRepetitive(ExpInfoPtr ExpArr, ExpInfo tempExpInfo, int ExpIndex);
Status ExpTreeCmp(BiTree T1, BiTree T2);
Status symArrCmp(char* arr1, char* arr2, int length);
Status opNumArrCmp(NumInfo* arr1, NumInfo* arr2, int length);
void addInExpArr(ExpInfoPtr ExpArr, ExpInfo tempExpInfo, int ExpIndex);
void getOpNumArr(BiTree ExpTree, NumInfo* OpNumArr, int& OpNumArrIndex);


//��Ŀ�ʹ��ļ���д����������
Status Priority_com(char c1, char c2);
void writeExp(BiTree T, FILE* file);
void writeAnswer(NumInfo result, FILE* file);

#endif // MYAPP_H_INCLUDED
