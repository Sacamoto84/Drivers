/* Virtuino bluetooth library
  * Created by Ilias Lamprou
 * Updated Noe 16 2017
 * 
 * Download latest Virtuino android app from the link: https://play.google.com/store/apps/details?id=com.virtuino_automations.virtuino
 * Video tutorial link: https://www.youtube.com/watch?v=CYR_jigRkgk
 * Contact address for questions or comments: iliaslampr@gmail.com
 */

/*========= VirtuinoBluetooth Class methods  
*  vPinMode(int pin, int state)                                  set pin as digital OUTPUT or digital INPUT.  (Insteed default pinMode method
*
*========= Virtuino General methods  
*  void vDigitalMemoryWrite(int digitalMemoryIndex, int value)   write a value to a Virtuino digital memory   (digitalMemoryIndex=0..31, value range = 0 or 1)
*  int  vDigitalMemoryRead(int digitalMemoryIndex)               read  the value of a Virtuino digital memory (digitalMemoryIndex=0..31, returned value range = 0 or 1)
*  void vMemoryWrite(int analogMemoryIndex, float value)         write a value to Virtuino float memory       (memoryIndex=0..31, value range as float value)
*  float vMemoryRead(int analogMemoryIndex)                      read the value of  Virtuino analog memory    (analogMemoryIndex=0..31, returned value range = 0..1023)
*  run()                                                         neccesary command to communicate with Virtuino android app  (on start of void loop)
*  int getPinValue(int pin)                                      read the value of a Pin. Usefull for PWM pins
*/


#include "VirtuinoBluetooth.h"
#include <string.h>
#include <stdio.h> /* for printf */
#include <stdbool.h>
#include <stdlib.h>

#include "SEGGER_RTT.h"

extern char BTserial_available(void);

extern osSemaphoreId myBinarySem_RX_CounterHandle;

#define size_commandBuffer 32
#define size_btResponseBuffer 32

char  textReceivedCommandBuffer[32];
char  textToSendCommandBuffer[32]; 

char  commandBuffer[size_commandBuffer]; 
char  btResponseBuffer[size_btResponseBuffer];

int   virtualDigitalMemory[bt_virtualDigitalMemorySize];                       
int   virtualDigitalMemoryIdol[bt_virtualDigitalMemorySize]; 
float virtualFloatMemory[bt_virtualAnalogMemorySize]; 

void readBluetoothSerialData(void);
void checkVirtuinoCommand(char * command);
char VirtuinoBluetooth_getCommandType(char c);
void VirtuinoBluetooth_checkIfIOsHaveChanged(void);
void substring (char * str_source, char * str_desc, int pos , int len );
void  VirtuinoBluetooth_run(void);

static void getErrorCommand(char * str ,char code);
static void executeReceivedCommand(char activeCommandType, int activeCommandPin ,char * commandString, bool returnInfo);
int getCommandPin(char * aCommand);
float getCommandValue(char* aCommand);

char str_temp[16];

void TaskVirtuinoTX(void *argument);
void TaskVirtuoRoutine(void *argument);

void TaskVirtuinoTX(void *argument)
{
  for(;;)
  {
    osDelay(1);
  }
}

void TaskVirtuoRoutine(void *argument)
{
  for(;;)
  {
		VirtuinoBluetooth_run();
    //osDelay(1);
  }
}

////================================================================== VirtuinoBluetooth init
//VirtuinoBluetooth::VirtuinoBluetooth(SoftwareSerial &uart, uint32_t baud): BTserial(&uart){
//    //BTserial->begin(baud);
//    while (BTserial_available()) BTserial_read();
//}



