#pragma once

#include "server/parameter_server.h"
#include "server/graphs_helper.h"

#include "config.h"

#include "ServerDataLed.h"

void webSocketEvent(byte num, WStype_t ws_type, uint8_t *payload, size_t length);

class ServerLed : public ParameterServer
{
public:
  ServerDataLed data;

public:
  ServerLed() : ParameterServer() { ParameterServer::pData = &data; }
  void setup()
  {
    ParameterServer::setup(DEVICE_NAME, webSocketEvent);

    const char *name = DEVICE_NAME;
    // if (!MDNS.begin(name))
    if (!MDNS.begin("led-driver"))
      Serial.println("Error setting up mDNS responder!");
    else
      Serial.printf("mDNS responder started: http://%s.local\n", DEVICE_NAME);

    ParameterServer::begin();
  }
  void loop() { ParameterServer::loop(); }
};

ServerLed server;

// the parameters of this callback function are always the same -> num: id of the client who send the event, ws_type: ws_type of message, payload: actual data sent and length: length of payload
void webSocketEvent(byte num, WStype_t ws_type, uint8_t *payload, size_t length)
{
  // Serial.printf("webSocketEvent(%d, %d, %s, %d)\n", num, ws_type, payload, length);
  // todo maybe handle websocketevent in socketserver and create overridable callbacks for ws_type for better usability
  switch (ws_type)
  {
  case WStype_DISCONNECTED:
  {
#if DEBUG_SERVER
    Serial.println("Client Nr. " + String(num) + " disconnected");
#endif
    break;
  }
  case WStype_CONNECTED:
  {
#if DEBUG_SERVER
    Serial.println("Client Nr. " + String(num) + " connected");
#endif
    // send variables to connected web client -> as optimization: could send it just to the new client "num" in future
    server.sendAllParameters();
    break;
  }
  case WStype_TEXT:
  {
    bool changesOccured = server.data.parseAll(payload);

    if (changesOccured)
    {
      for (auto param : server.data.getParameter_changed_from_server())
      {
        if (param->name.startsWith("control_"))
        {
        }
        else
          server.sendJson(param); // "loopback"
      }
    }

    break;
  }
  }
}
