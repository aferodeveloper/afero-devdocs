# MCU to ASR Communication

The MCU is the microcontroller in your IoT device. The MCU sends attribute states to the ASR module and receives state updates from the Afero Cloud via ASR; that is, this communication is bi-directional. The MCU to ASR communication protocol is implemented on one side by ASR firmware and on the other side by the afLib library. afLib wraps the communication protocol in a simple API for use in your application.

afLib is provided in a form ready to be used with the Arduino development environment. We’ve designed it so you can port it to any hardware and OS platform easily. It’s provided as C source code that you integrate into your development workflow. You will just need to write the low-level SPI or UART functions for your hardware and software environment.

In the first two links below, we’ve provided timing diagrams to help you understand the communication protocol using real examples. The state machine in afLib implements this protocol and should also be used as a model when porting the code. Note that the diagrams in the first two sections below were captured realtime during ASR operation. The third link below is to the “Practical Guide” to using the UART protocol, which describes a single (“happy”) path through the UART protocol.

- [afPro SPI Protocol - How It Works](../afPro-SPI)
- [afPro UART Protocol - How It Works](../afPro-UART) - Also available as a [Tech Note (.pdf file)](../files/TechNote-AferoUARTProtocol.pdf).
- [afPro UART Protocol - Practical Guide](../afPro-UART-S) - An alternative explanation.

 *Next:*&nbsp;&nbsp;[**&#8674;** afPro SPI Protocol](../afPro-SPI)