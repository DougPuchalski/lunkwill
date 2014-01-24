#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include "lwutils.h"
#include "datastructures.h"

enum UI_TYPE
{
	UI_TYPE_TEXT,
	UI_TYPE_IMG,
	UI_TYPE_LIST,
	UI_TYPE_TABLE,
	UI_TYPE_TABLE_CELL,
	UI_TYPE_FORM,
	UI_TYPE_INPUT,
	UI_TYPE_POPUP,
	UI_TYPE_EXEC,
};

union ui_elem_u{
	struct ui_elem_text *text;
	struct ui_elem_list *list;
	struct ui_elem_table *table;
	struct ui_elem_table_cell *cell;
	struct ui_elem_form *form;
	struct ui_elem_input *input;
	struct ui_elem_popup *popup;
	struct ui_elem_exec *exec;
};

typedef struct
{
	enum UI_TYPE ui_elem_type;
	union ui_elem_u ui_elem_data;
} ui_elem_t;

struct  ui_elem_text
{
	char *text;
};

struct  ui_elem_exec
{
	char *jscript;
};

struct  ui_elem_img
{
	char *img_path;
};

struct  ui_elem_list
{
	int direction; //horizontal/vertical
	char *title;
	char bullet;
	list_elem *content;
};

struct  ui_elem_table_cell
{
	int x;
	int y;
	ui_elem_t *content;
};

struct  ui_elem_table
{
	list_elem *cells;
};


struct  ui_elem_input
{
	ui_elem_t *description;
	char *input_type;
	char *cookie_name;
};

struct  ui_elem_form
{
	list_elem *inputs;
	char *submit_button_text;
};

struct  ui_elem_popup
{
	ui_elem_t *content;
};

typedef struct
{
	list_elem *title;
	list_elem *sidebar_title;
	list_elem *sidebar;
	list_elem *site;
} page_t;

/* \brief Generate HTML Code out of the page struct */
extern char *ui_show(page_t *p);

/* \brief Generate HTML Code out of the page struct */
extern char *ui_set_cookie(page_t *p, char *cookie_name, char *cookie_content);

#endif
