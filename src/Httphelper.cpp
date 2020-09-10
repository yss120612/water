#include "Httphelper.h"
//#include <LittleFS.h>
#include <FS.h>
#include <ArduinoJson.h>
#include  "Log.h"
#include  "Config.h"


HttpHelper::HttpHelper()
{
	server = new ESP8266WebServer(80);
	if (!SPIFFS.begin()){
		logg.logging("FS Error");
	}
	//LittleFS.format();
	//SPIFFS.begin();
}

HttpHelper::~HttpHelper()
{
	//delete httpUpdater;
	delete httpSpiffsUpdater;
	delete server;
	SPIFFS.end();
	// SPIFFS.end();	
}

void HttpHelper::setup() {
	if (server == NULL) return;

	WiFiconnect();
			
	server->on("/", std::bind(&HttpHelper::handleRoot, this));
		
	server->on("/logdata", std::bind(&HttpHelper::handleLog, this));

	//server->on("/distilldata", std::bind(&HttpHelper::handleDistill, this));
	//server->on("/distillset", std::bind(&HttpHelper::handleDistillSet, this)); // Установка distill

	//server->on("/rectifydata", std::bind(&HttpHelper::handleRectify, this));
	//server->on("/rectifyset", std::bind(&HttpHelper::handleRectifySet, this)); // Установка rectify

	//server->on("/suviddata", std::bind(&HttpHelper::handleSuvid, this));
	//server->on("/suvidset", std::bind(&HttpHelper::handleSuvidSet, this)); // Установка suvid

	//server->on("/brewingdata", std::bind(&HttpHelper::handleBrewing, this));
	//server->on("/brewingset", std::bind(&HttpHelper::handleBrewingSet, this));

	//server->on("/restart", web_handlers::restart);
		
	server->on("/upd", std::bind(&HttpHelper::handleUpdate, this));

	//server->serveStatic("/heater",SPIFFS,"/heater.htm", NULL);

	server->serveStatic("/log", SPIFFS, "/log.htm", NULL);

	//server->serveStatic("/distill", SPIFFS, "/distillation.htm", NULL);

	//server->serveStatic("/rectify", SPIFFS, "/rectify.htm", NULL);

	//server->serveStatic("/suvid", SPIFFS, "/suvid.htm", NULL);

	//server->serveStatic("/brewing", SPIFFS, "/brewing.htm", NULL);

	server->serveStatic("/css/bootstrap.min.css", SPIFFS, "/css/bootstrap.min.css", NULL);

	server->serveStatic("/js/bootstrap.min.js", SPIFFS, "/js/bootstrap.min.js", NULL);

	server->serveStatic("/js/jquery.min.js", SPIFFS, "/js/jquery.min.js", NULL);

	server->serveStatic("/js/export-data.js", SPIFFS, "/js/export-data.js", NULL);

	server->serveStatic("/js/exporting.js", SPIFFS, "/js/exporting.js", NULL);

	server->serveStatic("/js/highstock.js", SPIFFS, "/js/highstock.js", NULL);
			
	server->begin();

	httpSpiffsUpdater = new ESP8266WebSpiffsUpdater();
	httpSpiffsUpdater->setup(server);
}

boolean HttpHelper::isConnected()
{
	return WiFi.status() == WL_CONNECTED;
}

void HttpHelper::handleRoot() {
	
	if (!server->authenticate(webuser, webpass))
		return server->requestAuthentication();
	if (!handleFileRead("/")) {
		server->send(200, "text/plain", "NOT FOUND!!!");
	}
}

void HttpHelper::handleLog()
{
	String str = "{\"logdata\":\"<ul>" + logg.getAll2Web() + "</ul>\"}";
	server->send(200, "text/json",str); // Oтправляем ответ No Reset
}

// void HttpHelper::handleDistill()
// {
// 	String str =   "{\"tsa_data\":\"" + ds->getData(DS_TTSA) 
// 			  + "\", \"def_data\":\"" + ds->getData(DS_TTSARGA) 
// 			  + "\", \"kube_data\":\"" + ds->getData(DS_TKUBE) 
// 			  + "\", \"cooler_data\":\"" + ds->getData(DS_TTRIAK) 
// 			  + "\", \"heater_data\":\"" + ds->getData(DS_HPOWER) 
// 			  + "\", \"kran_data\":\"" + ds->getData(DS_KRANSTATE) 
// 			  + "\", \"tstop_data\":\"" + ds->getData(DS_DISTSTOP)
// 			  + "\", \"wsu_data\":\"" + ds->getData(DS_WSUROVEN)
// 			  + "\", \"wsf_data\":\"" + ds->getData(DS_WSFLOOD)
// 		      + "\", \"state_data\":\"" + ds->getData(DS_DISTSTATE) + "\" }";
// 	server->send(200, "text/json", str); // Oтправляем ответ No Reset
// }

