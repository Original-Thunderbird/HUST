#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>

/*---------page 10 on textbook ---------*/
#define TRUE 1
#define FALSE - 1
#define OK 1
#define ERROR - 2
typedef int Status; // is the type of the function, its value is the function result status code
typedef int ElemType; // Data element type definition
typedef struct BiTNode {
  ElemType key;
  char data;
  struct BiTNode * lchild, * rchild; // Left and right subtree pointers
}
BiTNode, * BitTree;
Status InitBiTree(BitTree * L);
Status DestroyBiTree(BitTree * L);
Status CreateBiTree(BitTree * L);
Status ClearBiTree(BitTree * L);
Status BiTreeEmpty(BitTree L);
Status BiTreeDepth(BitTree L);
BitTree Root(BitTree L);
char Value(BitTree L, ElemType key);
Status Assign(BitTree * L, ElemType key, char data);
BitTree * Parent(BitTree L, ElemType key, BitTree * node);
BitTree LeftChild(BitTree L, ElemType key, BitTree * node);
BitTree RightChild(BitTree L, ElemType key, BitTree * node);
BitTree LeftSibling(BitTree L, ElemType key, BitTree * node);
BitTree RightSibling(BitTree L, ElemType key, BitTree * node);
BitTree FindNode(BitTree L, ElemType key, BitTree * node);
Status InsertChild(BitTree * L, BitTree p, int LR, BitTree c);
Status DeleteChild(BitTree * L, BitTree p, int LR);
Status PreOrderTraverse(BitTree L);
Status InOrderTraverse(BitTree L);
Status PostOrderTraverse(BitTree L);
Status LevelOrderTraverse(BitTree L);
Status SaveBiTree(FILE * fp, BitTree L);
Status ReadBiTree(FILE * fp, BitTree * L);
BitTree Search(BitTree L, ElemType key);
char * Filename(void);
FILE * fp;
char * filename, * pc, appe[5] = ".dat", input[60]; // are used to generate file names
int main(void) {
  BitTree L = NULL, * p, c = NULL;
  FILE * fp;
  char ch, * filename, data;
  int LR, n, op = 1;
  ElemType key;
  p = (BitTree * ) malloc(sizeof(BitTree));
  InitBiTree( & c);
  while (op) {
    * p = NULL;
    system("cls");
    printf("\n\n");
    printf(" Menu for Binary Tree On Node Structure \n");
    printf("---------------------------------------------- --\n");
    printf(" 1.InitBiTree 12.RightChild\n");
    printf(" 2. DestroyBiTree 13. LeftSibling\n");
    printf(" 3. CreateBiTree 14. RightSibling\n");
    printf(" 4. ClearBiTree 15. InsertChild\n");
    printf(" 5. BiTreeEmpty 16. DeleteChild\n");
    printf(" 6. BiTreeDepth 17. PreOrderTraverse\n");
    printf(" 7. Root 18. InOrderTraverse\n");
    printf(" 8. Value 19. PostOrderTraverse\n");
    printf(" 9. Assign 20. LevelOrderTraverse\n");
    printf(" 10. Parent 21. SaveBiTree\n");
    printf(" 11. LeftChild 22. ReadBiTree\n");
    printf(" 0. Exit\n");
    printf("\n");
    printf("---------------------------------------------- --\n");
    printf(" Please choose your operation [0~22]");
    scanf("%d", & op);
    switch (op) {
    case 1:
      if (InitBiTree( & L) == OK) printf(" Initialize the binary tree successfully ");
      else printf(" Failed to initialize binary tree ");
      getchar();
      getchar();
      break;
    case 2:
      if (!L) printf(" Binary tree not initialized ");
      else if (DestroyBiTree( & L) == OK) {
        printf(" Binary tree destroyed successfully ");
        L = NULL;
      } else printf(" Binary tree destruction failed ");
      getchar();
      getchar();
      break;
    case 3:
      if (!L) printf(" Binary tree not initialized ");
      else if (L -> key != -1) printf(" Binary tree already exists "); // It is more reliable to directly use the condition of judging the empty tree
      else if (CreateBiTree( & L) == OK) printf(" Created successfully ");
      else printf(" Failed to create ");
      getchar();
      getchar();
      break;
    case 4:
      if (!L) printf(" Binary tree not initialized ");
      else if (L -> key == -1) printf(" Binary tree is empty ");
      else if (ClearBiTree( & L) == OK) printf(" Clear successfully ");
      else printf(" Empty failed ");
      getchar();
      getchar();
      break;
    case 5:
      if (!L) printf(" Binary tree not initialized ");
      else if (BiTreeEmpty(L) == TRUE) printf(" BiTreeEmpty(L) is empty ");
      else printf(" Binary tree is not empty ");
      getchar();
      getchar();
      break;
    case 6:
      if (!L) printf(" Binary tree not initialized ");
      else if (L -> key == -1) printf(" Binary tree is empty "); // Empty tree is discussed separately
      else printf(" Binary tree depth is %d", BiTreeDepth(L));
      getchar();
      getchar();
      break;
    case 7:
      if (!L) printf(" Binary tree not initialized ");
      else if (L -> key == -1) printf(" Binary tree is empty ");
      else if (Root(L)) printf(" The root node key is %d", L -> key);
      getchar();
      getchar();
      break;
    case 8:
      if (!L) printf(" Binary tree not initialized ");
      else if (L -> key == -1) printf(" Binary tree is empty ");
      else {
        while ((ch = getchar()) != EOF && ch != '\n');
        printf(" Please enter the target node key\n");
        scanf("%d", & key);
        getchar();
        data = Value(L, key); // The assignment expression is used as a judgment condition to report warning in the current compilation environment , so it is proposed as an independent statement
        if (data) printf("key 's data is %c", data);
        else printf(" Failed to find node "); // The reason for the failure will be discussed in the function
      }
      getchar();
      getchar();
      break;
    case 9:
      if (!L) printf(" Binary tree not initialized ");
      else if (L -> key == -1) printf(" Binary tree is empty ");
      else {
        while ((ch = getchar()) != EOF && ch != '\n');
        printf(" Please enter the target node key\n");
        scanf("%d", & key);
        getchar();
        printf(" Please input new data\n");
        scanf("%c", & data);
        getchar();
        if (Assign( & L, key, data) == OK) printf(" Assignment succeeded ");
        else printf(" Assignment failed ");
      }
      getchar();
      getchar();
      break;
    case 10:
      if (!L) printf(" Binary tree not initialized ");
      else if (L -> key == -1) printf(" Binary tree is empty ");
      else {
        while ((ch = getchar()) != EOF && ch != '\n');
        printf(" Location node key:");
        scanf("%d", & key);
        getchar();
        if (!Search(L, key)) printf(" No target node found ");
        else if (Parent(L, key, p) != NULL)
          printf(" parent key is %d", ( * p) -> key);
        else printf(" return failed ");
      }
      getchar();
      getchar();
      break;
    case 11:
      if (!L) printf(" Binary tree not initialized ");
      else if (L -> key == -1) printf(" Binary tree is empty ");
      else {
        while ((ch = getchar()) != EOF && ch != '\n');
        printf(" Location node key:");
        scanf("%d", & key);
        getchar();
        if (!Search(L, key)) printf(" No target node found ");
        else if (LeftChild(L, key, p) != NULL)
          printf(" Left child key is %d", ( * p) -> key);
        else printf(" node has no left child return failed ");
      }
      getchar();
      getchar();
      break;
    case 12:
      if (!L) printf(" Binary tree not initialized ");
      else if (L -> key == -1) printf(" Binary tree is empty ");
      else {
        while ((ch = getchar()) != EOF && ch != '\n');
        printf(" Location node key:");
        scanf("%d", & key);
        getchar();
        if (!Search(L, key)) printf(" No target node found ");
        else if (RightChild(L, key, p))
          printf(" The right child key is %d", ( * p) -> key);
        else printf(" node has no right child return failed ");
      }
      getchar();
      getchar();
      break;
    case 13:
      if (!L) printf(" Binary tree not initialized ");
      else if (L -> key == -1) printf(" Binary tree is empty ");
      else {
        while ((ch = getchar()) != EOF && ch != '\n');
        printf(" Location node key:");
        scanf("%d", & key);
        getchar();
        if (!Search(L, key)) printf(" No target node found ");
        else if (LeftSibling(L, key, p))
          printf(" The key of the left brother is %d", ( * p) -> key);
        else printf(" node has no left sibling return failed ");
      }
      getchar();
      getchar();
      break;
    case 14:
      if (!L) printf(" Binary tree not initialized ");
      else if (L -> key == -1) printf(" Binary tree is empty ");
      else { // The function to be recursive is not convenient to write the input and error judgment conditions, so this part is moved out of the function before and after
        while ((ch = getchar()) != EOF && ch != '\n');
        printf(" Location node key:");
        scanf("%d", & key);
        getchar();
        if (!Search(L, key)) printf(" No target node found ");
        else if (RightSibling(L, key, p))
          printf(" The right sibling key is %d", ( * p) -> key);
        else
          printf(" node has no right sibling return failed ");
      }
      getchar();
      getchar();
      break;
    case 15:
      if (!L) printf(" Binary tree not initialized ");
      else if (L -> key == -1) printf(" Binary tree is empty ");
      else {
        ClearBiTree( & (c));
        while ((ch = getchar()) != EOF && ch != '\n');
        printf(" Create a binary tree , enter CreateBiTree\n");
        CreateBiTree( & (c));
        printf(" insert key at position p :");
        scanf("%d", & key);
        FindNode(L, key, p); // Find the node corresponding to key and return its pointer
        while ((ch = getchar()) != EOF && ch != '\n');
        if ( * p == NULL) printf(" No target node found ");
        else {
          printf(" Enter the value of LR : ");
          scanf("%d", & LR);
          getchar();
          if (InsertChild( & L, * p, LR, c) == OK)
            printf(" insert successfully ");
          else printf(" Insert failed ");
        }
      }
      getchar();
      getchar();
      break;
    case 16:
      if (!L) printf(" Binary tree not initialized ");
      else if (L -> key == -1) printf(" Binary tree is empty ");
      else {
        while ((ch = getchar()) != EOF && ch != '\n');
        printf(" Delete the key at position p :");
        scanf("%d", & key);
        getchar();
        FindNode(L, key, p);
        if ( * p == NULL) printf(" No target node found ");
        else {
          printf(" Please enter the value of LR : ");
          scanf("%d", & LR);
          getchar();
          n = DeleteChild( & L, * p, LR);
          if (n == OK) printf(" Deleted successfully ");
          else if (n == ERROR) printf(" Delete failed ");
          else printf(" cannot delete empty subtree ");
        }
      }
      getchar();
      getchar();
      break;
    case 17:
      if (!L) printf(" Binary tree not initialized ");
      else if (L -> key == -1) printf(" Binary tree is empty ");
      else {
        printf("\n----------------PreOrderTraverse all elements------------------\n");
        PreOrderTraverse(L);
        printf("\n----------------------------end---------------- -----------------\n");
      }
      getchar();
      getchar();
      break;
    case 18:
      if (!L) printf(" Binary tree not initialized ");
      else if (L -> key == -1) printf(" Binary tree is empty ");
      else {
        printf("\n----------------InOrderTraverse all elements------------------\n");
        InOrderTraverse(L);
        printf("\n----------------------------end---------------- -----------------\n");
      }
      getchar();
      getchar();
      break;
    case 19:
      if (!L) printf(" Binary tree not initialized ");
      else if (L -> key == -1) printf(" Binary tree is empty ");
      else {
        printf("\n----------------PostOrderTraverse all elements------------------\n");
        PostOrderTraverse(L);
        printf("\n----------------------------end---------------- -----------------\n");
      }
      getchar();
      getchar();
      break;
    case 20:
      if (!L) printf(" Binary tree not initialized ");
      else if (L -> key == -1) printf(" Binary tree is empty ");
      else {
        printf("\n----------------LevelOrderTraverse all elements------------------\n");
        LevelOrderTraverse(L);
        printf("\n----------------------------end---------------- -----------------\n");
      }
      getchar();
      getchar();
      break;
    case 21:
      if (!L) printf(" Binary tree not initialized ");
      else if (L -> key == -1) printf(" Binary tree is empty ");
      else {
        filename = Filename(); // Automatically generate filename with system time
        if ((fp = fopen(filename, "w")) == NULL) {
          printf(" Failed to open the file ");
          return ERROR;
        } else if (SaveBiTree(fp, L) == OK) {
          fclose(fp);
          printf(" Save the file successfully ");
        } else printf(" Failed to save file ");
      }
      getchar();
      getchar();
      break;
    case 22:
      if (!L) printf(" Binary tree not initialized ");
      else {
        if (BiTreeEmpty(L) != TRUE)
          ClearBiTree( & L);
        free(L);
        L = NULL;
        printf(" Please enter the file name to read: ");
        while ((ch = getchar()) != EOF && ch != '\n');
        scanf("%s", input);
        getchar();
        filename = input; // Use input string to transition between buffer and filename string
        if ((fp = fopen(filename, "r")) == NULL)
          printf(" Failed to open the file ");
        else if (ReadBiTree(fp, & L) == OK) {
          fclose(fp);
          printf(" File read successfully ");
        } else
          printf(" Failed to read file ");
      }
      getchar();
      getchar();
      break;
    case 0:
      break;
    }
  }
  printf(" Welcome to use this system next time! \n");
  getchar();
  getchar();
  return 0;
}
/**
 * Function name: InitBiTree
 * Function parameters: the address of the binary tree head pointer L
 * Function function: Construct a list of 20 empty binary tree headers
 * Return value: return OK if successfully constructed , otherwise return ERROR
 **/
