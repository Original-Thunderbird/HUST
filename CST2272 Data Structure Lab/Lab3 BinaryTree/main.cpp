#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
/*---------page 10 on textbook ---------*/
#define TRUE 1
#define FALSE -1
#define OK 1
#define ERROR -2
typedef int Status;   //是函数的类型，其值是函数结果状态代码
typedef int ElemType; //数据元素类型定义
typedef struct BiTNode
{
	ElemType key;
	char data;
	struct BiTNode *lchild, *rchild;//左、右子树指针
}BiTNode, *BitTree;
Status InitBiTree(BitTree *L);
Status DestroyBiTree(BitTree *L);
Status CreateBiTree(BitTree *L);
Status ClearBiTree(BitTree *L);
Status BiTreeEmpty(BitTree L);
Status BiTreeDepth(BitTree L);
BitTree Root(BitTree L);
char Value(BitTree L, ElemType key);
Status Assign(BitTree *L, ElemType key, char data);
BitTree* Parent(BitTree L, ElemType key, BitTree *node);
BitTree LeftChild(BitTree L, ElemType key, BitTree *node);
BitTree RightChild(BitTree L, ElemType key, BitTree *node);
BitTree LeftSibling(BitTree L, ElemType key, BitTree *node);
BitTree RightSibling(BitTree L, ElemType key, BitTree *node);
BitTree FindNode(BitTree L, ElemType key, BitTree *node);
Status InsertChild(BitTree *L, BitTree p, int LR, BitTree c);
Status DeleteChild(BitTree *L, BitTree p, int LR);
Status PreOrderTraverse(BitTree L);
Status InOrderTraverse(BitTree L);
Status PostOrderTraverse(BitTree L);
Status LevelOrderTraverse(BitTree L);
Status SaveBiTree(FILE *fp,BitTree L);
Status ReadBiTree(FILE *fp,BitTree *L);
BitTree Search(BitTree L, ElemType key);
char* Filename(void);
FILE *fp;
char *filename, *pc, appe[5]=".dat",input[60]; //均用于生成文件名
int main(void)
{
	BitTree L=NULL,*p,c=NULL;
	FILE *fp;
	char ch,*filename,data;
	int LR,n,op = 1;
	ElemType key;
	p = (BitTree *)malloc(sizeof(BitTree));
	InitBiTree(&c);
	while (op) {
		*p = NULL;
		system("cls");	printf("\n\n");
		printf("      Menu for Binary Tree On Node Structure \n");
		printf("-------------------------------------------------\n");
		printf("    	  1. InitBiTree          12.RightChild\n");
		printf("    	  2. DestroyBiTree       13.LeftSibling\n");
		printf("    	  3. CreateBiTree        14.RightSibling \n");
		printf("    	  4. ClearBiTree         15.InsertChild\n");
		printf("    	  5. BiTreeEmpty         16.DeleteChild\n");
		printf("    	  6. BiTreeDepth         17.PreOrderTraverse\n");
		printf("          7. Root                18.InOrderTraverse\n");
		printf("          8. Value               19.PostOrderTraverse\n");
		printf("          9. Assign              20.LevelOrderTraverse\n");
		printf("          10. Parent             21.SaveBiTree\n");
		printf("          11. LeftChild          22.ReadBiTree\n");
		printf("          0. Exit\n");
		printf("\n");
		printf("-------------------------------------------------\n");
		printf("    请选择你的操作[0~22] ");
		scanf("%d", &op);
		switch (op) {
		case 1:
			if (InitBiTree(&L) == OK)		printf("初始化二叉树成功");
			else				printf("初始化二叉树失败");
			getchar(); getchar();			break;
		case 2:
			if (!L)             printf("二叉树未初始化");
			else if (DestroyBiTree(&L) == OK)
			{
				printf("二叉树销毁成功");		L = NULL;
			}
			else		printf("二叉树销毁失败");
			getchar(); getchar();			break;
		case 3:
			if (!L)                             printf("二叉树未初始化");
			else if (L->key != -1)              printf("二叉树已存在");   //直接用判断树空的条件，更为可靠
			else if (CreateBiTree(&L) == OK)	printf("创建成功");
			else                                printf("创建失败");
			getchar();getchar();			break;
		case 4:
			if (!L)                             printf("二叉树未初始化");
			else if (L->key==-1)                printf("二叉树为空");
			else if (ClearBiTree(&L) == OK)     printf("清空成功");
			else				                printf("清空失败");
			getchar();getchar();			break;
		case 5:
			if (!L)                             printf("二叉树未初始化");
			else if (BiTreeEmpty(L) ==TRUE)     printf("二叉树为空");
			else                                printf("二叉树非空");
			getchar();getchar();			break;
		case 6:
			if (!L)                 printf("二叉树未初始化");
			else if (L->key==-1)    printf("二叉树为空");    //树空单独讨论
			else        printf("二叉树深度为%d", BiTreeDepth(L));
			getchar();getchar();			break;
		case 7:
			if (!L)		            printf("二叉树未初始化");
            else if (L->key==-1)    printf("二叉树为空");
			else if (Root(L))       printf("根节点key为%d", L->key);
			getchar();getchar();			break;
		case 8:
			if (!L)                 printf("二叉树未初始化");
			else if (L->key==-1)    printf("二叉树为空");
			else{
                while ((ch = getchar()) != EOF && ch != '\n');
                printf("请输入目标结点key\n");
                scanf("%d", &key);			getchar();
                data=Value(L, key); //赋值表达式作为判断条件在当前编译环境下报warning,因此均提出作为独立语句
                if (data)     printf("key的data为%c", data);
                else          printf("查找节点失败"); //失败原因之后均放在函数内讨论
			}
			getchar();getchar();			break;
		case 9:
			if (!L)                 printf("二叉树未初始化");
			else if (L->key==-1)    printf("二叉树为空");
			else{
                while ((ch = getchar()) != EOF && ch != '\n');
                printf("请输入目标节点key\n");
                scanf("%d", &key);			getchar();
                printf("请输入新data\n");
                scanf("%c", &data);			getchar();
                if (Assign(&L, key, data) == OK)    printf("赋值成功");
                else	printf("赋值失败");
			}
			getchar();getchar();			break;
		case 10:
			if (!L)				    printf("二叉树未初始化");
			else if (L->key==-1)    printf("二叉树为空");
            else{
                while ((ch = getchar()) != EOF && ch != '\n');
                printf("定位结点key:");
                scanf("%d", &key);			getchar();
                if(!Search(L,key))  printf("未找到目标节点");
                else if (Parent(L, key ,p)!=NULL)
                    printf("双亲key为%d", (*p)->key);
                else	printf("返回失败");
            }
			getchar();getchar();			break;
		case 11:
            if (!L)				    printf("二叉树未初始化");
			else if (L->key==-1)    printf("二叉树为空");
            else{
                while ((ch = getchar()) != EOF && ch != '\n');
                printf("定位结点key:");
                scanf("%d", &key);			getchar();
                if(!Search(L,key))  printf("未找到目标节点");
                else if (LeftChild(L, key, p)!=NULL)
                    printf("左孩子key为%d",(*p)->key);
                else    printf("节点无左孩子 返回失败");
            }
			getchar();getchar();			break;
		case 12:
            if (!L)				    printf("二叉树未初始化");
			else if (L->key==-1)    printf("二叉树为空");
			else{
                while ((ch = getchar()) != EOF && ch != '\n');
                printf("定位结点key:");
                scanf("%d", &key);          getchar();
                if(!Search(L,key))  printf("未找到目标节点");
                else if (RightChild(L, key, p))
                    printf("右孩子key为%d",(*p)->key);
                else    printf("节点无右孩子 返回失败");
			}
			getchar();getchar();			break;
		case 13:
            if (!L)				    printf("二叉树未初始化");
			else if (L->key==-1)    printf("二叉树为空");
			else{
                while ((ch = getchar()) != EOF && ch != '\n');
                printf("定位结点key:");
                scanf("%d", &key);          getchar();
                if(!Search(L,key))  printf("未找到目标节点");
                else if (LeftSibling(L, key, p))
                    printf("左兄弟key为%d",(*p)->key);
                else    printf("节点无左兄弟 返回失败");
			}
			getchar();getchar();			break;
		case 14:
            if (!L)				    printf("二叉树未初始化");
			else if (L->key==-1)    printf("二叉树为空");
			else{   //要进行递归的函数不便于将输入及出错判断条件写入，故此部分在之前及之后均移出函数
                while ((ch = getchar()) != EOF && ch != '\n');
                printf("定位结点key:");
                scanf("%d", &key);         getchar();
                if(!Search(L,key))  printf("未找到目标节点");
                else if (RightSibling(L, key, p))
                    printf("右兄弟key为%d",(*p)->key);
                else
                    printf("节点无右兄弟 返回失败");
			}
			getchar();getchar();			break;
		case 15:
            if (!L)				    printf("二叉树未初始化");
			else if (L->key==-1)    printf("二叉树为空");
			else{
                ClearBiTree(&(c));
                while ((ch = getchar()) != EOF && ch != '\n');
                printf("创建二叉树,进入CreateBiTree\n");
                CreateBiTree(&(c));
                printf("插入位置p的key:");
                scanf("%d", &key);
                FindNode(L, key, p);    //寻找与key对应的结点，返回其指针
                while ((ch = getchar()) != EOF && ch != '\n');
                if (*p == NULL) 	printf("未找到目标节点");
                else
                {
                    printf("输入LR的值：");
                    scanf("%d", &LR);   getchar();
                    if (InsertChild(&L, *p, LR, c) == OK)
                        printf("插入成功");
                    else    printf("插入失败");
                }
			}
			getchar();getchar();			break;
		case 16:
            if (!L)				    printf("二叉树未初始化");
			else if (L->key==-1)    printf("二叉树为空");
			else{
                while ((ch = getchar()) != EOF && ch != '\n');
                printf("删除位置p的key:");
                scanf("%d", &key);			getchar();
                FindNode(L, key, p);
                if (*p == NULL)     printf("未找到目标节点");
                else
                {
                    printf("请输入LR的值：");
                    scanf("%d", &LR);   getchar();
                    n = DeleteChild(&L, *p, LR);
                    if (n == OK)            printf("删除成功");
                    else if (n == ERROR)    printf("删除失败");
                    else                    printf("不能删除空子树");
                }
			}
			getchar();getchar();			break;
		case 17:
            if (!L)				    printf("二叉树未初始化");
			else if (L->key==-1)    printf("二叉树为空");
			else{
			printf("\n----------------PreOrderTraverse all elements------------------\n");
			PreOrderTraverse(L);
			printf("\n----------------------------end---------------------------------\n");
			}
			getchar();getchar();			break;
		case 18:
            if (!L)				    printf("二叉树未初始化");
			else if (L->key==-1)    printf("二叉树为空");
            else{
                printf("\n----------------InOrderTraverse all elements------------------\n");
                InOrderTraverse(L);
                printf("\n----------------------------end---------------------------------\n");
            }
			getchar();getchar();			break;
		case 19:
            if (!L)				    printf("二叉树未初始化");
			else if (L->key==-1)    printf("二叉树为空");
			else{
                printf("\n----------------PostOrderTraverse all elements------------------\n");
                PostOrderTraverse(L);
                printf("\n----------------------------end---------------------------------\n");
            }
			getchar();getchar();			break;
		case 20:
            if (!L)				    printf("二叉树未初始化");
			else if (L->key==-1)    printf("二叉树为空");
			else{
                printf("\n----------------LevelOrderTraverse all elements------------------\n");
                LevelOrderTraverse(L);
                printf("\n----------------------------end---------------------------------\n");
            }
			getchar();getchar();			break;
		case 21:
            if (!L)				    printf("二叉树未初始化");
			else if (L->key==-1)    printf("二叉树为空");
			else{
                filename = Filename();  //用系统时间自动生成文件名
                if ((fp = fopen(filename, "w")) == NULL){
                    printf("文件打开失败 ");  return ERROR;
                }
                else if (SaveBiTree(fp,L) == OK){
                    fclose(fp);				printf("保存文件成功");
                }
                else    printf("保存文件失败");
			}
			getchar();getchar();			break;
		case 22:
            if (!L)				printf("二叉树未初始化");
			else{
                if (BiTreeEmpty(L) != TRUE)
                    ClearBiTree(&L);
                free(L);
                L=NULL;
                printf("请输入要读取的文件名：");
                while ((ch = getchar()) != EOF && ch != '\n');
                scanf("%s", input);  getchar();
                filename=input;     //用input串在buffer和filename串间过渡
                if ((fp = fopen(filename, "r")) == NULL)
                    printf("文件打开失败");
                else if (ReadBiTree(fp, &L) == OK)
                {
                    fclose(fp); printf("文件读取成功");
                }
                else
                    printf("文件读取失败");
			}
			getchar();getchar();			break;
		case 0:
			break;
		}
	}
	printf("欢迎下次再使用本系统！\n");
	getchar();getchar();
	return 0;
}
/**
 * 函数名称：InitBiTree
 * 函数参数：二叉树头指针L的地址
 * 函数功能：构造一列20个空的二叉树表头
 * 返回值：成功构造返回OK，否则返回ERROR
 **/
