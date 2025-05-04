#include <iostream>
#include <unistd.h>
#include <string>
#include <MQTTClient.h>
#include "derek_LED.h"

using namespace std;

#define ADDRESS     "tcp://io.adafruit.com:1883"
#define CLIENTID    "blinkcount"
#define TOPIC       "A1imohammed/feeds/syedfinalproject.blinkcount"  
#define QOS         1
#define TIMEOUT     10000L

int main() {
	LED usr3(3);
	usr3.removeTrigger();  

	MQTTClient client;
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;


	MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);			       
       	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = true;
	conn_opts.username = "A1imohammed";  
	conn_opts.password = "aio_TbKn39tHtCa2jBrJ0Idv4h3U47P7";   


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
	       
			if (payload.length() == 1 && isdigit(payload[0])) {
				int count = payload[0] - '0'; 																			    
				for (int i = 0; i < count; ++i) {
					usr3.turnOn();
					usleep(250000);

					usr3.turnOff();
					usleep(250000); 
				}
			}

		
		       	MQTTClient_freeMessage(&message);
	
	       		MQTTClient_free(topicName);
		}

		usleep(100000); 
	}

	MQTTClient_disconnect(client, 10000);
	MQTTClient_destroy(&client);
	return 0;
}																			    
