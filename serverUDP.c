/*Server UDP 
Alunos: Eduardo Mathias de Souza e Vinicius Matheus
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
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>

#define LOCAL_SERVER_PORT 1500
#define MAX_MSG 1024
#define NUM_DATA_SENT 1000


// Função para printar os dados recebidos e resetar as variáveis
void resetData(long int **lostData, int *lostDataTam, long int *count, int *order){
    // Calcula a taxa de perda
    int loss = *count - *lostDataTam;
    loss = (loss * 100) / NUM_DATA_SENT;

    printf("Taxa de perda: %d%%\n", loss);
    if(*order == 1){
      printf("Mensagens foram recebidas em ordem\n");
    }else{
      printf("Mensagens foram recebidas fora de ordem\n");
    }

    *count = 1;
    *order = 1;
    *lostDataTam = 0;
}

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

  //Define timeout de 2 segundos
  struct timeval timeout = {2,0};
  setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));


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
  long int *lostData;
  lostData = (long int *) malloc((NUM_DATA_SENT) * sizeof(long int));
  if(lostData == NULL){
    printf("Erro ao alocar memoria");
    exit(1);
  }

  int lostDataTam = 0;
  long int count = 1;
  int order = 1;

  /* server infinite loop */
  while(1) {
    
    /* init buffer */
    memset(msg,0x0,MAX_MSG);

    /* receive message */
    cliLen = sizeof(cliAddr);
    n = recvfrom(sockfd, msg, MAX_MSG, 0, 
		 (struct sockaddr *) &cliAddr, &cliLen);

    // Verifica se houve erros na recepção
    if(n<0) {
      // Verifica se o erro foi de timeout
      if(errno == EWOULDBLOCK || errno == EAGAIN){
        // Se foi timeout e já estava recebendo mensagens, reinicia a contagem
        if(count > 1)
          resetData(&lostData, &lostDataTam, &count, &order);
        continue;
      } else {
        printf("%s: cannot receive data \n",argv[0]);
        continue;
      }
    // Se não houve erross
    } else {
      //adiciona a mensagem na lista dos recebidos
      long int i;
      sscanf(msg, "%ld", &i);

      // Verifica se a mensagem já está no vetor de mensagens perdidas
      if(lostDataTam > 0){
        for(int j = 0; j < lostDataTam; j++){
          // Se a mensagem já está no vetor, remove ela do vetor
          if(i == lostData[j]){
            lostData[j] = 0;
            order = 0;
            break;
          }
        }
      }

      // Verifica se a mensagem recebida é a próxima da sequência
      if(i != count){
        // Se não for, adiciona a mensagem no vetor de mensagens perdidas
        if(i < count)
          order = 0;
        lostData[lostDataTam] = count;
        lostDataTam++;
      }
      count++;

      /* print received message */
      printf("%s: from %s:UDP%u : %s \n", 
      argv[0],inet_ntoa(cliAddr.sin_addr),
      ntohs(cliAddr.sin_port),msg);

      // Verifica se já recebeu todas as mensagens
      if(count == NUM_DATA_SENT+1){
          resetData(&lostData, &lostDataTam, &count, &order);
      }
    }
    
  }/* end of server infinite loop */

return 0;

}
