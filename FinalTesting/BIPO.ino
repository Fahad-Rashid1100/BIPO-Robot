/*
 * Copyright (c) 2024 Fahad Rashid (Fahad-Rashid1100)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <Servo.h>

// -=+= ULTRASONIC SENSOR VARIABLES =+=-
const int TrigPin = 3;
const int EchoPin = 5;

// -=+= SERVO VARIABLES =+=-
Servo myservo;
Servo neckServo;
Servo faceServo;
int direction = 0;

const int servoMoveTime = 250;       // Time for each movement
unsigned long lastMovementTime = 0;  // Track last movement time

struct coords {
  // float distance;
  int minBaseAngle;
  int maxBaseAngle;
  coords *next;

  coords()
    : minBaseAngle(0), maxBaseAngle(0), next(NULL) {}
  coords(int minAngle, int maxAngle)
    : minBaseAngle(minAngle), maxBaseAngle(maxAngle), next(NULL) {}

  bool isEmpty() {
    if(minBaseAngle == 0 && maxBaseAngle == 0) {
      return true;
    }
    return false;
  }
};

class Map {
public:
  coords *head;

  Map() : head(NULL) {}

  void insert(int minAngle, int maxAngle) {
    coords *c = new coords(minAngle, maxAngle);

    if (head == NULL) {
      head = c;  // If the list is empty, the new node becomes the head
    } else {
      coords *current = head;

      while (current->next != NULL) {  // Traverse to the last node
        current = current->next;
      }

      current->next = c;  // Append the new node to the end
    }
  }


    bool check(int baseAngle) {
    if (head != NULL) {
      coords *temp = head;

      while (temp != NULL) {  // Changed condition
        if ((baseAngle >= temp->minBaseAngle - 7 && baseAngle <= temp->maxBaseAngle + 7)) {
          return true;
        } else {
          temp = temp->next;
        }
      }
    }
    return false;
  }

  coords* search(int baseAngle) {
    coords* current = head;

    while (current != NULL) {  // Changed condition
      if (baseAngle >= current->minBaseAngle - 7 && baseAngle <= current->maxBaseAngle + 7) {
        // Found a matching node
        return current;  // Return the pointer to the matching node
      } else {
        current = current->next;
      }
    }

    return NULL;  // No matching node found
  }

  coords* getRandomNode() {
    coords* current = head;
    int count = 0;

    // First, count the number of nodes
    while (current != NULL) {
        count++;
        current = current->next;
    }

    // Generate a random index
    // randomSeed(analogRead(0));
    int randomIndex = random(count);

    // Traverse to the random node
    current = head;
    for (int i = 0; i < randomIndex; i++) {
        current = current->next;
    }

    // Return the random node
    return current;
  }

  
  void arrangeNodes() {
    // First, sort the nodes in increasing order of their minAngle
    coords* current;
    coords* index;
    int tempMin, tempMax;

    for(current = head; current->next != NULL; current = current->next) {
        for(index = current->next; index != NULL; index = index->next) {
            if(current->minBaseAngle > index->minBaseAngle) {
                // Swap the minBaseAngle and maxBaseAngle of current and index
                tempMin = current->minBaseAngle;
                tempMax = current->maxBaseAngle;
                current->minBaseAngle = index->minBaseAngle;
                current->maxBaseAngle = index->maxBaseAngle;
                index->minBaseAngle = tempMin;
                index->maxBaseAngle = tempMax;
            }
        }
    }

    // Then, combine the close nodes
    current = head;
    while (current != NULL && current->next != NULL) {
        if (abs(current->maxBaseAngle - current->next->minBaseAngle) <= 30) {
            // If the nodes are close to each other, combine them
            current->maxBaseAngle = current->next->maxBaseAngle;

            // Delete the next node
            coords* temp = current->next;
            current->next = current->next->next;
            delete temp;
        } else {
            // Move to the next node
            current = current->next;
        }
    }
  }


  void remove(int baseAngle) {
    coords *current = head;
    coords *previous = NULL;
    

    while (current != NULL) {
      if (baseAngle >= current->minBaseAngle && baseAngle <= current->maxBaseAngle) {
        Serial.print("Removed Coords");
        // Found the node to remove

        coords *nextNode = current->next;  // Store the next node

        if (previous == NULL) { // Removing the head node
          head = nextNode;
        } else {
          previous->next = nextNode;
        }

        delete current;

        // Continue searching from the next node
        current = nextNode;
      } else {
        previous = current;
        current = current->next;
      }
    }
  }

  ~Map() {
    coords *current = head;
    while (current != NULL) {
      coords *nextNode = current->next;
      delete current;
      current = nextNode;
    }
  }

};


Map ThreeDMap;


// -=+= FUNCTIONS =+=-

float calculateDistance() {
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(15);
  digitalWrite(TrigPin, LOW);

  long duration = pulseIn(EchoPin, HIGH);
  float distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.println(distance);

  return distance;
}

int changeDetects = 0;
unsigned long searchStartTime = 0;

bool detectObject() {
  float distance = calculateDistance();

  if (distance < 25) {



    bool isInMap = ThreeDMap.check(myservo.read());

    if(!isInMap) {

      tone(11, 475);
      delay(100);
      noTone(11);
      Serial.print("temp empty...");
      return true;
    }
    else {

      // while(calculateDistance() < 25) {
      //   myservo.write(myservo.read() + direction);
      //   delay(15);
      // }

      return false;

      // if(temp.distance <= calculateDistance() + 5 && temp.distance >= calculateDistance() - 5) {
      //   Serial.print("Distance same");
      //   return false;
      // }
      // else {
      //   tone(11, 475);
      //   delay(100);
      //   noTone(11);
      //   return false;
      // }
    }
  }
  else {
    // coords temp = ThreeDMap.check(distance, myservo.read(), neckServo.read());
    // if (!temp.isEmpty()) {
    //   if (temp.distance < distance) { // Distance change detected
    //     Serial.print("Distance Changed");

    //     // Perform searching movements for 2 attempts
    //     if(!myservo.read() + 30 >= 180) {
    //       moveServosTo(myservo, myservo.read() + 30 , neckServo, 140);
    //       moveServosTo(myservo, myservo.read() - 30 , neckServo, 160);
    //     }
          
    //       // Check for object re-detection during each search attempt
    //       if (calculateDistance() < 25) {
    //         // Object detected again, continue normal behavior
    //         return true;
    //       }
        

    //     // Object not found after 2 attempts, remove coordinate
    //     ThreeDMap.remove(distance, myservo.read(), neckServo.read());
        return false;
      }
    }
  

void moveServosTo(Servo &servo1, int targetAngle1, Servo &servo2, int targetAngle2) {
  int currentAngle1 = servo1.read();
  int currentAngle2 = servo2.read();
  int increment1 = 1, increment2 = 1;
  direction = 1;

  // Determine movement directions
  if (targetAngle1 < currentAngle1) {
    increment1 = -1;  // Decrement for decreasing angle
  }
  if (targetAngle2 < currentAngle2) {
    increment2 = -1;  // Decrement for decreasing angle
    direction = -1;
  }

  // Loop until both servos reach their targets
  while (currentAngle1 != targetAngle1 || currentAngle2 != targetAngle2) {
    // Move only the servos that haven't reached their targets
    if (currentAngle1 != targetAngle1) {
      currentAngle1 = constrain(currentAngle1 + increment1, 0, 180);  // Ensure angle stays within 0-180
      servo1.write(currentAngle1);
    }
    if (currentAngle2 != targetAngle2) {
      currentAngle2 = constrain(currentAngle2 + increment2, 0, 180);  // Ensure angle stays within 0-180
      servo2.write(currentAngle2);
    }
    if (detectObject()) {
            handleObjectDetection();
          }
    delay(15);  // Adjust delay for smoother movement
  }
}

void moveServoTo(Servo &servo, int targetAngle) {
  int currentAngle = servo.read();
  int increment = 1;

  // Determine movement direction
  if (targetAngle < currentAngle) {
    increment = -1;
  }

  // Loop until the servo reaches its target
  while (currentAngle != targetAngle) {
    currentAngle = constrain(currentAngle + increment, 0, 180);
    servo.write(currentAngle);
    delay(15);  // Adjust delay for smoother movement
  }
}

void performHappyGesture() {
    Serial.println("HAPPPYYYYY");
    neckServo.write(160);
    delay(300);
    neckServo.write(120);
    delay(300);
    neckServo.write(160);
    delay(300);
    neckServo.write(120);
    delay(300);
    faceServo.write(170);
    delay(300);
    faceServo.write(130);
    delay(300);
    faceServo.write(150);
    delay(1000);
}

void performMadGesture(coords* randomNode) {
    Serial.println("MAAADDD GHRRRR!!! WHERE'S MY OBJECT?!!!");
    neckServo.write(150);
    myservo.write(30);
    delay(700);
    myservo.write(150);
    delay(700);
    myservo.write(randomNode->maxBaseAngle - randomNode->minBaseAngle);
    delay(1000);
    moveServoTo(neckServo, 170);
    moveServoTo(myservo, 180);
    moveServoTo(neckServo, 140);
    ThreeDMap.remove(randomNode->maxBaseAngle - randomNode->minBaseAngle);
}

void scanAndReact() {
    int sum = 0;
    int count = 0;
    int avgDistance = 0;
    coords* randomNode = ThreeDMap.getRandomNode();

    if(randomNode != NULL) {
        myservo.write(randomNode->minBaseAngle);
        for(int i = randomNode->minBaseAngle; i <= randomNode->maxBaseAngle; i++) {
            myservo.write(i);
            sum += calculateDistance();
            count++;
            delay(15);
        }
        delay(500);
        moveServoTo(myservo, randomNode->maxBaseAngle - randomNode->minBaseAngle);
        delay(1000);

        avgDistance = sum / count;
        Serial.println(avgDistance);
        if(avgDistance < 25) {
            performHappyGesture();
        } else {
            performMadGesture(randomNode);
        }
    }
}



// -=+= MAIN PROGRAM =+=-
void setup() {
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  Serial.begin(9600);
  myservo.attach(7);
  neckServo.attach(8);
  faceServo.attach(9);
  randomSeed(analogRead(0));
}

void loop() {
    moveServoTo(neckServo, 150);
    faceServo.write(150);
    delay(500);

    if (millis() - lastMovementTime >= 1000) {
        switch (random(14)) {
            case 1: {
                direction = 1;
                for (int i = 0; i <= 30; i++) {
                    myservo.write(myservo.read() + i);
                    if (detectObject()) {
                        handleObjectDetection();
                    }
                }
                lastMovementTime = millis();
                break;
            }
            case 2:
            case 4:
            case 7:
                moveServosTo(neckServo, random(120, 160), myservo, random(0, 180));
                break;
            case 3: {
                direction = -1;
                for (int i = myservo.read(); i >= 0; i--) {
                    myservo.write(i);
                    delay(15);
                    if (detectObject()) {
                        handleObjectDetection();
                    }
                }
                delay(servoMoveTime);
                direction = 1;
                for (int angle = 0; angle <= 90; angle++) {
                    myservo.write(angle);
                    delay(15);
                    if (detectObject()) {
                        handleObjectDetection();
                    }
                }
                moveServoTo(neckServo, random(120, 160));
                delay(500);
                for (int angle = 90; angle <= 180; angle++) {
                    myservo.write(angle);
                    delay(15);
                    if (detectObject()) {
                        handleObjectDetection();
                    }
                }
                moveServoTo(neckServo, random(120, 160));
                delay(servoMoveTime);
                break;
            }
            case 5:
            case 8:
            case 10:
            case 11:
            case 12:
                scanAndReact();
                break;
            case 6: {
                direction = -1;
                for (int i = myservo.read(); i >= 0; i--) {
                    myservo.write(i);
                    delay(15);
                    if (detectObject()) {
                        handleObjectDetection();
                    }
                }
                moveServoTo(neckServo, random(120, 160));
                direction = 1;
                for (int angle = 0; angle <= 90; angle++) {
                    myservo.write(angle);
                    delay(15);
                    if (detectObject()) {
                        handleObjectDetection();
                        break;
                    }
                }
                delay(500);
                if (detectObject()) {
                    handleObjectDetection();
                }
                break;
            }
            case 9: {
                direction = -1;
                for (int angle = myservo.read(); angle >= 90; angle--) {
                    myservo.write(angle);
                    if (detectObject()) {
                        handleObjectDetection();
                    }
                    delay(15);
                }
                break;
            }
            case 13:
                moveServosTo(neckServo, 150, myservo, 90);
                faceServo.write(150);
                break;
            default:
                break;
        }
    }
}

void handleObjectDetection() {
  Serial.println("OBJECT DETECTED");

  int j = 0;
  int minAngle = 0;
  int maxAngle = 0;

  if(direction == 1) {
    // for(int i = 0 ; i <= 30 ; i++) {
    //   myservo.write(myservo.read() + 1);
    //   delay(45);
    // }
    minAngle = myservo.read();
    while(calculateDistance() < 25) {
      myservo.write(myservo.read() + 1);
      j++;

    }
    maxAngle = myservo.read();
    for(int i = 0 ; i <= j/2 ; i++) {
      myservo.write(myservo.read() - 1);
      delay(45);
    }
    ThreeDMap.insert(minAngle, maxAngle);
    ThreeDMap.arrangeNodes();
    
  }
  else if (direction = -1) {
    // for(int i = 0 ; i <= 30 ; i++) {
    //   myservo.write(myservo.read() - 1);
    //   delay(45);
    // }
    maxAngle = myservo.read();
    while(calculateDistance() < 25) {
      myservo.write(myservo.read() - 1);
      j++;
    }
    minAngle = myservo.read();
    for(int i = 0 ; i <= j/2 ; i++) {
      myservo.write(myservo.read() + 1);
      delay(45);
    }
    ThreeDMap.insert(minAngle, maxAngle);
    ThreeDMap.arrangeNodes();
    
  }
  
  faceServo.write(180);

  delay(1000);
  faceServo.write(110);

  delay(1000);  // Adjust delay as needed
  faceServo.write(150);

  delay(1000);
}
