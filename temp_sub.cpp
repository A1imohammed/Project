#include <iostream>
#include <sstream>
#include <unistd.h>
#include <MQTTClient.h>
#include "derek_LED.h"

#define ADDRESS     "tcp://io.adafruit.com:1883"
#define CLIENTID    "bbb-temp-sub-client"
#define TOPIC       "A1imohammed/feeds/syedfinalproject.temperaturesensor"
#define QOS         1
#define TIMEOUT     10000L

using namespace std;

const float THRESHOLD = 24.0;
LED usr0(0);  // USR0 LED

int messageArrived(void* context, char* topicName, int topicLen, MQTTClient_message* message) {
	string payload((char*)message->payload, message->payloadlen);
	float temperature;
	stringstream(payload) >> temperature;

	cout << "Received temperature: " << temperature << "°C" << endl;
	
	if (temperature < THRESHOLD) {
		cout << "Below threshold — LED ON" << endl;
		usr0.turnOn();
	} else {
		cout << "Above threshold — LED OFF" << endl;
		usr0.turnOff();
	 }

	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);
	return 1;
}

int main() {
	MQTTClient client;
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
	int rc;

	MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	conn_opts.username = "A1imohammed";
	conn_opts.password = "Enter AIO Pin";


	MQTTClient_setCallbacks(client, NULL, NULL, messageArrived, NULL);


	if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
		cerr << "Failed to connect, return code: " << rc << endl;
		return -1;
	}

	MQTTClient_subscribe(client, TOPIC, QOS);
	cout << "Subscribed to: " << TOPIC << endl;

	while (true) {
		sleep(1);
	}
	MQTTClient_disconnect(client, 10000);
	MQTTClient_destroy(&client);
	return 0;
}
