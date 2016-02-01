#define _GNU_SOURCE 1
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>


struct company {
    char symbol[6];
    size_t cents;
    char *name;
    int status; // 1 is add 0 is no symbol and -1 is subtract 
};

struct tree{
    struct company * data;
    struct tree *left, *right;
};

typedef struct{
    struct tree *root;
    int (*cmp)(const struct company *, const struct company *);
} market;

bool tree_insert(struct tree *t, struct company *comp,
    int (*cmp)(const struct company *, const struct company *));
struct company *stock_create( char* symbol, char *name, double price);
struct tree* tree_create(struct company* comp);
struct company* make_company( char* string);
int check_symbol(const struct company *first, const struct company *second);
int check_value(const struct company *first, const struct company *second);