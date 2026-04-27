#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void generateSyntheticData(void)
{
    FILE *fp;

    int RECORDS, maxSeat, confirmSeats = 0;

    printf("Enter the Number of Records to Generate: ");
    scanf("%d", &RECORDS);

    printf("Enter the Number of Confirm Seats: ");
    scanf("%d", &maxSeat);

    const char *names[] = {
        "Suraj ", "Amit ", "Rohit ", "Anjali ", "Priya ", "Neha ",
        "Chintu ", "Danish ", "Harsh ", "Sakshi ", "Aayushi ",
        "Inzamamul ", "Abhigyan ", "Dona ", "Rahul ", "Sagar ",
        "Raj ", "Samir ", "Sachin ", "Anirban ", "Anirudha "
    };

    const char *surnames[] = {
        "Ray", "Bhattacharjee", "Roy", "Yadav", "Chaudhary",
        "Ansari", "Kundu", "Mukherjee", "Singh", "Shaw",
        "Banerjee", "Mahato", "Das", "Ghosh", "Chauhan",
        "Khana", "Samanta", "Hazra"
    };

    const char *stations[] = {
        "Sealdah", "Howrah", "Raniganj",
        "Asansol", "Durgapur", "Bardhaman"
    };

    const char *classes[] = { "Lower", "Middle", "Higher" };

    fp = fopen("railway.csv", "w");
    if (fp == NULL)
    {
        printf("Error creating file.\n");
        return;
    }

    srand(time(NULL));

    for (int i = 0; i < RECORDS; i++)
    {
        int id = rand() % 9000000 + 1000000;
        int age = rand() % 43 + 18;

        char fullname[50];
        strcpy(fullname, names[rand() % 21]);
        strcat(fullname, surnames[rand() % 18]);

        const char *source = stations[rand() % 6];

        const char *destination;
        do {
            destination = stations[rand() % 6];
        } while (strcmp(source, destination) == 0);

        const char *class = classes[rand() % 3];
        const char *stat;

        if (confirmSeats < maxSeat)
        {
            stat = "CONFIRMED";
            confirmSeats++;
        }
        else
        {
            stat = "WAITING";
        }

        fprintf(fp, "%d,%d,%s,%s,%s,%s,%s\n",
                id, age, fullname, source, destination, class, stat);
    }

    fclose(fp);
    printf("Synthetic data generated successfully!\n");
}

int main(void)
{
    generateSyntheticData();
    return 0;
}
