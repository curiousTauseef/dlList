#include "doubly-linked_list.h"
#include <stdlib.h>

void dlList_init( struct dlList *list,
      void ( *destroyFunction )( void *data ),
      int ( *compareFunction )( const void *data1, const void *data2 ),
      void *( *dataDeepCopyFunction )( const void *data ) )
{
	if ( !list )
		return;

	list->size = 0;
	list->head = NULL;
	list->tail = NULL;
	list->compare = compareFunction;
	list->destroy = destroyFunction;
   list->copy = dataDeepCopyFunction;
}

void dlList_destroy( struct dlList *list )
{
	if ( !list )
		return;

	while ( list->size > 0 )
		dlList_remove( list, list->tail, NULL );

	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
}

int dlList_insertBefore( struct dlList *list, struct dlList_node *beforeNode,
      void *data )
{
	if ( !list || ( !beforeNode && list->size > 0 ) )
		return DLLIST_ERR_errArg;

	struct dlList_node *newNode = malloc( sizeof( struct dlList_node ) );
	if ( !newNode )
		return DLLIST_ERR_malloc;
	
	newNode->data = data;

	if ( list->size == 0 )
   {
		newNode->next = NULL;
		newNode->prev = NULL;
		list->head = newNode;
		list->tail = newNode;
	}

	else
   {
		newNode->next = beforeNode;
		newNode->prev = beforeNode->prev;

		if ( beforeNode->prev == NULL )
			list->head = newNode;
		else
			beforeNode->prev->next = newNode;

		beforeNode->prev = newNode;
	}

	++list->size;

	return DLLIST_OK;
}

int dlList_insertAfter( struct dlList *list, struct dlList_node *afterNode,
      void *data )
{
	if ( !list || ( !afterNode && list->size > 0 ) )
      return DLLIST_ERR_errArg;

	struct dlList_node *newNode = malloc( sizeof( struct dlList_node ) );
   if ( !newNode )
      return DLLIST_ERR_malloc;

	newNode->data = data;

	if ( list->size == 0 )
   {
		newNode->next = NULL;
		newNode->prev = NULL;
		list->head = newNode;
		list->tail = newNode;
	}

	else
   {
		newNode->next = afterNode->next;
		newNode->prev = afterNode;

		if ( !afterNode->next )
			list->tail = newNode;
		else
			afterNode->next->prev = newNode;

		afterNode->next = newNode;
	}

	++list->size;

   return DLLIST_OK;
}

int dlList_insertOrdered( struct dlList *list, void *data )
{
   if ( !list )
      return DLLIST_ERR_errArg;

   if ( !list->compare )
      return DLLIST_ERR_undefFunc;

   struct dlList_node *n;
   for ( n = list->head; n; n = n->next )
      if ( list->compare( &data, &n->data ) < 0 )
         return dlList_insertBefore( list, n, data );

   return dlList_append( list, data );
}

int dlList_append( struct dlList *list, void *data )
{
   if ( !list )
      return DLLIST_ERR_errArg;

	return dlList_insertAfter( list, list->tail, data );
}

int dlList_remove( struct dlList *list, struct dlList_node *node,
      void **data )
{
	if ( !list || list->size == 0 || !node )
      return DLLIST_ERR_errArg;

	if ( data )
		*data = node->data;

	else if ( list->destroy )
		list->destroy(node->data);

	if ( node == list->head )
   {
		list->head = node->next;

		if ( !list->head )
			list->tail = NULL;
		else
			node->next->prev = NULL;
	}
	
	else
   {
		node->prev->next = node->next;

		if ( !node->next )
			list->tail = node->prev;
		else
			node->next->prev = node->prev;
	}

	free( node );

	--list->size;

   return DLLIST_OK;
}

struct dlList_node *dlList_find( const struct dlList *list, const void *key )
{
	if ( !list || !list->compare || !key )
		return NULL;

	struct dlList_node *nodePointer = list->head;

	while ( nodePointer && list->compare( &key, &nodePointer->data) != 0 )
		nodePointer = nodePointer->next;

	return nodePointer;
}

struct dlList dlList_copy( const struct dlList *list )
{

   struct dlList l;
   dlList_init( &l, list->destroy, list->compare, list->copy );

   if ( !list )
      return l;

   struct dlList_node *n;
   for ( n = list->head; n; n = n->next )
   {
      void *data = n->data;
      if ( list->copy )
         data = list->copy( data );

      if ( dlList_append( &l, data ) )
      {
         dlList_destroy( &l );
         return l;
      }
   }

   return l;
}

int dlList_appendList( struct dlList *list1, const struct dlList *list2 )
{
   if ( !list1 || !list2 )
      return DLLIST_ERR_errArg;

   struct dlList_node *n;
   for ( n = list2->head; n; n = n->next )
   {
      void *data = n->data;
      if ( list1->copy )
         data = list1->copy( data );

      int retVal = dlList_append( list1, data );
      if ( retVal )
         return retVal;
   }

   return DLLIST_OK;
}

void dlList_sort( struct dlList *list )
{
   if ( !list )
      return;

   if ( !list->compare )
      return;

   if ( list->size <= 1 )
      return;

   /* The following algorithm is based on http://stackoverflow.com/a/3032462
    * */

   int numMerges;
   size_t leftSize, rightSize, listSize = 1;
   struct dlList_node *tail, *left, *right, *next;

   do {
      numMerges = 0;
      left = list->head;
      tail = list->head = NULL;

      while ( left )
      {
         ++numMerges;
         right = left;
         leftSize = 0;
         rightSize = listSize;

         while ( right && leftSize < listSize )
         {
            ++leftSize;
            right = right->next;
         }

         while ( leftSize > 0 || ( right && rightSize > 0 ) )
         {
            if ( !leftSize )
            {
               next = right;
               right = right->next;
               --rightSize;
            }

            else if ( !right || !rightSize )
            {
               next = left;
               left = left->next;
               --leftSize;
            }

            else if ( list->compare( &left->data, &right->data ) < 0 )
            {
               next = left;
               left = left->next;
               --leftSize;
            }

            else
            {
               next = right;
               right = right->next;
               --rightSize;
            }
            
            if ( tail )
               tail->next = next;
            else
               list->head = next;

            next->prev = tail;
            tail = next;
         }

         left = right;
      }

      tail->next = NULL;
      listSize <<= 1;

   } while ( numMerges > 1 );

   list->tail = tail;
}

size_t dlList_size( const struct dlList *list )
{
	if ( !list )
		return 0;

	return list->size;
}

struct dlList_node *dlList_first( const struct dlList *list )
{
   if ( !list )
      return NULL;

   return list->head;
}

struct dlList_node *dlList_last( const struct dlList *list )
{
   if ( !list )
      return NULL;

   return list->tail;
}
