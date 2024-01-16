import * as solana from './solana';
import { client } from './mqtt';
import express from 'express';
import env from './env';

client.on('connect', async () => {
  client.subscribe('balance_charge');
  client.subscribe('get_balance');

  console.log('MQTT client connected');
});

client.on('error', (error) => {
  console.log('MQTT client error:', error);
});

client.on('message', async (topic, message) => {
  console.log('MQTT message received:', topic, message.toString());
  if (topic === 'get_balance') {
    const balance = await solana.walletBalance();
    await solana.sendSignalForBalanceChange(balance);
  }

  if (topic === 'balance_charge') {
    // charge the meter
    const timerun = parseInt(message.toString());

    if (isNaN(timerun)) {
      console.log('No billing for non-numeric value');
      return;
    }

    if ((await solana.walletBalance()) < env.Fee * timerun) {
      console.log('Insufficient funds to charge meter');
      return;
    }

    if (timerun * env.Fee <= 0) {
      console.log('No billing for 0 seconds');
      return;
    }

    console.log(`Charging meter for ${timerun} seconds`);
    await solana.transfer(timerun * env.Fee);
  }
});

express().listen(3000, async () => {
  const subscriptionId = await solana.listenForBalanceChange();
  console.log('listenForBalanceChange Subscription ID:', subscriptionId);
  console.log('Listening on port 3000');
});