Status InitBiTree(BitTree * L) {
  if ( * L) return ERROR;
  * L = (BitTree) malloc(sizeof(BiTNode));
  if ( * L) {
    ( * L) -> key = -1; // initialize to an empty tree
    ( * L) -> lchild = NULL;
    ( * L) -> rchild = NULL; // The child pointer is empty
    return OK;
  }
  return ERROR;
}
/**
 * Function name: DestroyBiTree
 * Function parameters: the address of the binary tree head pointer L
 * Function function: delete binary tree
 * Return value: return OK if destroyed successfully , otherwise return ERROR
 **/
Status DestroyBiTree(BitTree * L) {
  if (( * L) == NULL) return ERROR;
  if (( * L) -> lchild != NULL)
    DestroyBiTree( & ( * L) -> lchild);
  if (( * L) -> rchild != NULL)
    DestroyBiTree( & ( * L) -> rchild);
  free( * L); // Recursively release nodes
  return OK;
}
/**
 * Function name: CreateBiTree
 * Function parameters: the address of the binary tree head pointer L
 * Function function: build a binary tree
 * Return value: return OK if established successfully , otherwise return ERROR
 **/
Status CreateBiTree(BitTree * L) {
  int keyin;
  if (!( * L)) return ERROR;
  printf(" Please enter the node keyword, -1 means not to create \n"); //-1 is used as a NULL mark, and it is also the beginning of a recursion to the root of the tree
  scanf("%d", & keyin);
  getchar();
  if (keyin == -1) {
    ( * L) = NULL;
    return 0;
  } else {
    if (!(( * L = (BiTNode * ) malloc(sizeof(BiTNode))))) {
      printf(" Space allocation failed ");
      return ERROR;
    }
    ( * L) -> key = keyin;
    printf(" Please enter the node value: \n");
    scanf("%c", & (( * L) -> data));
    getchar();
    CreateBiTree( & (( * L) -> lchild));
    CreateBiTree( & (( * L) -> rchild)); // recursively create nodes
  }
  return OK;
}
/**
 * Function name: ClearBiTree
 * Function parameters: the address of the binary tree head pointer L
 * Function function: reset the binary tree
 * Return value: return OK if it is empty successfully , otherwise return ERROR
 **/
