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
typedef int ElemType; //����Ԫ�����Ͷ���
typedef struct lnode{  //Ԫ�ؽڵ㶨��
	ElemType data;  //������
	struct lnode *next; //ָ����
}LNode;
typedef struct llist{    //ͷ��㶨��
    int len;     //len�洢�˱���
    struct lnode *head, *tail;      //headָ�������Ԫ��tailָ��βԪ
    struct llist *fore, *aft;          //aftָ����һ����ͷ��㣬oreָ��ǰһ����ͷ��㣬�����Ķ��巽ʽ�������ڵ�һ��ͷ���ǰӦ��һ�հ�ͷ��㡣
}Linklist;
int length=0, vol=0;       //lengthΪ��ĸ�����volΪ��ǰ�������λ��
FILE *fp;
char *filename, *pc, appe[5]=".dat", appf[5]=".txt", input[60]; //�����������ļ���

status IntiaList(Linklist **L);
status DestroyList(Linklist *L);
status ClearList(Linklist *L);
status ListEmpty(Linklist *L);
int ListLength(Linklist *L);
status GetElem(Linklist *L,int i,ElemType &e);
status LocateElem(Linklist *L,ElemType e); //�򻯹�
status PriorElem(Linklist *L,ElemType cur,ElemType &pre_e);
status NextElem(Linklist *L,ElemType cur,ElemType &next_e);
status ListInsert(Linklist *L,int i,ElemType e);
status ListDelete(Linklist *L,int i,ElemType &e);
status ListTrabverse(Linklist *L);  //�򻯹�
status LoadList(FILE *fp,Linklist *List);
status SaveList(FILE *fp,Linklist *List);
status ChooseList(int vol, Linklist **L, Linklist *List);
status ChartTraverse(Linklist *List);

int main(void){
  Linklist *List=NULL, *L=NULL;
  int op=1,i,cur,pre_e,next_e;
  while(op){
	system("cls");	printf("\n\n");
	printf("ϵͳ�й�%d������ǰ������%d����\n", length, vol);    //��ʾ��ǰ�����ı��λ��
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
	printf("    ��ѡ����Ĳ���[0~16]:");
	scanf("%d",&op);
    switch(op){
	   case 1:
		 if(IntiaList(&List)==OK){
            printf("���Ա����ɹ���");
            L=List->aft;     vol=1;
		 }
		     else printf("���Ա���ʧ�ܣ�");
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
         if(i==TRUE)            printf("���Ա�Ϊ�գ�");
         else if(i==FALSE)            printf("���Ա�Ϊ�գ�");
        getchar();getchar();
		 break;
	   case 5:
         if(ListLength(L)==ERROR){
            getchar();getchar();            break;
         }
		 printf("��Ϊ%d", L->len);
		 getchar();getchar();		 break;
	   case 6:
         if(L==NULL){
            printf("��δ��ʼ��");
            getchar();            break;
         }
         printf("������Ԫ��λ��i(1<=i<%d):",L->len+1);
         scanf("%d",&i);
         if(GetElem(L,i,cur)==OK)
            printf("��%d��Ԫ��Ϊ%d", i, cur);
		getchar();getchar();
		 break;
	   case 7:
         printf("�������������:");
         scanf("%d", &cur);
         i=LocateElem(L,cur);
         if(i>0)
            printf("%dΪ���е�%d��Ԫ��", cur, i);
         if(!i)
            printf("δ�ҵ���Ԫ��");
		 getchar();getchar();		 break;
	   case 8:
         printf("�����붨λ����:");
         scanf("%d", &cur);
         if(PriorElem(L,cur,pre_e)==OK)
            printf("%d��ǰ��Ϊ%d", cur, pre_e);
		 getchar();getchar();		 break;
	   case 9:
         printf("�����붨λ����:");
		 scanf("%d", &cur);
         if(NextElem(L,cur,next_e)==OK)
            printf("%d�ĺ��Ϊ%d", cur, next_e);
		 getchar();getchar();		 break;
	   case 10:
         if(L==NULL){
            printf("��δ��ʼ��");
            getchar(); getchar();           break;
         }
         printf("���������λ��i(1<=i<=%d):", L->len+1);
         scanf("%d", &i);
         printf("�������������e:");
         scanf("%d", &cur);
         if(ListInsert(L,i,cur)==ERROR)            printf("����ʧ��");
         else            printf("����ɹ�");
		 getchar();getchar();		 break;
	   case 11:
         if(L==NULL){
            printf("��δ��ʼ��");
            getchar();getchar();            break;
         }
		 printf("������ɾ��λ��i��1<=i<=%d��", L->len);
         scanf("%d", &i);
         if(ListDelete(L,i,cur)==ERROR)
            printf("ɾ��ʧ��");
         else
            printf("ɾ���ɹ�,ɾ����Ԫ��Ϊ%d",cur);
		 getchar();getchar();		 break;
	   case 12:
		 if(ListTrabverse(L)==ERROR) printf("����ʧ�ܣ�\n");
		 getchar();getchar();		 break;
       case 13:
         if(LoadList(fp,List)==OK){
             printf("�ļ����سɹ�");
         }
         getchar();getchar();         break;
       case 14:
         if(SaveList(fp,List)==OK){
             printf("�ļ�����ɹ�");
         }
         getchar();getchar();		 break;
       case 15:
         printf("������Ҫ�������λ��vol:(1<=vol<=%d)",length+1);
         scanf("%d", &vol);
         ChooseList(vol, &L, List);
         getchar();         break;
       case 16:
         if(ChartTraverse(List)==ERROR) printf("������ʧ�ܣ�");
		 getchar();getchar();
	   case 0:
         break;
	}//end of switch
  }//end of while
  printf("��ӭ�´���ʹ�ñ�ϵͳ��");
  return 0;
}//end of main()

