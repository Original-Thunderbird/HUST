#include "head.h"

int main() {
  COORD size = {
    SCR_COL,
    SCR_ROW
  }; /*window buffer size*/
  gh_std_out = GetStdHandle(STD_OUTPUT_HANDLE); /* Get standard output device handle*/
  gh_std_in = GetStdHandle(STD_INPUT_HANDLE); /* Get standard input device handle*/
  SetConsoleTitle(gp_sys_name); /*Set window title*/
  SetConsoleScreenBufferSize(gh_std_out, size); /*Set the window buffer size to 80*25*/
  //FileIn();
  Initial(); /*data loading*/
  InitInterface(); /*Interface initialization*/
  RunSys(); /*Selection and operation of system function modules*/
  CloseSys(); /*Exit the system*/
  return 0;
}

/**<The first part of text interface function*/

/**
 * Function name: InitInterface
 * Function function: Initialize the interface.
 * Input parameters: none
 * Output parameters: none
 * Return value: None
 *
 * Call description:
 */
void InitInterface() {
  /*
  WORD att = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
  | BACKGROUND_BLUE; //Yellow foreground and blue background
  SetConsoleTextAttribute(gh_std_out, att); //Set console screen buffer character attributes*/
  ClearScreen(); /* clear screen*/
  /*Create pop-up window information stack, treat the initialized screen window as the first layer of pop-up window*/
  gp_scr_att = (char * ) calloc(SCR_COL * SCR_ROW, sizeof(char)); /*screen character attribute*/
  gp_top_layer = (LAYER_NODE * ) malloc(sizeof(LAYER_NODE));
  gp_top_layer -> LayerNo = 0; /*The layer number of the pop-up window is 0*/
  gp_top_layer -> rcArea.Left = 0; /*The area of the pop-up window is the entire screen window*/
  gp_top_layer -> rcArea.Top = 0;
  gp_top_layer -> rcArea.Right = SCR_COL - 1;
  gp_top_layer -> rcArea.Bottom = SCR_ROW - 1;
  gp_top_layer -> pContent = NULL;
  gp_top_layer -> pScrAtt = gp_scr_att;
  gp_top_layer -> next = NULL;
  ShowMenu(); /*Show menu bar*/
  ShowState(); /*Display status bar*/
  return;
}

/**
 * Function name: ClearScreen
 * Function: Clear screen information.
 * Input parameters: None
 * Output parameters: none
 * Return value: None
 *
 * Call description:
 */
void ClearScreen(void) {
  CONSOLE_SCREEN_BUFFER_INFO bInfo;
  COORD home = {
    0,
    1
  }; /*Only clear the information starting from the first line, so that it can be called by each function*/
  unsigned long size;
  GetConsoleScreenBufferInfo(gh_std_out, & bInfo); /*Get screen buffer information*/
  size = bInfo.dwSize.X * (bInfo.dwSize.Y); /* Calculate the number of screen buffer character units*/
  /*Set the character attributes of all units in the screen buffer to the character attributes of the current screen buffer*/
  FillConsoleOutputAttribute(gh_std_out, bInfo.wAttributes, size, home, & ul);
  /* Fill all cells of the screen buffer with space characters*/
  FillConsoleOutputCharacter(gh_std_out, ' ', size, home, & ul);
  SetConsoleCursorPosition(gh_std_out, home); /*The cursor is placed in the upper left corner of the window*/
  return;
}

/**
 * Function name: ShowMenu
 * Function function: display the main menu on the screen, and set the hot zone, and put a check mark on the first item of the main menu.
 * Input parameters: none
 * Output parameters: none
 * Return value: None
 *
 * Call description:
 */
void ShowMenu() {
  CONSOLE_SCREEN_BUFFER_INFO bInfo;
  CONSOLE_CURSOR_INFO lpCur;
  COORD size, pos = {
    0,
    0
  };
  int PosA = 2, PosB, i, j;
  char ch;
  GetConsoleScreenBufferInfo(gh_std_out, & bInfo);
  size.X = bInfo.dwSize.X;
  size.Y = 1;
  SetConsoleCursorPosition(gh_std_out, pos);
  for (i = 0; i < 5; i++) /*Output the main menu item at the first row and first column of the window*/ {
    printf(" %s ", ga_main_menu[i]);
  }
  GetConsoleCursorInfo(gh_std_out, & lpCur);
  lpCur.bVisible = FALSE;
  SetConsoleCursorInfo(gh_std_out, & lpCur); /*hide cursor*/
  /* Apply for a dynamic storage area as a buffer for storing character information in the menu bar screen area */
  gp_buff_menubar_info = (CHAR_INFO * ) malloc(size.X * size.Y * sizeof(CHAR_INFO));
  SMALL_RECT rcMenu = {
    0,
    0,
    size.X - 1,
    0
  };
  /*Read the content of the first line of the window into the buffer that stores the character information of the menu bar screen area*/
  ReadConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, & rcMenu);
  /*Set the Chinese and English letters in this line to red, and set other character units to black on white background*/
  for (i = 0; i < size.X; i++) {
    (gp_buff_menubar_info + i) -> Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN |
      BACKGROUND_RED;
    ch = (char)((gp_buff_menubar_info + i) -> Char.AsciiChar);
    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
      (gp_buff_menubar_info + i) -> Attributes |= FOREGROUND_RED;
    }
  }
  /*The modified menu bar character information is written back to the first line of the window*/
  WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, & rcMenu);
  COORD endPos = {
    0,
    1
  };
  SetConsoleCursorPosition(gh_std_out, endPos); /*Set the cursor position to row 2, column 1*/
  /*Set the menu item as a hot zone, the hot zone number is the menu item number, and the hot zone type is 0 (button type)*/
  i = 0;
  do {
    PosB = PosA + strlen(ga_main_menu[i]); /*Locate the start and end positions of the i+1th menu item*/
    for (j = PosA; j < PosB; j++) {
      gp_scr_att[j] |= (i + 1) << 2; /*Set the attribute value of the character unit where the menu item is located*/
    }
    PosA = PosB + 4;
    i++;
  } while (i < 5);
  TagMainMenu(gi_sel_menu); /*mark the selected main menu item, the initial value of gi_sel_menu is 1*/
  return;
}

/**
 * Function name: ShowState
 * Function function: display the status bar.
 * Input parameters: None
 * Output parameters: None
 * Return value: None
 *
 * Call description: The character attribute of the status bar is black on a white background, and there is no status information in the initial state.
 */
void ShowState() {
  CONSOLE_SCREEN_BUFFER_INFO bInfo;
  COORD size, pos = {
    0,
    0
  };
  int i;
  GetConsoleScreenBufferInfo(gh_std_out, & bInfo);
  size.X = bInfo.dwSize.X;
  size.Y = 1;
  SMALL_RECT rcMenu = {
    0,
    bInfo.dwSize.Y - 1,
    size.X - 1,
    bInfo.dwSize.Y - 1
  };
  if (gp_buff_stateBar_info == NULL) {
    gp_buff_stateBar_info = (CHAR_INFO * ) malloc(size.X * size.Y * sizeof(CHAR_INFO));
    ReadConsoleOutput(gh_std_out, gp_buff_stateBar_info, size, pos, & rcMenu);
  }
  for (i = 0; i < size.X; i++) {
    (gp_buff_stateBar_info + i) -> Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN |
      BACKGROUND_RED;
    /*
    ch = (char)((gp_buff_stateBar_info+i)->Char.AsciiChar);
    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
    {
    (gp_buff_stateBar_info+i)->Attributes |= FOREGROUND_RED;
    }
    */
  }
  WriteConsoleOutput(gh_std_out, gp_buff_stateBar_info, size, pos, & rcMenu);
  return;
}

/**
 * Function name: TagMainMenu
 * Function: Set the check mark on the specified main menu item.
 * Input parameters: num selected main menu item number
 * Output parameters: none
 * Return value: None
 *
 * Call description:
 */
void TagMainMenu(int num) {
  CONSOLE_SCREEN_BUFFER_INFO bInfo;
  COORD size, pos = {
    0,
    0
  };
  int PosA = 2, PosB, i;
  char ch;
  if (num == 0) /*When num is 0, the check mark of the main menu item will be removed*/ {
    PosA = PosB = 0;
  } else /*Otherwise, locate the start and end positions of the selected main menu item: PosA is the start position, PosB is the end position*/ {
    for (i = 1; i < num; i++) {
      PosA += strlen(ga_main_menu[i - 1]) + 4;
    }
    PosB = PosA + strlen(ga_main_menu[num - 1]);
  }
  GetConsoleScreenBufferInfo(gh_std_out, & bInfo);
  size.X = bInfo.dwSize.X;
  size.Y = 1;
  /*Remove the menu item check mark in front of the selected menu item*/
  for (i = 0; i < PosA; i++) {
    (gp_buff_menubar_info + i) -> Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN |
      BACKGROUND_RED;
    ch = (gp_buff_menubar_info + i) -> Char.AsciiChar;
    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
      (gp_buff_menubar_info + i) -> Attributes |= FOREGROUND_RED;
    }
  }
  /*Mark on the selected menu item, black background and white text*/
  for (i = PosA; i < PosB; i++) {
    (gp_buff_menubar_info + i) -> Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN |
      FOREGROUND_RED;
  }
  /*Remove the menu item check mark behind the selected menu item*/
  for (i = PosB; i < bInfo.dwSize.X; i++) {
    (gp_buff_menubar_info + i) -> Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN |
      BACKGROUND_RED;
    ch = (char)((gp_buff_menubar_info + i) -> Char.AsciiChar);
    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
      (gp_buff_menubar_info + i) -> Attributes |= FOREGROUND_RED;
    }
  }
  /*Write the marked menu bar information to the first line of the window*/
  SMALL_RECT rcMenu = {
    0,
    0,
    size.X - 1,
    0
  };
  WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, & rcMenu);
  return;
}

/**
 * Function name: PopMenu
 * Function function: In the submenu and set the hot zone, set the check mark with the cursor movement.
 * Input parameters: num submenu number of items from top to bottom
 * Output parameters: None
 * Return value: None
 *
 * Call description:
 */
void PopMenu(int num) {
  LABEL_BUNDLE labels;
  HOT_AREA areas;
  SMALL_RECT rcPop;
  COORD pos;
  WORD att;
  char * pCh;
  int i, j, loc = 0;
  if (num != gi_sel_menu) /*If the specified main menu is not the selected menu*/ {
    if (gp_top_layer -> LayerNo != 0) /*If a submenu has popped up before*/ {
      PopOff();
      gi_sel_sub_menu = 0;
    }
  } else if (gp_top_layer -> LayerNo != 0) /*If the submenu has popped up, return*/ {
    return;
  }
  gi_sel_menu = num; /*Set the selected main menu item to the specified main menu item*/
  TagMainMenu(gi_sel_menu); /*mark the selected main menu item*/
  LocSubMenu(gi_sel_menu, & rcPop); /*Calculate the area position of the pop-up submenu, store it in rcPop*/
  /* Calculate the position (subscript) of the first item in the submenu in the submenu string array*/
  for (i = 1; i < gi_sel_menu; i++) {
    loc += ga_sub_menu_count[i - 1];
  }
  /* Store the item name of this group of submenu items in the tag bundle structure variable*/
  labels.ppLabel = ga_sub_menu + loc; /*Address of the first label string in the label bundle*/
  labels.num = ga_sub_menu_count[gi_sel_menu - 1]; /*The number of label strings in the label bundle*/
  COORD aLoc[labels.num]; /*Define a coordinate array to store the coordinates of the output position of each label string*/
  for (i = 0; i < labels.num; i++) /*Determine the output position of the label string and store it in the coordinate array*/ {
    aLoc[i].X = rcPop.Left + 2;
    aLoc[i].Y = rcPop.Top + i + 1;
  }
  labels.pLoc = aLoc; /*Make the member pLoc of the label bundle structure variable labels point to the first element of the coordinate array*/
  /*Set hot zone information*/
  areas.num = labels.num; /*The number of hot areas is equal to the number of labels, that is, the number of items in the submenu*/
  SMALL_RECT aArea[areas.num]; /*Define an array to store all hot area locations*/
  char aSort[areas.num]; /*Define an array to store the corresponding categories of all hot areas*/
  char aTag[areas.num]; /*Define an array to store the number of each hot area*/
  for (i = 0; i < areas.num; i++) {
    aArea[i].Left = rcPop.Left + 2; /*Hot area positioning*/
    aArea[i].Top = rcPop.Top + i + 1;
    aArea[i].Right = rcPop.Right - 2;
    aArea[i].Bottom = aArea[i].Top;
    aSort[i] = 0; /* hot zone category is 0 (button type)*/
    aTag[i] = i + 1; /*Hot zones are numbered sequentially*/
  }
  areas.pArea = aArea; /*Make the member pArea of the hot area structure variable areas point to the first element of the hot area position array*/
  areas.pSort = aSort; /*Make the member pSort of the hot area structure variable areas point to the first element of the hot area category array*/
  areas.pTag = aTag; /*Make the member pTag of the hot area structure variable areas point to the first element of the hot area number array*/
  att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED; /*black text on white background*/
  PopUp( & rcPop, att, & labels, & areas);
  DrawBox( & rcPop); /* Draw a border for the pop-up window*/
  pos.X = rcPop.Left + 2;
  for (pos.Y = rcPop.Top + 1; pos.Y < rcPop.Bottom; pos.Y++) {
    /*This cycle is used to draw a line at the position of the empty string sub-menu item to form a separation, and cancel the hot zone attribute of this menu item*/
    pCh = ga_sub_menu[loc + pos.Y - rcPop.Top - 1];
    if (strlen(pCh) == 0) /*The string length is 0, indicating that it is an empty string*/ {
      /*First draw a horizontal line*/
      FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right - rcPop.Left - 3, pos, & ul);
      for (j = rcPop.Left + 2; j < rcPop.Right - 1; j++) {
        /* cancel the hot area attribute of the character unit in this area */
        gp_scr_att[pos.Y * SCR_COL + j] &= 3; /*The result of bitwise AND retains the lower two bits*/
      }
    }
  }
  /*Set the function key of the submenu item as red letter on white background*/
  pos.X = rcPop.Left + 3;
  att = FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
  for (pos.Y = rcPop.Top + 1; pos.Y < rcPop.Bottom; pos.Y++) {
    if (strlen(ga_sub_menu[loc + pos.Y - rcPop.Top - 1]) == 0) {
      continue; /*skip empty string*/
    }
    FillConsoleOutputAttribute(gh_std_out, att, 1, pos, & ul);
  }
  return;
}

/**
* Function name: PopUp
* Function: Output the pop-up window information in the specified area, set the hot zone at the same time, and push the pop-up window position information into the stack.
* Input parameters: pRc The address where the location data of the pop-up window is stored
* att popup window area character attribute
* pLabel The address where the label bundle information is stored in the popup window
pHotArea The address where the hot area information is stored in the pop-up window
* Output parameters: None
* Return value: None
*
* Call description:
*/
void PopUp(SMALL_RECT * pRc, WORD att, LABEL_BUNDLE * pLabel, HOT_AREA * pHotArea) {
  LAYER_NODE * nextLayer;
  COORD size, pos = {
    0,
    0
  };
  char * pCh;
  int i, j, row;
  /* The character unit information of the location of the pop-up window is pushed into the stack*/
  size.X = pRc -> Right - pRc -> Left + 1; /*The width of the pop-up window*/
  size.Y = pRc -> Bottom - pRc -> Top + 1; /*The height of the pop-up window*/
  /*Apply for a dynamic storage area for storing pop-up window related information*/
  nextLayer = (LAYER_NODE * ) malloc(sizeof(LAYER_NODE));
  nextLayer -> next = gp_top_layer;
  nextLayer -> LayerNo = gp_top_layer -> LayerNo + 1;
  nextLayer -> rcArea = * pRc;
  nextLayer -> pContent = (CHAR_INFO * ) malloc(size.X * size.Y * sizeof(CHAR_INFO));
  nextLayer -> pScrAtt = (char * ) malloc(size.X * size.Y * sizeof(char));
  pCh = nextLayer -> pScrAtt;
  /*Save the character information of the area covered by the pop-up window to restore it when the pop-up window is closed*/
  ReadConsoleOutput(gh_std_out, nextLayer -> pContent, size, pos, pRc);
  for (i = pRc -> Top; i <= pRc -> Bottom; i++) {
    /*This double cycle stores the original attribute value of the covered character unit into the dynamic storage area, which is convenient for later recovery*/
    for (j = pRc -> Left; j <= pRc -> Right; j++) {
      * pCh = gp_scr_att[i * SCR_COL + j];
      pCh++;
    }
  }
  gp_top_layer = nextLayer; /*Complete pop-up window related information into the stack*/
  /* Set the new attribute of the character of the popup window area */
  pos.X = pRc -> Left;
  pos.Y = pRc -> Top;
  for (i = pRc -> Top; i <= pRc -> Bottom; i++) {
    FillConsoleOutputAttribute(gh_std_out, att, size.X, pos, & ul);
    pos.Y++;
  }
  /* Output the label string in the label bundle at the set position */
  for (i = 0; i < pLabel -> num; i++) {
    pCh = pLabel -> ppLabel[i];
    if (strlen(pCh) != 0) {
      WriteConsoleOutputCharacter(gh_std_out, pCh, strlen(pCh),
        pLabel -> pLoc[i], & ul);
    }
  }
  /* Set the new attribute of the character cell of the popup window area */
  for (i = pRc -> Top; i <= pRc -> Bottom; i++) {
    /*This double loop sets the layer number of the character unit*/
    for (j = pRc -> Left; j <= pRc -> Right; j++) {
      gp_scr_att[i * SCR_COL + j] = gp_top_layer -> LayerNo;
    }
  }

  for (i = 0; i < pHotArea -> num; i++) {
    /*This double loop sets the thermal zone type and thermal zone number of character cells in all thermal zones*/
    row = pHotArea -> pArea[i].Top;
    for (j = pHotArea -> pArea[i].Left; j <= pHotArea -> pArea[i].Right; j++) {
      gp_scr_att[row * SCR_COL + j] |= (pHotArea -> pSort[i] << 6) |
        (pHotArea -> pTag[i] << 2);
    }
  }
  return;
}

/**
 * Function name: PopOff
 * Function function: close the top-level pop-up window, restore the original appearance of the covered area and the original properties of the character unit.
 * Input parameters: None
 * Output parameters: None
 * Return value: None
 *
 * Call description:
 */
void PopOff(void) {
  LAYER_NODE * nextLayer;
  COORD size, pos = {
    0,
    0
  };
  char * pCh;
  int i, j;
  if ((gp_top_layer -> next == NULL) || (gp_top_layer -> pContent == NULL)) {
    /*The main interface screen information stored at the bottom of the stack, do not need to close*/
    return;
  }
  nextLayer = gp_top_layer -> next;
  /*Restore the original appearance of the popup window area*/
  size.X = gp_top_layer -> rcArea.Right - gp_top_layer -> rcArea.Left + 1;
  size.Y = gp_top_layer -> rcArea.Bottom - gp_top_layer -> rcArea.Top + 1;
  WriteConsoleOutput(gh_std_out, gp_top_layer -> pContent, size, pos, & (gp_top_layer -> rcArea));
  /*Restore the original attribute of the character unit*/
  pCh = gp_top_layer -> pScrAtt;
  for (i = gp_top_layer -> rcArea.Top; i <= gp_top_layer -> rcArea.Bottom; i++) {
    for (j = gp_top_layer -> rcArea.Left; j <= gp_top_layer -> rcArea.Right; j++) {
      gp_scr_att[i * SCR_COL + j] = * pCh;
      pCh++;
    }
  }
  free(gp_top_layer -> pContent); /*Release dynamic storage area*/
  free(gp_top_layer -> pScrAtt);
  free(gp_top_layer);
  gp_top_layer = nextLayer;
  gi_sel_sub_menu = 0;
  return;
}

/**
 * Function name: DrawBox
 * Function function: Draw a border in the specified area.
 * Input parameters: pRc stores the address of the location information of the region
 * Output parameters: None
 * Return value: None
 *
 * Call description:
 */
