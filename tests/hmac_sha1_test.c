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

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../core/utils/utils.h"
#include "../core/crypto/hmac.h"




/**
 * Runs the test cases for HMAC-SHA1 defined in RFC 2202.
 * @return 
 */
int hmac_sha1_tests() {
    puts("**********************************************************");
    puts("****Running HMAC-SHA1 tests (as specified in RFC 2022)****");
    puts("**********************************************************");
    puts("");


    //    test_case =     1
    //    key =           0x0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b
    //    key_len =       20
    //    data =          "Hi There"
    //    data_len =      8
    //    digest =        0xb617318655057264e28bc0b6fb378c8ef146be00
    printf("TEST CASE 1: ");
    uint8_t key1[20] = {0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b};
    char *in1 = "Hi There";
    uint8_t digest[20];
    char hexStr[43] = "";

    int fail = hmac_sha1(key1, 20, in1, 8, digest);
    encode_hex(digest, 20, hexStr);
    if (fail) {
        printf("FAILED! HMAC-SHA1 could not be calculated.\n");
        return EXIT_FAILURE;
    } else if (strcmp(hexStr, "0xb617318655057264e28bc0b6fb378c8ef146be00")) {
        printf("FAILED! HMAC-SHA1: \n\texpected 0xb617318655057264e28bc0b6fb378c8ef146be00\n\treturned %s\n", hexStr);
        return EXIT_FAILURE;
    } else {
        printf("OK\n");
    }
    

    //    test_case =     2
    //    key =           "Jefe"
    //    key_len =       4
    //    data =          "what do ya want for nothing?"
    //    data_len =      28
    //    digest =        0xeffcdf6ae5eb2fa2d27416d5f184df9c259a7c79
    printf("TEST CASE 2: ");
    char key2[] = "Jefe";
    char *in2 = "what do ya want for nothing?";
    fail = hmac_sha1(key2, 4, in2, 28, digest);
    encode_hex(digest, 20, hexStr);
    if (fail) {
        printf("FAILED! HMAC-SHA1 could not be calculated.\n");
        return EXIT_FAILURE;
    } else if (strcmp(hexStr, "0xeffcdf6ae5eb2fa2d27416d5f184df9c259a7c79")) {
        printf("FAILED! HMAC-SHA1: \n\texpected 0xeffcdf6ae5eb2fa2d27416d5f184df9c259a7c79\n\treturned %s\n", hexStr);
        return EXIT_FAILURE;
    } else {
        printf("OK\n");
    }
    

    //    test_case =     3
    //    key =           0xaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
    //    key_len =       20
    //    data =          0xdd repeated 50 times
    //    data_len =      50
    //    digest =        0x125d7342b9ac11cd91a39af48aa17b4f63f175d3
    printf("TEST CASE 3: ");
    uint8_t key3[] = { 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa };
    uint8_t in3[50];
    int j;
    for (j = 0; j < 50; j++)
        in3[j] = 0xdd;
    
    fail = hmac_sha1(key3, 20, in3, 50, digest);
    encode_hex(digest, 20, hexStr);
    if (fail) {
        printf("FAILED! HMAC-SHA1 could not be calculated.\n");
        return EXIT_FAILURE;
    } else if (strcmp(hexStr, "0x125d7342b9ac11cd91a39af48aa17b4f63f175d3")) {
        printf("FAILED! HMAC-SHA1: \n\texpected 0x125d7342b9ac11cd91a39af48aa17b4f63f175d3\n\treturned %s\n", hexStr);
        return EXIT_FAILURE;
    } else {
        printf("OK\n");
    }
    

    //test_case =     4
    //key =           0x0102030405060708090a0b0c0d0e0f10111213141516171819
    //key_len =       25
    //data =          0xcd repeated 50 times
    //data_len =      50
    //digest =        0x4c9007f4026250c6bc8414f9bf50c86c2d7235da
    printf("TEST CASE 4: ");
    uint8_t key4[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19 };
    uint8_t in4[50];

    for (j = 0; j < 50; j++)
        in4[j] = 0xcd;
    
    fail = hmac_sha1(key4, 25, in4, 50, digest);
    encode_hex(digest, 20, hexStr);
    if (fail) {
        printf("FAILED! HMAC-SHA1 could not be calculated.\n");
        return EXIT_FAILURE;
    } else if (strcmp(hexStr, "0x4c9007f4026250c6bc8414f9bf50c86c2d7235da")) {
        printf("FAILED! HMAC-SHA1: \n\texpected 0x4c9007f4026250c6bc8414f9bf50c86c2d7235da\n\treturned %s\n", hexStr);
        return EXIT_FAILURE;
    } else {
        printf("OK\n");
    }
    
    //test_case =     5
    //key =           0x0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c
    //key_len =       20
    //data =          "Test With Truncation"
    //data_len =      20
    //digest =        0x4c1a03424b55e07fe7f27be1d58bb9324a9a5a04
    printf("TEST CASE 5: ");
    uint8_t key5[] = { 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c };
    char in5[50] = "Test With Truncation";
    for (j = 0; j < 50; j++)
        in3[j] = 0xdd;
    
    fail = hmac_sha1(key5, 20, in5, 20, digest);
    encode_hex(digest, 20, hexStr);
    if (fail) {
        printf("FAILED! HMAC-SHA1 could not be calculated.\n");
        return EXIT_FAILURE;
    } else if (strcmp(hexStr, "0x4c1a03424b55e07fe7f27be1d58bb9324a9a5a04")) {
        printf("FAILED! HMAC-SHA1: \n\texpected 0x4c1a03424b55e07fe7f27be1d58bb9324a9a5a04\n\treturned %s\n", hexStr);
        return EXIT_FAILURE;
    } else {
        printf("OK\n");
    }
    
    //test_case =     6
    //key =           0xaa repeated 80 times
    //key_len =       80
    //data =          "Test Using Larger Than Block-Size Key - Hash Key First"
    //data_len =      54
    //digest =        0xaa4ae5e15272d00e95705637ce8a3b55ed402112
    printf("TEST CASE 6: ");
    uint8_t key6[80];
    char in6[] = "Test Using Larger Than Block-Size Key - Hash Key First";

    for (j = 0; j < 80; j++)
        key6[j] = 0xaa;
    
    fail = hmac_sha1(key6, 80, in6, 54, digest);
    encode_hex(digest, 20, hexStr);
    if (fail) {
        printf("FAILED! HMAC-SHA1 could not be calculated.\n");
        return EXIT_FAILURE;
    } else if (strcmp(hexStr, "0xaa4ae5e15272d00e95705637ce8a3b55ed402112")) {
        printf("FAILED! HMAC-SHA1: \n\texpected 0xaa4ae5e15272d00e95705637ce8a3b55ed402112\n\treturned %s\n", hexStr);
        return EXIT_FAILURE;
    } else {
        printf("OK\n");
    }
    
    //test_case =     7
    //key =           0xaa repeated 80 times
    //key_len =       80
    //data =          "Test Using Larger Than Block-Size Key and Larger
    //                Than One Block-Size Data"
    //data_len =      73
    //digest =        0xe8e99d0f45237d786d6bbaa7965c7808bbff1a91
    printf("TEST CASE 7: ");
    uint8_t key7[80];
    char in7[] = "Test Using Larger Than Block-Size Key and Larger Than One Block-Size Data";
    
    for (j = 0; j < 80; j++)
        key7[j] = 0xaa;
    
    fail = hmac_sha1(key7, 80, in7, 73, digest);
    encode_hex(digest, 20, hexStr);
    if (fail) {
        printf("FAILED! HMAC-SHA1 could not be calculated.\n");
        return EXIT_FAILURE;
    } else if (strcmp(hexStr, "0xe8e99d0f45237d786d6bbaa7965c7808bbff1a91")) {
        printf("FAILED! HMAC-SHA1: \n\texpected 0xe8e99d0f45237d786d6bbaa7965c7808bbff1a91\n\treturned %s\n", hexStr);
        return EXIT_FAILURE;
    } else {
        printf("OK\n");
    }
    
    
    return EXIT_SUCCESS;
}