////================================================================== run
void  VirtuinoBluetooth_run(void) {
  //VirtuinoBluetooth_checkIfIOsHaveChanged();                                   // This void informs VirtUino app every time a pin's status is changed 
	static uint16_t i = 0;
	
	
	//Ждем появления данных на RX по симафору
	
		if( xSemaphoreTake( myBinarySem_RX_CounterHandle,  portMAX_DELAY ) == pdTRUE )
    {	
      
      			
			SEGGER_SYSVIEW_Print("---Take VirtuinoBluetooth_run");
			SEGGER_RTT_printf(0, RTT_CTRL_RESET">"RTT_CTRL_TEXT_BRIGHT_GREEN"Take VirtuinoBluetooth_run %s%d\r\n", RTT_CTRL_BG_BRIGHT_BLUE, i);
		
			//SEGGER_RTT_WriteString(0, RTT_CTRL_RESET"Red: "RTT_CTRL_TEXT_BRIGHT_RED" This text is red. "RTT_CTRL_TEXT_BLACK""RTT_CTRL_BG_BRIGHT_RED"This background is red. "RTT_CTRL_RESET"Normal text again.");

			readBluetoothSerialData(); //Идем на обработку
			i++;
		}
}

////================================================================= checkIfIOsHaveChanged
void VirtuinoBluetooth_checkIfIOsHaveChanged(void){
     int pin;
     int p;
     int pinValue;
//       if (!connectedStatus) return;
//        //---- check if digitalInput state has changed
//         for (int i=0;i<bt_arduinoPinsSize; i++){
//           if ((arduinoPinsMap[i]>0) && (arduinoPinsMap[i]<3)) {
//              pinValue=digitalRead(i);
//              if (arduinoPinsValue[i]!=pinValue){
//                  arduinoPinsValue[i]=pinValue;
//                  if ((arduinoPinsMap[i]==1) || (arduinoPinsMap[i]==2)) sendCommandResponse('Q',i,String(pinValue));
//               }
//           }
//         } // end for  
//        
//       //---- check if digitalMemory has changed
//          for (int i=0;i<bt_virtualDigitalMemorySize; i++){
//           if (virtualDigitalMemoryIdol[i] != virtualDigitalMemory[i]){
//              virtualDigitalMemoryIdol[i] = virtualDigitalMemory[i];
//              sendCommandResponse('D',i,String(virtualDigitalMemory[i]));
//           } 
//          }

}








//================================================================= readBluetoothSerialData
//This void reads data from bluetooth serial and stores them to commandBuffer
//If we have a valid  command from app, void then calls the void executeReceivedCommand to execute the command
//If command doesn't have a valid format it returns an error response to App  - error code = 0
//This is a system fuction. Don't change this code

 void readBluetoothSerialData(void){
       
	 char res = 0;	 
	 
	 while (BTserial_available()) 
				{
         char tempChar = BTserial_read();
           
					#if defined (DEBUG) 
					  //HAL_UART_Transmit(&huart2, (uint8_t *) tempChar, 1, 1000);
					#endif
									
          
					
					
					
					
					if (tempChar == bt_COMMAND_START_CHAR) 
					{                           // a new command is starting...
						for(uint32_t i = 0; i<size_commandBuffer;i++) 
						  commandBuffer[i]=0;	 //Очистка commandBuffer 				
              
						commandBuffer[0]=tempChar;             
						res = BTserial_readStringUntil(bt_COMMAND_END_CHAR, &commandBuffer[1], size_commandBuffer - 2, 1000);
						
						if (res)
							return; //Выход по ошибке, т.к. не получили конца
						
						strcat (commandBuffer, bt_COMMAND_END_CHAR_STRING);
						
              #if defined (DEBUG) 
						    char str[32];
						    sprintf(str, "Command=(%s)\n", commandBuffer);
								HAL_UART_Transmit(&huart2, str, strlen(str), 1000);
						  #endif
						
						
						  char str[32];
						  sprintf(str, "Command=[%s]\r\n", commandBuffer);
						  SEGGER_RTT_printf(0, str);
						  SEGGER_SYSVIEW_Print(str);
						
						
//              if (commandBuffer.length() > 150)  sendCommandResponse('E',0,String(bt_ERROR_COMMAND));
//              else{
						        //////checkVirtuinoCommand(&commandBuffer[0]);       //Нет ошибок
//                  if (textToSendCommandBuffer.length()>0) {
//                    btResponseBuffer+=textToSendCommandBuffer;
//                    textToSendCommandBuffer="";
//                  }
//                  if (DEBUG) Serial.println("Response= "+btResponseBuffer);
//                  BTserial->print(btResponseBuffer);
//                  btResponseBuffer="";
//                }
//               break; 
          }
					
					
					
					
					
  }  //while (BTserial_available() >=6 )
}


