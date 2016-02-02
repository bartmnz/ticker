#define _GNU_SOURCE 1
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>


struct company {
    char symbol[6];
    int cents;
    char *name;
};

struct tree{
    struct company * data;
    struct tree *left, *right;
};

//typedef struct{
//    struct tree *root;
//    int (*cmp)(const struct company *, const struct company *);
//} market;

bool tree_insert(struct tree *t, struct company *comp,
    int (*cmp)(const struct company *, const struct company *));
struct company *stock_create( char* symbol, char *name, double price);
struct tree* tree_create(struct company* comp);
struct company* make_company( char* string);
int check_symbol(const struct company *first, const struct company *second);
int check_value(const struct company *first, const struct company *second);
struct tree* pop_tree( struct tree* root);
void tree_destroy( struct tree* root);
struct tree* read_file(char* filename);
void user_input(struct tree* farce);