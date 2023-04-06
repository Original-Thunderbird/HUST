#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <time.h>

// function result status code
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR - 1

typedef int Status; // is the type of the function, its value is the function result status code
typedef int QElemType; // subscript corresponding to the vertex node
typedef struct QNode {
  QElemType data; // node information
  struct QNode * next;
}
QNode, * QueuePtr; // queue node structure
typedef struct {
  QueuePtr front, rear; // Queue head, queue tail pointer
}
LinkQueue; // queue structure

/**
 * Function name: InitQueue
 * Function parameters: queue structure pointer Q
 * Function function: initialize an empty queue
 * Return value: return ERROR on failure , return OK on success
 **/
Status InitQueue(LinkQueue * Q) {
  // Construct an empty queue Q
  ( * Q).front = ( * Q).rear = (QueuePtr) malloc(sizeof(QNode));
  if (!( * Q).front) {
    printf(" Failed to allocate space ");
    return ERROR;
  }
  ( * Q).front -> next = NULL;
  return OK;
}

/**
 * Function name: QueueEmpty
 * Function parameter: queue structure Q
 * Function function: determine whether the queue is empty
 * Return value: return TRUE if yes , return FALSE if successful
 **/
Status QueueEmpty(LinkQueue Q) {
  // If Q is an empty queue , return TRUE, otherwise return FALSE
  if (Q.front == Q.rear) return TRUE;
  else return FALSE;
}

/**
 * Function name: EnQueue
 * Function parameters: queue structure pointer Q , vertex subscript e
 * Function function: put the subscript into the queue
 * Return value: return ERROR on failure , return OK on success
 **/
Status EnQueue(LinkQueue * Q, QElemType e) {
  // Insert element e as the new tail element of Q
  QueuePtr p = (QueuePtr) malloc(sizeof(QNode));
  if (!p) // storage allocation failed
    return ERROR;
  p -> data = e;
  p -> next = NULL;
  ( * Q).rear -> next = p;
  ( * Q).rear = p;
  return OK;
}

/**
 * Function name: DeQueue
 * Function parameters: queue structure pointer Q , vertex subscript pointer e
 * Function function: take the subscript out of the queue
 * Return value: return ERROR on failure , return OK on success
 **/
Status DeQueue(LinkQueue * Q, QElemType * e) {
  // If the queue is not empty , delete the head element of Q , return its value with e , and return OK, otherwise return ERROR
  QueuePtr p;
  if (( * Q).front == ( * Q).rear) {
    printf(" queue is empty ");
    return ERROR;
  }
  p = ( * Q).front -> next;
  * e = p -> data;
  ( * Q).front -> next = p -> next;
  if (( * Q).rear == p)
    ( * Q).rear = ( * Q).front;
  free(p);
  return OK;
}

#define KIND 3 // The default type of graph: 0- directed graph , 1- directed network , 2- undirected graph , 3- undirected network
#define COLUMN 5 // The number of graphs
#define MAX_VERTEX_NUM 20 // Maximum number of vertices
#define MAX_VERNAME_LEN 10 // maximum length of vertex name
// Data element type definition
typedef int Boolean;
typedef int InfoType; // arc length in the net
typedef char VertexType; // node name
typedef struct ArcNode {
  int adjvex; // The position of the vertex pointed by the arc
  struct ArcNode * nextarc; // pointer to the next arc
  InfoType * info; // The pointer of the arc related information
}
ArcNode; // arc node structure
typedef struct VNode {
  ArcNode * firstarc; // Pointer to the first arc attached to this vertex
  char data; // vertex information
}
VNode, AdjList[MAX_VERTEX_NUM]; // vertex node structure
typedef struct {
  AdjList vertices;
  int vexnum, arcnum; // The current number of vertices and arcs of the graph
  int kind; // The kind flag of the graph
}
ALGraph; // graph structure
Boolean judge[COLUMN], flag = FALSE;
Boolean visited[COLUMN][MAX_VERTEX_NUM]; // visit flag array
int col = 1; // The serial number of the operation table, the initial is 1
FILE * fp;
char * filename, * pc, appe[5] = ".dat", input[60]; // used to generate file name
//Status (*VisitFunc)(VertexType a); // Function variable

