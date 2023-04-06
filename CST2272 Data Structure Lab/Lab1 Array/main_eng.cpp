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
#define LIST_INIT_SIZE 100
#define LISTINCREMENT 10
typedef int status;
typedef int ElemType; // Data element type definition
typedef struct { // Definition of sequence table (sequence structure)
  ElemType * elem;
  int length;
  int listsize;
}
SqList;
// Arrays with a length of 20 determine whether the corresponding linear tables are initialized, TRUE means initialized; FALSE means uninitialized
//vol is used to locate the linear table of the current operation
int judge[20], vol = 1;
ElemType * newbase;
FILE * fp;
char * filename, * pc, appe[5] = ".dat", input[60]; // used to generate file name

status IntiaList(SqList * L);
status DestroyList(SqList * L);
status ClearList(SqList * L);
status ListEmpty(SqList * L);
int ListLength(SqList * L);
status GetElem(SqList * L, int i, ElemType & e);
status LocateElem(SqList * L, ElemType e); // simplified
status PriorElem(SqList * L, ElemType cur, ElemType & pre_e);
status NextElem(SqList * L, ElemType cur, ElemType & next_e);
status ListInsert(SqList * L, int i, ElemType e);
status ListDelete(SqList * L, int i, ElemType & e);
status ListTrabverse(SqList * L); // simplified
status LoadList(FILE * fp, SqList List[]);
status SaveList(FILE * fp, SqList List[]);
status ChooseList(int vol, SqList ** L, SqList List[]);
status ChartTraverse(SqList List[]);

