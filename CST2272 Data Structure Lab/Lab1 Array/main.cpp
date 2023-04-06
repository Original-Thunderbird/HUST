using namespace std;

/* Linear Table On Sequence Structure */
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define OK 1
#define TRUE 1
#define FALSE -1
#define ERROR -2
#define LIST_INIT_SIZE 100
#define LISTINCREMENT  10
typedef int status;
typedef int ElemType; //数据元素类型定义
typedef struct{  //顺序表（顺序结构）的定义
	ElemType * elem;
	int length;
	int listsize;
}SqList;
//长为20的数组判定对应的各个线性表是否初始化，TRUE为已初始化；FALSE为未初始化
//vol用于定位当前操作的线性表
int judge[20], vol=1;
ElemType *newbase;
FILE *fp;
char *filename, *pc, appe[5]=".dat", input[60]; //用于生成文件名

status IntiaList(SqList *L);
status DestroyList(SqList *L);
status ClearList(SqList *L);
status ListEmpty(SqList *L);
int ListLength(SqList *L);
status GetElem(SqList *L,int i,ElemType &e);
status LocateElem(SqList *L,ElemType e); //简化过
status PriorElem(SqList *L,ElemType cur,ElemType &pre_e);
status NextElem(SqList *L,ElemType cur,ElemType &next_e);
status ListInsert(SqList *L,int i,ElemType e);
status ListDelete(SqList *L,int i,ElemType &e);
status ListTrabverse(SqList *L);  //简化过
status LoadList(FILE *fp,SqList List[]);
status SaveList(FILE *fp,SqList List[]);
status ChooseList(int vol, SqList **L, SqList List[]);
status ChartTraverse(SqList List[]);

int main(void){
  SqList *L=NULL, List[20];
  int op=1,i,e,cur,pre_e,next_e;
  L=List;
  while(op){
	system("cls");	printf("\n\n");
	printf("系统中共20个表，当前操作第%d个表\n", vol);    //提示当前操作的表的位序
	printf("      Menu for Linear Table On Sequence Structure \n");
	printf("-------------------------------------------------\n");
	printf("    	  1. IntiaList       7. LocateElem\n");
	printf("    	  2. DestroyList     8. PriorElem\n");
	printf("    	  3. ClearList       9. NextElem \n");
	printf("    	  4. ListEmpty       10. ListInsert\n");
	printf("    	  5. ListLength      11. ListDelete\n");
	printf("    	  6. GetElem         12. ListTrabverse\n");
	printf("    	  13.LoadList        14. SaveList\n");
    printf("    	  15.ChooseList      16. ChartTraverse\n");
    printf("    	  0. Exit\n");
	printf("-------------------------------------------------\n");
	printf("    请选择你的操作[0~16]:");
	scanf("%d",&op);
    switch(op){
	   case 1:
		 if(IntiaList(L)==OK)
                printf("线性表创建成功！");
		     else printf("线性表创建失败！");
		 getchar();getchar();
		 break;
	   case 2:
         DestroyList(L);
		 getchar();
		 break;
	   case 3:
         ClearList(L);
		 getchar();
		 break;
	   case 4:
         i = ListEmpty(L);
         if(i==TRUE)
            printf("线性表为空！");
         else if(i==FALSE)
            printf("线性表不为空！");
        getchar();getchar();
		 break;
	   case 5:
         if(ListLength(L)==ERROR){
            getchar();getchar();
            break;
         }
		 printf("表长为%d", L->length);
		 getchar();getchar();
		 break;
	   case 6:
         printf("请输入元素位序i(1<=i<%d):",L->length+1);
         scanf("%d",&i);
         if(GetElem(L,i,e)==OK)
            printf("第%d个元素为%d", i, e);
		getchar();getchar();
		 break;
	   case 7:
         printf("请输入待查数据:");
         scanf("%d", &e);
         i=LocateElem(L,e);
         if(i>0)
            printf("%d为表中第%d个元素", e, i);
         if(!i)
            printf("未找到此元素");
		 getchar();getchar();
		 break;
	   case 8:
         printf("请输入定位数据:");
         scanf("%d", &cur);
         if(PriorElem(L,cur,pre_e)==OK)
            printf("%d的前驱为%d", cur, pre_e);
		 getchar();getchar();
		 break;
	   case 9:
         printf("请输入定位数据:");
		 scanf("%d", &cur);
         if(NextElem(L,cur,next_e)==OK)
            printf("%d的后继为%d", cur, next_e);
		 getchar();getchar();
		 break;
	   case 10:
         printf("请输入插入位置i(1<=i<=%d):", L->length+1);
         scanf("%d", &i);
         printf("请输入插入数据e:");
         scanf("%d", &e);
         if(ListInsert(L,i,e)==ERROR)
            printf("插入失败");
         else
            printf("插入成功");
		 getchar();getchar();
		 break;
	   case 11:
		 printf("请输入删除位置i（1<=i<=%d）", L->length);
         scanf("%d", &i);
         if(ListDelete(L,i,e)==ERROR)
            printf("删除失败");
         else
            printf("删除成功,删除的元素为%d",e);
		 getchar();getchar();
		 break;
	   case 12:
		 if(ListTrabverse(L)==ERROR) printf("遍历失败！\n");
		 getchar();getchar();
		 break;
       case 13:
         if(LoadList(fp,List)==OK){
             printf("文件加载成功");
         }
         getchar();getchar();
         break;
       case 14:
         if(SaveList(fp,List)==OK){
             printf("文件保存成功");
         }
         getchar();getchar();
		 break;
       case 15:
         printf("请输入要操作表的位序vol:(1<=vol<=20)");
         scanf("%d", &vol);
         ChooseList(vol, &L, List);
         getchar();
         break;
       case 16:
         if(ChartTraverse(List)==ERROR) printf("表单遍历失败！");
		 getchar();getchar();
	   case 0:
         break;
	}//end of switch
  }//end of while
  printf("欢迎下次再使用本系统！");
  return 0;
}//end of main()

