/*
 *  Simple molecular dynamics code.
 *
 */
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "coord.h"

double force(double W, double delta, double r);

void evolve(int count, double dt) {
  int step;
  int i, j, k, l;
  double Size, temp, tempForce, tempGmass, tempFLI[3];

  for (step = 1; step <= count; step++) {
    printf("timestep %d\ncollisions %d\n", step, collisions);

    // LOOP 1
    for (i = 0; i < Nbody; i++) {
      temp = 0;
      for (j = 0; j < Ndim; j++) {
        /* set the viscosity term in the force calculation */
        /* add the wind term in the force calculation */
        f[i][j] = -vis[i] * velo[i][j] - vis[i] * wind[j];
        temp += pos[i][j] * pos[i][j];
      }
      /* calculate distance from central mass */
      /* calculate central force */
      temp = sqrt(temp);
      tempGmass = GM * mass[i];
      for (j = 0; j < Ndim; j++) f[i][j] -= force(tempGmass, pos[i][j], temp);
    }

    // LOOP 2
    double delta_pos_new[Ndim], temp_norm;
    k = 0;
    for (i = 0; i < Nbody; i++) {
      memset(tempFLI, 0.0, sizeof(tempFLI));

      for (j = i + 1; j < Nbody; j++) {
        /* calculate pairwise separation of particles */
        delta_pos_new[0] = pos[i][0] - pos[j][0];
        delta_pos_new[1] = pos[i][1] - pos[j][1];
        delta_pos_new[2] = pos[i][2] - pos[j][2];

        /* calculate norm of separation vector */
        temp_norm = delta_pos_new[0] * delta_pos_new[0] +
                    delta_pos_new[1] * delta_pos_new[1] +
                    delta_pos_new[2] * delta_pos_new[2];
        temp_norm = sqrt(temp_norm);

        /* add pairwise forces */
        Size = radius[i] + radius[j];
        tempGmass = G * mass[i] * mass[j];

        /*  flip force if close in */
        if (temp_norm >= Size) {
          for (l = 0; l < Ndim; l++) {
            tempForce = force(tempGmass, delta_pos_new[l], temp_norm);
            tempFLI[l] -= tempForce;
            f[j][l] += tempForce;
          }
        } else {
          for (l = 0; l < Ndim; l++) {
            tempForce = force(tempGmass, delta_pos_new[l], temp_norm);
            tempFLI[l] += tempForce;
            f[j][l] -= tempForce;
          }
          collisions++;
        }

        k = k + 1;
      }
      f[i][0] += tempFLI[0];
      f[i][1] += tempFLI[1];
      f[i][2] += tempFLI[2];
    }

    // LOOP 3
    for (i = 0; i < Nbody; i++) {
      for (j = 0; j < Ndim; j++) {
        /* update positions */
        pos[i][j] += dt * velo[i][j];
        /* update velocities */
        velo[i][j] += dt * (f[i][j] / mass[i]);
      }
    }
  }
}
