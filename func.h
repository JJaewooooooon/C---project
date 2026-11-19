#ifndef FUNC_H
#define FUNC_H

#define _CRT_SECURE_NO_WARNINGS

#pragma execution_character_set("utf-8")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_ITEMS 100
#define MAX_CATEGORY 4
#define NAME_LEN 50

typedef struct {
    int id;
    char name[NAME_LEN];
    int category;
    int cost_price;
    int sell_price;
    int stock;
    int safe_stock;
    int sales_count;
} Product;

extern Product inventory[MAX_ITEMS];
extern int item_count;
extern int category_sales[MAX_CATEGORY];
extern const char* category_names[MAX_CATEGORY];

void init_sample_data(void);
void print_menu(void);
void clear_input_buffer(void);
int find_item_index_by_id(int id);

void add_item(void);
void sell_item(void);
void print_all(void);
void search_item(void);
void check_low_stock(void);
void sort_by_sales(void);
void show_sales_report(void);

#endif