Status ClearBiTree(BitTree * L) {
  if (!DestroyBiTree( & ( * L))) return ERROR;
  * L = NULL;
  if (!InitBiTree( & ( * L))) return ERROR;
  return OK;
}
/**
 * Function name: BiTreeEmpty
 * Function parameters: Binary tree head pointer L
 * Function function: determine whether the binary tree is empty
 * Return value: empty TRUE, non-empty FALSE
 **/
Status BiTreeEmpty(BitTree L) {
  if (L -> key == -1) return TRUE;
  else return FALSE;
}
/**
 * Function name: BiTreeDepth
 * Function parameters: Binary tree head pointer L
 * Function function: Find the depth of the binary tree
 * Return value: depth depth
 **/
int BiTreeDepth(BitTree L) {
  int depth = 0, lchilddepth, rchilddepth;
  if (L) {
    lchilddepth = BiTreeDepth((L -> lchild));
    rchilddepth = BiTreeDepth((L -> rchild));
    depth = rchilddepth >= lchilddepth ? rchilddepth + 1 : lchilddepth + 1;
  }
  return depth;
}
/**
 * Function name: Root
 * Function parameters: Binary tree head pointer L
 * Function function: find the root node of the binary tree
 * Return value: root node L
 **/
BitTree Root(BitTree L) {
  return L;
}
/**
 * Function name: Value
 * Function parameters: binary tree head pointer L , keyword key
 * Function function: Find the data of the node whose keyword is key in the binary tree
 * Return value: return data successfully , return 0 if failed
 **/