void DrawBox(SMALL_RECT * pRc) {
  char chBox[] = {
    '+',
    '-',
    '|'
  }; /*Character used for picture frame*/
  COORD pos = {
    pRc -> Left,
    pRc -> Top
  }; /* positioned at the upper left corner of the area*/

  WriteConsoleOutputCharacter(gh_std_out, & chBox[0], 1, pos, & ul); /*draw the upper left corner of the frame*/
  for (pos.X = pRc -> Left + 1; pos.X < pRc -> Right; pos.X++) {
    /* This loop draws a horizontal line on the border */
    WriteConsoleOutputCharacter(gh_std_out, & chBox[1], 1, pos, & ul);
  }
  pos.X = pRc -> Right;
  WriteConsoleOutputCharacter(gh_std_out, & chBox[0], 1, pos, & ul); /*draw the upper right corner of the frame*/
  for (pos.Y = pRc -> Top + 1; pos.Y < pRc -> Bottom; pos.Y++) {
    /*This cycle draws the left and right lines of the border*/
    pos.X = pRc -> Left;
    WriteConsoleOutputCharacter(gh_std_out, & chBox[2], 1, pos, & ul);
    pos.X = pRc -> Right;
    WriteConsoleOutputCharacter(gh_std_out, & chBox[2], 1, pos, & ul);
  }
  pos.X = pRc -> Left;
  pos.Y = pRc -> Bottom;
  WriteConsoleOutputCharacter(gh_std_out, & chBox[0], 1, pos, & ul); /*Draw the lower left corner of the frame*/
  for (pos.X = pRc -> Left + 1; pos.X < pRc -> Right; pos.X++) {
    /*Draw the border horizontal line*/
    WriteConsoleOutputCharacter(gh_std_out, & chBox[1], 1, pos, & ul);
  }
  pos.X = pRc -> Right;
  WriteConsoleOutputCharacter(gh_std_out, & chBox[0], 1, pos, & ul); /*Draw the lower right corner of the frame*/
  return;
}

/**
 * Function name: TagSubMenu
 * Function function: Make a checkmark on the specified submenu item.
 * Input parameters: num selected submenu item number
 * Output parameters: none
 * Return value: None
 *
 * Call description:
 */
void TagSubMenu(int num) {
  SMALL_RECT rcPop;
  COORD pos;
  WORD att;
  int width;
  LocSubMenu(gi_sel_menu, & rcPop); /*Calculate the area position of the pop-up submenu, store it in rcPop*/
  if ((num < 1) || (num == gi_sel_sub_menu) || (num > rcPop.Bottom - rcPop.Top - 1)) {
    /* If the submenu item number is out of bounds, or the item submenu has been selected, return */
    return;
  }
  pos.X = rcPop.Left + 2;
  width = rcPop.Right - rcPop.Left - 3;
  if (gi_sel_sub_menu != 0) /*First cancel the mark on the original selected submenu item*/ {
    pos.Y = rcPop.Top + gi_sel_sub_menu;
    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED; /*black text on white background*/
    FillConsoleOutputAttribute(gh_std_out, att, width, pos, & ul);
    pos.X += 1;
    att |= FOREGROUND_RED; /*red text on white background*/
    FillConsoleOutputAttribute(gh_std_out, att, 1, pos, & ul);
  }
  /* Make a check mark on the specified submenu item */
  pos.X = rcPop.Left + 2;
  pos.Y = rcPop.Top + num;
  att = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED; /*white text on black background*/
  FillConsoleOutputAttribute(gh_std_out, att, width, pos, & ul);
  gi_sel_sub_menu = num; /*Modify the selected submenu item number*/
  return;
}

/**
 * Function name: LocSubMenu
 * Function: Calculate the position of the upper left corner and the lower right corner of the pop-up submenu area.
 * Input parameters: num selected main menu item number
 * Output parameter: rc is the address to store the location information of the region
 * Return value: None
 *
 * Call description:
 */
void LocSubMenu(int num, SMALL_RECT * rc) {
  int i, len, loc = 0;
  rc -> Top = 1; /*The top of the area is set on the second line, and the line number is 1*/
  rc -> Left = 1;
  for (i = 1; i < num; i++) {
    /* Calculate the position of the left border of the area, and calculate the position of the first submenu item in the submenu string array */
    rc -> Left += strlen(ga_main_menu[i - 1]) + 4;
    loc += ga_sub_menu_count[i - 1];
  }
  rc -> Right = strlen(ga_sub_menu[loc]); /*Temporarily store the string length of the first submenu item*/
  for (i = 1; i < ga_sub_menu_count[num - 1]; i++) {
    /*Find the longest submenu string and store its length in rc->Right*/
    len = strlen(ga_sub_menu[loc + i]);
    if (rc -> Right < len) {
      rc -> Right = len;
    }
  }
  rc -> Right += rc -> Left + 3; /*calculate the right boundary of the region*/
  rc -> Bottom = rc -> Top + ga_sub_menu_count[num - 1] + 1; /* Calculate the line number below the area*/
  if (rc -> Right >= SCR_COL) /*The processing of the right boundary crossing */ {
    len = rc -> Right - SCR_COL + 1;
    rc -> Left -= len;
    rc -> Right = SCR_COL - 1;
  }
  return;
}

/**
 * Function name: DealInput
 * Function function: set the hot zone in the pop-up window area, wait for and respond to user input.
 * Input parameters: pHotArea hot area parameter pointer
 * piHot The storage address of the focus hot zone number, that is, the pointer to the focus hot zone number
 * Output parameter: pHotArea hot area parameter
 * piHot returns the current hot spot number when clicking with the mouse, pressing enter or space
 * Return value: iRet carriage return virtual key code
 *
 * Call description:
 */
int DealInput(HOT_AREA * pHotArea, int * piHot) {
  INPUT_RECORD inRec;
  DWORD res;
  COORD pos = {
    0,
    0
  };
  int num, arrow, iRet = 0;
  int cNo, cTag, cSort; /*cNo: layer number, cTag: hot zone number, cSort: hot zone type*/
  char vkc, asc; /*vkc: virtual key code, asc: character ASCII code value*/
  SetHotPoint(pHotArea, * piHot);
  while (TRUE) {
    /*loop*/
    ReadConsoleInput(gh_std_in, & inRec, 1, & res);
    if ((inRec.EventType == MOUSE_EVENT) &&
      (inRec.Event.MouseEvent.dwButtonState ==
        FROM_LEFT_1ST_BUTTON_PRESSED)) {
      pos = inRec.Event.MouseEvent.dwMousePosition;
      cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3;
      cTag = (gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2) & 15;
      cSort = (gp_scr_att[pos.Y * SCR_COL + pos.X] >> 6) & 3;
      if ((cNo == gp_top_layer -> LayerNo) && cTag > 0) {
        * piHot = cTag;
        SetHotPoint(pHotArea, * piHot);
        if (cSort == 0) {
          iRet = 13;
          break;
        }
      }
    } else if (inRec.EventType == KEY_EVENT && inRec.Event.KeyEvent.bKeyDown) {
      vkc = inRec.Event.KeyEvent.wVirtualKeyCode;
      asc = inRec.Event.KeyEvent.uChar.AsciiChar;;
      if (asc == 0) {
        arrow = 0;
        switch (vkc) {
          /*Arrow keys (left, up, right, down) processing*/
        case 37:
          arrow = 1;
          break;
        case 38:
          arrow = 2;
          break;
        case 39:
          arrow = 3;
          break;
        case 40:
          arrow = 4;
          break;
        }
        if (arrow > 0) {
          num = * piHot;
          while (TRUE) {
            if (arrow < 3) {
              num--;
            } else {
              num++;
            }
            if ((num < 1) || (num > pHotArea -> num) ||
              ((arrow % 2) && (pHotArea -> pArea[num - 1].Top ==
                pHotArea -> pArea[ * piHot - 1].Top)) || ((!(arrow % 2)) &&
                (pHotArea -> pArea[num - 1].Top !=
                  pHotArea -> pArea[ * piHot - 1].Top))) {
              break;
            }
          }
          if (num > 0 && num <= pHotArea -> num) {
            * piHot = num;
            SetHotPoint(pHotArea, * piHot);
          }
        }
      } else if (vkc == 27) {
        /*ESC key*/
        iRet = 27;
        break;
      } else if (vkc == 13 || vkc == 32) {
        /*Enter key or space means pressing the current button*/
        iRet = 13;
        break;
      }
    }
  }
  return iRet;
}

/**
 * Function name: SetHotPoint
 * Function function: set the hot zone in the pop-up window area, wait for and respond to user input.
 * Input parameters: pHotArea hot area parameter pointer
 * iHot focus hot zone number
 * Output parameter: pHotArea hot area parameter
 * Return value: None
 *
 * Call description:
 */
void SetHotPoint(HOT_AREA * pHotArea, int iHot) {
  CONSOLE_CURSOR_INFO lpCur;
  COORD pos = {
    0,
    0
  };
  WORD att1, att2;
  int i, width;
  att1 = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED; /*white text on black background*/
  att2 = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED; /*black text on white background*/
  for (i = 0; i < pHotArea -> num; i++) {
    /*Set button hotspots to black text on a white background*/
    pos.X = pHotArea -> pArea[i].Left;
    pos.Y = pHotArea -> pArea[i].Top;
    width = pHotArea -> pArea[i].Right - pHotArea -> pArea[i].Left + 1;
    if (pHotArea -> pSort[i] == 0) {
      /* hot zone is a button class */
      FillConsoleOutputAttribute(gh_std_out, att2, width, pos, & ul);
    }
  }
  pos.X = pHotArea -> pArea[iHot - 1].Left;
  pos.Y = pHotArea -> pArea[iHot - 1].Top;
  width = pHotArea -> pArea[iHot - 1].Right - pHotArea -> pArea[iHot - 1].Left + 1;
  if (pHotArea -> pSort[iHot - 1] == 0) {
    /*The activated hotspot is the button class*/
    FillConsoleOutputAttribute(gh_std_out, att1, width, pos, & ul);
  } else if (pHotArea -> pSort[iHot - 1] == 1) {
    /*The activated hotspot is the text box class*/
    SetConsoleCursorPosition(gh_std_out, pos);
    GetConsoleCursorInfo(gh_std_out, & lpCur);
    lpCur.bVisible = TRUE;
    SetConsoleCursorInfo(gh_std_out, & lpCur);
  }
}

/**
 * Function name: ShowModule
 * Function function: restore the system code table and three types of basic data from the specified data file.
 * Input parameters: pString The first address of the two-dimensional array of text information
 * n number of strings
 * Output parameters:
 * Return value: BOOL type, always TRUE
 *
 * Call description:
 */
BOOL ShowModule(char ** pString, int n) {
  LABEL_BUNDLE labels;
  HOT_AREA areas;
  BOOL bRet = TRUE;
  SMALL_RECT rcPop;
  COORD pos;
  WORD att;
  int i, maxlen, str_len, iHot = 1;
  for (i = 0, maxlen = 0; i < n; i++) {
    str_len = strlen(pString[i]);
    if (maxlen < str_len) {
      maxlen = str_len;
    }
  }
  pos.X = maxlen + 16;
  pos.Y = n + 15;
  rcPop.Left = (SCR_COL - pos.X) / 2 - 10;
  rcPop.Right = rcPop.Left + pos.X - 1;
  rcPop.Top = (SCR_ROW - pos.Y) / 2 - 12;
  rcPop.Bottom = rcPop.Top + pos.Y - 1;
  att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED; /*black text on white background*/
  labels.num = n;
  labels.ppLabel = pString;
  COORD aLoc[n];
  for (i = 0; i < n; i++) {
    aLoc[i].X = rcPop.Left + 3;
    aLoc[i].Y = rcPop.Top + (pos.Yn) / 2 + i;
  }
  str_len = strlen(pString[n - 1]);
  aLoc[n - 1].X = rcPop.Left + 3 + (maxlen - str_len) / 2;
  aLoc[n - 1].Y = aLoc[n - 1].Y + 2;
  labels.pLoc = aLoc;
  areas.num = 1;
  SMALL_RECT aArea[] = {
    {
      aLoc[n - 1].X, aLoc[n - 1].Y,
        aLoc[n - 1].X + 3, aLoc[n - 1].Y
    }
  };
  char aSort[] = {
    0
  }, aTag[] = {
    1
  };
  areas.pArea = aArea;
  areas.pSort = aSort;
  areas.pTag = aTag;
  PopUp( & rcPop, att, & labels, & areas);
  pos.X = rcPop.Left + 1;
  pos.Y = rcPop.Top + (pos.Yn) / 2 + n;
  FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right - rcPop.Left - 1, pos, & ul);
  DealInput( & areas, & iHot);
  PopOff();
  return bRet;
}

/**
 * Function name: Message
 * Function function: pop-up dialog box
 * Input parameters: **pCh The first address of the dialog box information array
 * Output parameters: none
 * Return value: BOOL type, exit is TRUE, cancel is FALSE
 *
 * Call description:
 */
BOOL Message(char ** pCh) {
  LABEL_BUNDLE labels;
  HOT_AREA areas;
  SMALL_RECT rcPop;
  COORD pos;
  WORD att;
  int iHot = 1;
  char len[3] = {
    (char) strlen(pCh[0]),
    (char) strlen(pCh[1]),
    (char) strlen(pCh[2])
  };
  char lmax = (len[0] > len[1] + len[2] + 4) ? len[0] : len[1] + len[2] + 4;
  pos.X = lmax + 6;
  pos.Y = 7;
  char blanc = (pos.X - (len[1] + len[2] + 4)) / 2;
  rcPop.Left = (SCR_COL - pos.X) / 2 - 10;
  rcPop.Right = rcPop.Left + pos.X - 1;
  rcPop.Top = (SCR_ROW - pos.Y) / 2 - 12;
  rcPop.Bottom = rcPop.Top + pos.Y - 1;
  att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED; /*black text on white background*/
  labels.num = 3;
  labels.ppLabel = pCh;
  COORD aLoc[] = {
    {
      rcPop.Left + 3, rcPop.Top + 2
    },
    {
      rcPop.Left + blanc,
      rcPop.Top + 5
    },
    {
      rcPop.Right - blanc - (len[2] - 1),
      rcPop.Top + 5
    }
  };
  labels.pLoc = aLoc;
  areas.num = 2;
  SMALL_RECT aArea[] = {
    {
      rcPop.Left + blanc, rcPop.Top + 5,
        rcPop.Left + blanc + (len[1] - 1), rcPop.Top + 5
    },
    {
      rcPop.Right - blanc - (len[2] - 1),
      rcPop.Top + 5,
      rcPop.Right - blanc,
      rcPop.Top + 5
    }
  };
  char aSort[] = {
    0,
    0
  }, aTag[] = {
    1,
    2
  };
  areas.pArea = aArea;
  areas.pSort = aSort;
  areas.pTag = aTag;
  PopUp( & rcPop, att, & labels, & areas);
  pos.X = rcPop.Left + 1;
  pos.Y = rcPop.Top + 4;
  FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right - rcPop.Left - 1, pos, & ul);
  if (DealInput( & areas, & iHot) == 13 && iHot == 1) {
    return FALSE;
  } else {
    return TRUE;
  }
}

/**<The second part of the system skeleton function*/

/**
 * Function name: LoadData
 * Function function: Load the code table and three types of basic data from the data file into the memory buffer and cross-linked list.
 * Input parameters: None
 * Output parameters: none
 * Return value: BOOL type, in the function function except the return value of the function ExitSys can be FALSE,
 * The return value of other functions must be TRUE.
 *
 * Call description: In order to call each function function in a unified way, the prototype of these function functions is set to
 * Consistent, that is, there are no parameters and the return value is BOOL. When the return value is FALSE, the program ends.
 */
/*
BOOL LoadData()
{
int Re = 0;
Re = CreateList();
gc_sys_state |= Re;
gc_sys_state &= ~(1 + 2 + 4 - Re);
if (gc_sys_state < (1 | 2 | 4 )){
printf("\nSystem basic data is incomplete!\n");
system("pause");
}
return TRUE;
}
*/
/**
 * Function name: CreateList
 * Function function: read the basic data from the data file, and store it in the created cross-linked list.
 * Input parameters: None
 * Output parameter: head The address of the main chain head pointer, used to return the created cross chain.
 * Return value: int type value, indicating the creation of the linked list.
 * 1 The departmental information data has been loaded, but there is no basic information of the scientific research team and scientific research project data
 * 3 The departmental information and basic information data of the scientific research team have been loaded, but there is no scientific research project data
 * 7 Three types of basic data have been loaded
 *
 * Call description:
 */
/*
int CreateList(void)
{
SCHOOL_NODE *head=NULL, tmp1;
TEAM_NODE tmp2;
PROJECT_NODE tmp3;
FILE *pFile;
int find;
int re = 0;
if ((pFile = fopen("school.dat", "rb+")) == NULL)
{
printf("Failed to open the department information data file!\n");
return re;
}
printf("Department information data file opened successfully!\n");
//Read the dormitory building information data from the data file, and store it in the main chain established by LIFO
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
printf("Department information data file failed to load!\n");
return re;
}
printf("Department information data file loaded successfully!\n");
sch_head = head;
re += 1;
if ((pFile = fopen("team.dat", "rb+")) == NULL)
{
printf("Failed to open the basic information data file of the scientific research team!\n");
return re;
}
printf("Scientific research team basic information data file opened successfully!\n");
re += 2;
//Read the basic information data of the students from the data file, and store it in the branch chain of the basic information of the students corresponding to the node of the main chain
while (fread(&tmp2, sizeof(TEAM_NODE), 1, pFile) == 1)
{
//Create a node to store the basic information of the students read from the data file
q = (TEAM_NODE *)malloc(sizeof(TEAM_NODE));
*q = tmp2;
q->proj_head = NULL;
//Find the main chain node corresponding to the student's dormitory building on the main chain
p = head;
while (p != NULL&& strcmp(p->school_name, q->school_name))
{
p = p->next;
}
if (p != NULL) //If found, insert the node into the student information branch chain in the last-in-first-out manner
{
q->next = p->team_head;
p->team_head = q;
}
else //If not found, release the memory space of the created node
{
free(q);
}
}
fclose(pFile);
if ((pFile = fopen("project.dat", "rb")) == NULL)
{
printf("Failed to open scientific research team information data file!\n");
return re;
}
printf("Scientific research team information data file opened successfully!\n");
re += 4;
//Read the student payment information data from the data file, and store it in the payment branch chain of the corresponding node of the student basic information branch chain
while (fread(&tmp3, sizeof(PROJECT_NODE), 1, pFile) == 1)
{
//Create a node to store the student payment information read from the data file
r = (PROJECT_NODE *)malloc(sizeof(PROJECT_NODE));
*r = tmp3;
//Find the corresponding student information node on the student information branch chain
p = head;
find = 0;
while (p != NULL && find == 0)
{
q = p->team_head;
while (q != NULL && find == 0)
{
if (strcmp(q->team_name, r->team_name) == 0)
{ find = 1; break; }
q = q->next;
}
p = p->next;
}
if (find) //if found, insert the node into the branch chain of student payment information in the way of last in first out
{
r->next = q->proj_head;
q->proj_head = r;
}
else //If not found, release the memory space of the created node
{ free(r); }
}
fclose(pFile);
printf("Press any key to continue...\n"); getch();
return re;
}
*/
/**
 * Function name: RunSys
 * Function function: Run the system, run the function module selected by the user under the main interface of the system.
 * Input parameters: None
 * Output parameters: none
 * Return value: None
 *
 * Call description:
 */