/**
 * �������ƣ�InitiaList
 * ������������ʽ��ָ��L�ĵ�ַ
 * �������ܣ�����һ���յ���ʽ��
 * ����ֵ���ɹ����췵��OK�����򷵻�ERROR
 **/
status IntiaList(Linklist **L){
    /*��ʼ�����ͷ-��ʼ*/
    Linklist *p;
	p =(Linklist*)malloc(sizeof(Linklist));
	if(!p){
        printf("�ڴ����ʧ��");   return ERROR;
    }
    p->fore =NULL;
    p->head = p->tail = NULL;
    p->len = 0;
    /*��ʼ�����ͷ-����*//*��ʼ����һ��С��ͷ-��ʼ*/
    p->aft =(Linklist*)malloc(sizeof(Linklist));
	if(!p->aft){
        printf("�ڴ����ʧ��");   return ERROR;
    }
    p->aft->aft =NULL;
    p->aft->head = p->aft->tail = NULL;
    p->aft->fore = p;
    p->aft->len = 0;
    /*��ʼ����һ��С��ͷ-����*/
    length = vol = 1;    *L=p;
	return OK;
}
/**
 * �������ƣ�DestroyList
 * ������������ʽ��ָ��L
 * �������ܣ�ɾ����ʽ��
 * ����ֵ���ɹ����ٷ���OK�����򷵻�ERROR
 **/
status DestroyList(Linklist *L){
    Linklist *q;
    if(L==NULL){
        printf("��δ��ʼ��");
        getchar();
        return ERROR;
    }
    LNode *p;
    if(L->head!=NULL){
        p = L->head;
        while(p->next != NULL){     //ɾ������Ԫ������Ԫ��
            L->head = p->next;
            free(p);            p = L->head;
        }
        free(p);            //ɾ����Ԫ
        L->head=L->tail=NULL;
    }
    L->fore->aft=L->aft;
    if(L->aft!=NULL)
        L->aft->fore=L->fore;
    q=L;     L=L->aft;  length--;
    free(q);    //ɾ��ͷ��㣬ǰ����ν�
    if(vol>length)  vol=length;
    printf("��ʽ��%d��ɾ����", vol+1);    getchar(); getchar();
    return OK;
}
/**
 * �������ƣ�ClearList
 * ������������ʽ��ָ��L
 * �������ܣ���ʽ���Ա�
 * ����ֵ���ɹ��ÿշ���OK�����򷵻�ERROR
 **/
status ClearList(Linklist *L){
    if(L==NULL){
        printf("��δ��ʼ��");
        getchar();
        return ERROR;
    }
    LNode *p;
    if(L->head!=NULL){  //��ǿ�
        p = L->head;
        while(p->next != NULL){     //ɾ������Ԫ������Ԫ��
            L->head = p->next;
            free(p);            p = L->head;
        }
        free(p);            //ɾ����Ԫ
        L->head=L->tail=NULL;
        L->len=0;       //����¼����
    }
    printf("������գ�");    getchar();
    return OK;
}
/**
 * �������ƣ�ListEmpty
 * ������������ʽ��ָ��L
 * �������ܣ��ж���ʽ���Ƿ�Ϊ��
 * ����ֵ����LΪ�ձ��򷵻�TRUE�����򷵻�FALSE
 **/