Status CreateGraph(ALGraph * G); // construct graph G according to the definition of V and VR
Status DestroyGraph(ALGraph * G); // destroy graph G
int LocateVex(ALGraph * G, VertexType u); // If u exists in graph G , return the position number of vertex u , otherwise return -1
char GetVex(ALGraph * G, int rec);
Status PutVex(ALGraph * G, int rec, VertexType v);
int FirstAdjVex(ALGraph * G, VertexType v); // Return the first adjacent vertex of v , if v has no adjacent vertex, return empty
int NextAdjVex(ALGraph * G, VertexType v, VertexType w); // Return v 's (relative to w ) next adjacent vertex, if w is the last adjacent vertex, return empty .
Status InsertVex(ALGraph * G, VertexType v); // Add a new vertex v in graph G
Status DeleteVex(ALGraph * G, VertexType v); // Delete vertex v and arcs related to v in graph G
Status InsertArc(ALGraph * G, VertexType v, VertexType w); // Add arc <v,w> in graph G , if graph G is an undirected graph, also need to add <w,v>
Status DeleteArc(ALGraph * G, VertexType v, VertexType w); // Delete arc <v,w> in graph G , if graph G is an undirected graph, also need to delete <w,v>
Status DFSTraverse(ALGraph * G); // Depth-first search traversal of graph G
Status BFSTraverse(ALGraph * G); // Breadth -first search traversal on graph G
int OutDegree(AdjList i); // Calculate the outdegree of the ith fixed point in graph G
Status SaveGraph(ALGraph List[]); // Save
Status LoadGraph(ALGraph List[]); // Read
Status ChooseList(int col, ALGraph ** G, ALGraph List[]);

int main() {
  int op, rec;
  VertexType e, v, w;
  ALGraph * G = NULL, List[COLUMN];
  G = List;
  while (op) {
    system("cls");
    printf("\n\n");
    printf(" This system operates an undirected network There are %d images in the system Current operation %d sheet \n", COLUMN, col);
    printf(" Menu for Linear Table On Node Structure \n");
    printf("---------------------------------------------- --\n");
    printf(" 1. CreateGraph 9. DeleteVex\n");
    printf(" 2. DestroyGraph 10. InsertArc\n");
    printf(" 3. LocateVex 11. DeleteArc\n");
    printf(" 4. GetVex 12. DFSTraverse\n");
    printf(" 5. PutVex 13. BFSTraverse\n");
    printf(" 6. FirstAdjVex 14. SaveGraph\n");
    printf(" 7. NextAdjVex 15. LoadGraph\n");
    printf(" 8. InsertVex 16. ChooseGraph\n");
    printf(" 0. Exit\n");
    printf("---------------------------------------------- --\n");
    printf(" Please choose your operation [0~16]:");
    scanf("%d", & op);
    switch (op) {
    case 1:
      if (CreateGraph(G) == OK) printf(" The graph was built successfully \n");
      else printf(" Graph construction failed \n");
      getchar();
      getchar();
      break;
    case 2:
      if (DestroyGraph(G) == OK) printf(" Graph destroyed successfully \n");
      else printf(" Graph destruction failed \n");
      getchar();
      getchar();
      break;
    case 3:
      printf(" Please enter the name of the vertex e you want to find (char type ):");
      fflush(stdin);
      scanf("%c", & e);
      getchar();
      rec = LocateVex(G, e);
      if (rec == -1) printf(" This vertex does not exist \n");
      else printf(" The serial number of the vertex in the graph or network is %d\n", rec);
      getchar();
      getchar();
      break;
    case 4:
      printf(" Please enter the subscript of the vertex rec you want to find (int type ):");
      fflush(stdin);
      scanf("%d", & rec);
      getchar();
      e = GetVex(G, rec);
      if (e == ERROR) printf(" Find failed \n");
      else printf("The name of %d is %c\n", rec, e);
      getchar();
      getchar();
      break;
    case 5:
      printf(" Please enter the serial number (int type ) of the vertex rec to be modified :");
      fflush(stdin);
      scanf("%d", & rec);
      getchar();
      printf(" Enter new name v(char type ):");
      scanf("%c", & v);
      getchar();
      if (PutVex(G, rec, v) == OK) printf(" Modified successfully \n");
      else printf(" Modification failed \n");
      getchar();
      getchar();
      break;
    case 6:
      flag = FALSE;
      printf(" Please enter the name of the vertex e you want to find (char type ):");
      fflush(stdin);
      scanf("%c", & e);
      getchar();
      rec = FirstAdjVex(G, e);
      if (rec == ERROR) {
        if (flag == TRUE) {
          printf("v has no adjacent vertices ");
          flag = FALSE;
        }
        printf(" return failed \n");
      } else
        printf(" The serial number of the first adjacent vertex of v is %d\n", rec);
      getchar();
      getchar();
      break;
    case 7:
      flag = FALSE;
      printf(" Please enter the name of the vertex v you want to find and its relative vertex w (char type ):");
      fflush(stdin);
      scanf("%c", & v);
      getchar();
      scanf("%c", & w);
      getchar();
      rec = NextAdjVex(G, v, w);
      if (rec == ERROR) {
        if (flag == TRUE) {
          printf("w is the last adjacent vertex of v ");
          flag = FALSE;
        }
        printf(" return failed \n");
      } else printf(" The serial number of v's next adjacent vertex relative to w is %d\n", rec);
      getchar();
      getchar();
      break;
    case 8:
      printf(" Please enter the name of the vertex v to be added (char type ):");
      fflush(stdin);
      scanf("%c", & v);
      getchar();
      if (InsertVex(G, v) == OK) printf(" Added successfully \n");
      else printf(" Add failed \n");
      getchar();
      getchar();
      break;
    case 9:
      printf(" Please enter the name of the vertex v to be deleted (char type ):");
      fflush(stdin);
      scanf("%c", & v);
      getchar();
      if (DeleteVex(G, v) == ERROR) printf(" This vertex does not exist \n");
      else printf(" delete successfully \n");
      getchar();
      getchar();
      break;
    case 10:
      printf(" Please input the name of vertex v and vertex w to add arc (char type ):");
      fflush(stdin);
      scanf("%c", & v);
      getchar();
      scanf("%c", & w);
      getchar();
      if (InsertArc(G, v, w) == ERROR) printf(" Vertex v or w does not exist ");
      else printf(" Added successfully \n");
      getchar();
      getchar();
      break;
    case 11:
      printf(" Please input the names of vertex v and vertex w to delete the arc (char type ):");
      fflush(stdin);
      scanf("%c", & v);
      getchar();
      scanf("%c", & w);
      getchar();
      if (DeleteArc(G, v, w) == ERROR) printf(" Vertex v or w does not exist ");
      else printf(" Arc deleted successfully ");
      getchar();
      getchar();
      break;
    case 12:
      DFSTraverse(G);
      getchar();
      getchar();
      break;
    case 13:
      BFSTraverse(G);
      getchar();
      getchar();
      break;
    case 14:
      if (SaveGraph(List) == OK) printf(" Save successfully \n");
      else printf(" Failed to save \n");
      getchar();
      getchar();
      break;
    case 15:
      if (LoadGraph(List) == OK) printf(" Loading successfully \n");
      else printf(" Failed to load \n");
      getchar();
      getchar();
      break;
    case 16:
      printf(" Please input the sequence vol:(1<=col<=%d)", COLUMN);
      scanf("%d", & col);
      ChooseList(col, & G, List);
      getchar();
      getchar();
    case 0:
      break;
    } //end of switch
  } //end of while
  return 0;
}

