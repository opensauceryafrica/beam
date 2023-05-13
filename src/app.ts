import * as solana from './solana';
import { client } from './mqtt';
import express from 'express';

client.on('connect', async () => {
  console.log('MQTT client connected');
});

client.on('error', (error) => {
  console.log('MQTT client error:', error);
});

void (async () => {
  const subscriptionId = await solana.listenForBalanceChange();
  console.log('Subscription ID:', subscriptionId);
})();

client.on('message', async (topic, message) => {
  console.log('MQTT message received:', topic, message.toString());
  if (topic === 'get_balance') {
    const balance = await solana.walletBalance();
    await solana.sendSignalForBalanceChange(balance);
  }
});

client.subscribe('get_balance');

express().listen(3000, () => {
  console.log('Listening on port 3000');
});
