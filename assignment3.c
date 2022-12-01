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
#include <stdbool.h>
#include "file_handler.h"

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
    struct node * head = NULL;
    while (true) {
        printf("\n Choose your option based on the menu below: \n");
        printf("1) Load an input file\n");
        printf("2) Merge holes\n");
        printf("3) Compact memory\n");
        printf("4) Print memory view\n");
        printf("5) Exit the program\n");
        printf("Option: ");
        // Get and save the number the user types
        scanf("%d", &option);

        switch (option) {
            case 1: {
                char inputFile[300];
                head =  NULL;
                printf("1) Type the file name: ");
                scanf("%s", &inputFile);
                FILE *input;
                input = fopen(inputFile, "r");
                //Check if can not open a file, exit
                if (input == NULL) {
                    printf("Error: Can not open a file");
                    exit(1);
                }
                head = processFile(input);
                fclose(input);
                break;
            }
            case 2: {
                mergeHoles(head);
                printf("Operation 2 successful!!! \n Select another option: \n");
                break;

            }
            case 3: {
                compaction(head);
                printf("Operation 3 successful!!! \n Select another option: \n");
                break;

            }
            case 4: {
                printList(head);
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
