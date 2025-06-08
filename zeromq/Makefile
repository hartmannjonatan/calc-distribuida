CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lm -lzmq  # Linka a biblioteca matemática e ZeroMQ

install: check-libzmq
	@echo "Dependências instaladas."

check-libzmq:
	@dpkg -s libzmq3-dev >/dev/null 2>&1 || { \
		echo "libzmq3-dev não está instalado. Instalando..."; \
		sudo apt update && sudo apt install -y libzmq3-dev; \
	}

server: install servidor.c dependencies/tinyexpr.o
	$(CC) $(CFLAGS) -o servidor servidor.c dependencies/tinyexpr.o $(LDFLAGS)

dependencies/tinyexpr.o: dependencies/tinyexpr.c dependencies/tinyexpr.h
	$(CC) $(CFLAGS) -c dependencies/tinyexpr.c -o dependencies/tinyexpr.o

client: install cliente.c
	$(CC) $(CFLAGS) -o cliente cliente.c $(LDFLAGS)

clean:
	rm -rf servidor cliente dependencies/tinyexpr.o