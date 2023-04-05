#include "dorm.h"
unsigned long ul;
int main()
{
    COORD size = {SCR_COL, SCR_ROW};              /*窗口缓冲区大小*/
    gh_std_out = GetStdHandle(STD_OUTPUT_HANDLE); /* 获取标准输出设备句柄*/
    gh_std_in = GetStdHandle(STD_INPUT_HANDLE);   /* 获取标准输入设备句柄*/
    SetConsoleTitle(gp_sys_name);                 /*设置窗口标题*/
    SetConsoleScreenBufferSize(gh_std_out, size); /*设置窗口缓冲区大小80*25*/
//    FileIn();
    LoadData();                     /*数据加载*/
    InitInterface();                /*界面初始化*/
    RunSys();                       /*系统功能模块的选择及运行*/
    CloseSys();                     /*退出系统*/
    return 0;
}

/**< 第一部分 文字界面函数 */

/**
 * 函数名称: InitInterface
 * 函数功能: 初始化界面.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void InitInterface()
{
    WORD att = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
               | BACKGROUND_BLUE;  /*黄色前景和蓝色背景*/
    SetConsoleTextAttribute(gh_std_out, att);  /*设置控制台屏幕缓冲区字符属性*/
    ClearScreen();  /* 清屏*/
    /*创建弹出窗口信息堆栈，将初始化后的屏幕窗口当作第一层弹出窗口*/
    gp_scr_att = (char *)calloc(SCR_COL * SCR_ROW, sizeof(char));/*屏幕字符属性*/
    gp_top_layer = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));
    gp_top_layer->LayerNo = 0;      /*弹出窗口的层号为0*/
    gp_top_layer->rcArea.Left = 0;  /*弹出窗口的区域为整个屏幕窗口*/
    gp_top_layer->rcArea.Top = 0;
    gp_top_layer->rcArea.Right = SCR_COL - 1;
    gp_top_layer->rcArea.Bottom = SCR_ROW - 1;
    gp_top_layer->pContent = NULL;
    gp_top_layer->pScrAtt = gp_scr_att;
    gp_top_layer->next = NULL;
    ShowMenu();     /*显示菜单栏*/
    ShowState();    /*显示状态栏*/
    return;
}

/**
 * 函数名称: ClearScreen
 * 函数功能: 清除屏幕信息.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void ClearScreen(void)
{
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    COORD home = {0, 1};    /*仅清空从第一行开始的信息，以便被各个功能函数调用*/
    unsigned long size;
    GetConsoleScreenBufferInfo( gh_std_out, &bInfo );/*取屏幕缓冲区信息*/
    size =  bInfo.dwSize.X * (bInfo.dwSize.Y); /*计算屏幕缓冲区字符单元数*/
    /*将屏幕缓冲区所有单元的字符属性设置为当前屏幕缓冲区字符属性*/
    FillConsoleOutputAttribute(gh_std_out, bInfo.wAttributes, size, home, &ul);
    /*将屏幕缓冲区所有单元填充为空格字符*/
    FillConsoleOutputCharacter(gh_std_out, ' ', size, home, &ul);
    SetConsoleCursorPosition(gh_std_out, home);     /*光标置于窗口左上角*/
    return;
}

/**
 * 函数名称: ShowMenu
 * 函数功能: 在屏幕上显示主菜单, 并设置热区, 在主菜单第一项上置选中标记.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
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
    for (i=0; i < 5; i++) /*在窗口第一行第一列处输出主菜单项*/
    {
        printf("  %s  ", ga_main_menu[i]);
    }
    GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
    SetConsoleCursorInfo(gh_std_out, &lpCur);  /*隐藏光标*/
    /*申请动态存储区作为存放菜单条屏幕区字符信息的缓冲区*/
    gp_buff_menubar_info = (CHAR_INFO *)malloc(size.X * size.Y * sizeof(CHAR_INFO));
    SMALL_RECT rcMenu ={0, 0, size.X-1, 0} ;
    /*将窗口第一行的内容读入到存放菜单条屏幕区字符信息的缓冲区中*/
    ReadConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
    /*将这一行中英文字母置为红色，其他字符单元置为白底黑字*/
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
    /*修改后的菜单条字符信息回写到窗口的第一行*/
    WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
    COORD endPos = {0, 1};
    SetConsoleCursorPosition(gh_std_out, endPos);  /*将光标位置设置在第2行第1列*/
    /*将菜单项置为热区，热区编号为菜单项号，热区类型为0(按钮型)*/
    i = 0;
    do
    {
        PosB = PosA + strlen(ga_main_menu[i]);  /*定位第i+1号菜单项的起止位置*/
        for (j=PosA; j<PosB; j++)
        {
            gp_scr_att[j] |= (i+1) << 2; /*设置菜单项所在字符单元的属性值*/
        }
        PosA = PosB + 4;    i++;
    } while (i<5);
    TagMainMenu(gi_sel_menu);  /*在选中主菜单项上做标记，gi_sel_menu初值为1*/
    return;
}

/**
 * 函数名称: ShowState
 * 函数功能: 显示状态条.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明: 状态条字符属性为白底黑字, 初始状态无状态信息.
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
 * 函数名称: TagMainMenu
 * 函数功能: 在指定主菜单项上置选中标志.
 * 输入参数: num 选中的主菜单项号
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void TagMainMenu(int num)
{
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    COORD size, pos = {0, 0};
    int PosA = 2, PosB, i;
    char ch;
    if (num == 0) /*num为0时，将会去除主菜单项选中标记*/
    {   PosA = PosB = 0;    }
    else  /*否则，定位选中主菜单项的起止位置: PosA为起始位置, PosB为截止位置*/
    {
        for (i=1; i<num; i++)
        {
            PosA += strlen(ga_main_menu[i-1]) + 4;
        }
        PosB = PosA + strlen(ga_main_menu[num-1]);
    }
    GetConsoleScreenBufferInfo( gh_std_out, &bInfo );
    size.X = bInfo.dwSize.X;    size.Y = 1;
    /*去除选中菜单项前面的菜单项选中标记*/
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
    /*在选中菜单项上做标记，黑底白字*/
    for (i=PosA; i<PosB; i++)
    {
        (gp_buff_menubar_info+i)->Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN
                                               | FOREGROUND_RED;
    }
    /*去除选中菜单项后面的菜单项选中标记*/
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
    /*将做好标记的菜单条信息写到窗口第一行*/
    SMALL_RECT rcMenu ={0, 0, size.X-1, 0};
    WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
    return;
}

/**
 * 函数名称: PopMenu
 * 函数功能: 在子菜单并设置热区, 随光标运动置选中标记.
 * 输入参数: num 子菜单从上向下的项数
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
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
    if (num != gi_sel_menu)       /*如果指定主菜单不是已选中菜单*/
    {
        if (gp_top_layer->LayerNo != 0) /*如果此前已有子菜单弹出*/
        {
            PopOff();       gi_sel_sub_menu = 0;
        }
    }
    else if (gp_top_layer->LayerNo != 0) /*若已弹出该子菜单，则返回*/
    {   return; }
    gi_sel_menu = num;    /*将选中主菜单项置为指定的主菜单项*/
    TagMainMenu(gi_sel_menu); /*在选中的主菜单项上做标记*/
    LocSubMenu(gi_sel_menu, &rcPop); /*计算弹出子菜单的区域位置, 存放在rcPop中*/
    /*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/
    for (i=1; i<gi_sel_menu; i++)
    {
        loc += ga_sub_menu_count[i-1];
    }
    /*将该组子菜单项项名存入标签束结构变量*/
    labels.ppLabel = ga_sub_menu + loc;   /*标签束第一个标签字符串的地址*/
    labels.num = ga_sub_menu_count[gi_sel_menu-1]; /*标签束中标签字符串的个数*/
    COORD aLoc[labels.num];/*定义一个坐标数组，存放每个标签字符串输出位置的坐标*/
    for (i=0; i<labels.num; i++) /*确定标签字符串的输出位置，存放在坐标数组中*/
    {
        aLoc[i].X = rcPop.Left + 2;     aLoc[i].Y = rcPop.Top + i + 1;
    }
    labels.pLoc = aLoc; /*使标签束结构变量labels的成员pLoc指向坐标数组的首元素*/
    /*设置热区信息*/
    areas.num = labels.num;       /*热区的个数，等于标签的个数，即子菜单的项数*/
    SMALL_RECT aArea[areas.num];                    /*定义数组存放所有热区位置*/
    char aSort[areas.num];                      /*定义数组存放所有热区对应类别*/
    char aTag[areas.num];                         /*定义数组存放每个热区的编号*/
    for (i=0; i<areas.num; i++)
    {
        aArea[i].Left = rcPop.Left + 2;  /*热区定位*/
        aArea[i].Top = rcPop.Top + i + 1;
        aArea[i].Right = rcPop.Right - 2;
        aArea[i].Bottom = aArea[i].Top;
        aSort[i] = 0;       /*热区类别都为0(按钮型)*/
        aTag[i] = i + 1;           /*热区按顺序编号*/
    }
    areas.pArea = aArea;/*使热区结构变量areas的成员pArea指向热区位置数组首元素*/
    areas.pSort = aSort;/*使热区结构变量areas的成员pSort指向热区类别数组首元素*/
    areas.pTag = aTag;   /*使热区结构变量areas的成员pTag指向热区编号数组首元素*/
    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    PopUp(&rcPop, att, &labels, &areas);
    DrawBox(&rcPop);  /*给弹出窗口画边框*/
    pos.X = rcPop.Left + 2;
    for (pos.Y=rcPop.Top+1; pos.Y<rcPop.Bottom; pos.Y++)
    { /*此循环用来在空串子菜项位置画线形成分隔，并取消此菜单项的热区属性*/
        pCh = ga_sub_menu[loc+pos.Y-rcPop.Top-1];
        if (strlen(pCh)==0) /*串长为0，表明为空串*/
        {   /*首先画横线*/
            FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-3, pos, &ul);
            for (j=rcPop.Left+2; j<rcPop.Right-1; j++)
            {   /*取消该区域字符单元的热区属性*/
                gp_scr_att[pos.Y*SCR_COL+j] &= 3; /*按位与的结果保留了低两位*/
            }
        }
    }
    /*将子菜单项的功能键设为白底红字*/
    pos.X = rcPop.Left + 3;
    att =  FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
    for (pos.Y=rcPop.Top+1; pos.Y<rcPop.Bottom; pos.Y++)
    {
        if (strlen(ga_sub_menu[loc+pos.Y-rcPop.Top-1])==0)
        {   continue;  /*跳过空串*/     }
        FillConsoleOutputAttribute(gh_std_out, att, 1, pos, &ul);
    }
    return;
}

