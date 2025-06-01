CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lm  # Linka a biblioteca matemática

# Alvo para compilar o servidor TCP
server_tcp: servidor_tcp.c dependencies/tinyexpr.o
	$(CC) $(CFLAGS) -o servidor servidor_tcp.c dependencies/tinyexpr.o $(LDFLAGS)

# Alvo para compilar o cliente TCP
client_tcp: cliente_tcp.c dependencies/tinyexpr.o
	$(CC) $(CFLAGS) -o cliente cliente_tcp.c dependencies/tinyexpr.o $(LDFLAGS)

# Alvo para compilar o servidor UDP
server_udp: servidor_udp.c dependencies/tinyexpr.o
	$(CC) $(CFLAGS) -o servidor servidor_udp.c dependencies/tinyexpr.o $(LDFLAGS)

# Alvo para compilar o cliente UDP
client_udp: cliente_udp.c dependencies/tinyexpr.o
	$(CC) $(CFLAGS) -o cliente cliente_udp.c dependencies/tinyexpr.o $(LDFLAGS)

# Alvo para limpar os arquivos temporários
clean:
	rm -rf servidor cliente dependencies/tinyexpr.o build/
