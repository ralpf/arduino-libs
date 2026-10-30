#include "asyncBackend.h"
#include "asyncWebsocket.h"

#include <uniPrinter.h>
#include <assertCrash.h>


#include <WiFi.h>
#include <SPIFFS.h>
#include <FS.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoOTA.h>


#ifndef IP_ADDRESS_4
  #error "IP_ADDRESS_4 symbol not #defined !! It sets the last byte of 192.168.100.* "
#endif


//............................................................................................STATIC

// think how to better store credentials
static const char* ssid = "StarNet - munteanu.v84";
static const char* password = "48575443A95B41AA";

// the html is hosted on esp32 in fylesystem as index.html

// the fancy async web server
static AsyncWebServer server(80);

// some sanity check
static bool isInited;       // the system overall
static bool isStarted;      // http server running

// some fun
static ui32 count_req;

//........................................................................................LOCAL-FUNC

void init_wifi_server()
{
    IPAddress localIp(192, 168, 100, IP_ADDRESS_4);
    IPAddress routerIp(192, 168, 100, 1);
    IPAddress subnetMask(255, 255, 255, 0);
    
    WiFi.mode(WIFI_STA);
    bool ok = WiFi.config(localIp, routerIp, subnetMask);
    ASSERT(ok, "ERR: Failed to configure WiFi with static IP");

    SPrint("... Connecting to WiFi");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        SPrint(".");
        delay(300);
    }
    SPrint("OK: connected WiFi\nto: '%s'\nip: %s\n", ssid, localIp.toString().c_str());
}


void init_frontend()
{
    bool ok = SPIFFS.begin(true);
    ASSERT(ok, "ERR: SPIFFS file system mount failed");
}


void init_websockets(bool initWebSocketLog, bool initWebSocketControl)
{
    if (initWebSocketLog)
    {
        server.addHandler(&ws_log);
        ws_log.onEvent(on_websocket_log_event);
    }
    if (initWebSocketControl)
    {
        server.addHandler(&ws_ctrl);
        ws_ctrl.onEvent(on_websocket_ctrl_event);
    }
}


bool validate_endpoint(const char* endpoint)
{
    if (endpoint == nullptr) return false;
    if (endpoint[0] != '/') return false;
    return true;
}

//.........................................................................................ENDPOINTS

void endpoint_test_handler(HttpRequest& request, const char* discard)
{
    SPrint("Received [test] request");
    // not sending respoce will cause auto responce 200 Ok
}


void endpoint_root_handler(HttpRequest& request, const char* discard)
{
    // this raw send does send 200 code itself
    // also using raw will bypass custom auto-send responce in asyncBackend_register_endpoint()
    request.raw()->send(SPIFFS, "/index.html", "text/html");
}

//.........................................................................................NOT-FOUND

void register_endpoint_not_found()
{
    server.onNotFound([](AsyncWebServerRequest* req) {
        HttpRequest r(req);
        r.print(404, ++count_req);
        r.send_notFound();
    });
}

//............................................................................................HEADER

// will init the fs, prepare to start the http server, and some websockets
void asyncBackend_init(bool initWebSocketLog, bool initWebSocketControl)
{
    init_wifi_server();
    init_frontend();
    init_websockets(initWebSocketLog, initWebSocketControl);
    ArduinoOTA.begin();     // other-the-air update
    isInited = true;
    SPrint("OTA updates ready");
}


void asyncBackend_start()
{
    ASSERT(isInited, "ERR: init server before starting it");
    ASSERT(!isStarted, "ERR: server was already started");

    // endpoints available in all projects
    asyncBackend_register_get_endpoint("/", endpoint_root_handler);
    asyncBackend_register_get_endpoint("/esp/test", endpoint_test_handler);
    // invalid endpoints
    register_endpoint_not_found();

    server.begin();
    isStarted = true;
}


void asyncBackend_maintain()
{
    ArduinoOTA.handle();    // check for firmware update
    websockets_maintain();  // cleanup clients
}


void asyncBackend_push_log(const char* message)
{
    ws_log_send_message(message);
}

//...................................................................................CB REGISTRATION

void asyncBackend_register_get_endpoint(const char* endpoint, HttpRequestHandler handlerFunc)
{
    // sanity check and report
    ASSERT(isInited, "ERR: init first before attaching endpoints");
    ASSERT(validate_endpoint(endpoint), "ERR: invalid endpoint");
    ASSERT(!isStarted, "ERR: can't attach more endpoints after server was started");
    // do register function callback
    server.on(endpoint, HTTP_GET,
        [handlerFunc,endpoint](AsyncWebServerRequest* req) {     // lambda
            HttpRequest r(req);                         // wrapper class
            r.print(200, ++count_req);                  // print to serial
            handlerFunc(r,endpoint);                    // invoke hanlder with wrapper as arg. The endpoint name is for debug
            if (r.wasResponceSent() == false)           // autoresponce on forget to respond
                r.send_ok("Ok");
        }
    );
}


void asyncBackend_register_ws_control(WsMessageHandler handlerFunc)
{
    ws_ctrl_register_handler(handlerFunc);
}

