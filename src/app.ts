import * as solana from './solana';
import { client } from './mqtt';
import express from 'express';

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

express().listen(3000, () => {
  console.log('Listening on port 3000');
});
