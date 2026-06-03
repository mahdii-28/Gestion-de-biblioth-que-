#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "book.h"
#include "date.h"
#include "subcriber.h"
#include "str_manip.h"
#include "library.h"


// BOOKLIST FUNCTIONS
//-------------------------------------------------------------------------------------------------------------------------
// empties a list
void empty_list(booklist* l ){
if (l==NULL) {
    printf("Échec de l’allocation mémoire\n!");
    return;
}
booknode* cur=l->head;
while (cur!=NULL){
    booknode*tmp=cur;
    cur=cur->next;
    free(tmp);
}
l->head=NULL;
l->tail=NULL;
printf("Liste des livres vidée avec succés\n");
return;
}
//------------------------------------------------------------------------------------------
//add an element in the tail
void add_book_tail(booklist* l , book b ){
booknode*new_node=(booknode*)malloc(sizeof(booknode));
if (new_node == NULL) {
    perror("Échec de l’allocation mémoire!");
    return;
}
copy_book(&new_node->info , b);
new_node->prev=l->tail;
new_node->next = NULL;
if (l->tail!=NULL){
    l->tail->next=new_node;
}
l->tail=new_node;
if (l->head==NULL){
    l->head=new_node;
}
printf("Livre ajouté avec succés\n");
return;
}
// saves the info of the booklist in the binary file
void save_booklist(booklist* l){
    if (l==NULL){
        printf("Échec de l’allocation mémoire!\n");
        return;
    }
    booknode*cur=l->head;
    book b;
    FILE*f=fopen(BOOKS_FILE,"wb");
        if (f == NULL) {
        perror("Échec de l'ouverture du fichier!");
        return;
    }
    while (cur!=NULL){
      copy_book(&b , cur->info);
      fwrite(&b , sizeof(book), 1 ,f);
      cur=cur->next;
    }
    fclose(f);
    printf("Liste des livres sauvgardée avec succés\n");
}



// loads a booklist from the binary
void load_booklist(booklist* l) {
    FILE *f = fopen(BOOKS_FILE, "rb");
    if (f == NULL) {
        perror("Échec de l'ouverture du fichier!");
        return;
    }
// Clear current list
    empty_list(l);

    book b;
    while (fread(&b, sizeof(book), 1, f) == 1) {
        add_book_tail(l, b);  // Add each book to the end
    }

    if (!feof(f)) {
        // If we didn't reach the end of file, an error occurred
        perror("Erruer lors de la lecture du fichier!");
    }
    else{
        printf("Liste des livres Chargée \n");
    }

    fclose(f);
    return;
}
//returns the lists number of elements in the booklist
int booklist_len(booklist* l){
int i=0;
booknode*cur=l->head;
while(cur!=NULL){
    i++;
    cur=cur->next;
}
return(i);
}
//tells if the booklist is empty or not
bool bookempty(booklist* l){
return(l->head==NULL);}
// adds a book to the head of the list
void add_book_head(booklist* l, book b){
booknode*new_node=(booknode*)malloc(sizeof(booknode));
if (new_node == NULL) {
    perror("Échec de l’allocation mémoire!");
    return;
}
copy_book(&new_node->info , b);
new_node->prev=NULL;
new_node->next=l->head;
if (l->head!=NULL){
    l->head->prev=new_node;
}
l->head=new_node;
if(l->tail==NULL){
    l->tail=new_node;
}
printf("Livre ajouté avec succés\n");
return;
}
//adds a book in a given position
 void add_book_position(booklist* l, book b, int n){
int max=booklist_len(l)+1;
if ((n<=0)||(n>max)||(l==NULL)){
  printf("Impossible d'inserer\n");
  if (l==NULL){
    printf("Échec de l’allocation mémoire!\n");
  }
  return;
}
else if (n==1){
    add_book_head(l,b);
    return;
}
else if (n==max){
    add_book_tail(l,b);
    return;
}
else{
    booknode* new_node=(booknode*)malloc(sizeof(booknode));
    if (new_node==NULL){
        printf("Échec de l’allocation mémoire!\n");
    }
    booknode*cur1=l->head;
    booknode*cur2=l->head->next;
    copy_book(&new_node->info , b);
    for (int i=2 ; (i<n) ; i++){
        cur1=cur1->next;
        cur2=cur2->next;
    }
    new_node->prev=cur1;
    new_node->next=cur2;
    cur1->next=new_node;
    cur2->prev=new_node;
    printf("Livre ajouté avec succés\n");

    return;
}
    }
