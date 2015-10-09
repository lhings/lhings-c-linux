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

#ifndef UTILS_H
#define	UTILS_H

#ifdef	__cplusplus
extern "C" {
#endif



    /**
     * Returns the string representation of an array of integers, preceded by the
     * 0x prefix. For example, for the array {255, 254, 253} it returns the string
     * "0xfffefd". 
     * @param intArray Original integer array.
     * @param len The length of intArray
     * @param hexStr A pointer to the resulting char array. This array must have
     * enough space to hold the resulting string.
     * @return A pointer to the resulting char array.
     */
    char* encode_hex(const uint8_t *intArray, size_t len, char* hexStr);

    /**
     * Returns true if host processor is big endian, false otherwise.
     * @return 
     */
    int is_big_endian(void);

    /**
     * Given a 32 bit integer with host endianness returns an array of bytes
     * containing its big endian representation. 
     * @param number The 32 bit integer to be converted to byte array.
     * @param byte_array The starting address of the byte array the resulting
     * bytes will be copied to.
     * @return 
     */
    uint8_t* uint32_to_byte_array(uint32_t number, uint8_t *byte_array);

    /**
     * Given a big endian byte array returns a 32 bit integer with host
     * endianness.
     * @param byte_array
     * @return 
     */
    uint32_t byte_array_to_uint32(const uint8_t *byte_array);


    /**
     * Given a big endian byte array returns a 16 bit integer with host
     * endianness.
     * @param byte_array
     * @return 
     */
    uint16_t byte_array_to_uint16_t(const uint8_t *byte_array);

    /**
     * Given a 16 bit integer with host endianness returns an array of bytes
     * containing its big endian representation.
     * @param number
     * @param byte_array
     * @return 
     */
    uint8_t* uint16_to_byte_array(uint16_t number, uint8_t *byte_array);

    /**
     * Converts an uuid in string representation to a byte array representation.
     * @param uuid 
     * @param bytes A preallocated array of bytes where the byte representation
     * will be stored.
     * @return 1 on success, 0 otherwise (e. g. if the string cannot be processed
     * as an uuid).
     */
    int uuid_string_to_byte_array(const char *uuid, uint8_t *bytes);

    /**
     * Converts a byte array containing an IEEE 754 floating-point type to float.
     * The conversion is performed by obtaining the equivalent uint32_t type and
     * then using an union to obtain the correct type.
     * @param byte_array
     * @return 
     */
    float byte_array_to_float(const uint8_t *byte_array);

    /**
     * 
     * @param number
     */
    void float_to_byte_array(float number, uint8_t * byte_array);
    
    
#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_H */

