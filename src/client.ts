import { client } from './mqtt';

client.on('connect', () => {
  client.publish('balance_charge', '5');

  client.subscribe('balance_empty');
  client.subscribe('balance_low');
  client.subscribe('balance_change');

  console.log('MQTT client connected');
});

client.on('error', (error) => {
  console.log('MQTT client error:', error);
});

client.on('message', (topic, message) => {
  console.log('MQTT message received:', topic, message.toString());
});

console.log('Listening for MQTT messages');
