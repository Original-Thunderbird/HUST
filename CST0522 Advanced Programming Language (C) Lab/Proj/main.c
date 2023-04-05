#include "dorm.h"
unsigned long ul;
int main()
{
    COORD size = {SCR_COL, SCR_ROW};              /*���ڻ�������С*/
    gh_std_out = GetStdHandle(STD_OUTPUT_HANDLE); /* ��ȡ��׼����豸���*/
    gh_std_in = GetStdHandle(STD_INPUT_HANDLE);   /* ��ȡ��׼�����豸���*/
    SetConsoleTitle(gp_sys_name);                 /*���ô��ڱ���*/
    SetConsoleScreenBufferSize(gh_std_out, size); /*���ô��ڻ�������С80*25*/
//    FileIn();
    LoadData();                     /*���ݼ���*/
    InitInterface();                /*�����ʼ��*/
    RunSys();                       /*ϵͳ����ģ���ѡ������*/
    CloseSys();                     /*�˳�ϵͳ*/
    return 0;
}

/**< ��һ���� ���ֽ��溯�� */

/**
 * ��������: InitInterface
 * ��������: ��ʼ������.
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void InitInterface()
{
    WORD att = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
               | BACKGROUND_BLUE;  /*��ɫǰ������ɫ����*/
    SetConsoleTextAttribute(gh_std_out, att);  /*���ÿ���̨��Ļ�������ַ�����*/
    ClearScreen();  /* ����*/
    /*��������������Ϣ��ջ������ʼ�������Ļ���ڵ�����һ�㵯������*/
    gp_scr_att = (char *)calloc(SCR_COL * SCR_ROW, sizeof(char));/*��Ļ�ַ�����*/
    gp_top_layer = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));
    gp_top_layer->LayerNo = 0;      /*�������ڵĲ��Ϊ0*/
    gp_top_layer->rcArea.Left = 0;  /*�������ڵ�����Ϊ������Ļ����*/
    gp_top_layer->rcArea.Top = 0;
    gp_top_layer->rcArea.Right = SCR_COL - 1;
    gp_top_layer->rcArea.Bottom = SCR_ROW - 1;
    gp_top_layer->pContent = NULL;
    gp_top_layer->pScrAtt = gp_scr_att;
    gp_top_layer->next = NULL;
    ShowMenu();     /*��ʾ�˵���*/
    ShowState();    /*��ʾ״̬��*/
    return;
}

/**
 * ��������: ClearScreen
 * ��������: �����Ļ��Ϣ.
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void ClearScreen(void)
{
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    COORD home = {0, 1};    /*����մӵ�һ�п�ʼ����Ϣ���Ա㱻�������ܺ�������*/
    unsigned long size;
    GetConsoleScreenBufferInfo( gh_std_out, &bInfo );/*ȡ��Ļ��������Ϣ*/
    size =  bInfo.dwSize.X * (bInfo.dwSize.Y); /*������Ļ�������ַ���Ԫ��*/
    /*����Ļ���������е�Ԫ���ַ���������Ϊ��ǰ��Ļ�������ַ�����*/
    FillConsoleOutputAttribute(gh_std_out, bInfo.wAttributes, size, home, &ul);
    /*����Ļ���������е�Ԫ���Ϊ�ո��ַ�*/
    FillConsoleOutputCharacter(gh_std_out, ' ', size, home, &ul);
    SetConsoleCursorPosition(gh_std_out, home);     /*������ڴ������Ͻ�*/
    return;
}

/**
 * ��������: ShowMenu
 * ��������: ����Ļ����ʾ���˵�, ����������, �����˵���һ������ѡ�б��.
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void ShowMenu()
{
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    CONSOLE_CURSOR_INFO lpCur;
    COORD size, pos = {0, 0};
    int PosA = 2, PosB, i, j;
    char ch;
    GetConsoleScreenBufferInfo( gh_std_out, &bInfo );
    size.X = bInfo.dwSize.X;    size.Y = 1;
    SetConsoleCursorPosition(gh_std_out, pos);
    for (i=0; i < 5; i++) /*�ڴ��ڵ�һ�е�һ�д�������˵���*/
    {
        printf("  %s  ", ga_main_menu[i]);
    }
    GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
    SetConsoleCursorInfo(gh_std_out, &lpCur);  /*���ع��*/
    /*���붯̬�洢����Ϊ��Ų˵�����Ļ���ַ���Ϣ�Ļ�����*/
    gp_buff_menubar_info = (CHAR_INFO *)malloc(size.X * size.Y * sizeof(CHAR_INFO));
    SMALL_RECT rcMenu ={0, 0, size.X-1, 0} ;
    /*�����ڵ�һ�е����ݶ��뵽��Ų˵�����Ļ���ַ���Ϣ�Ļ�������*/
    ReadConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
    /*����һ����Ӣ����ĸ��Ϊ��ɫ�������ַ���Ԫ��Ϊ�׵׺���*/
    for (i=0; i<size.X; i++)
    {
        (gp_buff_menubar_info+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN
                                               | BACKGROUND_RED;
        ch = (char)((gp_buff_menubar_info+i)->Char.AsciiChar);
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
        {
            (gp_buff_menubar_info+i)->Attributes |= FOREGROUND_RED;
        }
    }
    /*�޸ĺ�Ĳ˵����ַ���Ϣ��д�����ڵĵ�һ��*/
    WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
    COORD endPos = {0, 1};
    SetConsoleCursorPosition(gh_std_out, endPos);  /*�����λ�������ڵ�2�е�1��*/
    /*���˵�����Ϊ�������������Ϊ�˵���ţ���������Ϊ0(��ť��)*/
    i = 0;
    do
    {
        PosB = PosA + strlen(ga_main_menu[i]);  /*��λ��i+1�Ų˵������ֹλ��*/
        for (j=PosA; j<PosB; j++)
        {
            gp_scr_att[j] |= (i+1) << 2; /*���ò˵��������ַ���Ԫ������ֵ*/
        }
        PosA = PosB + 4;    i++;
    } while (i<5);
    TagMainMenu(gi_sel_menu);  /*��ѡ�����˵���������ǣ�gi_sel_menu��ֵΪ1*/
    return;
}

/**
 * ��������: ShowState
 * ��������: ��ʾ״̬��.
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��: ״̬���ַ�����Ϊ�׵׺���, ��ʼ״̬��״̬��Ϣ.
 */
void ShowState()
{
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    COORD size, pos = {0, 0};
    int i;
    GetConsoleScreenBufferInfo( gh_std_out, &bInfo );
    size.X = bInfo.dwSize.X;    size.Y = 1;
    SMALL_RECT rcMenu ={0, bInfo.dwSize.Y-1, size.X-1, bInfo.dwSize.Y-1};
    if (gp_buff_stateBar_info == NULL)
    {
        gp_buff_stateBar_info = (CHAR_INFO *)malloc(size.X * size.Y * sizeof(CHAR_INFO));
        ReadConsoleOutput(gh_std_out, gp_buff_stateBar_info, size, pos, &rcMenu);
    }
    for (i=0; i<size.X; i++)
    {
        (gp_buff_stateBar_info+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN
                                                | BACKGROUND_RED;
/*
        ch = (char)((gp_buff_stateBar_info+i)->Char.AsciiChar);
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
        {
            (gp_buff_stateBar_info+i)->Attributes |= FOREGROUND_RED;
        }
*/
    }
    WriteConsoleOutput(gh_std_out, gp_buff_stateBar_info, size, pos, &rcMenu);
    return;
}

/**
 * ��������: TagMainMenu
 * ��������: ��ָ�����˵�������ѡ�б�־.
 * �������: num ѡ�е����˵����
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void TagMainMenu(int num)
{
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    COORD size, pos = {0, 0};
    int PosA = 2, PosB, i;
    char ch;
    if (num == 0) /*numΪ0ʱ������ȥ�����˵���ѡ�б��*/
    {   PosA = PosB = 0;    }
    else  /*���򣬶�λѡ�����˵������ֹλ��: PosAΪ��ʼλ��, PosBΪ��ֹλ��*/
    {
        for (i=1; i<num; i++)
        {
            PosA += strlen(ga_main_menu[i-1]) + 4;
        }
        PosB = PosA + strlen(ga_main_menu[num-1]);
    }
    GetConsoleScreenBufferInfo( gh_std_out, &bInfo );
    size.X = bInfo.dwSize.X;    size.Y = 1;
    /*ȥ��ѡ�в˵���ǰ��Ĳ˵���ѡ�б��*/
    for (i=0; i<PosA; i++)
    {
        (gp_buff_menubar_info+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN
                                               | BACKGROUND_RED;
        ch = (gp_buff_menubar_info+i)->Char.AsciiChar;
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
        {
            (gp_buff_menubar_info+i)->Attributes |= FOREGROUND_RED;
        }
    }
    /*��ѡ�в˵���������ǣ��ڵװ���*/
    for (i=PosA; i<PosB; i++)
    {
        (gp_buff_menubar_info+i)->Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN
                                               | FOREGROUND_RED;
    }
    /*ȥ��ѡ�в˵������Ĳ˵���ѡ�б��*/
    for (i=PosB; i<bInfo.dwSize.X; i++)
    {
        (gp_buff_menubar_info+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN
                                               | BACKGROUND_RED;
        ch = (char)((gp_buff_menubar_info+i)->Char.AsciiChar);
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
        {
            (gp_buff_menubar_info+i)->Attributes |= FOREGROUND_RED;
        }
    }
    /*�����ñ�ǵĲ˵�����Ϣд�����ڵ�һ��*/
    SMALL_RECT rcMenu ={0, 0, size.X-1, 0};
    WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
    return;
}

/**
 * ��������: PopMenu
 * ��������: ���Ӳ˵�����������, �����˶���ѡ�б��.
 * �������: num �Ӳ˵��������µ�����
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void PopMenu(int num)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh;
    int i, j, loc = 0;
    if (num != gi_sel_menu)       /*���ָ�����˵�������ѡ�в˵�*/
    {
        if (gp_top_layer->LayerNo != 0) /*�����ǰ�����Ӳ˵�����*/
        {
            PopOff();       gi_sel_sub_menu = 0;
        }
    }
    else if (gp_top_layer->LayerNo != 0) /*���ѵ������Ӳ˵����򷵻�*/
    {   return; }
    gi_sel_menu = num;    /*��ѡ�����˵�����Ϊָ�������˵���*/
    TagMainMenu(gi_sel_menu); /*��ѡ�е����˵����������*/
    LocSubMenu(gi_sel_menu, &rcPop); /*���㵯���Ӳ˵�������λ��, �����rcPop��*/
    /*������Ӳ˵��еĵ�һ�����Ӳ˵��ַ��������е�λ��(�±�)*/
    for (i=1; i<gi_sel_menu; i++)
    {
        loc += ga_sub_menu_count[i-1];
    }
    /*�������Ӳ˵������������ǩ���ṹ����*/
    labels.ppLabel = ga_sub_menu + loc;   /*��ǩ����һ����ǩ�ַ����ĵ�ַ*/
    labels.num = ga_sub_menu_count[gi_sel_menu-1]; /*��ǩ���б�ǩ�ַ����ĸ���*/
    COORD aLoc[labels.num];/*����һ���������飬���ÿ����ǩ�ַ������λ�õ�����*/
    for (i=0; i<labels.num; i++) /*ȷ����ǩ�ַ��������λ�ã����������������*/
    {
        aLoc[i].X = rcPop.Left + 2;     aLoc[i].Y = rcPop.Top + i + 1;
    }
    labels.pLoc = aLoc; /*ʹ��ǩ���ṹ����labels�ĳ�ԱpLocָ�������������Ԫ��*/
    /*����������Ϣ*/
    areas.num = labels.num;       /*�����ĸ��������ڱ�ǩ�ĸ��������Ӳ˵�������*/
    SMALL_RECT aArea[areas.num];                    /*������������������λ��*/
    char aSort[areas.num];                      /*��������������������Ӧ���*/
    char aTag[areas.num];                         /*����������ÿ�������ı��*/
    for (i=0; i<areas.num; i++)
    {
        aArea[i].Left = rcPop.Left + 2;  /*������λ*/
        aArea[i].Top = rcPop.Top + i + 1;
        aArea[i].Right = rcPop.Right - 2;
        aArea[i].Bottom = aArea[i].Top;
        aSort[i] = 0;       /*�������Ϊ0(��ť��)*/
        aTag[i] = i + 1;           /*������˳����*/
    }
    areas.pArea = aArea;/*ʹ�����ṹ����areas�ĳ�ԱpAreaָ������λ��������Ԫ��*/
    areas.pSort = aSort;/*ʹ�����ṹ����areas�ĳ�ԱpSortָ���������������Ԫ��*/
    areas.pTag = aTag;   /*ʹ�����ṹ����areas�ĳ�ԱpTagָ���������������Ԫ��*/
    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    PopUp(&rcPop, att, &labels, &areas);
    DrawBox(&rcPop);  /*���������ڻ��߿�*/
    pos.X = rcPop.Left + 2;
    for (pos.Y=rcPop.Top+1; pos.Y<rcPop.Bottom; pos.Y++)
    { /*��ѭ�������ڿմ��Ӳ���λ�û����γɷָ�����ȡ���˲˵������������*/
        pCh = ga_sub_menu[loc+pos.Y-rcPop.Top-1];
        if (strlen(pCh)==0) /*����Ϊ0������Ϊ�մ�*/
        {   /*���Ȼ�����*/
            FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-3, pos, &ul);
            for (j=rcPop.Left+2; j<rcPop.Right-1; j++)
            {   /*ȡ���������ַ���Ԫ����������*/
                gp_scr_att[pos.Y*SCR_COL+j] &= 3; /*��λ��Ľ�������˵���λ*/
            }
        }
    }
    /*���Ӳ˵���Ĺ��ܼ���Ϊ�׵׺���*/
    pos.X = rcPop.Left + 3;
    att =  FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
    for (pos.Y=rcPop.Top+1; pos.Y<rcPop.Bottom; pos.Y++)
    {
        if (strlen(ga_sub_menu[loc+pos.Y-rcPop.Top-1])==0)
        {   continue;  /*�����մ�*/     }
        FillConsoleOutputAttribute(gh_std_out, att, 1, pos, &ul);
    }
    return;
}

