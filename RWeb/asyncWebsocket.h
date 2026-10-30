# pragma once
#include <Types.h>
#include <ESPAsyncWebServer.h>
#include "asyncBackend.h"   // for delegate WsMessageHandler



void websockets_maintain();
void ws_log_send_message(const char* message);

void ws_ctrl_register_handler(WsMessageHandler handlerFunc);   // invoked when data is pushed via control websocket to esp
// there is no handler for log messages, since we don't expect to receive data via log websocket

void on_websocket_log_event(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t data_len);
void on_websocket_ctrl_event(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t data_len);


extern AsyncWebSocket ws_log;
extern AsyncWebSocket ws_ctrl;