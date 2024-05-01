# xNucleo-IKS4A1_LSM6DSV16X_mbedOS_002-6D_Orientation

This application shows how to use LSM6DSV16X accelerometer to find out the 6D orientation and display data on a hyperterminal.

---
### Description:

This code demonstrates the use of the LSM6DSV16X sensor to detect 6D orientation changes (tilt) and display them using ASCII art representations of the sensor's orientation. It utilizes interrupts to detect orientation changes and prints the corresponding orientation using the serial interface.

### How it works:

- **Initialization**: 
  - It includes necessary libraries for communication (`mbed.h`) and for interfacing with the LSM6DSV16X sensor (`plt_iks4a1.h`).
  - It initializes objects for serial communication (`Serial`), the LSM6DSV16X sensor, and an LED for visual indication.

- **Interrupt Setup**:
  - It sets up an interrupt (`InterruptIn`) on pin D5 to detect 6D orientation events.

- **Main Loop**:
  - It continuously checks for 6D orientation events.
  - When an event occurs, it toggles an LED to indicate detection.
  - It reads the 6D orientation status from the sensor and sends the orientation over serial.
  - It prints the ASCII art representation of the detected orientation.

- **Interrupt Event Handler**:
  - When the interrupt occurs, it sets a flag to indicate an event.

- **Orientation Sending Function**:
  - It reads the orientation values from the sensor.
  - Based on the orientation, it generates an ASCII art representation.
  - It sends the representation over serial.

---

This description outlines the functionality of the code, including initialization, interrupt setup, main loop operation, and handling of 6D orientation events.