/**
 * 函数名称: PopUp
 * 函数功能: 在指定区域输出弹出窗口信息, 同时设置热区, 将弹出窗口位置信息入栈.
 * 输入参数: pRc 弹出窗口位置数据存放的地址
 *           att 弹出窗口区域字符属性
 *           pLabel 弹出窗口中标签束信息存放的地址
             pHotArea 弹出窗口中热区信息存放的地址
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void PopUp(SMALL_RECT *pRc, WORD att, LABEL_BUNDLE *pLabel, HOT_AREA *pHotArea)
{
    LAYER_NODE *nextLayer;
    COORD size, pos = {0, 0};
    char *pCh;
    int i, j, row;
    /*弹出窗口所在位置字符单元信息入栈*/
    size.X = pRc->Right - pRc->Left + 1;    /*弹出窗口的宽度*/
    size.Y = pRc->Bottom - pRc->Top + 1;    /*弹出窗口的高度*/
    /*申请存放弹出窗口相关信息的动态存储区*/
    nextLayer = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));
    nextLayer->next = gp_top_layer;
    nextLayer->LayerNo = gp_top_layer->LayerNo + 1;
    nextLayer->rcArea = *pRc;
    nextLayer->pContent = (CHAR_INFO *)malloc(size.X*size.Y*sizeof(CHAR_INFO));
    nextLayer->pScrAtt = (char *)malloc(size.X*size.Y*sizeof(char));
    pCh = nextLayer->pScrAtt;
    /*将弹出窗口覆盖区域的字符信息保存，用于在关闭弹出窗口时恢复原样*/
    ReadConsoleOutput(gh_std_out, nextLayer->pContent, size, pos, pRc);
    for (i=pRc->Top; i<=pRc->Bottom; i++)
    {   /*此二重循环将所覆盖字符单元的原先属性值存入动态存储区，便于以后恢复*/
        for (j=pRc->Left; j<=pRc->Right; j++)
        {
            *pCh = gp_scr_att[i*SCR_COL+j];     pCh++;
        }
    }
    gp_top_layer = nextLayer;  /*完成弹出窗口相关信息入栈操作*/
    /*设置弹出窗口区域字符的新属性*/
    pos.X = pRc->Left;
    pos.Y = pRc->Top;
    for (i=pRc->Top; i<=pRc->Bottom; i++)
    {
        FillConsoleOutputAttribute(gh_std_out, att, size.X, pos, &ul);
        pos.Y++;
    }
    /*将标签束中的标签字符串在设定的位置输出*/
    for (i=0; i<pLabel->num; i++)
    {
        pCh = pLabel->ppLabel[i];
        if (strlen(pCh) != 0)
        {
            WriteConsoleOutputCharacter(gh_std_out, pCh, strlen(pCh),
                                        pLabel->pLoc[i], &ul);
        }
    }
    /*设置弹出窗口区域字符单元的新属性*/
    for (i=pRc->Top; i<=pRc->Bottom; i++)
    {   /*此二重循环设置字符单元的层号*/
        for (j=pRc->Left; j<=pRc->Right; j++)
        {
            gp_scr_att[i*SCR_COL+j] = gp_top_layer->LayerNo;
        }
    }

    for (i=0; i<pHotArea->num; i++)
    {   /*此二重循环设置所有热区中字符单元的热区类型和热区编号*/
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
 * 函数名称: PopOff
 * 函数功能: 关闭顶层弹出窗口, 恢复覆盖区域原外观和字符单元原属性.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void PopOff(void)
{
    LAYER_NODE *nextLayer;
    COORD size, pos = {0, 0};
    char *pCh;
    int i, j;
    if ((gp_top_layer->next==NULL) || (gp_top_layer->pContent==NULL))
    {   /*栈底存放的主界面屏幕信息，不用关闭*/
        return;
    }
    nextLayer = gp_top_layer->next;
    /*恢复弹出窗口区域原外观*/
    size.X = gp_top_layer->rcArea.Right - gp_top_layer->rcArea.Left + 1;
    size.Y = gp_top_layer->rcArea.Bottom - gp_top_layer->rcArea.Top + 1;
    WriteConsoleOutput(gh_std_out, gp_top_layer->pContent, size, pos, &(gp_top_layer->rcArea));
    /*恢复字符单元原属性*/
    pCh = gp_top_layer->pScrAtt;
    for (i=gp_top_layer->rcArea.Top; i<=gp_top_layer->rcArea.Bottom; i++)
    {
        for (j=gp_top_layer->rcArea.Left; j<=gp_top_layer->rcArea.Right; j++)
        {
            gp_scr_att[i*SCR_COL+j] = *pCh;     pCh++;
        }
    }
    free(gp_top_layer->pContent);    /*释放动态存储区*/
    free(gp_top_layer->pScrAtt);
    free(gp_top_layer);
    gp_top_layer = nextLayer;
    gi_sel_sub_menu = 0;
    return;
}

/**
 * 函数名称: DrawBox
 * 函数功能: 在指定区域画边框.
 * 输入参数: pRc 存放区域位置信息的地址
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void DrawBox(SMALL_RECT *pRc)
{
    char chBox[] = {'+','-','|'};  /*画框用的字符*/
    COORD pos = {pRc->Left, pRc->Top};  /*定位在区域的左上角*/

    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*画边框左上角*/
    for (pos.X = pRc->Left + 1; pos.X < pRc->Right; pos.X++)
    {   /*此循环画上边框横线*/
        WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &ul);
    }
    pos.X = pRc->Right;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*画边框右上角*/
    for (pos.Y = pRc->Top+1; pos.Y < pRc->Bottom; pos.Y++)
    {   /*此循环画边框左边线和右边线*/
        pos.X = pRc->Left;
        WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &ul);
        pos.X = pRc->Right;
        WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &ul);
    }
    pos.X = pRc->Left;      pos.Y = pRc->Bottom;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*画边框左下角*/
    for (pos.X = pRc->Left + 1; pos.X < pRc->Right; pos.X++)
    {   /*画下边框横线*/
        WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &ul);
    }
    pos.X = pRc->Right;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*画边框右下角*/
    return;
}

/**
 * 函数名称: TagSubMenu
 * 函数功能: 在指定子菜单项上做选中标记.
 * 输入参数: num 选中的子菜单项号
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void TagSubMenu(int num)
{
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    int width;
    LocSubMenu(gi_sel_menu, &rcPop);  /*计算弹出子菜单的区域位置, 存放在rcPop中*/
    if ((num<1) || (num == gi_sel_sub_menu) || (num>rcPop.Bottom-rcPop.Top-1))
    {   /*如果子菜单项号越界，或该项子菜单已被选中，则返回*/
        return;
    }
    pos.X = rcPop.Left + 2;
    width = rcPop.Right - rcPop.Left - 3;
    if (gi_sel_sub_menu != 0) /*首先取消原选中子菜单项上的标记*/
    {
        pos.Y = rcPop.Top + gi_sel_sub_menu;
        att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
        FillConsoleOutputAttribute(gh_std_out, att, width, pos, &ul);
        pos.X += 1;
        att |=  FOREGROUND_RED;/*白底红字*/
        FillConsoleOutputAttribute(gh_std_out, att, 1, pos, &ul);
    }
    /*在制定子菜单项上做选中标记*/
    pos.X = rcPop.Left + 2;     pos.Y = rcPop.Top + num;
    att = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  /*黑底白字*/
    FillConsoleOutputAttribute(gh_std_out, att, width, pos, &ul);
    gi_sel_sub_menu = num;  /*修改选中子菜单项号*/
    return;
}

/**
 * 函数名称: LocSubMenu
 * 函数功能: 计算弹出子菜单区域左上角和右下角的位置.
 * 输入参数: num 选中的主菜单项号
 * 输出参数: rc 存放区域位置信息的地址
 * 返 回 值: 无
 *
 * 调用说明:
 */
void LocSubMenu(int num, SMALL_RECT *rc)
{
    int i, len, loc = 0;
    rc->Top = 1; /*区域的上边定在第2行，行号为1*/
    rc->Left = 1;
    for (i=1; i<num; i++)
    {   /*计算区域左边界位置, 同时计算第一个子菜单项在子菜单字符串数组中的位置*/
        rc->Left += strlen(ga_main_menu[i-1]) + 4;
        loc += ga_sub_menu_count[i-1];
    }
    rc->Right = strlen(ga_sub_menu[loc]);/*暂时存放第一个子菜单项字符串长度*/
    for (i=1; i<ga_sub_menu_count[num-1]; i++)
    {   /*查找最长子菜单字符串，将其长度存放在rc->Right*/
        len = strlen(ga_sub_menu[loc+i]);
        if (rc->Right < len)
        {   rc->Right = len;    }
    }
    rc->Right += rc->Left + 3;  /*计算区域的右边界*/
    rc->Bottom = rc->Top + ga_sub_menu_count[num-1] + 1;/*计算区域下边的行号*/
    if (rc->Right >= SCR_COL)  /*右边界越界的处理*/
    {
        len = rc->Right - SCR_COL + 1;
        rc->Left -= len;
        rc->Right = SCR_COL - 1;
    }
    return;
}

/**
 * 函数名称: DealInput
 * 函数功能: 在弹出窗口区域设置热区, 等待并响应用户输入.
 * 输入参数: pHotArea   热区参数指针
 *           piHot 焦点热区编号的存放地址, 即指向焦点热区编号的指针
 * 输出参数: pHotArea   热区参数
 *            piHot 用鼠标单击、按回车或空格时返回当前热区编号
 * 返 回 值: iRet 回车虚拟键码
 *
 * 调用说明:
 */