char Value(BitTree L, ElemType key) {
  BitTree M = Search(L, key);
  if (M) return M -> data;
  printf(" The node was not found ");
  return 0;
}
/**
 * Function name: Assign
 * Function parameters: the address of the binary tree head pointer L , the keyword key , data domain data
 * Function function: assign the data of the node whose keyword is key in the binary tree to value
 * Return value: return OK if the assignment is successful , otherwise return ERROR
 **/
Status Assign(BitTree * L, ElemType key, char data) {
  if ( * L == NULL) return 0;
  BitTree M = Search( * L, key);
  if (M) {
    M -> data = data;
    return OK;
  }
  printf(" The node was not found ");
  return 0;
}
/**
 * Function name: Parent
 * Function parameters: binary tree head pointer L , keyword key
 * Function function: Find the parents of the node whose keyword is key in the binary tree
 * Return value: return parent node pointer successfully, otherwise return NULL
 **/
BitTree * Parent(BitTree L, ElemType key, BitTree * node) {
  if (L == NULL) return NULL;
  if (L -> key == key) {
    printf(" The target node is the head node no parent ");
    return NULL;
  }
  if ((L -> lchild) != NULL)
    if (L -> lchild -> key == key) {
      * node = L;
      return node;
    }
  if ((L -> rchild) != NULL)
    if (L -> rchild -> key == key) {
      * node = L;
      return node;
    }
  if (Parent(L -> lchild, key, node)) return node;
  if (Parent(L -> rchild, key, node)) return node;
  return NULL;
}
/**
 * Function name: LeftChild
 * Function parameters: binary tree head pointer L , keyword key
 * Function function: Find the left child of the node whose keyword is key in the binary tree
 * Return value: successfully returns the left child node pointer, otherwise returns NULL
 **/
