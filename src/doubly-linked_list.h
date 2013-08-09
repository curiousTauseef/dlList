/* 
 * Copyright (c) 2013 Andreas Misje
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/**
 * \mainpage Doubly-linked list
 *
 * A simple doubly-linked list implementation in C. Supports sorting and
 * user-defined node deep-copy and destroy functions.
 */

/**
 * \defgroup doublyLinkedList Doubly-linked list
 *
 * \brief A simple doubly-linked list implementation
 *
 * Supports sorting and user-defined node deep-copy and destroy functions.
 */

/**
 * \addtogroup doublyLinkedList
 * @{
 *
 * \file
 * \author Andreas Misje
 * \data 22.03.13
 *
 * \brief A simple doubly-linked list implementation
 *
 * Supports sorting and user-defined node deep-copy and destroy functions.
 */

#ifndef DOUBLYLINKED_LIST
#define DOUBLYLINKED_LIST

#include <stddef.h>

/**
 * \brief Node
 */
struct dlList_node
{
   /** \brief User data */
   void *data;
   /** \brief Pointer to previous node */
   struct dlList_node *prev;
   /** \brief Pointer to next node */
   struct dlList_node *next;
};

/**
 * \brief Doubly-linked list
 */
struct dlList
{
   /** \brief Number of nodes */
   size_t size;
   /** \brief Pointer to first node */
   struct dlList_node *head;
   /** \brief Pointer to last node */
   struct dlList_node *tail;

   /** \brief Pointer to a user-supplied comparison function. May be NULL */
   int ( *compare )( const void *data1, const void *data2 );
   /**
    * \brief Pointer to a user-supplied node user-data destroy function. May
    * be NULL
    */
   void ( *destroy )( void *data );
   /**
    * \brief Pointer to a user-supplied function that makes a deep copy of a
    * supplied user data. May be NULL, in which case data is always shared
    * when nodes are copied
    */
   void *( *copy )( const void *data );
};

/**
 * \brief Return values
 */
enum dlList_returnValues
{
   /** \brief Everything went fine */
   DLLIST_OK = 0,
   /** \brief An erroneous argument was passed to the function */
   DLLIST_ERR_errArg,
   /** \brief Memory allocation failed */
   DLLIST_ERR_malloc,
   /** \brief An undefined function pointer was attempted called */
   DLLIST_ERR_undefFunc,
};

/** 
 * \brief Initialise a doubly-linked list object
 *
 * \param list doubly-linked list object
 * \param destroyFunction pointer to a function that will be called
 * whenever a node is destroyed. The supplied argument is the node data
 * \param compareFunction pointer to a function that compares two instances of
 * user data. It must return an integer less than, equal to, or greater than
 * zero if the first argument is considered to be respectively less than,
 * equal to or larger than the second
 * \param dataDeepCopyFunction pointer to a function that creates a deep copy
 * of user data. If NULL, data will be shared and not copied
 */
void dlList_init( struct dlList *list,
      void ( *destroyFunction )( void *data ),
      int ( *compareFunction )( const void *data1, const void *data2),
      void *( *dataDeepCopyFunction )( const void *data ) );

/**
 * \brief Destroy a doubly-linked list
 *
 * All nodes will be deleted, and destroy(), if defined, will be called on all
 * user-supplised data
 *
 * \param list doubly-linked list object
 */
void dlList_destroy( struct dlList *list );

/**
 * \brief Insert a new node before another node
 *
 * Insert a new node before the given node. The given node must exist if the
 * list is non-empty.
 *
 * \param list doubly-linked list object
 * \param beforeNode the node to insert the new node in front of
 * \param data data to add to the new node
 *
 * \retval DLLIST_OK if no problems occurred
 * \retval DLLIST_ERR_errArg if list is NULL or if the list is non-empty and
 * beforeNode is NULL
 * \retval DLLIST_ERR_malloc if there was not enough memory to create a new
 * node
 */
int dlList_insertBefore( struct dlList *list, struct dlList_node *beforeNode,
      void *data );