//remove the head of the booklist
 void remove_book_head(booklist* l){
 if (bookempty(l)){
    printf("Rien à supprimer\n");
    return;
 }
 else if (booklist_len(l)==1){
    empty_list(l);
    return;
 }
 else {
    booknode* new_head = l->head->next;
    booknode* tmp=l->head;
    new_head->prev=NULL;
    l->head=new_head;
    free(tmp);
    printf("Livre supprimé avec succés\n");
    return;
 }
 }
 //remove the tail of the booklist
void remove_book_tail(booklist *l){
 if (bookempty(l)){
    printf("Rien à supprimer\n");
    return;
 }
 else if (booklist_len(l)==1){
    empty_list(l);
    return;
 }
 else {
    booknode*new_tail=l->tail->prev;
    booknode*tmp=l->tail;
    new_tail->next=NULL;
    l->tail=new_tail;
    free(tmp);
    printf("Livre supprimé avec succés\n");
    return;
 }

 }
 //remove a book in a given position
void remove_book_position(booklist *l, int n) {
    int len = booklist_len(l);
    if ((n <= 0) || (n > len)) {
        printf("Rien à supprimer\n");
        return;
    }
    else if (n == 1) {
        remove_book_head(l);
        return;
    }
    else if (n == len) {
        remove_book_tail(l);
        return;
    }
    else {
        // Two-pointer approach
        booknode* cur1 = l->head;       // node before the one to remove
        booknode* cur2 = l->head->next; // node to remove

        // Move cur1 and cur2 to correct positions
        for (int i = 2; i < n; i++) {
            cur1 = cur1->next;
            cur2 = cur2->next;
        }

        // Remove cur2 by linking cur1 and cur2->next
        booknode* nxt = cur2->next;
        cur1->next = nxt;
        nxt->prev = cur1;

        free(cur2);
        printf("Livre supprimé avec succés\n");
    }
}
//search book by name
int search_book_by_name(booklist* l, char name[] , booknode** found ){
if (bookempty(l)){
        printf("La liste des livres est actuellement vide!\n ");
        *found=NULL;
        return(0);
}
else {
int i=1;
booknode* cur=l->head;
while (cur!=NULL){
    if (strcmp(cur->info.title , name)==0){
        *found=cur;
        printf("Livre trouvé\n");
        return(i);


    }
    else{
        cur=cur->next;
    }

}
*found=NULL;
printf("Livre non trouvé\n");
return(0);
}


}
//searches a book by id
int search_book_by_id(booklist* l, char id[] , booknode** found){
    if (bookempty(l)){
        printf("La liste des livres est actuellement vide!\n ");
        *found=NULL;
        return(0);
}
else {
booknode* cur=l->head;
int i=1;
while (cur!=NULL){
    if (strcmp(cur->info.id , id)==0){
        printf("Livre trouvé\n");
        *found=cur;
        return(i);
    }
    else{
        cur=cur->next;
    }

}
printf("Livre non trouvé\n");
*found=NULL;
return(0);
}
}
//show books written by the author
void show_author(booklist* l, char author[]){
if (bookempty(l)){
   printf("La liste des livres est actuellement vide\n ");
   return;
}
else{
    bool yes=false;
    booknode*cur=l->head;
    while (cur!=NULL){
        if (strcmp(cur->info.author , author)==0){
            if (yes==false){
                printf("Liste des livres crées par  %s:\n" , author);
            }
            yes=true;
            printf("%s\n" , cur->info.title);
        }
        cur=cur->next;
    }
    if (yes==false){
        printf("cet auteur n'as pas de livres\n");
    }


}
}
// creates an empty booklist
booklist* create_empty_booklist(void)
{
    booklist*l=malloc(sizeof(booklist));
    l->head=NULL;
    l->tail=NULL;
    printf("liste des lives crée avec succés\n");
    return(l);
}
//show the books available
void show_all_books_available(booklist* l){
if (bookempty(l)){
        printf("Il n y a aucun livre\n");
        return;
}
else{
      printf("Liste des livres:\n");
      booknode* cur=l->head;
      while(cur!=NULL){
        printf("Livre id # %s :\n", cur->info.id);
        printf("---------------------------\n");
        printf("titre: %s \n" , cur->info.title);
        printf("auteur: %s \n" , cur->info.author);
        printf("nombres des copies disponibles: %d \n" , cur->info.a_cop);
        printf("nombres des copies totale: %d \n" , cur->info.t_cop);
        printf("---------------------------\n");
        cur=cur->next;

      }
}

}
//SUBLIST FUNCTIONS
//create an empty sublist
sublist* create_empty_sublist(void){
sublist*l=malloc(sizeof(sublist));
    l->head=NULL;
    l->tail=NULL;
    printf("liste des abonnés crée avec succés\n");
    return(l);
}
//clears a list
void empty_sublist(sublist* l){
    if (l==NULL) {
    printf("Echec d'allocation de mémoire \n!");
    return;
}
subnode* cur=l->head;
while (cur!=NULL){
    subnode*tmp=cur;
    cur=cur->next;
    free(tmp);
}
l->head=NULL;
l->tail=NULL;
printf("la liste des abonnés est vidée avec succés\n");
return;
}
//saves a sublist in a bin folder
void save_sublist(sublist* l ){
    if (l==NULL){
        printf("Eched d'allocation de mémoire\n");
        return;
    }
    subnode*cur=l->head;
    subscriber s;
    FILE*f=fopen(SUBS_FILE,"wb");
        if (f == NULL) {
        perror("Echec d'ouverture du fichier");
        return;
    }
    while (cur!=NULL){
      copy_sub(&s , cur->info);
      fwrite(&s , sizeof(subscriber), 1 ,f);
      cur=cur->next;
    }
    fclose(f);
    printf("Liste des abonnés sauvgardé avec succés \n");
}
//loads a sublist from a bin folder
void load_sublist(sublist* l){
    FILE *f = fopen(SUBS_FILE, "rb");
    if (f == NULL) {
        perror("Echec d'ouverture du fichier");
        return;
    }
// Clear current list
    empty_sublist(l);

    subscriber s;
    while (fread(&s, sizeof(subscriber), 1, f) == 1) {
        add_sub_tail(l, s);  // Add each book to the end
    }

    if (!feof(f)) {
        // If we didn't reach the end of file, an error occurred
        perror("Erreur lors de la lecture du fichier ");
    }
    else{
        printf("Liste des abonnés ouverte avec succes\n");
    }

    fclose(f);
    return;
}
//add a subscriber in the tail of the sublist
 void add_sub_tail(sublist* l, subscriber s){
 subnode* newnode=(subnode*)malloc(sizeof(subnode));
 if (newnode==NULL){
    printf("Echec d'allocation de mémoire\n");
    return;
 }
 copy_sub(&newnode->info , s);
 newnode->prev=l->tail;
 newnode->next=NULL;
 if (l->tail!=NULL){
    l->tail->next=newnode;
 }
 else{
    l->head=newnode;
 }
 l->tail=newnode;
 printf("Abonné ajouté avec succés\n");
 return;
 }
 //add a subscriber in the tail of the sublist
