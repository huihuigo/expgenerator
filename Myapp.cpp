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
    int questionCount = 0, numRange = 0;  //Ĭ��Myapp.exe -n ��Ŀ�� -r ��Χֵ������������ٴ���

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
        printf("�����в����������...\n");
        printf("�����ʽΪMyapp.exe -n ��Ŀ��(����) -r ��Χֵ(����)\n");
        exit(1);
    }
    srand((int)time(0));


    FILE* questionFile,  *answerFile;
    questionFile = fopen("question.txt", "w");
    answerFile = fopen("answer.txt", "w");
    if(!questionFile || !answerFile)
    {
        printf("�ļ���ʧ��...��������˳�\n");    getchar();
        exit(1);
    }

    ExpInfoPtr ExpArr = NULL;
    ExpArr = (ExpInfoPtr)malloc( (questionCount+1) * sizeof(ExpInfo));
    if(!ExpArr)
    {
        printf("�ռ�����ʧ��...��������˳�\n");    getchar();
        exit(1);
    }

    int ExpIndex = 1;
    int repetiveTime = 0;
    int runtimeFlag = TRUE;

    while(ExpIndex <= questionCount)
    {
        //1������������������������ѡ�������
        int symCount = rand()%3 +1;   //һ�����ʽ���ѡ��3�������
        char* symArr = chooseSym(symCount);
        if(!symArr)
        {
            runtimeFlag = FALSE;
            break;
        }

        //2������������
        BiTree symTree = createSymTree(symCount, symArr);
        if(!symTree)
        {
            runtimeFlag = FALSE;
            break;
        }

        //3�������������Ҷ�ӽ��Ӳ�����
        int addFlag = TRUE;
        addOpNumInSymTree(numRange, symTree, addFlag);
        if(addFlag == FALSE)
        {
            runtimeFlag = FALSE;
            break;
        }

        //4���ж����ɵı��ʽ�Ƿ�Ϸ�����Ҫ�������Ƿ�Ϊ0���������ļ���д
        BiTree ExpTree = symTree;
        NumInfo result;

        if( isLegalExp(ExpTree, result) )
        {
            //5�����ʽ�Ϸ������ظ���
            //5.1   �Ȼ�ȡ��ǰ���ʽ�Ļ�����Ϣ
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


            if(ExpIndex == 1)    //�ڶ�����Ŀ��ʼ��Ҫ����ظ���
                addInExpArr(ExpArr, tempExpInfo, ExpIndex);
            else{

                if( !isRepetitive(ExpArr, tempExpInfo, ExpIndex) )
                {
                    //5.2.1 �Ϸ��Ļ�������뵽���ʽ������
                    addInExpArr(ExpArr, tempExpInfo, ExpIndex);
                    repetiveTime = 0;
                }
                else {
                        //5.2.2 ���Ϸ��Ļ��������ظ��������£��ͷſռ�
                        repetiveTime++;
                        if(repetiveTime >= ExpIndex)
                            break;
                        free(opNumArr);
                        continue;
                }
            }

            //6���ڿ���̨����Ŀ�ļ������ļ���ӡ��Ŀ���
            printf("%d.\t", ExpIndex);
            fprintf(questionFile,"%d.\t", ExpIndex);
            fprintf(answerFile, "%d.\t", ExpIndex);

            //6.1 ���ʽд����Ŀ�ļ�
            writeExp(ExpTree, questionFile);
            printf("= ");
            fprintf(questionFile, "= \n");

            //6.2 д����ļ�
            writeAnswer(result, answerFile);

            //6.3 ����̨չʾ�������
            //int flag = TRUE;
            //getValue(ExpTree, flag, 1);
            //printf("\n");

            //7�� �������
            ExpIndex++;
        }
    }

    //8�� ������ϣ��ر��ļ����ͷű��ʽ����ռ�
    fclose(questionFile);
    fclose(answerFile);
    free(ExpArr);

    if(runtimeFlag == FALSE)
    {
        printf("�ռ�����ʧ��...��������˳�\n");    getchar();
        exit(1);
    }
    printf("\n��ǰ������ %d ������������ΧΪ %d ������������Ŀ.\n", ExpIndex, numRange);
    printf("��Ŀ�����ڵ�ǰĿ¼�µ�question.txt�ļ���\n�𰸱����ڵ�ǰĿ¼�µ�answer.txt�ļ���\n");
    return 0;
}
