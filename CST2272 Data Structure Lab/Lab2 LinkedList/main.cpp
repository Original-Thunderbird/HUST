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
typedef int status;
typedef int ElemType; //数据元素类型定义
typedef struct lnode{  //元素节点定义
	ElemType data;  //数据域
	struct lnode *next; //指针域
}LNode;
typedef struct llist{    //头结点定义
    int len;     //len存储此表长度
    struct lnode *head, *tail;      //head指向表内首元，tail指向尾元
    struct llist *fore, *aft;          //aft指向下一个表头结点，ore指向前一个表头结点，这样的定义方式决定了在第一个头结点前应有一空白头结点。
}Linklist;
int length=0, vol=0;       //length为表的个数，vol为当前操作表的位序
FILE *fp;
char *filename, *pc, appe[5]=".dat", appf[5]=".txt", input[60]; //均用于生成文件名

status IntiaList(Linklist **L);
status DestroyList(Linklist *L);
status ClearList(Linklist *L);
status ListEmpty(Linklist *L);
int ListLength(Linklist *L);
status GetElem(Linklist *L,int i,ElemType &e);
status LocateElem(Linklist *L,ElemType e); //简化过
status PriorElem(Linklist *L,ElemType cur,ElemType &pre_e);
status NextElem(Linklist *L,ElemType cur,ElemType &next_e);
status ListInsert(Linklist *L,int i,ElemType e);
status ListDelete(Linklist *L,int i,ElemType &e);
status ListTrabverse(Linklist *L);  //简化过
status LoadList(FILE *fp,Linklist *List);
status SaveList(FILE *fp,Linklist *List);
status ChooseList(int vol, Linklist **L, Linklist *List);
status ChartTraverse(Linklist *List);

