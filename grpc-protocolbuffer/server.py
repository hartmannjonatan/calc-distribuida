from concurrent import futures
import grpc
import calculator_pb2
import calculator_pb2_grpc
import expr
import socket
import signal
import sys

class CalculatorServicer(calculator_pb2_grpc.CalculatorServicer):
    def Calculate(self, request, context):
        print(f"Request: {request}")
        result = None
        response = None
        try:
            result = expr.evaluate(request.op)
            response = calculator_pb2.Result(type="Success", result=float(result))
        except Exception as e:
            response = calculator_pb2.Result(type="Error", result=0)
        
        if response.type == "Success":
            print(response.result, "\n")
        else:
            print("Operação inválida!\n")

        return response

def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    calculator_pb2_grpc.add_CalculatorServicer_to_server(CalculatorServicer(), server)
    server.add_insecure_port('[::]:50051')
    server.start()

    def handle_sigint(sig, frame):
        print("\nServidor encerrado!")
        server.stop(grace=3)
        sys.exit(0)

    signal.signal(signal.SIGINT, handle_sigint)

    print(f"Servidor rodando em {get_local_ip()}:50051")
    server.wait_for_termination()

def get_local_ip():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        s.connect(('8.8.8.8', 80))
        ip = s.getsockname()[0]
    except Exception:
        ip = '127.0.0.1'
    finally:
        s.close()
    return ip

if __name__ == '__main__':
    serve()