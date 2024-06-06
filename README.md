# BIPO: Your Desk Companion Robot

This repository contains the Arduino code for BIPO, a delightful desk companion robot. BIPO is designed to interact with its environment by scanning for objects, remembering their locations, and expressing its "emotions" through a series of servo-controlled gestures.

<img width="521" alt="BIPO-Poster" src="https://github.com/Fahad-Rashid1100/BIPO-Robot/assets/123345898/1991e9d9-8b4a-46db-b968-b90c9d67384c">

## Features

- **Object Detection:** Using an ultrasonic sensor, BIPO constantly scans its surroundings for objects within a certain range.
- **Spatial Mapping:** BIPO builds and maintains an internal map of its environment, storing the angular positions of detected objects.
- **Object Persistence Awareness:** BIPO periodically checks its map to see if previously detected objects are still present.
- **Emotional Responses:**
    - **Happy Gesture:** Performed when BIPO confirms the presence of a previously mapped object.
    - **Mad Gesture:** Expressed when BIPO fails to find an object it expects to be in a specific location.
- **Dynamic Movement Patterns:** BIPO incorporates a variety of servo movements, including small rotations, full sweeps, and random position targeting. 

## Hardware Components

- Arduino board (Arduino UNO)
- Ultrasonic Sensor (HC-SR04 or similar)
- 3x Servo Motors
- Jumper Wires
- Power Source (e.g., battery pack or Arduino's USB power)
- (Optional) Chassis or structural components to house the electronics

## Wiring Diagram

<img width="641" alt="BIPO-Circuit-Diagram" src="https://github.com/Fahad-Rashid1100/BIPO-Robot/assets/123345898/66d4f999-d94f-4072-a91c-dc3c5ae0222b">

## Getting Started

1. **Clone the repository:** 
   ```bash
   git clone https://github.com/Fahad-Rashid1100/BIPO-Robot.git
   ```
2. **Hardware Assembly:**
    - Connect the ultrasonic sensor, servos, and other components to the Arduino board according to the wiring diagram.
    - Securely mount the components to a chassis or base.
3. **Upload the Code:**
    - Open the `BIPO.ino` file (this file) in the Arduino IDE.
    - Select your Arduino board and port from the Tools menu.
    - Click the upload button to flash the code to your Arduino.

## Code Explanation

- **`coords` Struct:** Stores the minimum and maximum angular positions of a detected object.
- **`Map` Class:** Manages the linked list of `coords`, providing functions for insertion, searching, arrangement, and removal of object location data.
- **`calculateDistance()`:** Reads data from the ultrasonic sensor and calculates the distance to an object.
- **`detectObject()`:** Checks for the presence of an object within a specified distance and determines if it's a new or previously mapped object. 
- **`moveServosTo()` and `moveServoTo()`:** Functions for controlling the movement of the servos to specific angles.
- **`handleObjectDetection()`:** Records the location of a new object or reacts based on the persistence of previously mapped objects. 
- **`loop()`:** The main program loop that governs BIPO's overall behavior, including movement patterns, object detection, and emotional responses. 

## Customization

- **Movement Variations:** Experiment with different movement patterns in the `switch` statement within the `loop()` function to personalize BIPO's behavior.
- **Gesture Design:** Modify the `performHappyGesture()` and `performMadGesture()` functions to create unique servo movements that represent BIPO's emotions. 
- **Detection Range:** Adjust the distance threshold in the `detectObject()` function to control BIPO's sensitivity to nearby objects. 

## Future Enhancements

- **Obstacle Avoidance:** Integrate additional sensors (e.g., infrared or time-of-flight) to enable BIPO to navigate around obstacles. 
- **Sound Effects:** Add a speaker or buzzer to accompany BIPO's gestures with sound effects.
- **Remote Control:** Implement Bluetooth or Wi-Fi control for remote interaction with BIPO.
- **Camera:** Add a Camera and allow it to detect actual objects and make it more interactive.

## Contributing

Contributions are welcome! If you have any ideas, bug fixes, or feature suggestions, feel free to open an issue or submit a pull request.

## License

This project is licensed under the [MIT License](LICENSE).