int main(void) {
  SqList * L = NULL, List[20];
  int op = 1, i, e, cur, pre_e, next_e;
  L = List;
  while (op) {
    system("cls");
    printf("\n\n");
    printf(" There are 20 tables in the system , and the table %d is currently being operated \n", vol); // Prompt the order of the table currently being operated
    printf(" Menu for Linear Table On Sequence Structure \n");
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
      if (IntiaList(L) == OK)
        printf(" Linear table created successfully! ");
      else printf(" Linear table creation failed! ");
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
      if (i == TRUE)
        printf(" Linear table is empty! ");
      else if (i == FALSE)
        printf(" The linear table is not empty! ");
      getchar();
      getchar();
      break;
    case 5:
      if (ListLength(L) == ERROR) {
        getchar();
        getchar();
        break;
      }
      printf(" Table length is %d", L -> length);
      getchar();
      getchar();
      break;
    case 6:
      printf(" Please enter element sequence i(1<=i<%d):", L -> length + 1);
      scanf("%d", & i);
      if (GetElem(L, i, e) == OK)
        printf(" The %dth element is %d", i, e);
      getchar();
      getchar();
      break;
    case 7:
      printf(" Please enter the data to be checked :");
      scanf("%d", & e);
      i = LocateElem(L, e);
      if (i > 0)
        printf("%d is the %dth element in the table ", e, i);
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
      printf(" Please input insert position i(1<=i<=%d):", L -> length + 1);
      scanf("%d", & i);
      printf(" Please input insert data e:");
      scanf("%d", & e);
      if (ListInsert(L, i, e) == ERROR)
        printf(" Insert failed ");
      else
        printf(" insert successfully ");
      getchar();
      getchar();
      break;
    case 11:
      printf(" Please input delete position i ( 1<=i<=%d ) ", L -> length);
      scanf("%d", & i);
      if (ListDelete(L, i, e) == ERROR)
        printf(" Delete failed ");
      else
        printf(" Deleted successfully , the deleted element is %d", e);
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
      printf(" Please enter the bit sequence vol of the table to be operated:(1<=vol<=20)");
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
 * Function parameters: linear table pointer L or address of List[]
 * Function function: construct an empty linear table
 * Return value: return OK if successfully constructed , otherwise return ERROR
 **/
status IntiaList(SqList * L) {
  L -> elem = (ElemType * ) malloc(LIST_INIT_SIZE * sizeof(ElemType));
  if (!L -> elem) return ERROR;
  L -> length = 0;
  L -> listsize = LIST_INIT_SIZE; // Initial allocation space
  judge[vol - 1] = TRUE; // The current table has been initialized, modify the corresponding judgment variable
  return OK;
}
/**
 * Function name: DestroyList
 * Function parameters: linear table pointer L
 * Function function: delete linear table
 * Return value: return OK if destroyed successfully , otherwise return ERROR
 **/
status DestroyList(SqList * L) {
  // For each element of the judgment array that has not been operated, its value is a random number, not necessarily FALSE , so the condition should be non- TRUE , and will not be explained later
  if (judge[vol - 1] != TRUE) {
    printf("\nThis table is not initialized! ");
    getchar();
    return ERROR;
  }
  free(L -> elem);
  L -> length = L -> listsize = 0;
  // Destroy table L and all its affiliated information
  L = NULL;
  judge[vol - 1] = FALSE; // Modify the corresponding judgment variable
  printf(" Linear table %d has been deleted! ", vol);
  getchar();
  return OK;
}
/**
 * Function name: ClearList
 * Function parameters: linear table pointer L
 * Function function: reset the linear table
 * Return value: return OK if it is empty successfully , otherwise return ERROR
 **/
status ClearList(SqList * L) {
  if (judge[vol - 1] != TRUE) {
    printf(" This table is not initialized! ");
    getchar();
    return ERROR;
  }
  L -> length = 0; // Initial table length
  L -> listsize = LIST_INIT_SIZE; // Initial state space size
  printf(" Table has been cleared! ");
  getchar();
  return OK;
}
/**
 * Function name: ListEmpty
 * Function parameters: linear table pointer L
 * Function function: determine whether the linear table is empty
 * Return value: Return TRUE if L is an empty list , otherwise return FALSE
 **/
status ListEmpty(SqList * L) {
  if (judge[vol - 1] != TRUE) {
    printf(" This table is not initialized! ");
    getchar();
    return ERROR;
  }
  int flag = TRUE;
  if (L -> length != 0)
    flag = FALSE; // table length is not 0
  return flag;
}
/**
 * Function name: ListLength
 * Function parameters: linear table pointer L
 * Function function: calculate the number of data elements in the linear table L ;
 * Return value: successfully returns the number of data elements in L , fails and returns ERROR
 **/
int ListLength(SqList * L) {
  if (judge[vol - 1] != TRUE) {
    printf(" This table has not been initialized ");
    getchar();
    return ERROR;
  }
  return L -> length;
}
/**
 * Function name: GetElem
 * Function parameters: linear table pointer L , e is the address of the i- th data ;
 * Function function: Find and display the value of the i- th element in L ;
 * Return value: return the value of the i- th element if successful , otherwise return ERROR
 **/
status GetElem(SqList * L, int i, ElemType & e) {
  if (judge[vol - 1] != TRUE) {
    printf(" This table has not been initialized ");
    getchar();
    return ERROR;
  }
  if (ListEmpty(L) == TRUE) {
    printf("\nThere is no element in the table! ");
    getchar();
    return ERROR;
  }
  if (i < 1 || i > L -> length + 1) {
    printf(" Subscript out of bounds! ");
    getchar();
    return ERROR;
  }
  e = L -> elem[i - 1];
  return OK;
}
/**
 * Function name: LocateElem
 * Function parameters: linear table pointer L , the relative element value ;
 * Function function: Find the bit sequence of the same data as e in L ; the bit sequence starts from 1
 * Return value: return the bit sequence of the first data that is the same as e if successful , return 0 if it does not exist, and return ERROR in other cases
 **/
status LocateElem(SqList * L, ElemType e) {
  int i;
  if (judge[vol - 1] != TRUE) {
    printf(" This table has not been initialized ");
    getchar();
    return ERROR;
  }
  if (ListEmpty(L) == TRUE) {
    printf(" There is no element in the table! ");
    getchar();
    return ERROR;
  }

  for (i = 0; i < L -> length; i++) {
    if (L -> elem[i] == e)
      return (i + 1); // subscript + 1 is the bit sequence
  }
  return 0; // element not found
}
/**
 * Function name: PriorElem
 * Function parameters: linear table pointer L , searched data cur_e, precursor pre_e ;
 * Function function: Find the first data in L that is the same as cur_e , and return its predecessor
 * Return value: If successful, return the first data same as cur , and return its first predecessor; if there is no predecessor and other conditions, return ERROR
 **/
status PriorElem(SqList * L, ElemType cur, ElemType & pre_e) {
  int i;
  if (judge[vol - 1] != TRUE) {
    printf(" This table has not been initialized ");
    getchar();
    return ERROR;
  }
  if (ListEmpty(L) == TRUE) {
    printf(" There is no element in the table! ");
    getchar();
    return ERROR;
  }
  for (i = 0; i < L -> length; i++) {
    if (L -> elem[i] == cur) { // Most loops only do this step
      if (i == 0) { // the first element in the table has no predecessor
        printf(" This element has no predecessor! ");
        getchar();
        return ERROR;
      } else {
        pre_e = L -> elem[i - 1];
        return OK;
      }
    }
  }
  printf(" The element was not found! ");
  getchar();
  return ERROR;
}
/**
 * Function name: NextElem
 * Function parameters: linear table pointer L , search data cur, rear drive next_e ;
 * Function function: Find the first data in L that is the same as cur , and return its successor
 * Return value: If successful, return the first data that is the same as cur , and return its successor; if there is no follower and other conditions, return ERROR
 **/
status NextElem(SqList * L, ElemType cur, ElemType & next_e) {
  int i;
  if (judge[vol - 1] != TRUE) {
    printf(" This table has not been initialized! ");
    getchar();
    return ERROR;
  }
  if (ListEmpty(L) == TRUE) {
    printf(" There is no element in the table! ");
    getchar();
    return ERROR;
  }
  for (i = 0; i < L -> length; i++) {
    if (L -> elem[i] == cur) { // Most loops only do this step
      if (i != L -> length - 1) {
        next_e = L -> elem[i + 1];
        return OK;
      } else { // the last element in the table has no successor
        printf(" This element has no successor! ");
        getchar();
        return ERROR;
      }
    }
  }
  printf(" The element was not found! ");
  return ERROR;
}
/**
 * Function name: ListInsert
 * Function parameters: linear table pointer L , inserted position i , inserted data element e .
 * Function function: Insert a new data element e before the i -th position of L ; if it is full, allocate more space according to the allocation increment .
 * Return value: return OK for successful insertion , otherwise return ERROR
 **/
status ListInsert(SqList * L, int i, ElemType e) {
  ElemType * p, * q;
  if (judge[vol - 1] != TRUE) {
    printf(" This table has not been initialized, ");
    getchar();
    return ERROR;
  }
  if (i < 1 || i > L -> length + 1) {
    printf(" Subscript out of bounds, ");
    getchar();
    return ERROR;
  }
  // Allocate additional space when the number of elements overflows
  if (L -> length >= L -> listsize) {
    newbase = (ElemType * ) realloc(L -> elem, (L -> listsize + LISTINCREMENT) * sizeof(ElemType));
    if (!newbase) {
      printf(" Memory allocation failed, ");
      getchar();
      return ERROR;
    }
    L -> elem = newbase;
    L -> listsize += LISTINCREMENT;
  }
  q = & (L -> elem[i - 1]);
  for (p = & (L -> elem[L -> length - 1]); p >= q; p--) {
    *(p + 1) = * p; // The elements from the end of the table to the insertion place advance in sequence, leaving space for the element to be inserted
  }
  * q = e; // insert element
  L -> length++;
  return OK;
}
/**
 * Function name: ListDelete
 * Function parameters: linear table pointer L , deleted bit sequence i , address of pointer e .
 * Function function: delete the i- th data element of L , and return its value with e .
 * Return value: return OK if deleted successfully , otherwise return ERROR
 **/
status ListDelete(SqList * L, int i, ElemType & e) {
  ElemType * p, * q;
  if (judge[vol - 1] != TRUE) {
    printf(" This table has not been initialized, ");
    getchar();
    return ERROR;
  }
  if (ListEmpty(L) == TRUE) {
    printf(" There is no element in the table, ");
    getchar();
    return ERROR;
  }
  if (i < 1 || i > L -> length) {
    printf(" Subscript out of bounds, ");
    return ERROR;
  }
  q = & (L -> elem[i - 1]);
  e = L -> elem[i - 1];
  for (p = q; p <= & (L -> elem[L -> length - 1]); p++) {
    *(p) = * (p + 1); // The elements from the insertion point to the end of the table are reversed one by one
  }
  L -> length--;
  return OK;
}
/**
 * Function name: ListTrabverse
 * Function parameters: linear table pointer L
 * Function function: Display each element in the linear table in turn .
 * Return value: If successful traversal returns the length of the linear table, otherwise returns ERROR
 **/
status ListTrabverse(SqList * L) {
  int i;
  if (judge[vol - 1] != TRUE) {
    printf(" This table has not been initialized, ");
    getchar();
    return ERROR;
  }
  if (ListEmpty(L) == TRUE) {
    printf(" There is no element in the table, ");
    getchar();
    return ERROR;
  }
  printf("\n-----------all elements -----------------------\n");
  for (i = 0; i < L -> length; i++) printf("%d ", L -> elem[i]);
  printf("\n------------------ end ------------------------\n ");
  return L -> length;
}
/**
 * Function name: LoadList
 * Function parameters: file pointer, linear form List[20]
 * Function function: data loading, input elements for the linear table .
 * Return value: return OK if loading is successful , return ERROR if loading fails
 **/
status LoadList(FILE * fp, SqList List[20]) {
  int i, j, yl;
  printf(" Please enter the file name where the data you want to restore is located :");
  scanf("%s", input);
  filename = input;
  if ((fp = fopen(filename, "r")) == NULL) {
    printf(" Failed to open file ");
    return ERROR;
  }
  fread( & yl, sizeof(int), 1, fp); // read the number of valid tables
  for (i = 0; i < yl; i++) {
    IntiaList( & List[i]); // Only initialize yl tables ,
    fread( & (List[i].length), sizeof(int), 1, fp); // read the length of each table
    judge[i] = TRUE; // Change the initialized table judgment variable to TRUE
  }
  for (i = yl; i < 20; i++) {
    judge[i] = FALSE; // Uninitialized table judgment variables are uniformly FALSE
  }
  for (i = 0; i < yl; i++) {
    for (j = 0; j < List[i].length; j++) {
      fread( & (List[i].elem[j]), sizeof(ElemType), 1, fp);
      // Allocate additional space when the number of elements overflows
      if (List[i].length >= List[i].listsize) {
        newbase = (ElemType * ) realloc(List[i].elem, (List[i].listsize + LISTINCREMENT) * sizeof(ElemType));
        if (!newbase) {
          printf(" Memory allocation failed ");
          return ERROR;
        }
        List[i].elem = newbase;
        List[i].listsize += LISTINCREMENT;
      }
    }
  }
  // Here, read the data elements from the file one by one to restore the sequence table. For different physical structures, you can restore the physical structure in the memory through the read data elements .
  fclose(fp);
  return OK;
}
/**
 * Function name: SaveList
 * Function parameters: file pointer, linear form List[20]
 * Function function: save the data in L to the file pointed to by the fp pointer
 * Return value: OK
 **/
status SaveList(FILE * fp, SqList List[20]) {
  int i, j, yl = 0;
  for (i = 0; i < 20; i++) { // count the number of tables with elements
    if (judge[i] == TRUE && List[i].length != 0)
      yl++;
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
  for (i = 0; i < 20; i++) { // write the table length of each valid table
    if (judge[i] == TRUE && List[i].length != 0)
      fwrite( & (List[i].length), sizeof(int), 1, fp);
  }
  for (i = 0; i < 20; i++) { // loop to write each valid table element
    if (judge[i] == TRUE && List[i].length != 0)
      for (j = 0; j < List[i].length; j++)
        fwrite( & (List[i].elem[j]), sizeof(ElemType), 1, fp);
  }

  // Access data elements one by one by traversing and write to the file
  fclose(fp);
  return OK;
}
/**
 * Function name: ChooseList
 * Function parameters: table position vol to be replaced , file pointer, linear form List[20]
 * Function function: point L to the subscript vol-1 of the table to be operated
 * Return value: return ERROR on failure , return OK on success
 **/
status ChooseList(int vol, SqList ** L, SqList List[20]) {
  if (vol < 1 || vol > 20) {
    printf(" Subscript out of bounds, operation failed! ");
    getchar();
    return ERROR;
  }
  * L = & (List[vol - 1]);
  if (judge[vol - 1] == FALSE) {
    printf(" This table has not been initialized "); // Prevent misoperation
    getchar();
    getchar();
  }
  return OK;
}
/**
 * Function name: ChartTraverse
 * Function parameters: table position vol to be replaced , file pointer, linear form List[20]
 * Function function: point L to the subscript vol-1 of the table to be operated
 * Return value: return ERROR on failure , return OK on success
 **/
status ChartTraverse(SqList List[20]) {
  int i, j, flag = ERROR;
  printf("\n-----------all elements -----------------------\n");
  for (i = 0; i < 20; i++) {
    if (judge[i] != TRUE) { // uninitialized
      printf("List[%d] is not initialized \n", i);
      continue;
    }
    if (List[i].length == 0) { // empty list
      printf(" No element in List[%d] \n", i);
      continue;
    }
    for (j = 0; j < List[i].length; j++) printf("%d ", List[i].elem[j]);
    flag = OK;
    putchar('\n'); // New line after each table traversal
  }
  printf("\n------------------ end ------------------------\n ");
  return flag;
}