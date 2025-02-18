# Door Locker Security System

## Overview

The Door Locker Security System is a secure access control system implemented using an ATmega32 microcontroller. It requires a user to enter a password to unlock a door, providing a simple yet effective security mechanism.

## Project Structure

The project is modular and consists of several drivers, each serving a specific function:

- **LCD Driver**: Manages interactions with an LCD display, showing prompts and feedback messages.
- **Keypad Driver**: Handles user input through a matrix keypad.
- **UART Driver**: Facilitates serial communication between microcontrollers.
- **TWI (I2C) Driver**: Supports communication with EEPROM for storing passwords securely.
- **DC Motor Driver**: Controls the door locking and unlocking mechanism.
- **Timer Driver**: Manages system timing and delays.
- **External EEPROM Driver**: Stores persistent user credentials securely.
- **Buzzer Driver**: Alerts users with sound notifications for system status.
- **SPI Driver**: Enables serial communication between the microcontroller and other peripherals.
- **Interrupt Driver**: Handles external and internal interrupts for efficient event management.

## Control Unit

The Control Unit is responsible for processing input data and managing system logic. It handles:

- Communication with peripherals such as the LCD, keypad, and EEPROM.
- Authentication and password verification.
- Controlling the door locking mechanism based on the input received.

## HMI (Human-Machine Interface) unit

The HMI is the user-facing part of the system, allowing interaction via:

- **Keypad Input**: Users enter passwords and commands.
- **LCD Display**: Provides feedback, prompts, and system status updates.

## Notes

- The `.metadata` directory is only necessary for Eclipse IDE users and does not affect the functionality of the project.

## How It Works

1. **Compile the Project**:

   - Build the project in Eclipse by selecting **Project > Build Project**.
   - The compilation will generate a `.elf` file.

2. **Locate the ****************************************`.elf`**************************************** File**:

   - The `.elf` file is typically found in the `Debug/` or `Release/` folder inside the project directory.
   - If you don’t see it, refresh the project by right-clicking the project in Eclipse and selecting **Refresh**.
   - You can also check the **Console Output** in Eclipse to find the exact location.

3. **Load the ****************************************`.elf`**************************************** File onto the ATmega32**:

   - Use a simulation tool like **Proteus** or **AVR Studio**.
   - Upload the `.elf` file to the microcontroller in the simulation.
   - Run the simulation to test the system.

4. **Ensure Each Component Has Its Own ****************************************`.elf`**************************************** File**:

   - In case of errors while running the simulation, ensure that each ATmega32 microcontroller and the PIR motion sensor have their own `.elf` files.
   - The `.elf` files for ATmega32 can be found in the `Debug/` folder.
   - The `.elf` file for the PIR motion sensor is provided as an external file in the GitHub repository.

5. **Use the System**:

   - Enter a password using the keypad.
   - Observe the system’s behavior on the LCD and door mechanism.
   - Modify configurations or passwords as needed.

## Requirements to run

- Simulation software Proteus if using the already provided simulation on Github.
- Embedded C development environment (Eclipse, AVR-GCC, or similar)

## Contributing

Feel free to contribute by submitting issues or pull requests to improve the project.

