#include <iostream>
#include <cstring>
#include "cmd.h"

// TODO: Need to rename this class from Command to something more general like Message

// default constructor
Command::Command()
{
   // TODO: Add a label field to command that is optional but captures what the command does
   this->system = 0;
   this->subsystem = 0; // Subsystem field is optional
   this->type = COMMAND;
   this->data_length = MAXDATALENGTH;
   this->data = (uint8_t*)malloc(MAXDATALENGTH * sizeof(uint8_t));
   memset(this->data, 0x00, this->data_length);
}

Command::Command(uint32_t system, uint32_t subsystem, uint32_t type, uint32_t data_length, uint8_t* data)
{
   this->system = system;
   this->subsystem = subsystem;
   this->type = type;
   this->data_length = data_length;
   this->data = data; 
}

uint32_t Command::getSystem(void)
{
   return system;
}

uint32_t Command::getSubSystem(void)
{
   return subsystem;
}

uint32_t Command::getDataLength(void)
{
   return data_length;
}

uint32_t Command::getType(void)
{
   return type;
}

uint8_t* Command::getData(void)
{
   return data;
}

uint8_t* Command::serialize(Command cmd, int32_t* error)
{
   if(error != 0) {
      uint32_t index = 0;
      uint8_t* buffer = (uint8_t*)malloc(MAXCOMMANDLENGTH * sizeof(uint8_t));
      packU32(buffer, cmd.system, &index, error);
      packU32(buffer, cmd.subsystem, &index, error);
      packU32(buffer, cmd.type, &index, error);
      packU32(buffer, cmd.data_length, &index, error);

      for(uint32_t i = 0u; i < cmd.data_length; i++){
         buffer[index + i] = cmd.data[i];
      }
      return buffer;
   }
   return NULL;
}

Command Command::deserialize(uint8_t* buffer, int32_t* error)
{
   if(error != 0) {
      uint8_t* data = (uint8_t*)malloc(MAXDATALENGTH * sizeof(uint8_t));
      resetBuffer(data, MAXDATALENGTH, error); 
      Command cmd(UNSET, UNSET, UNSET, MAXDATALENGTH, data);
      // error handling
      if(buffer == 0){
         *error = EPTR;
         return cmd;
      }
      uint32_t index = 0;
      unpackU32(buffer, &(cmd.system), &index, error);
      unpackU32(buffer, &(cmd.subsystem), &index, error);
      unpackU32(buffer, &(cmd.type), &index, error);
      unpackU32(buffer, &(cmd.data_length), &index, error);
      uint32_t i = 0;
      for(i = 0; i < index; i++){
         cmd.data[i] = buffer[index + i];
      }
      return cmd;
   }
}

void Command::packU32(uint8_t* buffer, uint32_t value, uint32_t* index, int32_t* error)
{ 
   if(error != 0) {
      // error handling in here
      if(buffer == 0) {
         *error = EPTR;
         return;
      }
      if(index == 0){
         *error = EPTR;
         return;
      }
      // assign 4 bytes buffer to passed in value
      buffer[*index] = (uint8_t)((value >> (uint32_t)(32 - 8)) & (uint32_t)0xff);
      (*index)++;
      buffer[*index] = (uint8_t)((value >> (uint32_t)(32 - 16)) & (uint32_t)0xff);
      (*index)++;
      buffer[*index] = (uint8_t)((value >> (uint32_t)(32 - 24)) & (uint32_t)0xff);
      (*index)++;
      buffer[*index] = (uint8_t)((value >> (uint32_t)(32 - 32)) & (uint32_t)0xff);
      (*index)++;
   }
}

void Command::unpackU32(uint8_t* buffer, uint32_t* value, uint32_t* index, int32_t* error)
{
   if(error != 0){
      // error handling in here
      if(buffer == 0) {
         *error = EPTR;
         return;
      }
      if(value == 0){
         *error = EPTR;
         return;
      }
      if(index == 0){
         *error = EPTR;
         return;
      }
   }
   uint32_t temp = buffer[0];
   temp <<= 8;
   temp |= buffer[*index];
   (*index)++;
   temp <<= 8;
   temp |= buffer[*index];
   (*index)++;
   temp <<= 8;
   temp |= buffer[*index];
   (*index)++;
   temp <<= 8;
   temp |= buffer[*index];
   (*index)++;
   *value = temp;
}

void Command::resetCommand(Command* cmd, int32_t* error)
{
   if(error != 0) {
      // error handling
      if (cmd == 0){
         *error = EPTR;
         return;
      }
   }
   // clear metadata
   cmd->system = UNSET; // UNSET == 0
   cmd->subsystem = UNSET;
   cmd->data_length = 0;
   // clear data
   resetBuffer(cmd->data, MAXDATALENGTH, error); 
}

void Command::resetBuffer(uint8_t* buffer, uint32_t size, int32_t* error)
{
   if(error != 0){
      // error handling
      if (buffer == 0) {
         *error = EPTR;
         return;
      }
   }
   // zero out the buffer
   memset(buffer, 0x00, size);
}