Status InitBiTree(BitTree *L){
	if (*L)		return ERROR;
    *L = (BitTree)malloc(sizeof(BiTNode));
	if (*L)
	{
		(*L)->key = -1;     //初始化为空树
		(*L)->lchild = NULL;		(*L)->rchild = NULL;    //孩子指针置空
		return OK;
	}
    return ERROR;
}
/**
 * 函数名称：DestroyBiTree
 * 函数参数：二叉树头指针L的地址
 * 函数功能：删除二叉树
 * 返回值：成功销毁返回OK，否则返回ERROR
 **/
Status DestroyBiTree(BitTree *L){
	if ((*L)==NULL)		return ERROR;
	if ((*L)->lchild != NULL)
        DestroyBiTree(&(*L)->lchild);
	if ((*L)->rchild != NULL)
        DestroyBiTree(&(*L)->rchild);
	free(*L);   //递归释放节点
	return OK;
}
/**
 * 函数名称：CreateBiTree
 * 函数参数：二叉树头指针L的地址
 * 函数功能：建立二叉树
 * 返回值：成功建立返回OK，否则返回ERROR
 **/
Status CreateBiTree(BitTree *L){
	int keyin;
	if(!(*L))   return ERROR;
    printf("请输入结点关键字，-1表示不创建\n");   //-1作为NULL的标记，同样也是一次递归向树根方向进行的开始
	scanf("%d", &keyin);	getchar();
	if (keyin==-1)
	{		(*L) = NULL;		return 0;	}
	else
	{
		if (!((*L = (BiTNode *)malloc(sizeof(BiTNode))))){
            printf("空间分配失败");   return ERROR;
		}
		(*L)->key = keyin;
        printf("请输入结点值：\n");
		scanf("%c", &((*L)->data));		getchar();
		CreateBiTree(&((*L)->lchild));
		CreateBiTree(&((*L)->rchild));  //递归建立节点
	}
	return OK;
}
/**
 * 函数名称：ClearBiTree
 * 函数参数：二叉树头指针L的地址
 * 函数功能：重置二叉树
 * 返回值：成功置空返回OK，否则返回ERROR
 **/
