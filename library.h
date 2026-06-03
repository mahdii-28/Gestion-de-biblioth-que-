#ifndef LIB_H
#define LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "book.h"
#include "date.h"
#include "subcriber.h"
#include "str_manip.h"
#define DATA_FOLDER       "data"
#define BOOKS_FILE        DATA_FOLDER "/books.bin"
#define SUBS_FILE         DATA_FOLDER "/subscribers.bin"
#define SUB_ACT_FOLDER    DATA_FOLDER "/Sub_act"
#define SUB_HIST_FOLDER   DATA_FOLDER "/Sub_hist"


// --------------------
// Book node & list
// --------------------
typedef struct booknode {
    book info;
    struct booknode* prev;
    struct booknode* next;
} booknode;

typedef struct booklist {
    booknode* head;
    booknode* tail;
} booklist;

// --------------------
// Subscriber node & list
// --------------------
typedef struct subnode {
    subscriber info;
    struct subnode* prev;
    struct subnode* next;
} subnode;

typedef struct sublist {
    subnode* head;
    subnode* tail;
} sublist;

// --------------------
// Booklist functions
// --------------------
booklist* create_empty_booklist(void);
void empty_list(booklist* l);//
void save_booklist(booklist* l );
void load_booklist(booklist* l );
int booklist_len(booklist* l);
bool bookempty(booklist* l);
int search_book_by_name(booklist* l, char name[] , booknode** found);
int search_book_by_id(booklist* l, char id[] , booknode** found);
void add_book_head(booklist* l, book b);
void add_book_tail(booklist* l, book b);
void add_book_position(booklist* l, book b, int n);
void remove_book_head(booklist* l);
void remove_book_tail(booklist* l);
void remove_book_position(booklist* l, int n);
void show_author(booklist* l, char author[]);
void show_all_books_available(booklist* l);
void show_all_books_available(booklist* l);



// --------------------
// Sublist functions
// --------------------
sublist* create_empty_sublist(void);
void empty_sublist(sublist* l);
void save_sublist(sublist* l );
void load_sublist(sublist* l);
int sublist_len(sublist* l);
bool subempty(sublist* l);
int search_sub_by_name(sublist* l, char name[], subnode** found);
int search_sub_by_id(sublist* l, char id[], subnode** found);
void add_sub_head(sublist* l, subscriber s);
void add_sub_tail(sublist* l, subscriber s);
void add_sub_position(sublist* l, subscriber s, int n);
void remove_sub_head(sublist* l);
void remove_sub_tail(sublist* l);
void remove_sub_position(sublist* l, int n);
void show_all_subscribers(sublist* l);

#endif
