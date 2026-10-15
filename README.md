# Room Climate Station

An Arduino-based climate monitor for my game room. It reads temperature and humidity, shows them on an LCD, changes an RGB "mood light" color based on how the room feels, and automatically runs a fan when it gets too warm, using hysteresis so the fan doesn't flicker on and off.

This was my first electronics project, built from scratch with an Elegoo starter kit.

![Finished build](images/Climate-temp.jpg)

---

## Features

- **Live readings**: temperature (°F) and humidity on a 16x2 LCD and the Serial Monitor
- **Status messages**: "Getting Warm!", "Getting Cold!", or "Comfy ol vibe!"
- **RGB mood light**: red = warm, blue = cold, green = comfortable, cyan = sensor error
- **Automatic fan with hysteresis**: turns on above one temperature and only turns off once the room drops below a lower one (for example, on above 80°F and off below 78°F), so it doesn't rapidly switch on and off near the threshold
- **Error handling**: if the sensor fails, the screen says so, the light turns cyan, and the fan shuts off

---

## Parts

| Part | Purpose |
|---|---|
| Elegoo Uno R3 | Microcontroller |
| DHT11 sensor module | Temperature and humidity |
| LCD1602 (16x2, parallel) | Display |
| RGB LED (common cathode) + 3x 220Ω resistors | Mood light |
| Small DC motor + fan blade | Fan |
| NPN transistor (PN2222) + 1kΩ resistor | Lets the Arduino switch the motor |
| Diode | Protects against the motor's voltage spike when it turns off |
| 1kΩ resistor | LCD contrast (used instead of a potentiometer) |
| 220Ω resistor | LCD backlight |
| Breadboard power module + 9V battery | Separate power for the fan |
| Breadboard + jumper wires | Connections |

---

## Wiring

| Component | Pin | Arduino |
|---|---|---|
| DHT11 | S (signal) | D2 |
| LCD | RS | D7 |
| LCD | E | D8 |
| LCD | D4 / D5 / D6 / D7 | D9 / D10 / D11 / D12 |
| LCD | V0 | GND through 1kΩ |
| LCD | RW, VSS, K | GND |
| LCD | VDD | 5V |
| LCD | A (backlight) | 5V through 220Ω |
| RGB LED | Red / Green / Blue | D6 / D5 / D3 (each through 220Ω) |
| RGB LED | Common (long leg) | GND |
| Fan transistor | Base | D4 through 1kΩ |
| Fan transistor | Emitter | GND |
| Fan transistor | Collector | Motor (−) |
| Motor | (+) | 5V from the power module |
| Diode | Across the motor | Stripe toward + |

The Arduino and the fan run on separate power sources but share a common ground.

---

## Libraries

- **DHT sensor library** by Adafruit (plus **Adafruit Unified Sensor**)
- **LiquidCrystal** (built into the Arduino IDE)

---

## Problems I Ran Into (and How I Fixed Them)

**Sensor said "Sensor error"**
After moving power to the breadboard rails, the sensor stopped working. I'd connected its signal wire but forgotten its power and ground wires. Lesson: check that every pin has its wire.

**No potentiometer for LCD contrast**
My kit didn't have one, so I used a fixed 1kΩ resistor from V0 to ground, which worked well.

**LCD and LED dimmed when the fan turned on**
The motor pulled too much current from the computer's USB port, causing a brownout. I gave the fan its own power from a breadboard power module, removed the module's jumper on the Arduino's side so the two supplies didn't fight, and kept a shared ground so the transistor could still switch the fan.

**Text overlapping on the LCD**
My temperature text was 11 characters but I started printing humidity at column 8, so it wrote over itself. Shortening the label fixed it.

**Serial output printing in the wrong order**
Mixing `print` and `println` in the wrong order glued each temperature reading to the next loop's message. I moved the readings out of the if/else chain so they print once, every loop, before the message.

**Fan flickering near the threshold**
I added hysteresis: a `bool` variable declared outside `loop()` remembers whether the fan is on, so in the zone between the two thresholds it keeps doing whatever it was already doing.

---

## What I Learned

- Reading sensors and driving displays
- Controlling a motor safely with a transistor and a flyback diode
- Why motors need separate power, and why grounds must be shared
- `if / else if / else` logic, global vs. local variables, and booleans
- Reading compiler errors (always fix the first one first)

---

## What's Next (v2.0)

- Move from the Uno to an Arduino Nano
- Add an I2C backpack to the LCD (16 wires down to 4)
- Solder everything onto a perfboard instead of a breadboard
- Run everything from a single 5V wall adapter, with a capacitor to handle the fan's startup surge
- Design a 3D-printed enclosure in Tinkercad
