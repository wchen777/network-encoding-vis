# Network Encoding on the Physical Layer -- Visualizer

Visualizing how bits are transformed into signals on the physical layer. NRZ, Manchester, block encoding.

## How to use

Usage: `./visualizer <udp_port_num>`

### Supported commands:

**Changing encoding method:**
- `nrz_l` -> change the visualization mode to NRZ (non-return to zero) level 
- `nrz_i` -> change the visualization mode to NRZ (no-return to zero) inverted
- `manchester_biphase` -> change the visualization mode to Bi-phase Manchester
- `manchester_differential` -> change the visualization mode to Differential Manchester
- `block` -> change the visualization mode to 4B/5B block encoding

Default mode is `nrz_l`. More data on encoding techniques below.

**Running the visualizer:**
- `ping <data string>` -> send a packet containing `<data string>` to the listener, outputting the visualization to `stdout`.
- `ping_random <num_bytes>` -> read `<num_bytes>` bytes from `/dev/random` and send it as a packet to the listener, outputting the visualization to `stdout`.
- `ping_file <filepath>` -> read from the specified filepath and split up the file into packets to send to the visualizer.

**Visualizer settings:**
- `pace (0.0, 1.0]` -> set the signal visualizer speed


## Encoding techniques
