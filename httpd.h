#ifndef __httpd__
#define __httpd__

#define START_HTML    "<html>"
#define END_HTML      "</html>"
#define START_STYLE   "<style>"
#define END_STYLE     "</style>"
#define START_BODY    "<body"
#define END_BODY      "</body>"
#define START_TABLE   "<table>"
#define END_TABLE     "</table>"
#define START_ROW     "<tr>"
#define END_ROW       "</tr>"
#define START_COLUMN  "<td>"
#define END_COLUMN    "</td>"
#define SUBMIT        "<input type=\"submit\" class=\"save\" >" //value=\"saved\"
#define START_FORM    "<form method=\"post\">"
#define END_FORM      "<\form>"
#define BREAK_LINE    "<br>"
#define CENTER_START  "<center>"
#define CENTER_END    "</center>"


#define IO_SIZE 3

#define IO_1    1
#define IO_2    2
#define IO_3    3





int8_t start_webserver(int* io);

void stop_webserver(void);


#endif