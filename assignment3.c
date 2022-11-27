#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> /* memcpy */

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
//                processFile();
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

        return 0;
    }
}
