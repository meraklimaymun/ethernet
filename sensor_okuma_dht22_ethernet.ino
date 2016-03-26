#include <DHT.h>
#define DHTPIN 2 //DHT22'nin veri pinini Arduino'daki hangi pine bağladığımızı belirtiyoruz.
#define DHTTYPE DHT22 //Hangi DHT sensörünü kullandığımızı belirtiyoruz.
#define ALTITUDE 80.0 //Bulunduğunuz şehrin rakım değeri. Yaşadığınız şehrin rakımına buradan bakın: http://tr.wikipedia.org/wiki/T%C3%BCrkiye_il_merkez_rak%C4%B1mlar%C4%B1
DHT dht(DHTPIN, DHTTYPE); //Belirttiğimiz değişkenleri kaydettiriyoruz.
// This is a demo of the RBBB running as webserver with the Ether Card
// 2010-05-28 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

#include <EtherCard.h>

// ethernet interface mac address, must be unique on the LAN
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
static byte myip[] = { 192,168,1,203 };
byte Ethernet::buffer[500];
BufferFiller bfill;
int nem;
int sicaklik;

void setup () {
  dht.begin();
  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0)
    Serial.println(F("Failed to access Ethernet controller"));
  ether.staticSetup(myip);
}

static word homePage() {
  long t = millis() / 1000;
  word h = t / 3600;
  byte m = (t / 60) % 60;
  byte s = t % 60;
  bfill = ether.tcpOffset();
  bfill.emit_p(PSTR(
    "HTTP/1.0 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Pragma: no-cache\r\n"
    "\r\n"
    "<meta http-equiv='refresh' content='1'/>"
    "<title>Sensor okuma ornek</title>" 
    "<br /><div align='center'>"
    "<h1>Arduino su kadar suredir acik: $D$D:$D$D:$D$D</h1>"
    "<h2>Sicaklik: $DC</h2>"
    "<h3>Nem: %$D$</h3>"),
      h/10, h%10, m/10, m%10, s/10, s%10, sicaklik, nem);
  return bfill.position();
}

void loop () {
  nem = dht.readHumidity();
  sicaklik = dht.readTemperature();
  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);
  
  if (pos)  // check if valid tcp data is received
    ether.httpServerReply(homePage()); // send web page data
}