void RunSys(void) {
  INPUT_RECORD inRec;
  DWORD res;
  COORD pos = {
    0,
    0
  };
  BOOL bRet = TRUE;
  int i, loc, num;
  int cNo, cAtt; /*cNo: character unit layer number, cAtt: character unit attribute*/
  char vkc, asc; /*vkc: virtual key code, asc: character ASCII code value*/
  while (bRet) {
    /* read a record from the console input buffer */
    ReadConsoleInput(gh_std_in, & inRec, 1, & res);
    if (inRec.EventType == MOUSE_EVENT) /*If the record is generated by mouse event*/ {
      pos = inRec.Event.MouseEvent.dwMousePosition; /*Get mouse coordinate position*/
      cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3; /*Get the layer number of this position*/
      cAtt = gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2; /*take the character unit attribute*/
      if (cNo == 0) /*The layer number is 0, indicating that the position is not covered by the pop-up submenu*/ {
        /* cAtt > 0 indicates that the position is in the hot zone (main menu item character unit)
         * cAtt != gi_sel_menu indicates that the main menu item at this position is not selected
         * gp_top_layer->LayerNo > 0 indicates that there is currently a submenu pop-up
         */
        if (cAtt > 0 && cAtt != gi_sel_menu && gp_top_layer -> LayerNo > 0) {
          PopOff(); /*Close the pop-up submenu*/
          gi_sel_sub_menu = 0; /*Set the item number of the selected submenu item to 0*/
          PopMenu(cAtt); /*Pop up the submenu corresponding to the main menu item where the mouse is located*/
        }
      } else if (cAtt > 0) /*The position of the mouse is the menu item character unit of the pop-up submenu*/ {
        TagSubMenu(cAtt); /*Make a checkmark on the submenu item*/
      }

      if (inRec.Event.MouseEvent.dwButtonState ==
        FROM_LEFT_1ST_BUTTON_PRESSED) /*If the first button on the left side of the mouse is pressed*/ {
        if (cNo == 0) /*The layer number is 0, indicating that the position is not covered by the pop-up submenu*/ {
          if (cAtt > 0) /*If the position is in the hot zone (main menu item character unit)*/ {
            PopMenu(cAtt); /*Pop up the submenu corresponding to the main menu item where the mouse is located*/
          }
          /*If the position does not belong to the character unit of the main menu item, and there is a submenu to pop up*/
          else if (gp_top_layer -> LayerNo > 0) {
            PopOff(); /*Close the pop-up submenu*/
            gi_sel_sub_menu = 0; /*Set the item number of the selected submenu item to 0*/
          }
        } else /*The layer number is not 0, indicating that the position is covered by the pop-up submenu*/ {
          if (cAtt > 0) /*if the position is in the hot zone (submenu item character unit)*/ {
            PopOff(); /*Close the pop-up submenu*/
            gi_sel_sub_menu = 0; /*Set the item number of the selected submenu item to 0*/
            /* Execute the corresponding function: gi_sel_menu main menu item number, cAtt submenu item number*/
            bRet = ExeFunction(gi_sel_menu, cAtt);
          }
        }
      } else if (inRec.Event.MouseEvent.dwButtonState ==
        RIGHTMOST_BUTTON_PRESSED) /*If the right mouse button is pressed*/ {
        if (cNo == 0) /*The layer number is 0, indicating that the position is not covered by the pop-up submenu*/ {
          PopOff(); /*Close the pop-up submenu*/
          gi_sel_sub_menu = 0; /*Set the item number of the selected submenu item to 0*/
        }
      }
    } else if (inRec.EventType == KEY_EVENT /*if the record is generated by a key*/ &&
      inRec.Event.KeyEvent.bKeyDown) /*and the key is pressed*/ {
      vkc = inRec.Event.KeyEvent.wVirtualKeyCode; /*Get the virtual key code of the key*/
      asc = inRec.Event.KeyEvent.uChar.AsciiChar; /*Get the ASC code of the key*/
      /* Handling of system shortcut keys */
      if (vkc == 112) /*if the F1 key is pressed*/ {
        if (gp_top_layer -> LayerNo != 0) /*If a submenu pops up*/ {
          PopOff(); /*Close the pop-up submenu*/
          gi_sel_sub_menu = 0; /*Set the item number of the selected submenu item to 0*/
        }
        bRet = ExeFunction(5, 1); /*run help topic function*/
      } else if (inRec.Event.KeyEvent.dwControlKeyState &
        (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED)) {
        /* If the left or right Alt key is pressed */
        switch (vkc) /*judgment key combination Alt+letter*/ {
        case 88:
          /*Alt+X exit*/
          if (gp_top_layer -> LayerNo != 0) {
            PopOff();
            gi_sel_sub_menu = 0;
          }
          bRet = ExeFunction(1, 4);
          break;
        case 70:
          /*Alt+F*/ PopMenu(1);
          break;
        case 77:
          /*Alt+M*/ PopMenu(2);
          break;
        case 81:
          /*Alt+Q*/ PopMenu(3);
          break;
        case 83:
          /*Alt+S*/ PopMenu(4);
          break;
        case 72:
          /*Alt+H*/ PopMenu(5);
          break;
        }
      } else if (asc == 0) /*handling of other control keys*/ {
        if (gp_top_layer -> LayerNo == 0) /*If no submenu pops up*/ {
          switch (vkc) /* handle the arrow keys (left, right, down), do not respond to other control keys */ {
          case 37:
            gi_sel_menu--;
            if (gi_sel_menu == 0) {
              gi_sel_menu = 5;
            }
            TagMainMenu(gi_sel_menu);
            break;
          case 39:
            gi_sel_menu++;
            if (gi_sel_menu == 6) {
              gi_sel_menu = 1;
            }
            TagMainMenu(gi_sel_menu);
            break;
          case 40:
            PopMenu(gi_sel_menu);
            TagSubMenu(1);
            break;
          }
        } else /*When the submenu has popped up*/ {
          for (loc = 0, i = 1; i < gi_sel_menu; i++) {
            loc += ga_sub_menu_count[i - 1];
          } /* Calculate the position (subscript) of the first item in the submenu in the submenu string array*/
          switch (vkc) /*Arrow keys (left, right, up, down) processing*/ {
          case 37:
            gi_sel_menu--;
            if (gi_sel_menu < 1) {
              gi_sel_menu = 5;
            }
            TagMainMenu(gi_sel_menu);
            PopOff();
            PopMenu(gi_sel_menu);
            TagSubMenu(1);
            break;
          case 38:
            num = gi_sel_sub_menu - 1;
            if (num < 1) {
              num = ga_sub_menu_count[gi_sel_menu - 1];
            }
            if (strlen(ga_sub_menu[loc + num - 1]) == 0) {
              num--;
            }
            TagSubMenu(num);
            break;
          case 39:
            gi_sel_menu++;
            if (gi_sel_menu > 5) {
              gi_sel_menu = 1;
            }
            TagMainMenu(gi_sel_menu);
            PopOff();
            PopMenu(gi_sel_menu);
            TagSubMenu(1);
            break;
          case 40:
            num = gi_sel_sub_menu + 1;
            if (num > ga_sub_menu_count[gi_sel_menu - 1]) {
              num = 1;
            }
            if (strlen(ga_sub_menu[loc + num - 1]) == 0) {
              num++;
            }
            TagSubMenu(num);
            break;
          }
        }
      } else if ((asc - vkc == 0) || (asc - vkc == 32)) {
        /*Press a normal key*/
        if (gp_top_layer -> LayerNo == 0) /*If no submenu pops up*/ {
          switch (vkc) {
          case 70:
            /*f or F*/ PopMenu(1);
            break;
          case 77:
            /*m or M*/ PopMenu(2);
            break;
          case 81:
            /*q or Q*/ PopMenu(3);
            break;
          case 83:
            /*s or S*/ PopMenu(4);
            break;
          case 72:
            /*h or H*/ PopMenu(5);
            break;
          case 13:
            /*Enter*/
            PopMenu(gi_sel_menu);
            TagSubMenu(1);
            break;
          }
        } else /*Keyboard input processing when the submenu has popped up*/ {
          if (vkc == 27) /*if the ESC key is pressed*/ {
            PopOff();
            gi_sel_sub_menu = 0;
          } else if (vkc == 13) /*if enter is pressed*/ {
            num = gi_sel_sub_menu;
            PopOff();
            gi_sel_sub_menu = 0;
            bRet = ExeFunction(gi_sel_menu, num);
          } else /* other common key processing */ {
            /* Calculate the position (subscript) of the first item in the submenu in the submenu string array*/
            for (loc = 0, i = 1; i < gi_sel_menu; i++) {
              loc += ga_sub_menu_count[i - 1];
            }

            /* Compare with the representative characters of each item in the current submenu in turn*/
            for (i = loc; i < loc + ga_sub_menu_count[gi_sel_menu - 1]; i++) {
              if (strlen(ga_sub_menu[i]) > 0 && vkc == ga_sub_menu[i][1]) {
                /* if the match is successful */
                PopOff();
                gi_sel_sub_menu = 0;
                bRet = ExeFunction(gi_sel_menu, i - loc + 1);
              }
            }
          }
        }
      }
    }
  }
}

/**
 * Function name: ExeFunction
 * Function function: Execute the function determined by the main menu number and submenu number.
 * Input parameters: m main menu item number
 * s submenu item number
 * Output parameters: None
 * Return value: BOOL type, TRUE or FALSE
 *
 * Call description: Only when the function ExitSys is executed, it is possible to return FALSE, and it always returns TRUE in other cases
 */
BOOL ExeFunction(int m, int s) {
  BOOL bRet = TRUE;
  /*Array of function pointers, used to store the entry addresses of all function functions*/
  BOOL( * pFunction[ga_sub_menu_count[0] + ga_sub_menu_count[1] + ga_sub_menu_count[2] + ga_sub_menu_count[3] + ga_sub_menu_count[4]])(void);
  int i, loc;
  /*Save the entry address of the function function into the array element corresponding to the subscript of the main menu number and submenu number where the function function is located*/
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
  for (i = 1, loc = 0; i < m; i++) /*Calculate the corresponding subscript according to the main menu number and submenu number*/ {
    loc += ga_sub_menu_count[i - 1];
  }
  loc += s - 1;
  if (pFunction[loc] != NULL) {
    bRet = ( * pFunction[loc])(); /*use the function pointer to call the pointed function*/
  }
  return bRet;
}

/**
 * Function name: CloseSys
 * Function function: shut down the system.
 * Input parameters: hd main chain head pointer
 * Output parameters: None
 * Return value: None
 *
 * Call description:
 */
void CloseSys(void) {
  ClearMemory();
  ClearScreen(); /*clear screen*/
  /*Release the dynamic storage area for storing information such as menu bar, status bar, gender code and student category code*/
  free(gp_buff_menubar_info);
  free(gp_buff_stateBar_info);
  /*Close standard input and output device handles*/
  CloseHandle(gh_std_out);
  CloseHandle(gh_std_in);
  /* Set the window title bar to the end of the run */
  SetConsoleTitle("Exit");
  return;
}

/**
 * Function name: ExitSys
 * Function function: pop up the option to exit the system
 * Input parameters: None
 * Output parameters: none
 * Return value: BOOL type, exit is TRUE, cancel is FALSE
 *
 * Call description:
 */
BOOL ExitSys(void) {
  char * pCh[] = {
    "Do you want to exit?",
    "Yes",
    "No"
  };
  BOOL bRet = Message(pCh);
  PopOff();
  return bRet;
}

/**
 * Function name: ClearMemory
 * Function function: clear the memory
 * Input parameters: None
 * Output parameters: None
 * Return value: None
 *
 * Call description:
 */
void ClearMemory(void) {
  for (i = 0; i < telist -> length; i++) {
    for (j = 0; j < 6; j++)
      for (pocc = telist -> elem[i].ochead[j].head; pocc != NULL; pocc = telist -> elem[i].ochead[j].head) {
        telist -> elem[i].ochead[j].head = pocc -> next;
        free(pocc);
        pocc = NULL;
      }
  }
  free(telist -> elem);
  telist -> elem = NULL;
  telist -> length = 0;
  for (i = 0; i < rolist -> length; i++) {
    for (j = 0; j < 6; j++)
      for (pocc = rolist -> elem[i].ochead[j].head; pocc != NULL; pocc = rolist -> elem[i].ochead[j].head) {
        rolist -> elem[i].ochead[j].head = pocc -> next;
        free(pocc);
        pocc = NULL;
      }
  }
  free(rolist -> elem);
  rolist -> elem = NULL;
  rolist -> length = 0;
  for (i = 0; i < cougraph -> length; i++) { //Delete all prerequisite course nodes
    for (pcarc = cougraph -> coulist[i].archhead; pcarc != NULL; pcarc = cougraph -> coulist[i].archhead) {
      cougraph -> coulist[i].archhead = pcarc -> nextarc;
      free(pcarc -> prename);
      free(pcarc);
    }
  }
  free(cougraph -> coulist);
  cougraph -> coulist = NULL;
  cougraph -> length = 0;
  for (i = 0; i < 3; i++) {
    for (psch = grade[i].schead; psch != NULL; psch = grade[i].schead) {
      for (j = 0; j < 6; j++) {
        for (pcls = psch -> week[j].classhead; pcls != NULL; pcls = psch -> week[j].classhead) {
          psch -> week[j].classhead = pcls -> next;
          free(pcls);
          pcls = NULL;
        }
        for (pocc = psch -> week[j].ochead.head; pocc != NULL; pocc = psch -> week[j].ochead.head) {
          psch -> week[j].ochead.head = pocc -> next;
          free(pocc);
          pcls = NULL;
        }
      }
      grade[i].schead = psch -> next;
      free(psch);
      psch = NULL;
    }
  }
}

BOOL SaveData(void) {
  BOOL bRet = TRUE;
  char * plabel_name[] = {
    "Main Menu Option: File",
    "Sub Menu Option: Data Backup",
    "Yes"
  };

  ShowModule(plabel_name, 3);

  return bRet;
}

BOOL Initial(void) { //pass
  //Initialize classroom, teacher, course list structure
  telist = (TeList * ) malloc(sizeof(TeList));
  rolist = (RoList * ) malloc(sizeof(RoList));
  cougraph = (CouGraph * ) malloc(sizeof(CouGraph));
  //Initialize classroom, teacher, course linear lookup table
  telist -> elem = (Teacher * ) malloc(LIST_INIT_SIZE * sizeof(Teacher));
  rolist -> elem = (Room * ) malloc(LIST_INIT_SIZE * sizeof(Room));
  cougraph -> coulist = (CouHead * ) malloc(LIST_INIT_SIZE * sizeof(CouHead));
  if (!(telist -> elem && rolist -> elem && cougraph -> coulist)) return FALSE;
  //The length of each table is set to 0, and the pointer is set to NULL
  telist -> length = rolist -> length = cougraph -> length = 0;
  telist -> listsize = rolist -> listsize = cougraph -> listsize = LIST_INIT_SIZE;
  cougraph -> length = 0;
  grade[0].gr_num = grade[1].gr_num = grade[2].gr_num = 0;
  grade[0].schead = grade[1].schead = grade[2].schead = NULL;
  printf("System Startup Successfully");
  getchar();
  ClearScreen();
  return TRUE;
}

BOOL AddTeacher(void) { //pass
  fflush(stdin);
  printf("Teacher Name:");
  scanf("%s", cbuffer);
  getchar();
  for (i = 0; i < telist -> length; i++)
    if (!strcmp(telist -> elem[i].tname, cbuffer)) {
      printf("The teacher already exists!");
      getchar();
      ClearScreen();
      return TRUE;
    }
  AddNewTeacher(cbuffer);
  printf("Gender(Male--1\tFemale--0):");
  scanf("%hu", & telist -> elem[telist -> length].gender);
  getchar();
  printf("Age:");
  scanf("%hu", & telist -> elem[telist -> length].age);
  getchar();
  telist -> length++;
  printf("Inserted");
  getchar();
  ClearScreen();
  return TRUE;
}

BOOL AddRoom(void) { //pass
  fflush(stdin);
  printf("Room No.:");
  scanf("%s", cbuffer);
  getchar();
  for (i = 0; i < rolist -> length; i++)
    if (!strcmp(rolist -> elem[i].code, cbuffer)) {
      printf("The room already exists!");
      getchar();
      ClearScreen();
      return TRUE;
    }
  AddNewRoom(cbuffer);
  printf("Capacity:");
  scanf("%hu", & rolist -> elem[rolist -> length].rsize);
  getchar();
  rolist -> length++;
  printf("Insert Success");
  getchar();
  ClearScreen();
  return TRUE;
}

BOOL AddCourse(void) { //pass
  fflush(stdin);
  printf("Course Name:");
  scanf("%s", cbuffer);
  getchar();
  for (i = 0; i < cougraph -> length; i++)
    if (!strcmp(cougraph -> coulist[i].name, cbuffer)) {
      printf("The course already exists!");
      getchar();
      ClearScreen();
      return TRUE;
    }
  AddNewCourse(cbuffer);
  printf("Number of pre-requisites:");
  scanf("%hu", & (cougraph -> coulist[cougraph -> length].arcnum));
  getchar();
  printf("Name of each pre-requisite, \"Enter\" to separate name of each course:");
  for (j = 0; j < cougraph -> coulist[cougraph -> length].arcnum; j++) {
    scanf("%s", cbuffer);
    getchar();
    for (i = 0; i < cougraph -> length; i++)
      if (!strcmp(cougraph -> coulist[i].name, cbuffer)) { //Find the course in the course list
        pcarc = (CouArc * ) malloc(sizeof(CouArc));
        pcarc -> prename = (char * ) malloc(sizeof(cbuffer) * sizeof(char));
        strcpy(pcarc -> prename, cbuffer);
        pcarc -> nextarc = cougraph -> coulist[cougraph -> length].archhead;
        cougraph -> coulist[cougraph -> length].archhead = pcarc;
      }
    else printf("Do not have this course in system, abort.");
  }
  cougraph -> length++;
  printf("Inserted");
  getchar();
  ClearScreen();
  return TRUE;
}

BOOL AddGroup(void) { //pass
  char cbuf3[20];
  fflush(stdin);
  printf("Class Number:");
  psch = ClassInput(cbuf3);
  if (psch != NULL) {
    printf("The class already exists!");
    getchar();
    ClearScreen();
    return TRUE;
  }
  //Create the class node if it does not exist
  psch = (Schedule * ) malloc(sizeof(Schedule));
  strcpy(psch -> classname, cbuf3);
  printf("Number of courses taken:");
  scanf("%hu", & psch -> do_num);
  getchar();
  psch -> pr_num = 0; //Courses in progress cannot be entered manually, and need to be imported together with new courses
  printf("Name of each course taken, \"Enter\" to separate name of each course:\n");
  for (i = 0; i < psch -> do_num; i++) {
    scanf("%s", psch -> done[i]);
    getchar();
  }
  printf("Size:");
  scanf("%hu", & psch -> clsize);
  getchar();
  for (i = 0; i < 6; i++) { //New class class structure initialization
    psch -> week[i].classhead = NULL;
    psch -> week[i].cl_num = 0;
    psch -> week[i].ochead.head = NULL;
    psch -> week[i].ochead.len = 0;
  }
  // Insert new classes into the class-based curriculum by grade
  if (strstr(cbuf3, "15")) {
    psch -> next = grade[0].schead;
    grade[0].schead = psch;
    grade[0].gr_num++;
  } else if (strstr(cbuf3, "16")) {
    psch -> next = grade[1].schead;
    grade[1].schead = psch;
    grade[1].gr_num++;
  } else if (strstr(cbuf3, "17")) {
    psch -> next = grade[2].schead;
    grade[2].schead = psch;
    grade[2].gr_num++;
  }
  printf("Inserted");
  getchar();
  ClearScreen();
  return TRUE;
}

BOOL DelTeacher(void) {
  fflush(stdin);
  if (telist -> length == 0) {
    printf("No teachers in system!");
    getchar();
    ClearScreen();
    return TRUE;
  }
  printf("Teacher name:");
  scanf("%s", cbuffer);
  getchar();
  // Locate the target teacher and confirm its existence
  for (i = 0; i < telist -> length; i++) {
    if (!strcmp(telist -> elem[i].tname, cbuffer)) break;
  }
  if (i == telist -> length) {
    printf("Not Found.");
    getchar();
    ClearScreen();
    return TRUE;
  }
  //Delete the time occupancy linked list under the target teacher structure
  for (j = 0; j < 6; j++)
    for (pocc = telist -> elem[i].ochead[j].head; pocc != NULL; pocc = telist -> elem[i].ochead[j].head) {
      telist -> elem[i].ochead[j].head = pocc -> next;
      free(pocc);
    }
  //Delete all classes with corresponding teachers
  for (k = 0; k < 3; k++)
    for (psch = grade[k].schead; psch != NULL; psch = psch -> next)
      for (j = 0; j < 6; j++)
        for (pcls = psch -> week[j].classhead; pcls != NULL; pcls = pcls -> next)
          if (!strcmp(telist -> elem[i].tname, pcls -> tname)) { //Check whether the teacher's name matches
            strcpy(cbuf1, pcls -> roomcode);
            memcpy(buf3, pcls -> wk_st, sizeof(buf3));
            memcpy(buf4, pcls -> wk_ed, sizeof(buf4));
            buf5 = pcls -> cou_st;
            buf6 = pcls -> cou_ed; //Copy the course node information into the temporarily created course node
            //Delete the relevant time period in the subordinate array of the class
            DelTimeNode(pcls -> wk_st, pcls -> wk_ed, pcls -> cou_st, pcls -> cou_ed, & (psch -> week[j].ochead));
            SeekRemClass(psch, cbuffer); //Check whether the target course still has a class under this class
            //Remove seat time in classroom structure
            for (l = 0; l < rolist -> length; l++)
              if (!strcmp(rolist -> elem[l].code, cbuf1)) break;
            if (l != rolist -> length)
              DelTimeNode(buf3, buf4, buf5, buf6, & (rolist -> elem[l].ochead[j]));
            DelClassNode(pcls, psch, (unsigned short) j); //delete course node
          }
  //delete target teacher
  for (j = i; j < telist -> length - 1; j++)
    telist -> elem[j] = telist -> elem[j + 1];
  telist -> length--;
  printf("Deleted");
  if (telist -> length == 0) printf("Now there are no teachers in the system");
  getchar();
  ClearScreen();
  return TRUE;
}