/**
 * \brief Insert a new node after another node
 *
 * Insert a new node after the given node. The given node must exist if the
 * list is non-empty.
 *
 * \param list doubly-linked list object
 * \param afterNode the node to insert the new node after
 * \param data data to add to the new node
 *
 * \retval DLLIST_OK if no problems occurred
 * \retval DLLIST_ERR_errArg if list is NULL or if the list is non-empty and
 * afterNode is NULL
 * \retval DLLIST_ERR_malloc if there was not enough memory to create a new
 * node
 */
int dlList_insertAfter( struct dlList *list, struct dlList_node *afterNode,
      void *data );

/**
 * \brief Insert a new node in a sorted list
 *
 * The new node will be inserted so that the list will remain sorted by using
 * the user-supplied compare() function. The compare() function must be
 * defined.
 *
 * \param list doubly-linked list object
 * \param data data to add to the new node
 *
 * \retval DLLIST_OK if no problems occurred
 * \retval DLLIST_ERR_errArg if list is NULL
 * \retval DLLIST_ERR_undefFunc if compare() is undefined
 * \retval DLLIST_ERR_malloc if there was not enough memory to create a new
 * * node
 */
int dlList_insertOrdered( struct dlList *list, void *data );

/**
 * \brief Insert a new node at the end of a list
 *
 * \param list doubly-linked list object
 * \param data data to add to the new node
 *
 * \retval DLLIST_OK if no problems occurred
 * \retval DLLIST_ERR_errArg if list is NULL
 * \retval DLLIST_ERR_malloc if there was not enough memory to create a new
 * * node
 */
int dlList_append( struct dlList *list, void *data );

/**
 * \brief Remove a node from a list
 *
 * Remove the given node, destroy user data if destroy() is defined and data
 * is NULL. If data is non-NULL, the data from the node that is to be removed
 * will be set to the given pointer.
 *
 * \param list doubly-linked list object
 * \param node node to remove. Must exist
 * \param pointer to user data. If non-NULL, it will contain the user data of
 * the node to remove. If NULL, and if destroy() is defined, user data will be
 * destroyed by calling destroy( node->data )
 *
 * \retval DLLIST_OK if no problems occurred
 * \retval DLLIST_ERR_errArg if list is NULL
 */
int dlList_remove( struct dlList *list, struct dlList_node *node,
      void **data );

/**
 * \brief Search for a node in a list
 *
 * The first node with data matching the given key will be return, or NULL if
 * no node could be found.
 *
 * \param list doubly-linked list object
 * \param key data that will be compared to all node's data by using the
 * user-supplied compare() function. Cannot be NULL
 *
 * \returns a node object if a node was found, or NULL if no node could be
 * found. NULL will also be returned if any erroneous arguments was supplied
 * or if compare() is undefined
 */
struct dlList_node *dlList_find( const struct dlList *list, const void *key );

/**
 * \brief Creates a duplicate of a list
 *
 * This function uses copy(), if defined, to deep-copy user data.
 *
 * \param list doubly-linked list object to copy
 *
 * \return an exact copy of the given list, unless list is NULL, in which case
 * an empty list is returned. If a memory allocation error occurs, an empty
 * list is returned
 */
struct dlList dlList_copy( const struct dlList *list );

/**
 * \brief Append one list to another
 *
 * This function copies all data from one list to another. Deep data copy will
 * be performed of user data if the user-supplied copy() function is defined.
 *
 * \param list1 list to append data to
 * \param list2 list to fetch data from
 *
 * \retval DLLIST_OK if no problems occurred
 * \retval DLLIST_ERR_errArg if list is NULL
 * \retval DLLIST_ERR_malloc if there was not enough memory to create a new
 * node
 */
int dlList_appendList( struct dlList *list1, const struct dlList *list2 );

/**
 * \brief Sort a list
 *
 * The user must have supplied a compare() function
 *
 * \param list doubly-linked list object
 */
void dlList_sort( struct dlList *list );

/**
 * \brief Number of nodes in list
 */
size_t dlList_size( const struct dlList *list );

/**
 * \brief First node in list
 */
struct dlList_node *dlList_first( const struct dlList *list );

/**
 * \brief Last node in list
 */
struct dlList_node *dlList_last( const struct dlList *list );

#endif //DOUBLYLINKED_LIST

/**
 * @}
 */
