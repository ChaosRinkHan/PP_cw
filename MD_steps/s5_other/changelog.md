# Change log

- Used flags
- Ststic attary
  - control 44 use temp to reduce calculation
  - control 107 i removed
  - OFFSET 64 <- 1324037 USED
  - OFFSET  0 <- 1324039
  - OFFSET 32 <- 1324134

- loop fusion
  - visc and wind merged                      <- bench_c.o1324440
  - new r, unroll, interchanges, merge        <- bench_c.o1324469
  - central force, GM, interchange done by compiler
  - norm of separation vector, interchange, merge, unroll       <- 1324567 48s
    - merge with airwise separation of particles                 <- 1324708 45s
  - position and velocity update, interchange, merged
  - add pairwise forces, interchange, conditionals              <- 1324579 46s
    - if removed
    - f[l][i] moved out of j loop to reduce dependency

Final: 1324667 43s

- r, delta_pos, delta_r
  - r removed
  - delta_r removed
  - delta_pos removed           <- 1325591 34.1s
- Read the logic and make optimisation
  - central force optmisation
    - mannual unroll
  - pairwise separation of particles
    - manually unroll
    - merge                     <- bench_c.o1325470 39s
    - merge with add            <- 1325487 40s
- pos[i][j] interchange         <- 1325637 34.5s
- f[i][j] interchange           <- 1325663 34.1s
  - new_force inlined
  - f can be local              <- 1325733 34.6s not used
  - first two loop merged       <- 1325692 34.2s
- velo[i][j] interchanged       <- 1325717 34.1s
- Nbody, Npair calculated       <- 1325777 34.2s
- save calculation for mass     <- TBT