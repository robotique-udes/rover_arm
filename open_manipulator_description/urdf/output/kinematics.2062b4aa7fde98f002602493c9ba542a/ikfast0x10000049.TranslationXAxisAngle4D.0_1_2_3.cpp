/// autogenerated analytical inverse kinematics code from ikfast program part of OpenRAVE
/// \author Rosen Diankov
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///     http://www.apache.org/licenses/LICENSE-2.0
/// 
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// ikfast version 0x10000049 generated on 2021-01-25 02:19:35.261089
/// To compile with gcc:
///     gcc -lstdc++ ik.cpp
/// To compile without any main function as a shared object (might need -llapack):
///     gcc -fPIC -lstdc++ -DIKFAST_NO_MAIN -DIKFAST_CLIBRARY -shared -Wl,-soname,libik.so -o libik.so ik.cpp
#define IKFAST_HAS_LIBRARY
#include "ikfast.h" // found inside share/openrave-X.Y/python/ikfast.h
using namespace ikfast;

// check if the included ikfast version matches what this file was compiled with
#define IKFAST_COMPILE_ASSERT(x) extern int __dummy[(int)x]
IKFAST_COMPILE_ASSERT(IKFAST_VERSION==0x10000049);

#include <cmath>
#include <vector>
#include <limits>
#include <algorithm>
#include <complex>

#ifndef IKFAST_ASSERT
#include <stdexcept>
#include <sstream>
#include <iostream>

#ifdef _MSC_VER
#ifndef __PRETTY_FUNCTION__
#define __PRETTY_FUNCTION__ __FUNCDNAME__
#endif
#endif

#ifndef __PRETTY_FUNCTION__
#define __PRETTY_FUNCTION__ __func__
#endif

#define IKFAST_ASSERT(b) { if( !(b) ) { std::stringstream ss; ss << "ikfast exception: " << __FILE__ << ":" << __LINE__ << ": " <<__PRETTY_FUNCTION__ << ": Assertion '" << #b << "' failed"; throw std::runtime_error(ss.str()); } }

#endif

#if defined(_MSC_VER)
#define IKFAST_ALIGNED16(x) __declspec(align(16)) x
#else
#define IKFAST_ALIGNED16(x) x __attribute((aligned(16)))
#endif

#define IK2PI  ((IkReal)6.28318530717959)
#define IKPI  ((IkReal)3.14159265358979)
#define IKPI_2  ((IkReal)1.57079632679490)

#ifdef _MSC_VER
#ifndef isnan
#define isnan _isnan
#endif
#ifndef isinf
#define isinf _isinf
#endif
//#ifndef isfinite
//#define isfinite _isfinite
//#endif
#endif // _MSC_VER

// lapack routines
extern "C" {
  void dgetrf_ (const int* m, const int* n, double* a, const int* lda, int* ipiv, int* info);
  void zgetrf_ (const int* m, const int* n, std::complex<double>* a, const int* lda, int* ipiv, int* info);
  void dgetri_(const int* n, const double* a, const int* lda, int* ipiv, double* work, const int* lwork, int* info);
  void dgesv_ (const int* n, const int* nrhs, double* a, const int* lda, int* ipiv, double* b, const int* ldb, int* info);
  void dgetrs_(const char *trans, const int *n, const int *nrhs, double *a, const int *lda, int *ipiv, double *b, const int *ldb, int *info);
  void dgeev_(const char *jobvl, const char *jobvr, const int *n, double *a, const int *lda, double *wr, double *wi,double *vl, const int *ldvl, double *vr, const int *ldvr, double *work, const int *lwork, int *info);
}

using namespace std; // necessary to get std math routines

