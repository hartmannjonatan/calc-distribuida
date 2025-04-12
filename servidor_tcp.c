#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <signal.h>
#include "dependencies/tinyexpr.h"

struct data_result{
  char tipo;
  double resultado;
};

const int MAX_LEN_OPERATION = 100;

struct data_result calc_operation(const char* data);
int close_connection(int sig);
int server_sockfd;

int main()
{
  int client_sockfd;
  int server_len, client_len;
  struct sockaddr_in server_address;
  struct sockaddr_in client_address;

  server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(9734);
  server_len = sizeof(server_address);
  bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
	listen(server_sockfd, 5);

  struct sockaddr_in local_address;
  socklen_t local_len = sizeof(local_address);
  getsockname(server_sockfd, (struct sockaddr *)&local_address, &local_len);

  char ip_str[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &local_address.sin_addr, ip_str, sizeof(ip_str));

  printf("Servidor iniciado em %s\n", ip_str);

  signal(SIGINT, close_connection);
  printf("Server waiting operations\n");
	while (1) {
    client_len = sizeof(client_address);
    client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_address.sin_addr, client_ip, sizeof(client_ip));
    printf("Cliente %s:%d conectado\n", client_ip, ntohs(client_address.sin_port));

    while (1) {
        char operation[MAX_LEN_OPERATION];
        ssize_t bytes_read = read(client_sockfd, &operation, sizeof(char) * MAX_LEN_OPERATION);
        
        if (bytes_read <= 0) {
            printf("Cliente %s:%d desconectado (erro ou encerramento)\n", client_ip, ntohs(client_address.sin_port));
            break;
        }

        if (strcmp(operation, "close") == 0){
            printf("Cliente %s:%d solicitou encerramento\n", client_ip, ntohs(client_address.sin_port));
            break;
        }

        struct data_result resultado = calc_operation(operation);
        write(client_sockfd, &resultado, sizeof(struct data_result));
    }

    close(client_sockfd);
    printf("Conexão com %s:%d encerrada\n", client_ip, ntohs(client_address.sin_port));
    printf("\nServer waiting operations\n");
  }
}

struct data_result calc_operation(const char* data)
{
  printf("Processando %s\n", data);
  struct data_result resultado;
  resultado.tipo = 'E';
  resultado.resultado = NAN;

  int erro;
  double resultado_operacao = te_interp(data, &erro);

  if (erro == 0 && !isnan(resultado_operacao)) {
    resultado.tipo = 'F';
    resultado.resultado = resultado_operacao;
    printf("Resultado: %.2f\n", resultado.resultado);
  } else{
    printf("Operação inválida: retornando %c\n", resultado.tipo);
  }

  return resultado;
}

int close_connection(int sig){
  close(server_sockfd);
  printf("\nServidor encerrado.\n");
  exit(0);
}
