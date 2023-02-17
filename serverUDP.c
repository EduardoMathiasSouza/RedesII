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
#include "utils.h"

#define LOCAL_SERVER_PORT 1500
#define MAX_MSG 1024
#define NUM_DATA_SENT 100

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

  //lista de dados recebidos
  char receivedData[NUM_DATA_SENT][1];
  for(int i = 0; i < 100; i++){
      receivedData[i][0] = 0;
  }
  int count = 1;
  int order = 1;

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

    //adiciona a mensagem na lista dos recebidos
    int i;
    sscanf(msg, "%d", &i);
    receivedData[i][0] = 1;

    if(i != count){
      order = 0;
    }

    count++;

    /* print received message */
    printf("%s: from %s:UDP%u : %s \n", 
	   argv[0],inet_ntoa(cliAddr.sin_addr),
	   ntohs(cliAddr.sin_port),msg);

     if(count == NUM_DATA_SENT+1){
        //printar taxa de perda e se a ordem foi mantida
        int loss = 0;
        for(int i = 1; i < NUM_DATA_SENT+1; i++){
          if(receivedData[i][0] == 0){
            loss++;
          }
        }
        printf("Taxa de perda: %d%%\n", loss);
        if(order == 1){
          printf("Mensagens foram recebidas em ordem\n");
        }else{
          printf("Mensagens foram recebidas fora de ordem\n");
        }

        //zerar lista de dados recebidos
        for(int i = 0; i < 100; i++){
          receivedData[i][0] = 0;
        }
        count = 1;
        order = 1;
     }
    
  }/* end of server infinite loop */

return 0;

}
