#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct list_node {
    int data;
    struct list_node* next;
} list_node_s;

int Insert(int value, list_node_s** head_p);
int Delete(int value, list_node_s** head_p);
int Member(int value, list_node_s* head_p);
void Destroy(list_node_s* head_p);

#endif // LINKED_LIST_H