Status ClearBiTree(BitTree *L){
	if (!DestroyBiTree(&(*L)))		return ERROR;
	*L = NULL;
	if (!InitBiTree(&(*L))) 		return ERROR;
	return OK;
}
/**
 * 函数名称：BiTreeEmpty
 * 函数参数：二叉树头指针L
 * 函数功能：判断二叉树是否为空
 * 返回值：空TRUE,非空FALSE
 **/
Status BiTreeEmpty(BitTree L){
	if (L->key==-1)		return TRUE;
	else        		return FALSE;
}
/**
 * 函数名称：BiTreeDepth
 * 函数参数：二叉树头指针L
 * 函数功能：求二叉树深度
 * 返回值：深度depth
 **/
int BiTreeDepth(BitTree L){
	int depth = 0,lchilddepth,rchilddepth;
	if (L)
	{
		lchilddepth=BiTreeDepth((L->lchild));
		rchilddepth=BiTreeDepth((L->rchild));
		depth = rchilddepth >= lchilddepth ? rchilddepth + 1 : lchilddepth + 1;
	}
	return depth;
}
/**
 * 函数名称：Root
 * 函数参数：二叉树头指针L
 * 函数功能：找二叉树的根节点
 * 返回值：根节点L
 **/
BitTree Root(BitTree L){
	return L;
}
/**
 * 函数名称：Value
 * 函数参数：二叉树头指针L，关键字key
 * 函数功能：找二叉树中关键字为key的节点的data
 * 返回值：成功返回data，失败返回0
 **/
