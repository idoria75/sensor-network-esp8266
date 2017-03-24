#include <Arduino.h>
#include <Thread.h>
#include <ThreadController.h>

#include "debug.h"
#include "connection.h"
#include "ota.h"

#define LED D4

void hardwareInit()
{
    Serial.begin(115200);
    debug("Starting Hardware.");
    pinMode(LED, OUTPUT);
}

Thread printDataThread = Thread();
Thread connectionThread = Thread();
ThreadController groupOfThreads = ThreadController();

void setup()
{
    hardwareInit();

    Data::self();
    printDataThread.onRun([]() {
        const static unsigned long startTime = micros();
        Data::self().data->freeHeap = ESP.getFreeHeap();
        Data::self().data->upTime = (micros() - startTime)/1e3;
        Data::self().print();
    });
    printDataThread.setInterval(1000);
    
    if(Connection::self().scanForNetwork("provant") == 1){
      Connection::self();
      Connection::self().setData(Data::self().data);
      Connection::self().addWifi("provant");
      debug("provant network found, set Slave Mode");
    }  
    else{
      debug("provant network not found, set Master Mode");
      const static char* apname = "provant";
      WiFi.softAP(apname); 
    }

    delay(5000);

    connectionThread.onRun([]() {
        if(Connection::self().run()) {
            digitalWrite(LED, !digitalRead(LED));
        }
    });
    connectionThread.setInterval(1000);

    groupOfThreads.add(&printDataThread);
    groupOfThreads.add(&connectionThread);

    Ota::self();
}

void loop()
{
    groupOfThreads.run();
    Ota::self().handle();
    yield();
}

//Ideas:
//Create function to handle each thread instead of having it all defined on setup