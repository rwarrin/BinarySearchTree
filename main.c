/*******************************************************************************
 * main.c
 *
 * Binary Search Tree Test
 * Demonstrates creating, searching, and deleting a binary search tree.
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int mValue;
    struct Node *mLeft;
    struct Node *mRight;
} Node;

/**
 * Loads a list of numbers from a text file and stores then in a binray tree.
 */
bool LoadTreeValuesFromFile(Node **head, char *filename);

/**
 * Creates a new node and inserts it somewhere into the tree specified by head.
 */
bool AddNodeToTree(Node **head, int value);

/**
 * Deletes a tree pointed to by head and frees all allocated memory.
 **/
bool DeleteTree(Node **head);

/**
 * Searches the tree for the specified value.
 * Returns a pointer to the node containing the value, otherwise NULL.
 */
Node* FindInTree(Node **head, int value);

int main(int argc, char *argv[]) {
    // check for proper command line usage
    if(argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    // create a node pointer to track the head of the tree
    Node *head = NULL;
    
    // attempt to load values from a file into the tree
    bool success = false;
    success = LoadTreeValuesFromFile(&head, argv[1]);
    if(success == false)
        return 2;

    // create a node to access a found node
    Node *found = NULL;

    // value to find
    int find = 99;

    // attempt to find the value
    found = FindInTree(&head, find);
    if(found)
        printf("Found %d in list.\n", found->mValue);
    else
        printf("Couldn't find %d in list.\n", find);

    // attempt to find a new value
    find = 3;
    found = FindInTree(&head, find);
    if(found)
        printf("Found %d in list.\n", found->mValue);
    else
        printf("Couldn't find %d in list.\n", find);

    // clean up
    DeleteTree(&head);

    return 0;
}

/**
 * Opens the file filename and reads in values line by line.
 * Returns false if the file couldn't be opened.
 */
bool LoadTreeValuesFromFile(Node **head, char *filename) {
    FILE *file = fopen(filename, "r");
    if(file == NULL) {
        fprintf(stderr, "Couldn't open '%s' for reading.\n", filename);
        return false;
    }

    const int MAX_READ_CHARACTERS = 255;
    char line[MAX_READ_CHARACTERS];
    while(fgets(line, MAX_READ_CHARACTERS, file) != NULL) {
        printf("%d\n", atoi(line));
        AddNodeToTree(head, atoi(line));
    }

    fclose(file);
    return true;
}

/**
 * Attempts to add a new node containing the value value in the tree pointed to
 * by head. Creates a new node and initializes it with appropriate values. If
 * head is NULL then head is set to newNode. If head contains a list then the
 * tree is walked until the value is found in the list and newNode is deleted,
 * or newNode is able to be inserted into the list.
 * Returns true when a new node is successfully added to the tree.
 */
bool AddNodeToTree(Node **head, int value) {
    Node *newNode = NULL;
    newNode = malloc(sizeof(Node));
    if(newNode == NULL) {
        fprintf(stderr, "Couldn't allocate new node.\n");
        return false;
    }
    newNode->mValue = value;
    newNode->mLeft = NULL;
    newNode->mRight = NULL;

    if(*head == NULL) {
        *head = newNode;
        printf("Added head.\n");
        return true;
    }

    Node *walker = *head;
    while(true) {
        if(newNode->mValue == walker->mValue) {
            free(newNode);
            printf("- Node already exists.\n");
            return false;
        }
        else if(newNode->mValue < walker->mValue) {
            if(walker->mLeft == NULL) {
                walker->mLeft = newNode;
                printf("+ Added left node.\n");
                return true;
            }
            walker = walker->mLeft;
            printf("| Stepping left.\n");
        }
        else if(newNode->mValue > walker->mValue) {
            if(walker->mRight == NULL) {
                walker->mRight = newNode;
                printf("+ Added right node.\n");
                return true;
            }
            walker = walker->mRight;
            printf("| Stepping right.\n");
        }
    }

    return true;
}

/**
 * Deletes a tree pointed to by head freeing all allocated memory and sets
 * pointers to NULL. Uses a recursive method to free nodes from left to
 * right and bottom to top.
 * Returns true when all nodes have been freed.
 */
bool DeleteTree(Node **head) {
    if((*head)->mLeft != NULL) {
        printf("| Moving left.\n");
        DeleteTree(&(*head)->mLeft);
    }

    if((*head)->mRight != NULL) {
        printf("| Moving right.\n");
        DeleteTree(&(*head)->mRight);
    }

    if(*head != NULL) {
        printf("- Removing node.\n");
        free(*head);
        *head = NULL;
    }

    return true;
}

/**
 * Attempts to locate a node containing the value value in the tree pointed to
 * by head. If the value is not found in the tree NULL is returned, otherwise
 * a pointer to the node containing the value is returned.
 */
Node* FindInTree(Node **head, int value) {
    Node *node = *head;

    while(true) {
        if(node == NULL)
            break;

        if(value == node->mValue)
            return node;
        if(value < node->mValue)
            node = node->mLeft;
        if(value > node->mValue)
            node = node->mRight;
    }

    return NULL;
}