/**
 * ��������: PopUp
 * ��������: ��ָ�������������������Ϣ, ͬʱ��������, ����������λ����Ϣ��ջ.
 * �������: pRc ��������λ�����ݴ�ŵĵ�ַ
 *           att �������������ַ�����
 *           pLabel ���������б�ǩ����Ϣ��ŵĵ�ַ
             pHotArea ����������������Ϣ��ŵĵ�ַ
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void PopUp(SMALL_RECT *pRc, WORD att, LABEL_BUNDLE *pLabel, HOT_AREA *pHotArea)
{
    LAYER_NODE *nextLayer;
    COORD size, pos = {0, 0};
    char *pCh;
    int i, j, row;
    /*������������λ���ַ���Ԫ��Ϣ��ջ*/
    size.X = pRc->Right - pRc->Left + 1;    /*�������ڵĿ��*/
    size.Y = pRc->Bottom - pRc->Top + 1;    /*�������ڵĸ߶�*/
    /*�����ŵ������������Ϣ�Ķ�̬�洢��*/
    nextLayer = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));
    nextLayer->next = gp_top_layer;
    nextLayer->LayerNo = gp_top_layer->LayerNo + 1;
    nextLayer->rcArea = *pRc;
    nextLayer->pContent = (CHAR_INFO *)malloc(size.X*size.Y*sizeof(CHAR_INFO));
    nextLayer->pScrAtt = (char *)malloc(size.X*size.Y*sizeof(char));
    pCh = nextLayer->pScrAtt;
    /*���������ڸ���������ַ���Ϣ���棬�����ڹرյ�������ʱ�ָ�ԭ��*/
    ReadConsoleOutput(gh_std_out, nextLayer->pContent, size, pos, pRc);
    for (i=pRc->Top; i<=pRc->Bottom; i++)
    {   /*�˶���ѭ�����������ַ���Ԫ��ԭ������ֵ���붯̬�洢���������Ժ�ָ�*/
        for (j=pRc->Left; j<=pRc->Right; j++)
        {
            *pCh = gp_scr_att[i*SCR_COL+j];     pCh++;
        }
    }
    gp_top_layer = nextLayer;  /*��ɵ������������Ϣ��ջ����*/
    /*���õ������������ַ���������*/
    pos.X = pRc->Left;
    pos.Y = pRc->Top;
    for (i=pRc->Top; i<=pRc->Bottom; i++)
    {
        FillConsoleOutputAttribute(gh_std_out, att, size.X, pos, &ul);
        pos.Y++;
    }
    /*����ǩ���еı�ǩ�ַ������趨��λ�����*/
    for (i=0; i<pLabel->num; i++)
    {
        pCh = pLabel->ppLabel[i];
        if (strlen(pCh) != 0)
        {
            WriteConsoleOutputCharacter(gh_std_out, pCh, strlen(pCh),
                                        pLabel->pLoc[i], &ul);
        }
    }
    /*���õ������������ַ���Ԫ��������*/
    for (i=pRc->Top; i<=pRc->Bottom; i++)
    {   /*�˶���ѭ�������ַ���Ԫ�Ĳ��*/
        for (j=pRc->Left; j<=pRc->Right; j++)
        {
            gp_scr_att[i*SCR_COL+j] = gp_top_layer->LayerNo;
        }
    }

    for (i=0; i<pHotArea->num; i++)
    {   /*�˶���ѭ�����������������ַ���Ԫ���������ͺ��������*/
        row = pHotArea->pArea[i].Top;
        for (j=pHotArea->pArea[i].Left; j<=pHotArea->pArea[i].Right; j++)
        {
            gp_scr_att[row*SCR_COL+j] |= (pHotArea->pSort[i] << 6)
                                    | (pHotArea->pTag[i] << 2);
        }
    }
    return;
}

/**
 * ��������: PopOff
 * ��������: �رն��㵯������, �ָ���������ԭ��ۺ��ַ���Ԫԭ����.
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void PopOff(void)
{
    LAYER_NODE *nextLayer;
    COORD size, pos = {0, 0};
    char *pCh;
    int i, j;
    if ((gp_top_layer->next==NULL) || (gp_top_layer->pContent==NULL))
    {   /*ջ�״�ŵ���������Ļ��Ϣ�����ùر�*/
        return;
    }
    nextLayer = gp_top_layer->next;
    /*�ָ�������������ԭ���*/
    size.X = gp_top_layer->rcArea.Right - gp_top_layer->rcArea.Left + 1;
    size.Y = gp_top_layer->rcArea.Bottom - gp_top_layer->rcArea.Top + 1;
    WriteConsoleOutput(gh_std_out, gp_top_layer->pContent, size, pos, &(gp_top_layer->rcArea));
    /*�ָ��ַ���Ԫԭ����*/
    pCh = gp_top_layer->pScrAtt;
    for (i=gp_top_layer->rcArea.Top; i<=gp_top_layer->rcArea.Bottom; i++)
    {
        for (j=gp_top_layer->rcArea.Left; j<=gp_top_layer->rcArea.Right; j++)
        {
            gp_scr_att[i*SCR_COL+j] = *pCh;     pCh++;
        }
    }
    free(gp_top_layer->pContent);    /*�ͷŶ�̬�洢��*/
    free(gp_top_layer->pScrAtt);
    free(gp_top_layer);
    gp_top_layer = nextLayer;
    gi_sel_sub_menu = 0;
    return;
}

/**
 * ��������: DrawBox
 * ��������: ��ָ�����򻭱߿�.
 * �������: pRc �������λ����Ϣ�ĵ�ַ
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void DrawBox(SMALL_RECT *pRc)
{
    char chBox[] = {'+','-','|'};  /*�����õ��ַ�*/
    COORD pos = {pRc->Left, pRc->Top};  /*��λ����������Ͻ�*/

    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*���߿����Ͻ�*/
    for (pos.X = pRc->Left + 1; pos.X < pRc->Right; pos.X++)
    {   /*��ѭ�����ϱ߿����*/
        WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &ul);
    }
    pos.X = pRc->Right;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*���߿����Ͻ�*/
    for (pos.Y = pRc->Top+1; pos.Y < pRc->Bottom; pos.Y++)
    {   /*��ѭ�����߿�����ߺ��ұ���*/
        pos.X = pRc->Left;
        WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &ul);
        pos.X = pRc->Right;
        WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &ul);
    }
    pos.X = pRc->Left;      pos.Y = pRc->Bottom;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*���߿����½�*/
    for (pos.X = pRc->Left + 1; pos.X < pRc->Right; pos.X++)
    {   /*���±߿����*/
        WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &ul);
    }
    pos.X = pRc->Right;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*���߿����½�*/
    return;
}

/**
 * ��������: TagSubMenu
 * ��������: ��ָ���Ӳ˵�������ѡ�б��.
 * �������: num ѡ�е��Ӳ˵����
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void TagSubMenu(int num)
{
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    int width;
    LocSubMenu(gi_sel_menu, &rcPop);  /*���㵯���Ӳ˵�������λ��, �����rcPop��*/
    if ((num<1) || (num == gi_sel_sub_menu) || (num>rcPop.Bottom-rcPop.Top-1))
    {   /*����Ӳ˵����Խ�磬������Ӳ˵��ѱ�ѡ�У��򷵻�*/
        return;
    }
    pos.X = rcPop.Left + 2;
    width = rcPop.Right - rcPop.Left - 3;
    if (gi_sel_sub_menu != 0) /*����ȡ��ԭѡ���Ӳ˵����ϵı��*/
    {
        pos.Y = rcPop.Top + gi_sel_sub_menu;
        att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
        FillConsoleOutputAttribute(gh_std_out, att, width, pos, &ul);
        pos.X += 1;
        att |=  FOREGROUND_RED;/*�׵׺���*/
        FillConsoleOutputAttribute(gh_std_out, att, 1, pos, &ul);
    }
    /*���ƶ��Ӳ˵�������ѡ�б��*/
    pos.X = rcPop.Left + 2;     pos.Y = rcPop.Top + num;
    att = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  /*�ڵװ���*/
    FillConsoleOutputAttribute(gh_std_out, att, width, pos, &ul);
    gi_sel_sub_menu = num;  /*�޸�ѡ���Ӳ˵����*/
    return;
}

/**
 * ��������: LocSubMenu
 * ��������: ���㵯���Ӳ˵��������ϽǺ����½ǵ�λ��.
 * �������: num ѡ�е����˵����
 * �������: rc �������λ����Ϣ�ĵ�ַ
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void LocSubMenu(int num, SMALL_RECT *rc)
{
    int i, len, loc = 0;
    rc->Top = 1; /*������ϱ߶��ڵ�2�У��к�Ϊ1*/
    rc->Left = 1;
    for (i=1; i<num; i++)
    {   /*����������߽�λ��, ͬʱ�����һ���Ӳ˵������Ӳ˵��ַ��������е�λ��*/
        rc->Left += strlen(ga_main_menu[i-1]) + 4;
        loc += ga_sub_menu_count[i-1];
    }
    rc->Right = strlen(ga_sub_menu[loc]);/*��ʱ��ŵ�һ���Ӳ˵����ַ�������*/
    for (i=1; i<ga_sub_menu_count[num-1]; i++)
    {   /*������Ӳ˵��ַ��������䳤�ȴ����rc->Right*/
        len = strlen(ga_sub_menu[loc+i]);
        if (rc->Right < len)
        {   rc->Right = len;    }
    }
    rc->Right += rc->Left + 3;  /*����������ұ߽�*/
    rc->Bottom = rc->Top + ga_sub_menu_count[num-1] + 1;/*���������±ߵ��к�*/
    if (rc->Right >= SCR_COL)  /*�ұ߽�Խ��Ĵ���*/
    {
        len = rc->Right - SCR_COL + 1;
        rc->Left -= len;
        rc->Right = SCR_COL - 1;
    }
    return;
}

/**
 * ��������: DealInput
 * ��������: �ڵ�������������������, �ȴ�����Ӧ�û�����.
 * �������: pHotArea   ��������ָ��
 *           piHot ����������ŵĴ�ŵ�ַ, ��ָ�򽹵�������ŵ�ָ��
 * �������: pHotArea   ��������
 *            piHot ����굥�������س���ո�ʱ���ص�ǰ�������
 * �� �� ֵ: iRet �س��������
 *
 * ����˵��:
 */
int DealInput(HOT_AREA *pHotArea, int *piHot)
{
    INPUT_RECORD inRec;
    DWORD res;
    COORD pos = {0, 0};
    int num, arrow, iRet = 0;
    int cNo, cTag, cSort;/*cNo:���, cTag:�������, cSort: ��������*/
    char vkc, asc;       /*vkc:���������, asc:�ַ���ASCII��ֵ*/
    SetHotPoint(pHotArea, *piHot);
    while (TRUE)
    {    /*ѭ��*/
        ReadConsoleInput(gh_std_in, &inRec, 1, &res);
        if ((inRec.EventType == MOUSE_EVENT) &&
            (inRec.Event.MouseEvent.dwButtonState
             == FROM_LEFT_1ST_BUTTON_PRESSED))
        {
            pos = inRec.Event.MouseEvent.dwMousePosition;
            cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3;
            cTag = (gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2) & 15;
            cSort = (gp_scr_att[pos.Y * SCR_COL + pos.X] >> 6) & 3;
            if ((cNo == gp_top_layer->LayerNo) && cTag > 0)
            {
                *piHot = cTag;
                SetHotPoint(pHotArea, *piHot);
                if (cSort == 0)
                {   iRet = 13;      break;  }
            }
        }
        else if (inRec.EventType == KEY_EVENT && inRec.Event.KeyEvent.bKeyDown)
        {
            vkc = inRec.Event.KeyEvent.wVirtualKeyCode;
            asc = inRec.Event.KeyEvent.uChar.AsciiChar;;
            if (asc == 0)
            {
                arrow = 0;
                switch (vkc)
                {  /*�����(���ϡ��ҡ���)�Ĵ���*/
                    case 37: arrow = 1; break;
                    case 38: arrow = 2; break;
                    case 39: arrow = 3; break;
                    case 40: arrow = 4; break;
                }
                if (arrow > 0)
                {
                    num = *piHot;
                    while (TRUE)
                    {
                        if (arrow < 3)  {   num--;  }
                        else            {   num++;  }
                        if ((num < 1) || (num > pHotArea->num) ||
                            ((arrow % 2) && (pHotArea->pArea[num-1].Top
                            == pHotArea->pArea[*piHot-1].Top)) || ((!(arrow % 2))
                            && (pHotArea->pArea[num-1].Top
                            != pHotArea->pArea[*piHot-1].Top)))
                        {   break;  }
                    }
                    if (num > 0 && num <= pHotArea->num)
                    {
                        *piHot = num;
                        SetHotPoint(pHotArea, *piHot);
                    }
                }
            }
            else if (vkc == 27)
            {  /*ESC��*/     iRet = 27;      break;  }
            else if (vkc == 13 || vkc == 32)
            {  /*�س�����ո��ʾ���µ�ǰ��ť*/   iRet = 13;      break;  }
        }
    }
    return iRet;
}

/**
 * ��������: SetHotPoint
 * ��������: �ڵ�������������������, �ȴ�����Ӧ�û�����.
 * �������: pHotArea   ��������ָ��
 *            iHot �����������
 * �������: pHotArea   ��������
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void SetHotPoint(HOT_AREA *pHotArea, int iHot)
{
    CONSOLE_CURSOR_INFO lpCur;
    COORD pos = {0, 0};
    WORD att1, att2;
    int i, width;
    att1 = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  /*�ڵװ���*/
    att2 = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    for (i=0; i<pHotArea->num; i++)
    {  /*����ť��������Ϊ�׵׺���*/
        pos.X = pHotArea->pArea[i].Left;
        pos.Y = pHotArea->pArea[i].Top;
        width = pHotArea->pArea[i].Right - pHotArea->pArea[i].Left + 1;
        if (pHotArea->pSort[i] == 0)
        {  /*�����ǰ�ť��*/
            FillConsoleOutputAttribute(gh_std_out, att2, width, pos, &ul);
        }
    }
    pos.X = pHotArea->pArea[iHot-1].Left;
    pos.Y = pHotArea->pArea[iHot-1].Top;
    width = pHotArea->pArea[iHot-1].Right - pHotArea->pArea[iHot-1].Left + 1;
    if (pHotArea->pSort[iHot-1] == 0)
    {  /*�����������ǰ�ť��*/
        FillConsoleOutputAttribute(gh_std_out, att1, width, pos, &ul);
    }
    else if (pHotArea->pSort[iHot-1] == 1)
    {  /*�������������ı�����*/
        SetConsoleCursorPosition(gh_std_out, pos);
        GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = TRUE;
        SetConsoleCursorInfo(gh_std_out, &lpCur);
    }
}

/**
 * ��������: ShowModule
 * ��������: ��ָ�������ļ��лָ�ϵͳ�����������������.
 * �������: pString ������Ϣ��ά�����׵�ַ
 *           n �ַ�������
 * �������:
 * �� �� ֵ: BOOL����, ����ΪTRUE
 *
 * ����˵��:
 */
