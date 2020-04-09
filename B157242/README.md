# Overview

This directory contains a simple molecular dynamics (MD) code

The program implements:

    long range inverse square forces between particles. F = G * m1*m2 / r**2
    viscosity term     F = -u V

If 2 particles approach closer than Size we flip the direction of the
interaction force to approximate a collision.

Coordinates are relative to a large central mass and the entire system is moving relative to the
viscous media.

# Build and run

## Build the code

```
make clean
make
```

`qopt` report can be find in `report/`

## Run the code

```
./MD [parameter]
```

Output can be found in `output/`.

## Build correctness test

```
cd Test
make
```

## Test correctness

```
make diff
```

## Submit to PBS

```
qsub bench_c.pbs
```

## Submit a memory profile

```
qsub mem_profile.pbs
```
Profile report can be find in `report/`

## Clean all outputs and report
```
make cclean
```