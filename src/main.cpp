#include <Arduino.h>
#include <Wire.h>

/* The I2C address of the module */
#define HMC5803L_Address 0x1E //Pointer to the registers addresses

/* Register address for the X Y and Z data */
#define X 3 //Data Output X MSB Register
#define Y 7 //Data Output Y MSB Register
#define Z 5 //Data Output Z MSB Register

int xmin, xmax, ymin, ymax, zmin, zmax;

/* This function will initialise the module and only needs to be run once
after the module is first powered up or reset */
void Init_HMC5803L(void)
{
/* Set the module to 8x averaging and 15Hz measurement rate */
Wire.beginTransmission(HMC5803L_Address); //How the I2C start communication
Wire.write(0x00); // A register address
Wire.write(0x70); // In bits: 0 11 100 00(01)

/* Set a gain of 5 */
Wire.write(0x01);// B register address
Wire.write(0xA0);// In bits 101 00000
Wire.endTransmission();
}

/* This function will read once from one of the 3 axis data registers
and return the 16 bit signed result. */
int HMC5803L_Read(byte Axis)
{
int Result;

/* Initiate a single measurement */
Wire.beginTransmission(HMC5803L_Address);
Wire.write(0x02); // Mode Register
Wire.write(0x00); // Single-measurement mode
Wire.endTransmission();
delay(6);

/* Move modules the resiger pointer to one of the axis data registers */
Wire.beginTransmission(HMC5803L_Address);
Wire.write(Axis);
Wire.endTransmission();

/* Read the data from registers (there are two 8 bit registers for each axis) */ 
Wire.requestFrom(HMC5803L_Address, 2);
Result = Wire.read() << 8;
Result |= Wire.read();

return Result;

}

int minmag(int Axisvalue)
{
    int axisvalue;
    int axismin = 99999;
    
    for(int i=10; i>=0; i--)
    {
        axisvalue = HMC5803L_Read(Axisvalue);

        if(axisvalue < axismin)
            axismin = axisvalue;

    }

    return axismin;
}

int maxmag(int Axisvalue)
{
    int axisvalue;
    int axismax = -99999;
    
    for(int i=10; i>=0; i--)
    {
        axisvalue = HMC5803L_Read(Axisvalue);

        if(axisvalue > axismax)
            axismax = axisvalue;
    }

    return axismax;
}

void calibratemag(int x, int y, int z)
{

    // Valores máximos e minimos de cada eixo por amostragem - Calibration
    // Deixar o sensor na posicao normalizada(defaut) durante esse periodo
    
    Serial.println("Gire o sensor no sentido negativo do eixo X, pronto? [s]");
    char confirmation = Serial.read();
    if(confirmation == 's') xmin = minmag(X);

    Serial.println("Gire o sensor no sentido positivo do eixo X, pronto? [s]");
    confirmation = Serial.read();
    if(confirmation == 's') xmax = maxmag(X);
    
    Serial.println("Gire o sensor no sentido negativo do eixo Y, pronto? [s]");
    confirmation = Serial.read();
    if(confirmation == 's') ymin = minmag(Y);
    
    Serial.println("Gire o sensor no sentido positivo do eixo Y, pronto? [s]");
    confirmation = Serial.read();
    if(confirmation == 's') ymax = maxmag(Y);
    
    Serial.println("Gire o sensor no sentido negativo do eixo Z, pronto? [s]");
    confirmation = Serial.read();
    if(confirmation == 's') zmin = minmag(Z);

    Serial.println("Gire o sensor no sentido positivo do eixo Z, pronto? [s]");
    confirmation = Serial.read();
    if(confirmation == 's') zmax = maxmag(Z);

}

void calibratemag1(int x, int y, int z)
{
    // Valores máximos e minimos de cada eixo por amostragem - Calibration
    // Deixar o sensor na posicao normalizada(defaut) durante esse periodo   
    xmin = minmag(X);
    xmax = maxmag(X);
        
    ymin = minmag(Y);   
    ymax = maxmag(Y);
        
    zmin = minmag(Z);
    zmax = maxmag(Z);
}

int xvalue, xinclination, yvalue, yinclination, zvalue;

void setup() 
{
Serial.begin(9600); 
Wire.begin();

/* Initialise the module */ 
Init_HMC5803L();

calibratemag1(X,Y,Z);
//calibratemag(X,Y,Z);



}

void loop() 
{
/* Read each sensor axis data and output to the serial port*/
Serial.print(HMC5803L_Read(X));
Serial.print(" ");
Serial.print(HMC5803L_Read(Y));
Serial.print(" ");
Serial.println(HMC5803L_Read(Z));


/*
xvalue = HMC5803L_Read(X);
xinclination = map(xvalue, xmin, xmax, 0, 100); //Escala de 0 a 100% que determina a inclinacao
Serial.print("X inclination:");
Serial.print(xvalue);
Serial.print(" ");
Serial.print(xinclination);

yvalue = HMC5803L_Read(Y);
yinclination = map(yvalue, ymin, ymax, 0, 100); //Escala de 0 a 100%
Serial.print("Y inclination:");
Serial.print(yvalue);
Serial.print(" ");
Serial.print(yinclination);
zvalue = HMC5803L_Read(Z);
*/
delay(100);
}

/* End of Code */