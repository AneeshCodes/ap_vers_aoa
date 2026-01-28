# Misinformation Simulation AoA

An Agent-Based Model (ABM) designed to simulate the spread of misinformation and truth within a synthetic model of Phoenix, Arizona.

## Core Model: SEDPNR

The simulation utilizes the SEDPNR epidemiological model to track agent states regarding a claim:

- **S (Susceptible)**: Has not yet encountered the claim.
- **E (Exposed)**: Has been exposed via social connections.
- **D (Doubtful)**: Evaluating the claim; requires social reinforcement to progress.
- **P (Propagating)**: Has adopted and is actively spreading the claim.
- **N (Not-Spreading)**: Has adopted but is not actively sharing.
- **R (Recovered)**: Has rejected the claim or ceased spreading it.

## Installation & Usage

#### Installing Dependencies (Windows)

1. Open the **UCRT64** terminal and install the toolchain:

   ```bash
   pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-make mingw-w64-ucrt-x86_64-sfml mingw-w64-ucrt-x86_64-python
   ```

```bash
make clean && make
```

### Running

To run the core simulation:

```bash
./simulation
```

To run the visualizer

```bash
./visualizer
```

Results are saved to `output/simulation_results.csv` and `output/spatial_data.csv`.
