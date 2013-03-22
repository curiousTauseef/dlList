#ifndef DOUBLYLINKED_LIST
#define DOUBLYLINKED_LIST

#include <stddef.h>

struct dlList_node
{
	void *data;
	struct dlList_node *prev;
	struct dlList_node *next;
};

typedef struct dlList dlList;

struct dlList{
	size_t size;
	struct dlList_node *head;
	struct dlList_node *tail;

	int ( *compare )( const void *data1, const void *data2 );
	void ( *destroy )( void *data );
};

enum dlList_returnValues
{
   DLLIST_OK,
   DLLIST_ERR_errArg,
   DLLIST_ERR_malloc,
};

void dlList_init( struct dlList *list,
      void ( *destroyFunction )( void *data ),
      int ( *compareFunction )( const void *data1, const void *data2) );

void dlList_destroy( struct dlList *list );

int dlList_insertBefore( struct dlList *list, struct dlList_node *beforeNode,
      void *data );

int dlList_insertAfter( struct dlList *list, struct dlList_node *afterNode,
      void *data );

int dlList_append( struct dlList *list, void *data );

int dlList_remove( struct dlList *list, struct dlList_node *node,
      void **data );

struct dlList_node *dlList_find( const struct dlList *list, const void *key );

struct dlList *dlList_findAll( const struct dlList *list, const void *key );

void dlList_sort( struct dlList *list );

size_t dlList_size( const struct dlList *list );

struct dlList_node *dlList_first( const struct dlList *list );

struct dlList_node *dlList_last( const struct dlList *list );

#endif //DOUBLYLINKED_LIST
