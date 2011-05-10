#include "doubly-linked_list.h"
#include <stdlib.h>

void dlList_init(struct dlList *list, void (*destroyFunction)(void *data), int (*compareFunction)(const void *data1, const void *data2)){
	if(list == NULL)
		return;

	list->size = 0;
	list->head = NULL;
	list->tail = NULL;
	list->compare = compareFunction;
	list->destroy = destroyFunction;
}

void dlList_destroy(struct dlList *list){
	if(list == NULL)
		return;

	while(list->size > 0)
		dlList_remove(list, list->tail, NULL);

	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
}

int dlList_insertBefore(struct dlList *list, struct dlList_node *beforeNode, void *data){
	if(list == NULL || (list->size > 0 && beforeNode == NULL))
		return -1;

	struct dlList_node *newNode = malloc(sizeof(struct dlList_node));
	if(newNode == NULL)
		return -1;
	
	newNode->data = data;

	if(list->size == 0){
		newNode->next = NULL;
		newNode->prev = NULL;
		list->head = newNode;
		list->tail = newNode;
	}

	else{
		newNode->next = beforeNode;
		newNode->prev = beforeNode->prev;

		if(beforeNode->prev == NULL)
			list->head = newNode;
		else
			beforeNode->prev->next = newNode;

		beforeNode->prev = newNode;
	}

	++list->size;

	return 0;
}

int dlList_insertAfter(struct dlList *list, struct dlList_node *afterNode, void *data){
	if(list == NULL || (afterNode == NULL && list->size > 0))
		return -1;

	struct dlList_node *newNode = malloc(sizeof(struct dlList_node));
	if(newNode == NULL)
		return -1;

	newNode->data = data;

	if(list->size == 0){
		newNode->next = NULL;
		newNode->prev = NULL;
		list->head = newNode;
		list->tail = newNode;
	}

	else{
		newNode->next = afterNode->next;
		newNode->prev = afterNode;

		if(afterNode->next == NULL)
			list->tail = newNode;
		else
			afterNode->next->prev = newNode;

		afterNode->next = newNode;
	}

	++list->size;

	return 0;
}

int dlList_append(struct dlList *list, void *data){
	if(list == NULL)
		return -1;

	return dlList_insertAfter(list, list->tail, data);
}

int dlList_remove(struct dlList *list, struct dlList_node *node, void **data){
	if(list == NULL || list->size == 0 || node == NULL)
		return -1;

	if(data != NULL)
		*data = node->data;
	else if(list->destroy != NULL)
		list->destroy(node->data);

	if(node == list->head){
		list->head = node->next;

		if(list->head == NULL)
			list->tail = NULL;
		else
			node->next->prev = NULL;
	}
	
	else{
		node->prev->next = node->next;

		if(node->next == NULL)
			list->tail = node->prev;
		else
			node->next->prev = node->prev;
	}

	free(node);

	--list->size;

	return 0;
}

//struct dlList_node *dlList_find(struct dlList *list, const void *key){
//	if(list == NULL || list->compare == NULL || key == NULL)
//		return NULL;
//
//	struct dlList_node *nodePointer = list->head;
//
//	while(nodePointer != NULL || list->compare(&key, &nodePointer->data) != 0)
//		nodePointer = nodePointer->next;
//
//	return nodePointer;
//}

unsigned int dlList_getSize(struct dlList *list){
	if(list == NULL)
		return 0;

	return list->size;
}
