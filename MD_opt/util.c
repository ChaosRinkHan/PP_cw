#include <math.h>

double force(double W, double delta, double r) {
  return W * delta / (r * r * r);
}

void visc_force(const int N, double *f, double *vis, double *velo) {
  int i;
#pragma vector aligned
#pragma ivdep
  for (i = 0; i < N; i++) {
    f[i] = -vis[i] * velo[i];
  }
}

void add_norm(const int N, double *r, double *delta) {
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

