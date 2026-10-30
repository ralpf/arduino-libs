#pragma once
/***************************************************************************************************
This file is a facade to the esp http and websocket features.
It also initilizes the filesystem, since the esp can store the index.html
It's an experiment to add functionality C-style
***************************************************************************************************/

#include <Types.h>
#include "httpRequest.h"


// delegate
using HttpRequestHandler = void (*)(HttpRequest& request, const char* endpoint);
// delegate
using WsMessageHandler   = void (*)(const char* message, ui32 clientID);


// call once before _start()
void asyncBackend_init(bool initWebSocketLog = true, bool initWebSocketControl = false);

// add hanlder func for endpoints of http server
void asyncBackend_register_get_endpoint(const char* endpoint, HttpRequestHandler handlerFunc);
// implement this to add support for file uploads and custom firware updates
//void asyncBackend_register_post_endpoint(const char* endpoint, HttpRequestHandler handlerFunc);
// add hanlder func for 'esp control' websocket. Use it to push data from outside to esp and implement a control protocol
void asyncBackend_register_ws_control(WsMessageHandler handlerFunc);

// call after all endpoints are attached
void asyncBackend_start();

// call every tick
void asyncBackend_maintain();

// call to push message via the 'log' websocket
void asyncBackend_push_log(const char* message);



// note: I like the C style feel when we have exposed small funcs instead of overengineering the usecase
// C simplicity is awesome