/**
 * 函数名称：InitiaList
 * 函数参数：线性表指针L或List[]的地址
 * 函数功能：构造一个空的线性表
 * 返回值：成功构造返回OK，否则返回ERROR
 **/
status IntiaList(SqList *L){
	L->elem = (ElemType *)malloc( LIST_INIT_SIZE * sizeof (ElemType));
    if(!L->elem) return ERROR;
	L->length=0;
    L->listsize=LIST_INIT_SIZE;     //初始分配空间
    judge[vol-1] = TRUE;            //当前表已初始化，修改对应判定变量
	return OK;
}
/**
 * 函数名称：DestroyList
 * 函数参数：线性表指针L
 * 函数功能：删除线性表
 * 返回值：成功销毁返回OK，否则返回ERROR
 **/
status DestroyList(SqList *L){
//对于未操作过的判定数组各元素，其值为一随机数，不一定为FALSE，故条件应为非TRUE，后不再说明
    if(judge[vol-1] !=TRUE){
        printf("\n此表未初始化！");
        getchar();
        return ERROR;
    }
    free(L->elem);
    L->length=L->listsize=0;
    //销毁表L及附属全部信息
    L=NULL;
    judge[vol-1] = FALSE;  //修改对应判定变量
    printf("线性表%d已删除！", vol);
    getchar();
    return OK;
}
/**
 * 函数名称：ClearList
 * 函数参数：线性表指针L
 * 函数功能：重置线性表
 * 返回值：成功置空返回OK，否则返回ERROR
 **/
status ClearList(SqList *L){
    if(judge[vol-1]  !=TRUE){
        printf("此表未初始化！");
        getchar();
        return ERROR;
    }
    L->length=0;                 //初始表长
    L->listsize=LIST_INIT_SIZE;  //初始状态空间大小
    printf("表已清空！");
    getchar();
    return OK;
}
/**
 * 函数名称：ListEmpty
 * 函数参数：线性表指针L
 * 函数功能：判断线性表是否为空
 * 返回值：若L为空表则返回TRUE，否则返回FALSE
 **/
