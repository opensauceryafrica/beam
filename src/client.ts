import { config } from 'dotenv';
config();
import env from './env';
import mqtt from 'mqtt';

const client = mqtt.connect({
  host: env.MqttHost,
  port: env.MqttPort,
  protocol: env.MqttProtocol,
  username: env.MqttUsername,
  password: env.MqttPassword,
});

client.on('connect', () => {
  console.log('MQTT client connected');
});

client.on('error', (error) => {
  console.log('MQTT client error:', error);
});

client.on('message', (topic, message) => {
  console.log('MQTT message received:', topic, message.toString());
});

client.subscribe('balance_change');
