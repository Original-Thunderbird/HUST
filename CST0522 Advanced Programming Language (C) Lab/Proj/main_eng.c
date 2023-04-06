#include "dorm.h"

unsigned long ul;
int main() {
  COORD size = {
    SCR_COL,
    SCR_ROW
  }; /* window buffer size */
  gh_std_out = GetStdHandle(STD_OUTPUT_HANDLE); /* Get standard output device handle */
  gh_std_in = GetStdHandle(STD_INPUT_HANDLE); /* Get standard input device handle */
  SetConsoleTitle(gp_sys_name); /* Set window title */
  SetConsoleScreenBufferSize(gh_std_out, size); /* Set the window buffer size to 80*25*/
  //FileIn();
  LoadData(); /* data loading */
  InitInterface(); /* Interface initialization */
  RunSys(); /* Selection and operation of system function modules */
  CloseSys(); /* Exit the system */
  return 0;
}

/**< first part text interface function */

/**
 * Function name : InitInterface
 * Function function : Initialize the interface .
 * Input parameters : None
 * Output parameters : None
 * back back Value : None
 *
 * Call instructions :
 */
void InitInterface() {
  WORD att = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY |
    BACKGROUND_BLUE; /* yellow foreground and blue background */
  SetConsoleTextAttribute(gh_std_out, att); /* Set console screen buffer character attributes */
  ClearScreen(); /* clear screen */
  /* Create pop-up window information stack, treat the initialized screen window as the first layer of pop-up window */
  gp_scr_att = (char * ) calloc(SCR_COL * SCR_ROW, sizeof(char)); /* screen character attributes */
  gp_top_layer = (LAYER_NODE * ) malloc(sizeof(LAYER_NODE));
  gp_top_layer -> LayerNo = 0; /* The layer number of the pop-up window is 0*/
  gp_top_layer -> rcArea.Left = 0; /* The area of the pop-up window is the entire screen window */
  gp_top_layer -> rcArea.Top = 0;
  gp_top_layer -> rcArea.Right = SCR_COL - 1;
  gp_top_layer -> rcArea.Bottom = SCR_ROW - 1;
  gp_top_layer -> pContent = NULL;
  gp_top_layer -> pScrAtt = gp_scr_att;
  gp_top_layer -> next = NULL;
  ShowMenu(); /* Show menu bar */
  ShowState(); /* Display status bar */
  return;
}

/**
 * Function name : ClearScreen
 * Function : Clear screen information .
 * Input parameters : none
 * Output parameters : none
 * back back Value : None
 *
 * Call instructions :
 */
void ClearScreen(void) {
  CONSOLE_SCREEN_BUFFER_INFO bInfo;
  COORD home = {
    0,
    1
  }; /* Only clear the information starting from the first line, so that it can be called by each function */
  unsigned long size;
  GetConsoleScreenBufferInfo(gh_std_out, & bInfo); /* Get screen buffer information */
  size = bInfo.dwSize.X * (bInfo.dwSize.Y); /* Calculate the number of screen buffer character units */
  /* Set the character attributes of all units in the screen buffer to the character attributes of the current screen buffer */
  FillConsoleOutputAttribute(gh_std_out, bInfo.wAttributes, size, home, & ul);
  /* Fill all cells of the screen buffer with space characters */
  FillConsoleOutputCharacter(gh_std_out, ' ', size, home, & ul);
  SetConsoleCursorPosition(gh_std_out, home); /* The cursor is placed in the upper left corner of the window */
  return;
}

/**
 * Function name : ShowMenu
 * Function function : display the main menu on the screen , set the hot zone , and put a check mark on the first item of the main menu .
 * Input parameters : None
 * Output parameters : None
 * back back Value : None
 *
 * Call instructions :
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
  for (i = 0; i < 5; i++) /* Output the main menu item at the first row and first column of the window */ {
    printf(" %s ", ga_main_menu[i]);
  }
  GetConsoleCursorInfo(gh_std_out, & lpCur);
  lpCur.bVisible = FALSE;
  SetConsoleCursorInfo(gh_std_out, & lpCur); /* hide cursor */
  /* Apply for a dynamic storage area as a buffer for storing character information in the menu bar screen area */
  gp_buff_menubar_info = (CHAR_INFO * ) malloc(size.X * size.Y * sizeof(CHAR_INFO));
  SMALL_RECT rcMenu = {
    0,
    0,
    size.X - 1,
    0
  };
  /* Read the content of the first line of the window into the buffer that stores the character information of the menu bar screen area */
  ReadConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, & rcMenu);
  /* Set the Chinese and English letters in this line to red, and set other character units to black on white background */
  for (i = 0; i < size.X; i++) {
    (gp_buff_menubar_info + i) -> Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN |
      BACKGROUND_RED;
    ch = (char)((gp_buff_menubar_info + i) -> Char.AsciiChar);
    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
      (gp_buff_menubar_info + i) -> Attributes |= FOREGROUND_RED;
    }
  }
  /* The modified menu bar character information is written back to the first line of the window */
  WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, & rcMenu);
  COORD endPos = {
    0,
    1
  };
  SetConsoleCursorPosition(gh_std_out, endPos);
  /* Set the cursor position to row 2, column 1 * /
/* Set the menu item as a hot zone, the hot zone number is the menu item number, and the hot zone type is 0 ( button type )*/
  i = 0;
  do {
    PosB = PosA + strlen(ga_main_menu[i]); /* Locate the start and end positions of the i+1th menu item */
    for (j = PosA; j < PosB; j++) {
      gp_scr_att[j] |= (i + 1) << 2; /* Set the attribute value of the character unit where the menu item is located */
    }
    PosA = PosB + 4;
    i++;
  } while (i < 5);
  TagMainMenu(gi_sel_menu); /* mark the selected main menu item, the initial value of gi_sel_menu is 1*/
  return;
}

/**
 * Function name : ShowState
 * Function function : display the status bar .
 * Input parameters : none
 * Output parameters : none
 * back back Value : None
 *
 * Calling description : The character attribute of the status bar is black on a white background , and there is no status information in the initial state .
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
 * Function name : TagMainMenu
 * Function function : Set the check mark on the specified main menu item .
 * Input parameters : num selected main menu item number
 * Output parameters : none
 * back back Value : None
 *
 * Call instructions :
 */
void TagMainMenu(int num) {
  CONSOLE_SCREEN_BUFFER_INFO bInfo;
  COORD size, pos = {
    0,
    0
  };
  int PosA = 2, PosB, i;
  char ch;
  if (num == 0) /*When num is 0 , the check mark of the main menu item will be removed */ {
    PosA = PosB = 0;
  } else /* Otherwise, locate the start and end positions of the selected main menu item : PosA is the start position , PosB is the end position */ {
    for (i = 1; i < num; i++) {
      PosA += strlen(ga_main_menu[i - 1]) + 4;
    }
    PosB = PosA + strlen(ga_main_menu[num - 1]);
  }
  GetConsoleScreenBufferInfo(gh_std_out, & bInfo);
  size.X = bInfo.dwSize.X;
  size.Y = 1;
  /* Remove the menu item check mark in front of the selected menu item */
  for (i = 0; i < PosA; i++) {
    (gp_buff_menubar_info + i) -> Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN |
      BACKGROUND_RED;
    ch = (gp_buff_menubar_info + i) -> Char.AsciiChar;
    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
      (gp_buff_menubar_info + i) -> Attributes |= FOREGROUND_RED;
    }
  }
  /* Mark on the selected menu item, black background and white text */
  for (i = PosA; i < PosB; i++) {
    (gp_buff_menubar_info + i) -> Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN |
      FOREGROUND_RED;
  }
  /* Remove the menu item check mark behind the selected menu item */
  for (i = PosB; i < bInfo.dwSize.X; i++) {
    (gp_buff_menubar_info + i) -> Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN |
      BACKGROUND_RED;
    ch = (char)((gp_buff_menubar_info + i) -> Char.AsciiChar);
    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
      (gp_buff_menubar_info + i) -> Attributes |= FOREGROUND_RED;
    }
  }
  /* Write the marked menu bar information to the first line of the window */
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
 * Function name : PopMenu
 * Function function : In the submenu and set the hot zone , set the check mark with the cursor movement .
 * Input parameters : num submenu number of items from top to bottom
 * Output parameters : none
 * back back Value : None
 *
 * Call instructions :
 */
void PopMenu(int num) {
  LABEL_BUNDLE labels;
  HOT_AREA areas;
  SMALL_RECT rcPop;
  COORD pos;
  WORD att;
  char * pCh;
  int i, j, loc = 0;
  if (num != gi_sel_menu) /* If the specified main menu is not the selected menu */ {
    if (gp_top_layer -> LayerNo != 0) /* If a submenu has popped up before */ {
      PopOff();
      gi_sel_sub_menu = 0;
    }
  } else if (gp_top_layer -> LayerNo != 0) /* If the submenu has popped up, return */ {
    return;
  }
  gi_sel_menu = num; /* Set the selected main menu item to the specified main menu item */
  TagMainMenu(gi_sel_menu); /* mark the selected main menu item */
  LocSubMenu(gi_sel_menu, & rcPop);
  /* Calculate the area position of the pop-up submenu and store it in rcPop * /
/* Calculate the position ( subscript ) of the first item in the submenu in the submenu string array */
  for (i = 1; i < gi_sel_menu; i++) {
    loc += ga_sub_menu_count[i - 1];
  }
  /* Store the item name of this group of submenu items in the tag bundle structure variable */
  labels.ppLabel = ga_sub_menu + loc; /* Address of the first label string in the label bundle */
  labels.num = ga_sub_menu_count[gi_sel_menu - 1]; /* The number of label strings in the label bundle */
  COORD aLoc[labels.num]; /* Define a coordinate array to store the coordinates of the output position of each label string */
  for (i = 0; i < labels.num; i++) /* Determine the output position of the label string and store it in the coordinate array */ {
    aLoc[i].X = rcPop.Left + 2;
    aLoc[i].Y = rcPop.Top + i + 1;
  }
  labels.pLoc = aLoc; /* Make the member pLoc of the label bundle structure variable labels point to the first element of the coordinate array */
  /* Set hot zone information */
  areas.num = labels.num; /* The number of hot areas is equal to the number of labels, that is, the number of items in the submenu */
  SMALL_RECT aArea[areas.num]; /* Define an array to store all hot area locations */
  char aSort[areas.num]; /* Define an array to store the corresponding categories of all hot areas */
  char aTag[areas.num]; /* Define an array to store the number of each hot area */
  for (i = 0; i < areas.num; i++) {
    aArea[i].Left = rcPop.Left + 2; /* Hot area positioning */
    aArea[i].Top = rcPop.Top + i + 1;
    aArea[i].Right = rcPop.Right - 2;
    aArea[i].Bottom = aArea[i].Top;
    aSort[i] = 0; /* hot zone category is 0 ( button type )*/
    aTag[i] = i + 1; /* Hot zones are numbered sequentially */
  }
  areas.pArea = aArea; /* Make the member pArea of the hot area structure variable areas point to the first element of the hot area position array */
  areas.pSort = aSort; /* Make the member pSort of the hot area structure variable areas point to the first element of the hot area category array */
  areas.pTag = aTag; /* Make the member pTag of the hot area structure variable areas point to the first element of the hot area number array */
  att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED; /* black text on white background */
  PopUp( & rcPop, att, & labels, & areas);
  DrawBox( & rcPop); /* Draw a border for the pop-up window */
  pos.X = rcPop.Left + 2;
  for (pos.Y = rcPop.Top + 1; pos.Y < rcPop.Bottom; pos.Y++) {
    /* This cycle is used to draw a line at the position of the empty string sub-menu item to form a separation, and cancel the hot zone attribute of this menu item */
    pCh = ga_sub_menu[loc + pos.Y - rcPop.Top - 1];
    if (strlen(pCh) == 0) /* The length of the string is 0 , indicating that it is an empty string */ {
      /* First draw a horizontal line */
      FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right - rcPop.Left - 3, pos, & ul);
      for (j = rcPop.Left + 2; j < rcPop.Right - 1; j++) {
        /* cancel the hot area attribute of the character unit in this area */
        gp_scr_att[pos.Y * SCR_COL + j] &= 3; /* The result of bitwise AND retains the lower two bits */
      }
    }
  }
  /* Set the function key of the submenu item as red letter on white background */
  pos.X = rcPop.Left + 3;
  att = FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
  for (pos.Y = rcPop.Top + 1; pos.Y < rcPop.Bottom; pos.Y++) {
    if (strlen(ga_sub_menu[loc + pos.Y - rcPop.Top - 1]) == 0) {
      continue; /* skip empty string */
    }
    FillConsoleOutputAttribute(gh_std_out, att, 1, pos, & ul);
  }
  return;
}