// void HttpHelper::handleDistillSet()
// {
// 	ds->setData(SET_DISTILLTSTOP, server->arg("TSTOP"));
// 	logg.logging("Temperature (cube) end of distill set on " + server->arg("TSTOP"));
// 	//server->send(200, "text/plain", "OK");
// }

// void HttpHelper::handleRectify()
// {
	
// 	String str =   "{\"tsa_data\":\"" + ds->getData(DS_TTSA) 
// 			  + "\", \"heater_data\":\"" + ds->getData(DS_HPOWER) 
// 			  + "\", \"def_data\":\"" + ds->getData(DS_TTSARGA) 
// 		      + "\", \"kube_data\":\"" + ds->getData(DS_TKUBE) 
// 		      + "\", \"cooler_data\":\"" + ds->getData(DS_TTRIAK) 
// 		      + "\", \"tstop_data\":\"" + ds->getData(DS_RECTSTOP) 
// 		      + "\", \"kran_data\":\"" + ds->getData(DS_KRANSTATE) 
// 			  + "\", \"wsu_data\":\"" + ds->getData(DS_WSUROVEN)
// 			  + "\", \"wsf_data\":\"" + ds->getData(DS_WSFLOOD)
// 		      + "\", \"state_data\":\"" + ds->getData(DS_RECTSTATE) + "\" }";
// 	server->send(200, "text/json", str); // Oтправляем ответ No Reset
// }

// void HttpHelper::handleRectifySet()
// {
// 	ds->setData(SET_RECTTSTOP, server->arg("TSTOPSET"));
// 	logg.logging("Temperature (def) end of rectify sett on " + server->arg("TSTOPSET"));
// 	//server->send(200, "text/plain", "OK");
// }

//void HttpHelper::handleSuvid(){
//	String str = "{\"kube_data\":" + ds->getData(DS_TKUBE) 
//		      + ", \"cooler_data\":" + ds->getData(DS_TTRIAK) 
//		      + ", \"heater_data\":" + ds->getData(DS_HPOWER) 
//		      + ", \"ttarget_data\":\"" + ds->getData(DS_SUVIDTARGET)
//			  + "\", \"timetarget_data\":\"" + ds->getData(DS_SUVIDTIME)
//		      + "\", \"state_data\":\"" + ds->getData(DS_SUVIDSTATE) 
//		      + "\", \"time_data\":\"" + ds->getData(DS_SUVIDTIMELEFT) + "\" }";
//	server->send(200, "text/json", str); // Oтправляем ответ No Reset
//}
//
//void HttpHelper::handleSuvidSet()
//{
//	ds->setData(SET_SUVIDTEMP, server->arg("TMP"));
//	logg.logging("SUVID Temperature set on " + server->arg("TMP"));
//	ds->setData(SET_SUVIDTIME, server->arg("TIME"));
//	logg.logging("SUVID Time set on " + server->arg("TIME"));
//}

// void HttpHelper::handleBrewing() {
// 	String str = "{\"kube_data\":\"" + ds->getData(DS_TKUBE)
// 			+ "\", \"cooler_data\":\"" + ds->getData(DS_TTRIAK)
// 			+ "\", \"heater_data\":\"" + ds->getData(DS_HPOWER)
// 			+ "\", \"ttarget1_data\":\"" + ds->getData(DS_BREWINGTMP1)
// 			+ "\", \"timetarget1_data\":\"" + ds->getData(DS_BREWINGTIME1)
// 			+ "\", \"ttarget2_data\":\"" + ds->getData(DS_BREWINGTMP2)
// 			+ "\", \"timetarget2_data\":\"" + ds->getData(DS_BREWINGTIME2)
// 			+ "\", \"ttarget3_data\":\"" + ds->getData(DS_BREWINGTMP3)
// 			+ "\", \"timetarget3_data\":\"" + ds->getData(DS_BREWINGTIME3)
// 			+ "\", \"ttarget4_data\":\"" + ds->getData(DS_BREWINGTMP4)
// 			+ "\", \"timetarget4_data\":\"" + ds->getData(DS_BREWINGTIME4)
// 			+ "\", \"ttarget_data\":\"" + ds->getData(DS_BREWINGTARGET)
// 			+ "\", \"chiller_data\":\"" + ds->getData(DS_BREWINGCHILLER)
// 			+ "\", \"kran_data\":\"" + ds->getData(DS_KRANSTATE)
// 			+ "\", \"phase_data\":\"" + ds->getData(DS_BREWINGPHASE)
// 			+ "\", \"state_data\":\"" + ds->getData(DS_BREWINGSTATE)
// 			+ "\", \"time_data\":\"" + ds->getData(DS_BREWINGTIMELEFT) + "\" }";
// 	server->send(200, "text/json", str); // Oтправляем ответ No Reset
// }