status ListEmpty(Linklist *L){
    if(L==NULL){
        printf("��δ��ʼ��");   getchar();
        return ERROR;
    }
    int flag=TRUE;
    if(L->head!=NULL)
        flag = FALSE;   //����Ϊ0
    return flag;
}
/**
 * �������ƣ�ListLength
 * ������������ʽ��ָ��L
 * �������ܣ�������ʽ��L������Ԫ�ظ�����
 * ����ֵ������L������Ԫ�ظ���
 **/
int ListLength(Linklist *L){
    if(L==NULL){
        printf("��δ��ʼ��");   getchar();
        return ERROR;
    }
    return L->len;  //��շ���0���ǿշ��س���
}
/**
 * �������ƣ�GetElem
 * ������������ʽ��ָ��L��eΪ��i�����ݵĵ�ַ��
 * �������ܣ����Ҳ���ʾL�е�i��Ԫ�ص�ֵ��
 * ����ֵ���ɹ��򷵻ص�i��Ԫ�ص�ֵ�����򷵻�ERROR
 **/
status GetElem(Linklist *L,int i,ElemType &e){
    LNode *p; int j=1;
    if(L->head==NULL){
        printf("������Ԫ�أ�");  getchar();
        return ERROR;
    }
    if(i<1 || i>L->len){
        printf("�±�Խ�磡");  getchar();
        return ERROR;
    }
    for(p=L->head;j<i;p=p->next,j++){}
    e = p->data;    return OK;
}
/**
 * �������ƣ�LocateElem
 * ������������ʽ��ָ��L����Աȵ�Ԫ��ֵ��
 * �������ܣ�����L����e��ͬ�������ڵ�λ��λ���1��ʼ
 * ����ֵ���ɹ��򷵻ص�һ����e��ͬ�����ݵ�λ�򣬲������򷵻�0�������������ERROR
 **/
status LocateElem(Linklist *L,ElemType e){
    LNode *p;
    char i=1;
    if(L==NULL){
        printf("��δ��ʼ��");  getchar();
        return ERROR;
    }
    if(L->head==NULL){
        printf("������Ԫ�أ�");  getchar();
        return ERROR;
    }
    for(p=L->head;p!=NULL;p=p->next,i++)
        if(p->data==e)      return i;
    return 0;  //δ�ҵ�Ԫ��
}
/**
 * �������ƣ�PriorElem
 * ������������ʽ��ָ��L�����ҵ�����cur_e,ǰ��pre_e��
 * �������ܣ�����L����cur_e��ͬ�ĵ�һ�����ݣ���������ǰ��
 * ����ֵ���ɹ��򷵻���cur��ͬ�ĵ�һ�����ݣ����������һ��ǰ��������ǰ���Լ���������򷵻�ERROR
 **/
status PriorElem(Linklist *L,ElemType cur,ElemType &pre_e){
    LNode *p, *q;
    char i=1, j=0;
    if(L==NULL){
        printf("��δ��ʼ��");   getchar();
        return ERROR;
    }
    if(L->head==NULL){
        printf("������Ԫ�أ�");  getchar();
        return ERROR;
    }
    for(p=L->head;p!=NULL;p=p->next,i++)
        if(p->data==cur){
            if(i==1){            //������Ԫ��ǰ��
                printf("��Ԫ����ǰ����");   getchar();
                return ERROR;
            }
            else{
                for(q=L->head;j<i-2;q=q->next,j++){}
                pre_e= q->data;
                return OK;
            }
        }
    printf("δ�ҵ���Ԫ�أ�");   getchar();
    return ERROR;
}
/**
 * �������ƣ�NextElem
 * ������������ʽ��ָ��L�����ҵ�����cur,����next_e��
 * �������ܣ�����L����cur��ͬ�ĵ�һ�����ݣ�����������
 * ����ֵ���ɹ��򷵻���cur��ͬ�ĵ�һ�����ݣ����������̣����޺����Լ���������򷵻�ERROR
 **/
status NextElem(Linklist *L,ElemType cur,ElemType &next_e){
    LNode *p;
    char i=1;
    if(L==NULL){
        printf("��δ��ʼ��");   getchar();
        return ERROR;
    }
    if(L->head==NULL){
        printf("������Ԫ�أ�");  getchar();
        return ERROR;
    }
    for(p=L->head;p!=NULL;p=p->next,i++)
        if(p->data==cur){
            if(p->next==NULL){            //������Ԫ��ǰ��
                printf("��Ԫ���޺�̣�");     getchar();
                return ERROR;
            }
            else{
                next_e=p->next->data;   return OK;
            }

        }
    printf("δ�ҵ���Ԫ�أ�");    getchar();
    return ERROR;
}
/**
 * �������ƣ�ListInsert
 * ������������ʽ��ָ��L�������λ��i�����������Ԫ��e��
 * �������ܣ���L�ĵ�i��λ��֮ǰ�����µ�����Ԫ��e�����Ѳ��������շ����������������Ŀռ䡣
 * ����ֵ���ɹ����뷵��OK�����򷵻�ERROR
 **/