char Value(BitTree L, ElemType key){
    BitTree M = Search(L, key);
    if(M)   return M->data;
    printf("未找到该节点 ");  return 0;
}
/**
 * 函数名称：Assign
 * 函数参数：二叉树头指针L的地址，关键字key， 数据域data
 * 函数功能：将二叉树中关键字为key的节点的data赋值为value
 * 返回值：成功赋值返回OK，否则返回ERROR
 **/
Status Assign(BitTree *L, ElemType key, char data){
	if (*L==NULL)		return 0;
	BitTree M = Search(*L, key);
	if(M){    M->data=data;   return OK;      }
	printf("未找到该节点 ");  return 0;
}
/**
 * 函数名称：Parent
 * 函数参数：二叉树头指针L，关键字key
 * 函数功能：找二叉树中关键字为key的节点的双亲
 * 返回值：成功返回双亲节点指针，否则返回NULL
 **/
BitTree* Parent(BitTree L, ElemType key,BitTree *node)
{
	if(L==NULL)     return NULL;
	if (L->key == key){
        printf("目标节点为头结点 无双亲 ");    return NULL;
	}
	if ((L->lchild) != NULL)
		if (L->lchild->key == key)
		{	*node = L;		return node;	}
	if ((L->rchild)!=NULL)
		if (L->rchild->key == key)
		{    *node = L;     return node;	}
	if (Parent(L->lchild, key ,node))		return node;
	if (Parent(L->rchild, key ,node))		return node;
    return NULL;
}
/**
 * 函数名称：LeftChild
 * 函数参数：二叉树头指针L，关键字key
 * 函数功能：找二叉树中关键字为key的节点的左孩子
 * 返回值：成功返回左孩子节点指针，否则返回NULL
 **/
