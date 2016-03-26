//Bu kod ile Arduino'ya bağlı herhangi bir elektronik bileşene web üzerinden komut verebilirsiniz.
//RandomNerdTutorials'den alınıp, projeye uyarlanmıştır. http://randomnerdtutorials.com/

#include <UIPEthernet.h>

int led = 4;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };   //physical mac address
byte ip[] = { 192, 168, 1, 196 };                      // ip in lan (that's what you need to use in your browser. 
byte gateway[] = { 192, 168, 1, 1 };                   // internet access via router
byte subnet[] = { 255, 255, 255, 0 };                  //subnet mask
EthernetServer server(80);                             //server port     
String readString;

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  pinMode(led, OUTPUT);

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip, gateway);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // Create a client connection
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {   
      if (client.available()) {
        char c = client.read();
     
        //read char by char HTTP request
        if (readString.length() < 100) {
          //store characters to string
          readString += c;
          //Serial.print(c);
         }

         //if HTTP request has ended
         if (c == '\n') {          
           Serial.println(readString); //print to serial monitor for debuging
     
           client.println("HTTP/1.1 200 OK"); //send new page
           client.println("Content-Type: text/html");
           client.println();     
           client.println("<HTML>");
           client.println("<HEAD>");
           client.println("");
           client.println("");
           client.println("");
           client.println("<TITLE>Merakli Maymun LED Kontrol</TITLE>");
           client.println("</HEAD>");
           client.println("<BODY>");
           client.println("<H1>Merakli Maymun LED Kontrol</H1>");
           client.println("<hr />");
           client.println("<br />");  
           client.println("<a href=\"/?led_acik\"\">Led'i Ac</a>");
           client.println("<a href=\"/?led_kapali\"\">Led'i Kapat</a><br />");   
           client.println("<br />");     
           client.println("<br />"); 
           client.println("");  
           client.println("<br />"); 
           client.println("</BODY>");
           client.println("</HTML>");
     
           delay(1);
           //stopping client
           client.stop();
           //controls the Arduino if you press the buttons
           if (readString.indexOf("?led_acik") >0){
               digitalWrite(led, HIGH);
           }
           if (readString.indexOf("?led_kapali") >0){
               digitalWrite(led, LOW);
           }
            //clearing string for next read
            readString="";  
           
         }
       }
    }
}
}