/**
 * Function name: CreateGraph
 * Function parameters: graph structure pointer G
 * Function function: construct a graph G
 * Return value: return OK if successfully constructed , otherwise return ERROR
 **/
Status CreateGraph(ALGraph * G) {
  int i, j, k;
  int w; // weight W
  VertexType va, vb; // Arc tail Va , arc head Vb
  ArcNode * p;
  /*
  printf(" Please enter 3 basic information of the graph \nType ( 0- directed graph , 1- directed network , 2- undirected graph , 3- undirected network ):\n");
  // Output image type
  scanf("%d",&(*G).kind); getchar();
  while ((*G). kind < 0 || (*G). kind > 3)
  {
  printf(" Illegal input value Please retype :\n");
  scanf("%d",&(*G).kind); getchar();
  }
  */
  if (judge[col - 1] == TRUE) {
    printf(" The current location has a picture Please destroy it before creating a new one ");
    return ERROR;
  }
  G -> kind = KIND; // The default is undirected network
  // Enter the number of vertices
  printf(" Number of vertices ( within %d ) :", MAX_VERTEX_NUM);
  scanf("%d", & (G -> vexnum));
  getchar();
  while (G -> vexnum < 0 || G -> vexnum > MAX_VERTEX_NUM) {
    printf(" Illegal input value Please retype :");
    scanf("%d", & (G -> vexnum));
    getchar();
  }
  if (G -> vexnum) judge[col - 1] = TRUE;
  // Number of sides
  printf(" Number of arcs ( edges ) :");
  scanf("%d", & (G -> arcnum));
  getchar();
  printf(" Please enter the name or value of %d vertices (1 character , separated by spaces ):", ( * G).vexnum);
  for (i = 0; i < G -> vexnum; i++) // Input each vertex information
  {
    scanf("%c", & (G -> vertices[i].data));
    getchar();
    G -> vertices[i].firstarc = NULL;
  }
  if (G -> kind % 2) //1 and 3 are nets
    printf(" Please enter the arc ( edge ) weight W , arc tail Va and arc head Vb in order ( separated by spaces ):\n");
  else //0 and 2 are pictures
    printf(" Please enter arc tail Va and arc head Vb ( separated by spaces ) of the arc ( edge ) in order:\n");
  for (k = 0; k < G -> arcnum; k++) // construct adjacency list
  {
    printf("%d:", k + 1);
    if (G -> kind % 2) // net
      scanf("%d %c %c", & w, & va, & vb);
    else // graph
      scanf("%c %c", & va, & vb);
    i = LocateVex(G, va); // arc tail
    j = LocateVex(G, vb); // arc head
    if (i == -1 || j == -1) { // vertex does not exist
      printf(" Vertex does not exist Please retype \n");
      k--;
      continue;
    }
    p = (ArcNode * ) malloc(sizeof(ArcNode));
    p -> adjvex = j;
    if (( * G).kind % 2) // net
    {
      p -> info = (int * ) malloc(sizeof(int));
      *(p -> info) = w;
    } else // graph
      p -> info = NULL; //
    p -> nextarc = ( * G).vertices[i].firstarc; // Insert in the header, last in first out
    G -> vertices[i].firstarc = p;
    // Undirected graph or undirected network needs reverse
    if (G -> kind >= 2) {
      p = (ArcNode * ) malloc(sizeof(ArcNode));
      p -> adjvex = i;
      if (( * G).kind == 3) // undirected network
      {
        p -> info = (int * ) malloc(sizeof(int));
        *(p -> info) = w;
      } else // undirected graph
        p -> info = NULL;
      p -> nextarc = ( * G).vertices[j].firstarc; // Insert in the header
      G -> vertices[j].firstarc = p;
    }
  }
  return OK;
}

