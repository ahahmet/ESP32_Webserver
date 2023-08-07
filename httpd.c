#include <stdint.h>
#include "esp_http_server.h"
#include "debug.h"
#include "httpd.h"

#define MIN(a, b)         (((a) < (b)) ? (a) : (b))

esp_err_t server_handler(httpd_req_t *req);


httpd_handle_t server = NULL;

httpd_uri_t uri_get = {
    .uri      = "/",
    .method   = HTTP_GET,
    .handler  = server_handler,
    .user_ctx = NULL
};

httpd_uri_t uri_post = {
    .uri      = "/",
    .method   = HTTP_POST,
    .handler  = server_handler,
    .user_ctx = NULL
};

typedef struct{
    int  num;
    bool out;
}gpio_t;

gpio_t gpio[IO_SIZE];

esp_err_t send_backgroundColor(httpd_req_t *req, uint8_t r, uint8_t g, uint8_t b)
{
    char data[128]; memset(data, 0, sizeof(data));

    sprintf(data, "style=\"background-color:rgb(%d, %d, %d);\">", r, g, b);

    return httpd_send(req, data, strlen(data));
}

void print_page(httpd_req_t *req)
{
    char *header = "<head>\n"
                "<meta charset=\"UTF-8\"> \n"
                " <title> Miltera Test server Version 1.0 </title>\n"
                "</meta></head>";

    char* svg = "<center>"
                "<svg width=\"400\" height=\"120\"><defs>"
                "<linearGradient id=\"grad1\" x1=\"0%\" y1=\"10%\" x2=\"100%\" y2=\"90%\">"
                "<stop offset=\"0%\" \"style=\"stop-color:rgb(199, 179, 179);stop-opacity:1\"/>"
                "<stop offset=\"100%\" style=\"stop-color:rgb(72, 50, 50);stop-opacity:1\"/>"
                "</linearGradient> </defs>"
                "<rect width=\"400\" height=\"120\" fill=\"url(#grad1)\" />"
                "<text fill=\"#ffffff\" font-size=\"45\" font-family=\"Verdana\""
                "x=\"90\" y=\"70\" style=\"fill:rgb(255,255,255);\">IO PAGE</text>"
                "Sorry, your browser does not support inline SVG.</svg>"
                "</center><br>";

    char* css = ".switch { position:relative; display:inline-block; width:60px; height:34px;}"
                ".switch input { opacity:0; width:0; height:0;}"
                ".slider{position:absolute; cursor:pointer; top: 0;left: 0;"
                "right:0; bottom:0; background-color: #ccc;}"
                ".slider:before { position: absolute; content: \"\";"
                "height: 26px; width: 26px; left: 4px; bottom: 4px;"
                "background-color: white; -webkit-transition: .5s; transition: .5s;}"
                "input:checked + .slider { background-color: #f32121;}"
                "input:focus + .slider{ box-shadow: 0 0 1px #f32121;}"
                "input:checked + .slider:before{ -webkit-transform: translateX(26px);"
                "-ms-transform: translateX(26px); transform: translateX(26px);}"
                "table{border: 3px solid #CF000F; width:100%;"
                "height: 50%; text-align:center; font-family:\"Times New Roman\","
                "cursive;background-color: #413f3f;color:#ff0000;}"
                "table td{ border: 6px solid #93a4a4;}"
                ".save { background-color: rgb(72, 50, 50); border: none;"
                "color: white; padding: 15px 32px; text-align: center;"
                "text-decoration: none; display: inline-block;"
                "font-size: 16px; margin: 4px 2px; cursor: pointer;}";


    httpd_send(req, START_HTML, strlen(START_HTML));
   
    httpd_send(req, header, strlen(header));

    httpd_send(req, START_STYLE, strlen(START_STYLE));

    httpd_send(req, css, strlen(css));

    httpd_send(req, END_STYLE, strlen(END_STYLE));
    
    httpd_send(req, START_BODY, strlen(START_BODY));

    send_backgroundColor(req, 210, 222, 220);

    httpd_send(req, svg, strlen(svg));
    
    httpd_send(req, START_FORM, strlen(START_FORM));     
    httpd_send(req, START_TABLE, strlen(START_TABLE));

    httpd_send(req, START_ROW, strlen(START_ROW));

    httpd_send(req, START_COLUMN, strlen(START_COLUMN));
    httpd_send(req, "Index", strlen("Index"));
    httpd_send(req, END_COLUMN, strlen(END_COLUMN));
    httpd_send(req, START_COLUMN, strlen(START_COLUMN));
    httpd_send(req, "Status", strlen("Status"));
    httpd_send(req, END_COLUMN, strlen(END_COLUMN));

    httpd_send(req, END_ROW, strlen(END_ROW));

    char io[16]; memset(io, 0, sizeof(io));
    char checkbox[64]; memset(checkbox, 0, sizeof(checkbox));
    for(int i=0; i<IO_SIZE; ++i)
    {
        httpd_send(req, START_ROW, strlen(START_ROW));

        httpd_send(req, START_COLUMN, strlen(START_COLUMN));
        sprintf(io, "IO%d", i);
        httpd_send(req, io, strlen(io));
        httpd_send(req, END_COLUMN, strlen(END_COLUMN));

        httpd_send(req, START_COLUMN, strlen(START_COLUMN));
        httpd_send(req, "<label class=\"switch\">", strlen("<label class=\"switch\">"));
        if(gpio[i].out != false){
            sprintf(checkbox, "<input type=\"checkbox\" name=\"io%02d\" checked/>", i+1);
            httpd_send(req, checkbox, strlen(checkbox));
        }else{
            sprintf(checkbox, "<input type=\"checkbox\" name=\"io%02d\"/>", i+1);
            httpd_send(req, checkbox, strlen(checkbox));
        }

        httpd_send(req, "<span class=\"slider\"></span></label>", strlen("<span class=\"slider\"></span></label>"));
        httpd_send(req, END_COLUMN, strlen(END_COLUMN));

        httpd_send(req, END_ROW, strlen(END_ROW));
    }

    httpd_send(req, END_TABLE, strlen(END_TABLE));

    httpd_send(req, BREAK_LINE, strlen(BREAK_LINE));
    httpd_send(req, CENTER_START, strlen(CENTER_START));
    httpd_send(req, SUBMIT, strlen(SUBMIT));
    httpd_send(req, CENTER_END, strlen(CENTER_END));
    httpd_send(req, END_FORM, strlen(END_FORM));

    httpd_send(req, END_BODY, strlen(END_BODY));
    httpd_send(req, END_HTML, strlen(END_HTML));
}