BOOL ShowModule(char **pString, int n)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    int i, maxlen, str_len, iHot = 1;
    for (i=0,maxlen=0; i<n; i++) {
        str_len = strlen(pString[i]);
        if (maxlen < str_len) {
            maxlen = str_len;
        }
    }
    pos.X = maxlen + 16;         pos.Y = n + 15;
    rcPop.Left = (SCR_COL - pos.X) / 2 - 10;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2 - 12;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;
    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = n;
    labels.ppLabel = pString;
    COORD aLoc[n];
    for (i=0; i<n; i++) {
        aLoc[i].X = rcPop.Left + 3;
        aLoc[i].Y = rcPop.Top + (pos.Y-n)/2 + i;
    }
    str_len = strlen(pString[n-1]);
    aLoc[n-1].X = rcPop.Left + 3 + (maxlen-str_len)/2;
    aLoc[n-1].Y = aLoc[n-1].Y + 2;
    labels.pLoc = aLoc;
    areas.num = 1;
    SMALL_RECT aArea[] = {{aLoc[n-1].X, aLoc[n-1].Y,
                           aLoc[n-1].X + 3, aLoc[n-1].Y}};
    char aSort[] = {0}, aTag[] = {1};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    pos.X = rcPop.Left + 1;     pos.Y = rcPop.Top + (pos.Y-n)/2 + n;
    FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-1, pos, &ul);
    DealInput(&areas, &iHot);
    PopOff();
    return bRet;
}

/**
 * ��������: Message
 * ��������: �����Ի���
 * �������: **pCh �Ի�����Ϣ�����׵�ַ
 * �������: ��
 * �� �� ֵ: BOOL���ͣ��˳�ΪTRUE��ȡ��ΪFALSE
 *
 * ����˵��:
 */
BOOL Message(char **pCh)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    int iHot = 1;
    char len[3] = { strlen(pCh[0]), strlen(pCh[1]), strlen(pCh[2])};
    char lmax = (len[0] > len[1]+len[2]+4) ? len[0] : len[1]+len[2]+4;
    pos.X = lmax + 6;
    pos.Y = 7;
    char blanc = (pos.X - (len[1]+len[2]+4)) / 2;
    rcPop.Left = (SCR_COL - pos.X) / 2 - 10;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2 - 12;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;
    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = 3;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+3, rcPop.Top+2},
                    {rcPop.Left+blanc, rcPop.Top+5},
                    {rcPop.Right-blanc-(len[2]-1), rcPop.Top+5}};
    labels.pLoc = aLoc;
    areas.num = 2;
    SMALL_RECT aArea[] = {{rcPop.Left+blanc, rcPop.Top+5,
                           rcPop.Left+blanc+(len[1]-1), rcPop.Top + 5},
                          {rcPop.Right-blanc-(len[2]-1), rcPop.Top + 5,
                           rcPop.Right - blanc, rcPop.Top + 5}};
    char aSort[] = {0, 0}, aTag[] = {1, 2};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    pos.X = rcPop.Left + 1;     pos.Y = rcPop.Top + 4;
    FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-1, pos, &ul);
    if (DealInput(&areas, &iHot) == 13 && iHot == 1)
    {   return FALSE;   }
    else{   return TRUE;    }
}
/*
    BOOL bRet;
    int judge;
    judge=MessageBoxA(NULL,"ȷ���˳�ϵͳ��","ϵͳ��ʾ",MB_YESNO);
    if(judge==IDYES){
        bRet = FALSE;
    }
    else{
        bRet = TRUE;
    }
    */

/**< �ڶ����� ϵͳ�Ǽܺ��� */

/**
 * ��������: LoadData
 * ��������: ������������������ݴ������ļ����뵽�ڴ滺������ʮ��������.
 * �������: ��
 * �������: ��
 * �� �� ֵ: BOOL����, ���ܺ����г��˺���ExitSys�ķ���ֵ����ΪFALSE��,
 *           ���������ķ���ֵ����ΪTRUE.
 *
 * ����˵��: Ϊ���ܹ���ͳһ�ķ�ʽ���ø����ܺ���, ����Щ���ܺ�����ԭ����Ϊ
 *           һ��, ���޲����ҷ���ֵΪBOOL. ����ֵΪFALSEʱ, ������������.
 */

BOOL LoadData()
{
    int Re = 0;
    Re = CreatList();
    gc_sys_state |= Re;
    gc_sys_state &= ~(1 + 2 + 4 - Re);
    if (gc_sys_state < (1 | 2 | 4 )){
        printf("\nϵͳ�������ݲ�����!\n");
        system("pause");
    }
    return TRUE;
}

/**
 * ��������: CreatList
 * ��������: �������ļ���ȡ��������, ����ŵ���������ʮ��������.
 * �������: ��
 * �������: head ����ͷָ��ĵ�ַ, ����������������ʮ����.
 * �� �� ֵ: int����ֵ, ��ʾ�����������.
 *           1 �Ѽ���Ժϵ��Ϣ���ݣ��޿����Ŷӻ�����Ϣ�Ϳ�����Ŀ����
 *           3 �Ѽ���Ժϵ��Ϣ�Ϳ����Ŷӻ�����Ϣ���ݣ��޿�����Ŀ����
 *           7 ����������ݶ��Ѽ���
 *
 * ����˵��:
 */
int CreatList(void)
{
    SCHOOL_NODE *head=NULL, tmp1;
    TEAM_NODE tmp2;
    PROJECT_NODE tmp3;
    FILE *pFile;
    int find;
    int re = 0;
    if ((pFile = fopen("school.dat", "rb+")) == NULL)
    {
        printf("Ժϵ��Ϣ�����ļ���ʧ��!\n");
        return re;
    }
    printf("Ժϵ��Ϣ�����ļ��򿪳ɹ�!\n");
    /*�������ļ��ж�����¥��Ϣ���ݣ������Ժ���ȳ���ʽ������������*/
    while (fread(&tmp1, sizeof(SCHOOL_NODE), 1, pFile) == 1)
    {
        p = (SCHOOL_NODE *)malloc(sizeof(SCHOOL_NODE));
        *p = tmp1;
        p->team_head = NULL;
        p->next = head;
        head = p;
    }
    fclose(pFile);
    if (head == NULL)
    {
        printf("Ժϵ��Ϣ�����ļ�����ʧ��!\n");
        return re;
    }
    printf("Ժϵ��Ϣ�����ļ����سɹ�!\n");
    sch_head = head;
    re += 1;
    if ((pFile = fopen("team.dat", "rb+")) == NULL)
    {
        printf("�����Ŷӻ�����Ϣ�����ļ���ʧ��!\n");
        return re;
    }
    printf("�����Ŷӻ�����Ϣ�����ļ��򿪳ɹ�!\n");
    re += 2;
    /*�������ļ��ж�ȡѧ��������Ϣ���ݣ�����������Ӧ����ѧ��������Ϣ֧����*/
    while (fread(&tmp2, sizeof(TEAM_NODE), 1, pFile) == 1)
    {
        /*������㣬��Ŵ������ļ��ж�����ѧ��������Ϣ*/
        q = (TEAM_NODE *)malloc(sizeof(TEAM_NODE));
        *q = tmp2;
        q->proj_head = NULL;
        /*�������ϲ��Ҹ�ѧ����ס����¥��Ӧ���������*/
        p = head;
        while (p != NULL&& strcmp(p->school_name, q->school_name))
        {
            p = p->next;
        }
        if (p != NULL) /*����ҵ����򽫽���Ժ���ȳ���ʽ����ѧ����Ϣ֧��*/
        {
            q->next = p->team_head;
            p->team_head = q;
        }
        else  /*���δ�ҵ������ͷ������������ڴ�ռ�*/
        {
            free(q);
        }
    }
    fclose(pFile);
    if ((pFile = fopen("project.dat", "rb")) == NULL)
    {
        printf("�����Ŷ���Ϣ�����ļ���ʧ��!\n");
        return re;
    }
    printf("�����Ŷ���Ϣ�����ļ��򿪳ɹ�!\n");
    re += 4;
    /*�������ļ��ж�ȡѧ���ɷ���Ϣ���ݣ�����ѧ��������Ϣ֧����Ӧ���Ľɷ�֧����*/
    while (fread(&tmp3, sizeof(PROJECT_NODE), 1, pFile) == 1)
    {
        /*������㣬��Ŵ������ļ��ж�����ѧ���ɷ���Ϣ*/
        r = (PROJECT_NODE *)malloc(sizeof(PROJECT_NODE));
        *r = tmp3;
        /*����ѧ����Ϣ֧���϶�Ӧѧ����Ϣ���*/
        p = head;
        find = 0;
        while (p != NULL && find == 0)
        {
            q = p->team_head;
            while (q != NULL && find == 0)
            {
                if (strcmp(q->team_name, r->team_name) == 0)
                {   find = 1;   break;  }
                q = q->next;
            }
            p = p->next;
        }
        if (find)  /*����ҵ����򽫽���Ժ���ȳ���ʽ����ѧ���ɷ���Ϣ֧����*/
        {
            r->next = q->proj_head;
            q->proj_head = r;
        }
        else /*���δ�ҵ������ͷ������������ڴ�ռ�*/
        {   free(r);    }
    }
    fclose(pFile);
    printf("�����������...\n"); getch();
    return re;
}

/**
 * ��������: RunSys
 * ��������: ����ϵͳ, ��ϵͳ�������������û���ѡ��Ĺ���ģ��.
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void RunSys(void)
{
    INPUT_RECORD inRec;
    DWORD res;
    COORD pos = {0, 0};
    BOOL bRet = TRUE;
    int i, loc, num;
    int cNo, cAtt;      /*cNo:�ַ���Ԫ���, cAtt:�ַ���Ԫ����*/
    char vkc, asc;      /*vkc:���������, asc:�ַ���ASCII��ֵ*/
    while (bRet)
    {
        /*�ӿ���̨���뻺�����ж�һ����¼*/
        ReadConsoleInput(gh_std_in, &inRec, 1, &res);
        if (inRec.EventType == MOUSE_EVENT) /*�����¼������¼�����*/
        {
            pos = inRec.Event.MouseEvent.dwMousePosition;  /*��ȡ�������λ��*/
            cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3; /*ȡ��λ�õĲ��*/
            cAtt = gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2;/*ȡ���ַ���Ԫ����*/
            if (cNo == 0) /*���Ϊ0��������λ��δ�������Ӳ˵�����*/
            {
                /* cAtt > 0 ������λ�ô�������(���˵����ַ���Ԫ)
                 * cAtt != gi_sel_menu ������λ�õ����˵���δ��ѡ��
                 * gp_top_layer->LayerNo > 0 ������ǰ���Ӳ˵�����
                 */
                if (cAtt > 0 && cAtt != gi_sel_menu && gp_top_layer->LayerNo > 0)
                {
                    PopOff();            /*�رյ������Ӳ˵�*/
                    gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/
                    PopMenu(cAtt);       /*��������������˵����Ӧ���Ӳ˵�*/
                }
            }
            else if (cAtt > 0) /*�������λ��Ϊ�����Ӳ˵��Ĳ˵����ַ���Ԫ*/
            {
                TagSubMenu(cAtt); /*�ڸ��Ӳ˵�������ѡ�б��*/
            }

            if (inRec.Event.MouseEvent.dwButtonState
                == FROM_LEFT_1ST_BUTTON_PRESSED) /*������������ߵ�һ��*/
            {
                if (cNo == 0) /*���Ϊ0��������λ��δ�������Ӳ˵�����*/
                {
                    if (cAtt > 0) /*�����λ�ô�������(���˵����ַ���Ԫ)*/
                    {
                        PopMenu(cAtt);   /*��������������˵����Ӧ���Ӳ˵�*/
                    }
                    /*�����λ�ò��������˵����ַ���Ԫ�������Ӳ˵�����*/
                    else if (gp_top_layer->LayerNo > 0)
                    {
                        PopOff();            /*�رյ������Ӳ˵�*/
                        gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/
                    }
                }
                else /*��Ų�Ϊ0��������λ�ñ������Ӳ˵�����*/
                {
                    if (cAtt > 0) /*�����λ�ô�������(�Ӳ˵����ַ���Ԫ)*/
                    {
                        PopOff(); /*�رյ������Ӳ˵�*/
                        gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/
                        /*ִ�ж�Ӧ���ܺ���:gi_sel_menu���˵����,cAtt�Ӳ˵����*/
                        bRet = ExeFunction(gi_sel_menu, cAtt);
                    }
                }
            }
            else if (inRec.Event.MouseEvent.dwButtonState
                     == RIGHTMOST_BUTTON_PRESSED) /*�����������Ҽ�*/
            {
                if (cNo == 0) /*���Ϊ0��������λ��δ�������Ӳ˵�����*/
                {
                    PopOff();            /*�رյ������Ӳ˵�*/
                    gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/
                }
            }
        }
        else if (inRec.EventType == KEY_EVENT  /*�����¼�ɰ�������*/
                 && inRec.Event.KeyEvent.bKeyDown) /*�Ҽ�������*/
        {
            vkc = inRec.Event.KeyEvent.wVirtualKeyCode; /*��ȡ�������������*/
            asc = inRec.Event.KeyEvent.uChar.AsciiChar; /*��ȡ������ASC��*/
            /*ϵͳ��ݼ��Ĵ���*/
            if (vkc == 112) /*�������F1��*/
            {
                if (gp_top_layer->LayerNo != 0) /*�����ǰ���Ӳ˵�����*/
                {
                    PopOff();            /*�رյ������Ӳ˵�*/
                    gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/
                }
                bRet = ExeFunction(5, 1);  /*���а������⹦�ܺ���*/
            }
            else if (inRec.Event.KeyEvent.dwControlKeyState
                     & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED))
            { /*������������Alt��*/
                switch (vkc)  /*�ж���ϼ�Alt+��ĸ*/
                {
                    case 88:  /*Alt+X �˳�*/
                        if (gp_top_layer->LayerNo != 0)
                        {   PopOff();   gi_sel_sub_menu = 0;    }
                        bRet = ExeFunction(1,4);    break;
                    case 70:  /*Alt+F*/     PopMenu(1);     break;
                    case 77: /*Alt+M*/      PopMenu(2);     break;
                    case 81: /*Alt+Q*/      PopMenu(3);     break;
                    case 83: /*Alt+S*/      PopMenu(4);     break;
                    case 72: /*Alt+H*/      PopMenu(5);     break;
                }
            }
            else if (asc == 0) /*�������Ƽ��Ĵ���*/
            {
                if (gp_top_layer->LayerNo == 0) /*���δ�����Ӳ˵�*/
                {
                    switch (vkc) /*�������(���ҡ���)������Ӧ�������Ƽ�*/
                    {
                        case 37:
                            gi_sel_menu--;
                            if (gi_sel_menu == 0)
                            {   gi_sel_menu = 5;    }
                            TagMainMenu(gi_sel_menu);   break;
                        case 39:
                            gi_sel_menu++;
                            if (gi_sel_menu == 6)
                            {   gi_sel_menu = 1;    }
                            TagMainMenu(gi_sel_menu);   break;
                        case 40:
                            PopMenu(gi_sel_menu);
                            TagSubMenu(1);              break;
                    }
                }
                else  /*�ѵ����Ӳ˵�ʱ*/
                {
                    for (loc=0,i=1; i<gi_sel_menu; i++)
                    {
                        loc += ga_sub_menu_count[i-1];
                    }  /*������Ӳ˵��еĵ�һ�����Ӳ˵��ַ��������е�λ��(�±�)*/
                    switch (vkc) /*�����(���ҡ��ϡ���)�Ĵ���*/
                    {
                        case 37:
                            gi_sel_menu--;
                            if (gi_sel_menu < 1)
                            {   gi_sel_menu = 5;    }
                            TagMainMenu(gi_sel_menu);
                            PopOff();       PopMenu(gi_sel_menu);
                            TagSubMenu(1);      break;
                        case 38:
                            num = gi_sel_sub_menu - 1;
                            if (num < 1)
                            {
                                num = ga_sub_menu_count[gi_sel_menu-1];
                            }
                            if (strlen(ga_sub_menu[loc+num-1]) == 0)
                            {   num--;  }
                            TagSubMenu(num);    break;
                        case 39:
                            gi_sel_menu++;
                            if (gi_sel_menu > 5)
                            {   gi_sel_menu = 1;    }
                            TagMainMenu(gi_sel_menu);
                            PopOff();       PopMenu(gi_sel_menu);
                            TagSubMenu(1);      break;
                        case 40:
                            num = gi_sel_sub_menu + 1;
                            if (num > ga_sub_menu_count[gi_sel_menu-1])
                            {   num = 1;    }
                            if (strlen(ga_sub_menu[loc+num-1]) == 0)
                            {   num++;  }
                            TagSubMenu(num);    break;
                    }
                }
            }
            else if ((asc-vkc == 0) || (asc-vkc == 32)){  /*������ͨ��*/
                if (gp_top_layer->LayerNo == 0)  /*���δ�����Ӳ˵�*/
                {
                    switch (vkc)
                    {
                        case 70: /*f��F*/        PopMenu(1);     break;
                        case 77: /*m��M*/        PopMenu(2);     break;
                        case 81: /*q��Q*/        PopMenu(3);     break;
                        case 83: /*s��S*/        PopMenu(4);     break;
                        case 72: /*h��H*/        PopMenu(5);     break;
                        case 13: /*�س�*/
                            PopMenu(gi_sel_menu);
                            TagSubMenu(1);      break;
                    }
                }
                else /*�ѵ����Ӳ˵�ʱ�ļ������봦��*/
                {
                    if (vkc == 27) /*�������ESC��*/
                    {
                        PopOff();       gi_sel_sub_menu = 0;
                    }
                    else if(vkc == 13) /*������»س���*/
                    {
                        num = gi_sel_sub_menu;
                        PopOff();       gi_sel_sub_menu = 0;
                        bRet = ExeFunction(gi_sel_menu, num);
                    }
                    else /*������ͨ���Ĵ���*/
                    {
                        /*������Ӳ˵��еĵ�һ�����Ӳ˵��ַ��������е�λ��(�±�)*/
                        for (loc=0,i=1; i<gi_sel_menu; i++)
                        {
                            loc += ga_sub_menu_count[i-1];
                        }

                        /*�����뵱ǰ�Ӳ˵���ÿһ��Ĵ����ַ����бȽ�*/
                        for (i=loc; i<loc+ga_sub_menu_count[gi_sel_menu-1]; i++)
                        {
                            if (strlen(ga_sub_menu[i])>0 && vkc==ga_sub_menu[i][1])
                            { /*���ƥ��ɹ�*/
                                PopOff();       gi_sel_sub_menu = 0;
                                bRet = ExeFunction(gi_sel_menu, i-loc+1);
                            }
                        }
                    }
                }
            }
        }
    }
}