/**
 * Function name: DestroyGraph
 * Function parameters: graph structure pointer G
 * Function function: destroy graph G
 * Return value: return OK if destroyed successfully , otherwise return ERROR
 **/
Status DestroyGraph(ALGraph * G) {
  if (judge[col - 1] != TRUE) {
    printf(" Graph not initialized ");
    return ERROR;
  }
  int i;
  ArcNode * p, * q;
  for (i = 0; i < G -> vexnum; i++) {
    p = G -> vertices[i].firstarc;
    while (p) {
      q = p -> nextarc;
      if (( * G).kind % 2) // net
        free(p -> info);
      free(p);
      p = q;
    }
  }
  G -> vexnum = G -> arcnum = 0;
  judge[col - 1] = FALSE;
  return OK;
}

/**
 * Function name: LocateVex
 * Function parameters: graph structure pointer G , vertex name u to be checked
 * Function function: find the vertex named u
 * Return value: find the subscript of the returned node, otherwise return ERROR (ie -1 )
 **/
int LocateVex(ALGraph * G, VertexType u) {
  if (judge[col - 1] != TRUE) {
    printf(" Graph not initialized ");
    return ERROR;
  }
  if (G -> vexnum == 0) {
    printf(" The picture is empty ");
    return ERROR;
  }
  int i;
  for (i = 0; i < G -> vexnum; i++)
    if (u == G -> vertices[i].data)
      return i;
  return -1; // return -1 if not found
}

/**
 * Function name: GetVex
 * Function parameters: graph structure pointer G , vertex subscript rec to be checked
 * Function function: get the name of the vertex whose subscript is rec
 * Return value: find the return node name, otherwise return ERROR
 **/
VertexType GetVex(ALGraph * G, int rec) {
  if (judge[col - 1] != TRUE) {
    printf(" Graph not initialized ");
    return ERROR;
  }
  if (G -> vexnum == 0) {
    printf(" The picture is empty ");
    return ERROR;
  }
  if (rec >= G -> vexnum) {
    printf(" subscript out of bounds ");
    return ERROR;
  }
  return G -> vertices[rec].data;
}

/**
 * Function name: PutVex
 * Function parameters: graph structure pointer G , subscript rec of the vertex to be modified , new name of the corresponding node v
 * Function function: Assign the name u to the vertex whose subscript is rec
 * Return value: return OK if found , otherwise return ERROR
 **/
Status PutVex(ALGraph * G, int rec, VertexType v) {
  if (judge[col - 1] != TRUE) {
    printf(" Graph not initialized ");
    return ERROR;
  }
  if (G -> vexnum == 0) {
    printf(" The picture is empty ");
    return ERROR;
  }
  if (rec >= G -> vexnum) {
    printf(" subscript out of bounds ");
    return ERROR;
  }
  G -> vertices[rec].data = v;
  return OK;
}

/**
 * Function name: FirstAdjVex
 * Function parameters: graph structure pointer G , vertex name v to be checked
 * Function function: Find the subscript of the first adjacent node of the node named v
 * Return value: return subscript value if found, otherwise return ERROR
 **/
int FirstAdjVex(ALGraph * G, VertexType v) {
  ArcNode * p;
  int v1;
  v1 = LocateVex(G, v); // Get the serial number v1 of v in graph G
  if (judge[col - 1] != TRUE) {
    printf(" Graph not initialized ");
    return ERROR;
  }
  if (G -> vexnum == 0) {
    printf(" The picture is empty ");
    return ERROR;
  }
  if (v1 == -1) {
    printf(" This vertex does not exist ");
    return ERROR;
  }
  p = G -> vertices[v1].firstarc;
  if (p) return p -> adjvex;
  else {
    flag = TRUE;
    return ERROR;
  }
}