BitTree LeftChild(BitTree L, ElemType key, BitTree *node)
{
	if (L == NULL)		return NULL;
	if (L->key == key)
		if (L->lchild != NULL)
		{   *node = L->lchild;  return *node;   }
	if (LeftChild(L->lchild, key, node))		return *node;;
	if (LeftChild(L->rchild, key, node))		return *node;
	return NULL;
}
/**
 * 函数名称：RightChild
 * 函数参数：二叉树头指针L，关键字key
 * 函数功能：找二叉树中关键字为key的节点的右孩子
 * 返回值：成功返回右孩子节点指针，否则返回NULL
 **/
BitTree RightChild(BitTree L, ElemType key, BitTree *node)
{
	if (L == NULL)		return NULL;
	if (L->key == key)
		if (L->rchild != NULL)
		{   *node = L->rchild;  return *node;   }
	if (RightChild(L->lchild, key, node))		return *node;
	if (RightChild(L->rchild, key, node))		return *node;
	return NULL;
}
/**
 * 函数名称：LeftSibling
 * 函数参数：二叉树头指针L，关键字key
 * 函数功能：找二叉树中关键字为key的节点的左兄弟
 * 返回值：成功返回左兄弟节点指针，否则返回NULL
 **/
BitTree LeftSibling(BitTree L, ElemType key, BitTree *node)
{
	if (L == NULL)		return NULL;
	if (L->rchild != NULL)
		if (L->rchild->key == key && L->lchild != NULL)
		{	*node = L->lchild;  return *node;   }
	if (LeftSibling(L->lchild, key,node))		return *node;
	if (LeftSibling(L->rchild, key, node))		return *node;
	return NULL;
}
/**
 * 函数名称：RightSibling
 * 函数参数：二叉树头指针L，关键字key
 * 函数功能：找二叉树中关键字为key的节点的右兄弟
 * 返回值：成功返回右兄弟节点指针，否则返回NULL
 **/