int main(void){
  Linklist *List=NULL, *L=NULL;
  int op=1,i,cur,pre_e,next_e;
  while(op){
	system("cls");	printf("\n\n");
	printf("系统中共%d个表，当前操作第%d个表\n", length, vol);    //提示当前操作的表的位序
	printf("      Menu for Linear Table On Node Structure \n");
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
		 if(IntiaList(&List)==OK){
            printf("线性表创建成功！");
            L=List->aft;     vol=1;
		 }
		     else printf("线性表创建失败！");
		 getchar();getchar();
		 break;
	   case 2:
         DestroyList(L);
		 getchar();		 break;
	   case 3:
         ClearList(L);
		 getchar();		 break;
	   case 4:
         i = ListEmpty(L);
         if(i==TRUE)            printf("线性表为空！");
         else if(i==FALSE)            printf("线性表不为空！");
        getchar();getchar();
		 break;
	   case 5:
         if(ListLength(L)==ERROR){
            getchar();getchar();            break;
         }
		 printf("表长为%d", L->len);
		 getchar();getchar();		 break;
	   case 6:
         if(L==NULL){
            printf("表未初始化");
            getchar();            break;
         }
         printf("请输入元素位序i(1<=i<%d):",L->len+1);
         scanf("%d",&i);
         if(GetElem(L,i,cur)==OK)
            printf("第%d个元素为%d", i, cur);
		getchar();getchar();
		 break;
	   case 7:
         printf("请输入待查数据:");
         scanf("%d", &cur);
         i=LocateElem(L,cur);
         if(i>0)
            printf("%d为表中第%d个元素", cur, i);
         if(!i)
            printf("未找到此元素");
		 getchar();getchar();		 break;
	   case 8:
         printf("请输入定位数据:");
         scanf("%d", &cur);
         if(PriorElem(L,cur,pre_e)==OK)
            printf("%d的前驱为%d", cur, pre_e);
		 getchar();getchar();		 break;
	   case 9:
         printf("请输入定位数据:");
		 scanf("%d", &cur);
         if(NextElem(L,cur,next_e)==OK)
            printf("%d的后继为%d", cur, next_e);
		 getchar();getchar();		 break;
	   case 10:
         if(L==NULL){
            printf("表未初始化");
            getchar(); getchar();           break;
         }
         printf("请输入插入位置i(1<=i<=%d):", L->len+1);
         scanf("%d", &i);
         printf("请输入插入数据e:");
         scanf("%d", &cur);
         if(ListInsert(L,i,cur)==ERROR)            printf("插入失败");
         else            printf("插入成功");
		 getchar();getchar();		 break;
	   case 11:
         if(L==NULL){
            printf("表未初始化");
            getchar();getchar();            break;
         }
		 printf("请输入删除位置i（1<=i<=%d）", L->len);
         scanf("%d", &i);
         if(ListDelete(L,i,cur)==ERROR)
            printf("删除失败");
         else
            printf("删除成功,删除的元素为%d",cur);
		 getchar();getchar();		 break;
	   case 12:
		 if(ListTrabverse(L)==ERROR) printf("遍历失败！\n");
		 getchar();getchar();		 break;
       case 13:
         if(LoadList(fp,List)==OK){
             printf("文件加载成功");
         }
         getchar();getchar();         break;
       case 14:
         if(SaveList(fp,List)==OK){
             printf("文件保存成功");
         }
         getchar();getchar();		 break;
       case 15:
         printf("请输入要操作表的位序vol:(1<=vol<=%d)",length+1);
         scanf("%d", &vol);
         ChooseList(vol, &L, List);
         getchar();         break;
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
 * 函数参数：链式表指针L的地址
 * 函数功能：构造一个空的链式表
 * 返回值：成功构造返回OK，否则返回ERROR
 **/
status IntiaList(Linklist **L){
    /*初始化大表头-开始*/
    Linklist *p;
	p =(Linklist*)malloc(sizeof(Linklist));
	if(!p){
        printf("内存分配失败");   return ERROR;
    }
    p->fore =NULL;
    p->head = p->tail = NULL;
    p->len = 0;
    /*初始化大表头-结束*//*初始化第一个小表头-开始*/
    p->aft =(Linklist*)malloc(sizeof(Linklist));
	if(!p->aft){
        printf("内存分配失败");   return ERROR;
    }
    p->aft->aft =NULL;
    p->aft->head = p->aft->tail = NULL;
    p->aft->fore = p;
    p->aft->len = 0;
    /*初始化第一个小表头-结束*/
    length = vol = 1;    *L=p;
	return OK;
}
/**
 * 函数名称：DestroyList
 * 函数参数：链式表指针L
 * 函数功能：删除链式表
 * 返回值：成功销毁返回OK，否则返回ERROR
 **/
status DestroyList(Linklist *L){
    Linklist *q;
    if(L==NULL){
        printf("表未初始化");
        getchar();
        return ERROR;
    }
    LNode *p;
    if(L->head!=NULL){
        p = L->head;
        while(p->next != NULL){     //删除除首元外所有元素
            L->head = p->next;
            free(p);            p = L->head;
        }
        free(p);            //删除首元
        L->head=L->tail=NULL;
    }
    L->fore->aft=L->aft;
    if(L->aft!=NULL)
        L->aft->fore=L->fore;
    q=L;     L=L->aft;  length--;
    free(q);    //删除头结点，前后表衔接
    if(vol>length)  vol=length;
    printf("链式表%d已删除！", vol+1);    getchar(); getchar();
    return OK;
}
/**
 * 函数名称：ClearList
 * 函数参数：链式表指针L
 * 函数功能：链式线性表
 * 返回值：成功置空返回OK，否则返回ERROR
 **/
status ClearList(Linklist *L){
    if(L==NULL){
        printf("表未初始化");
        getchar();
        return ERROR;
    }
    LNode *p;
    if(L->head!=NULL){  //表非空
        p = L->head;
        while(p->next != NULL){     //删除除首元外所有元素
            L->head = p->next;
            free(p);            p = L->head;
        }
        free(p);            //删除首元
        L->head=L->tail=NULL;
        L->len=0;       //表长记录归零
    }
    printf("表已清空！");    getchar();
    return OK;
}
/**
 * 函数名称：ListEmpty
 * 函数参数：链式表指针L
 * 函数功能：判断链式表是否为空
 * 返回值：若L为空表则返回TRUE，否则返回FALSE
 **/
status ListEmpty(Linklist *L){
    if(L==NULL){
        printf("表未初始化");   getchar();
        return ERROR;
    }
    int flag=TRUE;
    if(L->head!=NULL)
        flag = FALSE;   //表长不为0
    return flag;
}
/**
 * 函数名称：ListLength
 * 函数参数：链式表指针L
 * 函数功能：计算链式表L中数据元素个数；
 * 返回值：返回L中数据元素个数
 **/
int ListLength(Linklist *L){
    if(L==NULL){
        printf("表未初始化");   getchar();
        return ERROR;
    }
    return L->len;  //表空返回0，非空返回长度
}
/**
 * 函数名称：GetElem
 * 函数参数：链式表指针L，e为第i个数据的地址；
 * 函数功能：查找并显示L中第i个元素的值；
 * 返回值：成功则返回第i个元素的值，否则返回ERROR
 **/
status GetElem(Linklist *L,int i,ElemType &e){
    LNode *p; int j=1;
    if(L->head==NULL){
        printf("表中无元素！");  getchar();
        return ERROR;
    }
    if(i<1 || i>L->len){
        printf("下标越界！");  getchar();
        return ERROR;
    }
    for(p=L->head;j<i;p=p->next,j++){}
    e = p->data;    return OK;
}
/**
 * 函数名称：LocateElem
 * 函数参数：链式表指针L，相对比的元素值；
 * 函数功能：查找L中与e相同数据所在的位序；位序从1开始
 * 返回值：成功则返回第一个与e相同的数据的位序，不存在则返回0，其余情况返回ERROR
 **/
status LocateElem(Linklist *L,ElemType e){
    LNode *p;
    char i=1;
    if(L==NULL){
        printf("表未初始化");  getchar();
        return ERROR;
    }
    if(L->head==NULL){
        printf("表中无元素！");  getchar();
        return ERROR;
    }
    for(p=L->head;p!=NULL;p=p->next,i++)
        if(p->data==e)      return i;
    return 0;  //未找到元素
}
/**
 * 函数名称：PriorElem
 * 函数参数：链式表指针L，查找的数据cur_e,前驱pre_e；
 * 函数功能：查找L中与cur_e相同的第一个数据，并返回其前驱
 * 返回值：成功则返回与cur相同的第一个数据，并返回其第一个前驱；若无前驱以及其他情况则返回ERROR
 **/
status PriorElem(Linklist *L,ElemType cur,ElemType &pre_e){
    LNode *p, *q;
    char i=1, j=0;
    if(L==NULL){
        printf("表未初始化");   getchar();
        return ERROR;
    }
    if(L->head==NULL){
        printf("表中无元素！");  getchar();
        return ERROR;
    }
    for(p=L->head;p!=NULL;p=p->next,i++)
        if(p->data==cur){
            if(i==1){            //表中首元无前驱
                printf("此元素无前驱！");   getchar();
                return ERROR;
            }
            else{
                for(q=L->head;j<i-2;q=q->next,j++){}
                pre_e= q->data;
                return OK;
            }
        }
    printf("未找到此元素！");   getchar();
    return ERROR;
}
/**
 * 函数名称：NextElem
 * 函数参数：链式表指针L，查找的数据cur,后驱next_e；
 * 函数功能：查找L中与cur相同的第一个数据，并返回其后继
 * 返回值：成功则返回与cur相同的第一个数据，并返回其后继；若无后驱以及其他情况则返回ERROR
 **/
status NextElem(Linklist *L,ElemType cur,ElemType &next_e){
    LNode *p;
    char i=1;
    if(L==NULL){
        printf("表未初始化");   getchar();
        return ERROR;
    }
    if(L->head==NULL){
        printf("表中无元素！");  getchar();
        return ERROR;
    }
    for(p=L->head;p!=NULL;p=p->next,i++)
        if(p->data==cur){
            if(p->next==NULL){            //表中首元无前驱
                printf("此元素无后继！");     getchar();
                return ERROR;
            }
            else{
                next_e=p->next->data;   return OK;
            }

        }
    printf("未找到此元素！");    getchar();
    return ERROR;
}
/**
 * 函数名称：ListInsert
 * 函数参数：链式表指针L，插入的位置i，插入的数据元素e。
 * 函数功能：在L的第i个位置之前插入新的数据元素e；若已插满，则按照分配增量，分配更大的空间。
 * 返回值：成功插入返回OK，否则返回ERROR
 **/
status ListInsert(Linklist *L,int i,ElemType e){
    LNode *p=L->head, *s;
    char j=1;
    if(i<1 || i>L->len+1) {     //尾元之后有一个位置可插入
        printf("下标越界，");
        getchar();   return ERROR;
    }
    s = (LNode*)malloc(sizeof(LNode));  //建立节点
    s->data= e;
    if(i==1){   //插入首元时直接让其被头结点所指
        s->next=p;        L->head=s;
    }
    else{       //在其他位置插入元素
        for(;j<i-1;p=p->next,j++);
        s->next=p->next;        //让新节点指向当前位置现有节点
        p->next=s;      //现有节点的前驱指向新节点
        if(i==L->len+1)     //插入尾元时表尾指向NULL
            s->next=NULL;
    }
    L->len++;    return OK;
}
/**
 * 函数名称：ListDelete
 * 函数参数：链式表指针L，删除的位序i，指针e的地址。
 * 函数功能：删除L的第i个数据元素，用e返回其值。
 * 返回值：成功删除返回OK，否则返回ERROR
 **/
status ListDelete(Linklist *L,int i,ElemType &e){
    LNode *p=L->head, *s;
    char j=1;
    if(L->head==NULL){
        printf("表中无元素！");   getchar();
        return ERROR;
    }
    if(i<1 || i>L->len) {
        printf("下标越界，");
        getchar();   return ERROR;
    }
    if(i==1){   //删除首元时让头结点指向其后继
        L->head=p->next;        e=p->data;  free(p);    p=NULL;
    }
    else{   //在其他位置删除元素
        for(;j<i-1;p=p->next,j++);
        s=p->next;        p->next=s->next;    //当前节点前驱指向其后继
        e=s->data;        free(s);  s=NULL;
    }
    L->len--;    return OK;
}
/**
 * 函数名称：ListTrabverse
 * 函数参数：链式表指针L
 * 函数功能：依次显示线性表中的每个元素。
 * 返回值：成功遍历返回线性表的长度，否则返回ERROR
 **/
status ListTrabverse(Linklist *L){
    LNode *p=L->head;
    if(L==NULL){
        printf("表未初始化");    getchar();
        return ERROR;
    }
    if(L->head==NULL){
        printf("表中无元素，");   getchar();
        return ERROR;
    }
   printf("\n-----------all elements -----------------------\n");
   for(;p!=NULL;p=p->next) printf("%d ",p->data);
   printf("\n------------------ end ------------------------\n");
   return L->len;
}
/**
 * 函数名称：LoadList
 * 函数参数：文件指针，链式表单List
 * 函数功能：数据加载，为线性表输入元素。
 * 返回值：加载成功返回OK，加载失败返回ERROR
 **/
 status LoadList(FILE *fp,Linklist *List){
     int i,yl;
     Linklist *p;
     LNode *r;
     if(List==NULL){
        printf("表未初始化");    getchar();
        return ERROR;
    }
     printf("请输入您要恢复的数据所在的文件名:");
     scanf("%s",input);
     filename=input;
     if ((fp=fopen(filename,"r"))==NULL)
     {
         printf("打开文件失败");  return ERROR;
     }
     fread(&yl,sizeof(int),1,fp);      //读取有效表个数
     length=yl;
     for(i=0,p=List; i<yl; i++,p=p->aft){
        p->aft =(Linklist*)malloc(sizeof(Linklist));
        if(!p->aft){
            printf("内存分配失败");   return ERROR;
        }
        p->aft->fore = p;
        p->aft->aft = NULL;
        p->aft->head = p->aft->tail = NULL;
        fread(&(p->aft->len),sizeof(int),1,fp);  //读入每个表的表长
     }
     for(p=List->aft;p!=NULL;p=p->aft){
        //printf("%d\n",p->len);
        p->head=(LNode *)malloc(sizeof(LNode));
        p->head->next=NULL;
        fread(&(p->head->data),sizeof(ElemType),1,fp);
        for(i=1,r=p->head; i<p->len; i++,r=r->next){
            r->next=(LNode *)malloc(sizeof(LNode));
            r->next->next=NULL;
            fread(&(r->next->data),sizeof(ElemType),1,fp);
        }
     }
     ChartTraverse(List);
     if(vol>length)  vol=length;
     //这里从文件中逐个读取数据元素恢复顺序表，对于不同的物理结构，可通过读取的数据元素恢复内存中的物理结构。
     fclose(fp);	 return OK;
 }
 /**
 * 函数名称：SaveList
 * 函数参数：文件指针fp，链式表单指针List
 * 函数功能：将L中的数据保存到fp指针指向的文件中
 * 返回值：OK
 **/
 status SaveList(FILE *fp,Linklist *List){
    int yl=0;
    Linklist *p;
    LNode *q;
    if(List==NULL){
        printf("表未初始化");    getchar();
        return ERROR;
    }
    for(p=List->aft;p!=NULL;p=p->aft){      //统计有元素的表的个数
        if(p->len!=0)       yl++;
    }
    if(yl ==0){       //无有效数据时报错返回
         printf("没有待存数据");  getchar();
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
        printf("文件打开失败");   getchar();getchar();
        return ERROR;
    }
    fwrite(&yl,sizeof(int),1,fp);   //写入有效表的个数
    for(p=List->aft;p!=NULL;p=p->aft){      //写入每个有效表的表长
        if(p->len!=0)
            fwrite(&(p->len),sizeof(int),1,fp);
    }
    for(p=List->aft;p!=NULL;p=p->aft){      //循环写入每个有效表的元素
        if(p->len!=0)
            for(q=p->head;q!=NULL;q=q->next)
                fwrite(&(q->data),sizeof(ElemType),1,fp);
    }
    //通过遍历逐个访问数据元素并写入到文件中
    fclose(fp);    return OK;
}
/**
 * 函数名称：ChooseList
 * 函数参数：要替换的表位置vol，链式表单指针L，链式表单指针List
 * 函数功能：将L指向要操作的表下标vol-1
 * 返回值：失败返回ERROR，成功返回OK
 **/
status ChooseList(int vol, Linklist **L, Linklist *List){
    Linklist *p;
    if(List==NULL){
        printf("表未初始化");    getchar();
        return ERROR;
    }
    if(vol<1 || vol>length+1){
        printf("下标越界，操作失败！");   getchar();
        return ERROR;
    }
    if(vol==length+1){
        for(p=List->aft;p->aft!=NULL;p=p->aft);
        p->aft=(Linklist*)malloc(sizeof(Linklist));
        if(!p->aft){
            printf("内存分配失败");   return ERROR;
        }
        p->aft->aft = NULL;
        p->aft->head = p->aft->tail = NULL;   //此时通过‘前’尾节点操作新建节点
        p->aft->fore = p;
        p= p->aft;   //L指向尾节点
        p->len = 0;    length++;   vol = length;
    }
    else{
        char i=1;
        for(p=List->aft;i<vol;p=p->aft,i++){}
    }
    *L=p;    return OK;
}
/**
 * 函数名称：ChartTraverse
 * 函数参数：链式表单指针List
 * 函数功能：遍历整个表单，表中无元素时单独说明
 * 返回值：失败返回ERROR，成功返回OK
 **/
status ChartTraverse(Linklist *List){
    char i=0, flag=ERROR;
    Linklist *p;
    LNode *q;
    if(List==NULL){
        printf("表未初始化");    getchar();
        return ERROR;
    }
    printf("\n-----------all elements -----------------------\n");
    for(p=List->aft;p!=NULL;p=p->aft,i++){
        if(p->len==0){  //空表
            printf("List[%d]中无元素\n", i+1);
            continue;
        }
        for(q=p->head;q!=NULL;q=q->next) printf("%d ",q->data);
        flag=OK;
        putchar('\n');      //每个表遍历完成后换行
    }
    printf("\n------------------ end ------------------------\n");
    return flag;
}