/**
 * Function name: NextAdjVex
 * Function parameters: graph structure pointer G , the name of the vertex to be checked v , the name of the adjacent vertex w of v
 * Function function: Find the next adjacent vertex named v ( relative to the vertex named w )
 * Return value: the serial number of the next adjacent vertex of v ( relative to w ) ,
 * If w is the last adjacent point of v or the search fails , return ERROR
 **/
int NextAdjVex(ALGraph * G, VertexType v, VertexType w) {
  ArcNode * p;
  int v1, w1;
  if (judge[col - 1] != TRUE) {
    printf(" Graph not initialized ");
    return ERROR;
  }
  if (G -> vexnum == 0) {
    printf(" The picture is empty ");
    return ERROR;
  }
  v1 = LocateVex(G, v); // Get the serial number v1 of v in graph G
  w1 = LocateVex(G, w); // Get the serial number w1 of w in graph G
  if (v1 == -1) { // vertex does not exist
    printf(" Vertex v does not exist ");
    return ERROR;
  }
  if (w1 == -1) { // adjacent point does not exist
    printf(" Vertex w does not exist ");
    return ERROR;
  }
  p = G -> vertices[v1].firstarc;
  while (p && p -> adjvex != w1) // The pointer p is not empty and the pointed table node is not w
    p = p -> nextarc;
  // Did not find w or w is the last adjacent point
  if (!p) {
    printf("w is not an adjacent vertex of v ");
    return ERROR;
  } else if (!p -> nextarc) {
    flag = TRUE;
    return ERROR; //flag is used to distinguish whether to call from the main function or from other functions to decide whether to output relevant prompt information
  } else return p -> nextarc -> adjvex;
  // Return the serial number of the next adjacent vertex of v ( relative to w )
}

/**
 * Function name: InsertVex
 * Function parameters: graph structure pointer G , new vertex name v
 * Function function: add a new vertex named v in graph G
 * Return value: return OK if successful , otherwise return ERROR
 **/
Status InsertVex(ALGraph * G, VertexType v) {
  if (judge[col - 1] != TRUE) {
    printf(" Graph not initialized ");
    return ERROR;
  }
  if (( * G).vexnum == 0) {
    printf(" The picture is empty ");
    return ERROR;
  }
  if (( * G).vexnum == MAX_VERTEX_NUM) { // The number of vertices reaches the maximum value
    printf(" The number of vertices reaches the maximum value ");
    return ERROR;
  }
  // construct a new vertex
  ( * G).vertices[( * G).vexnum].data = v;
  ( * G).vertices[( * G).vexnum].firstarc = NULL;
  ( * G).vexnum++; // Add 1 to the number of vertices in graph G
  return OK;
}

/**
 * Function name: DeleteVex
 * Function parameters: graph structure pointer G , vertex name v to be deleted
 * Function function: delete the vertex named v and its related arc in G
 * Return value: return OK if successful , otherwise return ERROR
 **/
Status DeleteVex(ALGraph * G, VertexType v) {
  int i, j;
  ArcNode * p, * q;
  if (judge[col - 1] != TRUE) {
    printf(" Graph not initialized ");
    return ERROR;
  }
  if (( * G).vexnum == 0) {
    printf(" The picture is empty ");
    return ERROR;
  }
  j = LocateVex(G, v); // Get the serial number of vertex v
  if (j == -1) return ERROR; //v is not a vertex of graph G {
  // Delete the arc or edge with v as the out degree
  p = ( * G).vertices[j].firstarc;
  while (p) {
    q = p;
    p = p -> nextarc;
    if (( * G).kind % 2) // net
      free(q -> info);
    free(q);
    ( * G).arcnum--; // The number of arcs or edges minus 1
  }
  // rearrange vertices
  ( * G).vexnum--; // The number of vertices minus 1
  for (i = j; i < ( * G).vexnum; i++) // The vertex behind vertex v moves forward
    ( * G).vertices[i] = ( * G).vertices[i + 1];
  // Delete the arc or edge with v as the in-degree and modify the position value of the vertex pointed by the arc
  for (i = 0; i < ( * G).vexnum; i++) {
    p = ( * G).vertices[i].firstarc; // point to the first arc or edge
    while (p) // has arc or edge
    {
      if (p -> adjvex == j) // The arc points to the deleted node
      {
        if (p == ( * G).vertices[i].firstarc) // The node to be deleted is the first node
        {
          ( * G).vertices[i].firstarc = p -> nextarc;
          if (( * G).kind % 2) // net
            free(p -> info);
          free(p);
          p = ( * G).vertices[i].firstarc;
          if (( * G).kind < 2) // The directed graph needs to be subtracted again
            ( * G).arcnum--; // The number of arcs or edges minus 1
        } else {
          q -> nextarc = p -> nextarc;
          if (( * G).kind % 2) // net
            free(p -> info);
          free(p);
          p = q -> nextarc;
          if (( * G).kind < 2) // The direction needs to be reduced again
            ( * G).arcnum--; // The number of arcs or edges minus 1
        }
      } else // The arc does not point to the deleted node
      {
        if (p -> adjvex > j) // The position number of the element pointed by the arc is after the vertex is deleted
          p -> adjvex--; // Change the position number of the vertex pointed by the arc
        q = p;
        p = p -> nextarc; // analyze the next arc
      }
    }
  }
  if (G -> vexnum == 0) printf(" There is no vertex in the graph ");
  return OK;
}

