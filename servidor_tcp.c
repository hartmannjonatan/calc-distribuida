#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <signal.h>
#include <ifaddrs.h>
#include "dependencies/tinyexpr.h"

struct data_result{
  char tipo;
  double resultado;
};

const int MAX_LEN_OPERATION = 100;
const int PORTA = 9734;

struct data_result calc_operation(const char* data);
int close_connection(int sig);
char* get_ipv4();
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
  server_address.sin_port = htons(PORTA);
  server_len = sizeof(server_address);
  bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
	listen(server_sockfd, 5);

  char* ip_str = get_ipv4();

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

char* get_ipv4() {
  static char ip_str[INET_ADDRSTRLEN] = "127.0.0.1"; // fallback

  struct ifaddrs *ifaddr, *ifa;
  if (getifaddrs(&ifaddr) == -1) {
      perror("getifaddrs");
      return ip_str;
  }

  for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
      if (ifa->ifa_addr == NULL) continue;

      if (ifa->ifa_addr->sa_family == AF_INET) {
          struct sockaddr_in *sa = (struct sockaddr_in *)ifa->ifa_addr;
          char temp[INET_ADDRSTRLEN];
          inet_ntop(AF_INET, &sa->sin_addr, temp, sizeof(temp));

          // Ignora 127.0.0.1 (loopback), pega o primeiro que não for
          if (strcmp(temp, "127.0.0.1") != 0) {
              strncpy(ip_str, temp, INET_ADDRSTRLEN);
              break;
          }
      }
  }

  freeifaddrs(ifaddr);
  return ip_str;
}
