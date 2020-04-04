/*
 *  Simple molecular dynamics code.
 *
 */
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "coord.h"

void visc_force(int N, double *f, double *vis, double *vel);
void add_norm(int N, double *r, double *delta);
double force(double W, double delta, double r);
void wind_force(int N, double *f, double *vis, double vel);

void evolve(int count, double dt) {
	int step;
	int i, j, k, l;
	int collided;
	double Size;
	/*
	 * Loop over timesteps.
	 */
#pragma vector aligned
	for (step = 1; step <= count; step++) {
		printf("timestep %d\n", step);
		printf("collisions %d\n", collisions);

		memset(r, 0., sizeof(double) * Nbody + 16);

		/* set the viscosity term in the force calculation */
		for (j = 0; j < Ndim; j++) {
			visc_force(Nbody, f[j], vis, velo[j]);
		}

		/* add the wind term in the force calculation */
		for (j = 0; j < Ndim; j++) {
			wind_force(Nbody, f[j], vis, wind[j]);
		}
		/* calculate distance from central mass */
		//for (k = 0; k < Nbody; k++) { // Moved to front
		//  r[k] = 0.0;									// memset
		//}
		for (i = 0; i < Ndim; i++) {
			add_norm(Nbody, r, pos[i]);
		}

		for (k = 0; k < Nbody; k++) {
			r[k] = sqrt(r[k]);
		}
		/* calculate central force */
		for (l = 0; l < Ndim; l++) {
#pragma vector aligned
#pragma ivdep
			for (i = 0; i < Nbody; i++) {
				double temp = f[l][i] - force(G * mass[i] * M_central, pos[l][i], r[i]);
				f[l][i] = temp;
			}
		}
		/* calculate pairwise separation of particles */
		//k = 0;
		for (l = 0; l < Ndim; l++) {
			k = 0;
			for (i = 0; i < Nbody; i++) {
#pragma ivdep
//#pragma vector aligned
				for (j = i + 1; j < Nbody; j++) {
					delta_pos[l][k] = pos[l][i] - pos[l][j];
					k = k + 1;
				}

			}
		} // TODO: improve
		/* calculate norm of separation vector */
		//for (k = 0; k < Npair; k++) {
		//	delta_r[k] = 0.0;
		//}
		memset(delta_r, 0., sizeof(double) * Npair + 16);

		for (i = 0; i < Ndim; i++) {
			add_norm(Npair, delta_r, delta_pos[i]);
		}
		for (k = 0; k < Npair; k++) {
			delta_r[k] = sqrt(delta_r[k]);
		}

		/*
		 * add pairwise forces.
		 */
		k = 0;
		double tempForce; // TODO: improve
		for (i = 0; i < Nbody; i++) {
			for (j = i + 1; j < Nbody; j++) {
				Size = radius[i] + radius[j];
				collided = 0;
				if (delta_r[k] >= Size) {
					for (l = 0; l < Ndim; l++) {
						/*  flip force if close in */
						tempForce = force(G * mass[i] * mass[j], delta_pos[l][k], delta_r[k]);
						f[l][i] = f[l][i] - tempForce;
						f[l][j] = f[l][j] + tempForce;
					}
				} else {
					for (l = 0; l < Ndim; l++) {
						tempForce = force(G * mass[i] * mass[j], delta_pos[l][k], delta_r[k]);
						f[l][i] = f[l][i] + tempForce;
						f[l][j] = f[l][j] - tempForce;
						collided = 1;
					}
					if (collided == 1) {
						collisions++;
					}
				}
				k = k + 1;
			}
		}

	/* update positions */
	for (j = 0; j < Ndim; j++) {
#pragma ivdep
		for (i = 0; i < Nbody; i++) {
			pos[j][i] = pos[j][i] + dt * velo[j][i];
			velo[j][i] = velo[j][i] + dt * (f[j][i] / mass[i]);
		}
	}

	/* update velocities */
//	for (j = 0; j < Ndim; j++) {
//#pragma ivdep
//		for (i = 0; i < Nbody; i++) {
//			velo[j][i] = velo[j][i] + dt * (f[j][i] / mass[i]);
//		}
//	}
}
}

double force(double W, double delta, double r) {
	return W * delta / (r*r*r);
}

void visc_force(const int N, double *f, double *vis, double *velo) {
	int i;
#pragma vector aligned
#pragma ivdep
	for (i = 0; i < N; i++) {
		f[i] = -vis[i] * velo[i];
	}
}

void add_norm(const int N, double * r, double * delta) {
	int k;
#pragma vector aligned
#pragma ivdep
	for (k = 0; k < N; k++) {
		r[k] += (delta[k] * delta[k]);
	}
}

void wind_force(const int N, double *f, double *vis, double velo) {
	int i;
#pragma vector aligned
#pragma ivdep
	for (i = 0; i < N; i++) {
		f[i] -= vis[i] * velo;
	}
}