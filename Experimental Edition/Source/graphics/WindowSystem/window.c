#include "window.h"
#include "graphics.h"
#include "process.h"
#include "tasking.h"
#include "common.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

void WindowCompiler(uint32_t* buff, window_t* window, dirtyRectangle_t* dr)
{
  window_locInfo_t* inf = &window->locInfo;
  uint32_t* wbuff = window->screen_buff;
  uint32_t alpha = 0;
  for(int bi = MAX(inf->o_y, dr->o_y), wi = bi - inf->o_y; bi < MIN(inf->o_y + inf->y_l, dr->e_y); wi++, bi++)
  {
    for(int bj = MAX(inf->o_x, dr->o_x), wj = wi*inf->x_l + (bj - inf->o_x), ba = bi*widthVESA + bj; bj < MIN(inf->o_x + inf->x_l, dr->e_x); wj++, bj++, ba++)
    {
        alpha = ((wbuff[wj] & 0xff000000));
        alpha |= (alpha>>8) | (alpha>>16) | (alpha>>24);
        buff[ba] = ((wbuff[wj]&(alpha)) | (buff[ba]&(0xffffffff - alpha)));
    }
  }
}

void WindowComposter(uint32_t* buff, window_list_t* list)
{
  dirtyRectangle_t* tmp = dirtyRects;
  int i = 1;
  window_list_t* tl;
  for(; dirtyRegions; dirtyRegions--, i++)
  {
    tl = list;
    while(1)
    {
      WindowCompiler(buff, tl->window, &tmp[i]);
      if(tl == wlist_end)
        break;
      tl = tl->next;
    }
  }
}

void UpdateScreen(int o_x, int o_y, int e_x, int e_y)
{
  ++dirtyRegions;
  dirtyRects[dirtyRegions].o_x = o_x;
  dirtyRects[dirtyRegions].o_y = o_y;
  dirtyRects[dirtyRegions].e_x = e_x;
  dirtyRects[dirtyRegions].e_y = e_y;
}

void Window_Manager()
{
  while(1)
  {

  }
}

int pointIn_LocInfo(window_locInfo_t* info, int x, int y)
{
  if(info->o_x < x && info->x_l + info->o_x > x && info->o_y < y && info->y_l + info->o_y > y )
    return 1;
  return 0;
}

window_t* getTopWindow(int x, int y)
{
  window_list_t* tmp = wlist_end;
  while(tmp->back)
  {
    if(pointIn_LocInfo(&tmp->window->locInfo, x, y))
    {
      return tmp->window;
    }
    tmp = tmp->back;
  }
  return NULL;
}

window_object_t* getWindowClickedObject(window_t* w, int x, int y)
{
  window_object_t* tmp = w->last_obj;
  x -= w->locInfo.o_x;
  y -= w->locInfo.o_y;
  while(tmp)
  {
    if(pointIn_LocInfo(&tmp->local_locInfo, x, y))
    {
      return tmp;
    }
    tmp = tmp->back;
  }
  return NULL;
}

void BringWindowToFront(window_t* w, window_list_t* wlist)
{
  window_list_t* tmp = wlist;
  for(int i = 0; tmp != NULL; i++)
  {
    if(tmp->window == w)
    {
      tmp->back->next = tmp->next;
      tmp->next->back = tmp->back;
      break;
    }
    tmp = tmp->next;
  }
  //tmp = wlist_end;
  wlist_end->next = tmp;
  tmp->next = NULL;
  tmp->back = wlist_end;
  tmp->window = wlist_end->window;
  wlist_end->window = w;
  wlist_end = tmp;

  UpdateScreen(w->locInfo.o_x, w->locInfo.o_y, w->locInfo.o_x + w->locInfo.x_l, w->locInfo.o_y + w->locInfo.y_l);
}

void Mouse_LeftClick_Handler(int x, int y)
{
  window_t* w = getTopWindow(x,y);
  if(!w) return;
  window_object_t* tmp = getWindowClickedObject(w, x, y);
  w->curr_obj = tmp;
  //func_t exec = window_execs[tmp->exec_id];
  //exec();
  BringWindowToFront(w, &main_wlist);
  if(tmp)
  {
    if(tmp->exec_id & WIN_DRAGABLE)
    {
      Mouse_Dragged_Window = w;
      Mouse_Plot = Mouse_Drag_Window;
    }
  }
}
/*
Point_t* get_RelativeOrdinate(int r_x, int r_y)
{
    Point_t
    int x = r_x;
    if(x < 0) x = w->locInfo.x_l;
    int y = r_y;
    if(y < 0) y = w->locInfo.y_l;
}*/

void createWindowObjectRects_relative(window_t* w, int c_x, int c_y, int l_x, int l_y, int z_index, int r_x, int r_y, window_object_t* parent_obj, uint32_t color, uint32_t exec_opt)
{
  int x = r_x;
  if(x < 0) x = w->locInfo.x_l;
  int y = r_y;
  if(y < 0) y = w->locInfo.y_l;
  int ll = l_x;
  if(ll < 0) ll = w->locInfo.x_l;
  int hh = l_y;
  if(hh < 0) hh = w->locInfo.y_l;
  drawRect_window(w, x + c_x, y + c_y, ll, hh, color);
  window_object_t* new_obj = kmalloc(sizeof(window_object_t));
  new_obj->exec_id = exec_opt;
  new_obj->z_index = z_index;
  new_obj->bg_color = color;
  new_obj->local_locInfo.o_x = x + c_x;
  new_obj->local_locInfo.o_y = y + c_y;
  new_obj->local_locInfo.x_l = ll;
  new_obj->local_locInfo.y_l = hh;
  if(!w->last_obj)
  {
    w->obj_list = new_obj;
    w->curr_obj = new_obj;
    new_obj->back = NULL;
  }
  else
  {
    w->last_obj->next = new_obj;
  }
  new_obj->back = w->last_obj;
  w->last_obj = new_obj;
  new_obj->next = NULL;
}