////================================================================================== urlencode
////==================================================================================
//void VirtuinoBluetooth::urlencode(String* str){
//      str->replace("!","%21");
//      str->replace("$","%24");
//}
////================================================================================== urldecode
////==================================================================================
//void VirtuinoBluetooth::urldecode(String* str){
//    str->replace("%21","!");
//    str->replace("%24","$");
//}

////======================================================================================== clearTextBuffer
////========================================================================================
void VirtuinoBluetooth_clearTextBuffer(){
	for(uint32_t i = 0; i<sizeof(textReceivedCommandBuffer);i++)
	  textReceivedCommandBuffer[i]=0;
}

////======================================================================================== clearTextBuffer
////========================================================================================
//int VirtuinoBluetooth::  textAvailable(){return textReceivedCommandBuffer.length();}
//  
////======================================================================================== getTextByID
////========================================================================================
//String VirtuinoBluetooth:: getText(byte ID){
//   String returnedText="";
//   String ID_string ="";
//   ID_string+=bt_COMMAND_START_CHAR;
//   ID_string+='T';
//   if (ID<10) ID_string+='0';
//   ID_string+=ID;
//   ID_string+="=";       
//   int  pos=textReceivedCommandBuffer.indexOf(ID_string);
//   if (pos>=0) {
//      pos= textReceivedCommandBuffer.indexOf("=",pos);
//      int lastPos=textReceivedCommandBuffer.indexOf(bt_COMMAND_END_CHAR,pos);
//      returnedText= textReceivedCommandBuffer.substring(pos+1,lastPos);
//      urldecode(&returnedText);
//      clearTextByID(ID,&textReceivedCommandBuffer);
//   }
//   return returnedText;

//  }

////======================================================================================== clearTextByID
////========================================================================================
//void VirtuinoBluetooth:: clearTextByID(byte ID, String* textBuffer){
//   String ID_string ="";
//   ID_string+=bt_COMMAND_START_CHAR;
//   ID_string+='T';
//   if (ID<10) ID_string+='0';
//   ID_string+=ID;
//   ID_string+="=";                   // !ID3=
//   int  pos=textBuffer->indexOf(ID_string);
//   if (pos>=0) {
//      int lastPos= textBuffer->indexOf(bt_COMMAND_END_CHAR,pos);
//      textBuffer->remove(pos,lastPos+1);
//   }
//}

////======================================================================================== addTextToBuffer
////========================================================================================
//void VirtuinoBluetooth::addTextToReceivedBuffer(byte ID, String* text){
//   clearTextByID(ID,&textReceivedCommandBuffer);
//   textReceivedCommandBuffer+=bt_COMMAND_START_CHAR;
//   textReceivedCommandBuffer+= 'T';
//   if (ID<10) textReceivedCommandBuffer+= '0';
//   textReceivedCommandBuffer+=ID;
//   textReceivedCommandBuffer+= "=";
//   textReceivedCommandBuffer+= *text;
//   textReceivedCommandBuffer+=bt_COMMAND_END_CHAR;
//}

////======================================================================================== addTextToBuffer
////========================================================================================
//void VirtuinoBluetooth::sendText(byte ID, String text){
//   clearTextByID(ID,&textToSendCommandBuffer);
//   textToSendCommandBuffer+=bt_COMMAND_START_CHAR;
//   textToSendCommandBuffer+= 'T';
//   if (ID<10) textToSendCommandBuffer+= '0';
//   textToSendCommandBuffer+=ID;
//   textToSendCommandBuffer+= "=";
//   urlencode(&text);
//   textToSendCommandBuffer+=text; 
//   textToSendCommandBuffer+=bt_COMMAND_END_CHAR;
// }