status ListEmpty(SqList *L){
    if(judge[vol-1]  !=TRUE){
        printf("此表未初始化！");
        getchar();
        return ERROR;
    }
    int flag=TRUE;
    if(L->length!=0)
        flag = FALSE;   //表长不为0
    return flag;
}
/**
 * 函数名称：ListLength
 * 函数参数：线性表指针L
 * 函数功能：计算线性表L中数据元素个数；
 * 返回值：成功返回L中数据元素个数，失败返回ERROR
 **/
int ListLength(SqList *L){
    if(judge[vol-1] !=TRUE){
        printf("此表尚未初始化");
        getchar();
        return ERROR;
    }
    return L->length;
}
/**
 * 函数名称：GetElem
 * 函数参数：线性表指针L，e为第i个数据的地址；
 * 函数功能：查找并显示L中第i个元素的值；
 * 返回值：成功则返回第i个元素的值，否则返回ERROR
 **/
status GetElem(SqList *L,int i,ElemType &e){
    if(judge[vol-1] !=TRUE){
        printf("此表尚未初始化");
        getchar();
        return ERROR;
    }
    if(ListEmpty(L)==TRUE){
        printf("\n表中无元素！");
        getchar();
        return ERROR;
    }
    if(i<1 || i>L->length+1){
        printf("下标越界！");
        getchar();
        return ERROR;
    }
    e=L->elem[i-1];
    return OK;
}
/**
 * 函数名称：LocateElem
 * 函数参数：线性表指针L，相对比的元素值；
 * 函数功能：查找L中与e相同数据所在的位序；位序从1开始
 * 返回值：成功则返回第一个与e相同的数据的位序，不存在则返回0，其余情况返回ERROR
 **/
status LocateElem(SqList *L,ElemType e){
    int i;
    if(judge[vol-1] !=TRUE){
        printf("此表尚未初始化");
        getchar();
        return ERROR;
    }
    if(ListEmpty(L)==TRUE){
        printf("表中无元素！");
        getchar();
        return ERROR;
    }

    for(i=0;i<L->length;i++){
        if(L->elem[i] == e)
            return (i+1);   //下标+1即为位序
    }
    return 0;  //未找到元素
}
/**
 * 函数名称：PriorElem
 * 函数参数：线性表指针L，查找的数据cur_e,前驱pre_e；
 * 函数功能：查找L中与cur_e相同的第一个数据，并返回其前驱
 * 返回值：成功则返回与cur相同的第一个数据，并返回其第一个前驱；若无前驱以及其他情况则返回ERROR
 **/
status PriorElem(SqList *L,ElemType cur,ElemType &pre_e){
    int i;
    if(judge[vol-1] !=TRUE){
        printf("此表尚未初始化");
        getchar();
        return ERROR;
    }
    if(ListEmpty(L)==TRUE){
        printf("表中无元素！");
        getchar();
        return ERROR;
    }
    for(i=0;i<L->length;i++){
        if(L->elem[i] == cur){   //大部分循环只做到这一步
            if(i==0){            //表中首元无前驱
                printf("此元素无前驱！");
                getchar();
                return ERROR;
            }
            else{
                pre_e = L->elem[i-1];
                return OK;
            }
        }
    }
    printf("未找到此元素！");
    getchar();
    return ERROR;
}
/**
 * 函数名称：NextElem
 * 函数参数：线性表指针L，查找的数据cur,后驱next_e；
 * 函数功能：查找L中与cur相同的第一个数据，并返回其后继
 * 返回值：成功则返回与cur相同的第一个数据，并返回其后继；若无后驱以及其他情况则返回ERROR
 **/
status NextElem(SqList *L,ElemType cur,ElemType &next_e){
    int i;
    if(judge[vol-1] !=TRUE){
        printf("此表尚未初始化！");
        getchar();
        return ERROR;
    }
    if(ListEmpty(L)==TRUE){
        printf("表中无元素！");
        getchar();
        return ERROR;
    }
    for(i=0;i<L->length;i++){
        if(L->elem[i] == cur){   //大部分循环只做到这一步
            if(i!=L->length-1){
                next_e = L->elem[i+1];
                return OK;
            }
            else{               //表中尾元无后继
                printf("此元素无后继！");
                getchar();
                return ERROR;
            }
        }
    }
    printf("未找到此元素！");
    return ERROR;
}
/**
 * 函数名称：ListInsert
 * 函数参数：线性表指针L，插入的位置i，插入的数据元素e。
 * 函数功能：在L的第i个位置之前插入新的数据元素e；若已插满，则按照分配增量，分配更大的空间。
 * 返回值：成功插入返回OK，否则返回ERROR
 **/