BitTree LeftChild(BitTree L, ElemType key, BitTree * node) {
  if (L == NULL) return NULL;
  if (L -> key == key)
    if (L -> lchild != NULL) {
      * node = L -> lchild;
      return * node;
    }
  if (LeftChild(L -> lchild, key, node)) return * node;;
  if (LeftChild(L -> rchild, key, node)) return * node;
  return NULL;
}
/**
 * Function name: RightChild
 * Function parameters: binary tree head pointer L , keyword key
 * Function function: Find the right child of the node whose keyword is key in the binary tree
 * Return value: successfully returns the right child node pointer, otherwise returns NULL
 **/
BitTree RightChild(BitTree L, ElemType key, BitTree * node) {
  if (L == NULL) return NULL;
  if (L -> key == key)
    if (L -> rchild != NULL) {
      * node = L -> rchild;
      return * node;
    }
  if (RightChild(L -> lchild, key, node)) return * node;
  if (RightChild(L -> rchild, key, node)) return * node;
  return NULL;
}
/**
 * Function name: LeftSibling
 * Function parameters: binary tree head pointer L , keyword key
 * Function function: Find the left brother of the node whose keyword is key in the binary tree
 * Return value: successfully return the left sibling node pointer, otherwise return NULL
 **/
BitTree LeftSibling(BitTree L, ElemType key, BitTree * node) {
  if (L == NULL) return NULL;
  if (L -> rchild != NULL)
    if (L -> rchild -> key == key && L -> lchild != NULL) {
      * node = L -> lchild;
      return * node;
    }
  if (LeftSibling(L -> lchild, key, node)) return * node;
  if (LeftSibling(L -> rchild, key, node)) return * node;
  return NULL;
}
/**
 * Function name: RightSibling
 * Function parameters: binary tree head pointer L , keyword key
 * Function function: Find the right sibling of the node whose keyword is key in the binary tree
 * Return value: successfully returns the pointer to the right sibling node, otherwise returns NULL
 **/