BitTree RightSibling(BitTree L, ElemType key, BitTree *node)
{
	if (L == NULL)		return NULL;
	if (L->lchild != NULL)
		if (L->lchild->key == key && L->rchild != NULL)
		{   *node = L->rchild;      return *node;   }
	if (RightSibling(L->lchild, key, node))		return *node;
	if (RightSibling(L->rchild, key, node))		return *node;
	return NULL;
}
/**
 * 函数名称：InsertChild
 * 函数参数：二叉树头指针L的地址，二叉树结点指针p, 定位变量LR，二叉树结点指针c
 * 函数功能：在节点p处插入子树c
 * 返回值：成功返回OK，否则返回ERROR
 **/
Status InsertChild(BitTree *L,BitTree p, int LR, BitTree c)
{
	if (LR == 0){
        if (p->rchild != NULL){
            printf("二叉树C含有左子树\n");  return ERROR;
        }
	    p->lchild = c;  return OK;
    }
	else if (LR == 1){
        if (p->rchild != NULL){
            printf("二叉树C含有右子树\n");  return ERROR;
        }
        p->rchild = c;  return OK;
    }
	else    return ERROR;
}
/**
 * 函数名称：DeleteChild
 * 函数参数：二叉树头指针L的地址，二叉树结点指针p, 定位变量LR
 * 函数功能：在节点p处删除左/右子树c
 * 返回值：成功返回OK，否则返回ERROR
 **/
Status DeleteChild(BitTree *L, BitTree p, int LR)
{
	if (LR == 0)
	{
		if ((*L)->lchild == NULL)	return FALSE;
		DestroyBiTree(&((*L)->lchild));
		p->lchild = NULL;
	}
	else if (LR == 1)
	{
		if ((*L)->rchild == NULL)   return FALSE;
		DestroyBiTree(&((*L)->rchild));
		p->rchild = NULL;
	}
	else		return ERROR;
	return OK;
}
/**
 * 函数名称：PreOrderTraverse
 * 函数参数：二叉树头指针L
 * 函数功能：先序遍历二叉树
 * 返回值：OK
 **/
Status PreOrderTraverse(BitTree L)
{
	if (L == NULL)
		return 0;
	printf("%d-%c  ", L->key,L->data);
	if (L->lchild != NULL)
		PreOrderTraverse(L->lchild);
	if (L->rchild != NULL)
		PreOrderTraverse(L->rchild);
	return OK;
}
/**
 * 函数名称：InOrderTraverse
 * 函数参数：二叉树头指针L
 * 函数功能：中序遍历二叉树
 * 返回值：OK
 **/
Status InOrderTraverse(BitTree L)
{
	if (L == NULL)
		return 0;
	if (L->lchild != NULL)
		InOrderTraverse(L->lchild);
	printf("%d-%c  ", L->key,L->data);
	if (L->rchild != NULL)
		InOrderTraverse(L->rchild);
	return OK;
}
/**
 * 函数名称：PostOrderTraverse
 * 函数参数：二叉树头指针L
 * 函数功能：后序遍历二叉树
 * 返回值：OK
 **/
