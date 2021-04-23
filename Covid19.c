#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NAME_LEN 19
#define PLACE_LEN 9
#define MAX_TRAVELS 5

#define DEBUG_MODE 1

struct user {
   char name[NAME_LEN+1];
   char places[MAX_TRAVELS][PLACE_LEN+1];
   struct user *next;
};

struct user *head = NULL;
struct user *current = NULL;

int usersCount() {
    int n = 0;
    for(current = head; current != NULL; current = current->next) {
        n++;
    }
    return n;
}

void saveFile() {
    int count = usersCount();
    FILE *fptr = fopen("Covid19.txt","w");
    fprintf(fptr, "%d\n", count);
    if(head != NULL)
    {
        int i;
        struct user* temp = head;
        while(temp->next != NULL)
                {
                    fprintf(fptr, "%s\n", temp->name);
                    for(i=0;i<MAX_TRAVELS;i++)
                    {
                            fprintf(fptr, "%s\n", temp->places[i]);
                    }
                    temp=temp->next;
                }
                fprintf(fptr, "%s\n", temp->name);
                for(i=0;i<MAX_TRAVELS;i++)
                    {
                            fprintf(fptr, "%s\n", temp->places[i]);
                    }
    }
    fclose(fptr);
}

void readFile()
{
    FILE *fptr = fopen("Covid19.txt","r");
    int n,j;
    fscanf(fptr, "%d", &n);
    head = NULL;
    if(n != 0)
    {
        int i;
        struct user* temp;
        for(i=0;i<n;i++)
        {
            temp = malloc(sizeof(struct user));
            temp->next = head;
            head = temp;
        }
        for(i=0; i<n; i++,temp = temp->next) {
            fscanf(fptr, "%s", temp->name);
            char dummy = fgetc(fptr); //to scan the unnecessary '\n'
            char tempPlace[PLACE_LEN];
            for(j=0;j<MAX_TRAVELS;j++)
            {
                fgets(tempPlace, PLACE_LEN, fptr);
                if(tempPlace[0] == '\n'){
                    temp->places[j][0]='\0';
                }
                else {
                    int k=0;
                    strcpy(temp->places[j], tempPlace);
                    while(temp->places[j][k] != '\n')
                        k++;
                    temp->places[j][k] = '\0'; //removing '\n' that appeared in place name while scanning using fgets()
                }
            }
        }
    }
    fclose(fptr);
}

void printList() {
    struct user *temp = head;
    printf("\n[ ");
    while(temp != NULL) {
        printf("%s ", temp->name);
        temp = temp->next;
    }
    printf("]\n");
}

struct user* find(char* name) {
    current = head;
    while(strcmp(current->name, name)) {

        //if it is last user
        if(current->next == NULL) {
        printf("\nNo such user found\n");
        return NULL;
        }
        else {
            current = current->next;
        }
    }
    return current;
}

void removeUser(char* name) {
    current = head;
    struct user* previous = NULL;
    while(strcmp(current->name, name)) {

        //if it is last user
        if(current->next == NULL) {
        printf("\nNo such user found\n");
        return;
        }
        else {
            previous = current;
            current = current->next;
        }
    }
    //found a match
    if(current == head)
        head = head->next;
    else
        previous->next = current->next;
    free(current);
    printList();
}

void insertSorted(char *name) {
    int i;
    struct user *temp;
    temp = malloc(sizeof(struct user));
    strcpy(temp->name, name);
    temp->next = NULL;

    //Giving empty strings to places array
    for(i=0;i<MAX_TRAVELS;i++)
    {
        temp->places[i][0]='\0';
    }

    //if list is empty
    if (head == NULL) {
        head = temp;
    }

    //for non-empty list
    else {
        struct user *currptr = head->next;
        struct user *previous = head;
        if (strcmp(head->name, temp->name)>0) {
            temp->next = head;
            head = temp;
        }
        else {
            while (currptr != NULL) {
                if (strcmp(currptr->name, temp->name)>0) {
                    previous->next = temp;
                    temp->next = currptr;
                    break;
                }
                previous = currptr;
                currptr = currptr->next;
            }
            if(currptr == NULL)
                previous->next = temp;
            }
        }
    printList();
}

#if DEBUG_MODE
void visitedPlaces(struct user* curr){
    int i;
    printf("\nPlaces visited by %s: [ ",curr->name);
    for(i=0;i<MAX_TRAVELS;i++)
        if(curr->places[i][0] != '\0')
            printf("%s ",curr->places[i]);
    printf("]\n");

}
#endif

void main(int argc, char *argv[]) {
    char name[NAME_LEN];
    char place[PLACE_LEN];
    int choice;
    do {
        printf("\n-------MENU-------\n1. Introduce user\n2. Introduce visit to venue\n3. Remove user\n4. Check which users have been in a particular place\n5. Save to file\n6. Retrieve data from file\n7. Exit\nEnter Your Choice: ");
        scanf("%d",&choice);
        switch(choice)
        {
        case 1: printf("Enter user Name: ");
                scanf("%s", name);
                insertSorted(name);
                break;
        case 2: if(head == NULL) {
                    printf("\nList is Empty. Add users first\n");
                    break;
                }
                printf("Enter venue: ");
                scanf("%s", place);
                printf("Enter visitor name: ");
                scanf("%s", name);

                //check user exists
                struct user* curr = find(name);
                if(curr != NULL)
                {
                int i;
                for(i=0;i<MAX_TRAVELS;i++)
                {
                    if(curr->places[i][0] != '\0')
                        continue;
                    else
                        break;
                }
                strcpy(curr->places[i], place);
                #if DEBUG_MODE
                visitedPlaces(curr);
                #endif
                }
                break;
        case 3: if(head == NULL) {
                    printf("\nList is Empty\n");
                    break;
                }
                printf("Enter user Name: ");
                scanf("%s", name);
                removeUser(name);
                break;
        case 4: //char place[PLACE_LEN];
                printf("Enter venue: ");
                scanf("%s", place);
                printf("\nVisited by: [ ");
                if(head != NULL)
                {
                struct user* temp = head;
                int i;
                //if only one user in list
                while(temp->next != NULL)
                {
                    for(i=0;i<MAX_TRAVELS;i++)
                    {
                        if(strcmp(temp->places[i], place) == 0)
                            {printf("%s ", temp->name);
                            break;}
                    }
                    temp=temp->next;
                }
                for(i=0;i<MAX_TRAVELS;i++)
                    {
                        if(strcmp(temp->places[i], place) == 0)
                            {printf("%s ", temp->name);
                            break;}
                    }
                }
                printf("]\n");
                break;
        case 5: saveFile();
                printf("Saved as Covid19.txt in current directory\n");
                break;
        case 6: printf("Retrieved Successfully");
                readFile();
                printList();
                break;
        case 7: exit(0);
                break;
        default: printf("\nEnter valid choice\n");
                 break;
        }
    } while(choice != 7);
}
