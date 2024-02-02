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
  // await solana.airdrop();

  console.log('MQTT message received:', topic, message.toString());
  if (topic === 'get_balance') {
    const balance = await solana.walletBalance();
    await solana.sendSignalForBalanceChange(balance);
  }

  if (topic === 'balance_charge') {
    // charge the meter
    const energy = (parseFloat(message.toString()) / 1000) * (5 / 60 / 60); // convert 5 seconds to hours (5/60/60)
    if (isNaN(energy)) {
      console.log('No billing for non-numeric value');
      return;
    }

    if ((await solana.walletBalance()) < env.Fee * energy) {
      console.log('Insufficient funds to charge meter');
      const balance = 0.0;
      client.publish('balance_empty', balance.toString());
      return;
    }

    if (energy * env.Fee <= 0) {
      console.log('No billing for 0 kwh');
      return;
    }

    console.log(`Charging meter ${energy * env.Fee} for ${energy} kwh`);

    /*

      1 unit = 1 kwh = 1000 watts * 1 hour

      total units consumed over 5 seconds = (energy * 1 unit) / 1 kwh

      bill = total units consumed * fee (tarrif)
    */

    await solana.transfer(energy * env.Fee);
  }
});

express().listen(3000, async () => {
  const subscriptionId = await solana.listenForBalanceChange();
  console.log('listenForBalanceChange Subscription ID:', subscriptionId);
  console.log('Listening on port 3000');
});
