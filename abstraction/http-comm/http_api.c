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

#include <curl/curl.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "http_api.h"
#include "../../core/utils/data_structures.h"
#include "../../core/logging/log.h"

struct string {
    char *ptr;
    size_t len;
};

void init_string(struct string *s) {
    s->len = 0;
    s->ptr = malloc(s->len + 1);
    if (s->ptr == NULL) {
        log_error("Could not allocate memory (malloc) while reading response from server\n");
    }
    s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s) {
    size_t new_len = s->len + size*nmemb;
    s->ptr = realloc(s->ptr, new_len + 1);
    if (s->ptr == NULL) {
        log_error("Could not allocate memory (realloc) while reading response from server\n");
        return 0;
    }
    memcpy(s->ptr + s->len, ptr, size * nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;

    return size*nmemb;
}

void lh_http_free(LH_HttpResponse* response) {
    free(response->http_code);
    //free(response->http_message);
    free(response->response_body);
    free(response);
}

LH_HttpResponse* http_execute_request(const char* url, LH_Dict* headers, uint8_t method, const char* request_body) {
    CURL *curl = curl_easy_init();
    if (curl) {
        CURLcode res;
        LH_List* keys_of_headers = lh_dict_get_keys(headers);
        struct string s;
        init_string(&s);
        int j;
        struct curl_slist *header_list = NULL;
        for (j = 0; j < headers->size; j++) {
            char* header_key = lh_list_get(keys_of_headers, j);
            char* header_value = (char*) lh_dict_get(headers, header_key);
            uint16_t header_len = strlen(header_key) + strlen(header_value) + 3; // +3 because we will add ": " and the null terminating byte.
            char* header_text = malloc(header_len * sizeof header_text);
            header_text[0] = 0;
            strcat(header_text, header_key);
            strcat(header_text, ": ");
            strcat(header_text, header_value);
            header_list = curl_slist_append(header_list, header_text);
            free(header_text);
        }
        lh_list_free(keys_of_headers);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        if (method == PERFORM_POST) {
            curl_easy_setopt(curl, CURLOPT_POST, 1);
            if (request_body != NULL)
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request_body);

        }
        if (method == PERFORM_PUT) {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
            if (request_body != NULL)
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request_body);
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

        res = curl_easy_perform(curl);
	
        if (res != CURLE_OK) {
            char* message_template = "Request to url %s failed. CURLcode %d.";
            char* message = malloc((strlen(url) + strlen(message_template) + 15) * sizeof message);
            sprintf(message, message_template, url, res);
            log_error(message);
            free(message);
        }
        long http_response_code_long;
        uint16_t* http_response_code = malloc(sizeof http_response_code);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_response_code_long);
        *http_response_code = (uint16_t) http_response_code_long;
        LH_HttpResponse* response = malloc(sizeof *response);
        response->http_code = http_response_code;
        response->response_body = s.ptr;
        curl_slist_free_all(header_list);
        curl_easy_cleanup(curl);
        return response;
    }
    return NULL;
}

LH_HttpResponse* lh_http_execute_get(const char* url, LH_Dict* headers) {
    return http_execute_request(url, headers, PERFORM_GET, NULL);
}

LH_HttpResponse* lh_http_execute_post(const char* url, LH_Dict* headers, const char* post_body) {
    return http_execute_request(url, headers, PERFORM_POST, post_body);
}

LH_HttpResponse* lh_http_execute_put(const char* url, LH_Dict* headers, const char* put_body) {
    return http_execute_request(url, headers, PERFORM_PUT, put_body);
}
