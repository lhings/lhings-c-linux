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


#ifndef DATA_STRUCTURES_H
#define	DATA_STRUCTURES_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
    
#define LIST_INITIAL_CAPACITY 10
#define LIST_INCREMENT_CAPACITY_FACTOR 2
#define DICT_INITIAL_CAPACITY 10
#define DICT_INCREMENT_CAPACITY_FACTOR 2
#define DICT_MAX_LOAD_FACTOR 0.5
    
    typedef struct collection_item {
        void* ptr_this;
    }LH_ListItem;
    
    typedef struct list {
        LH_ListItem* items;
        uint16_t size, max_capacity;
    }LH_List;
    
    typedef struct dictionary_item {
        char* ptr_key;
        void* ptr_value;
    }LH_DictItem;
    
    typedef struct dictionary {
        LH_DictItem* items;
        uint16_t size, max_capacity;
    }LH_Dict;
    
    /**
     * Creates a new list with default initial capacity. 
     * 
     * The list is a data structure provided by the library that can store
     * any number of elements of any type. If the list is full, it is automatically
     * resized to allow room for more elements. The memory needed by the list
     * is dynamically allocated, so do not forget to free that memory using
     * the lh_list_free.
     * 
     * The default initial
     * capacity can be changed at compile time by changing the value of the
     * directive LIST_INITIAL_CAPACITY in lhings.h.
     * @return A pointer to the LH_List struct created.
     */
    LH_List* lh_list_new();
    /**
     * Creates a new list with the given default initial capacity. If the size
     * of the list is known in advanced providing its value will allow making
     * better use of available memory. 
     * @param capacity
     * @return A pointer to the LH_List struct created.
     */
    LH_List* lh_list_new_custom_capacity(int capacity);
    
    /**
     * Adds a new item to the list. If full the list is resized, multiplying 
     * its current max_capacity by the constant LIST_INCREMENT_CAPACITY_FACTOR
     * defined in lhings.h.
     * @param my_list
     * @param item
     */
    void lh_list_add(LH_List* my_list, void* item);
    /**
     * Retrieves the item of the list with the given index. The index of the 
     * first and last items are 0, and my_list->size - 1, respectively. 
     * @param my_list
     * @param index
     * @return A void pointer to the item. This pointer should be cast to the
     * appropriate data type before using it. If index is negative or greater than
     * my_list->size - 1, NULL is returned.
     */
    void* lh_list_get(LH_List* my_list, uint16_t index);
    
    /**
     * Frees the memory allocated by lh_list_new and lh_list_new_custom_capacity.
     * @param my_list
     */
    void lh_list_free(LH_List* my_list);
    
    /**
     * Creates a new dictionary with default capacity.
     * 
     * A dictionary is a dynamic data structure that stores key - value pairs.
     * In this dictionary implementation, keys are always strings, and values can
     * be any data type. This implementation uses a simple and fast hash based
     * approach to index items and being able to retrieve them. 
     * 
     * When the dictionary is full, its size is automatically increased and its
     * elements rehashed.
     * 
     * The default initial
     * capacity can be changed at compile time by changing the value of the
     * directive DICT_INITIAL_CAPACITY in lhings.h.
     * @return A pointer to the LH_Dict struct created.
     */
    LH_Dict* lh_dict_new();
    
    /**
     * Creates a new dictionary with given capacity. 
     * 
     * If the size
     * of the dictionary is known in advanced providing its value will allow making
     * better use of available memory.
     * @param capacity
     * @return 
     */
    LH_Dict* lh_dict_new_custom_capacity(int capacity);
    /**
     * Stores the given key - value pair in the dictionary. 
     * 
     * This function makes a copy of the given key, which is automatically freed
     * when calling lh_dict_free. No copy is make of the value, only the pointer 
     * to it is stored. 
     * 
     * @param my_dict
     * @param key A pointer to the string that holds the value of the key. Bear in mind that this dictionary implementation does not allow for NULL keys.
     * @param value
     */
    void lh_dict_put(LH_Dict* my_dict, const char* key, void* value);
    /**
     * Retrieves the value of the given key.
     * 
     * @param my_dict
     * @param key
     * @return A void pointer to the value. It should be cast to the appropriate
     * data type before using it. If the key is not found NULL is returned.
     */
    void* lh_dict_get(LH_Dict* my_dict, const char* key);
    /**
     * Removes the key - value pair from the dictionary for the given key.
     * If the dictionary does not contain the given key, the call has no effect.
     * @param my_dict
     * @param key
     */
    void lh_dict_remove(LH_Dict* my_dict, const char* key);
    
    /**
     * Frees the memory allocated by lh_dict_new and lh_dict_new_custom_capacity.
     * @param my_dict
     */
    void lh_dict_free(LH_Dict* my_dict);
    
    /**
     * Returns a list with the keys of the given dictionary.
     * 
     * Useful to iterate over dictionary elements.
     * @param my_dict
     * @return 
     */
    LH_List* lh_dict_get_keys(LH_Dict* my_dict);

#ifdef	__cplusplus
}
#endif

#endif	/* DATA_STRUCTURES_H */

