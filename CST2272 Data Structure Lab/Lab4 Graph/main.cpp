#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//�������״̬����
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR -1

typedef int Status;     //�Ǻ��������ͣ���ֵ�Ǻ������״̬����
typedef int QElemType;  //��Ӧ��������±�
typedef struct QNode{
	QElemType data;     //�ڵ���Ϣ
	struct QNode *next;
}QNode,*QueuePtr;   //���нڵ�ṹ
typedef struct{
	QueuePtr front,rear; //��ͷ����βָ��
}LinkQueue;     //���нṹ

/**
 * �������ƣ�InitQueue
 * �������������нṹָ��Q
 * �������ܣ���ʼ��һ���ն���
 * ����ֵ��ʧ�ܷ���ERROR���ɹ�����OK
 **/
Status InitQueue(LinkQueue *Q)
{
    //����һ���ն���Q
	(*Q).front = (*Q).rear = (QueuePtr)malloc(sizeof(QNode));
	if(!(*Q).front) {
	    printf("����ռ�ʧ��");            return ERROR;
	}
	(*Q).front->next = NULL;
	return OK;
}

/**
 * �������ƣ�QueueEmpty
 * �������������нṹQ
 * �������ܣ��ж϶����Ƿ�Ϊ��
 * ����ֵ���Ƿ���TRUE���ɹ�����FALSE
 **/
Status QueueEmpty(LinkQueue Q)
{
    //��QΪ�ն���,�򷵻�TRUE,���򷵻�FALSE
	if(Q.front == Q.rear) return TRUE;
	else return FALSE;
}

/**
 * �������ƣ�EnQueue
 * �������������нṹָ��Q�������±�e
 * �������ܣ����±�������
 * ����ֵ��ʧ�ܷ���ERROR���ɹ�����OK
 **/
Status EnQueue(LinkQueue *Q, QElemType e)
{
    //����Ԫ��eΪQ���µĶ�βԪ��
	QueuePtr p = (QueuePtr)malloc(sizeof(QNode));
	if(!p) //�洢����ʧ��
		return ERROR;
	p->data = e;
	p->next = NULL;
	(*Q).rear->next = p;
	(*Q).rear = p;
	return OK;
}

/**
 * �������ƣ�DeQueue
 * �������������нṹָ��Q�������±�ָ��e
 * �������ܣ����±�Ӷ���ȡ��
 * ����ֵ��ʧ�ܷ���ERROR���ɹ�����OK
 **/
Status DeQueue(LinkQueue *Q, QElemType *e)
{
    //�����в���,ɾ��Q�Ķ�ͷԪ��,��e������ֵ,������OK,���򷵻�ERROR
	QueuePtr p;
	if((*Q).front == (*Q).rear){
        printf("������Ϊ��");    return ERROR;
	}
	p = (*Q).front->next;
	*e = p->data;
	(*Q).front->next = p->next;
	if((*Q).rear == p)
		(*Q).rear = (*Q).front;
	free(p);
	return OK;
}

#define KIND 3  //ͼ��Ĭ�����ࣺ0-����ͼ, 1-������, 2-����ͼ, 3-������
#define COLUMN 5    //ͼ�ĸ���
#define MAX_VERTEX_NUM 20 //��󶥵���
#define MAX_VERNAME_LEN 10 //����������󳤶�
//����Ԫ�����Ͷ���
typedef int Boolean;
typedef int InfoType;   //���л���
typedef char VertexType;    //�ڵ�����
typedef struct ArcNode{
	int adjvex;               //�û���ָ��Ķ���λ��
	struct ArcNode * nextarc; //ָ����һ������ָ��
	InfoType *info;           //�û������Ϣ��ָ��
}ArcNode;                     //�����ṹ
typedef struct VNode{
	ArcNode * firstarc;             //ָ���һ�������ö���Ļ���ָ��
	char data;                      //������Ϣ
}VNode, AdjList[MAX_VERTEX_NUM];    //������ṹ
typedef struct{
	AdjList vertices;
	int vexnum, arcnum;       //ͼ�ĵ�ǰ�������ͻ���
	int kind;                 //ͼ�������־
}ALGraph;                     //ͼ�ṹ
Boolean judge[COLUMN], flag=FALSE;
Boolean visited[COLUMN][MAX_VERTEX_NUM];  //���ʱ�־����
int col=1;  //�����ı����ţ���ʼΪ1
FILE *fp;
char *filename, *pc, appe[5]=".dat", input[60]; //���������ļ���
//Status (*VisitFunc)(VertexType a);  //��������

