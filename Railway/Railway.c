#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>
static int ID=100;


typedef struct Passenger {
    int id, age;
    char name[50], src[20], dest[20], berth[10], status[20];
    struct Passenger *next;
} Passenger;



Passenger *confirmedHead = NULL;
Passenger *waitingHead   = NULL;

int Confirmed = 0, Waiting = 0;
int maxSeats = 0;

#define DATA_FILE "railway.csv"   



Passenger *create_Pass(int id, int age, char *name,
                       char *src, char *dest,
                       char *berth, char *status)
{
    Passenger *p = malloc(sizeof(Passenger));
    p->id = id;
    p->age = age;
    strcpy(p->name, name);
    strcpy(p->src, src);
    strcpy(p->dest, dest);
    strcpy(p->berth, berth);
    strcpy(p->status, status);
    p->next = p;
    return p;
}

void insert_Pass(Passenger **head, Passenger *newPas)
{
    if (*head == NULL) {
        *head = newPas;
        newPas->next = newPas;
        return;
    }

    Passenger *temp = *head;
    while (temp->next != *head)
        temp = temp->next;

    temp->next = newPas;
    newPas->next = *head;
}

Passenger *search_Pass(Passenger *head, int id)
{
    if (!head) return NULL;

    Passenger *temp = head;
    do {
        if (temp->id == id)
            return temp;
        temp = temp->next;
    } while (temp != head);

    return NULL;
}

void searchPassenger()
{
    int id;
    char name[50];

    scanf("%d", &id);
    getchar();  // clear newline after scanf

    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;  // remove newline

    int found = 0;

    /* SEARCH CONFIRMED LIST */
    if (confirmedHead) {
        Passenger *tempC = confirmedHead;
        do {
            if ((id != 0 && tempC->id == id) ||
                (strlen(name) > 0 && strcmp(name, tempC->name) == 0)) {

                printf("%d,%d,%s,%s,%s,%s,%s\n",
                       tempC->id, tempC->age, tempC->name,
                       tempC->src, tempC->dest,
                       tempC->berth, tempC->status);
                found = 1;
            }
            tempC = tempC->next;
        } while (tempC != confirmedHead);
    }

    /* SEARCH WAITING LIST */
    if (waitingHead) {
        Passenger *tempW = waitingHead;
        do {
            if ((id != 0 && tempW->id == id) ||
                (strlen(name) > 0 && strcmp(name, tempW->name) == 0)) {

                printf("%d,%d,%s,%s,%s,%s,%s\n",
                       tempW->id, tempW->age, tempW->name,
                       tempW->src, tempW->dest,
                       tempW->berth, tempW->status);
                found = 1;
            }
            tempW = tempW->next;
        } while (tempW != waitingHead);
    }

    if (!found) {
        printf("NOT_FOUND\n");
    }
}



void delete_Pass(Passenger **head, int id, int flag)
{
    if (*head == NULL) return;

    Passenger *curr = *head, *prev = NULL;

    do {
        if (curr->id == id) {
            if (curr->next == curr) {
                *head = NULL;
            } else {
                if (curr == *head) {
                    Passenger *last = *head;
                    while (last->next != *head)
                        last = last->next;
                    *head = curr->next;
                    last->next = *head;
                } else {
                    prev->next = curr->next;
                }
            }
            if(flag){
                FILE *fp=fopen("history.csv","a");
                if(fp==NULL) return;
                time_t current_time;
                time(&current_time);
                fprintf(fp, "%d,%d,%s,%s,%s,%s,%s,%s",
                    curr->id, curr->age, curr->name,
                    curr->src, curr->dest, curr->berth, curr->status,ctime(&current_time));
                fclose(fp);
            }
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    } while (curr != *head);
}



void saveCSV(FILE* fp, Passenger* head)
{
    if (!head) return;

    Passenger* temp = head;
    do {
        fprintf(fp, "%d,%d,%s,%s,%s,%s,%s\n",
                temp->id, temp->age, temp->name,
                temp->src, temp->dest, temp->berth, temp->status);
        temp = temp->next;
    } while (temp != head);
}

void saveToFile()
{
    FILE *fp = fopen(DATA_FILE, "w");
    if (!fp) return;

    saveCSV(fp, confirmedHead);
    saveCSV(fp, waitingHead);

    fclose(fp);
}

void loadFromFile()
{
    FILE *fp = fopen(DATA_FILE, "r");
    if (!fp) return;

    int id, age;
    char name[50], src[20], dest[20], berth[10], status[20];

    while (fscanf(fp,
        "%d,%d,%49[^,],%19[^,],%19[^,],%9[^,],%19[^\n]\n",
        &id, &age, name, src, dest, berth, status) == 7)
    {
        Passenger* p = create_Pass(id, age, name, src, dest, berth, status);

        if (strcmp(status, "CONFIRMED") == 0) {
            insert_Pass(&confirmedHead, p);
            Confirmed++;
        } else {
            insert_Pass(&waitingHead, p);
            Waiting++;
        }
    }
    ID = ++id;
    fclose(fp);
}



void addPassenger()
{
    int id, age;
    char name[50], src[20], dest[20], berth[10];

    id=ID++;  
    getchar();

    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    scanf("%d", &age);
    getchar();

    fgets(src, sizeof(src), stdin);
    src[strcspn(src, "\n")] = 0;

    fgets(dest, sizeof(dest), stdin);
    dest[strcspn(dest, "\n")] = 0;

    fgets(berth, sizeof(berth), stdin);
    berth[strcspn(berth, "\n")] = 0;

    if (Confirmed < maxSeats) {
        Passenger *p = create_Pass(id, age, name, src, dest, berth, "CONFIRMED");
        insert_Pass(&confirmedHead, p);
        Confirmed++;
    } else {
        Passenger *p = create_Pass(id, age, name, src, dest, berth, "WAITING");
        insert_Pass(&waitingHead, p);
        Waiting++;
    }

    saveToFile();
}


void cancelPassenger()
{
    int id;
    scanf("%d", &id);

    if (search_Pass(confirmedHead, id)) {
        delete_Pass(&confirmedHead, id,1);
        Confirmed--;
    }
    else if (search_Pass(waitingHead, id)) {
        delete_Pass(&waitingHead, id,1);
        Waiting--;
        saveToFile();
        printf("CANCEL_SUCCESS\n");
        return;
    }
    else {
        printf("Invalid\n");
        return;
    }

    if (waitingHead) {
        Passenger *p = waitingHead;

        Passenger *newP = create_Pass(
            p->id, p->age, p->name,
            p->src, p->dest, p->berth,
            "CONFIRMED"
        );

        delete_Pass(&waitingHead, p->id,0);
        Waiting--;

        insert_Pass(&confirmedHead, newP);
        Confirmed++;
    }

    saveToFile();
    printf("CANCEL_SUCCESS\n");
}



int main()
{
    int choice;
    loadFromFile();

    while (scanf("%d", &choice) == 1) {
        switch (choice) {
            case 1: addPassenger(); break;
            case 2: cancelPassenger(); break;
            case 3: searchPassenger(); break;  
            case 5: saveToFile(); return 0;
        }
    }
    return 0;
}

