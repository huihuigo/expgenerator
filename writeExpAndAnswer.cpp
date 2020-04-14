#include "Myapp.h"
/**
 * @function    比较运算符优先级
 * @param       运算符c1, c2
 *
 * @return      Bool值
 */
Status Priority_com(char c1,char c2){
	if((c1 == '+' || c1 == '-' || c1 == '*' || c1 == '/') && (c2 == '+' || c2 == '-' || c2 == '*' || c2 == '/'))
    {
		if(c1 == '*' || c1 == '/')
		{
			if(c2 == '+' || c2 == '-')
				return TRUE;
			else
				return FALSE;
		}
		else
			return FALSE;
	}
	else
		return FALSE;
}


/**
 * @function    将当前表达式写入题目文件
 * @param       T:      表达式树
                file:   题目文件指针
 *
 * @return      void
 */
void writeExp(BiTree T, FILE* file)
{
    if(!T)
        return;

    if(T->lchild && T->lchild->flag == 0){ //如果根节点与左子树的根节点是运算符
        if(Priority_com(T->value.opSym, T->lchild->value.opSym)){
            printf("( ");
            fprintf(file,"( ");
            writeExp(T->lchild, file);
            printf(") ");
            fprintf(file,") ");
        }
        else
            writeExp(T->lchild, file);
    }
    else
        writeExp(T->lchild, file);


    if(T->flag == 0){
		printf("%c  ",T->value.opSym);
		fprintf(file,"%c  ",T->value.opSym);
    }
	else {
        int numerator = T->value.numForm.numerator;
        int denominator = T->value.numForm.denominator;

        if(numerator % denominator == 0){ //自然数
            printf("%d  ",numerator/denominator);
            fprintf(file,"%d  ",numerator/denominator);
        }
        else{
            if(numerator < denominator){ //真分数
                printf("%d/%d  ",numerator,denominator);
                fprintf(file,"%d/%d  ",numerator,denominator);
            }
            else{ //带分数
                printf("%d\'%d/%d  ",numerator/denominator,numerator%denominator,denominator);
                fprintf(file,"%d\'%d/%d  ",numerator/denominator,numerator%denominator,denominator);
            }
        }
    }

    if(T->rchild && T->rchild->flag == 0){ //如果根节点与右子树的根节点是运算符
            printf("( ");
            fprintf(file,"(  ");
            writeExp(T->rchild, file);
            printf(") ");
            fprintf(file,")  ");
    }
    else
        writeExp(T->rchild, file);
}

/**
 * @function    将当前表达式写入题目文件
 * @param       T:      表达式树
                file:   题目文件指针
 *
 * @return      void
 */
void writeAnswer(NumInfo result, FILE* answerFile)
{
    int numerator = result.numerator;
    int denominator = result.denominator;
    if(numerator % denominator == 0){ //自然数
        printf("%d\n",numerator/denominator);
        fprintf(answerFile,"%d\n",numerator/denominator);
    }
    else{
        if(abs(numerator) < abs(denominator)){ //真分数
            printf("%d/%d\n",numerator,denominator);
            fprintf(answerFile,"%d/%d\n",numerator,denominator);
        }
        else{ //带分数
            printf("%d\'%d/%d\n",numerator/denominator,abs(numerator)%denominator,denominator);
            fprintf(answerFile,"%d\'%d/%d\n",numerator/denominator,abs(numerator)%denominator,denominator);
        }
    }
}
