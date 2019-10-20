# DopePods

## Memory Game (2P Simon)

### Getting Started

- connect arduino to USB
- open arduino sketch `memory_puzzle.ino`
- Mega 2460
- wait for serial port to present itself

### TODO
* wiring shorts on P2 RED and BLUE. Most likely wiring (lights only)
  - Tighten the crimps. seems like side wires. 
    - blue/green on BLUE light.
    - orange/yellow on RED?

* BUG: pressing button at same time only counts one.
  - pressing the same time loses?
* BUG: All the delays block user input. They are 200ms and under, does it matter?
  - 2 people playing fast may get really frustrated.
  - code is in a way where it will check both inputs at once, mitigating
* BUG: randomness is crummy? lots of repeats? may be temporal
