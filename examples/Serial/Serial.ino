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

/**
 * @brief MessageParser Object
 * 
 * @details A MessageParser object is created to deal with incoming stream data and allows parsing into MESSAGE format
 * 
 */
MessageParser uart;

/**
 * @brief Initial Setup
 * 
 * @details When the device is booted the following actions are performed:
 *  -# Serial communication is begun at desired baud rate 9600
 * 
 */
void setup()
{
  Serial.begin(9600);
}

/**
 * @brief Main Application Loop
 * 
 * @details In this example the main loop does nothing to demonstrate the "background" handling of Stream communication
 * 
 */
void loop()
{
  //
}

/**
 * @brief serialEvent
 * 
 * @details Provides serial interrupt-style reading of incoming bytes:
 *  -# While stream bytes are available the bytes are appended to inputStream
 *  -# The inputStream string is sent to the MessageParser object to be converted into a MESSAGE
 *  -# If the resultant MESSAGE is complete it is sent to handleMessage() along with the Stream being used
 * 
 */
void serialEvent() {
  String inputStream = "";
  while (Serial.available())
  {
    inputStream += (char)Serial.read();
  }
  Message currentMessage = uart.parse(inputStream);
  if (currentMessage.state != "")
  {
    handleMessage(Serial, currentMessage);
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