status ListInsert(Linklist *L,int i,ElemType e){
    LNode *p=L->head, *s;
    char j=1;
    if(i<1 || i>L->len+1) {     //βԪ֮����һ��λ�ÿɲ���
        printf("�±�Խ�磬");
        getchar();   return ERROR;
    }
    s = (LNode*)malloc(sizeof(LNode));  //�����ڵ�
    s->data= e;
    if(i==1){   //������Ԫʱֱ�����䱻ͷ�����ָ
        s->next=p;        L->head=s;
    }
    else{       //������λ�ò���Ԫ��
        for(;j<i-1;p=p->next,j++);
        s->next=p->next;        //���½ڵ�ָ��ǰλ�����нڵ�
        p->next=s;      //���нڵ��ǰ��ָ���½ڵ�
        if(i==L->len+1)     //����βԪʱ��βָ��NULL
            s->next=NULL;
    }
    L->len++;    return OK;
}
/**
 * �������ƣ�ListDelete
 * ������������ʽ��ָ��L��ɾ����λ��i��ָ��e�ĵ�ַ��
 * �������ܣ�ɾ��L�ĵ�i������Ԫ�أ���e������ֵ��
 * ����ֵ���ɹ�ɾ������OK�����򷵻�ERROR
 **/
status ListDelete(Linklist *L,int i,ElemType &e){
    LNode *p=L->head, *s;
    char j=1;
    if(L->head==NULL){
        printf("������Ԫ�أ�");   getchar();
        return ERROR;
    }
    if(i<1 || i>L->len) {
        printf("�±�Խ�磬");
        getchar();   return ERROR;
    }
    if(i==1){   //ɾ����Ԫʱ��ͷ���ָ������
        L->head=p->next;        e=p->data;  free(p);    p=NULL;
    }
    else{   //������λ��ɾ��Ԫ��
        for(;j<i-1;p=p->next,j++);
        s=p->next;        p->next=s->next;    //��ǰ�ڵ�ǰ��ָ������
        e=s->data;        free(s);  s=NULL;
    }
    L->len--;    return OK;
}
/**
 * �������ƣ�ListTrabverse
 * ������������ʽ��ָ��L
 * �������ܣ�������ʾ���Ա��е�ÿ��Ԫ�ء�
 * ����ֵ���ɹ������������Ա�ĳ��ȣ����򷵻�ERROR
 **/
