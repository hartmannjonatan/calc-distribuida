#include <stdio.h>
#include <arpa/inet.h>
#include <math.h>
#include <signal.h>
#include <ifaddrs.h>
#include <zmq.h>
#include "dependencies/tinyexpr.h"

struct data_result{
  char tipo;
  double resultado;
};

#define MAX_OPERATION 128
const int PORTA = 5555;

struct data_result calc_operation(const char* data);
int close_connection(int sig);
char* get_ipv4();
void* context = 0;
void* responder = 0;

int main() {
    context = zmq_ctx_new();
    responder = zmq_socket(context, ZMQ_REP);
    char endpoint[64];
    snprintf(endpoint, sizeof(endpoint), "tcp://*:%d", PORTA);
    zmq_bind(responder, endpoint);

    signal(SIGINT, close_connection);
    char* ip_str = get_ipv4();
    printf("Servidor iniciado em %s:5555\n", ip_str);

    while (1) {
        char buffer[MAX_OPERATION] = {0};
        zmq_recv(responder, buffer, MAX_OPERATION-1, 0);

        printf("Recebido: %s\n", buffer);

        struct data_result resultado = calc_operation(buffer);
        zmq_send(responder, &resultado, sizeof(resultado), 0);
    }

    close_connection(0);
    return 0;
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
  zmq_close(responder);
  zmq_ctx_destroy(context);
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

