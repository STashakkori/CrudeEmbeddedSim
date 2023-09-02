#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include "cmd.h"
#include "comm.cpp"
#include "global.h"

class Executive
{
   public:
      uint32_t getMissionTime(void) {
         return mission_time;
      };

      void loadCommand(Command cmd, int32_t* error)
      {
         if(cmd.getType() == COMMAND && !isCommandTableLocked){
            commandTable.push_back(cmd);
         } 
         else if(cmd.getType() == TELEMETRY && !isTelemetryTableLocked){
            telemetryTable.push_back(cmd);
         }
         else {
            // Health and status can't be in initial load
            *error = EHSLOAD;
         }
      };

      void lockCommandTable(void)
      {
         isCommandTableLocked = true; 
      };

      void lockTelemetryTable(void)
      {
         isCommandTableLocked = true; 
      };

      // TODO: Need to turn this into a state machine that can handle modes such as open-loop mode versus
      //       a closed-loop mode or built-in test mode or shutdown mode.
      void executeMission(int32_t* error)
      {
         std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
         int mission_time = 0;
         while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsed_time = current_time - start_time;
            mission_time = static_cast<int>(elapsed_time.count());
            std::cout << "Mission time: " << mission_time << " seconds" << std::endl;
            setMissionTime(mission_time);

            processCommands(error);
            getInputs();
            readSensors();
            sendHealthStatus(error);
         }
      };

      void printCommandTable(void)
      {
         uint32_t count = 1;
         for(Command i: commandTable){
            std::cout << "Command " << count << ": " << i.getSystem() << " " 
                      << i.getSubSystem() << " " << i.getType() << " " << std::endl;
            count++;
         }
      }

      void printTelemetryTable(void)
      {
         uint32_t count = 1;
         for(Command i: telemetryTable){
            std::cout << "Telemetry " << count << ": " << i.getSystem() << " " 
                      << i.getSubSystem() << " " << i.getType() << " " << std::endl;
            count++;
         }
      }

   private:
      bool isCommandTableLocked = false;
      bool isTelemetryTableLocked = false;
      std::vector<Command> commandTable;
      std::vector<Command> telemetryTable; 
      uint32_t mission_time;
      CommRouter comm;

      void setMissionTime(uint32_t time)
      {
         mission_time = time;
      };

      void processCommands(int32_t* error) 
      {
         for(Command cmd: telemetryTable){
            if (cmd.getSystem() == EXEC1){
               // TODO: Invoke command processor
            }
            else{
               comm.dispatch(cmd, error);
            }
            commandTable.erase(commandTable.begin());
         }
      };

      void getInputs(void)
      {
         // TODO: Check CommRouter queue for input messages 
      };

      void readSensors(void)
      {
         uint32_t eax;
         asm volatile("mov %%eax, %0;":"=r"(eax));
         
         uint32_t ebx;
         asm volatile("mov %%ebx, %0;":"=r"(eax));
 
         uint32_t ecx;
         asm volatile("mov %%ecx, %0;":"=r"(eax));
  
         uint32_t edx;
         asm volatile("mov %%edx, %0;":"=r"(eax));
  
         uint32_t esi;
         asm volatile("mov %%esi, %0;":"=r"(eax));
  
         uint32_t edi;
         asm volatile("mov %%ebx, %0;":"=r"(eax));
  
         uint32_t ebp;
         asm volatile("mov %%ebp, %0;":"=r"(eax));
  
         uint32_t esp;
         asm volatile("mov %%esp, %0;":"=r"(eax));

         if(debug) {
            std::cout << "EAX: " << eax << std::endl;  
            std::cout << "EBX: " << ebx << std::endl;
            std::cout << "ECX: " << ecx << std::endl;
            std::cout << "EDX: " << edx << std::endl;  
            std::cout << "ESI: " << esi << std::endl;  
            std::cout << "EDI: " << edi << std::endl;  
            std::cout << "EBP: " << ebp << std::endl;  
            std::cout << "ESP: " << esp << std::endl;  
         }
      };

      void sendHealthStatus(int32_t* error)
      {
         // Get the amount of RAM
         FILE* fp = fopen("/proc/meminfo", "r");
         if (fp == NULL) {
            *error = EFOPEN;
            return;
         }
         char* buffer = (char*)malloc(256 * sizeof(char));
         int32_t n;
         while (fgets(buffer, 256, fp)) {
            if (sscanf(buffer, "MemTotal: %d kB", &n) == 1) {
               if(debug) { printf("Total memory: %d MB\n", n / 1024); }
               break;
            }
         }
         fclose(fp);

         // TODO: build a health status message and dispatch
         Command hs = Command(GROUND, UNSET, HEALTHSTATUS, 256, (uint8_t*)buffer);
         comm.dispatch(hs, error);
      };
};
