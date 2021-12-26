/**
 * @file MessageParser.h
 * @author Larry Colvin (pclabtools@projectcolvin.com)
 * @brief Library header file for the MessageParser Arduino library
 * @version 1.0
 * @date 2021-12-26
 * 
 * @copyright Copyright PCLabTools (c) 2021
 * 
 */

#include <Arduino.h>

#ifndef MAX_DATA_ELEMENTS
  #define MAX_DATA_ELEMENTS 10
#endif

struct Message {
  String state;
  String data[MAX_DATA_ELEMENTS];
};

#ifndef MessageParser_h

  #define MessageParser_h

  class MessageParser {
    public:
      MessageParser(String DELIMITER = "::", String DATA_DELIMITER = ",", String END_OF_MESSAGE = "\n");
      Message parse(String RAW_STRING);
      String serialise(Message MESSAGE);
             
     private:
      String _DELIMITER;
      String _DATA_DELIMITER;
      String _END_OF_MESSAGE;
      String _BUFFER;
  };

#endif
