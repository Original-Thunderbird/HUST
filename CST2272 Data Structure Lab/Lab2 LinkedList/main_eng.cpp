/* Linear Table On Sequence Structure */
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define OK 1
#define TRUE 1
#define FALSE - 1
#define ERROR - 2
typedef int status;
typedef int ElemType; // Data element type definition
typedef struct lnode { // element node definition
  ElemType data; // data field
  struct lnode * next; // pointer field
}
LNode;
typedef struct llist { // head node definition
  int len; //len stores the length of this table
  struct lnode * head, * tail; //head points to the first element in the table, tail points to the tail element
  struct llist * fore, * aft; //aft points to the next header node, ore points to the previous header node, this way of definition determines that there should be a blank header node before the first header node .
}
Linklist;
int length = 0, vol = 0; //length is the number of tables, vol is the bit sequence of the current operation table
FILE * fp;
char * filename, * pc, appe[5] = ".dat", appf[5] = ".txt", input[60]; // are used to generate file names

status IntiaList(Linklist ** L);
status DestroyList(Linklist * L);
status ClearList(Linklist * L);
status ListEmpty(Linklist * L);
int ListLength(Linklist * L);
status GetElem(Linklist * L, int i, ElemType & e);
status LocateElem(Linklist * L, ElemType e); // simplified
status PriorElem(Linklist * L, ElemType cur, ElemType & pre_e);
status NextElem(Linklist * L, ElemType cur, ElemType & next_e);
status ListInsert(Linklist * L, int i, ElemType e);
status ListDelete(Linklist * L, int i, ElemType & e);
status ListTrabverse(Linklist * L); // simplified
status LoadList(FILE * fp, Linklist * List);
status SaveList(FILE * fp, Linklist * List);
status ChooseList(int vol, Linklist ** L, Linklist * List);
status ChartTraverse(Linklist * List);

int main(void) {
  Linklist * List = NULL, * L = NULL;
  int op = 1, i, cur, pre_e, next_e;
  while (op) {
    system("cls");
    printf("\n\n");
    printf(" There are %d tables in the system , and the %d table is currently being operated \n", length, vol); // Prompt the bit sequence of the currently operating table
    printf(" Menu for Linear Table On Node Structure \n");
    printf("---------------------------------------------- --\n");
    printf(" 1. IntiaList 7. LocateElem\n");
    printf(" 2. DestroyList 8. PriorElem\n");
    printf(" 3. ClearList 9. NextElem \n");
    printf(" 4. ListEmpty 10. ListInsert\n");
    printf(" 5. ListLength 11. ListDelete\n");
    printf(" 6. GetElem 12. ListTrabverse\n");
    printf(" 13.LoadList 14.SaveList\n");
    printf(" 15. ChooseList 16. ChartTraverse\n");
    printf(" 0. Exit\n");
    printf("---------------------------------------------- --\n");
    printf(" Please choose your operation [0~16]:");
    scanf("%d", & op);
    switch (op) {
    case 1:
      if (IntiaList( & List) == OK) {
        printf(" Linear table created successfully! ");
        L = List -> aft;
        vol = 1;
      } else printf(" Linear table creation failed! ");
      getchar();
      getchar();
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
      if (i == TRUE) printf(" The linear table is empty! ");
      else if (i == FALSE) printf(" The linear table is not empty! ");
      getchar();
      getchar();
      break;
    case 5:
      if (ListLength(L) == ERROR) {
        getchar();
        getchar();
        break;
      }
      printf(" Table length is %d", L -> len);
      getchar();
      getchar();
      break;
    case 6:
      if (L == NULL) {
        printf(" table not initialized ");
        getchar();
        break;
      }
      printf(" Please enter element sequence i(1<=i<%d):", L -> len + 1);
      scanf("%d", & i);
      if (GetElem(L, i, cur) == OK)
        printf(" The %dth element is %d", i, cur);
      getchar();
      getchar();
      break;
    case 7:
      printf(" Please enter the data to be checked :");
      scanf("%d", & cur);
      i = LocateElem(L, cur);
      if (i > 0)
        printf("%d is the %dth element in the table ", cur, i);
      if (!i)
        printf(" The element was not found ");
      getchar();
      getchar();
      break;
    case 8:
      printf(" Please enter location data :");
      scanf("%d", & cur);
      if (PriorElem(L, cur, pre_e) == OK)
        printf(" The predecessor of %d is %d", cur, pre_e);
      getchar();
      getchar();
      break;
    case 9:
      printf(" Please enter location data :");
      scanf("%d", & cur);
      if (NextElem(L, cur, next_e) == OK)
        printf("%d' s successor is %d", cur, next_e);
      getchar();
      getchar();
      break;
    case 10:
      if (L == NULL) {
        printf(" table not initialized ");
        getchar();
        getchar();
        break;
      }
      printf(" Please input insert position i(1<=i<=%d):", L -> len + 1);
      scanf("%d", & i);
      printf(" Please input insert data e:");
      scanf("%d", & cur);
      if (ListInsert(L, i, cur) == ERROR) printf(" Insert failed ");
      else printf(" insert successfully ");
      getchar();
      getchar();
      break;
    case 11:
      if (L == NULL) {
        printf(" table not initialized ");
        getchar();
        getchar();
        break;
      }
      printf(" Please input delete position i ( 1<=i<=%d ) ", L -> len);
      scanf("%d", & i);
      if (ListDelete(L, i, cur) == ERROR)
        printf(" Delete failed ");
      else
        printf(" Deleted successfully , the deleted element is %d", cur);
      getchar();
      getchar();
      break;
    case 12:
      if (ListTrabverse(L) == ERROR) printf(" Traversal failed! \n");
      getchar();
      getchar();
      break;
    case 13:
      if (LoadList(fp, List) == OK) {
        printf(" File loaded successfully ");
      }
      getchar();
      getchar();
      break;
    case 14:
      if (SaveList(fp, List) == OK) {
        printf(" File saved successfully ");
      }
      getchar();
      getchar();
      break;
    case 15:
      printf(" Please input the bit sequence vol of the table to be operated:(1<=vol<=%d)", length + 1);
      scanf("%d", & vol);
      ChooseList(vol, & L, List);
      getchar();
      break;
    case 16:
      if (ChartTraverse(List) == ERROR) printf(" Form traversal failed! ");
      getchar();
      getchar();
    case 0:
      break;
    } //end of switch
  } //end of while
  printf(" Welcome to use this system again next time! ");
  return 0;
} //end of main()