/**
 * ��������: ExeFunction
 * ��������: ִ�������˵��ź��Ӳ˵���ȷ���Ĺ��ܺ���.
 * �������: m ���˵����
 *           s �Ӳ˵����
 * �������: ��
 * �� �� ֵ: BOOL����, TRUE �� FALSE
 *
 * ����˵��: ����ִ�к���ExitSysʱ, �ſ��ܷ���FALSE, ������������Ƿ���TRUE
 */
BOOL ExeFunction(int m, int s)
{
    BOOL bRet = TRUE;
    /*����ָ�����飬����������й��ܺ�������ڵ�ַ*/
    BOOL (*pFunction[ga_sub_menu_count[0]+ga_sub_menu_count[1]+ga_sub_menu_count[2]+ga_sub_menu_count[3]+ga_sub_menu_count[4]])(void);
    int i, loc;
    /*�����ܺ�����ڵ�ַ�����빦�ܺ����������˵��ź��Ӳ˵��Ŷ�Ӧ�±������Ԫ��*/
    pFunction[0] = FileOut;
    pFunction[1] = BackupData;
    pFunction[2] = RestoreData;
    pFunction[3] = ExitSys;
    pFunction[4] = MaintainSchoolInfo;
    pFunction[5] = MaintainTeamInfo;
    pFunction[6] = MaintainProjectInfo;
    pFunction[7] = SeekSchool;
    pFunction[8] = SeekTeam;
    pFunction[9] = SeekProject;
    pFunction[10] = TeachStuProp;
    pFunction[11] = ProjectTotal;
    pFunction[12] = TeamOfMostNSFC;
    pFunction[13] = ProjTeachProp;
    pFunction[14] = LeaderOfProj;
    pFunction[15] = HelpTopic;
    pFunction[16] = NULL;
    pFunction[17] = AboutDorm;
    for (i=1,loc=0; i<m; i++)  /*�������˵��ź��Ӳ˵��ż����Ӧ�±�*/
    {       loc += ga_sub_menu_count[i-1];      }
    loc += s - 1;
    if (pFunction[loc] != NULL)
    {
        bRet = (*pFunction[loc])();  /*�ú���ָ�������ָ��Ĺ��ܺ���*/
    }
    return bRet;
}

/**
 * ��������: CloseSys
 * ��������: �ر�ϵͳ.
 * �������: hd ����ͷָ��
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void CloseSys(void)
{
    ClearMemory();    ClearScreen();        /*����*/
    /*�ͷŴ�Ų˵�����״̬�����Ա�����ѧ�����������Ϣ��̬�洢��*/
    free(gp_buff_menubar_info);
    free(gp_buff_stateBar_info);
    /*�رձ�׼���������豸���*/
    CloseHandle(gh_std_out);        CloseHandle(gh_std_in);
    /*�����ڱ�������Ϊ���н���*/
    SetConsoleTitle("���н���");
    return;
}

/**
 * ��������: ExitSys
 * ��������: �����˳�ϵͳ��ѡ��
 * �������: ��
 * �������: ��
 * �� �� ֵ: BOOL���ͣ��˳�ΪTRUE��ȡ��ΪFALSE
 *
 * ����˵��:
 */
BOOL ExitSys(void)
{
    char *pCh[] = {"ȷ���˳�ϵͳ��", "ȷ��", "ȡ��"};
    BOOL bRet = Message(pCh);
    PopOff();
    return bRet;
}

/**
 * ��������: ClearMemory
 * ��������: ����ڴ�
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void ClearMemory(void){
    p1=sch_head;
    while (p1 != NULL) /*�ͷ�ʮ�ֽ�������Ķ�̬�洢��*/
    {
        p2 = p1->next;        q1 = p1->team_head;
        while (q1 != NULL) /*�ͷ�ѧ��������Ϣ֧���Ķ�̬�洢��*/
        {
            q2 = q1->next;      r1 = q1->proj_head;
            while (r1 != NULL) /*�ͷŽɷ���Ϣ֧���Ķ�̬�洢��*/
            {   r2 = r1->next;      free(r1);       r1 = r2;    }
            free(q1);   q1 = q2;
        }
        free(p1);  /*�ͷ��������Ķ�̬�洢��*/
        p1 = p2;
    }
}

BOOL SaveData(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"���˵���ļ�",
                           "�Ӳ˵�����ݱ���",
                           "ȷ��"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}

/**< �������� ϵͳ���ܺ��� */

/**
 * ��������: BackupData
 * ��������: ��ϵͳ����������������ݱ��ݵ�һ�������ļ�.
 * �������: ��
 * �������: ��
 * �� �� ֵ: BOOL����, ����ΪTRUE
 *
 * ����˵��:
 */
BOOL BackupData(void)
{
    FILE *out;
// int handle;
    unsigned short school_num=0, team_num=0, project_num=0;
    time_t lt;
    char *filename, *pc, *appe=".dat";
    for(p=sch_head;p!=NULL;p=p->next){
        school_num++;       q=p->team_head;
        while(q!=NULL){
            team_num++;     r=q->proj_head;
            while(r!=NULL){
                project_num++;  r=r->next;
            }
            q=q->next;
        }
    }
//    printf("%hu  %hu  %hu\n",school_num,team_num,project_num);
    lt=time(NULL);
    filename=ctime(&lt);
    filename[strlen(filename)-1]='\0';
    strcat(filename,appe);
    pc=&filename[0];
    while(*pc){
        if((*pc==':' ) ||   (*pc==' ')){   *pc='_';    }
        pc++;
    }
/*
    if((handle=open(filename,O_WRONLY|O_BINARY)== -1)){
        handle=open(filename,O_CREAT|O_BINARY|O_RDWR ,S_IREAD|S_IWRITE);
    }
    write(handle,(char*)&school_num,sizeof(school_num));
    write(handle,(char*)&team_num,sizeof(team_num));
    write(handle,(char*)&project_num,sizeof(project_num));
    for(p=sch_head;p!=NULL;p=p->next){
        write(handle,(char*)p,sizeof(p));
    }
    for(p=sch_head;p!=NULL;q=q->next){
        for(q=p->team_head;q!=NULL;q=q->next){
            write(handle,(char*)q,sizeof(q));
        }
    }
    for(p=sch_head;p!=NULL;q=q->next){
        for(q=p->team_head;q!=NULL;q=q->next){
            for(r=q->proj_head;r!=NULL;r=r->next){
                write(handle,(char*)r,sizeof(r));
            }
        }
    }
    close(handle);
*/
    if((out=fopen(filename,"wb"))==NULL){
        printf("��ʧ�ܣ�\n");
    }
    fwrite(&school_num,sizeof(unsigned short),1,out);
    fwrite(&team_num,sizeof(unsigned short),1,out);
    fwrite(&project_num,sizeof(unsigned short),1,out);
    for(p=sch_head;p!=NULL;p=p->next){
        fwrite(p,sizeof(SCHOOL_NODE),1,out);
    }
    for(p=sch_head;p!=NULL;p=p->next){
        for(q=p->team_head;q!=NULL;q=q->next){
            fwrite(q,sizeof(TEAM_NODE),1,out);
        }
    }
    for(p=sch_head;p!=NULL;p=p->next){
        for(q=p->team_head;q!=NULL;q=q->next){
            for(r=q->proj_head;r!=NULL;r=r->next){
               fwrite(r,sizeof(PROJECT_NODE),1,out);
            }
        }
    }
    fclose(out);
    printf("���ݳɹ��������������...\n");  getch();     ClearScreen();
    return TRUE;
}

/**
 * ��������: RestoreData
 * ��������: ��ָ�������ļ��лָ������������.
 * �������: ��
 * �������: ��
 * �� �� ֵ: BOOL����, ����ΪTRUE
 *
 * ����˵��:
 */
BOOL RestoreData(void)
{
//    FILE *in;
    CONSOLE_CURSOR_INFO lpCur;
    GetConsoleCursorInfo(gh_std_out,&lpCur);
    lpCur.bVisible=TRUE;
    SetConsoleCursorInfo(gh_std_out,&lpCur);
    printf("��������ݽ�����ϵͳ��ǰ���ݣ���������Y   �������˵��������������\n");
    fflush(stdin);  j=getchar(); getchar();
    if(j!='y'&&j!='Y'){ ClearScreen();  return TRUE;   }
    ClearMemory();
    char input[100], *filename;
    printf("��������Ҫ�ָ����������ڵ��ļ���:");
    fflush(stdin);      scanf("%s",input);
    filename=input;
    SCHOOL_NODE *hd=NULL,*psch_node;
    TEAM_NODE *pteam_node;
    PROJECT_NODE *psciprj_node;
    unsigned short sch_node_num=0, team_node_num=0, sciprj_node_num=0, loop;
    int handle,find;
    if( (handle=open(filename,O_RDONLY|O_BINARY)) == -1)
    {
        char *plabel_name[] = {"������ı����ļ������ڣ�",
                            "���ݻָ�ʧ�ܣ����Զ��������һ�θ��µ�����",
                            "ȷ��"
                                };
        ShowModule(plabel_name, 3);
        CreatList();    ClearScreen();
        return TRUE;
    }
    /*��ȡ���ֻ�����Ϣ�ļ�¼����*/
    read(handle, (char*)&sch_node_num, sizeof(sch_node_num));
    read(handle, (char*)&team_node_num, sizeof(team_node_num));
    read(handle, (char*)&sciprj_node_num, sizeof(sciprj_node_num));
    /*��ȡԺϵ������Ϣ������ʮ������*/
    for(loop=1; loop<=sch_node_num; loop++)
    {
        psch_node=(SCHOOL_NODE*)malloc(sizeof(SCHOOL_NODE));
        read(handle, (char*)psch_node, sizeof(SCHOOL_NODE));
        psch_node->team_head=NULL;//��������ָ��
        psch_node->next=hd;
        hd=psch_node;
    }
    sch_head=hd;
    /*��ȡ�����Ŷӻ�����Ϣ�����������Ŷ���Ϣ֧��*/
    for(loop=1; loop<=team_node_num; loop++)
    {
        pteam_node=(TEAM_NODE*)malloc(sizeof(TEAM_NODE));
        read(handle, (char*)pteam_node, sizeof(TEAM_NODE));
        pteam_node->proj_head=NULL;//�������� ָ��
        psch_node=hd;
        while( (psch_node!=NULL)  &&  ( strcmp(psch_node->school_name,pteam_node->school_name )!=0)  )
        {
           psch_node=psch_node->next;
        }
        if(psch_node!=NULL)
        {
            pteam_node->next=psch_node->team_head;
            psch_node->team_head=pteam_node;
        }
        else
        {   free(pteam_node);   }
    }
     /*��ȡ������Ŀ������Ϣ������������Ŀ������Ϣ֧��*/
     for(loop=1; loop<=sciprj_node_num; loop++)
     {
         psciprj_node=(PROJECT_NODE*)malloc(sizeof(PROJECT_NODE));
         read(handle, (char*)psciprj_node, sizeof(PROJECT_NODE));
         psch_node=hd;
         find=0;
         while( (psch_node!=NULL) && (find==0) )
         {
             pteam_node=psch_node->team_head;
             while(  (pteam_node!=NULL)&&(find==0)  )
             {
                 if( strcmp(pteam_node->team_name, psciprj_node->team_name) == 0  )
                 {  find=1;     break;  }
                 pteam_node=pteam_node->next;
             }
             psch_node=psch_node->next;
         }
         if(find)
         {
             psciprj_node->next=pteam_node->proj_head;
             pteam_node->proj_head=psciprj_node;
         }
         else{  free(psciprj_node);     }
     }
     close(handle);
     printf("�����������...\n"); getch(); ClearScreen();
     GetConsoleCursorInfo(gh_std_out,&lpCur);
     lpCur.bVisible=FALSE;
     SetConsoleCursorInfo(gh_std_out,&lpCur);
     return TRUE;
}

