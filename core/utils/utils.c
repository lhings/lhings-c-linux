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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

char* encode_hex(const uint8_t *intArray, size_t len, char* hexStr) {
    char byte[3] = "FF";
    int j;

    strcpy(hexStr, "0x");

    for (j = 0; j < len; j++) {
        sprintf(byte, "%02x", *(intArray + j));
        strcat(hexStr, byte);
    }

    return hexStr;
}

int is_big_endian(void) {

    union {
        uint32_t i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1;
}

uint8_t* uint32_to_byte_array(uint32_t number, uint8_t *byte_array) {
    if (is_big_endian) {
        byte_array[0] = number >> 24;
        byte_array[1] = (number & 0x00ff0000) >> 16;
        byte_array[2] = (number & 0x0000ff00) >> 8;
        byte_array[3] = number & 0x000000ff;
    } else {
        byte_array[0] = number & 0x000000ff;
        byte_array[1] = (number & 0x0000ff00) >> 8;
        byte_array[2] = (number & 0x00ff0000) >> 16;
        byte_array[3] = number >> 24;
    }
    return byte_array;
}

uint32_t byte_array_to_uint32(const uint8_t *byte_array) {
    if (is_big_endian) {
        return (uint32_t) (byte_array[0] << 24 | byte_array[1] << 16 | byte_array[2] << 8 | byte_array[3]);
    } else {
        return (uint32_t) (byte_array[3] << 24 | byte_array[2] << 16 | byte_array[1] << 8 | byte_array[0]);
    }
}

uint8_t* uint16_to_byte_array(uint16_t number, uint8_t *byte_array) {
    if (is_big_endian) {
        byte_array[0] = number >> 8;
        byte_array[1] = number & 0x00ff;
    } else {
        byte_array[0] = number & 0x00ff;
        byte_array[1] = number >> 8;
    }
    return byte_array;
}

uint16_t byte_array_to_uint16_t(const uint8_t *byte_array) {

    if (is_big_endian) {
        return (uint16_t) (byte_array[0] << 8 | byte_array[1]);
    } else
        return (uint16_t) (byte_array[1] << 8 | byte_array[0]);
}

int uuid_string_to_byte_array(const char *uuid, uint8_t *bytes) {
    char buffer[5];
    buffer[0] = uuid[0];
    buffer[1] = uuid[1];
    buffer[2] = uuid[2];
    buffer[3] = uuid[3];
    buffer[4] = 0;
    long number = strtol(buffer, NULL, 16);
    uint16_to_byte_array((uint16_t) number, bytes);
    buffer[0] = uuid[4];
    buffer[1] = uuid[5];
    buffer[2] = uuid[6];
    buffer[3] = uuid[7];
    number = strtol(buffer, NULL, 16);
    uint16_to_byte_array((uint16_t) number, bytes + 2);
    buffer[0] = uuid[9];
    buffer[1] = uuid[10];
    buffer[2] = uuid[11];
    buffer[3] = uuid[12];
    number = strtol(buffer, NULL, 16);
    uint16_to_byte_array((uint16_t) number, bytes + 4);
    buffer[0] = uuid[14];
    buffer[1] = uuid[15];
    buffer[2] = uuid[16];
    buffer[3] = uuid[17];
    number = strtol(buffer, NULL, 16);
    uint16_to_byte_array((uint16_t) number, bytes + 6);
    buffer[0] = uuid[19];
    buffer[1] = uuid[20];
    buffer[2] = uuid[21];
    buffer[3] = uuid[22];
    number = strtol(buffer, NULL, 16);
    uint16_to_byte_array((uint16_t) number, bytes + 8);
    buffer[0] = uuid[24];
    buffer[1] = uuid[25];
    buffer[2] = uuid[26];
    buffer[3] = uuid[27];
    number = strtol(buffer, NULL, 16);
    uint16_to_byte_array((uint16_t) number, bytes + 10);
    buffer[0] = uuid[28];
    buffer[1] = uuid[29];
    buffer[2] = uuid[30];
    buffer[3] = uuid[31];
    number = strtol(buffer, NULL, 16);
    uint16_to_byte_array((uint16_t) number, bytes + 12);
    buffer[0] = uuid[32];
    buffer[1] = uuid[33];
    buffer[2] = uuid[34];
    buffer[3] = uuid[35];
    number = strtol(buffer, NULL, 16);
    uint16_to_byte_array((uint16_t) number, bytes + 14);
    return 1;
}

float byte_array_to_float(const uint8_t *byte_array) {
    uint32_t number = byte_array_to_uint32(byte_array);

    union {
        uint32_t i;
        float f;
    } u;
    u.i = (uint32_t) number;
    return (float) u.f;
}

void float_to_byte_array(float number, uint8_t * byte_array) {

    union {
        uint32_t i;
        float f;
    } u;
    u.f = number;
    uint32_to_byte_array(u.i, byte_array);
}