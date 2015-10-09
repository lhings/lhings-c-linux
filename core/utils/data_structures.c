/* Copyright 2015 Lyncos Technologies S. L.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "data_structures.h"

LH_List* lh_list_new() {
    return lh_list_new_custom_capacity(LIST_INITIAL_CAPACITY);
}

LH_List* lh_list_new_custom_capacity(int capacity){
    LH_List* my_list = malloc(sizeof *my_list);
    my_list->max_capacity = capacity;
    my_list->items = malloc(my_list->max_capacity * sizeof my_list->items);
    my_list->size = 0;
    return my_list;
}

void* lh_list_get(LH_List *my_list, uint16_t index) {
    if (index < 0 || index >= my_list->size)
        return NULL;
    return my_list->items[index].ptr_this;

}

void lh_list_add(LH_List *my_list, void *element) {
    if (my_list->size == 0) {
        my_list->items[0].ptr_this = element;
        my_list->size = 1;
    } else if (my_list->size > 0 && my_list->size < my_list->max_capacity) {
        my_list->items[my_list->size].ptr_this = element;
        my_list->size++;
    } else {
        uint16_t new_capacity = my_list->max_capacity * LIST_INCREMENT_CAPACITY_FACTOR;
        my_list->items = realloc(my_list->items, new_capacity * sizeof my_list->items);
        my_list->max_capacity = new_capacity;
        lh_list_add(my_list, element);
    }
}

void lh_list_free(LH_List *my_list) {
    free(my_list->items);
    free(my_list);
}

uint32_t string_hash_code(const char *string) {
    uint32_t h = 0;
    uint16_t j = 0;
    while (string[j] != 0) {
        h = (h << 5) - h + string[j];
        j++;
    }
    return h;
}

LH_DictItem* dict_get_entry (const LH_Dict* my_dict, const char* key){
    uint32_t position = string_hash_code(key) % my_dict->max_capacity;
    uint32_t iteration_count = 0;
    do {
        LH_DictItem* entry = my_dict->items + position;
        if (entry->ptr_key == NULL)
            return entry;
        else if (strcmp(entry->ptr_key, key) == 0)
            return entry;
        position++;
        iteration_count++;
        if(position >= my_dict->max_capacity)
            position = 0;
    } while (iteration_count < my_dict->max_capacity);
    // this code only returns NULL if dict is full and key is not found
    return NULL;
}


LH_Dict* lh_dict_new() {
    return lh_dict_new_custom_capacity(DICT_INITIAL_CAPACITY);
}

LH_Dict* lh_dict_new_custom_capacity(int capacity){
    LH_Dict *my_dict = malloc(sizeof *my_dict);
    my_dict->items = malloc(capacity * sizeof *my_dict->items);
    // initialize memory to zero (required for dict to work)
    memset(my_dict->items, 0, capacity * sizeof *my_dict->items);
    my_dict->size = 0;
    my_dict->max_capacity = capacity;
    
    int j;
    for (j = 0; j < my_dict->max_capacity; j++){
        int key_null = 1;
        int value_null = 1;
        if ((my_dict->items+j)->ptr_key != NULL)
            key_null = 0;
        if((my_dict->items+j)->ptr_value != NULL)
            value_null = 0;
        if ((my_dict->items+j)->ptr_key != NULL || (my_dict->items+j)->ptr_value != NULL)
            printf("item %d, key_null %d, value_null %d\n", j, key_null, value_null);
    }
    return my_dict;
}

void dict_increase_and_rehash(LH_Dict* my_dict){
    int size = my_dict->size;
    char** keys = malloc(size * sizeof *keys);
    void** values = malloc (size * sizeof *values);
    int i, j;
    i = 0;
    for(j = 0; j < my_dict->max_capacity; j++){
        char* key = my_dict->items[j].ptr_key;
        if (key == NULL || strlen(key) == 0)
            continue;
        void* value = my_dict->items[j].ptr_value;
        keys[i] = key;
        values[i] = value;
        i++;
    }
    
    int my_dict_new_capacity = my_dict->max_capacity * DICT_INCREMENT_CAPACITY_FACTOR;
    my_dict->items = realloc(my_dict->items, my_dict_new_capacity * sizeof *my_dict->items);
   
    memset(my_dict->items, 0, my_dict_new_capacity * sizeof *my_dict->items);
    my_dict->max_capacity = my_dict_new_capacity;
    my_dict->size = 0;
    for (j=0; j<size; j++){
        lh_dict_put(my_dict, keys[j], values[j]);
    }
    
    
    
   free(keys);
   free(values);
}

void lh_dict_put(LH_Dict* my_dict, const char *key, void* value){
    LH_DictItem *entry = dict_get_entry(my_dict, key);
    if (entry != NULL && entry->ptr_key != NULL && strcmp(entry->ptr_key, key) == 0){
        // update entry
        entry->ptr_value = value;
        return;
    }
    // add new entry
    uint16_t keylen = strlen(key);
    char* keycopy = calloc(1 + keylen, sizeof *keycopy);
    strcpy(keycopy, key);
    entry->ptr_key = keycopy;
    entry->ptr_value = value;
    my_dict->size++;
    
    if ((float)my_dict->size / my_dict->max_capacity > DICT_MAX_LOAD_FACTOR)
        dict_increase_and_rehash(my_dict);
    
    
}

void* lh_dict_get(LH_Dict* my_dict, const char* key) {
    LH_DictItem* entry = dict_get_entry(my_dict, key);
    if (entry == NULL)
        return NULL;
    else if (entry->ptr_key == NULL)
        return NULL;
    else
        return entry->ptr_value;
}

void lh_dict_remove(LH_Dict* my_dict, const char* key){
    LH_DictItem* entry = dict_get_entry(my_dict, key);
    if (my_dict->size > 0 && entry != NULL){
        if(entry->ptr_key != NULL && strcmp(entry->ptr_key, key) == 0){
            strcpy(entry->ptr_key, "");
            my_dict->size --;
        }
            //free(entry->ptr_key);
        //entry->ptr_key = NULL;
        entry->ptr_value = NULL;
    }
}

void lh_dict_free(LH_Dict* my_dict) {
    int j;
    for (j = 0; j < my_dict->max_capacity; j++){
        char* entry_key = (my_dict->items+j)->ptr_key;
        if(entry_key != NULL)
            free(entry_key);
    }
    free(my_dict->items);
    free(my_dict);
}

LH_List* lh_dict_get_keys(LH_Dict* my_dict){
    LH_List* keys = lh_list_new();
    int j;
    for (j = 0; j < my_dict->max_capacity; j++){
        char* entry_key = (my_dict->items+j)->ptr_key;
        if(entry_key != NULL)
            lh_list_add(keys, entry_key);
    }
    return keys;
}

