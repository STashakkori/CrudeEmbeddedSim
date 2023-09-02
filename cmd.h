#ifndef CMD_HH
#define CMD_HH

#define MAXDATALENGTH 2000
#define MAXCOMMANDLENGTH (MAXDATALENGTH + 4 + 4 + 4 + 4)
#define COMMAND 1
#define TELEMETRY 2
#define HEALTHSTATUS 3
#define UNSET 0

#include "err.h"
#include "sys.h"
#include "subsys.h"

class Command {
   public:
      // prototypes
       Command();
       Command(uint32_t, uint32_t, uint32_t, uint32_t, uint8_t*);
       uint32_t getSystem(void);
       uint32_t getSubSystem(void);
       uint32_t getType(void);
       uint32_t getDataLength(void);
       uint8_t* getData(void);
       uint8_t* serialize(Command, int32_t*);
       Command deserialize(uint8_t*, int32_t*);
   private:
      // member variables 
      uint32_t system;
      uint32_t subsystem;
      uint32_t type;
      uint32_t data_length;
      uint8_t* data;

      // prototypes
      void packU32(uint8_t*, uint32_t, uint32_t*, int32_t*);
      void unpackU32(uint8_t*, uint32_t*, uint32_t*, int32_t*);
      void resetCommand(Command* cmd, int32_t*);
      void resetBuffer(uint8_t*, uint32_t, int32_t*);
};

#endif