// void HttpHelper::handleBrewingSet()
// {
// 	uint8_t part = (server->arg("PART")).toInt();
// 	switch (part) {
// 	case 1:
// 	case 2:
// 	case 3:
// 	case 4:
// 		ds->setData(SET_BREWINGTMP1+(part-1)*2, server->arg("TMP"));
// 		logg.logging("BREWING Temperature " + String(part) +" set on " + server->arg("TMP"));
// 		ds->setData(SET_BREWINGTIME1 + (part - 1) * 2, server->arg("TIME"));
// 		logg.logging("BREWING Time " + String(part) + " set on " + server->arg("TIME"));
// 		break;
// 	case 5:
// 		ds->setData(SET_BREWINGCHILLER, server->arg("CHILLER"));
// 		break;

// 	}
// }

void HttpHelper::WiFiconnect()
{
	WiFi.disconnect();
	//IPAddress apIP(192, 168, 0, 100);
	WiFi.mode(WIFI_STA);
	//WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
	//WiFi.softAP(conf->getWiFiN().c_str(), conf->getWiFiP().c_str());
	uint8_t modeWiFi = 0;
	int n_network = WiFi.scanNetworks(); // запрос количества доступных сетей
	for (int i = 0; i < n_network; ++i) {
		logg.logging("network " + WiFi.SSID(i) + " present");
		if (WiFi.SSID(i).equals(wifiuser))
		{
			modeWiFi = 1; // наша сеть присутствует
			break;
		}

		if (WiFi.SSID(i).equals(wifiuser1))
		{
			modeWiFi = 2; // наша сеть присутствует
			break;
		}


	}

	if (modeWiFi >0 ) {
		// пробуем подключиться

		logg.logging("Connecting to " + String(modeWiFi==1?wifiuser:wifiuser1));

		WiFi.disconnect(true);
		WiFi.begin(modeWiFi==1?wifiuser:wifiuser1,modeWiFi==1?wifipass:wifipass1);
		// ждем N кол-во попыток, если нет, то AP Mode
		byte tmp_while = 0;
		while (WiFi.waitForConnectResult() != WL_CONNECTED) {
			delay(1000);

			logg.logging("Connecting to " + String(modeWiFi==1?wifiuser:wifiuser1));

			if (tmp_while < 5) tmp_while++;
			else {
				modeWiFi = 0;
				logg.logging("Connection to " + String(modeWiFi==1?wifiuser:wifiuser1) + " failed!");
				break;
			}
		}

		

		if (WiFi.isConnected()){
			logg.logging("Connected! My IP is "+WiFi.localIP().toString()+" My Gateway ip IP is "+WiFi.gatewayIP().toString());
		}
	}
}

void HttpHelper::WiFiReconnect()
{
	if (isConnected()) {
		return;
	}

	WiFi.reconnect();
	logg.logging("Reconnect...");

	// При потери связи с базовой станцией переходим в режим точки доступа и пробуем переподключиться
	/*if (conf->getWiFiN().length() && tCnt >= setRestartWiFi && !WiFi.softAPgetStationNum()) {
	WiFi.reconnect();
	Serial.println("reconnect");
	}*/
}