Status CreateGraph(ALGraph *G);  //��V��VR�Ķ��幹��ͼG
Status DestroyGraph(ALGraph *G);  //����ͼG
int LocateVex(ALGraph *G, VertexType u);  //��u��ͼG�д��ڣ����ض���u��λ����ţ����򷵻�-1
char GetVex(ALGraph *G, int rec);
Status PutVex(ALGraph *G, int rec, VertexType v);
int FirstAdjVex(ALGraph *G, VertexType v);  //����v�ĵ�һ���ڽӶ��㣬���vû���ڽӶ��㣬���ؿ�
int NextAdjVex(ALGraph *G, VertexType v, VertexType w);  //����v�ģ������w����һ���ڽӶ��㣬���w�����һ���ڽӶ��㣬���ؿա�
Status InsertVex(ALGraph *G, VertexType v);  //��ͼG�������¶���v
Status DeleteVex(ALGraph *G, VertexType v);  //��ͼG��ɾ������v����v��صĻ�
Status InsertArc(ALGraph *G, VertexType v, VertexType w);  //��ͼG�����ӻ�<v,w>�����ͼG������ͼ������Ҫ����<w,v>
Status DeleteArc(ALGraph *G, VertexType v, VertexType w);  //��ͼG��ɾ����<v,w>�����ͼG������ͼ������Ҫɾ��<w,v>
Status DFSTraverse(ALGraph *G);//��ͼG�������������������
Status BFSTraverse(ALGraph *G);  //��ͼG���й��������������
int OutDegree(AdjList i);  //����ͼG��i������ĳ���
Status SaveGraph(ALGraph List[]);  //����
Status LoadGraph(ALGraph List[]);  //��ȡ
Status ChooseList(int col, ALGraph **G, ALGraph List[]);

int main(){
    int op, rec;
    VertexType e,v,w;
    ALGraph *G=NULL, List[COLUMN];
    G=List;
    while(op){
        system("cls");       printf("\n\n");
        printf("��ϵͳ���������� ϵͳ�й�%d��ͼ ��ǰ������%d��\n", COLUMN, col);
        printf("      Menu for Linear Table On Node Structure \n");
        printf("-------------------------------------------------\n");
        printf("    	  1. CreateGraph      9. DeleteVex\n");
        printf("    	  2. DestroyGraph     10. InsertArc\n");
        printf("    	  3. LocateVex        11. DeleteArc\n");
        printf("    	  4. GetVex           12. DFSTraverse\n");
        printf("    	  5. PutVex           13. BFSTraverse\n");
        printf("    	  6. FirstAdjVex      14. SaveGraph\n");
        printf("    	  7. NextAdjVex       15. LoadGraph\n");
        printf("    	  8. InsertVex        16. ChooseGraph\n");
        printf("    	  0. Exit\n");
        printf("-------------------------------------------------\n");
        printf("    ��ѡ����Ĳ���[0~16]:");
        scanf("%d",&op);
        switch(op){
           	case 1:
             	if(CreateGraph(G)==OK) printf("ͼ�����ɹ�\n");
             	else printf("ͼ����ʧ��\n");
             	getchar();  getchar();      break;
           	case 2:
             	if(DestroyGraph(G)==OK) printf("ͼ���ٳɹ�\n");
             	else printf("ͼ����ʧ��\n");
             	getchar();  getchar();      break;
           	case 3:
           		printf("������Ҫ���ҵĶ���e������(char��):");
             	fflush(stdin);
             	scanf("%c",&e);  getchar();
             	rec = LocateVex(G,e);
             	if(rec==-1) printf("�����ڴ˶���\n");
             	else printf("�ö�����ͼ�����е����Ϊ%d\n",rec);
             	getchar();  getchar();      break;
            case 4:
                printf("������Ҫ���ҵĶ���rec���±�(int��):");
             	fflush(stdin);
             	scanf("%d",&rec);  getchar();
             	e = GetVex(G,rec);
             	if(e==ERROR)     printf("����ʧ��\n");
             	else    printf("%d������Ϊ%c\n", rec, e);
             	getchar();  getchar();      break;
            case 5:
                printf("������Ҫ�޸ĵĶ���rec�����(int��):");
             	fflush(stdin);
             	scanf("%d",&rec);  getchar();
             	printf("�����µ�����v(char��):");
             	scanf("%c", &v);  getchar();
             	if(PutVex(G,rec,v)==OK)     printf("�޸ĳɹ�\n");
             	else    printf("�޸�ʧ��\n");
             	getchar();  getchar();      break;
          	case 6:
          	    flag = FALSE;
             	printf("������Ҫ���ҵĶ���e������(char��):");
             	fflush(stdin);
             	scanf("%c",&e);  getchar();
             	rec = FirstAdjVex(G,e);
             	if(rec == ERROR){
                    if(flag==TRUE){
                        printf("vû���ڽӶ��� ");     flag = FALSE;
                    }
                    printf("����ʧ��\n");
             	}
             	else
                    printf("v�ĵ�һ���ڽӶ�������Ϊ%d\n",rec);
             	getchar();  getchar();      break;
           	case 7:
           	    flag = FALSE;
             	printf("������Ҫ���ҵĶ���v������ԵĶ���w������(char��):");
             	fflush(stdin);
             	scanf("%c", &v);  getchar();
             	scanf("%c", &w);  getchar();
             	rec = NextAdjVex(G,v,w);
             	if(rec==ERROR){
                    if(flag==TRUE){
                        printf("w��v�����һ���ڽӶ��� ");    flag = FALSE;
                    }
                    printf("����ʧ��\n");
             	}
             	else        printf("v�����w����һ���ڽӶ�������Ϊ%d\n",rec);
             	getchar();  getchar();      break;
           	case 8:
             	printf("������Ҫ��ӵĶ���v������(char��):");
             	fflush(stdin);
             	scanf("%c", &v);  getchar();
             	if(InsertVex(G,v)==OK) printf("��ӳɹ�\n");
             	else printf("���ʧ��\n");
             	getchar();  getchar();      break;
           	case 9:
            	printf("������Ҫɾ���Ķ���v������(char��):");
             	fflush(stdin);
             	scanf("%c", &v);  getchar();
             	if(DeleteVex(G,v)==ERROR) printf("�����ڴ˶���\n");
             	else printf("ɾ���ɹ�\n");
             	getchar();  getchar();      break;
           	case 10:
           		printf("������Ҫ��ӻ��Ķ���v�Ͷ���w������(char��):");
             	fflush(stdin);
             	scanf("%c", &v);  getchar();
             	scanf("%c", &w);  getchar();
             	if(InsertArc(G,v,w)==ERROR) printf("����v��w������");
             	else printf("��ӳɹ�\n");
             	getchar();  getchar();      break;
           	case 11:
           		printf("������Ҫɾ�����Ķ���v�Ͷ���w������(char��):");
             	fflush(stdin);
             	scanf("%c", &v);  getchar();
             	scanf("%c", &w);  getchar();
             	if(DeleteArc(G,v,w)==ERROR) printf("����v��w������");
             	else printf("��ɾ���ɹ�");
             	getchar();  getchar();      break;
            case 12:
            	DFSTraverse(G);
            	getchar();  getchar();      break;
            case 13:
            	BFSTraverse(G);
            	getchar();  getchar();      break;
            case 14:
                if(SaveGraph(List)==OK)      printf("����ɹ�\n");
                else    printf("����ʧ��\n");
                getchar();  getchar();      break;
            case 15:
                if(LoadGraph(List)==OK)   printf("���سɹ�\n");
                else    printf("����ʧ��\n");
                getchar();  getchar();      break;
            case 16:
                printf("������Ҫ�������λ��vol:(1<=col<=%d)", COLUMN);
                scanf("%d", &col);
                ChooseList(col, &G, List);
                getchar();  getchar();
           	case 0:
             	break;
        }//end of switch
    }//end of while
    return 0;
}

