/////////////////////////////////////////////////////////////////

#pragma once
#ifndef ESPTelnetBase_h
#define ESPTelnetBase_h

/////////////////////////////////////////////////////////////////

#define ASCII_BELL 7
#define ASCII_BACKSPACE 8
#define ASCII_LF 10
#define ASCII_CR 13
#define KEEP_ALIVE_INTERVAL_MS 1000

/////////////////////////////////////////////////////////////////

#include <Arduino.h>
#if defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#elif defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#endif

/////////////////////////////////////////////////////////////////

#include "DebugMacros.h"

/////////////////////////////////////////////////////////////////

using TCPClient = WiFiClient;
using TCPServer = WiFiServer;

/////////////////////////////////////////////////////////////////
class ESPTelnetBase {
  typedef void (*CallbackFunction)(String str);

 public:
  ESPTelnetBase();

  bool begin(uint16_t port = 23, bool checkConnection = true);
  void stop();
  void loop();

  bool isClientConnected(TCPClient &client);
  void disconnectClient(bool triggerEvent = true);

  void setKeepAliveInterval(int interval);
  int getKeepAliveInterval();
  bool doKeepAliveCheckNow();
  
  String getIP() const;
  String getLastAttemptIP() const;

  void onConnect(CallbackFunction f);
  void onConnectionAttempt(CallbackFunction f);
  void onReconnect(CallbackFunction f);
  void onDisconnect(CallbackFunction f);
  void onInputReceived(CallbackFunction f);

 protected:
  // must be initalized here
  TCPServer server = TCPServer(23);
  TCPClient client;
  boolean isConnected = false;  // needed because I cannot do "client = NULL"
  String ip = "";
  String attemptIp;
  String input = "";

  uint16_t server_port = 23;
  int keep_alive_interval = KEEP_ALIVE_INTERVAL_MS;
  long last_status_check;

  CallbackFunction on_connect = NULL;
  CallbackFunction on_reconnect = NULL;
  CallbackFunction on_disconnect = NULL;
  CallbackFunction on_connection_attempt = NULL;
  CallbackFunction on_input = NULL;

  void emptyClientStream();
  bool _isIPSet(IPAddress ip);
  virtual void handleInput() = 0;

 private:
  void connectClient(bool triggerEvent = true);
};

/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
