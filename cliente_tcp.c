#include <sys/types.h>
#include <sys/socket.h>
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
const int PORTA = 9734;

int close_connection(int sig);

int main()
{
  int len;
  struct sockaddr_in address;
  int result;
  char operation[MAX_LEN_OPERATION];
  char server_ip[100];
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  
  // Solicita o IP do servidor ao usuário
  printf("Digite o endereço IP do servidor: ");
  fgets(server_ip, sizeof(server_ip), stdin);
  server_ip[strcspn(server_ip, "\n")] = 0; // Remove o '\n' do final

  // Define os dados do servidor
  address.sin_family = AF_INET;
  address.sin_port = htons(PORTA);
  inet_pton(AF_INET, server_ip, &address.sin_addr); // Converte string IP para binário

  len = sizeof(address);
  result = connect(sockfd, (struct sockaddr *)&address, len);
  if(result == -1) {
    perror("Não foi possível conectar com o servidor de operações!");
    exit(1);
  }

  signal(SIGINT, close_connection);
  printf("Conexão iniciada!\n");
  
  while (1) {
    struct data_result resultado;
    printf(">> ");
    fgets(operation, sizeof(operation), stdin);
    operation[strcspn(operation, "\n")] = '\0'; // remove '\n' se existir

    int op_len = strlen(operation) + 1; // inclui \0
    write(sockfd, operation, op_len);

    ssize_t bytes_read = read(sockfd, &resultado, sizeof(struct data_result));
    if (bytes_read == 0) {
        printf("Servidor fechou a conexão.\n");
        break;
    } else if (bytes_read < 0) {
        perror("Erro ao ler do servidor");
        break;
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
  printf("\nConexão encerrada!\n");
	exit(0);
}