/**
 * �������ƣ�CreateGraph
 * ����������ͼ�ṹָ��G
 * �������ܣ�����һ��ͼG
 * ����ֵ���ɹ����췵��OK�����򷵻�ERROR
 **/
Status CreateGraph(ALGraph *G){
	int i,j,k;
	int w; //ȨֵW
	VertexType va, vb;//��βVa����ͷVb
	ArcNode *p;
	/*
    printf("������ͼ��3�������Ϣ\n����(0-����ͼ, 1-������, 2-����ͼ, 3-������):\n");
	//��ͼ����
	scanf("%d",&(*G).kind);          getchar();
	while ((*G).kind < 0 || (*G).kind > 3)
	{
		printf("����ֵ�Ƿ� ������:\n");
		scanf("%d",&(*G).kind);     getchar();
	}
	*/
	if(judge[col-1]==TRUE){
        printf("��ǰλ������ͼ ���������ٽ�����ͼ ");    return ERROR;
	}
	G->kind= KIND;    //Ĭ��Ϊ������
	//�䶥����
	printf("��������%d���ڣ�:", MAX_VERTEX_NUM);
	scanf("%d", &(G->vexnum));   getchar();
	while (G->vexnum < 0 || G->vexnum > MAX_VERTEX_NUM)
	{
		printf("����ֵ�Ƿ� ������:");
		scanf("%d",&(G->vexnum));   getchar();
	}
	if(G->vexnum)   judge[col-1] = TRUE;
	//�����
	printf("��(��)��:");
	scanf("%d", &(G->arcnum));      getchar();
	printf("������%d����������ƻ�ֵ(1���ַ�,�ո�ֿ�):", (*G).vexnum);
	for (i = 0; i < G->vexnum; i++) //�����������Ϣ
	{
		scanf("%c",&(G->vertices[i].data));  getchar();
		G->vertices[i].firstarc = NULL;
	}
	if (G->kind % 2) //1��3Ϊ��
        printf("�밴�����뻡(��)ȨֵW����βVa�ͻ�ͷVb(�ո�ֿ�):\n");
	else //0��2Ϊͼ
		printf("�밴�����뻡(��)�Ļ�βVa�ͻ�ͷVb(�ո�ֿ�):\n");
	for (k = 0; k < G->arcnum; k++) //�����ڽӱ�
	{
		printf("%d:", k+1);
		if (G->kind % 2) //��
			scanf("%d %c %c", &w, &va, &vb);
		else //ͼ
			scanf("%c %c", &va, &vb);
		i = LocateVex(G, va); //��β
		j = LocateVex(G, vb); //��ͷ
		if (i == -1 || j == -1)
		{//���㲻����
			printf("���㲻���� ������\n");
			k--;		continue;
		}
		p = (ArcNode *)malloc(sizeof(ArcNode));
		p->adjvex = j;
		if ((*G).kind % 2)//��
		{
			p->info = (int *)malloc(sizeof(int));
			*(p->info) = w;
		}
		else  //ͼ
			p->info = NULL;     //
		p->nextarc = (*G).vertices[i].firstarc; //���ڱ�ͷ������ȳ�ʽ
		G->vertices[i].firstarc = p;
		//����ͼ�����������������
		if(G->kind >= 2)
		{
			p = (ArcNode*)malloc(sizeof(ArcNode));
			p->adjvex = i;
			if ((*G).kind == 3) //������
			{
				p->info = (int*)malloc(sizeof(int));
				*(p->info) = w;
			}
			else //����ͼ
				p->info = NULL;
			p->nextarc = (*G).vertices[j].firstarc; //���ڱ�ͷ
			G->vertices[j].firstarc = p;
		}
	}
	return OK;
}

