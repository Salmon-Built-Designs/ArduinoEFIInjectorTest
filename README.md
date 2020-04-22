#Arduino Based PWM Control for High Impedance Injectors 
 
Useful for flow testing and cleaning of EFI injectors
 

       Arduino                Transistor                    Injector
                             P1(B)  P2(C)  P3(E)            12V     GND
         ->GND  - -            |      |      |               |       |
                  |            |      |      |               |       |
         ->P10 - ^|^ - - - - - -      - - - ^|^- - - - - - -^|^- - - - 
                  |                          |               |
    12V Source    |                          |               |
                  |                          |               |
         ->12V --^|^- - - - - - - - - - - - ^|^- - - - - - - - 
                  |                          |
         ->GND - - - - - - - - - - - - - - - -
  
    Notes: 

              |
        A - -^|^- - Indicates that the wire from L to R does *not* connect. eg P10 only connects to P1 of the transistor (A Mosfet Can Be Used Too). 
              |

      Ensure the chosen Transistor / Mosfet has a fast enough trigger time for the openTime and adjust open time to compensate. Timing is calculated based on the arduino pin only.

      The Injector gets 12V all the time the Transistor used should be a NPN transistor I used a ST 2SD882 
              I had one on hand. it needs to trigger on ~5v or less and handle at least 200ma per injector or so. 

      If using a benchtop power supply ensure that the grounds are all equal between the Arduino power and the 12V source. Using a battery would work as well. 

      Injectors can overheat if opened too long without fuel flowing through them, they use the fuel to cool themselves as well as low open times to keep heat down. 
