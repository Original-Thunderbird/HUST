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

#define SCR_ROW 50 /* number of screen rows */
#define SCR_COL 80 /* Screen columns */
/**
 * Department information link point structure
 */
typedef struct school_info {
  char school_name[20], leader[12], contact_num[15]; /* Department name, department head, contact information */
  unsigned short num_proj, num_973, num_863, q_count; /* The total number of projects under the department (for statistics), 973 projects, 863 projects, and the total number of teams */
  float sum_teacher, sum_student, proportion, budget; /* The total number of teachers in each team, the total number of graduate students, the ratio of graduate students to teachers, and the total budget */
  struct school_info * next;
  struct team_info * team_head;
}SCHOOL_NODE;

/**
 * Scientific research team information link point structure
 */
typedef struct team_info {
  char team_name[30], leader[12], school_name[20]; /* team name, team leader, department name */
  int teacher_num, stu_num; /* Number of teachers, number of graduate students */
  unsigned short proj_num, projsci_num, r_count; /* Number of scientific research projects (for statistics), number of projects of the National Natural Science Foundation of China */
  float proj_budget, proportion; /* National Natural Science Foundation of China total funding, the ratio of scientific research projects to the number of teachers, the number of scientific research projects (for importing data) */
  struct team_info * next;
  struct project_info * proj_head;
}TEAM_NODE;

/**
 * Scientific research project information link point structure
 */
typedef struct project_info {
  char project_num[15], type, open_time[8], leader[12], team_name[30]; /* project number, project type, start time, project leader, team name */
  float budget; /* project budget */
  struct project_info * next;
}PROJECT_NODE;

/**
 * Team leader information link point structure
 */
typedef struct leader {
  /*( exclusive for additional functions )*/
  char leader[12]; /* team leader name */
  unsigned short n; /* corresponding to the number of items in charge */
  struct leader * next;
}LEADER_NODE;

/**
 * Screen window information link node structure
 */
typedef struct layer_node {
  char LayerNo; /**< number of popup window layers */
  SMALL_RECT rcArea; /**< pop-up window area coordinates */
  CHAR_INFO * pContent; /**< Pop-up window area character unit original information storage buffer */
  char * pScrAtt; /**< The original attribute value storage buffer of the pop-up window area character unit */
  struct layer_node * next; /**< pointer to the next node */
}LAYER_NODE;

/**
 * Tag bundle structure
 */
typedef struct label1_bundle {
  char ** ppLabel; /**< the first address of the label string array */
  COORD * pLoc; /**< The first address of the label positioning array */
  int num; /**< Number of tags */
}LABEL_BUNDLE;

/**
 * Hot zone structure
 */
typedef struct hot_area {
  SMALL_RECT * pArea; /**< The first address of hot area location array */
  char * pSort; /**< The first address of the hot area category ( button, text box, option box ) array */
  char * pTag; /**< the first address of hot zone serial number array */
  int num; /**< number of hot zones */
}HOT_AREA;

LAYER_NODE * gp_top_layer = NULL; /* Pop-up window information chain head */

char * gp_sys_name = " Scientific Research Project Information Management System "; /* system name */

char * ga_main_menu[] = {
  " File (F)",
  /* system main menu name */
  " Data Maintenance (M)",
  " Data query (Q)",
  " Data statistics (S)",
  " Help (H)"
};

char * ga_sub_menu[] = {
  "[S] Data Save ",
  /* system submenu name */
  "[B] Data Backup ",
  "[R] Data Recovery ",
  "[X] Exit    Alt+X",
  "[S] Faculty Information ",
  "[T] research team information ",
  "[P] Scientific research project information ",
  "[S] Faculty Information ",
  "[T] research team information ",
  "[P] Scientific research project information ",
  "[I] Teacher-student ratio ",
  "[T] number of scientific research projects - departments ",
  "[C] Number of projects of the National Natural Science Foundation of China - Teams ",
  "[U] Project / teacher ratio ",
  "[P] Person in charge leading the project ",
  "[T] Help Topics ",
  "",
  "[A] about ..."
};

int ga_sub_menu_count[] = {
  4,
  3,
  3,
  5,
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

void RunSys(void); /* Selection and operation of system function modules */
BOOL ExeFunction(int main_menu_num, int sub_menu_num); /* call of function module */
void CloseSys(void); /* Exit the system */
void ClearMemory(void); /* clear memory data */

BOOL MaintainSchoolInfo(void); /* Maintain school information */
BOOL MaintainTeamInfo(void); /* Maintain research team information */
BOOL MaintainProjectInfo(void); /* Maintain scientific research project information */
BOOL AddSchool(void); /* add department */
BOOL AddTeam(void); /* add team */
BOOL AddProject(void); /* add project */
BOOL AltSchool(void); /* modify department */
BOOL AltTeam(void); /* modify the team */
BOOL AltProject(void); /* modify project */
BOOL DeleteSchool(void); /* delete department */
BOOL DeleteTeam(void); /* delete team */
BOOL DeleteProject(void); /* delete project */

BOOL SeekSchool(void); /* Find departments */
BOOL SeekTeam(void); /* find team */
BOOL SeekProject(void); /* find project */
BOOL TeachStuProp(void); /* The total number of teachers in each department, the total number of graduate students and the ratio of the number of graduate students to teachers */
BOOL ProjectTotal(void); /* The number of scientific research projects in each department, the number of 973 projects, the number of 863 projects and the total research funding */
BOOL TeamOfMostNSFC(void); /* The 10 scientific research teams with the largest number of scientific research projects of the National Natural Science Foundation of China */
BOOL ProjTeachProp(void); /* The 5 research teams with the highest ratio of the number of scientific research projects to the number of teachers */
BOOL LeaderOfProj(void); /* Sort in descending order according to the number of projects the person in charge is responsible for in a specific department (additional) */

int LoadCode(char * filename, char ** ppbuffer); /* code table loading */
BOOL LoadData(void); /* Data loading */
int CreateList(); /* data linked list initialization */
BOOL FileOut(void); /* data loading */
BOOL FileIn(void); /* save data */
BOOL BackupData(void); /* backup data */
BOOL RestoreData(void); /* restore data */
BOOL Print(void); /* screen output report */
BOOL ExitSys(void); /* exit the system */
BOOL HelpTopic(void); /* help topic */
BOOL AboutDorm(void); /* About the system */

unsigned short choice;
char i, j, k, seek[30], flag, b = ' ';
SCHOOL_NODE * sch_head = NULL, * p, * p1, * p2, * prior1, * prior2, * t;
TEAM_NODE * q, * q1, * q2, * qrior1, * qrior2, * head, * u;
PROJECT_NODE * r, * r1, * r2;
char * pError[] = {
  " No target information found! Return to main menu? ",
  " OK ",
  " Cancel "
};
char * pContinue[] = {
  " Back to main menu? ",
  " OK ",
  " Cancel "
};
#endif /**< TYPE_H_INCLUDED*/
