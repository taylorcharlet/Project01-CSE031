#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variables
void printPuzzle(char** arr);
void searchPuzzle(char** arr, char* word);
//void searchPuzzle2(char** arr, char* word);
int bSize;
char letterIndex;
int wordLen;
int numLetters = 0;


typedef struct node {
    char letterNum;
    int row;
    int column;
    struct node* next;
    struct node* prev;

} node;

node* front = NULL;
node* back = NULL;

void appendNode(char letterNum, int row, int column) {
    node* newNode = (node*)malloc(sizeof(node));
    newNode->letterNum = letterNum;
    newNode->row = row;
    newNode->column = column;
    newNode->next = NULL;
    newNode->prev = NULL;

    if(front == NULL) {
        front = newNode;
        back = newNode;
    } else {
        back->next = newNode;
        newNode->prev = back;
        back = newNode;
    }


}

void removeLast() {
    if(back == NULL) {
        return;
    } else if(front == back) {
        free(back);
        front = NULL;
        back = NULL;
    } else {
        node* temp = back;
        back = back->prev;
        back->next = NULL;
        free(temp);
    }
}

void printList() {
    char*** row = (char***)malloc(bSize * sizeof(char**));
    for(int i = 0; i < bSize; i++) {
        *(row + i) = (char**)malloc(bSize * sizeof(char*));
        for(int j = 0; j < bSize; j++) {
            *(*(row+ i) + j) = calloc(8, sizeof(char));
            strcpy(*(*(row+ i) + j), "0");
        }
    }

    node* curr = front;
    while(curr != NULL) {
        //printf("%c: (%d, %d)\n", curr->letterNum, curr->row, curr->column);
        //(*(*(row + curr->row) + curr->column)) = curr->letterNum;
        char indexValue[2] = {curr->letterNum + '\0'};
        if(strcmp((*(*(row + curr->row) + curr->column)), "0") == 0) {
            strcpy((*(*(row + curr->row) + curr->column)), indexValue);
        } else {
            strcat((*(*(row + curr->row) + curr->column)), indexValue);
        }

        curr = curr->next;
    }

    printf("Printing the search path:\n");
    for(int i = 0; i < bSize; i++) {
        for(int j = 0; j < bSize; j++) {
            printf("%-10s ", *(*(row + i) + j));
        }
        printf("\n");
    }

    for (int i = 0; i < bSize; i++) {
        for (int j = 0; j < bSize; j++) {
            free(*(*(row + i) + j));   
        }
        free(*(row + i));              
    }
    free(row);
    
}

char* toUpperCase(char* word) {
    for(int i = 0; *(word + i); i++) {
        if(*(word + i) >= 'a' && *(word + i) <= 'z') {
            *(word + i) = *(word + i) - 32;
        }
    }
    return word;
}

int wordLength(char* word) {
    int length = 0;
    for(int i = 0; *(word + i); i++) {
        length++;
    }
    return length;
}

/*
int searchAround2(char** arr, node* currentNode, char* word) {
    //printf("numLetters is : %d and wordLen is : %d and word is : %s \n", numLetters, wordLen, word);
    if(numLetters == wordLen) {
        return 1;
    }
    int row = currentNode->row;
    int column = currentNode->column;
    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {
            if(row + i >= 0 && row + i < bSize && column + j >= 0 && column + j < bSize) {
                //printf("Checking around (%d, %d)\n", row + i, column + j);
                if(*(*(arr + row + i) + column + j) == *(word) && (!(i == 0 && j == 0))) {
                    numLetters++;
                    appendNode(numLetters + '0', row + i, column + j);
                    //printf("Found the %d letter of the word %c at (%d, %d)\n", numLetters, *(word), row + i, column + j);
                    //printf("Searching Around %c \n", currentNode->letterNum);
                    //printList();
                    if(searchAround2(arr, back, word + 1)) {
                        return 1;
                    }
                }
            }
        }
    }

    removeLast();
    numLetters--;
    return 0;
    
}  
*/

int searchAround(char** arr, int row, int column, int wordIndex, char* word) {
    if(wordIndex == wordLen) {
        return 1;
    }

    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {
            if(row + i >= 0 && row + i < bSize && column + j >= 0 && column + j < bSize) {
                if((*(*(arr + (row + i)) + (column + j)) == *(word + wordIndex)) && (!(i == 0 && j == 0))) {
                    letterIndex = wordIndex + 1 + '0';
                    appendNode(letterIndex, row + i, column + j);
                    if(searchAround(arr, row + i, column + j, wordIndex + 1, word)) {
                        return 1;
                    }
                }
            }
        }
    }

    removeLast();
    return 0;
    
}  