/**
 * �������ƣ�DestroyGraph
 * ����������ͼ�ṹָ��G
 * �������ܣ�����ͼG
 * ����ֵ���ɹ����ٷ���OK�����򷵻�ERROR
 **/
Status DestroyGraph(ALGraph *G){
    if(judge[col-1]!=TRUE){
        printf("ͼδ��ʼ�� ");   return ERROR;
    }
	int i;
	ArcNode *p, *q;
	for(i = 0; i < G->vexnum; i++)
	{
		p=G->vertices[i].firstarc;
		while(p)
		{
			q = p->nextarc;
			if((*G).kind % 2) //��
				free(p->info);
			free(p);
			p = q;
		}
	}
	G->vexnum = G->arcnum = 0;
	judge[col-1]=FALSE;
	return OK;
}

/**
 * �������ƣ�LocateVex
 * ����������ͼ�ṹָ��G�����鶥������u
 * �������ܣ�������Ϊu�Ķ���
 * ����ֵ���ҵ����ؽ���±꣬���򷵻�ERROR����-1��
 **/
int LocateVex(ALGraph *G, VertexType u){
    if(judge[col-1]!=TRUE){
        printf("ͼδ��ʼ�� ");   return ERROR;
    }
    if(G->vexnum == 0){
        printf("ͼΪ�� ");     return ERROR;
    }
	int i;
	for(i=0; i < G->vexnum; i++)
		if(u == G->vertices[i].data)
			return i;
	return -1;//δ�ҵ��򷵻�-1
}

/**
 * �������ƣ�GetVex
 * ����������ͼ�ṹָ��G�����鶥���±�rec
 * �������ܣ�����±�Ϊrec�Ķ��������
 * ����ֵ���ҵ����ؽ�����ƣ����򷵻�ERROR
 **/
VertexType GetVex(ALGraph *G, int rec){
    if(judge[col-1]!=TRUE){
        printf("ͼδ��ʼ�� ");   return ERROR;
    }
    if(G->vexnum == 0){
        printf("ͼΪ�� ");     return ERROR;
    }
    if(rec>= G->vexnum){
        printf("�±�Խ�� ");    return ERROR;
    }
    return G->vertices[rec].data;
}

/**
 * �������ƣ�PutVex
 * ����������ͼ�ṹָ��G��Ҫ�޸ĵĶ�����±�rec����Ӧ���������v
 * �������ܣ����±�Ϊrec�Ķ��㸳��Ϊu
 * ����ֵ���ҵ�����OK�����򷵻�ERROR
 **/
Status PutVex(ALGraph *G, int rec, VertexType v){
    if(judge[col-1]!=TRUE){
        printf("ͼδ��ʼ�� ");   return ERROR;
    }
    if(G->vexnum == 0){
        printf("ͼΪ�� ");     return ERROR;
    }
    if(rec>= G->vexnum){
        printf("�±�Խ�� ");    return ERROR;
    }
    G->vertices[rec].data = v;
    return OK;
}

/**
 * �������ƣ�FirstAdjVex
 * ����������ͼ�ṹָ��G�����鶥������v
 * �������ܣ�������Ϊv�Ľ��ĵ�һ�ڽӽ���±�
 * ����ֵ���ҵ������±�ֵ�����򷵻�ERROR
 **/
int FirstAdjVex(ALGraph *G, VertexType v){
	ArcNode *p;
	int v1;
	v1 = LocateVex(G,v);//��ȡv��ͼG�е����v1
    if(judge[col-1]!=TRUE){
        printf("ͼδ��ʼ�� ");   return ERROR;
    }
    if(G->vexnum == 0){
        printf("ͼΪ�� ");     return ERROR;
    }
	if (v1 == -1){
	    printf("�����ڴ˶��� ");		return ERROR;
	}
	p = G->vertices[v1].firstarc;
	if (p)		return p->adjvex;
	else
	{	    flag = TRUE;		return ERROR;	}
}

