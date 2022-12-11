# Writeup 

## Introduction 
In this project, we wanted to create a simple visualizer for the different data encoding methods when data is sent over the physical wire, namely: NRZ, NRZ inverted, Manchester, and 4B/5B block encoding.

## Design/Implementation: 
What did you build, and how does it work? 
The resulting binary, `visualizer`, takes in a port number and listens for UDP packets on that particular port. And then there is a REPL that offers options for changing the 
- pace, which controls how fast the wire is moving across your screen. 
- encoding type, the default being NRZ
- sending a packet (using `ping <string>`). The LF is included in the string that is sent and is also visualized as a result.  

When the system starts up: 
- A listener thread is spawned that listens for incoming UDP packets
  - When a UDP packet is received, it visualizes the packet given the current settings (pace, encoding protocol)
  - Before the visualization step, the data is received and converted into a bit array according to the ASCII value of the character. For instance, the "!" character would result in [0, 0, 1, 0, 0, 0, 0, 1]. For 4B/5B block encoding, each of the four bits would get converted to the corresponding 5 bit number according to the conversion table. 
- A REPL that lets the user input a command to change settings or send data to the visualizer

For the visualizer, we took the array of bits and converted it to "signal transition segments", which contained two segments of either a LO or a HI segment to represent a change/no change in high or low voltage along the wire.
For printing, we also had to ensure the spacing of the individual signal transitions lined up and were printed correctly. This means we had to accumulate each line separately, and then print them all at once to get the results we wanted. 

We had a window size that would dictate how many of the signal transition segments we would print out at a time, and we would incrementally slide the window after each window was visualized. And between each window, we needed to clear out the terminal screen in order to create the animation, so we decided to use the `system` function call, passing in "clear" as the argument. And before clearing the terminal, we would sleep depending on how long the pace is. 

## Discussion/Results: 

We were able to reach our goal, though we chose to only implement one of the Manchester encoding type. Our results are displayed in a screen capture below for each of the different encoding types: 

**NRZ**

Displays 1 as high voltage, 0 as low voltage.

**NRZ Inverted**

0 stays at the same voltage, 1 transitions to the opposite voltage.

**Manchester**

0 is a low to high voltage transition, 1 is a high to low voltage transition.

**4B/5B Block Encoding**

Same as NRZ Inverted, but each segment of 4 bits is converted into 5 bits according to the conversion table so as to break up continuous 0's.


We've learned about the different encoding types and how each encoding type will impact the signal that is emitted from the wire, i.e. HI or LO depending on whether the bit was 1 or 0. 

The challenges we faced including finding the best way to visualize the data -- we ended up following a sliding window approach, to create a moving animation feel (similar to the `sl` command). 

## Conclusions/Future work: 
Overall, what have you learned? 
- Learned how to do ASCII art in C
- The different encoding types and the strengths and limitations of each 
  - For example, how NRZ (non-return to zero) can cause clock drift
  - and how NRZ-Inverted and Manchester attempt to address these problems. We weren't able to properly simulate the issues with each encoding, i.e. clock synchronization problems. 

How did you feel about this project overall? 
- This was really cool! And ASCII art in C is quite difficult, but is do-able. 

If you could keep working on this project, what would you do next? 
- Draw the clock
- More encoding techniques, more settings and customization
- Scale the visualization by terminal environment width
- Ethernet encoding

Are there any other directions of this work you find interesting? 
- Learning in more detail about how the hardware interacts with the software (the circuitry, etc.)