#include "Myapp.h"
/**
 * @function    表达式合法性检测，即能不能计算结果，能计算结果的返回计算结果
 * @param       ExpTree:    完整的表达式树
                result:     存储合法表达式计算结果
 *
 * @return      flag:       当前表达式是否合法的标记
 */
Status isLegalExp(BiTree ExpTree, NumInfo& result)
{
    int flag = TRUE;
    result = getValue(ExpTree, flag, 0);
    return flag;
}


/**
 * @function    计算合法表达式的值，计算方式采用分数形式的计算
 * @param       ExpTree:    完整的表达式树
                flag:       实时检测表达式是否合法的标记值
                model:      控制台是否打印计算过程的标记值; 0:不打印计算过程， 1:打印
 *
 * @return      result:     当前表达式的计算结果
 */
NumInfo getValue(BiTree ExpTree, int&flag, int model)
{
    if(ExpTree->flag == 1)
        return ExpTree->value.numForm;
    else {
        NumInfo opNum1 = getValue(ExpTree->lchild, flag, model);
        NumInfo opNum2 = getValue(ExpTree->rchild, flag, model);
        NumInfo result;

        //通分，获得两个操作数分母的最小公倍数
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
 * @function    寻找最大公约数，辗转相除法
 * @param       a:  参数1
                b:  参数2
 *
 * @return      最大公约数
 */
int findGCD(int a, int b)
{
    if(a == 0)
        return b;
    return findGCD(b%a, a);
}


/**
 * @function    寻找最小公倍数，两数积除以最大公约数
 * @param       a:  参数1
                b:  参数2
 *
 * @return      最小公倍数
 */
int findLCM(int a, int b)
{
    return (a*b)/findGCD(a, b);
}


/**
 * @function    化简分子和分母
 * @param       numForm:    一个用分子/分母表示的数
 *
 * @return      afterSim:   化简后的分子/分母表示的数
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

    //寻找最大公因数
    int GCD = findGCD(numForm.denominator, abs(numForm.numerator));
    afterSim.numerator = numForm.numerator / GCD;
    afterSim.denominator = numForm.denominator / GCD;
    return afterSim;
}


/**
 * @function    检测当前生成的表达式是否与前面的题目重复
 * @param       ExpArr:         表达式数组
                ExpTree:        当前表达式树
                ExpIndex:       表达式数组的最后一个元素的索引
 *
 * @return      flag:   当前表达式是否与前面题目重复的标记值  FALSE:不重复,    TRUE:重复
 */
Status isRepetitive(ExpInfoPtr ExpArr, ExpInfo tempExpInfo, int ExpIndex)
{
    int flag = FALSE;
    int i;
    int tempExpInfoOpNumCount = tempExpInfo.symCount+1;

    for(i = 1; i < ExpIndex; i++)
    {
        ExpInfo temp = ExpArr[i];
        //1、比较表达式运算符数量
        if( tempExpInfo.symCount == temp.symCount )
            //2、比较表达式结果，分子分母对应相等
            if( tempExpInfo.result.denominator == temp.result.denominator && tempExpInfo.result.numerator == temp.result.numerator)
                    //3、比较表达式的运算符数组
                    if( symArrCmp(tempExpInfo.symArr, temp.symArr, tempExpInfo.symCount) )
                        //4、比较表达式的操作数数组
                        if( opNumArrCmp(tempExpInfo.opNumArr, temp.opNumArr, tempExpInfoOpNumCount) )
                            //5、比较表达式树的结构
                            if( ExpTreeCmp(tempExpInfo.ExpTree, temp.ExpTree) )
                            {
                                flag = TRUE;
                                break;
                            }
    }
    return flag;
}


/**
 * @function    检测表达式1的树结构和表达式2是否有重复关系
 * @param       T1:     表达式树1
                T2:     表达式树2
 *
 * @return      flag:   表达式1和表达式2是否有重复关系的标记值, FLASE:不重复,    TRUE:重复
 */
Status ExpTreeCmp(BiTree T1, BiTree T2)
{
    if(T1->flag ==  T2->flag){ //结点类型相同
        if(T1->flag == 1){ //都是操作数结点
            if(T1->value.numForm.numerator==0 && T2->value.numForm.numerator==0)
                return TRUE;
            else if(T1->value.numForm.numerator == T2->value.numForm.numerator
               && T1->value.numForm.denominator == T2->value.numForm.denominator)
                return TRUE;
            else
                return FALSE;
        }
        else if(T1->flag == 0){ //都是操作符结点
            if(T1->value.opSym != T2->value.opSym)
                return FALSE;
            // 操作符相同才递归比较
            else{
                if(T1->value.opSym == '*' || T1->value.opSym == '+'){
                    int flag1, flag2;
                    flag1 = ExpTreeCmp(T1->lchild, T2->lchild) && ExpTreeCmp(T1->rchild, T2->rchild); //比较左左和右右的结果
                    flag2 = ExpTreeCmp(T1->lchild, T2->rchild) && ExpTreeCmp(T1->rchild, T2->lchild);  // 比较左右和右左的结果
                    return flag1 || flag2; //其中某一种相同则相同
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
 * @function    检测运算符数组1和数组2是否有重复关系
 * @param       arr1:     运算符数组1
                arr2:     运算符数组2
                length:   数组长度
 *
 * @return      flag:   运算符数组1和数组2是否有重复关系的标记值, FALSE:不重复,    TRUE:重复
 */
Status symArrCmp(char* arr1, char* arr2, int length)
{
    //由于是乱序的数组比较，采用辅助标记数组的方式
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
 * @function    检测操作数数组1和数组2是否有重复关系
 * @param       arr1:     操作数数组1
                arr2:     操作数数组2
                length:   数组长度
 *
 * @return      flag:     操作数数组1和数组2是否有重复关系的标记值, FALSE:不重复,    TRUE:重复
 */
Status opNumArrCmp(NumInfo* arr1, NumInfo* arr2, int length)
{
    //由于是乱序的数组比较，采用辅助标记数组的方式
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
 * @function    把当前表达式的基本信息存入表达式数组中
 * @param       ExpArr:         表达式数组
                ExpInfo:        当前表达式的基本信息

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
 * @function    获取当前表达式的操作数，并记录在一个数组中
 * @param       ExpTree:        表达式树
                opNumArr:       操作数数组指针
                opNumArrIndex:  操作数数组索引
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