BitTree RightSibling(BitTree L, ElemType key, BitTree * node) {
  if (L == NULL) return NULL;
  if (L -> lchild != NULL)
    if (L -> lchild -> key == key && L -> rchild != NULL) {
      * node = L -> rchild;
      return * node;
    }
  if (RightSibling(L -> lchild, key, node)) return * node;
  if (RightSibling(L -> rchild, key, node)) return * node;
  return NULL;
}
/**
 * Function name: InsertChild
 * Function parameters: address of binary tree head pointer L , binary tree node pointer p, positioning variable LR , binary tree node pointer c
 * Function function: insert subtree c at node p
 * Return value: return OK if successful , otherwise return ERROR
 **/
Status InsertChild(BitTree * L, BitTree p, int LR, BitTree c) {
  if (LR == 0) {
    if (p -> rchild != NULL) {
      printf(" Binary tree C contains left subtree \n");
      return ERROR;
    }
    p -> lchild = c;
    return OK;
  } else if (LR == 1) {
    if (p -> rchild != NULL) {
      printf(" Binary tree C contains right subtree \n");
      return ERROR;
    }
    p -> rchild = c;
    return OK;
  } else return ERROR;
}
/**
 * Function name: DeleteChild
 * Function parameters: address of binary tree head pointer L , binary tree node pointer p, positioning variable LR
 * Function function: delete left / right subtree c at node p
 * Return value: return OK if successful , otherwise return ERROR
 **/
Status DeleteChild(BitTree * L, BitTree p, int LR) {
  if (LR == 0) {
    if (( * L) -> lchild == NULL) return FALSE;
    DestroyBiTree( & (( * L) -> lchild));
    p -> lchild = NULL;
  } else if (LR == 1) {
    if (( * L) -> rchild == NULL) return FALSE;
    DestroyBiTree( & (( * L) -> rchild));
    p -> rchild = NULL;
  } else return ERROR;
  return OK;
}
/**
 * Function name: PreOrderTraverse
 * Function parameters: Binary tree head pointer L
 * Function: Preorder traversal of the binary tree
 * Return value: OK
 **/
Status PreOrderTraverse(BitTree L) {
  if (L == NULL)
    return 0;
  printf("%d-%c ", L -> key, L -> data);
  if (L -> lchild != NULL)
    PreOrderTraverse(L -> lchild);
  if (L -> rchild != NULL)
    PreOrderTraverse(L -> rchild);
  return OK;
}
/**
 * Function name: InOrderTraverse
 * Function parameters: Binary tree head pointer L
 * Function function: Inorder traversal of binary tree
 * Return value: OK
 **/
Status InOrderTraverse(BitTree L) {
  if (L == NULL)
    return 0;
  if (L -> lchild != NULL)
    InOrderTraverse(L -> lchild);
  printf("%d-%c ", L -> key, L -> data);
  if (L -> rchild != NULL)
    InOrderTraverse(L -> rchild);
  return OK;
}
/**
 * Function name: PostOrderTraverse
 * Function parameters: Binary tree head pointer L
 * Function function: post-order traversal of the binary tree
 * Return value: OK
 **/
