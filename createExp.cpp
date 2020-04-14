#include "Myapp.h"
/**
 * @function    选择当前生成的表达式中的运算符
 * @param       symNum:     运算符数量
 *
 * @return      symArr:     选择的运算符数组
 */
char* chooseSym(int symNum)
{
    char* symArr = NULL;
    char sym[4] = {'+', '-', '*', '/'};
    symArr = (char*)malloc( symNum*sizeof(char));
    if(!symArr)
        return NULL;

    int i;
    for(i = 0; i < symNum; i++)
        symArr[i] = sym[rand()%4];

    return symArr;
}

/**
 * @function    根据传入的运算符数组索引创建运算符结点
 * @param       symArrIndex:    运算符数组索引;
                symArr:         选择的运算符数组
 *
 * @return      nodePtr:        新创建运算符结点的指针
 */
BiTNode* createSymNode(int& symArrIndex, char* symArr)
{
    BiTNode* nodePtr = NULL;
    nodePtr = (BiTNode*) malloc(sizeof(BiTNode));

    if(!nodePtr)
        return NULL;

    nodePtr->flag = 0;
    nodePtr->value.opSym = symArr[symArrIndex++];
    nodePtr->rchild = nodePtr->lchild = NULL;

    return nodePtr;
}

/**
 * @function    根据传入的运算符数组组成一棵树
 * @param       symCount:     运算符数量;
                symArr:选择的运算符数组
 *
 * @return      T:只有运算符的树根结点指针
 */
BiTree createSymTree(int symCount, char* symArr)
{
    BiTree root = NULL;
    int symArrIndex = 0, i;
    root = createSymNode(symArrIndex, symArr);
    if(!root)
        return NULL;

    for(i = 0; i < symCount-1; i++)
    {
        BiTNode* newNodePtr = createSymNode(symArrIndex, symArr);
        BiTNode* temp = root;

        while(1)
        {
            //根据随机数，随机生成运算符的树形结构
            if(rand()%2)
            {
                if(!temp->lchild)
                {
                    temp->lchild = newNodePtr;
                    break;
                }
                else temp = temp->lchild;
            }
            else {
                if(!temp->rchild)
                {
                    temp->rchild = newNodePtr;
                    break;
                }
                else temp = temp->rchild;
            }
        }
    }
    return root;
}


/**
 * @function    为传入的运算符树添加符合要求的操作数
 * @param       numRange:   操作数的范围
                symTree:    运算符树
                flag:       当前步骤是否成功的标记值，默认为TRUE
 *
 * @return      void
 */
void addOpNumInSymTree(int numRange, BiTree symTree, int& flag)
{
    //左孩子指针和右孩子指针都为空
    if(!symTree->lchild && !symTree->rchild)
    {
        //范围大于1，随机生成操作数
        if(numRange > 1)
        {
            int num1_numerator, num1_denominator;
            int num2_numerator, num2_denominator;

            //如果运算符是-和/，操作数有要求
            if( symTree->value.opSym == '-' )
                setNumForSubtraction(num1_numerator, num1_denominator, num2_numerator, num2_denominator, numRange);

            else if( symTree->value.opSym == '/' )
                setNumForDivision(num1_numerator, num1_denominator, num2_numerator, num2_denominator, numRange);

            else {
                getRandomNum(num1_numerator, num1_denominator, numRange);
                getRandomNum(num2_numerator, num2_denominator, numRange);
            }

            symTree->lchild = createNumNode(num1_numerator, num1_denominator);
            symTree->rchild = createNumNode(num2_numerator, num2_denominator);
        }
        else {
            //范围等于1，操作数只能为0
            symTree->lchild = createNumNode(0, 1);
            symTree->rchild = createNumNode(0, 1);
        }

        if(!symTree->lchild || !symTree->rchild)
            flag = FALSE;

        return;
    }

    //左孩子指针和右孩子指针都不为空
    else if( symTree->lchild && symTree->rchild)
    {
        addOpNumInSymTree(numRange, symTree->lchild, flag);
        addOpNumInSymTree(numRange, symTree->rchild, flag);
    }

    //左孩子指针和右孩子指针其中一个为空
    else
    {
        int _numerator, _denominator;
        getRandomNum(_numerator, _denominator, numRange);
        BiTNode* newNumNode = createNumNode(_numerator, _denominator);

        if(!newNumNode)
            flag = FALSE;

        //左孩子指针为空，则插入左孩子，否则插入右孩子
        if(!symTree->lchild){
            symTree->lchild = newNumNode;
            addOpNumInSymTree(numRange, symTree->rchild, flag);
        }
        else{
            symTree->rchild = newNumNode;
            addOpNumInSymTree(numRange, symTree->lchild, flag);
        }
    }
}

