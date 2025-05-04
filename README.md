# RFID-Based Access Control System

- This project implements an RFID-based access control system using a microcontroller, designed for secure authentication. 
- The system uses an RFID reader to scan RFID tags and makes access decisions based on pre-programmed valid IDs.
- It features a modular embedded C codebase and is developed in Keil µVision IDE for 8051 or compatible microcontrollers.

---

## 🔧 Features

- Detects and reads RFID tags via UART
- Displays tag ID and access status on an LCD
- Optional keypad support for password entry
- Modular code: separate modules for I2C, LCD, keypad, delays, etc.
- Real-time processing with interrupt and polling support
- Easy to extend with EEPROM or buzzer modules

---

## ⚙️ Working Principle

1. The system initializes the RFID reader, LCD, and keypad.
2. When an RFID tag is brought near the reader, the tag's unique ID is read.
3. The microcontroller compares the scanned ID against a stored list.
4. If the ID matches, "Access Granted" is shown on the LCD. Otherwise, it displays "Access Denied".
5. (Optional) A buzzer or relay can be triggered for door unlocking or alert.

---

## 🧰 Hardware Requirements

| Component           | Description                                 |
|---------------------|---------------------------------------------|
| Microcontroller     | 8051 (AT89C51/AT89S52 or similar)           |
| RFID Module         | 125kHz or 13.56MHz (RC522, EM-18, etc.)     |
| LCD Display         | 16x2 Character LCD                          |
| Keypad (optional)   | 4x4 Matrix Keypad                           |
| Power Supply        | 5V Regulated                                |
| Resistors, Capacitors, Crystal Oscillator, etc. |

---

## 📐 Circuit Diagram Overview

- **RFID TX** connected to microcontroller RX pin (UART)
- **LCD** connected via parallel data lines (D4–D7) and control pins (RS, EN)
- **Keypad** connected to digital GPIO pins
- **Power** supplied to all components from 5V regulator

--- 


---

## 🚀 Getting Started

### Step 1: Open Project
- Load `Project_Main.uvproj` into Keil µVision.

### Step 2: Hardware Setup
- Connect RFID reader's TX pin to RX pin of microcontroller.
- Connect LCD and keypad as per the code pin configuration.
- Ensure stable 5V power supply to all components.

### Step 3: Compile & Upload
- Build the project in Keil.
- Upload HEX file using a programmer (e.g., USBASP, ISP).

### Step 4: Test
- Scan a valid RFID card. LCD should display "Access Granted".
- Scan an unknown tag. LCD should display "Access Denied".

---

## 📶 RFID Module Integration

- Most low-frequency RFID modules (like EM-18) use UART for data transmission.
- RFID tag UID is transmitted serially as ASCII characters.
- Baud rate is usually 9600 bps.

