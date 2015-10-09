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
 * @file http_api.h
 * @brief This file defines all the functions related to HTTP communication.
 * 
 * The HTTP API defines a set of generic functions used by the Lhings C library
 * to perform all kind of HTTP request. 
 * 
 * All the functions in this header file belong to the abstraction API of the 
 * library and need to be reimplemented when changing platform. The documentation
 * of each function contains all the information about its expected behaviour. This
 * information must be carefully followed when porting the library to other platforms.
 */

#ifndef HTTP_API_H
#define	HTTP_API_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "../../core/utils/data_structures.h"
    
#define PERFORM_GET  0
#define PERFORM_POST 1
#define PERFORM_PUT  2
#define HTTP_OK 200
#define HTTP_CREATED 201
    
    /**
     * Data type that encapsulates an HTTP response.
     */
    typedef struct http_response{
        /**
         * Body of the response.
         */
        char* response_body;
        //char* http_message;
        /**
         * HTTP status code of the response.
         */
        uint16_t *http_code;
    }LH_HttpResponse;

    /**
     * Performs an HTTP GET request to the given url, sending the given
     * headers to the server. Stores the HTTP code of the response and the response
     * body in a struct LH_HttpResponse. 
     * @param url The url of the request.
     * @param headers An LH_Dict containing the key value pairs of the headers (for instance key "Content-type", value "application/json").
     * @return A pointer to LH_HttpResponse containing the HTTP code and body of the response.
     */
    LH_HttpResponse* lh_http_execute_get(const char *url, LH_Dict *headers);
    /**
     * Performs an HTTP POST request to the given url, sending the given
     * headers to the server, and the given request body. Stores the HTTP code of the response and the response
     * body in a struct LH_HttpResponse. 
     * @param url The url of the request.
     * @param headers An LH_Dict containing the key value pairs of the headers (for instance key "Content-type", value "application/json").
     * @param post_body A string containing the request body.
     * @return A pointer to LH_HttpResponse containing the HTTP code and body of the response.
     */
    LH_HttpResponse* lh_http_execute_post(const char *url, LH_Dict *headers, const char *post_body);
    /**
     * Performs an HTTP PUT request to the given url, sending the given
     * headers to the server, and the given request body. Stores the HTTP code of the response and the response
     * body in a struct LH_HttpResponse. 
     * @param url The url of the request.
     * @param headers An LH_Dict containing the key value pairs of the headers (for instance key "Content-type", value "application/json").
     * @param post_body A string containing the request body.
     * @return A pointer to LH_HttpResponse containing the HTTP code and body of the response.
     */
    LH_HttpResponse* lh_http_execute_put(const char *url, LH_Dict *headers, const char *put_body);
    /**
     * Function used to free the memory allocated for LH_HttpResponse*  by any of the @c lh_http_execute_* functions.
     * @param response 
     */
    void lh_http_free(LH_HttpResponse* response);
    
#ifdef	__cplusplus
}
#endif

#endif	/* HTTP_API_H */

