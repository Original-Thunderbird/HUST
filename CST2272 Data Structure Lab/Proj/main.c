#include"head.h"
int main()
{
    COORD size = {SCR_COL, SCR_ROW};              /*窗口缓冲区大小*/
    gh_std_out = GetStdHandle(STD_OUTPUT_HANDLE); /* 获取标准输出设备句柄*/
    gh_std_in = GetStdHandle(STD_INPUT_HANDLE);   /* 获取标准输入设备句柄*/
    SetConsoleTitle(gp_sys_name);                 /*设置窗口标题*/
    SetConsoleScreenBufferSize(gh_std_out, size); /*设置窗口缓冲区大小80*25*/
//    FileIn();
    Initial();                     /*数据加载*/
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
    /*
    WORD att = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
               | BACKGROUND_BLUE;  //黄色前景和蓝色背景
    SetConsoleTextAttribute(gh_std_out, att);  //设置控制台屏幕缓冲区字符属性*/
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
    char len[3] = { (char)strlen(pCh[0]), (char)strlen(pCh[1]), (char)strlen(pCh[2])};
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
/*
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
*/
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
/*
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
    //从数据文件中读宿舍楼信息数据，存入以后进先出方式建立的主链中
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
    //从数据文件中读取学生基本信息数据，存入主链对应结点的学生基本信息支链中
    while (fread(&tmp2, sizeof(TEAM_NODE), 1, pFile) == 1)
    {
        //创建结点，存放从数据文件中读出的学生基本信息
        q = (TEAM_NODE *)malloc(sizeof(TEAM_NODE));
        *q = tmp2;
        q->proj_head = NULL;
        //在主链上查找该学生所住宿舍楼对应的主链结点
        p = head;
        while (p != NULL&& strcmp(p->school_name, q->school_name))
        {
            p = p->next;
        }
        if (p != NULL) //如果找到，则将结点以后进先出方式插入学生信息支链
        {
            q->next = p->team_head;
            p->team_head = q;
        }
        else  //如果未找到，则释放所创建结点的内存空间
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
    //从数据文件中读取学生缴费信息数据，存入学生基本信息支链对应结点的缴费支链中
    while (fread(&tmp3, sizeof(PROJECT_NODE), 1, pFile) == 1)
    {
        //创建结点，存放从数据文件中读出的学生缴费信息
        r = (PROJECT_NODE *)malloc(sizeof(PROJECT_NODE));
        *r = tmp3;
        //查找学生信息支链上对应学生信息结点
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
        if (find)  //如果找到，则将结点以后进先出方式插入学生缴费信息支链中
        {
            r->next = q->proj_head;
            q->proj_head = r;
        }
        else //如果未找到，则释放所创建结点的内存空间
        {   free(r);    }
    }
    fclose(pFile);
    printf("按任意键继续...\n"); getch();
    return re;
}
*/
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
    pFunction[1] = FileIn;
    pFunction[2] = ExitSys;
    pFunction[3] = MaintainTeacherInfo;
    pFunction[4] = MaintainRoomInfo;
    pFunction[5] = MaintainGroupInfo;
    pFunction[6] = MaintainCourseInfo;
    pFunction[7] = MaintainClassInfo;
    pFunction[8] = SeekTeacher;
    pFunction[9] = SeekRoom;
    pFunction[10] = SeekGroup;
    pFunction[11] = SeekCourse;
    pFunction[12] = SeekClass;
    pFunction[13] = RoomUsage;
    pFunction[14] = TeacherAnalysis;
    pFunction[15] = ClassAnalysis;
    pFunction[16] = HelpTopic;
    pFunction[17] = NULL;
    pFunction[18] = AboutDorm;
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
    for(i=0;i<telist->length;i++){
        for(j=0;j<6;j++)
            for(pocc=telist->elem[i].ochead[j].head;pocc!=NULL;pocc=telist->elem[i].ochead[j].head){
                telist->elem[i].ochead[j].head=pocc->next;     free(pocc);     pocc=NULL;
            }
    }
    free(telist->elem);     telist->elem=NULL;      telist->length=0;
    for(i=0;i<rolist->length;i++){
        for(j=0;j<6;j++)
            for(pocc=rolist->elem[i].ochead[j].head;pocc!=NULL;pocc=rolist->elem[i].ochead[j].head){
                rolist->elem[i].ochead[j].head=pocc->next;     free(pocc);     pocc=NULL;
            }
    }
    free(rolist->elem);     rolist->elem=NULL;      rolist->length=0;
    for(i=0;i<cougraph->length;i++){    //删除所有先修课节点
        for(pcarc=cougraph->coulist[i].archead;pcarc!=NULL;pcarc=cougraph->coulist[i].archead){
            cougraph->coulist[i].archead=pcarc->nextarc;
            free(pcarc->prename);     free(pcarc);
        }
    }
    free(cougraph->coulist);    cougraph->coulist=NULL;      cougraph->length=0;
    for(i=0;i<3;i++){
        for(psch=grade[i].schead;psch!=NULL;psch=grade[i].schead){
            for(j=0;j<6;j++){
                for(pcls=psch->week[j].classhead;pcls!=NULL;pcls=psch->week[j].classhead){
                    psch->week[j].classhead=pcls->next;        free(pcls);      pcls=NULL;
                }
                for(pocc=psch->week[j].ochead.head;pocc!=NULL;pocc=psch->week[j].ochead.head){
                    psch->week[j].ochead.head=pocc->next;       free(pocc);     pcls=NULL;
                }
            }
            grade[i].schead=psch->next;     free(psch);     psch=NULL;
        }
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

BOOL Initial(void){     //pass
    //初始化教室、教师，课程清单结构
    telist=(TeList *)malloc(sizeof(TeList));
    rolist=(RoList *)malloc(sizeof(RoList));
    cougraph = (CouGraph *)malloc(sizeof(CouGraph));
    //初始化教室，教师，课程线性查找表
    telist->elem = (Teacher *)malloc(LIST_INIT_SIZE*sizeof(Teacher));
    rolist->elem = (Room *)malloc(LIST_INIT_SIZE*sizeof(Room));
    cougraph->coulist = (CouHead *)malloc(LIST_INIT_SIZE*sizeof(CouHead));
    if(!(telist->elem && rolist->elem && cougraph->coulist)) return FALSE;
    //各表长度置0，指针置NULL
	telist->length = rolist->length = cougraph->length = 0;
    telist->listsize = rolist->listsize = cougraph->listsize = LIST_INIT_SIZE;
    cougraph->length = 0;
    grade[0].gr_num=grade[1].gr_num=grade[2].gr_num=0;
    grade[0].schead = grade[1].schead = grade[2].schead =NULL;
    printf("系统初始化成功 ");
    getchar();  ClearScreen();
    return TRUE;
}

BOOL AddTeacher(void){  //pass
    fflush(stdin);
    printf("输入教师姓名:");
    scanf("%s", cbuffer);      getchar();
    for(i=0;i<telist->length;i++)
        if(!strcmp(telist->elem[i].tname,cbuffer)){
            printf("该教师已存在");
            getchar();   ClearScreen();    return TRUE;
        }
    AddNewTeacher(cbuffer);
    printf("输入性别(男--1\t女--0):");
    scanf("%hu",&telist->elem[telist->length].gender);   getchar();
    printf("输入年龄:");
    scanf("%hu",&telist->elem[telist->length].age);   getchar();
    telist->length++;    printf("插入成功");
    getchar();  ClearScreen();
    return TRUE;
}

BOOL AddRoom(void){     //pass
    fflush(stdin);
    printf("请输入教室编号:");
    scanf("%s", cbuffer);      getchar();
    for(i=0;i<rolist->length;i++)
        if(!strcmp(rolist->elem[i].code,cbuffer)){
            printf("该教室已存在");
            getchar();   ClearScreen();    return TRUE;
        }
    AddNewRoom(cbuffer);
    printf("输入容量:");
    scanf("%hu",&rolist->elem[rolist->length].rsize);   getchar();
    rolist->length++;   printf("插入成功");
    getchar();  ClearScreen();
    return TRUE;
}

BOOL AddCourse(void){   //pass
    fflush(stdin);
    printf("请输入课程名称:");
    scanf("%s", cbuffer);      getchar();
    for(i=0;i<cougraph->length;i++)
        if(!strcmp(cougraph->coulist[i].name,cbuffer)){
            printf("该课程已存在");
            getchar();   ClearScreen();    return TRUE;
        }
    AddNewCourse(cbuffer);
    printf("输入先修课个数:");
    scanf("%hu", &(cougraph->coulist[cougraph->length].arcnum));    getchar();
    printf("输入各先修课程:");
    for(j=0;j<cougraph->coulist[cougraph->length].arcnum;j++){
        scanf("%s", cbuffer);   getchar();
        for(i=0;i<cougraph->length;i++)
            if(!strcmp(cougraph->coulist[i].name, cbuffer)){    //在课程清单中找到该课程
                pcarc = (CouArc *)malloc(sizeof(CouArc));
                pcarc->prename=(char *)malloc(sizeof(cbuffer)*sizeof(char));
                strcpy(pcarc->prename, cbuffer);
                pcarc->nextarc = cougraph->coulist[cougraph->length].archead;
                cougraph->coulist[cougraph->length].archead = pcarc;
            }
            else    printf("清单中无此课程,放弃对其操作");
    }
    cougraph->length++;     printf("插入成功");
    getchar();  ClearScreen();
    return TRUE;
}

BOOL AddGroup(void){    //pass
    char cbuf3[20];
    fflush(stdin);
    printf("输入班号:");
    psch=ClassInput(cbuf3);
    if(psch!=NULL){
        printf("该班级已存在");
        getchar();   ClearScreen();    return TRUE;
    }
    //不存在时建立该班级结点
    psch=(Schedule *)malloc(sizeof(Schedule));
    strcpy(psch->classname, cbuf3);
    printf("输入已修课程数目:");
    scanf("%hu",&psch->do_num);    getchar();
    psch->pr_num=0;     //在修课程不可手动输入，需配合新课程一并导入
    printf("输入各已修课程:\n");
    for(i=0;i<psch->do_num;i++){
        scanf("%s",psch->done[i]);      getchar();
    }
    printf("输入人数:");
    scanf("%hu",&psch->clsize);   getchar();
    for(i=0;i<6;i++){   //新班级下课堂结构初始化
        psch->week[i].classhead=NULL;
        psch->week[i].cl_num=0;
        psch->week[i].ochead.head=NULL;
        psch->week[i].ochead.len=0;
    }
    //按年级将新班级插入基于班级的课程表
    if(strstr(cbuf3,"15")){
        psch->next=grade[0].schead;     grade[0].schead=psch;
        grade[0].gr_num++;
    }
    else if(strstr(cbuf3,"16")){
        psch->next=grade[1].schead;     grade[1].schead=psch;
        grade[1].gr_num++;
    }
    else if(strstr(cbuf3,"17")){
        psch->next=grade[2].schead;     grade[2].schead=psch;
        grade[2].gr_num++;
    }
    printf("插入成功");
    getchar();  ClearScreen();
    return TRUE;
}

BOOL DelTeacher(void){
    fflush(stdin);
    if(telist->length==0){
        printf("教师名单为空");
        getchar();   ClearScreen();    return TRUE;
    }
    printf("输入待删教师姓名:");
    scanf("%s", cbuffer);   getchar();
    //定位目标教师，同时确认其存在性
    for(i=0;i<telist->length;i++){
        if(!strcmp(telist->elem[i].tname,cbuffer))    break;
    }
    if(i==telist->length){
        printf("未找到相应教师");
        getchar();   ClearScreen();    return TRUE;
    }
    //删除目标教师结构下时间占位链表
    for(j=0;j<6;j++)
        for(pocc=telist->elem[i].ochead[j].head;pocc!=NULL;pocc=telist->elem[i].ochead[j].head){
            telist->elem[i].ochead[j].head = pocc->next;    free(pocc);
        }
    //删除所有有相应教师的课堂
    for(k=0;k<3;k++)
        for(psch=grade[k].schead;psch!=NULL;psch=psch->next)
            for(j=0;j<6;j++)
                for(pcls=psch->week[j].classhead;pcls!=NULL;pcls=pcls->next)
                    if(!strcmp(telist->elem[i].tname,pcls->tname)){     //检查教师姓名是否吻合
                        strcpy(cbuf1, pcls->roomcode);
                        memcpy(buf3, pcls->wk_st, sizeof(buf3));    memcpy(buf4, pcls->wk_ed, sizeof(buf4));
                        buf5=pcls->cou_st;  buf6=pcls->cou_ed;      //将课程结点信息复制进临时创建的课程结点
                        //在班级下属数组中删除相关时间段
                        DelTimeNode(pcls->wk_st,pcls->wk_ed,pcls->cou_st,pcls->cou_ed,&(psch->week[j].ochead));
                        SeekRemClass(psch, cbuffer);    //检查目标课程是否还在该班级下有课堂
                        //在教室结构中删除占位时间
                        for(l=0;l<rolist->length;l++)
                            if(!strcmp(rolist->elem[l].code, cbuf1))    break;
                        if(l!=rolist->length)
                            DelTimeNode(buf3,buf4,buf5,buf6,&(rolist->elem[l].ochead[j]));
                        DelClassNode(pcls, psch, (unsigned short)j);   //删除课程结点
                    }
    //删除目标教师
    for(j=i;j<telist->length-1;j++)
        telist->elem[j]=telist->elem[j+1];
    telist->length--;   printf("删除成功");
    if(telist->length==0)   printf("表中已无教师");
    getchar();  ClearScreen();
    return TRUE;
}

BOOL DelRoom(void){
    fflush(stdin);
    if(rolist->length==0){
        printf("教室名单为空");
        getchar();   ClearScreen();    return TRUE;
    }
    printf("请输入待删教室编号:");
    scanf("%s", cbuffer);   getchar();
    //定位目标教室，同时确认其存在性
    for(i=0;i<rolist->length;i++){
        if(!strcmp(rolist->elem[i].code,cbuffer))    break;
    }
    if(i==rolist->length){
        printf("未找到相应教室");
        getchar();   ClearScreen();    return TRUE;
    }
    //删除所有先修课节点
    for(j=0;j<6;j++)
        for(pocc=rolist->elem[i].ochead[j].head;pocc!=NULL;pocc=rolist->elem[i].ochead[j].head){
            rolist->elem[i].ochead[j].head = pocc->next;    free(pocc);
        }
    //删除所有有相应教室的课堂
    for(k=0;k<3;k++)
        for(psch=grade[k].schead;psch!=NULL;psch=psch->next)
            for(j=0;j<6;j++)
                for(pcls=psch->week[j].classhead;pcls!=NULL;pcls=pcls->next)
                    if(!strcmp(rolist->elem[i].code,pcls->roomcode)){
                        strcpy(cbuf2, pcls->tname);     //将课程结点信息复制进临时创建的课程结点
                        memcpy(buf3, pcls->wk_st, sizeof(buf3));    memcpy(buf4, pcls->wk_ed, sizeof(buf4));
                        buf5=pcls->cou_st;  buf6=pcls->cou_ed;
                        //在班级下属数组中删除相关时间段
                        DelTimeNode(pcls->wk_st,pcls->wk_ed,pcls->cou_st,pcls->cou_ed,&(psch->week[j].ochead));
                        SeekRemClass(psch, cbuffer); //检查目标课程是否还在该班级下有课堂
                        //在教师结构中删除占位时间
                        for(l=0;l<telist->length;l++)
                            if(!strcmp(telist->elem[l].tname, cbuf2))   break;
                        if(l!=telist->length)
                            DelTimeNode(buf3,buf4,buf5,buf6,&(telist->elem[l].ochead[j]));
                        DelClassNode(pcls, psch, (unsigned short)j);   //删除课程结点
                    }
    for(j=i;j<rolist->length-1;j++)     //删除教室结构
        rolist->elem[j]=rolist->elem[j+1];
    rolist->length--;   printf("删除成功");
    if(rolist->length==0)   printf("表中已无教室");
    getchar();  ClearScreen();
    return TRUE;
}

BOOL DelCourse(void){
    fflush(stdin);
    if(cougraph->length==0){
        printf("课程名单为空");
        getchar();   ClearScreen();    return TRUE;
    }
    CouArc *rcarc;
    printf("输入待删课程名称:");
    scanf("%s", cbuffer);       getchar();
    //定位目标课程，同时确认其存在性
    for(i=0;i<cougraph->length;i++)
        if(!strcmp(cougraph->coulist[i].name,cbuffer))    break;
    if(i==cougraph->length){
        printf("未找到相应课程");
        getchar();   ClearScreen();    return TRUE;
    }
    //若课程清单中某门课的先修课为目标课程，则将目标课程结点删除，同时添加目标课程的先修课为新的先修课
	for(j=0; j<cougraph->length; j++){
        if(j==i)    continue;
		qcarc = cougraph->coulist[j].archead;
		while(qcarc){ //有弧或边
			if(!strcmp(qcarc->prename, cbuffer)){ //该弧被待删结点所指
			    pcarc = cougraph->coulist[i].archead;
			    while(pcarc){   //将先修课程的先修课程指向当前课程
                    rcarc = (CouArc *)malloc(sizeof(CouArc));
                    strcpy(rcarc->prename, pcarc->prename);
                    rcarc->nextarc = cougraph->coulist[j].archead;
                    cougraph->coulist[j].archead = rcarc;
                    pcarc = pcarc->nextarc;
			    }
			    rcarc = cougraph->coulist[j].archead;
			    while(rcarc->nextarc!=qcarc) rcarc=rcarc->nextarc;  //删除弧头为指定课程的弧
                rcarc->nextarc = qcarc->nextarc;
                free(qcarc->prename);   free(qcarc);
                qcarc = rcarc->nextarc;
			}
			qcarc = qcarc->nextarc;
        }   }
	cougraph->length--;     //课程清单长度自减
	for(pcarc=cougraph->coulist[i].archead;pcarc!=NULL;pcarc=cougraph->coulist[i].archead){     //删除目标课程的所有先修课
        cougraph->coulist[i].archead=pcarc->nextarc;
        free(pcarc->prename);       free(pcarc);
	}
    //删除所有有相应课程的课堂
    for(k=0;k<3;k++)
        for(psch=grade[k].schead;psch!=NULL;psch=psch->next){
            for(j=0;j<psch->pr_num;j++)
                if(!strcmp(cougraph->coulist[i].name,psch->progress[j]))    break;
            if(j==psch->pr_num) continue;
            for(j=0;j<6;j++)
                for(pcls=psch->week[j].classhead;pcls!=NULL;pcls=pcls->next)
                    if(!strcmp(cougraph->coulist[i].name,pcls->coursename)){
                        strcpy(cbuf1, pcls->roomcode);      strcpy(cbuf2, pcls->tname);     //将课程结点信息复制进临时创建的课程结点
                        memcpy(buf3, pcls->wk_st, sizeof(buf3));    memcpy(buf4, pcls->wk_ed, sizeof(buf4));
                        buf5=pcls->cou_st;  buf6=pcls->cou_ed;
                        //在班级下属数组中删除相关时间段
                        DelTimeNode(pcls->wk_st,pcls->wk_ed,pcls->cou_st,pcls->cou_ed,&(psch->week[j].ochead));
                        SeekRemClass(psch, cbuffer);    //检查目标课程是否还在该班级下有课堂
                        //在教室结构中删除目标课程占位时间
                        for(l=0;l<rolist->length;l++)
                            if(!strcmp(rolist->elem[l].code, cbuf1))    break;
                        if(l!=rolist->length)
                            DelTimeNode(buf3,buf4,buf5,buf6,&(rolist->elem[l].ochead[j]));
                        //在教师结构中删除占位时间
                        for(l=0;l<telist->length;l++)
                            if(!strcmp(telist->elem[l].tname, cbuf2))   break;
                        if(l!=telist->length)
                            DelTimeNode(buf3,buf4,buf5,buf6,&(telist->elem[l].ochead[j]));
                        //删除目标课程
                        DelClassNode(pcls, psch, (unsigned short)j);   //删除课程结点
                    }
        }
	for(j=i; j<cougraph->length-1; j++) //顶点v后面的顶点前移
		cougraph->coulist[j] = cougraph->coulist[j+1];
	cougraph->length--;     printf("删除成功");
	if(cougraph->length==0)        printf("图中已无顶点");
	getchar();  ClearScreen();
    return TRUE;
}

BOOL DelGroup(void){    //pass
    char cbuf3[20];
    fflush(stdin);
    Schedule *psch=NULL, *qsch=NULL;
    Class *pcls=NULL;
    printf("将删除班级下所有课堂\n继续? 是--Y\t否--其它任意键\n");
    j=getchar();    getchar();
    if(j!='y' && j!='Y'){
        ClearScreen();    return TRUE;
    }
    printf("请输入待删班级名称:");
    psch=ClassInput(cbuf3);
    if(psch==NULL){
        printf("该班级不存在");
        getchar();   ClearScreen();    return TRUE;
    }
    for(i=0;i<6;i++){
        for(pcls=psch->week[i].classhead;pcls!=NULL;pcls=psch->week[i].classhead){   //删除班级下所有课堂
            psch->week[i].classhead=pcls->next;     free(pcls);
        }
        for(pocc=psch->week[i].ochead.head;pocc!=NULL;pocc=psch->week[i].ochead.head){      //删除班级下所有时间占位结点
            psch->week[i].ochead.head=pocc->next;   free(pocc);
        }
    }
    //根据年级找到班级节点并删除
    if(strstr(cbuf3,"15")){
        if(psch==grade[0].schead){  //目标班级为头结点
            grade[0].schead=psch->next;     free(psch);
        }
        else{   //目标班级非头结点
            for(qsch=grade[0].schead;qsch->next!=psch;qsch=qsch->next)  ;
            qsch->next=psch->next;  free(psch);
        }
        grade[0].gr_num--;
        if(grade[0].gr_num==0)  printf("2015级已无班级");
    }
    else if(strstr(cbuf3,"16")){  //目标班级为头结点
        if(psch==grade[1].schead){
            grade[1].schead=psch->next;     free(psch);
        }
        else{   //目标班级非头结点
            for(qsch=grade[1].schead;qsch->next!=psch;qsch=qsch->next)  ;
            qsch->next=psch->next;  free(psch);
        }
        grade[1].gr_num--;
        if(grade[1].gr_num==0)  printf("2016级已无班级");
    }
    else if(strstr(cbuf3,"17")){  //目标班级为头结点
        if(psch==grade[2].schead){
            grade[2].schead=psch->next;     free(psch);
        }
        else{   //目标班级非头结点
            for(qsch=grade[2].schead;qsch->next!=psch;qsch=qsch->next)  ;
            qsch->next=psch->next;  free(psch);
        }
        grade[2].gr_num--;
        if(grade[2].gr_num==0)  printf("2017级已无班级");
    }
    printf("删除成功");
    getchar();  ClearScreen();
    return TRUE;
}

BOOL AltTeacher(void){  //pass
    fflush(stdin);
    if(telist->length==0){
        printf("教师名单为空");
        getchar();   ClearScreen();    return TRUE;
    }
    printf("请输入待改教师姓名:");
    scanf("%s", cbuffer);   getchar();
    for(i=0;i<telist->length;i++)
        if(!strcmp(telist->elem[i].tname,cbuffer))    break;    //i保存目标教师在清单中的位置
    if(i==telist->length){
        printf("未找到相应教师");
        getchar();   ClearScreen();    return TRUE;
    }
    count=0;
    //选择修改方式
    do{
        if(count++)     printf("选项越界,重新输入");
        printf("修改什么信息?\n1--姓名\t2--性别\t3--年龄\t4--回主菜单\n");
        scanf("%hu", &choice);   getchar();
        if(choice==4){ ClearScreen();  return TRUE;    }
    }while(choice<1 || choice>4);
    switch(choice){
        case 1:
            printf("请输入新教师姓名:");
            scanf("%s", cbuffer);   getchar();
            //验证新名字的合法性
            for(j=0;j<telist->length;j++){
                if(j==i)    continue;
                if(!strcmp(telist->elem[j].tname,cbuffer)){
                    printf("新教师已存在,将停止操作并返回主菜单");
                    getchar();   ClearScreen();    return TRUE;
                }
            }
            for(k=0;k<3;k++)     //修改所有有相应教师的课堂
                for(psch=grade[k].schead;psch!=NULL;psch=psch->next)
                    for(j=0;j<6;j++)
                        for(pcls=psch->week[j].classhead;pcls!=NULL;pcls=pcls->next)    //找到课堂并修改
                            if(!strcmp(telist->elem[i].tname,pcls->tname))   strcpy(pcls->tname,cbuffer);
            strcpy(telist->elem[i].tname, cbuffer);
            printf("修改成功");
            break;
        case 2:
            //性别只有两种可能，互相改动即可
            if(telist->elem[i].gender==0)      telist->elem[i].gender=1;
            else    telist->elem[i].gender=0;
            printf("性别修改成功,当前性别为");
            if(telist->elem[i].gender==0)   printf("女");
            else    printf("男");
            break;
        case 3:
            printf("请输入新年龄:");
            scanf("%hu", &telist->elem[i].age);   getchar();
            printf("修改成功");
        case 4:
            break;
    }
    getchar();  ClearScreen();
    return TRUE;
}

BOOL AltRoom(void){     //pass
    fflush(stdin);
    if(rolist->length==0){
        printf("教室名单为空");
        getchar();   ClearScreen();    return TRUE;
    }
    printf("请输入待改教室编号:");
    scanf("%s", cbuffer);   getchar();
    //查找目标教室
    for(i=0;i<rolist->length;i++)
        if(!strcmp(rolist->elem[i].code,cbuffer))    break;     //i保存目标教室在清单中的位置
    if(i==rolist->length){
        printf("未找到相应教室");
        getchar();   ClearScreen();    return TRUE;
    }
    //选择修改方式
    count = 0;
    do{
        if(count)     printf("选项越界,重新输入");
        printf("修改什么信息?\n1--编号\t2--容量\t3--回主菜单\n");
        scanf("%hu", &choice);   getchar();     count++;
        if(choice==3){ ClearScreen();  return TRUE;    }
    }while(choice<1 || choice>3);
    switch(choice){
        case 1:
            printf("请输入新教室编号:");
            scanf("%s", cbuffer);   getchar();
            //检查新编号的合法性
            for(j=0;j<rolist->length;j++){
                if(j==i)    continue;
                if(!strcmp(rolist->elem[j].code,cbuffer)){
                    printf("目标教室已存在,将停止操作并返回主菜单");
                    getchar();   ClearScreen();    return TRUE;
                }
            }
            for(k=0;k<3;k++)     //修改所有有相应教师的课堂
                for(psch=grade[k].schead;psch!=NULL;psch=psch->next)
                    for(j=0;j<6;j++)
                        for(pcls=psch->week[j].classhead;pcls!=NULL;pcls=pcls->next)
                            if(!strcmp(rolist->elem[i].code,pcls->roomcode))   strcpy(pcls->roomcode,cbuffer);
            //修改教室结点
            strcpy(rolist->elem[i].code, cbuffer);
            printf("修改成功");
            break;
        case 2:
            printf("请输入新容量:");
            scanf("%hu", &rolist->elem[i].rsize);   getchar();
            printf("修改成功");
        case 3:
            break;
    }
    getchar();  ClearScreen();
    return TRUE;
}

BOOL AltCourse(void){   //pass
    fflush(stdin);
    if(cougraph->length==0){
        printf("课程名单为空");
        getchar();   ClearScreen();    return TRUE;
    }
    printf("请输入待改课程名称:");
    scanf("%s", cbuffer);       getchar();
    //查找目标课程
    for(i=0;i<cougraph->length;i++)
        if(!strcmp(cougraph->coulist[i].name,cbuffer))    break;    //i保存目标课程在清单中的位置,之后不可用i做累加器
    if(i==cougraph->length){
        printf("未找到相应课程");
        getchar();   ClearScreen();    return TRUE;
    }
    //选择修改方式
    count = 0;
    do{
        if(count++)     printf("选项越界,重新输入");
        printf("修改什么信息?\n1--课程名称\t2--添加先修课程\t3--删除先修课程\t4--返回菜单\n");
        scanf("%hu", &choice);   getchar();
        if(choice==4){ ClearScreen();  return TRUE;    }
    }while(choice<1 || choice>4);
    switch(choice){
        case 1:
            printf("输入新名称:");
            scanf("%s", cbuffer);   getchar();
            //检查新课名的合法性
            for(j=0;j<cougraph->length;j++){
                if(j==i)    continue;
                if(!strcmp(cougraph->coulist[j].name,cbuffer)){
                    printf("该课程已存在");
                    getchar();   ClearScreen();    return TRUE;
                }
            }
            for(j=0;j<cougraph->length;j++){
                if(j==i)    continue;
                for(pcarc=cougraph->coulist[j].archead;pcarc!=NULL;pcarc=pcarc->nextarc)    //修改所有对应的先修课弧节点
                    if(!strcmp(cougraph->coulist[i].name,pcarc->prename))
                        strcpy(pcarc->prename,cbuffer);
            }
            for(k=0;k<3;k++)     //修改所有有相应课程的课堂
                for(psch=grade[k].schead;psch!=NULL;psch=psch->next)
                    for(j=0;j<6;j++)
                        for(pcls=psch->week[j].classhead;pcls!=NULL;pcls=pcls->next)
                            if(!strcmp(cougraph->coulist[i].name,pcls->coursename))   strcpy(pcls->coursename,cbuffer);
            //修改目标课程结点
            strcpy(cougraph->coulist[i].name, cbuffer);
            printf("修改成功");
            break;
        case 2:
            do{
                printf("输入新先修课程名称:");
                scanf("%s", cbuffer);   getchar();
                for(j=0;j<cougraph->length;j++){    //在课程查找表中查找对应课程
                    if(j==i)    continue;
                    if(!strcmp(cougraph->coulist[j].name, cbuffer))     break;
                }
                if(j==cougraph->length){
                    printf("目标课程不存在");
                    getchar();   ClearScreen();    return TRUE;
                }
                pcarc = cougraph->coulist[i].archead;
                while(pcarc){   //检查该课程是否为已有的先修课
                    if(!strcmp(pcarc->prename, cbuffer)){
                        printf("该先修课程已存在");
                        getchar();   ClearScreen();    return TRUE;
                    }
                    pcarc = pcarc->nextarc;
                }
                pcarc = (CouArc *)malloc((sizeof(CouArc))); //插入先修课弧结点
                pcarc->prename=(char *)malloc(sizeof(cbuffer));
                strcpy(pcarc->prename, cbuffer);
                //新节点插入先修课链表
                pcarc->nextarc = cougraph->coulist[i].archead;
                cougraph->coulist[i].archead = pcarc;
                cougraph->coulist[i].arcnum++;
                printf("添加成功\n继续? 是--Y\t否--其他键");
                j=getchar();    getchar();
            }while(j=='Y' || j=='y');
            break;
        case 3:
            if(cougraph->coulist[i].arcnum==0){
                printf("该课程无先修课");
                getchar();   ClearScreen();    return TRUE;
            }
            printf("输入待删先修课程名称:");
            scanf("%s", cbuffer);   getchar();
            pcarc = cougraph->coulist[i].archead;
            while(pcarc){   //查找该课程是否为已有的先修课
                if(!strcmp(pcarc->prename, cbuffer))    break;
                pcarc = pcarc->nextarc;
            }
            if(!pcarc){
                printf("未找到相应课程");
                getchar();   ClearScreen();    return TRUE;
            }
            if(pcarc == cougraph->coulist[i].archead){ //待删结点是头结点
                cougraph->coulist[i].archead = pcarc->nextarc;
                free(pcarc->prename);   free(pcarc);
            }
            else{   //待删结点非头结点
                for(qcarc=cougraph->coulist[i].archead;qcarc->nextarc!=pcarc;qcarc=qcarc->nextarc)  ;   //q移动至p的前驱
                qcarc->nextarc = pcarc->nextarc;    free(pcarc->prename);   free(pcarc);
            }
            cougraph->coulist[i].arcnum--;
            printf("删除成功");
            break;
        case 4:
            return TRUE;
    }
    getchar();  ClearScreen();
    return TRUE;
}

BOOL AltGroup(void){    //pass
    char cbuf3[20];
    fflush(stdin);
    printf("请输入待改班级名称:");
    psch=ClassInput(cbuf3);
    if(psch==NULL){
        printf("该班级不存在");
        getchar();   ClearScreen();    return TRUE;
    }
    //选择修改方式
    count = 0;
    do{
        if(count)     printf("选项越界,重新输入");
        printf("修改什么信息?\n1--班号\t2--添加已修课程\t3--修改已修课程\t4--删除已修课程\t5--人数\t6--返回菜单\n");
        scanf("%hu", &hubuffer);   getchar();   count++;
        if(choice==6){ ClearScreen();  return TRUE;    }
    }while(hubuffer<1 || hubuffer>6);
    switch(hubuffer){
        case 1:
            flag=TRUE;
            do{
                printf("请输入新班号:");
                scanf("%s", cbuf2);   getchar();
                //验证新班号合法性
                if((strstr(cbuf3,"15") && !strstr(cbuf2,"15")) || (strstr(cbuf3,"16") && !strstr(cbuf2,"16")) || (strstr(cbuf3,"17") && !strstr(cbuf2,"17"))){
                    printf("不可跨年级修改班号\n");      flag=FALSE;
                }
            }while(flag==FALSE);
            //按班号确定验证头结点
            if(strstr(cbuf2,"15"))        qsch=grade[0].schead;
            else if(strstr(cbuf2,"16"))   qsch=grade[1].schead;
            else if(strstr(cbuf2,"17"))   qsch=grade[2].schead;
            //验证新班号存在性
            for(;qsch!=NULL;qsch=qsch->next){
                if(!strcmp(qsch->classname,cbuf2)){
                    printf("目标班级已存在");
                    getchar();   ClearScreen();    return TRUE;
                }
            }
            //修改班号
            strcpy(psch->classname, cbuf2);
            printf("修改成功");
            break;
        case 2:
            printf("请输入新已修课程名称:");
            scanf("%s", cbuffer);   getchar();
            //验证新课程存在性
            for(i=0; i<psch->do_num; i++)
                if(!strcmp(psch->done[i], cbuffer)){
                    printf("该课程已存在");
                    getchar();   ClearScreen();    return TRUE;
                }
            //添加课程
            strcpy(psch->done[psch->do_num], cbuffer);
            psch->do_num++;
            printf("添加成功");
            break;
        case 3:
            printf("输入待改已修课程名称:");
            scanf("%s", cbuffer);   getchar();
            //验待改课程存在性
            for(i=0; i<psch->do_num; i++)
                if(!strcmp(psch->done[i], cbuffer))   break;
            if(j==psch->do_num){
                printf("未找到相应课程");
                getchar();   ClearScreen();    return TRUE;
            }
            printf("请输入新已修课程名称:");
            scanf("%s", cbuffer);   getchar();
            //验证新班号存课程名称合法性
            for(j=0;j<psch->do_num;j++){
                if(!strcmp(psch->done[j], cbuffer)){
                    printf("该课程已存在");
                    getchar();   ClearScreen();    return TRUE;
                }
            }
            //修改课程
            strcpy(psch->done[i], cbuffer);
            printf("修改成功");
            break;
        case 4:
            if(psch->do_num==0){
                printf("无已修课程");   break;
            }
            printf("请输入待删已修课程:");
            scanf("%c", cbuffer);   getchar();
            //验待删课程存在性
            for(i=0; i<psch->do_num; i++)
                if(!strcmp(psch->done[i], cbuffer))   break;
            if(i==psch->do_num){
                printf("未找到相应课程");
                getchar();   ClearScreen();    return TRUE;
            }
            //删除课程
            for(j=i;j<psch->do_num-1;j++)
                strcpy(psch->done[j],psch->done[j+1]);
            psch->do_num--;
            if(psch->do_num==0)   printf("表中已无已修课程");
            printf("删除成功");     getchar();
            break;
        case 5:
            printf("请输入新人数:");
            scanf("%hu", &(psch->clsize));   getchar();
            printf("修改成功");
        /*屏蔽修改在修课程的接口
        case 5:
            printf("请输入新在修课程名称:");
            scanf("%s", cbuffer);   getchar();
            for(i=0; i<psch->pr_num; i++){
                if(!strcmp(psch->progress[i], cbuffer)){
                    printf("该课程已存在");   return FALSE;
                }
            }
            strcpy(psch->progress[psch->pr_num], cbuffer);
            psch->pr_num++;
            printf("添加成功");
            break;
        case 6:
            if(psch->pr_num==0){
                printf("已无在修课程");   break;
            }
            printf("请输入待改在修课程名称:");
            scanf("%s", cbuffer);   getchar();
            for(i=0; i<psch->pr_num; i++)
                if(!strcmp(psch->progress[i],cbuffer))   break;
            if(i==psch->pr_num){
                printf("未找到相应课程");  return FALSE;
            }
            printf("请输入新在修课程名称:");
            scanf("%s", cbuffer);   getchar();
            for(j=0;j<psch->pr_num;j++){
                if(!strcmp(psch->progress[j], cbuffer)){
                    printf("该课程已存在");   return FALSE;
                }
            }
            strcpy(psch->progress[i], cbuffer);
            printf("修改成功");
            break;
        case 7:
            if(psch->pr_num==0){
                printf("无在修课程");   break;
            }
            printf("请输入待删在修课程名称:");
            scanf("%s", cbuffer);   getchar();
            for(i=0; i<psch->pr_num; i++)
                if(!strcmp(psch->progress[i],cbuffer))   break;
            if(i==psch->pr_num){
                printf("未找到相应课程");  return FALSE;
            }
            for(j=i;j<psch->pr_num-1;j++)
                strcpy(psch->progress[j], psch->progress[j+1]);
            printf("删除成功");
            psch->pr_num--;
            if(psch->pr_num==0)   printf("表中已无已修课程");
            break;
        */
        case 6:
            break;
    }
    getchar();  ClearScreen();
    return TRUE;
}

BOOL SeekTeacher(void){     //pass
    fflush(stdin);
    if(telist->length==0){
        printf("教师名单为空");
        getchar();   ClearScreen();    return TRUE;
    }
    count=0;     flag=FALSE;
    do{
        if(count)     printf("选项越界,重新输入");
        printf("查找方式:\n1--姓名\t2--年龄区间\t3--性别\t4--回主菜单\n");
        scanf("%hu", &choice);   getchar();     count++;
        if(choice==4){ ClearScreen();  return TRUE;    }
    }while(choice<1 || choice>4);
    switch(choice){
        case 1: //按姓名查找
            printf("请输入教师姓名:");
            scanf("%s", cbuffer);   getchar();
            for(i=0;i<telist->length;i++)
                if(!strcmp(telist->elem[i].tname,cbuffer))    break;
            if(i==telist->length){
                printf("未找到相应教师");
                getchar();   ClearScreen();    return TRUE;
            }
            printf("性别:");
            if(telist->elem[i].gender==0)   printf("女");
            else    printf("男");
            printf("年龄:%hu\n查找成功",telist->elem[i].age);
            break;
        case 2:     //按年龄区间查找
            printf("输入年龄下限:");
            scanf("%hu", &buf1);   getchar();
            printf("输入年龄上限:");
            scanf("%hu", &buf2);   getchar();
            for(i=0;i<telist->length;i++)
                if(telist->elem[i].age>=buf1 && telist->elem[i].age<=buf2){
                    printf("姓名:%s\t年龄:%hu\t性别:",telist->elem[i].tname,telist->elem[i].age);
                    flag=TRUE;
                    if(telist->elem[i].gender==0)   printf("女\n");
                    else    printf("男\n");
                }
            if(flag==FALSE)     printf("无符合要求的教师");     getchar();
            break;
        case 3:     //按性别查找
            printf("输入目标性别(男--1\t女--0):");
            j=getchar();    getchar();
            for(i=0;i<telist->length;i++){
                if(j=='0'){
                    if(!telist->elem[i].gender){
                        printf("姓名:%s\t年龄:%hu\n",telist->elem[i].tname,telist->elem[i].age);
                        flag=TRUE;      //flag标记输出了有效信息
                    }   }
                else{
                    if(telist->elem[i].gender){
                        printf("姓名:%s\t年龄:%hu\n",telist->elem[i].tname,telist->elem[i].age);
                        flag=TRUE;      //flag标记输出了有效信息
                    }   }   }
            if(flag==FALSE)     printf("无符合要求的教师");     getchar();
        case 4:
            break;
    }
    getchar();  ClearScreen();
    return TRUE;
}


BOOL SeekRoom(void){    //pass
    fflush(stdin);
    if(rolist->length==0){
        printf("教室名单为空");
        getchar();   ClearScreen();    return TRUE;
    }
    count=0;
    do{
        if(count)     printf("选项越界,重新输入");
        printf("查找方式:\n1--编号\t2--容量\t3--回主菜单\n");
        scanf("%hu", &choice);   getchar();     count++;
        if(choice==3){ ClearScreen();  return TRUE;    }
    }while(choice<1 || choice>3);
    switch(choice){
        case 1: //按教室编号查找
            printf("请输入教室编号:");
            scanf("%s", cbuffer);   getchar();
            for(i=0;i<rolist->length;i++)
                if(!strcmp(rolist->elem[i].code,cbuffer))    break;
            if(i==rolist->length){
                printf("未找到相应教室");
                getchar();   ClearScreen();    return TRUE;
            }
            printf("教室容量:%hu\n查找成功",rolist->elem[i].rsize);
            break;
        case 2:     //按教室容量查找
            flag=FALSE;
            printf("输入容量下限:");
            scanf("%hu", &buf1);   getchar();
            printf("输入容量上限:");
            scanf("%hu", &buf2);   getchar();
            for(i=0;i<rolist->length;i++)
                if(rolist->elem[i].rsize>=buf1 && rolist->elem[i].rsize<=buf2){
                    printf("编号:%s\t容量:%hu\n",rolist->elem[i].code,rolist->elem[i].rsize);
                    flag=TRUE;      //flag标记输出了有效信息
                }
            if(flag==FALSE)     printf("无符合要求的教室");     getchar();
        case 3:
            break;
    }
    getchar();  ClearScreen();
    return TRUE;
}

BOOL SeekCourse(void){    //pass
    fflush(stdin);
    BOOL glag=FALSE;
    if(cougraph->length==0){
        printf("课程名单为空");
        getchar();   ClearScreen();    return TRUE;
    }
    printf("请输入待查课程名称子串:");
    scanf("%s", cbuffer);   getchar();
    for(i=0; i<cougraph->length; i++)
        //模糊查找
        if(strstr(cougraph->coulist[i].name,cbuffer)){
            printf("课程名称:%s\n",cougraph->coulist[i].name);
            flag = FALSE;
            printf("先修课程:\n");
            for(pcarc=cougraph->coulist[i].archead;pcarc!=NULL;pcarc=pcarc->nextarc){
                printf("%s\n", pcarc->prename);
                flag = TRUE;    //flag标记输出了有效信息
            }
            if(flag==FALSE)     printf("该课无先修课程\n");
            flag = FALSE;
            printf("后继课程:\n");
            for(j=0; j<cougraph->length; j++){
                if(j==i)    continue;
                //查找方法：寻找当前课程是否有先修课为目标课程
                for(pcarc=cougraph->coulist[j].archead;pcarc!=NULL;pcarc = pcarc->nextarc)
                    if(!strcmp(pcarc->prename, cougraph->coulist[i].name)){
                        printf("%s\n", cougraph->coulist[j].name);
                        flag = TRUE;    //flag标记输出了有效信息
                    }
            }
            if(flag==FALSE)     printf("该课无后继课程\n");
            glag=TRUE;
        }
    if(glag==FALSE)        printf("未找到相应课程");
    getchar();  ClearScreen();
    return TRUE;
}

BOOL SeekGroup(void){   //pass
    char cbuf3[20];
    fflush(stdin);
    if(!grade[0].schead && !grade[1].schead && !grade[2].schead){
        printf("课表名单为空");   return FALSE;
    }
    count=0;
    do{
        if(count)     printf("选项越界,重新输入");
        printf("查找方式\n1--班号\t2--人数\t3--回主菜单\n");
        scanf("%hu", &choice);   getchar();     count++;
        if(choice==3){ ClearScreen();  return TRUE;    }
    }while(choice<1 || choice>3);
    switch(choice){
        case 1:     //按班号查找
            printf("请输入待查班级名称:");
            psch=ClassInput(cbuf3);
            if(psch==NULL){
                printf("该班级不存在");
                getchar();   ClearScreen();    return TRUE;
            }
            //输出班级相关信息
            printf("人数:%hu\n",psch->clsize);
            if(psch->do_num!=0){
                printf("已修课程清单:\n");
                for(j=0; j<psch->do_num; j++)
                    printf("%s\n", psch->done[j]);
                printf("共%hu门\n",psch->do_num);
            }
            else    printf("无已修课程\n");
            if(psch->pr_num!=0){
                printf("在修课程清单:\n");
                for(j=0; j<psch->pr_num; j++)
                    printf("%s\n", psch->progress[j]);
                printf("共%hu门\n",psch->pr_num);
            }
            else    printf("无在修课程\n");
            break;
        case 2:     //按班级人数查找
            flag=FALSE;
            printf("输入人数下限:");
            scanf("%hu", &buf1);   getchar();
            printf("输入人数上限:");
            scanf("%hu", &buf2);   getchar();
            for(i=0;i<3;i++)
                for(psch=grade[i].schead;psch!=NULL;psch=psch->next)
                    if(psch->clsize>=buf1 && psch->clsize<=buf2){
                        //输出班级相关信息
                        printf("班级:%s\t人数:%hu\n",psch->classname,psch->clsize);
                        if(psch->do_num!=0){
                            printf("已修课程清单:\n");
                            for(j=0; j<psch->do_num; j++)
                                printf("%s\n", psch->done[j]);
                            printf("共%hu门\n",psch->do_num);
                        }
                        else    printf("无已修课程\n");
                        if(psch->pr_num!=0){
                            printf("在修课程清单:\n");
                            for(j=0; j<psch->pr_num; j++)
                                printf("%s\n", psch->progress[j]);
                            printf("共%hu门\n",psch->pr_num);
                        }
                        else    printf("无在修课程\n");
                        flag=TRUE;  //flag标记输出了有效信息
                    }
            if(flag==FALSE)     printf("无符合要求的班级");     getchar();
        case 3:
            break;
    }
    getchar();  ClearScreen();
    return TRUE;
}

BOOL AddClass(void){    //pass
    fflush(stdin);
    Class *ptemp=(Class *)malloc(sizeof(Class));
    //课程名写入样本结构
    printf("输入课程名:");
    scanf("%s", ptemp->coursename);      getchar();
    for(i=0;i<cougraph->length;i++)
        if(!strcmp(cougraph->coulist[i].name,ptemp->coursename))   break;
    if(i==cougraph->length){    //新建课程(先修课节点未添加)
        AddNewCourse(ptemp->coursename);
        cougraph->length++;
        printf("新课程已导入\n");
    }
    //教室号写入样本结构
    printf("输入教室号:");
    scanf("%s", ptemp->roomcode);      getchar();       //多个教室的处理？
    for(k=0; k<rolist->length; k++)
        if(!strcmp(rolist->elem[k].code,ptemp->roomcode))   break;  //k存放目标教室在教室清单中的位置
    if(k==rolist->length)   AddNewRoom(ptemp->roomcode);//新建教室
    rolist->length++;
    //教师姓名写入样本结构
    printf("输入教师姓名:");
    scanf("%s", ptemp->tname);      getchar();      //多个教师的处理？
    for(l=0; l<telist->length; l++)
        if(!strcmp(telist->elem[l].tname,ptemp->tname))   break;    //l存放目标教师在教师清单中的位置
    if(l==telist->length)  AddNewTeacher(ptemp->tname);//新建教师
    telist->length++;
    //课堂信息存入临时结点，待确定班级后以其为模板向各个班级中复制结点
    printf("输入上课星期(阿拉伯数字):");
    scanf("%hu", &buf1);     getchar();
    memset(ptemp->wk_st,0,sizeof(ptemp->wk_st));
    memset(ptemp->wk_ed,0,sizeof(ptemp->wk_ed));
    TimeInput(&(ptemp->cou_st), &(ptemp->cou_ed), ptemp->wk_st, ptemp->wk_ed);
    count=0;
    printf("输入班级数量:");
    scanf("%hu", &hubuffer); getchar();
    for(i=0;i<hubuffer;i++){
        pcls=(Class *)malloc(sizeof(Class));
        //复制样本结构用于接入链表
        strcpy(pcls->coursename, ptemp->coursename);
        strcpy(pcls->roomcode, ptemp->roomcode);
        strcpy(pcls->tname, ptemp->tname);
        pcls->cou_st=ptemp->cou_st;     pcls->cou_ed=ptemp->cou_ed;
        memcpy(pcls->wk_st, ptemp->wk_st, sizeof(ptemp->wk_st));
        memcpy(pcls->wk_ed, ptemp->wk_ed, sizeof(ptemp->wk_ed));
        //输入班级信息
        printf("输入班号:");
        //按班号确定查找头结点
        scanf("%s", cbuffer);   getchar();
        if(strstr(cbuffer,"15")){
            //查找目标班级的存在性
            for(psch=grade[0].schead; psch!=NULL; psch=psch->next)
                if(!strcmp(psch->classname,cbuffer))    break;
            if(psch==NULL){ //在课表结构中插入新班级
                psch=(Schedule *)malloc(sizeof(Schedule));
                strcpy(psch->classname, cbuffer);
                psch->do_num=psch->pr_num=0;
                printf("输入人数:");
                scanf("%hu",&(psch->clsize));   getchar();
                psch->next=grade[0].schead;
                grade[0].schead=psch;
                for(j=0;j<6;j++){   //时间占位及单周课表结构初始化
                    psch->week[j].classhead=NULL;       psch->week[j].cl_num=0;
                    psch->week[j].ochead.head=NULL;     psch->week[j].ochead.len=0;
                }
                grade[0].gr_num++;
            }
        }
        else if(strstr(cbuffer,"16")){
            //查找目标班级的存在性
            for(psch=grade[1].schead; psch!=NULL; psch=psch->next)
                if(!strcmp(psch->classname,cbuffer))    break;
            if(psch==NULL){ //在课表结构中插入新班级
                psch=(Schedule *)malloc(sizeof(Schedule));
                strcpy(psch->classname, cbuffer);
                psch->do_num=psch->pr_num=0;
                psch->next=grade[1].schead;
                grade[1].schead=psch;
                for(j=0;j<6;j++){   //时间占位及单周课表结构初始化
                    psch->week[j].classhead=NULL;       psch->week[j].cl_num=0;
                    psch->week[j].ochead.head=NULL;     psch->week[j].ochead.len=0;
                }
                grade[1].gr_num++;
            }
        }
        //查找目标班级的存在性
        else if(strstr(cbuffer,"17")){
            for(psch=grade[2].schead; psch!=NULL; psch=psch->next)
                if(!strcmp(psch->classname,cbuffer))    break;
            if(psch==NULL){ //在课表结构中插入新班级
                psch=(Schedule *)malloc(sizeof(Schedule));
                strcpy(psch->classname, cbuffer);
                psch->do_num=psch->pr_num=0;
                psch->next=grade[2].schead;    grade[2].schead=psch;
                for(j=0;j<6;j++){   //时间占位及单周课表结构初始化
                    psch->week[j].classhead=NULL;       psch->week[j].cl_num=0;
                    psch->week[j].ochead.head=NULL;     psch->week[j].ochead.len=0;
                }
                grade[2].gr_num++;
            }
        }
        else{   //截住乱码
            printf("班号输入错误");
            getchar();   ClearScreen();    return TRUE;
        }
        for(j=0;j<psch->pr_num;j++)     //查找在修课单中是否有目标课程
            if(!strcmp(psch->progress[j],ptemp->coursename))    break;
        if(j==psch->pr_num){    //若无则添加该课
            strcpy(psch->progress[j],ptemp->coursename);
            psch->pr_num++;
        }
        //插入课程节点
        //特殊情况：当前本日无课或新课为一天中最先上的课
        if(psch->week[buf1-1].classhead==NULL || psch->week[buf1-1].classhead->cou_st > pcls->cou_st || psch->week[buf1-1].classhead->wk_st[0]>pcls->wk_st[0]){
            pcls->next=psch->week[buf1-1].classhead;
            psch->week[buf1-1].classhead=pcls;
        }
        else{   //在课程链表中部插入
            for(qcls=psch->week[buf1-1].classhead; qcls->next!=NULL; qcls=qcls->next)
                if(qcls->next->cou_st>pcls->cou_st || (qcls->next->cou_st==pcls->cou_st && qcls->next->wk_st[0]>pcls->wk_st[0])){
                    pcls->next=qcls->next;    qcls->next=pcls;  break;
                }
            if(qcls->next==NULL){   //在课程链表尾部插入
                pcls->next=qcls->next;    qcls->next=pcls;
            }
        }
        psch->week[buf1-1].cl_num++;
        //在班级下属数组中对相关时间段占位
        AddTimeNode(pcls->wk_st, pcls->wk_ed, pcls->cou_st, pcls->cou_ed, &(psch->week[buf1-1].ochead));
    }
    /*
    //在相应教师结点下插入时间占位节点
    AddTimeNode(ptemp->wk_st, ptemp->wk_ed, ptemp->cou_st, ptemp->cou_ed, &(telist->elem[l].ochead[buf1-1]));
     //在相应教室结点下插入时间占位节点
    AddTimeNode(ptemp->wk_st, ptemp->wk_ed, ptemp->cou_st, ptemp->cou_ed, &(rolist->elem[k].ochead[buf1-1]));
    */
    count=0;
    while(ptemp->wk_st[count] && count<5){
        for(i=ptemp->wk_st[count];i<=ptemp->wk_ed[count];i++){
            for(j=ptemp->cou_st; j<=ptemp->cou_ed; j++){
                //按先周数后节数的升序插入时间占位节点
                pocc=(Occupy *)malloc(sizeof(Occupy));
                pocc->week=i;       pocc->time=j;
                if(telist->elem[l].ochead[buf1-1].len==0){  //无结点时插在第一位
                    pocc->next=telist->elem[l].ochead[buf1-1].head;     telist->elem[l].ochead[buf1-1].head=pocc;
                }
                else{
                    dwk=pocc->week - telist->elem[l].ochead[buf1-1].head->week;
                    dtm=pocc->time - telist->elem[l].ochead[buf1-1].head->time;
                    if(dwk<0 || (dwk>=0 && dtm<0)){ //新结点值最小
                        pocc->next=telist->elem[l].ochead[buf1-1].head;
                        telist->elem[l].ochead[buf1-1].head=pocc;
                    }
                    else{   //在时间占位链表中部插入
                        for(qocc=telist->elem[l].ochead[buf1-1].head;qocc->next!=NULL;qocc=qocc->next){
                            dwk=pocc->week-qocc->next->week;
                            dtm=pocc->time-qocc->next->time;
                            if(dwk<0 || (dwk>=0 && dtm<0)){
                                pocc->next=qocc->next;  qocc->next=pocc;
                            }
                        }
                        if(qocc->next==NULL){   //在时间占位链表尾部插入
                            pocc->next=qocc->next;  qocc->next=pocc;
                        }
                    }
                }
                telist->elem[l].ochead[buf1-1].len++;
            }
        }
        count++;
    }
     //在相应教室结点下插入时间占位节点
    count=0;
    while(ptemp->wk_st[count] && count<5){
        for(i=ptemp->wk_st[count];i<=ptemp->wk_ed[count];i++){
            for(j=ptemp->cou_st; j<=ptemp->cou_ed; j++){
                //按先周数后节数的升序插入时间占位节点
                pocc=(Occupy *)malloc(sizeof(Occupy));
                pocc->week=i;       pocc->time=j;
                //在教师结构中添加占位时间
                if(rolist->elem[k].ochead[buf1-1].len==0){  //无结点时插在第一位
                    pocc->next=rolist->elem[k].ochead[buf1-1].head;     rolist->elem[k].ochead[buf1-1].head=pocc;
                }
                else{
                    dwk=pocc->week - rolist->elem[k].ochead[buf1-1].head->week;
                    dtm=pocc->time - rolist->elem[k].ochead[buf1-1].head->time;
                    if(dwk<0 || (dwk>=0 && dtm<0)){ //新结点值最小
                        pocc->next=rolist->elem[k].ochead[buf1-1].head;
                        rolist->elem[k].ochead[buf1-1].head=pocc;
                    }
                    else{   //在时间占位链表中部插入
                        for(qocc=rolist->elem[k].ochead[buf1-1].head;qocc->next!=NULL;qocc=qocc->next){
                            dwk=pocc->week-qocc->next->week;
                            dtm=pocc->time-qocc->next->time;
                            if(dwk<0 || (dwk>=0 && dtm<0)){
                                pocc->next=qocc->next;  qocc->next=pocc;
                            }
                        }
                        if(qocc->next==NULL){   //在时间占位链表尾部插入
                            pocc->next=qocc->next;  qocc->next=pocc;
                        }   }   }
                rolist->elem[k].ochead[buf1-1].len++;
                //在教室结构中添加占位时间
            }
        }
        count++;
    }
    printf("输入完毕");
    getchar();  ClearScreen();
    return TRUE;
}

BOOL DelClass(void){    //pass
    fflush(stdin);
    char cbuf3[20];
    unsigned short buf3[5], buf4[5], buf5=0, buf6=0;
    printf("输入待删课堂课程名:");
    scanf("%s", cbuffer);   getchar();
    printf("输入待删课堂班级数目:");
    scanf("%hu", &hubuffer); getchar();
    printf("输入上课日期(阿拉伯数字)");
    scanf("%hu", &buf1); getchar();
    for(i=0;i<hubuffer;i++){
        count=0;
        printf("输入班号:");
        psch=ClassInput(cbuf3);
        if(psch==NULL){
            printf("未找到该班级,继续操作下一班级");   getchar();      continue;
        }
        //删除目标课程
        for(pcls=psch->week[buf1-1].classhead;pcls!=NULL;pcls=pcls->next)
            if(!strcmp(pcls->coursename, cbuffer))  break;
        if(pcls==NULL){
            printf("未在该日期下找到相应课程\n");     break;
        }
        //复制结点信息
        strcpy(cbuf1, pcls->roomcode);      strcpy(cbuf2, pcls->tname);
        memcpy(buf3, pcls->wk_st, sizeof(buf3));    memcpy(buf4, pcls->wk_ed, sizeof(buf4));
        buf5=pcls->cou_st;  buf6=pcls->cou_ed;
        //在班级下属数组中删除相关时间段
        DelTimeNode(pcls->wk_st,pcls->wk_ed,pcls->cou_st,pcls->cou_ed,&(psch->week[buf1-1].ochead));
        DelClassNode(pcls, psch, buf1-1);   //删除课程结点
    }
    //在教室结构中删除占位时间结点
    for(i=0;i<rolist->length;i++)
        if(!strcmp(rolist->elem[i].code, cbuf1))    break;
    if(i!=rolist->length)
        DelTimeNode(buf3,buf4,buf5,buf6,&(rolist->elem[i].ochead[buf1-1]));
    //在教师结构中删除占位时间
    for(i=0;i<telist->length;i++)
        if(!strcmp(telist->elem[i].tname, cbuf2))   break;
    if(i!=telist->length)
        DelTimeNode(buf3,buf4,buf5,buf6,&(telist->elem[i].ochead[buf1-1]));
    printf("删除成功");
    getchar();  ClearScreen();
    return TRUE;
}

BOOL AltClass(void){    //pass
    fflush(stdin);
    unsigned short buf3[5], buf4[5], buf5, buf6, buf7, buf8[5], buf9[5];
    //buf1-buf4为新信息，buf6-buf9为旧信息
    char cbuf3[20];
    Class *pcls=NULL;
    Schedule *psch=NULL;
    printf("输入待改课堂课程名:");
    scanf("%s", cbuffer);   getchar();
    printf("输入待改课堂班级数目:");
    scanf("%hu", &hubuffer); getchar();
    printf("输入上课日(阿拉伯数字)");
    scanf("%hu", &buf5); getchar();     //以上信息用于在课表中定位课堂
    count=0;
    do{
        if(count)     printf("选项越界,重新输入");
        printf("修改什么信息?\n1--教室\t2--任课教师\t3--上课时间\t4--回主菜单\n");
        scanf("%hu", &choice);   getchar();     count++;
        if(choice==4){ ClearScreen();  return TRUE;    }
    }while(choice<1 || choice>4);
    switch(choice){
        case 1:     //改教室
            printf("输入新教室:");
            scanf("%s",cbuf2);  getchar();
            for(l=0;l<rolist->length;l++)
                if(!strcmp(rolist->elem[l].code,cbuf2))   break;
            if(l==rolist->length)      AddNewRoom(cbuf2);//改成新教室且越界时线性表扩容
            rolist->length++;
            //l记录查找表中新教室位置
            break;
        case 2:     //改教师
            printf("输入新教师姓名:");
            scanf("%s",cbuf2);  getchar();
            for(l=0; l<telist->length; l++)
                if(!strcmp(telist->elem[l].tname,cbuf2))   break;
            if(l==telist->length)  AddNewTeacher(cbuf2);//新建教师
            telist->length++;
            //l记录查找表中新教师位置
            break;
        case 3:     //改时间
            memset(buf3,0,sizeof(buf3));
            memset(buf4,0,sizeof(buf4));
            TimeInput(&buf1, &buf2, buf3, buf4);
            break;
        case 4:
            return TRUE;
    }
    for(i=0;i<hubuffer;i++){
        printf("输入班号:");
        psch=ClassInput(cbuf3);
        if(psch==NULL){
            printf("未找到%s班,继续查找下个班级\n",cbuf3);   continue;
        }
        //检查输入课堂存在性
        for(pcls=psch->week[buf5-1].classhead; pcls!=NULL; pcls=pcls->next)
            if(!strcmp(pcls->coursename, cbuffer))    break;
        if(pcls==NULL){
            printf("未找到目标课堂,继续查找下个班级\n");  continue;
        }
        buf6=pcls->cou_st;  buf7=pcls->cou_ed;
        memcpy(buf8, pcls->wk_st, sizeof(pcls->wk_st));
        memcpy(buf9, pcls->wk_ed, sizeof(pcls->wk_ed)); //原上课时段放入缓存
        switch(choice){
            case 1: //修改教室占用情况
                strcpy(cbuf3, pcls->roomcode);      //cbuf3为旧信息，cbuf2为新信息
                strcpy(pcls->roomcode,cbuf2);   break;
            case 2: //修改教师占用情况
                strcpy(cbuf3, pcls->tname);
                strcpy(pcls->tname,cbuf2);   break;
            case 3: //修改时间占用情况
                //删除目标课堂占用的原时段
                DelTimeNode(pcls->wk_st,pcls->wk_ed,pcls->cou_st,pcls->cou_ed,&(psch->week[buf5-1].ochead));
                strcpy(cbuf2, pcls->roomcode);      strcpy(cbuf3, pcls->tname);
                //两个缓冲串用于在对应的教室及教师结构下修改时间占位
                pcls->cou_st=buf1; pcls->cou_ed=buf2;
                memcpy(pcls->wk_st, buf3, sizeof(buf3));
                memcpy(pcls->wk_ed, buf4, sizeof(buf4));    //在目标课堂中更改上课时段
                AddTimeNode(pcls->wk_st, pcls->wk_ed, pcls->cou_st, pcls->cou_ed, &(psch->week[buf5-1].ochead));
        }
    }
    flag=FALSE;
    switch(choice){
        case 1: //修改教室占用情况
            for(j=0;j<rolist->length;j++)
                if(!strcmp(rolist->elem[j].code, cbuf3))   break;
            //删除目标课堂对原教室的占用
            if(j!=rolist->length)
                DelTimeNode(buf8,buf9,buf6,buf7,&(rolist->elem[j].ochead[buf5-1]));
            //添加目标课堂对新教室的占用
            AddTimeNode(buf8, buf9, buf6, buf7, &(rolist->elem[l].ochead[buf5-1]));
            break;
        case 2: //修改教师占用情况
            for(j=0;j<telist->length;j++)
                if(!strcmp(telist->elem[j].tname, cbuf3))     break;
            //删除目标课堂对原教师的占用
            if(j!=telist->length)
                DelTimeNode(buf8,buf9,buf6,buf7,&(telist->elem[j].ochead[buf5-1]));
            //添加目标课堂对新教师的占用
            AddTimeNode(buf8, buf9, buf6, buf7, &(telist->elem[l].ochead[buf5-1]));
            break;
        case 3:
            for(l=0;l<rolist->length;l++)
                if(!strcmp(rolist->elem[l].code, cbuf2))   break;
            //删除目标教室的原占用时间
            if(l!=rolist->length)
                DelTimeNode(buf8,buf9,buf6,buf7,&(rolist->elem[l].ochead[buf5-1]));
            //添加目标教室的新占用时间
            AddTimeNode(buf3, buf4, buf1, buf2, &(rolist->elem[l].ochead[buf5-1]));
            for(l=0;l<telist->length;l++)
                if(!strcmp(telist->elem[l].tname, cbuf3))     break;
            //删除目标教师的原占用时间
            if(l!=telist->length)
                DelTimeNode(buf8,buf9,buf6,buf7,&(telist->elem[l].ochead[buf5-1]));
            //添加目标教师的新占用时间
            AddTimeNode(buf3, buf4, buf1, buf2, &(telist->elem[l].ochead[buf5-1]));
            break;
    }
    printf("修改成功");
    getchar();  ClearScreen();
    return TRUE;
}

BOOL SeekClass(void){   //pass
    fflush(stdin);
    char cbuf3[20];
    unsigned short buf8;
    BOOL glag;
    count=0;
    do{
        if(count++)     printf("选项越界,重新输入");
        printf("按何种方式检索?\n1--任课教师\t2--班级\t\t3--教室+课程+时间\t4--时间\t\t5--返回主菜单\n");
        scanf("%hu", &hubuffer);   getchar();
        if(hubuffer==5){ ClearScreen();  return TRUE;    }
    }while(hubuffer<1 || hubuffer>5);
    flag=FALSE;
    switch(hubuffer){
        case 1:
            printf("输入任课教师:");
            scanf("%s", cbuffer);   getchar();
            for(i=0;i<3;i++){   //三个年级
                for(psch=grade[i].schead; psch!=NULL; psch=psch->next)
                    for(j=0;j<6;j++)    //每星期有课的六天
                        for(pcls=psch->week[j].classhead;pcls!=NULL;pcls=pcls->next)
                            //找到相应课程
                            if(!strcmp(pcls->tname, cbuffer)){
                                printf("课程:%s\t班级:%s\t上课教室:%s\n上课时间:", pcls->coursename,psch->classname,pcls->roomcode);
                                k=0;    flag=TRUE;  //flag标记输出了有效信息
                                while(pcls->wk_st[k]&&k<5){
                                    if(k)   putchar(',');
                                    printf("%hu-%hu",pcls->wk_st[k],pcls->wk_ed[k]);    k++;
                                }
                                printf("周,周%hu,%hu-%hu节\n",j+1,pcls->cou_st,pcls->cou_ed);
                            }
            }
            if(flag==FALSE)     printf("未找到相关课堂");
            break;
        case 2:
            printf("输入班号:");
            psch=ClassInput(cbuf3);
            if(psch==NULL){
                printf("未找到该班级");
                getchar();   ClearScreen();    return TRUE;
            }
            for(i=0;i<6;i++)
                for(pcls=psch->week[i].classhead;pcls!=NULL;pcls=pcls->next){
                    printf("课程:%s\t任课教师:%s\t上课教室:%s\n上课时间:", pcls->coursename,pcls->tname,pcls->roomcode);
                    j=0;    flag=TRUE;      //flag标记输出了有效信息
                    while(pcls->wk_st[j]&&j<5){
                        if(j)   putchar(',');
                        printf("%hu-%hu",pcls->wk_st[j],pcls->wk_ed[j]);    j++;
                    }
                    printf("周,周%hu,%hu-%hu节\n",i+1,pcls->cou_st,pcls->cou_ed);
                }
            if(flag==FALSE)                printf("未找到相关课堂");
            break;
        case 3:
            printf("输入待查课堂课程名:");
            scanf("%s", cbuffer);   getchar();
            printf("输入上课教室:");
            scanf("%s",cbuf1);  getchar();
            printf("输入上课星期:");
            scanf("%hu", &buf5);      getchar();
            memset(buf3,0,sizeof(buf3));
            memset(buf4,0,sizeof(buf4));
            TimeInput(&buf1, &buf2, buf3, buf4);
            for(i=0;i<3;i++){
                for(psch=grade[i].schead; psch!=NULL; psch=psch->next)
                    for(pcls=psch->week[buf5-1].classhead;pcls!=NULL;pcls=pcls->next){
                        hubuffer=0;//检查上课时间对应关系
                        hubuffer=(pcls->cou_st-buf1)+(pcls->cou_ed-buf2);
                        count=0;
                        while((pcls->wk_st[count]||buf3[count]||pcls->wk_ed[count]||buf4[count])&&count<5){     //上课周数数组元素有效
                            hubuffer=(pcls->wk_st[count]-buf3[count])+(pcls->wk_ed[count]-buf4[count]);
                            count++;
                        }
                        //全部信息核查完毕，且完全重合
                        if(!strcmp(pcls->coursename, cbuffer)&&!strcmp(pcls->roomcode,cbuf1)&&!hubuffer){
                            printf("班级:%s\t任课教师:%s\n", psch->classname,pcls->tname);
                            flag=TRUE;  //flag标记输出了有效信息
                        }
                    }
            }
            if(flag==FALSE)  printf("未找到相关课堂");
            break;
        case 4:
            printf("输入开始节数:");
            scanf("%hu", &buf1);      getchar();
            printf("输入结束节数:");
            scanf("%hu", &buf2);      getchar();
            printf("输入目标星期:");
            scanf("%hu", &buf8);      getchar();
            printf("输入开始周数:");
            scanf("%hu", &buf5);      getchar();
            printf("输入结束周数:");
            scanf("%hu", &buf6);      getchar();
            flag=FALSE;
            for(i=0;i<3;i++){
                for(psch=grade[i].schead; psch!=NULL; psch=psch->next)
                    for(pcls=psch->week[buf8-1].classhead;pcls!=NULL;pcls=pcls->next){
                        glag=FALSE;
                        hubuffer=count=0;//检查上课时间对应关系
                        if(pcls->cou_st>=buf1&&pcls->cou_ed<=buf2)  hubuffer=1;
                        //目标班级在指定时间范围内有课
                        while(pcls->wk_st[count]||pcls->wk_ed[count]){
                            if(buf5 >= pcls->wk_st[count] && buf6<= pcls->wk_ed[count]){
                                glag=TRUE;  break;  //flag标记输出了有效信息
                            }
                            count++;
                        }
                        //时间参数满足要求
                        if(hubuffer&&glag){
                            printf("课程:%s\t教室:%s\t班级:%s\t任课教师:%s\n", pcls->coursename,pcls->roomcode,psch->classname,pcls->tname);
                            flag=TRUE;  //flag标记输出了有效信息
                        }   }
            }
            if(flag==FALSE)  printf("未找到相关课堂");
        case 5:
            break;
    }
    getchar();  ClearScreen();
    return TRUE;
}

BOOL RoomUsage(void){
    fflush(stdin);
    unsigned short buf7, buf8;
    BOOL glag;
    count=0;
    do{
        if(count)   printf("请重新输入\n");
        printf("选择分析方式:\n1--空闲教室查找\t2--教室利用率\t3--教室使用合理性\t4--返回主菜单\n");
        scanf("%hu", &choice);    getchar();  count++;
        if(choice==4){ ClearScreen();  return TRUE;    }
    }while(choice<1 || choice>4);
    switch(choice){
        case 1:
            printf("输入目标周:");
            scanf("%hu", &hubuffer);    getchar();
            printf("输入目标星期:");
            scanf("%hu", &buf5);    getchar();
            printf("输入起始节:");
            scanf("%hu", &buf1);    getchar();
            printf("输入终止节:");
            scanf("%hu", &buf2);    getchar();
            glag=FALSE;
            count=0;
            for(i=0;i<rolist->length;i++){
                flag=FALSE;
                for(pocc=rolist->elem[i].ochead[buf5-1].head;pocc!=NULL;pocc=pocc->next){
                    if(pocc->week==hubuffer && pocc->time>=buf1 && pocc->time<=buf2){
                        flag=TRUE;      break;
                    }
                }
                if(flag==FALSE){
                    printf("%s\t", rolist->elem[i].code);     glag=TRUE;
                    count++;
                    if(!(count%5))      putchar('\n');
                }
            }
            if(glag==FALSE)     printf("该时间段内无满足要求的教室");
            break;
        case 2:
            printf("输入目标周区间(只有一周则始末周相同)\n起始周:");
            scanf("%hu", &buf1);    getchar();
            printf("结束周:");
            scanf("%hu", &buf2);    getchar();
            printf("输入目标日区间(只有一天则始末节相同)\n起始日:");
            scanf("%hu", &buf7);    getchar();
            printf("结束日:");
            scanf("%hu", &buf8);    getchar();
            printf("输入目标节区间(只有一周则始末节相同)\n起始节:");
            scanf("%hu", &buf5);    getchar();
            printf("结束节:");
            scanf("%hu", &buf6);    getchar();
            count=0;
            for(i=0;i<rolist->length;i++)
                for(j=buf7-1; j<buf8; j++)
                    for(pocc=rolist->elem[i].ochead[j].head;pocc!=NULL;pocc=pocc->next)
                        if(pocc->week>=buf1 && pocc->week<=buf2 && pocc->time>=buf5 && pocc->time<=buf6)    count++;
            printf("目标时间段内教室利用率为%f", count*1.0/(rolist->length*(buf2-buf1+1)*(buf8-buf7+1)*(buf6-buf5+1)));
            break;
        case 3:
            buf6=0;
            printf("输入上课教室:");
            scanf("%s",cbuf1);  getchar();
            for(i=0;i<rolist->length;i++)
                if(!strcmp(cbuf1,rolist->elem[i].code))     break;
            if(i==rolist->length){
                printf("未找到目标教室");
                getchar();   ClearScreen();    return TRUE;
            }
            printf("当前教室最大容量为%hu\n",rolist->elem[i].rsize);
            printf("输入待查课堂课程名:");
            scanf("%s", cbuffer);   getchar();
            printf("输入上课星期:");
            scanf("%hu", &buf5);      getchar();
            memset(buf3,0,sizeof(buf3));
            memset(buf4,0,sizeof(buf4));
            TimeInput(&buf1, &buf2, buf3, buf4);
            flag=FALSE;
            for(j=0;j<3;j++){
                for(psch=grade[j].schead; psch!=NULL; psch=psch->next)
                    for(pcls=psch->week[buf5-1].classhead;pcls!=NULL;pcls=pcls->next){
                        hubuffer=0;//检查上课时间对应关系
                        hubuffer=(pcls->cou_st-buf1)+(pcls->cou_ed-buf2);
                        count=0;
                        while((pcls->wk_st[count]||buf3[count]||pcls->wk_ed[count]||buf4[count])&&count<5){
                            hubuffer=(pcls->wk_st[count]-buf3[count])+(pcls->wk_ed[count]-buf4[count]);
                            count++;
                        }
                        if(!strcmp(pcls->coursename, cbuffer)&&!strcmp(pcls->roomcode,cbuf1)&&!hubuffer){
                            printf("班级:%s\t人数%hu\n", psch->classname,psch->clsize);
                            buf6+=psch->clsize;
                            flag=TRUE;
                        }
                    }
            }
            if(flag==FALSE){
                printf("未找到相关课堂");  break;
            }
            else{
                if(buf6 > rolist->elem[i].rsize)
                    printf("课堂超员,需更换更大的教室\n");
                else if(buf6/rolist->elem[i].rsize<=1 && buf6*1.0/rolist->elem[i].rsize>=0.6)
                    printf("座位紧缺,需更换更大的教室\n");
                else if(buf6*1.0/rolist->elem[i].rsize<=0.5)
                    printf("空余座位过多,请考虑合并课堂或搬至较小的教室\n");
                else
                    printf("教室合适\n");
            }
        case 4:
            break;
    }
    getchar();  ClearScreen();
    return TRUE;
}

BOOL ClassAnalysis(void){
    fflush(stdin);
    unsigned short occupy[22][6];
    char cbuf3[20];
    printf("输入待分析班级:");
    psch=ClassInput(cbuf3);
    if(psch==NULL){
        printf("未找到该班级");
        getchar();   ClearScreen();    return TRUE;
    }
    count=0;
    do{
        if(count)   printf("请重新输入\n");
        printf("选择分析方式:\n1--课程在每周内按早晚排布的合理性\t2--课程在每周内按日排布的合理性\n3--修课合理性\t4--返回主菜单\n");
        scanf("%hu", &choice);    getchar();  count++;
        if(choice==4){ ClearScreen();  return TRUE;    }
    }while(choice<1 || choice>4);
    switch(choice){
        case 1: //课程在每天内排布的合理性
            printf("输入本学期有效星期数:");
            scanf("%hu", &hubuffer);     getchar();
            for(j=1;j<hubuffer;j++){
                flag=TRUE;
                buf1=buf2=buf5=0;
                printf("第%d周",(int)j);
                for(i=0;i<6;i++)
                    for(pocc=psch->week[i].ochead.head;pocc!=NULL;pocc=pocc->next)
                        if(pocc->week==j){
                            if(pocc->time>=1 && pocc->time<=4)  buf1++;
                            else if(pocc->time>=5 && pocc->time<=8)  buf2++;
                            else if(pocc->time>=9 && pocc->time<=12)  buf5++;
                        }
                if(buf5>buf1/2 || buf5>buf2/2){
                    printf("晚课比重过大\n");     flag=FALSE;
                }
                if(flag==TRUE)  printf("课程在每一天内排布较合理\n");
            }
            break;
        case 2:
            printf("输入本学期有效星期数:");
            scanf("%hu", &hubuffer);     getchar();
            memset(occupy,0,sizeof(occupy));
            for(j=0;j<6;j++)
                for(pocc=psch->week[j].ochead.head;pocc!=NULL;pocc=pocc->next)
                    occupy[pocc->week-1][j]++;
            for(i=0;i<hubuffer;i++){
                flag=TRUE;
                printf("第%d周",(int)i+1);
                for(j=0;j<5;j++)
                    for(k=j;k<5;k++)
                        if(occupy[i][j]-occupy[i][k]>=6){
                            printf("周%d与周%d课程分布过于悬殊\n",j+1,k+1);    flag=FALSE;
                        }
                if(flag==TRUE)  printf("课程分布较合理\n");
            }
            buf1=buf2=buf5=buf6=0;
            for(i=1;i<6;i++){
                buf1+=occupy[0][i];     buf2+=occupy[1][i];
                buf5+=occupy[hubuffer-2][i];    buf6+=occupy[hubuffer-1][i];
            }
            if(buf1>=20)    printf("第1周课安排过多\n");
            if(buf2>=22)    printf("第2周课安排过多\n");
            if(buf5>=10)    printf("第%hu周课安排过多\n",hubuffer-1);
            if(buf6>=4)     printf("第%hu周课安排过多\n",hubuffer);
            break;
        case 3:
            for(i=0;i<psch->pr_num;i++){
                flag=FALSE;
                for(j=0;j<cougraph->length;j++)
                    if(!strcmp(psch->progress[i],cougraph->coulist[j].name))     break;
                if(j==cougraph->length){
                    printf("课程%s未被录入,直接分析下一课程\n",cougraph->coulist[j].name);    continue;
                }
                for(pcarc=cougraph->coulist[j].archead;pcarc!=NULL;pcarc=pcarc->nextarc){
                    for(k=0;k<psch->do_num;k++)
                        if(!strcmp(psch->done[k],pcarc->prename))   break;
                    if(k==psch->do_num){
                        printf("%s尚未修，本学期不应修%s\n",pcarc->prename,psch->progress[i]);
                        flag=TRUE;
                    }
                }
                if(flag==FALSE)     printf("课程%s可在本学期修\n",cougraph->coulist[j].name);
                putchar('\n');
            }
        case 4:
            break;
    }
    //课程在每周内排布的合理性
    getchar();  ClearScreen();
    return TRUE;
}

BOOL TeacherAnalysis(void){
    fflush(stdin);
    unsigned short occupy[22];
    printf("输入待分析教师:");
    scanf("%s",cbuffer);    getchar();
    for(i=0;i<telist->length;i++)
        if(!strcmp(telist->elem[i].tname,cbuffer))  break;
    if(i==telist->length){
        printf("未找到目标教师");
        getchar();   ClearScreen();    return TRUE;
    }
    count=0;
    do{
        if(count)     printf("选项越界,重新输入");
        printf("选择分析方式:\n1--教学任务按日分配\t2--教学任务分配量\t3--楼层安排合理性\t4回主菜单\n");
        scanf("%hu", &choice);   getchar();     count++;
        if(choice==4){ ClearScreen();  return TRUE;    }
    }while(choice<1 || choice>4);
    switch(choice){
        case 1:
            printf("输入本学期有效星期数:");
            scanf("%hu", &hubuffer);     getchar();
            for(j=1;j<=hubuffer;j++){
                flag=TRUE;
                printf("第%d周",(int)j);
                buf1=buf2=buf5=0;
                for(k=0;k<6;k++)
                    for(pocc=telist->elem[i].ochead[k].head;pocc!=NULL;pocc=pocc->next)
                        if(pocc->week==j){
                            if(pocc->time>=1 && pocc->time<=4)  buf1++;
                            else if(pocc->time>=5 && pocc->time<=8)  buf2++;
                            else if(pocc->time>=9 && pocc->time<=12)  buf5++;
                        }
                if(buf5>buf1/2 || buf5>buf2/2){   //在有课的基础上判断
                     printf("晚课比重过大\n");    flag=FALSE;
                }
                if(flag==TRUE)  printf("课程在每一天内排布较合理\n");
            }
            break;
        case 2:
            printf("输入本学期有效星期数:");
            scanf("%hu", &hubuffer);     getchar();
            memset(occupy,0,sizeof(occupy));
            for(j=0;j<6;j++)
                for(pocc=psch->week[j].ochead.head;pocc!=NULL;pocc=pocc->next)
                    occupy[pocc->week-1]++;
            flag=FALSE;
            for(j=0;j<hubuffer;j++)
                //退休年龄:男-55    女-60
                if((telist->elem[i].gender==0 && telist->elem[i].age<50) || (telist->elem[i].gender==1 && telist->elem[i].age<55)){
                    if(occupy[j]>14){
                        printf("第%hu周担负课堂过多\n",j+1);    flag=TRUE;
                    }
                }
                else{
                    if(occupy[j]>10){
                        printf("第%hu周担负课堂过多\n",j+1);    flag=TRUE;
                    }
                }
            if(flag==FALSE)     printf("该教师教学任务分配较为适宜");
            break;
        case 3:
            if((telist->elem[i].gender==0 && telist->elem[i].age<55) || (telist->elem[i].gender==1 && telist->elem[i].age<60)){
                 printf("该教师无特别楼层要求\n");    break;
            }
            else{
                flag=FALSE;
                for(j=0;j<3;j++){
                    for(psch=grade[j].schead;psch!=NULL;psch=psch->next){
                        for(k=0;k<6;k++){
                            for(pcls=psch->week[k].classhead;pcls!=NULL;pcls=pcls->next){
                                if(!strstr(pcls->tname,telist->elem[i].tname))  continue;
                                if(strstr(pcls->roomcode,"4") || strstr(pcls->roomcode,"5")){
                                    printf("%s所在教室%s的楼层过高\n",pcls->coursename,pcls->roomcode);
                                    flag=TRUE;
                                }   }   }   }   }
                if(flag==FALSE)     printf("该教师上课的楼层较为合理\n");
            }
        case 4:
            break;
    }
    getchar();  ClearScreen();
    return TRUE;
}

/*
BOOL SchedueIn(void){
    Schedule *psch=NULL;
    Class *pcls=NULL;
    char cls_stack[7][60], *pch, ;
    string sbuf1, sbuf2, send="\0", sstack[7];
    ifstream infile;
    unsigned short buf[2]={0,0};
    unsigned char i,j;
    sbuf1.assign("15_timetable_info.txt");
    infile.open(sbuf1.c_str());
    if(!infile){
        printf("打开15_timetable_info.txt失败");
    }
    while(1){
        getline(infile, sbuf1);
        if(sbuf1.find("班级")!=sbuf1.npos){
        else if(sbuf1.find(""))
            i=0;
            while(1){
                getline(infile, sbuf1);
                strcpy(cbuffer,sbuf1.c_str());
                if(strstr(cbuffer,"]")) break;
                strcpy(cls_stack[i++],sbuf1.c_str());
            }
            while(i){
                getline(infile, sbuf1);
                if(sbuf1.find("课程")!=sbuf1.npos){
                    getline(infile, sbuf1);
                    strcpy(cbuffer,sbuf1.c_str());
                    if(strstr(cbuffer,"]")) break;
                    //删除课程中的标点
                    for(j=0;j<cougraph->length;j++)
                        if(!strcmp(cougraph->coulist[j].name,cbuffer))  break;
                    if(j==cougraph->length){
                        strcpy(cougraph->coulist[cougraph->length].name, cbuffer);
                        cougraph->coulist[cougraph->length++].archead = NULL;
                        printf("先修课程待输入……");
                    }
                }
                getline(infile, sbuf1);
                if(sbuf1.find("课程表")!=sbuf1.npos){
                    for(psch=grade[0].schead;psch!=NULL;psch=psch->next)
                        if(!strcmp(psch->classname, cstack[--i]))   break;
                    if(psch==NULL){ //在课表结构中插入新班级
                        psch=(Schedule *)malloc(sizeof(Schedule));
                        strcpy(psch->classname, cstack[i]);
                        psch->next=grade[0].schead;     grade[0].schead=psch;
                        for(j=0;j<6;j++)
                            memset(psch->week[j].occupy,0,sizeof(psch->week[j].occupy));
                        grade[0].gr_num++;
                        getline(infile, sbuf1);
                        if(sbuf1.find("星期一"!=sbuf1.npos)){
                            getline(infile, sbuf1);
                            if(sbuf1.find("["!=sbuf1.npos)
                                pcls=(Class*)malloc(sizeof(Class));
                                memset(pcls->wk_ed,0,sizeof(pcls->wk_ed));
                                memset(pcls->wk_st,0,sizeof(pcls->wk_st));
                                while(1){
                                    getline(infile, sbuf1);
                                    if(sbuf1.find("name")!=sbuf1.npos){
                                        sbuf1.replace(sbuf1.find_last_of("\""),2,send);
                                        sbuf2=sbuf1.substr(21);
                                        strcpy(pcls->coursename,sbuf2.c_str());
                                    }
                                    else if(sbuf1.find("time")!=sbuf1.npos){
                                        strcpy(cbuffer,sbuf1.c_str());   j=0;
                                        for(k=0;cbuffer[k];i++)
                                            if(cbuffer[k]>='0' && cbuffer[k]<='9'){
                                                buf[j]=buf[j]*10+cbuffer[k]-'0';
                                                if(cbuffer[k+1]<='0' || cbuffer[k+1]>='9')    j++;
                                            }
                                        pcls->cou_st=buf[0];        pcls->cou_ed=buf[1];
                                    }
                                    else if(sbuf1.find("date")!=sbuf1.npos){
                                        strcpy(cbuffer,sbuf1.c_str());
                                        count=0;
                                        pch = strtok (cbuffer,"，");
                                        while (pch != NULL)
                                        {
                                            j=0;
                                            buf[0]=buf[1]=0;
                                            for(k=0;pch[k];k++)
                                                if(pch[k]>='0' && pch[k]<='9'){
                                                    buf[k]=buf[k]*10+pch[k]-'0';
                                                    if(pch[k+1]<='0' || pch[k+1]>='9')    j++;
                                                }
                                            if(buf[0]>buf[1])   buf[1]=buf[0];
                                            pcls->wk_st[count]=buf[0];  pcls->wk_ed[count]=buf[1];  count++;
                                            pch = strtok (NULL, "，");
                                        }
                                    }
                                    else if(sbuf1.find("classroom")!=sbuf1.npos){
                                        sbuf1= sbuf1.substr(26);
                                        sbuf1.replace(sbuf1.find_last_of("\""),2,send);
                                        strcpy(pcls->roomcode,sbuf1.c_str());
                                        if(sbuf1.find("£¬")!=sbuf1.npos){
                                            strcpy(cbuffer,sbuf1.c_str());
                                            pch = strtok(cbuffer,"£¬");
                                            while (pch != NULL)
                                            {
                                                strcpy(cstack[k++],pch);
                                                pch = strtok(NULL, "£¬");
                                            }
                                            for(j=0;j<k;j++){
                                                sstack[j].assign(cstack[j]);
                                            }
                                            flag=FALSE;
                                            for(j=0;j<sstack[0].length();j++){
                                                if(sstack[0][j]>='0'&&sstack[0][j]<='9'){
                                                    if(flag==FALSE) save=j;
                                                    flag=TRUE;
                                                    sstack[k].push_back(sstack[0][j]);
                                                }
                                            }
                                            sstack[0].erase(save);
                                            for(j=1;j<=k;j++)   sstack[j]=sstack[0]+sstack[j];
                                            for(j=0;j<=k;j++)   sstack[j]=sstack[j+1];
                                            for(j=0;j<=k;j++){
                                                if(rolist->length >= rolist->listsize){
                                                    Room *newbase = (Room *)realloc(rolist->elem, (rolist->listsize+LISTINCREMENT)* sizeof(Room));
                                                    if(!newbase){
                                                        printf("内存分配失败");
                                                        getchar();   return FALSE;
                                                    }
                                                    rolist->elem = newbase;
                                                    rolist->listsize += LISTINCREMENT;
                                                }
                                                for(l=0;l<rolist->length;l ++)
                                                    if(!sstack[j].compare(rolist->elem[l].code))   break;
                                                if(l==rolist->length){
                                                    strcpy(rolist->elem[rolist->length].code, sstack[j].c_str());
                                                    printf("请输入%s教室座位数:",sstack[j].c_str());
                                                    scanf("%hu",&rolist->elem[rolist->length].seat);    getchar();
                                                    memset(rolist->elem[rolist->length].occupy,0,sizeof(rolist->elem[rolist->length].occupy));
                                                    rolist->length++;
                                                }
                                            }
                                        }
                                    }
                                }
                        }
                    }
                    i--;
                }
            }
            cout << buff << endl;
        }
    }
    infile.close();
}
*/

BOOL FileOut(void){
    fflush(stdin);
    FILE *out;
    time_t lt;
    char *filename, *pc;
    const char *appe=".txt";
    lt=time(NULL);
    filename=ctime(&lt);
    filename[strlen(filename)-1]='\0';
    strcat(filename,appe);
    pc=&filename[0];
    while(*pc){
        if((*pc==':' ) ||   (*pc==' ')){   *pc='_';    }
        pc++;
    }
    if((out=fopen(filename,"w"))==NULL){
        printf("打开失败！\n");
        getchar();   ClearScreen();    return TRUE;
    }
    //教师清单
    fprintf(out, "%hu ", telist->length);
    for(i=0;i<telist->length;i++){
        fprintf(out, "%s ", telist->elem[i].tname);
        fprintf(out, "%hu ", telist->elem[i].gender);
        fprintf(out, "%hu ", telist->elem[i].age);
        for(j=0;j<6;j++){
            fprintf(out, "%hu ", telist->elem[i].ochead[j].len);
            for(pocc=telist->elem[i].ochead[j].head;pocc!=NULL;pocc=pocc->next){
                fprintf(out, "%hu ", pocc->time);
                fprintf(out, "%hu ", pocc->week);
            }
        }
    }
    //教室清单
    fprintf(out, "%hu ", rolist->length);
    for(i=0;i<rolist->length;i++){
        fprintf(out, "%s ", rolist->elem[i].code);
        fprintf(out, "%hu ", rolist->elem[i].rsize);
        for(j=0;j<6;j++){
            fprintf(out, "%hu ", rolist->elem[i].ochead[j].len);
            for(pocc=rolist->elem[i].ochead[j].head;pocc!=NULL;pocc=pocc->next){
                fprintf(out, "%hu ", pocc->time);
                fprintf(out, "%hu ", pocc->week);
            }
        }
    }
    //课程清单
    fprintf(out, "%hu ", cougraph->length);
    for(i=0;i<cougraph->length;i++){
        fprintf(out, "%s ",cougraph->coulist[i].name);
        fprintf(out, "%hu " ,cougraph->coulist[i].arcnum);
        for(pcarc=cougraph->coulist[i].archead;pcarc!=NULL;pcarc=pcarc->nextarc)
            fprintf(out, "%s ",pcarc->prename);
    }
    //班级+课表清单
    for(i=0;i<3;i++){
        fprintf(out, "%hu ",grade[i].gr_num);
        for(psch=grade[i].schead;psch!=NULL;psch=psch->next){
            fprintf(out, "%s ",psch->classname);
            fprintf(out, "%hu ",psch->clsize);
            fprintf(out, "%hu ",psch->do_num);
            fprintf(out, "%hu ",psch->pr_num);
            if(psch->do_num)
                for(j=0;j<psch->do_num;j++)
                    fprintf(out, "%s ",psch->done[j]);
            if(psch->pr_num)
                for(j=0;j<psch->pr_num;j++)
                    fprintf(out, "%s ",psch->progress[j]);
            for(j=0;j<6;j++){
                fprintf(out, "%hu ",psch->week[j].cl_num);
                for(pcls=psch->week[j].classhead;pcls!=NULL;pcls=pcls->next){
                    fprintf(out, "%s ",pcls->coursename);
                    fprintf(out, "%s ",pcls->roomcode);
                    fprintf(out, "%s ",pcls->tname);
                    fprintf(out, "%hu ",pcls->cou_st);
                    fprintf(out, "%hu ",pcls->cou_ed);
                    for(k=0;k<5;k++){
                        fprintf(out, "%hu ",pcls->wk_st[k]);
                        fprintf(out, "%hu ",pcls->wk_ed[k]);
                    }
                }
                fprintf(out, "%hu ",psch->week[j].ochead.len);
                for(pocc=psch->week[j].ochead.head;pocc!=NULL;pocc=pocc->next){
                    fprintf(out, "%hu ", pocc->time);
                    fprintf(out, "%hu ", pocc->week);
                }
            }
        }
    }
    fclose(out);
    printf("备份成功！按任意键继续...\n");
    getchar();  ClearScreen();
    return TRUE;
}

BOOL FileIn(void){
    fflush(stdin);
    FILE *in;
    printf("导入的数据将覆盖系统当前数据！继续——Y   返回主菜单——其它任意键\n");
    fflush(stdin);  j=getchar(); getchar();
    if(j!='y'&& j!='Y'){
        printf("数据导入取消");
        getchar();   ClearScreen();    return TRUE;
    }
    char input[80], *filename;
    printf("请输入您要恢复的数据所在的文件名:");
    fflush(stdin);      scanf("%s",input);    getchar();
    filename=input;
    //int handle,find;
    if ((in = fopen(filename, "r+")) == NULL)
    {
        printf("您输入的备份文件不存在,系统将保留当前数据");
        getchar();   ClearScreen();    return TRUE;
    }
    ClearMemory();
    Initial();
    fscanf(in, "%hu", &(telist->length));
    if(telist->length >= telist->listsize)
        buf1 = (telist->length-telist->listsize)/LISTINCREMENT+1;
    while(telist->length >= telist->listsize){
        Teacher *te_newbase = (Teacher *)realloc(telist->elem, (telist->listsize+buf1*LISTINCREMENT)* sizeof(Teacher));
        if(!te_newbase){
            printf("内存分配失败");   return TRUE;
        }
        telist->elem = te_newbase;
        telist->listsize += buf1*LISTINCREMENT;
    }
    for(i=0;i<telist->length;i++){
        fscanf(in, "%s", telist->elem[i].tname);
        fscanf(in, "%hu ", &(telist->elem[i].gender));
        fscanf(in, "%hu ", &(telist->elem[i].age));
        for(j=0;j<6;j++){
            fscanf(in, "%hu", &(telist->elem[i].ochead[j].len));
            telist->elem[i].ochead[j].head=NULL;
            for(k=0;k<telist->elem[i].ochead[j].len;k++){
                pocc = (Occupy *)malloc(sizeof(Occupy));
                fscanf(in, "%hu", &(pocc->time));
                fscanf(in, "%hu", &(pocc->week));
                pocc->next=telist->elem[i].ochead[j].head;
                telist->elem[i].ochead[j].head=pocc;
            }
        }
    }

    fscanf(in, "%hu", &(rolist->length));
    if(rolist->length >= rolist->listsize)
        buf1 = (rolist->length-rolist->listsize)/LISTINCREMENT+1;
    while(rolist->length >= rolist->listsize){
        Room *ro_newbase = (Room*)realloc(rolist->elem, (rolist->listsize+buf1*LISTINCREMENT)* sizeof(Room));
        if(!ro_newbase){
            printf("内存分配失败");   return TRUE;
        }
        rolist->elem = ro_newbase;
        rolist->listsize += buf1*LISTINCREMENT;
    }
    for(i=0;i<rolist->length;i++){
        fscanf(in, "%s", rolist->elem[i].code);
        fscanf(in, "%hu ", &(rolist->elem[i].rsize));
        for(j=0;j<6;j++){
            fscanf(in, "%hu", &(rolist->elem[i].ochead[j].len));
            rolist->elem[i].ochead[j].head=NULL;
            for(k=0;k<rolist->elem[i].ochead[j].len;k++){
                pocc = (Occupy *)malloc(sizeof(Occupy));
                fscanf(in, "%hu", &(pocc->time));
                fscanf(in, "%hu", &(pocc->week));
                pocc->next=rolist->elem[i].ochead[j].head;   //节点插入顺序反了？
                rolist->elem[i].ochead[j].head=pocc;
            }
        }
    }

    fscanf(in, "%hu", &(cougraph->length));
    if(cougraph->length >= cougraph->listsize)
        buf1 = (cougraph->length-cougraph->listsize)/LISTINCREMENT+1;
    while(cougraph->length >= cougraph->listsize){
        CouHead *cou_newbase = (CouHead*)realloc(cougraph->coulist, (cougraph->listsize+buf1*LISTINCREMENT)* sizeof(CouHead));
        if(!cou_newbase){
            printf("内存分配失败");   return TRUE;
        }
        cougraph->coulist = cou_newbase;
        cougraph->listsize += buf1*LISTINCREMENT;
    }
    for(i=0;i<cougraph->length;i++){
        fscanf(in, "%s", cougraph->coulist[i].name);
        fscanf(in, "%hu", &(cougraph->coulist[i].arcnum));
        cougraph->coulist[i].archead=NULL;
        for(j=0;j<cougraph->coulist[i].arcnum;j++){
            pcarc=(CouArc *)malloc(sizeof(CouArc));
            pcarc->prename=(char *)malloc(sizeof(cbuffer));
            fscanf(in, "%s", pcarc->prename);
            pcarc->nextarc=cougraph->coulist[i].archead;
            cougraph->coulist[i].archead=pcarc;
        }
    }

    for(i=0;i<3;i++){
        fscanf(in, "%hu", &(grade[i].gr_num));
        grade[i].schead=NULL;
        for(j=0;j<grade[i].gr_num;j++){
            psch=(Schedule *)malloc(sizeof(Schedule));
            fscanf(in, "%s", psch->classname);
            fscanf(in, "%hu ",&(psch->clsize));
            fscanf(in, "%hu", &(psch->do_num));
            fscanf(in, "%hu", &(psch->pr_num));
            if(psch->do_num)
                for(k=0;k<psch->do_num;k++)
                    fscanf(in, "%s", psch->done[k]);
            if(psch->pr_num)
                for(k=0;k<psch->pr_num;k++)
                    fscanf(in, "%s", psch->progress[k]);
            for(k=0;k<6;k++){
                fscanf(in, "%hu", &(psch->week[k].cl_num));
                psch->week[k].classhead=NULL;
                for(l=0;l<psch->week[k].cl_num;l++){
                    pcls=(Class *)malloc(sizeof(Class));
                    fscanf(in, "%s", pcls->coursename);
                    fscanf(in, "%s", pcls->roomcode);
                    fscanf(in, "%s", pcls->tname);
                    fscanf(in, "%hu", &(pcls->cou_st));
                    fscanf(in, "%hu", &(pcls->cou_ed));
                    for(m=0;m<5;m++){
                        fscanf(in, "%hu", &(pcls->wk_st[m]));
                        fscanf(in, "%hu", &(pcls->wk_ed[m]));
                    }
                    pcls->next=NULL;
                    if(psch->week[k].classhead==NULL)
                        psch->week[k].classhead=pcls;
                    else{
                        for(qcls=psch->week[k].classhead;qcls->next!=NULL;qcls=qcls->next)  ;
                        qcls->next=pcls;
                    }
                }
                fscanf(in, "%hu", &(psch->week[k].ochead.len));
                psch->week[k].ochead.head=NULL;
                for(l=0;l<psch->week[k].ochead.len;l++){
                    pocc = (Occupy *)malloc(sizeof(Occupy));
                    fscanf(in, "%hu", &(pocc->time));
                    fscanf(in, "%hu", &(pocc->week));
                    pocc->next=psch->week[k].ochead.head;
                    psch->week[k].ochead.head=pocc;
                }
            }
            psch->next=grade[i].schead;
            grade[i].schead=psch;
        }
    }
     fclose(in);
     printf("数据导入成功,按任意键继续...\n");
     getchar();  ClearScreen();
     return TRUE;
}

/**
 * 函数名称: MaintainTeacherlInfo
 * 函数功能: 教师信息维护目录
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型, 总是为TRUE
 *
 * 调用说明:
 */
BOOL MaintainTeacherInfo(void){
    BOOL bRet = TRUE;
    do{
        printf("您想进行何种操作？\n");
        printf("录入          press1\n修改          press2\n删除          press3\n返回主菜单    press4\n");
        fflush(stdin);  k=getchar(); getchar();
        if(k=='4')  { ClearScreen();  return TRUE;    }
    }while(k<'1'||k>'4');
    if(k=='1')        bRet = AddTeacher();
    else if(k=='2')   bRet = AltTeacher();
    else if(k=='3')   bRet = DelTeacher();
    return bRet;
}

/**
 * 函数名称: MaintainRoomInfo
 * 函数功能: 教室信息维护目录
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型, 总是为TRUE
 *
 * 调用说明:
 */
BOOL MaintainRoomInfo(void){
    BOOL bRet = TRUE;
    do{
        printf("您想进行何种操作？\n");
        printf("录入          press1\n修改          press2\n删除          press3\n返回主菜单    press4\n");
        fflush(stdin);  k=getchar(); getchar();
        if(k=='4')    { ClearScreen();  return TRUE;    }
    }while(k<'1'||k>'4');
    if(k=='1'){        bRet = AddRoom();  }
    else if(k=='2'){   bRet = AltRoom();  }
    else if(k=='3'){   bRet = DelRoom();  }
    return bRet;
}

/**
 * 函数名称: MaintainGroupInfo
 * 函数功能: 班级信息维护目录
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型, 总是为TRUE
 *
 * 调用说明:
 */
BOOL MaintainGroupInfo(void){
    BOOL bRet = TRUE;
   do{
        printf("您想进行何种操作？\n");
        printf("录入          press1\n修改          press2\n删除          press3\n返回主菜单    press4\n");
        fflush(stdin);  k=getchar(); getchar();
        if(k=='4')    { ClearScreen();  return TRUE;    }
    }while(k<'1'||k>'4');
    if(k=='1'){        bRet = AddGroup();  }
    else if(k=='2'){   bRet = AltGroup();  }
    else if(k=='3'){   bRet = DelGroup();  }
    return bRet;
}

/**
 * 函数名称: MaintainCourseInfo
 * 函数功能: 课程信息维护目录
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型, 总是为TRUE
 *
 * 调用说明:
 */
BOOL MaintainCourseInfo(void){
    BOOL bRet = TRUE;
   do{
        printf("您想进行何种操作？\n");
        printf("录入          press1\n修改          press2\n删除          press3\n返回主菜单    press4\n");
        fflush(stdin);  k=getchar(); getchar();
        if(k=='4')    { ClearScreen();  return TRUE;    }
    }while(k<'1'||k>'4');
    if(k=='1'){        bRet = AddCourse();  }
    else if(k=='2'){   bRet = AltCourse();  }
    else if(k=='3'){   bRet = DelCourse();  }
    return bRet;
}

/**
 * 函数名称: MaintainClassInfo
 * 函数功能: 课程信息维护目录
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型, 总是为TRUE
 *
 * 调用说明:
 */
BOOL MaintainClassInfo(void){
    BOOL bRet = TRUE;
   do{
        printf("您想进行何种操作？\n");
        printf("录入          press1\n修改          press2\n删除          press3\n返回主菜单    press4\n");
        fflush(stdin);  k=getchar(); getchar();
        if(k=='4')    { ClearScreen();  return TRUE;    }
    }while(k<'1'||k>'4');
    if(k=='1'){        bRet = AddClass();  }
    else if(k=='2'){   bRet = AltClass();  }
    else if(k=='3'){   bRet = DelClass();  }
    return bRet;
}

BOOL Add(void){
    fflush(stdin);
    for(i=0;i<telist->length;i++){
        printf("教师姓名:%s\n输入性别(男--1\t女--0):",telist->elem[i].tname);
        scanf("%hu",&telist->elem[i].gender);   getchar();
        printf("输入年龄:");
        scanf("%hu",&telist->elem[i].age);   getchar();
    }
    for(i=0;i<rolist->length;i++){
        printf("教室编号:%s\n输入容量:",rolist->elem[i].code);
        scanf("%hu",&rolist->elem[i].rsize);   getchar();
    }
    for(i=0;i<3;i++){
        for(psch=grade[i].schead;psch!=NULL;psch=psch->next){
            printf("班号:%s\n输入人数:",psch->classname);
            scanf("%hu",&psch->clsize);   getchar();
        }
    }
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

BOOL DelTimeNode(unsigned short wk_st[], unsigned short wk_ed[], unsigned short cou_st, unsigned short cou_ed, Ochead *ochead){
        count=0;
        while(wk_st[count] && count<5){   //按各段上课周数遍历
        for(p=wk_st[count]; p<=wk_ed[count]; p++){  //按上课周遍历
            for(n=cou_st; n<=cou_ed; n++){  //按上课节数遍历
                for(pocc=ochead->head;pocc!=NULL;pocc=pocc->next){
                    if(pocc->week==p && pocc->time==n){ //上课时间吻合
                        if(pocc==ochead->head){    //时间占位节点为头结点
                            ochead->head=pocc->next;
                            free(pocc);     pocc=ochead->head;
                            if(ochead->head==NULL){
                                ochead->len=0;     break;
                            }   }
                        else{   //时间占位结点非头结点
                            for(qocc=ochead->head;qocc->next!=pocc;qocc=qocc->next)   ;
                            qocc->next = pocc->next;    free(pocc);     pocc=qocc;
                        }
                        ochead->len--;     //时间占位结点数自减
                        break;
                    }   }   }   }
        count++;
    }
    return TRUE;
}

BOOL AddTimeNode(unsigned short wk_st[], unsigned short wk_ed[], unsigned short cou_st, unsigned short cou_ed, Ochead *ochead){
    unsigned short dwk, dtm;
    count=0;
    while(wk_st[count]&&count<5){
        for(p=wk_st[count]; p<=wk_ed[count]; p++)
            for(n=cou_st;n<=cou_ed; n++){
                //按先周数后节数的升序插入时间占位节点
                pocc=(Occupy *)malloc(sizeof(Occupy));
                pocc->week=p;       pocc->time=n;
                if(ochead->len==0){  //无结点时插在第一位
                    pocc->next=ochead->head;     ochead->head=pocc;
                }
                else{
                    dwk=pocc->week - ochead->head->week;
                    dtm=pocc->time - ochead->head->time;
                    if(dwk<0 || (dwk>=0 && dtm<0)){ //新结点值最小
                        pocc->next=ochead->head;
                        ochead->head=pocc;
                    }
                    else{   //在时间占位链表中部插入
                        for(qocc=ochead->head;qocc->next!=NULL;qocc=qocc->next){
                            dwk=pocc->week-qocc->next->week;
                            dtm=pocc->time-qocc->next->time;
                            if(dwk<0 || (dwk>=0 && dtm<0)){
                                pocc->next=qocc->next;  qocc->next=pocc;
                            }   }
                        if(qocc->next==NULL){   //在时间占位链表尾部插入
                            pocc->next=qocc->next;  qocc->next=pocc;
                        }   }   }
                ochead->len++;
            }
        count++;
    }
    return TRUE;
}

Schedule *ClassInput(char cbuf[]){
    fflush(stdin);
    Schedule *ps=NULL;
    scanf("%s", cbuf); getchar();
    //按班号确定查找头结点
    if(strstr(cbuf,"15"))         ps=grade[0].schead;
    else if(strstr(cbuf,"16"))    ps=grade[1].schead;
    else if(strstr(cbuf,"17"))    ps=grade[2].schead;
    else{
        printf("班号输入错误");
        getchar();   ClearScreen();    return NULL;
    }
    //检查班号的存在性
    for(;ps!=NULL;ps=ps->next)
        if(!strcmp(ps->classname, cbuf))  break;
    return ps;
}

BOOL AddNewRoom(char cbuf[]){
    if(rolist->length >= rolist->listsize){
        Room *ro_newbase = (Room *)realloc(rolist->elem, (rolist->listsize+LISTINCREMENT)* sizeof(Room));
        if(!ro_newbase){
            printf("内存分配失败,返回主菜单");
            getchar();   ClearScreen();    return TRUE;
        }
        rolist->elem = ro_newbase;
        rolist->listsize += LISTINCREMENT;
    }
    //添加新教室
    strcpy(rolist->elem[rolist->length].code, cbuf);
    //时间结构初始化
    for(i=0;i<6;i++){
        rolist->elem[rolist->length].ochead[i].head=NULL;
        rolist->elem[rolist->length].ochead[i].len=0;
    }
    printf("新教室已导入\n");
    return TRUE;
}

BOOL AddNewTeacher(char cbuf[]){
    if(telist->length >= telist->listsize){
        Teacher *te_newbase = (Teacher *)realloc(telist->elem, (telist->listsize+LISTINCREMENT)* sizeof(Teacher));
        if(!te_newbase){
            printf("内存分配失败,返回主菜单");
            getchar();   ClearScreen();    return TRUE;
        }
        telist->elem = te_newbase;
        telist->listsize += LISTINCREMENT;
    }
    //添加新教师
    strcpy(telist->elem[telist->length].tname, cbuf);
    //时间结构初始化
    for(i=0;i<6;i++){
        telist->elem[telist->length].ochead[i].head=NULL;
        telist->elem[telist->length].ochead[i].len=0;
    }
    printf("新教师已导入\n");
    return TRUE;
}

BOOL AddNewCourse(char cbuf[]){
    if(cougraph->length >= cougraph->listsize){     //查找表扩容
        CouHead *cou_newbase = (CouHead *)realloc(cougraph->coulist, (cougraph->listsize+LISTINCREMENT)* sizeof(CouHead));
        if(!cou_newbase){
            printf("内存分配失败,将返回主菜单");
            getchar();   ClearScreen();    return TRUE;
        }
        cougraph->coulist = cou_newbase;
        cougraph->listsize += LISTINCREMENT;
    }
    //节点初始化
    strcpy(cougraph->coulist[cougraph->length].name,cbuf);
    cougraph->coulist[cougraph->length].arcnum=0;
    cougraph->coulist[cougraph->length].archead=NULL;
    return TRUE;
}

BOOL SeekRemClass(Schedule *ps, char cbuf[]){
    Class *qc;
    for(p=0;p<ps->pr_num;p++)
        if(!strcmp(ps->progress[p],cbuf))  break;  //在班级在修课程中寻找被删课程
    flag=FALSE;
    if(p!=ps->pr_num){
        for(n=0;n<6;n++)    //查找此课程是否还有课堂
            for(qc=ps->week[n].classhead;qc!=NULL;qc=qc->next){
                if(flag==TRUE)  break;
                if(!strcmp(qc->coursename,ps->progress[p])){
                    flag=TRUE;  break;
                }
            }
        if(flag==FALSE){    //无课堂时删除该在修课程
            for(n=p;n<ps->pr_num-1;n++)   strcpy(ps->progress[n],ps->progress[n+1]);  //在班级节点中删除相应的在修课
            ps->pr_num--;
        }
    }
    return TRUE;
}

BOOL DelClassNode(Class *pc, Schedule *ps, unsigned short hu){\
    Class *qc;
    if(pc==ps->week[hu].classhead){         //课程链下头结点
        ps->week[hu].classhead=pc->next;
        free(pc);     pc=ps->week[j].classhead;
        if(ps->week[hu].classhead==NULL){
            ps->week[hu].cl_num=0;     return TRUE;
        }   }
    else{   //课程链下非头结点
        for(qc=ps->week[hu].classhead;qc->next!=pc;qc=qc->next) ;   //找到目标结点前驱
        qc->next=pc->next;      free(pc);     pc=qc;
    }
    psch->week[hu].cl_num--;
    return TRUE;
}

BOOL TimeInput(unsigned short *cou_st, unsigned short *cou_ed, unsigned short wk_st[], unsigned short wk_ed[]){
    fflush(stdin);
    printf("输入开始课时:");
    scanf("%hu", cou_st);      getchar();
    printf("输入结束课时:");
    scanf("%hu", cou_ed);      getchar();
    printf("分段输入上课周数:\n");
    count = 0;
    do{
        printf("起始周:");
        scanf("%hu", &wk_st[count]);  getchar();
        printf("结束周:");
        scanf("%hu", &wk_ed[count]);  getchar();
        printf("继续? 是--Y\t否--其他键\n");
        scanf("%c", &j);  getchar();    count++;
    }while(j=='Y' || j=='y');
    return TRUE;
}