/**
 * �������ƣ�NextAdjVex
 * ����������ͼ�ṹָ��G�����鶥������v�� v���ڽӶ�������w
 * �������ܣ�������Ϊv��(�������Ϊw�Ķ����)��һ���ڽӶ���
 * ����ֵ��v��(�����w��)��һ���ڽӶ������ţ�
 *			��w��v�����һ���ڽӵ�����ʧ��,�򷵻�ERROR
 **/
int NextAdjVex(ALGraph *G,VertexType v,VertexType w){
	ArcNode *p;
	int v1, w1;
    if(judge[col-1]!=TRUE){
        printf("ͼδ��ʼ�� ");   return ERROR;
    }
    if(G->vexnum == 0){
        printf("ͼΪ�� ");     return ERROR;
    }
	v1 = LocateVex(G,v); //��ȡv��ͼG�е����v1
	w1 = LocateVex(G,w); //��ȡw��ͼG�е����w1
	if (v1 == -1){//���㲻����
	    printf("����v������ ");		return ERROR;
	}
	if (w1 == -1){//�ڽӵ㲻����
	    printf("����w������ ");		return ERROR;
	}
	p = G->vertices[v1].firstarc;
	while (p && p->adjvex != w1)//ָ��p��������ָ���㲻��w
		p = p->nextarc;
	//û�ҵ�w��w�����һ���ڽӵ�
	if(!p){
        printf("w����v���ڽӶ��� ");  return ERROR;
	}
	else if(!p->nextarc){
        flag=TRUE;  return ERROR;       //flag���������Ǵ����������û��Ǵ������������ã��Ծ����Ƿ���������ʾ��Ϣ
	}
	else    return p->nextarc->adjvex;
	//����v��(�����w��)��һ���ڽӶ�������
}

/**
 * �������ƣ�InsertVex
 * ����������ͼ�ṹָ��G���¶�������v
 * �������ܣ���ͼG��������Ϊv���¶���
 * ����ֵ���ɹ�����OK�����򷵻�ERROR
 **/
Status InsertVex(ALGraph *G, VertexType v){
    if(judge[col-1]!=TRUE){
        printf("ͼδ��ʼ�� ");   return ERROR;
    }
    if((*G).vexnum == 0){
        printf("ͼΪ�� ");     return ERROR;
    }
	if((*G).vexnum == MAX_VERTEX_NUM){//�����������ֵ
        printf("�����������ֵ ");     return ERROR;
	}
	//�����¶���
	(*G).vertices[(*G).vexnum].data = v;
	(*G).vertices[(*G).vexnum].firstarc=NULL;
	(*G).vexnum++; //ͼG�Ķ�������1
	return OK;
 }

/**
 * �������ƣ�DeleteVex
 * ����������ͼ�ṹָ��G����ɾ��������v
 * �������ܣ�ɾ��G����Ϊv�Ķ��㼰����ػ�
 * ����ֵ���ɹ�����OK�����򷵻�ERROR
 **/
Status DeleteVex(ALGraph *G, VertexType v){
	int i, j;
	ArcNode *p, *q;
    if(judge[col-1]!=TRUE){
        printf("ͼδ��ʼ�� ");   return ERROR;
    }
    if((*G).vexnum == 0){
        printf("ͼΪ�� ");     return ERROR;
    }
	j = LocateVex(G, v); //��ȡ����v�����
	if(j == -1)     return ERROR;//v����ͼG�Ķ���{
	//ɾ����vΪ���ȵĻ����
    p = (*G).vertices[j].firstarc;
	while(p)
	{
		q = p;
		p = p->nextarc;
		if((*G).kind % 2) //��
			free(q->info);
		free(q);
		(*G).arcnum--; //���������1
	}
	//�������ж���
	(*G).vexnum--; //��������1
	for(i = j; i < (*G).vexnum; i++) //����v����Ķ���ǰ��
		(*G).vertices[i] = (*G).vertices[i+1];
	//ɾ����vΪ��ȵĻ�������޸Ļ���ָ����λ��ֵ
	for(i = 0; i < (*G).vexnum; i++)
	{
		p = (*G).vertices[i].firstarc; //ָ���1�������
		while(p) //�л����
		{
			if(p->adjvex == j)//�û�ָ��ɾ���
			{
				if(p == (*G).vertices[i].firstarc) //��ɾ����ǵ�1�����
				{
					(*G).vertices[i].firstarc = p->nextarc;
					if((*G).kind%2) //��
						free(p->info);
					free(p);
					p = (*G).vertices[i].firstarc;
					if((*G).kind < 2)//����ͼ���ټ�һ��
						(*G).arcnum--; //���������1
				}
				else
				{
					q->nextarc = p->nextarc;
					if((*G).kind % 2) //��
						free(p->info);
					free(p);
					p = q->nextarc;
					if((*G).kind < 2)//�������ټ�һ��
						(*G).arcnum--; //���������1
				}
			}
			else//�û���ָ��ɾ���
			{
				if(p->adjvex > j)//�û���ָԪ��λ�������ɾ������֮��
					p->adjvex--;    //���Ļ���ָ����λ�����
				q = p;
				p = p->nextarc;//������һ����
			}
		}
	}
	if(G->vexnum==0)        printf("ͼ�����޶���");
	return OK;
}

