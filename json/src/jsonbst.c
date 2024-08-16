#include "jsonbst.h"

void json_bst_new(t_json_bst* tree) {
    tree->size = 0;
    tree->capacity = 0;
    tree->keys = (char**) malloc(0);
    tree->value_type = (e_json_value_type*) malloc(0);
    tree->values = (t_json_value*) malloc(0);
}

void _json_bst_grow_capacity(t_json_bst* tree) {
    tree->capacity = tree->capacity == 0 ? 1 : 2 * tree->capacity;
    tree->keys = (char**) realloc(tree->keys, tree->capacity * sizeof(char*));
    for(int i = (int)(tree->capacity / 2.0); i < tree->capacity; i++) {
        tree->keys[i] = 0;
    }
    tree->value_type = (e_json_value_type*) realloc(tree->value_type, tree->capacity * sizeof(e_json_value_type));
    tree->values = (t_json_value*) realloc(tree->values, tree->capacity * sizeof(t_json_value));
}

static int _key_match(char* a, char* b) {
    // Assumption: keys are sha256 word lists
    for(int i = 0; i < 32; i++) {
        if(a[i] != b[i]) return 0;
    }
    return 1;
}

static int _key_compare(char* a, char* b) {
    for(int i = 0; i < 32; i++) {
        if(a[i] < b[i]) return 1;
        if(a[i] > b[i]) return -1;
    }
    return 0;
}

void json_bst_insert(t_json_bst* tree, char* key, e_json_value_type value_type, t_json_value value) {
    char* key_sha256 = json_sha256(key);
    if(tree->size == tree->capacity) {
        _json_bst_grow_capacity(tree);
    }
    int node = 0;
    while(tree->keys[node]) {
        if(_key_match(key_sha256, tree->keys[node])) {
            printf("ERROR::INSERT - Cannot insert into hash table, key '%s' exists\n", key);
            return;
        }
        node = 2 * node + ((_key_compare(key_sha256, tree->keys[node]) == 1) ? 1 : 2);
        if(node >= tree->capacity) break;
    }
    if(node >= tree->capacity) {
        _json_bst_grow_capacity(tree);
    }
    // Set key
    tree->keys[node] = key_sha256;
    
    // Set value_type
    tree->value_type[node] = value_type;

    // Set value
    tree->values[node] = value;

    // Increment Size
    tree->size += 1;
}

e_json_value_type json_bst_get(t_json_bst* tree, char* key, t_json_value* out) {
    char* key_sha256 = json_sha256(key);
    int node = 0;
    while(tree->keys[node]) {
        if(_key_match(key_sha256, tree->keys[node])) {
            free(key_sha256);
            *out = tree->values[node];
            return tree->value_type[node];
        }
        node = 2 * node + ((_key_compare(key_sha256, tree->keys[node]) == 1) ? 1 : 2);
        if(node >= tree->capacity) break;
    }
    printf("ERROR::BST::GET - No value associated with key '%s'\n", key);
    free(key_sha256);

    return -1;
}

void json_bst_set(t_json_bst* tree, char* key, e_json_value_type value_type, t_json_value value) {
    char* key_sha256 = json_sha256(key);
    int node = 0;
    while(tree->keys[node]) {
        if(_key_match(key_sha256, tree->keys[node])) {
            free(key_sha256);
            if(tree->value_type[node] == jsonSTRING){
                json_string_free(&tree->values[node].sval);
            } else if(tree->value_type[node] == jsonARRAY) {
                printf("its an array 1\n");
                if(tree->values[node].aval->allocated) {
                    json_array_free(tree->values[node].aval);
                }
            } else if (tree->value_type[node] == jsonJSON) {
                if(tree->values[node].jval->allocated) {
                    json_free(tree->values[node].jval);
                }
            }
            tree->values[node] = value;
            tree->value_type[node] = value_type;

            return;
        }
        node = 2 * node + ((_key_compare(key_sha256, tree->keys[node]) == 1) ? 1 : 2);
        if(node >= tree->capacity) break;
    }
    free(key_sha256);

    json_bst_insert(tree, key, value_type, value);

    return;
}


void json_bst_free(t_json_bst* tree) {
    for(int i = 0; i < tree->capacity; i++) {
        if(tree->keys[i]) {
            free(tree->keys[i]);
        } 
        // Don't have to free bst values because they were freed in json_free.
    }
    free(tree->values);
    free(tree->value_type);
    free(tree->keys);
}