/**
 * ��������: FileIn
 * ��������: ��ָ�������ļ��е�������������ݣ����ã�.
 * �������: ��
 * �������: sch_head ʮ�������׵�ַ
 * �� �� ֵ: BOOL����, ����ΪTRUE
 *
 * ����˵��:
 */
BOOL FileIn(void)
{
    FILE *in;
    char find=0;
    SCHOOL_NODE *head = NULL;
    /*¼��Ժϵ����*/
    if ((in = fopen("school.dat", "rb+")) == NULL)
    {
        printf("school.dat��ʧ��!\n");
    }
    while (!feof(in))
    {
        p=(SCHOOL_NODE*)malloc(sizeof(SCHOOL_NODE));
        if(!fread(p,sizeof(SCHOOL_NODE),1,in)){  free(p);    break;  }
        p->team_head = NULL;
        p->next = head;
        head = p;
    }
    fclose(in);
    /*ָ�뵹�أ�¼���Ŷ���Ϣ���ڸ�Ժϵ�¹���*/
    sch_head = head;
    if ((in = fopen("team.dat", "rb+")) == NULL)
    {
        printf("team.dat��ʧ��!\n");
    }
    while (!feof(in))
    {
        q=(TEAM_NODE*)malloc(sizeof(TEAM_NODE));
        if(!fread(q,sizeof(TEAM_NODE),1,in)){  free(q);    break;  }
        q->proj_head = NULL;
        p=head;
        while (p!=NULL&&(strcmp(p->school_name,q->school_name)))
        {   p=p->next;  }
        if (p!=NULL)
        {   q->next = p->team_head;     p->team_head = q;   }
        else{   free(q);    }
    }
    fclose(in);
    /*ָ�뵹�أ�¼����Ŀ��Ϣ���ڸ��Ŷ��¹���*/
    if ((in = fopen("project.dat", "rb+")) == NULL)
    {
        printf("project.dat��ʧ��!\n");
    }
    while (!feof(in))
    {
        find=0;
        r=(PROJECT_NODE*)malloc(sizeof(PROJECT_NODE));
        if(!fread(r,sizeof(PROJECT_NODE),1,in)){  free(r);    break;  }
        p=head;
        while (p!= NULL&&find == 0)
        {
            q=p->team_head;
            while (q!= NULL&&find == 0)
            {
                if (!strcmp(q->team_name,r->team_name))
                {   find = 1;   break;  }
                q=q->next;
            }
            p=p->next;
        }
        if (find)
        {   r->next = q->proj_head;     q->proj_head = r;   }
        else{   free(r);   }
    }
    fclose(in);
    printf("���ݶ�ȡ��ϣ�\n");    system("pause");
    return TRUE;
}

/**
 * ��������: FileOut
 * ��������: ����ϵͳ���ݵ�ָ�������ļ����ı��ļ������ã�.
 * �������: ��
 * �������: ��
 * �� �� ֵ: BOOL����, ����ΪTRUE
 *
 * ����˵��:
 */
BOOL FileOut(void)     /*����ͬʱ����������ļ����ı��ļ������ж������ļ���Ϊ��һ�ν���ϵͳ�������������*/
{
    FILE *out;
    /*дtxt�ļ���Ժϵ��Ϣ-�Ŷ���Ϣ-��Ŀ��ϢΪ����������ʽ�������Ķ�*/
    if((out=fopen("output.txt","w+"))==NULL){
        printf("output.txt��ʧ�ܣ�");
        exit(-1);
    }
    for(p=sch_head;p!=NULL;p=p->next){
        fprintf(out,"Ժϵ���ƣ�\t\t%s\n�����ˣ�\t\t%s\n��ϵ�绰\t\t%s\n",
                p->school_name,   p->leader,      p->contact_num);
        for(q=p->team_head;q!=NULL;q=q->next){
            fprintf(out,"\t�Ŷ����ƣ�\t\t%s\n\t�����ˣ�\t\t%s\n\t��ʦ����\t\t%d\n\t�о�������\t\t%d\n",
                    q->team_name,        q->leader,        q->teacher_num,   q->stu_num);
            for(r=q->proj_head;r!=NULL;r=r->next){
                fprintf(out,"\t\t��Ŀ��ţ�\t\t%s\n\t\t��Ŀ���\t\t",r->project_num);
                /*��Ŀ���ͱ�Ż�ԭΪ����,���ڲ鿴*/
                switch(r->type){
                    case '1':   fprintf(out,"973�ƻ���Ŀ\n");  break;
                    case '2':   fprintf(out,"������Ȼ��ѧ������Ŀ\n");  break;
                    case '3':   fprintf(out,"863�ƻ���Ŀ\n");  break;
                    case '4':   fprintf(out,"���ʺ�����Ŀ\n");  break;
                    case '5':   fprintf(out,"������Ŀ\n");  break;
                }
                fprintf(out,"\t\t��ʼʱ�䣺\t\t%s\n\t\t��Ŀ���ѣ�\t\t%f��Ԫ\n\t\t�����ˣ�\t\t%s\n",
                        r->open_time,      r->budget,            r->leader);
            }
        }
    }
    fclose(out);
    /*дԺϵdat�ļ�*/
    if((out=fopen("school.dat","wb+"))==NULL){
        printf("school.dat��ʧ�ܣ�");      exit(-1);
    }
    for(p=sch_head;p!=NULL;p=p->next){
        fwrite(p,sizeof(SCHOOL_NODE),1,out);
    }
    fclose(out);
    /*д�Ŷ�dat�ļ�*/
    if((out=fopen("team.dat","wb+"))==NULL){
        printf("team.dat��ʧ�ܣ�");        exit(-1);
    }
    for(p=sch_head;p!=NULL;p=p->next){
        for(q=p->team_head;q!=NULL;q=q->next){
            fwrite(q,sizeof(TEAM_NODE),1,out);
        }
    }
    fclose(out);
    /*д��Ŀdat�ļ�*/
    if((out=fopen("project.dat","wb+"))==NULL){
        printf("project.dat��ʧ�ܣ�");     exit(-1);
    }
    for(p=sch_head;p!=NULL;p=p->next){
        for(q=p->team_head;q!=NULL;q=q->next){
            for(r=q->proj_head;r!=NULL;r=r->next){
               fwrite(r,sizeof(PROJECT_NODE),1,out);
            }
        }
    }
    fclose(out);
    return TRUE;
}

/**
 * ��������: Print
 * ��������: ��������ʾʮ������ȫ����Ϣ�����ã�.
 * �������: ��
 * �������: ��
 * �� �� ֵ: BOOL����, ����ΪTRUE
 *
 * ����˵��:
 */
BOOL Print(void)
{
    /*дstdout��Ժϵ��Ϣ-�Ŷ���Ϣ-��Ŀ��ϢΪ����������ʽ�������Ķ�*/
    for(p=sch_head;p!=NULL;p=p->next){
        printf("Ժϵ���ƣ�\t%s\n�����ˣ�\t%s\n��ϵ�绰\t%s\n",
                p->school_name,   p->leader,      p->contact_num);
        for(q=p->team_head;q!=NULL;q=q->next){
            printf("\t�Ŷ����ƣ�\t%s\n\t�����ˣ�\t%s\n\t��ʦ����\t%d\n\t�о�������\t%d\n",
                    q->team_name,        q->leader,        q->teacher_num,   q->stu_num);
            for(r=q->proj_head;r!=NULL;r=r->next){
                printf("\t\t��Ŀ��ţ�\t%s\n\t\t��Ŀ���\t",r->project_num);
                switch(r->type){
                    case '1':   printf("973�ƻ���Ŀ\n");  break;
                    case '2':   printf("������Ȼ��ѧ������Ŀ\n");  break;
                    case '3':   printf("863�ƻ���Ŀ\n");  break;
                    case '4':   printf("���ʺ�����Ŀ\n");  break;
                    case '5':   printf("������Ŀ\n");  break;
                }
                printf("\t\t��ʼʱ�䣺\t%s\n\t\t��Ŀ���ѣ�\t%f��Ԫ\n\t�����ˣ�\t%s\n",
                        r->open_time,      r->budget,            r->leader);
            }
        }
    }
    return TRUE;
}

/**
 * ��������: MaintainSchoolInfo
 * ��������: Ժϵ��Ϣά��Ŀ¼
 * �������: ��
 * �������: ��
 * �� �� ֵ: BOOL����, ����ΪTRUE
 *
 * ����˵��:
 */
BOOL MaintainSchoolInfo(void)
{
    BOOL bRet = TRUE;
    do{
        printf("������к��ֲ�����\n");
        printf("¼��          press1\n");
        printf("�޸�          press2\n");
        printf("ɾ��          press3\n");
        printf("�������˵�    press4\n");
        fflush(stdin);  k=getchar(); getchar();
        if(k=='4'){ ClearScreen();  return TRUE;    }
    }while(k<'1'||k>'4');
    ClearScreen();
    if(k=='1'){        bRet = AddSchool();  }
    else if(k=='2'){   bRet = AltSchool();  }
    else{              bRet = DeleteSchool();  }
    return bRet;
}

/**
 * ��������: MaintainTeamInfo
 * ��������: �����Ŷ���Ϣά��Ŀ¼
 * �������: ��
 * �������: ��
 * �� �� ֵ: BOOL����, ����ΪTRUE
 *
 * ����˵��:
 */
BOOL MaintainTeamInfo(void)
{
    BOOL bRet = TRUE;
    do{
        printf("������к��ֲ�����\n");
        printf("¼��          press1\n");
        printf("�޸�          press2\n");
        printf("ɾ��          press3\n");
        printf("�������˵�    press4\n");
        fflush(stdin);  k=getchar(); getchar();
        if(k=='4'){ ClearScreen();  return TRUE;    }
    }while(k<'1'||k>'4');
    ClearScreen();
    if(k=='1'){        bRet = AddTeam();  }
    else if(k=='2'){   bRet = AltTeam();  }
    else{              bRet = DeleteTeam();  }
    return bRet;
}

/**
 * ��������: MaintainProjectInfo
 * ��������: ������Ŀ��Ϣά��Ŀ¼
 * �������: ��
 * �������: ��
 * �� �� ֵ: BOOL����, ����ΪTRUE
 *
 * ����˵��:
 */
BOOL MaintainProjectInfo(void)
{
    BOOL bRet = TRUE;
    do{
        printf("������к��ֲ�����\n");
        printf("¼��          press1\n");
        printf("�޸�          press2\n");
        printf("ɾ��          press3\n");
        printf("�������˵�    press4\n");
        fflush(stdin);  k=getchar(); getchar();
        if(k=='4'){ ClearScreen();  return TRUE;    }
    }while(k<'1'||k>'4');
    ClearScreen();
    if(k=='1'){        bRet = AddProject();   }
    else if(k=='2'){   bRet = AltProject();   }
    else{              bRet = DeleteProject(); }
    return bRet;
}

/**
 * ��������: AddSchool
 * ��������: ��ʮ�������в���һ��Ժϵ��Ϣ���.
 * �������: ��
 * �������: ��
 * �� �� ֵ: BOOL����-TRUE
 *
 * ����˵��:
 */
BOOL AddSchool(void)
{
    while(1){
        ClearScreen();  fflush(stdin);
        p = (SCHOOL_NODE *)malloc(sizeof(SCHOOL_NODE));
        /*p-���ݳ�Ա¼�뿪ʼ*/
        while(1){
            flag = 0;
            printf("����Ժϵ���ƣ�ע�����ִ�Сд����");
            scanf("%s", p->school_name);    getchar();
            for(p1=sch_head;p1!=NULL;p1=p1->next){
                if(!strcmp(p1->school_name,p->school_name)){
                    printf("��Ժϵ�Ѵ���!�������˵���ȷ��--Y  ȡ��--���������\n");
                    k=getchar();    getchar();
                    if(k=='y'||k=='Y'){
                        free(p);    p=NULL;    ClearScreen();  return TRUE;
                    }
                    strcpy(p->school_name,"");
                    flag = 1;   ClearScreen();  break;
                }
                if(flag){   break;  }
            }
        if(!flag){  break;  }
        }
        printf("����Ժϵ������������");
        scanf("%s", p->leader);         getchar();
        printf("������ϵ�绰��");
        scanf("%s", p->contact_num);    getchar();
        p->team_head=NULL;
        /*p-���ݳ�Ա¼�����*/
        p->next=sch_head;       sch_head=p;
        printf("��������Y   �������˵��������������\n");
        j=getchar(); getchar();        FileOut();
        if(j!='Y'&&j!='y'){ break;  }
    }
    ClearScreen();
    return TRUE;
}

/**
 * ��������: AddTeam
 * ��������: ��ʮ�������в���һ�������Ŷ���Ϣ���.
 * �������: ��
 * �������: ��
 * �� �� ֵ: BOOL����-TRUE
 *
 * ����˵��:
 */
BOOL AddTeam(void)
{
    while(1){
        ClearScreen();  fflush(stdin);      flag=0;
        q=(TEAM_NODE*)malloc(sizeof(TEAM_NODE));
        while(1){
            flag = 0;
            printf("�����Ŷ����ƣ�");
            scanf("%s", q->team_name);    getchar();
            for(p1=sch_head;p1!=NULL;p1=p1->next){
                for(q1=p1->team_head;q1!=NULL;q1=q1->next){
                    if(!strcmp(q1->team_name,q->team_name)){
                        printf("���Ŷ��Ѵ���!�������˵���ȷ��--Y  ȡ��--���������\n");
                        k=getchar();    getchar();
                        if(k=='y'||k=='Y'){
                            free(q);    q=NULL;    ClearScreen();  return TRUE;
                        }
                        strcpy(q->team_name,"");
                        flag = 1;   break;
                    }
                    if(flag){   break;  }
                }
                if(flag){   break;  }
            }
            if(!flag){  break;  }
        }
        printf("�����Ŷ�����Ժϵ��ע�����ִ�Сд����");
        scanf("%s", q->school_name);    getchar();
        for(p=sch_head;p!=NULL;p=p->next){
            /*���ҵ�����Ժϵ���˳���������ǰpλ��*/
            if(!strcmp(p->school_name,q->school_name)){    flag=1;    break;    }
        }
        /*���Ŷ�����Ժϵ�У��������Ժϵ��������Ժϵ��������Ӧ�Ŷ�*/
        if(!flag){
            p = (SCHOOL_NODE *)malloc(sizeof(SCHOOL_NODE));
            /*p-���ݳ�Ա¼�뿪ʼ*/
            printf("����Ժϵ������������");
            scanf("%s", p->leader);         getchar();
            printf("������ϵ�绰��");
            scanf("%s", p->contact_num);    getchar();
            strcpy(p->school_name,q->school_name);
            p->team_head=NULL;
            /*p-���ݳ�Ա¼�����*/
            p->next=sch_head;       sch_head=p;
        }
        /*q-���ݳ�Ա¼�뿪ʼ*/
        printf("�����ŶӸ�����������");
        scanf("%s", q->leader);    getchar();
        printf("�����ʦ������");
        scanf("%d", &q->teacher_num);    getchar();
        printf("�����о���������");
        scanf("%d", &q->stu_num);    getchar();
        q->proj_head=NULL;
        /*q-���ݳ�Ա¼�����*/
        q->next=p->team_head;       p->team_head=q;
        printf("��������Y   �������˵��������������\n");
        j=getchar(); getchar();          FileOut();
        if(j!='Y'&&j!='y'){ break;  }
    }
    ClearScreen();
    return TRUE;
}