/**
 * �������ƣ�InsertArc
 * ����������ͼ�ṹָ��G����������v��w
 * �������ܣ���ͼG�����ӻ�<v,w>�����ͼG������ͼ/��������Ҫ����<w,v>
 * ����ֵ���ɹ�����OK�����򷵻�ERROR
 **/
Status InsertArc(ALGraph *G,VertexType v,VertexType w){
	ArcNode *p;
	int w1, i, j;
    if(judge[col-1]!=TRUE){
        printf("ͼδ��ʼ�� ");   return ERROR;
    }
    if((*G).vexnum == 0){
        printf("ͼΪ�� ");     return ERROR;
    }
	i = LocateVex(G, v); //��ȡvλ�����
	j = LocateVex(G, w); //��ȡwλ�����
	if(i == -1 || j == -1)      return ERROR;//δ�ҵ���Ӧ����
	//������v-w
	(*G).arcnum++;
	if((*G).kind % 2) //��Ҫ��Ȩֵ
	{
		printf("�����뻡(��)%c->%c��Ȩֵ:\n", v, w);
		scanf("%d", &w1);   getchar();
	}
	p=(ArcNode*)malloc(sizeof(ArcNode));
	p->adjvex = j;
	if((*G).kind % 2) //��
	{
		p->info = (int *)malloc(sizeof(int));
		*(p->info) = w1;
	}
	else		p->info=NULL;
	p->nextarc = (*G).vertices[i].firstarc;
	(*G).vertices[i].firstarc = p;
	//����ͼ��Ҫ�ٽ�����w->v
	if((*G).kind >= 2)
	{
		p = (ArcNode*)malloc(sizeof(ArcNode));
		p->adjvex = i;
		if((*G).kind == 3) //������
		{
			p->info = (int*)malloc(sizeof(int));
			*(p->info) = w1;
		}
		else			p->info = NULL;
		p->nextarc = (*G).vertices[j].firstarc;
		(*G).vertices[j].firstarc = p;
	}
	return OK;
}

/**
 * �������ƣ�DeleteArc
 * ����������ͼ�ṹָ��G����ɾ������������v��w
 * �������ܣ���ͼG��ɾ����<v,w>�����ͼG������ͼ/��������Ҫ����<w,v>
 * ����ֵ���ɹ�����OK�����򷵻�ERROR
 **/
Status DeleteArc(ALGraph *G,VertexType v,VertexType w){
	ArcNode *p, *q;
	int i, j;
	if(judge[col-1]!=TRUE){
        printf("ͼδ��ʼ�� ");   return ERROR;
    }
    if((*G).vexnum == 0){
        printf("ͼΪ�� ");     return ERROR;
    }
    if((*G).arcnum == 0){
        printf("ͼ�����޻� ");     return ERROR;
    }
	i = LocateVex(G, v); //����v���
	j = LocateVex(G, w); //����w���
	if(i < 0 || j < 0)		return ERROR;
	//ɾ����<v,w>
	p = (*G).vertices[i].firstarc;
	while(p && p->adjvex != j) //p��������ָ֮�����Ǵ�ɾ����<v,w>
	{ //pָ����һ����
		q = p;		p = p->nextarc;
	}
	if(p && p->adjvex == j) //�ҵ���<v,w>
	{
		if(p == (*G).vertices[i].firstarc) //p��ָ�ǵ�1����
			(*G).vertices[i].firstarc = p->nextarc;     //ָ����һ����
		else	q->nextarc = p->nextarc;    //ָ����һ����
		if((*G).kind % 2)   free(p->info);  //��Ҫ���ͷ�Ȩֵ
		free(p); //�ͷŴ˽��
		(*G).arcnum--; //���������1
	}
	//����ͼҪɾ���Գƻ�<w,v>
	if((*G).kind >= 2)
	{
		p = (*G).vertices[j].firstarc;
		while(p && p->adjvex != i) //p��������ָ֮�����Ǵ�ɾ����<w,v>
		{ //pָ����һ���� */
			q = p;			p = p->nextarc;
		}
		if(p && p->adjvex == i) //�ҵ���<w,v>
		{
			if(p == (*G).vertices[j].firstarc) //p��ָ�ǵ�1����
				(*G).vertices[j].firstarc=p->nextarc;   //ָ����һ����
			else		q->nextarc=p->nextarc;      //ָ����һ����
			if((*G).kind % 2)   free(p->info);      //��Ҫ���ͷ�Ȩֵ
			free(p); //�ͷŴ˽��
		}
	}
	if((*G).arcnum == 0)     printf("ͼ�����޻� ");
	return OK;
 }

/**
 * �������ƣ�DFS
 * ����������ͼ�ṹָ��G�������±�v
 * �������ܣ��ӵ�v����������ݹ��������ȱ���ͼG
 * ����ֵ����
 **/