/**
 * Function name: InitiaList
 * Function parameter: the address of the linked list pointer L
 * Function function: construct an empty linked list
 * Return value: return OK if successfully constructed , otherwise return ERROR
 **/
status IntiaList(Linklist ** L) {
  /* Initialize the big table header - start */
  Linklist * p;
  p = (Linklist * ) malloc(sizeof(Linklist));
  if (!p) {
    printf(" Memory allocation failed ");
    return ERROR;
  }
  p -> fore = NULL;
  p -> head = p -> tail = NULL;
  p -> len = 0;
  /* Initialize the large table header - end */
  /* Initialize the first small table header - start */
  p -> aft = (Linklist * ) malloc(sizeof(Linklist));
  if (!p -> aft) {
    printf(" Memory allocation failed ");
    return ERROR;
  }
  p -> aft -> aft = NULL;
  p -> aft -> head = p -> aft -> tail = NULL;
  p -> aft -> fore = p;
  p -> aft -> len = 0;
  /* Initialize the first small table header - end */
  length = vol = 1;* L = p;
  return OK;
}
/**
 * Function name: DestroyList
 * Function parameters: linked list pointer L
 * Function function: delete the linked list
 * Return value: return OK if destroyed successfully , otherwise return ERROR
 **/
status DestroyList(Linklist * L) {
  Linklist * q;
  if (L == NULL) {
    printf(" table not initialized ");
    getchar();
    return ERROR;
  }
  LNode * p;
  if (L -> head != NULL) {
    p = L -> head;
    while (p -> next != NULL) { // Delete all elements except the first element
      L -> head = p -> next;
      free(p);
      p = L -> head;
    }
    free(p); // Delete the first element
    L -> head = L -> tail = NULL;
  }
  L -> fore -> aft = L -> aft;
  if (L -> aft != NULL)
    L -> aft -> fore = L -> fore;
  q = L;
  L = L -> aft;
  length--;
  free(q); // Delete the head node, connect the front and back tables
  if (vol > length) vol = length;
  printf(" Linked list %d has been deleted! ", vol + 1);
  getchar();
  getchar();
  return OK;
}
/**
 * Function name: ClearList
 * Function parameters: linked list pointer L
 * Function function: chained linear table
 * Return value: return OK if it is empty successfully , otherwise return ERROR
 **/
