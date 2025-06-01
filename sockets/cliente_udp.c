#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdlib.h>
#include <signal.h>

struct data_result{
  char tipo;
  double resultado;
};
int sockfd;
const int MAX_LEN_OPERATION = 100;
const int PORTA = 6000;

int close_connection(int sig);

int main()
{
  int len;
  struct sockaddr_in address;
  int result;
  char operation[MAX_LEN_OPERATION];
  char server_ip[100];
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);

  struct timeval tv;
  tv.tv_sec = 5; // timeout de 5 segundos
  tv.tv_usec = 0;
  setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)); 
  
  // Solicita o IP do servidor ao usuário
  printf("Digite o endereço IP do servidor: ");
  fgets(server_ip, sizeof(server_ip), stdin);
  server_ip[strcspn(server_ip, "\n")] = 0; // Remove o '\n' do final

  // Define os dados do servidor
  address.sin_family = AF_INET;
  address.sin_port = htons(PORTA);
  inet_pton(AF_INET, server_ip, &address.sin_addr); // Converte string IP para binário

  signal(SIGINT, close_connection);
  printf("Socket iniciado! Envie operações udp e aguarde uma resposta.\n");
  
  while (1) {
    printf(">> ");
    fgets(operation, sizeof(operation), stdin);
    operation[strcspn(operation, "\n")] = '\0'; // remove '\n' se existir

    int op_len = strlen(operation) + 1; // inclui \0

    // Enviar para o servidor
    ssize_t sent = sendto(sockfd, operation, op_len, 0, (struct sockaddr*)&address, sizeof(address));
    if (sent < 0) {
        perror("Envio falhou!");
        printf("\n");
        continue;
    }

    // Receber resposta
    struct data_result resultado;
    socklen_t addr_len = sizeof(address);
    ssize_t received = recvfrom(sockfd, &resultado, sizeof(resultado), 0, (struct sockaddr*)&address, &addr_len);
    if (received < 0) {
        perror("Recebimento falhou!");
        printf("\n");
        continue;
    }
    process_resultado(resultado.tipo, resultado.resultado);
  }
	
  close_connection(0);
}

void process_resultado(char tipo, double resultado)
{
  if(tipo == 'E'){
    printf("Operação inválida!\n\n");
  } else {
    printf("%.4f\n\n", resultado);
  }
}

int close_connection(int sig)
{
  close(sockfd);
  printf("\nSocket encerrado!\n");
	exit(0);
}
