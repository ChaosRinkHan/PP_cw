# Change log

- Used flags
- Ststic attary
  - control 44 use temp to reduce calculation
  - control 107 i removed
  - OFFSET 64 <- 1324037 USED
  - OFFSET  0 <- 1324039
  - OFFSET 32 <- 1324134

- loop fusion
  - visc and wind merged <- bench_c.o1324440
  - new r, unroll, interchanges, merge <- bench_c.o1324469
  - central force, GM, interchange done by compiler
  - norm of separation vector, interchange, merge, unroll <- 1324567 48s