/**
* Function name : PopUp
* Function function : Output the pop-up window information in the specified area , set the hot zone at the same time , and push the pop-up window position information into the stack .
* Input parameters : the address where the location data of the pRc pop-up window is stored
* att popup window area character attribute
* The address where the label bundle information is stored in the pLabel pop-up window
The address where the hot area information is stored in the pHotArea pop-up window
* Output parameters : none
* back back Value : None
*
* Call instructions :
*/
void PopUp(SMALL_RECT * pRc, WORD att, LABEL_BUNDLE * pLabel, HOT_AREA * pHotArea) {
  LAYER_NODE * nextLayer;
  COORD size, pos = {
    0,
    0
  };
  char * pCh;
  int i, j, row;
  /* The character unit information of the location of the pop-up window is pushed into the stack */
  size.X = pRc -> Right - pRc -> Left + 1; /* The width of the pop-up window */
  size.Y = pRc -> Bottom - pRc -> Top + 1; /* The height of the pop-up window */
  /* Apply for a dynamic storage area for storing pop-up window related information */
  nextLayer = (LAYER_NODE * ) malloc(sizeof(LAYER_NODE));
  nextLayer -> next = gp_top_layer;
  nextLayer -> LayerNo = gp_top_layer -> LayerNo + 1;
  nextLayer -> rcArea = * pRc;
  nextLayer -> pContent = (CHAR_INFO * ) malloc(size.X * size.Y * sizeof(CHAR_INFO));
  nextLayer -> pScrAtt = (char * ) malloc(size.X * size.Y * sizeof(char));
  pCh = nextLayer -> pScrAtt;
  /* Save the character information of the area covered by the pop-up window to restore it when the pop-up window is closed */
  ReadConsoleOutput(gh_std_out, nextLayer -> pContent, size, pos, pRc);
  for (i = pRc -> Top; i <= pRc -> Bottom; i++) {
    /* This double cycle stores the original attribute value of the covered character unit into the dynamic storage area, which is convenient for later recovery */
    for (j = pRc -> Left; j <= pRc -> Right; j++) {
      * pCh = gp_scr_att[i * SCR_COL + j];
      pCh++;
    }
  }
  gp_top_layer = nextLayer; /* Complete pop-up window related information into the stack */
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
    /* This double loop sets the layer number of the character unit */
    for (j = pRc -> Left; j <= pRc -> Right; j++) {
      gp_scr_att[i * SCR_COL + j] = gp_top_layer -> LayerNo;
    }
  }

  for (i = 0; i < pHotArea -> num; i++) {
    /* This double loop sets the thermal zone type and thermal zone number of character cells in all thermal zones */
    row = pHotArea -> pArea[i].Top;
    for (j = pHotArea -> pArea[i].Left; j <= pHotArea -> pArea[i].Right; j++) {
      gp_scr_att[row * SCR_COL + j] |= (pHotArea -> pSort[i] << 6) |
        (pHotArea -> pTag[i] << 2);
    }
  }
  return;
}

/**
 * Function name : PopOff
 * Function function : close the top pop-up window , restore the original appearance of the covered area and the original attributes of the character unit .
 * Input parameters : None
 * Output parameters : None
 * back back Value : None
 *
 * Call instructions :
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
    /* The main interface screen information stored at the bottom of the stack, do not need to close */
    return;
  }
  nextLayer = gp_top_layer -> next;
  /* Restore the original appearance of the popup window area */
  size.X = gp_top_layer -> rcArea.Right - gp_top_layer -> rcArea.Left + 1;
  size.Y = gp_top_layer -> rcArea.Bottom - gp_top_layer -> rcArea.Top + 1;
  WriteConsoleOutput(gh_std_out, gp_top_layer -> pContent, size, pos, & (gp_top_layer -> rcArea));
  /* Restore the original attribute of the character unit */
  pCh = gp_top_layer -> pScrAtt;
  for (i = gp_top_layer -> rcArea.Top; i <= gp_top_layer -> rcArea.Bottom; i++) {
    for (j = gp_top_layer -> rcArea.Left; j <= gp_top_layer -> rcArea.Right; j++) {
      gp_scr_att[i * SCR_COL + j] = * pCh;
      pCh++;
    }
  }
  free(gp_top_layer -> pContent); /* Release dynamic storage area */
  free(gp_top_layer -> pScrAtt);
  free(gp_top_layer);
  gp_top_layer = nextLayer;
  gi_sel_sub_menu = 0;
  return;
}

/**
 * Function name : DrawBox
 * Function function : Draw a border in the specified area .
 * Input parameters : the address where pRc stores the location information of the region
 * Output parameters : None
 * back back Value : None
 *
 * Call instructions :
 */
void DrawBox(SMALL_RECT * pRc) {
  char chBox[] = {
    '+',
    '-',
    '|'
  }; /* Character used for picture frame */
  COORD pos = {
    pRc -> Left,
    pRc -> Top
  }; /* positioned at the upper left corner of the area */

  WriteConsoleOutputCharacter(gh_std_out, & chBox[0], 1, pos, & ul); /* draw the upper left corner of the frame */
  for (pos.X = pRc -> Left + 1; pos.X < pRc -> Right; pos.X++) {
    /* This loop draws a horizontal line on the border */
    WriteConsoleOutputCharacter(gh_std_out, & chBox[1], 1, pos, & ul);
  }
  pos.X = pRc -> Right;
  WriteConsoleOutputCharacter(gh_std_out, & chBox[0], 1, pos, & ul); /* draw the upper right corner of the frame */
  for (pos.Y = pRc -> Top + 1; pos.Y < pRc -> Bottom; pos.Y++) {
    /* This cycle draws the left and right lines of the border */
    pos.X = pRc -> Left;
    WriteConsoleOutputCharacter(gh_std_out, & chBox[2], 1, pos, & ul);
    pos.X = pRc -> Right;
    WriteConsoleOutputCharacter(gh_std_out, & chBox[2], 1, pos, & ul);
  }
  pos.X = pRc -> Left;
  pos.Y = pRc -> Bottom;
  WriteConsoleOutputCharacter(gh_std_out, & chBox[0], 1, pos, & ul); /* Draw the lower left corner of the frame */
  for (pos.X = pRc -> Left + 1; pos.X < pRc -> Right; pos.X++) {
    /* Draw the border horizontal line */
    WriteConsoleOutputCharacter(gh_std_out, & chBox[1], 1, pos, & ul);
  }
  pos.X = pRc -> Right;
  WriteConsoleOutputCharacter(gh_std_out, & chBox[0], 1, pos, & ul); /* Draw the lower right corner of the frame */
  return;
}

/**
 * Function name : TagSubMenu
 * Function function : Make a checkmark on the specified submenu item .
 * Input parameters : num selected submenu item number
 * Output parameters : none
 * back back Value : None
 *
 * Call instructions :
 */
void TagSubMenu(int num) {
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    int width;
    LocSubMenu(gi_sel_menu, & rcPop);
    //Calculate the area position of the pop-up submenu and store it in rcPop
    if ((num<1) || (num == gi_sel_sub_menu) || (num>rcPop.Bottom-rcPop.Top-1)) {
    // If the submenu item number is out of bounds, or the item submenu has been selected, return
      return;
    }
    pos.X = rcPop.Left + 2;
    width = rcPop.Right - rcPop.Left - 3;
    /* First cancel the mark on the original selected submenu item */
    if (gi_sel_sub_menu != 0){
      pos.Y = rcPop.Top + gi_sel_sub_menu;
      att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED; /* black text on white background */
      FillConsoleOutputAttribute(gh_std_out, att, width, pos, & ul);
      pos.X += 1;
      att |= FOREGROUND_RED; /* red text on white background */
      FillConsoleOutputAttribute(gh_std_out, att, 1, pos, & ul);
    }
    /* Make a check mark on the specified submenu item */
    pos.X = rcPop.Left + 2;
    pos.Y = rcPop.Top + num;
    att = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED; /* white text on black background */
    FillConsoleOutputAttribute(gh_std_out, att, width, pos, & ul);
    gi_sel_sub_menu = num; /* Modify the selected submenu item number */
    return;
}

/**
 * Function name : LocSubMenu
 * Function function : Calculate the position of the upper left corner and the lower right corner of the pop-up submenu area .
 * Input parameters : num selected main menu item number
 * Output parameters : rc stores the address of the location information of the region
 * back back Value : None
 *
 * Call instructions :
 */
void LocSubMenu(int num, SMALL_RECT * rc) {
  int i, len, loc = 0;
  rc -> Top = 1; /* The top of the area is set on the second line, and the line number is 1*/
  rc -> Left = 1;
  for (i = 1; i < num; i++) {
    /* Calculate the position of the left border of the area , and calculate the position of the first submenu item in the submenu string array */
    rc -> Left += strlen(ga_main_menu[i - 1]) + 4;
    loc += ga_sub_menu_count[i - 1];
  }
  rc -> Right = strlen(ga_sub_menu[loc]); /* Temporarily store the string length of the first submenu item */
  for (i = 1; i < ga_sub_menu_count[num - 1]; i++) {
    /* Find the longest submenu string and store its length in rc->Right*/
    len = strlen(ga_sub_menu[loc + i]);
    if (rc -> Right < len) {
      rc -> Right = len;
    }
  }
  rc -> Right += rc -> Left + 3; /* calculate the right boundary of the region */
  rc -> Bottom = rc -> Top + ga_sub_menu_count[num - 1] + 1; /* Calculate the line number below the area */
  if (rc -> Right >= SCR_COL) /* The processing of the right boundary crossing */ {
    len = rc -> Right - SCR_COL + 1;
    rc -> Left -= len;
    rc -> Right = SCR_COL - 1;
  }
  return;
}

/**
 * Function name : DealInput
 * Function function : set the hot zone in the pop-up window area , wait for and respond to user input .
 * Input parameters : pHotArea hot area parameter pointer
 * The storage address of the piHot focus hot zone number , that is, the pointer to the focus hot zone number
 * Output parameter : pHotArea hot area parameter
 * piHot returns the current hot zone number when clicking with the mouse, pressing enter or space
 * back back Value : iRet carriage return virtual key code
 *
 * Call instructions :
 */
int DealInput(HOT_AREA * pHotArea, int * piHot) {
  INPUT_RECORD inRec;
  DWORD res;
  COORD pos = {
    0,
    0
  };
  int num, arrow, iRet = 0;
  int cNo, cTag, cSort; /*cNo: layer number , cTag: hot zone number , cSort: hot zone type */
  char vkc, asc; /*vkc: virtual key code , asc: character ASCII code value */
  SetHotPoint(pHotArea, * piHot);
  while (TRUE) {
    /* loop */
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
          /* Arrow keys ( left, up, right, down ) processing */
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
        /*ESC key */
        iRet = 27;
        break;
      } else if (vkc == 13 || vkc == 32) {
        /* Enter key or space means pressing the current button */
        iRet = 13;
        break;
      }
    }
  }
  return iRet;
}

/**
 * Function name : SetHotPoint
 * Function function : set the hot zone in the pop-up window area , wait for and respond to user input .
 * Input parameters : pHotArea hot area parameter pointer
 * iHot focus hot zone number
 * Output parameter : pHotArea hot area parameter
 * back back Value : None
 *
 * Call instructions :
 */
void SetHotPoint(HOT_AREA * pHotArea, int iHot) {
  CONSOLE_CURSOR_INFO lpCur;
  COORD pos = {
    0,
    0
  };
  WORD att1, att2;
  int i, width;
  att1 = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED; /* white text on black background */
  att2 = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED; /* black text on white background */
  for (i = 0; i < pHotArea -> num; i++) {
    /* Set button hotspots to black text on a white background */
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
    /* The activated hotspot is the button class */
    FillConsoleOutputAttribute(gh_std_out, att1, width, pos, & ul);
  } else if (pHotArea -> pSort[iHot - 1] == 1) {
    /* The activated hotspot is the text box class */
    SetConsoleCursorPosition(gh_std_out, pos);
    GetConsoleCursorInfo(gh_std_out, & lpCur);
    lpCur.bVisible = TRUE;
    SetConsoleCursorInfo(gh_std_out, & lpCur);
  }
}

/**
 * Function name : ShowModule
 * Function function : restore the system code table and three types of basic data from the specified data file .
 * Input parameters : the first address of the two-dimensional array of pString text information
 * n number of strings
 * Output parameters :
 * back back Value : BOOL type , always TRUE
 *
 * Call instructions :
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
  att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED; /* black text on white background */
  labels.num = n;
  labels.ppLabel = pString;
  COORD aLoc[n];
  for (i = 0; i < n; i++) {
    aLoc[i].X = rcPop.Left + 3;
    aLoc[i].Y = rcPop.Top + (pos.Y) / 2 + i;
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
  pos.Y = rcPop.Top + (pos.Y) / 2 + n;
  FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right - rcPop.Left - 1, pos, & ul);
  DealInput( & areas, & iHot);
  PopOff();
  return bRet;
}

/**
 * Function name : Message
 * Function function : pop-up dialog box
 * Input parameters : **pCh dialog box information array first address
 * Output parameters : none
 * back back Value : BOOL type, exit is TRUE , cancel is FALSE
 *
 * Call instructions :
 */
