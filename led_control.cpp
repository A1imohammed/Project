#include <iostream>
#include <unistd.h>
#include <string>
#include <MQTTClient.h>
#include "GPIO.h"

using namespace std;
using namespace exploringBB;

#define ADDRESS     "tcp://io.adafruit.com:1883"
#define CLIENTID    "ledcontrol"
#define TOPIC       "A1imohammed/feeds/syedfinalproject.ledcontrol"
#define QOS         1
#define TIMEOUT     10000L

const int LED_GPIO = 60;  

int main() {
	GPIO led(LED_GPIO);
	led.setDirection(OUTPUT);

	MQTTClient client;
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

	MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = true;
	conn_opts.username = "A1imohammed";
	conn_opts.password = "Enter AIO Pin";

	int rc = MQTTClient_connect(client, &conn_opts);
	if (rc != MQTTCLIENT_SUCCESS) {
		cerr << "Failed to connect to MQTT, code: " << rc << endl;
		return -1;
	}

	MQTTClient_subscribe(client, TOPIC, QOS);
	cout << "Subscribed to: " << TOPIC << endl;

	while (true) {
		MQTTClient_message* message = nullptr;
		char* topicName = nullptr;
		int topicLen;

		rc = MQTTClient_receive(client, &topicName, &topicLen, &message, TIMEOUT);
		if (rc == MQTTCLIENT_SUCCESS && message != nullptr) {
			string payload((char*)message->payload, message->payloadlen);
			cout << "Received: " << payload << endl;

		
			if (payload == "ON") {
				led.setValue(HIGH);
				cout << "LED ON" << endl;
			} else if (payload == "OFF") {
				led.setValue(LOW);
				cout << "LED OFF" << endl;
			}


			MQTTClient_freeMessage(&message);
			MQTTClient_free(topicName);
	}


	usleep(100000);  // 100ms polling
}


	MQTTClient_disconnect(client, 10000);
	MQTTClient_destroy(&client);
	return 0;
}

