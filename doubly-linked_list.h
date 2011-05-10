#ifndef DOUBLYLINKED_LIST
#define DOUBLYLINKED_LIST

struct dlList_node{
	void *data;
	struct dlList_node *prev;
	struct dlList_node *next;
};


struct dlList{
	unsigned int size;
	struct dlList_node *head;
	struct dlList_node *tail;

	int (*compare)(const void *data1, const void *data2);
	void (*destroy)(void *data);
};

void dlList_init(struct dlList *list, void (*destroyFunction)(void *data), int (*compareFunction)(const void *data1, const void *data2));

void dlList_destroy(struct dlList *list);

int dlList_insertBefore(struct dlList *list, struct dlList_node *beforeNode, void *data);

int dlList_insertAfter(struct dlList *list, struct dlList_node *afterNode, void *data);

int dlList_append(struct dlList *list, void *data);

int dlList_remove(struct dlList *list, struct dlList_node *node, void **data);

struct dlList_node *dlList_find(struct dlList *list, const void *key);

struct dlList *dlList_findAll(struct dlList *list, const void *key);

void dlList_sort(struct dlList *list);

unsigned int dlList_getSize(struct dlList *list);

#endif //DOUBLYLINKED_LIST
