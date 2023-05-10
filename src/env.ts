import { config } from 'dotenv';
config();
interface IENV {
  Port: string | number;
  RPC: string;
  WS: string;
  MqttHost: string;
  MqttPort: number;
  MqttProtocol: 'mqtts' | 'mqtt';
  MqttUsername: string;
  MqttPassword: string;
  PublicKey: string;
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
};

export default env;
