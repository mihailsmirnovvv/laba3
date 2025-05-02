#include "handlers.h"
#include "../constants/constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int uri_match(struct mg_str uri, const char *pattern) {
    size_t len = strlen(pattern);
    return uri.len == len && memcmp(uri.buf, pattern, len) == 0;
}

void fn(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *)ev_data;
        
        if (uri_match(hm->uri, "/")) {
            mg_http_reply(c, 200, "Content-Type: text/html\r\n", MAIN_PAGE);
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
                
                char result[1024];
                snprintf(result, sizeof(result), RESULT_TEMPLATE, h, m, dist, spd);
                mg_http_reply(c, 200, "Content-Type: text/html\r\n", result);
            } else {
                mg_http_reply(c, 400, "Content-Type: text/html\r\n", ERROR_PAGE);
            }
        }
        else {
            struct mg_http_serve_opts opts = { .root_dir = "./" };
            mg_http_serve_dir(c, hm, &opts);
        }
        
    }
}
