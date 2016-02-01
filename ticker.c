#include "ticker.h"

/*Code included as part of assignment 
 *
 */
bool tree_insert(struct tree *t, struct company *comp,
    int (*cmp)(const struct company *, const struct company *))
{
    if(cmp(comp, t->data) < 1) {
        if (t->left) {
            return tree_insert(t->left, comp, cmp);
        } else {
            t->left = tree_create(comp);
            return t->left;
        }
    } else {
        if (t->right){
            return tree_insert(t->right, comp, cmp);
        } else {
            t->right = tree_create(comp);
            return t->right;
        }
    }
}

/*Code included as part of assignment
 *
 */
struct company *stock_create( char* symbol, char *name, double price)
{
    struct company *new_stock = malloc(sizeof(*new_stock));
    if(!new_stock){
        return NULL;
    }
    
    new_stock->name = strdup(name);
    if( !new_stock->name ){
        free (new_stock);
        return NULL;
    }
    
    strncpy( new_stock->symbol, symbol, sizeof(new_stock->symbol)-1);
    new_stock->symbol[sizeof(new_stock->symbol)-1] = '\0';
    
    new_stock->cents = 100 * price;
    return new_stock;
}

struct tree* tree_create(struct company* comp){
    // do stuff here
    struct tree* tree = malloc(sizeof(struct tree*));
    tree->data = comp;
    return tree;
}

int main(void){
    // do stuff here
}