int lastIndexOf(char * str, char x) 
{ 
    int index = -1; 
    for (int i = 0; i < strlen(str); i++) 
        if (str[i] == x) 
            index = i; 
				
    return index; 
} 


//Удаление конца строки начиная с позиции pos
// size общий размер строки
void remove1(char * str, uint8_t pos, int size) 
{  
	 char * p;
	 p = str + pos;
	 //int i = pos;
	
    for (pos = pos; pos < size; pos++) 
       *p++ = 0;
} 

int indexOf(char * str, char x, int size)
{
	int index = -1;
	
	char * p;
	p = str;
	
	 for (int i = 0; i < size; i++)
	  if (*p++ == x) return i;
	
	return index;
}

void substring (char * str_source, char * str_desc, int pos , int len )
{
	for(uint32_t i = 0; i< len; i++)
	{
		str_desc[i] = str_source[pos+i];
	}
}

////====================================================================================== checkBluetoothCommand
void  checkVirtuinoCommand(char * command){
	
	int pos = lastIndexOf(command, bt_COMMAND_END_CHAR); //Поиск последнего символа '$' с строке
	
	if (pos>5){                                          // Проверка размера команды                                                      
		//remove1(command, pos+1, size_commandBuffer);        //Удаляем конец буффера команды, для надежности !XYY=Z.ZZ?$

		for(uint32_t i = 0; i< size_btResponseBuffer;i++) 
		  btResponseBuffer[i]=0;	//Очистка буффера
		     
//    //  if ((password.length()==0) || (commandPassword.equals(password))) {                       // check if password correct
 
              int cStart = 0;
              int cEnd   = indexOf(command, bt_COMMAND_END_CHAR  , size_commandBuffer);
							 
							 if (cEnd > 5)  //Проверка на длину строки
							 {							
							  //char oneCommand[32];
							  //substring (command, oneCommand, cStart+1 , cEnd );  // get one command		
								char commandType; 
								 commandType = VirtuinoBluetooth_getCommandType(command[1]); //Определение типи команды, если ошибка то вернет 'E'
								
								 if (commandType!='E') //Ошибок в типе команды нет
										{
                      int pin= getCommandPin(&command[2]); //Определение номена ячейки
                      if (pin!=-1) //Номер пина без ошибки
												{
                          bool returnInfo=false;
                          if (command[5]=='?') returnInfo=true; //Запрос на текущее значение											    
											 
													 executeReceivedCommand(commandType , pin , &command[5], returnInfo); //Выполняем что хотят
													
////                        oneCommand.remove(0,4);
////                        if (oneCommand.length()<bt_TEXT_COMMAND_MAX_SIZE)
////                        else {
////                          btResponseBuffer+=getErrorCommand(bt_ERROR_SIZE);
////                          String s="";
////                          if (pin<9) s+="0";
////                          s+=String(pin);
////                          btResponseBuffer+=bt_COMMAND_START_CHAR;
////                          btResponseBuffer+='T';
////                          btResponseBuffer+=s;
////                          btResponseBuffer+="= ";
////                          btResponseBuffer+=bt_COMMAND_END_CHAR; 
////                        }
//											 
////                        lastCommunicationTime=millis();
////                      //  espResponseBuffer += commandResponse;
                     } //else  btResponseBuffer +=getErrorCommand(bt_ERROR_PIN);  // response  error pin number   !E00=1$

                  } //else btResponseBuffer +=getErrorCommand(bt_ERROR_TYPE);     // response  error type   !E00=3$   
////          
//              } else{
////                btResponseBuffer+=getErrorCommand(bt_ERROR_SIZE);  // response  error size of command   !E00=4$   
             }
////              command->remove(0,cEnd+1-cStart);
     
   
	 } //if (pos>5)
   //else  btResponseBuffer+=getErrorCommand(bt_ERROR_SIZE);         // the size of command is not correct
 

}