BOOL Message(char ** pCh) {
  LABEL_BUNDLE labels;
  HOT_AREA areas;
  SMALL_RECT rcPop;
  COORD pos;
  WORD att;
  int iHot = 1;
  char len[3] = {
    strlen(pCh[0]),
    strlen(pCh[1]),
    strlen(pCh[2])
  };
  char lmax = (len[0] > len[1] + len[2] + 4) ? len[0] : len[1] + len[2] + 4;
  pos.X = lmax + 6;
  pos.Y = 7;
  char blanc = (pos.X - (len[1] + len[2] + 4)) / 2;
  rcPop.Left = (SCR_COL - pos.X) / 2 - 10;
  rcPop.Right = rcPop.Left + pos.X - 1;
  rcPop.Top = (SCR_ROW - pos.Y) / 2 - 12;
  rcPop.Bottom = rcPop.Top + pos.Y - 1;
  att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED; /* black text on white background */
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
/*
BOOL bRet;
int judge;
judge=MessageBoxA(NULL," Are you sure to exit the system? "," System Prompt ",MB_YESNO);
if(judge==IDYES){
bRet = FALSE;
}
else {
bRet = TRUE;
}
*/

/**< Second part System skeleton function */

/**
 * Function name : LoadData
 * Function function : load the code table and three types of basic data from the data file into the memory buffer and cross-linked list .
 * Input parameters : none
 * Output parameters : none
 * back back Value : BOOL type , in the function function except the return value of the function ExitSys can be FALSE ,
 * The return value of other functions must be TRUE.
 *
 * Calling instructions : In order to call each function function in a unified way , the prototype of these function functions is set to
 * Consistent , that is, there are no parameters and the return value is BOOL. When the return value is FALSE , the program ends .
 */

BOOL LoadData() {
  int Re = 0;
  Re = CreateList();
  gc_sys_state |= Re;
  gc_sys_state &= ~(1 + 2 + 4 - Re);
  if (gc_sys_state < (1 | 2 | 4)) {
    printf("\nSystem basic data is incomplete !\n");
    system("pause");
  }
  return TRUE;
}

/**
 * Function name : CreateList
 * Function function : read the basic data from the data file and store it in the created cross list .
 * Input parameters : none
 * Output parameter : head The address of the main chain head pointer , which is used to return the created cross chain .
 * back back Value : int type value , indicating the creation of the linked list .
 * 1 Faculty and department information data has been loaded, but there is no basic information of scientific research team and scientific research project data
 * 3 Faculty information and basic information data of scientific research teams have been loaded, but no scientific research project data
 * 7 Three types of basic data have been loaded
 *
 * Call instructions :
 */
int CreateList(void) {
  SCHOOL_NODE * head = NULL, tmp1;
  TEAM_NODE tmp2;
  PROJECT_NODE tmp3;
  FILE * pFile;
  int find;
  int re = 0;
  if ((pFile = fopen("school.dat", "rb+")) == NULL) {
    printf(" Failed to open the department information data file !\n");
    return re;
  }
  printf(" Department information data file opened successfully !\n");
  /* Read the dormitory information data from the data file, and store it in the main chain established by LIFO */
  while (fread( & tmp1, sizeof(SCHOOL_NODE), 1, pFile) == 1) {
    p = (SCHOOL_NODE * ) malloc(sizeof(SCHOOL_NODE));
    * p = tmp1;
    p -> team_head = NULL;
    p -> next = head;
    head = p;
  }
  fclose(pFile);
  if (head == NULL) {
    printf(" Department information data file failed to load !\n");
    return re;
  }
  printf(" Department information data file loaded successfully !\n");
  sch_head = head;
  re += 1;
  if ((pFile = fopen("team.dat", "rb+")) == NULL) {
    printf(" Failed to open the basic information data file of the scientific research team !\n");
    return re;
  }
  printf(" Scientific research team basic information data file opened successfully !\n");
  re += 2;
  /* Read the basic information data of students from the data file, and store it in the branch chain of the corresponding node of the main chain */
  while (fread( & tmp2, sizeof(TEAM_NODE), 1, pFile) == 1) {
    /* Create a node to store the basic information of students read from the data file */
    q = (TEAM_NODE * ) malloc(sizeof(TEAM_NODE));
    * q = tmp2;
    q -> proj_head = NULL;
    /* Find the main chain node corresponding to the dormitory building where the student lives on the main chain */
    p = head;
    while (p != NULL && strcmp(p -> school_name, q -> school_name)) {
      p = p -> next;
    }
    if (p != NULL) /* If found, insert the node into the student information branch in the last in first out mode */ {
      q -> next = p -> team_head;
      p -> team_head = q;
    } else /* If not found, release the memory space of the created node */ {
      free(q);
    }
  }
  fclose(pFile);
  if ((pFile = fopen("project.dat", "rb")) == NULL) {
    printf(" Failed to open scientific research team information data file !\n");
    return re;
  }
  printf(" Scientific research team information data file opened successfully !\n");
  re += 4;
  /* Read the student payment information data from the data file, and store it in the payment branch chain of the corresponding node of the student basic information branch chain */
  while (fread( & tmp3, sizeof(PROJECT_NODE), 1, pFile) == 1) {
    /* Create a node to store the student payment information read from the data file */
    r = (PROJECT_NODE * ) malloc(sizeof(PROJECT_NODE));
    * r = tmp3;
    /* Find the corresponding student information node on the student information branch chain */
    p = head;
    find = 0;
    while (p != NULL && find == 0) {
      q = p -> team_head;
      while (q != NULL && find == 0) {
        if (strcmp(q -> team_name, r -> team_name) == 0) {
          find = 1;
          break;
        }
        q = q -> next;
      }
      p = p -> next;
    }
    if (find) /* If found, insert the node into the branch chain of student payment information */ {
      r -> next = q -> proj_head;
      q -> proj_head = r;
    } else /* If not found, release the memory space of the created node */ {
      free(r);
    }
  }
  fclose(pFile);
  printf(" Press any key to continue ...\n");
  getch();
  return re;
}

/**
 * Function name : RunSys
 * Function function : run the system , and run the function module selected by the user under the main interface of the system .
 * Input parameters : None
 * Output parameters : None
 * back back Value : None
 *
 * Call instructions :
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
  int cNo, cAtt; /*cNo: character unit layer number , cAtt: character unit attribute */
  char vkc, asc; /*vkc: virtual key code , asc: character ASCII code value */
  while (bRet) {
    /* read a record from the console input buffer */
    ReadConsoleInput(gh_std_in, & inRec, 1, & res);
    if (inRec.EventType == MOUSE_EVENT) /* If the record is generated by mouse event */ {
      pos = inRec.Event.MouseEvent.dwMousePosition; /* Get mouse coordinate position */
      cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3; /* Get the layer number of this position */
      cAtt = gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2; /* take the character unit attribute */
      if (cNo == 0) /* The layer number is 0 , indicating that the position is not covered by the pop-up submenu */ {
        /* cAtt > 0 indicates that the position is in the hot zone ( main menu item character unit )
         * cAtt != gi_sel_menu indicates that the main menu item at this position is not selected
         * gp_top_layer->LayerNo > 0 indicates that there is currently a submenu pop-up
         */
        if (cAtt > 0 && cAtt != gi_sel_menu && gp_top_layer -> LayerNo > 0) {
          PopOff(); /* Close the pop-up submenu */
          gi_sel_sub_menu = 0; /* Set the item number of the selected submenu item to 0*/
          PopMenu(cAtt); /* Pop up the submenu corresponding to the main menu item where the mouse is located */
        }
      } else if (cAtt > 0) /* The position of the mouse is the menu item character unit of the pop-up submenu */ {
        TagSubMenu(cAtt); /* Make a checkmark on the submenu item */
      }

      if (inRec.Event.MouseEvent.dwButtonState ==
        FROM_LEFT_1ST_BUTTON_PRESSED) /* If the first button on the left side of the mouse is pressed */ {
        if (cNo == 0) /* The layer number is 0 , indicating that the position is not covered by the pop-up submenu */ {
          if (cAtt > 0) /* If the position is in the hot zone ( main menu item character unit )*/ {
            PopMenu(cAtt); /* Pop up the submenu corresponding to the main menu item where the mouse is located */
          }
          /* If the position does not belong to the character unit of the main menu item, and there is a submenu to pop up */
          else if (gp_top_layer -> LayerNo > 0) {
            PopOff(); /* Close the pop-up submenu */
            gi_sel_sub_menu = 0; /* Set the item number of the selected submenu item to 0*/
          }
        } else /* The layer number is not 0 , indicating that the position is covered by the pop-up submenu */ {
          if (cAtt > 0) /* if the position is in the hot zone ( submenu item character unit )*/ {
            PopOff(); /* Close the pop-up submenu */
            gi_sel_sub_menu = 0; /* Set the item number of the selected submenu item to 0*/
            /* Execute the corresponding function : gi_sel_menu main menu item number , cAtt submenu item number */
            bRet = ExeFunction(gi_sel_menu, cAtt);
          }
        }
      } else if (inRec.Event.MouseEvent.dwButtonState ==
        RIGHTMOST_BUTTON_PRESSED) /* If the right mouse button is pressed */ {
        if (cNo == 0) /* The layer number is 0 , indicating that the position is not covered by the pop-up submenu */ {
          PopOff(); /* Close the pop-up submenu */
          gi_sel_sub_menu = 0; /* Set the item number of the selected submenu item to 0*/
        }
      }
    } else if (inRec.EventType == KEY_EVENT /* if the record is generated by a key */ &&
      inRec.Event.KeyEvent.bKeyDown) /* and the key is pressed */ {
      vkc = inRec.Event.KeyEvent.wVirtualKeyCode; /* Get the virtual key code of the key */
      asc = inRec.Event.KeyEvent.uChar.AsciiChar; /* Get the ASC code of the key */
      /* Handling of system shortcut keys */
      if (vkc == 112) /* if the F1 key is pressed */ {
        if (gp_top_layer -> LayerNo != 0) /* If a submenu pops up */ {
          PopOff(); /* Close the pop-up submenu */
          gi_sel_sub_menu = 0; /* Set the item number of the selected submenu item to 0*/
        }
        bRet = ExeFunction(5, 1); /* run help topic function */
      } else if (inRec.Event.KeyEvent.dwControlKeyState &
        (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED)) {
        /* If the left or right Alt key is pressed */
        switch (vkc) /* judgment key combination Alt+ letter */ {
        case 88:
          /*Alt+X to exit */
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
      } else if (asc == 0) /* handling of other control keys */ {
        if (gp_top_layer -> LayerNo == 0) /* If no submenu pops up */ {
          switch (vkc) /* handle the arrow keys ( left, right, down ) , do not respond to other control keys */ {
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
        } else /* When the submenu has popped up */ {
          for (loc = 0, i = 1; i < gi_sel_menu; i++) {
            loc += ga_sub_menu_count[i - 1];
          } /* Calculate the position ( subscript ) of the first item in the submenu in the submenu string array */
          switch (vkc) /* Arrow keys ( left, right, up, down ) processing */ {
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
        /* Press a normal key */
        if (gp_top_layer -> LayerNo == 0) /* If no submenu pops up */ {
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
            /* Enter */
            PopMenu(gi_sel_menu);
            TagSubMenu(1);
            break;
          }
        } else /* Keyboard input processing when the submenu has popped up */ {
          if (vkc == 27) /* if the ESC key is pressed */ {
            PopOff();
            gi_sel_sub_menu = 0;
          } else if (vkc == 13) /* if enter is pressed */ {
            num = gi_sel_sub_menu;
            PopOff();
            gi_sel_sub_menu = 0;
            bRet = ExeFunction(gi_sel_menu, num);
          } else /* other common key processing */ {
            /* Calculate the position ( subscript ) of the first item in the submenu in the submenu string array */
            for (loc = 0, i = 1; i < gi_sel_menu; i++) {
              loc += ga_sub_menu_count[i - 1];
            }

            /* Compare with the representative characters of each item in the current submenu in turn */
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
 * Function name : ExeFunction
 * Function function : Execute the function determined by the main menu number and submenu number .
 * Input parameters : mMain menu item number
 * s submenu item number
 * Output parameters : none
 * back back Value : BOOL type , TRUE or FALSE
 *
 * Call description : Only when the function ExitSys is executed , it is possible to return FALSE, and it always returns TRUE in other cases
 */
BOOL ExeFunction(int m, int s) {
  BOOL bRet = TRUE;
  /* Array of function pointers, used to store the entry addresses of all function functions */
  BOOL( * pFunction[ga_sub_menu_count[0] + ga_sub_menu_count[1] + ga_sub_menu_count[2] + ga_sub_menu_count[3] + ga_sub_menu_count[4]])(void);
  int i, loc;
  /* Save the entry address of the function function into the array element corresponding to the subscript of the main menu number and submenu number where the function function is located */
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
  for (i = 1, loc = 0; i < m; i++) /*Calculate the corresponding subscript according to the main menu number and submenu number */ {
    loc += ga_sub_menu_count[i - 1];
  }
  loc += s - 1;
  if (pFunction[loc] != NULL) {
    bRet = ( * pFunction[loc])(); /* use the function pointer to call the pointed function */
  }
  return bRet;
}

/**
 * Function name : CloseSys
 * Function function : close the system .
 * Input parameters : hd main chain head pointer
 * Output parameters : none
 * back back Value : None
 *
 * Call instructions :
 */
void CloseSys(void) {
  ClearMemory();
  ClearScreen(); /* clear screen */
  /* Release the dynamic storage area for storing information such as menu bar, status bar, gender code and student category code */
  free(gp_buff_menubar_info);
  free(gp_buff_stateBar_info);
  /* Close standard input and output device handles */
  CloseHandle(gh_std_out);
  CloseHandle(gh_std_in);
  /* Set the window title bar to the end of the run */
  SetConsoleTitle(" End of operation ");
  return;
}

/**
 * Function name : ExitSys
 * Function function : pop up the option to exit the system
 * Input parameters : none
 * Output parameters : none
 * back back Value : BOOL type, exit is TRUE , cancel is FALSE
 *
 * Call instructions :
 */
BOOL ExitSys(void) {
  char * pCh[] = {
    " Are you sure to exit the system? ",
    " OK ",
    " Cancel "
  };
  BOOL bRet = Message(pCh);
  PopOff();
  return bRet;
}

/**
 * Function name : ClearMemory
 * Function function : clear memory
 * Input parameters : none
 * Output parameters : none
 * back back Value : None
 *
 * Call instructions :
 */
void ClearMemory(void) {
  p1 = sch_head;
  while (p1 != NULL) /* Release the dynamic storage area of the cross-linked list */ {
    p2 = p1 -> next;
    q1 = p1 -> team_head;
    while (q1 != NULL) /* Release the dynamic storage area of the student's basic information branch chain */ {
      q2 = q1 -> next;
      r1 = q1 -> proj_head;
      while (r1 != NULL) /* Release the dynamic storage area of the payment information branch chain */ {
        r2 = r1 -> next;
        free(r1);
        r1 = r2;
      }
      free(q1);
      q1 = q2;
    }
    free(p1); /* Release the dynamic storage area of the main chain node */
    p1 = p2;
  }
}

BOOL SaveData(void) {
  BOOL bRet = TRUE;
  char * plabel_name[] = {
    " Main menu item: File ",
    " Submenu item: Data Save ",
    " Confirm "
  };

  ShowModule(plabel_name, 3);

  return bRet;
}

/**< The third part System function function */

/**
 * Function name : BackupData
 * Function function : backup the system code table and three types of basic data to a data file .
 * Input parameters : None
 * Output parameters : None
 * back back Value : BOOL type , always TRUE
 *
 * Call instructions :
 */
BOOL BackupData(void) {
  FILE * out;
  // int handle;
  unsigned short school_num = 0, team_num = 0, project_num = 0;
  time_t lt;
  char * filename, * pc, * appe = ".dat";
  for (p = sch_head; p != NULL; p = p -> next) {
    school_num++;
    q = p -> team_head;
    while (q != NULL) {
      team_num++;
      r = q -> proj_head;
      while (r != NULL) {
        project_num++;
        r = r -> next;
      }
      q = q -> next;
    }
  }
  // printf("%hu %hu %hu\n",school_num,team_num,project_num);
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
  /*
  if((handle=open(filename,O_WRONLY|O_BINARY)== -1)){
  handle=open(filename,O_CREAT|O_BINARY|O_RDWR,S_IREAD|S_IWRITE);
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
  if ((out = fopen(filename, "wb")) == NULL) {
    printf(" Failed to open! \n");
  }
  fwrite( & school_num, sizeof(unsigned short), 1, out);
  fwrite( & team_num, sizeof(unsigned short), 1, out);
  fwrite( & project_num, sizeof(unsigned short), 1, out);
  for (p = sch_head; p != NULL; p = p -> next) {
    fwrite(p, sizeof(SCHOOL_NODE), 1, out);
  }
  for (p = sch_head; p != NULL; p = p -> next) {
    for (q = p -> team_head; q != NULL; q = q -> next) {
      fwrite(q, sizeof(TEAM_NODE), 1, out);
    }
  }
  for (p = sch_head; p != NULL; p = p -> next) {
    for (q = p -> team_head; q != NULL; q = q -> next) {
      for (r = q -> proj_head; r != NULL; r = r -> next) {
        fwrite(r, sizeof(PROJECT_NODE), 1, out);
      }
    }
  }
  fclose(out);
  printf(" Backup succeeded! Press any key to continue ...\n");
  getch();
  ClearScreen();
  return TRUE;
}

/**
 * Function name : RestoreData
 * Function function : restore three types of basic data from the specified data file .
 * Input parameters : none
 * Output parameters : none
 * back back Value : BOOL type , always TRUE
 *
 * Call instructions :
 */
BOOL RestoreData(void) {
  // FILE *in;
  CONSOLE_CURSOR_INFO lpCur;
  GetConsoleCursorInfo(gh_std_out, & lpCur);
  lpCur.bVisible = TRUE;
  SetConsoleCursorInfo(gh_std_out, & lpCur);
  printf(" The imported data will overwrite the current data of the system! Continue - Y to return to the main menu - any other key \n");
  fflush(stdin);
  j = getchar();
  getchar();
  if (j != 'y' && j != 'Y') {
    ClearScreen();
    return TRUE;
  }
  ClearMemory();
  char input[100], * filename;
  printf(" Please enter the file name of the data you want to restore :");
  fflush(stdin);
  scanf("%s", input);
  filename = input;
  SCHOOL_NODE * hd = NULL, * psch_node;
  TEAM_NODE * pteam_node;
  PROJECT_NODE * psciprj_node;
  unsigned short sch_node_num = 0, team_node_num = 0, sciprj_node_num = 0, loop;
  int handle, find;
  if ((handle = open(filename, O_RDONLY | O_BINARY)) == -1) {
    char * plabel_name[] = {
      " The backup file you entered does not exist! ",
      " Data recovery failed, the last updated data will be loaded automatically ",
      " Confirm "
    };
    ShowModule(plabel_name, 3);
    CreateList();
    ClearScreen();
    return TRUE;
  }
  /* Read the number of records of the three basic information */
  read(handle, (char * ) & sch_node_num, sizeof(sch_node_num));
  read(handle, (char * ) & team_node_num, sizeof(team_node_num));
  read(handle, (char * ) & sciprj_node_num, sizeof(sciprj_node_num));
  /* Read the basic information of the department and create a cross-linked list */
  for (loop = 1; loop <= sch_node_num; loop++) {
    psch_node = (SCHOOL_NODE * ) malloc(sizeof(SCHOOL_NODE));
    read(handle, (char * ) psch_node, sizeof(SCHOOL_NODE));
    psch_node -> team_head = NULL; // cannot hang the pointer
    psch_node -> next = hd;
    hd = psch_node;
  }
  sch_head = hd;
  /* Read the basic information of the scientific research team, and establish a branch chain of scientific research team information */
  for (loop = 1; loop <= team_node_num; loop++) {
    pteam_node = (TEAM_NODE * ) malloc(sizeof(TEAM_NODE));
    read(handle, (char * ) pteam_node, sizeof(TEAM_NODE));
    pteam_node -> proj_head = NULL; // cannot hang pointer
    psch_node = hd;
    while ((psch_node != NULL) && (strcmp(psch_node -> school_name, pteam_node -> school_name) != 0)) {
      psch_node = psch_node -> next;
    }
    if (psch_node != NULL) {
      pteam_node -> next = psch_node -> team_head;
      psch_node -> team_head = pteam_node;
    } else {
      free(pteam_node);
    }
  }
  /* Read the basic information of scientific research projects, and establish a branch chain of basic information of scientific research projects */
  for (loop = 1; loop <= sciprj_node_num; loop++) {
    psciprj_node = (PROJECT_NODE * ) malloc(sizeof(PROJECT_NODE));
    read(handle, (char * ) psciprj_node, sizeof(PROJECT_NODE));
    psch_node = hd;
    find = 0;
    while ((psch_node != NULL) && (find == 0)) {
      pteam_node = psch_node -> team_head;
      while ((pteam_node != NULL) && (find == 0)) {
        if (strcmp(pteam_node -> team_name, psciprj_node -> team_name) == 0) {
          find = 1;
          break;
        }
        pteam_node = pteam_node -> next;
      }
      psch_node = psch_node -> next;
    }
    if (find) {
      psciprj_node -> next = pteam_node -> proj_head;
      pteam_node -> proj_head = psciprj_node;
    } else {
      free(psciprj_node);
    }
  }
  close(handle);
  printf(" Press any key to continue ...\n");
  getch();
  ClearScreen();
  GetConsoleCursorInfo(gh_std_out, & lpCur);
  lpCur.bVisible = FALSE;
  SetConsoleCursorInfo(gh_std_out, & lpCur);
  return TRUE;
}

/**
 * Function name : FileIn
 * Function function : Import three types of basic data (spare) from the specified data file .
 * Input parameters : none
 * Output parameter : sch_head first address of cross linked list
 * back back Value : BOOL type , always TRUE
 *
 * Call instructions :
 */
BOOL FileIn(void) {
  FILE * in;
  char find = 0;
  SCHOOL_NODE * head = NULL;
  /* Enter department data */
  if ((in = fopen("school.dat", "rb+")) == NULL) {
    printf(" failed to open school.dat !\n");
  }
  while (!feof(in)) {
    p = (SCHOOL_NODE * ) malloc(sizeof(SCHOOL_NODE));
    if (!fread(p, sizeof(SCHOOL_NODE), 1, in)) {
      free(p);
      break;
    }
    p -> team_head = NULL;
    p -> next = head;
    head = p;
  }
  fclose(in);
  /* Rewind the pointer, enter the team information and classify it under each department */
  sch_head = head;
  if ((in = fopen("team.dat", "rb+")) == NULL) {
    printf("team.dat failed to open !\n");
  }
  while (!feof(in)) {
    q = (TEAM_NODE * ) malloc(sizeof(TEAM_NODE));
    if (!fread(q, sizeof(TEAM_NODE), 1, in)) {
      free(q);
      break;
    }
    q -> proj_head = NULL;
    p = head;
    while (p != NULL && (strcmp(p -> school_name, q -> school_name))) {
      p = p -> next;
    }
    if (p != NULL) {
      q -> next = p -> team_head;
      p -> team_head = q;
    } else {
      free(q);
    }
  }
  fclose(in);
  /* Rewind the pointer, enter project information and classify it under each team */
  if ((in = fopen("project.dat", "rb+")) == NULL) {
    printf("project.dat failed to open !\n");
  }
  while (!feof(in)) {
    find = 0;
    r = (PROJECT_NODE * ) malloc(sizeof(PROJECT_NODE));
    if (!fread(r, sizeof(PROJECT_NODE), 1, in)) {
      free(r);
      break;
    }
    p = head;
    while (p != NULL && find == 0) {
      q = p -> team_head;
      while (q != NULL && find == 0) {
        if (!strcmp(q -> team_name, r -> team_name)) {
          find = 1;
          break;
        }
        q = q -> next;
      }
      p = p -> next;
    }
    if (find) {
      r -> next = q -> proj_head;
      q -> proj_head = r;
    } else {
      free(r);
    }
  }
  fclose(in);
  printf(" The data has been read! \n");
  system("pause");
  return TRUE;
}

/**
 * Function name : FileOut
 * Function function : export system data to specified data file and text file (spare) .
 * Input parameters : none
 * Output parameters : none
 * back back Value : BOOL type , always TRUE
 *
 * Call instructions :
 */
BOOL FileOut(void) /* Data is stored in binary file and text file at the same time, and the binary file is used as the basis for the next entry into the system to generate a linked list */ {
  FILE * out;
  /* Write a txt file, the department information - team information - project information is in a three-layer indentation format, which is easy to read */
  if ((out = fopen("output.txt", "w+")) == NULL) {
    printf("output.txt failed to open! ");
    exit(-1);
  }
  for (p = sch_head; p != NULL; p = p -> next) {
    fprintf(out, " Department name: \t\t%s\nPerson in charge: \t\t%s\nContact number \t\t%s\n",
      p -> school_name, p -> leader, p -> contact_num);
    for (q = p -> team_head; q != NULL; q = q -> next) {
      fprintf(out, "\tTeam name: \t\t%s\n\tPrincipal: \t\t%s\n\tNumber of teachers \t\t%d\n\tNumber of graduate students \t\t%d\n",
        q -> team_name, q -> leader, q -> teacher_num, q -> stu_num);
      for (r = q -> proj_head; r != NULL; r = r -> next) {
        fprintf(out, "\t\tProject number: \t\t%s\n\t\tProject category: \t\t", r -> project_num);
        /* The project type number is restored to the name for easy viewing */
        switch (r -> type) {
        case '1':
          fprintf(out, "973 project \n");
          break;
        case '2':
          fprintf(out, " National Natural Science Foundation of China \n");
          break;
        case '3':
          fprintf(out, "863 project \n");
          break;
        case '4':
          fprintf(out, " International Cooperation Project \n");
          break;
        case '5':
          fprintf(out, " horizontal item \n");
          break;
        }
        fprintf(out, "\t\tStarting time: \t\t%s\n\t\tProject funding: \t\t%f ten thousand yuan \n\t\tPerson in charge: \t\t%s\n",
          r -> open_time, r -> budget, r -> leader);
      }
    }
  }
  fclose(out);
  /* Write department dat file */
  if ((out = fopen("school.dat", "wb+")) == NULL) {
    printf("School.dat failed to open! ");
    exit(-1);
  }
  for (p = sch_head; p != NULL; p = p -> next) {
    fwrite(p, sizeof(SCHOOL_NODE), 1, out);
  }
  fclose(out);
  /* Write team dat file */
  if ((out = fopen("team.dat", "wb+")) == NULL) {
    printf("team.dat failed to open! ");
    exit(-1);
  }
  for (p = sch_head; p != NULL; p = p -> next) {
    for (q = p -> team_head; q != NULL; q = q -> next) {
      fwrite(q, sizeof(TEAM_NODE), 1, out);
    }
  }
  fclose(out);
  /* write project dat file */
  if ((out = fopen("project.dat", "wb+")) == NULL) {
    printf("project.dat failed to open! ");
    exit(-1);
  }
  for (p = sch_head; p != NULL; p = p -> next) {
    for (q = p -> team_head; q != NULL; q = q -> next) {
      for (r = q -> proj_head; r != NULL; r = r -> next) {
        fwrite(r, sizeof(PROJECT_NODE), 1, out);
      }
    }
  }
  fclose(out);
  return TRUE;
}

/**
 * Function name : Print
 * Function function : display all information of the cross list in the interface (backup) .
 * Input parameters : None
 * Output parameters : None
 * back back Value : BOOL type , always TRUE
 *
 * Call instructions :
 */
BOOL Print(void) {
  /* Write stdout , department information - team information - project information in three-layer indentation format, easy to read */
  for (p = sch_head; p != NULL; p = p -> next) {
    printf(" Department name: \t%s\nPerson in charge: \t%s\nContact number \t%s\n",
      p -> school_name, p -> leader, p -> contact_num);
    for (q = p -> team_head; q != NULL; q = q -> next) {
      printf("\tTeam name: \t%s\n\tPrincipal: \t%s\n\tNumber of teachers \t%d\n\tNumber of graduate students \t%d\n",
        q -> team_name, q -> leader, q -> teacher_num, q -> stu_num);
      for (r = q -> proj_head; r != NULL; r = r -> next) {
        printf("\t\tProject number: \t%s\n\t\tProject category: \t", r -> project_num);
        switch (r -> type) {
        case '1':
          printf("973 project \n");
          break;
        case '2':
          printf(" National Natural Science Foundation of China \n");
          break;
        case '3':
          printf("863 project \n");
          break;
        case '4':
          printf(" International Cooperation Project \n");
          break;
        case '5':
          printf(" horizontal item \n");
          break;
        }
        printf("\t\tStarting time: \t%s\n\t\tProject funding: \t%f ten thousand yuan \n\tPerson in charge: \t%s\n",
          r -> open_time, r -> budget, r -> leader);
      }
    }
  }
  return TRUE;
}

/**
 * Function name : MaintainSchoolInfo
 * Function function : Faculty information maintenance directory
 * Input parameters : none
 * Output parameters : none
 * back back Value : BOOL type , always TRUE
 *
 * Call instructions :
 */
BOOL MaintainSchoolInfo(void) {
  BOOL bRet = TRUE;
  do {
    printf(" What do you want to do? \n");
    printf(" Enter press1\n");
    printf(" Modify press2\n");
    printf(" delete press3\n");
    printf(" Back to main menu press4\n");
    fflush(stdin);
    k = getchar();
    getchar();
    if (k == '4') {
      ClearScreen();
      return TRUE;
    }
  } while (k < '1' || k > '4');
  ClearScreen();
  if (k == '1') {
    bRet = AddSchool();
  } else if (k == '2') {
    bRet = AltSchool();
  } else {
    bRet = DeleteSchool();
  }
  return bRet;
}

/**
 * Function name : MaintainTeamInfo
 * Function function : scientific research team information maintenance directory
 * Input parameters : none
 * Output parameters : none
 * back back Value : BOOL type , always TRUE
 *
 * Call instructions :
 */
BOOL MaintainTeamInfo(void) {
  BOOL bRet = TRUE;
  do {
    printf(" What do you want to do? \n");
    printf(" Enter press1\n");
    printf(" Modify press2\n");
    printf(" delete press3\n");
    printf(" Back to main menu press4\n");
    fflush(stdin);
    k = getchar();
    getchar();
    if (k == '4') {
      ClearScreen();
      return TRUE;
    }
  } while (k < '1' || k > '4');
  ClearScreen();
  if (k == '1') {
    bRet = AddTeam();
  } else if (k == '2') {
    bRet = AltTeam();
  } else {
    bRet = DeleteTeam();
  }
  return bRet;
}

/**
 * Function name : MaintainProjectInfo
 * Function function : scientific research project information maintenance directory
 * Input parameters : None
 * Output parameters : none
 * back back Value : BOOL type , always TRUE
 *
 * Call instructions :
 */
BOOL MaintainProjectInfo(void) {
  BOOL bRet = TRUE;
  do {
    printf(" What do you want to do? \n");
    printf(" Enter press1\n");
    printf(" Modify press2\n");
    printf(" delete press3\n");
    printf(" Back to main menu press4\n");
    fflush(stdin);
    k = getchar();
    getchar();
    if (k == '4') {
      ClearScreen();
      return TRUE;
    }
  } while (k < '1' || k > '4');
  ClearScreen();
  if (k == '1') {
    bRet = AddProject();
  } else if (k == '2') {
    bRet = AltProject();
  } else {
    bRet = DeleteProject();
  }
  return bRet;
}

/**
 * Function name : AddSchool
 * Function function : Insert a department information node in the cross list .
 * Input parameters : none
 * Output parameters : none
 * back back Value : BOOL type - TRUE
 *
 * Call instructions :
 */
BOOL AddSchool(void) {
  while (1) {
    ClearScreen();
    fflush(stdin);
    p = (SCHOOL_NODE * ) malloc(sizeof(SCHOOL_NODE));
    /*p -data member entry start */
    while (1) {
      flag = 0;
      printf(" Enter the department name (note that it is case-sensitive): ");
      scanf("%s", p -> school_name);
      getchar();
      for (p1 = sch_head; p1 != NULL; p1 = p1 -> next) {
        if (!strcmp(p1 -> school_name, p -> school_name)) {
          printf(" This department already exists ! Return to the main menu? Confirm --Y to cancel -- any other key \n");
          k = getchar();
          getchar();
          if (k == 'y' || k == 'Y') {
            free(p);
            p = NULL;
            ClearScreen();
            return TRUE;
          }
          strcpy(p -> school_name, "");
          flag = 1;
          ClearScreen();
          break;
        }
        if (flag) {
          break;
        }
      }
      if (!flag) {
        break;
      }
    }
    printf(" Enter the name of the person in charge of the department: ");
    scanf("%s", p -> leader);
    getchar();
    printf(" Enter contact number: ");
    scanf("%s", p -> contact_num);
    getchar();
    p -> team_head = NULL;
    /*p- End of data member entry */
    p -> next = sch_head;
    sch_head = p;
    printf(" Continue - Y returns to the main menu - any other key \n");
    j = getchar();
    getchar();
    FileOut();
    if (j != 'Y' && j != 'y') {
      break;
    }
  }
  ClearScreen();
  return TRUE;
}

/**
 * Function name : AddTeam
 * Function function : Insert a scientific research team information node into the cross-linked list .
 * Input parameters : none
 * Output parameters : none
 * back back Value : BOOL type - TRUE
 *
 * Call instructions :
 */
BOOL AddTeam(void) {
  while (1) {
    ClearScreen();
    fflush(stdin);
    flag = 0;
    q = (TEAM_NODE * ) malloc(sizeof(TEAM_NODE));
    while (1) {
      flag = 0;
      printf(" Enter team name: ");
      scanf("%s", q -> team_name);
      getchar();
      for (p1 = sch_head; p1 != NULL; p1 = p1 -> next) {
        for (q1 = p1 -> team_head; q1 != NULL; q1 = q1 -> next) {
          if (!strcmp(q1 -> team_name, q -> team_name)) {
            printf(" This team already exists ! Return to the main menu? Confirm --Y to cancel -- any other key \n");
            k = getchar();
            getchar();
            if (k == 'y' || k == 'Y') {
              free(q);
              q = NULL;
              ClearScreen();
              return TRUE;
            }
            strcpy(q -> team_name, "");
            flag = 1;
            break;
          }
          if (flag) {
            break;
          }
        }
        if (flag) {
          break;
        }
      }
      if (!flag) {
        break;
      }
    }
    printf(" Enter the department of the team (note that it is case-sensitive): ");
    scanf("%s", q -> school_name);
    getchar();
    for (p = sch_head; p != NULL; p = p -> next) {
      /* If you find the department you are in, exit and keep the current p position */
      if (!strcmp(p -> school_name, q -> school_name)) {
        flag = 1;
        break;
      }
    }
    /* If the team is in the new department, create a new department according to the input department name and create the corresponding team */
    if (!flag) {
      p = (SCHOOL_NODE * ) malloc(sizeof(SCHOOL_NODE));
      /*p -data member entry start */
      printf(" Enter the name of the person in charge of the department: ");
      scanf("%s", p -> leader);
      getchar();
      printf(" Enter contact number: ");
      scanf("%s", p -> contact_num);
      getchar();
      strcpy(p -> school_name, q -> school_name);
      p -> team_head = NULL;
      /*p- End of data member entry */
      p -> next = sch_head;
      sch_head = p;
    }
    /*q- data member input start */
    printf(" Enter the name of the team leader: ");
    scanf("%s", q -> leader);
    getchar();
    printf(" Enter the number of teachers: ");
    scanf("%d", & q -> teacher_num);
    getchar();
    printf(" Enter the number of graduate students: ");
    scanf("%d", & q -> stu_num);
    getchar();
    q -> proj_head = NULL;
    /*q- End of data member entry */
    q -> next = p -> team_head;
    p -> team_head = q;
    printf(" Continue - Y returns to the main menu - any other key \n");
    j = getchar();
    getchar();
    FileOut();
    if (j != 'Y' && j != 'y') {
      break;
    }
  }
  ClearScreen();
  return TRUE;
}

/**
 * Function name : AddProject
 * Function function : Insert a scientific research project information node in the cross-linked list .
 * Input parameters : none
 * Output parameters : none
 * back back Value : BOOL type - TRUE
 *
 * Call instructions :
 */
BOOL AddProject(void) {
  char school_name[20];
  while (1) {
    ClearScreen();
    fflush(stdin);
    flag = 0;
    r = (PROJECT_NODE * ) malloc(sizeof(PROJECT_NODE));
    while (1) {
      flag = 0;
      printf(" Enter item number: ");
      scanf("%s", r -> project_num);
      getchar();
      for (p1 = sch_head; p1 != NULL; p1 = p1 -> next) {
        for (q1 = p1 -> team_head; q1 != NULL; q1 = q1 -> next) {
          for (r1 = q1 -> proj_head; r1 != NULL; r1 = r1 -> next) {
            if (!strcmp(r1 -> project_num, r -> project_num)) {
              printf(" This item already exists ! Return to the main menu? Confirm --Y to cancel -- any other key \n");
              k = getchar();
              getchar();
              if (k == 'y' || k == 'Y') {
                free(r);
                r = NULL;
                ClearScreen();
                return TRUE;
              }
              strcpy(r -> project_num, "");
              flag = 1;
              break;
            }
            if (flag) {
              break;
            }
          }
          if (flag) {
            break;
          }
        }
        if (flag) {
          break;
        }
      }
      if (!flag) {
        break;
      }
    }
    printf(" Enter the department where the project is located (note that it is case-sensitive): ");
    scanf("%s", school_name);
    getchar();
    printf(" Enter the team name of the project: ");
    scanf("%s", r -> team_name);
    getchar();
    for (p = sch_head; p != NULL; p = p -> next) {
      /* If you find the department you are in, exit and keep the current p position */
      if (!strcmp(p -> school_name, school_name)) {
        flag = 1;
        break;
      }
    }
    /* If the project is attached to the new department, create a new department according to the input department name and create the corresponding department and team */
    if (!flag) {
      p = (SCHOOL_NODE * ) malloc(sizeof(SCHOOL_NODE));
      /*p -data member entry start */
      printf(" Enter the name of the person in charge of the department: ");
      scanf("%s", p -> leader);
      getchar();
      printf(" Enter contact number: ");
      scanf("%s", p -> contact_num);
      getchar();
      strcpy(p -> school_name, school_name);
      /*p- End of data member entry */
      p -> team_head = NULL;
      p -> next = sch_head;
      sch_head = p;
      q = (TEAM_NODE * ) malloc(sizeof(TEAM_NODE));
      /*q- data member input start */
      printf(" Enter the name of the team leader: ");
      scanf("%s", q -> leader);
      getchar();
      printf(" Enter the number of teachers: ");
      scanf("%d", & q -> teacher_num);
      getchar();
      printf(" Enter the number of graduate students: ");
      scanf("%d", & q -> stu_num);
      getchar();
      strcpy(q -> team_name, r -> team_name);
      strcpy(q -> school_name, school_name);
      /*q- End of data member entry */
      q -> proj_head = NULL;
      q -> next = p -> team_head;
      p -> team_head = q;
    } else {
      flag = 0;
      for (q = p -> team_head; q != NULL; q = q -> next) {
        /* If the team is found, exit and keep the current p position */
        if (!strcmp(q -> team_name, r -> team_name)) {
          flag = 1;
          break;
        }
      }
      /* If the project is attached to the new team, create a new team according to the entered team name */
      if (!flag) {
        q = (TEAM_NODE * ) malloc(sizeof(TEAM_NODE));
        /*q- data member input start */
        printf(" Enter the name of the team leader: ");
        scanf("%s", q -> leader);
        getchar();
        printf(" Enter the number of teachers: ");
        scanf("%d", & q -> teacher_num);
        getchar();
        printf(" Enter the number of graduate students: ");
        scanf("%d", & q -> stu_num);
        getchar();
        strcpy(q -> team_name, r -> team_name);
        strcpy(q -> school_name, p -> school_name);
        /*q- End of data member entry */
        q -> proj_head = NULL;
        q -> next = p -> team_head;
        p -> team_head = q;
      }
    }
    /*r- data member input start */
    printf("973 Program Project --1\t\tNational Natural Science Foundation Project --2\t\t863 Program Project --3\nInternational Cooperation Project --4\t\tHorizontal Project --5\nInput Item Category: ");
    r -> type = getchar();
    getchar();
    printf(" Enter the starting time (the year occupies four digits, if it is insufficient, fill it with leading 0 ): ");
    scanf("%s", r -> open_time);
    getchar();
    printf(" Enter project funds (unit: 10,000 yuan): ");
    scanf("%f", & r -> budget);
    getchar();
    printf(" Enter the name of the project leader: ");
    scanf("%s", r -> leader);
    getchar();
    /*r- End of data member entry */
    r -> next = q -> proj_head;
    q -> proj_head = r;
    printf(" Continue - Y returns to the main menu - any other key \n");
    j = getchar();
    getchar();
    FileOut();
    if (j != 'Y' && j != 'y') {
      break;
    }
  }
  ClearScreen();
  return TRUE;
}

/**
 * Function name : AltSchool
 * Function function : Modify a department information node in the cross-linked list .
 * Input parameters : None
 * Output parameters : None
 * back back Value : BOOL type - TRUE
 *
 * Call instructions :
 */
BOOL AltSchool(void) {
  while (1) {
    ClearScreen();
    fflush(stdin);
    char count = 0;
    do {
      if (count) {
        /* error message */
        printf(" The target information is not found! Continue - Y returns to the main menu - any other key \n");
        j = getchar();
        getchar();
        ClearScreen();
        if (j != 'y' && j != 'Y') {
          return TRUE;
        }
      }
      do {
        fflush(stdin);
        printf(" How do you want to query the information you want to modify? \nDepartment name --1\t\tThe person in charge of the department --2\t\tContact number --3\n");
        scanf("%hu", & choice);
        getchar();
      } while (choice < 1 || choice > 3);
      if (choice == 1) {
        printf(" Enter the department name: ");
      } else if (choice == 2) {
        printf(" Enter the person in charge of the department: ");
      } else {
        printf(" Enter contact information: ");
      }
      fflush(stdin);
      scanf("%s", seek);
      getchar();
      /* Find the corresponding node according to the given information */
      for (p = sch_head; p != NULL; p = p -> next) {
        if (choice == 1) {
          if (!strcmp(seek, p -> school_name)) break;
        } else if (choice == 2) {
          if (!strcmp(seek, p -> leader)) break;
        } else {
          if (!strcmp(seek, p -> contact_num)) break;
        }
      }
      count++;
    } while (p == NULL);
    do {
      /* Modify information */
      printf(" What information do you want to modify? \nDepartment name --1\t\tThe person in charge of the department --2\t\tContact number --3\t\tComplete --4\n");
      scanf("%hu", & choice);
      getchar();
      if (choice >= '1' && choice <= '3') {
        printf(" Please enter new information: ");
      }
      switch (choice) {
      case 1:
        printf(" Enter the new department name: ");
        scanf("%s", p -> school_name);
        getchar();
        break;
      case 2:
        printf(" Enter new head of department: ");
        scanf("%s", p -> leader);
        getchar();
        break;
      case 3:
        printf(" Enter new contact information: ");
        scanf("%s", p -> contact_num);
        getchar();
        break;
      }
      if (choice == 4) {
        break;
      }
    } while (1);
    printf(" Continue - Y returns to the main menu - any other key \n");
    fflush(stdin);
    j = getchar();
    getchar();
    if (j != 'Y' && j != 'y') {
      break;
    }
  }
  ClearScreen();
  FileOut();
  return TRUE;
}

/**
 * Function name : AltTeam
 * Function function : Modify a scientific research team information node in the cross-linked list .
 * Input parameters : None
 * Output parameters : None
 * back back Value : BOOL type - TRUE
 *
 * Call instructions :
 */
BOOL AltTeam(void) {
  while (1) {
    ClearScreen();
    fflush(stdin);
    char count = 0;
    do {
      if (count) {
        /* error message */
        printf(" The target information is not found! Continue - Y returns to the main menu - any other key \n");
        j = getchar();
        getchar();
        ClearScreen();
        if (j != 'y' && j != 'Y') {
          return TRUE;
        }
      }
      printf(" Enter team name: ");
      fflush(stdin);
      scanf("%s", seek);
      getchar();
      /* Search the target team according to the given information, record the position of q and exit the loop */
      for (p = sch_head; p != NULL; p = p -> next) {
        for (q = p -> team_head; q != NULL; q = q -> next) {
          j = strcmp(seek, q -> team_name);
          if (!j) break;
        }
        if (!j) break;
      }
      count++;
    } while (p == NULL);
    do {
      /* Modify information */
      printf(" What information do you want to modify? \nTeam name --1\tTeam leader --2\tNumber of teachers --3\tNumber of graduate students --4\tComplete --5\n");
      /* How to modify the department? */
      scanf("%hu", & choice);
      getchar();
      if (choice >= '1' && choice <= '4') {
        printf(" Please enter new information: ");
      }
      switch (choice) {
      case 1:
        printf(" Enter the new team name: ");
        scanf("%s", q -> team_name);
        getchar();
        break;
      case 2:
        printf(" Enter the new team leader: ");
        scanf("%s", q -> leader);
        getchar();
        break;
      case 3:
        printf(" Enter the number of new teachers: ");
        scanf("%d", & q -> teacher_num);
        getchar();
        break;
      case 4:
        printf(" Enter the number of new graduate students: ");
        scanf("%d", & q -> stu_num);
        getchar();
        break;
      }
      if (choice == 5) {
        break;
      }
    } while (1);
    printf(" Continue - Y returns to the main menu - any other key \n");
    fflush(stdin);
    j = getchar();
    getchar();
    if (j != 'Y' && j != 'y') {
      break;
    }
  }
  ClearScreen();
  FileOut();
  return TRUE;
}

/**
 * Function name : AltProject
 * Function function : Modify a scientific research team information node in the cross-linked list .
 * Input parameters : none
 * Output parameters : None
 * back back Value : BOOL type - TRUE
 *
 * Call instructions :
 */
BOOL AltProject(void) {
  while (1) {
    ClearScreen();
    fflush(stdin);
    char count = 0;
    do {
      if (count) {
        /* error message */
        printf(" The target information is not found! Continue - Y returns to the main menu - any other key \n");
        j = getchar();
        getchar();
        ClearScreen();
        if (j != 'y' && j != 'Y') {
          return TRUE;
        }
      }
      printf(" Enter item number: ");
      fflush(stdin);
      scanf("%s", seek);
      getchar();
      /* Search the target item according to the given information, record the position of r and exit the loop */
      for (p = sch_head; p != NULL; p = p -> next) {
        for (q = p -> team_head; q != NULL; q = q -> next) {
          for (r = q -> proj_head; r != NULL; r = r -> next) {
            j = strcmp(seek, r -> project_num);
            if (!j) break;
          }
          if (!j) break;
        }
        if (!j) break;
      }
      count++;
    } while (p == NULL);
    do {
      /* Modify information */
      printf(" What information do you want to modify? \nProject number -- 1 \tProject category --2\tStart time --3\nProject funding --4\tProject leader --5\tComplete- -6\n"); /* How to modify the team? */
      scanf("%hu", & choice);
      getchar();
      if (choice >= '1' && choice <= '5') {
        printf(" Please enter new information: ");
      }
      switch (choice) {
      case 1:
        printf(" Enter new project number: ");
        scanf("%s", r -> project_num);
        getchar();
        break;
      case 2:
        printf(" Enter new item category: ");
        scanf("%c", & r -> type);
        getchar();
        break;
      case 3:
        printf(" Enter new start time: ");
        scanf("%s", r -> open_time);
        getchar();
        break;
      case 4:
        printf(" Enter new project budget: ");
        scanf("%f", & r -> budget);
        getchar();
        break;
      case 5:
        printf(" Enter the new project leader: ");
        scanf("%s", r -> leader);
        getchar();
        break;
      }
      if (choice == 6) {
        break;
      }
    } while (1);
    printf(" Continue - Y returns to the main menu - any other key \n");
    fflush(stdin);
    j = getchar();
    getchar();
    if (j != 'Y' && j != 'y') {
      break;
    }
  }
  ClearScreen();
  FileOut();
  return TRUE;
}

/**
 * Function name : DeleteSchool
 * Function function : delete a department information node in the cross list .
 * Input parameters : none
 * Output parameters : None
 * back back Value : BOOL type - TRUE
 *
 * Call instructions :
 */
BOOL DeleteSchool(void) {
  while (1) {
    ClearScreen();
    char count = 0;
    do {
      p1 = sch_head;
      if (count) {
        /* error message */
        printf(" The target information is not found! Continue - Y returns to the main menu - any other key \n");
        j = getchar();
        getchar();
        ClearScreen();
        if (j != 'y' && j != 'Y') {
          return TRUE;
        }
      }
      do {
        printf("\nNote : When deleting the data of the department, the data of all the teams and projects under the department will be deleted! \n");
        printf(" How do you want to query the information you want to modify? \nDepartment name --1\t\tThe person in charge of the department --2\t\tContact number --3\n");
        scanf("%hu", & choice);
        getchar();
      } while (choice < 1 || choice > 3);
      if (choice == 1) {
        printf(" Enter the department name: ");
      } else if (choice == 2) {
        printf(" Enter the person in charge of the department: ");
      } else {
        printf(" Enter contact information: ");
      }
      fflush(stdin);
      scanf("%s", seek);
      getchar();
      /* Find the corresponding department according to the given information */
      while (p1) {
        if (choice == 1) {
          if (!strcmp(seek, p1 -> school_name)) break;
        } else if (choice == 2) {
          if (!strcmp(seek, p1 -> leader)) break;
        } else {
          if (!strcmp(seek, p1 -> contact_num)) break;
        }
        p2 = p1;
        p1 = p1 -> next;
      }
      count++;
    } while (p1 == NULL);
    if (!p1 -> team_head); /* There is no team under the department */
    else {
      /* There are teams under the department */
      /* delete all team nodes - start */
      q = p1 -> team_head -> next;
      while (q != NULL) {
        if (!q -> proj_head); /* There is no project under the team */
        else {
          /* There are projects under the team */
          /* delete all item nodes - start */
          r = q -> proj_head -> next;
          while (r != NULL) {
            q -> proj_head -> next = r -> next;
            free(r);
            r = q -> proj_head -> next;
          }
          r = q -> proj_head;
          q -> proj_head = NULL;
          free(r);
        }
        /* delete all item nodes - end */
        p1 -> team_head -> next = q -> next;
        free(q);
        q = p1 -> team_head -> next;
      }
      q = p1 -> team_head;
      /* delete all item nodes - start */
      r = q -> proj_head -> next;
      while (r != NULL) {
        q -> proj_head -> next = r -> next;
        free(r);
        r = q -> proj_head -> next;
      }
      r = q -> proj_head;
      q -> proj_head = NULL;
      free(r);
      /* delete all item nodes - end */
      p1 -> team_head = NULL;
      free(q);
      /* delete all team nodes - end */
    }
    if (p1 == sch_head) {
      sch_head = p1 -> next;
    } else {
      p2 -> next = p1 -> next;
    }
    free(p1); /* delete department node */
    printf(" Delete successfully! \nContinue - Y to return to the main menu - any other key \n");
    fflush(stdin);
    j = getchar();
    getchar();
    if (j != 'Y' && j != 'y') {
      break;
    }
  }
  ClearScreen();
  FileOut();
  return TRUE;
}

/**
 * Function name : DeleteTeam
 * Function function : Delete a scientific research team information node in the cross-linked list .
 * Input parameters : none
 * Output parameters : None
 * back back Value : BOOL type - TRUE
 *
 * Call instructions :
 */
BOOL DeleteTeam(void) {
  while (1) {
    ClearScreen();
    printf("\nNote: When deleting team data, the data of all projects under the team will be deleted! \n");
    char count = 0;
    do {
      if (count) {
        /* error message */
        printf(" The target information is not found! Continue - Y returns to the main menu - any other key \n");
        j = getchar();
        getchar();
        ClearScreen();
        if (j != 'y' && j != 'Y') {
          return TRUE;
        }
      }
      printf(" Enter team name: ");
      fflush(stdin);
      scanf("%s", seek);
      getchar();
      /* Find the corresponding team node according to the given information */
      for (p = sch_head; p != NULL; p = p -> next) {
        q1 = p -> team_head;
        while (q1 != NULL) {
          j = strcmp(seek, q1 -> team_name);
          if (!j) {
            break;
          }
          q2 = q1;
          q1 = q1 -> next;
        }
        if (!j) {
          break;
        }
      }
      count++;
    } while (p == NULL);
    if (!q1 -> proj_head); /* There is no project under the team */
    else {
      /* There are projects under the team */
      /* delete all team nodes - start */
      r = q1 -> proj_head -> next;
      while (r != NULL) {
        q1 -> proj_head -> next = r -> next;
        free(r);
        r = q1 -> proj_head -> next;
      }
      r = q1 -> proj_head;
      q1 -> proj_head = NULL;
      free(r);
      /* delete all team nodes - end */
    }
    if (q1 == p -> team_head) {
      p -> team_head = q1 -> next;
    } else {
      q2 -> next = q1 -> next;
    }
    free(q1); /* delete team node */
    /* The team under the department is forced to quit when it is empty, to prevent entering an endless loop due to human operation */
    if (p -> team_head == NULL) {
      printf(" There is no team under this college! \n");
      printf(" Press any key to continue ...\n");
      getch();
      ClearScreen();
      return TRUE;
    }
    printf(" Delete successfully! \nContinue - Y to return to the main menu - any other key \n");
    fflush(stdin);
    j = getchar();
    getchar();
    if (j != 'Y' && j != 'y') {
      break;
    }
  }
  ClearScreen();
  FileOut();
  return TRUE;
}

/**
 * Function name : DeleteProject
 * Function function : Delete a scientific research project information node in the cross-linked list .
 * Input parameters : none
 * Output parameters : None
 * back back Value : BOOL type - TRUE
 *
 * Call instructions :
 */
BOOL DeleteProject(void) {
  do {
    ClearScreen();
    char count = 0;
    do {
      if (count) {
        /* error message */
        printf(" The target information is not found! Continue - Y returns to the main menu - any other key \n");
        j = getchar();
        getchar();
        ClearScreen();
        if (j != 'y' && j != 'Y') {
          return TRUE;
        }
      }
      printf(" Enter item number: ");
      fflush(stdin);
      scanf("%s", seek);
      getchar();
      /* Find the corresponding item node according to the given information */
      for (p = sch_head; p != NULL; p = p -> next) {
        for (q = p -> team_head; q != NULL; q = q -> next) {
          r1 = q -> proj_head;
          while (r1 != NULL) {
            j = strcmp(seek, r1 -> project_num);
            if (!j) {
              break;
            }
            r2 = r1;
            r1 = r1 -> next;
          }
          if (!j) {
            break;
          }
        }
        if (!j) {
          break;
        }
      }
      count++;
    } while (p == NULL);
    if (r1 == q -> proj_head) {
      q -> proj_head = r1 -> next;
    } else {
      r2 -> next = r1 -> next;
    }
    free(r1); /* delete item node */
    /* Forcibly exit when the project under the team is empty, to prevent entering an infinite loop due to human operation */
    if (q -> proj_head == NULL) {
      printf(" There is no project under this team! \n");
      printf(" Press any key to continue ...\n");
      getch();
      ClearScreen();
      return TRUE;
    }
    printf(" Delete successfully! \nContinue - Y to return to the main menu - any other key \n");
    fflush(stdin);
    j = getchar();
  } while (j == 'Y' || j == 'y');
  ClearScreen();
  FileOut();
  return TRUE;
}

/**
 * Function name : SeekSchool
 * Function function : Given the requirements, find all the information nodes of the colleges and departments that meet the requirements in the cross-linked list .
 * Input parameters : none
 * Output parameters : None
 * back back Value : BOOL type - TRUE
 *
 * Call instructions :
 */
BOOL SeekSchool(void) {
  while (1) {
    char count = 0, flag = 0, judge;
    ClearScreen();
    do {
      judge = 1;
      if (count) {
        /* error message */
        printf(" The target information is not found! Continue - Y returns to the main menu - any other key \n");
        j = getchar();
        getchar();
        ClearScreen();
        if (j != 'y' && j != 'Y') {
          return TRUE;
        }
      }
      printf(" How do you want to inquire? \nThe person in charge of the department %6c--press1\nThe name of the department %6c--press2\n", ' ', ' ');
      k = getchar();
      getchar();
      if (k == '1') {
        printf(" Head of department :");
      } else {
        printf(" Department name substring :");
      }
      fflush(stdin);
      scanf("%s", seek);
      getchar();
      printf(" Department name %16cThe name of the person in charge of the department %4cContact number \n", ' ', ' ');
      for (p = sch_head; p != NULL; p = p -> next) {
        if (k == '1') {
          /* Query by department head */
          if (strstr(p -> leader, seek)) {
            printf("%-24s%-18s%s\n", p -> school_name, p -> leader, p -> contact_num);
            flag = 1;
          }
        } else {
          /* Search by department name keywords */
          if (strstr(p -> school_name, seek)) {
            printf("%-24s%-18s%s\n", p -> school_name, p -> leader, p -> contact_num);
            flag = 1;
          }
        }
      }
      if (!judge) {
        break;
      }
      count++;
    } while (p1 == NULL && !flag);
    printf(" Continue - Y returns to the main menu - any other key \n");
    fflush(stdin);
    j = getchar();
    getchar();
    if (j != 'y' && j != 'Y') {
      ClearScreen();
      return TRUE;
    }
  }
  ClearScreen();
  return TRUE;
}

/**
 * Function name : SeekTeam
 * Function function : Given the requirements, find all the scientific research team information nodes that meet the requirements in the cross-linked list .
 * Input parameters : none
 * Output parameters : None
 * back back Value : BOOL type - TRUE
 *
 * Call instructions :
 */
BOOL SeekTeam(void) {
  unsigned short low, high;
  while (1) {
    char count = 0, flag = 0;
    ClearScreen();
    do {
      /* error message */
      if (count) {
        /* error message */
        printf(" The target information is not found! Continue - Y returns to the main menu - any other key \n");
        j = getchar();
        getchar();
        ClearScreen();
        if (j != 'y' && j != 'Y') {
          return TRUE;
        }
      }
      printf(" How do you want to query? \nTeam name %6c--press1\nNumber of teachers %6c--press2\n", ' ', ' ');
      k = getchar();
      getchar();
      if (k == '1') {
        printf(" Team name substring :");
        fflush(stdin);
        scanf("%s", seek);
        getchar();
      } else {
        printf(" Minimum number of teachers :");
        scanf("%hu", & low);
        getchar();
        printf(" The upper limit of the number of teachers :");
        scanf("%hu", & high);
        getchar();
      }
      printf(" Team name %14c Team leader's name %2c Faculty and department %16c Number of teachers %2c Number of graduate students \n", ' ', ' ', ' ', ' ');
      for (p = sch_head; p != NULL; p = p -> next) {
        for (q = p -> team_head; q != NULL; q = q -> next) {
          if (k == '1') {
            /* Query by team name keyword */
            if (strstr(q -> team_name, seek)) {
              printf("%-22s%-16s%-24s%-10d%d\n", q -> team_name, q -> leader, q -> school_name, q -> teacher_num, q -> stu_num);
              flag = 1;
            }
          } else {
            /* Query by the number of teachers */
            if (q -> teacher_num >= low && q -> teacher_num <= high) {
              printf("%-22s%-16s%-24s%-10d%d\n", q -> team_name, q -> leader, q -> school_name, q -> teacher_num, q -> stu_num);
              flag = 1;
            }
          }
        }
      }
      count++;
    } while (p == NULL && !flag);
    printf(" Continue - Y returns to the main menu - any other key \n");
    j = getchar();
    getchar();
    if (j != 'Y' && j != 'y') {
      break;
    }
  }
  ClearScreen();
  return TRUE;
}

/**
 * Function name : SeekProject
 * Function function : Given the requirements, find all the scientific research team information nodes that meet the requirements in the cross-linked list .
 * Input parameters : none
 * Output parameters : None
 * back back Value : BOOL type - TRUE
 *
 * Call instructions :
 */
BOOL SeekProject(void) {
  while (1) {
    char count = 0, flag = 0;
    ClearScreen();
    do {
      /* error message */
      if (count) {
        /* error message */
        printf(" The target information is not found! Continue - Y returns to the main menu - any other key \n");
        j = getchar();
        getchar();
        ClearScreen();
        if (j != 'y' && j != 'Y') {
          return TRUE;
        }
      }
      printf(" Which method do you want to query? \nProject number %6c--press1\nTeam %6c--press2\n", ' ', ' ');
      k = getchar();
      getchar();
      if (k == '1') {
        printf(" Item number :");
      } else {
        printf(" The team the project belongs to :");
      }
      fflush(stdin);
      scanf("%s", seek);
      getchar();
      printf(" Project number %9c start time %4c project funding %6c project leader %5c team %10c project category \n", ' ', ' ', ' ', ' ', ' ');
      for (p = sch_head; p != NULL; p = p -> next) {
        for (q = p -> team_head; q != NULL; q = q -> next) {
          if (k == '1') {
            /* Query by item number */
            for (r = q -> proj_head; r != NULL; r = r -> next) {
              if (!strcmp(r -> project_num, seek)) {
                printf("%-17s%-12s%-27f million %-15s%-18s", r -> project_num, r -> open_time, r -> budget, r -> leader, r -> team_name);
                switch (r -> type) {
                case '1':
                  printf("973 project \n");
                  break;
                case '2':
                  printf(" National Natural Science Foundation of China \n");
                  break;
                case '3':
                  printf("863 project \n");
                  break;
                case '4':
                  printf(" International Cooperation Project \n");
                  break;
                case '5':
                  printf(" horizontal item \n");
                  break;
                }
                flag = 1;
              }
            }
          } else {
            /* Query by team */
            if (!strcmp(q -> team_name, seek)) {
              for (r = q -> proj_head; r != NULL; r = r -> next) {
                printf("%-17s%-12s%-27f million %-15s%-18s", r -> project_num, r -> open_time, r -> budget, r -> leader, r -> team_name);
                switch (r -> type) {
                case '1':
                  printf("973 project \n");
                  break;
                case '2':
                  printf(" National Natural Science Foundation of China \n");
                  break;
                case '3':
                  printf("863 project \n");
                  break;
                case '4':
                  printf(" International Cooperation Project \n");
                  break;
                case '5':
                  printf(" horizontal item \n");
                  break;
                }
                flag = 1;
              }
            }
          }
        }
      }
      count++;
    } while (p == NULL && !flag);
    printf(" Continue - Y returns to the main menu - any other key \n");
    j = getchar();
    getchar();
    if (j != 'Y' && j != 'y') {
      break;
    }
  }
  ClearScreen();
  return TRUE;
}

/**
 * Function name : TeachStuProp
 * Function function : count the total number of teachers in each department, the total number of graduate students, and the ratio of graduate students to teachers (reserve 2 decimal places),
 * After sorting in descending order according to the ratio of the number of students to teachers, output the statistical results .
 * Input parameters : none
 * Output parameters : None
 * back back Value : BOOL type - TRUE
 *
 * Call instructions :
 */
BOOL TeachStuProp(void) {
  /* Sum the number of teachers and graduate students in each department and calculate the ratio */
  for (p = sch_head; p != NULL; p = p -> next) {
    p -> sum_teacher = p -> sum_student = p -> proportion = 0;
    for (q = p -> team_head; q != NULL; q = q -> next) {
      p -> sum_teacher += q -> teacher_num;
      p -> sum_student += q -> stu_num;
    }
    p -> proportion = p -> sum_student * 1.0 / p -> sum_teacher;
    if (p -> sum_teacher == 0 || p -> sum_student == 0) {
      p -> proportion = 0;
    }
  }
  /* Sort in descending order by the method of exchanging pointer fields by ratio */
  p1 = (SCHOOL_NODE * ) malloc(sizeof(SCHOOL_NODE));
  p1 -> next = sch_head;;
  sch_head = prior1 = p1;
  for (p1 = prior1 -> next; p1 -> next != NULL; prior1 = p1, p1 = p1 -> next) {
    for (p2 = p1 -> next, prior2 = p1; p2 != NULL; prior2 = p2, p2 = p2 -> next) {
      if (p1 -> proportion < p2 -> proportion) {
        t = p2 -> next;
        prior1 -> next = p2;
        prior2 -> next = p1;
        p2 -> next = p1 -> next;
        p1 -> next = t;
        t = p1;
        p1 = p2;
        p2 = t;
      }
    }
  }
  p1 = sch_head;
  sch_head = sch_head -> next;
  free(p1);
  /* print sorting result */
  printf("%8c departments %16c number of graduate students %3c number of teachers %3c ratio of graduate teachers to teachers \n", b, b, b, b);
  for (p = sch_head; p != NULL; p = p -> next) {
    if (p -> proportion == 0) {
      continue;
    }
    printf("%-20s%14d%12d%16.2f\n", p -> school_name, (int) p -> sum_student, (int) p -> sum_teacher, p -> proportion);
  }
  printf(" Press any key to continue ...\n");
  getch();
  ClearScreen();
  return TRUE;
}

/**
 * Function name : ProjectTotal
 * Function function : count the number of scientific research projects of each department in a certain year, the number of 973 projects, the number of 863 projects,
 * and the total scientific research funds, and the number of scientific research projects are sorted in descending order and output .
 * Input parameters : none
 * Output parameters : None
 * back back Value : BOOL type - TRUE
 *
 * Call instructions :
 */
BOOL ProjectTotal(void) {
  do {
    unsigned short year, ori;
    ClearScreen();
    flag = 0;
    printf(" Please enter the starting year :");
    scanf("%hu", & year);
    getchar();
    /* For projects that meet the requirements of the year, count the total number of the two types of projects and sum the funds */
    for (p = sch_head; p != NULL; p = p -> next) {
      p -> num_proj = p -> num_863 = p -> num_973 = p -> budget = 0;
      for (q = p -> team_head; q != NULL; q = q -> next) {
        for (r = q -> proj_head; r != NULL; r = r -> next) {
          ori = (r -> open_time[0] - '0') * 1000 + (r -> open_time[1] - '0') * 100 + (r -> open_time[2] - '0') * 10 + (r -> open_time[3] - '0');
          if (ori <= year) {
            flag = 1;
            p -> num_proj++;
            p -> budget += r -> budget;
            if (r -> type == '1') {
              p -> num_973++;
            } else if (r -> type == '3') {
              p -> num_863++;
            }
          }
        }
      }
    }
    if (flag) {
      p1 = (SCHOOL_NODE * ) malloc(sizeof(SCHOOL_NODE));
      p1 -> next = sch_head;
      /* Sort in descending order by the total number of items */
      sch_head = prior1 = p1;
      for (p1 = prior1 -> next; p1 -> next != NULL; prior1 = p1, p1 = p1 -> next) {
        for (p2 = p1 -> next, prior2 = p1; p2 != NULL; prior2 = p2, p2 = p2 -> next) {
          if (p1 -> num_proj < p2 -> num_proj) {
            t = p2 -> next;
            prior1 -> next = p2;
            prior2 -> next = p1;
            p2 -> next = p1 -> next;
            p1 -> next = t;
            t = p1;
            p1 = p2;
            p2 = t;
          }
        }
      }
      p1 = sch_head;
      sch_head = sch_head -> next;
      free(p1);
      /* print sorting result */
      printf("%hu year \n%8c departments %15c total number of projects %3c973 total number of projects %3c863 total number of projects %3c total scientific research funds \n", year, b, b, b, b, b);
      for (p = sch_head; p != NULL; p = p -> next) {
        if (p -> num_proj == 0) {
          continue;
        }
        printf("%-20s%12u%13u%14u%18f\n", p -> school_name, p -> num_proj, p -> num_973, p -> num_863, p -> budget);
      }
    } else {
      printf(" There are no projects started this year! \n");
    }
    printf(" Continue - Y returns to the main menu - any other key \n");
    j = getchar();
    getchar();
  } while (j == 'Y' || j == 'y');
  ClearScreen();
  return TRUE;
}

/**
 * Function name : TeamOfMostNSFC
 * Function function : count the 10 scientific research teams with the largest number of scientific research projects of the National Natural Science Foundation of China over the years, sort by the number of projects in descending order and output the name of the scientific research team,
 * The category is the number of scientific research projects of the National Natural Science Foundation of China and the total amount of project funding .
 * Input parameters : none
 * Output parameters : none
 * back back Value : BOOL type - TRUE
 *
 * Call instructions :
 */
BOOL TeamOfMostNSFC(void) {
  unsigned char min = 255, count = 0;
  /* For the specified type of project, count the total and sum the funds */
  for (p = sch_head; p != NULL; p = p -> next) {
    for (q = p -> team_head; q != NULL; q = q -> next) {
      q -> projsci_num = q -> proj_budget = 0;
      for (r = q -> proj_head; r != NULL; r = r -> next) {
        if (r -> type == '2') {
          q -> projsci_num++;
          q -> proj_budget += r -> budget;
        }
      }
    }
  }
  /* Copy the names and project numbers of all teams to the new linked list for unified sorting */
  q1 = (TEAM_NODE * ) malloc(sizeof(TEAM_NODE));
  head = q1;
  for (p = sch_head; p != NULL; p = p -> next) {
    for (q = p -> team_head; q != NULL; q = q -> next) {
      q1 -> next = (TEAM_NODE * ) malloc(sizeof(TEAM_NODE));
      q1 = q1 -> next;
      q1 -> projsci_num = q -> projsci_num;
      strcpy(q1 -> team_name, q -> team_name);
      q1 -> proj_budget = q -> proj_budget;
      q1 -> next = NULL;
    }
  }
  q1 -> next = NULL;
  /* Sort in descending order by the total number of items */
  qrior1 = head;
  for (q1 = qrior1 -> next; q1 -> next != NULL; qrior1 = q1, q1 = q1 -> next) {
    for (q2 = q1 -> next, qrior2 = q1; q2 != NULL; qrior2 = q2, q2 = q2 -> next) {
      if (q1 -> projsci_num < q2 -> projsci_num) {
        u = q2 -> next;
        qrior1 -> next = q2;
        qrior2 -> next = q1;
        q2 -> next = q1 -> next;
        q1 -> next = u;
        u = q1;
        q1 = q2;
        q2 = u;
      }
    }
  }
  q1 = head;
  head = head -> next;
  free(q1);
  /* Print the sorting results, taking into account the juxtaposition here, and appropriately increase the number of output data groups */
  printf("%6c team name %6c number of NSFC projects %4c total funding of such projects \n", b, b, b);
  for (u = head; u != NULL; u = u -> next, count++) {
    if (count >= 10 && u -> projsci_num < min) {
      break;
    }
    min = u -> projsci_num;
    printf("%-31s%-15u%f\n", u -> team_name, u -> projsci_num, u -> proj_budget);
  }
  q1 = head;
  for (q2 = q1 -> next; q2 != NULL; q1 = q2, q2 = q1 -> next)
    free(q1);
  printf(" Press any key to continue ...\n");
  getch();
  ClearScreen();
  return TRUE;
}

/**
 * Function name : ProjTeachProp
 * Function function : count the 10 scientific research teams with the largest number of scientific research projects classified as the National Natural Science Foundation of China over the years , and output the name of the scientific research team after sorting in descending order of the number of projects.
 * The category is the number of scientific research projects of the National Natural Science Foundation of China and the total amount of project funding .
 * Input parameters : none
 * Output parameters : none
 * back back Value : BOOL type - TRUE
 *
 * Call instructions :
 */
BOOL ProjTeachProp(void) {
  unsigned char count = 0;
  float min = 1.0e6;
  /* Statistical ratio of the number of scientific research projects to the number of teachers */
  for (p = sch_head; p != NULL; p = p -> next) {
    for (q = p -> team_head; q != NULL; q = q -> next) {
      q -> proj_num = 0;
      for (r = q -> proj_head; r != NULL; r = r -> next) {
        q -> proj_num++;
      }
      q -> proportion = q -> proj_num * 1.0 / q -> teacher_num;
    }
  }
  /* Create a linked list of scientific research teams and copy the content from the original table to the new table */
  q1 = (TEAM_NODE * ) malloc(sizeof(TEAM_NODE));
  head = q1;
  for (p = sch_head; p != NULL; p = p -> next) {
    for (q = p -> team_head; q != NULL; q = q -> next) {
      q1 -> next = (TEAM_NODE * ) malloc(sizeof(TEAM_NODE));
      q1 = q1 -> next;
      strcpy(q1 -> team_name, q -> team_name);
      q1 -> teacher_num = q -> teacher_num;
      q1 -> proj_num = q -> proj_num;
      q1 -> proportion = q -> proportion;
    }
  }
  q1 -> next = NULL;
  /* Sort in descending order of the ratio of the number of items to the number of teachers */
  qrior1 = head;
  for (q1 = qrior1 -> next; q1 -> next != NULL; qrior1 = q1, q1 = q1 -> next) {
    for (q2 = q1 -> next, qrior2 = q1; q2 != NULL; qrior2 = q2, q2 = q2 -> next) {
      if (q1 -> proportion < q2 -> proportion) {
        u = q2 -> next;
        qrior1 -> next = q2;
        qrior2 -> next = q1;
        q2 -> next = q1 -> next;
        q1 -> next = u;
        u = q1;
        q1 = q2;
        q2 = u;
      }
    }
  }
  q1 = head;
  head = head -> next;
  free(q1);
  /* Print the sorting results, taking into account the juxtaposition here, and appropriately increase the number of output data groups */
  printf("%6c team name %6c the number of teachers %3c the total number of projects %3c the ratio of the number of projects to the number of teachers \n", b, b, b, b);
  for (u = head; u != NULL; u = u -> next, count++) {
    if (count >= 5 && u -> proportion < min) {
      break;
    }
    min = u -> proportion;
    printf("%-23s%-11d%-16u%.2f\n", u -> team_name, u -> teacher_num, u -> proj_num, u -> proportion);
  }
  /* Delete the new linked list */
  q1 = head;
  for (q2 = q1 -> next; q2 != NULL; q1 = q2, q2 = q1 -> next)
    free(q1);
  printf(" Press any key to continue ...\n");
  getch();
  ClearScreen();
  return TRUE;
}

/**
 * Function name : LeaderOfProj
 * Function function : count the number of projects in charge of the person in charge in a specific department,
 * After sorting in descending order, output the name of the person in charge and the number of projects in charge
 * Input parameters : none
 * Output parameters : none
 * back back Value : BOOL type - TRUE
 *
 * Call instructions :
 */
BOOL LeaderOfProj(void) {
  LEADER_NODE * head, * t, * prior1, * prior2, * p1, * p2;
  unsigned short count, save;
  do {
    count = 1;
    head = NULL;
    /* selected departments */
    do {
      if (count - 1) {
        /* error message */
        printf(" The target information is not found! Continue - Y returns to the main menu - any other key \n");
        j = getchar();
        getchar();
        ClearScreen();
        if (j != 'y' && j != 'Y') {
          return TRUE;
        }
      }
      printf(" Department name: ");
      scanf("%s", seek);
      getchar();
      for (p = sch_head; p != NULL; p = p -> next) {
        if (!strcmp(p -> school_name, seek)) {
          break;
        }
      }
      count++;
    } while (!p);
    if (!p -> team_head) {
      printf(" There is no project under this department! \n");
    } else {
      /* Create a dedicated linked list for the person in charge, and traverse the original table to count the number of projects undertaken by each person in charge */
      for (q = p -> team_head; q != NULL; q = q -> next) {
        for (r = q -> proj_head; r != NULL; r = r -> next) {
          for (p1 = head; p1 != NULL; p1 = p1 -> next) {
            if (!strcmp(r -> leader, p1 -> leader)) {
              break;
            }
          }
          if (p1) {
            p1 -> n++;
          } else {
            t = (LEADER_NODE * ) malloc(sizeof(LEADER_NODE));
            strcpy(t -> leader, r -> leader);
            t -> n = 1;
            t -> next = head;
            head = t;
          }
        }
      }
      /* Sort in descending order by the ratio of the number of projects to the number of teachers */
      prior1 = head = (LEADER_NODE * ) malloc(sizeof(LEADER_NODE));
      head -> n = 0;
      head -> next = t;
      for (p1 = prior1 -> next; p1 -> next != NULL; prior1 = p1, p1 = p1 -> next) {
        for (p2 = p1 -> next, prior2 = p1; p2 != NULL; prior2 = p2, p2 = p2 -> next) {
          if (p1 -> n < p2 -> n) {
            t = p2 -> next;
            prior1 -> next = p2;
            prior2 -> next = p1;
            p2 -> next = p1 -> next;
            p1 -> next = t;
            t = p1;
            p1 = p2;
            p2 = t;
          }
        }
      }
      /* Print the sorting results, taking the juxtaposition into consideration here */
      printf(" Ranking \tName of the person in charge \tResponsible for the number of projects \n");
      count = 1;
      save = count;
      for (prior1 = head, p1 = head -> next; p1 != NULL; prior1 = prior1 -> next, p1 = prior1 -> next, count++) {
        if (p1 -> n == prior1 -> n) {
          printf("%2hu", save);
        } else {
          save = count;
          printf("%2hu", count);
        }
        printf("%13s%15hu\n", p1 -> leader, p1 -> n);
      }
      /* Release the dynamic storage area of the payment information branch chain */
      p1 = head;
      while (p1 != NULL) {
        p2 = p1 -> next;
        free(p1);
        p1 = p2;
      }
    }
    printf(" Continue - Y returns to the main menu - any other key \n");
    j = getchar();
    getchar();
    ClearScreen();
  } while (j == 'Y' || j == 'y');
  return TRUE;
}

BOOL HelpTopic(void) {
  ShellExecuteA(NULL, "open", "NOTEPAD.EXE", "help.txt", NULL, SW_SHOWNORMAL);
  printf(" Press any key to continue ...\n");
  getch();
  ClearScreen();
  return TRUE;
}

BOOL AboutDorm(void) {
  char * plabel_name[] = {
    " System version: V1.0",
    "**************",
    "All Rights Reserved",
    " Confirm "
  };
  ShowModule(plabel_name, 4);
  return TRUE;
}
