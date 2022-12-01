//
// Created by Anh Nguyen on 2022-11-29.
//

#ifndef ASSIGNMENT3_C_FILE_HANDLER_H
#define ASSIGNMENT3_C_FILE_HANDLER_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> /* memcpy */
#include <ctype.h> /* isdigit */
#include <math.h>

#endif //ASSIGNMENT3_C_FILE_HANDLER_H
struct node {
    char type[3];
    int base;
    int limit;
    struct node * next;
};

/* Struct node for Linked List. */

int checkValue(char * type, int base, int limit);
void pushBack(struct node** head, struct node * temp);
struct node * processFile(FILE * input);
void printList(struct node* head);
void mergeSort(struct node ** headPtr);
struct node * merge (struct node * first, struct node * second);
void mergeHoles(struct node *head);
int max(int a, int b);
void compaction(struct node * head);
void saniticheck(struct node * head);