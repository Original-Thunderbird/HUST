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

#define SCR_ROW 50              /*屏幕行数*/
#define SCR_COL 80             /*屏幕列数*/
/**
 *院系信息链结点结构
 */
typedef struct school_info{
    char school_name[20], leader[12], contact_num[15];      /*院系名称，院系负责人，联系方式*/
    unsigned short num_proj, num_973, num_863,q_count;     /*院系下总项目数（统计用），973项目数，863项目数，总团队数*/
    float sum_teacher, sum_student, proportion, budget;     /*各团队教师总数，研究生总数，研究生与教师人数比，总预算*/
    struct school_info *next;
    struct team_info *team_head;
}SCHOOL_NODE;

/**
 *科研团队信息链结点结构
 */
typedef struct team_info{
    char team_name[30], leader[12], school_name[20];        /*团队名称，团队负责人，所属院系名称*/
    int teacher_num, stu_num;                               /*教师数量，研究生数量*/
    unsigned short proj_num, projsci_num,r_count;          /*科研项目数（统计用），国家自然科学基金项目数*/
    float proj_budget, proportion;                          /*国家自然科学基金项目总经费，科研项目与教师人数比值，科研项目数（导入数据用）*/
    struct team_info *next;
    struct project_info *proj_head;
}TEAM_NODE;

/**
 *科研项目信息链结点结构
 */
typedef struct project_info{
    char project_num[15], type, open_time[8], leader[12], team_name[30];    /*项目编号，项目种类，开始时间，项目负责人，团队名称*/
    float budget;                                                           /*项目预算*/
    struct project_info *next;
}PROJECT_NODE;

/**
 *团队负责人信息链结点结构
 */
typedef struct leader{                                      /*(追加功能专用)*/
    char leader[12];                                        /*团队负责人姓名*/
    unsigned short n;                                       /*对应负责的项目数量*/
    struct leader *next;
}LEADER_NODE;

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

char *gp_sys_name = "科研项目信息管理系统";    /*系统名称*/

char *ga_main_menu[] = {"文件(F)",             /*系统主菜单名*/
    	                "数据维护(M)",
                        "数据查询(Q)",
                        "数据统计(S)",
                        "帮助(H)"
                       };

char *ga_sub_menu[] = {"[S] 数据保存",          /*系统子菜单名*/
                       "[B] 数据备份",
                       "[R] 数据恢复",
                       "[X] 退出    Alt+X",
                       "[S] 院系信息",
                       "[T] 科研团队信息",
                       "[P] 科研项目信息",
                       "[S] 院系信息",
                       "[T] 科研团队信息",
                       "[P] 科研项目信息",
                       "[I] 师生人数比",
                       "[T] 科研项目数-院系",
                       "[C] 自科基金项目数-团队",
                       "[U] 项目/教师人数比",
                       "[P] 负责人领导项目情况",
                       "[T] 帮助主题",
                       "",
                       "[A] 关于..."
                      };

int ga_sub_menu_count[] = {4, 3, 3, 5, 3};  /*各主菜单项下子菜单的个数*/
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

void RunSys(void);                                              /*系统功能模块的选择和运行*/
BOOL ExeFunction(int main_menu_num, int sub_menu_num);          /*功能模块的调用*/
void CloseSys(void);                                            /*退出系统*/
void ClearMemory(void);                                         /*清空内存数据*/

BOOL MaintainSchoolInfo(void);   /*维护院系信息*/
BOOL MaintainTeamInfo(void);    /*维护科研团队信息*/
BOOL MaintainProjectInfo(void); /*维护科研项目信息*/
BOOL AddSchool(void);           /*添加院系*/
BOOL AddTeam(void);             /*添加团队*/
BOOL AddProject(void);          /*添加项目*/
BOOL AltSchool(void);           /*修改院系*/
BOOL AltTeam(void);             /*修改团队*/
BOOL AltProject(void);          /*修改项目*/
BOOL DeleteSchool(void);        /*删除院系*/
BOOL DeleteTeam(void);          /*删除团队*/
BOOL DeleteProject(void);       /*删除项目*/

BOOL SeekSchool(void);    /*查找院系*/
BOOL SeekTeam(void);      /*查找团队*/
BOOL SeekProject(void);   /*查找项目*/
BOOL TeachStuProp(void);    /*各院系教师总数，研究生总数及研究生与教师的人数比*/
BOOL ProjectTotal(void);    /*各院系科研项目数，973 项目数，863 项目数及科研总经费*/
BOOL TeamOfMostNSFC(void);  /*国家自然科学基金的科研项目数最多的10 个科研团队*/
BOOL ProjTeachProp(void);   /*科研项目数和教师人数比值最高的5 个科研团队*/
BOOL LeaderOfProj(void);    /*在特定院系内按负责人负责项目的多少降序排序（附加）*/

int LoadCode(char *filename, char **ppbuffer);  /*代码表加载*/
BOOL LoadData(void);                            /*数据加载*/
int CreatList();                                /*数据链表初始化*/
BOOL FileOut(void);                             /*数据加载*/
BOOL FileIn(void);                              /*保存数据*/
BOOL BackupData(void);                          /*备份数据*/
BOOL RestoreData(void);                         /*恢复数据*/
BOOL Print(void);                               /*屏幕输出报表*/
BOOL ExitSys(void);                             /*退出系统*/
BOOL HelpTopic(void);                           /*帮助主体*/
BOOL AboutDorm(void);                           /*关于系统*/

unsigned short choice;
char i, j, k, seek[30], flag, b=' ';
SCHOOL_NODE *sch_head= NULL, *p, *p1, *p2, *prior1, *prior2, *t;
TEAM_NODE *q, *q1, *q2, *qrior1, *qrior2, *head, *u;
PROJECT_NODE *r, *r1, *r2;
char *pError[]={"未找到目标信息！返回主菜单？", "确定", "取消"};
char *pContinue[]={"返回主菜单？", "确定", "取消"};
#endif /**< TYPE_H_INCLUDED*/
