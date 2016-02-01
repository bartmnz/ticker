#include "ticker.h"

/*Code included as part of assignment 
 * modified to manipulate existing data 
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
    } else if (cmp(comp, t->data) > 1){
        if (t->right){
            return tree_insert(t->right, comp, cmp);
        } else {
            t->right = tree_create(comp);
            return t->right;
        }
    } else if( cmp == &check_symbol ){ // using the same symbol
        if ( comp->status == 1){
            return (t->data->cents += comp->cents);
        } else if( comp->status == -1 ){
            int temp = t->data->cents - comp->cents;
            if (temp > 0){
                return (t->data->cents = temp);
            }
        }
    }
    return false;
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

/* Returns a newly Malloc'd tree with left and right nodes set to NULL and data 
 * set as the parameter comp.
 */
struct tree* tree_create(struct company* comp){
    struct tree* tree = malloc(sizeof(struct tree));
    tree->data = comp;
    tree->left = NULL;
    tree->right = NULL;
    return tree;
}

/* Returns a newly Malloc'd company structure with Malloc'd string name
 * derrived from the input string company name
 * if string is invalid returns NULL
 */
struct company* make_company( char* string){
    const char *symb, *value, *title;
    struct company* newComp = malloc(sizeof(struct company)); 
    // get ticker symbol
    symb = strtok(string, " ");
    if (strnlen(symb, 6) > 5){
        free(newComp);
        return NULL;
    } else {
        strcpy(newComp->symbol, symb);
    }
    
    // get value of stock (or adjustment)
    double dollars;
    char *end;
    value = strtok(NULL, " ");
    if (!value || strnlen(value, 11) > 10){ // value is out of bounds or nonexistant
        free(newComp);
        return NULL;
    }
    dollars = strtod(value, &end);
    if( value == end){ // did not successfully convert a number.
        free(newComp);
        return NULL;
    }
    newComp->cents = (int)(dollars * 100);
    
    // get name
    title = strtok(NULL, "\0");
    if (title){
        int len = strnlen(title, 65);
        newComp->name = malloc(len);
        strcpy( newComp->name, title);
    }
    
    return newComp;
}

/* compares the alphabetical order of the ticker symbols of the two companies.
 * returns -1 if first before second 0 if they are equal and 1 if second is before first
 * returns 10 if either argument is invalid
 */
int check_symbol(const struct company *first, const struct company *second){
    if( ! first || ! second){
        return 10;
    }
    return strncmp(first->symbol, second->symbol, 6);
}

/* compares value of the cents field of the two companies
 * returns -1 if first is less than second 0 if they are equal and 1 if first is greater
 * returns 10 if either argument is invalid
 */
int check_value(const struct company *first, const struct company *second){
    if( ! first || ! second){
        return 10;
    }
    if ( first->cents > second->cents){
        return 1;
    } else if (first->cents < second->cents){
        return -1;
    } else return 0;
}

/* returns the node found at the lowest level of the tree from left to right
 * removes the node from the tree, DOES NOT FREE THE MEMORY!
 *
 */
struct tree* pop_tree( struct tree* root){
    if (! root){
        return NULL;
    }
    struct tree* temp;
    if( root->left){
        temp = pop_tree(root->left);
        if (temp == root->left){
            root->left = NULL;
        }
        return temp;
    }else if ( root->right){
        temp = pop_tree(root->right);
        if (temp == root->right){
            root->right = NULL;
        }
        return temp;
    } else return root;
}

/* Free's memory from tree and companies stored in the tree. 
 *
 */
void tree_destroy( struct tree* root){
    if (! root ){
        return;
    }
    if( root->left ){
        tree_destroy (root->left);
        root->left = NULL;
    } else if ( root->right ){
        tree_destroy(root->right);
        root->right = NULL;
    } else {
        free(root->data->name);
        free(root->data);
        free(root);
    }
}

/* prints each member of the tree.
 *
 */
void print_tree(struct tree* root){
    if (! root ){
        return;
    }
    if ( root->left ){
        print_tree(root->left);
    } else if ( root->right ){
        print_tree(root->right);
    }
    fprintf(stdout, "%s %d.%d %s\n", root->data->symbol, (int)root->data->cents/100, 
                        (int)root->data->cents%100, root->data->name);
}

/* Reads fromt he specified file and creates a tree based on the data inside
 * returns a MALLOC'd tree structure.
 */
struct tree* read_file(char* filename){
    FILE* file;
    if ( ! (file = fopen(filename, "r"))){
        fprintf(stderr, "ERROR: could not open file. Check filename and permissions.\n");
    }
    struct tree* farce = malloc(sizeof(struct tree));
    if (! farce){
        fprintf(stderr, "ERROR: could not allocate space.\n");
        return NULL;
    }
    char temp[82];
    struct company *pmet;
    while( fgets(temp, sizeof(temp), file) != NULL){
        pmet = make_company(temp);
        if ( pmet) {
            tree_insert(farce, pmet, check_symbol);
        }
        pmet = NULL;
    }
    return farce;
}


/* program execution begins here
 *
 */
int main(int argc, char* argv[]){
    if( argc != 2 ){
        fprintf(stderr, "ERROR: useage is ./ticker FILENAME\n");
    }
    FILE* file;
    if ( ! (file = fopen(argv[1], "r"))){
        fprintf(stderr, "ERROR: could not open file. Check filename and permissions.\n");
    }
    // malloc new market farce with CMP fn for strings.
    // open file specified in argv
    // read one line at a time line is max  symbol + space + value + sapce + name + newline 
    // 5 + 1 + 9 + 1 + 64 + 1 bytes long 
    // tree_insert(farce->root, make_company(input line))
    
    // while input 
    // if ctrl-D print_in_order()
    // tree_insert(farce->root, make_company(input line))
}