void DFS(ALGraph *G,int v){
	int w;
	VertexType v1, w1;
	v1 = G->vertices[v].data;
	visited[col-1][v] = TRUE; //���÷��ʱ�־ΪTRUE(�ѷ���)
	printf("���:%d-����:%c  ", v, G->vertices[v].data); /* ���ʵ�v������ */
	for(w = FirstAdjVex(G,v1); w >= 0; w = NextAdjVex(G, v1, w1=G->vertices[w].data))
		if(!visited[col-1][w])
			DFS(G,w); //��v����δ���ʵ��ڽӵ�w�ݹ����DFS
}

/*�������ƣ� DFSTraverse
*��������� ͼG, ���ú���visit
*����ֵ��  Status �ɹ�����OK��
*�������ܣ� ��ͼG������������������������ζ�ͼ�е�ÿһ������ʹ�ú�
*				��visit����һ�Σ��ҽ�����һ��
*/

/**
 * �������ƣ�DFSTraverse
 * ����������ͼ�ṹָ��G
 * �������ܣ���ͼG������������������������ζ�ͼ�е�ÿһ������ʹ�ú�
 *			  ��visit����һ�Σ��ҽ�����һ��
 * ����ֵ���ɹ�����OK�����򷵻�ERROR
 **/
Status DFSTraverse(ALGraph *G){
	int v;
	if(judge[col-1]!=TRUE){
        printf("ͼδ��ʼ�� ");   return ERROR;
    }
    if(G->vexnum == 0){
        printf("ͼΪ�� ");     return ERROR;
    }
//	VisitFunc = Visit; //ʹ��ȫ�ֱ���VisitFunc,ʹDFS�����躯��ָ�����
	for(v = 0; v < G->vexnum; v++)
		visited[col-1][v]=FALSE; //���ʱ�־�����ʼ��
	for(v = 0; v < G->vexnum; v++)
		if(!visited[col-1][v])
            DFS(G,v); //����δ���ʵĶ������DFS
	return OK;
}

/**
 * �������ƣ�BFSTraverse
 * ����������ͼ�ṹָ��G
 * �������ܣ���������ȷǵݹ����ͼG
 * ����ֵ���ɹ�����OK�����򷵻�ERROR
 **/
Status BFSTraverse(ALGraph *G){
	int v, u, w;
	VertexType u1, w1;
	LinkQueue Q;
	if(judge[col-1]!=TRUE){
        printf("ͼδ��ʼ�� ");   return ERROR;
    }
    if(G->vexnum == 0){
        printf("ͼΪ�� ");     return ERROR;
    }
	for(v = 0; v < G->vexnum; ++v)   visited[col-1][v]=FALSE;
    InitQueue(&Q); //�ÿյĸ�������Q
    for(v = 0; v < G->vexnum; ++v)
		if(!visited[col-1][v]) //v��δ����
		{
			visited[col-1][v] = TRUE;
			printf("���:%d-����:%c  ", v, G->vertices[v].data);
			EnQueue(&Q, v); //v�����
			while(!QueueEmpty(Q))
			{
				DeQueue(&Q, &u); //��ͷԪ�س��Ӳ���Ϊu
				u1 = G->vertices[u].data;
				for(w = FirstAdjVex(G,u1); w >= 0; w = NextAdjVex(G,u1,w1=G->vertices[w].data))
					if(!visited[col-1][w]) //wΪu����δ���ʵ��ڽӶ���
					{
						visited[col-1][w] = TRUE;
						printf("���:%d-����:%c  ", w, G->vertices[w].data);
						EnQueue(&Q,w); //w���
					}
			}
		}
	return OK;
}

/**
 * �������ƣ�OutDegree
 * ����������ͼG�ĵ�i�������ַ
 * �������ܣ������i���������
 * ����ֵ����i���������
 **/
int OutDegree(AdjList i){
	int num = 0;
	ArcNode * p;
	p = i->firstarc;
	while (p != NULL)
	{
		num++;
		p = p->nextarc;
	}
	return num;
}

/**
 * �������ƣ�LoadGraph
 * ����������ͼ�ṹ����List
 * �������ܣ����ļ��ж�ȡ���ͼ����Ϣ
 * ����ֵ���ɹ�����OK�����򷵻�ERROR
 **/
