/* Web_Parms_1.pde - very simple Webduino example of parameter passing and parsing */

/*
 * This is mostly a tool for testing and debugging the library, but can
 * also be used as an example of coding for it.
 *
 * To use it,  enter one of the following USLs into your browser.
 * Replace "host" with the IP address assigned to the Arduino.
 *
 * http://host/
 * http://host/index.html
 *
 * These return a "success" HTTP result and display the parameters
 * (if any) passed to them as a single string,  without attempting to
 * parse them.  This is done with a call to defaultCmd.
 * 
 * 
 * http://host/raw.html
 *
 * This is essentially the same as the index.html URL processing,
 * but is done by calling rawCmd.
 * 
 * 
 * http://host/parsed.html
 *
 * This invokes parsedCmd,  which displays the "raw" parameter string,
 * but also uses the "nexyURLparam" routine to parse out the individual
 * parameters, and display them.
 */


#define WEBDUINO_FAIL_MESSAGE "<h1>Request Failed</h1>"
#include "SPI.h" // new include
#include "avr/pgmspace.h" // new include
#include "Ethernet.h"
#include "WebServer.h"
#include "EEPROM.h"



#define VERSION_STRING "0.1"
#define aref_voltage 3.3

/* CHANGE THIS TO YOUR OWN UNIQUE VALUE.  The MAC number should be
 * different from any other devices on your network or you'll have
 * problems receiving packets. */
static uint8_t mac[] = { 
  0xDE, 0xAD, 0xBE, 0xAF, 0xAE, 0xDD };


/* CHANGE THIS TO MATCH YOUR HOST NETWORK.  Most home networks are in
 * the 192.168.0.XXX or 192.168.1.XXX subrange.  Pick an address
 * that's not in use and isn't going to be automatically allocated by
 * DHCP from your router. */
static uint8_t ip[] = { 
  192, 168, 223, 195 };

// ROM-based messages used by the application
// These are needed to avoid having the strings use up our limited
//    amount of RAM.

P(Page_start) = "<html><head><title>Web_Parms_1 Version " VERSION_STRING "</title></head><body>\n";
P(Page_end) = "</body></html>";
P(Get_head) = "<h1>GET from ";
P(Post_head) = "<h1>POST to ";
P(Unknown_head) = "<h1>UNKNOWN request for ";
P(Default_head) = "unidentified URL requested.</h1><br>\n";
P(Raw_head) = "raw.html requested.</h1><br>\n";
P(Parsed_head) = "parsed.html requested.</h1><br>\n";
P(Good_tail_begin) = "URL tail = '";
P(Bad_tail_begin) = "INCOMPLETE URL tail = '";
P(Tail_end) = "'<br>\n";
P(Parsed_tail_begin) = "URL parameters:<br>\n";
P(Parsed_item_separator) = " = '";
P(Params_end) = "End of parameters<br>\n";
P(Post_params_begin) = "Parameters sent by POST:<br>\n";
P(Line_break) = "<br>\n";



/* This creates an instance of the webserver.  By specifying a prefix
 * of "", all pages will be at the root of the server. */
#define PREFIX ""
WebServer webserver(PREFIX, 80);


/* commands are functions that get called by the webserver framework
 * they can read any posted data from client, and they output to the
 * server to send data back to the web browser. */
void helloCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  /* this line sends the standard "we're all OK" headers back to the
   browser */
  server.httpSuccess("application/json", NULL);

  /* if we're handling a GET or POST, we can output our data here.
   For a HEAD request, we just stop after outputting headers. */
  if (type == WebServer::HEAD)
    return;

  server.printP(Page_start);
  switch (type)
  {
    case WebServer::GET:
    server.printP(Get_head);
    break;
    case WebServer::POST:
    server.printP(Post_head);
    break;
  default:
    server.printP(Unknown_head);
  }

  server.printP(Default_head);
  server.printP(tail_complete ? Good_tail_begin : Bad_tail_begin);
  server.print(url_tail);
  server.printP(Tail_end);
  server.printP(Page_end);

}


void rawCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  /* this line sends the standard "we're all OK" headers back to the
   browser */
  server.httpSuccess();

  /* if we're handling a GET or POST, we can output our data here.
   For a HEAD request, we just stop after outputting headers. */
  if (type == WebServer::HEAD)
    return;

  server.printP(Page_start);
  switch (type)
  {
    case WebServer::GET:
    server.printP(Get_head);
    break;
    case WebServer::POST:
    server.printP(Post_head);
    break;
  default:
    server.printP(Unknown_head);
  }

  server.printP(Raw_head);
  server.printP(tail_complete ? Good_tail_begin : Bad_tail_begin);
  server.print(url_tail);
  server.printP(Tail_end);
  server.printP(Page_end);

}

#define NAMELEN 32
#define VALUELEN 32

void parsedCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  //not authenticated
  //if(!server.checkCredentials("YWRtaW46YWRtaW4=")) //admin:admin
  if(!server.checkCredentials("YWRtaW46UG9ra2VyODAwMA==")) //admin:Pokker8000
  {
    server.httpUnauthorized();
  }
  else //authenticated
  {
    URLPARAM_RESULT rc;
    char name[NAMELEN];
    int  name_len;
    char value[VALUELEN];
    int value_len;

    /* this line sends the standard "we're all OK" headers back to the
     browser */
    //  server.httpSuccess();
    server.httpSuccess("application/json", "Content-Length: 115\r\n");
    //server.httpSuccess("application/json", NULL);    

    /* if we're handling a GET or POST, we can output our data here.
     For a HEAD request, we just stop after outputting headers. */
    if (type == WebServer::HEAD)
      return;

    //server.printP(Page_start);
    //  switch (type)
    //  {
    //    case WebServer::GET:
    //    //server.printP(Get_head);
    //    break;
    //    case WebServer::POST:
    //    //server.printP(Post_head);
    //    break;
    //  default:
    //    //server.printP(Unknown_head);
    //  }

    //server.printP(Parsed_head);
    //server.printP(tail_complete ? Good_tail_begin : Bad_tail_begin);
    //server.print(url_tail);
    //server.printP(Tail_end);

    if (strlen(url_tail))
    {

      //server.printP(Parsed_tail_begin);
      while (strlen(url_tail))
      {
        rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
        if (rc == URLPARAM_EOS)
          server.printP(Params_end);
        else
        {
          String nameString = String(name);
          String valueString = String(value);

          if(nameString.indexOf("pinD") > -1)
          {

            //server.print("Pin: " + nameString);

            //          char pin_as_char[nameString.length()]; // or:  char *test_as_char; .... not sure if you have to initialize the buffer
            //          nameString.toCharArray(pin_as_char, nameString.length());
            //          int intPin = int(pin_as_char);
            //          
            //          char value_as_char[valueString.length()]; // or:  char *test_as_char; .... not sure if you have to initialize the buffer
            //          valueString.toCharArray(value_as_char, valueString.length());
            //          int intValue = int(value_as_char);

            nameString.replace("pinD", "");
            char this_char[nameString.length() + 1];
            nameString.toCharArray(this_char, (nameString.length() + 1));
            int my_pin_data = atoi(this_char);

            char this_char_value[valueString.length() + 1];
            valueString.toCharArray(this_char_value, (valueString.length() + 1));
            int pinvalue = atoi(this_char_value);
            Serial.println(pinvalue);

            //pinMode(my_pin_data, OUTPUT);
            if(my_pin_data == 6 || my_pin_data == 7 || my_pin_data == 8)
            {
              digitalWrite(my_pin_data , pinvalue);
              EEPROM.write(my_pin_data, pinvalue);
            }

            //          server.println("nameString: " + nameString);
            //          server.println("Pin: " + String(my_pin_data));
            //          server.println("PinValue: " + String(pinvalue));
            //          server.println();
          }


          //

          //server.print(name);
          //server.printP(Parsed_item_separator);
          //server.print(value);
          //server.printP(Tail_end);
        }
      }
    }



    //  if (type == WebServer::POST)
    //  {
    //    server.printP(Post_params_begin);
    //    while (server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN))
    //    {
    //      server.print(name);
    //      server.printP(Parsed_item_separator);
    //      server.print(value);
    //      server.printP(Tail_end);
    //    }
    //  }
    //server.printP(Page_end);
    server.print("{");
    server.print("\"Data\" : {");
    server.print("\"Switches\"  : [");

    // output the value of each analog input pin
    int NoOfPins = 14;
    for (int i = 0; i < NoOfPins; i++) {

      //Only add pins I am using to make to request shorter
      if(i == 6 || i == 7 || i == 8)
      {
        server.print("{");
        printData(server, "T", "D", true);
        printData(server, "P", String(i), true);
        printData(server, "V", getPinStatus(i), false);
        //if(i == NoOfPins -1)
        if(i == 8)        // 8 is highest pin used
        {
          server.print("}");
        }
        else
        {
          server.print("},");
        }   
      }
      //client.print("Pin:" + String(i+2) + "Value:" + EEPROM.read(i+2));        
    }
    server.print("]");

    //float temperature = getVoltage(0);  //getting the voltage reading from the temperature sensor
    //temperature = (temperature - .5) * 100;          //converting from 10 mv per degree wit 500 mV offset
    //to degrees ((volatge - 500mV) times 100)

    //    int tempReading = analogRead(0);
    //    float voltage = tempReading * aref_voltage;
    //    voltage /= 1024.0; 
    //    float temperature = (voltage - 0.5) * 100 ;
    //
    //    char test[20];
    //    String tem1p =   floatToString(test, temperature, 3, 6);
    //server.print(temp);                     //printing the result
    //printData(server, "Temp", tem1p , false);
    server.print("}");
    server.print("}");
  }
}

