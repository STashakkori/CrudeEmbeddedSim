// System includes
#include <iostream>
#include <string>
#include <cstring>
#include <stdint.h>
#include <stdlib.h>
#include <memory.h>

// User includes
#include "exec.cpp"
#include "cmd.h"
#include "global.h"

// Typedefs
#define LAUNCH 10 // Warning - curently unused
#define NAVIGATE 20 // Warning - currently unused
#define ABORT 30 // Warning - currently unused

// Prototypes
void print_alert(std::string);
void print_data(uint8_t*);
void print_serialized(uint8_t*);
void print_cmd(Command);
void reset_data(uint8_t*);

int main () {
   print_alert("Started embedded system simulator");

   // TODO: Want to move the initial commands to being defined in a CSV file then being read in by
   //       a file processor CSCI then loading into Executive one by one in a loop.

   // Create a data object
   uint8_t* data1 = (uint8_t*)malloc(MAXDATALENGTH);
   memset((void*)data1, 0, (MAXDATALENGTH * sizeof(uint8_t)));
   data1[0] = 0x01;
   data1[1] = 0x02;
   data1[2] = 0x44;
   data1[3] = 0x88;
   data1[1999] = 0xff;

   // Create another data object
   uint8_t* data2 = (uint8_t*)malloc(MAXDATALENGTH);
   memset((void*)data2, 0, (MAXDATALENGTH * sizeof(uint8_t)));
   data2[0] = 0x03;
   data2[3] = 0x33;
   data2[4] = 0x01;
   data2[5] = 0x11;
   data2[100] = 0xee;
   data2[200] = 0xff;

   // Create a command destined for executive 1
   Command cmd1(EXEC1, UNSET, COMMAND, MAXDATALENGTH, data1);

   // create a command destined for slice 1
   Command cmd2(SLICE1, UNSET, COMMAND, MAXDATALENGTH, data2);

   Executive exec1;

   int32_t error = 0; // The debugging error pointer. Should be set by callee and checked by caller.
   // TODO: Actually check the error pointer by caller every time.

   exec1.loadCommand(cmd1, &error);
   exec1.loadCommand(cmd2, &error);
   print_alert("Loaded initial values into RAM");

   exec1.lockCommandTable();
   exec1.printCommandTable();

   print_alert("Beginning mission execution");
   exec1.executeMission(&error);   
   
   /********** TESTED EXAMPLE CODE HERE *************************
   uint8_t* buffer = cmd1.serialize(cmd1, &error);
   Command cmd1_deserialized = cmd1.deserialize(buffer, &error);
   uint8_t* buffer2 = cmd2.serialize(cmd2, &error);
   Command cmd2_deserialized = cmd2.deserialize(buffer2, &error);
   **************************************************************/

   print_alert("Shutting down simulator.");
}

void print_alert(std::string message)
{
   std::cout << "<ALERT>" << message << "</ALERT>" << std::endl;
}

void print_cmd(Command cmd)
{
   std::cout << "cmd" << std::endl;
   std::cout << "system: " << cmd.getSystem() << std::endl;
   std::cout << "subsystem: " << cmd.getSubSystem() << std::endl;
   std::cout << "type: " << cmd.getType() << std::endl;
   std::cout << "data_length: " << cmd.getDataLength() << std::endl;
   print_data(cmd.getData());
}

void print_data(uint8_t* buffer)
{
   printf("data: ");
   for(uint32_t i = 0; i < MAXDATALENGTH; i++) {
      printf("%02x", buffer[i]);
   }
   printf("\n");
}

void print_serialized(uint8_t* buffer)
{
   printf("serialized: ");
   for(uint32_t i = 0; i < MAXCOMMANDLENGTH; i++) {
      printf("%02x", buffer[i]);
   }
   printf("\n");
}

void reset_data(uint8_t* buffer)
{
   for(uint32_t i = 0; i < MAXDATALENGTH; i++){
      buffer[i] = 0x00;
   }
}
