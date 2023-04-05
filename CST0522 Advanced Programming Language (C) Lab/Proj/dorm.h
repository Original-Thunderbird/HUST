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

#define SCR_ROW 50              /*��Ļ����*/
#define SCR_COL 80             /*��Ļ����*/
/**
 *Ժϵ��Ϣ�����ṹ
 */
typedef struct school_info{
    char school_name[20], leader[12], contact_num[15];      /*Ժϵ���ƣ�Ժϵ�����ˣ���ϵ��ʽ*/
    unsigned short num_proj, num_973, num_863,q_count;     /*Ժϵ������Ŀ����ͳ���ã���973��Ŀ����863��Ŀ�������Ŷ���*/
    float sum_teacher, sum_student, proportion, budget;     /*���Ŷӽ�ʦ�������о����������о������ʦ�����ȣ���Ԥ��*/
    struct school_info *next;
    struct team_info *team_head;
}SCHOOL_NODE;

/**
 *�����Ŷ���Ϣ�����ṹ
 */
typedef struct team_info{
    char team_name[30], leader[12], school_name[20];        /*�Ŷ����ƣ��ŶӸ����ˣ�����Ժϵ����*/
    int teacher_num, stu_num;                               /*��ʦ�������о�������*/
    unsigned short proj_num, projsci_num,r_count;          /*������Ŀ����ͳ���ã���������Ȼ��ѧ������Ŀ��*/
    float proj_budget, proportion;                          /*������Ȼ��ѧ������Ŀ�ܾ��ѣ�������Ŀ���ʦ������ֵ��������Ŀ�������������ã�*/
    struct team_info *next;
    struct project_info *proj_head;
}TEAM_NODE;

/**
 *������Ŀ��Ϣ�����ṹ
 */
typedef struct project_info{
    char project_num[15], type, open_time[8], leader[12], team_name[30];    /*��Ŀ��ţ���Ŀ���࣬��ʼʱ�䣬��Ŀ�����ˣ��Ŷ�����*/
    float budget;                                                           /*��ĿԤ��*/
    struct project_info *next;
}PROJECT_NODE;

/**
 *�ŶӸ�������Ϣ�����ṹ
 */
typedef struct leader{                                      /*(׷�ӹ���ר��)*/
    char leader[12];                                        /*�ŶӸ���������*/
    unsigned short n;                                       /*��Ӧ�������Ŀ����*/
    struct leader *next;
}LEADER_NODE;

/**
 *��Ļ������Ϣ�������ṹ
 */
typedef struct layer_node {
    char LayerNo;            /**< �������ڲ���*/
    SMALL_RECT rcArea;       /**< ����������������*/
    CHAR_INFO *pContent;     /**< �������������ַ���Ԫԭ��Ϣ�洢������*/
    char *pScrAtt;           /**< �������������ַ���Ԫԭ����ֵ�洢������*/
    struct layer_node *next; /**< ָ����һ����ָ��*/
} LAYER_NODE;

/**
 *��ǩ���ṹ
 */
typedef struct labe1_bundle {
    char **ppLabel;        /**< ��ǩ�ַ��������׵�ַ*/
    COORD *pLoc;           /**< ��ǩ��λ�����׵�ַ*/
    int num;               /**< ��ǩ����*/
} LABEL_BUNDLE;

/**
 *�����ṹ
 */
typedef struct hot_area {
    SMALL_RECT *pArea;     /**< ������λ�����׵�ַ*/
    char *pSort;           /**< �������(�������ı���ѡ���)�����׵�ַ*/
    char *pTag;            /**< ������������׵�ַ*/
    int num;               /**< ��������*/
} HOT_AREA;

LAYER_NODE *gp_top_layer = NULL;               /*����������Ϣ����ͷ*/

char *gp_sys_name = "������Ŀ��Ϣ����ϵͳ";    /*ϵͳ����*/

char *ga_main_menu[] = {"�ļ�(F)",             /*ϵͳ���˵���*/
    	                "����ά��(M)",
                        "���ݲ�ѯ(Q)",
                        "����ͳ��(S)",
                        "����(H)"
                       };

char *ga_sub_menu[] = {"[S] ���ݱ���",          /*ϵͳ�Ӳ˵���*/
                       "[B] ���ݱ���",
                       "[R] ���ݻָ�",
                       "[X] �˳�    Alt+X",
                       "[S] Ժϵ��Ϣ",
                       "[T] �����Ŷ���Ϣ",
                       "[P] ������Ŀ��Ϣ",
                       "[S] Ժϵ��Ϣ",
                       "[T] �����Ŷ���Ϣ",
                       "[P] ������Ŀ��Ϣ",
                       "[I] ʦ��������",
                       "[T] ������Ŀ��-Ժϵ",
                       "[C] �Կƻ�����Ŀ��-�Ŷ�",
                       "[U] ��Ŀ/��ʦ������",
                       "[P] �������쵼��Ŀ���",
                       "[T] ��������",
                       "",
                       "[A] ����..."
                      };

int ga_sub_menu_count[] = {4, 3, 3, 5, 3};  /*�����˵������Ӳ˵��ĸ���*/
int gi_sel_menu = 1;                        /*��ѡ�е����˵����,��ʼΪ1*/
int gi_sel_sub_menu = 0;                    /*��ѡ�е��Ӳ˵����,��ʼΪ0,��ʾδѡ��*/

