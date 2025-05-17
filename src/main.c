#include <stdio.h>
#include "../mongoose/mongoose.h"
#include "handlers.h"
#include <signal.h>

static int s_signo;
static void signal_handler(int signo) {
    s_signo = signo;
}

int main() {
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);
    
    if (!mg_http_listen(&mgr, "http://localhost:8000", fn, NULL)) {
        fprintf(stderr, "Ошибка запуска сервера\n");
        return 1;
    }
    
    printf("Сервер запущен на http://localhost:8000\nНажмите Ctrl+C для остановки...\n");
    
    while (s_signo == 0) mg_mgr_poll(&mgr, 1000);
    
    printf("\nСервер остановлен\n");
    mg_mgr_free(&mgr);
    return 0;
}