Status PostOrderTraverse(BitTree L) {
  if (L == NULL)
    return 0;
  if (L -> lchild != NULL)
    PostOrderTraverse(L -> lchild);
  if (L -> rchild != NULL)
    PostOrderTraverse(L -> rchild);
  printf("%d-%c ", L -> key, L -> data);
  return OK;
}
/**
 * Function name: LevelOrderTraverse
 * Function parameters: Binary tree head pointer L
 * Function function: traverse the binary tree in sequence
 * Return value: OK
 **/
Status LevelOrderTraverse(BitTree T) {
  if (T == NULL) return 0;
  BitTree q[1000];
  q[0] = T;
  int front = 0, rear = 1;
  while (front < rear) {
    printf("%d-%c ", q[front] -> key, q[front] -> data);
    if (q[front] -> lchild) q[rear++] = q[front] -> lchild;
    if (q[front] -> rchild) q[rear++] = q[front] -> rchild;
    ++front;
  }
  return OK;
}
/**
 * Function name: SaveBiTree
 * Function parameters: file pointer fp , binary tree head pointer L
 * Function function: save the data in L to the file pointed to by the fp pointer
 * Return value: OK
 **/
Status SaveBiTree(FILE * fp, BitTree L) {
  ElemType c = -1;
  if (L == NULL)
    fwrite( & (c), sizeof(ElemType), 1, fp);
  else {
    fwrite( & (L -> key), sizeof(ElemType), 1, fp);
    fwrite( & (L -> data), sizeof(char), 1, fp);
    SaveBiTree(fp, L -> lchild);
    SaveBiTree(fp, L -> rchild);
  }
  return OK;
}
/**
 * Function name: LoadBiTree
 * Function parameters: file pointer fp , binary tree head pointer L
 * Function function: data loading, input elements for the binary tree .
 * Return value: OK
 **/
Status ReadBiTree(FILE * fp, BitTree * L) {
  ElemType key;
  fread( & (key), sizeof(ElemType), 1, fp);
  if (key == -1) * L = NULL;
  else {
    * L = (BitTree) malloc(sizeof(BiTNode));
    ( * L) -> key = key;
    fread( & (( * L) -> data), sizeof(char), 1, fp);
    ReadBiTree(fp, & ( * L) -> lchild);
    ReadBiTree(fp, & ( * L) -> rchild);
  }
  return OK;
}
/**
 * Function name: FindNode
 * Function parameters: binary tree head pointer L , keyword key , address of binary tree node pointer node ,
 * Function function: find the node whose keyword is key and return its pointer
 * Return value: successfully return the address of node , otherwise return NULL
 **/
BitTree FindNode(BitTree L, ElemType key, BitTree * node) {
  if (L == NULL) return NULL;
  if (L -> key == key) {
    * node = L;
    return * node;
  }
  if (L -> lchild != NULL && FindNode(L -> lchild, key, node))
    return * node;
  if (L -> rchild != NULL && FindNode(L -> rchild, key, node))
    return * node;
  return NULL;
}
/**
 * Function name: Search
 * Function parameters: binary tree head pointer L , keyword key
 * Function function: find the node whose keyword is key and return its pointer
 * Return value: successfully return the address of M , otherwise return NULL
 **/
BitTree Search(BitTree L, ElemType key) {
  BitTree M = NULL;
  if (L == NULL) return M;
  if (L -> key == key) M = L;
  if (!M) M = Search(L -> lchild, key);
  if (!M) M = Search(L -> rchild, key);
  return M;
}
/**
 * Function name: Filename
 * Function parameters: none
 * Function function: automatically generate and modify the file name according to the system time
 * Return value: return time string pointer
 **/
char * Filename(void) {
  time_t lt;
  lt = time(NULL);
  filename = ctime( & lt);
  filename[strlen(filename) - 1] = '\0';
  strcat(filename, appe); // The file name string named at the above generation time
  pc = & filename[0];
  while ( * pc) { // normalize the file name string, replace ' : ' and ' ' with ' _ '
    if (( * pc == ':') || ( * pc == ' ')) {
      * pc = '_';
    }
    pc++;
  }
  return filename;
}