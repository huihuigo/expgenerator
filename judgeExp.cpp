#include "Myapp.h"
/**
 * @function    ���ʽ�Ϸ��Լ�⣬���ܲ��ܼ��������ܼ������ķ��ؼ�����
 * @param       ExpTree:    �����ı��ʽ��
                result:     �洢�Ϸ����ʽ������
 *
 * @return      flag:       ��ǰ���ʽ�Ƿ�Ϸ��ı��
 */
Status isLegalExp(BiTree ExpTree, NumInfo& result)
{
    int flag = TRUE;
    result = getValue(ExpTree, flag, 0);
    return flag;
}


/**
 * @function    ����Ϸ����ʽ��ֵ�����㷽ʽ���÷�����ʽ�ļ���
 * @param       ExpTree:    �����ı��ʽ��
                flag:       ʵʱ�����ʽ�Ƿ�Ϸ��ı��ֵ
                model:      ����̨�Ƿ��ӡ������̵ı��ֵ; 0:����ӡ������̣� 1:��ӡ
 *
 * @return      result:     ��ǰ���ʽ�ļ�����
 */
NumInfo getValue(BiTree ExpTree, int&flag, int model)
{
    if(ExpTree->flag == 1)
        return ExpTree->value.numForm;
    else {
        NumInfo opNum1 = getValue(ExpTree->lchild, flag, model);
        NumInfo opNum2 = getValue(ExpTree->rchild, flag, model);
        NumInfo result;

        //ͨ�֣����������������ĸ����С������
        int LCM = findLCM(opNum1.denominator, opNum2.denominator);

        switch(ExpTree->value.opSym)
        {
            case '+' :  result.denominator = LCM;
                        result.numerator = opNum1.numerator*(LCM / opNum1.denominator) + opNum2.numerator*(LCM / opNum2.denominator);
                        break;

            case '-' :  result.denominator = LCM;
                        result.numerator = opNum1.numerator*(LCM / opNum1.denominator) - opNum2.numerator*(LCM / opNum2.denominator);
                        break;

            case '*' :
                        result.denominator = opNum1.denominator * opNum2.denominator;
                        result.numerator = opNum1.numerator * opNum2.numerator;
                        break;

            case '/' :
                        if(opNum2.numerator == 0)
                            flag = FALSE;
                        else {
                            result.denominator = opNum1.denominator * opNum2.numerator;
                            result.numerator = opNum1.numerator * opNum2.denominator;
                        }
                        break;
            default: ;
        }

        if(flag == TRUE)
        {
            result = simplify(result);
            if(model == 1)
                printf("(%d/%d) %c (%d/%d) = %d/%d\n", opNum1.numerator, opNum1.denominator, ExpTree->value.opSym,
                                                    opNum2.numerator, opNum2.denominator, result.numerator, result.denominator);
        }
        else {
            result.numerator = 0;
            result.denominator = 1;
        }
        return result;
    }
}

/**
 * @function    Ѱ�����Լ����շת�����
 * @param       a:  ����1
                b:  ����2
 *
 * @return      ���Լ��
 */
int findGCD(int a, int b)
{
    if(a == 0)
        return b;
    return findGCD(b%a, a);
}


/**
 * @function    Ѱ����С���������������������Լ��
 * @param       a:  ����1
                b:  ����2
 *
 * @return      ��С������
 */
int findLCM(int a, int b)
{
    return (a*b)/findGCD(a, b);
}


/**
 * @function    ������Ӻͷ�ĸ
 * @param       numForm:    һ���÷���/��ĸ��ʾ����
 *
 * @return      afterSim:   �����ķ���/��ĸ��ʾ����
 */
NumInfo simplify(NumInfo numForm)
{
    NumInfo afterSim;
    if(numForm.numerator == 0)
    {
        afterSim.numerator = 0;
        afterSim.denominator = 1;
        return afterSim;
    }

    //Ѱ���������
    int GCD = findGCD(numForm.denominator, abs(numForm.numerator));
    afterSim.numerator = numForm.numerator / GCD;
    afterSim.denominator = numForm.denominator / GCD;
    return afterSim;
}


/**
 * @function    ��⵱ǰ���ɵı��ʽ�Ƿ���ǰ�����Ŀ�ظ�
 * @param       ExpArr:         ���ʽ����
                ExpTree:        ��ǰ���ʽ��
                ExpIndex:       ���ʽ��������һ��Ԫ�ص�����
 *
 * @return      flag:   ��ǰ���ʽ�Ƿ���ǰ����Ŀ�ظ��ı��ֵ  FALSE:���ظ�,    TRUE:�ظ�
 */
Status isRepetitive(ExpInfoPtr ExpArr, ExpInfo tempExpInfo, int ExpIndex)
{
    int flag = FALSE;
    int i;
    int tempExpInfoOpNumCount = tempExpInfo.symCount+1;

    for(i = 1; i < ExpIndex; i++)
    {
        ExpInfo temp = ExpArr[i];
        //1���Ƚϱ��ʽ���������
        if( tempExpInfo.symCount == temp.symCount )
            //2���Ƚϱ��ʽ��������ӷ�ĸ��Ӧ���
            if( tempExpInfo.result.denominator == temp.result.denominator && tempExpInfo.result.numerator == temp.result.numerator)
                    //3���Ƚϱ��ʽ�����������
                    if( symArrCmp(tempExpInfo.symArr, temp.symArr, tempExpInfo.symCount) )
                        //4���Ƚϱ��ʽ�Ĳ���������
                        if( opNumArrCmp(tempExpInfo.opNumArr, temp.opNumArr, tempExpInfoOpNumCount) )
                            //5���Ƚϱ��ʽ���Ľṹ
                            if( ExpTreeCmp(tempExpInfo.ExpTree, temp.ExpTree) )
                            {
                                flag = TRUE;
                                break;
                            }
    }
    return flag;
}


