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
typedef int ElemType; //����Ԫ�����Ͷ���
typedef struct{  //˳���˳��ṹ���Ķ���
	ElemType * elem;
	int length;
	int listsize;
}SqList;
//��Ϊ20�������ж���Ӧ�ĸ������Ա��Ƿ��ʼ����TRUEΪ�ѳ�ʼ����FALSEΪδ��ʼ��
//vol���ڶ�λ��ǰ���������Ա�
int judge[20], vol=1;
ElemType *newbase;
FILE *fp;
char *filename, *pc, appe[5]=".dat", input[60]; //���������ļ���

status IntiaList(SqList *L);
status DestroyList(SqList *L);
status ClearList(SqList *L);
status ListEmpty(SqList *L);
int ListLength(SqList *L);
status GetElem(SqList *L,int i,ElemType &e);
status LocateElem(SqList *L,ElemType e); //�򻯹�
status PriorElem(SqList *L,ElemType cur,ElemType &pre_e);
status NextElem(SqList *L,ElemType cur,ElemType &next_e);
status ListInsert(SqList *L,int i,ElemType e);
status ListDelete(SqList *L,int i,ElemType &e);
status ListTrabverse(SqList *L);  //�򻯹�
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
	printf("ϵͳ�й�20������ǰ������%d����\n", vol);    //��ʾ��ǰ�����ı��λ��
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
	printf("    ��ѡ����Ĳ���[0~16]:");
	scanf("%d",&op);
    switch(op){
	   case 1:
		 if(IntiaList(L)==OK)
                printf("���Ա����ɹ���");
		     else printf("���Ա���ʧ�ܣ�");
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
            printf("���Ա�Ϊ�գ�");
         else if(i==FALSE)
            printf("���Ա�Ϊ�գ�");
        getchar();getchar();
		 break;
	   case 5:
         if(ListLength(L)==ERROR){
            getchar();getchar();
            break;
         }
		 printf("��Ϊ%d", L->length);
		 getchar();getchar();
		 break;
	   case 6:
         printf("������Ԫ��λ��i(1<=i<%d):",L->length+1);
         scanf("%d",&i);
         if(GetElem(L,i,e)==OK)
            printf("��%d��Ԫ��Ϊ%d", i, e);
		getchar();getchar();
		 break;
	   case 7:
         printf("�������������:");
         scanf("%d", &e);
         i=LocateElem(L,e);
         if(i>0)
            printf("%dΪ���е�%d��Ԫ��", e, i);
         if(!i)
            printf("δ�ҵ���Ԫ��");
		 getchar();getchar();
		 break;
	   case 8:
         printf("�����붨λ����:");
         scanf("%d", &cur);
         if(PriorElem(L,cur,pre_e)==OK)
            printf("%d��ǰ��Ϊ%d", cur, pre_e);
		 getchar();getchar();
		 break;
	   case 9:
         printf("�����붨λ����:");
		 scanf("%d", &cur);
         if(NextElem(L,cur,next_e)==OK)
            printf("%d�ĺ��Ϊ%d", cur, next_e);
		 getchar();getchar();
		 break;
	   case 10:
         printf("���������λ��i(1<=i<=%d):", L->length+1);
         scanf("%d", &i);
         printf("�������������e:");
         scanf("%d", &e);
         if(ListInsert(L,i,e)==ERROR)
            printf("����ʧ��");
         else
            printf("����ɹ�");
		 getchar();getchar();
		 break;
	   case 11:
		 printf("������ɾ��λ��i��1<=i<=%d��", L->length);
         scanf("%d", &i);
         if(ListDelete(L,i,e)==ERROR)
            printf("ɾ��ʧ��");
         else
            printf("ɾ���ɹ�,ɾ����Ԫ��Ϊ%d",e);
		 getchar();getchar();
		 break;
	   case 12:
		 if(ListTrabverse(L)==ERROR) printf("����ʧ�ܣ�\n");
		 getchar();getchar();
		 break;
       case 13:
         if(LoadList(fp,List)==OK){
             printf("�ļ����سɹ�");
         }
         getchar();getchar();
         break;
       case 14:
         if(SaveList(fp,List)==OK){
             printf("�ļ�����ɹ�");
         }
         getchar();getchar();
		 break;
       case 15:
         printf("������Ҫ�������λ��vol:(1<=vol<=20)");
         scanf("%d", &vol);
         ChooseList(vol, &L, List);
         getchar();
         break;
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
 * �������������Ա�ָ��L��List[]�ĵ�ַ
 * �������ܣ�����һ���յ����Ա�
 * ����ֵ���ɹ����췵��OK�����򷵻�ERROR
 **/
status IntiaList(SqList *L){
	L->elem = (ElemType *)malloc( LIST_INIT_SIZE * sizeof (ElemType));
    if(!L->elem) return ERROR;
	L->length=0;
    L->listsize=LIST_INIT_SIZE;     //��ʼ����ռ�
    judge[vol-1] = TRUE;            //��ǰ���ѳ�ʼ�����޸Ķ�Ӧ�ж�����
	return OK;
}
/**
 * �������ƣ�DestroyList
 * �������������Ա�ָ��L
 * �������ܣ�ɾ�����Ա�
 * ����ֵ���ɹ����ٷ���OK�����򷵻�ERROR
 **/
status DestroyList(SqList *L){
//����δ���������ж������Ԫ�أ���ֵΪһ���������һ��ΪFALSE��������ӦΪ��TRUE������˵��
    if(judge[vol-1] !=TRUE){
        printf("\n�˱�δ��ʼ����");
        getchar();
        return ERROR;
    }
    free(L->elem);
    L->length=L->listsize=0;
    //���ٱ�L������ȫ����Ϣ
    L=NULL;
    judge[vol-1] = FALSE;  //�޸Ķ�Ӧ�ж�����
    printf("���Ա�%d��ɾ����", vol);
    getchar();
    return OK;
}
/**
 * �������ƣ�ClearList
 * �������������Ա�ָ��L
 * �������ܣ��������Ա�
 * ����ֵ���ɹ��ÿշ���OK�����򷵻�ERROR
 **/
status ClearList(SqList *L){
    if(judge[vol-1]  !=TRUE){
        printf("�˱�δ��ʼ����");
        getchar();
        return ERROR;
    }
    L->length=0;                 //��ʼ��
    L->listsize=LIST_INIT_SIZE;  //��ʼ״̬�ռ��С
    printf("������գ�");
    getchar();
    return OK;
}
/**
 * �������ƣ�ListEmpty
 * �������������Ա�ָ��L
 * �������ܣ��ж����Ա��Ƿ�Ϊ��
 * ����ֵ����LΪ�ձ��򷵻�TRUE�����򷵻�FALSE
 **/
status ListEmpty(SqList *L){
    if(judge[vol-1]  !=TRUE){
        printf("�˱�δ��ʼ����");
        getchar();
        return ERROR;
    }
    int flag=TRUE;
    if(L->length!=0)
        flag = FALSE;   //����Ϊ0
    return flag;
}
/**
 * �������ƣ�ListLength
 * �������������Ա�ָ��L
 * �������ܣ��������Ա�L������Ԫ�ظ�����
 * ����ֵ���ɹ�����L������Ԫ�ظ�����ʧ�ܷ���ERROR
 **/
int ListLength(SqList *L){
    if(judge[vol-1] !=TRUE){
        printf("�˱���δ��ʼ��");
        getchar();
        return ERROR;
    }
    return L->length;
}
/**
 * �������ƣ�GetElem
 * �������������Ա�ָ��L��eΪ��i�����ݵĵ�ַ��
 * �������ܣ����Ҳ���ʾL�е�i��Ԫ�ص�ֵ��
 * ����ֵ���ɹ��򷵻ص�i��Ԫ�ص�ֵ�����򷵻�ERROR
 **/
status GetElem(SqList *L,int i,ElemType &e){
    if(judge[vol-1] !=TRUE){
        printf("�˱���δ��ʼ��");
        getchar();
        return ERROR;
    }
    if(ListEmpty(L)==TRUE){
        printf("\n������Ԫ�أ�");
        getchar();
        return ERROR;
    }
    if(i<1 || i>L->length+1){
        printf("�±�Խ�磡");
        getchar();
        return ERROR;
    }
    e=L->elem[i-1];
    return OK;
}
/**
 * �������ƣ�LocateElem
 * �������������Ա�ָ��L����Աȵ�Ԫ��ֵ��
 * �������ܣ�����L����e��ͬ�������ڵ�λ��λ���1��ʼ
 * ����ֵ���ɹ��򷵻ص�һ����e��ͬ�����ݵ�λ�򣬲������򷵻�0�������������ERROR
 **/
status LocateElem(SqList *L,ElemType e){
    int i;
    if(judge[vol-1] !=TRUE){
        printf("�˱���δ��ʼ��");
        getchar();
        return ERROR;
    }
    if(ListEmpty(L)==TRUE){
        printf("������Ԫ�أ�");
        getchar();
        return ERROR;
    }

    for(i=0;i<L->length;i++){
        if(L->elem[i] == e)
            return (i+1);   //�±�+1��Ϊλ��
    }
    return 0;  //δ�ҵ�Ԫ��
}
/**
 * �������ƣ�PriorElem
 * �������������Ա�ָ��L�����ҵ�����cur_e,ǰ��pre_e��
 * �������ܣ�����L����cur_e��ͬ�ĵ�һ�����ݣ���������ǰ��
 * ����ֵ���ɹ��򷵻���cur��ͬ�ĵ�һ�����ݣ����������һ��ǰ��������ǰ���Լ���������򷵻�ERROR
 **/
status PriorElem(SqList *L,ElemType cur,ElemType &pre_e){
    int i;
    if(judge[vol-1] !=TRUE){
        printf("�˱���δ��ʼ��");
        getchar();
        return ERROR;
    }
    if(ListEmpty(L)==TRUE){
        printf("������Ԫ�أ�");
        getchar();
        return ERROR;
    }
    for(i=0;i<L->length;i++){
        if(L->elem[i] == cur){   //�󲿷�ѭ��ֻ������һ��
            if(i==0){            //������Ԫ��ǰ��
                printf("��Ԫ����ǰ����");
                getchar();
                return ERROR;
            }
            else{
                pre_e = L->elem[i-1];
                return OK;
            }
        }
    }
    printf("δ�ҵ���Ԫ�أ�");
    getchar();
    return ERROR;
}
/**
 * �������ƣ�NextElem
 * �������������Ա�ָ��L�����ҵ�����cur,����next_e��
 * �������ܣ�����L����cur��ͬ�ĵ�һ�����ݣ�����������
 * ����ֵ���ɹ��򷵻���cur��ͬ�ĵ�һ�����ݣ����������̣����޺����Լ���������򷵻�ERROR
 **/
status NextElem(SqList *L,ElemType cur,ElemType &next_e){
    int i;
    if(judge[vol-1] !=TRUE){
        printf("�˱���δ��ʼ����");
        getchar();
        return ERROR;
    }
    if(ListEmpty(L)==TRUE){
        printf("������Ԫ�أ�");
        getchar();
        return ERROR;
    }
    for(i=0;i<L->length;i++){
        if(L->elem[i] == cur){   //�󲿷�ѭ��ֻ������һ��
            if(i!=L->length-1){
                next_e = L->elem[i+1];
                return OK;
            }
            else{               //����βԪ�޺��
                printf("��Ԫ���޺�̣�");
                getchar();
                return ERROR;
            }
        }
    }
    printf("δ�ҵ���Ԫ�أ�");
    return ERROR;
}
/**
 * �������ƣ�ListInsert
 * �������������Ա�ָ��L�������λ��i�����������Ԫ��e��
 * �������ܣ���L�ĵ�i��λ��֮ǰ�����µ�����Ԫ��e�����Ѳ��������շ����������������Ŀռ䡣
 * ����ֵ���ɹ����뷵��OK�����򷵻�ERROR
 **/
status ListInsert(SqList *L,int i,ElemType e){
    ElemType *p, *q;
    if(judge[vol-1] !=TRUE){
        printf("�˱���δ��ʼ����");
        getchar();   return ERROR;
    }
    if(i<1 || i >L->length+1) {
        printf("�±�Խ�磬");
        getchar();   return ERROR;
    }
    //Ԫ�ظ������ʱ�������ռ�
    if(L->length >= L->listsize){
        newbase = (ElemType *)realloc(L->elem, (L->listsize+LISTINCREMENT)* sizeof(ElemType));
        if(!newbase){
            printf("�ڴ����ʧ�ܣ�");
            getchar();   return ERROR;
        }
        L->elem = newbase;
        L->listsize += LISTINCREMENT;
    }
    q = &(L->elem[i-1]);
    for(p=&(L->elem[L->length-1]); p>=q; p--){
        *(p+1) = *p;    //�ӱ�β�����봦Ԫ������ǰ����Ϊ������Ԫ�������ռ�
    }
    *q = e;         //����Ԫ��
    L->length++;
    return OK;
}
/**
 * �������ƣ�ListDelete
 * �������������Ա�ָ��L��ɾ����λ��i��ָ��e�ĵ�ַ��
 * �������ܣ�ɾ��L�ĵ�i������Ԫ�أ���e������ֵ��
 * ����ֵ���ɹ�ɾ������OK�����򷵻�ERROR
 **/
status ListDelete(SqList *L,int i,ElemType &e){
    ElemType *p, *q;
    if(judge[vol-1] !=TRUE){
        printf("�˱���δ��ʼ����");
        getchar();
        return ERROR;
    }
    if(ListEmpty(L)==TRUE){
        printf("������Ԫ�أ�");
        getchar();
        return ERROR;
    }
    if(i<1 || i >L->length) {
        printf("�±�Խ�磬");   return ERROR;
    }
    q = &(L->elem[i-1]);
    e = L->elem[i-1];
    for(p=q;p<=&(L->elem[L->length-1]); p++){
        *(p) = *(p+1);  //�Ӳ��봦����βԪ�����κ���
    }
    L->length--;
    return OK;
}
/**
 * �������ƣ�ListTrabverse
 * �������������Ա�ָ��L
 * �������ܣ�������ʾ���Ա��е�ÿ��Ԫ�ء�
 * ����ֵ���ɹ������������Ա�ĳ��ȣ����򷵻�ERROR
 **/
status ListTrabverse(SqList *L){
    int i;
    if(judge[vol-1] !=TRUE){
        printf("�˱���δ��ʼ����");
        getchar();
        return ERROR;
    }
    if(ListEmpty(L)==TRUE){
        printf("������Ԫ�أ�");
        getchar();
        return ERROR;
    }
   printf("\n-----------all elements -----------------------\n");
   for(i=0;i<L->length;i++) printf("%d ",L->elem[i]);
   printf("\n------------------ end ------------------------\n");
   return L->length;
}
/**
 * �������ƣ�LoadList
 * �����������ļ�ָ�룬���Ա�List[20]
 * �������ܣ����ݼ��أ�Ϊ���Ա�����Ԫ�ء�
 * ����ֵ�����سɹ�����OK������ʧ�ܷ���ERROR
 **/
 status LoadList(FILE *fp,SqList List[20]){
     int i,j, yl;
     printf("��������Ҫ�ָ����������ڵ��ļ���:");
     scanf("%s",input);
     filename=input;
     if ((fp=fopen(filename,"r"))==NULL)
     {
         printf("���ļ�ʧ��");
         return ERROR;
     }
     fread(&yl,sizeof(int),1,fp);      //��ȡ��Ч�����
     for(i=0;i<yl;i++){
        IntiaList(&List[i]);           //ֻ��ʼ��yl����
        fread(&(List[i].length),sizeof(int),1,fp);  //����ÿ����ı�
        judge[i]=TRUE;                  //�ѳ�ʼ���ı��ж������޸�ΪTRUE
     }
     for(i=yl;i<20;i++){
        judge[i]=FALSE;         //δ��ʼ���ı��ж�����ͳһΪFALSE
     }
     for(i=0;i<yl;i++){
        for(j=0;j<List[i].length;j++){
            fread(&(List[i].elem[j]),sizeof(ElemType),1,fp);
            //Ԫ�ظ������ʱ�������ռ�
            if(List[i].length >= List[i].listsize){
                newbase = (ElemType *)realloc(List[i].elem, (List[i].listsize+LISTINCREMENT)* sizeof(ElemType));
                if(!newbase){
                    printf("�ڴ����ʧ��");   return ERROR;
                }
                List[i].elem = newbase;
                List[i].listsize += LISTINCREMENT;
            }
        }
     }
     //������ļ��������ȡ����Ԫ�ػָ�˳������ڲ�ͬ������ṹ����ͨ����ȡ������Ԫ�ػָ��ڴ��е�����ṹ��
     fclose(fp);
	 return OK;
 }
 /**
 * �������ƣ�SaveList
 * �����������ļ�ָ�룬���Ա�List[20]
 * �������ܣ���L�е����ݱ��浽fpָ��ָ����ļ���
 * ����ֵ��OK
 **/
 status SaveList(FILE *fp,SqList List[20]){
    int i,j,yl=0;
    for(i=0;i<20;i++){      //ͳ����Ԫ�صı�ĸ���
        if(judge[i]==TRUE && List[i].length!=0)
            yl++;
    }
    if(yl ==0){       //����Ч����ʱ������
         printf("û�д�������");
         getchar();
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
        printf("�ļ���ʧ��");
        getchar();getchar();
        return ERROR;
    }
    fwrite(&yl,sizeof(int),1,fp);   //д����Ч��ĸ���
    for(i=0;i<20;i++){      //д��ÿ����Ч��ı�
        if(judge[i]==TRUE && List[i].length!=0)
            fwrite(&(List[i].length),sizeof(int),1,fp);
    }
    for(i=0;i<20;i++){      //ѭ��д��ÿ����Ч���Ԫ��
        if(judge[i]==TRUE && List[i].length!=0)
            for(j=0;j<List[i].length;j++)
                fwrite(&(List[i].elem[j]),sizeof(ElemType),1,fp);
    }

    //ͨ�����������������Ԫ�ز�д�뵽�ļ���
    fclose(fp);
    return OK;
}
/**
 * �������ƣ�ChooseList
 * ����������Ҫ�滻�ı�λ��vol���ļ�ָ�룬���Ա�List[20]
 * �������ܣ���Lָ��Ҫ�����ı��±�vol-1
 * ����ֵ��ʧ�ܷ���ERROR���ɹ�����OK
 **/
status ChooseList(int vol, SqList **L, SqList List[20]){
    if(vol<1 || vol>20){
        printf("�±�Խ�磬����ʧ�ܣ�");
        getchar();
        return ERROR;
    }
    *L=&(List[vol-1]);
    if(judge[vol-1]==FALSE){
        printf("�˱���δ��ʼ��");    //��ֹ�����
        getchar();  getchar();
    }
    return OK;
}
/**
 * �������ƣ�ChartTraverse
 * ����������Ҫ�滻�ı�λ��vol���ļ�ָ�룬���Ա�List[20]
 * �������ܣ���Lָ��Ҫ�����ı��±�vol-1
 * ����ֵ��ʧ�ܷ���ERROR���ɹ�����OK
 **/
status ChartTraverse(SqList List[20]){
    int i,j,flag=ERROR;
    printf("\n-----------all elements -----------------------\n");
    for(i=0; i<20; i++){
        if(judge[i] !=TRUE){    //δ��ʼ��
            printf("List[%d]δ��ʼ��\n", i);
            continue;
        }
        if(List[i].length==0){  //�ձ�
            printf("List[%d]����Ԫ��\n", i);
            continue;
        }
        for(j=0;j<List[i].length;j++) printf("%d ",List[i].elem[j]);
        flag=OK;
        putchar('\n');      //ÿ���������ɺ���
    }
    printf("\n------------------ end ------------------------\n");
    return flag;
}