////================================================================= executeReceivedCommand
void executeReceivedCommand(char activeCommandType, int activeCommandPin ,char * commandString, bool returnInfo)
{
   // The value activeCommandType contains command type such as Digital output, Analog output etc.
   // The value activeCommandPin contains the pin number of the command
   // The value activeCommandValue contains the value (0 or 1 for digital, 0-255 for analog outputs, 0-1023 for analog memory) 
   // In this void we have to check if the command is correct and then execute the command 
   // After executing the command we have to send a response to app
     
	  char response[20]={0,};
	
    getErrorCommand(response, bt_ERROR_UNKNOWN);
	
//    String pinString="";
//    if (activeCommandPin<10) pinString = "0"+String(activeCommandPin);
//    else pinString=String(activeCommandPin);

    float activeCommandValue=0;
//    
    switch (activeCommandType) {
//        case 'T':                         
//					if ((activeCommandPin>=0) & (activeCommandPin < 100)){
//                response="";
//                response +=bt_COMMAND_START_CHAR;
//                response +=activeCommandType;
//                response +=pinString;
//                response +="=";
//                response +=*commandString;
//                response +=bt_COMMAND_END_CHAR;  // response 
//              //urldecode(commandString);
//               addTextToReceivedBuffer(activeCommandPin,commandString);
//            }
//          break;
			
//      case 'I':                      
//            if ((activeCommandPin>=0) & (activeCommandPin < bt_arduinoPinsSize))
//              response =bt_COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(digitalRead(activeCommandPin))+bt_COMMAND_END_CHAR;  // response 
//            else   response =getErrorCommand(bt_ERROR_PIN);  // response  error pin number   !E00=1$       
//          break;
			
//      case 'Q': 
//            if ((activeCommandPin>=0) & (activeCommandPin < bt_arduinoPinsSize)){
//                 if (returnInfo) response =bt_COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(digitalRead(activeCommandPin))+bt_COMMAND_END_CHAR;  // response 
//                 else {
//                   activeCommandValue = getCommandValue(commandString);
//                   if ((activeCommandValue == 0) || (activeCommandValue == 1)) {
//                          digitalWrite(activeCommandPin,activeCommandValue);
//                          arduinoPinsValue[activeCommandPin]=activeCommandValue;
//                          //virtualDigitalMemoryIdol[activeCommandPin]=activeCommandValue;
//                          response =bt_COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(digitalRead(activeCommandPin))+bt_COMMAND_END_CHAR;  // response 
//                   } else    response =getErrorCommand(bt_ERROR_VALUE);
//                   }
//            } else   response =getErrorCommand(bt_ERROR_PIN);  // response  error pin number   !E00=1$   
//          break; 

//       case 'D':
//            if ((activeCommandPin>=0) & (activeCommandPin<bt_virtualDigitalMemorySize)){ 
//                if (returnInfo) response =bt_COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(virtualDigitalMemory[activeCommandPin])+bt_COMMAND_END_CHAR;  // response 
//                else{
//                      activeCommandValue = getCommandValue(commandString);
//                       if (activeCommandValue==bt_WRONG_VALUE) response =getErrorCommand(bt_ERROR_VALUE);
//                       else {
//                              virtualDigitalMemory[activeCommandPin]= activeCommandValue; 
//                              virtualDigitalMemoryIdol[activeCommandPin]= activeCommandValue; 
//                              response =bt_COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(virtualDigitalMemory[activeCommandPin])+bt_COMMAND_END_CHAR;  // response
//                            } 
//                } //else    response =getErrorCommand(bt_ERROR_VALUE);
//            } else   response =getErrorCommand(bt_ERROR_PIN);  // response  error pin number   !E00=1$   
//            break; 
//      



       case 'V': 
           if ((activeCommandPin >=0 ) && (activeCommandPin < bt_virtualAnalogMemorySize))
					 { //Номер пина в норме
						 
             if (returnInfo) //Запрос на чтения текущего значения
						   {	 
							 //response =bt_COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(virtualFloatMemory[activeCommandPin])+bt_COMMAND_END_CHAR;  // response
              
						 //response =bt_COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(virtualFloatMemory[activeCommandPin])+bt_COMMAND_END_CHAR;
						   }
						 else 
						   {   
							        //Установка значения в ОЗУ из запроса
                      activeCommandValue = getCommandValue(commandString);
//                    if (activeCommandValue==bt_WRONG_VALUE) response =getErrorCommand(bt_ERROR_VALUE);
//                    else { 
                            virtualFloatMemory[activeCommandPin]= activeCommandValue; 
							              response[0] = bt_COMMAND_START_CHAR;
							              response[1] = 'V';
							              response[2] = 0x30 + activeCommandPin / 10;
							              response[3] = 0x30 + activeCommandPin % 10;
							              response[4] = '=';
							              sprintf(&response[5],"%.2f$", virtualFloatMemory[activeCommandPin]);							 
//                     }
						   } 
						 
						 } 
					   else 
							 getErrorCommand(response, bt_ERROR_PIN); //Номер пина выше максимального
             break;

						 
						 
						 
						 
//       case 'O': 
//           if ((activeCommandPin>=0) & (activeCommandPin < bt_arduinoPinsSize)){
//                   
//                   if (returnInfo) {
//                      int pwm_value = pulseIn(activeCommandPin, HIGH);
//                      pwm_value = pwm_value /7.85;
//                      response =bt_COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(pwm_value)+bt_COMMAND_END_CHAR;  // response 
//                  }
//                   else {
//                        activeCommandValue = getCommandValue(commandString);
//                        if (activeCommandValue==bt_WRONG_VALUE) response =getErrorCommand(bt_ERROR_VALUE);
//                          else { 
//                            arduinoPinsMap[activeCommandPin]=3; 
//                            arduinoPinsValue[activeCommandPin]=(int) activeCommandValue;
//                            analogWrite(activeCommandPin,(int)activeCommandValue);
//                            response =bt_COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String((int)activeCommandValue)+bt_COMMAND_END_CHAR;  // response
//                           } 
//                   }
//           } else   response =getErrorCommand(bt_ERROR_PIN);
//          break;

//         case 'A':                        
//            if ((activeCommandPin>=0) & (activeCommandPin < arduinoAnalogPinsSize))
//              response ="!"+String(activeCommandType)+pinString+"="+String(analogRead(analogInputPinsMap[activeCommandPin]))+"$";  // response 
//            else   response =getErrorCommand(bt_ERROR_PIN);  // response  error pin number   !E00=1$       
//          break;  
//         
//           case 'C':                   
//                      response="";  
//                      activeCommandValue= getCommandValue(commandString);
//                      if (activeCommandValue==1){
//                        response =bt_firmwareCode;//"!C00="+String(bt_firmwareCode)"+"$";//bt_COMMAND_START_CHAR+"C"+pinString+"=1"+bt_COMMAND_END_CHAR;     // return firmware version
//                        connectedStatus=true;
//                      } 
//                      else if (activeCommandValue==0) connectedStatus=false;
//                      else response =getErrorCommand(bt_ERROR_VALUE);
//                      break;  
           }
//      btResponseBuffer += response;   
}

