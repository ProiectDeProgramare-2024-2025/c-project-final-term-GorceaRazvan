#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Product {
    char nume[100];
    int cantitate;
    double pret;
} produse[100];

struct zi {
    char nrzi[100];
    struct Product produse[100];
    int products_count;
} zile[31];

int zile_nr;
int zi_curenta = 1;

void citire_fisier() {
    system("cls");
    FILE* products_file = fopen("istoric.txt", "r");
    for (int j = 0; j < 31; j++) {
        if (!fgets(zile[j].nrzi, 100, products_file)) break;
        zile[j].nrzi[strcspn(zile[j].nrzi, "\n")] = '\0';

        if (fscanf(products_file, "%d", &zile[j].products_count) != 1) break;
        fgetc(products_file);

        for (int i = 0; i < zile[j].products_count; i++) {
            if (!fgets(zile[j].produse[i].nume, 100, products_file)) break;
            zile[j].produse[i].nume[strcspn(zile[j].produse[i].nume, "\n")] = '\0';

            if (fscanf(products_file, "%d %lf", &zile[j].produse[i].cantitate, &zile[j].produse[i].pret) != 2) break;
            fgetc(products_file);
        }
    }
    fclose(products_file);
}

void scriere_fisier() {
    FILE *products_file = fopen("istoric.txt", "w");
    if (!products_file) {
        perror("Eroare la deschiderea fisierului pentru scriere");
        return;
    }

    char s[100];
    int c;
    double p;
    while ((getchar()) != '\n');

    printf("Introdu numele produsului: ");
    fgets(s, 100, stdin);
    s[strcspn(s, "\n")] = '\0';

    printf("Introdu cantitatea: ");
    scanf("%d", &c);
    printf("Introdu pretul: ");
    scanf("%lf", &p);

    strcpy(zile[zi_curenta - 1].produse[zile[zi_curenta - 1].products_count].nume, s);
    zile[zi_curenta - 1].produse[zile[zi_curenta - 1].products_count].cantitate = c;
    zile[zi_curenta - 1].produse[zile[zi_curenta - 1].products_count].pret = p;
    zile[zi_curenta - 1].products_count++;

    for (int i = 0; i < 31; i++) {
        if (strlen(zile[i].nrzi) == 0) continue;

        fprintf(products_file, "%s\n", zile[i].nrzi);
        fprintf(products_file, "%d\n", zile[i].products_count);
        for (int j = 0; j < zile[i].products_count; j++) {
            fprintf(products_file, "%s\n%d %.2f\n", zile[i].produse[j].nume, zile[i].produse[j].cantitate, zile[i].produse[j].pret);
        }
    }

    fclose(products_file);
}

void istoric_zi() {
    for (int j = 0; j < zile[zi_curenta - 1].products_count; j++) {
        printf("\033[1;36m%s\033[0m\n%d %.2f \033[1;37mRON\033[0m\n",
               zile[zi_curenta - 1].produse[j].nume,
               zile[zi_curenta - 1].produse[j].cantitate,
               zile[zi_curenta - 1].produse[j].pret);
    }
}

void profit_zi() {
    double b = 0;
    for (int j = 0; j < zile[zi_curenta - 1].products_count; j++)
        b += zile[zi_curenta - 1].produse[j].pret;

    printf("\033[1;31mCantitatea\033[0m de bani castigata astazi \033[1;36m%.2f\033[0m RON\n", b);
}

void istoric_zi_alegere() {
    printf("Precizeaza \033[1;36mziua\033[0m (1-31): ");
    int zi_curenta_alegere;
    scanf("%d", &zi_curenta_alegere);
    if(zi_curenta_alegere>31 || zi_curenta_alegere<1) printf("\033[1;36mZiua\033[0m nu este in intervalul specificat\n");
    for (int j = 0; j < zile[zi_curenta_alegere - 1].products_count; j++) {
        printf("\033[1;36m%s\033[0m\n%d %.2f \033[1;37mRON\033[0m\n",
               zile[zi_curenta_alegere - 1].produse[j].nume,
               zile[zi_curenta_alegere - 1].produse[j].cantitate,
               zile[zi_curenta_alegere - 1].produse[j].pret);
    }
}

void profitabila_zi() {
    double max = 0;
    int nume = 1;
    for (int j = 0; j < 31; j++) {
        double bani = 0;
        for (int i = 0; i < zile[j].products_count; i++)
            bani += zile[j].produse[i].pret;

        if (bani > max) {
            max = bani;
            nume = j + 1;
        }
    }
    printf("Cea mai profitabila zi este ziua \033[0;35m%d\033[0m cu un profit de %.2f \033[1;31mRON\033[0m\n", nume, max);
}