void createDefaultWindowObjects(window_t* w)
{
  createWindowObjectRects_relative(w, 0, 0, -1, 16, 1, 0, 0, NULL, 0xa00f0f0f, WIN_DRAGABLE);   // Create The Title Bar
  createWindowObjectRects_relative(w, -16, 0, 16, 16, 2, -1, 0, NULL, 0xa0ff0000, WIN_SELECTABLE); // Close Button
  createWindowObjectRects_relative(w, -32, 0, 16, 16, 2, -1, 0, NULL, 0xa000ff00, WIN_SELECTABLE); // Maximize Button
  createWindowObjectRects_relative(w, -48, 0, 16, 16, 2, -1, 0, NULL, 0xa00000ff, WIN_SELECTABLE); // Minimize Button
}

window_t* create_window_basic(char* window_name, Process_t* proc, window_t* parentw, int center_x, int center_y, int length_x, int length_y, int z_index)
{
  window_t* w = kmalloc(sizeof(window_t));
  strcpy(w->name, window_name);
  w->proc = proc;
  w->z_index = z_index;
  w->locInfo.o_x = center_x;
  w->locInfo.o_y = center_y;
  w->locInfo.x_l = length_x;
  w->locInfo.y_l = length_y;
  w->last_obj = NULL;
  w->obj_list = NULL;
  w->screen_buff = kmalloc(depthVESA*length_x*length_y/2);
  UpdateScreen(w->locInfo.o_x, w->locInfo.o_y, w->locInfo.o_x + w->locInfo.x_l, w->locInfo.o_y + w->locInfo.y_l);

  w->childs = 0;
  if(!parentw) return w;
  w->id = parentw->childs;
  ++parentw->childs;

  //createDefaultWindowObjects(w);

  return w;
}

window_t* create_window_complete(char* window_name, Process_t* proc, window_t* parentw, int center_x, int center_y, int length_x, int length_y, int z_index)
{
  window_t* w = kmalloc(sizeof(window_t));
  strcpy(w->name, window_name);
  w->proc = proc;
  w->z_index = z_index;
  w->locInfo.o_x = center_x;
  w->locInfo.o_y = center_y;
  w->locInfo.x_l = length_x;
  w->locInfo.y_l = length_y;
  w->last_obj = NULL;
  w->obj_list = NULL;
  w->screen_buff = kmalloc(depthVESA*length_x*length_y/2);
  UpdateScreen(w->locInfo.o_x, w->locInfo.o_y, w->locInfo.o_x + w->locInfo.x_l, w->locInfo.o_y + w->locInfo.y_l);

  w->childs = 0;
  if(!parentw) return w;
  w->id = parentw->childs;
  ++parentw->childs;

  createDefaultWindowObjects(w);

  return w;
}

window_t* create_window_autoMap(char* window_name, Process_t* proc, window_t* parentw, int center_x, int center_y, int length_x, int length_y, int z_index)
{
  window_t* w = kmalloc(sizeof(window_t));
  strcpy(w->name, window_name);
  w->proc = proc;
  w->z_index = z_index;
  w->locInfo.o_x = center_x;
  w->locInfo.o_y = center_y;
  w->locInfo.x_l = length_x;
  w->locInfo.y_l = length_y;
  w->last_obj = NULL;
  w->obj_list = NULL;
  WindowListAdd(w);
  w->screen_buff = kmalloc((depthVESA*length_x*length_y)/2);
  UpdateScreen(w->locInfo.o_x, w->locInfo.o_y, w->locInfo.o_x + w->locInfo.x_l, w->locInfo.o_y + w->locInfo.y_l);

  w->childs = 0;
  if(!parentw) return w;
  w->id = parentw->childs;
  ++parentw->childs;
  return w;
}

void WindowListAdd(window_t* w)
{
  window_list_t* tmp = main_wlist.next;
  for(int i = 0; tmp != wlist_end; i++)
  {
    if(tmp->window->z_index > w->z_index)
    {
      window_list_t* tmp2 = tmp;
      tmp = tmp->back;
      tmp->next = kmalloc(sizeof(window_list_t));
      tmp->next->window = w;
      tmp->next->back = tmp;
      tmp = tmp->next;
      tmp->next = tmp2;
      tmp2->back = tmp;
      return;
    }
    tmp = tmp->next;
  }
  wlist_end->next = kmalloc(sizeof(window_list_t));
  wlist_end->next->window = wlist_end->window;
  wlist_end->window = w;
  wlist_end = wlist_end->next;
  wlist_end->next = NULL;
  wlist_end->back = tmp;
}

void WindowDrag(window_t* w, int n_x, int n_y)
{
  UpdateScreen(w->locInfo.o_x, w->locInfo.o_y, w->locInfo.o_x + w->locInfo.x_l, w->locInfo.o_y + w->locInfo.y_l);
  w->locInfo.o_x = n_x;
  w->locInfo.o_y = n_y;
  UpdateScreen(n_x, n_y, n_x + w->locInfo.x_l, n_y + w->locInfo.y_l);
}

void WindowDrag_relative(window_t* w, int d_x, int d_y)
{
  window_locInfo_t* loc = &w->locInfo;
  UpdateScreen(loc->o_x, loc->o_y, loc->o_x + loc->x_l, loc->o_y + loc->y_l);
  loc->o_x += d_x;
  loc->o_y += d_y;
  UpdateScreen(loc->o_x, loc->o_y, loc->o_x + loc->x_l, loc->o_y + loc->y_l);
}
