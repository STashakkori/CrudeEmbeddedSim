#include "cmd.h"

// TODO: Check if all of these includes are actually being used. I copied the superset here for time.
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080 // TODO: Need to turn sys.h systems and subsys.h into a datastructure that maps IPaddr/port
#define IP_ADDRESS "127.0.0.1"

class CommRouter
{
   public:
      struct sockaddr addr;
      void dispatch(Command cmd, int32_t* error) 
      {
         // TODO: In here will want to eventually do a lookup on the datastructures define in sys and subsys
         //       to route packets to the propre destinations. Hardcoding some stuff for now.
         int sockfd;
         struct sockaddr_in servaddr;

         // Create a UDP socket
         sockfd = socket(AF_INET, SOCK_DGRAM, 0);
         if(!sockfd){
            *error = ESCK;
         }

         // Configure the server address and port
         memset(&servaddr, 0, sizeof(servaddr));
         servaddr.sin_family = AF_INET;
         servaddr.sin_port = htons(PORT);
         inet_pton(AF_INET, IP_ADDRESS, &servaddr.sin_addr);

         // Send a message to the server
         uint8_t* payload = cmd.serialize(cmd, error);
         sendto(sockfd, payload, MAXCOMMANDLENGTH, 0, (const struct sockaddr*) &servaddr, sizeof(servaddr));

         // Close the socket
         close(sockfd);
      }

      void getInputs(void)
      {
         // TODO: Get UDP traffic, deserialize, and buffer them for Executive to process.
         //       Need to open a thread to do this continously then this function will read from the queue
         //       when Executive is ready to pull from it.
      }
   private:
};