void cel_mai_vandut() {
    struct {
        char nume[100];
        int total_cantitate;
    } produse_vandute[1000];
    int nr_produse = 0;

    for (int i = 0; i < 31; i++) {
        for (int j = 0; j < zile[i].products_count; j++) {
            char* nume_curent = zile[i].produse[j].nume;
            int cantitate = zile[i].produse[j].cantitate;
            int gasit = 0;
            for (int k = 0; k < nr_produse; k++) {
                if (strcmp(produse_vandute[k].nume, nume_curent) == 0) {
                    produse_vandute[k].total_cantitate += cantitate;
                    gasit = 1;
                    break;
                }
            }
            if (!gasit) {
                strcpy(produse_vandute[nr_produse].nume, nume_curent);
                produse_vandute[nr_produse].total_cantitate = cantitate;
                nr_produse++;
            }
        }
    }

    int max_idx = 0;
    for (int i = 1; i < nr_produse; i++) {
        if (produse_vandute[i].total_cantitate > produse_vandute[max_idx].total_cantitate)
            max_idx = i;
    }

    if (nr_produse > 0) {
        printf("Cel mai vandut produs: \033[0;35m%s\033[0m (\033[0;31m%d\033[0m bucati vandute)\n",
               produse_vandute[max_idx].nume,
               produse_vandute[max_idx].total_cantitate);
    } else {
        printf("Nu exista produse vandute.\n");
    }
}

void media_cantitatilor() {
    double medie = 0;
    int c = 0;
    for (int j = 0; j < 31; j++) {
        for (int i = 0; i < zile[j].products_count; i++) {
            medie += zile[j].produse[i].cantitate;
            c++;
        }
    }
    if (c > 0)
        printf("Media cantitatii este \033[0;31m%.2f\033[0m\n", medie / c);
    else
        printf("Nu sunt date suficiente pentru a calcula media.\n");
}

void alegere_zi() {
    printf("Alege \033[0;31mziua\033[0m (1-31): ");
    scanf("%d", &zi_curenta);
    if(zi_curenta>31 || zi_curenta<1){ system("cls");printf("\033[1;33mZiua\033[0m nu este in intervalul precizat\n");}
    else{
    system("cls");
    printf("Zi selectata cu succes.\n");
    zi_curenta=1;}
}

int main() {
    while (1) {
        printf("\033[0;36m-=\033[0mSistem inregistrare vanzari fastfood\033[0;36m=-\033[0m\n");
        printf("\033[1;31m1.\033[0mRezumatul zilei\n");
        printf("\033[1;33m2.\033[0mIstoric pe zile\n");
        printf("\033[0;32m3.\033[0mStatistici pe toate zilele\n");
        printf("\033[0;35m4.\033[0mAdaugare\n");
        printf("\033[1;31m5.\033[0mAlege ziua\n");
        printf("\033[1;33m6.\033[0mIesire\n");
        int choice = 0;
        while (choice < 1 || choice > 6) {
            printf("Introduceti optiunea dorita: ");
            scanf("%d", &choice);
        }
        system("cls");

        if (choice == 1) {
            while (1) {
                printf("\033[0;36m-=\033[0mRezumatul zilei\033[0;36m=-\033[0m\n");
                printf("\033[1;31m1.\033[0mIstoric vanzari efectuate astazi\n");
                printf("\033[1;33m2.\033[0mCantitatea de bani castigata astazi\n");
                printf("\033[1;35m3.\033[0mInapoi\n");
                int choice2 = 0;
                while (choice2 < 1 || choice2 > 3) {
                    printf("Introduceti optiunea dorita: ");
                    scanf("%d", &choice2);
                }
                if (choice2 == 3) { system("cls"); break; }
                citire_fisier();
                if (choice2 == 1) {
                    system("cls");
                    printf("\033[1;35mIstoric\033[0m pe \033[1;31mziua %d\033[0m\n", zi_curenta);
                    istoric_zi();
                }
                if (choice2 == 2) {
                    profit_zi();
                }
            }
        }

        if (choice == 2) {
            citire_fisier();
            istoric_zi_alegere();
        }

        if (choice == 3) {
            while (1) {
                printf("\033[0;36m-=\033[0mStatistici pe toate zilele\033[0;36m=-\033[0m\n");
                printf("\033[0;36m1.\033[0mCea mai profitabila zi\n");
                printf("\033[1;31m2.\033[0mCel mai vandut produs\n");
                printf("\033[1;33m3.\033[0mMedia cantitati\n");
                printf("\033[1;34m4.\033[0mIesire\n");
                int choice3 = 0;
                while (choice3 < 1 || choice3 > 4) {
                    printf("Introduceti optiunea dorita: ");
                    scanf("%d", &choice3);
                }
                if (choice3 == 4) { system("cls"); break; }
                citire_fisier();
                if (choice3 == 1) profitabila_zi();
                if (choice3 == 2) cel_mai_vandut();
                if (choice3 == 3) media_cantitatilor();
            }
        }

        if (choice == 4) {
            citire_fisier();
            scriere_fisier();
        }

        if (choice == 5) alegere_zi();
        if (choice == 6) break;
    }

    return 0;
}
