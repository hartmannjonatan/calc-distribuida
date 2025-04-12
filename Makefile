CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lm  # Linka a biblioteca matemática

# Alvo para compilar o servidor
server_tcp: servidor_tcp.c dependencies/tinyexpr.o
	$(CC) $(CFLAGS) -o servidor servidor_tcp.c dependencies/tinyexpr.o $(LDFLAGS)

# Alvo para compilar o cliente
client_tcp: cliente_tcp.c dependencies/tinyexpr.o
	$(CC) $(CFLAGS) -o cliente cliente_tcp.c dependencies/tinyexpr.o $(LDFLAGS)

# Alvo para limpar os arquivos temporários
clean:
	rm -f servidor client dependencies/tinyexpr.o