int DealInput(HOT_AREA *pHotArea, int *piHot)
{
    INPUT_RECORD inRec;
    DWORD res;
    COORD pos = {0, 0};
    int num, arrow, iRet = 0;
    int cNo, cTag, cSort;/*cNo:层号, cTag:热区编号, cSort: 热区类型*/
    char vkc, asc;       /*vkc:虚拟键代码, asc:字符的ASCII码值*/
    SetHotPoint(pHotArea, *piHot);
    while (TRUE)
    {    /*循环*/
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
                {  /*方向键(左、上、右、下)的处理*/
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
            {  /*ESC键*/     iRet = 27;      break;  }
            else if (vkc == 13 || vkc == 32)
            {  /*回车键或空格表示按下当前按钮*/   iRet = 13;      break;  }
        }
    }
    return iRet;
}

/**
 * 函数名称: SetHotPoint
 * 函数功能: 在弹出窗口区域设置热区, 等待并响应用户输入.
 * 输入参数: pHotArea   热区参数指针
 *            iHot 焦点热区编号
 * 输出参数: pHotArea   热区参数
 * 返 回 值: 无
 *
 * 调用说明:
 */
void SetHotPoint(HOT_AREA *pHotArea, int iHot)
{
    CONSOLE_CURSOR_INFO lpCur;
    COORD pos = {0, 0};
    WORD att1, att2;
    int i, width;
    att1 = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  /*黑底白字*/
    att2 = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    for (i=0; i<pHotArea->num; i++)
    {  /*将按钮类热区置为白底黑字*/
        pos.X = pHotArea->pArea[i].Left;
        pos.Y = pHotArea->pArea[i].Top;
        width = pHotArea->pArea[i].Right - pHotArea->pArea[i].Left + 1;
        if (pHotArea->pSort[i] == 0)
        {  /*热区是按钮类*/
            FillConsoleOutputAttribute(gh_std_out, att2, width, pos, &ul);
        }
    }
    pos.X = pHotArea->pArea[iHot-1].Left;
    pos.Y = pHotArea->pArea[iHot-1].Top;
    width = pHotArea->pArea[iHot-1].Right - pHotArea->pArea[iHot-1].Left + 1;
    if (pHotArea->pSort[iHot-1] == 0)
    {  /*被激活热区是按钮类*/
        FillConsoleOutputAttribute(gh_std_out, att1, width, pos, &ul);
    }
    else if (pHotArea->pSort[iHot-1] == 1)
    {  /*被激活热区是文本框类*/
        SetConsoleCursorPosition(gh_std_out, pos);
        GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = TRUE;
        SetConsoleCursorInfo(gh_std_out, &lpCur);
    }
}

/**
 * 函数名称: ShowModule
 * 函数功能: 从指定数据文件中恢复系统代码表和三类基础数据.
 * 输入参数: pString 文字信息二维数组首地址
 *           n 字符串数量
 * 输出参数:
 * 返 回 值: BOOL类型, 总是为TRUE
 *
 * 调用说明:
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
    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
 * 函数名称: Message
 * 函数功能: 弹出对话框
 * 输入参数: **pCh 对话框信息数组首地址
 * 输出参数: 无
 * 返 回 值: BOOL类型，退出为TRUE，取消为FALSE
 *
 * 调用说明:
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
    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
    judge=MessageBoxA(NULL,"确认退出系统吗？","系统提示",MB_YESNO);
    if(judge==IDYES){
        bRet = FALSE;
    }
    else{
        bRet = TRUE;
    }
    */

/**< 第二部分 系统骨架函数 */

/**
 * 函数名称: LoadData
 * 函数功能: 将代码表和三类基础数据从数据文件载入到内存缓冲区和十字链表中.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型, 功能函数中除了函数ExitSys的返回值可以为FALSE外,
 *           其他函数的返回值必须为TRUE.
 *
 * 调用说明: 为了能够以统一的方式调用各功能函数, 将这些功能函数的原型设为
 *           一致, 即无参数且返回值为BOOL. 返回值为FALSE时, 结束程序运行.
 */

BOOL LoadData()
{
    int Re = 0;
    Re = CreatList();
    gc_sys_state |= Re;
    gc_sys_state &= ~(1 + 2 + 4 - Re);
    if (gc_sys_state < (1 | 2 | 4 )){
        printf("\n系统基础数据不完整!\n");
        system("pause");
    }
    return TRUE;
}

/**
 * 函数名称: CreatList
 * 函数功能: 从数据文件读取基础数据, 并存放到所创建的十字链表中.
 * 输入参数: 无
 * 输出参数: head 主链头指针的地址, 用来返回所创建的十字链.
 * 返 回 值: int型数值, 表示链表创建的情况.
 *           1 已加载院系信息数据，无科研团队基本信息和科研项目数据
 *           3 已加载院系信息和科研团队基本信息数据，无科研项目数据
 *           7 三类基础数据都已加载
 *
 * 调用说明:
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
        printf("院系信息数据文件打开失败!\n");
        return re;
    }
    printf("院系信息数据文件打开成功!\n");
    /*从数据文件中读宿舍楼信息数据，存入以后进先出方式建立的主链中*/
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
        printf("院系信息数据文件加载失败!\n");
        return re;
    }
    printf("院系信息数据文件加载成功!\n");
    sch_head = head;
    re += 1;
    if ((pFile = fopen("team.dat", "rb+")) == NULL)
    {
        printf("科研团队基本信息数据文件打开失败!\n");
        return re;
    }
    printf("科研团队基本信息数据文件打开成功!\n");
    re += 2;
    /*从数据文件中读取学生基本信息数据，存入主链对应结点的学生基本信息支链中*/
    while (fread(&tmp2, sizeof(TEAM_NODE), 1, pFile) == 1)
    {
        /*创建结点，存放从数据文件中读出的学生基本信息*/
        q = (TEAM_NODE *)malloc(sizeof(TEAM_NODE));
        *q = tmp2;
        q->proj_head = NULL;
        /*在主链上查找该学生所住宿舍楼对应的主链结点*/
        p = head;
        while (p != NULL&& strcmp(p->school_name, q->school_name))
        {
            p = p->next;
        }
        if (p != NULL) /*如果找到，则将结点以后进先出方式插入学生信息支链*/
        {
            q->next = p->team_head;
            p->team_head = q;
        }
        else  /*如果未找到，则释放所创建结点的内存空间*/
        {
            free(q);
        }
    }
    fclose(pFile);
    if ((pFile = fopen("project.dat", "rb")) == NULL)
    {
        printf("科研团队信息数据文件打开失败!\n");
        return re;
    }
    printf("科研团队信息数据文件打开成功!\n");
    re += 4;
    /*从数据文件中读取学生缴费信息数据，存入学生基本信息支链对应结点的缴费支链中*/
    while (fread(&tmp3, sizeof(PROJECT_NODE), 1, pFile) == 1)
    {
        /*创建结点，存放从数据文件中读出的学生缴费信息*/
        r = (PROJECT_NODE *)malloc(sizeof(PROJECT_NODE));
        *r = tmp3;
        /*查找学生信息支链上对应学生信息结点*/
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
        if (find)  /*如果找到，则将结点以后进先出方式插入学生缴费信息支链中*/
        {
            r->next = q->proj_head;
            q->proj_head = r;
        }
        else /*如果未找到，则释放所创建结点的内存空间*/
        {   free(r);    }
    }
    fclose(pFile);
    printf("按任意键继续...\n"); getch();
    return re;
}

