# 🔐 Arduino Smart Lock System

![Project Banner](https://via.placeholder.com/800x200?text=Arduino+Smart+Lock+System)  
*Secure access control with RFID and keypad*

## 🚀 Features
- **🪪 RFID Authentication** - Register up to 4 cards
- **⌨️ Keypad Entry** - 4-digit PIN (Default: `1234`)
- **🔓 Servo-Controlled Lock** - Smooth door mechanism
- **📱 User Interface** 
  - 16x2 LCD for feedback
  - Buzzer for audio alerts
- **⚙️ Admin Functions**
  - Change password (`99 + old + new`)
  - Add new cards (`88 + PIN`)
  - Factory reset (`00 + PIN`)

## 📦 Hardware Components
| Component | Quantity | Connection |
|-----------|----------|------------|
| Arduino Uno | 1 | - |
| MFRC522 RFID | 1 | SS:10, RST:9 |
| Servo Motor | 1 | A0 |
| 4x3 Keypad | 1 | Rows:8-5, Cols:4-2 |
| I2C LCD | 1 | SDA:A4, SCL:A5 |
| Buzzer | 1 | A1 |
| Push Button | 1 | A2 |

## 🔧 Installation
```bash
# Clone repository
git clone https://github.com/yourusername/arduino-smart-lock.git

# Required libraries:
1. Servo.h
2. MFRC522.h
3. Keypad.h
4. LiquidCrystal_I2C.h
5. EEPROM.h
🛠️ Wiring Diagram
plaintext
        ARDUINO UNO
       +------------+
RFID   | 10(SS),9   |
SERVO  | A0         |
BUZZER | A1         |
BUTTON | A2         |
LCD    | A4(SDA),A5 |
KEYPAD | Rows:8-5   |
       | Cols:4-2   |
       +------------+
💡 Usage Instructions
Normal Operation:
Scan registered RFID card → Unlocks door

Press ** + enter PIN → Alternative unlock

Press ## → Lock door immediately

Admin Mode:
Combination	Function
99 + Old + New PIN	Change password
88 + PIN + Scan card	Register new card
00 + PIN	Factory reset
📜 License
MIT License © 2023 - See LICENSE for details.