// 

//Выделение самого получаемого значения char -> float
float getCommandValue(char* aCommand){
	
	char str[16]={0,};
	char * p;
	p = aCommand;
	char temp;
	
	//Построение строки для преобразования
	for( uint8_t i = 0 ; i< 16; i++)
	{
		temp = *p++;
		if (temp == '$') break;
		str[i] =  temp;
	}
	
	return atof(str);
}

//   //================================================================= getCommandType
//  //This function returns the command type which was received from app
//  //The second character of command string determines the command type
//  // I  digital input read command    
//  // Q  digital ouput write
//  // D  digital memory read - write commang
//  // A  analog input analog input read command
//  // O  analog output write command
//  // V  analog memory read - write command
//  // C  connect or disconect command    
//  // E  error
//  
//  // Other characters are recognized as an error and function returns the character E
//  //This is a system fuction. Don't change this code
//  
char VirtuinoBluetooth_getCommandType(char c)
{
	if (!(c== 'I' || c == 'Q' || c == 'D' ||c == 'A' ||c == 'O' || c == 'V' || c == 'C' || c == 'T')){
		c='E'; //error  command
	}
	return c;
}
	 
//  //================================================================= getCommandPin
//  //This function returns the pin number of the command string which was received from app
//  //Fuction checks if pin number is correct. If not it returns -1 as error code
// 
int  getCommandPin(char * aCommand){
	char *p;
	p = aCommand+1;
	return (*aCommand-0x30)*10 + (*p-0x30); //Затычка
}



