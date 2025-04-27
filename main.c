#include "./mongoose/mongoose.h"
#include "./input/input.h"
#include "./constants/constants.h"
#include <stdlib.h>
#include <stdbool.h>

enum {
    ERR_OK = 0,
    ERR_MISSING_ENV = 1,
    ERR_INVALID_CREDENTIALS = 2,
    ERR_FILE_NOT_FOUND = 3
};

static int process_request(struct mg_connection *c, 
                         struct mg_http_message *hm) {
    int status_code = 500;
    const char *ctype = "";
    char *response = NULL;
    int error_code = ERR_FILE_NOT_FOUND;

    // Отладочный вывод
    printf("---\nПолучен запрос:\nURI: %.*s\nMethod: %.*s\n",
           (int)hm->uri.len, hm->uri.ptr,
           (int)hm->method.len, hm->method.ptr);

    if (!mg_strcmp(hm->uri, mg_str("/login")) &&
        !mg_strcasecmp(hm->method, mg_str("POST"))) {
        
        printf("Это запрос авторизации!\n");
        char username[100], password[100];
        const char *expected_user = getenv("LOGIN_USER");
        const char *expected_pass = getenv("LOGIN_PASS");

        mg_http_get_var(&hm->body, "username", username, sizeof(username));
        mg_http_get_var(&hm->body, "password", password, sizeof(password));
        
        printf("Введённые данные: username=%s, password=%s\n", username, password);
        printf("Ожидаемые данные: username=%s, password=%s\n", expected_user, expected_pass);

        if (!expected_user || !expected_pass) {
            error_code = ERR_MISSING_ENV;
        } else {
            if (!strcmp(username, expected_user) && 
                !strcmp(password, expected_pass)) {
                response = read_file(PATH_SUCCESS_HTML);
            } else {
                response = read_file(PATH_ERROR_HTML);
                error_code = ERR_INVALID_CREDENTIALS;
            }

            if (response) {
                status_code = 200;
                ctype = CONTENT_TYPE_HTML;
                error_code = ERR_OK;
            }
        }
    }
    else if (!mg_strcmp(hm->uri, mg_str("/styles.css"))) {
        response = read_file(PATH_CSS_STYLES);
        if (response) {
            status_code = 200;
            ctype = CONTENT_TYPE_CSS;
            error_code = ERR_OK;
        }
    }
    else {
        response = read_file(PATH_LOGIN_HTML);
        if (response) {
            status_code = 200;
            ctype = CONTENT_TYPE_HTML;
            error_code = ERR_OK;
        }
    }

    if (error_code == ERR_OK)
        mg_http_reply(c, status_code, ctype, "%s", response);
    else
        mg_http_reply(c, 500, "", "");

    free(response);
    return error_code;
}

static void main_fun(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *)ev_data;
        process_request(c, hm);
    }
}

int main(void) {
    const char *server_address = "http://localhost:8081";
    struct mg_mgr mgr;
    
    mg_mgr_init(&mgr);
    mg_http_listen(&mgr, server_address, main_fun, NULL);
    
    printf("Сервер запущен на %s\n", server_address);

    for (;;) {
        mg_mgr_poll(&mgr, 1000);
    }
    
    mg_mgr_free(&mgr);
    return 0;
}
