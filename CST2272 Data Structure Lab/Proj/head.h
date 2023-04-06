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
#define LISTINCREMENT  10

/**
 *在课时间压缩存储链表结点结构
 */
typedef struct Occupy{
    unsigned short week;
    unsigned short time;
    struct Occupy *next;
}Occupy;

/**
 *在课时间压缩存储链表头结构
 */
typedef struct ochead{
    unsigned short len;
    Occupy *head;
}Ochead;

/**
 *教师结点结构
 */
typedef struct{
    char tname[20];
    unsigned short gender, age;     //gender：female:0，male:1
    Ochead ochead[6];               //occupation of time slots. 3 dimentions: week-day in a week-timeslot
}Teacher;

/**
 *教师线性表头结构
 */
typedef struct {
    Teacher *elem;
    unsigned short length, listsize;
}TeList;

/**
 *教室结点结构
 */
typedef struct{
    char code[20];
    unsigned short rsize;
    Ochead ochead[6];
}Room;

/**
 *教室线性表头结构
 */
typedef struct {
    Room *elem;
    unsigned short length, listsize;
}RoList;

/**
 *课程结点结构
 */
typedef struct CouArc{
	char *prename;
	struct CouArc *nextarc;
}CouArc; //pre-requisite

/**
 *课程逆邻接表弧结点结构
 */
typedef struct {
    char name[60];
    unsigned short arcnum;
    CouArc *archead;            //先修课头指针
}CouHead;

/**
 *课程逆邻接表头结构
 */
typedef struct {
	CouHead *coulist;                   //逆邻接表
	unsigned short length, listsize;
}CouGraph;

/**
 *课堂结点结构
 */
typedef struct Class{
    char coursename[60], roomcode[20], tname[20];
    unsigned short cou_st, cou_ed, wk_st[5], wk_ed[5];          //开始节数,结束节数,开始周数,持续周数
    struct Class *next;
}Class;

/**
 *班级结点结构
 */
typedef struct Schedule{
    char classname[20];                         //班号
    unsigned short do_num, pr_num, clsize;      //已修/在修课程数目
    char done[60][60], progress[15][60];        //已修/在修课程
    struct weekday{
        Class *classhead;                       //课堂链表头指针
        Ochead ochead;                          //在课时间链表头结构
        unsigned short cl_num;                  //课堂数目
    }week[6];                                   //周日无课，每周只用考虑六天
    struct Schedule *next;
}Schedule;

/**
 *班级链表头结构
 */
typedef struct {
    unsigned short gr_num;      //班级数目
    Schedule *schead;           //表头指针
}Grade;

/**
 *屏幕窗口信息链结点结点结构
 */
typedef struct layer_node {
    char LayerNo;            /**< 弹出窗口层数*/
    SMALL_RECT rcArea;       /**< 弹出窗口区域坐标*/
    CHAR_INFO *pContent;     /**< 弹出窗口区域字符单元原信息存储缓冲区*/
    char *pScrAtt;           /**< 弹出窗口区域字符单元原属性值存储缓冲区*/
    struct layer_node *next; /**< 指向下一结点的指针*/
} LAYER_NODE;

/**
 *标签束结构
 */
typedef struct labe1_bundle {
    char **ppLabel;        /**< 标签字符串数组首地址*/
    COORD *pLoc;           /**< 标签定位数组首地址*/
    int num;               /**< 标签个数*/
} LABEL_BUNDLE;

/**
 *热区结构
 */
typedef struct hot_area {
    SMALL_RECT *pArea;     /**< 热区定位数组首地址*/
    char *pSort;           /**< 热区类别(按键、文本框、选项框)数组首地址*/
    char *pTag;            /**< 热区序号数组首地址*/
    int num;               /**< 热区个数*/
} HOT_AREA;

LAYER_NODE *gp_top_layer = NULL;               /*弹出窗口信息链链头*/

char *gp_sys_name = "University Course Arrangement & Scheduling System";    /*系统名称*/

char *ga_main_menu[] = {"File(F)",             /*系统主菜单名*/
    	                  "Data Maintainance(M)",
                        "Data Querying(Q)",
                        "Gte Statistics(S)",
                        "Help(H)"
                       };

