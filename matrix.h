#ifndef MATRIX_H
#define MATRIX_H


struct Matrix4x4{
    float m[4][4];
    Matrix4x4();
    Matrix4x4(float mat[4][4]);
    Matrix4x4(float t00,float t01,float t02,float t03,
         float t10,float t11,float t12,float t13,
         float t20,float t21,float t22,float t23,
         float t30,float t31,float t32,float t33);

    Matrix4x4 operator*(const Matrix4x4 &m2) const;

    bool operator==(const Matrix4x4 &m2) const;

    bool operator!=(const Matrix4x4 &m2) const;

    friend Matrix4x4 Transpose(const Matrix4x4 &);

    friend Matrix4x4 Inv(const Matrix4x4 &m);


};

#endif