BOOL DelRoom(void) {
  fflush(stdin);
  if (rolist -> length == 0) {
    printf("No rooms in system!");
    getchar();
    ClearScreen();
    return TRUE;
  }
  printf("No. of Room:");
  scanf("%s", cbuffer);
  getchar();
  // Locate the target classroom and confirm its existence
  for (i = 0; i < rolist -> length; i++) {
    if (!strcmp(rolist -> elem[i].code, cbuffer)) break;
  }
  if (i == rolist -> length) {
    printf("Not Found");
    getchar();
    ClearScreen();
    return TRUE;
  }
  //Delete all prerequisite nodes
  for (j = 0; j < 6; j++)
    for (pocc = rolist -> elem[i].ochead[j].head; pocc != NULL; pocc = rolist -> elem[i].ochead[j].head) {
      rolist -> elem[i].ochead[j].head = pocc -> next;
      free(pocc);
    }
  //Delete all classes that have corresponding classrooms
  for (k = 0; k < 3; k++)
    for (psch = grade[k].schead; psch != NULL; psch = psch -> next)
      for (j = 0; j < 6; j++)
        for (pcls = psch -> week[j].classhead; pcls != NULL; pcls = pcls -> next)
          if (!strcmp(rolist -> elem[i].code, pcls -> roomcode)) {
            strcpy(cbuf2, pcls -> tname); //Copy the course node information into the temporarily created course node
            memcpy(buf3, pcls -> wk_st, sizeof(buf3));
            memcpy(buf4, pcls -> wk_ed, sizeof(buf4));
            buf5 = pcls -> cou_st;
            buf6 = pcls -> cou_ed;
            //Delete the relevant time period in the subordinate array of the class
            DelTimeNode(pcls -> wk_st, pcls -> wk_ed, pcls -> cou_st, pcls -> cou_ed, & (psch -> week[j].ochead));
            SeekRemClass(psch, cbuffer); //Check whether the target course still has a class under this class
            // remove seat time in teacher struct
            for (l = 0; l < telist -> length; l++)
              if (!strcmp(telist -> elem[l].tname, cbuf2)) break;
            if (l != telist -> length)
              DelTimeNode(buf3, buf4, buf5, buf6, & (telist -> elem[l].ochead[j]));
            DelClassNode(pcls, psch, (unsigned short) j); //delete course node
          }
  for (j = i; j < rolist -> length - 1; j++) //Delete classroom structure
    rolist -> elem[j] = rolist -> elem[j + 1];
  rolist -> length--;
  printf("Deleted");
  if (rolist -> length == 0) printf("Now there are no rooms in the system");
  getchar();
  ClearScreen();
  return TRUE;
}

BOOL DelCourse(void) {
  fflush(stdin);
  if (cougraph -> length == 0) {
    printf("No courses in system!");
    getchar();
    ClearScreen();
    return TRUE;
  }
  CouArc * rcarc;
  printf("Course Name:");
  scanf("%s", cbuffer);
  getchar();
  // Locate the target course and confirm its existence
  for (i = 0; i < cougraph -> length; i++)
    if (!strcmp(cougraph -> coulist[i].name, cbuffer)) break;
  if (i == cougraph -> length) {
    printf("Not Found");
    getchar();
    ClearScreen();
    return TRUE;
  }
  //If the prerequisite course of a course in the course list is the target course, delete the target course node, and add the prerequisite course of the target course as the new prerequisite course
  for (j = 0; j < cougraph -> length; j++) {
    if (j == i) continue;
    qcarc = cougraph -> coulist[j].archhead;
    while (qcarc) { //There are arcs or edges
      if (!strcmp(qcarc -> prename, cbuffer)) { //The arc is pointed by the node to be deleted
        pcarc = cougraph -> coulist[i].archhead;
        while (pcarc) { //Point the prerequisite course of the prerequisite course to the current course
          rcarc = (CouArc * ) malloc(sizeof(CouArc));
          strcpy(rcarc -> prename, pcarc -> prename);
          rcarc -> nextarc = cougraph -> coulist[j].archhead;
          cougraph -> coulist[j].archhead = rcarc;
          pcarc = pcarc -> nextarc;
        }
        rcarc = cougraph -> coulist[j].archhead;
        while (rcarc -> nextarc != qcarc) rcarc = rcarc -> nextarc; //delete the arc whose head is the specified course
        rcarc -> nextarc = qcarc -> nextarc;
        free(qcarc -> prename);
        free(qcarc);
        qcarc = rcarc -> nextarc;
      }
      qcarc = qcarc -> nextarc;
    }
  }
  cougraph -> length--; //The length of the course list is reduced
  for (pcarc = cougraph -> coulist[i].archhead; pcarc != NULL; pcarc = cougraph -> coulist[i].archhead) { //Delete all prerequisite courses of the target course
    cougraph -> coulist[i].archhead = pcarc -> nextarc;
    free(pcarc -> prename);
    free(pcarc);
  }
  //Delete all classes with corresponding courses
  for (k = 0; k < 3; k++)
    for (psch = grade[k].schead; psch != NULL; psch = psch -> next) {
      for (j = 0; j < psch -> pr_num; j++)
        if (!strcmp(cougraph -> coulist[i].name, psch -> progress[j])) break;
      if (j == psch -> pr_num) continue;
      for (j = 0; j < 6; j++)
        for (pcls = psch -> week[j].classhead; pcls != NULL; pcls = pcls -> next)
          if (!strcmp(cougraph -> coulist[i].name, pcls -> coursename)) {
            strcpy(cbuf1, pcls -> roomcode);
            strcpy(cbuf2, pcls -> tname); //Copy the course node information into the temporarily created course node
            memcpy(buf3, pcls -> wk_st, sizeof(buf3));
            memcpy(buf4, pcls -> wk_ed, sizeof(buf4));
            buf5 = pcls -> cou_st;
            buf6 = pcls -> cou_ed;
            //Delete the relevant time period in the subordinate array of the class
            DelTimeNode(pcls -> wk_st, pcls -> wk_ed, pcls -> cou_st, pcls -> cou_ed, & (psch -> week[j].ochead));
            SeekRemClass(psch, cbuffer); //Check whether the target course still has a class under this class
            //Delete the target course occupancy time in the classroom structure
            for (l = 0; l < rolist -> length; l++)
              if (!strcmp(rolist -> elem[l].code, cbuf1)) break;
            if (l != rolist -> length)
              DelTimeNode(buf3, buf4, buf5, buf6, & (rolist -> elem[l].ochead[j]));
            // remove seat time in teacher struct
            for (l = 0; l < telist -> length; l++)
              if (!strcmp(telist -> elem[l].tname, cbuf2)) break;
            if (l != telist -> length)
              DelTimeNode(buf3, buf4, buf5, buf6, & (telist -> elem[l].ochead[j]));
            //delete target course
            DelClassNode(pcls, psch, (unsigned short) j); //delete course node
          }
    }
  for (j = i; j < cougraph -> length - 1; j++) //The vertex behind vertex v moves forward
    cougraph -> coulist[j] = cougraph -> coulist[j + 1];
  cougraph -> length--;
  printf("Deleted");
  if (cougraph -> length == 0) printf("Now there are no courses in the system");
  getchar();
  ClearScreen();
  return TRUE;
}

BOOL DelGroup(void) { //pass
  char cbuf3[20];
  fflush(stdin);
  Schedule * psch = NULL, * qsch = NULL;
  Class * pcls = NULL;
  printf("Will also delete all relevant sessions\nContinue? Yes--Y\tNo--any other key\n");
  j = getchar();
  getchar();
  if (j != 'y' && j != 'Y') {
    ClearScreen();
    return TRUE;
  }
  printf("Class Name:");
  psch = ClassInput(cbuf3);
  if (psch == NULL) {
    printf("Not Found");
    getchar();
    ClearScreen();
    return TRUE;
  }
  for (i = 0; i < 6; i++) {
    for (pcls = psch -> week[i].classhead; pcls != NULL; pcls = psch -> week[i].classhead) { //Delete all classes under the class
      psch -> week[i].classhead = pcls -> next;
      free(pcls);
    }
    for (pocc = psch -> week[i].ochead.head; pocc != NULL; pocc = psch -> week[i].ochead.head) { //Delete all time-occupied nodes under the class
      psch -> week[i].ochead.head = pocc -> next;
      free(pocc);
    }
  }
  // Find the class node according to the grade and delete it
  if (strstr(cbuf3, "15")) {
    if (psch == grade[0].schead) { //The target class is the head node
      grade[0].schead = psch -> next;
      free(psch);
    } else { //target class non-head node
      for (qsch = grade[0].schead; qsch -> next != psch; qsch = qsch -> next);
      qsch -> next = psch -> next;
      free(psch);
    }
    grade[0].gr_num--;
    if (grade[0].gr_num == 0) printf("Batch 2015 has no classes now");
  } else if (strstr(cbuf3, "16")) { //The target class is the head node
    if (psch == grade[1].schead) {
      grade[1].schead = psch -> next;
      free(psch);
    } else { //target class non-head node
      for (qsch = grade[1].schead; qsch -> next != psch; qsch = qsch -> next);
      qsch -> next = psch -> next;
      free(psch);
    }
    grade[1].gr_num--;
    if (grade[1].gr_num == 0) printf("Batch 2016 has no classes now");
  } else if (strstr(cbuf3, "17")) { //The target class is the head node
    if (psch == grade[2].schead) {
      grade[2].schead = psch -> next;
      free(psch);
    } else { //target class non-head node
      for (qsch = grade[2].schead; qsch -> next != psch; qsch = qsch -> next);
      qsch -> next = psch -> next;
      free(psch);
    }
    grade[2].gr_num--;
    if (grade[2].gr_num == 0) printf("Batch 2017 has no classes now");
  }
  printf("Deleted");
  getchar();
  ClearScreen();
  return TRUE;
}

BOOL AltTeacher(void) { //pass
  fflush(stdin);
  if (telist -> length == 0) {
    printf("No courses in system!");
    getchar();
    ClearScreen();
    return TRUE;
  }
  printf("Teacher name:");
  scanf("%s", cbuffer);
  getchar();
  for (i = 0; i < telist -> length; i++)
    if (!strcmp(telist -> elem[i].tname, cbuffer)) break; //i saves the position of the target teacher in the list
  if (i == telist -> length) {
    printf("Not Found");
    getchar();
    ClearScreen();
    return TRUE;
  }
  count = 0;
  //Choose the modification method
  do {
    if (count++) printf("Option invalid. Re-enter:");
    printf("What info to change?\n1--Name\t2--Gender\t3--Age\t4--Back to Menu\n");
    scanf("%hu", & choice);
    getchar();
    if (choice == 4) {
      ClearScreen();
      return TRUE;
    }
  } while (choice < 1 || choice > 4);
  switch (choice) {
  case 1:
    printf("New Name:");
    scanf("%s", cbuffer);
    getchar();
    //Verify the legitimacy of the new name
    for (j = 0; j < telist -> length; j++) {
      if (j == i) continue;
      if (!strcmp(telist -> elem[j].tname, cbuffer)) {
        printf("The name already exists! Abort update & return to menu...");
        getchar();
        ClearScreen();
        return TRUE;
      }
    }
    for (k = 0; k < 3; k++) //Modify all classes with corresponding teachers
      for (psch = grade[k].schead; psch != NULL; psch = psch -> next)
        for (j = 0; j < 6; j++)
          for (pcls = psch -> week[j].classhead; pcls != NULL; pcls = pcls -> next) //Find the class and modify it
            if (!strcmp(telist -> elem[i].tname, pcls -> tname)) strcpy(pcls -> tname, cbuffer);
    strcpy(telist -> elem[i].tname, cbuffer);
    printf("Updated");
    break;
  case 2:
    // There are only two possibilities for gender, just change each other
    if (telist -> elem[i].gender == 0) telist -> elem[i].gender = 1;
    else telist -> elem[i].gender = 0;
    printf("Gender updated. new gender is ");
    if (telist -> elem[i].gender == 0) printf("Female");
    else printf("Male");
    break;
  case 3:
    printf("New Age:");
    scanf("%hu", & telist -> elem[i].age);
    getchar();
    printf("Updated");
  case 4:
    break;
  }
  getchar();
  ClearScreen();
  return TRUE;
}

BOOL AltRoom(void) { //pass
  fflush(stdin);
  if (rolist -> length == 0) {
    printf("No rooms in system!");
    getchar();
    ClearScreen();
    return TRUE;
  }
  printf("Room No.:");
  scanf("%s", cbuffer);
  getchar();
  //Find the target classroom
  for (i = 0; i < rolist -> length; i++)
    if (!strcmp(rolist -> elem[i].code, cbuffer)) break; //i saves the position of the target classroom in the list
  if (i == rolist -> length) {
    printf("Not Found");
    getchar();
    ClearScreen();
    return TRUE;
  }
  //Choose the modification method
  count = 0;
  do {
    if (count) printf("Option invalid. Re-enter:");
    printf("What info to change?\n1--No.\t2--Capacity\t3--Back to Menu\n");
    scanf("%hu", & choice);
    getchar();
    count++;
    if (choice == 3) {
      ClearScreen();
      return TRUE;
    }
  } while (choice < 1 || choice > 3);
  switch (choice) {
  case 1:
    printf("New Room No.:");
    scanf("%s", cbuffer);
    getchar();
    // Check the validity of the new number
    for (j = 0; j < rolist -> length; j++) {
      if (j == i) continue;
      if (!strcmp(rolist -> elem[j].code, cbuffer)) {
        printf("New room No. is already occupied! Abort update & return to menu...");
        getchar();
        ClearScreen();
        return TRUE;
      }
    }
    for (k = 0; k < 3; k++) //Modify all classes with corresponding teachers
      for (psch = grade[k].schead; psch != NULL; psch = psch -> next)
        for (j = 0; j < 6; j++)
          for (pcls = psch -> week[j].classhead; pcls != NULL; pcls = pcls -> next)
            if (!strcmp(rolist -> elem[i].code, pcls -> roomcode)) strcpy(pcls -> roomcode, cbuffer);
    //Modify the classroom node
    strcpy(rolist -> elem[i].code, cbuffer);
    printf("Updated");
    break;
  case 2:
    printf("New Capacity:");
    scanf("%hu", & rolist -> elem[i].rsize);
    getchar();
    printf("Updated");
  case 3:
    break;
  }
  getchar();
  ClearScreen();
  return TRUE;
}

BOOL AltCourse(void) { //pass
  fflush(stdin);
  if (cougraph -> length == 0) {
    printf("No courses in system!");
    getchar();
    ClearScreen();
    return TRUE;
  }
  printf("Course Name:");
  scanf("%s", cbuffer);
  getchar();
  //Find the target course
  for (i = 0; i < cougraph -> length; i++)
    if (!strcmp(cougraph -> coulist[i].name, cbuffer)) break; //i saves the position of the target course in the list, and i cannot be used as an accumulator afterwards
  if (i == cougraph -> length) {
    printf("Not Found");
    getchar();
    ClearScreen();
    return TRUE;
  }
  //Choose the modification method
  count = 0;
  do {
    if (count++) printf("Option invalid. Re-enter:");
    printf("What info to change?\n1--Name\t2--Add a pre-requisite\t3--Delete a pre-requisite\t4--Back to Menu\n");
    scanf("%hu", & choice);
    getchar();
    if (choice == 4) {
      ClearScreen();
      return TRUE;
    }
  } while (choice < 1 || choice > 4);
  switch (choice) {
  case 1:
    printf("New Name:");
    scanf("%s", cbuffer);
    getchar();
    // Check the validity of the new class name
    for (j = 0; j < cougraph -> length; j++) {
      if (j == i) continue;
      if (!strcmp(cougraph -> coulist[j].name, cbuffer)) {
        printf("Course name is already occupied! Abort update & return to menu...");
        getchar();
        ClearScreen();
        return TRUE;
      }
    }
    for (j = 0; j < cougraph -> length; j++) {
      if (j == i) continue;
      for (pcarc = cougraph -> coulist[j].archhead; pcarc != NULL; pcarc = pcarc -> nextarc) //Modify all corresponding prerequisite course arc nodes
        if (!strcmp(cougraph -> coulist[i].name, pcarc -> prename))
          strcpy(pcarc -> prename, cbuffer);
    }
    for (k = 0; k < 3; k++) //Modify all classes with corresponding courses
      for (psch = grade[k].schead; psch != NULL; psch = psch -> next)
        for (j = 0; j < 6; j++)
          for (pcls = psch -> week[j].classhead; pcls != NULL; pcls = pcls -> next)
            if (!strcmp(cougraph -> coulist[i].name, pcls -> coursename)) strcpy(pcls -> coursename, cbuffer);
    //Modify the target course node
    strcpy(cougraph -> coulist[i].name, cbuffer);
    printf("Updated");
    break;
  case 2:
    do {
      printf("Name of the new pre-requisite:");
      scanf("%s", cbuffer);
      getchar();
      for (j = 0; j < cougraph -> length; j++) { //Find the corresponding course in the course lookup table
        if (j == i) continue;
        if (!strcmp(cougraph -> coulist[j].name, cbuffer)) break;
      }
      if (j == cougraph -> length) {
        printf("This course does not exist!");
        getchar();
        ClearScreen();
        return TRUE;
      }
      pcarc = cougraph -> coulist[i].archhead;
      while (pcarc) { //Check if the course is an existing prerequisite course
        if (!strcmp(pcarc -> prename, cbuffer)) {
          printf("Already a prerequisite");
          getchar();
          ClearScreen();
          return TRUE;
        }
        pcarc = pcarc -> nextarc;
      }
      pcarc = (CouArc * ) malloc((sizeof(CouArc))); //Insert the pre-requisite course arc node
      pcarc -> prename = (char * ) malloc(sizeof(cbuffer));
      strcpy(pcarc -> prename, cbuffer);
      //The new node is inserted into the prerequisite course linked list
      pcarc -> nextarc = cougraph -> coulist[i].archhead;
      cougraph -> coulist[i].archhead = pcarc;
      cougraph -> coulist[i].arcnum++;
      printf("Added.\nContinue? Yes--Y\tNo--Any other key");
      j = getchar();
      getchar();
    } while (j == 'Y' || j == 'y');
    break;
  case 3:
    if (cougraph -> coulist[i].arcnum == 0) {
      printf("This course do not have prerequisites");
      getchar();
      ClearScreen();
      return TRUE;
    }
    printf("Name of the prerequisite to be deleted:");
    scanf("%s", cbuffer);
    getchar();
    pcarc = cougraph -> coulist[i].archhead;
    while (pcarc) { //Check if the course is an existing prerequisite course
      if (!strcmp(pcarc -> prename, cbuffer)) break;
      pcarc = pcarc -> nextarc;
    }
    if (!pcarc) {
      printf("Not Found");
      getchar();
      ClearScreen();
      return TRUE;
    }
    if (pcarc == cougraph -> coulist[i].archhead) { //The node to be deleted is the head node
      cougraph -> coulist[i].archhead = pcarc -> nextarc;
      free(pcarc -> prename);
      free(pcarc);
    } else { //The node to be deleted is not the head node
      for (qcarc = cougraph -> coulist[i].archhead; qcarc -> nextarc != pcarc; qcarc = qcarc -> nextarc); //q moves to the predecessor of p
      qcarc -> nextarc = pcarc -> nextarc;
      free(pcarc -> prename);
      free(pcarc);
    }
    cougraph -> coulist[i].arcnum--;
    printf("Deleted");
    break;
  case 4:
    return TRUE;
  }
  getchar();
  ClearScreen();
  return TRUE;
}