Status PostOrderTraverse(BitTree L)
{
	if (L == NULL)
		return 0;
	if (L->lchild != NULL)
		PostOrderTraverse(L->lchild);
	if (L->rchild != NULL)
		PostOrderTraverse(L->rchild);
	printf("%d-%c  ", L->key,L->data);
	return OK;
}
/**
 * 函数名称：LevelOrderTraverse
 * 函数参数：二叉树头指针L
 * 函数功能：层序遍历二叉树
 * 返回值：OK
 **/
Status LevelOrderTraverse(BitTree T)
{
	if (T == NULL)		return 0;
	BitTree q[1000];
	q[0] = T;
	int front = 0,rear = 1;
	while (front < rear)
	{
		printf("%d-%c  ", q[front]->key,q[front]->data);
		if (q[front]->lchild)			q[rear++] = q[front]->lchild;
		if (q[front]->rchild)			q[rear++] = q[front]->rchild;
		++front;
	}
	return OK;
}
/**
 * 函数名称：SaveBiTree
 * 函数参数：文件指针fp，二叉树头指针L
 * 函数功能：将L中的数据保存到fp指针指向的文件中
 * 返回值：OK
 **/
Status SaveBiTree(FILE *fp, BitTree L)
{
    ElemType c= -1;
	if (L == NULL)
		fwrite(&(c),sizeof(ElemType),1,fp);
	else
	{
		fwrite(&(L->key),sizeof(ElemType),1,fp);
		fwrite(&(L->data),sizeof(char),1,fp);
		SaveBiTree(fp, L->lchild);
		SaveBiTree(fp, L->rchild);
	}
	return OK;
}
/**
 * 函数名称：LoadBiTree
 * 函数参数：文件指针fp，二叉树头指针L
 * 函数功能：数据加载，为二叉树输入元素。
 * 返回值：OK
 **/
Status ReadBiTree(FILE *fp, BitTree *L)
{
	ElemType key;
	fread(&(key),sizeof(ElemType),1,fp);
	if (key == -1)		*L = NULL;
	else
	{
		*L = (BitTree)malloc(sizeof(BiTNode));
		(*L)->key = key;
		fread(&((*L)->data),sizeof(char),1,fp);
		ReadBiTree(fp, &(*L)->lchild);
		ReadBiTree(fp, &(*L)->rchild);
	}
	return OK;
}
/**
 * 函数名称：FindNode
 * 函数参数：二叉树头指针L，关键字key，二叉树结点指针node的地址,
 * 函数功能：寻找关键字为key的节点并返回其指针
 * 返回值：成功返回node的地址，否则返回NULL
 **/
BitTree FindNode(BitTree L, ElemType key, BitTree *node)
{
	if (L == NULL)		return NULL;
	if (L->key == key){   *node = L;  return *node;   }
	if (L->lchild != NULL&& FindNode(L->lchild, key, node))
        return *node;
	if (L->rchild != NULL&& FindNode(L->rchild, key, node))
		return *node;
	return NULL;
}
/**
 * 函数名称：Search
 * 函数参数：二叉树头指针L，关键字key
 * 函数功能：寻找关键字为key的节点并返回其指针
 * 返回值：成功返回M的地址，否则返回NULL
 **/
BitTree Search(BitTree L, ElemType key){
    BitTree M=NULL;
    if(L==NULL)     return M;
    if(L->key==key)   M=L;
    if(!M)  M=Search(L->lchild,key);
    if(!M)  M=Search(L->rchild,key);
    return M;
}
/**
 * 函数名称：Filename
 * 函数参数：无
 * 函数功能：按照系统时间自动生成文件名并修饰
 * 返回值：返回时间字符串指针
 **/
char* Filename(void){
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
    return filename;
}