/**
 * ��������: AddProject
 * ��������: ��ʮ�������в���һ��������Ŀ��Ϣ���.
 * �������: ��
 * �������: ��
 * �� �� ֵ: BOOL����-TRUE
 *
 * ����˵��:
 */
BOOL AddProject(void)
{
    char school_name[20];
    while(1){
        ClearScreen();  fflush(stdin);
        flag=0;
        r=(PROJECT_NODE*)malloc(sizeof(PROJECT_NODE));
        while(1){
            flag = 0;
            printf("������Ŀ��ţ�");
            scanf("%s", r->project_num);    getchar();
            for(p1=sch_head;p1!=NULL;p1=p1->next){
                for(q1=p1->team_head;q1!=NULL;q1=q1->next){
                    for(r1=q1->proj_head;r1!=NULL;r1=r1->next){
                        if(!strcmp(r1->project_num,r->project_num)){
                            printf("����Ŀ�Ѵ���!�������˵���ȷ��--Y  ȡ��--���������\n");
                            k=getchar();    getchar();
                            if(k=='y'||k=='Y'){
                                free(r);    r=NULL;    ClearScreen();  return TRUE;
                            }
                            strcpy(r->project_num,"");
                            flag = 1;   break;
                        }
                        if(flag){   break;  }
                    }
                    if(flag){   break;  }
                }
                if(flag){   break;  }
            }
            if(!flag){  break;  }
        }
        printf("������Ŀ����Ժϵ��ע�����ִ�Сд����");
        scanf("%s", school_name);    getchar();
        printf("������Ŀ�����Ŷ����ƣ�");
        scanf("%s", r->team_name);    getchar();
        for(p=sch_head;p!=NULL;p=p->next){
            /*���ҵ�����Ժϵ���˳���������ǰpλ��*/
            if(!strcmp(p->school_name,school_name)){    flag=1;    break;    }
        }
        /*����Ŀ�ҿ�����Ժϵ�£��������Ժϵ��������Ժϵ��������ӦԺϵ���Ŷ�*/
        if(!flag){
            p = (SCHOOL_NODE *)malloc(sizeof(SCHOOL_NODE));
            /*p-���ݳ�Ա¼�뿪ʼ*/
            printf("����Ժϵ������������");
            scanf("%s", p->leader);         getchar();
            printf("������ϵ�绰��");
            scanf("%s", p->contact_num);    getchar();
            strcpy(p->school_name,school_name);
            /*p-���ݳ�Ա¼�����*/
            p->team_head=NULL;
            p->next=sch_head;       sch_head=p;
            q=(TEAM_NODE*)malloc(sizeof(TEAM_NODE));
            /*q-���ݳ�Ա¼�뿪ʼ*/
            printf("�����ŶӸ�����������");
            scanf("%s", q->leader);         getchar();
            printf("�����ʦ������");
            scanf("%d", &q->teacher_num);    getchar();
            printf("�����о���������");
            scanf("%d", &q->stu_num);    getchar();
            strcpy(q->team_name,r->team_name);
            strcpy(q->school_name,school_name);
            /*q-���ݳ�Ա¼�����*/
            q->proj_head=NULL;
            q->next=p->team_head;   p->team_head=q;
        }
        else{
            flag=0;
            for(q=p->team_head;q!=NULL;q=q->next){
                /*���ҵ������Ŷӣ��˳���������ǰpλ��*/
                if(!strcmp(q->team_name,r->team_name)){    flag=1;    break;    }
            }
            /*����Ŀ�ҿ������Ŷ��£���������Ŷ����������Ŷ�*/
            if(!flag){
                q=(TEAM_NODE*)malloc(sizeof(TEAM_NODE));
                /*q-���ݳ�Ա¼�뿪ʼ*/
                printf("�����ŶӸ�����������");
                scanf("%s", q->leader);    getchar();
                printf("�����ʦ������");
                scanf("%d", &q->teacher_num);    getchar();
                printf("�����о���������");
                scanf("%d", &q->stu_num);    getchar();
                strcpy(q->team_name,r->team_name);
                strcpy(q->school_name,p->school_name);
                /*q-���ݳ�Ա¼�����*/
                q->proj_head=NULL;
                q->next=p->team_head;   p->team_head=q;
            }
        }
        /*r-���ݳ�Ա¼�뿪ʼ*/
        printf("973�ƻ���Ŀ--1\t\t������Ȼ��ѧ������Ŀ--2\t\t863�ƻ���Ŀ--3\n���ʺ�����Ŀ--4\t\t������Ŀ--5\n������Ŀ���");
        r->type=getchar();    getchar();
        printf("������ʼʱ�䣨���ռ��λ���������ǰ��0���룩��");
        scanf("%s", r->open_time);    getchar();
        printf("������Ŀ���ѣ���λ����Ԫ����");
        scanf("%f", &r->budget);    getchar();
        printf("������Ŀ������������");
        scanf("%s", r->leader);    getchar();
        /*r-���ݳ�Ա¼�����*/
        r->next=q->proj_head;           q->proj_head=r;
        printf("��������Y   �������˵��������������\n");
        j=getchar(); getchar();    FileOut();
        if(j!='Y'&&j!='y'){ break;  }
    }
    ClearScreen();
    return TRUE;
}

/**
 * ��������: AltSchool
 * ��������: ��ʮ���������޸�һ��Ժϵ��Ϣ���.
 * �������: ��
 * �������: ��
 * �� �� ֵ: BOOL����-TRUE
 *
 * ����˵��:
 */
BOOL AltSchool(void)
{
    while(1){
        ClearScreen();  fflush(stdin);
        char count=0;
        do{
            if(count){
                /*������ʾ*/
                printf("δ�ҵ�Ŀ����Ϣ����������Y   �������˵��������������\n");
                j=getchar(); getchar(); ClearScreen();
                if(j!='y'&&j!='Y'){   return TRUE;   }
        }
            do{
                fflush(stdin);
                printf("��Ҫ�����ַ�ʽ��ѯ��Ҫ�޸ĵ���Ϣ��\nԺϵ����--1\t\tԺϵ������--2\t\t��ϵ�绰--3\n");
                scanf("%hu", &choice);  getchar();
            }while(choice<1||choice>3);
            if(choice==1){          printf("����Ժϵ���ƣ�");      }
            else if(choice==2){     printf("����Ժϵ�����ˣ�");    }
            else{                   printf("������ϵ��ʽ��");      }
            fflush(stdin);  scanf("%s", seek);    getchar();
            /*��������Ϣ������Ӧ�ڵ�*/
            for(p=sch_head;p!=NULL;p=p->next){
                if(choice==1){      if(!strcmp(seek,p->school_name))  break;  }
                else if(choice==2){ if(!strcmp(seek,p->leader))       break;  }
                else{               if(!strcmp(seek,p->contact_num))  break;  }
            }
            count++;
        }while(p==NULL);
        do{
            /*�޸���Ϣ*/
            printf("��Ҫ�޸�ʲô��Ϣ��\nԺϵ����--1\t\tԺϵ������--2\t\t��ϵ�绰--3\t\t���--4\n");
            scanf("%hu", &choice);  getchar();
                        if(choice>='1'&&choice<='3'){   printf("����������Ϣ��");  }
            switch(choice){
                case 1:
                    printf("�����µ�Ժϵ���ƣ�");  scanf("%s", p->school_name);
                    getchar();  break;
                case 2:
                    printf("�����µ�Ժϵ�����ˣ�");    scanf("%s", p->leader);
                    getchar();  break;
                case 3:
                    printf("�����µ���ϵ��ʽ��");  scanf("%s",p->contact_num);
                    getchar();  break;
            }
            if(choice==4){  break;  }
        }while(1);
        printf("��������Y   �������˵��������������\n");
        fflush(stdin);  j=getchar(); getchar();
        if(j!='Y'&&j!='y'){ break;  }
    }
    ClearScreen();  FileOut();
    return TRUE;
}

/**
 * ��������: AltTeam
 * ��������: ��ʮ���������޸�һ�������Ŷ���Ϣ���.
 * �������: ��
 * �������: ��
 * �� �� ֵ: BOOL����-TRUE
 *
 * ����˵��:
 */
BOOL AltTeam(void)
{
    while(1){
        ClearScreen();  fflush(stdin);
        char count=0;
        do{
            if(count){
                /*������ʾ*/
                printf("δ�ҵ�Ŀ����Ϣ����������Y   �������˵��������������\n");
                j=getchar(); getchar(); ClearScreen();
                if(j!='y'&&j!='Y'){   return TRUE;   }
            }
            printf("�����Ŷ����ƣ�");
            fflush(stdin);  scanf("%s", seek);    getchar();
            /*��������Ϣ����Ŀ���Ŷӣ���¼q��λ�ò��˳�ѭ��*/
            for(p=sch_head;p!=NULL;p=p->next){
                for(q=p->team_head;q!=NULL;q=q->next){
                    j=strcmp(seek,q->team_name);
                    if(!j)    break;
                }
                if(!j)    break;
            }
            count++;
        }while(p==NULL);
        do{
            /*�޸���Ϣ*/
            printf("��Ҫ�޸�ʲô��Ϣ��\n�Ŷ�����--1\t�ŶӸ�����--2\t��ʦ����--3\t�о�������--4\t���--5\n");/*����Ժϵ����޸ģ�*/
            scanf("%hu", &choice);  getchar();
                        if(choice>='1'&&choice<='4'){   printf("����������Ϣ��");  }
            switch(choice){
                case 1:
                    printf("�����µ��Ŷ����ƣ�");    scanf("%s", q->team_name);
                    getchar();  break;
                case 2:
                    printf("�����µ��ŶӸ����ˣ�");    scanf("%s", q->leader);
                    getchar();  break;
                case 3:
                    printf("�����µĽ�ʦ������");     scanf("%d", &q->teacher_num);
                    getchar();  break;
                case 4:
                    printf("�����µ��о���������");     scanf("%d", &q->stu_num);
                    getchar();  break;
            }
            if(choice==5){  break;  }
        }while(1);
        printf("��������Y   �������˵��������������\n");
        fflush(stdin);  j=getchar(); getchar();
        if(j!='Y'&&j!='y'){ break;  }
    }
    ClearScreen();      FileOut();
    return TRUE;
}

/**
 * ��������: AltProject
 * ��������: ��ʮ���������޸�һ�������Ŷ���Ϣ���.
 * �������: ��
 * �������: ��
 * �� �� ֵ: BOOL����-TRUE
 *
 * ����˵��:
 */
BOOL AltProject(void)
{
    while(1){
        ClearScreen();  fflush(stdin);
        char count=0;
        do{
            if(count){
                /*������ʾ*/
                printf("δ�ҵ�Ŀ����Ϣ����������Y   �������˵��������������\n");
                j=getchar(); getchar(); ClearScreen();
                if(j!='y'&&j!='Y'){   return TRUE;   }
            }
            printf("������Ŀ��ţ�");
            fflush(stdin);    scanf("%s", seek);    getchar();
            /*��������Ϣ����Ŀ����Ŀ����¼r��λ�ò��˳�ѭ��*/
            for(p=sch_head;p!=NULL;p=p->next){
                for(q=p->team_head;q!=NULL;q=q->next){
                    for(r=q->proj_head;r!=NULL;r=r->next){
                        j=strcmp(seek,r->project_num);
                        if(!j)    break;
                    }
                    if(!j)    break;
                }
                if(!j)    break;
            }
            count++;
        }while(p==NULL);
        do{
            /*�޸���Ϣ*/
            printf("��Ҫ�޸�ʲô��Ϣ��\n��Ŀ���--1\t��Ŀ���--2\t��ʼʱ��--3\n��Ŀ����--4\t��Ŀ������--5\t���--6\n");/*�����Ŷ�����޸ģ�*/
            scanf("%hu", &choice);  getchar();
            if(choice>='1'&&choice<='5'){   printf("����������Ϣ��");  }
            switch(choice){
                case 1:
                    printf("�����µ���Ŀ��ţ�");     scanf("%s", r->project_num);
                    getchar();  break;
                case 2:
                    printf("�����µ���Ŀ���");     scanf("%c", &r->type);
                    getchar();  break;
                case 3:
                    printf("�����µ���ʼʱ�䣺");     scanf("%s", r->open_time);
                    getchar();  break;
                case 4:
                    printf("�����µ���Ŀ���ѣ�");     scanf("%f", &r->budget);
                    getchar();  break;
                case 5:
                    printf("�����µ���Ŀ�����ˣ�");     scanf("%s", r->leader);
                    getchar();  break;
            }
            if(choice==6){  break;  }
        }while(1);
        printf("��������Y   �������˵��������������\n");
        fflush(stdin);  j=getchar(); getchar();
        if(j!='Y'&&j!='y'){ break;  }
    }
    ClearScreen();      FileOut();
    return TRUE;
}

/**
 * ��������: DeleteSchool
 * ��������: ��ʮ��������ɾ��һ��Ժϵ��Ϣ���.
 * �������: ��
 * �������: ��
 * �� �� ֵ: BOOL����-TRUE
 *
 * ����˵��:
 */
