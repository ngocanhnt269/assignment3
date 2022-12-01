/**
 * File: assignment3.c
 *
 * Author: Anh Nguyen, Linh Nguyen
 * Date: Nov 30, 2022
 * Course: COMP2510
 *
 * Summary of File:
 *
 * This file contains functions that simulate memory management.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* memcpy */
#include <ctype.h> /* isdigit */
#include "file_handler.h"

/**
 * Checks type, base and limit values before adding node to the linked list.
 *
 * @param type hole or process
 * @param base starting index
 * @param limit length of block
 * @return
 */
int checkValue(struct node * head, char * type, int base, int limit) {
    if (base < 0 || limit <= 0) {
         printf("Error: Invalid base/limit !!\n");
        return 0;
    }
    if (type[0] != 'P' && type[0] != 'H') {
         printf("Error: Type does not start with 'P' or 'H' !!\n");
        return 0;
    }
    if (strlen(type) > 1) {
        for (int i = 1; i < strlen(type); i++) {
            if (isdigit(type[i]) == 0) { // Check if type "process" is in format "P%"
                 printf("Error: Invalid process name!!\n");
                return 0;
            }
        }
    }
    if (head != NULL) {
      struct node * current = head;
      while (current != NULL) {
        if (strcmp(current->type, type) == 0) {
          printf("Error: 2 processes have same name!!\n");
          return 0;
        }
        current = current->next;
      }
    }
    return 1;
}

/**
 * Function that will push the node to the back of linked list.
 *
 * @param head head node
 * @param temp node to add to linked list
 */
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

/**
 * Checks whether or not two memory blocks overlap/have gap between.
 *
 * @param head the head node of linked list
 */
void saniticheck(struct node * head){
    struct node * curr = head;
    struct node * nextPtr = NULL;

    while ((curr != NULL) && (curr->next != NULL)) {
        nextPtr = curr->next;
        // check overlapped
        if ((curr-> base + curr-> limit) > (nextPtr-> base)){
            printf("Error: Memory blocks are overlapped ");
            exit(0);
        }
        // check if any gap between memory and holes
        if ((curr-> base + curr -> limit) < (nextPtr->base)){
            printf("Error: Memory blocks have gap between");
            exit(0);
        }
        curr = curr->next;
    }
}

/**
 * Reads each line from the input file, then creates a node
 * and pushes it to the linked list.
 *
 * @param input pointer to the input file
 * @return the head node of linked list
 */
struct node* processFile(FILE * input) {
    char buffer[100];
    struct node *head = NULL;
    //Reads a line from the specified stream and stores it into the string pointed to by buffer
    while(fgets(buffer, 100, input) != NULL){
        // Create a new Node
        struct node * temp  = (struct node*) malloc(sizeof(struct node));
        sscanf(buffer, "%s %d %d", temp->type, &temp->base,&temp->limit);
        temp->next = NULL;
        // Check all values before adding new node to the linked list
        if (checkValue(head,temp->type, temp->base, temp->limit)==0) {
            // If any value is invalid, exit the function
            return NULL;
        }
        pushBack(&head, temp);
    }
    mergeSort(&head); // Sort the linked list
    saniticheck(head);
    return head;
}

/**
 * Prints the whole linked list.
 *
 * @param head the head node of linked list
 */
void printList(struct node* head) {
    struct node* curr = head;
    while (curr != NULL) {
        printf("Node: %s, base = %d, limit = %d\n", curr->type, curr->base, curr->limit);
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

/**
 * Finds the maximum value between 2 integers.
 *
 * @param a first integer
 * @param b second integer
 * @return max value
 */
int max(int a, int b) {
    return (a > b) ? a : b;
}

/**
 * Merges the consecutive free blocks (holes).
 *
 * @param head of a node
 */
void mergeHoles(struct node *head) {
    struct node *curr = head;
    struct node *nextPtr = NULL;
    while ((curr!= NULL) && (curr->next != NULL)) {
        nextPtr = curr->next;
        // check if two consecutive nodes both have type as 'H'
        if ((curr->type[0]=='H') && (nextPtr->type[0] == 'H')) {
            if ((curr->base + curr->limit) >= nextPtr->base) {
                //
                 curr->limit = max(curr->limit,(nextPtr->base + nextPtr->limit) - curr->base); // if can overlapped
//                curr->limit = curr->limit + curr->base;
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

/**
 * Pushes all nodes representing free blocks to the end of the linked list.
 *
 * @param head the head of linked list
 */
void compaction(struct node * head) {
    struct node * curr = head;
    struct node * nextPtr = NULL;
    while ((curr != NULL) && (curr->next !=NULL)) {
        nextPtr = curr->next;
        // move memory block to the current hole position
        if ((curr->type[0] == 'H') && (nextPtr->type[0] == 'P')) {
            // move memory to the front
            strcpy(curr->type, nextPtr->type); // copy type of nextPtr to current type
            int holeLimit = curr->limit; // store limit value
            curr->limit= nextPtr->limit; // point current limit = value of P
            strcpy(nextPtr->type, "H"); // update the nextPtr to "H"
            nextPtr->base = curr->base + curr->limit; // update base of H = (base P + limit)
            nextPtr->limit = holeLimit; //update limit of H
            curr = nextPtr;
        } else if ((curr->type[0] == 'H') && (nextPtr->type[0] == 'H')) {
            //If 2 "H", we do the same as merge hole
            if ((curr->base + curr->limit) >= nextPtr->base) {
//                curr->limit = max(curr->limit,(nextPtr->base+ nextPtr->limit) - curr->base);
                curr->limit = curr->limit + curr->base;
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