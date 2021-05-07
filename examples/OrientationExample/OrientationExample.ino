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
#include <MadgwickAHRS.h>
#include <AM_WiFiNINA.h>

/******************

 Instructions 

 1) Sketch - Include Library - Manage Libraries
 2) Search for Arduino_LSM6DS3 and click Install
 3) Search for Madgwick, select the library provided by Arduino and click Install
 4) Configure ssid and password below as per your network
 5) Open Arduino Manager
 6) Add a Gyroscope Widget and call it Orientation
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

char ssid[]    = "myssid";      // your network SSID (name)
char pass[]    = "mypassword";  // your network password

int status = WL_IDLE_STATUS;

WiFiServer server(80);

/**

   Other initializations

*/

// initialize a Madgwick filter:
Madgwick filter;
// sensor's sample rate is fixed at 104 Hz:
const float sensorRate = 104.00;
// values for orientation:
float roll, pitch, yaw;

AMController amController(&server, &doWork, &doSync, &processIncomingMessages, &processOutgoingMessages, &deviceConnected, &deviceDisconnected);

void setup() {
  Serial.begin(9600);
  // attempt to start the IMU:
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU");
    // stop here if you can't access the IMU:
    while (true);
  }
  // start the filter to run at the sample rate:
  filter.begin(sensorRate);

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

  Serial.println("Starting....");
  delay(1500);
  Serial.println("Ready");
}

void loop() {
  amController.loop(1);
}

void doWork() {
  // values for acceleration & rotation:
  float xAcc, yAcc, zAcc;
  float xGyro, yGyro, zGyro;

  // check if the IMU is ready to read:
  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
    // read accelerometer & gyrometer:
    IMU.readAcceleration(xAcc, yAcc, zAcc);
    IMU.readGyroscope(xGyro, yGyro, zGyro);

    // update the filter, which computes orientation:
    filter.updateIMU(xGyro, yGyro, zGyro, xAcc, yAcc, zAcc);

    // print the heading, pitch and roll
    roll = filter.getRoll();
    pitch = filter.getPitch();
    yaw = filter.getYaw();
    //    Serial.print("Orientation: ");
    //    Serial.print(yaw);
    //    Serial.print(" ");
    //    Serial.print(pitch);
    //    Serial.print(" ");
    //    Serial.println(roll);
  }

  delay(10);
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
  amController.writeMessage("KeepAlive",1);
  amController.writeTripleMessage("Orientation",roll*PI/180,yaw*PI/180,-pitch*PI/180);
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