char * floatToString(char * outstr, double val, byte precision, byte widthp){
  char temp[16];
  byte i;

  // compute the rounding factor and fractional multiplier
  double roundingFactor = 0.5;
  unsigned long mult = 1;
  for (i = 0; i < precision; i++)
  {
    roundingFactor /= 10.0;
    mult *= 10;
  }

  temp[0]='\0';
  outstr[0]='\0';

  if(val < 0.0){
    strcpy(outstr,"-\0");
    val = -val;
  }

  val += roundingFactor;

  strcat(outstr, itoa(int(val),temp,10));  //prints the int part
  if( precision > 0) {
    strcat(outstr, ".\0"); // print the decimal point
    unsigned long frac;
    unsigned long mult = 1;
    byte padding = precision -1;
    while(precision--)
      mult *=10;

    if(val >= 0)
      frac = (val - int(val)) * mult;
    else
      frac = (int(val)- val ) * mult;
    unsigned long frac1 = frac;

    while(frac1 /= 10)
      padding--;

    while(padding--)
      strcat(outstr,"0\0");

    strcat(outstr,itoa(frac,temp,10));
  }

  // generate space padding
  if ((widthp != 0)&&(widthp >= strlen(outstr))){
    byte J=0;
    J = widthp - strlen(outstr);

    for (i=0; i< J; i++) {
      temp[i] = ' ';
    }

    temp[i++] = '\0';
    strcat(temp,outstr);
    strcpy(outstr,temp);
  }

  return outstr;
} 
/*
 * getVoltage() - returns the voltage on the analog input defined by
 * pin
 */
float getVoltage(int pin){
  return (analogRead(pin) * .004882814); //converting from a 0 to 1023 digital range
  // to 0 to 5 volts (each 1 reading equals ~ 5 millivolts
}
String printquote()
{
  return "\"";
}

String getPinStatus(int pin)
{
  if(digitalRead(pin) == HIGH)
  {
    return "1";
  }
  else
  {
    return "0";    
  }
}

//Toggles a relay on or off when a input is detected on inputSwitchPin
void setupButton(int inputSwitchPin, int OutPutRelay)
{
  if(digitalRead(inputSwitchPin) == LOW)
  {
    delay(10);
    if(digitalRead(inputSwitchPin) == LOW)
    {
      //Serial.println("Button Pressed");
      if(digitalRead(OutPutRelay) == 1)
      {
        digitalWrite(OutPutRelay, 0);
        EEPROM.write(OutPutRelay, 0);
      }
      else
      {
        digitalWrite(OutPutRelay, 1);
        EEPROM.write(OutPutRelay, 1);
      }
      delay(500);
    }
  }
}

void my_failCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  /* this line sends the standard "we're all OK" headers back to the
   browser */
  server.httpFail();

  /* if we're handling a GET or POST, we can output our data here.
   For a HEAD request, we just stop after outputting headers. */
  if (type == WebServer::HEAD)
    return;

  server.printP(Page_start);
  switch (type)
  {
    case WebServer::GET:
    server.printP(Get_head);
    break;
    case WebServer::POST:
    server.printP(Post_head);
    break;
  default:
    server.printP(Unknown_head);
  }

  server.printP(Default_head);
  server.printP(tail_complete ? Good_tail_begin : Bad_tail_begin);
  server.print(url_tail);
  server.printP(Tail_end);
  server.printP(Page_end);

}


void setupInitalState(int relayPin)
{
  if(EEPROM.read(relayPin) == 1)
  {
    digitalWrite(relayPin, 1);
    //Serial.println("Digital Write 8: State:1 \r\n");
  }
  else
  {
    digitalWrite(relayPin, 0);
    //Serial.println("Digital Write 8: State:0 \r\n");
  }

  //Serial.println(EEPROM.read(relayPin));
}

void setup()
{
  //digitalWrite(2, HIGH);
  delay(5000);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(5, INPUT);

  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);

  setupInitalState(6); //get inital state from eeprom and if set to 1 turn on light
  setupInitalState(7); //get inital state from eeprom and if set to 1 turn on light
  setupInitalState(8); //get inital state from eeprom and if set to 1 turn on light

  /* initialize the Ethernet adapter */
  Ethernet.begin(mac, ip);

  /* setup our default command that will be run when the user accesses
   * the root page on the server */
  webserver.setDefaultCommand(&helloCmd);

  /* setup our default command that will be run when the user accesses
   * a page NOT on the server */
  webserver.setFailureCommand(&my_failCmd);

  /* run the same command if you try to load /index.html, a common
   * default page name */
  webserver.addCommand("index.html", &helloCmd);

  /*This command  is called if you try to load /raw.html */
  webserver.addCommand("raw.html", &rawCmd);
  webserver.addCommand("lights.html", &parsedCmd);

  /* start the webserver */
  webserver.begin();
}

void loop()
{
  char buff[64];
  int len = 64;

  setupButton(3, 6);
  setupButton(5, 7);
  setupButton(2, 8);
  /* process incoming connections one at a time forever */
  webserver.processConnection(buff, &len);
}

void printData(WebServer &server, String fieldname, String value, boolean comma)
{
  server.print(""); 
  //client.println("{");
  server.print(""); 
  server.print("\"");
  server.print(fieldname);
  server.print("\"");
  server.print(": ");
  server.print("\"");
  server.print(value);
  server.print("\"");
  if(comma)
  {
    server.print(",");
  }
  else
  {
    server.print("");
  }
}