void add_sub_head(sublist* l, subscriber s)
{
    subnode* newnode = (subnode*)malloc(sizeof(subnode));
    if (newnode == NULL) {
        printf("Echec d'allocation de mémoire\n");
        return;
    }

    copy_sub(&newnode->info, s);

    newnode->prev = NULL;
    newnode->next = l->head;

    if (l->head != NULL) {
        l->head->prev = newnode;
    } else {
        l->tail = newnode;
    }

    l->head = newnode;
    printf("Abonné ajouté avec succés\n");
}
//determines the lenght of the sublist
int sublist_len(sublist* l){
if (l==NULL)return(0);
int i=0;
subnode* cur=l->head;
while (cur!=NULL){
    i++;
    cur=cur->next;
}
return(i);
}
//checks if list is empty
bool subempty(sublist* l){
return(l->head==NULL);}
//adds sub in a given position
void add_sub_position(sublist* l, subscriber s, int n){
if ((n<=0)|| (n>sublist_len(l)+1)||(l==NULL)){
    printf("impossible à inserer\n");
    if (l==NULL) {
        printf("Echec d'allocation de mémoire\n");
    }
    return;
}
else if (n==1){
    add_sub_head(l , s);
    return;
}
else if (n==sublist_len(l)+1){
    add_sub_tail(l , s );
    return;
}
else{
    subnode*cur1=l->head;
    subnode*cur2=l->head->next;
    subnode* newnode=(subnode*)malloc(sizeof(subnode));
    if (newnode==NULL){
        printf("Echec d'allocation de mémoire\n");
    }
    copy_sub(&newnode->info , s);
    for (int i=2 ; i<n ; i++){
        cur1=cur1->next;
        cur2=cur2->next;
    }
    newnode->prev=cur1;
    newnode->next=cur2;
    cur1->next=newnode;
    cur2->prev=newnode;
    printf("Abonné ajouté avec succés\n");
}
}
//removes the head of the function
void remove_sub_head(sublist* l){
if (l==NULL){
    printf("Echec d'allocation de mémoire\n");
    return;
}
else if (subempty(l)){
    printf("Rien à supprimer\n");
    return;
}
else if (l->head==l->tail){
    empty_sublist(l);
    return;
}
else{
    subnode* tmp=l->head;
    subnode* newhead=l->head->next;
    newhead->prev=NULL;
    l->head=newhead;
    free(tmp);
    printf("Abonné supprimé avec succés\n");

}
}
//removes the tail of sublist
void remove_sub_tail(sublist* l){
if (l==NULL){
    printf("Echec d'allocation de mémoire\n");
    return;
}
else if (subempty(l)){
    printf("Rien a supprimer\n");
    return;
}
else if (l->head==l->tail){
    empty_sublist(l);
    return;
}
else{
    subnode* tmp=l->tail;
    subnode* newtail=l->tail->prev;
    newtail->next=NULL;
    l->tail=newtail;
    free(tmp);
    printf("Abonné spprimé avec succés\n");
}
}
//removes a subscriber in a given position
void remove_sub_position(sublist* l, int n){
    if (l == NULL){
        printf("Echec d'allocation de mémoire\n");
        return;
    }
    else if (subempty(l)){
        printf("Rien a supprimer\n");
        return;
    }
    else if ((n <= 0) || (n > sublist_len(l))){
        printf("La position n'existe pas\n");
        return;
    }
    else if (n == 1){
        remove_sub_head(l);
        return;
    }
    else if (n == sublist_len(l)){
        remove_sub_tail(l);
        return;
    }
    else{
        subnode *tmp = l->head;
        subnode *prev;
        subnode *next;

        for (int i = 1; i < n; i++){
            tmp = tmp->next;
        }

        prev = tmp->prev;
        next = tmp->next;
        prev->next = next;
        next->prev = prev;
        free(tmp);
        printf("Abonné supprimé avec succés\n");
        return;
    }
}
//seaches a subscriber by his name
int search_sub_by_name(sublist* l, char name[], subnode** found){
    if (subempty(l)){
        printf("La liste des Abonnés est actullement vide\n");
        *found = NULL;
        return 0;
    }
    else {
        int i = 1;
        subnode* cur = l->head;
        while (cur != NULL){
            if (strcmp(cur->info.name, name) == 0){
                *found = cur;
                printf("Abonné trouvé\n");
                return i;
            }
            cur = cur->next;
            i++;  // increment position
        }
        *found = NULL;
        printf("Abonné non trouvé\n");
        return 0;
    }
}
//
int search_sub_by_id(sublist* l, char name[], subnode** found){
    if (subempty(l)){
        printf("La liste des abonnés est actuellement vide\n");
        *found = NULL;
        return 0;
    }
    else {
        int i = 1;
        subnode* cur = l->head;
        while (cur != NULL){
            if (strcmp(cur->info.id, name) == 0){
                *found = cur;
                printf("Abonné trouvé\n");
                return i;
            }
            cur = cur->next;
            i++;  // increment position
        }
        *found = NULL;
        printf("Abonné non trouvé\n");
        return 0;
    }
}
//shows all subscribers
void show_all_subscribers(sublist* l){
if (subempty(l)){
        printf("La liste des abonnés est vide \n");
        return;
}
else{
      printf("Liste des abonnés:\n");
      subnode* cur=l->head;
      char date[1001];
      while(cur!=NULL){
        printf("Abonné id # %s :\n", cur->info.id);
        printf("---------------------------\n");
        printf("nom: %s \n" , cur->info.name);
        repr_date( cur->info.birth  , date);
        printf("Date de naissance: %s \n" , date);
        printf("---------------------------\n");
        cur=cur->next;

      }
}

}
//file complete