CHAR_INFO *gp_buff_menubar_info = NULL;     /*��Ų˵�����Ļ���ַ���Ϣ�Ļ�����*/
CHAR_INFO *gp_buff_stateBar_info = NULL;    /*���״̬����Ļ���ַ���Ϣ�Ļ�����*/

char *gp_scr_att = NULL;    /*�����Ļ���ַ���Ԫ����ֵ�Ļ�����*/
char gc_sys_state = '\0';   /*��������ϵͳ״̬���ַ�*/

HANDLE gh_std_out;          /*��׼����豸���*/
HANDLE gh_std_in;           /*��׼�����豸���*/

void InitInterface(void);                                       /*ϵͳ�����ʼ��*/
void ClearScreen(void);                                         /*����*/
void ShowMenu(void);                                            /*��ʾ�˵���*/
void PopMenu(int num);                                          /*��ʾ�����˵�*/
void PopPrompt(int num);                                        /*��ʾ��������*/
void PopUp(SMALL_RECT *, WORD, LABEL_BUNDLE *, HOT_AREA *);     /*����������Ļ��Ϣά��*/
void PopOff(void);                                              /*�رն��㵯������*/
void DrawBox(SMALL_RECT *parea);                                /*���Ʊ߿�*/
void LocSubMenu(int num, SMALL_RECT *parea);                    /*���˵������˵���λ*/
void ShowState(void);                                           /*��ʾ״̬��*/
void TagMainMenu(int num);                                      /*��Ǳ�ѡ�е����˵���*/
void TagSubMenu(int num);                                       /*��Ǳ�ѡ�е��Ӳ˵���*/
int DealConInput(HOT_AREA *phot_area, int *pihot_num);          /*����̨���봦��*/
void SetHotPoint(HOT_AREA *phot_area, int hot_num);             /*��������*/
BOOL ShowModule(char **pString, int n);                         /*������Ϣ����*/
BOOL Message(char **);                                          /*������ʾ����*/

void RunSys(void);                                              /*ϵͳ����ģ���ѡ�������*/
BOOL ExeFunction(int main_menu_num, int sub_menu_num);          /*����ģ��ĵ���*/
void CloseSys(void);                                            /*�˳�ϵͳ*/
void ClearMemory(void);                                         /*����ڴ�����*/

BOOL MaintainSchoolInfo(void);   /*ά��Ժϵ��Ϣ*/
BOOL MaintainTeamInfo(void);    /*ά�������Ŷ���Ϣ*/
BOOL MaintainProjectInfo(void); /*ά��������Ŀ��Ϣ*/
BOOL AddSchool(void);           /*���Ժϵ*/
BOOL AddTeam(void);             /*����Ŷ�*/
BOOL AddProject(void);          /*�����Ŀ*/
BOOL AltSchool(void);           /*�޸�Ժϵ*/
BOOL AltTeam(void);             /*�޸��Ŷ�*/
BOOL AltProject(void);          /*�޸���Ŀ*/
BOOL DeleteSchool(void);        /*ɾ��Ժϵ*/
BOOL DeleteTeam(void);          /*ɾ���Ŷ�*/
BOOL DeleteProject(void);       /*ɾ����Ŀ*/

BOOL SeekSchool(void);    /*����Ժϵ*/
BOOL SeekTeam(void);      /*�����Ŷ�*/
BOOL SeekProject(void);   /*������Ŀ*/
BOOL TeachStuProp(void);    /*��Ժϵ��ʦ�������о����������о������ʦ��������*/
BOOL ProjectTotal(void);    /*��Ժϵ������Ŀ����973 ��Ŀ����863 ��Ŀ���������ܾ���*/
BOOL TeamOfMostNSFC(void);  /*������Ȼ��ѧ����Ŀ�����Ŀ������10 �������Ŷ�*/
BOOL ProjTeachProp(void);   /*������Ŀ���ͽ�ʦ������ֵ��ߵ�5 �������Ŷ�*/
BOOL LeaderOfProj(void);    /*���ض�Ժϵ�ڰ������˸�����Ŀ�Ķ��ٽ������򣨸��ӣ�*/

int LoadCode(char *filename, char **ppbuffer);  /*��������*/
BOOL LoadData(void);                            /*���ݼ���*/
int CreatList();                                /*���������ʼ��*/
BOOL FileOut(void);                             /*���ݼ���*/
BOOL FileIn(void);                              /*��������*/
BOOL BackupData(void);                          /*��������*/
BOOL RestoreData(void);                         /*�ָ�����*/
BOOL Print(void);                               /*��Ļ�������*/
BOOL ExitSys(void);                             /*�˳�ϵͳ*/
BOOL HelpTopic(void);                           /*��������*/
BOOL AboutDorm(void);                           /*����ϵͳ*/

unsigned short choice;
char i, j, k, seek[30], flag, b=' ';
SCHOOL_NODE *sch_head= NULL, *p, *p1, *p2, *prior1, *prior2, *t;
TEAM_NODE *q, *q1, *q2, *qrior1, *qrior2, *head, *u;
PROJECT_NODE *r, *r1, *r2;
char *pError[]={"δ�ҵ�Ŀ����Ϣ���������˵���", "ȷ��", "ȡ��"};
char *pContinue[]={"�������˵���", "ȷ��", "ȡ��"};
#endif /**< TYPE_H_INCLUDED*/
