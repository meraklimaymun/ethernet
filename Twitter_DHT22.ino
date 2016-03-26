// Twitter client sketch for ENC28J60 based Ethernet Shield. Uses 
// arduino-tweet.appspot.com as a OAuth gateway.
// Step by step instructions:
// 
//  1. Get a oauth token:
//     http://arduino-tweet.appspot.com/oauth/twitter/login
//  2. Put the token value in the TOKEN define below
//  3. Run the sketch!
//
//  WARNING: Don't send more than 1 tweet per minute!
//  NOTE: Twitter rejects tweets with identical content as dupes (returns 403)

#include <EtherCard.h>

#include <DHT.h>
#define DHTPIN 4 //DHT22'nin veri pinini Arduino'daki hangi pine bağladığımızı belirtiyoruz.
#define DHTTYPE DHT22 //Hangi DHT sensörünü kullandığımızı belirtiyoruz.
#define ALTITUDE 80.0 //Bulunduğunuz şehrin rakım değeri. Yaşadığınız şehrin rakımına buradan bakın: http://tr.wikipedia.org/wiki/T%C3%BCrkiye_il_merkez_rak%C4%B1mlar%C4%B1
DHT dht(DHTPIN, DHTTYPE);
int nem;
int sicaklik;

// OAUTH key from http://arduino-tweet.appspot.com/
#define TOKEN   ""

// ethernet interface mac address, must be unique on the LAN
byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

const char website[] PROGMEM = "arduino-tweet.appspot.com";

static byte session;

byte Ethernet::buffer[700];
Stash stash;

static void sendToTwitter () {
nem = dht.readHumidity(); 
sicaklik = dht.readTemperature();
  Serial.println("Sending tweet...");
  byte sd = stash.create();

  char tweet[60];
  snprintf(tweet, sizeof(tweet), "Odamdaki nem: %d%%, Odamin sicakligi %dC", nem, sicaklik); // format the string we want to send
  stash.print("token=");
  stash.print(TOKEN);
  stash.print("&status=");
  stash.println(tweet);
  stash.save();
  int stash_size = stash.size();

  // Compose the http POST request, taking the headers below and appending
  // previously created stash in the sd holder.
  Stash::prepare(PSTR("POST http://$F/update HTTP/1.0" "\r\n"
    "Host: $F" "\r\n"
    "Content-Length: $D" "\r\n"
    "\r\n"
    "$H"),
  website, website, stash_size, sd);

  // send the packet - this also releases all stash buffers once done
  // Save the session ID so we can watch for it in the main loop.
  session = ether.tcpSend();
}

void setup () {
  dht.begin();
  Serial.begin(57600);
  Serial.println("\n[Twitter Client]");

  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
    Serial.println(F("Failed to access Ethernet controller"));
  if (!ether.dhcpSetup())
    Serial.println(F("DHCP failed"));

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);  
  ether.printIp("DNS: ", ether.dnsip);  

  if (!ether.dnsLookup(website))
    Serial.println(F("DNS failed"));

  ether.printIp("SRV: ", ether.hisip);

  sendToTwitter();
}

void loop () {

  ether.packetLoop(ether.packetReceive());

  const char* reply = ether.tcpReply(session);
  if (reply != 0) {
    Serial.println("Got a response!");
    Serial.println(reply);
  }
}