// Main function, DO NOT MODIFY 	
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <puzzle file name>\n", argv[0]);
        return 2;
    }
    int i, j;
    FILE *fptr;

    // Open file for reading puzzle
    fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        printf("Cannot Open Puzzle File!\n");
        return 0;
    }

    // Read the size of the puzzle block
    fscanf(fptr, "%d\n", &bSize);
    
    // Allocate space for the puzzle block and the word to be searched
    char **block = (char**)malloc(bSize * sizeof(char*));
    char *word = (char*)malloc(20 * sizeof(char));

    // Read puzzle block into 2D arrays
    for(i = 0; i < bSize; i++) {
        *(block + i) = (char*)malloc(bSize * sizeof(char));
        for (j = 0; j < bSize - 1; ++j) {
            fscanf(fptr, "%c ", *(block + i) + j);            
        }
        fscanf(fptr, "%c \n", *(block + i) + j);
    }
    fclose(fptr);

    printf("Enter the word to search: ");
    scanf("%19s", word);
    
    // Print out original puzzle grid
    printf("\nPrinting puzzle before search:\n");
    printPuzzle(block);
    
    // Call searchPuzzle to the word in the puzzle
    searchPuzzle(block, word);
    
     // Free allocated memory
    for (i = 0; i < bSize; i++)
        free(*(block + i));
    free(block);
    free(word);

    return 0;
}

void printPuzzle(char** arr) {
	// This function will print out the complete puzzle grid (arr). 
    // It must produce the output in the SAME format as the samples 
    // in the instructions.
    // Your implementation here...

    for(int i = 0; i < bSize; i++) {
        for(int j = 0; j < bSize; j++) {
            printf("%c ", *(*(arr + i) + j));
        }
        printf("\n");
    }

    printf("\n");

}

void searchPuzzle(char** arr, char* word) {
    // This function checks if arr contains the search word. If the 
    // word appears in arr, it will print out a message and the path 
    // as shown in the sample runs. If not found, it will print a 
    // different message as shown in the sample runs.
    // Your implementation here...

    for(int row = 0; row < bSize; row++) {
        for(int column = 0; column < bSize; column++) {
            if(*(*(arr + row) + column) >= 'a' && *(*(arr + row) + column) <= 'z') {
                *(*(arr + row) + column) = *(*(arr + row) + column) - 32;
            }
        }
    }

    word = toUpperCase(word);
    wordLen = wordLength(word);
    int wordsFound = 0;
    
    for(int row = 0; row < bSize; row++) {
        for(int column = 0; column < bSize; column++) {
            if(*(*(arr + row) + column) == *word) {
                letterIndex = '1';
                appendNode(letterIndex, row, column);

                if(searchAround(arr, row, column, 1, word)) {
                    wordsFound++;
                }
            }
        }
    }

    if(wordsFound > 0) {
        printf("Word found!\n");
        printList();
    } else {
        printf("Word not found!");
    }

    //free all storage
    node* temp = front;
    while (temp != NULL) {
        node* next = temp->next;
        free(temp);
        temp = next;
    }
    front = NULL;
    back = NULL;

}

/*
void searchPuzzle2(char** arr, char* word) {

    // This function checks if arr contains the search word. If the 
    // word appears in arr, it will print out a message and the path 
    // as shown in the sample runs. If not found, it will print a 
    // different message as shown in the sample runs.
    // Your implementation here...

    word = toUpperCase(word);
    wordLen = wordLength(word);
    int wordsFound = 0;

    //printf("Word length: %d\n", wordLen);

    

    for(int row = 0; row < bSize; row++) {
        for(int column = 0; column < bSize; column++) {
            if(*(*(arr + row) + column) >= 'a' && *(*(arr + row) + column) <= 'z') {
                *(*(arr + row) + column) = *(*(arr + row) + column) - 32;
            }
        }
    }

    
    
    for(int row = 0; row < bSize; row++) {
        for(int column = 0; column < bSize; column++) {
            if(*(*(arr + row) + column) == *word) {
                //printf("Found the first letter of the word %c at (%d, %d)\n", *word, row, column);
                numLetters++;
                letterIndex = '1';
                appendNode(letterIndex, row, column);

                if(searchAround2(arr, back, word + 1)) {
                    wordsFound++;
                    //record path and keep searching
                }

                numLetters = 0;

                //printList();

            }
        }
    }

    

    if(wordsFound > 0) {
        //print out all info assignment needs (all paths found)
        printList();
    }

    node* temp = front;
    while (temp != NULL) {
        node* next = temp->next;
        free(temp);
        temp = next;
    }
    front = NULL;
    back = NULL;
    //free all storage

}
*/