status ClearList(Linklist * L) {
  if (L == NULL) {
    printf(" table not initialized ");
    getchar();
    return ERROR;
  }
  LNode * p;
  if (L -> head != NULL) { // The table is not empty
    p = L -> head;
    while (p -> next != NULL) { // Delete all elements except the first element
      L -> head = p -> next;
      free(p);
      p = L -> head;
    }
    free(p); // Delete the first element
    L -> head = L -> tail = NULL;
    L -> len = 0; // reset the table length record to zero
  }
  printf(" The table has been cleared! ");
  getchar();
  return OK;
}
/**
 * Function name: ListEmpty
 * Function parameters: linked list pointer L
 * Function function: determine whether the linked list is empty
 * Return value: Return TRUE if L is an empty list , otherwise return FALSE
 **/
status ListEmpty(Linklist * L) {
  if (L == NULL) {
    printf(" table not initialized ");
    getchar();
    return ERROR;
  }
  int flag = TRUE;
  if (L -> head != NULL)
    flag = FALSE; // table length is not 0
  return flag;
}
/**
 * Function name: ListLength
 * Function parameters: linked list pointer L
 * Function function: calculate the number of data elements in the linked list L ;
 * Return value: returns the number of data elements in L
 **/
int ListLength(Linklist * L) {
  if (L == NULL) {
    printf(" table not initialized ");
    getchar();
    return ERROR;
  }
  return L -> len; // return 0 for empty table , return length for non-empty
}
/**
 * Function name: GetElem
 * Function parameters: linked list pointer L , e is the address of the i- th data ;
 * Function function: Find and display the value of the i- th element in L ;
 * Return value: return the value of the i- th element if successful , otherwise return ERROR
 **/
status GetElem(Linklist * L, int i, ElemType & e) {
  LNode * p;
  int j = 1;
  if (L -> head == NULL) {
    printf(" There is no element in the table! ");
    getchar();
    return ERROR;
  }
  if (i < 1 || i > L -> len) {
    printf(" Subscript out of bounds! ");
    getchar();
    return ERROR;
  }
  for (p = L -> head; j < i; p = p -> next, j++) {}
  e = p -> data;
  return OK;
}
/**
 * Function name: LocateElem
 * Function parameter: Linked list pointer L , element value to be compared ;
 * Function function: Find the bit sequence of the same data as e in L ; the bit sequence starts from 1
 * Return value: return the bit sequence of the first data that is the same as e if successful , return 0 if it does not exist, and return ERROR in other cases
 **/
status LocateElem(Linklist * L, ElemType e) {
  LNode * p;
  char i = 1;
  if (L == NULL) {
    printf(" table not initialized ");
    getchar();
    return ERROR;
  }
  if (L -> head == NULL) {
    printf(" There is no element in the table! ");
    getchar();
    return ERROR;
  }
  for (p = L -> head; p != NULL; p = p -> next, i++)
    if (p -> data == e) return i;
  return 0; // element not found
}
/**
 * Function name: PriorElem
 * Function parameters: linked list pointer L , searched data cur_e, precursor pre_e ;
 * Function function: Find the first data in L that is the same as cur_e , and return its predecessor
 * Return value: If successful, return the first data same as cur , and return its first predecessor; if there is no predecessor and other conditions, return ERROR
 **/
status PriorElem(Linklist * L, ElemType cur, ElemType & pre_e) {
  LNode * p, * q;
  char i = 1, j = 0;
  if (L == NULL) {
    printf(" table not initialized ");
    getchar();
    return ERROR;
  }
  if (L -> head == NULL) {
    printf(" There is no element in the table! ");
    getchar();
    return ERROR;
  }
  for (p = L -> head; p != NULL; p = p -> next, i++)
    if (p -> data == cur) {
      if (i == 1) { // the first element in the table has no predecessor
        printf(" This element has no predecessor! ");
        getchar();
        return ERROR;
      } else {
        for (q = L -> head; j < i - 2; q = q -> next, j++) {}
        pre_e = q -> data;
        return OK;
      }
    }
  printf(" This element was not found! ");
  getchar();
  return ERROR;
}
/**
 * Function name: NextElem
 * Function parameters: linked list pointer L , searched data cur, rear drive next_e ;
 * Function function: Find the first data in L that is the same as cur , and return its successor
 * Return value: If successful, return the first data that is the same as cur , and return its successor; if there is no follower and other conditions, return ERROR
 **/