esp_err_t server_handler(httpd_req_t *req)
{
    DEBUGI("Code in get request");
    DEBUGI("content len = %d", req->content_len);

    char content[req->content_len]; //memset(content, 0, sizeof(content));
    if(req->content_len < 0){
        return ESP_FAIL;
    }

    if(req->method == HTTP_GET){
        print_page(req);
        return ESP_OK;
    }

    int ret = httpd_req_recv(req, content, sizeof(content));
    DEBUGI("content len = %d******** %s", ret, content);
    if(ret <= 0){
        DEBUGI("ret len = %d", ret);
        if(ret == HTTPD_SOCK_ERR_TIMEOUT){
            DEBUGI("Burda");
            httpd_resp_send_408(req);
        }
        goto end;
    }

    char buff[32];
    char io[16]; memset(io, 0, sizeof(io));
    for(int i=0; i<IO_SIZE; ++i)
    {
        sprintf(io, "io%02d", i+1);
        if(httpd_query_key_value(content, io, buff, sizeof(buff)) == ESP_OK){
            gpio[i].out = true;
            //TODO set gpio
        }else{
            gpio[i].out = false;
            //TODO reset gpio
        }
    }


    print_page(req);
end:
    return ESP_OK;
}

int8_t start_webserver(int* io)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    if (httpd_start(&server, &config) != ESP_OK) {
        //error msg
        DEBUGE("HTTP start err");
        return -1;
    }

    for(int i=0; i<IO_SIZE; ++i)
    {
        gpio[i].num = io[i];
        gpio[i].out = false;
    } 


    ERROR_CHECK_SOFT(httpd_register_uri_handler(server, &uri_get),
                                "http server get register error");

    ERROR_CHECK_SOFT(httpd_register_uri_handler(server, &uri_post),
                                "http server post register error");

    DEBUGI("HTTPD success");
    return 0;
}


void stop_webserver(void)
{
    if (server != NULL) {
        httpd_stop(server);
    }
}