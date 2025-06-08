const express = require('express');
const math = require('mathjs');
const cors = require('cors');
const os = require('os');

const app = express();
const PORT = 8000;

process.on('uncaughtException', (err) => {
  console.error('Erro não capturado:', err);
});

// Função para obter o IPv4 local da máquina
function getLocalIp() {
  const interfaces = os.networkInterfaces();
  for (let name in interfaces) {
    for (let iface of interfaces[name]) {
      if (iface.family === 'IPv4' && !iface.internal) {
        return iface.address;
      }
    }
  }
  return '127.0.0.1';
}

app.use(express.json());
app.use(cors()); // Habilita CORS para qualquer origem

app.post('/calculate', (req, res) => {
  const { operation } = req.body;
  console.log(operation)

  if (!operation || typeof operation !== 'string') {
    return res.status(400).json({ type: 'Error', result: 0, message: 'Invalid input' });
  }

  try {
    const result = math.evaluate(operation);
    console.log(result)
    res.json({ type: 'Success', result });
  } catch (error) {
    res.status(400).json({ type: 'Error', result: 0, message: error.message });
  }
});

const localIp = getLocalIp();
app.listen(PORT, '0.0.0.0', () => {
  console.log(`API disponível em: http://${localIp}:${PORT}/calculate`);
});
