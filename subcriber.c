#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include"book.h"
#include"date.h"
#include"str_manip.h"
#include "subcriber.h"
#define DATA_FOLDER       "data"
#define BOOKS_FILE        DATA_FOLDER "/books.bin"
#define SUBS_FILE         DATA_FOLDER "/subscribers.bin"
#define SUB_ACT_FOLDER    DATA_FOLDER "/Sub_act"
#define SUB_HIST_FOLDER   DATA_FOLDER "/Sub_hist"

//creates an subscriber
subscriber create_sub(char n[], char i[], date b) {
    subscriber s;
    char hist_path[1001];
    char act_path[1001];

    /* --- SÉCURITÉ : On enlève le \n s'il est passé par erreur --- */
    i[strcspn(i, "\n")] = 0;
    n[strcspn(n, "\n")] = 0;
    /* ------------------------------------------------------------ */

    strcpy(s.name, n);
    strcpy(s.id, i);
    s.birth = b;

    /* Construction des chemins */
    sprintf(hist_path, "%s/%s.txt", SUB_HIST_FOLDER, i);
    sprintf(act_path,  "%s/%s.bin", SUB_ACT_FOLDER, i);

    strcpy(s.histo, hist_path);
    strcpy(s.activity, act_path);

    /* Création des fichiers vides */
    FILE *fh = fopen(s.histo, "w"); // "w" pour texte
    if (fh) fclose(fh);
    else perror("Erreur creation historique");

    FILE *fa = fopen(s.activity, "wb"); // "wb" pour binaire
    if (fa) fclose(fa);
    else perror("Erreur creation activite");

    return s;
}

// updates the subscriber's history in the text file
void update_history(subscriber sub , char act[]){
FILE *f = fopen(sub.histo , "a");
strcat(act , "\n");
fprintf(f, "%s", act);
fclose(f);
return;
}
//creates a activity instance
activity create_activity(action ac , char book[] , date res , date dead ){
activity x;
x.ac=ac;
strcpy(x.book , book);
copy_date(&x.res_date , res);
copy_date(&x.dead_date , dead);
switch(x.ac){
case RESERVED:
    x.echec=false;
    break;
case RETURNED:
    if (compare(res , dead)==C_GREATER){
        x.echec=true;
    }
    else{
       x.echec=false;
    }
    break;
}
return(x);
}
// updates the subscriber's activity folder
void update_activity(subscriber sub , activity ac){
    FILE *f = fopen(sub.activity, "ab");
    if (!f) {
        perror("Erreur ouverture fichier activité");
        return;
    }

    if (fwrite(&ac, sizeof(activity), 1, f) != 1) {
        perror("Erreur écriture activité");
    }

    fclose(f);
}

