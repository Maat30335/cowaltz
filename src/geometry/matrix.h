#ifndef MATRIX_H
#define MATRIX_H


struct Matrix4x4{
    double m[4][4];
    Matrix4x4();
    Matrix4x4(double mat[4][4]);
    Matrix4x4(double t00,double t01,double t02,double t03,
         double t10,double t11,double t12,double t13,
         double t20,double t21,double t22,double t23,
         double t30,double t31,double t32,double t33);

    Matrix4x4 operator*(const Matrix4x4 &m2) const;

    bool operator==(const Matrix4x4 &m2) const;

    bool operator!=(const Matrix4x4 &m2) const;

    friend Matrix4x4 Transpose(const Matrix4x4 &);

    friend Matrix4x4 Inv(const Matrix4x4 &m);


};

#endif
