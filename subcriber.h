#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include"book.h"
#include"date.h"
#include"str_manip.h"

#ifndef SUBS_H
#define SUBS_H
#define DATA_FOLDER       "data"
#define BOOKS_FILE        DATA_FOLDER "/books.bin"
#define SUBS_FILE         DATA_FOLDER "/subscribers.bin"
#define SUB_ACT_FOLDER    DATA_FOLDER "/Sub_act"
#define SUB_HIST_FOLDER   DATA_FOLDER "/Sub_hist"

//subscriber data structure
typedef struct{
char name[1001];
char id[9];
date birth;
char histo[1001];
char activity[1001];
}
subscriber;
//action data structure
typedef enum {
RESERVED , RETURNED
}action;
//activity data structure
typedef struct{
action ac;
char book[1001];
date res_date;
date dead_date;
bool echec;
}activity;
//functions related to subscriber
subscriber create_sub(char n[],char i[], date b);
void update_history(subscriber sub , char act[]);
void update_activity(subscriber sub , activity ac);
activity create_activity(action ac , char book[] , date res , date dead );
void reserve_book_sub(book *b , subscriber s);
void return_book_sub(book *b , subscriber s) ;
bool compare_sub(subscriber s1 , subscriber s2 );
void copy_sub(subscriber *s , subscriber s1);
void show_sub_history(subscriber s);
#endif
