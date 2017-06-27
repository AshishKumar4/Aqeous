#ifndef WINDOW_H
#define WINDOW_H

#include "graphics.h"
#include "common.h"
#include "tasking.h"

#define WIN_DRAGABLE    0b00000000000000000000000000000010
#define WIN_SIZABLE     0b00000000000000000000000000000100
#define WIN_SELECTABLE  0b00000000000000000000000000001000

typedef struct window_locInfo
{
  int o_x;    //Center Position X
  int o_y;    //Center Position Y
  int x_l;    //Length
  int y_l;    //Width
}window_locInfo_t;

typedef struct window_object
{
  uint32_t exec_id;
  uint32_t z_index;
  window_locInfo_t local_locInfo;
  uint32_t bg_color;
  struct window_object* next;
  struct window_object* back;
  struct window_object* childs;
}window_object_t;

typedef struct window
{
  window_locInfo_t locInfo;
  Process_t* proc;
  uint32_t z_index;
  uintptr_t screen_buff;
  int childs;
  int id;
  window_object_t* obj_list;
  window_object_t* last_obj;
  window_object_t* curr_obj;
  uint32_t bg_color;
  char name[64];
}window_t;

window_t* bgnd;

typedef struct window_list
{
  window_t* window;
  struct window_list* next;
  struct window_list* back;
}window_list_t;

typedef struct dirtyRectangle
{
  int o_x;
  int o_y;
  int e_x;
  int e_y;
  //struct dirtyRectangle_t* next;
}dirtyRectangle_t;

dirtyRectangle_t  dirtyRects[2048];

func_t window_execs[1024];

int dirtyRegions = 0;

window_list_t main_wlist, *wlist_end = NULL;

void WindowCompiler(uint32_t* buff, window_t* window, dirtyRectangle_t* dr);

void WindowComposter(uint32_t* buff, window_list_t* list);

void WindowDrag(window_t* w, int n_x, int n_y);

void WindowDrag_relative(window_t* w, int d_x, int d_y);

void UpdateScreen(int o_x, int o_y, int e_x, int e_y);

int pointIn_LocInfo(window_locInfo_t* info, int x, int y);

window_t* getTopWindow(int x, int y);

window_object_t* getWindowClickedObject(window_t* w, int x, int y);

void BringWindowToFront(window_t* w, window_list_t* wlist);

void Mouse_LeftClick_Handler(int x, int y);

void createWindowObjectRects_relative(window_t* w, int c_x, int c_y, int l_x, int l_y, int z_index, int r_x, int r_y, window_object_t* parent_obj, uint32_t color, uint32_t exec_opt);

window_t* create_window_basic(char* window_name, Process_t* proc, window_t* parentw, int center_x, int center_y, int length_x, int length_y, int z_index);
window_t* create_window_partial(char* window_name, Process_t* proc, window_t* parentw, int center_x, int center_y, int length_x, int length_y, int z_index);
window_t* create_window_autoMap(char* window_name, Process_t* proc, window_t* parentw, int center_x, int center_y, int length_x, int length_y, int z_index);

void createDefaultWindowObjects(window_t* w);

void WindowListAdd(window_t* w);

#endif
