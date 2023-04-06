#include <stdio.h>

#include <stdlib.h>

#include <windows.h>

#include <wincon.h>

#include <conio.h>

#include <string.h>

#include <io.h>

#include <fcntl.h>

#include <sys\stat.h>

#include <ctype.h>

#include <time.h>

#ifndef TEST_H
#define TEST_H
#endif

#ifndef TYPE_H_INCLUDED
#define TYPE_H_INCLUDED

#define SCR_ROW 60
#define SCR_COL 100
#define LIST_INIT_SIZE 100
#define LISTINCREMENT 10

/**
 * Compress and store the linked list node structure during class time
 */
typedef struct Occupy {
  unsigned short week;
  unsigned short time;
  struct Occupy * next;
}
Occupy;

/**
 * Compress and store the linked list header structure during class time
 */
typedef struct ochead {
  unsigned short len;
  Occupy * head;
}
Ochead;

/**
 * Teacher node structure
 */
typedef struct {
  char tname[20];
  unsigned short gender, age; //gender : female:0 , male:1
  Ochead ochead[6]; //occupation of time slots. 3 dimentions: week-day in a week-timeslot
}
Teacher;

/**
 * Teacher linear header structure
 */
typedef struct {
  Teacher * elem;
  unsigned short length, list size;
}
TeList;

/**
 * Classroom node structure
 */
typedef struct {
  char code[20];
  unsigned short rsize;
  Ochead ochead[6];
}
Room;

/**
 * Classroom linear header structure
 */
typedef struct {
  Room * elem;
  unsigned short length, list size;
}
RoList;

/**
 * Course node structure
 */
typedef struct CouArc {
  char * prename;
  struct CouArc * nextarc;
}
CouArc; //pre-requisite

/**
 * course inverse adjacency list arc node structure
 */
typedef struct {
  char name[60];
  unsigned short arcnum;
  CouArc * archhead; // pointer to the head of the course
}
CouHead;

/**
 * course inverse adjacency header structure
 */
typedef struct {
  CouHead * coulist; // reverse adjacency list
  unsigned short length, list size;
}
CouGraph;

/**
 * classroom node structure
 */
typedef struct Class {
  char coursename[60], roomcode[20], tname[20];
  unsigned short cou_st, cou_ed, wk_st[5], wk_ed[5]; // Start section number , end section number , start week number , continuous week number
  struct Class * next;
}
Class;

/**
 * class node structure
 */
typedef struct Schedule {
  char classname[20]; // class number
  unsigned short do_num, pr_num, clsize; // Number of completed / current courses
  char done[60][60], progress[15][60]; // Courses completed / in progress
  struct weekday {
    Class * classhead; // class list head pointer
    Ochead ochead; // head structure of class time chain list
    unsigned short cl_num; // class number
  }
  week[6]; // There is no class on Sunday, only six days a week
  struct Schedule * next;
}
Schedule;

/**
 * class list header structure
 */
typedef struct {
  unsigned short gr_num; // class number
  Schedule * schead; // Schedule head pointer
}
Grade;

/**
 * Screen window information link node structure
 */
typedef struct layer_node {
  char LayerNo; /**< number of popup window layers */
  SMALL_RECT rcArea; /**< pop-up window area coordinates */
  CHAR_INFO * pContent; /**< Pop-up window area character unit original information storage buffer */
  char * pScrAtt; /**< The original attribute value storage buffer of the pop-up window area character unit */
  struct layer_node * next; /**< pointer to the next node */
}
LAYER_NODE;

/**
 * Tag bundle structure
 */
typedef struct label1_bundle {
  char ** ppLabel; /**< the first address of the label string array */
  COORD * pLoc; /**< The first address of the label positioning array */
  int num; /**< Number of tags */
}
LABEL_BUNDLE;

/**
 * Hot zone structure
 */
typedef struct hot_area {
  SMALL_RECT * pArea; /**< The first address of hot area location array */
  char * pSort; /**< The first address of the hot area category ( button, text box, option box ) array */
  char * pTag; /**< the first address of hot zone serial number array */
  int num; /**< number of hot zones */
}
HOT_AREA;

LAYER_NODE * gp_top_layer = NULL; /* Pop-up window information chain head */

char * gp_sys_name = "University Course Arrangement & Scheduling System"; /* system name */

char * ga_main_menu[] = {
  "File(F)",
  /* system main menu name */
  "Data Maintenance(M)",
  "Data Querying(Q)",
  "Gte Statistics(S)",
  "Help(H)"
};

char * ga_sub_menu[] = {
  "[S] Data Backup",
  /* system submenu name */
  "[R] Data Restore",
  "[X] Exit Alt+X",
  "[S] Teacher Info",
  "[T] Room Info",
  "[P] Class Info",
  "[S] Course Info",
  "[U] Session Info",
  "[S] Teacher Info",
  "[T] Room Info",
  "[P] Class Info",
  "[S] Course Info",
  "[U] Session Info",
  "[G] Room Statistics",
  "[T] Teacher Statistics",
  "[C] Session Statistics",
  "[T] Help",
  "",
  "[A] About..."
};

int ga_sub_menu_count[] = {
  3,
  5,
  5,
  3,
  3
}; /* The number of submenus under each main menu item */
int gi_sel_menu = 1; /* The selected main menu item number , the initial is 1*/
int gi_sel_sub_menu = 0; /* The selected submenu item number , the initial value is 0, which means it is not selected */

CHAR_INFO * gp_buff_menubar_info = NULL; /* The buffer for storing the character information of the menu bar screen area */
CHAR_INFO * gp_buff_stateBar_info = NULL; /* The buffer for storing the character information of the status bar screen area */

