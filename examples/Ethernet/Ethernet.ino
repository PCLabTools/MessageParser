/**
 * @file Serial.ino
 * @author Larry Colvin (pclabtools@projectcolvin.com)
 * @brief Message parser for reading/writing and splitting Message state and data information from a STREAM message
 * @version 0.1
 * @date 2021-12-26
 * 
 * @copyright Copyright PCLabTools(c) 2021
 * 
 */

#include <Arduino.h>
#include <MessageParser.h>
#include <SPI.h>
#include <Ethernet.h>

/**
 * @brief MessageParser Object
 * 
 * @details A MessageParser object is created to deal with incoming stream data and allows parsing into MESSAGE format
 * 
 */
MessageParser eth;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);
EthernetServer server(23);

/**
 * @brief Initial Setup
 * 
 * @details When the device is booted the following actions are performed:
 *  -# TCP communication is begun at desired baud rate 9600
 * 
 */
void setup()
{
  Ethernet.begin(mac, ip);
  server.begin();
}

/**
 * @brief Main Application Loop
 * 
 * @details In this example the main loop calls handleEthernet() to deal with reading the incoming stream
 * 
 */
void loop()
{
  handleEthernet();
}

/**
 * @brief handleEthernet
 * 
 * @details Provides serial interrupt-style reading of incoming bytes:
 *  -# While stream bytes are available the bytes are appended to inputStream
 *  -# The inputStream string is sent to the MessageParser object to be converted into a MESSAGE
 *  -# If the resultant MESSAGE is complete it is sent to handleMessage() along with the Stream being used
 * 
 */
void handleEthernet() {
  String inputStream = "";
  EthernetClient client = server.available();
  while (client.available()) 
  {
    inputStream += client.read();
  }
  Message currentMessage = eth.parse(inputStream);
  if (currentMessage.state != "")
  {
    handleMessage(client, currentMessage);
    currentMessage.state = "";
  }
}

/**
 * @brief Process a stream message and call an appropriate action if the message state is valid
 * 
 * @details Calls a subfunction based on the state information of the incoming message
 * Included data in the incoming message can be used to customise the action taken
 * The state is case-insensitive to provide flexibility to the incoming stream message
 * 
 * State     | Description            | Data        | Example            |
 * --------- | -----------------      | ----------- | ------------------ |
 * *IDN?     | Identity Query         |             | *IDN?              |
 * *ECHO     | Echo Message Data      | String Echo | *ECHO::Hello World |
 * <INVALID> | No Corresponding State |             | ABC::123           |
 * 
 * @param STREAM Communication stream (Serial/I2C/Ethernet/FileIO)
 * @param MESSAGE Message to parse (MESSAGE.state & MESSAGE.data[])
 */
void handleMessage(Stream &STREAM, Message MESSAGE)
{
  if (MESSAGE.state == "*idn?") 
  {
    STREAM.println("Example,Serial");
  }
  else if (MESSAGE.state == "*echo") 
  {
    STREAM.println(MESSAGE.data[0]);
  }
  else
  {
    STREAM.println("ERROR::UNKNOWN_STATE_CALLED");
  }
}