status ListInsert(SqList *L,int i,ElemType e){
    ElemType *p, *q;
    if(judge[vol-1] !=TRUE){
        printf("此表尚未初始化，");
        getchar();   return ERROR;
    }
    if(i<1 || i >L->length+1) {
        printf("下标越界，");
        getchar();   return ERROR;
    }
    //元素个数溢出时额外分配空间
    if(L->length >= L->listsize){
        newbase = (ElemType *)realloc(L->elem, (L->listsize+LISTINCREMENT)* sizeof(ElemType));
        if(!newbase){
            printf("内存分配失败，");
            getchar();   return ERROR;
        }
        L->elem = newbase;
        L->listsize += LISTINCREMENT;
    }
    q = &(L->elem[i-1]);
    for(p=&(L->elem[L->length-1]); p>=q; p--){
        *(p+1) = *p;    //从表尾至插入处元素依次前进，为待插入元素留出空间
    }
    *q = e;         //插入元素
    L->length++;
    return OK;
}
/**
 * 函数名称：ListDelete
 * 函数参数：线性表指针L，删除的位序i，指针e的地址。
 * 函数功能：删除L的第i个数据元素，用e返回其值。
 * 返回值：成功删除返回OK，否则返回ERROR
 **/
status ListDelete(SqList *L,int i,ElemType &e){
    ElemType *p, *q;
    if(judge[vol-1] !=TRUE){
        printf("此表尚未初始化，");
        getchar();
        return ERROR;
    }
    if(ListEmpty(L)==TRUE){
        printf("表中无元素，");
        getchar();
        return ERROR;
    }
    if(i<1 || i >L->length) {
        printf("下标越界，");   return ERROR;
    }
    q = &(L->elem[i-1]);
    e = L->elem[i-1];
    for(p=q;p<=&(L->elem[L->length-1]); p++){
        *(p) = *(p+1);  //从插入处至表尾元素依次后退
    }
    L->length--;
    return OK;
}
/**
 * 函数名称：ListTrabverse
 * 函数参数：线性表指针L
 * 函数功能：依次显示线性表中的每个元素。
 * 返回值：成功遍历返回线性表的长度，否则返回ERROR
 **/