/**
 * 函数名称: RunSys
 * 函数功能: 运行系统, 在系统主界面下运行用户所选择的功能模块.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void RunSys(void)
{
    INPUT_RECORD inRec;
    DWORD res;
    COORD pos = {0, 0};
    BOOL bRet = TRUE;
    int i, loc, num;
    int cNo, cAtt;      /*cNo:字符单元层号, cAtt:字符单元属性*/
    char vkc, asc;      /*vkc:虚拟键代码, asc:字符的ASCII码值*/
    while (bRet)
    {
        /*从控制台输入缓冲区中读一条记录*/
        ReadConsoleInput(gh_std_in, &inRec, 1, &res);
        if (inRec.EventType == MOUSE_EVENT) /*如果记录由鼠标事件产生*/
        {
            pos = inRec.Event.MouseEvent.dwMousePosition;  /*获取鼠标坐标位置*/
            cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3; /*取该位置的层号*/
            cAtt = gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2;/*取该字符单元属性*/
            if (cNo == 0) /*层号为0，表明该位置未被弹出子菜单覆盖*/
            {
                /* cAtt > 0 表明该位置处于热区(主菜单项字符单元)
                 * cAtt != gi_sel_menu 表明该位置的主菜单项未被选中
                 * gp_top_layer->LayerNo > 0 表明当前有子菜单弹出
                 */
                if (cAtt > 0 && cAtt != gi_sel_menu && gp_top_layer->LayerNo > 0)
                {
                    PopOff();            /*关闭弹出的子菜单*/
                    gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
                    PopMenu(cAtt);       /*弹出鼠标所在主菜单项对应的子菜单*/
                }
            }
            else if (cAtt > 0) /*鼠标所在位置为弹出子菜单的菜单项字符单元*/
            {
                TagSubMenu(cAtt); /*在该子菜单项上做选中标记*/
            }

            if (inRec.Event.MouseEvent.dwButtonState
                == FROM_LEFT_1ST_BUTTON_PRESSED) /*如果按下鼠标左边第一键*/
            {
                if (cNo == 0) /*层号为0，表明该位置未被弹出子菜单覆盖*/
                {
                    if (cAtt > 0) /*如果该位置处于热区(主菜单项字符单元)*/
                    {
                        PopMenu(cAtt);   /*弹出鼠标所在主菜单项对应的子菜单*/
                    }
                    /*如果该位置不属于主菜单项字符单元，且有子菜单弹出*/
                    else if (gp_top_layer->LayerNo > 0)
                    {
                        PopOff();            /*关闭弹出的子菜单*/
                        gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
                    }
                }
                else /*层号不为0，表明该位置被弹出子菜单覆盖*/
                {
                    if (cAtt > 0) /*如果该位置处于热区(子菜单项字符单元)*/
                    {
                        PopOff(); /*关闭弹出的子菜单*/
                        gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
                        /*执行对应功能函数:gi_sel_menu主菜单项号,cAtt子菜单项号*/
                        bRet = ExeFunction(gi_sel_menu, cAtt);
                    }
                }
            }
            else if (inRec.Event.MouseEvent.dwButtonState
                     == RIGHTMOST_BUTTON_PRESSED) /*如果按下鼠标右键*/
            {
                if (cNo == 0) /*层号为0，表明该位置未被弹出子菜单覆盖*/
                {
                    PopOff();            /*关闭弹出的子菜单*/
                    gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
                }
            }
        }
        else if (inRec.EventType == KEY_EVENT  /*如果记录由按键产生*/
                 && inRec.Event.KeyEvent.bKeyDown) /*且键被按下*/
        {
            vkc = inRec.Event.KeyEvent.wVirtualKeyCode; /*获取按键的虚拟键码*/
            asc = inRec.Event.KeyEvent.uChar.AsciiChar; /*获取按键的ASC码*/
            /*系统快捷键的处理*/
            if (vkc == 112) /*如果按下F1键*/
            {
                if (gp_top_layer->LayerNo != 0) /*如果当前有子菜单弹出*/
                {
                    PopOff();            /*关闭弹出的子菜单*/
                    gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
                }
                bRet = ExeFunction(5, 1);  /*运行帮助主题功能函数*/
            }
            else if (inRec.Event.KeyEvent.dwControlKeyState
                     & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED))
            { /*如果按下左或右Alt键*/
                switch (vkc)  /*判断组合键Alt+字母*/
                {
                    case 88:  /*Alt+X 退出*/
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
            else if (asc == 0) /*其他控制键的处理*/
            {
                if (gp_top_layer->LayerNo == 0) /*如果未弹出子菜单*/
                {
                    switch (vkc) /*处理方向键(左、右、下)，不响应其他控制键*/
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
                else  /*已弹出子菜单时*/
                {
                    for (loc=0,i=1; i<gi_sel_menu; i++)
                    {
                        loc += ga_sub_menu_count[i-1];
                    }  /*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/
                    switch (vkc) /*方向键(左、右、上、下)的处理*/
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
            else if ((asc-vkc == 0) || (asc-vkc == 32)){  /*按下普通键*/
                if (gp_top_layer->LayerNo == 0)  /*如果未弹出子菜单*/
                {
                    switch (vkc)
                    {
                        case 70: /*f或F*/        PopMenu(1);     break;
                        case 77: /*m或M*/        PopMenu(2);     break;
                        case 81: /*q或Q*/        PopMenu(3);     break;
                        case 83: /*s或S*/        PopMenu(4);     break;
                        case 72: /*h或H*/        PopMenu(5);     break;
                        case 13: /*回车*/
                            PopMenu(gi_sel_menu);
                            TagSubMenu(1);      break;
                    }
                }
                else /*已弹出子菜单时的键盘输入处理*/
                {
                    if (vkc == 27) /*如果按下ESC键*/
                    {
                        PopOff();       gi_sel_sub_menu = 0;
                    }
                    else if(vkc == 13) /*如果按下回车键*/
                    {
                        num = gi_sel_sub_menu;
                        PopOff();       gi_sel_sub_menu = 0;
                        bRet = ExeFunction(gi_sel_menu, num);
                    }
                    else /*其他普通键的处理*/
                    {
                        /*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/
                        for (loc=0,i=1; i<gi_sel_menu; i++)
                        {
                            loc += ga_sub_menu_count[i-1];
                        }

                        /*依次与当前子菜单中每一项的代表字符进行比较*/
                        for (i=loc; i<loc+ga_sub_menu_count[gi_sel_menu-1]; i++)
                        {
                            if (strlen(ga_sub_menu[i])>0 && vkc==ga_sub_menu[i][1])
                            { /*如果匹配成功*/
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
 * 函数名称: ExeFunction
 * 函数功能: 执行由主菜单号和子菜单号确定的功能函数.
 * 输入参数: m 主菜单项号
 *           s 子菜单项号
 * 输出参数: 无
 * 返 回 值: BOOL类型, TRUE 或 FALSE
 *
 * 调用说明: 仅在执行函数ExitSys时, 才可能返回FALSE, 其他情况下总是返回TRUE
 */
BOOL ExeFunction(int m, int s)
{
    BOOL bRet = TRUE;
    /*函数指针数组，用来存放所有功能函数的入口地址*/
    BOOL (*pFunction[ga_sub_menu_count[0]+ga_sub_menu_count[1]+ga_sub_menu_count[2]+ga_sub_menu_count[3]+ga_sub_menu_count[4]])(void);
    int i, loc;
    /*将功能函数入口地址存入与功能函数所在主菜单号和子菜单号对应下标的数组元素*/
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
    for (i=1,loc=0; i<m; i++)  /*根据主菜单号和子菜单号计算对应下标*/
    {       loc += ga_sub_menu_count[i-1];      }
    loc += s - 1;
    if (pFunction[loc] != NULL)
    {
        bRet = (*pFunction[loc])();  /*用函数指针调用所指向的功能函数*/
    }
    return bRet;
}

/**
 * 函数名称: CloseSys
 * 函数功能: 关闭系统.
 * 输入参数: hd 主链头指针
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void CloseSys(void)
{
    ClearMemory();    ClearScreen();        /*清屏*/
    /*释放存放菜单条、状态条、性别代码和学生类别代码等信息动态存储区*/
    free(gp_buff_menubar_info);
    free(gp_buff_stateBar_info);
    /*关闭标准输入和输出设备句柄*/
    CloseHandle(gh_std_out);        CloseHandle(gh_std_in);
    /*将窗口标题栏置为运行结束*/
    SetConsoleTitle("运行结束");
    return;
}

/**
 * 函数名称: ExitSys
 * 函数功能: 弹出退出系统的选项
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型，退出为TRUE，取消为FALSE
 *
 * 调用说明:
 */
BOOL ExitSys(void)
{
    char *pCh[] = {"确认退出系统吗？", "确定", "取消"};
    BOOL bRet = Message(pCh);
    PopOff();
    return bRet;
}

/**
 * 函数名称: ClearMemory
 * 函数功能: 清空内存
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void ClearMemory(void){
    p1=sch_head;
    while (p1 != NULL) /*释放十字交叉链表的动态存储区*/
    {
        p2 = p1->next;        q1 = p1->team_head;
        while (q1 != NULL) /*释放学生基本信息支链的动态存储区*/
        {
            q2 = q1->next;      r1 = q1->proj_head;
            while (r1 != NULL) /*释放缴费信息支链的动态存储区*/
            {   r2 = r1->next;      free(r1);       r1 = r2;    }
            free(q1);   q1 = q2;
        }
        free(p1);  /*释放主链结点的动态存储区*/
        p1 = p2;
    }
}

BOOL SaveData(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"主菜单项：文件",
                           "子菜单项：数据保存",
                           "确认"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}

/**< 第三部分 系统功能函数 */

/**
 * 函数名称: BackupData
 * 函数功能: 将系统代码表和三类基础数据备份到一个数据文件.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型, 总是为TRUE
 *
 * 调用说明:
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
        printf("打开失败！\n");
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
    printf("备份成功！按任意键继续...\n");  getch();     ClearScreen();
    return TRUE;
}

/**
 * 函数名称: RestoreData
 * 函数功能: 从指定数据文件中恢复三类基础数据.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型, 总是为TRUE
 *
 * 调用说明:
 */
BOOL RestoreData(void)
{
//    FILE *in;
    CONSOLE_CURSOR_INFO lpCur;
    GetConsoleCursorInfo(gh_std_out,&lpCur);
    lpCur.bVisible=TRUE;
    SetConsoleCursorInfo(gh_std_out,&lpCur);
    printf("导入的数据将覆盖系统当前数据！继续——Y   返回主菜单——其它任意键\n");
    fflush(stdin);  j=getchar(); getchar();
    if(j!='y'&&j!='Y'){ ClearScreen();  return TRUE;   }
    ClearMemory();
    char input[100], *filename;
    printf("请输入您要恢复的数据所在的文件名:");
    fflush(stdin);      scanf("%s",input);
    filename=input;
    SCHOOL_NODE *hd=NULL,*psch_node;
    TEAM_NODE *pteam_node;
    PROJECT_NODE *psciprj_node;
    unsigned short sch_node_num=0, team_node_num=0, sciprj_node_num=0, loop;
    int handle,find;
    if( (handle=open(filename,O_RDONLY|O_BINARY)) == -1)
    {
        char *plabel_name[] = {"您输入的备份文件不存在！",
                            "数据恢复失败，将自动载入最后一次更新的数据",
                            "确认"
                                };
        ShowModule(plabel_name, 3);
        CreatList();    ClearScreen();
        return TRUE;
    }
    /*读取三种基本信息的记录条数*/
    read(handle, (char*)&sch_node_num, sizeof(sch_node_num));
    read(handle, (char*)&team_node_num, sizeof(team_node_num));
    read(handle, (char*)&sciprj_node_num, sizeof(sciprj_node_num));
    /*读取院系基本信息，建立十字链表*/
    for(loop=1; loop<=sch_node_num; loop++)
    {
        psch_node=(SCHOOL_NODE*)malloc(sizeof(SCHOOL_NODE));
        read(handle, (char*)psch_node, sizeof(SCHOOL_NODE));
        psch_node->team_head=NULL;//不能悬挂指针
        psch_node->next=hd;
        hd=psch_node;
    }
    sch_head=hd;
    /*读取科研团队基本信息，建立科研团队信息支链*/
    for(loop=1; loop<=team_node_num; loop++)
    {
        pteam_node=(TEAM_NODE*)malloc(sizeof(TEAM_NODE));
        read(handle, (char*)pteam_node, sizeof(TEAM_NODE));
        pteam_node->proj_head=NULL;//不能悬挂 指针
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
     /*读取科研项目基本信息，建立科研项目基本信息支链*/
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
     printf("按任意键继续...\n"); getch(); ClearScreen();
     GetConsoleCursorInfo(gh_std_out,&lpCur);
     lpCur.bVisible=FALSE;
     SetConsoleCursorInfo(gh_std_out,&lpCur);
     return TRUE;
}

/**
 * 函数名称: FileIn
 * 函数功能: 从指定数据文件中导入三类基础数据（备用）.
 * 输入参数: 无
 * 输出参数: sch_head 十字链表首地址
 * 返 回 值: BOOL类型, 总是为TRUE
 *
 * 调用说明:
 */
BOOL FileIn(void)
{
    FILE *in;
    char find=0;
    SCHOOL_NODE *head = NULL;
    /*录入院系数据*/
    if ((in = fopen("school.dat", "rb+")) == NULL)
    {
        printf("school.dat打开失败!\n");
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
    /*指针倒回，录入团队信息并在各院系下归类*/
    sch_head = head;
    if ((in = fopen("team.dat", "rb+")) == NULL)
    {
        printf("team.dat打开失败!\n");
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
    /*指针倒回，录入项目信息并在各团队下归类*/
    if ((in = fopen("project.dat", "rb+")) == NULL)
    {
        printf("project.dat打开失败!\n");
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
    printf("数据读取完毕！\n");    system("pause");
    return TRUE;
}

/**
 * 函数名称: FileOut
 * 函数功能: 导出系统数据到指定数据文件与文本文件（备用）.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型, 总是为TRUE
 *
 * 调用说明:
 */
BOOL FileOut(void)     /*数据同时存入二进制文件与文本文件，其中二进制文件作为下一次进入系统生成链表的依据*/
{
    FILE *out;
    /*写txt文件，院系信息-团队信息-项目信息为三层缩进格式，便于阅读*/
    if((out=fopen("output.txt","w+"))==NULL){
        printf("output.txt打开失败！");
        exit(-1);
    }
    for(p=sch_head;p!=NULL;p=p->next){
        fprintf(out,"院系名称：\t\t%s\n负责人：\t\t%s\n联系电话\t\t%s\n",
                p->school_name,   p->leader,      p->contact_num);
        for(q=p->team_head;q!=NULL;q=q->next){
            fprintf(out,"\t团队名称：\t\t%s\n\t负责人：\t\t%s\n\t教师人数\t\t%d\n\t研究生人数\t\t%d\n",
                    q->team_name,        q->leader,        q->teacher_num,   q->stu_num);
            for(r=q->proj_head;r!=NULL;r=r->next){
                fprintf(out,"\t\t项目编号：\t\t%s\n\t\t项目类别：\t\t",r->project_num);
                /*项目类型编号还原为名称,便于查看*/
                switch(r->type){
                    case '1':   fprintf(out,"973计划项目\n");  break;
                    case '2':   fprintf(out,"国家自然科学基金项目\n");  break;
                    case '3':   fprintf(out,"863计划项目\n");  break;
                    case '4':   fprintf(out,"国际合作项目\n");  break;
                    case '5':   fprintf(out,"横向项目\n");  break;
                }
                fprintf(out,"\t\t起始时间：\t\t%s\n\t\t项目经费：\t\t%f万元\n\t\t负责人：\t\t%s\n",
                        r->open_time,      r->budget,            r->leader);
            }
        }
    }
    fclose(out);
    /*写院系dat文件*/
    if((out=fopen("school.dat","wb+"))==NULL){
        printf("school.dat打开失败！");      exit(-1);
    }
    for(p=sch_head;p!=NULL;p=p->next){
        fwrite(p,sizeof(SCHOOL_NODE),1,out);
    }
    fclose(out);
    /*写团队dat文件*/
    if((out=fopen("team.dat","wb+"))==NULL){
        printf("team.dat打开失败！");        exit(-1);
    }
    for(p=sch_head;p!=NULL;p=p->next){
        for(q=p->team_head;q!=NULL;q=q->next){
            fwrite(q,sizeof(TEAM_NODE),1,out);
        }
    }
    fclose(out);
    /*写项目dat文件*/
    if((out=fopen("project.dat","wb+"))==NULL){
        printf("project.dat打开失败！");     exit(-1);
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
 * 函数名称: Print
 * 函数功能: 界面内显示十字链表全部信息（备用）.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型, 总是为TRUE
 *
 * 调用说明:
 */
BOOL Print(void)
{
    /*写stdout，院系信息-团队信息-项目信息为三层缩进格式，便于阅读*/
    for(p=sch_head;p!=NULL;p=p->next){
        printf("院系名称：\t%s\n负责人：\t%s\n联系电话\t%s\n",
                p->school_name,   p->leader,      p->contact_num);
        for(q=p->team_head;q!=NULL;q=q->next){
            printf("\t团队名称：\t%s\n\t负责人：\t%s\n\t教师人数\t%d\n\t研究生人数\t%d\n",
                    q->team_name,        q->leader,        q->teacher_num,   q->stu_num);
            for(r=q->proj_head;r!=NULL;r=r->next){
                printf("\t\t项目编号：\t%s\n\t\t项目类别：\t",r->project_num);
                switch(r->type){
                    case '1':   printf("973计划项目\n");  break;
                    case '2':   printf("国家自然科学基金项目\n");  break;
                    case '3':   printf("863计划项目\n");  break;
                    case '4':   printf("国际合作项目\n");  break;
                    case '5':   printf("横向项目\n");  break;
                }
                printf("\t\t起始时间：\t%s\n\t\t项目经费：\t%f万元\n\t负责人：\t%s\n",
                        r->open_time,      r->budget,            r->leader);
            }
        }
    }
    return TRUE;
}

/**
 * 函数名称: MaintainSchoolInfo
 * 函数功能: 院系信息维护目录
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型, 总是为TRUE
 *
 * 调用说明:
 */
BOOL MaintainSchoolInfo(void)
{
    BOOL bRet = TRUE;
    do{
        printf("您想进行何种操作？\n");
        printf("录入          press1\n");
        printf("修改          press2\n");
        printf("删除          press3\n");
        printf("返回主菜单    press4\n");
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
 * 函数名称: MaintainTeamInfo
 * 函数功能: 科研团队信息维护目录
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型, 总是为TRUE
 *
 * 调用说明:
 */
BOOL MaintainTeamInfo(void)
{
    BOOL bRet = TRUE;
    do{
        printf("您想进行何种操作？\n");
        printf("录入          press1\n");
        printf("修改          press2\n");
        printf("删除          press3\n");
        printf("返回主菜单    press4\n");
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
 * 函数名称: MaintainProjectInfo
 * 函数功能: 科研项目信息维护目录
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型, 总是为TRUE
 *
 * 调用说明:
 */
BOOL MaintainProjectInfo(void)
{
    BOOL bRet = TRUE;
    do{
        printf("您想进行何种操作？\n");
        printf("录入          press1\n");
        printf("修改          press2\n");
        printf("删除          press3\n");
        printf("返回主菜单    press4\n");
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
 * 函数名称: AddSchool
 * 函数功能: 在十字链表中插入一个院系信息结点.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型-TRUE
 *
 * 调用说明:
 */
BOOL AddSchool(void)
{
    while(1){
        ClearScreen();  fflush(stdin);
        p = (SCHOOL_NODE *)malloc(sizeof(SCHOOL_NODE));
        /*p-数据成员录入开始*/
        while(1){
            flag = 0;
            printf("输入院系名称（注意区分大小写）：");
            scanf("%s", p->school_name);    getchar();
            for(p1=sch_head;p1!=NULL;p1=p1->next){
                if(!strcmp(p1->school_name,p->school_name)){
                    printf("此院系已存在!返回主菜单？确认--Y  取消--其它任意键\n");
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
        printf("输入院系负责人姓名：");
        scanf("%s", p->leader);         getchar();
        printf("输入联系电话：");
        scanf("%s", p->contact_num);    getchar();
        p->team_head=NULL;
        /*p-数据成员录入结束*/
        p->next=sch_head;       sch_head=p;
        printf("继续——Y   返回主菜单——其它任意键\n");
        j=getchar(); getchar();        FileOut();
        if(j!='Y'&&j!='y'){ break;  }
    }
    ClearScreen();
    return TRUE;
}

/**
 * 函数名称: AddTeam
 * 函数功能: 在十字链表中插入一个科研团队信息结点.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型-TRUE
 *
 * 调用说明:
 */
BOOL AddTeam(void)
{
    while(1){
        ClearScreen();  fflush(stdin);      flag=0;
        q=(TEAM_NODE*)malloc(sizeof(TEAM_NODE));
        while(1){
            flag = 0;
            printf("输入团队名称：");
            scanf("%s", q->team_name);    getchar();
            for(p1=sch_head;p1!=NULL;p1=p1->next){
                for(q1=p1->team_head;q1!=NULL;q1=q1->next){
                    if(!strcmp(q1->team_name,q->team_name)){
                        printf("此团队已存在!返回主菜单？确认--Y  取消--其它任意键\n");
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
        printf("输入团队所处院系（注意区分大小写）：");
        scanf("%s", q->school_name);    getchar();
        for(p=sch_head;p!=NULL;p=p->next){
            /*若找到所在院系，退出并保留当前p位置*/
            if(!strcmp(p->school_name,q->school_name)){    flag=1;    break;    }
        }
        /*若团队在新院系中，依输入的院系名创建新院系并创建相应团队*/
        if(!flag){
            p = (SCHOOL_NODE *)malloc(sizeof(SCHOOL_NODE));
            /*p-数据成员录入开始*/
            printf("输入院系负责人姓名：");
            scanf("%s", p->leader);         getchar();
            printf("输入联系电话：");
            scanf("%s", p->contact_num);    getchar();
            strcpy(p->school_name,q->school_name);
            p->team_head=NULL;
            /*p-数据成员录入结束*/
            p->next=sch_head;       sch_head=p;
        }
        /*q-数据成员录入开始*/
        printf("输入团队负责人姓名：");
        scanf("%s", q->leader);    getchar();
        printf("输入教师人数：");
        scanf("%d", &q->teacher_num);    getchar();
        printf("输入研究生人数：");
        scanf("%d", &q->stu_num);    getchar();
        q->proj_head=NULL;
        /*q-数据成员录入结束*/
        q->next=p->team_head;       p->team_head=q;
        printf("继续——Y   返回主菜单——其它任意键\n");
        j=getchar(); getchar();          FileOut();
        if(j!='Y'&&j!='y'){ break;  }
    }
    ClearScreen();
    return TRUE;
}

/**
 * 函数名称: AddProject
 * 函数功能: 在十字链表中插入一个科研项目信息结点.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型-TRUE
 *
 * 调用说明:
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
            printf("输入项目编号：");
            scanf("%s", r->project_num);    getchar();
            for(p1=sch_head;p1!=NULL;p1=p1->next){
                for(q1=p1->team_head;q1!=NULL;q1=q1->next){
                    for(r1=q1->proj_head;r1!=NULL;r1=r1->next){
                        if(!strcmp(r1->project_num,r->project_num)){
                            printf("此项目已存在!返回主菜单？确认--Y  取消--其它任意键\n");
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
        printf("输入项目所处院系（注意区分大小写）：");
        scanf("%s", school_name);    getchar();
        printf("输入项目所在团队名称：");
        scanf("%s", r->team_name);    getchar();
        for(p=sch_head;p!=NULL;p=p->next){
            /*若找到所在院系，退出并保留当前p位置*/
            if(!strcmp(p->school_name,school_name)){    flag=1;    break;    }
        }
        /*若项目挂靠在新院系下，依输入的院系名创建新院系并创建相应院系与团队*/
        if(!flag){
            p = (SCHOOL_NODE *)malloc(sizeof(SCHOOL_NODE));
            /*p-数据成员录入开始*/
            printf("输入院系负责人姓名：");
            scanf("%s", p->leader);         getchar();
            printf("输入联系电话：");
            scanf("%s", p->contact_num);    getchar();
            strcpy(p->school_name,school_name);
            /*p-数据成员录入结束*/
            p->team_head=NULL;
            p->next=sch_head;       sch_head=p;
            q=(TEAM_NODE*)malloc(sizeof(TEAM_NODE));
            /*q-数据成员录入开始*/
            printf("输入团队负责人姓名：");
            scanf("%s", q->leader);         getchar();
            printf("输入教师人数：");
            scanf("%d", &q->teacher_num);    getchar();
            printf("输入研究生人数：");
            scanf("%d", &q->stu_num);    getchar();
            strcpy(q->team_name,r->team_name);
            strcpy(q->school_name,school_name);
            /*q-数据成员录入结束*/
            q->proj_head=NULL;
            q->next=p->team_head;   p->team_head=q;
        }
        else{
            flag=0;
            for(q=p->team_head;q!=NULL;q=q->next){
                /*若找到所在团队，退出并保留当前p位置*/
                if(!strcmp(q->team_name,r->team_name)){    flag=1;    break;    }
            }
            /*若项目挂靠在新团队下，依输入的团队名创建新团队*/
            if(!flag){
                q=(TEAM_NODE*)malloc(sizeof(TEAM_NODE));
                /*q-数据成员录入开始*/
                printf("输入团队负责人姓名：");
                scanf("%s", q->leader);    getchar();
                printf("输入教师人数：");
                scanf("%d", &q->teacher_num);    getchar();
                printf("输入研究生人数：");
                scanf("%d", &q->stu_num);    getchar();
                strcpy(q->team_name,r->team_name);
                strcpy(q->school_name,p->school_name);
                /*q-数据成员录入结束*/
                q->proj_head=NULL;
                q->next=p->team_head;   p->team_head=q;
            }
        }
        /*r-数据成员录入开始*/
        printf("973计划项目--1\t\t国家自然科学基金项目--2\t\t863计划项目--3\n国际合作项目--4\t\t横向项目--5\n输入项目类别：");
        r->type=getchar();    getchar();
        printf("输入起始时间（年份占四位，不足的用前置0补齐）：");
        scanf("%s", r->open_time);    getchar();
        printf("输入项目经费（单位：万元）：");
        scanf("%f", &r->budget);    getchar();
        printf("输入项目负责人姓名：");
        scanf("%s", r->leader);    getchar();
        /*r-数据成员录入结束*/
        r->next=q->proj_head;           q->proj_head=r;
        printf("继续——Y   返回主菜单——其它任意键\n");
        j=getchar(); getchar();    FileOut();
        if(j!='Y'&&j!='y'){ break;  }
    }
    ClearScreen();
    return TRUE;
}

/**
 * 函数名称: AltSchool
 * 函数功能: 在十字链表中修改一个院系信息结点.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型-TRUE
 *
 * 调用说明:
 */
BOOL AltSchool(void)
{
    while(1){
        ClearScreen();  fflush(stdin);
        char count=0;
        do{
            if(count){
                /*出错提示*/
                printf("未找到目标信息！继续——Y   返回主菜单——其它任意键\n");
                j=getchar(); getchar(); ClearScreen();
                if(j!='y'&&j!='Y'){   return TRUE;   }
        }
            do{
                fflush(stdin);
                printf("您要按何种方式查询您要修改的信息？\n院系名称--1\t\t院系负责人--2\t\t联系电话--3\n");
                scanf("%hu", &choice);  getchar();
            }while(choice<1||choice>3);
            if(choice==1){          printf("输入院系名称：");      }
            else if(choice==2){     printf("输入院系负责人：");    }
            else{                   printf("输入联系方式：");      }
            fflush(stdin);  scanf("%s", seek);    getchar();
            /*按给定信息查找相应节点*/
            for(p=sch_head;p!=NULL;p=p->next){
                if(choice==1){      if(!strcmp(seek,p->school_name))  break;  }
                else if(choice==2){ if(!strcmp(seek,p->leader))       break;  }
                else{               if(!strcmp(seek,p->contact_num))  break;  }
            }
            count++;
        }while(p==NULL);
        do{
            /*修改信息*/
            printf("您要修改什么信息？\n院系名称--1\t\t院系负责人--2\t\t联系电话--3\t\t完成--4\n");
            scanf("%hu", &choice);  getchar();
                        if(choice>='1'&&choice<='3'){   printf("请输入新信息：");  }
            switch(choice){
                case 1:
                    printf("输入新的院系名称：");  scanf("%s", p->school_name);
                    getchar();  break;
                case 2:
                    printf("输入新的院系负责人：");    scanf("%s", p->leader);
                    getchar();  break;
                case 3:
                    printf("输入新的联系方式：");  scanf("%s",p->contact_num);
                    getchar();  break;
            }
            if(choice==4){  break;  }
        }while(1);
        printf("继续——Y   返回主菜单——其它任意键\n");
        fflush(stdin);  j=getchar(); getchar();
        if(j!='Y'&&j!='y'){ break;  }
    }
    ClearScreen();  FileOut();
    return TRUE;
}

/**
 * 函数名称: AltTeam
 * 函数功能: 在十字链表中修改一个科研团队信息结点.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型-TRUE
 *
 * 调用说明:
 */
BOOL AltTeam(void)
{
    while(1){
        ClearScreen();  fflush(stdin);
        char count=0;
        do{
            if(count){
                /*出错提示*/
                printf("未找到目标信息！继续——Y   返回主菜单——其它任意键\n");
                j=getchar(); getchar(); ClearScreen();
                if(j!='y'&&j!='Y'){   return TRUE;   }
            }
            printf("输入团队名称：");
            fflush(stdin);  scanf("%s", seek);    getchar();
            /*按给定信息搜索目标团队，记录q的位置并退出循环*/
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
            /*修改信息*/
            printf("您要修改什么信息？\n团队名称--1\t团队负责人--2\t教师人数--3\t研究生人数--4\t完成--5\n");/*所属院系如何修改？*/
            scanf("%hu", &choice);  getchar();
                        if(choice>='1'&&choice<='4'){   printf("请输入新信息：");  }
            switch(choice){
                case 1:
                    printf("输入新的团队名称：");    scanf("%s", q->team_name);
                    getchar();  break;
                case 2:
                    printf("输入新的团队负责人：");    scanf("%s", q->leader);
                    getchar();  break;
                case 3:
                    printf("输入新的教师人数：");     scanf("%d", &q->teacher_num);
                    getchar();  break;
                case 4:
                    printf("输入新的研究生人数：");     scanf("%d", &q->stu_num);
                    getchar();  break;
            }
            if(choice==5){  break;  }
        }while(1);
        printf("继续——Y   返回主菜单——其它任意键\n");
        fflush(stdin);  j=getchar(); getchar();
        if(j!='Y'&&j!='y'){ break;  }
    }
    ClearScreen();      FileOut();
    return TRUE;
}

/**
 * 函数名称: AltProject
 * 函数功能: 在十字链表中修改一个科研团队信息结点.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型-TRUE
 *
 * 调用说明:
 */
BOOL AltProject(void)
{
    while(1){
        ClearScreen();  fflush(stdin);
        char count=0;
        do{
            if(count){
                /*出错提示*/
                printf("未找到目标信息！继续——Y   返回主菜单——其它任意键\n");
                j=getchar(); getchar(); ClearScreen();
                if(j!='y'&&j!='Y'){   return TRUE;   }
            }
            printf("输入项目编号：");
            fflush(stdin);    scanf("%s", seek);    getchar();
            /*按给定信息搜索目标项目，记录r的位置并退出循环*/
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
            /*修改信息*/
            printf("您要修改什么信息？\n项目编号--1\t项目类别--2\t起始时间--3\n项目经费--4\t项目负责人--5\t完成--6\n");/*所属团队如何修改？*/
            scanf("%hu", &choice);  getchar();
            if(choice>='1'&&choice<='5'){   printf("请输入新信息：");  }
            switch(choice){
                case 1:
                    printf("输入新的项目编号：");     scanf("%s", r->project_num);
                    getchar();  break;
                case 2:
                    printf("输入新的项目类别：");     scanf("%c", &r->type);
                    getchar();  break;
                case 3:
                    printf("输入新的起始时间：");     scanf("%s", r->open_time);
                    getchar();  break;
                case 4:
                    printf("输入新的项目经费：");     scanf("%f", &r->budget);
                    getchar();  break;
                case 5:
                    printf("输入新的项目负责人：");     scanf("%s", r->leader);
                    getchar();  break;
            }
            if(choice==6){  break;  }
        }while(1);
        printf("继续——Y   返回主菜单——其它任意键\n");
        fflush(stdin);  j=getchar(); getchar();
        if(j!='Y'&&j!='y'){ break;  }
    }
    ClearScreen();      FileOut();
    return TRUE;
}

/**
 * 函数名称: DeleteSchool
 * 函数功能: 在十字链表中删除一个院系信息结点.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型-TRUE
 *
 * 调用说明:
 */
BOOL DeleteSchool(void)
{
    while(1){
        ClearScreen();
        char count=0;
        do{
            p1=sch_head;
            if(count){
                /*出错提示*/
                printf("未找到目标信息！继续——Y   返回主菜单——其它任意键\n");
                j=getchar(); getchar(); ClearScreen();
                if(j!='y'&&j!='Y'){   return TRUE;   }
            }
            do{
                printf("\n注意：删除院系数据时，将一并删除院系下全部团队与项目的数据！\n");
                printf("您要按何种方式查询您要修改的信息？\n院系名称--1\t\t院系负责人--2\t\t联系电话--3\n");
                scanf("%hu", &choice);  getchar();
            }while(choice<1||choice>3);
            if(choice==1){          printf("输入院系名称：");      }
            else if(choice==2){     printf("输入院系负责人：");    }
            else{                   printf("输入联系方式：");      }
            fflush(stdin);  scanf("%s", seek);    getchar();
            /*按给定信息查找相应院系*/
            while(p1){
                if(choice==1){      if(!strcmp(seek,p1->school_name))  break;  }
                else if(choice==2){ if(!strcmp(seek,p1->leader))       break;  }
                else{               if(!strcmp(seek,p1->contact_num))  break;  }
                p2=p1;
                p1=p1->next;
            }
            count++;
        }while(p1==NULL);
        if(!p1->team_head); /*院系下无团队*/
        else{               /*院系下有团队*/
            /*删除所有团队节点-开始*/
            q=p1->team_head->next;
            while(q!=NULL){
                if(!q->proj_head);  /*团队下无项目*/
                else{               /*团队下有项目*/
                    /*删除所有项目节点-开始*/
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
                /*删除所有项目节点-结束*/
                p1->team_head->next=q->next;
                free(q);
                q=p1->team_head->next;
            }
            q=p1->team_head;
            /*删除所有项目节点-开始*/
            r=q->proj_head->next;
            while(r!=NULL){
                q->proj_head->next=r->next;
                free(r);
                r=q->proj_head->next;
            }
            r=q->proj_head;
            q->proj_head=NULL;
            free(r);
            /*删除所有项目节点-结束*/
            p1->team_head=NULL;
            free(q);
            /*删除所有团队节点-结束*/
        }
        if(p1==sch_head){  sch_head=p1->next;     }
        else{   p2->next=p1->next;  }        free(p1);   /*删除院系节点*/
        printf("删除成功！\n继续——Y   返回主菜单——其它任意键\n");
        fflush(stdin);  j=getchar(); getchar();
        if(j!='Y'&&j!='y'){ break;  }
    }
    ClearScreen();      FileOut();
    return TRUE;
}

/**
 * 函数名称: DeleteTeam
 * 函数功能: 在十字链表中删除一个科研团队信息结点.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型-TRUE
 *
 * 调用说明:
 */
BOOL DeleteTeam(void)
{
    while(1){
        ClearScreen();
        printf("\n注意：删除团队数据时，将一并删除团队下全部项目的数据！\n");
        char count=0;
        do{
            if(count){
                /*出错提示*/
                printf("未找到目标信息！继续——Y   返回主菜单——其它任意键\n");
                j=getchar(); getchar(); ClearScreen();
                if(j!='y'&&j!='Y'){   return TRUE;   }
            }
            printf("输入团队名称：");
            fflush(stdin);  scanf("%s", seek);    getchar();
            /*按给定信息查找相应团队节点*/
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
        if(!q1->proj_head); /*团队下无项目*/
        else{               /*团队下有项目*/
            /*删除所有团队节点-开始*/
            r=q1->proj_head->next;
            while(r!=NULL){
                q1->proj_head->next=r->next;
                free(r);
                r=q1->proj_head->next;
            }
            r=q1->proj_head;
            q1->proj_head=NULL;
            free(r);
        /*删除所有团队节点-结束*/
        }
        if(q1==p->team_head){   p->team_head=q1->next; }
        else{   q2->next=q1->next;  }        free(q1);   /*删除团队节点*/
        /*院系下团队为空时强制退出，防止因人为操作进入死循环*/
        if(p->team_head==NULL){
            printf("此学院下已无团队！\n");
            printf("按任意键继续...\n"); getch();     ClearScreen();
            return TRUE;
        }
        printf("删除成功！\n继续——Y   返回主菜单——其它任意键\n");
        fflush(stdin);  j=getchar(); getchar();
        if(j!='Y'&&j!='y'){ break;  }
    }
    ClearScreen();      FileOut();
    return TRUE;
}

/**
 * 函数名称: DeleteProject
 * 函数功能: 在十字链表中删除一个科研项目信息结点.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型-TRUE
 *
 * 调用说明:
 */
BOOL DeleteProject(void)
{
    do{
        ClearScreen();
        char count=0;
        do{
            if(count){
                /*出错提示*/
                printf("未找到目标信息！继续——Y   返回主菜单——其它任意键\n");
                j=getchar(); getchar();     ClearScreen();
                if(j!='y'&&j!='Y'){   return TRUE;   }
            }
            printf("输入项目编号：");
            fflush(stdin);  scanf("%s", seek);    getchar();
            /*按给定信息查找相应项目节点*/
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
        else{   r2->next=r1->next;  }        free(r1);  /*删除项目节点*/
        /*团队下项目为空时强制退出，防止因人为操作进入死循环*/
        if(q->proj_head==NULL){
            printf("此团队下已无项目！\n");
            printf("按任意键继续...\n"); getch();    ClearScreen();
            return TRUE;
        }
        printf("删除成功！\n继续——Y   返回主菜单——其它任意键\n");
        fflush(stdin);  j=getchar();
    }while(j=='Y'||j=='y');
    ClearScreen();      FileOut();
    return TRUE;
}

/**
 * 函数名称: SeekSchool
 * 函数功能: 给定要求，在十字链表中查找所有符合要求院系信息结点.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型-TRUE
 *
 * 调用说明:
 */
BOOL SeekSchool(void)
{
    while(1){
        char count=0,flag=0,judge;
        ClearScreen();
        do{
            judge=1;
            if(count){
                /*出错提示*/
                printf("未找到目标信息！继续——Y   返回主菜单——其它任意键\n");
                j=getchar(); getchar();     ClearScreen();
                if(j!='y'&&j!='Y'){   return TRUE;   }
            }
            printf("您要按何种方式查询？\n院系负责人%6c--press1\n院系名称%6c--press2\n",' ',' ');
            k=getchar();    getchar();
            if(k=='1')  {   printf("院系负责人:");      }
            else        {   printf("院系名称子串:");    }
            fflush(stdin);  scanf("%s", seek);    getchar();
            printf("院系名称%16c院系负责人姓名%4c联系电话\n",' ',' ');
            for(p=sch_head;p!=NULL;p=p->next){
                if(k=='1'){     /*按院系负责人查询*/
                    if(strstr(p->leader,seek)){
                        printf("%-24s%-18s%s\n",p->school_name,p->leader,p->contact_num);
                        flag=1;
                    }
                }
                else{           /*按院系名称关键词查询*/
                    if(strstr(p->school_name,seek)){
                        printf("%-24s%-18s%s\n",p->school_name,p->leader,p->contact_num);
                        flag=1;
                    }
                }
            }
            if(!judge){     break;  }
            count++;
        }while(p1==NULL&&!flag);
        printf("继续——Y   返回主菜单——其它任意键\n");
        fflush(stdin);  j=getchar();    getchar();
        if(j!='y'&&j!='Y'){ ClearScreen();  return TRUE;   }
    }
    ClearScreen();
    return TRUE;
}

/**
 * 函数名称: SeekTeam
 * 函数功能: 给定要求，在十字链表中查找所有符合要求科研团队信息结点.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型-TRUE
 *
 * 调用说明:
 */
BOOL SeekTeam(void)
{
    unsigned short low,high;
    while(1){
        char count=0,flag=0;
        ClearScreen();
        do{
            /*出错提示*/
            if(count){
                /*出错提示*/
                printf("未找到目标信息！继续——Y   返回主菜单——其它任意键\n");
                j=getchar(); getchar();     ClearScreen();
                if(j!='y'&&j!='Y'){   return TRUE;   }
            }
            printf("您要按何种方式查询？\n团队名称%6c--press1\n教师人数%6c--press2\n",' ',' ');
            k= getchar(); getchar();
            if(k=='1'){
                printf("团队名称子串:");
                fflush(stdin);  scanf("%s", seek);    getchar();
            }
            else{
                printf("教师人数下限:");
                scanf("%hu", &low);     getchar();
                printf("教师人数上限:");
                scanf("%hu", &high);    getchar();
            }
            printf("团队名称%14c团队负责人姓名%2c所属院系%16c教师人数%2c研究生人数\n",' ',' ',' ',' ');
            for(p=sch_head;p!=NULL;p=p->next){
                for(q=p->team_head;q!=NULL;q=q->next){
                    if(k=='1'){     /*按团队名称关键词查询*/
                        if(strstr(q->team_name,seek)){
                            printf("%-22s%-16s%-24s%-10d%d\n",q->team_name,q->leader,q->school_name,q->teacher_num,q->stu_num);
                            flag=1;
                        }
                    }
                    else{           /*按教师人数范围查询*/
                        if(q->teacher_num>=low&&q->teacher_num<=high){
                            printf("%-22s%-16s%-24s%-10d%d\n",q->team_name,q->leader,q->school_name,q->teacher_num,q->stu_num);
                            flag=1;
                        }
                    }
                }
            }
            count++;
        }while(p==NULL&&!flag);
        printf("继续——Y   返回主菜单——其它任意键\n");
        j=getchar(); getchar();
        if(j!='Y'&&j!='y'){ break;  }
    }
    ClearScreen();
    return TRUE;
}

/**
 * 函数名称: SeekProject
 * 函数功能: 给定要求，在十字链表中查找所有符合要求科研团队信息结点.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型-TRUE
 *
 * 调用说明:
 */
BOOL SeekProject(void)
{
    while(1){
        char count=0,flag=0;
        ClearScreen();
        do{
            /*出错提示*/
            if(count){
                /*出错提示*/
                printf("未找到目标信息！继续——Y   返回主菜单——其它任意键\n");
                j=getchar(); getchar();     ClearScreen();
                if(j!='y'&&j!='Y'){   return TRUE;   }
            }
            printf("您要按何种方式查询？\n项目编号%6c--press1\n所属团队%6c--press2\n",' ',' ');
            k= getchar(); getchar();
            if(k=='1'){   printf("项目编号:");       }
            else{          printf("项目所属团队:");   }
            fflush(stdin);  scanf("%s", seek);    getchar();
            printf("项目编号%9c起始时间%4c项目经费%6c项目负责人%5c所属团队%10c项目类别\n",' ',' ',' ',' ',' ');
            for(p=sch_head;p!=NULL;p=p->next){
                for(q=p->team_head;q!=NULL;q=q->next){
                    if(k=='1'){     /*按项目编号查询*/
                        for(r=q->proj_head;r!=NULL;r=r->next){
                            if(!strcmp(r->project_num,seek)){
                                printf("%-17s%-12s%-27f万元%-15s%-18s",r->project_num,r->open_time,r->budget,r->leader,r->team_name);
                                switch(r->type){
                                    case '1':   printf("973计划项目\n");  break;
                                    case '2':   printf("国家自然科学基金项目\n");  break;
                                    case '3':   printf("863计划项目\n");  break;
                                    case '4':   printf("国际合作项目\n");  break;
                                    case '5':   printf("横向项目\n");  break;
                                }
                                flag=1;
                            }
                        }
                    }
                    else{           /*按所属团队查询*/
                        if(!strcmp(q->team_name,seek)){
                            for(r=q->proj_head;r!=NULL;r=r->next){
                                printf("%-17s%-12s%-27f万元%-15s%-18s",r->project_num,r->open_time,r->budget,r->leader,r->team_name);
                                switch(r->type){
                                    case '1':   printf("973计划项目\n");  break;
                                    case '2':   printf("国家自然科学基金项目\n");  break;
                                    case '3':   printf("863计划项目\n");  break;
                                    case '4':   printf("国际合作项目\n");  break;
                                    case '5':   printf("横向项目\n");  break;
                                }
                                flag=1;
                            }
                        }
                    }
                }
            }
            count++;
        }while(p==NULL&&!flag);
        printf("继续——Y   返回主菜单——其它任意键\n");
        j=getchar(); getchar();
        if(j!='Y'&&j!='y'){ break;  }
    }
    ClearScreen();
    return TRUE;
}

/**
 * 函数名称: TeachStuProp
 * 函数功能: 统计各院系教师总数，研究生总数，及研究生与教师的人数比（保留2位小数），
 *            按学生教师人数比值降序排序后，输出统计结果.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型-TRUE
 *
 * 调用说明:
 */
BOOL TeachStuProp(void)
{
    /*对各院系教师及研究生人数求和并计算比值*/
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
    /*按比值用交换指针域的方法降序排序*/
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
	/*打印排序结果*/
	printf("%8c院系%16c研究生人数%3c教师人数%3c研究生教师人数比\n",b,b,b,b);
	for(p=sch_head;p!=NULL;p=p->next){
        if(p->proportion==0){   continue;   }
        printf("%-20s%14d%12d%16.2f\n",p->school_name,(int)p->sum_student,(int)p->sum_teacher,p->proportion);
	}
    printf("按任意键继续...\n"); getch(); ClearScreen();
	return TRUE;
}

/**
 * 函数名称: ProjectTotal
 * 函数功能: 统计某年度各院系科研项目数，973 项目数，863 项目数，
 *           及科研总经费，科研项目数降序排序后输出.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型-TRUE
 *
 * 调用说明:
 */
BOOL ProjectTotal(void)
{
    do{
        unsigned short year, ori;
        ClearScreen();  flag=0;
        printf("请输入起始年份:");
        scanf("%hu",&year);  getchar();
        /*对满足要求年份的项目，统计两类项目总数并对经费求和*/
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
            /*按项目总数用交换指针域的方法降序排序*/
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
            /*打印排序结果*/
            printf("%hu年\n%8c院系%15c项目总数%3c973项目总数%3c863项目总数%3c科研总经费\n",year,b,b,b,b,b);
            for(p=sch_head;p!=NULL;p=p->next){
                if(p->num_proj==0){     continue;   }
                printf("%-20s%12u%13u%14u%18f\n",p->school_name,p->num_proj,p->num_973,p->num_863,p->budget);
            }
        }
        else{   printf("本年度没有已经开始的项目！\n");  }
        printf("继续——Y   返回主菜单——其它任意键\n");
        j=getchar(); getchar();
    }while(j=='Y'||j=='y');
    ClearScreen();
    return TRUE;
}

/**
 * 函数名称: TeamOfMostNSFC
 * 函数功能: 统计历年来类别为国家自然科学基金的科研项目数最多的10 个科研团队，按项目数降序排序后输出科研团队名称，
 *            类别为国家自然科学基金的科研项目数，以及项目经费总数.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型-TRUE
 *
 * 调用说明:
 */
BOOL TeamOfMostNSFC(void)
{
    unsigned char min=255, count=0;
    /*对指定类型的项目，统计总数并对经费求和*/
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
    /*将所有团队的名称及项目数拷贝到新链表中以统一排序*/
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
    /*按项目总数用交换指针域的方法降序排序*/
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
	/*打印排序结果，此处将并列的情况纳入考虑，视情况适当增加输出数据组数*/
	printf("%6c团队名称%6c国家自然科学基金项目数%4c此类项目总经费\n",b,b,b);
	for(u=head;u!=NULL;u=u->next,count++){
        if(count>=10&&u->projsci_num<min){  break;  }
        min=u->projsci_num;
        printf("%-31s%-15u%f\n",u->team_name,u->projsci_num,u->proj_budget);
	}
	q1=head;
	for(q2=q1->next;q2!=NULL;q1=q2,q2=q1->next)
        free(q1);
    printf("按任意键继续...\n"); getch(); ClearScreen();
    return TRUE;
}

/**
 * 函数名称: ProjTeachProp
 * 函数功能: 统计历年来类别为国家自然科学基金的科研项目数最多的10 个科研团队，按项目数降序排序后输出科研团队名称，
 *            类别为国家自然科学基金的科研项目数，以及项目经费总数.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型-TRUE
 *
 * 调用说明:
 */
BOOL ProjTeachProp(void)
{
    unsigned char count=0;
    float min=1.0e6;
    /*统计科研项目数与教师人数比值*/
    for(p=sch_head;p!=NULL;p=p->next){
        for(q=p->team_head;q!=NULL;q=q->next){
            q->proj_num=0;
            for(r=q->proj_head;r!=NULL;r=r->next){  q->proj_num++;  }
                q->proportion=q->proj_num*1.0/q->teacher_num;
            }
    }
    /*新建科研团队链表并从原表向新表复制内容*/
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
	/*按项目数与教师人数比值降序排序*/
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
	/*打印排序结果，此处将并列的情况纳入考虑，视情况适当增加输出数据组数*/
	printf("%6c团队名称%6c教师人数%3c项目总数%3c项目数和教师人数比值\n",b,b,b,b);
	for(u=head;u!=NULL;u=u->next,count++){
        if(count>=5&&u->proportion<min){    break;  }
        min=u->proportion;
        printf("%-23s%-11d%-16u%.2f\n",u->team_name,u->teacher_num,u->proj_num,u->proportion);
	}
	/*删除新建链表*/
	q1=head;
	for(q2=q1->next;q2!=NULL;q1=q2,q2=q1->next)
        free(q1);
    printf("按任意键继续...\n"); getch(); ClearScreen();
    return TRUE;
}

/**
 * 函数名称: LeaderOfProj
 * 函数功能: 在特定院系内统计负责人负责项目的数目，
 * 降序排序后输出负责人姓名及负责项目的数量
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型-TRUE
 *
 * 调用说明:
 */
BOOL LeaderOfProj(void)
{
    LEADER_NODE *head, *t, *prior1, *prior2, *p1, *p2;
    unsigned short count,save;
    do{
        count=1;    head=NULL;
        /*选定院系*/
        do{
            if(count-1){
                /*出错提示*/
                printf("未找到目标信息！继续——Y   返回主菜单——其它任意键\n");
                j=getchar(); getchar();     ClearScreen();
                if(j!='y'&&j!='Y'){   return TRUE;   }
            }
            printf("院系名称：");
            scanf("%s",seek); getchar();
            for(p=sch_head;p!=NULL;p=p->next){
                if(!strcmp(p->school_name,seek)){    break;   }
            }
            count++;
        }while(!p);
        if(!p->team_head){  printf("此院系下暂无项目！\n");   }
        else{
            /*新建负责人专用链表，遍历原表统计各负责人承接的项目数量*/
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
            /*按项目数量与教师人数比值降序排序*/
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
            /*打印排序结果，此处将并列的情况纳入考虑*/
            printf("排名\t负责人姓名\t负责项目数量\n");
            count=1;    save=count;
            for(prior1=head,p1=head->next;p1!=NULL;prior1=prior1->next,p1=prior1->next,count++){
                if(p1->n==prior1->n){   printf("%2hu", save);    }
                else{   save=count;     printf("%2hu",count);    }
                printf("%13s%15hu\n",p1->leader,p1->n);
            }
            /*释放缴费信息支链的动态存储区*/
            p1=head;
            while (p1 != NULL)
            {   p2 = p1->next;      free(p1);      p1 = p2;    }
        }
        printf("继续——Y   返回主菜单——其它任意键\n");
        j=getchar(); getchar();     ClearScreen();
    }while(j=='Y'||j=='y');
    return TRUE;
}

BOOL HelpTopic(void)
{
    ShellExecuteA(NULL,"open","NOTEPAD.EXE","help.txt",NULL,SW_SHOWNORMAL);
    printf("按任意键继续...\n"); getch(); ClearScreen();
    return TRUE;
}

BOOL AboutDorm(void)
{
    char *plabel_name[] = {"系统版本：V1.0",
                           "**************",
                           "All Rights Reserved",
                           "确认"
                          };
    ShowModule(plabel_name, 4);
    return TRUE;
}
