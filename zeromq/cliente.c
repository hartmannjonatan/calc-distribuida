#include <zmq.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

struct data_result{
  char tipo;
  double resultado;
};
#define MAX_OPERATION 128
const int PORTA = 5555;

int close_connection(int sig);
void process_resultado(char tipo, double resultado);
void* context = 0;
void* requester = 0;

int main(int argc, char* argv[]) {
    char server_ip[100];
    
    // Solicita o IP do servidor ao usuário
    printf("Digite o endereço IP do servidor: ");
    fgets(server_ip, sizeof(server_ip), stdin);
    server_ip[strcspn(server_ip, "\n")] = 0; // Remove o '\n' do final

    context = zmq_ctx_new();
    requester = zmq_socket(context, ZMQ_REQ);
    char operation[MAX_OPERATION];

    // Monta o endereço tcp://ip:5555
    char endpoint[256];
    snprintf(endpoint, sizeof(endpoint), "tcp://%s:%d", server_ip, PORTA);

    zmq_connect(requester, endpoint);
    signal(SIGINT, close_connection);
    printf("Conexão iniciada!\n");

    int timeout_ms = 2000;
    zmq_setsockopt(requester, ZMQ_RCVTIMEO, &timeout_ms, sizeof(timeout_ms));
    
    while (1) {
      struct data_result resultado;
      printf(">> ");
      fgets(operation, sizeof(operation), stdin);
      operation[strcspn(operation, "\n")] = '\0'; // remove '\n' se existir

      int op_len = strlen(operation) + 1; // inclui \0
      zmq_send(requester, operation, op_len, 0);

      int bytes_read = zmq_recv(requester, &resultado, sizeof(resultado), 0);
      if (bytes_read == -1) {
          if (errno == EAGAIN) {
              printf("Timeout ao esperar resposta do servidor.\n");
              close_connection(-10);
          } else {
              perror("Erro ao receber do servidor");
              close_connection(-10);
          }
      } else if (bytes_read == 0) {
          printf("Zero bytes recebidos - possível problema na resposta do servidor.\n");
          close_connection(-10);
      }

      process_resultado(resultado.tipo, resultado.resultado);
    }
    
    close_connection(0);
    return 0;
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
  printf("\nConexão encerrada!\n");
  if(sig != -10){
    zmq_close(requester);
    zmq_ctx_destroy(context);
  }
	exit(0);
}
