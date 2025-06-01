JAVAC=javac
JAVA=java
JAR=dependencies/exp4j-0.4.8.jar
CLASSPATH=.:$(JAR)

# Compila e executa o servidor
server: Server.class
	@echo "Iniciando o servidor..."
	$(JAVA) -cp $(CLASSPATH) Server

# Compila e executa o cliente
client: Client.class
	@echo "Iniciando o cliente..."
	$(JAVA) -cp $(CLASSPATH) Client

# Compila todos os .java necessários
Server.class: Server.java Calculator.java CalculatorInterface.java
	$(JAVAC) -cp $(CLASSPATH) Server.java Calculator.java CalculatorInterface.java

Client.class: Client.java CalculatorInterface.java
	$(JAVAC) -cp $(CLASSPATH) Client.java CalculatorInterface.java

# Limpa os arquivos compilados
clean:
	rm -f *.class