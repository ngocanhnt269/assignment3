#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* memcpy */
#include <ctype.h> /* isdigit */
#include "file_handler.h"

int checkValue(char * type, int base, int limit) {
    if (base < 0 || limit <= 0) {
         printf("error 1");
        return 0;
    }
    if (type[0] != 'P' && type[0] != 'H') {
         printf("error 2");
        return 0;
    }
    if (strlen(type) > 1) {
        for (int i = 1; i < strlen(type); i++) {
            if (isdigit(type[i]) == 0) {
                 printf("error 3");
                return 0;
            }
        }
    }
    return 1;
}


void pushBack(struct node** head, struct node *temp) {
    temp->next = NULL;
    if (*head == NULL) {
        *head = temp;
        return;
    }
    struct node * curr = *head;
    while (curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = temp;
}

struct node* processFile(FILE * input) {
    char buffer[100];
    struct node *head = NULL;
    while(fgets(buffer, 100, input) != NULL){
        // Create a new Node
        struct node * temp  = (struct node*) malloc(sizeof(struct node));
        sscanf(buffer, "%s %d %d", temp->type, &temp->base,&temp->limit);

        temp->next = NULL;
        // Check all values before adding new node to the linked list
        if (checkValue(temp->type, temp->base, temp->limit) == 0) { 
            // If any value is invalid, exit the function
            printf("Error: Invalid value!");
            return NULL;
        }
        pushBack(&head, temp);
    }
//    if ((head != NULL) && (head->base !=0))
//    {
//        printf("Error: Memory does not start from 0.");
//        return NULL;
//    }
//    printf("\nBefore sorting: \n");
//    printList(head);
    mergeSort(&head);
//    printf("\nAfter sorting: \n");
//    printList(head);
    return head;
}

void printList(struct node* head) {
    struct node* curr = head;
    while (curr != NULL) {
        printf("%s %d %d\n", curr->type, curr->base, curr->limit);
        curr = curr->next;
    }
}

/**
 *  Sorts the linked list using the merge sort algorithm.
 * 
 * @param headPtr pointer of pointer head node
*/
void mergeSort(struct node ** headPtr) {
    struct node * h = *headPtr;

    /* Base case for linked list's length = 0 or 1. */
    if (h == NULL || h->next == NULL) {
        return;
    }

    /* Divides the linked list into 2 halves (sublists). */
    struct node * slow = h;
    struct node * fast = h->next;

    while (fast != NULL) {
        // fast pointer advances 2 times faster than slow pointer
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    struct node * secondHalf = slow->next; // slow pointer is in the middle of linked list
    slow->next = NULL; // Separates the first half with the second half

    /* Merge sort the sublists.*/
    mergeSort(&h);
    mergeSort(&secondHalf);

    /* merge the sorted sublists. */
    *headPtr = merge(h, secondHalf);
}

/**
 * Merges two sublists.
 * 
 * @param first first node of the first sublist
 * @param second first node of the second sublist
 * @return pointer to the head of the sorted linked list
*/
struct node * merge (struct node * first, struct node * second) {
    struct node * result = malloc(sizeof(struct node));
    struct node * temp = malloc(sizeof(struct node));

    result = temp;

    while (first != NULL && second != NULL) {
        if (first->base <= second->base) {
            temp->next = first;
            first = first->next;
        } else {
            temp->next = second;
            second = second->next;
        }
        temp = temp->next;
    }

    while (first != NULL) {
        temp->next = first;
        first = first->next;
        temp = temp->next;
    }

    while (second != NULL) {
        temp->next = second;
        second = second->next;
        temp = temp->next;
    }
    return result->next;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

void mergeHoles(struct node *head) {
    struct node *curr = head;
    struct node *nextPtr = NULL;
    while ((curr!= NULL) && (curr->next != NULL)) {
        nextPtr = curr->next;

        // merge 2 holes if they are overlapped
        // Can 2 holes overlapped?
        if ((curr->type[0]=='H') && (nextPtr->type[0] == 'H')) {
            if ((curr->base + curr->limit) >= nextPtr->base) {
                curr->limit = max(curr->limit,nextPtr->base+ nextPtr->limit-curr->base);
                // delete node next
                curr->next = curr->next->next;
            } else {
                curr = curr->next;
            }
        } else {
            curr = curr->next;
        }
    }
}

void compaction(struct node * head) {
    struct node * curr = head;
    struct node * nextPtr = NULL;
    while ((curr != NULL) && (curr->next !=NULL)) {
        nextPtr = curr->next;
        // move memory block to the current hole position
        if ((curr->type[0] == 'H') && (nextPtr->type[0] == 'P')) {
            if ((curr->base+curr->limit) > nextPtr->base)
            {
                printf("Error: Overlapped between memory blocks.");
                return;
            }

            if ((curr->base+curr->limit) < nextPtr->base) {
                printf("Error: Gap between memory blocks.");
                return;
            }

            // move memory to the front
            strcpy(curr->type, nextPtr->type);
            int holeLimit = curr->limit;
            curr->limit= nextPtr->limit;
            strcpy(nextPtr->type, "H");
            nextPtr->base = curr->base+curr->limit;
            nextPtr->limit = holeLimit;
            curr = nextPtr;
        } else if ((curr->type[0] == 'H') && (nextPtr->type[0] == 'H')) {
            if ((curr->base + curr->limit) >= nextPtr->base) {
                curr->limit = max(curr->limit,nextPtr->base+ nextPtr->limit-curr->base);
                // delete node next
                curr->next = curr->next->next;
            } else {
                curr = curr->next;
            }
        } else {
            curr = curr->next;
        }
    }
}