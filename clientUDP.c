/*Cliente UDP 
Alunos: Eduardo Mathias de Souza e  Vinicius Matheus
Criado em: 02/2023
*/


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> 
#include <sys/time.h> 

#define REMOTE_SERVER_PORT 1500
#define MAX_MSG 1024
#define NUM_DATA_SENT 1000

int main(int argc, char *argv[]) {
  
  int sockfd, rc, i;
  struct sockaddr_in cliAddr, remoteServAddr;
  struct hostent *h;

  /* Confimando os argumentos da linha de comando*/
  if(argc<2) {
    puts("Uso: ./cliente <server>\n");
    exit(1);
  }

  /* get server IP address (no check if input is IP address or DNS name */
  h = gethostbyname(argv[1]);
  if(h==NULL) {
    printf("%s: unknown host '%s' \n", argv[0], argv[1]);
    exit(1);
  }

  printf("%s: sending data to '%s' (IP : %s) \n", argv[0], h->h_name,
	 inet_ntoa(*(struct in_addr *)h->h_addr_list[0]));

  remoteServAddr.sin_family = h->h_addrtype;
  memcpy((char *) &remoteServAddr.sin_addr.s_addr, 
	 h->h_addr_list[0], h->h_length);
  remoteServAddr.sin_port = htons(REMOTE_SERVER_PORT);

  /* Cria um socket 
  AF_INET = IPv4
  SOCK_DGRAM = tipo de socket UDP*/
  sockfd = socket(AF_INET,SOCK_DGRAM,0);
  if(sockfd<0) {
    printf("%s: cannot open socket \n",argv[0]);
    exit(1);
  }
  
  /* bind any port */
  cliAddr.sin_family = AF_INET;
  cliAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  cliAddr.sin_port = htons(0);
  
  rc = bind(sockfd, (struct sockaddr *) &cliAddr, sizeof(cliAddr));
  if(rc<0) {
    printf("%s: cannot bind port\n", argv[0]);
    exit(1);
  }


  /* send data */
  for(long int i=1;i<=NUM_DATA_SENT;i++) {
    char msg[NUM_DATA_SENT];
    sprintf(msg, "%ld", i);
    if(sendto(sockfd, msg, sizeof(i), 0, 
		(struct sockaddr *) &remoteServAddr, 
		sizeof(remoteServAddr))<0) {
      printf("%s: cannot send data %ld \n",argv[0],i-1);
      close(sockfd);
      exit(1);
    }
  }
  
  return 1;

}
