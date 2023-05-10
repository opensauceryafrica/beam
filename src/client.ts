import { client } from './mqtt';

client.on('connect', () => {
  console.log('MQTT client connected');
});

client.on('error', (error) => {
  console.log('MQTT client error:', error);
});

client.on('message', (topic, message) => {
  console.log('MQTT message received:', topic, message.toString());
});

client.subscribe('balance_empty');
client.subscribe('balance_low');
client.subscribe('balance_change');
