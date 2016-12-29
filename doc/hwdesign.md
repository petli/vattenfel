
# Panel

Arduino Micro: Atmega 32U4

## RF

Pin assignment:

    PB1/SCK
    PB2/MOSI
    PB3/MISO
    PB4/DP8:      CE
    PB7/DP11:     CSN

TODO: seem to be no need for IRQ with autoack

    INT2/PD2/DP0: IRQ

    
## Door magnet sensor

INT1/PD1/DP2

Any edge triggered input.  Not really interesting whether door is open
or not.

Sensor NC, need to limit current through it.


## Status/on/off switch

INT0/PD0/DP3

Falling edge triggered input, react on press.

Press to activate unit (alternative to door).

When unit is active, press to turn water on/off.

TODO: Support long press to reset?


## Status LEDs: R, G, Y

Can source 20mA per pin, max 100mA per IO port.

When sourcing 20mA the pin output voltage is 4.2V.

Blinks to grab attention and save battery.  When R/G blinks the water
can be turned on/off with the button.

PB6/DP10: R: water off
PC6/DP5:  G: water on
PC7/DP13: Y: RF communication, error indicator

R+G: low pressure detected.

TODO: fast blink if there's been a power outage too?

This avoid the LEDs sharing IO banks with the inputs and mostly with
the SPI on bank PB.


## Timer

Timer 1 to blink LEDs.  TODO: figure out scaling/compare registers. 



## Sleep mode

Power-down mode: Wake up on level interrupt INT6, interrupt on INT3:0,
or a pin change interrupt.

Disable: ADC, comparator, BOD, IVR, WDT, JTD.

10.2.6: enable pull up inputs on unused ports to save power during
deep sleep mode.


# Controller

Arduino Uno: Atmega 328P

Connected to mains power, so no need to worry about saving power.


## RF

Default RX mode, waiting for messages from control panel using IRQ.

Pin assignment:

    PB2/DP10:     SS (unused)
    PB3/DP11:     MOSI
    PB4/DP12:     MISO
    PB5/DB13:     SCK
    PB0/DP8:      CSN
    PB1/DP9:      CE
    INT0/PD2/DP2: IRQ


## Pressure sensor

TODO Pin change interrupt?

Edge triggered, but also interesting to know the state.

TODO: opened/closed on no pressure?


## Solenoid relay

PD4/DP4 output

TODO: pull low to close relay and active solenoid?


## Reset button

INT1/PD3/DP3

Level-triggered.

Press to turn water on if off.


## Status LEDs: R, G, Y

TODO: PC5 bank?  Or split over PD/PB bank?

G blinking: unit running

Y blinking: RF comm

R blinking: water off

R solid: low pressure detected

