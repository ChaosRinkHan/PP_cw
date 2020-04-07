/*
 *  Simple molecular dynamics code.
 *
 */
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "coord.h"

double force(double W, double delta, double r);
void new_force(int N, double *f, double *vis, double *velo, double wind);

void evolve(int count, double dt) {
  int step;
  int i, j, k, l;
  double Size, temp, tempForce, tempGmass, tempFLI[3];

  /*
   * Loop over timesteps.
   */
  for (step = 1; step <= count; step++) {
    printf("timestep %d\ncollisions %d\n", step, collisions);

    // memset(r, 0., sizeof(r));
    /* set the viscosity term in the force calculation */
    /* add the wind term in the force calculation */
    for (j = 0; j < Ndim; j++) {
      new_force(Nbody, f[j], vis, velo[j], wind[j]);
    }

    /* calculate distance from central mass */
    for (k = 0; k < Nbody; k++) {
      temp =
          pos[0][k] * pos[0][k] + pos[1][k] * pos[1][k] + pos[2][k] * pos[2][k];
      // r[k] = sqrt(temp);
      temp = sqrt(temp);
      tempGmass = GM * mass[k];
      f[0][k] -= force(tempGmass, pos[0][k], temp);
      f[1][k] -= force(tempGmass, pos[1][k], temp);
      f[2][k] -= force(tempGmass, pos[2][k], temp);
    }

    // for (i = 0; i < Nbody; i++) {
    //   f[0][i] -= force(GM * mass[i], pos[0][i], r[i]);
    //   f[1][i] -= force(GM * mass[i], pos[1][i], r[i]);
    //   f[2][i] -= force(GM * mass[i], pos[2][i], r[i]);
    // }
    // }

    /* calculate pairwise separation of particles */
    /* calculate norm of separation vector */
    k = 0;
    for (i = 0; i < Nbody; i++) {
      // #pragma ivdep
      for (j = i + 1; j < Nbody; j++) {
        delta_pos[0][k] = pos[0][i] - pos[0][j];
        delta_pos[1][k] = pos[1][i] - pos[1][j];
        delta_pos[2][k] = pos[2][i] - pos[2][j];

        temp = delta_pos[0][k] * delta_pos[0][k] +
               delta_pos[1][k] * delta_pos[1][k] +
               delta_pos[2][k] * delta_pos[2][k];
        delta_r[k] = sqrt(temp);
        k = k + 1;
      }
    }
    // for (k = 0; k < Npair; k++) {
    //   temp = delta_pos[0][k] * delta_pos[0][k] +
    //          delta_pos[1][k] * delta_pos[1][k] +
    //          delta_pos[2][k] * delta_pos[2][k];
    //   delta_r[k] = sqrt(temp);
    // }

    /*
     * add pairwise forces.
     */
    k = 0;
    for (i = 0; i < Nbody; i++) {
      memset(tempFLI, 0.0, sizeof(tempFLI));
      for (j = i + 1; j < Nbody; j++) {
        Size = radius[i] + radius[j];
        tempGmass = G * mass[i] * mass[j];

        /*  flip force if close in */
        if (delta_r[k] >= Size) {
          for (l = 0; l < Ndim; l++) {
            tempForce = force(tempGmass, delta_pos[l][k], delta_r[k]);
            // f[l][i] = f[l][i] - tempForce;
            tempFLI[l] -= tempForce;
            f[l][j] += tempForce;
          }
        } else {
          for (l = 0; l < Ndim; l++) {
            tempForce = force(tempGmass, delta_pos[l][k], delta_r[k]);
            // f[l][i] = f[l][i] + tempForce;
            tempFLI[l] += tempForce;
            f[l][j] -= tempForce;
          }
          collisions++;
        }
        k = k + 1;
      }
      f[0][i] += tempFLI[0];
      f[1][i] += tempFLI[1];
      f[2][i] += tempFLI[2];
    }

    /* update positions */
    /* update velocities */
    for (j = 0; j < Ndim; j++) {
      for (i = 0; i < Nbody; i++) {
        pos[j][i] += dt * velo[j][i];
        velo[j][i] += dt * (f[j][i] / mass[i]);
      }
    }
  }
}
