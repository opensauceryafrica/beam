import * as solana from './solana';
import { client } from './mqtt';

client.on('connect', () => {
  console.log('MQTT client connected');
});

client.on('error', (error) => {
  console.log('MQTT client error:', error);
});

void (async () => {
  const subscriptionId = await solana.listenForBalanceChange();
  console.log('Subscription ID:', subscriptionId);
})();
