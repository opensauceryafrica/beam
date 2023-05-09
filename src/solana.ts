import env from './env';
import { Connection, PublicKey, LAMPORTS_PER_SOL } from '@solana/web3.js';

const solanaConnection = new Connection(env.RPC, {
  wsEndpoint: env.WS,
});

const sleep = (ms: number) => {
  return new Promise((resolve) => setTimeout(resolve, ms));
};

(async () => {
  const wallet = new PublicKey(env.PublicKey);
  const subscriptionId = await solanaConnection.onAccountChange(
    wallet,
    (updatedAccountInfo) =>
      console.log(
        `---Event Notification for ${wallet.toString()}--- \nNew Account Balance:`,
        updatedAccountInfo.lamports / LAMPORTS_PER_SOL,
        ' SOL'
      ),
    'confirmed'
  );
  console.log('Starting web socket, subscription ID: ', subscriptionId);
  await sleep(10000); //Wait 10 seconds for Socket Testing
  await solanaConnection.requestAirdrop(wallet, LAMPORTS_PER_SOL);
})();
