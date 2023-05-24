import { config } from 'dotenv';
config();
interface IENV {
  Port: string | number;
  RPC: string;
  WS: string;
  MqttHost: string;
  MqttPort: number;
  MqttProtocol: 'mqtt' | 'mqtts';
  MqttUsername: string;
  MqttPassword: string;
  MqttClientId?: string;
  PublicKey: string;
  PrivateKey: string;
  FeePublicKey: string;
  Fee: number;
}

var env: IENV = {
  Port: process.env.PORT || 3000,
  RPC: process.env.RPC_URL || 'https://api.devnet.solana.com',
  WS: process.env.WS_URL || 'wss://api.devnet.solana.com',
  MqttHost: process.env.MQTT_HOST || '',
  MqttPort: Number(process.env.MQTT_PORT) || 8883,
  MqttProtocol: (process.env.MQTT_PROTOCOL as '') || 'mqtts',
  MqttUsername: process.env.MQTT_USERNAME || 'opensaucerer',
  MqttPassword: process.env.MQTT_PASSWORD || '<your_password>',
  PublicKey: process.env.PUBLIC_KEY || '',
  MqttClientId: process.env.MQTT_CLIENT_ID || 'mqttjs_34f95deb',
  PrivateKey: process.env.PRIVATE_KEY || '',
  FeePublicKey: process.env.FEE_PUBLIC_KEY || '',
  Fee: Number(process.env.FEE) || 0.001,
};

export default env;
