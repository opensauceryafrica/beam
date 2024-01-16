import env from './env';
import {
  Connection,
  PublicKey,
  LAMPORTS_PER_SOL,
  Transaction,
  SystemProgram,
  Keypair,
  sendAndConfirmTransaction,
} from '@solana/web3.js';
import { client } from './mqtt';
import bs58 from 'bs58';

const solanaConnection = new Connection(env.RPC, {
  wsEndpoint: env.WS,
});

export const listenForBalanceChange = async (): Promise<number> => {
  const wallet = new PublicKey(env.PublicKey);
  const subscriptionId = solanaConnection.onAccountChange(
    wallet,
    async (updatedAccountInfo) => {
      const balance = parseFloat(
        (updatedAccountInfo.lamports / LAMPORTS_PER_SOL).toFixed(3)
      );

      console.log(`New Account Balance:`, balance, ' SOL');

      await sendSignalForBalanceChange(balance);
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

export const walletBalance = async (): Promise<number> => {
  const wallet = new PublicKey(env.PublicKey);
  const balance = await solanaConnection.getBalance(wallet);
  return parseFloat((balance / LAMPORTS_PER_SOL).toFixed(3));
};

export const sendSignalForBalanceChange = async (
  balance: number
): Promise<void> => {
  if (balance <= 0.01) {
    // assume the balance as 0`
    balance = 0.0;
    client.publish('balance_empty', balance.toString());
  } else if (balance < 0.05) {
    client.publish('balance_low', balance.toString());
  } else {
    client.publish('balance_change', balance.toString());
  }
};

export const transfer = async (
  amount: number,
  destination: string = env.FeePublicKey
): Promise<void> => {
  try {
    const base = bs58.decode(env.PrivateKey);
    const secret = new Uint8Array(
      base.buffer,
      base.byteOffset,
      base.byteLength / Uint8Array.BYTES_PER_ELEMENT
    );

    const transaction = new Transaction().add(
      SystemProgram.transfer({
        fromPubkey: Keypair.fromSecretKey(new Uint8Array(secret)).publicKey,
        toPubkey: new PublicKey(destination),
        lamports: amount * LAMPORTS_PER_SOL,
      })
    );

    // Sign transaction, broadcast, and confirm
    await sendAndConfirmTransaction(solanaConnection, transaction, [
      Keypair.fromSecretKey(new Uint8Array(secret)),
    ]);
  } catch (error) {
    // console.log(`Transfer error ::`, error);
    await sendSignalForBalanceChange(await walletBalance());
    // return transfer((await walletBalance()) - 0.01);
  }
};
