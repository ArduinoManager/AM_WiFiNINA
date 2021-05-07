/*
   Test Arduino Manager for iPad / iPhone / Mac

   A simple test program for the Gyroscope Widget

   Author: Fabrizio Boco - fabboco@gmail.com

   Version: 1.0

   05/07/2021

   All rights reserved

*/

/*

   AMController libraries, example sketches (The Software) and the related documentation (The Documentation) are supplied to you
   by the Author in consideration of your agreement to the following terms, and your use or installation of The Software and the use of The Documentation
   constitutes acceptance of these terms.
   If you do not agree with these terms, please do not use or install The Software.
   The Author grants you a personal, non-exclusive license, under authors copyrights in this original software, to use The Software.
   Except as expressly stated in this notice, no other rights or licenses, express or implied, are granted by the Author, including but not limited to any
   patent rights that may be infringed by your derivative works or by other works in which The Software may be incorporated.
   The Software and the Documentation are provided by the Author on an AS IS basis.  THE AUTHOR MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT
   LIMITATION THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, REGARDING THE SOFTWARE OR ITS USE AND OPERATION
   ALONE OR IN COMBINATION WITH YOUR PRODUCTS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR CONSEQUENTIAL DAMAGES (INCLUDING,
   BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE,
   REPRODUCTION AND MODIFICATION OF THE SOFTWARE AND OR OF THE DOCUMENTATION, HOWEVER CAUSED AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
   STRICT LIABILITY OR OTHERWISE, EVEN IF THE AUTHOR HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include <Arduino_LSM6DS3.h>
#include <AM_WiFiNINA.h>

/******************

  Instructions

  1) Sketch - Include Library - Manage Libraries
  2) Search for Arduino_LSM6DS3 and click Install
  3) Search for Madgwick, select the library provided by Arduino and click Install
  4) Configure ssid and password below as per your network
  5) Open Arduino Manager
  6) Add a Gyroscope Widget and call it Gyro
  7) Connect to Arduino Nano and you are good to go

  For more details access the available documentation from Arduino Manager or at this link:


  https://sites.google.com/site/fabboco/home/arduino-manager-for-iphone-ipad


******************/

/*

   WIFI Library configuration

*/
IPAddress ip(192, 168, 1, 233);
IPAddress dns(8, 8, 8, 8);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

//char ssid[]    = "myssid";           // your network SSID (name)
//char pass[]    = "mypassword";  // your network password

char ssid[]    = "homewirelessnetwork";      // your network SSID (name)
char pass[]    = "Fabrizio22Camilla22Laura26!";  // your network password


int status = WL_IDLE_STATUS;

WiFiServer server(80);

AMController amController(&server, &doWork, &doSync, &processIncomingMessages, &processOutgoingMessages, &deviceConnected, &deviceDisconnected);

/*
   Acceleration around axis. It's proveded in [G] (Earth gravity). 1g is 9.81 m/s^2
*/
float gyroX;
float gyroY;
float gyroZ;


void setup() {
  Serial.begin(9600);

  Serial.println("Start");


  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");

    while (1);
  }

  // attempt to connect to Wifi network

  WiFi.config(ip, dns, gateway, subnet);

  while (status != WL_CONNECTED) {

    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network
    status = WiFi.begin(ssid, pass);

    // wait 2 seconds for connection:
    delay(2000);
  }

  // print your WiFi shields IP address

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Ready");
}

/**
   Standard loop function
*/
void loop() {
  //amController.loop();
  amController.loop(20);
}

/**
  This function is called periodically and its equivalent to the standard loop() function
*/
void doWork() {
  if (IMU.accelerationAvailable()) {
    IMU.readGyroscope(gyroX, gyroY, gyroZ);
  }
}

/**


  This function is called when the ios device connects and needs to initialize the position of switches and knobs

*/
void doSync (char *variable) {
}


/**
  This function is called when a new message is received from the iOS device
*/
void processIncomingMessages(char *variable, char *value) {
}

/**
  This function is called periodically and messages can be sent to the iOS device
*/
void processOutgoingMessages() {

  amController.writeTripleMessage("Gyro", gyroX / 1000, gyroY / 1000, gyroZ / 1000);

  Serial.print(gyroX);
  Serial.print('\t');
  Serial.print(gyroY);
  Serial.print('\t');
  Serial.println(gyroZ);
}

/**
  This function is called when the iOS device connects
*/
void deviceConnected () {

  Serial.println("Device connected");
}

/**
  This function is called when the iOS device disconnects
*/
void deviceDisconnected () {

  Serial.println("Device disconnected");
}