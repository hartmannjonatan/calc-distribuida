import grpc
import calculator_pb2
import calculator_pb2_grpc

def run():
    try:
        server_ip = input("Digite o ip do servidor: ")
        channel = grpc.insecure_channel(f'{server_ip}:50051')
        stub = calculator_pb2_grpc.CalculatorStub(channel)
    except Exception:
        print("Não foi possível conectar!")
        return

    print("Conectado com servidor! Digite a operação a ser calculada (exit para sair).")

    while True:
        op = input(">> ")
        if op == "exit":
            print("Conexão encerrada")
            break
        try:
            response = stub.Calculate(calculator_pb2.Operation(op=op))
            if response.type == "Success":
                print(response.result, "\n")
            else:
                print("Operação inválida!\n")
        except Exception as e:
            print("Erro interno!")
            break

if __name__ == '__main__':
    run()