/**
 * @function    �����ʽ1�����ṹ�ͱ��ʽ2�Ƿ����ظ���ϵ
 * @param       T1:     ���ʽ��1
                T2:     ���ʽ��2
 *
 * @return      flag:   ���ʽ1�ͱ��ʽ2�Ƿ����ظ���ϵ�ı��ֵ, FLASE:���ظ�,    TRUE:�ظ�
 */
Status ExpTreeCmp(BiTree T1, BiTree T2)
{
    if(T1->flag ==  T2->flag){ //���������ͬ
        if(T1->flag == 1){ //���ǲ��������
            if(T1->value.numForm.numerator==0 && T2->value.numForm.numerator==0)
                return TRUE;
            else if(T1->value.numForm.numerator == T2->value.numForm.numerator
               && T1->value.numForm.denominator == T2->value.numForm.denominator)
                return TRUE;
            else
                return FALSE;
        }
        else if(T1->flag == 0){ //���ǲ��������
            if(T1->value.opSym != T2->value.opSym)
                return FALSE;
            // ��������ͬ�ŵݹ�Ƚ�
            else{
                if(T1->value.opSym == '*' || T1->value.opSym == '+'){
                    int flag1, flag2;
                    flag1 = ExpTreeCmp(T1->lchild, T2->lchild) && ExpTreeCmp(T1->rchild, T2->rchild); //�Ƚ���������ҵĽ��
                    flag2 = ExpTreeCmp(T1->lchild, T2->rchild) && ExpTreeCmp(T1->rchild, T2->lchild);  // �Ƚ����Һ�����Ľ��
                    return flag1 || flag2; //����ĳһ����ͬ����ͬ
                }
                else
                    return ExpTreeCmp(T1->lchild, T2->lchild) && ExpTreeCmp(T1->rchild, T2->rchild);
            }
        }
    }
    else
        return FALSE;

}

/**
 * @function    ������������1������2�Ƿ����ظ���ϵ
 * @param       arr1:     ���������1
                arr2:     ���������2
                length:   ���鳤��
 *
 * @return      flag:   ���������1������2�Ƿ����ظ���ϵ�ı��ֵ, FALSE:���ظ�,    TRUE:�ظ�
 */
Status symArrCmp(char* arr1, char* arr2, int length)
{
    //���������������Ƚϣ����ø����������ķ�ʽ
    int* flagArr = (int*) malloc(length * sizeof(int));
    if(!flagArr)
        return FALSE;

    int i, j;
    for(i = 0; i < length; i++)
        flagArr[i] = 0;

    for(i = 0; i < length; i++)
    {
        for(j = 0; j < length; j++)
            if(arr1[i] == arr2[j] && flagArr[j] == 0 )
            {
                flagArr[j] = 1;
                break;
            }
    }

    for(i = 0; i < length; i++)
        if(flagArr[i] != 1)
            return FALSE;

    return TRUE;
}

/**
 * @function    ������������1������2�Ƿ����ظ���ϵ
 * @param       arr1:     ����������1
                arr2:     ����������2
                length:   ���鳤��
 *
 * @return      flag:     ����������1������2�Ƿ����ظ���ϵ�ı��ֵ, FALSE:���ظ�,    TRUE:�ظ�
 */
Status opNumArrCmp(NumInfo* arr1, NumInfo* arr2, int length)
{
    //���������������Ƚϣ����ø����������ķ�ʽ
    int* flagArr = (int*) malloc(length * sizeof(int));
    if(!flagArr)
        return FALSE;

    int i, j;
    for(i = 0; i < length; i++)
        flagArr[i] = 0;

    for(i = 0; i < length; i++)
    {
        for(j = 0; j < length; j++)
            if(arr1[i].numerator == arr2[j].numerator &&
                    arr1[i].denominator == arr2[j].denominator && flagArr[j] == 0)
            {
                flagArr[j] = 1;
                break;
            }
    }

    for(i = 0; i < length; i++)
        if(flagArr[i] != 1)
            return FALSE;

    return TRUE;
}

/**
 * @function    �ѵ�ǰ���ʽ�Ļ�����Ϣ������ʽ������
 * @param       ExpArr:         ���ʽ����
                ExpInfo:        ��ǰ���ʽ�Ļ�����Ϣ

 * @return      void
 */
void addInExpArr(ExpInfoPtr ExpArr, ExpInfo tempExpInfo, int ExpIndex)
{
    ExpArr[ExpIndex].ExpTree = tempExpInfo.ExpTree;
    ExpArr[ExpIndex].symCount = tempExpInfo.symCount;
    ExpArr[ExpIndex].symArr = tempExpInfo.symArr;
    ExpArr[ExpIndex].opNumArr = tempExpInfo.opNumArr;
    ExpArr[ExpIndex].result = tempExpInfo.result;
}


/**
 * @function    ��ȡ��ǰ���ʽ�Ĳ�����������¼��һ��������
 * @param       ExpTree:        ���ʽ��
                opNumArr:       ����������ָ��
                opNumArrIndex:  ��������������
 *
 * @return      void
 */
void getOpNumArr(BiTree ExpTree, NumInfo* opNumArr, int& opNumArrIndex)
{
    if(ExpTree->flag == 1)
        opNumArr[opNumArrIndex++] = ExpTree->value.numForm;

    else{
        getOpNumArr(ExpTree->lchild, opNumArr, opNumArrIndex);
        getOpNumArr(ExpTree->rchild, opNumArr, opNumArrIndex);
    }
}