/**
 * Function name: InsertArc
 * Function parameters: graph structure pointer G , vertex name v , w
 * Function function: add arc <v,w> in graph G , if graph G is an undirected graph / network, also need to add <w,v>
 * Return value: return OK if successful , otherwise return ERROR
 **/
Status InsertArc(ALGraph * G, VertexType v, VertexType w) {
  ArcNode * p;
  int w1, i, j;
  if (judge[col - 1] != TRUE) {
    printf(" Graph not initialized ");
    return ERROR;
  }
  if (( * G).vexnum == 0) {
    printf(" The picture is empty ");
    return ERROR;
  }
  i = LocateVex(G, v); // Get the serial number of v position
  j = LocateVex(G, w); // Get the position number of w
  if (i == -1 || j == -1) return ERROR; // Corresponding vertex not found
  // create arc vw
  ( * G).arcnum++;
  if (( * G).kind % 2) // The network needs to lose weight
  {
    printf(" Please enter the weight of the arc ( edge )%c->%c :\n", v, w);
    scanf("%d", & w1);
    getchar();
  }
  p = (ArcNode * ) malloc(sizeof(ArcNode));
  p -> adjvex = j;
  if (( * G).kind % 2) // net
  {
    p -> info = (int * ) malloc(sizeof(int));
    *(p -> info) = w1;
  } else p -> info = NULL;
  p -> nextarc = ( * G).vertices[i].firstarc;
  ( * G).vertices[i].firstarc = p;
  // The undirected graph needs to create an arc w->v
  if (( * G).kind >= 2) {
    p = (ArcNode * ) malloc(sizeof(ArcNode));
    p -> adjvex = i;
    if (( * G).kind == 3) // undirected network
    {
      p -> info = (int * ) malloc(sizeof(int));
      *(p -> info) = w1;
    } else p -> info = NULL;
    p -> nextarc = ( * G).vertices[j].firstarc;
    ( * G).vertices[j].firstarc = p;
  }
  return OK;
}

/**
 * Function name: DeleteArc
 * Function parameters: graph structure pointer G , names of two vertices v and w to be deleted
 * Function function: delete the arc <v,w> in the graph G , if the graph G is an undirected graph / network, you need to add <w,v>
 * Return value: return OK if successful , otherwise return ERROR
 **/
Status DeleteArc(ALGraph * G, VertexType v, VertexType w) {
  ArcNode * p, * q;
  int i, j;
  if (judge[col - 1] != TRUE) {
    printf(" Graph not initialized ");
    return ERROR;
  }
  if (( * G).vexnum == 0) {
    printf(" The picture is empty ");
    return ERROR;
  }
  if (( * G).arcnum == 0) {
    printf(" There is no arc in the graph ");
    return ERROR;
  }
  i = LocateVex(G, v); // Vertex v number
  j = LocateVex(G, w); // The serial number of vertex w
  if (i < 0 || j < 0) return ERROR;
  // delete arc <v,w>
  p = ( * G).vertices[i].firstarc;
  while (p && p -> adjvex != j) //p is not empty and the arc pointed to is not an arc to be deleted <v,w>
  { //p points to the next arc
    q = p;
    p = p -> nextarc;
  }
  if (p && p -> adjvex == j) // find arc <v,w>
  {
    if (p == ( * G).vertices[i].firstarc) //p refers to the first arc
      ( * G).vertices[i].firstarc = p -> nextarc; // point to the next arc
    else q -> nextarc = p -> nextarc; // point to the next arc
    if (( * G).kind % 2) free(p -> info); // The network needs to release more weights
    free(p); // release this node
    ( * G).arcnum--; // The number of arcs or edges minus 1
  }
  // The undirected graph needs to delete the symmetric arc <w,v>
  if (( * G).kind >= 2) {
    p = ( * G).vertices[j].firstarc;
    while (p && p -> adjvex != i) //p is not empty and the arc pointed to is not an arc to be deleted <w,v>
    { //p points to the next arc */
      q = p;
      p = p -> nextarc;
    }
    if (p && p -> adjvex == i) // find arc <w,v>
    {
      if (p == ( * G).vertices[j].firstarc) //p refers to the first arc
        ( * G).vertices[j].firstarc = p -> nextarc; // point to the next arc
      else q -> nextarc = p -> nextarc; // point to the next arc
      if (( * G).kind % 2) free(p -> info); // The network needs to release more weights
      free(p); // release this node
    }
  }
  if (( * G).arcnum == 0) printf(" There is no arc in the graph ");
  return OK;
}