BOOL DeleteSchool(void)
{
    while(1){
        ClearScreen();
        char count=0;
        do{
            p1=sch_head;
            if(count){
                /*������ʾ*/
                printf("δ�ҵ�Ŀ����Ϣ����������Y   �������˵��������������\n");
                j=getchar(); getchar(); ClearScreen();
                if(j!='y'&&j!='Y'){   return TRUE;   }
            }
            do{
                printf("\nע�⣺ɾ��Ժϵ����ʱ����һ��ɾ��Ժϵ��ȫ���Ŷ�����Ŀ�����ݣ�\n");
                printf("��Ҫ�����ַ�ʽ��ѯ��Ҫ�޸ĵ���Ϣ��\nԺϵ����--1\t\tԺϵ������--2\t\t��ϵ�绰--3\n");
                scanf("%hu", &choice);  getchar();
            }while(choice<1||choice>3);
            if(choice==1){          printf("����Ժϵ���ƣ�");      }
            else if(choice==2){     printf("����Ժϵ�����ˣ�");    }
            else{                   printf("������ϵ��ʽ��");      }
            fflush(stdin);  scanf("%s", seek);    getchar();
            /*��������Ϣ������ӦԺϵ*/
            while(p1){
                if(choice==1){      if(!strcmp(seek,p1->school_name))  break;  }
                else if(choice==2){ if(!strcmp(seek,p1->leader))       break;  }
                else{               if(!strcmp(seek,p1->contact_num))  break;  }
                p2=p1;
                p1=p1->next;
            }
            count++;
        }while(p1==NULL);
        if(!p1->team_head); /*Ժϵ�����Ŷ�*/
        else{               /*Ժϵ�����Ŷ�*/
            /*ɾ�������Ŷӽڵ�-��ʼ*/
            q=p1->team_head->next;
            while(q!=NULL){
                if(!q->proj_head);  /*�Ŷ�������Ŀ*/
                else{               /*�Ŷ�������Ŀ*/
                    /*ɾ��������Ŀ�ڵ�-��ʼ*/
                    r=q->proj_head->next;
                    while(r!=NULL){
                        q->proj_head->next=r->next;
                        free(r);
                        r=q->proj_head->next;
                    }
                    r=q->proj_head;
                    q->proj_head=NULL;
                    free(r);
                }
                /*ɾ��������Ŀ�ڵ�-����*/
                p1->team_head->next=q->next;
                free(q);
                q=p1->team_head->next;
            }
            q=p1->team_head;
            /*ɾ��������Ŀ�ڵ�-��ʼ*/
            r=q->proj_head->next;
            while(r!=NULL){
                q->proj_head->next=r->next;
                free(r);
                r=q->proj_head->next;
            }
            r=q->proj_head;
            q->proj_head=NULL;
            free(r);
            /*ɾ��������Ŀ�ڵ�-����*/
            p1->team_head=NULL;
            free(q);
            /*ɾ�������Ŷӽڵ�-����*/
        }
        if(p1==sch_head){  sch_head=p1->next;     }
        else{   p2->next=p1->next;  }        free(p1);   /*ɾ��Ժϵ�ڵ�*/
        printf("ɾ���ɹ���\n��������Y   �������˵��������������\n");
        fflush(stdin);  j=getchar(); getchar();
        if(j!='Y'&&j!='y'){ break;  }
    }
    ClearScreen();      FileOut();
    return TRUE;
}

/**
 * ��������: DeleteTeam
 * ��������: ��ʮ��������ɾ��һ�������Ŷ���Ϣ���.
 * �������: ��
 * �������: ��
 * �� �� ֵ: BOOL����-TRUE
 *
 * ����˵��:
 */
BOOL DeleteTeam(void)
{
    while(1){
        ClearScreen();
        printf("\nע�⣺ɾ���Ŷ�����ʱ����һ��ɾ���Ŷ���ȫ����Ŀ�����ݣ�\n");
        char count=0;
        do{
            if(count){
                /*������ʾ*/
                printf("δ�ҵ�Ŀ����Ϣ����������Y   �������˵��������������\n");
                j=getchar(); getchar(); ClearScreen();
                if(j!='y'&&j!='Y'){   return TRUE;   }
            }
            printf("�����Ŷ����ƣ�");
            fflush(stdin);  scanf("%s", seek);    getchar();
            /*��������Ϣ������Ӧ�Ŷӽڵ�*/
            for(p=sch_head;p!=NULL;p=p->next){
                q1=p->team_head;
                while(q1!=NULL){
                    j=strcmp(seek,q1->team_name);
                    if(!j){     break;  }
                    q2=q1;      q1=q1->next;
                }
                if(!j){     break;  }
            }
            count++;
        }while(p==NULL);
        if(!q1->proj_head); /*�Ŷ�������Ŀ*/
        else{               /*�Ŷ�������Ŀ*/
            /*ɾ�������Ŷӽڵ�-��ʼ*/
            r=q1->proj_head->next;
            while(r!=NULL){
                q1->proj_head->next=r->next;
                free(r);
                r=q1->proj_head->next;
            }
            r=q1->proj_head;
            q1->proj_head=NULL;
            free(r);
        /*ɾ�������Ŷӽڵ�-����*/
        }
        if(q1==p->team_head){   p->team_head=q1->next; }
        else{   q2->next=q1->next;  }        free(q1);   /*ɾ���Ŷӽڵ�*/
        /*Ժϵ���Ŷ�Ϊ��ʱǿ���˳�����ֹ����Ϊ����������ѭ��*/
        if(p->team_head==NULL){
            printf("��ѧԺ�������Ŷӣ�\n");
            printf("�����������...\n"); getch();     ClearScreen();
            return TRUE;
        }
        printf("ɾ���ɹ���\n��������Y   �������˵��������������\n");
        fflush(stdin);  j=getchar(); getchar();
        if(j!='Y'&&j!='y'){ break;  }
    }
    ClearScreen();      FileOut();
    return TRUE;
}

/**
 * ��������: DeleteProject
 * ��������: ��ʮ��������ɾ��һ��������Ŀ��Ϣ���.
 * �������: ��
 * �������: ��
 * �� �� ֵ: BOOL����-TRUE
 *
 * ����˵��:
 */
BOOL DeleteProject(void)
{
    do{
        ClearScreen();
        char count=0;
        do{
            if(count){
                /*������ʾ*/
                printf("δ�ҵ�Ŀ����Ϣ����������Y   �������˵��������������\n");
                j=getchar(); getchar();     ClearScreen();
                if(j!='y'&&j!='Y'){   return TRUE;   }
            }
            printf("������Ŀ��ţ�");
            fflush(stdin);  scanf("%s", seek);    getchar();
            /*��������Ϣ������Ӧ��Ŀ�ڵ�*/
            for(p=sch_head;p!=NULL;p=p->next){
                for(q=p->team_head;q!=NULL;q=q->next){
                    r1=q->proj_head;
                    while(r1!=NULL){
                        j=strcmp(seek,r1->project_num);
                        if(!j){   break;  }
                        r2=r1;      r1=r1->next;
                    }
                    if(!j){    break;   }
                }
                if(!j){    break;   }
            }
            count++;
        }while(p==NULL);
        if(r1==q->proj_head){   q->proj_head=r1->next;  }
        else{   r2->next=r1->next;  }        free(r1);  /*ɾ����Ŀ�ڵ�*/
        /*�Ŷ�����ĿΪ��ʱǿ���˳�����ֹ����Ϊ����������ѭ��*/
        if(q->proj_head==NULL){
            printf("���Ŷ���������Ŀ��\n");
            printf("�����������...\n"); getch();    ClearScreen();
            return TRUE;
        }
        printf("ɾ���ɹ���\n��������Y   �������˵��������������\n");
        fflush(stdin);  j=getchar();
    }while(j=='Y'||j=='y');
    ClearScreen();      FileOut();
    return TRUE;
}

/**
 * ��������: SeekSchool
 * ��������: ����Ҫ����ʮ�������в������з���Ҫ��Ժϵ��Ϣ���.
 * �������: ��
 * �������: ��
 * �� �� ֵ: BOOL����-TRUE
 *
 * ����˵��:
 */
BOOL SeekSchool(void)
{
    while(1){
        char count=0,flag=0,judge;
        ClearScreen();
        do{
            judge=1;
            if(count){
                /*������ʾ*/
                printf("δ�ҵ�Ŀ����Ϣ����������Y   �������˵��������������\n");
                j=getchar(); getchar();     ClearScreen();
                if(j!='y'&&j!='Y'){   return TRUE;   }
            }
            printf("��Ҫ�����ַ�ʽ��ѯ��\nԺϵ������%6c--press1\nԺϵ����%6c--press2\n",' ',' ');
            k=getchar();    getchar();
            if(k=='1')  {   printf("Ժϵ������:");      }
            else        {   printf("Ժϵ�����Ӵ�:");    }
            fflush(stdin);  scanf("%s", seek);    getchar();
            printf("Ժϵ����%16cԺϵ����������%4c��ϵ�绰\n",' ',' ');
            for(p=sch_head;p!=NULL;p=p->next){
                if(k=='1'){     /*��Ժϵ�����˲�ѯ*/
                    if(strstr(p->leader,seek)){
                        printf("%-24s%-18s%s\n",p->school_name,p->leader,p->contact_num);
                        flag=1;
                    }
                }
                else{           /*��Ժϵ���ƹؼ��ʲ�ѯ*/
                    if(strstr(p->school_name,seek)){
                        printf("%-24s%-18s%s\n",p->school_name,p->leader,p->contact_num);
                        flag=1;
                    }
                }
            }
            if(!judge){     break;  }
            count++;
        }while(p1==NULL&&!flag);
        printf("��������Y   �������˵��������������\n");
        fflush(stdin);  j=getchar();    getchar();
        if(j!='y'&&j!='Y'){ ClearScreen();  return TRUE;   }
    }
    ClearScreen();
    return TRUE;
}

/**
 * ��������: SeekTeam
 * ��������: ����Ҫ����ʮ�������в������з���Ҫ������Ŷ���Ϣ���.
 * �������: ��
 * �������: ��
 * �� �� ֵ: BOOL����-TRUE
 *
 * ����˵��:
 */
BOOL SeekTeam(void)
{
    unsigned short low,high;
    while(1){
        char count=0,flag=0;
        ClearScreen();
        do{
            /*������ʾ*/
            if(count){
                /*������ʾ*/
                printf("δ�ҵ�Ŀ����Ϣ����������Y   �������˵��������������\n");
                j=getchar(); getchar();     ClearScreen();
                if(j!='y'&&j!='Y'){   return TRUE;   }
            }
            printf("��Ҫ�����ַ�ʽ��ѯ��\n�Ŷ�����%6c--press1\n��ʦ����%6c--press2\n",' ',' ');
            k= getchar(); getchar();
            if(k=='1'){
                printf("�Ŷ������Ӵ�:");
                fflush(stdin);  scanf("%s", seek);    getchar();
            }
            else{
                printf("��ʦ��������:");
                scanf("%hu", &low);     getchar();
                printf("��ʦ��������:");
                scanf("%hu", &high);    getchar();
            }
            printf("�Ŷ�����%14c�ŶӸ���������%2c����Ժϵ%16c��ʦ����%2c�о�������\n",' ',' ',' ',' ');
            for(p=sch_head;p!=NULL;p=p->next){
                for(q=p->team_head;q!=NULL;q=q->next){
                    if(k=='1'){     /*���Ŷ����ƹؼ��ʲ�ѯ*/
                        if(strstr(q->team_name,seek)){
                            printf("%-22s%-16s%-24s%-10d%d\n",q->team_name,q->leader,q->school_name,q->teacher_num,q->stu_num);
                            flag=1;
                        }
                    }
                    else{           /*����ʦ������Χ��ѯ*/
                        if(q->teacher_num>=low&&q->teacher_num<=high){
                            printf("%-22s%-16s%-24s%-10d%d\n",q->team_name,q->leader,q->school_name,q->teacher_num,q->stu_num);
                            flag=1;
                        }
                    }
                }
            }
            count++;
        }while(p==NULL&&!flag);
        printf("��������Y   �������˵��������������\n");
        j=getchar(); getchar();
        if(j!='Y'&&j!='y'){ break;  }
    }
    ClearScreen();
    return TRUE;
}

/**
 * ��������: SeekProject
 * ��������: ����Ҫ����ʮ�������в������з���Ҫ������Ŷ���Ϣ���.
 * �������: ��
 * �������: ��
 * �� �� ֵ: BOOL����-TRUE
 *
 * ����˵��:
 */
BOOL SeekProject(void)
{
    while(1){
        char count=0,flag=0;
        ClearScreen();
        do{
            /*������ʾ*/
            if(count){
                /*������ʾ*/
                printf("δ�ҵ�Ŀ����Ϣ����������Y   �������˵��������������\n");
                j=getchar(); getchar();     ClearScreen();
                if(j!='y'&&j!='Y'){   return TRUE;   }
            }
            printf("��Ҫ�����ַ�ʽ��ѯ��\n��Ŀ���%6c--press1\n�����Ŷ�%6c--press2\n",' ',' ');
            k= getchar(); getchar();
            if(k=='1'){   printf("��Ŀ���:");       }
            else{          printf("��Ŀ�����Ŷ�:");   }
            fflush(stdin);  scanf("%s", seek);    getchar();
            printf("��Ŀ���%9c��ʼʱ��%4c��Ŀ����%6c��Ŀ������%5c�����Ŷ�%10c��Ŀ���\n",' ',' ',' ',' ',' ');
            for(p=sch_head;p!=NULL;p=p->next){
                for(q=p->team_head;q!=NULL;q=q->next){
                    if(k=='1'){     /*����Ŀ��Ų�ѯ*/
                        for(r=q->proj_head;r!=NULL;r=r->next){
                            if(!strcmp(r->project_num,seek)){
                                printf("%-17s%-12s%-27f��Ԫ%-15s%-18s",r->project_num,r->open_time,r->budget,r->leader,r->team_name);
                                switch(r->type){
                                    case '1':   printf("973�ƻ���Ŀ\n");  break;
                                    case '2':   printf("������Ȼ��ѧ������Ŀ\n");  break;
                                    case '3':   printf("863�ƻ���Ŀ\n");  break;
                                    case '4':   printf("���ʺ�����Ŀ\n");  break;
                                    case '5':   printf("������Ŀ\n");  break;
                                }
                                flag=1;
                            }
                        }
                    }
                    else{           /*�������ŶӲ�ѯ*/
                        if(!strcmp(q->team_name,seek)){
                            for(r=q->proj_head;r!=NULL;r=r->next){
                                printf("%-17s%-12s%-27f��Ԫ%-15s%-18s",r->project_num,r->open_time,r->budget,r->leader,r->team_name);
                                switch(r->type){
                                    case '1':   printf("973�ƻ���Ŀ\n");  break;
                                    case '2':   printf("������Ȼ��ѧ������Ŀ\n");  break;
                                    case '3':   printf("863�ƻ���Ŀ\n");  break;
                                    case '4':   printf("���ʺ�����Ŀ\n");  break;
                                    case '5':   printf("������Ŀ\n");  break;
                                }
                                flag=1;
                            }
                        }
                    }
                }
            }
            count++;
        }while(p==NULL&&!flag);
        printf("��������Y   �������˵��������������\n");
        j=getchar(); getchar();
        if(j!='Y'&&j!='y'){ break;  }
    }
    ClearScreen();
    return TRUE;
}