#ifdef IKFAST_NAMESPACE
namespace IKFAST_NAMESPACE {
#endif

inline float IKabs(float f) { return fabsf(f); }
inline double IKabs(double f) { return fabs(f); }

inline float IKsqr(float f) { return f*f; }
inline double IKsqr(double f) { return f*f; }

inline float IKlog(float f) { return logf(f); }
inline double IKlog(double f) { return log(f); }

// allows asin and acos to exceed 1. has to be smaller than thresholds used for branch conds and evaluation
#ifndef IKFAST_SINCOS_THRESH
#define IKFAST_SINCOS_THRESH ((IkReal)1e-7)
#endif

// used to check input to atan2 for degenerate cases. has to be smaller than thresholds used for branch conds and evaluation
#ifndef IKFAST_ATAN2_MAGTHRESH
#define IKFAST_ATAN2_MAGTHRESH ((IkReal)1e-7)
#endif

// minimum distance of separate solutions
#ifndef IKFAST_SOLUTION_THRESH
#define IKFAST_SOLUTION_THRESH ((IkReal)1e-6)
#endif

// there are checkpoints in ikfast that are evaluated to make sure they are 0. This threshold speicfies by how much they can deviate
#ifndef IKFAST_EVALCOND_THRESH
#define IKFAST_EVALCOND_THRESH ((IkReal)0.00001)
#endif


inline float IKasin(float f)
{
IKFAST_ASSERT( f > -1-IKFAST_SINCOS_THRESH && f < 1+IKFAST_SINCOS_THRESH ); // any more error implies something is wrong with the solver
if( f <= -1 ) return float(-IKPI_2);
else if( f >= 1 ) return float(IKPI_2);
return asinf(f);
}
inline double IKasin(double f)
{
IKFAST_ASSERT( f > -1-IKFAST_SINCOS_THRESH && f < 1+IKFAST_SINCOS_THRESH ); // any more error implies something is wrong with the solver
if( f <= -1 ) return -IKPI_2;
else if( f >= 1 ) return IKPI_2;
return asin(f);
}

// return positive value in [0,y)
inline float IKfmod(float x, float y)
{
    while(x < 0) {
        x += y;
    }
    return fmodf(x,y);
}

// return positive value in [0,y)
inline double IKfmod(double x, double y)
{
    while(x < 0) {
        x += y;
    }
    return fmod(x,y);
}

inline float IKacos(float f)
{
IKFAST_ASSERT( f > -1-IKFAST_SINCOS_THRESH && f < 1+IKFAST_SINCOS_THRESH ); // any more error implies something is wrong with the solver
if( f <= -1 ) return float(IKPI);
else if( f >= 1 ) return float(0);
return acosf(f);
}
inline double IKacos(double f)
{
IKFAST_ASSERT( f > -1-IKFAST_SINCOS_THRESH && f < 1+IKFAST_SINCOS_THRESH ); // any more error implies something is wrong with the solver
if( f <= -1 ) return IKPI;
else if( f >= 1 ) return 0;
return acos(f);
}
inline float IKsin(float f) { return sinf(f); }
inline double IKsin(double f) { return sin(f); }
inline float IKcos(float f) { return cosf(f); }
inline double IKcos(double f) { return cos(f); }
inline float IKtan(float f) { return tanf(f); }
inline double IKtan(double f) { return tan(f); }
inline float IKsqrt(float f) { if( f <= 0.0f ) return 0.0f; return sqrtf(f); }
inline double IKsqrt(double f) { if( f <= 0.0 ) return 0.0; return sqrt(f); }
inline float IKatan2Simple(float fy, float fx) {
    return atan2f(fy,fx);
}
inline float IKatan2(float fy, float fx) {
    if( isnan(fy) ) {
        IKFAST_ASSERT(!isnan(fx)); // if both are nan, probably wrong value will be returned
        return float(IKPI_2);
    }
    else if( isnan(fx) ) {
        return 0;
    }
    return atan2f(fy,fx);
}
inline double IKatan2Simple(double fy, double fx) {
    return atan2(fy,fx);
}
inline double IKatan2(double fy, double fx) {
    if( isnan(fy) ) {
        IKFAST_ASSERT(!isnan(fx)); // if both are nan, probably wrong value will be returned
        return IKPI_2;
    }
    else if( isnan(fx) ) {
        return 0;
    }
    return atan2(fy,fx);
}

template <typename T>
struct CheckValue
{
    T value;
    bool valid;
};

template <typename T>
inline CheckValue<T> IKatan2WithCheck(T fy, T fx, T epsilon)
{
    CheckValue<T> ret;
    ret.valid = false;
    ret.value = 0;
    if( !isnan(fy) && !isnan(fx) ) {
        if( IKabs(fy) >= IKFAST_ATAN2_MAGTHRESH || IKabs(fx) > IKFAST_ATAN2_MAGTHRESH ) {
            ret.value = IKatan2Simple(fy,fx);
            ret.valid = true;
        }
    }
    return ret;
}

inline float IKsign(float f) {
    if( f > 0 ) {
        return float(1);
    }
    else if( f < 0 ) {
        return float(-1);
    }
    return 0;
}

inline double IKsign(double f) {
    if( f > 0 ) {
        return 1.0;
    }
    else if( f < 0 ) {
        return -1.0;
    }
    return 0;
}

template <typename T>
inline CheckValue<T> IKPowWithIntegerCheck(T f, int n)
{
    CheckValue<T> ret;
    ret.valid = true;
    if( n == 0 ) {
        ret.value = 1.0;
        return ret;
    }
    else if( n == 1 )
    {
        ret.value = f;
        return ret;
    }
    else if( n < 0 )
    {
        if( f == 0 )
        {
            ret.valid = false;
            ret.value = (T)1.0e30;
            return ret;
        }
        if( n == -1 ) {
            ret.value = T(1.0)/f;
            return ret;
        }
    }

    int num = n > 0 ? n : -n;
    if( num == 2 ) {
        ret.value = f*f;
    }
    else if( num == 3 ) {
        ret.value = f*f*f;
    }
    else {
        ret.value = 1.0;
        while(num>0) {
            if( num & 1 ) {
                ret.value *= f;
            }
            num >>= 1;
            f *= f;
        }
    }
    
    if( n < 0 ) {
        ret.value = T(1.0)/ret.value;
    }
    return ret;
}

/// solves the forward kinematics equations.
/// \param pfree is an array specifying the free joints of the chain.
IKFAST_API void ComputeFk(const IkReal* j, IkReal* eetrans, IkReal* eerot) {
for(int dummyiter = 0; dummyiter < 1; ++dummyiter) {
IkReal x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19,x20;
x0=IKcos(j[0]);
x1=IKcos(j[2]);
x2=IKcos(j[1]);
x3=IKsin(j[1]);
x4=IKsin(j[2]);
x5=IKsin(j[3]);
x6=IKsin(j[0]);
x7=IKcos(j[3]);
x8=((0.126)*x4);
x9=((0.124)*x6);
x10=((1.0)*x0);
x11=((0.128)*x3);
x12=((0.124)*x4);
x13=((0.126)*x0);
x14=((0.126)*x6);
x15=(x1*x2);
x16=(x1*x3);
x17=(x2*x6);
x18=(x0*x2);
x19=(x0*x3);
x20=(x3*x4);
IkReal x21=((1.0)*x19);
eetrans[0]=((0.012)+((x0*x11))+(((0.124)*x0*x15))+((x5*(((((-1.0)*x18*x8))+(((-1.0)*x13*x16))))))+(((0.024)*x18))+((x7*(((((-1.0)*x21*x8))+((x13*x15))))))+(((-1.0)*x12*x21)));
IkReal x22=((1.0)*x8);
eetrans[1]=(((x7*(((((-1.0)*x22*x3*x6))+((x14*x15))))))+((x5*(((((-1.0)*x17*x22))+(((-1.0)*x14*x16))))))+((x11*x6))+(((-1.0)*x20*x9))+(((0.024)*x17))+((x15*x9)));
IkReal x23=((1.0)*x2);
eetrans[2]=((0.0765)+(((0.128)*x2))+((x7*(((((-1.0)*x23*x8))+(((-0.126)*x16))))))+(((-0.124)*x16))+((x5*(((((-0.126)*x15))+((x3*x8))))))+(((-0.024)*x3))+(((-1.0)*x12*x23)));
if( ((((x7*((((x10*x16))+((x10*x2*x4))))))+((x5*((((x0*x15))+(((-1.0)*x10*x20)))))))) < -1-IKFAST_SINCOS_THRESH || ((((x7*((((x10*x16))+((x10*x2*x4))))))+((x5*((((x0*x15))+(((-1.0)*x10*x20)))))))) > 1+IKFAST_SINCOS_THRESH )
    continue;
eerot[0]=IKacos((((x7*((((x10*x16))+((x10*x2*x4))))))+((x5*((((x0*x15))+(((-1.0)*x10*x20))))))));
return;
}
IKFAST_ASSERT(0);
}

IKFAST_API int GetNumFreeParameters() { return 0; }
IKFAST_API int* GetFreeParameters() { return NULL; }
IKFAST_API int GetNumJoints() { return 4; }

IKFAST_API int GetIkRealSize() { return sizeof(IkReal); }

IKFAST_API int GetIkType() { return 0x4400000b; }

class IKSolver {
public:
IkReal j0,cj0,sj0,htj0,j0mul,j1,cj1,sj1,htj1,j1mul,j2,cj2,sj2,htj2,j2mul,j3,cj3,sj3,htj3,j3mul,new_r00,r00,rxp0_0,new_r01,r01,rxp0_1,new_r02,r02,rxp0_2,new_px,px,npx,new_py,py,npy,new_pz,pz,npz,pp;
unsigned char _ij0[2], _nj0,_ij1[2], _nj1,_ij2[2], _nj2,_ij3[2], _nj3;

IkReal j100, cj100, sj100;
unsigned char _ij100[2], _nj100;
bool ComputeIk(const IkReal* eetrans, const IkReal* eerot, const IkReal* pfree, IkSolutionListBase<IkReal>& solutions) {
j0=numeric_limits<IkReal>::quiet_NaN(); _ij0[0] = -1; _ij0[1] = -1; _nj0 = -1; j1=numeric_limits<IkReal>::quiet_NaN(); _ij1[0] = -1; _ij1[1] = -1; _nj1 = -1; j2=numeric_limits<IkReal>::quiet_NaN(); _ij2[0] = -1; _ij2[1] = -1; _nj2 = -1; j3=numeric_limits<IkReal>::quiet_NaN(); _ij3[0] = -1; _ij3[1] = -1; _nj3 = -1; 
for(int dummyiter = 0; dummyiter < 1; ++dummyiter) {
    solutions.Clear();
px = eetrans[0]; py = eetrans[1]; pz = eetrans[2];

r00 = eerot[0];
px = eetrans[0]; py = eetrans[1]; pz = eetrans[2];
new_px=px;
new_py=py;
new_pz=pz;
new_r00=r00;
r00 = new_r00; px = new_px; py = new_py; pz = new_pz;

pp=((px*px)+(py*py)+(pz*pz));
{
IkReal j0array[2], cj0array[2], sj0array[2];
bool j0valid[2]={false};
_nj0 = 2;
CheckValue<IkReal> x25 = IKatan2WithCheck(IkReal(((-1.0)*py)),IkReal(((-0.012)+px)),IKFAST_ATAN2_MAGTHRESH);
if(!x25.valid){
continue;
}
IkReal x24=x25.value;
j0array[0]=((-1.0)*x24);
sj0array[0]=IKsin(j0array[0]);
cj0array[0]=IKcos(j0array[0]);
j0array[1]=((3.14159265358979)+(((-1.0)*x24)));
sj0array[1]=IKsin(j0array[1]);
cj0array[1]=IKcos(j0array[1]);
if( j0array[0] > IKPI )
{
    j0array[0]-=IK2PI;
}
else if( j0array[0] < -IKPI )
{    j0array[0]+=IK2PI;
}
j0valid[0] = true;
if( j0array[1] > IKPI )
{
    j0array[1]-=IK2PI;
}
else if( j0array[1] < -IKPI )
{    j0array[1]+=IK2PI;
}
j0valid[1] = true;
for(int ij0 = 0; ij0 < 2; ++ij0)
{
if( !j0valid[ij0] )
{
    continue;
}
_ij0[0] = ij0; _ij0[1] = -1;
for(int iij0 = ij0+1; iij0 < 2; ++iij0)
{
if( j0valid[iij0] && IKabs(cj0array[ij0]-cj0array[iij0]) < IKFAST_SOLUTION_THRESH && IKabs(sj0array[ij0]-sj0array[iij0]) < IKFAST_SOLUTION_THRESH )
{
    j0valid[iij0]=false; _ij0[1] = iij0; break; 
}
}
j0 = j0array[ij0]; cj0 = cj0array[ij0]; sj0 = sj0array[ij0];

IkReal op[162], zeror[48];
int numroots;;
IkReal x26=IKcos(j0);
IkReal x27=((0.409)*pz);
IkReal x28=((0.024)*px);
IkReal x29=IKsin(j0);
IkReal x30=((0.101432)+(((-0.496)*pz)));
IkReal x31=((-0.2045)+pz);
IkReal x32=((2.0)*pz);
IkReal x33=((0.153)+(((-1.0)*x32)));
IkReal x34=((0.103)*pz);
IkReal x35=((0.496)*pz);
IkReal x36=((-0.103)+(((-1.0)*x32)));
IkReal x37=((-0.0515)+(((-1.0)*pz)));
IkReal x38=((1.0)*(px*px));
IkReal x39=((1.0)*(py*py));
IkReal x40=((1.0)*(pz*pz));
IkReal x41=((0.012)*x26);
IkReal x42=((-0.409)+x32);
IkReal x43=(py*x29);
IkReal x44=(px*x26);
IkReal x45=((2.0)*x26);
IkReal x46=((0.006144)*x26);
IkReal x47=((-0.153)+x32);
IkReal x48=((0.000576)*x26);
IkReal x49=((0.003552)*x26);
IkReal x50=((0.025544)+x35);
IkReal x51=((0.0024)*x26);
IkReal x52=((0.048)*x43);
IkReal x53=((0.048)*x44);
IkReal x54=((-1.0)*(IKcos(r00)));
IkReal x55=((-2.0)*x26);
IkReal x56=((0.512)*x44);
IkReal x57=((0.512)*x43);
IkReal x58=((0.296)*x43);
IkReal x59=((0.296)*x44);
IkReal x60=((0.2)*x43);
IkReal x61=((0.2)*x44);
IkReal x62=(x27+x28);
IkReal x63=(x48+x28);
IkReal x64=(x44+x43);
IkReal x65=((((1.0)*x44))+(((1.0)*x43)));
IkReal x66=(x57+x56);
IkReal x67=(x60+x61);
IkReal x68=(x53+x52);
IkReal x69=(x59+x58);
IkReal x70=((0.274)+(((-1.0)*x41))+x64);
IkReal x71=((0.022)+(((-1.0)*x41))+x64);
IkReal x72=(x39+x38+x40);
IkReal x73=((((-0.992)*x43))+(((-0.992)*x44))+(((0.011904)*x26)));
IkReal x74=((((4.0)*x43))+(((4.0)*x44))+(((-0.048)*x26)));
IkReal x75=((((-0.024)*x26))+(((2.0)*x44))+(((2.0)*x43)));
IkReal x76=((0.274)+x41+(((-1.0)*x65)));
IkReal x77=((0.022)+x41+(((-1.0)*x65)));
IkReal x78=(x72+x34);
IkReal x79=(x72+x48);
IkReal x80=((0.007344)+(((-0.096)*pz))+(((-1.0)*x46))+x66);
IkReal x81=((0.045288)+(((-0.592)*pz))+(((-1.0)*x46))+x66);
IkReal x82=((-0.0306)+(((0.4)*pz))+(((-1.0)*x46))+x66);
IkReal x83=(x78+x68);
IkReal x84=((0.01995975)+(((-1.0)*x79))+x62+x68);
IkReal x85=((-0.04253625)+(((-1.0)*x79))+x62+x68);
IkReal x86=((-0.04799225)+(((-1.0)*x72))+(((-1.0)*x49))+x62+x69);
IkReal x87=((-0.03608825)+(((-1.0)*x72))+x51+x62+(((-1.0)*x67)));
IkReal x88=((0.05912775)+x63+(((-1.0)*x83)));
IkReal x89=((-0.00336825)+x63+(((-1.0)*x83)));
IkReal x90=((-0.00882425)+(((-1.0)*x78))+x49+x28+(((-1.0)*x69)));
IkReal x91=((0.00307975)+(((-1.0)*x78))+x67+x28+(((-1.0)*x51)));
op[0]=x84;
op[1]=0;
op[2]=x85;
op[3]=x84;
op[4]=0;
op[5]=x85;
op[6]=0;
op[7]=0;
op[8]=0;
op[9]=x86;
op[10]=x86;
op[11]=0;
op[12]=x30;
op[13]=x30;
op[14]=0;
op[15]=x87;
op[16]=x87;
op[17]=0;
op[18]=x76;
op[19]=0;
op[20]=x77;
op[21]=0;
op[22]=-0.504;
op[23]=0;
op[24]=((-0.226)+x41+(((-1.0)*x65)));
op[25]=0;
op[26]=((0.026)+x41+(((-1.0)*x65)));
op[27]=x76;
op[28]=0;
op[29]=x77;
op[30]=x42;
op[31]=0;
op[32]=x42;
op[33]=((0.226)+(((-1.0)*x41))+x64);
op[34]=0;
op[35]=((-0.026)+(((-1.0)*x41))+x64);
op[36]=x31;
op[37]=0.252;
op[38]=x31;
op[39]=0.5;
op[40]=0;
op[41]=-0.004;
op[42]=x31;
op[43]=-0.252;
op[44]=x31;
op[45]=x54;
op[46]=x45;
op[47]=x54;
op[48]=x45;
op[49]=0;
op[50]=x55;
op[51]=x54;
op[52]=x55;
op[53]=x54;
op[54]=x80;
op[55]=0;
op[56]=x80;
op[57]=x80;
op[58]=0;
op[59]=x80;
op[60]=0;
op[61]=0;
op[62]=0;
op[63]=x81;
op[64]=x81;
op[65]=0;
op[66]=x73;
op[67]=x73;
op[68]=0;
op[69]=x82;
op[70]=x82;
op[71]=0;
op[72]=x47;
op[73]=0;
op[74]=x47;
op[75]=0;
op[76]=0;
op[77]=0;
op[78]=x47;
op[79]=0;
op[80]=x47;
op[81]=x47;
op[82]=0;
op[83]=x47;
op[84]=x74;
op[85]=0;
op[86]=x74;
op[87]=x33;
op[88]=0;
op[89]=x33;
op[90]=x75;
op[91]=0;
op[92]=x75;
op[93]=0;
op[94]=0;
op[95]=0;
op[96]=x75;
op[97]=0;
op[98]=x75;
op[99]=x45;
op[100]=0;
op[101]=x55;
op[102]=0;
op[103]=((-8.0)*x26);
op[104]=0;
op[105]=x55;
op[106]=0;
op[107]=x45;
op[108]=x88;
op[109]=0;
op[110]=x89;
op[111]=x88;
op[112]=0;
op[113]=x89;
op[114]=0;
op[115]=0;
op[116]=0;
op[117]=x90;
op[118]=x90;
op[119]=0;
op[120]=x50;
op[121]=x50;
op[122]=0;
op[123]=x91;
op[124]=x91;
op[125]=0;
op[126]=x70;
op[127]=0;
op[128]=x71;
op[129]=0;
op[130]=-0.504;
op[131]=0;
op[132]=((-0.226)+(((-1.0)*x41))+x64);
op[133]=0;
op[134]=((0.026)+(((-1.0)*x41))+x64);
op[135]=x70;
op[136]=0;
op[137]=x71;
op[138]=x36;
op[139]=0;
op[140]=x36;
op[141]=((0.226)+x41+(((-1.0)*x65)));
op[142]=0;
op[143]=((-0.026)+x41+(((-1.0)*x65)));
op[144]=x37;
op[145]=0.252;
op[146]=x37;
op[147]=0.5;
op[148]=0;
op[149]=-0.004;
op[150]=x37;
op[151]=-0.252;
op[152]=x37;
op[153]=x54;
op[154]=x55;
op[155]=x54;
op[156]=x55;
op[157]=0;
op[158]=x45;
op[159]=x54;
op[160]=x45;
op[161]=x54;
solvedialyticpoly12qep(op,zeror,numroots);
IkReal j1array[16], cj1array[16], sj1array[16], j2array[16], cj2array[16], sj2array[16], j3array[16], cj3array[16], sj3array[16];
int numsolutions = 0;
for(int ij1 = 0; ij1 < numroots; ij1 += 3)
{
IkReal htj1 = zeror[ij1+0], htj2 = zeror[ij1+1], htj3 = zeror[ij1+2];
if(isnan(htj1)||isnan(htj2)||isnan(htj3)){
continue;
}
j1array[numsolutions]=((2.0)*(atan(htj1)));
j2array[numsolutions]=((2.0)*(atan(htj2)));
j3array[numsolutions]=((2.0)*(atan(htj3)));
if(isinf(htj1)){
cj1array[numsolutions] = IKcos(j1array[numsolutions]);
sj1array[numsolutions] = IKsin(j1array[numsolutions]);
}
else{
IkReal x92=htj1*htj1;
CheckValue<IkReal> x93=IKPowWithIntegerCheck(((1.0)+x92),-1);
if(!x93.valid){
continue;
}
cj1array[numsolutions]=((x93.value)*(((1.0)+(((-1.0)*x92)))));
CheckValue<IkReal> x94=IKPowWithIntegerCheck(((1.0)+(htj1*htj1)),-1);
if(!x94.valid){
continue;
}
sj1array[numsolutions]=((2.0)*htj1*(x94.value));
}
if(isinf(htj2)){
cj2array[numsolutions] = IKcos(j2array[numsolutions]);
sj2array[numsolutions] = IKsin(j2array[numsolutions]);
}
else{
IkReal x95=htj2*htj2;
CheckValue<IkReal> x96=IKPowWithIntegerCheck(((1.0)+x95),-1);
if(!x96.valid){
continue;
}
cj2array[numsolutions]=((x96.value)*(((1.0)+(((-1.0)*x95)))));
CheckValue<IkReal> x97=IKPowWithIntegerCheck(((1.0)+(htj2*htj2)),-1);
if(!x97.valid){
continue;
}
sj2array[numsolutions]=((2.0)*htj2*(x97.value));
}
if(isinf(htj3)){
cj3array[numsolutions] = IKcos(j3array[numsolutions]);
sj3array[numsolutions] = IKsin(j3array[numsolutions]);
}
else{
IkReal x98=htj3*htj3;
CheckValue<IkReal> x99=IKPowWithIntegerCheck(((1.0)+x98),-1);
if(!x99.valid){
continue;
}
cj3array[numsolutions]=((x99.value)*(((1.0)+(((-1.0)*x98)))));
CheckValue<IkReal> x100=IKPowWithIntegerCheck(((1.0)+(htj3*htj3)),-1);
if(!x100.valid){
continue;
}
sj3array[numsolutions]=((2.0)*htj3*(x100.value));
}
if( j1array[numsolutions] > IKPI )
{
    j1array[numsolutions]-=IK2PI;
}
else if( j1array[numsolutions] < -IKPI )
{
    j1array[numsolutions]+=IK2PI;
}
if( j2array[numsolutions] > IKPI )
{
    j2array[numsolutions]-=IK2PI;
}
else if( j2array[numsolutions] < -IKPI )
{
    j2array[numsolutions]+=IK2PI;
}
if( j3array[numsolutions] > IKPI )
{
    j3array[numsolutions]-=IK2PI;
}
else if( j3array[numsolutions] < -IKPI )
{
    j3array[numsolutions]+=IK2PI;
}
numsolutions++;
}
bool j1valid[16]={true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true};
_nj1 = 16;
_nj2 = 1;
_nj3 = 1;
for(int ij1 = 0; ij1 < numsolutions; ++ij1)
    {
if( !j1valid[ij1] )
{
    continue;
}
_ij1[0] = ij1; _ij1[1] = -1;
_ij2[0] = 0; _ij2[1] = -1;
_ij3[0] = 0; _ij3[1] = -1;
for(int iij1 = ij1+1; iij1 < numsolutions; ++iij1)
{
if( !j1valid[iij1] ) { continue; }
if( IKabs(cj1array[ij1]-cj1array[iij1]) < IKFAST_SOLUTION_THRESH && IKabs(sj1array[ij1]-sj1array[iij1]) < IKFAST_SOLUTION_THRESH && IKabs(cj2array[ij1]-cj2array[iij1]) < IKFAST_SOLUTION_THRESH && IKabs(sj2array[ij1]-sj2array[iij1]) < IKFAST_SOLUTION_THRESH && IKabs(cj3array[ij1]-cj3array[iij1]) < IKFAST_SOLUTION_THRESH && IKabs(sj3array[ij1]-sj3array[iij1]) < IKFAST_SOLUTION_THRESH &&  1 )
{
    j1valid[iij1]=false; _ij1[1] = iij1; _ij2[1] = 0; _ij3[1] = 0;  break; 
}
}
    j1 = j1array[ij1]; cj1 = cj1array[ij1]; sj1 = sj1array[ij1];

    j2 = j2array[ij1]; cj2 = cj2array[ij1]; sj2 = sj2array[ij1];

    j3 = j3array[ij1]; cj3 = cj3array[ij1]; sj3 = sj3array[ij1];

{
std::vector<IkSingleDOFSolutionBase<IkReal> > vinfos(4);
vinfos[0].jointtype = 1;
vinfos[0].foffset = j0;
vinfos[0].indices[0] = _ij0[0];
vinfos[0].indices[1] = _ij0[1];
vinfos[0].maxsolutions = _nj0;
vinfos[1].jointtype = 1;
vinfos[1].foffset = j1;
vinfos[1].indices[0] = _ij1[0];
vinfos[1].indices[1] = _ij1[1];
vinfos[1].maxsolutions = _nj1;
vinfos[2].jointtype = 1;
vinfos[2].foffset = j2;
vinfos[2].indices[0] = _ij2[0];
vinfos[2].indices[1] = _ij2[1];
vinfos[2].maxsolutions = _nj2;
vinfos[3].jointtype = 1;
vinfos[3].foffset = j3;
vinfos[3].indices[0] = _ij3[0];
vinfos[3].indices[1] = _ij3[1];
vinfos[3].maxsolutions = _nj3;
std::vector<int> vfree(0);
solutions.AddSolution(vinfos,vfree);
}
    }
}
}
}
return solutions.GetNumSolutions()>0;
}

/// \brief Solve the det Ax^2+Bx+C = 0 problem using the Manocha and Canny method (1994)
///
/// matcoeffs is of length 54*3, for 3 matrices
static inline void solvedialyticpoly12qep(const IkReal* matcoeffs, IkReal* rawroots, int& numroots)
{
    const IkReal tol = 128.0*std::numeric_limits<IkReal>::epsilon();
    IkReal IKFAST_ALIGNED16(M[24*24]) = {0};
    IkReal IKFAST_ALIGNED16(A[12*12]);
    IkReal IKFAST_ALIGNED16(work[24*24*23]);
    int ipiv[12];
    int info, coeffindex;
    const int worksize=24*24*23;
    const int matrixdim = 12;
    const int matrixdim2 = 24;
    numroots = 0;
    // first setup M = [0 I; -C -B] and A
    coeffindex = 0;
    for(int j = 0; j < 6; ++j) {
        for(int k = 0; k < 9; ++k) {
            M[matrixdim+(j+6)+2*matrixdim*k] = M[matrixdim+j+2*matrixdim*(k+3)] = -matcoeffs[coeffindex++];
        }
    }
    for(int j = 0; j < 6; ++j) {
        for(int k = 0; k < 9; ++k) {
            M[matrixdim+(j+6)+2*matrixdim*k+matrixdim*2*matrixdim] = M[matrixdim+j+2*matrixdim*(k+3)+matrixdim*2*matrixdim] = -matcoeffs[coeffindex++];
        }
    }
    for(int j = 0; j < 6; ++j) {
        for(int k = 0; k < 9; ++k) {
            A[(j+6)+matrixdim*k] = A[j+matrixdim*(k+3)] = matcoeffs[coeffindex++];
        }
        for(int k = 0; k < 3; ++k) {
            A[j+matrixdim*k] = A[(j+6)+matrixdim*(k+9)] = 0;
        }
    }
    const IkReal lfpossibilities[4][4] = {{1,-1,1,1},{1,0,-2,1},{1,1,2,0},{1,-1,4,1}};
    int lfindex = -1;
    bool bsingular = true;
    do {
        dgetrf_(&matrixdim,&matrixdim,A,&matrixdim,&ipiv[0],&info);
        if( info == 0 ) {
            bsingular = false;
            for(int j = 0; j < matrixdim; ++j) {
                if( IKabs(A[j*matrixdim+j]) < 100*tol ) {
                    bsingular = true;
                    break;
                }
            }
            if( !bsingular ) {
                break;
            }
        }
        if( lfindex == 3 ) {
            break;
        }
        // transform by the linear functional
        lfindex++;
        const IkReal* lf = lfpossibilities[lfindex];
        // have to reinitialize A
        coeffindex = 0;
        for(int j = 0; j < 6; ++j) {
            for(int k = 0; k < 9; ++k) {
                IkReal a = matcoeffs[coeffindex+108], b = matcoeffs[coeffindex+54], c = matcoeffs[coeffindex];
                A[(j+6)+matrixdim*k] = A[j+matrixdim*(k+3)] = lf[0]*lf[0]*a+lf[0]*lf[2]*b+lf[2]*lf[2]*c;
                M[matrixdim+(j+6)+2*matrixdim*k] = M[matrixdim+j+2*matrixdim*(k+3)] = -(lf[1]*lf[1]*a + lf[1]*lf[3]*b + lf[3]*lf[3]*c);
                M[matrixdim+(j+6)+2*matrixdim*k+matrixdim*2*matrixdim] = M[matrixdim+j+2*matrixdim*(k+3)+matrixdim*2*matrixdim] = -(2*lf[0]*lf[1]*a + (lf[0]*lf[3]+lf[1]*lf[2])*b + 2*lf[2]*lf[3]*c);
                coeffindex++;
            }
            for(int k = 0; k < 3; ++k) {
                A[j+matrixdim*k] = A[(j+6)+matrixdim*(k+9)] = 0;
            }
        }
    } while(lfindex<4);

    if( bsingular ) {
        return;
    }
    dgetrs_("No transpose", &matrixdim, &matrixdim2, A, &matrixdim, &ipiv[0], &M[matrixdim], &matrixdim2, &info);
    if( info != 0 ) {
        return;
    }

    // set identity in upper corner
    for(int j = 0; j < matrixdim; ++j) {
        M[matrixdim*2*matrixdim+j+matrixdim*2*j] = 1;
    }
    IkReal IKFAST_ALIGNED16(wr[24]);
    IkReal IKFAST_ALIGNED16(wi[24]);
    IkReal IKFAST_ALIGNED16(vr[24*24]);
    int one=1;
    dgeev_("N", "V", &matrixdim2, M, &matrixdim2, wr, wi,NULL, &one, vr, &matrixdim2, work, &worksize, &info);
    if( info != 0 ) {
        return;
    }
    IkReal Breal[matrixdim-1];
    for(int i = 0; i < matrixdim2; ++i) {
        if( IKabs(wi[i]) < tol*100 ) {
            IkReal* ev = vr+matrixdim2*i;
            if( IKabs(wr[i]) > 1 ) {
                ev += matrixdim;
            }
            // consistency has to be checked!!
            if( IKabs(ev[0]) < tol ) {
                continue;
            }
            IkReal iconst = 1/ev[0];
            for(int j = 1; j < matrixdim; ++j) {
                Breal[j-1] = ev[j]*iconst;
            }
            if( checkconsistency12(Breal) ) {
                if( lfindex >= 0 ) {
                    const IkReal* lf = lfpossibilities[lfindex];
                    rawroots[numroots++] = (wr[i]*lf[0]+lf[1])/(wr[i]*lf[2]+lf[3]);
                }
                else {
                    rawroots[numroots++] = wr[i];
                }
                bool bsmall0=IKabs(ev[0]) > IKabs(ev[3]);
                bool bsmall1=IKabs(ev[0]) > IKabs(ev[1]);
                if( bsmall0 && bsmall1 ) {
                    rawroots[numroots++] = ev[3]/ev[0];
                    rawroots[numroots++] = ev[1]/ev[0];
                }
                else if( bsmall0 && !bsmall1 ) {
                    rawroots[numroots++] = ev[5]/ev[2];
                    rawroots[numroots++] = ev[2]/ev[1];
                }
                else if( !bsmall0 && bsmall1 ) {
                    rawroots[numroots++] = ev[9]/ev[6];
                    rawroots[numroots++] = ev[10]/ev[9];
                }
                else if( !bsmall0 && !bsmall1 ) {
                    rawroots[numroots++] = ev[11]/ev[8];
                    rawroots[numroots++] = ev[11]/ev[10];
                }
            }
        }
    }
}
static inline bool checkconsistency12(const IkReal* Breal)
{
    IkReal norm = 0.1;
    for(int i = 0; i < 11; ++i) {
        norm += IKabs(Breal[i]);
    }
    IkReal tol = 1e-6*norm; // have to increase the threshold since many computations are involved
    return IKabs(Breal[0]*Breal[0]-Breal[1]) < tol && IKabs(Breal[0]*Breal[2]-Breal[3]) < tol && IKabs(Breal[1]*Breal[2]-Breal[4]) < tol && IKabs(Breal[2]*Breal[2]-Breal[5]) < tol && IKabs(Breal[0]*Breal[5]-Breal[6]) < tol && IKabs(Breal[1]*Breal[5]-Breal[7]) < tol && IKabs(Breal[2]*Breal[5]-Breal[8]) < tol && IKabs(Breal[0]*Breal[8]-Breal[9]) < tol && IKabs(Breal[1]*Breal[8]-Breal[10]) < tol;
}
};


/// solves the inverse kinematics equations.
/// \param pfree is an array specifying the free joints of the chain.
IKFAST_API bool ComputeIk(const IkReal* eetrans, const IkReal* eerot, const IkReal* pfree, IkSolutionListBase<IkReal>& solutions) {
IKSolver solver;
return solver.ComputeIk(eetrans,eerot,pfree,solutions);
}

IKFAST_API bool ComputeIk2(const IkReal* eetrans, const IkReal* eerot, const IkReal* pfree, IkSolutionListBase<IkReal>& solutions, void* pOpenRAVEManip) {
IKSolver solver;
return solver.ComputeIk(eetrans,eerot,pfree,solutions);
}

IKFAST_API const char* GetKinematicsHash() { return "2062b4aa7fde98f002602493c9ba542a"; }

IKFAST_API const char* GetIkFastVersion() { return "0x10000049"; }

#ifdef IKFAST_NAMESPACE
} // end namespace
#endif

