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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "storage_api.h"
#include "../../core/logging/log.h"


#define UUIDS_FILENAME "uuid.list"

char* lh_storage_get_uuid(char* requested_device_name){
    char buffer[80];
    FILE *uuid_file = fopen(UUIDS_FILENAME, "a+");
    if (uuid_file == NULL){
        char *message = lh_get_message_str("Could not read uuid file. Reason: %s \n", strerror(errno));
        log_error(message);
        free(message);
        return NULL;
    }

    char *returned_uuid = NULL;
    
    while (fgets(buffer, 79, uuid_file) != NULL){
        char *dev_name = strtok(buffer, "=");
        char *dev_uuid = strtok(NULL, "\n");
        if (strcmp(dev_name, requested_device_name) == 0){
            returned_uuid = malloc((strlen(dev_uuid)+1)*sizeof returned_uuid);
            strcpy(returned_uuid, dev_uuid);
        }
    }
    fclose(uuid_file);
    return returned_uuid;
    
}

int lh_storage_save_uuid(const char *device_name, const char *uuid){
    FILE *uuid_file = fopen(UUIDS_FILENAME, "a+");
    if (uuid_file == NULL){
        char *message = lh_get_message_str("Could not save uuid file. Reason: %s \n", strerror(errno));
        log_error(message);
        free(message);
        return 0;
    }
    fprintf(uuid_file, "%s=%s\n", device_name, uuid);
    fclose(uuid_file);
    return 1;
}