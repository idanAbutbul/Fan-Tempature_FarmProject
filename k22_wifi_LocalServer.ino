#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

int temp;

IPAddress    apIP(55, 55, 55, 55); 
const char* ssid = "IdanJoy"; //שם הרשת שיוצג
const char* password = "88888888";// הסיסמה לרשת אם נרצה להשתמש בה 

ESP8266WebServer server(80); 

void wifi_loop() {
      server.handleClient();//.server is defined at this tab
      delay(10);
  } 
void wifi_setup(){
   WiFi.mode(WIFI_AP_STA);
   WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));   // subnet FF FF FF 00   
   WiFi.softAP(ssid); 
//   WiFi.softAP(ssid,password); 

  server.on("/", handleRoot); 
  server.onNotFound(handleNotFound); 
  server.begin(); 
  
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  
}
void handleRoot(){
 if (server.hasArg("degree")) {
      givenDegree = server.arg("degree").toInt();
  }
     if (server.hasArg("WorkOnTemp")) {
       state = 1;
     }
     if (server.hasArg("AlwaysOff")) {
       state = 2;
     }     
     if (server.hasArg("AlwaysOn")) {
       state = 3;
     }         
  char currentTmp[6]="";
  char TempSet[6]="";
  char tmp[5]=""; 
  char html[2000]="";
strcat(html, "<html>");
strcat(html, "<!DOCTYPE html>");
strcat(html, "<html lang=\"en\">");
strcat(html, "<head>");
strcat(html, "<meta charset=\"UTF-8\">");
strcat(html, "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">");
strcat(html, "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
strcat(html, "<META HTTP-EQUIV=\"refresh\" CONTENT=\"5\">");
strcat(html,"<title>Degree and Fan Control</title>");
strcat(html,"<style>");
strcat(html,"form, #fan-state {");
strcat(html,"text-align: center;");
strcat(html,"margin: 0 auto;");
strcat(html,"width: 50%;");
strcat(html,"}");
strcat(html,"body {");
strcat(html,"font-family: Arial, sans-serif;}");
strcat(html,"h1 {");
strcat(html,"text-align: center;");
strcat(html,"color: #333;");
strcat(html,"text-shadow: 2px 2px #ccc;");
strcat(html,"margin-top: 50px;}");
strcat(html,"input[type=\"text\"], input[type=\"submit\"] {");
strcat(html,"padding: 12px 20px;");
strcat(html,"margin: 8px 0;");
strcat(html,"box-sizing: border-box;");
strcat(html,"border: 2px solid #ccc;");
strcat(html,"border-radius: 4px;");
strcat(html,"font-size: 16px;}");
strcat(html,"input[type=\"submit\"] {");
strcat(html,"background-color: #4CAF50;");
strcat(html,"color: white;");
strcat(html,"cursor: pointer;}");
strcat(html,".fan-state {    text-align: center; margin: 0 auto; width: 50%;}");
strcat(html,".fan-control button {padding: 12px 20px;margin: 8px 0;  box-sizing: border-box;  border: 2px solid #ccc;  border-radius: 4px;  font-size: 16px;  background-color: #4CAF50;  color: white;  cursor: pointer;}");
strcat(html,".temp-display {  text-align: center;  margin: 20px auto;  width: 50%;  font-size: 20px;}");
strcat(html,".fan-control {    display: flex;    justify-content: center;    padding: 20px 0;}");
strcat(html,"</style>");
strcat(html,"</head>");
strcat(html,"<body>");
strcat(html,"<h1>Degree and Fan Control</h1>");
strcat(html,"<form>");
strcat(html,"<label for=\"degree-input\">Enter degree:</label>");
strcat(html,"<input type=\"number\" id=\"degree-input\" name=\"degree\" type=\"number\">");
strcat(html,"<input type=\"submit\" value=\"Submit\" type=\"number\">");
itoa(givenDegree,tmp,10.0);
strcat(html,"</form>");
strcat(html,"<form action=\"/\" method=\"get\" class=\"fan-control\">");
strcat(html,"<input id=\"fan-on\" name=\"AlwaysOn\" type=\"submit\" value=\"Always On\">");
strcat(html,"<input id=\"fan-off\" name=\"AlwaysOff\" type=\"submit\" value=\"Always Off\">");
strcat(html,"<input id=\"fan-temp\" name=\"WorkOnTemp\" type=\"submit\" value=\"Works On Temp\">");
strcat(html,"</form>");
strcat(html,"</div>");
if(state == 1)
strcat(html,"<p class=\"fan-state\"><span id=\"fan-state\">Working on Tempature</span></p>");
if(state == 2)
strcat(html,"<p class=\"fan-state\"><span id=\"fan-state\">Always Off</span></p>");
if(state == 3)
strcat(html,"<p class=\"fan-state\"><span id=\"fan-state\">Always on</span></p>");
strcat(html,"<p class=\"temp-display\">Current Temperature: <span id=\"current-temp\">");
itoa(RN_Temp,currentTmp,10.0);
strcat(html,currentTmp);
strcat(html,"</span></p>");
strcat(html,"<p class=\"temp-display\">Set Temperature: <span id=\"set-temp\">");
itoa(givenDegree,TempSet,10.0);
strcat(html,TempSet);
strcat(html,"</span></p>");
strcat(html,"</body>");
strcat(html,"</html>");


  
  server.send(200, "text/html", html);
}
void handleNotFound(){
  String message = "File Not Foundn\n\n";
      message += "URI: ";
      message += server.uri();
      message += "\nMethod: ";
      message += (server.method() == HTTP_GET) ? "GET" : "POST";
      message += "\nArguments: ";
      message += server.args();
      message += "\n";

      for (uint8_t i = 0; i < server.args(); i++) {
          message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
      }

      server.send(404, "text/plain", message);
}