void HttpHelper::handleUpdate() {
	if (!server->authenticate(webuser, webpass))
		return server->requestAuthentication();
	String resp = "<!DOCTYPE html>\n<html>\n<head>\n";
	resp += "<meta charset = \"utf-8\">\n";
	resp += "<title>YssSM прошивка</title>\n";
	resp += "<meta name = \"description\" content = \"Версия 0.1\">\n";
	resp += "<meta name = \"author\" content = \"Yss\">\n";
	resp += "<link href = \"/css/bootstrap.min.css\" type = \"text/css\" rel = \"stylesheet\">\n";
	resp += "<script type = \"text/javascript\" src = \"/js/jquery.min.js\"></script>\n";
	resp += "<script type = \"text/javascript\" src = \"/js/bootstrap.min.js\"></script>\n";
	resp += "</head>\n<body>\n";
	resp += "<div class = \"col-md-12\">\n";
	resp += "<a href = \"/\"class = \"btn btn-info\">Дом</a>\n";
	resp += "</div>\n";
	resp += "<div class = \"alert alert-info\" role = \"alert\">";
	resp += "<h3>Прошивка</h3>\n";
	resp += "<form method = \"POST\" action = \"/update\" enctype = \"multipart/form-data\" class=\"form-inline\">\n";
	resp += "<div class = \"btn-group\">\n";
	resp += "<input type = \"file\" class = \"btn\" name = \"update\">\n";
	resp += "<input type = \"submit\" class = \"btn\" value = \"Прошить\" onclick = \"this.value = 'Подождите...';\">\n";
	resp += "</div>\n";
	resp += "</form>\n";
	resp += "</div>\n";
	resp += "<div class = \"alert alert-success\" role = \"alert\">";
	resp += "<h3>WEB сервер</h3>\n";
	resp += "<form method = \"POST\" action = \"/spiffs\" enctype = \"multipart/form-data\" class=\"form-inline\">";
	resp += "<div class = \"btn-group\">\n";
	resp += "<input type = \"file\" class = \"btn\" name = \"spiffs\">\n";
	resp += "<input type = \"submit\" class = \"btn\" value = \"Прошить\" onclick = \"this.value = 'Подождите...';\">\n";
	resp += "</div>\n";
	resp += "</form>\n";
	resp += "</div>\n";
	resp += "</body>\n</html>\n";

	server->send(200, "text/html", resp);
}

void HttpHelper::clientHandle() {
	if (server!=NULL) server->handleClient();
}

boolean HttpHelper::handleFileRead(String path) {

	if (path.endsWith("/")) path += "index.htm";
	String contentType;
	if (path.endsWith(".htm") || path.endsWith(".html")) contentType = "text/html";
	else if (path.endsWith(".css")) contentType = "text/css";
	else if (path.endsWith(".js")) contentType = "application/javascript";
	else if (path.endsWith(".png")) contentType = "image/png";
	else if (path.endsWith(".gif")) contentType = "image/gif";
	else if (path.endsWith(".jpg")) contentType = "image/jpeg";
	else if (path.endsWith(".ico")) contentType = "image/x-icon";
	else if (path.endsWith(".xml")) contentType = "text/xml";
	else if (path.endsWith(".pdf")) contentType = "application/x-pdf";
	else if (path.endsWith(".zip")) contentType = "application/x-zip";
	else if (path.endsWith(".gz")) contentType = "application/x-gzip";
	else if (path.endsWith(".json")) contentType = "application/json";
	else contentType = "text/plain";

	//split filepath and extension

	String prefix = path, ext = "";
	int lastPeriod = path.lastIndexOf('.');
	if (lastPeriod >= 0) {
		prefix = path.substring(0, lastPeriod);
		ext = path.substring(lastPeriod);
	}

	//look for smaller versions of file
	//minified file, good (myscript.min.js)
	 
	if (SPIFFS.exists(prefix + ".min" + ext)) path = prefix + ".min" + ext;
	//gzipped file, better (myscript.js.gz)
	if (SPIFFS.exists(prefix + ext + ".gz")) path = prefix + ext + ".gz";
	//min and gzipped file, best (myscript.min.js.gz)
	if (SPIFFS.exists(prefix + ".min" + ext + ".gz")) path = prefix + ".min" + ext + ".gz";

	if (SPIFFS.exists(path)) {

		File file = SPIFFS.open(path, "r");
		//if (server->hasArg())
		if (server->hasArg("download"))
			server->sendHeader("Content-Disposition", " attachment;");
		if (server->uri().indexOf("nocache") < 0)
			server->sendHeader("Cache-Control", " max-age=172800");

		//optional alt arg (encoded url), server sends redirect to file on the web
		//if (WiFi.status() == WL_CONNECTED && ESP8266WebServer::hasArg("alt")) {
		//	ESP8266WebServer::sendHeader("Location", ESP8266WebServer::arg("alt"), true);
		//	ESP8266WebServer::send(302, "text/plain", "");
		//}
		//else {
		//	//server sends file
		//	size_t sent = ESP8266WebServer::streamFile(file, contentType);
		//}
		size_t sent = server->streamFile(file, contentType);
		file.close();
		return true;
	} //if SPIFFS.exists

	//server->send(200, "text/plain", SPIFFS.);
	return false;
} //bool handleFileRead