/**
 * Function name: DFS
 * Function parameters: graph structure pointer G , vertex subscript v
 * Function function: starting from the vth vertex, recursively traverse the graph G in depth first
 * Return value: None
 **/
void DFS(ALGraph * G, int v) {
  int w;
  VertexType v1, w1;
  v1 = G -> vertices[v].data;
  visited[col - 1][v] = TRUE; // Set the visited flag to TRUE ( visited )
  printf(" Number : %d- Name : %c ", v, G -> vertices[v].data); /* Visit the vth vertex */
  for (w = FirstAdjVex(G, v1); w >= 0; w = NextAdjVex(G, v1, w1 = G -> vertices[w].data))
    if (!visited[col - 1][w])
      DFS(G, w); // Call DFS recursively on v 's unvisited adjacent point w
}

/* Function name: DFSTraverse
 * Input parameters: Figure G, calling the function visit
 * return value:  Status returns OK successfully ;
 * Function function: Perform a depth-first search traversal on the graph G , and use the function for each vertex in the graph in turn
 * Number of visits to visit once, and only visit once
 */

/**
 * Function name: DFSTraverse
 * Function parameters: graph structure pointer G
 * Function function: perform a depth-first search traversal on the graph G , and use the function on each vertex in the graph in turn
 * Number of visits to visit once, and only visit once
 * Return value: return OK if successful , otherwise return ERROR
 **/
Status DFSTraverse(ALGraph * G) {
  int v;
  if (judge[col - 1] != TRUE) {
    printf(" Graph not initialized ");
    return ERROR;
  }
  if (G -> vexnum == 0) {
    printf(" The picture is empty ");
    return ERROR;
  }
  // VisitFunc = Visit; // Use the global variable VisitFunc, so that DFS does not need to set function pointer parameters
  for (v = 0; v < G -> vexnum; v++)
    visited[col - 1][v] = FALSE; // visit flag array initialization
  for (v = 0; v < G -> vexnum; v++)
    if (!visited[col - 1][v])
      DFS(G, v); // Call DFS on vertices that have not been visited yet
  return OK;
}

/**
 * Function name: BFSTraverse
 * Function parameters: graph structure pointer G
 * Function function: Traverse graph G non-recursively according to breadth first
 * Return value: return OK if successful , otherwise return ERROR
 **/
Status BFSTraverse(ALGraph * G) {
  int v, u, w;
  VertexType u1, w1;
  LinkQueue Q;
  if (judge[col - 1] != TRUE) {
    printf(" Graph not initialized ");
    return ERROR;
  }
  if (G -> vexnum == 0) {
    printf(" The picture is empty ");
    return ERROR;
  }
  for (v = 0; v < G -> vexnum; ++v) visited[col - 1][v] = FALSE;
  InitQueue( & Q); // Empty auxiliary queue Q
  for (v = 0; v < G -> vexnum; ++v)
    if (!visited[col - 1][v]) //v has not been visited yet
  {
    visited[col - 1][v] = TRUE;
    printf(" Number : %d- Name : %c ", v, G -> vertices[v].data);
    EnQueue( & Q, v); //v into the queue
    while (!QueueEmpty(Q)) {
      DeQueue( & Q, & u); // The head element of the queue is dequeued and set to u
      u1 = G -> vertices[u].data;
      for (w = FirstAdjVex(G, u1); w >= 0; w = NextAdjVex(G, u1, w1 = G -> vertices[w].data))
        if (!visited[col - 1][w]) //w is the unvisited adjacent vertex of u
      {
        visited[col - 1][w] = TRUE;
        printf(" Number : %d- Name : %c ", w, G -> vertices[w].data);
        EnQueue( & Q, w); //w enters the queue
      }
    }
  }
  return OK;
}

/**
 * Function name: OutDegree
 * Function parameters: the i- th vertex address of graph G
 * Function function: Calculate the out-degree of the i- th vertex
 * Return value: out- degree of the i- th vertex
 **/
int OutDegree(AdjList i) {
  int num = 0;
  ArcNode * p;
  p = i -> firstarc;
  while (p != NULL) {
    num++;
    p = p -> nextarc;
  }
  return num;
}

/**
 * Function name: LoadGraph
 * Function parameters: graph structure array List
 * Function function: read the information of multiple graphs from the file
 * Return value: return OK if successful , otherwise return ERROR
 **/
