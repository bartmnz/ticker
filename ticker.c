#include "ticker.h"

/*Code included as part of assignment 
 * modified to manipulate existing data 
 */
bool tree_insert(struct tree *t, struct company *comp,
    int (*cmp)(const struct company *, const struct company *))
{
    // insert left
    bool rValue = false;
    if(cmp(comp, t->data) == -1) {
        if (t->left) {
            return tree_insert(t->left, comp, cmp);
        } else {
            if(comp->cents < .01){ // does not match any known data and is invalid value
                fprintf(stderr, "ERROR: stock price cannot be less than $.01\n");
                return false;
            }
            t->left = tree_create(comp);
            return t->left;
            
        }
        
    //insert right
    } else if (cmp(comp, t->data) == 1){
        if (t->right){
            return tree_insert(t->right, comp, cmp);
        } else {
            if(comp->cents < .01){ // does not match any known data and is invalid value
                fprintf(stderr, "ERROR: stock price cannot be less than $.01\n");
                return false;
            }
            t->right = tree_create(comp);
            return t->right;
        }
        
    // insert at head
    } else if(t->data == NULL){
        t->data = comp;
    
    // the symbol is the same
    } else if( check_symbol(comp, t->data) == 0){ // using the same symbol
        int temp = t->data->cents + comp->cents;
        if (temp > 0){
            t->data->cents = temp;
            rValue = true;
        } else{
            fprintf(stderr, "ERROR: stock price cannot be less than $.01\n");
        }
        free(comp->name);
        free(comp);
        return rValue;
        
    // data is the same but symbol is different
    } else if(cmp(comp, t->data) ==0){ 
        if (t->left) {
            return tree_insert(t->left, comp, cmp);
        } else {
            t->left = tree_create(comp);
            return t->left;
        } // arbitrarly insert to the left
    }    
    else{
        free(comp->name);
        free(comp);
        return rValue;
    }
    return false;
}

/* Returns a newly Malloc'd tree with left and right nodes set to NULL and data 
 * set as the parameter comp.
 */
struct tree* tree_create(struct company* comp){
    struct tree* tree = malloc(sizeof(struct tree));
    if (! tree){
        fprintf(stderr, "ERROR: could not allocate space.\n");
        return NULL;
    }
    memset(tree, 0, sizeof(struct tree));
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
    if (! newComp){
        fprintf(stderr, "ERROR: could not allocate space.\n");
        return NULL;
    }
    memset(newComp, 0, sizeof(struct company));
    // get ticker symbol
    symb = strtok(string, " ");
    if (strnlen(symb, 6) > 5){
        free(newComp);
        return NULL;
    } else 
        strncpy(newComp->symbol, symb, 6);
    for (int i = 0; i < 6; i++){
        if ( newComp->symbol[i] == '\0'){
            break;
        }
        if (! (isalpha((newComp->symbol)[i]))){
            free(newComp);
            return NULL;
        }
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
        printf("%d\n", len);
        newComp->name = malloc(sizeof(char)*(len+1));
        if (! (newComp->name)){
            fprintf(stderr, "ERROR: could not allocate space.\n");
            free(newComp);
            return NULL;
        }
        memset(newComp->name, 0, sizeof(char)*(len+1));
        strncpy( newComp->name, title, len);
        if(title[len-1] == '\n'){
            newComp->name[len-1]='\0';
        }
    }
    return newComp;
}

/* compares the alphabetical order of the ticker symbols of the two companies.
 * returns -1 if first before second 0 if they are equal and 1 if second is before first
 * returns 10 if either argument is invalid
 */
int check_symbol(const struct company *first, const struct company *second){
    if( !first || !second){
        return 10;
    }
    int a = strnlen(first->symbol, 6);
    int b = strnlen(second->symbol, 6);
    int size = a > b ? a : b;
    int temp = strncmp(first->symbol, second->symbol, size);
    if (temp >= 1) return 1;
    if (temp <= -1) return -1;
    return temp;
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
 * used to prevent multiple references to pointers and repeated calls to malloc the same data.
 */
struct tree* pop_tree( struct tree* root){
    if (! root){
        return NULL;
    }
    struct tree* temp = NULL;
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
    } 
    if ( root->right ){
        tree_destroy(root->right);
        root->right = NULL;
    }
    if( root->data ){
        if (root->data->name){
            free(root->data->name);
        }
        free(root->data);
    }
    free(root);
    
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
    }
    if ( root->data && root->data->name){
        fprintf(stdout, "%s %d.%d %s\n", root->data->symbol, root->data->cents/100, 
                        root->data->cents%100, root->data->name);
    }
    if ( root->right ){
        print_tree(root->right);
    }
    
}

/* Reads fromt he specified file and creates a tree based on the data inside
 * returns a MALLOC'd tree structure.
 */
struct tree* read_file(char* filename){
    FILE* file;
    if ( ! (file = fopen(filename, "r"))){
        fprintf(stderr, "ERROR: could not open file. Check filename and permissions.\n");
        return NULL;
    }
    struct tree* farce = malloc(sizeof(struct tree));
    if (! farce){
        fprintf(stderr, "ERROR: could not allocate space.\n");
        return NULL;
    }
    memset( farce, 0, sizeof(struct tree));
    char temp[82];
    struct company *pmet;
    while( fgets(temp, sizeof(temp), file) != NULL){
        temp[81] = '\0';
        pmet = make_company(temp);
        if ( pmet) {
            tree_insert(farce, pmet, check_symbol);
        }
        pmet = NULL;
    }
    fclose(file);
    return farce;
}

/* continusly gets user input and updates the data in farce 
 *
 */
void user_input(struct tree* farce){
    if (! farce){
        return;
    }
    char temp[82];
    struct company *pmet;
    while( fgets(temp, sizeof(temp), stdin) && temp != NULL){
        pmet = make_company(temp);
        if ( pmet) {
            tree_insert(farce, pmet, check_symbol);
        }
        pmet = NULL;
    }
}


/* program execution begins here
 *
 */
int main(int argc, char* argv[]){
    if( argc != 2 ){
        fprintf(stderr, "ERROR: useage is ./ticker FILENAME\n");
        return -1;
    }
    struct tree* tree, *tempC = NULL, *tempT = malloc(sizeof(struct tree));
    if (!tempT){
        fprintf(stderr, "ERROR: could not allocate space.\n");
        return -1;
    }
    memset(tempT, 0, sizeof(struct tree));
    tree = read_file(argv[1]);
    if (!tree){ // allocation or read failed somehow
        return -1;
    }
    user_input(tree);
    while( (tempC = pop_tree(tree)) && tempC != tree){
        tree_insert(tempT, tempC->data, check_value);
        free(tempC);
    }
    tree_insert(tempT, tree->data, check_value);
    print_tree(tempT);
    free(tree);
    tree_destroy(tempT);
    
}