status NextElem(Linklist * L, ElemType cur, ElemType & next_e) {
  LNode * p;
  char i = 1;
  if (L == NULL) {
    printf(" table not initialized ");
    getchar();
    return ERROR;
  }
  if (L -> head == NULL) {
    printf(" There is no element in the table! ");
    getchar();
    return ERROR;
  }
  for (p = L -> head; p != NULL; p = p -> next, i++)
    if (p -> data == cur) {
      if (p -> next == NULL) { // the first element in the table has no predecessor
        printf(" This element has no successor! ");
        getchar();
        return ERROR;
      } else {
        next_e = p -> next -> data;
        return OK;
      }

    }
  printf(" This element was not found! ");
  getchar();
  return ERROR;
}
/**
 * Function name: ListInsert
 * Function parameters: linked list pointer L , inserted position i , inserted data element e .
 * Function function: Insert a new data element e before the i -th position of L ; if it is full, allocate more space according to the allocation increment .
 * Return value: return OK for successful insertion , otherwise return ERROR
 **/
status ListInsert(Linklist * L, int i, ElemType e) {
  LNode * p = L -> head, * s;
  char j = 1;
  if (i < 1 || i > L -> len + 1) { // There is a place to insert after the tail element
    printf(" Subscript out of bounds, ");
    getchar();
    return ERROR;
  }
  s = (LNode * ) malloc(sizeof(LNode)); // Create a node
  s -> data = e;
  if (i == 1) { // When inserting the first element, directly let it be pointed by the head node
    s -> next = p;
    L -> head = s;
  } else { // insert element elsewhere
    for (; j < i - 1; p = p -> next, j++);
    s -> next = p -> next; // Let the new node point to the existing node at the current position
    p -> next = s; // The predecessor of the existing node points to the new node
    if (i == L -> len + 1) // The end of the table points to NULL when inserting the tail element
      s -> next = NULL;
  }
  L -> len++;
  return OK;
}
/**
 * Function name: ListDelete
 * Function parameters: linked list pointer L , deleted bit sequence i , address of pointer e .
 * Function function: delete the i- th data element of L , and return its value with e .
 * Return value: return OK if deleted successfully , otherwise return ERROR
 **/
status ListDelete(Linklist * L, int i, ElemType & e) {
  LNode * p = L -> head, * s;
  char j = 1;
  if (L -> head == NULL) {
    printf(" There is no element in the table! ");
    getchar();
    return ERROR;
  }
  if (i < 1 || i > L -> len) {
    printf(" Subscript out of bounds, ");
    getchar();
    return ERROR;
  }
  if (i == 1) { // Let the head node point to its successor when deleting the head
    L -> head = p -> next;
    e = p -> data;
    free(p);
    p = NULL;
  } else { // delete element elsewhere
    for (; j < i - 1; p = p -> next, j++);
    s = p -> next;
    p -> next = s -> next; // The predecessor of the current node points to its successor
    e = s -> data;
    free(s);
    s = NULL;
  }
  L -> len--;
  return OK;
}
/**
 * Function name: ListTrabverse
 * Function parameters: linked list pointer L
 * Function function: Display each element in the linear table in turn .
 * Return value: If successful traversal returns the length of the linear table, otherwise returns ERROR
 **/
status ListTrabverse(Linklist * L) {
  LNode * p = L -> head;
  if (L == NULL) {
    printf(" table not initialized ");
    getchar();
    return ERROR;
  }
  if (L -> head == NULL) {
    printf(" There is no element in the table, ");
    getchar();
    return ERROR;
  }
  printf("\n-----------all elements -----------------------\n");
  for (; p != NULL; p = p -> next) printf("%d ", p -> data);
  printf("\n------------------ end ------------------------\n ");
  return L -> len;
}
/**
 * Function name: LoadList
 * Function parameters: file pointer, chain form List
 * Function function: data loading, input elements for the linear table .
 * Return value: return OK if loading is successful , return ERROR if loading fails
 **/
status LoadList(FILE * fp, Linklist * List) {
  int i, yl;
  Linklist * p;
  LNode * r;
  if (List == NULL) {
    printf(" table not initialized ");
    getchar();
    return ERROR;
  }
  printf(" Please enter the file name where the data you want to restore is located :");
  scanf("%s", input);
  filename = input;
  if ((fp = fopen(filename, "r")) == NULL) {
    printf(" Failed to open file ");
    return ERROR;
  }
  fread( & yl, sizeof(int), 1, fp); // read the number of valid tables
  length = yl;
  for (i = 0, p = List; i < yl; i++, p = p -> aft) {
    p -> aft = (Linklist * ) malloc(sizeof(Linklist));
    if (!p -> aft) {
      printf(" Memory allocation failed ");
      return ERROR;
    }
    p -> aft -> fore = p;
    p -> aft -> aft = NULL;
    p -> aft -> head = p -> aft -> tail = NULL;
    fread( & (p -> aft -> len), sizeof(int), 1, fp); // read the length of each table
  }
  for (p = List -> aft; p != NULL; p = p -> aft) {
    //printf("%d\n",p->len);
    p -> head = (LNode * ) malloc(sizeof(LNode));
    p -> head -> next = NULL;
    fread( & (p -> head -> data), sizeof(ElemType), 1, fp);
    for (i = 1, r = p -> head; i < p -> len; i++, r = r -> next) {
      r -> next = (LNode * ) malloc(sizeof(LNode));
      r -> next -> next = NULL;
      fread( & (r -> next -> data), sizeof(ElemType), 1, fp);
    }
  }
  ChartTraverse(List);
  if (vol > length) vol = length;
  // Here, read the data elements from the file one by one to restore the sequence table. For different physical structures, you can restore the physical structure in the memory through the read data elements .
  fclose(fp);
  return OK;
}
/**
 * Function name: SaveList
 * Function parameters: file pointer fp , chained form pointer List
 * Function function: save the data in L to the file pointed to by the fp pointer
 * Return value: OK
 **/