BOOL AltGroup(void) { //pass
  char cbuf3[20];
  fflush(stdin);
  printf("Class Name:");
  psch = ClassInput(cbuf3);
  if (psch == NULL) {
    printf("Not Found");
    getchar();
    ClearScreen();
    return TRUE;
  }
  //Choose the modification method
  count = 0;
  do {
    if (count) printf("Option invalid. Re-enter:");
    printf("What info to change?\n1--Class No.\t2--Add a taken course\t3--Update a taken course\t4--Delete a taken course\t5--Size\t6--Back to Menu\n");
    scanf("%hu", & hubuffer);
    getchar();
    count++;
    if (choice == 6) {
      ClearScreen();
      return TRUE;
    }
  } while (hubuffer < 1 || hubuff > 6);
  switch (hubuffer) {
  case 1:
    flag = TRUE;
    do {
      printf("New Class No.:");
      scanf("%s", cbuf2);
      getchar();
      //Verify the legitimacy of the new class number
      if ((strstr(cbuf3, "15") && !strstr(cbuf2, "15")) || (strstr(cbuf3, "16") && !strstr(cbuf2, "16")) || (strstr(cbuf3, "17") && !strstr(cbuf2, "17"))) {
        printf("Not allowed to change batch!\n");
        flag = FALSE;
      }
    } while (flag == FALSE);
    //Determine the verification head node according to the class number
    if (strstr(cbuf2, "15")) qsch = grade[0].schead;
    else if (strstr(cbuf2, "16")) qsch = grade[1].schead;
    else if (strstr(cbuf2, "17")) qsch = grade[2].schead;
    // Verify the existence of the new class number
    for (; qsch != NULL; qsch = qsch -> next) {
      if (!strcmp(qsch -> classname, cbuf2)) {
        printf("Class already exists");
        getchar();
        ClearScreen();
        return TRUE;
      }
    }
    //Modify class number
    strcpy(psch -> classname, cbuf2);
    printf("Updated");
    break;
  case 2:
    printf("Course Name:");
    scanf("%s", cbuffer);
    getchar();
    //Verify the existence of the new course
    for (i = 0; i < psch -> do_num; i++)
      if (!strcmp(psch -> done[i], cbuffer)) {
        printf("Course already recorded as taken");
        getchar();
        ClearScreen();
        return TRUE;
      }
    //add course
    strcpy(psch -> done[psch -> do_num], cbuffer);
    psch -> do_num++;
    printf("Inserted");
    break;
  case 3:
    printf("Course Name:");
    scanf("%s", cbuffer);
    getchar();
    // Check the existence of the course to be modified
    for (i = 0; i < psch -> do_num; i++)
      if (!strcmp(psch -> done[i], cbuffer)) break;
    if (j == psch -> do_num) {
      printf("Not Found");
      getchar();
      ClearScreen();
      return TRUE;
    }
    printf("New Name:");
    scanf("%s", cbuffer);
    getchar();
    //Verify the legitimacy of the course name stored in the new class number
    for (j = 0; j < psch -> do_num; j++) {
      if (!strcmp(psch -> done[j], cbuffer)) {
        printf("Course already recorded as taken");
        getchar();
        ClearScreen();
        return TRUE;
      }
    }
    //Modify the course
    strcpy(psch -> done[i], cbuffer);
    printf("Updated");
    break;
  case 4:
    if (psch -> do_num == 0) {
      printf("No taken courses");
      break;
    }
    printf("Course Name:");
    scanf("%c", cbuffer);
    getchar();
    // Check the existence of courses to be deleted
    for (i = 0; i < psch -> do_num; i++)
      if (!strcmp(psch -> done[i], cbuffer)) break;
    if (i == psch -> do_num) {
      printf("Not Found");
      getchar();
      ClearScreen();
      return TRUE;
    }
    //delete course
    for (j = i; j < psch -> do_num - 1; j++)
      strcpy(psch -> done[j], psch -> done[j + 1]);
    psch -> do_num--;
    if (psch -> do_num == 0) printf("This class has no taken courses now");
    printf("Deleted");
    getchar();
    break;
  case 5:
    printf("New Size:");
    scanf("%hu", & (psch -> clsize));
    getchar();
    printf("Updated");
    /* Shield the interface of modifying the course in progress
    case 5:
    printf("Please enter the new course name:");
    scanf("%s", cbuffer); getchar();
    for(i=0; i<psch->pr_num; i++){
    if(!strcmp(psch->progress[i], cbuffer)){
    printf("The class already exists"); return FALSE;
    }
    }
    strcpy(psch->progress[psch->pr_num], cbuffer);
    psch->pr_num++;
    printf("Added successfully");
    break;
    case 6:
    if(psch->pr_num==0){
    printf("There are no courses in progress"); break;
    }
    printf("Please enter the name of the course to be changed:");
    scanf("%s", cbuffer); getchar();
    for(i=0; i<psch->pr_num; i++)
    if(!strcmp(psch->progress[i],cbuffer)) break;
    if(i==psch->pr_num){
    printf("Course not found"); return FALSE;
    }
    printf("Please enter the new course name:");
    scanf("%s", cbuffer); getchar();
    for(j=0;j<psch->pr_num;j++){
    if(!strcmp(psch->progress[j], cbuffer)){
    printf("The class already exists"); return FALSE;
    }
    }
    strcpy(psch->progress[i], cbuffer);
    printf("modified successfully");
    break;
    case 7:
    if(psch->pr_num==0){
    printf("No courses in progress"); break;
    }
    printf("Please enter the name of the course to be deleted:");
    scanf("%s", cbuffer); getchar();
    for(i=0; i<psch->pr_num; i++)
    if(!strcmp(psch->progress[i],cbuffer)) break;
    if(i==psch->pr_num){
    printf("Course not found"); return FALSE;
    }
    for(j=i;j<psch->pr_num-1;j++)
    strcpy(psch->progress[j], psch->progress[j+1]);
    printf("Deleted successfully");
    psch->pr_num--;
    if(psch->pr_num==0) printf("There are no courses in the table");
    break;
    */
  case 6:
    break;
  }
  getchar();
  ClearScreen();
  return TRUE;
}

BOOL SeekTeacher(void) { //pass
  fflush(stdin);
  if (telist -> length == 0) {
    printf("Teacher list is empty");
    getchar();
    ClearScreen();
    return TRUE;
  }
  count = 0;
  flag = FALSE;
  do {
    if (count) printf("The option is out of bounds, re-enter");
    printf("Search method:\n1--name\t2--age range\t3--gender\t4--return to main menu\n");
    scanf("%hu", & choice);
    getchar();
    count++;
    if (choice == 4) {
      ClearScreen();
      return TRUE;
    }
  } while (choice < 1 || choice > 4);
  switch (choice) {
  case 1: //Search by name
    printf("Please enter the teacher's name:");
    scanf("%s", cbuffer);
    getchar();
    for (i = 0; i < telist -> length; i++)
      if (!strcmp(telist -> elem[i].tname, cbuffer)) break;
    if (i == telist -> length) {
      printf("No corresponding teacher found");
      getchar();
      ClearScreen();
      return TRUE;
    }
    printf("Gender:");
    if (telist -> elem[i].gender == 0) printf("female");
    else printf("male");
    printf("Age: %hu\nfind success", telist -> elem[i].age);
    break;
  case 2: //Search by age range
    printf("Enter the minimum age limit:");
    scanf("%hu", & buf1);
    getchar();
    printf("Enter the upper limit of age:");
    scanf("%hu", & buf2);
    getchar();
    for (i = 0; i < telist -> length; i++)
      if (telist -> elem[i].age >= buf1 && telist -> elem[i].age <= buf2) {
        printf("Name: %s\tAge: %hu\tGender:", telist -> elem[i].tname, telist -> elem[i].age);
        flag = TRUE;
        if (telist -> elem[i].gender == 0) printf("female\n");
        else printf("Male\n");
      }
    if (flag == FALSE) printf("No qualified teacher");
    getchar();
    break;
  case 3: //Search by gender
    printf("Enter the target gender (male--1\tfemale--0):");
    j = getchar();
    getchar();
    for (i = 0; i < telist -> length; i++) {
      if (j == '0') {
        if (!telist -> elem[i].gender) {
          printf("Name: %s\tAge:%hu\n", telist -> elem[i].tname, telist -> elem[i].age);
          flag = TRUE; //flag mark output valid information
        }
      } else {
        if (telist -> elem[i].gender) {
          printf("Name: %s\tAge:%hu\n", telist -> elem[i].tname, telist -> elem[i].age);
          flag = TRUE; //flag mark output valid information
        }
      }
    }
    if (flag == FALSE) printf("No qualified teacher");
    getchar();
  case 4:
    break;
  }
  getchar();
  ClearScreen();
  return TRUE;
}

BOOL SeekRoom(void) { //pass
  fflush(stdin);
  if (rolist -> length == 0) {
    printf("Classroom list is empty");
    getchar();
    ClearScreen();
    return TRUE;
  }
  count = 0;
  do {
    if (count) printf("The option is out of bounds, re-enter");
    printf("Search method:\n1--number\t2--capacity\t3--back to main menu\n");
    scanf("%hu", & choice);
    getchar();
    count++;
    if (choice == 3) {
      ClearScreen();
      return TRUE;
    }
  } while (choice < 1 || choice > 3);
  switch (choice) {
  case 1: //Search by classroom number
    printf("Please enter the classroom number:");
    scanf("%s", cbuffer);
    getchar();
    for (i = 0; i < rolist -> length; i++)
      if (!strcmp(rolist -> elem[i].code, cbuffer)) break;
    if (i == rolist -> length) {
      printf("Could not find the corresponding classroom");
      getchar();
      ClearScreen();
      return TRUE;
    }
    printf("Classroom capacity: %hu\n searched successfully", rolist -> elem[i].rsize);
    break;
  case 2: //Search by classroom capacity
    flag = FALSE;
    printf("Enter the lower limit of capacity:");
    scanf("%hu", & buf1);
    getchar();
    printf("Enter the upper limit of capacity:");
    scanf("%hu", & buf2);
    getchar();
    for (i = 0; i < rolist -> length; i++)
      if (rolist -> elem[i].rsize >= buf1 && rolist -> elem[i].rsize <= buf2) {
        printf("Code: %s\tCapacity:%hu\n", rolist -> elem[i].code, rolist -> elem[i].rsize);
        flag = TRUE; //flag mark output valid information
      }
    if (flag == FALSE) printf("There is no classroom that meets the requirements");
    getchar();
  case 3:
    break;
  }
  getchar();
  ClearScreen();
  return TRUE;
}

BOOL SeekCourse(void) { //pass
  fflush(stdin);
  BOOL glag = FALSE;
  if (cougraph -> length == 0) {
    printf("Course list is empty");
    getchar();
    ClearScreen();
    return TRUE;
  }
  printf("Please enter the substring of the course name to be checked:");
  scanf("%s", cbuffer);
  getchar();
  for (i = 0; i < cougraph -> length; i++)
    // fuzzy search
    if (strstr(cougraph -> coulist[i].name, cbuffer)) {
      printf("Course name: %s\n", cougraph -> coulist[i].name);
      flag = FALSE;
      printf("Prerequisite courses:\n");
      for (pcarc = cougraph -> coulist[i].archhead; pcarc != NULL; pcarc = pcarc -> nextarc) {
        printf("%s\n", pcarc -> prename);
        flag = TRUE; //flag tag output valid information
      }
      if (flag == FALSE) printf("There is no prerequisite course for this course\n");
      flag = FALSE;
      printf("Successor course:\n");
      for (j = 0; j < cougraph -> length; j++) {
        if (j == i) continue;
        //Search method: find out whether the current course has a prerequisite course as the target course
        for (pcarc = cougraph -> coulist[j].archhead; pcarc != NULL; pcarc = pcarc -> nextarc)
          if (!strcmp(pcarc -> prename, cougraph -> coulist[i].name)) {
            printf("%s\n", cougraph -> coulist[j].name);
            flag = TRUE; //flag tag output valid information
          }
      }
      if (flag == FALSE) printf("There is no follow-up course for this course\n");
      glag = TRUE;
    }
  if (glag == FALSE) printf("No corresponding course found");
  getchar();
  ClearScreen();
  return TRUE;
}

BOOL SeekGroup(void) { //pass
  char cbuf3[20];
  fflush(stdin);
  if (!grade[0].schead && !grade[1].schead && !grade[2].schead) {
    printf("Class list is empty");
    return FALSE;
  }
  count = 0;
  do {
    if (count) printf("The option is out of bounds, re-enter");
    printf("Search method\n1--class number\t2--number of people\t3--return to main menu\n");
    scanf("%hu", & choice);
    getchar();
    count++;
    if (choice == 3) {
      ClearScreen();
      return TRUE;
    }
  } while (choice < 1 || choice > 3);
  switch (choice) {
  case 1: //Search by class number
    printf("Please enter the name of the class to be checked:");
    psch = ClassInput(cbuf3);
    if (psch == NULL) {
      printf("This class does not exist");
      getchar();
      ClearScreen();
      return TRUE;
    }
    // output class related information
    printf("Number of people:%hu\n", psch -> clsize);
    if (psch -> do_num != 0) {
      printf("List of courses taken:\n");
      for (j = 0; j < psch -> do_num; j++)
        printf("%s\n", psch -> done[j]);
      printf("A total of %hu doors\n", psch -> do_num);
    } else printf("No courses taken\n");
    if (psch -> pr_num != 0) {
      printf("Course list:\n");
      for (j = 0; j < psch -> pr_num; j++)
        printf("%s\n", psch -> progress[j]);
      printf("A total of %hu doors\n", psch -> pr_num);
    } else printf("No current courses\n");
    break;
  case 2: //Search by class size
    flag = FALSE;
    printf("Enter the minimum number of people:");
    scanf("%hu", & buf1);
    getchar();
    printf("Enter the upper limit of the number of people:");
    scanf("%hu", & buf2);
    getchar();
    for (i = 0; i < 3; i++)
      for (psch = grade[i].schead; psch != NULL; psch = psch -> next)
        if (psch -> clsize >= buf1 && psch -> clsize <= buf2) {
          // output class related information
          printf("Class: %s\tnumber of people:%hu\n", psch -> classname, psch -> clsize);
          if (psch -> do_num != 0) {
            printf("List of courses taken:\n");
            for (j = 0; j < psch -> do_num; j++)
              printf("%s\n", psch -> done[j]);
            printf("A total of %hu doors\n", psch -> do_num);
          } else printf("No courses taken\n");
          if (psch -> pr_num != 0) {
            printf("Course list:\n");
            for (j = 0; j < psch -> pr_num; j++)
              printf("%s\n", psch -> progress[j]);
            printf("A total of %hu doors\n", psch -> pr_num);
          } else printf("No current courses\n");
          flag = TRUE; //flag mark output valid information
        }
    if (flag == FALSE) printf("No class that meets the requirements");
    getchar();
  case 3:
    break;
  }
  getchar();
  ClearScreen();
  return TRUE;
}

BOOL AddClass(void) { //pass
  fflush(stdin);
  Class * ptemp = (Class * ) malloc(sizeof(Class));
  //The course name is written into the sample structure
  printf("Enter course name:");
  scanf("%s", ptemp -> coursename);
  getchar();
  for (i = 0; i < cougraph -> length; i++)
    if (!strcmp(cougraph -> coulist[i].name, ptemp -> coursename)) break;
  if (i == cougraph -> length) { //Create a new course (the prerequisite course node is not added)
    AddNewCourse(ptemp -> coursename);
    cougraph -> length++;
    printf("The new course has been imported\n");
  }
  //classroom number write sample structure
  printf("Enter classroom number:");
  scanf("%s", ptemp -> roomcode);
  getchar(); //How to deal with multiple classrooms?
  for (k = 0; k < rolist -> length; k++)
    if (!strcmp(rolist -> elem[k].code, ptemp -> roomcode)) break; //k stores the position of the target classroom in the classroom list
  if (k == rolist -> length) AddNewRoom(ptemp -> roomcode); //New classroom
  rolist -> length++;
  // Write the teacher's name into the sample structure
  printf("Enter the teacher's name:");
  scanf("%s", ptemp -> tname);
  getchar(); //How to deal with multiple teachers?
  for (l = 0; l < telist -> length; l++)
    if (!strcmp(telist -> elem[l].tname, ptemp -> tname)) break; //l stores the position of the target teacher in the teacher list
  if (l == telist -> length) AddNewTeacher(ptemp -> tname); //New teacher
  telist -> length++;
  //Classroom information is stored in a temporary node, and after the class is determined, use it as a template to copy the node to each class
  printf("Enter class week (Arabic numerals):");
  scanf("%hu", & buf1);
  getchar();
  memset(ptemp -> wk_st, 0, sizeof(ptemp -> wk_st));
  memset(ptemp -> wk_ed, 0, sizeof(ptemp -> wk_ed));
  TimeInput( & (ptemp -> cou_st), & (ptemp -> cou_ed), ptemp -> wk_st, ptemp -> wk_ed);
  count = 0;
  printf("Enter the number of classes:");
  scanf("%hu", & hubuffer);
  getchar();
  for (i = 0; i < hubuffer; i++) {
    pcls = (Class * ) malloc(sizeof(Class));
    //Copy the sample structure for accessing the linked list
    strcpy(pcls -> coursename, ptemp -> coursename);
    strcpy(pcls -> roomcode, ptemp -> roomcode);
    strcpy(pcls -> tname, ptemp -> tname);
    pcls -> cou_st = ptemp -> cou_st;
    pcls -> cou_ed = ptemp -> cou_ed;
    memcpy(pcls -> wk_st, ptemp -> wk_st, sizeof(ptemp -> wk_st));
    memcpy(pcls -> wk_ed, ptemp -> wk_ed, sizeof(ptemp -> wk_ed));
    //Enter class information
    printf("Enter class number:");
    //Determine the search head node according to the class number
    scanf("%s", cbuffer);
    getchar();
    if (strstr(cbuffer, "15")) {
      // Find the existence of the target class
      for (psch = grade[0].schead; psch != NULL; psch = psch -> next)
        if (!strcmp(psch -> classname, cbuffer)) break;
      if (psch == NULL) { //Insert a new class in the class schedule structure
        psch = (Schedule * ) malloc(sizeof(Schedule));
        strcpy(psch -> classname, cbuffer);
        psch -> do_num = psch -> pr_num = 0;
        printf("Enter the number of people:");
        scanf("%hu", & (psch -> clsize));
        getchar();
        psch -> next = grade[0].schead;
        grade[0].schead = psch;
        for (j = 0; j < 6; j++) { //Time occupancy and weekly schedule structure initialization
          psch -> week[j].classhead = NULL;
          psch -> week[j].cl_num = 0;
          psch -> week[j].ochead.head = NULL;
          psch -> week[j].ochead.len = 0;
        }
        grade[0].gr_num++;
      }
    } else if (strstr(cbuffer, "16")) {
      // Find the existence of the target class
      for (psch = grade[1].schead; psch != NULL; psch = psch -> next)
        if (!strcmp(psch -> classname, cbuffer)) break;
      if (psch == NULL) { //Insert a new class in the class schedule structure
        psch = (Schedule * ) malloc(sizeof(Schedule));
        strcpy(psch -> classname, cbuffer);
        psch -> do_num = psch -> pr_num = 0;
        psch -> next = grade[1].schead;
        grade[1].schead = psch;
        for (j = 0; j < 6; j++) { //Time occupancy and weekly schedule structure initialization
          psch -> week[j].classhead = NULL;
          psch -> week[j].cl_num = 0;
          psch -> week[j].ochead.head = NULL;
          psch -> week[j].ochead.len = 0;
        }
        grade[1].gr_num++;
      }
    }
    // Find the existence of the target class
    else if (strstr(cbuffer, "17")) {
      for (psch = grade[2].schead; psch != NULL; psch = psch -> next)
        if (!strcmp(psch -> classname, cbuffer)) break;
      if (psch == NULL) { //Insert a new class in the class schedule structure
        psch = (Schedule * ) malloc(sizeof(Schedule));
        strcpy(psch -> classname, cbuffer);
        psch -> do_num = psch -> pr_num = 0;
        psch -> next = grade[2].schead;
        grade[2].schead = psch;
        for (j = 0; j < 6; j++) { //Time occupancy and weekly schedule structure initialization
          psch -> week[j].classhead = NULL;
          psch -> week[j].cl_num = 0;
          psch -> week[j].ochead.head = NULL;
          psch -> week[j].ochead.len = 0;
        }
        grade[2].gr_num++;
      }
    } else { // intercept garbled characters
      printf("The class number input error");
      getchar();
      ClearScreen();
      return TRUE;
    }
    for (j = 0; j < psch -> pr_num; j++) //Find whether there is a target course in the course list
      if (!strcmp(psch -> progress[j], ptemp -> coursename)) break;
    if (j == psch -> pr_num) { //Add this class if there is no
      strcpy(psch -> progress[j], ptemp -> coursename);
      psch -> pr_num++;
    }
    //insert course node
    //Special case: currently there is no class for this day or the new class is the first class of the day
    if (psch -> week[buf1 - 1].classhead == NULL || psch -> week[buf1 - 1].classhead -> cou_st > pcls -> cou_st || psch -> week[buf1 - 1].classhead -> wk_st[0] > pcls -> wk_st[0]) {
      pcls -> next = psch -> week[buf1 - 1].classhead;
      psch -> week[buf1 - 1].classhead = pcls;
    } else { //Insert in the middle of the course list
      for (qcls = psch -> week[buf1 - 1].classhead; qcls -> next != NULL; qcls = qcls -> next)
        if (qcls -> next -> cou_st > pcls -> cou_st || (qcls -> next -> cou_st == pcls -> cou_st && qcls -> next -> wk_st[0] > pcls -> wk_st[0])) {
          pcls -> next = qcls -> next;
          qcls -> next = pcls;
          break;
        }
      if (qcls -> next == NULL) { //Insert at the end of the course list
        pcls -> next = qcls -> next;
        qcls -> next = pcls;
      }
    }
    psch -> week[buf1 - 1].cl_num++;
    //Occupy the relevant time period in the subordinate array of the class
    AddTimeNode(pcls -> wk_st, pcls -> wk_ed, pcls -> cou_st, pcls -> cou_ed, & (psch -> week[buf1 - 1].ochead));
  }
  /*
  //Insert a time-occupancy node under the corresponding teacher node
  AddTimeNode(ptemp->wk_st, ptemp->wk_ed, ptemp->cou_st, ptemp->cou_ed, &(telist->elem[l].ochead[buf1-1]));
  //Insert a time-occupancy node under the corresponding classroom node
  AddTimeNode(ptemp->wk_st, ptemp->wk_ed, ptemp->cou_st, ptemp->cou_ed, &(rolist->elem[k].ochead[buf1-1]));
  */
  count = 0;
  while (ptemp -> wk_st[count] && count < 5) {
    for (i = ptemp -> wk_st[count]; i <= ptemp -> wk_ed[count]; i++) {
      for (j = ptemp -> cou_st; j <= ptemp -> cou_ed; j++) {
        //Insert the time-occupied nodes in ascending order of the number of weeks first and then the number of sections
        pocc = (Occupy * ) malloc(sizeof(Occupy));
        pocc -> week = i;
        pocc -> time = j;
        if (telist -> elem[l].ochead[buf1 - 1].len == 0) { //Insert at the first place when there is no node
          pocc -> next = telist -> elem[l].ochead[buf1 - 1].head;
          telist -> elem[l].ochead[buf1 - 1].head = pocc;
        } else {
          dwk = pocc -> week - telist -> elem[l].ochead[buf1 - 1].head -> week;
          dtm = pocc -> time - telist -> elem[l].ochead[buf1 - 1].head -> time;
          if (dwk < 0 || (dwk >= 0 && dtm < 0)) { //The minimum value of the new node
            pocc -> next = telist -> elem[l].ochead[buf1 - 1].head;
            telist -> elem[l].ochead[buf1 - 1].head = pocc;
          } else { //Insert in the middle of the time-occupied linked list
            for (qocc = telist -> elem[l].ochead[buf1 - 1].head; qocc -> next != NULL; qocc = qocc -> next) {
              dwk = pocc -> week - qocc -> next -> week;
              dtm = pocc -> time - qocc -> next -> time;
              if (dwk < 0 || (dwk >= 0 && dtm < 0)) {
                pocc -> next = qocc -> next;
                qocc -> next = pocc;
              }
            }
            if (qocc -> next == NULL) { //Insert at the end of the time-occupied linked list
              pocc -> next = qocc -> next;
              qocc -> next = pocc;
            }
          }
        }
        telist -> elem[l].ochead[buf1 - 1].len++;
      }
    }
    count++;
  }
  //Insert a time-occupancy node under the corresponding classroom node
  count = 0;
  while (ptemp -> wk_st[count] && count < 5) {
    for (i = ptemp -> wk_st[count]; i <= ptemp -> wk_ed[count]; i++) {
      for (j = ptemp -> cou_st; j <= ptemp -> cou_ed; j++) {
        //Insert the time-occupied nodes in ascending order of the number of weeks first and then the number of sections
        pocc = (Occupy * ) malloc(sizeof(Occupy));
        pocc -> week = i;
        pocc -> time = j;
        //add seat time to teacher structure
        if (rolist -> elem[k].ochead[buf1 - 1].len == 0) { //Insert at the first place when there is no node
          pocc -> next = rolist -> elem[k].ochead[buf1 - 1].head;
          rolist -> elem[k].ochead[buf1 - 1].head = pocc;
        } else {
          dwk = pocc -> week - rolist -> elem[k].ochead[buf1 - 1].head -> week;
          dtm = pocc -> time - rolist -> elem[k].ochead[buf1 - 1].head -> time;
          if (dwk < 0 || (dwk >= 0 && dtm < 0)) { //The minimum value of the new node
            pocc -> next = rolist -> elem[k].ochead[buf1 - 1].head;
            rolist -> elem[k].ochead[buf1 - 1].head = pocc;
          } else { //Insert in the middle of the time-occupied linked list
            for (qocc = rolist -> elem[k].ochead[buf1 - 1].head; qocc -> next != NULL; qocc = qocc -> next) {
              dwk = pocc -> week - qocc -> next -> week;
              dtm = pocc -> time - qocc -> next -> time;
              if (dwk < 0 || (dwk >= 0 && dtm < 0)) {
                pocc -> next = qocc -> next;
                qocc -> next = pocc;
              }
            }
            if (qocc -> next == NULL) { //Insert at the end of the time-occupied linked list
              pocc -> next = qocc -> next;
              qocc -> next = pocc;
            }
          }
        }
        rolist -> elem[k].ochead[buf1 - 1].len++;
        //add seat time to classroom structure
      }
    }
    count++;
  }
  printf("Entered");
  getchar();
  ClearScreen();
  return TRUE;
}

