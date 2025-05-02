#include <stdio.h>
#include "../mongoose/mongoose.h"
#include "handlers.h"

int main() {
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);
    
    if (!mg_http_listen(&mgr, "http://localhost:8000", fn, NULL)) {
        fprintf(stderr, "Ошибка запуска сервера\n");
        return 1;
    }
    
    printf("Сервер запущен на http://localhost:8000\n");
    
    while (true) mg_mgr_poll(&mgr, 1000);
    
    mg_mgr_free(&mgr);
    return 0;
}