char * gp_scr_att = NULL; /* buffer for storing character unit attribute values on the screen */
char gc_sys_state = '\0'; /* Character used to save system state */

HANDLE gh_std_out; /* Standard output device handle */
HANDLE gh_std_in; /* standard input device handle */

void InitInterface(void); /* system interface initialization */
void ClearScreen(void); /* clear screen */
void ShowMenu(void); /* Display the menu bar */
void PopMenu(int num); /* Display drop-down menu */
void PopPrompt(int num); /* Display the pop-up window */
void PopUp(SMALL_RECT * , WORD, LABEL_BUNDLE * , HOT_AREA * ); /* pop-up window screen information maintenance */
void PopOff(void); /* Close the top-level pop-up window */
void DrawBox(SMALL_RECT * parea); /* draw border */
void LocSubMenu(int num, SMALL_RECT * parea); /* Main menu drop-down menu location */
void ShowState(void); /* Display status bar */
void TagMainMenu(int num); /* mark the selected main menu item */
void TagSubMenu(int num); /* mark the selected submenu item */
int DealConInput(HOT_AREA * phot_area, int * pihot_num); /* Console input processing */
void SetHotPoint(HOT_AREA * phot_area, int hot_num); /* Set hot area */
BOOL ShowModule(char ** pString, int n); /* popup information window */
BOOL Message(char ** ); /* popup prompt window */

BOOL LoadData(void); /* Data loading */
void RunSys(void); /* Selection and operation of system function modules */
BOOL ExeFunction(int main_menu_num, int sub_menu_num); /* call of function module */
void CloseSys(void); /* Exit the system */
void ClearMemory(void); /* clear memory data */

BOOL Initial(void); /* data structure initialization */
BOOL AddTeacher(void); /* insert teacher */
BOOL AddRoom(void); /* insert classroom */
BOOL AddCourse(void); /* insert course */
BOOL AddGroup(void); /* insert class */
BOOL DelTeacher(void); /* delete teacher */
BOOL DelRoom(void); /* Delete room */
BOOL DelCourse(void); /* Delete course */
BOOL DelGroup(void); /* Delete class */
BOOL AltTeacher(void); /* Modify teacher */
BOOL AltRoom(void); /* Modify classroom */
BOOL AltCourse(void); /* modify course */
BOOL AltGroup(void); /* Modify class */
BOOL SeekTeacher(void); /* find teacher */
BOOL SeekRoom(void); /* find classroom */
BOOL SeekCourse(void); /* find course */
BOOL SeekGroup(void); /* find class */

BOOL MaintainTeacherInfo(void); /* Maintain teacher information */
BOOL MaintainRoomInfo(void); /* Maintain classroom information */
BOOL MaintainGroupInfo(void); /* Maintain class information */
BOOL MaintainCourseInfo(void); /* maintain course information */
BOOL MaintainClassInfo(void); /* Maintain class information */

BOOL AddClass(void); /* insert class */
BOOL DelClass(void); /* Delete class */
BOOL AltClass(void); /* modify class */
BOOL SeekClass(void); /* find class */
BOOL RoomUsage(void); /* room usage */
BOOL TeacherAnalysis(void); /* Analysis of classroom scheduling */
BOOL ClassAnalysis(void); /* class schedule analysis */
BOOL FileOut(void); /* Array backup */
BOOL FileIn(void); /* Array recovery */
BOOL ExitSys(void); /* exit the system */
BOOL HelpTopic(void); /* Help topic */
BOOL AboutDorm(void); /* About the system */
BOOL DelTimeNode(unsigned short wk_st[], unsigned short wk_ed[], unsigned short cou_st, unsigned short cou_ed, Ochead * ochead); /* delete class time node */
BOOL AddTimeNode(unsigned short wk_st[], unsigned short wk_ed[], unsigned short cou_st, unsigned short cou_ed, Ochead * ochead); /* Add class time node */
Schedule * ClassInput(char cbuf[]); /* class name input */
BOOL TimeInput(unsigned short * cou_st, unsigned short * cou_ed, unsigned short wk_st[], unsigned short wk_ed[]); /* class name input */
BOOL AddNewRoom(char cbuf[]); /* insert new room */
BOOL AddNewTeacher(char cbuf[]); /* insert new teacher */
BOOL AddNewCourse(char cbuf[]); /* insert a new course */
BOOL SeekRemClass(Schedule * ps, char cbuf[]); /* Find remaining classes */
BOOL DelClassNode(Class * pc, Schedule * ps, unsigned short j); /* delete class node */
BOOL Add(void);

TeList * telist = NULL;
RoList * rolist = NULL;
CouGraph * cougraph = NULL;
Grade grade[3];

CouArc * pcarc = NULL, * qcarc = NULL;
Occupy * pocc = NULL, * qocc = NULL;
Schedule * psch = NULL, * qsch = NULL;
Class * pcls = NULL, * qcls = NULL;
unsigned long ul;
unsigned short choice, hubuffer, buf1, buf2, buf3[5], buf4[5], buf5, buf6;
unsigned char i, j, k, l, m, n, p, b = ' ', count;
char cbuffer[60], cbuf1[60], cbuf2[60];
BOOL flag;
char * pError[] = {
  "Not FOund! Back to Main Menu?",
  "Yes",
  "No"
};
char * pContinue[] = {
  "Back to Main Menu?",
  "Yes",
  "No"
};
#endif /**< TYPE_H_INCLUDED*/