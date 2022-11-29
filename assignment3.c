/**
 * File: assignment3.c
 *
 * Author: Anh Nguyen, Linh Nguyen
 * Date: , 2022
 * Course: COMP2510
 *
 * Summary of File:
 *
 * This file contains functions that .
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> /* memcpy */
#include <ctype.h> /* isdigit */

struct node {
    char * type;
    int base;
    int limit;
    struct node * next;
};

struct node * head = NULL;

/* Struct node for Linked List. */
int checkValue(char * type, int base, int limit);
void push(struct node** headPtr, struct node * temp);
void processFile(FILE * input);
void printList(struct node* node);
void mergeSort(struct node ** headPtr);
struct node * merge (struct node * first, struct node * second);
int main(int argc, char **argv);


int checkValue(char * type, int base, int limit) {
    if (base < 0 || limit <= 0) {
        // printf("error 1");
        return 0;
    }
    if (type[0] != 'P' && type[0] != 'H') {
        // printf("error 2");
        return 0;
    }
    if (strlen(type) > 1) {
        for (int i = 1; i < strlen(type); i++) {
            if (isdigit(type[i]) == 0) {
                // printf("error 3");
                return 0;
            }
        }
    }
    return 1;
}

void push(struct node** headPtr, struct node * temp) {
    struct node * newNode = malloc(sizeof(struct node));
    newNode->type = malloc(strlen(temp->type));
    strcpy(newNode->type, temp->type);
    newNode->base = temp->base;
    newNode->limit = temp->limit;
    newNode->next = (*headPtr);
    (*headPtr) = newNode;
}

void processFile(FILE * input) {
    // printf("alo\n");
    char buffer[100];
    
    while(fgets(buffer, 100, input) != NULL){
        // Create a new Node
        struct node * temp  = (struct node*) malloc(sizeof(struct node));
        sscanf(buffer, "%s %d %d", temp -> type, &temp -> base, &temp->limit);
        // Check all values before adding new node to the linked list
        if (checkValue(temp->type, temp->base, temp->limit) == 0) { 
            // If any value is invalid, exit the function
            printf("Invalid value!");
            return;
        }
        push(&head, temp);
    }
    printf("before sorting: \n");
    printList(head);
    mergeSort(&head);
    printf("after sorting: \n");
    printList(head);
}

void printList(struct node* node) {
    while (node != NULL) {
        printf("%s %d %d\n", node->type, node->base, node->limit);
        node = node->next;
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
    printf("alo: %s %d %d\n", first->type, first->base, first->limit);
    printf("     %s %d %d\n", second->type, second->base, second->limit);
    struct node * result = malloc(sizeof(struct node));
    struct node * temp = malloc(sizeof(struct node));
    
    result = temp;

    while (first != NULL && second != NULL) {
        // printf("hello1\n");
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
        // printf("hello2\n");
        temp->next = first;
        first = first->next;
        temp = temp->next;
    }

    while (second != NULL) {
        // printf("hello3\n");
        temp->next = second;
        second = second->next;
        temp = temp->next;
    }
    printf("merged successfully\n");
    return result->next;
}


/**
 * Main function
 *
 * @param argc argc
 *
 * @param argv argv
 *
 * @return result
 *
 */
int main(int argc, char **argv) {
    int option = 0;
    while (true) {
        printf("\n Choose your option based on the menu below: \n");
        printf("1) Load an input file\n");
        printf("2) Merge holes\n");
        printf("3) Compact memory\n");
        printf("4) Print memory view\n");
        printf("5) Exit the program\n");

        // Get and save the number the user types
        scanf("%d", &option);

        switch (option) {
            case 1: {
                char inputFile[300];
                printf("1) Type the file name: \n");
                scanf("%s", &inputFile);
                FILE *input;
                input = fopen(inputFile, "r");
                //Check if can not open a file, exit
                if (input == NULL) {
                    printf("Error: Can not open a file");
                    exit(1);
                }
                processFile(input);
                fclose(input);
                break;

            }
            case 2: {
//                printf("1) Type the file name: \n");
                break;

            }
            case 3: {
//                printf("1) Type the file name: \n");
                break;

            }
            case 4: {
//                printf("1) Type the file name: \n");
                break;

            }
            case 5: {
                exit(0);
                break;

            }
            default: {
                printf("Error: Invalid option");
                break;
            }
        }

    }
    return 0;
}
