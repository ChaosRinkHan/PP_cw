/*
 *  Simple molecular dynamics code.
 *
 */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "coord.h"

void visc_force(int N, double *f, double * vis, double *vel);
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
	for (step = 1; step <= count; step++) {
		printf("timestep %d\n", step);
		printf("collisions %d\n", collisions);
		//for (k = 0; k < Nbody; k++) {
		//	r[k] = 0.0;
		//}
		memset(r, 0., sizeof(double)*Nbody);
/* set the viscosity term in the force calculation */
		for (j = 0; j < Ndim; j++) {
			//visc_force(Nbody, f[j], vis, velo[j]);
#pragma vector aligned
#pragma ivdep
			for (i = 0; i < Nbody; i++) {
				f[j][i] = -vis[i] * velo[j][i];
			}
		}
/* add the wind term in the force calculation */
		for (j = 0; j < Ndim; j++) {
			//wind_force(Nbody, f[j], vis, wind[j]);
#pragma vector aligned
#pragma ivdep
			for (i = 0; i < Nbody; i++) {
				f[j][i] -= - vis[i] * wind[j];
			}
		}
/* calculate distance from central mass */
		//for (k = 0; k < Nbody; k++) {
		//	r[k] = 0.0;
		//}
		// Moved to head

		for (i = 0; i < Ndim; i++) {
			//add_norm(Nbody, r, pos[i]);
#pragma vector aligned
#pragma ivdep
			for (k = 0; k < Nbody; k++) {
				r[k] += (pos[i][k] * pos[i][k]);
			}
		}

		for (k = 0; k < Nbody; k++) {
			r[k] = sqrt(r[k]);
		}
		/* calculate central force */
		for (l = 0; l < Ndim; l++) {
			for (i = 0; i < Nbody; i++) {
				f[l][i] = f[l][i] -
						force(G * mass[i] * M_central, pos[l][i], r[i]);
			}
		}
/* calculate pairwise separation of particles */
		k = 0;
		for (l = 0; l < Ndim; l++) {
			for (i = 0; i < Nbody; i++){
				for (j = i + 1; j < Nbody; j++) {
					delta_pos[l][k] = pos[l][i] - pos[l][j];
				}
				k = k + 1;
			}
		}

/* calculate norm of separation vector */
		for (k = 0; k < Npair; k++) {
			delta_r[k] = 0.0;
		}
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
		for (l = 0; l < Ndim; l++){
				for (i = 0; i < Nbody; i++) {
				Size = radius[i] + radius[j];
				collided = 0;
				int sign = 1;
				for (j = i + 1; j < Nbody; j++){
/*  flip force if close in */
					sign = (delta_r[k] >= Size) ? -1 : 1;
					f[l][i] = f[l][i] + sign *
							force(G * mass[i] * mass[j], delta_pos[l][k], delta_r[k]);
					f[l][j] = f[l][j] - sign *
							force(G * mass[i] * mass[j], delta_pos[l][k], delta_r[k]);
					collided = (delta_r[k] >= Size) ? 0 : 1;
				}
				if (collided == 1) {
					collisions++;
				}
				k = k + 1;
			}
		}

/* update positions */
		for (j = 0; j < Ndim; j++){
			for (i = 0; i < Nbody; i++) {
				pos[j][i] = pos[j][i] + dt * velo[j][i];
				velo[j][i] = velo[j][i] + dt * (f[j][i] / mass[i]);
			}
		}

/* update velocities */
		//for (i = 0; i < Nbody; i++) {
		//	for (j = 0; j < Ndim; j++) {
		//	}
		//}

	}

}

double inline force(double W, double delta, double r) {
	return W * delta / (r*r*r);
}