// reserves a book for a subscriber
void reserve_book_sub(book *b , subscriber s){
book b1;
char act[1001]="";
date res_d;
date dead_d;
char f1[1001]="";
char f2[1001]="";
copy_book(&b1 , *b);
bool yes=reserve_book(b);
if (yes==true){
    strcat(act ,"Réservé ");
    strcat(act , b->title );
    strcat(act , "le ");
    printf("Entrez la date de réservation:\n");
    printf("Jour:");
    scanf("%d", &res_d.day);
    printf("Mois:");
    scanf("%d", &res_d.month);
    printf("Année:");
    scanf("%d", &res_d.year);
    while(ver_cor_form(res_d)==false){
    printf("Format de données incorrect !\n");
    printf("Entrez la date de réservation:\n");
    printf("Jour:");
    scanf("%d", &res_d.day);
    printf("Mois:");
    scanf("%d", &res_d.month);
    printf("Année:");
    scanf("%d",& res_d.year);
    }
    repr_date(res_d , f1);
    printf("Date de réservation: %s \n", f1);
    strcat(act , f1 );
    strcat(act ," ");
    //-------------------------------------
    printf("Entrez la date limite:\n");
    printf("Jour:");
    scanf("%d", &dead_d.day);
    printf("Mois:");
    scanf("%d", &dead_d.month);
    printf("Année:");
    scanf("%d",& dead_d.year);
    while(ver_cor_form(dead_d)==false){
    printf("Format de données incorrect !\n");
    printf("Entrez la date de limie:\n");
    printf("Jour:");
    scanf("%d", &dead_d.day);
    printf("Mois:");
    scanf("%d", &dead_d.month);
    printf("Année:");
    scanf("%d", &dead_d.year);
    }
    while(compare(res_d , dead_d)==C_GREATER){
    printf("La date limite est antérieure à la date de réservation !\n");
    printf("Entrez la date de limite:\n");
    printf("Jour:");
    scanf("%d", &dead_d.day);
    printf("Mois:");
    scanf("%d", &dead_d.month);
    printf("Année:");
    scanf("%d", &dead_d.year);
    while(ver_cor_form(dead_d)==false){
    printf("Format de données incorrect !\n");
    printf("Entrez la date de limite:\n");
    printf("Jour:");
    scanf("%d", &dead_d.day);
    printf("Mois:");
    scanf("%d", &dead_d.month);
    printf("Année:");
    scanf("%d", &dead_d.year);
    }
    }
    repr_date(dead_d , f2);
    printf("Date limite: %s \n", f2);
    strcat(act , "Date limite: ");
    strcat(act , f2 );
    update_history(s , act);


}
return;
}
//a subscriber returns a copy of the book
void return_book_sub(book *b , subscriber s){
book b1;
char act[1001]="";
date res_d;
date dead_d;
char f1[1001]="";
char f2[1001]="";
copy_book(&b1 , *b);
bool yes=return_book(b);
if (yes==true){
strcat(act ,"Retourné ");
    strcat(act , b->title );
    strcat(act , "Le ");
    printf("Entrez la date de retour:\n");
    printf("Jour:");
    scanf("%d", &res_d.day);
    printf("Mois:");
    scanf("%d", &res_d.month);
    printf("Année:");
    scanf("%d", &res_d.year);
    while(ver_cor_form(res_d)==false){
    printf("Format de données incorrect !\n");
    printf("Entrez la date de retour:\n");
    printf("Jour:");
    scanf("%d", &res_d.day);
    printf("Mois:");
    scanf("%d", &res_d.month);
    printf("Année:");
    scanf("%d",& res_d.year);
    }
    repr_date(res_d , f1);
    printf("Date de retour: %s \n", f1);
    strcat(act , f1 );
    strcat(act ," ");
    //-------------------------------------
    printf("Entrez la date limite:\n");
    printf("Jour:");
    scanf("%d", &dead_d.day);
    printf("Mois:");
    scanf("%d", &dead_d.month);
    printf("Année:");
    scanf("%d",& dead_d.year);
    while(ver_cor_form(dead_d)==false){
    printf("Format de données incorrect !\n");
    printf("Entrez la date limite:\n");
    printf("Jour:");
    scanf("%d", &dead_d.day);
    printf("Mois:");
    scanf("%d", &dead_d.month);
    printf("Année:");
    scanf("%d", &dead_d.year);
    }
    repr_date(dead_d , f2);
    printf("Date limite: %s \n", f2);
    strcat(act , "Date limite: ");
    strcat(act , f2 );
    if (compare(res_d , dead_d)==C_GREATER){
        strcat(act , " \nDate limite dépassée");
    }
    else{
        strcat(act , " \nDate limite non dépassée");
    }
    update_history(s,act);

}
return;
}
//compare 2 subscribers
bool compare_sub(subscriber s1 , subscriber s2 ){
return((compare(s1.birth , s2.birth)==C_EQUAL)&&(strcmp(s1.histo,s2.histo)==0)&&(strcmp(s1.id,s2.id)==0)&&(strcmp(s1.name,s2.name)==0));
}
//copies the subscribers informations to another inscance
void copy_sub(subscriber *s , subscriber s1){
strcpy(s->name,s1.name);
strcpy(s->histo,s1.histo);
strcpy(s->id,s1.id);
copy_date(&s->birth , s1.birth);
return;
}
// shows subscriber's history
void show_sub_history(subscriber s) {
    FILE *f = fopen(s.histo, "r");

    if (!f) {
        printf("Aucun historique trouvé.\n");
        return;
    }

    char line[256];
    printf("\n=== HISTORIQUE DE %s ===\n", s.name);

    while (fgets(line, sizeof(line), f)) {
        printf("%s", line);
    }

    printf("\n========================\n");
    fclose(f);
}

//file complete
