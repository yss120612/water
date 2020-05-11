// ESP8266WepSpiffsUpdater.h

#ifndef _ESP8266WEPSPIFFSUPDATER_h
#define _ESP8266WEPSPIFFSUPDATER_h

#include "arduino.h"

#include <ESP8266WebServer.h>

class ESP8266WebSpiffsUpdater
{
public:
	ESP8266WebSpiffsUpdater();

	void setup(ESP8266WebServer *server)
	{
		setup(server, NULL, NULL);
		//setup(server, "/update", NULL, NULL);
	}
		

	void setup(ESP8266WebServer *server,  const char * username, const char * password);

protected:
	void _setUpdaterError();

private:
	
	ESP8266WebServer *_server;
	char * _username;
	char * _password;
	bool _authenticated;
	String _updaterError;
};

#endif