BOOL DelClass(void) { //pass
  fflush(stdin);
  char cbuf3[20];
  unsigned short buf3[5], buf4[5], buf5 = 0, buf6 = 0;
  printf("Enter the course name of the class to be deleted:");
  scanf("%s", cbuffer);
  getchar();
  printf("Enter the number of classes to be deleted:");
  scanf("%hu", & hubuffer);
  getchar();
  printf("Enter class date (Arabic numerals)");
  scanf("%hu", & buf1);
  getchar();
  for (i = 0; i < hubuffer; i++) {
    count = 0;
    printf("Enter class number:");
    psch = ClassInput(cbuf3);
    if (psch == NULL) {
      printf("The class is not found, continue to operate the next class");
      getchar();
      continue;
    }
    //delete target course
    for (pcls = psch -> week[buf1 - 1].classhead; pcls != NULL; pcls = pcls -> next)
      if (!strcmp(pcls -> coursename, cbuffer)) break;
    if (pcls == NULL) {
      printf("No corresponding course found under this date\n");
      break;
    }
    //Copy node information
    strcpy(cbuf1, pcls -> roomcode);
    strcpy(cbuf2, pcls -> tname);
    memcpy(buf3, pcls -> wk_st, sizeof(buf3));
    memcpy(buf4, pcls -> wk_ed, sizeof(buf4));
    buf5 = pcls -> cou_st;
    buf6 = pcls -> cou_ed;
    //Delete the relevant time period in the subordinate array of the class
    DelTimeNode(pcls -> wk_st, pcls -> wk_ed, pcls -> cou_st, pcls -> cou_ed, & (psch -> week[buf1 - 1].ochead));
    DelClassNode(pcls, psch, buf1 - 1); //delete course node
  }
  //Delete the occupancy time node in the classroom structure
  for (i = 0; i < rolist -> length; i++)
    if (!strcmp(rolist -> elem[i].code, cbuf1)) break;
  if (i != rolist -> length)
    DelTimeNode(buf3, buf4, buf5, buf6, & (rolist -> elem[i].ochead[buf1 - 1]));
  // remove seat time in teacher struct
  for (i = 0; i < telist -> length; i++)
    if (!strcmp(telist -> elem[i].tname, cbuf2)) break;
  if (i != telist -> length)
    DelTimeNode(buf3, buf4, buf5, buf6, & (telist -> elem[i].ochead[buf1 - 1]));
  printf("Deleted successfully");
  getchar();
  ClearScreen();
  return TRUE;
}

BOOL AltClass(void) { //pass
  fflush(stdin);
  unsigned short buf3[5], buf4[5], buf5, buf6, buf7, buf8[5], buf9[5];
  //buf1-buf4 is new information, buf6-buf9 is old information
  char cbuf3[20];
  Class * pcls = NULL;
  Schedule * psch = NULL;
  printf("Enter the course name of the class to be changed:");
  scanf("%s", cbuffer);
  getchar();
  printf("Enter the number of classes to be changed:");
  scanf("%hu", & hubuffer);
  getchar();
  printf("Enter school day (Arabic numerals)");
  scanf("%hu", & buf5);
  getchar(); //The above information is used to locate the class in the class schedule
  count = 0;
  do {
    if (count) printf("The option is out of bounds, re-enter");
    printf("What information to modify?\n1--classroom\t2--teacher\t3--class time\t4--return to main menu\n");
    scanf("%hu", & choice);
    getchar();
    count++;
    if (choice == 4) {
      ClearScreen();
      return TRUE;
    }
  } while (choice < 1 || choice > 4);
  switch (choice) {
  case 1: //Change the classroom
    printf("Enter new classroom:");
    scanf("%s", cbuf2);
    getchar();
    for (l = 0; l < rolist -> length; l++)
      if (!strcmp(rolist -> elem[l].code, cbuf2)) break;
    if (l == rolist -> length) AddNewRoom(cbuf2); //Change to a new classroom and expand the linear table when it crosses the boundary
    rolist -> length++;
    //l record the new classroom position in the lookup table
    break;
  case 2: //change teacher
    printf("Enter the new teacher's name:");
    scanf("%s", cbuf2);
    getchar();
    for (l = 0; l < telist -> length; l++)
      if (!strcmp(telist -> elem[l].tname, cbuf2)) break;
    if (l == telist -> length) AddNewTeacher(cbuf2); //New teacher
    telist -> length++;
    //l record the new teacher's position in the lookup table
    break;
  case 3: //Change time
    memset(buf3, 0, sizeof(buf3));
    memset(buf4, 0, sizeof(buf4));
    TimeInput( & buf1, & buf2, buf3, buf4);
    break;
  case 4:
    return TRUE;
  }
  for (i = 0; i < hubuffer; i++) {
    printf("Enter class number:");
    psch = ClassInput(cbuf3);
    if (psch == NULL) {
      printf("Class %s not found, continue to search for the next class\n", cbuf3);
      continue;
    }
    // check the existence of the input class
    for (pcls = psch -> week[buf5 - 1].classhead; pcls != NULL; pcls = pcls -> next)
      if (!strcmp(pcls -> coursename, cbuffer)) break;
    if (pcls == NULL) {
      printf("The target class is not found, continue to search for the next class\n");
      continue;
    }
    buf6 = pcls -> cou_st;
    buf7 = pcls -> cou_ed;
    memcpy(buf8, pcls -> wk_st, sizeof(pcls -> wk_st));
    memcpy(buf9, pcls -> wk_ed, sizeof(pcls -> wk_ed)); // Put the original class time into the cache
    switch (choice) {
    case 1: //Modify classroom occupancy
      strcpy(cbuf3, pcls -> roomcode); //cbuf3 is old information, cbuf2 is new information
      strcpy(pcls -> roomcode, cbuf2);
      break;
    case 2: //Modify teacher occupancy
      strcpy(cbuf3, pcls -> tname);
      strcpy(pcls -> tname, cbuf2);
      break;
    case 3: //Modify time occupancy
      //Delete the original time period occupied by the target classroom
      DelTimeNode(pcls -> wk_st, pcls -> wk_ed, pcls -> cou_st, pcls -> cou_ed, & (psch -> week[buf5 - 1].ochead));
      strcpy(cbuf2, pcls -> roomcode);
      strcpy(cbuf3, pcls -> tname);
      //Two buffer strings are used to modify the time occupancy in the corresponding classroom and teacher structure
      pcls -> cou_st = buf1;
      pcls -> cou_ed = buf2;
      memcpy(pcls -> wk_st, buf3, sizeof(buf3));
      memcpy(pcls -> wk_ed, buf4, sizeof(buf4)); //Change the class time in the target class
      AddTimeNode(pcls -> wk_st, pcls -> wk_ed, pcls -> cou_st, pcls -> cou_ed, & (psch -> week[buf5 - 1].ochead));
    }
  }
  flag = FALSE;
  switch (choice) {
  case 1: //Modify classroom occupancy
    for (j = 0; j < rolist -> length; j++)
      if (!strcmp(rolist -> elem[j].code, cbuf3)) break;
    //Delete the occupancy of the original classroom by the target classroom
    if (j != rolist -> length)
      DelTimeNode(buf8, buf9, buf6, buf7, & (rolist -> elem[j].ochead[buf5 - 1]));
    //Add the occupancy of the target classroom to the new classroom
    AddTimeNode(buf8, buf9, buf6, buf7, & (rolist -> elem[l].ochead[buf5 - 1]));
    break;
  case 2: //Modify teacher occupancy
    for (j = 0; j < telist -> length; j++)
      if (!strcmp(telist -> elem[j].tname, cbuf3)) break;
    //Delete the occupancy of the original teacher by the target classroom
    if (j != telist -> length)
      DelTimeNode(buf8, buf9, buf6, buf7, & (telist -> elem[j].ochead[buf5 - 1]));
    //Add the occupancy of the target class to the new teacher
    AddTimeNode(buf8, buf9, buf6, buf7, & (telist -> elem[l].ochead[buf5 - 1]));
    break;
  case 3:
    for (l = 0; l < rolist -> length; l++)
      if (!strcmp(rolist -> elem[l].code, cbuf2)) break;
    //Delete the original occupied time of the target classroom
    if (l != rolist -> length)
      DelTimeNode(buf8, buf9, buf6, buf7, & (rolist -> elem[l].ochead[buf5 - 1]));
    //Add the new occupancy time for the target classroom
    AddTimeNode(buf3, buf4, buf1, buf2, & (rolist -> elem[l].ochead[buf5 - 1]));
    for (l = 0; l < telist -> length; l++)
      if (!strcmp(telist -> elem[l].tname, cbuf3)) break;
    //Delete the original occupied time of the target teacher
    if (l != telist -> length)
      DelTimeNode(buf8, buf9, buf6, buf7, & (telist -> elem[l].ochead[buf5 - 1]));
    //Add the new occupancy time for the target teacher
    AddTimeNode(buf3, buf4, buf1, buf2, & (telist -> elem[l].ochead[buf5 - 1]));
    break;
  }
  printf("modified successfully");
  getchar();
  ClearScreen();
  return TRUE;
}

BOOL SeekClass(void) { //pass
  fflush(stdin);
  char cbuf3[20];
  unsigned short buf8;
  BOOL glag;
  count = 0;
  do {
    if (count++) printf("The option is out of bounds, re-enter");
    printf("How to search?\n1--teacher\t2--class\t\t3--classroom+course+time\t4--time\t\t5--return to main menu\n");
    scanf("%hu", & hubuffer);
    getchar();
    if (hubuffer == 5) {
      ClearScreen();
      return TRUE;
    }
  } while (hubuffer < 1 || hubuff > 5);
  flag = FALSE;
  switch (hubuffer) {
  case 1:
    printf("Enter the teacher:");
    scanf("%s", cbuffer);
    getchar();
    for (i = 0; i < 3; i++) { //three grades
      for (psch = grade[i].schead; psch != NULL; psch = psch -> next)
        for (j = 0; j < 6; j++) //six days a week with classes
          for (pcls = psch -> week[j].classhead; pcls != NULL; pcls = pcls -> next)
            // find the corresponding course
            if (!strcmp(pcls -> tname, cbuffer)) {
              printf("Course: %s\tClass: %s\tClass Room: %s\nClass Time:", pcls -> coursename, psch -> classname, pcls -> roomcode);
              k = 0;
              flag = TRUE; //flag mark output valid information
              while (pcls -> wk_st[k] && k < 5) {
                if (k) putchar(',');
                printf("%hu-%hu", pcls -> wk_st[k], pcls -> wk_ed[k]);
                k++;
              }
              printf("week, week %hu, %hu-%hu section\n", j + 1, pcls -> cou_st, pcls -> cou_ed);
            }
    }
    if (flag == FALSE) printf("No relevant class found");
    break;
  case 2:
    printf("Enter class number:");
    psch = ClassInput(cbuf3);
    if (psch == NULL) {
      printf("The class was not found");
      getchar();
      ClearScreen();
      return TRUE;
    }
    for (i = 0; i < 6; i++)
      for (pcls = psch -> week[i].classhead; pcls != NULL; pcls = pcls -> next) {
        printf("Course: %s\tTeacher: %s\tClass room: %s\nClass time:", pcls -> coursename, pcls -> tname, pcls -> roomcode);
        j = 0;
        flag = TRUE; //flag mark output valid information
        while (pcls -> wk_st[j] && j < 5) {
          if (j) putchar(',');
          printf("%hu-%hu", pcls -> wk_st[j], pcls -> wk_ed[j]);
          j++;
        }
        printf("week, week %hu, %hu-%hu section\n", i + 1, pcls -> cou_st, pcls -> cou_ed);
      }
    if (flag == FALSE) printf("No relevant class found");
    break;
  case 3:
    printf("Enter the name of the class to be checked:");
    scanf("%s", cbuffer);
    getchar();
    printf("Enter classroom:");
    scanf("%s", cbuf1);
    getchar();
    printf("Enter class week:");
    scanf("%hu", & buf5);
    getchar();
    memset(buf3, 0, sizeof(buf3));
    memset(buf4, 0, sizeof(buf4));
    TimeInput( & buf1, & buf2, buf3, buf4);
    for (i = 0; i < 3; i++) {
      for (psch = grade[i].schead; psch != NULL; psch = psch -> next)
        for (pcls = psch -> week[buf5 - 1].classhead; pcls != NULL; pcls = pcls -> next) {
          hubuffer = 0; //Check the corresponding relationship between class time
          hubuffer = (pcls -> cou_st - buf1) + (pcls -> cou_ed - buf2);
          count = 0;
          while ((pcls -> wk_st[count] || buf3[count] || pcls -> wk_ed[count] || buf4[count]) && count < 5) { //The array element of class week number is valid
            hubuffer = (pcls -> wk_st[count] - buf3[count]) + (pcls -> wk_ed[count] - buf4[count]);
            count++;
          }
          //All information has been checked and completely overlapped
          if (!strcmp(pcls -> coursename, cbuffer) && !strcmp(pcls -> roomcode, cbuf1) && !hubuffer) {
            printf("Class: %s\tTeacher: %s\n", psch -> classname, pcls -> tname);
            flag = TRUE; //flag mark output valid information
          }
        }
    }
    if (flag == FALSE) printf("No relevant class found");
    break;
  case 4:
    printf("Enter the starting section number:");
    scanf("%hu", & buf1);
    getchar();
    printf("Enter the end section number:");
    scanf("%hu", & buf2);
    getchar();
    printf("Enter target week:");
    scanf("%hu", & buf8);
    getchar();
    printf("Enter the starting week number:");
    scanf("%hu", & buf5);
    getchar();
    printf("Enter the end week number:");
    scanf("%hu", & buf6);
    getchar();
    flag = FALSE;
    for (i = 0; i < 3; i++) {
      for (psch = grade[i].schead; psch != NULL; psch = psch -> next)
        for (pcls = psch -> week[buf8 - 1].classhead; pcls != NULL; pcls = pcls -> next) {
          glag = FALSE;
          hubuffer = count = 0; //Check the corresponding relationship between class time
          if (pcls -> cou_st >= buf1 && pcls -> cou_ed <= buf2) hubuffer = 1;
          //The target class has classes within the specified time range
          while (pcls -> wk_st[count] || pcls -> wk_ed[count]) {
            if (buf5 >= pcls -> wk_st[count] && buf6 <= pcls -> wk_ed[count]) {
              glag = TRUE;
              break; //flag tag output valid information
            }
            count++;
          }
          //The time parameter meets the requirements
          if (hubuffer && glag) {
            printf("Course: %s\tClassroom: %s\tClass: %s\tTeacher:%s\n", pcls -> coursename, pcls -> roomcode, psch -> classname, pcls -> tname);
            flag = TRUE; //flag mark output valid information
          }
        }
    }
    if (flag == FALSE) printf("No relevant class found");
  case 5:
    break;
  }
  getchar();
  ClearScreen();
  return TRUE;
}

