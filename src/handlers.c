#include "handlers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int uri_match(struct mg_str uri, const char *pattern) {
    size_t len = strlen(pattern);
    return uri.len == len && memcmp(uri.buf, pattern, len) == 0;
}

static char* load_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = malloc(length + 1);
    if (buffer) {
        fread(buffer, 1, length, file);
        buffer[length] = '\0';
    }

    fclose(file);
    return buffer;
}

void fn(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *)ev_data;
        
        if (uri_match(hm->uri, "/")) {
            char* page = load_file("templates/index.html");
            mg_http_reply(c, page ? 200 : 404, 
                         "Content-Type: text/html\r\n", 
                         page ? page : "404 Not Found");
            free(page);
        }
        else if (uri_match(hm->uri, "/calculate")) {
            char distance[20], speed[20];
            mg_http_get_var(&hm->body, "distance", distance, sizeof(distance));
            mg_http_get_var(&hm->body, "speed", speed, sizeof(speed));
            
            double dist = atof(distance);
            double spd = atof(speed);
            
            if (dist > 0 && spd > 0) {
                double hours = dist / spd;
                int h = (int)hours;
                int m = (int)((hours - h) * 60);
                
                char* template = load_file("templates/result.html");
                if (template) {
                    char result[2048];
                    snprintf(result, sizeof(result), template, h, m, dist, spd);
                    mg_http_reply(c, 200, "Content-Type: text/html\r\n", result);
                    free(template);
                } else {
                    mg_http_reply(c, 404, "Content-Type: text/plain\r\n", "404 Not Found");
                }
            } else {
                char* page = load_file("templates/error.html");
                mg_http_reply(c, page ? 400 : 404, 
                             "Content-Type: text/html\r\n", 
                             page ? page : "404 Not Found");
                free(page);
            }
        }
        else {
            struct mg_http_serve_opts opts = { 
                .root_dir = ".",
                .ssi_pattern = NULL
            };
            mg_http_serve_dir(c, hm, &opts);
        }
    }
}
