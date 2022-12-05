# Network Encoding on the Physical Layer -- Visualizer

Visualizing how bits are transformed into signals on the physical layer. NRZ, Manchester, block encoding.

## How to use

Usage: `./visualizer <udp_port_num>`

### Supported commands:

**Changing encoding method:**
- `nrz` -> change the visualization mode to NRZ (no-return to zero)
- `manchester` -> change the visualization mode to Manchester
- `block` -> change the visualization mode to block ending

Default mode is `manchester`.

**Running the visualizer:**
- `ping <data string>` -> send a packet containing `<data string>` to the listener, outputting the visualization to `stdout`.
- `ping_random <num_bytes>` -> read `<num_bytes>` bytes from `/dev/random` and send it as a packet to the listener, outputting the visualization to `stdout`.



