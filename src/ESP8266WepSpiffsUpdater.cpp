// 
// 
// 
#include <WiFiUdp.h>
#include "StreamString.h"
#include "ESP8266WepSpiffsUpdater.h"
#include "Log.h"


static const char successResponse[] PROGMEM =
"<META http-equiv=\"refresh\" content=\"15;URL=/\">Update Success! Rebooting...\n";

static const char successSpiResponse[] PROGMEM =
"<META http-equiv=\"refresh\" content=\"15;URL=/\">Update SPIFFS Success! Rebooting...\n";

ESP8266WebSpiffsUpdater::ESP8266WebSpiffsUpdater()
{
	_server = NULL;
	_username = NULL;
	_password = NULL;
	_authenticated = false;
}

void ESP8266WebSpiffsUpdater::setup(ESP8266WebServer *server,  const char * username, const char * password)
{
	_server = server;
	_username = (char *)username;
	_password = (char *)password;
	
		
	// handler for the /update form POST (once file upload finishes)
	_server->on("/update", HTTP_POST, [&]() {
		if (!_authenticated)
			return _server->requestAuthentication();
		if (Update.hasError()) {
			_server->send(200, F("text/html"), String(F("Update error: ")) + _updaterError);
		}
		else {
			_server->client().setNoDelay(true);
			_server->send_P(200, PSTR("text/html"), successResponse);
			delay(100);
			_server->client().stop();
			ESP.restart();
		}
	}, [&]() {
		// handler for the file upload, get's the sketch bytes, and writes
		// them through the Update object
		HTTPUpload& upload = _server->upload();

		if (upload.status == UPLOAD_FILE_START) {
			_updaterError = String();
			
			_authenticated = (_username == NULL || _password == NULL || _server->authenticate(_username, _password));
			if (!_authenticated) {
				logg.logging("Unauthenticated Update");
				return;
			}

			WiFiUDP::stopAll();
			
			logg.logging("Update: " + upload.filename);
			uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
				
			if (!Update.begin(maxSketchSpace,U_FLASH)) {//start with max available size
				_setUpdaterError();
			}
		}
		else if (_authenticated && upload.status == UPLOAD_FILE_WRITE && !_updaterError.length()) {
			
			if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
				_setUpdaterError();
			}
		}
		else if (_authenticated && upload.status == UPLOAD_FILE_END && !_updaterError.length()) {
			if (Update.end(true)) { //true to set the size to the current progress
				logg.logging("Update Success: "+String(upload.totalSize)+" Rebooting...");
			}
			else {
				_setUpdaterError();
			}
		}
		else if (_authenticated && upload.status == UPLOAD_FILE_ABORTED) {
			Update.end();
			logg.logging("Update was aborted");
		}
		delay(0);
	});


	_server->on("/spiffs", HTTP_POST, [&]() {
		if (!_authenticated)
			return _server->requestAuthentication();
		if (Update.hasError()) {
			_server->send(200, F("text/html"), String(F("Update error: ")) + _updaterError);
		}
		else {
			_server->client().setNoDelay(true);
			_server->send_P(200, PSTR("text/html"), successSpiResponse);
			
			delay(100);
			_server->client().stop();
			ESP.restart();
		}
	}, [&]() {
		// handler for the file upload, get's the sketch bytes, and writes
		// them through the Update object
		HTTPUpload& upload = _server->upload();

		if (upload.status == UPLOAD_FILE_START) {
			_updaterError = String();

			_authenticated = (_username == NULL || _password == NULL || _server->authenticate(_username, _password));
			if (!_authenticated) {
				logg.logging("Unauthenticated Update");
				return;
			}

			WiFiUDP::stopAll();

			logg.logging("Update: " + upload.filename);
			uint32_t maxSketchSpace = ESP.getFlashChipRealSize() / 4;


			if (!Update.begin(maxSketchSpace, U_FS)) {//start with max available size
				logg.logging("Update low space");
				_setUpdaterError();
			}
		}
		else if (_authenticated && upload.status == UPLOAD_FILE_WRITE && !_updaterError.length()) {

			if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
				logg.logging("Update diff size");
				_setUpdaterError();
			}
			logg.logging("current length="+String(upload.currentSize));
		}
		else if (_authenticated && upload.status == UPLOAD_FILE_END && !_updaterError.length()) {
			if (Update.end(true)) { //true to set the size to the current progress
				logg.logging("Update Success: " + String(upload.totalSize) + " Rebooting...");
			}
			else {
				_setUpdaterError();
			}
		}
		else if (_authenticated && upload.status == UPLOAD_FILE_ABORTED) {
			Update.end();
			logg.logging("Update was aborted");
		}
		delay(0);
	});

}

void ESP8266WebSpiffsUpdater::_setUpdaterError()
{
	StreamString str;
	Update.printError(str);
	logg.logging(str.c_str());
	_updaterError = str;
}