#ifndef IKFAST_NO_MAIN
#include <stdio.h>
#include <stdlib.h>
#ifdef IKFAST_NAMESPACE
using namespace IKFAST_NAMESPACE;
#endif
int main(int argc, char** argv)
{
    if( argc != 12+GetNumFreeParameters()+1 ) {
        printf("\nUsage: ./ik r00 r01 r02 t0 r10 r11 r12 t1 r20 r21 r22 t2 free0 ...\n\n"
               "Returns the ik solutions given the transformation of the end effector specified by\n"
               "a 3x3 rotation R (rXX), and a 3x1 translation (tX).\n"
               "There are %d free parameters that have to be specified.\n\n",GetNumFreeParameters());
        return 1;
    }

    IkSolutionList<IkReal> solutions;
    std::vector<IkReal> vfree(GetNumFreeParameters());
    IkReal eerot[9],eetrans[3];
    eerot[0] = atof(argv[1]); eerot[1] = atof(argv[2]); eerot[2] = atof(argv[3]); eetrans[0] = atof(argv[4]);
    eerot[3] = atof(argv[5]); eerot[4] = atof(argv[6]); eerot[5] = atof(argv[7]); eetrans[1] = atof(argv[8]);
    eerot[6] = atof(argv[9]); eerot[7] = atof(argv[10]); eerot[8] = atof(argv[11]); eetrans[2] = atof(argv[12]);
    for(std::size_t i = 0; i < vfree.size(); ++i)
        vfree[i] = atof(argv[13+i]);
    bool bSuccess = ComputeIk(eetrans, eerot, vfree.size() > 0 ? &vfree[0] : NULL, solutions);

    if( !bSuccess ) {
        fprintf(stderr,"Failed to get ik solution\n");
        return -1;
    }

    printf("Found %d ik solutions:\n", (int)solutions.GetNumSolutions());
    std::vector<IkReal> solvalues(GetNumJoints());
    for(std::size_t i = 0; i < solutions.GetNumSolutions(); ++i) {
        const IkSolutionBase<IkReal>& sol = solutions.GetSolution(i);
        printf("sol%d (free=%d): ", (int)i, (int)sol.GetFree().size());
        std::vector<IkReal> vsolfree(sol.GetFree().size());
        sol.GetSolution(&solvalues[0],vsolfree.size()>0?&vsolfree[0]:NULL);
        for( std::size_t j = 0; j < solvalues.size(); ++j)
            printf("%.15f, ", solvalues[j]);
        printf("\n");
    }
    return 0;
}

#endif
