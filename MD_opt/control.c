/*
 *
 * Control program for the MD update
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define DECL
#include "coord.h"

double second(void);

int main(int argc, char *argv[]) {
	int i, j;
	FILE *in, *out;
	double tstart, tstop;
	double start, stop;
	char name[80];
	/*  timestep value */
	double dt = 0.02;

	/*  number of timesteps to use. */
	int Nstep = 100;
	int Nsave = 5;

	if (argc > 1) {
		Nstep = atoi(argv[1]);
	}
	wind[Xcoord] = 0.9;
	wind[Ycoord] = 0.4;
	wind[Zcoord] = 0.0;
	/* set up multi dimensional arrays */
	r = calloc(Nbody+32, sizeof(double));
	delta_r = calloc(Nbody * Nbody+32, sizeof(double));
	mass = calloc(Nbody+32, sizeof(double));
	radius = calloc(Nbody+32, sizeof(double));
	vis = calloc(Nbody+32, sizeof(double));
	f[0] = calloc(Ndim * Nbody+32, sizeof(double));
	pos[0] = calloc(Ndim * Nbody+32, sizeof(double));
	velo[0] = calloc(Ndim * Nbody+32, sizeof(double));
	delta_pos[0] = calloc(Ndim * Nbody * Nbody+32, sizeof(double));

	for (i = 1; i < Ndim; i++) {
		int temp = i * Nbody;
		f[i] = f[0] + temp;
		pos[i] = pos[0] + temp;
		velo[i] = velo[0] + temp;
		delta_pos[i] = delta_pos[0] + temp * Nbody;
	}

/* read the initial data from a file */

	collisions = 0;
	in = fopen("input.dat", "r");

	if (!in) {
		perror("input.dat");
		exit(1);
	}

	for (i = 0; i < Nbody; i++) {
		fscanf(in, "%16le%16le%16le%16le%16le%16le%16le%16le%16le\n",
					 mass + i, radius + i, vis + i,
					 &pos[Xcoord][i], &pos[Ycoord][i], &pos[Zcoord][i],
					 &velo[Xcoord][i], &velo[Ycoord][i], &velo[Zcoord][i]);
	}
	fclose(in);

/*
 * Run Nstep timesteps and time how long it took
 */

	tstart = second();
	for (j = 1; j <= Nsave; j++) {
		start = second();
		evolve(Nstep, dt);
		stop = second();
		printf("%d timesteps took %f seconds\n", Nstep, stop - start);
		printf("collisions %d\n", collisions);

/* write final result to a file */
		sprintf(name, "output/output.dat%03d", j * Nstep);
		out = fopen(name, "w");

		if (!out) {
			perror(name);
			exit(1);
		}

		for (i = 0; i < Nbody; i++) {
			fprintf(out, "%16.8E%16.8E%16.8E%16.8E%16.8E%16.8E%16.8E%16.8E%16.8E\n",
							mass[i], radius[i], vis[i],
							pos[Xcoord][i], pos[Ycoord][i], pos[Zcoord][i],
							velo[Xcoord][i], velo[Ycoord][i], velo[Zcoord][i]);
		}
		fclose(out);
	}
	tstop = second();
	printf("%d timesteps took %f seconds\n", Nsave * Nstep, tstop - tstart);

}

double second() {
	struct timeval tp;
	struct timezone tzp;
	gettimeofday(&tp, &tzp);
	return ((double) tp.tv_sec + (double) tp.tv_usec * 1.e-6);
}

