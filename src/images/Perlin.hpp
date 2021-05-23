#ifndef IMAGES_PERLIN_H_
#define IMAGES_PERLIN_H_

#include <cmath>
#include "../MathUtils.cpp"
#include "../algebra/Point.hpp"


class Perlin {

protected:

    static const int point_count = 256;
    Vector* ranvec;
    int* perm_x;
    int* perm_y;
    int* perm_z;

public:

    int seed;

	Perlin(int const& seed = 1) : seed(seed) {
        ranvec = new Vector[point_count];
        for (int i = 0; i < point_count; ++i)
            ranvec[i] = Vector(randseed(seed+i+1), randseed(seed+i+1), randseed(seed+i+1)).unit();

        perm_x = perlin_generate_perm();
        perm_y = perlin_generate_perm();
        perm_z = perlin_generate_perm();
    }

    ~Perlin() {
        delete[] ranvec;
        delete[] perm_x;
        delete[] perm_y;
        delete[] perm_z;
    }

    double noise(Vector const& vec) const {
        float u = vec.x - floor(vec.x);
        float v = vec.y - floor(vec.y);
        float w = vec.z - floor(vec.z);

        int i = int(vec.x);
        int j = int(vec.y);
        int k = int(vec.z);
        Vector c[2][2][2];

        for (int di = 0; di < 2; di++)
            for (int dj = 0; dj < 2; dj++)
                for (int dk = 0; dk < 2; dk++)
                    c[di][dj][dk] = ranvec[
                        perm_x[(i+di) & 255] ^
                        perm_y[(j+dj) & 255] ^
                        perm_z[(k+dk) & 255]
                    ];

        return perlin_interp(c, u, v, w);

    }

    double turb(Vector const& vec, int depth = 7) const {
        float accum = 0.;
        Vector temp_p = vec;
        float weight = 1.;

        for (int i = 0; i < depth; i++) {
            accum += weight*noise(temp_p);
            weight *= 0.5;
            temp_p *= 2;
        }

        return fabs(accum);
    }

protected:

    static double randseed(int t) {
        t = (t<<13) ^ t;
        t = (t * (t * t * 15731 + 789221) + 1376312589);
        return 1.0 - (t & 0x7fffffff) / 1073741824.0;
    }

    int* perlin_generate_perm() {
        int* p = new int[point_count];

        for (int i = 0; i < Perlin::point_count; i++) p[i] = i;

        permute(p, point_count);

        return p;
    }

    void permute(int* p, int n) {
        for (int i = n-1; i > 0; i--) {
            int target = int( (randseed(seed + i) + 1)*i/2 );
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }

    /*static double trilinear_interp(double c[2][2][2], double u, double v, double w) {
        auto accum = 0.0;
        for (int i=0; i < 2; i++)
            for (int j=0; j < 2; j++)
                for (int k=0; k < 2; k++)
                    accum += (i*u + (1-i)*(1-u))*
                            (j*v + (1-j)*(1-v))*
                            (k*w + (1-k)*(1-w))*c[i][j][k];

        return accum;
    }*/

    static double perlin_interp(Vector c[2][2][2], double u, double v, double w) {
        float uu = u*u*(3-2*u);
        float vv = v*v*(3-2*v);
        float ww = w*w*(3-2*w);
        float accum = 0.0;

        for (int i=0; i < 2; i++)
            for (int j=0; j < 2; j++)
                for (int k=0; k < 2; k++) {
                    Vector weight_v(u-i, v-j, w-k);
                    accum += (i*uu + (1-i)*(1-uu))
                            * (j*vv + (1-j)*(1-vv))
                            * (k*ww + (1-k)*(1-ww))
                            * (c[i][j][k] * weight_v);
                }

        return accum;
    }

};


#endif
