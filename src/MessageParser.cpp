/**
 * @file MessageParser.cpp
 * @author Larry Colvin (pclabtools@projectcolvin.com)
 * @brief MessageParser library functional definitions
 * @version 1.0
 * @date 2021-12-26
 * 
 * @copyright Copyright PCLabTools (c) 2021
 * 
 */

#include "MessageParser.h"

MessageParser::MessageParser(String DELIMITER, String DATA_DELIMITER, String END_OF_MESSAGE) {
  _DELIMITER = DELIMITER;
  _DATA_DELIMITER = DATA_DELIMITER;
  _END_OF_MESSAGE = END_OF_MESSAGE;
}

Message MessageParser::parse(String RAW_STRING) {
  _BUFFER += RAW_STRING;
  Message returnMessage;
  if (_BUFFER.indexOf(_END_OF_MESSAGE) > 0) {
    String FULL_MESSAGE = _BUFFER.substring(0, _BUFFER.indexOf(_END_OF_MESSAGE));
    _BUFFER.remove(0, (_BUFFER.indexOf(_END_OF_MESSAGE) + _END_OF_MESSAGE.length()));
    returnMessage.state = FULL_MESSAGE.substring(0, FULL_MESSAGE.indexOf(_DELIMITER));
    returnMessage.state = returnMessage.state.substring(0, returnMessage.state.lastIndexOf('\r'));
    returnMessage.state.toLowerCase();
    String DATA = FULL_MESSAGE.substring(FULL_MESSAGE.indexOf(_DELIMITER) + _DELIMITER.length());
    if (DATA.indexOf(_DATA_DELIMITER) != -1) {
      int index = 0;
      while (DATA.indexOf(_DATA_DELIMITER) != -1) {
        returnMessage.data[index] = DATA.substring(0, DATA.indexOf(_DATA_DELIMITER));
        DATA.remove(0, (DATA.indexOf(_DATA_DELIMITER) + _DATA_DELIMITER.length()));
        index++;
      }
      returnMessage.data[index] = DATA;
    } else {
      returnMessage.data[0] = DATA;
    }
    return returnMessage;
  } else {
    returnMessage.state = "";
    for (byte i = 0; i < MAX_DATA_ELEMENTS; i++) {
      returnMessage.data[i] = "";
    }
    return returnMessage;
  }
}

String MessageParser::serialise(Message MESSAGE) {
  String returnString = MESSAGE.state + _DELIMITER;
  int index = 0;
  while (MESSAGE.data[index] != "") {
    returnString += (MESSAGE.data[index] + _DATA_DELIMITER);
    index++;
  }
  if (index) {
    return returnString.substring(0,(returnString.length()-_DATA_DELIMITER.length()));
  } else {
    return returnString.substring(0,(returnString.length()-_DELIMITER.length()));
  }
}
