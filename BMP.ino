// Your sketch must #include this library, and the Wire library.
// (Wire is a standard library included with Arduino.):

#include <SFE_BMP180.h>
#include <Wire.h>



int i=0;
int z=0;
double a11=0, a12=0, a13=0, a14=0, a15=0, a1t=0;
double a21=0, a22=0, a23=0, a24=0, a25=0, a2t=0;

bool abierto=false;

// You will need to create an SFE_BMP180 object, here called "pressure":

SFE_BMP180 pressure;

double baseline; // baseline pressure

void setup()
{
  Serial.begin(9600);
  Serial.println("REBOOT");
  pinMode(9,OUTPUT);//PIN del mosfet
  digitalWrite(9, HIGH);

  // Initialize the sensor (it is important to get calibration values stored on the device).

  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    Serial.println("BMP180 init fail (disconnected?)\n\n");
    while(1); // Pause forever.
  }

  // Get the baseline pressure:
  
  baseline = getPressure();
  
  Serial.print("baseline pressure: ");
  Serial.print(baseline);
  Serial.println(" mb");  
}

void loop()
{
  
  
   z++;

    if(z>100){
    
          while(abierto==false){
                  double a,P;
                  
                  // Get a new pressure reading:
                
                  P = getPressure();
                
                  // Show the relative altitude difference between
                  // the new reading and the baseline reading:
                
                  a = pressure.altitude(P,baseline);
                  
                  Serial.print("altitud relativa: ");
                  if (a >= 0.0) Serial.print(" "); // add a space for positive numbers
                  Serial.print(a,2);
                  Serial.print(" metros, ");
                   
                  Serial.print(a*3.28084,0);
                  Serial.println(" pies");
                  
                  i++;
                  
                  
                    if(i==1)a11=a;
                    if(i==2)a12=a;
                    if(i==3)a13=a;
                    if(i==4)a14=a;
                    if(i==5)a15=a;
            
                  
                  if((i>=1)&&(i<=5)&&(a11!=0)&&(a12!=0)&&(a13!=0)&&(a14!=0)&&(a15!=0))//Comprovar primera medició alçada
                  {
                  a1t=((a11+a12+a13+a14+a15)/5);
                  Serial.print(" ###### ");
                  Serial.print(a1t,2);
                  Serial.print(" ###### ");
                  }
                  
                  
                  
                    
                    if(i==31)a21=a;
                    if(i==32)a22=a;
                    if(i==33)a23=a;
                    if(i==34)a24=a;
                    if(i==35)a25=a;
                    
                  
                   if((i>=31)&&(i<=35)&&(a21!=0)&&(a22!=0)&&(a23!=0)&&(a24!=0)&&(a25!=0))
                   {
                  a2t=((a21+a22+a23+a24+a25)/5); 
                  Serial.print(" ///// ");
                  Serial.print(a2t,2);
                  Serial.print(" ///// ");
                   }
                 
      
                  if(a2t>a1t)
                  {
                    abierto=false;
                    i=0;
                    a2t=0;
                  }
                  else if((a2t<a1t)&&(a2t<90)&&(a2t!=0))
                  {
                  
                    digitalWrite(9, LOW);
                    Serial.print("Tendría que encenderse el LED");
                    abierto=true;
                  }
                 
            
                  delay(5);
                  
        }
 }
 delay(5);
}
            
      
          
          
          
                      double getPressure()
                      {
                        char status;
                        double T,P,p0,a;
                      
                        // You must first get a temperature measurement to perform a pressure reading.
                        
                        // Start a temperature measurement:
                        // If request is successful, the number of ms to wait is returned.
                        // If request is unsuccessful, 0 is returned.
                      
                        status = pressure.startTemperature();
                        if (status != 0)
                        {
                          // Wait for the measurement to complete:
                      
                          delay(status);
                      
                          // Retrieve the completed temperature measurement:
                          // Note that the measurement is stored in the variable T.
                          // Use '&T' to provide the address of T to the function.
                          // Function returns 1 if successful, 0 if failure.
                      
                          status = pressure.getTemperature(T);
                          if (status != 0)
                          {
                            // Start a pressure measurement:
                            // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
                            // If request is successful, the number of ms to wait is returned.
                            // If request is unsuccessful, 0 is returned.
                      
                            status = pressure.startPressure(3);
                            if (status != 0)
                            {
                              // Wait for the measurement to complete:
                              delay(status);
                      
                              // Retrieve the completed pressure measurement:
                              // Note that the measurement is stored in the variable P.
                              // Use '&P' to provide the address of P.
                              // Note also that the function requires the previous temperature measurement (T).
                              // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
                              // Function returns 1 if successful, 0 if failure.
                      
                              status = pressure.getPressure(P,T);
                              if (status != 0)
                              {
                                return(P);
                              }
                              else Serial.println("error retrieving pressure measurement\n");
                            }
                            else Serial.println("error starting pressure measurement\n");
                          }
                          else Serial.println("error retrieving temperature measurement\n");
                        }
                        else Serial.println("error starting temperature measurement\n");
                      }
  


/* SFE_BMP180 altitude example sketch

This sketch shows how to use the Bosch BMP180 pressure sensor
as an altimiter.
https://www.sparkfun.com/products/11824

Like most pressure sensors, the BMP180 measures absolute pressure.
Since absolute pressure varies with altitude, you can use the pressure
to determine your altitude.

Because pressure also varies with weather, you must first take a pressure
reading at a known baseline altitude. Then you can measure variations
from that pressure

Hardware connections:

- (GND) to GND
+ (VDD) to 3.3V

(WARNING: do not connect + to 5V or the sensor will be damaged!)

You will also need to connect the I2C pins (SCL and SDA) to your
Arduino. The pins are different on different Arduinos:

Any Arduino pins labeled:  SDA  SCL
Uno, Redboard, Pro:        A4   A5
Mega2560, Due:             20   21
Leonardo:                   2    3

Leave the IO (VDDIO) pin unconnected. This pin is for connecting
the BMP180 to systems with lower logic levels such as 1.8V

Have fun! -Your friends at SparkFun.

The SFE_BMP180 library uses floating-point equations developed by the
Weather Station Data Logger project: http://wmrx00.sourceforge.net/

Our example code uses the "beerware" license. You can do anything
you like with this code. No really, anything. If you find it useful,
buy me a beer someday.

V10 Mike Grusin, SparkFun Electronics 10/24/2013
*/


