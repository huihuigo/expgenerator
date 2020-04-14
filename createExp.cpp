#include "Myapp.h"
/**
 * @function    ѡ��ǰ���ɵı��ʽ�е������
 * @param       symNum:     ���������
 *
 * @return      symArr:     ѡ������������
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
 * @function    ���ݴ�����������������������������
 * @param       symArrIndex:    �������������;
                symArr:         ѡ������������
 *
 * @return      nodePtr:        �´������������ָ��
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
 * @function    ���ݴ����������������һ����
 * @param       symCount:     ���������;
                symArr:ѡ������������
 *
 * @return      T:ֻ����������������ָ��
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
            //����������������������������νṹ
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
 * @function    Ϊ��������������ӷ���Ҫ��Ĳ�����
 * @param       numRange:   �������ķ�Χ
                symTree:    �������
                flag:       ��ǰ�����Ƿ�ɹ��ı��ֵ��Ĭ��ΪTRUE
 *
 * @return      void
 */
void addOpNumInSymTree(int numRange, BiTree symTree, int& flag)
{
    //����ָ����Һ���ָ�붼Ϊ��
    if(!symTree->lchild && !symTree->rchild)
    {
        //��Χ����1��������ɲ�����
        if(numRange > 1)
        {
            int num1_numerator, num1_denominator;
            int num2_numerator, num2_denominator;

            //����������-��/����������Ҫ��
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
            //��Χ����1��������ֻ��Ϊ0
            symTree->lchild = createNumNode(0, 1);
            symTree->rchild = createNumNode(0, 1);
        }

        if(!symTree->lchild || !symTree->rchild)
            flag = FALSE;

        return;
    }

    //����ָ����Һ���ָ�붼��Ϊ��
    else if( symTree->lchild && symTree->rchild)
    {
        addOpNumInSymTree(numRange, symTree->lchild, flag);
        addOpNumInSymTree(numRange, symTree->rchild, flag);
    }

    //����ָ����Һ���ָ������һ��Ϊ��
    else
    {
        int _numerator, _denominator;
        getRandomNum(_numerator, _denominator, numRange);
        BiTNode* newNumNode = createNumNode(_numerator, _denominator);

        if(!newNumNode)
            flag = FALSE;

        //����ָ��Ϊ�գ���������ӣ���������Һ���
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
 * @function     ���ݴ������ֵ�������������
 * @param        numerator:     �������ķ���
                 denominator:   �������ķ�ĸ
 *
 * @return       nodePtr:       ���������ָ��
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
 * @function    �������numRange��Χ�ڵĲ�����
 * @param       numRange:       �������ķ�Χ
                numerator:      �������ķ��Ӳ���
                denominator:    �������ķ�ĸ����
 *
 * @return      void
 */
void getRandomNum(int &numerator_, int &denominator_, int numRange)
{
    //denominator ��ĸ, numerator ����
    if(numRange != 1)
    {
        denominator_ = rand() % (numRange-1) + 1; // [1,numRange)
        numerator_ = rand() % (numRange*denominator_); //[0, numRange*denominator)
    }
    else {
        //��ΧnumRange == 1�� �������ķ���ֻ��Ϊ0�� ��ĸ����Ϊ1
        denominator_ = 1;
        numerator_ = 0;
    }
}

/**
 * @function    Ϊ_����_������������Ҫ��Ĳ�����
 * @param       num1_numerator:     ������1�ķ���
                num1_denominator:   ������1�ķ�ĸ
                num2_numerator:     ������2�ķ���
                num2_denominator:   ������2�ķ�ĸ
                numRange:           �������ķ�Χ
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
 * @function    Ϊ_����_������������Ҫ��Ĳ�����
 * @param       num1_numerator:     ������1�ķ���
                num1_denominator:   ������1�ķ�ĸ
                num2_numerator:     ������2�ķ���
                num2_denominator:   ������2�ķ�ĸ
                numRange:           �������ķ�Χ
 *
 * @return      void
 */
void setNumForDivision(int &num1_numerator, int &num1_denominator, int &num2_numerator, int &num2_denominator, int numRange)
{
    double num1, num2;  //num1 / num2
    do{
        num2_denominator = rand() % (numRange-1) + 1;  // [1,numRange)
        num2_numerator = rand() % (numRange*num2_denominator - 1) + 1; // [1,numRange*num2_denominator) ���������������Ϊ0������Ҫ����
        num2 = (double)num2_numerator / (double)num2_denominator;

        getRandomNum(num1_numerator, num1_denominator, numRange);
        num1 = (double)num1_numerator / (double)num1_denominator;
    }while(num1 > num2);
}

