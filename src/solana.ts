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
      console.log(
        `Event Notification for ${wallet.toString()} --- New Account Balance:`,
        updatedAccountInfo.lamports / LAMPORTS_PER_SOL,
        ' SOL'
      );

      const balance = updatedAccountInfo.lamports / LAMPORTS_PER_SOL;

      if (balance === 0) {
        client.publish(
          'balance_empty',
          `Your meter balance is empty: ${balance} SOL`
        );
      } else if (balance < 0.05) {
        client.publish(
          'balance_low',
          `Your meter balance is low: ${balance} SOL`
        );
      } else {
        client.publish(
          'balance_change',
          `Your meter balance ok: ${balance} SOL`
        );
      }
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