Status LoadGraph(ALGraph List[COLUMN]) {
  /*
  if(judge!=TRUE){
  printf(" Graph not initialized "); return ERROR;
  }
  */
  int i, j, k, num, yl; //k is the vertex subscript , num is the outgoing degree of each vertex
  FILE * fp;
  ArcNode * p;
  printf(" Please enter the file name where the data you want to restore is located :");
  scanf("%s", input);
  filename = input;
  if ((fp = fopen(filename, "r")) == NULL) {
    printf(" Failed to open the file ");
    return ERROR;
  }
  fread( & yl, sizeof(int), 1, fp); // read the number of valid tables
  // Read the basic information of the graph
  for (i = 0; i < yl; i++) {
    fread( & (List[i].kind), sizeof(int), 1, fp); // kind
    fread( & (List[i].vexnum), sizeof(int), 1, fp); // number of points
    fread( & (List[i].arcnum), sizeof(int), 1, fp); // Number of sides
    // read the adjacency list
    for (j = 0; j < List[i].vexnum; j++) { // There are remaining vertices
      fread( & (List[i].vertices[j].data), sizeof(char), 1, fp); // name
      printf(" Figure- %d, serial number- %d, name- %c", i, j, List[i].vertices[j].data);
      fread( & num, sizeof(int), 1, fp); // current vertex out degree
      if (num == 0) List[i].vertices[j].firstarc = NULL; // out degree is 0
      else {
        printf(" Adjacency number :");
        List[i].vertices[j].firstarc = NULL;
        for (k = 0; k < num; k++) {
          p = (ArcNode * ) malloc(sizeof(ArcNode));
          fread( & (p -> adjvex), sizeof(int), 1, fp); // adjacent point number
          printf("%d ", p -> adjvex);
          if (List[i].kind % 2) // net
          {
            p -> info = (int * ) malloc(sizeof(int));
            fread(p -> info, sizeof(InfoType), 1, fp); //
          } else p -> info = NULL; // graph
          p -> nextarc = List[i].vertices[j].firstarc; // Insert in the header, last in first out
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
 * Function name: SaveGraph
 * Function parameters: graph structure array List
 * Function function: save the information of multiple valid graphs into a file
 * Return value: return OK if successful , otherwise return ERROR
 **/
Status SaveGraph(ALGraph List[COLUMN]) {
  int i, h, num, yl = 0;
  FILE * fp;
  ArcNode * p;
  for (i = 0; i < COLUMN; i++) { // count the number of tables with elements
    if (judge[i] == TRUE && List[i].vexnum != 0)
      yl++;
  }
  if (yl == 0) { // returns an error if there is no valid data
    printf(" No data to save ");
    return ERROR;
  }
  /*
  printf(" Please enter the save file name :");
  scanf("%s",filename); getchar();
  */
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
  // method to write file
  if ((fp = fopen(filename, "w")) == NULL) {
    printf(" Failed to open the file ");
    return ERROR;
  }
  fwrite( & yl, sizeof(int), 1, fp); // The number of valid tables written
  // Save the basic information of the graph
  for (h = 0; h < COLUMN; h++) {
    if (judge[h] == TRUE && List[h].vexnum != 0) {
      fwrite( & (List[h].kind), sizeof(int), 1, fp); // kind
      fwrite( & (List[h].vexnum), sizeof(int), 1, fp); // number of points
      fwrite( & (List[h].arcnum), sizeof(int), 1, fp); // Number of sides
      // save the adjacency list
      for (i = 0; i < List[h].vexnum; i++) {
        fwrite( & (List[h].vertices[i].data), sizeof(char), 1, fp); // name
        printf(" Table- %d, serial number- %d, name- %c ", h, i, List[h].vertices[i].data);
        if (List[h].vertices[i].firstarc == NULL) printf("\n\n");
        num = OutDegree( & (List[h].vertices[i])); // current vertex outdegree
        fwrite( & num, sizeof(int), 1, fp);
        if ((p = List[h].vertices[i].firstarc) != NULL) {
          fwrite( & (p -> adjvex), sizeof(int), 1, fp); // adjacent vertex number
          printf(" Number of adjacent points :%d ", p -> adjvex);
          if (List[h].kind % 2) // net
            fwrite(p -> info, sizeof(int), 1, fp); // arc length
          while ((p = p -> nextarc) != NULL) {
            fwrite( & (p -> adjvex), sizeof(int), 1, fp); // adjacent vertex number
            printf("%d ", p -> adjvex);
            if (List[h].kind % 2) // net
              fwrite(p -> info, sizeof(int), 1, fp); // arc length
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
 * Function name: ChooseList
 * Function parameters: table position col to be replaced , graph structure pointer G , graph structure array List
 * Function function: point G to the subscript col-1 of the graph to be operated
 * Return value: return ERROR on failure , return OK on success
 **/
Status ChooseList(int col, ALGraph ** G, ALGraph List[COLUMN]) {
  if (col < 1 || col > COLUMN) {
    printf(" Subscript out of bounds Operation failed ");
    return ERROR;
  }
  * G = & (List[col - 1]);
  if (judge[col - 1] != TRUE)
    printf(" This table has not been initialized "); // Prevent misoperation
  return OK;
}