// //==================================================================  sendCommandResponse  
// void  VirtuinoBluetooth::sendCommandResponse(char commandType, int commandPin , String commandValueString){
//    if (!connectedStatus) return;
//    String s="";
//    s+=bt_COMMAND_START_CHAR;
//    s+=commandType;
//    if (commandPin<10) s+='0';
//    s=s+String(commandPin);
//    s+='=';
//    s+=commandValueString;
//    s+=bt_COMMAND_END_CHAR;
//    
//    if (textToSendCommandBuffer.length()>0) {
//      s+=textToSendCommandBuffer;
//     
//      textToSendCommandBuffer="";
//    }
//    BTserial->print(s);
//    delay(10);              
//}


////=================================================================== getErrorCommand
static void getErrorCommand(char * str ,char code)
{
	str[0]= bt_COMMAND_START_CHAR;
	str[1]= 'E';
	str[2]= '0';
	str[3]= '0';
	str[4]= '=';
	str[5]= 0x30+code;
	str[6]= bt_COMMAND_END_CHAR;
	str[7]= 0;
}

//=================================================================== vDigitalMemoryWrite
void VirtuinoBluetooth_vDigitalMemoryWrite(int digitalMemoryIndex, int value)
{
  if ((digitalMemoryIndex>=0) && (digitalMemoryIndex<bt_virtualDigitalMemorySize))
	{
    virtualDigitalMemory[digitalMemoryIndex]=value;
  }
}
 
//=================================================================== vDigitalMemoryRead
int VirtuinoBluetooth_vDigitalMemoryRead(int digitalMemoryIndex)
{
  if ((digitalMemoryIndex>=0) && ( digitalMemoryIndex < bt_virtualDigitalMemorySize))
	{
    return virtualDigitalMemory[digitalMemoryIndex];
  }
  else return 0;    // error
}

//=================================================================== vMemoryWrite
void VirtuinoBluetooth_vMemoryWrite(int memoryIndex, float value)
{
  if ((memoryIndex >= 0) && ( memoryIndex < bt_virtualAnalogMemorySize))
	{
    virtualFloatMemory[memoryIndex]=value;
  }
}

//=================================================================== vMemoryRead
float VirtuinoBluetooth_vMemoryRead(int memoryIndex)
{
  if ((memoryIndex>=0) && (memoryIndex<bt_virtualAnalogMemorySize))
	{
    return virtualFloatMemory[memoryIndex];
  }
  else 
		return 0;    // error
}


////=================================================================== getPinValue
//int VirtuinoBluetooth::getPinValue(int pin){
//  if (pin>=0 && pin<bt_arduinoPinsSize) return arduinoPinsValue[pin];
//  else return 0;
//  }

////=================================================================== vPinMode
//void VirtuinoBluetooth::vPinMode(int pin, int mode){
//  if (pin>=0 && pin<bt_arduinoPinsSize){
//    pinMode(pin,mode);
//    if (mode == OUTPUT ) arduinoPinsMap[pin] =  1;
//      else if (mode == INPUT ) arduinoPinsMap[pin] =  2;
//  }
//}


////=================================================================== vDelay
//void VirtuinoBluetooth::vDelay(long milliseconds){
//  long timeStart=millis();
//  while (millis()-timeStart<milliseconds) run();
//}



