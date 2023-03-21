#include "matrix.h"
#include <string.h>
#include "useful.h"
#include <utility>
#include <stdexcept>


Matrix4x4::Matrix4x4(){
    m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1;
    m[0][1] = m[0][2] = m[0][3] = m[1][0] = m[1][2] = m[1][3] = m[2][0] =
            m[2][1] = m[2][3] = m[3][0] = m[3][1] = m[3][2] = 0;
}

Matrix4x4::Matrix4x4(double mat[4][4]) { memcpy(m, mat, 16 * sizeof(double)); }

Matrix4x4::Matrix4x4(double t00,double t01,double t02,double t03,
         double t10,double t11,double t12,double t13,
         double t20,double t21,double t22,double t23,
         double t30,double t31,double t32,double t33){
    m[0][0] = t00;
    m[0][1] = t01;
    m[0][2] = t02;
    m[0][3] = t03;
    m[1][0] = t10;
    m[1][1] = t11;
    m[1][2] = t12;
    m[1][3] = t13;
    m[2][0] = t20;
    m[2][1] = t21;
    m[2][2] = t22;
    m[2][3] = t23;
    m[3][0] = t30;
    m[3][1] = t31;
    m[3][2] = t32;
    m[3][3] = t33;
}



bool Matrix4x4::operator==(const Matrix4x4 &m2) const{
    for(int i = 0; i < 4; ++i){
        for(int j = 0; j < 4; ++j){
            if(m[i][j] != m2.m[i][j]) return false;
        }
    }
    return true;
}

bool Matrix4x4::operator!=(const Matrix4x4 &m2) const{
    for(int i = 0; i < 4; ++i){
        for(int j = 0; j < 4; ++j){
            if(m[i][j] != m2.m[i][j]) return true;
        }
    }
    return false;
}



Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &m2) const{
    Matrix4x4 ans;
    for(int i = 0; i < 4; ++i){
        for(int j = 0; j < 4; j++){
            ans.m[i][j] = m[i][0] * m2.m[0][j] + 
                        m[i][1] * m2.m[1][j] + 
                        m[i][2] * m2.m[2][j] + 
                        m[i][3] * m2.m[3][j];
        }
    }
    return ans;
}

Matrix4x4 Transpose(const Matrix4x4 &m){
    return Matrix4x4(m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0],
                        m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1],
                        m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2],
                        m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3]);
}


Matrix4x4 Inv(const Matrix4x4 &m){
    int indxc[4], indxr[4];
    int ipiv[4] = {0, 0, 0, 0};
    double minv[4][4];
    memcpy(minv, m.m, 4 * 4 * sizeof(double));
    for (int i = 0; i < 4; i++) {
        int irow = 0, icol = 0;
        double big = 0;
        // Choose pivot
        for (int j = 0; j < 4; j++) {
            if (ipiv[j] != 1) {
                for (int k = 0; k < 4; k++) {
                    if (ipiv[k] == 0) {
                        if (std::abs(minv[j][k]) >= big) {
                            big = double(std::abs(minv[j][k]));
                            irow = j;
                            icol = k;
                        }
                    } else if (ipiv[k] > 1)
                        Error("Singular matrix in MatrixInvert");
                }
            }
        }
        ++ipiv[icol];
        // Swap rows _irow_ and _icol_ for pivot
        if (irow != icol) {
            for (int k = 0; k < 4; ++k) std::swap(minv[irow][k], minv[icol][k]);
        }
        indxr[i] = irow;
        indxc[i] = icol;
        if (minv[icol][icol] == 0) Error("Singular matrix in MatrixInvert");

        // Set $m[icol][icol]$ to one by scaling row _icol_ appropriately
        double pivinv = 1. / minv[icol][icol];
        minv[icol][icol] = 1.;
        for (int j = 0; j < 4; j++) minv[icol][j] *= pivinv;

        // Subtract this row from others to zero out their columns
        for (int j = 0; j < 4; j++) {
            if (j != icol) {
                double save = minv[j][icol];
                minv[j][icol] = 0;
                for (int k = 0; k < 4; k++) minv[j][k] -= minv[icol][k] * save;
            }
        }
    }
    // Swap columns to reflect permutation
    for (int j = 3; j >= 0; j--) {
        if (indxr[j] != indxc[j]) {
            for (int k = 0; k < 4; k++)
                std::swap(minv[k][indxr[j]], minv[k][indxc[j]]);
        }
    }
    return Matrix4x4(minv);
}
