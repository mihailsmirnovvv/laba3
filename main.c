#include <stdio.h>
#include <string.h>
#include "mongoose.h"

static int uri_match(struct mg_str uri, const char *pattern) {
    size_t pattern_len = strlen(pattern);
    return uri.len == pattern_len && memcmp(uri.buf, pattern, pattern_len) == 0;
}

static void fn(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *)ev_data;
        
        if (uri_match(hm->uri, "/")) {
            mg_http_reply(c, 200, 
                "Content-Type: text/html; charset=utf-8\r\n",
                "<!DOCTYPE html>"
                "<html lang='ru'>"
                "<head>"
                "  <meta charset='UTF-8'>"
                "  <meta name='viewport' content='width=device-width, initial-scale=1.0'>"
                "  <title>Калькулятор времени в пути</title>"
                "  <style>"
                "    body {"
                "      font-family: 'Arial', sans-serif;"
                "      background: linear-gradient(135deg, #f5f7fa 0%, #c3cfe2 100%);"
                "      margin: 0;"
                "      padding: 0;"
                "      display: flex;"
                "      justify-content: center;"
                "      align-items: center;"
                "      min-height: 100vh;"
                "    }"
                "    .container {"
                "      background: white;"
                "      border-radius: 10px;"
                "      box-shadow: 0 10px 25px rgba(0, 0, 0, 0.1);"
                "      padding: 2rem;"
                "      width: 100%;"
                "      max-width: 500px;"
                "    }"
                "    h1 {"
                "      color: #2c3e50;"
                "      text-align: center;"
                "      margin-bottom: 1.5rem;"
                "    }"
                "    .form-group {"
                "      margin-bottom: 1.5rem;"
                "    }"
                "    label {"
                "      display: block;"
                "      margin-bottom: 0.5rem;"
                "      color: #34495e;"
                "      font-weight: 600;"
                "    }"
                "    input {"
                "      width: 100%;"
                "      padding: 0.75rem;"
                "      border: 2px solid #dfe6e9;"
                "      border-radius: 5px;"
                "      font-size: 1rem;"
                "      transition: border 0.3s;"
                "    }"
                "    input:focus {"
                "      border-color: #3498db;"
                "      outline: none;"
                "    }"
                "    button {"
                "      width: 100%;"
                "      padding: 0.75rem;"
                "      background: #3498db;"
                "      color: white;"
                "      border: none;"
                "      border-radius: 5px;"
                "      font-size: 1rem;"
                "      font-weight: 600;"
                "      cursor: pointer;"
                "      transition: background 0.3s;"
                "    }"
                "    button:hover {"
                "      background: #2980b9;"
                "    }"
                "    .result {"
                "      text-align: center;"
                "      margin-top: 1.5rem;"
                "      padding: 1rem;"
                "      background: #f8f9fa;"
                "      border-radius: 5px;"
                "    }"
                "    .error {"
                "      color: #e74c3c;"
                "      text-align: center;"
                "    }"
                "    a {"
                "      display: inline-block;"
                "      margin-top: 1rem;"
                "      color: #3498db;"
                "      text-decoration: none;"
                "      font-weight: 600;"
                "    }"
                "    a:hover {"
                "      text-decoration: underline;"
                "    }"
                "  </style>"
                "</head>"
                "<body>"
                "  <div class='container'>"
                "    <h1>Калькулятор времени в пути</h1>"
                "    <form method='POST' action='/calculate'>"
                "      <div class='form-group'>"
                "        <label for='distance'>Расстояние (км):</label>"
                "        <input type='number' id='distance' name='distance' step='0.1' min='0.1' required>"
                "      </div>"
                "      <div class='form-group'>"
                "        <label for='speed'>Средняя скорость (км/ч):</label>"
                "        <input type='number' id='speed' name='speed' step='1' min='1' required>"
                "      </div>"
                "      <button type='submit'>Рассчитать</button>"
                "    </form>"
                "  </div>"
                "</body>"
                "</html>");
            return;
        }
        
        if (uri_match(hm->uri, "/calculate")) {
    char distance[20], speed[20];
    mg_http_get_var(&hm->body, "distance", distance, sizeof(distance));
    mg_http_get_var(&hm->body, "speed", speed, sizeof(speed));
    
    double dist = atof(distance);
    double spd = atof(speed);
    
    if (dist > 0 && spd > 0) {
        double hours = dist / spd;
        int h = (int)hours;
        int m = (int)((hours - h) * 60);
        
        mg_http_reply(c, 200, 
            "Content-Type: text/html; charset=utf-8\r\n",
            "<!DOCTYPE html>"
            "<html lang='ru'>"
            "<head>"
            "  <meta charset='UTF-8'>"
            "  <meta name='viewport' content='width=device-width, initial-scale=1.0'>"
            "  <title>Результат расчёта</title>"
            "  <style>"
            "    body {"
            "      font-family: 'Arial', sans-serif;"
            "      background: linear-gradient(135deg, #f5f7fa 0%, #c3cfe2 100%);"
            "      margin: 0;"
            "      padding: 0;"
            "      display: flex;"
            "      justify-content: center;"
            "      align-items: center;"
            "      min-height: 100vh;"
            "    }"
            "    .container {"
            "      background: white;"
            "      border-radius: 10px;"
            "      box-shadow: 0 10px 25px rgba(0, 0, 0, 0.1);"
            "      padding: 2rem;"
            "      width: 100%;"
            "      max-width: 500px;"
            "      text-align: center;"
            "    }"
            "    h1 {"
            "      color: #2c3e50;"
            "      margin-bottom: 1.5rem;"
            "    }"
            "    .result-box {"
            "      background: #f8f9fa;"
            "      border-radius: 8px;"
            "      padding: 1.5rem;"
            "      margin: 1.5rem 0;"
            "    }"
            "    .result-value {"
            "      font-size: 1.5rem;"
            "      color: #3498db;"
            "      font-weight: bold;"
            "      margin: 0.5rem 0;"
            "    }"
            "    .btn {"
            "      display: inline-block;"
            "      padding: 0.75rem 1.5rem;"
            "      background: #3498db;"
            "      color: white;"
            "      border: none;"
            "      border-radius: 5px;"
            "      font-size: 1rem;"
            "      font-weight: 600;"
            "      cursor: pointer;"
            "      text-decoration: none;"
            "      transition: background 0.3s;"
            "      margin-top: 1rem;"
            "    }"
            "    .btn:hover {"
            "      background: #2980b9;"
            "    }"
            "    .input-values {"
            "      margin: 1rem 0;"
            "      color: #7f8c8d;"
            "    }"
            "  </style>"
            "</head>"
            "<body>"
            "  <div class='container'>"
            "    <h1>Результат расчёта</h1>"
            "    <div class='input-values'>"
            "      <p>Расстояние: <strong>%.1f км</strong></p>"
            "      <p>Скорость: <strong>%.1f км/ч</strong></p>"
            "    </div>"
            "    <div class='result-box'>"
            "      <p>Время в пути:</p>"
            "      <div class='result-value'>%d ч %d мин</div>"
            "    </div>"
            "    <a href='/' class='btn'>Новый расчёт</a>"
            "  </div>"
            "</body>"
            "</html>", dist, spd, h, m);
    } else {
                mg_http_reply(c, 400, 
                    "Content-Type: text/html; charset=utf-8\r\n",
                    "<!DOCTYPE html>"
                    "<html lang='ru'>"
                    "<head>"
                    "  <meta charset='UTF-8'>"
                    "  <title>Ошибка</title>"
                    "  <style>/* Тот же CSS что и выше */</style>"
                    "</head>"
                    "<body>"
                    "  <div class='container'>"
                    "    <h1>Ошибка</h1>"
                    "    <div class='error'>"
                    "      <p>Пожалуйста, введите корректные значения:</p>"
                    "      <ul>"
                    "        <li>Расстояние должно быть больше 0</li>"
                    "        <li>Скорость должна быть больше 0</li>"
                    "      </ul>"
                    "    </div>"
                    "    <a href='/'>Попробовать снова</a>"
                    "  </div>"
                    "</body>"
                    "</html>");
            }
            return;
        }
        
        mg_http_reply(c, 404, 
            "Content-Type: text/html; charset=utf-8\r\n",
            "<!DOCTYPE html>"
            "<html lang='ru'>"
            "<head>"
            "  <meta charset='UTF-8'>"
            "  <title>Страница не найдена</title>"
            "  <style>/* Тот же CSS что и выше */</style>"
            "</head>"
            "<body>"
            "  <div class='container'>"
            "    <h1>404 - Страница не найдена</h1>"
            "    <a href='/'>Вернуться на главную</a>"
            "  </div>"
            "</body>"
            "</html>");
    }
}

int main(void) {
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);
    mg_http_listen(&mgr, "http://localhost:8000", fn, NULL);
    
    printf("Сервер запущен на http://localhost:8000\n");
    while (true) mg_mgr_poll(&mgr, 1000);
    
    mg_mgr_free(&mgr);
    return 0;
}
