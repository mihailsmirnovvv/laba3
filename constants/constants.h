#ifndef CONSTANTS_H
#define CONSTANTS_H

const char *login_page = 
"<!DOCTYPE html>"
"<html>"
"<head>"
"    <title>Time Calculator</title>"
"    <link rel=\"stylesheet\" href=\"/css/styles.css\">"
"</head>"
"<body>"
"    <div class=\"container\">"
"        <h1>Travel Time Calculator</h1>"
"        <form action=\"/calculate\" method=\"POST\">"
"            <div class=\"form-group\">"
"                <label for=\"distance\">Distance (km):</label>"
"                <input type=\"number\" id=\"distance\" name=\"distance\" step=\"0.01\" min=\"0.01\" required>"
"            </div>"
"            <div class=\"form-group\">"
"                <label for=\"speed\">Average Speed (km/h):</label>"
"                <input type=\"number\" id=\"speed\" name=\"speed\" step=\"0.1\" min=\"1\" required>"
"            </div>"
"            <button type=\"submit\">Calculate</button>"
"        </form>"
"    </div>"
"</body>"
"</html>";

const char *success_page = 
"<!DOCTYPE html>"
"<html>"
"<head>"
"    <title>Result</title>"
"    <link rel=\"stylesheet\" href=\"/css/styles.css\">"
"</head>"
"<body>"
"    <div class=\"container\">"
"        <h1>Travel Time</h1>"
"        <div class=\"result\">"
"            <p>Estimated travel time: <strong>%d hours and %d minutes</strong></p>"
"            <a href=\"/\">Calculate again</a>"
"        </div>"
"    </div>"
"</body>"
"</html>";

const char *error_page = 
"<!DOCTYPE html>"
"<html>"
"<head>"
"    <title>Error</title>"
"    <link rel=\"stylesheet\" href=\"/css/styles.css\">"
"</head>"
"<body>"
"    <div class=\"container\">"
"        <h1>Error</h1>"
"        <div class=\"error\">"
"            <p>Invalid input! Distance and speed must be positive numbers.</p>"
"            <a href=\"/\">Try again</a>"
"        </div>"
"    </div>"
"</body>"
"</html>";

#endif