char *ga_sub_menu[] = {"[S] Data Backup",          /*系统子菜单名*/
                       "[R] Data Restore",
                       "[X] Exit    Alt+X",
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

int ga_sub_menu_count[] = {3, 5, 5, 3, 3};  /*各主菜单项下子菜单的个数*/
int gi_sel_menu = 1;                        /*被选中的主菜单项号,初始为1*/
int gi_sel_sub_menu = 0;                    /*被选中的子菜单项号,初始为0,表示未选中*/

CHAR_INFO *gp_buff_menubar_info = NULL;     /*存放菜单条屏幕区字符信息的缓冲区*/
CHAR_INFO *gp_buff_stateBar_info = NULL;    /*存放状态条屏幕区字符信息的缓冲区*/

char *gp_scr_att = NULL;    /*存放屏幕上字符单元属性值的缓冲区*/
char gc_sys_state = '\0';   /*用来保存系统状态的字符*/

HANDLE gh_std_out;          /*标准输出设备句柄*/
HANDLE gh_std_in;           /*标准输入设备句柄*/

void InitInterface(void);                                       /*系统界面初始化*/
void ClearScreen(void);                                         /*清屏*/
void ShowMenu(void);                                            /*显示菜单栏*/
void PopMenu(int num);                                          /*显示下拉菜单*/
void PopPrompt(int num);                                        /*显示弹出窗口*/
void PopUp(SMALL_RECT *, WORD, LABEL_BUNDLE *, HOT_AREA *);     /*弹出窗口屏幕信息维护*/
void PopOff(void);                                              /*关闭顶层弹出窗口*/
void DrawBox(SMALL_RECT *parea);                                /*绘制边框*/
void LocSubMenu(int num, SMALL_RECT *parea);                    /*主菜单下拉菜单定位*/
void ShowState(void);                                           /*显示状态栏*/
void TagMainMenu(int num);                                      /*标记被选中的主菜单项*/
void TagSubMenu(int num);                                       /*标记被选中的子菜单项*/
int DealConInput(HOT_AREA *phot_area, int *pihot_num);          /*控制台输入处理*/
void SetHotPoint(HOT_AREA *phot_area, int hot_num);             /*设置热区*/
BOOL ShowModule(char **pString, int n);                         /*弹出信息窗口*/
BOOL Message(char **);                                          /*弹出提示窗口*/

BOOL LoadData(void);                                            /*数据加载*/
void RunSys(void);                                              /*系统功能模块的选择和运行*/
BOOL ExeFunction(int main_menu_num, int sub_menu_num);          /*功能模块的调用*/
void CloseSys(void);                                            /*退出系统*/
void ClearMemory(void);                                         /*清空内存数据*/

BOOL Initial(void);     /*数据结构初始化*/
BOOL AddTeacher(void);  /*插入教师*/
BOOL AddRoom(void);     /*插入教室*/
BOOL AddCourse(void);   /*插入课程*/
BOOL AddGroup(void);    /*插入班级*/
BOOL DelTeacher(void);  /*删除教师*/
BOOL DelRoom(void);     /*删除教室*/
BOOL DelCourse(void);   /*删除课程*/
BOOL DelGroup(void);    /*删除班级*/
BOOL AltTeacher(void);  /*修改教师*/
BOOL AltRoom(void);     /*修改教室*/
BOOL AltCourse(void);   /*修改课程*/
BOOL AltGroup(void);    /*修改班级*/
BOOL SeekTeacher(void); /*查找教师*/
BOOL SeekRoom(void);    /*查找教室*/
BOOL SeekCourse(void);  /*查找课程*/
BOOL SeekGroup(void);   /*查找班级*/

BOOL MaintainTeacherInfo(void);     /*维护教师信息*/
BOOL MaintainRoomInfo(void);        /*维护教室信息*/
BOOL MaintainGroupInfo(void);       /*维护班级信息*/
BOOL MaintainCourseInfo(void);      /*维护课程信息*/
BOOL MaintainClassInfo(void);       /*维护课堂信息*/

BOOL AddClass(void);    /*插入课堂*/
BOOL DelClass(void);    /*删除课堂*/
BOOL AltClass(void);    /*修改课堂*/
BOOL SeekClass(void);   /*查找课堂*/
BOOL RoomUsage(void);   /*教室利用情况*/
BOOL TeacherAnalysis(void);     /*教室排课分析*/
BOOL ClassAnalysis(void);       /*班级排课分析*/
BOOL FileOut(void);     /*数组备份*/
BOOL FileIn(void);      /*数组恢复*/
BOOL ExitSys(void);     /*退出系统*/
BOOL HelpTopic(void);   /*帮助主题*/
BOOL AboutDorm(void);   /*关于系统*/
BOOL DelTimeNode(unsigned short wk_st[], unsigned short wk_ed[], unsigned short cou_st, unsigned short cou_ed, Ochead *ochead); /*删除在课时间结点*/
BOOL AddTimeNode(unsigned short wk_st[], unsigned short wk_ed[], unsigned short cou_st, unsigned short cou_ed, Ochead *ochead); /*添加在课时间结点*/
Schedule *ClassInput(char cbuf[]);  /*班级名称输入*/
BOOL TimeInput(unsigned short *cou_st, unsigned short *cou_ed, unsigned short wk_st[], unsigned short wk_ed[]);     /*班级名称输入*/
BOOL AddNewRoom(char cbuf[]);       /*插入新教室*/
BOOL AddNewTeacher(char cbuf[]);    /*插入新教师*/
BOOL AddNewCourse(char cbuf[]);     /*插入新课程*/
BOOL SeekRemClass(Schedule *ps, char cbuf[]);   /*查找剩余课堂*/
BOOL DelClassNode(Class *pc, Schedule *ps, unsigned short j);   /*删除课堂结点*/
BOOL Add(void);

TeList *telist=NULL;
RoList *rolist=NULL;
CouGraph *cougraph=NULL;
Grade grade[3];

CouArc *pcarc=NULL, *qcarc=NULL;
Occupy *pocc=NULL, *qocc=NULL;
Schedule *psch=NULL, *qsch=NULL;
Class *pcls=NULL, *qcls=NULL;
unsigned long ul;
unsigned short choice, hubuffer, buf1, buf2, buf3[5], buf4[5], buf5, buf6;
unsigned char i, j, k, l, m, n, p, b=' ', count;
char cbuffer[60], cbuf1[60], cbuf2[60];
BOOL flag;
char *pError[]={"Not FOund! Back to Main Menu?", "Yes", "No"};
char *pContinue[]={"Back to Main Menu?", "Yes", "No"};
#endif /**< TYPE_H_INCLUDED*/
