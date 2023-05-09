import env from './env';
import { Connection, PublicKey, LAMPORTS_PER_SOL } from '@solana/web3.js';
import { client } from './mqtt';

const solanaConnection = new Connection(env.RPC, {
  wsEndpoint: env.WS,
});

export const listenForBalanceChange = async (): Promise<number> => {
  const wallet = new PublicKey(env.PublicKey);
  const subscriptionId = solanaConnection.onAccountChange(
    wallet,
    (updatedAccountInfo) => {
      console.log('updatedAccountInfo', updatedAccountInfo);
      console.log(
        `---Event Notification for ${wallet.toString()}--- \nNew Account Balance:`,
        updatedAccountInfo.lamports / LAMPORTS_PER_SOL,
        ' SOL'
      );

      client.publish('balance_change', 'Hello mqtt');
    },
    'confirmed'
  );
  return subscriptionId;
};

export const airdrop = async (): Promise<void> => {
  await solanaConnection.requestAirdrop(
    new PublicKey(env.PublicKey),
    LAMPORTS_PER_SOL
  );
};
