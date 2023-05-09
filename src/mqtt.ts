import mqtt from 'mqtt';
import env from './env';

export const client = mqtt.connect({
  host: env.MqttHost,
  port: env.MqttPort,
  protocol: env.MqttProtocol,
  username: env.MqttUsername,
  password: env.MqttPassword,
});