status SaveList(FILE * fp, Linklist * List) {
  int yl = 0;
  Linklist * p;
  LNode * q;
  if (List == NULL) {
    printf(" table not initialized ");
    getchar();
    return ERROR;
  }
  for (p = List -> aft; p != NULL; p = p -> aft) { // count the number of tables with elements
    if (p -> len != 0) yl++;
  }
  if (yl == 0) { // returns an error if there is no valid data
    printf(" No data to save ");
    getchar();
    return ERROR;
  }
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
  if ((fp = fopen(filename, "w")) == NULL) {
    printf(" Failed to open the file ");
    getchar();
    getchar();
    return ERROR;
  }
  fwrite( & yl, sizeof(int), 1, fp); // The number of valid tables written
  for (p = List -> aft; p != NULL; p = p -> aft) { // Write the table length of each valid table
    if (p -> len != 0)
      fwrite( & (p -> len), sizeof(int), 1, fp);
  }
  for (p = List -> aft; p != NULL; p = p -> aft) { // loop to write the elements of each valid list
    if (p -> len != 0)
      for (q = p -> head; q != NULL; q = q -> next)
        fwrite( & (q -> data), sizeof(ElemType), 1, fp);
  }
  // Access data elements one by one by traversing and write to the file
  fclose(fp);
  return OK;
}
/**
 * Function name: ChooseList
 * Function parameters: the table position vol to be replaced , the chain form pointer L , the chain form pointer List
 * Function function: point L to the subscript vol-1 of the table to be operated
 * Return value: return ERROR on failure , return OK on success
 **/
status ChooseList(int vol, Linklist ** L, Linklist * List) {
  Linklist * p;
  if (List == NULL) {
    printf(" table not initialized ");
    getchar();
    return ERROR;
  }
  if (vol < 1 || vol > length + 1) {
    printf(" Subscript out of bounds, operation failed! ");
    getchar();
    return ERROR;
  }
  if (vol == length + 1) {
    for (p = List -> aft; p -> aft != NULL; p = p -> aft);
    p -> aft = (Linklist * ) malloc(sizeof(Linklist));
    if (!p -> aft) {
      printf(" Memory allocation failed ");
      return ERROR;
    }
    p -> aft -> aft = NULL;
    p -> aft -> head = p -> aft -> tail = NULL; // At this time, create a new node through the ' front ' tail node operation
    p -> aft -> fore = p;
    p = p -> aft; //L points to the tail node
    p -> len = 0;
    length++;
    vol = length;
  } else {
    char i = 1;
    for (p = List -> aft; i < vol; p = p -> aft, i++) {}
  }
  * L = p;
  return OK;
}
/**
 * Function name: ChartTraverse
 * Function parameters: chained form pointer List
 * Function function: traverse the entire form, and explain separately when there is no element in the table
 * Return value: return ERROR on failure , return OK on success
 **/
status ChartTraverse(Linklist * List) {
  char i = 0, flag = ERROR;
  Linklist * p;
  LNode * q;
  if (List == NULL) {
    printf(" table not initialized ");
    getchar();
    return ERROR;
  }
  printf("\n-----------all elements -----------------------\n");
  for (p = List -> aft; p != NULL; p = p -> aft, i++) {
    if (p -> len == 0) { // empty table
      printf(" No element in List[%d] \n", i + 1);
      continue;
    }
    for (q = p -> head; q != NULL; q = q -> next) printf("%d ", q -> data);
    flag = OK;
    putchar('\n'); // New line after each table traversal
  }
  printf("\n------------------ end ------------------------\n ");
  return flag;
}