BOOL RoomUsage(void) {
  fflush(stdin);
  unsigned short buf7, buf8;
  BOOL glag;
  count = 0;
  do {
    if (count) printf("Please re-enter\n");
    printf("Choose the analysis method:\n1--search for free classrooms\t2--classroom utilization\t3--rationality of classroom use\t4--return to the main menu\n");
    scanf("%hu", & choice);
    getchar();
    count++;
    if (choice == 4) {
      ClearScreen();
      return TRUE;
    }
  } while (choice < 1 || choice > 4);
  switch (choice) {
  case 1:
    printf("Enter target week:");
    scanf("%hu", & hubuffer);
    getchar();
    printf("Enter target week:");
    scanf("%hu", & buf5);
    getchar();
    printf("Enter the start section:");
    scanf("%hu", & buf1);
    getchar();
    printf("Enter termination section:");
    scanf("%hu", & buf2);
    getchar();
    glag = FALSE;
    count = 0;
    for (i = 0; i < rolist -> length; i++) {
      flag = FALSE;
      for (pocc = rolist -> elem[i].ochead[buf5 - 1].head; pocc != NULL; pocc = pocc -> next) {
        if (pocc -> week == hubuffer && pocc -> time >= buf1 && pocc -> time <= buf2) {
          flag = TRUE;
          break;
        }
      }
      if (flag == FALSE) {
        printf("%s\t", rolist -> elem[i].code);
        glag = TRUE;
        count++;
        if (!(count % 5)) putchar('\n');
      }
    }
    if (glag == FALSE) printf("There are no classrooms that meet the requirements within this time period");
    break;
  case 2:
    printf("Enter the target week interval (only one week, the beginning and end weeks are the same)\nStarting week:");
    scanf("%hu", & buf1);
    getchar();
    printf("End week:");
    scanf("%hu", & buf2);
    getchar();
    printf("Enter the target date interval (only one day, the beginning and end are the same)\nStart date:");
    scanf("%hu", & buf7);
    getchar();
    printf("End date:");
    scanf("%hu", & buf8);
    getchar();
    printf("Enter the target section interval (the beginning and end sections are the same only for one week)\nStart section:");
    scanf("%hu", & buf5);
    getchar();
    printf("end section:");
    scanf("%hu", & buf6);
    getchar();
    count = 0;
    for (i = 0; i < rolist -> length; i++)
      for (j = buf7 - 1; j < buf8; j++)
        for (pocc = rolist -> elem[i].ochead[j].head; pocc != NULL; pocc = pocc -> next)
          if (pocc -> week >= buf1 && pocc -> week <= buf2 && pocc -> time >= buf5 && pocc -> time <= buf6) count++;
    printf("Classroom utilization rate in the target time period is %f", count * 1.0 / (rolist -> length * (buf2 - buf1 + 1) * (buf8 - buf7 + 1) * (buf6 - buf5 + 1)));
    break;
  case 3:
    buf6 = 0;
    printf("Enter classroom:");
    scanf("%s", cbuf1);
    getchar();
    for (i = 0; i < rolist -> length; i++)
      if (!strcmp(cbuf1, rolist -> elem[i].code)) break;
    if (i == rolist -> length) {
      printf("No target classroom found");
      getchar();
      ClearScreen();
      return TRUE;
    }
    printf("The maximum capacity of the current classroom is %hu\n", rolist -> elem[i].rsize);
    printf("Enter the name of the class to be checked:");
    scanf("%s", cbuffer);
    getchar();
    printf("Enter class week:");
    scanf("%hu", & buf5);
    getchar();
    memset(buf3, 0, sizeof(buf3));
    memset(buf4, 0, sizeof(buf4));
    TimeInput( & buf1, & buf2, buf3, buf4);
    flag = FALSE;
    for (j = 0; j < 3; j++) {
      for (psch = grade[j].schead; psch != NULL; psch = psch -> next)
        for (pcls = psch -> week[buf5 - 1].classhead; pcls != NULL; pcls = pcls -> next) {
          hubuffer = 0; //Check the corresponding relationship between class time
          hubuffer = (pcls -> cou_st - buf1) + (pcls -> cou_ed - buf2);
          count = 0;
          while ((pcls -> wk_st[count] || buf3[count] || pcls -> wk_ed[count] || buf4[count]) && count < 5) {
            hubuffer = (pcls -> wk_st[count] - buf3[count]) + (pcls -> wk_ed[count] - buf4[count]);
            count++;
          }
          if (!strcmp(pcls -> coursename, cbuffer) && !strcmp(pcls -> roomcode, cbuf1) && !hubuffer) {
            printf("Class: %s\tnumber of people%hu\n", psch -> classname, psch -> clsize);
            buf6 += psch -> clsize;
            flag = TRUE;
          }
        }
    }
    if (flag == FALSE) {
      printf("No related class found");
      break;
    } else {
      if (buf6 > rolist -> elem[i].rsize)
        printf("The classroom is overcrowded, a larger classroom needs to be replaced\n");
      else if (buf6 / rolist -> elem[i].rsize <= 1 && buf6 * 1.0 / rolist -> elem[i].rsize >= 0.6)
        printf("There is a shortage of seats, a larger classroom needs to be replaced\n");
      else if (buf6 * 1.0 / rolist -> elem[i].rsize <= 0.5)
        printf("There are too many vacant seats, please consider merging classrooms or moving to smaller classrooms\n");
      else
        printf("The classroom is suitable\n");
    }
  case 4:
    break;
  }
  getchar();
  ClearScreen();
  return TRUE;
}

BOOL ClassAnalysis(void) {
  fflush(stdin);
  unsigned short occupied[22][6];
  char cbuf3[20];
  printf("Enter the class to be analyzed:");
  psch = ClassInput(cbuf3);
  if (psch == NULL) {
    printf("The class was not found");
    getchar();
    ClearScreen();
    return TRUE;
  }
  count = 0;
  do {
    if (count) printf("Please re-enter\n");
    printf("Choose the analysis method:\n1--the rationality of the courses arranged in the morning and evening\t2--the rationality of the courses arranged by the day\n3--the rationality of the courses\t4- - Return to main menu\n");
    scanf("%hu", & choice);
    getchar();
    count++;
    if (choice == 4) {
      ClearScreen();
      return TRUE;
    }
  } while (choice < 1 || choice > 4);
  switch (choice) {
  case 1: //The rationality of the course arrangement in each day
    printf("Enter the number of valid weeks in this semester:");
    scanf("%hu", & hubuffer);
    getchar();
    for (j = 1; j < hubuffer; j++) {
      flag = TRUE;
      buf1 = buf2 = buf5 = 0;
      printf("Week %d", (int) j);
      for (i = 0; i < 6; i++)
        for (pocc = psch -> week[i].ochead.head; pocc != NULL; pocc = pocc -> next)
          if (pocc -> week == j) {
            if (pocc -> time >= 1 && pocc -> time <= 4) buf1++;
            else if (pocc -> time >= 5 && pocc -> time <= 8) buf2++;
            else if (pocc -> time >= 9 && pocc -> time <= 12) buf5++;
          }
      if (buf5 > buf1 / 2 || buf5 > buf2 / 2) {
        printf("The proportion of evening classes is too large\n");
        flag = FALSE;
      }
      if (flag == TRUE) printf("The courses are arranged more reasonably in each day\n");
    }
    break;
  case 2:
    printf("Enter the number of valid weeks in this semester:");
    scanf("%hu", & hubuffer);
    getchar();
    memset(occupy, 0, sizeof(occupy));
    for (j = 0; j < 6; j++)
      for (pocc = psch -> week[j].ochead.head; pocc != NULL; pocc = pocc -> next)
        occupy[pocc -> week - 1][j]++;
    for (i = 0; i < hubuffer; i++) {
      flag = TRUE;
      printf("Week %d", (int) i + 1);
      for (j = 0; j < 5; j++)
        for (k = j; k < 5; k++)
          if (occupy[i][j] - occupy[i][k] >= 6) {
            printf("The distribution of courses in week %d and week %d is too different\n", j + 1, k + 1);
            flag = FALSE;
          }
      if (flag == TRUE) printf("Course distribution is more reasonable\n");
    }
    buf1 = buf2 = buf5 = buf6 = 0;
    for (i = 1; i < 6; i++) {
      buf1 += occupy[0][i];
      buf2 += occupy[1][i];
      buf5 += occupy[hubuffer - 2][i];
      buf6 += occupy[hubuffer - 1][i];
    }
    if (buf1 >= 20) printf("There are too many classes in the first week\n");
    if (buf2 >= 22) printf("There are too many classes in the second week\n");
    if (buf5 >= 10) printf("There are too many classes in week %hu\n", hubuffer - 1);
    if (buf6 >= 4) printf("There are too many classes in week %hu\n", hubuffer);
    break;
  case 3:
    for (i = 0; i < psch -> pr_num; i++) {
      flag = FALSE;
      for (j = 0; j < cougraph -> length; j++)
        if (!strcmp(psch -> progress[i], cougraph -> coulist[j].name)) break;
      if (j == cougraph -> length) {
        printf("Course %s has not been entered, directly analyze the next course\n", cougraph -> coulist[j].name);
        continue;
      }
      for (pcarc = cougraph -> coulist[j].archhead; pcarc != NULL; pcarc = pcarc -> nextarc) {
        for (k = 0; k < psch -> do_num; k++)
          if (!strcmp(psch -> done[k], pcarc -> prename)) break;
        if (k == psch -> do_num) {
          printf("%s has not been completed, %s should not be completed this semester\n", pcarc -> prename, psch -> progress[i]);
          flag = TRUE;
        }
      }
      if (flag == FALSE) printf("Course %s can be taken in this semester\n", cougraph -> coulist[j].name);
      putchar('\n');
    }
  case 4:
    break;
  }
  //The rationality of the course arrangement within a week
  getchar();
  ClearScreen();
  return TRUE;
}

BOOL Teacher Analysis(void) {
  fflush(stdin);
  unsigned short occupied[22];
  printf("Enter the teacher to be analyzed:");
  scanf("%s", cbuffer);
  getchar();
  for (i = 0; i < telist -> length; i++)
    if (!strcmp(telist -> elem[i].tname, cbuffer)) break;
  if (i == telist -> length) {
    printf("Target teacher not found");
    getchar();
    ClearScreen();
    return TRUE;
  }
  count = 0;
  do {
    if (count) printf("The option is out of bounds, re-enter");
    printf("Choose the analysis method:\n1--Teaching tasks are assigned by day\t2--Teaching task allocation\t3--Floor arrangement is reasonable\t4Return to the main menu\n");
    scanf("%hu", & choice);
    getchar();
    count++;
    if (choice == 4) {
      ClearScreen();
      return TRUE;
    }
  } while (choice < 1 || choice > 4);
  switch (choice) {
  case 1:
    printf("Enter the number of valid weeks in this semester:");
    scanf("%hu", & hubuffer);
    getchar();
    for (j = 1; j <= hubuffer; j++) {
      flag = TRUE;
      printf("Week %d", (int) j);
      buf1 = buf2 = buf5 = 0;
      for (k = 0; k < 6; k++)
        for (pocc = telist -> elem[i].ochead[k].head; pocc != NULL; pocc = pocc -> next)
          if (pocc -> week == j) {
            if (pocc -> time >= 1 && pocc -> time <= 4) buf1++;
            else if (pocc -> time >= 5 && pocc -> time <= 8) buf2++;
            else if (pocc -> time >= 9 && pocc -> time <= 12) buf5++;
          }
      if (buf5 > buf1 / 2 || buf5 > buf2 / 2) { //judgment based on class
        printf("The proportion of evening classes is too large\n");
        flag = FALSE;
      }
      if (flag == TRUE) printf("The courses are arranged more reasonably in each day\n");
    }
    break;
  case 2:
    printf("Enter the number of valid weeks in this semester:");
    scanf("%hu", & hubuffer);
    getchar();
    memset(occupy, 0, sizeof(occupy));
    for (j = 0; j < 6; j++)
      for (pocc = psch -> week[j].ochead.head; pocc != NULL; pocc = pocc -> next)
        occupy[pocc -> week - 1]++;
    flag = FALSE;
    for (j = 0; j < hubuffer; j++)
      //Retirement age: male-55 female-60
      if ((telist -> elem[i].gender == 0 && telist -> elem[i].age < 50) || (telist -> elem[i].gender == 1 && telist -> elem[i].age < 55)) {
        if (occupy[j] > 14) {
          printf("There are too many classes in week %hu\n", j + 1);
          flag = TRUE;
        }
      }
    else {
      if (occupy[j] > 10) {
        printf("There are too many classes in week %hu\n", j + 1);
        flag = TRUE;
      }
    }
    if (flag == FALSE) printf("The teacher's teaching assignment is more appropriate");
    break;
  case 3:
    if ((telist -> elem[i].gender == 0 && telist -> elem[i].age < 55) || (telist -> elem[i].gender == 1 && telist -> elem[i].age < 60)) {
      printf("The teacher has no special floor requirements\n");
      break;
    } else {
      flag = FALSE;
      for (j = 0; j < 3; j++) {
        for (psch = grade[j].schead; psch != NULL; psch = psch -> next) {
          for (k = 0; k < 6; k++) {
            for (pcls = psch -> week[k].classhead; pcls != NULL; pcls = pcls -> next) {
              if (!strstr(pcls -> tname, telist -> elem[i].tname)) continue;
              if (strstr(pcls -> roomcode, "4") || strstr(pcls -> roomcode, "5")) {
                printf("The floor of the classroom %s where %s is located is too high\n", pcls -> coursename, pcls -> roomcode);
                flag = TRUE;
              }
            }
          }
        }
      }
      if (flag == FALSE) printf("The teacher's floor is more reasonable\n");
    }
  case 4:
    break;
  }
  getchar();
  ClearScreen();
  return TRUE;
}

/*
BOOL ScheduleIn(void){
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
printf("Failed to open 15_timetable_info.txt");
}
while (1) {
getline(infile, sbuf1);
if(sbuf1.find("class")!=sbuf1.npos){
else if(sbuf1. find(""))
i=0;
while (1) {
getline(infile, sbuf1);
strcpy(cbuffer, sbuf1.c_str());
if(strstr(cbuffer,"]")) break;
strcpy(cls_stack[i++],sbuf1.c_str());
}
while(i){
getline(infile, sbuf1);
if(sbuf1.find("course")!=sbuf1.npos){
getline(infile, sbuf1);
strcpy(cbuffer, sbuf1.c_str());
if(strstr(cbuffer,"]")) break;
//remove punctuation in the course
for(j=0;j<cougraph->length;j++)
if(!strcmp(cougraph->coulist[j].name,cbuffer)) break;
if(j==cougraph->length){
strcpy(cougraph->coulist[cougraph->length].name, cbuffer);
cougraph->coulist[cougraph->length++].archhead = NULL;
printf("Prerequisite courses to be entered...");
}
}
getline(infile, sbuf1);
if(sbuf1.find("schedule")!=sbuf1.npos){
for(psch=grade[0].schead;psch!=NULL;psch=psch->next)
if(!strcmp(psch->classname, cstack[--i])) break;
if(psch==NULL){ //Insert a new class in the class schedule structure
psch=(Schedule *)malloc(sizeof(Schedule));
strcpy(psch->classname, cstack[i]);
psch->next=grade[0].schead; grade[0].schead=psch;
for(j=0;j<6;j++)
memset(psch->week[j].occupy,0,sizeof(psch->week[j].occupy));
grade[0].gr_num++;
getline(infile, sbuf1);
if(sbuf1.find("Monday"!=sbuf1.npos)){
getline(infile, sbuf1);
if(sbuf1.find("["!=sbuf1.npos)
pcls=(Class*)malloc(sizeof(Class));
memset(pcls->wk_ed,0,sizeof(pcls->wk_ed));
memset(pcls->wk_st,0,sizeof(pcls->wk_st));
while (1) {
getline(infile, sbuf1);
if(sbuf1.find("name")!=sbuf1.npos){
sbuf1.replace(sbuf1.find_last_of("\""),2,send);
sbuf2=sbuf1.substr(21);
strcpy(pcls->coursename, sbuf2.c_str());
}
else if(sbuf1.find("time")!=sbuf1.npos){
strcpy(cbuffer,sbuf1.c_str()); j=0;
for(k=0;cbuffer[k];i++)
if(cbuffer[k]>='0' && cbuffer[k]<='9'){
buf[j]=buf[j]*10+cbuffer[k]-'0';
if(cbuffer[k+1]<='0' || cbuffer[k+1]>='9') j++;
}
pcls->cou_st=buf[0]; pcls->cou_ed=buf[1];
}
else if(sbuf1.find("date")!=sbuf1.npos){
strcpy(cbuffer, sbuf1.c_str());
count=0;
pch = strtok(cbuffer,",");
while (pch != NULL)
{
j=0;
buf[0]=buf[1]=0;
for(k=0;pch[k];k++)
if(pch[k]>='0' && pch[k]<='9'){
buf[k]=buf[k]*10+pch[k]-'0';
if(pch[k+1]<='0' || pch[k+1]>='9') j++;
}
if(buf[0]>buf[1]) buf[1]=buf[0];
pcls->wk_st[count]=buf[0]; pcls->wk_ed[count]=buf[1]; count++;
pch = strtok(NULL, ",");
}
}
else if(sbuf1.find("classroom")!=sbuf1.npos){
sbuf1 = sbuf1.substr(26);
sbuf1.replace(sbuf1.find_last_of("\""),2,send);
strcpy(pcls->roomcode, sbuf1.c_str());
if(sbuf1.find("£¬")!=sbuf1.npos){
strcpy(cbuffer, sbuf1.c_str());
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
for(j=1;j<=k;j++) sstack[j]=sstack[0]+sstack[j];
for(j=0;j<=k;j++) sstack[j]=sstack[j+1];
for(j=0;j<=k;j++){
if(rolist->length >= rolist->listsize){
Room *newbase = (Room *)realloc(rolist->elem, (rolist->listsize+LISTINCREMENT)* sizeof(Room));
if(!newbase){
printf("Memory allocation failed");
getchar(); return FALSE;
}
rolist->elem = newbase;
rolist->listsize += LISTINCREMENT;
}
for(l=0;l<rolist->length;l ++)
if(!sstack[j].compare(rolist->elem[l].code)) break;
if(l==rolist->length){
strcpy(rolist->elem[rolist->length].code, sstack[j].c_str());
printf("Please enter the number of classroom seats in %s:", sstack[j].c_str());
scanf("%hu",&rolist->elem[rolist->length].seat); getchar();
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
infile. close();
}
*/

BOOL FileOut(void) {
  fflush(stdin);
  FILE * out;
  time_t lt;
  char * filename, * pc;
  const char * appe = ".txt";
  lt = time(NULL);
  filename = ctime( & lt);
  filename[strlen(filename) - 1] = '\0';
  strcat(filename, appe);
  pc = & filename[0];
  while ( * pc) {
    if (( * pc == ':') || ( * pc == ' ')) {
      * pc = '_';
    }
    pc++;
  }
  if ((out = fopen(filename, "w")) == NULL) {
    printf("Failed to open!\n");
    getchar();
    ClearScreen();
    return TRUE;
  }
  //teacher list
  fprintf(out, "%hu ", telist -> length);
  for (i = 0; i < telist -> length; i++) {
    fprintf(out, "%s ", telist -> elem[i].tname);
    fprintf(out, "%hu ", telist -> elem[i].gender);
    fprintf(out, "%hu ", telist -> elem[i].age);
    for (j = 0; j < 6; j++) {
      fprintf(out, "%hu ", telist -> elem[i].ochead[j].len);
      for (pocc = telist -> elem[i].ochead[j].head; pocc != NULL; pocc = pocc -> next) {
        fprintf(out, "%hu ", pocc -> time);
        fprintf(out, "%hu ", pocc -> week);
      }
    }
  }
  //classroom list
  fprintf(out, "%hu ", rolist -> length);
  for (i = 0; i < rolist -> length; i++) {
    fprintf(out, "%s ", rolist -> elem[i].code);
    fprintf(out, "%hu ", rolist -> elem[i].rsize);
    for (j = 0; j < 6; j++) {
      fprintf(out, "%hu ", rolist -> elem[i].ochead[j].len);
      for (pocc = rolist -> elem[i].ochead[j].head; pocc != NULL; pocc = pocc -> next) {
        fprintf(out, "%hu ", pocc -> time);
        fprintf(out, "%hu ", pocc -> week);
      }
    }
  }
  // course list
  fprintf(out, "%hu ", cougraph -> length);
  for (i = 0; i < cougraph -> length; i++) {
    fprintf(out, "%s ", cougraph -> coulist[i].name);
    fprintf(out, "%hu ", cougraph -> coulist[i].arcnum);
    for (pcarc = cougraph -> coulist[i].archhead; pcarc != NULL; pcarc = pcarc -> nextarc)
      fprintf(out, "%s ", pcarc -> prename);
  }
  //class + schedule list
  for (i = 0; i < 3; i++) {
    fprintf(out, "%hu ", grade[i].gr_num);
    for (psch = grade[i].schead; psch != NULL; psch = psch -> next) {
      fprintf(out, "%s ", psch -> classname);
      fprintf(out, "%hu ", psch -> clsize);
      fprintf(out, "%hu ", psch -> do_num);
      fprintf(out, "%hu ", psch -> pr_num);
      if (psch -> do_num)
        for (j = 0; j < psch -> do_num; j++)
          fprintf(out, "%s ", psch -> done[j]);
      if (psch -> pr_num)
        for (j = 0; j < psch -> pr_num; j++)
          fprintf(out, "%s ", psch -> progress[j]);
      for (j = 0; j < 6; j++) {
        fprintf(out, "%hu ", psch -> week[j].cl_num);
        for (pcls = psch -> week[j].classhead; pcls != NULL; pcls = pcls -> next) {
          fprintf(out, "%s ", pcls -> coursename);
          fprintf(out, "%s ", pcls -> roomcode);
          fprintf(out, "%s ", pcls -> tname);
          fprintf(out, "%hu ", pcls -> cou_st);
          fprintf(out, "%hu ", pcls -> cou_ed);
          for (k = 0; k < 5; k++) {
            fprintf(out, "%hu ", pcls -> wk_st[k]);
            fprintf(out, "%hu ", pcls -> wk_ed[k]);
          }
        }
        fprintf(out, "%hu ", psch -> week[j].ochead.len);
        for (pocc = psch -> week[j].ochead.head; pocc != NULL; pocc = pocc -> next) {
          fprintf(out, "%hu ", pocc -> time);
          fprintf(out, "%hu ", pocc -> week);
        }
      }
    }
  }
  fclose(out);
  printf("Backup succeeded! Press any key to continue...\n");
  getchar();
  ClearScreen();
  return TRUE;
}

