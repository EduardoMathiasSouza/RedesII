/*Server UDP 
Alunos: Eduardo Mathias de Souza e ViniMax
Criado em: 02/2023
*/


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include <string.h> 

#define LOCAL_SERVER_PORT 1500
#define MAX_MSG 1024

int main(int argc, char *argv[]) {
  
  int sockfd, n, cliLen;
  struct sockaddr_in cliAddr, servAddr;
  char msg[MAX_MSG];

  /* Cria um socket 
  AF_INET = IPv4
  SOCK_DGRAM = tipo de socket UDP*/
  sockfd= socket(AF_INET, SOCK_DGRAM, 0);
  if(sockfd<0) {
    puts("Nao foi possivel criar o socket");
    exit(1);
  }

  /* bind local server port */
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servAddr.sin_port = htons(LOCAL_SERVER_PORT);
  int rc = bind (sockfd, (struct sockaddr *) &servAddr,sizeof(servAddr));
  if(rc < 0) {
    perror("Erro:");
    puts("Nao foi possivel realizar bind");
    exit(1);
  }

  printf("%s: waiting for data on port UDP %u\n", 
	   argv[0],LOCAL_SERVER_PORT);

  /* server infinite loop */
  while(1) {
    
    /* init buffer */
    memset(msg,0x0,MAX_MSG);


    /* receive message */
    cliLen = sizeof(cliAddr);
    n = recvfrom(sockfd, msg, MAX_MSG, 0, 
		 (struct sockaddr *) &cliAddr, &cliLen);

    if(n<0) {
      printf("%s: cannot receive data \n",argv[0]);
      continue;
    }
      
    /* print received message */
    printf("%s: from %s:UDP%u : %s \n", 
	   argv[0],inet_ntoa(cliAddr.sin_addr),
	   ntohs(cliAddr.sin_port),msg);
    
  }/* end of server infinite loop */

return 0;

}