/**
 * @function     根据传入的数值创建运算数结点
 * @param        numerator:     操作数的分子
                 denominator:   操作数的分母
 *
 * @return       nodePtr:       操作数结点指针
*/
BiTNode* createNumNode(int numerator, int denominator)
{
    BiTNode* nodePtr = NULL;
    nodePtr = (BiTree)malloc(sizeof(BiTNode));
    if(!nodePtr)
        return NULL;

    nodePtr->flag = 1;

    nodePtr->value.numForm.numerator = numerator;
    nodePtr->value.numForm.denominator = denominator;

    nodePtr->lchild = nodePtr->rchild = NULL;
    return nodePtr;
}

/**
 * @function    随机生成numRange范围内的操作数
 * @param       numRange:       操作数的范围
                numerator:      操作数的分子部分
                denominator:    操作数的分母部分
 *
 * @return      void
 */
void getRandomNum(int &numerator_, int &denominator_, int numRange)
{
    //denominator 分母, numerator 分子
    if(numRange != 1)
    {
        denominator_ = rand() % (numRange-1) + 1; // [1,numRange)
        numerator_ = rand() % (numRange*denominator_); //[0, numRange*denominator)
    }
    else {
        //范围numRange == 1， 操作数的分子只能为0， 分母设置为1
        denominator_ = 1;
        numerator_ = 0;
    }
}

/**
 * @function    为_减法_设置两个符合要求的操作数
 * @param       num1_numerator:     操作数1的分子
                num1_denominator:   操作数1的分母
                num2_numerator:     操作数2的分子
                num2_denominator:   操作数2的分母
                numRange:           操作数的范围
 *
 * @return      void
 */
void setNumForSubtraction(int &num1_numerator, int &num1_denominator, int &num2_numerator, int &num2_denominator, int numRange)
{
    double num1, num2;  //num1 - num2
    do{
        getRandomNum(num2_numerator, num2_denominator, numRange);
        num2 = (double)num2_numerator / (double)num2_denominator;

        getRandomNum(num1_numerator, num1_denominator, numRange);
        num1 = (double)num1_numerator / (double)num1_denominator;

    }while(num1 < num2);
}

/**
 * @function    为_除法_设置两个符合要求的操作数
 * @param       num1_numerator:     操作数1的分子
                num1_denominator:   操作数1的分母
                num2_numerator:     操作数2的分子
                num2_denominator:   操作数2的分母
                numRange:           操作数的范围
 *
 * @return      void
 */
void setNumForDivision(int &num1_numerator, int &num1_denominator, int &num2_numerator, int &num2_denominator, int numRange)
{
    double num1, num2;  //num1 / num2
    do{
        num2_denominator = rand() % (numRange-1) + 1;  // [1,numRange)
        num2_numerator = rand() % (numRange*num2_denominator - 1) + 1; // [1,numRange*num2_denominator) 除号情况除数不能为0这里需要处理
        num2 = (double)num2_numerator / (double)num2_denominator;

        getRandomNum(num1_numerator, num1_denominator, numRange);
        num1 = (double)num1_numerator / (double)num1_denominator;
    }while(num1 > num2);
}