Status LoadGraph(ALGraph List[COLUMN]){
    /*
    if(judge!=TRUE){
        printf("ͼδ��ʼ�� ");   return ERROR;
    }
    */
	int i, j, k, num, yl;      //kΪ�����±�, numΪÿ������ĳ���
	FILE *fp;
	ArcNode *p;
    printf("��������Ҫ�ָ����������ڵ��ļ���:");
    scanf("%s",input);
    filename=input;
	if ((fp = fopen(filename,"r")) == NULL){
		printf("�ļ���ʧ�� ");		return ERROR;
	}
	fread(&yl,sizeof(int),1,fp);      //��ȡ��Ч�����
	//��ȡͼ������Ϣ
    for(i=0;i<yl;i++){
        fread(&(List[i].kind), sizeof(int), 1, fp);    //���
        fread(&(List[i].vexnum), sizeof(int), 1, fp);  //����
        fread(&(List[i].arcnum), sizeof(int), 1, fp);  //����
        //��ȡ�ڽӱ�
        for(j=0; j<List[i].vexnum; j++){  //��ʣ�ඥ��
            fread(&(List[i].vertices[j].data), sizeof(char), 1, fp);     //����
            printf("ͼ-%d,���-%d,����-%c",i,j,List[i].vertices[j].data);
            fread(&num, sizeof(int), 1, fp);                        //��ǰ�������
            if (num == 0)			List[i].vertices[j].firstarc = NULL;     //����Ϊ0
            else
            {
                printf("�ڽӵ����:");
                List[i].vertices[j].firstarc = NULL;
                for (k = 0; k < num; k++)
                {
                    p = (ArcNode *)malloc(sizeof(ArcNode));
                    fread(&(p->adjvex), sizeof(int), 1, fp);    //�ڽӵ���
                    printf("%d ",p->adjvex);
                    if (List[i].kind % 2)//��
                    {
                        p->info = (int *)malloc(sizeof(int));
                        fread(p->info, sizeof(InfoType), 1, fp);    //
                    }
                    else    p->info = NULL; //ͼ
                    p->nextarc = List[i].vertices[j].firstarc; //���ڱ�ͷ������ȳ���
                    List[i].vertices[j].firstarc = p;
                }
            }
            printf("\n\n");
        }
        judge[i] = TRUE;
    }
	fclose(fp);
	return OK;
}

/**
 * �������ƣ�SaveGraph
 * ����������ͼ�ṹ����List
 * �������ܣ��������Чͼ����Ϣ�����ļ�
 * ����ֵ���ɹ�����OK�����򷵻�ERROR
 **/
Status SaveGraph(ALGraph List[COLUMN]){
	int i, h, num, yl=0;
	FILE  *fp;
	ArcNode *p;
    for(i=0;i<COLUMN;i++){      //ͳ����Ԫ�صı�ĸ���
        if(judge[i]==TRUE && List[i].vexnum!=0)
            yl++;
    }
    if(yl ==0){       //����Ч����ʱ������
         printf("û�д�������");         return ERROR;
    }
    /*
	printf("�����뱣���ļ���:");
    scanf("%s",filename);   getchar();
    */
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
    //д�ļ��ķ���
	if ((fp = fopen(filename, "w")) == NULL)
{
		printf("�ļ���ʧ�� ");		return ERROR;
	}
	fwrite(&yl,sizeof(int),1,fp);   //д����Ч��ĸ���
	//����ͼ������Ϣ
	for(h=0; h<COLUMN; h++){
        if(judge[h]==TRUE && List[h].vexnum!=0){
            fwrite(&(List[h].kind), sizeof(int), 1, fp);    //���
            fwrite(&(List[h].vexnum), sizeof(int), 1, fp);  //����
            fwrite(&(List[h].arcnum), sizeof(int), 1, fp);  //����
            //�����ڽӱ�
            for (i = 0; i < List[h].vexnum; i++)
            {
                fwrite(&(List[h].vertices[i].data), sizeof(char), 1, fp);   //����
                printf("��-%d,���-%d,����-%c ",h,i,List[h].vertices[i].data);
                if(List[h].vertices[i].firstarc==NULL)    printf("\n\n");
                num = OutDegree(&(List[h].vertices[i]));        //��ǰ�������
                fwrite(&num, sizeof(int), 1, fp);
                if ((p = List[h].vertices[i].firstarc) != NULL)
                {
                    fwrite(&(p->adjvex), sizeof(int), 1, fp);     //�ڽӶ�����
                    printf("�ڽӵ����:%d ",p->adjvex);
                    if (List[h].kind % 2)//��
                        fwrite(p->info, sizeof(int), 1, fp);    //����
                    while ((p = p->nextarc) != NULL)
                    {
                        fwrite(&(p->adjvex), sizeof(int), 1, fp); //�ڽӶ�����
                        printf("%d ",p->adjvex);
                        if (List[h].kind % 2)//��
                            fwrite(p->info, sizeof(int), 1, fp);//����
                    }
                    printf("\n\n");
                }
            }
        }
	}
	fclose(fp);
	return OK;
}

/**
 * �������ƣ�ChooseList
 * ����������Ҫ�滻�ı�λ��col��ͼ�ṹָ��G��ͼ�ṹ����List
 * �������ܣ���Gָ��Ҫ������ͼ�±�col-1
 * ����ֵ��ʧ�ܷ���ERROR���ɹ�����OK
 **/
Status ChooseList(int col, ALGraph **G, ALGraph List[COLUMN]){
    if(col<1 || col>COLUMN){
        printf("�±�Խ�� ����ʧ�� ");
        return ERROR;
    }
    *G=&(List[col-1]);
    if(judge[col-1]!=TRUE)
        printf("�˱���δ��ʼ�� ");    //��ֹ�����
    return OK;
}