status ListTrabverse(SqList *L){
    int i;
    if(judge[vol-1] !=TRUE){
        printf("此表尚未初始化，");
        getchar();
        return ERROR;
    }
    if(ListEmpty(L)==TRUE){
        printf("表中无元素，");
        getchar();
        return ERROR;
    }
   printf("\n-----------all elements -----------------------\n");
   for(i=0;i<L->length;i++) printf("%d ",L->elem[i]);
   printf("\n------------------ end ------------------------\n");
   return L->length;
}
/**
 * 函数名称：LoadList
 * 函数参数：文件指针，线性表单List[20]
 * 函数功能：数据加载，为线性表输入元素。
 * 返回值：加载成功返回OK，加载失败返回ERROR
 **/
 status LoadList(FILE *fp,SqList List[20]){
     int i,j, yl;
     printf("请输入您要恢复的数据所在的文件名:");
     scanf("%s",input);
     filename=input;
     if ((fp=fopen(filename,"r"))==NULL)
     {
         printf("打开文件失败");
         return ERROR;
     }
     fread(&yl,sizeof(int),1,fp);      //读取有效表个数
     for(i=0;i<yl;i++){
        IntiaList(&List[i]);           //只初始化yl个表，
        fread(&(List[i].length),sizeof(int),1,fp);  //读入每个表的表长
        judge[i]=TRUE;                  //已初始化的表判定变量修改为TRUE
     }
     for(i=yl;i<20;i++){
        judge[i]=FALSE;         //未初始化的表判定变量统一为FALSE
     }
     for(i=0;i<yl;i++){
        for(j=0;j<List[i].length;j++){
            fread(&(List[i].elem[j]),sizeof(ElemType),1,fp);
            //元素个数溢出时额外分配空间
            if(List[i].length >= List[i].listsize){
                newbase = (ElemType *)realloc(List[i].elem, (List[i].listsize+LISTINCREMENT)* sizeof(ElemType));
                if(!newbase){
                    printf("内存分配失败");   return ERROR;
                }
                List[i].elem = newbase;
                List[i].listsize += LISTINCREMENT;
            }
        }
     }
     //这里从文件中逐个读取数据元素恢复顺序表，对于不同的物理结构，可通过读取的数据元素恢复内存中的物理结构。
     fclose(fp);
	 return OK;
 }
 /**
 * 函数名称：SaveList
 * 函数参数：文件指针，线性表单List[20]
 * 函数功能：将L中的数据保存到fp指针指向的文件中
 * 返回值：OK
 **/
 status SaveList(FILE *fp,SqList List[20]){
    int i,j,yl=0;
    for(i=0;i<20;i++){      //统计有元素的表的个数
        if(judge[i]==TRUE && List[i].length!=0)
            yl++;
    }
    if(yl ==0){       //无有效数据时报错返回
         printf("没有待存数据");
         getchar();
         return ERROR;
    }
    time_t lt;
    lt=time(NULL);
    filename=ctime(&lt);
    filename[strlen(filename)-1]='\0';
    strcat(filename,appe);      //以上生成时间命名的文件名字符串
    pc=&filename[0];
    while(*pc){       //文件名字符串规范化，将‘:’与' ' 用‘_’代替
        if((*pc==':' ) ||   (*pc==' ')){   *pc='_';    }
        pc++;
    }
    if ((fp=fopen(filename,"w"))==NULL)
    {
        printf("文件打开失败");
        getchar();getchar();
        return ERROR;
    }
    fwrite(&yl,sizeof(int),1,fp);   //写入有效表的个数
    for(i=0;i<20;i++){      //写入每个有效表的表长
        if(judge[i]==TRUE && List[i].length!=0)
            fwrite(&(List[i].length),sizeof(int),1,fp);
    }
    for(i=0;i<20;i++){      //循环写入每个有效表的元素
        if(judge[i]==TRUE && List[i].length!=0)
            for(j=0;j<List[i].length;j++)
                fwrite(&(List[i].elem[j]),sizeof(ElemType),1,fp);
    }

    //通过遍历逐个访问数据元素并写入到文件中
    fclose(fp);
    return OK;
}
/**
 * 函数名称：ChooseList
 * 函数参数：要替换的表位置vol，文件指针，线性表单List[20]
 * 函数功能：将L指向要操作的表下标vol-1
 * 返回值：失败返回ERROR，成功返回OK
 **/
status ChooseList(int vol, SqList **L, SqList List[20]){
    if(vol<1 || vol>20){
        printf("下标越界，操作失败！");
        getchar();
        return ERROR;
    }
    *L=&(List[vol-1]);
    if(judge[vol-1]==FALSE){
        printf("此表尚未初始化");    //防止误操作
        getchar();  getchar();
    }
    return OK;
}
/**
 * 函数名称：ChartTraverse
 * 函数参数：要替换的表位置vol，文件指针，线性表单List[20]
 * 函数功能：将L指向要操作的表下标vol-1
 * 返回值：失败返回ERROR，成功返回OK
 **/
status ChartTraverse(SqList List[20]){
    int i,j,flag=ERROR;
    printf("\n-----------all elements -----------------------\n");
    for(i=0; i<20; i++){
        if(judge[i] !=TRUE){    //未初始化
            printf("List[%d]未初始化\n", i);
            continue;
        }
        if(List[i].length==0){  //空表
            printf("List[%d]中无元素\n", i);
            continue;
        }
        for(j=0;j<List[i].length;j++) printf("%d ",List[i].elem[j]);
        flag=OK;
        putchar('\n');      //每个表遍历完成后换行
    }
    printf("\n------------------ end ------------------------\n");
    return flag;
}
