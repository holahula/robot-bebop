int photocellPinA = 17;     // the cell and 10K pulldown are connected to a1
int photocellReadingA;     // the analog reading from the analog resistor divider

int photocellPinB = 16;     // the cell and 10K pulldown are connected to a2
int photocellReadingB;     // the analog reading from the analog resistor divider

int bigRead;
int difference = 0;
int value = 150;
int left =10;
int right=9;
int speed = 150;
int maximum = 160;

//Motor A
int enableA = 10;
int input1 = 12;
int input2 = 11;

//Motor B
int enableB = 9;
int input3 = 8;
int input4 = 13;

int greyscale=0;

//floats to hold colour arrays
float greyArray[]= {0};
float whiteArray[] = {0};
float blackArray[] = {0};

//place holder for average
int avgRead;
void setup() 
{
  pinMode(left, OUTPUT);
  pinMode(right, OUTPUT);
  pinMode(input1, OUTPUT);
  pinMode(input2, OUTPUT);
  pinMode(input3, OUTPUT);
  pinMode(input4, OUTPUT);
 Serial.begin(9600);
 setBalance();//sets the balance
}

void loop(void) {
  digitalWrite(input1, HIGH);
  digitalWrite(input2, LOW);
  analogWrite(enableA, 160);
  digitalWrite(input3, HIGH);
  digitalWrite(input4, LOW);
  analogWrite(enableB, 160);
  
  photocellReadingA = lefts(); // left
  Serial.print("Left Reading: ");
  Serial.println(photocellReadingA);
  photocellReadingB = rights(); // right
  Serial.print("Right Reading: ");
  Serial.println(photocellReadingB);

  Serial.print("Analog reading (A) = ");
  Serial.println(photocellReadingA);     // the raw analog reading

  Serial.print("Analog reading (B) = ");
  Serial.println(photocellReadingB);     // the raw analog reading
 
 // COMPARE READING A & B
 // white is (greyscale: 255)
 
  difference = (photocellReadingA - photocellReadingB);
  difference < 0 ? difference * -1 : difference * 1; 
  
  while (difference > value && speed <= maximum)
  {
    photocellReadingA > photocellReadingB ? bigRead = 1 : bigRead = 2;
    // if the greyscale > (choose value where is drifting into white)
    if (bigRead == 1)
    {
      //if (photocellReadingA > 200)
        // power right motor to balance (motor * 1.1) 
        analogWrite(right, speed + 5);
        analogWrite(left, speed - 5);
       // photocell if 
    } // bigread if 
    else 
    {
      //if (photocellReadingB > 200)
        // power left motor to balance (motor * 1.1)
        analogWrite(left, speed - 5);
        analogWrite(right, speed + 5); 
    } // else 
    difference = (photocellReadingA - photocellReadingB);
    difference < 0 ? difference * -1 : difference * 1; 
  } // while loop

// revert speed back to original
  analogWrite(left, speed);
  analogWrite(right, speed);
 
}
void setBalance(){
  //set white balance
  //scan the white sample.
  //go through each light, get a reading, set the base reading for each colour red, green, and blue to the white array
    getReadingLeft(5);          //number is the number of scans to take for average, this whole function is redundant, one reading works just as well.
    whiteArray[0] = avgRead;
    delay(100);
  //done scanning white, now it will pulse blue to tell you that it is time for the black (or grey) sample.
  //set black balance
  delay(5000);              //wait for five seconds so we can position our black sample 
  //go ahead and scan, sets the colour values for red, green, and blue when exposed to black
    getReadingLeft(5);
    blackArray[0] = avgRead;
    //blackArray[i] = analogRead(2);
    delay(100);
  //set boolean value so we know that balance is set
  delay(5000);     //delay another 5 seconds to let us catch up
}
int rights(){
    getReadingRight(5);                  //take a reading however many times
    greyArray[0] = avgRead;        //set the current colour in the array to the average reading
    float greyDiff = whiteArray[0] - blackArray[0];                    //the highest possible return minus the lowest returns the area for values in between
    greyArray[0] = (greyArray[0] - blackArray[0])/(greyDiff)*255; //the reading returned minus the lowest value divided by the possible range multiplied by 255 will give us a value roughly between 0-255 representing the value for the current reflectivity(for the colour it is exposed to) of what is being scanned
  return greyArray[0];
}
int lefts(){
    getReadingLeft(5);                  //take a reading however many times
    greyArray[0] = avgRead;        //set the current colour in the array to the average reading
    float greyDiff = whiteArray[0] - blackArray[0];                    //the highest possible return minus the lowest returns the area for values in between
    greyArray[0] = (greyArray[0] - blackArray[0])/(greyDiff)*255; //the reading returned minus the lowest value divided by the possible range multiplied by 255 will give us a value roughly between 0-255 representing the value for the current reflectivity(for the colour it is exposed to) of what is being scanned
  return greyArray[0];
}
void getReadingRight(int times){
  int reading;
  int tally=0;
  //take the reading however many times was requested and add them up
  for(int i = 0;i < times;i++){
    reading = analogRead(4);
    tally = reading + tally;
    delay(10);
  }
}
  void getReadingLeft(int times){
  int reading;
  int tally=0;
  //take the reading however many times was requested and add them up
  for(int i = 0;i < times;i++){
    reading = analogRead(2);
    tally = reading + tally;
    delay(10);
  }
  //calculate the average and set it
  avgRead = (tally)/times;
}
 


