#include "asyncWebsocket.h"
#include "uniPrinter.h"


//...............................................................................THE ONLY WEBSOCKETS

AsyncWebSocket ws_log("/ws/log");
AsyncWebSocket ws_ctrl("/ws/ctrl");

//...........................................................................................DEFINES

#define WS_MAX_CLIENTS 4        // max connected clients via 'control' ws
#define WS_BUFF_SZ 256          // accumulator per client

//..............................................................................................TYPE

struct WSChunk
{
    const ui8* pData;
    ui32       length;
    bool       isText;
    bool       isFirst;
    bool       isLast;
};

//..............................................................................................TYPE

struct ClientAccum      // accumulator
{
    char* pData;
    ui32  clientID;
    ui16  length;
    bool  inUse;
};

//...........................................................................................GLOBALS

static WsMessageHandler ws_ctrl_handler_func;
static ClientAccum clientAccums[WS_MAX_CLIENTS];

//...........................................................................................HELPERS

ClientAccum* find_free_client_accum()
{
    for (ClientAccum& accum : clientAccums)
    {
        if (accum.inUse == false)
        {
            if (!accum.pData) accum.pData = (char*)malloc(WS_BUFF_SZ + 1);    // not expecting to run dry of 256bytes of mem
            if (!accum.pData) { SPrint("[WS][ctrl] malloc() error !!"); return nullptr; }
            return &accum;
        }
    }
    SPrint("[WS][ctrl] Maximum client limit reached (%u). Rejecting new connection...", WS_MAX_CLIENTS);
    return nullptr;
}

ClientAccum* find_active_client_accum(ui32 clientID)
{
    for (ClientAccum& accum : clientAccums)
        if (accum.inUse && accum.clientID == clientID) return &accum;

    SPrint("[WS][ctrl] unexptected: no active client (%u) exist !!", clientID);
    return nullptr;
}

//..............................................................................................IMPL

void websockets_maintain()
{
    ws_log.cleanupClients();
    ws_ctrl.cleanupClients();
}


void ws_log_send_message(const char* message)
{
    ws_log.textAll(message, strlen(message)); // sending len will force websoket to copy the buffer
}


void ws_ctrl_register_handler(WsMessageHandler handlerFunc)
{
    ws_ctrl_handler_func = handlerFunc; // we allow for nullptr - this will stop invoking the handler
}


void on_websocket_log_event(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t data_len)
{
    switch (type)
    {
        case WS_EVT_CONNECT:
        SPrint("[WS][log] new client connected (id=%u | total=%u)", client->id(), server->count());
        break;  //----------------------------------------------------------------------------------

        case WS_EVT_DISCONNECT:
        SPrint("[WS][log] client disconnected (id=%u | total=%u)", client->id(), server->count());
        break;  //----------------------------------------------------------------------------------

        // other available events, just for info. Log websocket does not receive data from outside
        case WS_EVT_DATA:
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
        default:
        break;  //----------------------------------------------------------------------------------
    }
}


void on_websocket_ctrl_event(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t data_len)
{
    ClientAccum* accum = nullptr;
    AwsFrameInfo* frameInfo = nullptr;
    bool isFrameCompleted = false;

    switch (type)
    {
        case WS_EVT_CONNECT:
        accum = find_free_client_accum();
        if (!accum) { SPrint("[WS][ctrl] connection of client (%u) failed ...", client->id()); client->close(); return; }
        accum->clientID = client->id();
        accum->length = 0;
        accum->inUse = true;
        SPrint("[WS][ctrl] new client connected (id=%u | total=%u)", client->id(), server->count());
        break;  //----------------------------------------------------------------------------------

        case WS_EVT_DISCONNECT:
        accum = find_active_client_accum(client->id());
        if (!accum) { SPrint("[WS][ctrl] disconnect of client (%u) failed ...", client->id()); return; }
        accum->inUse = false;
        SPrint("[WS][ctrl] client disconnected (id=%u | total=%u)", client->id(), server->count());
        break;  //----------------------------------------------------------------------------------

        case WS_EVT_DATA:
        accum = find_active_client_accum(client->id());
        if (!accum) { SPrint("[WS][ctrl] can't process data, client (%u) not marked active ...", client->id()); return; }
        if (data_len > WS_BUFF_SZ - accum->length)
        {
            client->close(1009); // message too big
            accum->inUse = false;   // will ignore remaining chunks of the frame that arrives late
            SPrint("[WS][ctrl] message from client (%u) exceeds buffer capacity, force close client ...", client->id());
            return;
        }
        // copy data
        memcpy(accum->pData + accum->length, data, data_len);
        accum->length += data_len;
        accum->pData[accum->length] = '\0';
        // check if messages ended
        frameInfo = (AwsFrameInfo*)arg;
        isFrameCompleted = frameInfo->index + data_len == frameInfo->len;
        if (isFrameCompleted && frameInfo->final)
        {
            // invoke subscriber
            if (ws_ctrl_handler_func) ws_ctrl_handler_func(accum->pData, client->id());
            accum->length = 0;  // reset for the next message
        }
        break;  //----------------------------------------------------------------------------------

        case WS_EVT_PONG:
        case WS_EVT_ERROR:
        default:
        break;  //----------------------------------------------------------------------------------
    }
}