/**
 * ��������: TeachStuProp
 * ��������: ͳ�Ƹ�Ժϵ��ʦ�������о������������о������ʦ�������ȣ�����2λС������
 *            ��ѧ����ʦ������ֵ������������ͳ�ƽ��.
 * �������: ��
 * �������: ��
 * �� �� ֵ: BOOL����-TRUE
 *
 * ����˵��:
 */
BOOL TeachStuProp(void)
{
    /*�Ը�Ժϵ��ʦ���о���������Ͳ������ֵ*/
    for(p=sch_head;p!=NULL;p=p->next){
        p->sum_teacher=p->sum_student=p->proportion=0;
        for(q=p->team_head;q!=NULL;q=q->next){
            p->sum_teacher+=q->teacher_num;
            p->sum_student+=q->stu_num;
        }
        p->proportion=p->sum_student*1.0/p->sum_teacher;
        if(p->sum_teacher==0||p->sum_student==0){
            p->proportion=0;
        }
    }
    /*����ֵ�ý���ָ����ķ�����������*/
	p1 = (SCHOOL_NODE*)malloc(sizeof(SCHOOL_NODE));
	p1->next=sch_head;;
	sch_head=prior1=p1;
	for(p1 = prior1->next; p1->next !=NULL; prior1 = p1, p1 = p1->next) {
		for(p2 = p1->next, prior2 = p1; p2 != NULL; prior2 = p2, p2 = p2->next) {
			if(p1->proportion < p2->proportion) {
				t = p2->next ;		prior1->next = p2 ;		prior2->next = p1 ;
				p2->next = p1->next ;		p1->next = t; 		t=p1; p1=p2; p2=t;
			}
		}
	}
	p1=sch_head;		sch_head=sch_head->next;		free(p1);
	/*��ӡ������*/
	printf("%8cԺϵ%16c�о�������%3c��ʦ����%3c�о�����ʦ������\n",b,b,b,b);
	for(p=sch_head;p!=NULL;p=p->next){
        if(p->proportion==0){   continue;   }
        printf("%-20s%14d%12d%16.2f\n",p->school_name,(int)p->sum_student,(int)p->sum_teacher,p->proportion);
	}
    printf("�����������...\n"); getch(); ClearScreen();
	return TRUE;
}

/**
 * ��������: ProjectTotal
 * ��������: ͳ��ĳ��ȸ�Ժϵ������Ŀ����973 ��Ŀ����863 ��Ŀ����
 *           �������ܾ��ѣ�������Ŀ��������������.
 * �������: ��
 * �������: ��
 * �� �� ֵ: BOOL����-TRUE
 *
 * ����˵��:
 */
BOOL ProjectTotal(void)
{
    do{
        unsigned short year, ori;
        ClearScreen();  flag=0;
        printf("��������ʼ���:");
        scanf("%hu",&year);  getchar();
        /*������Ҫ����ݵ���Ŀ��ͳ��������Ŀ�������Ծ������*/
        for(p=sch_head;p!=NULL;p=p->next){
            p->num_proj=p->num_863=p->num_973=p->budget=0;
            for(q=p->team_head;q!=NULL;q=q->next){
                for(r=q->proj_head;r!=NULL;r=r->next){
                    ori=(r->open_time[0]-'0')*1000+(r->open_time[1]-'0')*100+(r->open_time[2]-'0')*10+(r->open_time[3]-'0');
                    if(ori<=year){
                        flag=1;
                        p->num_proj++;
                        p->budget+=r->budget;
                        if(r->type=='1'){       p->num_973++;   }
                        else if(r->type=='3'){  p->num_863++;   }
                    }
                }
            }
        }
        if(flag){
            p1 = (SCHOOL_NODE*)malloc(sizeof(SCHOOL_NODE));
            p1->next=sch_head;
            /*����Ŀ�����ý���ָ����ķ�����������*/
            sch_head=prior1=p1;
            for(p1 = prior1->next; p1->next !=NULL; prior1 = p1, p1 = p1->next) {
                for(p2 = p1->next, prior2 = p1; p2 != NULL; prior2 = p2, p2 = p2->next) {
                    if(p1->num_proj < p2->num_proj) {
                        t = p2->next ;		prior1->next = p2 ;		prior2->next = p1 ;
                        p2->next = p1->next ;		p1->next = t; 		t=p1; p1=p2; p2=t;
                    }
                }
            }
            p1=sch_head;		sch_head=sch_head->next;		free(p1);
            /*��ӡ������*/
            printf("%hu��\n%8cԺϵ%15c��Ŀ����%3c973��Ŀ����%3c863��Ŀ����%3c�����ܾ���\n",year,b,b,b,b,b);
            for(p=sch_head;p!=NULL;p=p->next){
                if(p->num_proj==0){     continue;   }
                printf("%-20s%12u%13u%14u%18f\n",p->school_name,p->num_proj,p->num_973,p->num_863,p->budget);
            }
        }
        else{   printf("�����û���Ѿ���ʼ����Ŀ��\n");  }
        printf("��������Y   �������˵��������������\n");
        j=getchar(); getchar();
    }while(j=='Y'||j=='y');
    ClearScreen();
    return TRUE;
}

/**
 * ��������: TeamOfMostNSFC
 * ��������: ͳ�����������Ϊ������Ȼ��ѧ����Ŀ�����Ŀ������10 �������Ŷӣ�����Ŀ�������������������Ŷ����ƣ�
 *            ���Ϊ������Ȼ��ѧ����Ŀ�����Ŀ�����Լ���Ŀ��������.
 * �������: ��
 * �������: ��
 * �� �� ֵ: BOOL����-TRUE
 *
 * ����˵��:
 */
BOOL TeamOfMostNSFC(void)
{
    unsigned char min=255, count=0;
    /*��ָ�����͵���Ŀ��ͳ���������Ծ������*/
    for(p=sch_head;p!=NULL;p=p->next){
        for(q=p->team_head;q!=NULL;q=q->next){
            q->projsci_num=q->proj_budget=0;
            for(r=q->proj_head;r!=NULL;r=r->next){
                if(r->type=='2'){
                    q->projsci_num++;   q->proj_budget+=r->budget;
                }
            }
        }
    }
    /*�������Ŷӵ����Ƽ���Ŀ������������������ͳһ����*/
    q1=(TEAM_NODE*)malloc(sizeof(TEAM_NODE));
    head=q1;
    for(p=sch_head;p!=NULL;p=p->next){
        for(q=p->team_head;q!=NULL;q=q->next){
            q1->next=(TEAM_NODE*)malloc(sizeof(TEAM_NODE));
            q1=q1->next;
            q1->projsci_num=q->projsci_num;
            strcpy(q1->team_name,q->team_name);
            q1->proj_budget=q->proj_budget;
            q1->next=NULL;
        }
    }
    q1->next=NULL;
    /*����Ŀ�����ý���ָ����ķ�����������*/
	qrior1=head;
	for(q1 = qrior1->next; q1->next !=NULL; qrior1 = q1, q1 = q1->next) {
		for(q2 = q1->next, qrior2 = q1; q2 != NULL; qrior2 = q2, q2 = q2->next) {
			if(q1->projsci_num<q2->projsci_num) {
				u = q2->next ;		qrior1->next = q2 ;		qrior2->next = q1 ;
				q2->next = q1->next ;		q1->next = u; 		u=q1; q1=q2; q2=u;
			}
		}
	}
	q1=head;		head=head->next;		free(q1);
	/*��ӡ���������˴������е�������뿼�ǣ�������ʵ����������������*/
	printf("%6c�Ŷ�����%6c������Ȼ��ѧ������Ŀ��%4c������Ŀ�ܾ���\n",b,b,b);
	for(u=head;u!=NULL;u=u->next,count++){
        if(count>=10&&u->projsci_num<min){  break;  }
        min=u->projsci_num;
        printf("%-31s%-15u%f\n",u->team_name,u->projsci_num,u->proj_budget);
	}
	q1=head;
	for(q2=q1->next;q2!=NULL;q1=q2,q2=q1->next)
        free(q1);
    printf("�����������...\n"); getch(); ClearScreen();
    return TRUE;
}

/**
 * ��������: ProjTeachProp
 * ��������: ͳ�����������Ϊ������Ȼ��ѧ����Ŀ�����Ŀ������10 �������Ŷӣ�����Ŀ�������������������Ŷ����ƣ�
 *            ���Ϊ������Ȼ��ѧ����Ŀ�����Ŀ�����Լ���Ŀ��������.
 * �������: ��
 * �������: ��
 * �� �� ֵ: BOOL����-TRUE
 *
 * ����˵��:
 */
BOOL ProjTeachProp(void)
{
    unsigned char count=0;
    float min=1.0e6;
    /*ͳ�ƿ�����Ŀ�����ʦ������ֵ*/
    for(p=sch_head;p!=NULL;p=p->next){
        for(q=p->team_head;q!=NULL;q=q->next){
            q->proj_num=0;
            for(r=q->proj_head;r!=NULL;r=r->next){  q->proj_num++;  }
                q->proportion=q->proj_num*1.0/q->teacher_num;
            }
    }
    /*�½������Ŷ�������ԭ�����±�������*/
    q1=(TEAM_NODE*)malloc(sizeof(TEAM_NODE));
    head=q1;
    for(p=sch_head;p!=NULL;p=p->next){
        for(q=p->team_head;q!=NULL;q=q->next){
            q1->next=(TEAM_NODE*)malloc(sizeof(TEAM_NODE));
            q1=q1->next;
            strcpy(q1->team_name,q->team_name);
            q1->teacher_num=q->teacher_num;
            q1->proj_num=q->proj_num;
            q1->proportion=q->proportion;
        }
    }
    q1->next=NULL;
	/*����Ŀ�����ʦ������ֵ��������*/
	qrior1=head;
	for(q1 = qrior1->next; q1->next !=NULL; qrior1 = q1, q1 = q1->next) {
		for(q2 = q1->next, qrior2 = q1; q2 != NULL; qrior2 = q2, q2 = q2->next) {
			if(q1->proportion<q2->proportion) {
				u = q2->next ;		qrior1->next = q2 ;		qrior2->next = q1 ;
				q2->next = q1->next ;		q1->next = u; 		u=q1; q1=q2; q2=u;
			}
		}
	}
	q1=head;		head=head->next;		free(q1);
	/*��ӡ���������˴������е�������뿼�ǣ�������ʵ����������������*/
	printf("%6c�Ŷ�����%6c��ʦ����%3c��Ŀ����%3c��Ŀ���ͽ�ʦ������ֵ\n",b,b,b,b);
	for(u=head;u!=NULL;u=u->next,count++){
        if(count>=5&&u->proportion<min){    break;  }
        min=u->proportion;
        printf("%-23s%-11d%-16u%.2f\n",u->team_name,u->teacher_num,u->proj_num,u->proportion);
	}
	/*ɾ���½�����*/
	q1=head;
	for(q2=q1->next;q2!=NULL;q1=q2,q2=q1->next)
        free(q1);
    printf("�����������...\n"); getch(); ClearScreen();
    return TRUE;
}

/**
 * ��������: LeaderOfProj
 * ��������: ���ض�Ժϵ��ͳ�Ƹ����˸�����Ŀ����Ŀ��
 * ������������������������������Ŀ������
 * �������: ��
 * �������: ��
 * �� �� ֵ: BOOL����-TRUE
 *
 * ����˵��:
 */
BOOL LeaderOfProj(void)
{
    LEADER_NODE *head, *t, *prior1, *prior2, *p1, *p2;
    unsigned short count,save;
    do{
        count=1;    head=NULL;
        /*ѡ��Ժϵ*/
        do{
            if(count-1){
                /*������ʾ*/
                printf("δ�ҵ�Ŀ����Ϣ����������Y   �������˵��������������\n");
                j=getchar(); getchar();     ClearScreen();
                if(j!='y'&&j!='Y'){   return TRUE;   }
            }
            printf("Ժϵ���ƣ�");
            scanf("%s",seek); getchar();
            for(p=sch_head;p!=NULL;p=p->next){
                if(!strcmp(p->school_name,seek)){    break;   }
            }
            count++;
        }while(!p);
        if(!p->team_head){  printf("��Ժϵ��������Ŀ��\n");   }
        else{
            /*�½�������ר����������ԭ��ͳ�Ƹ������˳нӵ���Ŀ����*/
            for(q=p->team_head;q!=NULL;q=q->next){
                for(r=q->proj_head;r!=NULL;r=r->next){
                    for(p1=head;p1!=NULL;p1=p1->next){  if(!strcmp(r->leader,p1->leader)){   break; }    }
                    if(p1){  p1->n++;     }
                    else{
                        t=(LEADER_NODE*)malloc(sizeof(LEADER_NODE));
                        strcpy(t->leader,r->leader);    t->n=1;
                        t->next=head;   head=t;
                    }
                }
            }
            /*����Ŀ�������ʦ������ֵ��������*/
            prior1=head=(LEADER_NODE*)malloc(sizeof(LEADER_NODE));
            head->n=0;
            head->next=t;
            for(p1 = prior1->next; p1->next !=NULL; prior1 = p1, p1 = p1->next) {
                for(p2 = p1->next, prior2 = p1; p2 != NULL; prior2 = p2, p2 = p2->next) {
                    if(p1->n<p2->n) {
                        t = p2->next ;		prior1->next = p2 ;		prior2->next = p1 ;
                        p2->next = p1->next ;		p1->next = t; 		t=p1; p1=p2; p2=t;
                    }
                }
            }
            /*��ӡ���������˴������е�������뿼��*/
            printf("����\t����������\t������Ŀ����\n");
            count=1;    save=count;
            for(prior1=head,p1=head->next;p1!=NULL;prior1=prior1->next,p1=prior1->next,count++){
                if(p1->n==prior1->n){   printf("%2hu", save);    }
                else{   save=count;     printf("%2hu",count);    }
                printf("%13s%15hu\n",p1->leader,p1->n);
            }
            /*�ͷŽɷ���Ϣ֧���Ķ�̬�洢��*/
            p1=head;
            while (p1 != NULL)
            {   p2 = p1->next;      free(p1);      p1 = p2;    }
        }
        printf("��������Y   �������˵��������������\n");
        j=getchar(); getchar();     ClearScreen();
    }while(j=='Y'||j=='y');
    return TRUE;
}

BOOL HelpTopic(void)
{
    ShellExecuteA(NULL,"open","NOTEPAD.EXE","help.txt",NULL,SW_SHOWNORMAL);
    printf("�����������...\n"); getch(); ClearScreen();
    return TRUE;
}

BOOL AboutDorm(void)
{
    char *plabel_name[] = {"ϵͳ�汾��V1.0",
                           "**************",
                           "All Rights Reserved",
                           "ȷ��"
                          };
    ShowModule(plabel_name, 4);
    return TRUE;
}