status ListTrabverse(Linklist *L){
    LNode *p=L->head;
    if(L==NULL){
        printf("��δ��ʼ��");    getchar();
        return ERROR;
    }
    if(L->head==NULL){
        printf("������Ԫ�أ�");   getchar();
        return ERROR;
    }
   printf("\n-----------all elements -----------------------\n");
   for(;p!=NULL;p=p->next) printf("%d ",p->data);
   printf("\n------------------ end ------------------------\n");
   return L->len;
}
/**
 * �������ƣ�LoadList
 * �����������ļ�ָ�룬��ʽ��List
 * �������ܣ����ݼ��أ�Ϊ���Ա�����Ԫ�ء�
 * ����ֵ�����سɹ�����OK������ʧ�ܷ���ERROR
 **/
 status LoadList(FILE *fp,Linklist *List){
     int i,yl;
     Linklist *p;
     LNode *r;
     if(List==NULL){
        printf("��δ��ʼ��");    getchar();
        return ERROR;
    }
     printf("��������Ҫ�ָ����������ڵ��ļ���:");
     scanf("%s",input);
     filename=input;
     if ((fp=fopen(filename,"r"))==NULL)
     {
         printf("���ļ�ʧ��");  return ERROR;
     }
     fread(&yl,sizeof(int),1,fp);      //��ȡ��Ч�����
     length=yl;
     for(i=0,p=List; i<yl; i++,p=p->aft){
        p->aft =(Linklist*)malloc(sizeof(Linklist));
        if(!p->aft){
            printf("�ڴ����ʧ��");   return ERROR;
        }
        p->aft->fore = p;
        p->aft->aft = NULL;
        p->aft->head = p->aft->tail = NULL;
        fread(&(p->aft->len),sizeof(int),1,fp);  //����ÿ����ı�
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
     //������ļ��������ȡ����Ԫ�ػָ�˳������ڲ�ͬ������ṹ����ͨ����ȡ������Ԫ�ػָ��ڴ��е�����ṹ��
     fclose(fp);	 return OK;
 }
 /**
 * �������ƣ�SaveList
 * �����������ļ�ָ��fp����ʽ��ָ��List
 * �������ܣ���L�е����ݱ��浽fpָ��ָ����ļ���
 * ����ֵ��OK
 **/
 status SaveList(FILE *fp,Linklist *List){
    int yl=0;
    Linklist *p;
    LNode *q;
    if(List==NULL){
        printf("��δ��ʼ��");    getchar();
        return ERROR;
    }
    for(p=List->aft;p!=NULL;p=p->aft){      //ͳ����Ԫ�صı�ĸ���
        if(p->len!=0)       yl++;
    }
    if(yl ==0){       //����Ч����ʱ������
         printf("û�д�������");  getchar();
         return ERROR;
    }
    time_t lt;
    lt=time(NULL);
    filename=ctime(&lt);
    filename[strlen(filename)-1]='\0';
    strcat(filename,appe);      //��������ʱ���������ļ����ַ���
    pc=&filename[0];
    while(*pc){       //�ļ����ַ����淶��������:����' ' �á�_������
        if((*pc==':' ) ||   (*pc==' ')){   *pc='_';    }
        pc++;
    }
    if ((fp=fopen(filename,"w"))==NULL)
    {
        printf("�ļ���ʧ��");   getchar();getchar();
        return ERROR;
    }
    fwrite(&yl,sizeof(int),1,fp);   //д����Ч��ĸ���
    for(p=List->aft;p!=NULL;p=p->aft){      //д��ÿ����Ч��ı�
        if(p->len!=0)
            fwrite(&(p->len),sizeof(int),1,fp);
    }
    for(p=List->aft;p!=NULL;p=p->aft){      //ѭ��д��ÿ����Ч���Ԫ��
        if(p->len!=0)
            for(q=p->head;q!=NULL;q=q->next)
                fwrite(&(q->data),sizeof(ElemType),1,fp);
    }
    //ͨ�����������������Ԫ�ز�д�뵽�ļ���
    fclose(fp);    return OK;
}
/**
 * �������ƣ�ChooseList
 * ����������Ҫ�滻�ı�λ��vol����ʽ��ָ��L����ʽ��ָ��List
 * �������ܣ���Lָ��Ҫ�����ı��±�vol-1
 * ����ֵ��ʧ�ܷ���ERROR���ɹ�����OK
 **/
status ChooseList(int vol, Linklist **L, Linklist *List){
    Linklist *p;
    if(List==NULL){
        printf("��δ��ʼ��");    getchar();
        return ERROR;
    }
    if(vol<1 || vol>length+1){
        printf("�±�Խ�磬����ʧ�ܣ�");   getchar();
        return ERROR;
    }
    if(vol==length+1){
        for(p=List->aft;p->aft!=NULL;p=p->aft);
        p->aft=(Linklist*)malloc(sizeof(Linklist));
        if(!p->aft){
            printf("�ڴ����ʧ��");   return ERROR;
        }
        p->aft->aft = NULL;
        p->aft->head = p->aft->tail = NULL;   //��ʱͨ����ǰ��β�ڵ�����½��ڵ�
        p->aft->fore = p;
        p= p->aft;   //Lָ��β�ڵ�
        p->len = 0;    length++;   vol = length;
    }
    else{
        char i=1;
        for(p=List->aft;i<vol;p=p->aft,i++){}
    }
    *L=p;    return OK;
}
/**
 * �������ƣ�ChartTraverse
 * ������������ʽ��ָ��List
 * �������ܣ�������������������Ԫ��ʱ����˵��
 * ����ֵ��ʧ�ܷ���ERROR���ɹ�����OK
 **/
status ChartTraverse(Linklist *List){
    char i=0, flag=ERROR;
    Linklist *p;
    LNode *q;
    if(List==NULL){
        printf("��δ��ʼ��");    getchar();
        return ERROR;
    }
    printf("\n-----------all elements -----------------------\n");
    for(p=List->aft;p!=NULL;p=p->aft,i++){
        if(p->len==0){  //�ձ�
            printf("List[%d]����Ԫ��\n", i+1);
            continue;
        }
        for(q=p->head;q!=NULL;q=q->next) printf("%d ",q->data);
        flag=OK;
        putchar('\n');      //ÿ���������ɺ���
    }
    printf("\n------------------ end ------------------------\n");
    return flag;
}
