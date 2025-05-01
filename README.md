Smart Lock System

This is an Arduino-based Smart Lock System designed to secure doors using RFID cards, a keypad, and a password. It features a servo motor for locking/unlocking, an LCD for user feedback, and a buzzer for audio notifications.

Features





RFID Card Access: Unlock the system by scanning a registered RFID card with the MFRC522 reader.



Keypad Password Entry: Enter a 4-digit password (default: 1234) to unlock the system.



Lock/Unlock: Lock the system manually using the keypad or a button.



Change Password: Update the password via the keypad.



Add New RFID Card: Register new RFID cards (up to 4 cards).



Reset System: Reset all saved cards and passwords.



User Feedback: LCD displays messages, and the buzzer provides audio feedback.

Hardware Requirements





Arduino Uno



Servo Motor (connected to A0)



Buzzer (A1)



Button (A2)



4x3 Keypad (pins 8, 7, 6, 5 for rows; 4, 3, 2 for columns)



MFRC522 RFID Reader (SS: 10, RST: 9, SPI pins: 11, 12, 13)



16x2 I2C LCD (SDA: A4, SCL: A5)

Connection Diagram

Servo Motor:





Signal: Arduino A0



VCC: Arduino 5V



GND: Arduino GND

Buzzer:





Positive: Arduino A1



Negative: Arduino GND

Button:





One end: Arduino A2



Other end: Arduino GND (with a pull-down resistor, e.g., 10k ohm)

4x3 Keypad:





Rows: Arduino Pins 8, 7, 6, 5



Columns: Arduino Pins 4, 3, 2

MFRC522 RFID Reader:





SS (SDA): Arduino Pin 10



RST: Arduino Pin 9



MOSI: Arduino Pin 11



MISO: Arduino Pin 12



SCK: Arduino Pin 13



VCC: Arduino 3.3V



GND: Arduino GND

I2C 16x2 LCD:





SDA: Arduino A4



SCL: Arduino A5



VCC: Arduino 5V



GND: Arduino GND

Usage Instructions





Unlock with RFID Card: Scan a registered card. If valid, the lock opens, LCD shows "Lock opened," and the buzzer beeps once.



Unlock with Password: Press "**" on the keypad, enter the 4-digit password. If correct, the lock opens.



Lock the System: Press "##" to lock. LCD shows "Lock closed."



Change Password: Press "99," enter the old password, then a new one. LCD confirms with "Password changed."



Add New Card: Press "88," enter the password, then scan a new card. LCD shows "New card saved."



Reset System: Press "00," enter the password to reset all data.



Manual Unlock/Lock: Press the button to toggle the lock state.

Installation





Clone the repository:

git clone https://github.com/yourusername/smart-lock-system.git



Open the smart_lock_code.ino file in the Arduino IDE.



Install the required libraries: Servo, Wire, LiquidCrystal_I2C, EEPROM, SPI, MFRC522, Keypad.



Upload the code to your Arduino Uno.



Connect the components as per the connection diagram.

License

This project is licensed under the MIT License - see the LICENSE file for details.
