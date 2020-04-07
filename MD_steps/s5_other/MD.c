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

    /* set the viscosity term in the force calculation */
    /* add the wind term in the force calculation */
    for (j = 0; j < Ndim; j++) {
      new_force(Nbody, f[j], vis, velo[j], wind[j]);
    }

    /* calculate distance from central mass */
    for (k = 0; k < Nbody; k++) {
      temp =
          pos[k][0] * pos[k][0] + pos[k][1] * pos[k][1] + pos[k][2] * pos[k][2];
      temp = sqrt(temp);
      tempGmass = GM * mass[k];
      f[0][k] -= force(tempGmass, pos[k][0], temp);
      f[1][k] -= force(tempGmass, pos[k][1], temp);
      f[2][k] -= force(tempGmass, pos[k][2], temp);
    }

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
        temp_norm = delta_pos_new[0] * delta_pos_new[0] + delta_pos_new[1] * delta_pos_new[1] +
                    delta_pos_new[2] * delta_pos_new[2];
        temp_norm = sqrt(temp_norm);

        /* add pairwise forces */
        Size = radius[i] + radius[j];
        tempGmass = G * mass[i] * mass[j];

        /*  flip force if close in */
        if (temp_norm >= Size) {
          for (l = 0; l < Ndim; l++) {
            tempForce = force(tempGmass, delta_pos_new[l], temp_norm);
            // f[l][i] = f[l][i] - tempForce;
            tempFLI[l] -= tempForce;
            f[l][j] += tempForce;
          }
        } else {
          for (l = 0; l < Ndim; l++) {
            tempForce = force(tempGmass, delta_pos_new[l], temp_norm);
            // f[l][i] += tempForce;
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
        pos[i][j] += dt * velo[j][i];
        velo[j][i] += dt * (f[j][i] / mass[i]);
      }
    }
  }
}
