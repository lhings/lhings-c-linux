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

/**
 * @file storage_api.h
 * @brief Contains an API that abstracts the access to permanent storage.
 * 
 * Basically this API must provide the functionality needed to store the uuid
 * of the device and its name between device executions. 
 * 
 * All the functions in this header file belong to the abstraction API of the 
 * library and need to be reimplemented when changing platform. The documentation
 * of each function contains all the information about its expected behaviour. This
 * information must be carefully followed when porting the library to other platforms.
 * 
 */

#ifndef CONFIG_FILE_H
#define	CONFIG_FILE_H

#ifdef	__cplusplus
extern "C" {
#endif

    /** 
     * Searchs in the permanent storage for the uuid of a device with the given
     * name. 
     * 
     * @param requested_device_name The name of the device whose uuid is needed.
     * @return A pointer to a string containing the uuid of the device or a null
     * pointer if it could not be found.
     */
    char* lh_storage_get_uuid(char* requested_device_name);
    
    /**
     * Saves the uuid of the device with the given name in the permanent storage,
     * so that it can be retrieved later using lh_storage_get_uuid.
     * 
     * Any storage API and/or hardware can be used, as long as it allows to 
     * store the pairs device name - uuid, in a way that they can be searched
     * and retrieved later (even after device reboot or shutdown).
     * 
     * @param device_name A pointer to a string containing the name of the device.
     * @param uuid A pointer to a string containing the uuid of the device. The uuid 
     * will always be a string like xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx, where x 
     * can be any hexadecimal digit ([0-9abcdef]).
     * @return 1 if the pair name, uuid could be successfully saved, 0 otherwise.
     * 
     */
    int lh_storage_save_uuid(const char *device_name, const char *uuid);
    


#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_FILE_H */