BOOL FileIn(void) {
  fflush(stdin);
  FILE * in;
  printf("The imported data will overwrite the current data of the system! Continue——Y to return to the main menu——any other key\n");
  fflush(stdin);
  j = getchar();
  getchar();
  if (j != 'y' && j != 'Y') {
    printf("Data import cancel");
    getchar();
    ClearScreen();
    return TRUE;
  }
  char input[80], * filename;
  printf("Please enter the file name of the data you want to restore:");
  fflush(stdin);
  scanf("%s", input);
  getchar();
  filename = input;
  //int handle, find;
  if ((in = fopen(filename, "r+")) == NULL) {
    printf("The backup file you input does not exist, the system will keep the current data");
    getchar();
    ClearScreen();
    return TRUE;
  }
  ClearMemory();
  Initial();
  fscanf(in, "%hu", & (telist -> length));
  if (telist -> length >= telist -> listsize)
    buf1 = (telist -> length - telist -> listsize) / LISTINCREMENT + 1;
  while (telist -> length >= telist -> listsize) {
    Teacher * te_newbase = (Teacher * ) realloc(telist -> elem, (telist -> listsize + buf1 * LISTINCREMENT) * sizeof(Teacher));
    if (!te_newbase) {
      printf("Memory allocation failed");
      return TRUE;
    }
    telist -> elem = te_newbase;
    telist -> listsize += buf1 * LISTINCREMENT;
  }
  for (i = 0; i < telist -> length; i++) {
    fscanf(in, "%s", telist -> elem[i].tname);
    fscanf(in, "%hu ", & (telist -> elem[i].gender));
    fscanf(in, "%hu ", & (telist -> elem[i].age));
    for (j = 0; j < 6; j++) {
      fscanf(in, "%hu", & (telist -> elem[i].ochead[j].len));
      telist -> elem[i].ochead[j].head = NULL;
      for (k = 0; k < telist -> elem[i].ochead[j].len; k++) {
        pocc = (Occupy * ) malloc(sizeof(Occupy));
        fscanf(in, "%hu", & (pocc -> time));
        fscanf(in, "%hu", & (pocc -> week));
        pocc -> next = telist -> elem[i].ochead[j].head;
        telist -> elem[i].ochead[j].head = pocc;
      }
    }
  }

  fscanf(in, "%hu", & (rolist -> length));
  if (rolist -> length >= rolist -> listsize)
    buf1 = (rolist -> length - rolist -> listsize) / LISTINCREMENT + 1;
  while (rolist -> length >= rolist -> listsize) {
    Room * ro_newbase = (Room * ) realloc(rolist -> elem, (rolist -> listsize + buf1 * LISTINCREMENT) * sizeof(Room));
    if (!ro_newbase) {
      printf("Memory allocation failed");
      return TRUE;
    }
    rolist -> elem = ro_newbase;
    rolist -> listsize += buf1 * LISTINCREMENT;
  }
  for (i = 0; i < rolist -> length; i++) {
    fscanf(in, "%s", rolist -> elem[i].code);
    fscanf(in, "%hu ", & (rolist -> elem[i].rsize));
    for (j = 0; j < 6; j++) {
      fscanf(in, "%hu", & (rolist -> elem[i].ochead[j].len));
      rolist -> elem[i].ochead[j].head = NULL;
      for (k = 0; k < rolist -> elem[i].ochead[j].len; k++) {
        pocc = (Occupy * ) malloc(sizeof(Occupy));
        fscanf(in, "%hu", & (pocc -> time));
        fscanf(in, "%hu", & (pocc -> week));
        pocc -> next = rolist -> elem[i].ochead[j].head; //The order of node insertion is reversed?
        rolist -> elem[i].ochead[j].head = pocc;
      }
    }
  }

  fscanf(in, "%hu", & (cougraph -> length));
  if (cougraph -> length >= cougraph -> listsize)
    buf1 = (cougraph -> length - cougraph -> listsize) / LISTINCREMENT + 1;
  while (cougraph -> length >= cougraph -> listsize) {
    CouHead * cou_newbase = (CouHead * ) realloc(cougraph -> coulist, (cougraph -> listsize + buf1 * LISTINCREMENT) * sizeof(CouHead));
    if (!cou_newbase) {
      printf("Memory allocation failed");
      return TRUE;
    }
    cougraph -> coulist = cou_newbase;
    cougraph -> listsize += buf1 * LISTINCREMENT;
  }
  for (i = 0; i < cougraph -> length; i++) {
    fscanf(in, "%s", cougraph -> coulist[i].name);
    fscanf(in, "%hu", & (cougraph -> coulist[i].arcnum));
    cougraph -> coulist[i].archhead = NULL;
    for (j = 0; j < cougraph -> coulist[i].arcnum; j++) {
      pcarc = (CouArc * ) malloc(sizeof(CouArc));
      pcarc -> prename = (char * ) malloc(sizeof(cbuffer));
      fscanf(in, "%s", pcarc -> prename);
      pcarc -> nextarc = cougraph -> coulist[i].archhead;
      cougraph -> coulist[i].archhead = pcarc;
    }
  }

  for (i = 0; i < 3; i++) {
    fscanf(in, "%hu", & (grade[i].gr_num));
    grade[i].schead = NULL;
    for (j = 0; j < grade[i].gr_num; j++) {
      psch = (Schedule * ) malloc(sizeof(Schedule));
      fscanf(in, "%s", psch -> classname);
      fscanf(in, "%hu ", & (psch -> clsize));
      fscanf(in, "%hu", & (psch -> do_num));
      fscanf(in, "%hu", & (psch -> pr_num));
      if (psch -> do_num)
        for (k = 0; k < psch -> do_num; k++)
          fscanf(in, "%s", psch -> done[k]);
      if (psch -> pr_num)
        for (k = 0; k < psch -> pr_num; k++)
          fscanf(in, "%s", psch -> progress[k]);
      for (k = 0; k < 6; k++) {
        fscanf(in, "%hu", & (psch -> week[k].cl_num));
        psch -> week[k].classhead = NULL;
        for (l = 0; l < psch -> week[k].cl_num; l++) {
          pcls = (Class * ) malloc(sizeof(Class));
          fscanf(in, "%s", pcls -> coursename);
          fscanf(in, "%s", pcls -> roomcode);
          fscanf(in, "%s", pcls -> tname);
          fscanf(in, "%hu", & (pcls -> cou_st));
          fscanf(in, "%hu", & (pcls -> cou_ed));
          for (m = 0; m < 5; m++) {
            fscanf(in, "%hu", & (pcls -> wk_st[m]));
            fscanf(in, "%hu", & (pcls -> wk_ed[m]));
          }
          pcls -> next = NULL;
          if (psch -> week[k].classhead == NULL)
            psch -> week[k].classhead = pcls;
          else {
            for (qcls = psch -> week[k].classhead; qcls -> next != NULL; qcls = qcls -> next);
            qcls -> next = pcls;
          }
        }
        fscanf(in, "%hu", & (psch -> week[k].ochead.len));
        psch -> week[k].ochead.head = NULL;
        for (l = 0; l < psch -> week[k].ochead.len; l++) {
          pocc = (Occupy * ) malloc(sizeof(Occupy));
          fscanf(in, "%hu", & (pocc -> time));
          fscanf(in, "%hu", & (pocc -> week));
          pocc -> next = psch -> week[k].ochead.head;
          psch -> week[k].ochead.head = pocc;
        }
      }
      psch -> next = grade[i].schead;
      grade[i].schead = psch;
    }
  }
  fclose(in);
  printf("Data imported successfully, press any key to continue...\n");
  getchar();
  ClearScreen();
  return TRUE;
}

/**
 * Function name: MaintainTeacherlInfo
 * Function function: teacher information maintenance directory
 * Input parameters: None
 * Output parameters: None
 * Return value: BOOL type, always TRUE
 *
 * Call description:
 */
BOOL MaintainTeacherInfo(void) {
  BOOL bRet = TRUE;
  do {
    printf("What do you want to do?\n");
    printf("Enter press1\nmodify press2\ndelete press3\nreturn to main menu press4\n");
    fflush(stdin);
    k = getchar();
    getchar();
    if (k == '4') {
      ClearScreen();
      return TRUE;
    }
  } while (k < '1' || k > '4');
  if (k == '1') bRet = AddTeacher();
  else if (k == '2') bRet = AltTeacher();
  else if (k == '3') bRet = DelTeacher();
  return bRet;
}

/**
 * Function name: MaintainRoomInfo
 * Function function: classroom information maintenance directory
 * Input parameters: None
 * Output parameters: None
 * Return value: BOOL type, always TRUE
 *
 * Call description:
 */
BOOL MaintainRoomInfo(void) {
  BOOL bRet = TRUE;
  do {
    printf("What do you want to do?\n");
    printf("Enter press1\nmodify press2\ndelete press3\nreturn to main menu press4\n");
    fflush(stdin);
    k = getchar();
    getchar();
    if (k == '4') {
      ClearScreen();
      return TRUE;
    }
  } while (k < '1' || k > '4');
  if (k == '1') {
    bRet = AddRoom();
  } else if (k == '2') {
    bRet = AltRoom();
  } else if (k == '3') {
    bRet = DelRoom();
  }
  return bRet;
}

/**
 * Function name: MaintainGroupInfo
 * Function: class information maintenance directory
 * Input parameters: None
 * Output parameters: None
 * Return value: BOOL type, always TRUE
 *
 * Call description:
 */
BOOL MaintainGroupInfo(void) {
  BOOL bRet = TRUE;
  do {
    printf("What do you want to do?\n");
    printf("Enter press1\nmodify press2\ndelete press3\nreturn to main menu press4\n");
    fflush(stdin);
    k = getchar();
    getchar();
    if (k == '4') {
      ClearScreen();
      return TRUE;
    }
  } while (k < '1' || k > '4');
  if (k == '1') {
    bRet = AddGroup();
  } else if (k == '2') {
    bRet = AltGroup();
  } else if (k == '3') {
    bRet = DelGroup();
  }
  return bRet;
}

/**
 * Function name: MaintainCourseInfo
 * Function function: course information maintenance directory
 * Input parameters: None
 * Output parameters: None
 * Return value: BOOL type, always TRUE
 *
 * Call description:
 */
BOOL MaintainCourseInfo(void) {
  BOOL bRet = TRUE;
  do {
    printf("What do you want to do?\n");
    printf("Enter press1\nmodify press2\ndelete press3\nreturn to main menu press4\n");
    fflush(stdin);
    k = getchar();
    getchar();
    if (k == '4') {
      ClearScreen();
      return TRUE;
    }
  } while (k < '1' || k > '4');
  if (k == '1') {
    bRet = AddCourse();
  } else if (k == '2') {
    bRet = AltCourse();
  } else if (k == '3') {
    bRet = DelCourse();
  }
  return bRet;
}

/**
 * Function name: MaintainClassInfo
 * Function function: course information maintenance directory
 * Input parameters: None
 * Output parameters: None
 * Return value: BOOL type, always TRUE
 *
 * Call description:
 */
BOOL MaintainClassInfo(void) {
  BOOL bRet = TRUE;
  do {
    printf("What do you want to do?\n");
    printf("Enter press1\nmodify press2\ndelete press3\nreturn to main menu press4\n");
    fflush(stdin);
    k = getchar();
    getchar();
    if (k == '4') {
      ClearScreen();
      return TRUE;
    }
  } while (k < '1' || k > '4');
  if (k == '1') {
    bRet = AddClass();
  } else if (k == '2') {
    bRet = AltClass();
  } else if (k == '3') {
    bRet = DelClass();
  }
  return bRet;
}

BOOL Add(void) {
  fflush(stdin);
  for (i = 0; i < telist -> length; i++) {
    printf("Teacher's name: %s\nEnter gender (male--1\tfemale--0):", telist -> elem[i].tname);
    scanf("%hu", & telist -> elem[i].gender);
    getchar();
    printf("Enter age:");
    scanf("%hu", & telist -> elem[i].age);
    getchar();
  }
  for (i = 0; i < rolist -> length; i++) {
    printf("Classroom number: %s\nInput capacity:", rolist -> elem[i].code);
    scanf("%hu", & rolist -> elem[i].rsize);
    getchar();
  }
  for (i = 0; i < 3; i++) {
    for (psch = grade[i].schead; psch != NULL; psch = psch -> next) {
      printf("Class number: %s\nEnter the number of people:", psch -> classname);
      scanf("%hu", & psch -> clsize);
      getchar();
    }
  }
  return TRUE;
}

BOOL HelpTopic(void) {
  ShellExecuteA(NULL, "open", "NOTEPAD.EXE", "help.txt", NULL, SW_SHOWNORMAL);
  printf("Press any key to continue...\n");
  getch();
  ClearScreen();
  return TRUE;
}

BOOL AboutDorm(void) {
  char * plabel_name[] = {
    "System version: V1.0",
    "**************",
    "All Rights Reserved",
    "confirm"
  };
  ShowModule(plabel_name, 4);
  return TRUE;
}

BOOL DelTimeNode(unsigned short wk_st[], unsigned short wk_ed[], unsigned short cou_st, unsigned short cou_ed, Ochead * ochead) {
  count = 0;
  while (wk_st[count] && count < 5) { //traverse according to the number of weeks in each class
    for (p = wk_st[count]; p <= wk_ed[count]; p++) { //Traverse by class week
      for (n = cou_st; n <= cou_ed; n++) { //traverse according to the number of lessons
        for (pocc = ochead -> head; pocc != NULL; pocc = pocc -> next) {
          if (pocc -> week == p && pocc -> time == n) { //The class time matches
            if (pocc == ochead -> head) { //The time occupying node is the head node
              ochead -> head = pocc -> next;
              free(pocc);
              pocc = ochead -> head;
              if (ochead -> head == NULL) {
                ochead -> len = 0;
                break;
              }
            } else { //The time-occupied node is not the head node
              for (qocc = ochead -> head; qocc -> next != pocc; qocc = qoccc -> next);
              qocc -> next = pocc -> next;
              free(pocc);
              pocc = qocc;
            }
            ochead -> len--; //The number of time-occupied nodes decreases by itself
            break;
          }
        }
      }
    }
    count++;
  }
  return TRUE;
}

BOOL AddTimeNode(unsigned short wk_st[], unsigned short wk_ed[], unsigned short cou_st, unsigned short cou_ed, Ochead * ochead) {
  unsigned short dwk, dtm;
  count = 0;
  while (wk_st[count] && count < 5) {
    for (p = wk_st[count]; p <= wk_ed[count]; p++)
      for (n = cou_st; n <= cou_ed; n++) {
        //Insert the time-occupied nodes in ascending order of the number of weeks first and then the number of sections
        pocc = (Occupy * ) malloc(sizeof(Occupy));
        pocc -> week = p;
        pocc -> time = n;
        if (ochead -> len == 0) { //Insert at the first place when there is no node
          pocc -> next = ochead -> head;
          ochead -> head = pocc;
        } else {
          dwk = pocc -> week - ochead -> head -> week;
          dtm = pocc -> time - ochead -> head -> time;
          if (dwk < 0 || (dwk >= 0 && dtm < 0)) { //The minimum value of the new node
            pocc -> next = ochead -> head;
            ochead -> head = pocc;
          } else { //Insert in the middle of the time-occupied linked list
            for (qocc = ochead -> head; qocc -> next != NULL; qocc = qoccc -> next) {
              dwk = pocc -> week - qocc -> next -> week;
              dtm = pocc -> time - qocc -> next -> time;
              if (dwk < 0 || (dwk >= 0 && dtm < 0)) {
                pocc -> next = qocc -> next;
                qocc -> next = pocc;
              }
            }
            if (qocc -> next == NULL) { //Insert at the end of the time-occupied linked list
              pocc -> next = qocc -> next;
              qocc -> next = pocc;
            }
          }
        }
        ochead -> len++;
      }
    count++;
  }
  return TRUE;
}

Schedule * ClassInput(char cbuf[]) {
  fflush(stdin);
  Schedule * ps = NULL;
  scanf("%s", cbuf);
  getchar();
  //Determine the search head node according to the class number
  if (strstr(cbuf, "15")) ps = grade[0].schead;
  else if (strstr(cbuf, "16")) ps = grade[1].schead;
  else if (strstr(cbuf, "17")) ps = grade[2].schead;
  else {
    printf("The class number input error");
    getchar();
    ClearScreen();
    return NULL;
  }
  // check the existence of the class number
  for (; ps != NULL; ps = ps -> next)
    if (!strcmp(ps -> classname, cbuf)) break;
  return ps;
}

BOOL AddNewRoom(char cbuf[]) {
  if (rolist -> length >= rolist -> listsize) {
    Room * ro_newbase = (Room * ) realloc(rolist -> elem, (rolist -> listsize + LISTINCREMENT) * sizeof(Room));
    if (!ro_newbase) {
      printf("Memory allocation failed, return to main menu");
      getchar();
      ClearScreen();
      return TRUE;
    }
    rolist -> elem = ro_newbase;
    rolist -> listsize += LISTINCREMENT;
  }
  //Add a new classroom
  strcpy(rolist -> elem[rolist -> length].code, cbuf);
  //time structure initialization
  for (i = 0; i < 6; i++) {
    rolist -> elem[rolist -> length].ochead[i].head = NULL;
    rolist -> elem[rolist -> length].ochead[i].len = 0;
  }
  printf("The new classroom has been imported\n");
  return TRUE;
}

BOOL AddNewTeacher(char cbuf[]) {
  if (telist -> length >= telist -> listsize) {
    Teacher * te_newbase = (Teacher * ) realloc(telist -> elem, (telist -> listsize + LISTINCREMENT) * sizeof(Teacher));
    if (!te_newbase) {
      printf("Memory allocation failed, return to main menu");
      getchar();
      ClearScreen();
      return TRUE;
    }
    telist -> elem = te_newbase;
    telist -> listsize += LISTINCREMENT;
  }
  //add new teacher
  strcpy(telist -> elem[telist -> length].tname, cbuf);
  //time structure initialization
  for (i = 0; i < 6; i++) {
    telist -> elem[telist -> length].ochead[i].head = NULL;
    telist -> elem[telist -> length].ochead[i].len = 0;
  }
  printf("New teacher has been imported\n");
  return TRUE;
}

BOOL AddNewCourse(char cbuf[]) {
  if (cougraph -> length >= cougraph -> listsize) { //Lookup table expansion
    CouHead * cou_newbase = (CouHead * ) realloc(cougraph -> coulist, (cougraph -> listsize + LISTINCREMENT) * sizeof(CouHead));
    if (!cou_newbase) {
      printf("Memory allocation failed, will return to the main menu");
      getchar();
      ClearScreen();
      return TRUE;
    }
    cougraph -> coulist = cou_newbase;
    cougraph -> listsize += LISTINCREMENT;
  }
  // node initialization
  strcpy(cougraph -> coulist[cougraph -> length].name, cbuf);
  cougraph -> coulist[cougraph -> length].arcnum = 0;
  cougraph -> coulist[cougraph -> length].archhead = NULL;
  return TRUE;
}

BOOL SeekRemClass(Schedule * ps, char cbuf[]) {
  Class * qc;
  for (p = 0; p < ps -> pr_num; p++)
    if (!strcmp(ps -> progress[p], cbuf)) break; //Look for the deleted courses in the courses in the class
  flag = FALSE;
  if (p != ps -> pr_num) {
    for (n = 0; n < 6; n++) //Find out if this course still has classes
      for (qc = ps -> week[n].classhead; qc != NULL; qc = qc -> next) {
        if (flag == TRUE) break;
        if (!strcmp(qc -> coursename, ps -> progress[p])) {
          flag = TRUE;
          break;
        }
      }
    if (flag == FALSE) { //Delete the ongoing course when there is no class
      for (n = p; n < ps -> pr_num - 1; n++) strcpy(ps -> progress[n], ps -> progress[n + 1]); //Delete the corresponding course in the class node
      ps -> pr_num--;
    }
  }
  return TRUE;
}

BOOL DelClassNode(Class * pc, Schedule * ps, unsigned short hu) {
  Class * qc;
  if (pc == ps -> week[hu].classhead) { //head node under the course chain
    ps -> week[hu].classhead = pc -> next;
    free(pc);
    pc = ps -> week[j].classhead;
    if (ps -> week[hu].classhead == NULL) {
      ps -> week[hu].cl_num = 0;
      return TRUE;
    }
  } else { //non-head node under the course chain
    for (qc = ps -> week[hu].classhead; qc -> next != pc; qc = qc -> next); //find the target node predecessor
    qc -> next = pc -> next;
    free(pc);
    pc = qc;
  }
  psch -> week[hu].cl_num--;
  return TRUE;
}

BOOL TimeInput(unsigned short * cou_st, unsigned short * cou_ed, unsigned short wk_st[], unsigned short wk_ed[]) {
  fflush(stdin);
  printf("Enter the start time:");
  scanf("%hu", cou_st);
  getchar();
  printf("Enter the end of class time:");
  scanf("%hu", cou_ed);
  getchar();
  printf("Enter the number of weeks in class:\n");
  count = 0;
  do {
    printf("Starting week:");
    scanf("%hu", & wk_st[count]);
    getchar();
    printf("End week:");
    scanf("%hu", & wk_ed[count]);
    getchar();
    printf("Continue? Yes--Y\tNo--other keys\n");
    scanf("%c", & j);
    getchar();
    count++;
  } while (j == 'Y' || j == 'y');
  return TRUE;
}