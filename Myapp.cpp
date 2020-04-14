#include "Myapp.h"
#include "createExp.cpp"
#include "judgeExp.cpp"
#include "writeExpAndAnswer.cpp"

Status isNumString(char* str)
{
    int i;
    for(i = 0; i < strlen(str); i++)
        if(str[i] < '0' || str[i] > '9')
        return FALSE;
    return TRUE;
}

int main(int argc, char *argv[])
{
    int questionCount = 0, numRange = 0;  //默认Myapp.exe -n 题目数 -r 范围值，出错处理后面再处理

    int opt;
    while((opt=getopt(argc,argv,"n:r:"))!=-1)
    {
        switch(opt)
        {
            case 'n':   if(isNumString(optarg))   questionCount = atoi(optarg);
                        break;
            case 'r':   if(isNumString(optarg))   numRange = atoi(optarg);
                        break;
            default:    ;
        }
    }
    if( questionCount == 0 || numRange == 0 )
    {
        printf("命令行参数输入错误...\n");
        printf("输入格式为Myapp.exe -n 题目数(整数) -r 范围值(整数)\n");
        exit(1);
    }
    srand((int)time(0));


    FILE* questionFile,  *answerFile;
    questionFile = fopen("question.txt", "w");
    answerFile = fopen("answer.txt", "w");
    if(!questionFile || !answerFile)
    {
        printf("文件打开失败...按任意键退出\n");    getchar();
        exit(1);
    }

    ExpInfoPtr ExpArr = NULL;
    ExpArr = (ExpInfoPtr)malloc( (questionCount+1) * sizeof(ExpInfo));
    if(!ExpArr)
    {
        printf("空间申请失败...按任意键退出\n");    getchar();
        exit(1);
    }

    int ExpIndex = 1;
    int repetiveTime = 0;
    int runtimeFlag = TRUE;

    while(ExpIndex <= questionCount)
    {
        //1、先生成运算符数量，再随机选择运算符
        int symCount = rand()%3 +1;   //一条表达式最多选择3个运算符
        char* symArr = chooseSym(symCount);
        if(!symArr)
        {
            runtimeFlag = FALSE;
            break;
        }

        //2、运算符搭成树
        BiTree symTree = createSymTree(symCount, symArr);
        if(!symTree)
        {
            runtimeFlag = FALSE;
            break;
        }

        //3、在运算符树的叶子结点加操作数
        int addFlag = TRUE;
        addOpNumInSymTree(numRange, symTree, addFlag);
        if(addFlag == FALSE)
        {
            runtimeFlag = FALSE;
            break;
        }

        //4、判断生成的表达式是否合法，主要检测除数是否为0，并进行文件读写
        BiTree ExpTree = symTree;
        NumInfo result;

        if( isLegalExp(ExpTree, result) )
        {
            //5、表达式合法则检查重复性
            //5.1   先获取当前表达式的基本信息
            ExpInfo tempExpInfo;
            tempExpInfo.ExpTree = ExpTree;
            tempExpInfo.symCount = symCount;
            tempExpInfo.symArr = symArr;

            int opNumCount = symCount+1;
            NumInfo* opNumArr = (NumInfo*)malloc( opNumCount*sizeof(NumInfo));
            if(!opNumArr)
            {
                runtimeFlag = FALSE;
                break;
            }

            int opNumArrIndex = 0;
            getOpNumArr(ExpTree, opNumArr, opNumArrIndex);

            tempExpInfo.opNumArr = opNumArr;
            tempExpInfo.result = result;


            if(ExpIndex == 1)    //第二条题目开始需要检测重复性
                addInExpArr(ExpArr, tempExpInfo, ExpIndex);
            else{

                if( !isRepetitive(ExpArr, tempExpInfo, ExpIndex) )
                {
                    //5.2.1 合法的话，则加入到表达式数组中
                    addInExpArr(ExpArr, tempExpInfo, ExpIndex);
                    repetiveTime = 0;
                }
                else {
                        //5.2.2 不合法的话，生成重复次数更新，释放空间
                        repetiveTime++;
                        if(repetiveTime >= ExpIndex)
                            break;
                        free(opNumArr);
                        continue;
                }
            }

            //6、在控制台、题目文件、答案文件打印题目序号
            printf("%d.\t", ExpIndex);
            fprintf(questionFile,"%d.\t", ExpIndex);
            fprintf(answerFile, "%d.\t", ExpIndex);

            //6.1 表达式写入题目文件
            writeExp(ExpTree, questionFile);
            printf("= ");
            fprintf(questionFile, "= \n");

            //6.2 写入答案文件
            writeAnswer(result, answerFile);

            //6.3 控制台展示运算过程
            //int flag = TRUE;
            //getValue(ExpTree, flag, 1);
            //printf("\n");

            //7、 更新题号
            ExpIndex++;
        }
    }

    //8、 生成完毕，关闭文件，释放表达式数组空间
    fclose(questionFile);
    fclose(answerFile);
    free(ExpArr);

    if(runtimeFlag == FALSE)
    {
        printf("空间申请失败...按任意键退出\n");    getchar();
        exit(1);
    }
    printf("\n当前已生成 %d 道，操作数范围为 %d 的四则运算题目.\n", ExpIndex -1, numRange);
    printf("题目保存在当前目录下的question.txt文件中\n答案保存在当前目录下的answer.txt文件中\n");
    return 0;
}
