#include "list.h"

#include <assert.h>
#include <stdlib.h>

#include "err.h"
#include "util.h"

/**
 * Struct for encapsulating a single list element.
 */
typedef struct ListItem {
    struct ListItem* next;  // pointer to the next element (NULL if last)
    void* data;             // pointer to the data
} ListItem;

List mkList(void) {
    List res;
    res.head = NULL;

    return res;
}

void clearList(List* s) {
    while (!isEmpty(s)) {
        pop(s);
    }
}

void push(List* s, void* data) {
    ListItem* node = malloc(sizeof(ListItem));
    if (node == NULL) {
        return;
    }
    node->data = data;
    node->next = s->head;
    s->head = node;
}

void* peek(List* s) {
    if (!isEmpty(s)) {
        return s->head->data;
    } else
        // ERROR("Stack is empty.");
        return NULL;  // just funny
}

void pop(List* s) {
    if (isEmpty(s)) {
        return;
    }
    ListItem* temp = s->head;

    s->head = s->head->next;
    free(temp);
    temp = NULL;
}

char isEmpty(List* s) {
    if (s->head == NULL) {
        return 1;
    } else {
        return 0;
    }
}

ListIterator mkIterator(List* list) {
    ListIterator res;
    res.list = list;
    res.prev = NULL;
    res.current = list->head;

    return res;
}

void* getCurr(ListIterator* it) {
    assert(it->current != NULL);
    return it->current->data;
}

void next(ListIterator* it) {
    assert(isValid(it));
    it->prev = it->current;
    it->current = it->current->next;
}

char isValid(ListIterator* it) { return it->current != NULL; }
