#include <cstdio>
#include <cstring>
#include <cassert>
#include <cmath>

#include "image.h"

using namespace std;

// HW0 #3
// const Image& im: input image
// return the corresponding grayscale image
Image rgb_to_grayscale(const Image& im)
  {
  assert(im.c == 3); // only accept RGB images
  Image gray(im.w,im.h,1); // create a new grayscale image (note: 1 channel)
  
  // TODO: calculate the pixels of 'gray'
  
  for(int q2=0;q2<im.h;q2++)for(int q1=0;q1<im.w;q1++)
    {
    float a=0;
    a+=im(q1,q2,0)*0.299f;
    a+=im(q1,q2,1)*0.587f;
    a+=im(q1,q2,2)*0.114f;
    gray(q1,q2)=a;
    }
  return gray;
  }



// Example function that changes the color of a grayscale image
Image grayscale_to_rgb(const Image& im, float r, float g, float b)
  {
  assert(im.c == 1);
  Image rgb(im.w,im.h,3);
  
  for(int q2=0;q2<im.h;q2++)for(int q1=0;q1<im.w;q1++)
    {
    rgb(q1,q2,0)=r*im(q1,q2);
    rgb(q1,q2,1)=g*im(q1,q2);
    rgb(q1,q2,2)=b*im(q1,q2);
    }
  
  return rgb;
  }




// HW0 #4
// Image& im: input image to be modified in-place
// int c: which channel to shift
// float v: how much to shift
void shift_image(Image& im, int c, float v)
  {
  assert(c>=0 && c<im.c); // needs to be a valid channel
  
  // TODO: shift all the pixels at the specified channel
  
  for(int q2=0;q2<im.h;q2++)for(int q1=0;q1<im.w;q1++)
    im(q1,q2,c)+=v;
  }

// HW0 #8
// Image& im: input image to be modified in-place
// int c: which channel to scale
// float v: how much to scale
void scale_image(Image& im, int c, float v)
  {
  assert(c>=0 && c<im.c); // needs to be a valid channel
  
  // TODO: scale all the pixels at the specified channel
  
  for(int q2=0;q2<im.h;q2++)for(int q1=0;q1<im.w;q1++)
    im(q1,q2,c)*=v;
  }


// HW0 #5
// Image& im: input image to be modified in-place
void clamp_image(Image& im)
  {
  // TODO: clamp all the pixels in all channel to be between 0 and 1
  
  for(int q2=0;q2<im.h;q2++)for(int q1=0;q1<im.w;q1++)
    for(int q3=0;q3<im.c;q3++)
      {
      float a=im(q1,q2,q3);
      if(a<0)a=0;
      if(a>1)a=1;
      im(q1,q2,q3)=a;
      }
  }

// These might be handy
float max(float a, float b, float c)
  {
  return max({a,b,c});
  }

float min(float a, float b, float c)
  {
  return min({a,b,c});
  }


// HW0 #6
// Image& im: input image to be modified in-place
void rgb_to_hsv(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
  
  // TODO: Convert all pixels from RGB format to HSV format
  
  for(int q2=0;q2<im.h;q2++)for(int q1=0;q1<im.w;q1++)
    {
    float r=im(q1,q2,0);
    float g=im(q1,q2,1);
    float b=im(q1,q2,2);
    
    float V=max(r,g,b);
    float m=min(r,g,b);
    float c=V-m;
    float S=V==0?0:c/V;
    
    float H=0;
    
    if(c==0)H=0;
    else if(V==r)H=(g-b)/c+0;
    else if(V==g)H=(b-r)/c+2;
    else if(V==b)H=(r-g)/c+4;
    
    if(H<0)H+=6;
    H/=6;
    
    im(q1,q2,0)=H;
    im(q1,q2,1)=S;
    im(q1,q2,2)=V;
    }
  }

// HW0 #7
// Image& im: input image to be modified in-place
void hsv_to_rgb(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
  
  // TODO: Convert all pixels from HSV format to RGB format
  
  for(int q2=0;q2<im.h;q2++)for(int q1=0;q1<im.w;q1++)
    {
    float h=im(q1,q2,0);
    float s=im(q1,q2,1);
    float v=im(q1,q2,2);
    
    float c=s*v;
    float m=v-c;
    
    float r=0,g=0,b=0;
    
    h*=6;
    if(h>5)h-=6;
    
         if(h<=0){h-=0;r=v;g=m;b=m-h*c;}
    else if(h<=1){h-=0;r=v;b=m;g=m+h*c;}
    else if(h<=2){h-=2;g=v;b=m;r=m-h*c;}
    else if(h<=3){h-=2;g=v;r=m;b=m+h*c;}
    else if(h<=4){h-=4;b=v;r=m;g=m-h*c;}
    else if(h<=5){h-=4;b=v;g=m;r=m+h*c;}
    else {printf("error %d %d  %f\n",q1,q2,h);}
    
    im(q1,q2,0)=r;
    im(q1,q2,1)=g;
    im(q1,q2,2)=b;
    
    
    }
  }


struct RGBcolor { float r,g,b; };
struct XYZcolor { float x,y,z; };
struct LCHcolor { float l,c,h; };

float l2g(float a)
  {
  if(a<0.0031308f)return 12.92f*a;
  else return 1.055f*powf(a,1.0f/2.4f)-0.055f;
  }

float g2l(float a)
  {
  if(a<0.04045f)return a/12.92f;
  else return powf((a+0.055f)/1.055f,2.4f);
  }

RGBcolor linear2gamma(RGBcolor a)
  {
  a.r=l2g(a.r);
  a.g=l2g(a.g);
  a.b=l2g(a.b);
  return a;
  }

RGBcolor gamma2linear(RGBcolor a)
  {
  a.r=g2l(a.r);
  a.g=g2l(a.g);
  a.b=g2l(a.b);
  return a;
  }

XYZcolor toXYZ(RGBcolor a)
  {
  XYZcolor b;
  a=gamma2linear(a);
  b.x=0.412383f*a.r+0.357585f*a.g+0.180480f*a.b;
  b.y=0.212635f*a.r+0.715170f*a.g+0.072192f*a.b;
  b.z=0.019330f*a.r+0.119195f*a.g+0.950528f*a.b;
  return b;
  }


RGBcolor toRGB(XYZcolor a)
  {
  RGBcolor b;
  b.r=(+3.241030f)*a.x+(-1.537410f)*a.y +(-0.498620f)*a.z;
  b.g=(-0.969242f)*a.x+(+1.875960f)*a.y +(+0.041555f)*a.z;
  b.b=(+0.055632f)*a.x+(-0.203979f)*a.y +(+1.056980f)*a.z;
  b=linear2gamma(b);
  return b;
  }

LCHcolor rgb2lch(RGBcolor a)
  {
  LCHcolor b={0.f,0.f,0.f};
  XYZcolor c=toXYZ(a);
  
  if(c.x==0.f && c.y==0.f && c.z==0.f)return b;
  
  //printf("xyz1   %f %f %f\n",c.x,c.y,c.z);
  
  float u1=4*c.x/(1*c.x+15*c.y+3*c.z);
  float v1=9*c.y/(1*c.x+15*c.y+3*c.z);
  
  
  float un=0.2009f;
  float vn=0.4610f;
  
  float cutoff=powf(6.f/29.f,3);
  
  float l=0;
  if(c.y<=cutoff)l=powf(29.f/3.f,3)*c.y;
  else l=116.f*powf(c.y,1.f/3.f)-16.f;
  float u=13.f*l*(u1-un);
  float v=13.f*l*(v1-vn);
  
  
  b.l=l;
  b.c=sqrtf(u*u+v*v);
  b.h=atan2f(u,v);
  
  return b;
  }

RGBcolor lch2rgb(LCHcolor a)
  {
  XYZcolor b={0.f,0.f,0.f};
  
  if(a.l==0.f && a.c==0.f && a.h==0.f)return toRGB(b);
  
  float u=a.c*sinf(a.h);
  float v=a.c*cosf(a.h);
  float l=a.l;
  
  
  
  float un=0.2009f;
  float vn=0.4610f;
  
  float cutoff=8;
  
  
  float u1=u/(13.f*l)+un;
  float v1=v/(13.f*l)+vn;
  
  if(l<=cutoff)b.y=l*powf(3.f/29.f,3);
  else b.y=powf((l+16.f)/116.f,3);
  
  b.x=b.y*(9*u1)/(4*v1);
  b.z=b.y*(12-3*u1-20*v1)/(4*v1);
  
  //printf("xyz2   %f %f %f\n",b.x,b.y,b.z);
  
  return toRGB(b);
  }

// HW0 #9
// Image& im: input image to be modified in-place
void rgb_to_lch(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
  
  // TODO: Convert all pixels from RGB format to LCH format
  
  for(int q2=0;q2<im.h;q2++)for(int q1=0;q1<im.w;q1++)
    {
    RGBcolor a;
    a.r=im(q1,q2,0);
    a.g=im(q1,q2,1);
    a.b=im(q1,q2,2);
    
    LCHcolor b=rgb2lch(a);
    
    if(isnan(b.c)){printf("%f %f %f\n",a.r,a.g,a.b);printf("%f %f %f\n",b.l,b.c,b.h);}
    if(isnan(b.h)){printf("%f %f %f\n",a.r,a.g,a.b);printf("%f %f %f\n",b.l,b.c,b.h);}
    if(isnan(b.l)){printf("%f %f %f\n",a.r,a.g,a.b);printf("%f %f %f\n",b.l,b.c,b.h);}
    
    im(q1,q2,0)=b.l;
    im(q1,q2,1)=b.c;
    im(q1,q2,2)=b.h;
    }
  }

// HW0 #9
// Image& im: input image to be modified in-place
void lch_to_rgb(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
  
  // TODO: Convert all pixels from LCH format to RGB format
  
  for(float q1=0.f;q1<=1.f;q1+=0.1f)
  for(float q2=0.f;q2<=1.f;q2+=0.1f)
  for(float q3=0.f;q3<=1.f;q3+=0.1f)
    {
    RGBcolor a={q1,q2,q3};
    LCHcolor b=rgb2lch(a);
    RGBcolor c=lch2rgb(b);
    
    float err=0;
    err+=fabsf(c.r-a.r);
    err+=fabsf(c.g-a.g);
    err+=fabsf(c.b-a.b);
    
    if(err>0.0001f)
      {
      printf("%f %f %f\n",a.r,a.g,a.b);
      printf("%f %f %f\n",b.l,b.c,b.h);
      printf("%f %f %f\n",c.r,c.g,c.b);
      }
    }
  
  for(int q2=0;q2<im.h;q2++)for(int q1=0;q1<im.w;q1++)
    {
    LCHcolor a;
    a.l=im(q1,q2,0);
    a.c=im(q1,q2,1);
    a.h=im(q1,q2,2);
    
    RGBcolor b=lch2rgb(a);
    
    im(q1,q2,0)=b.r;
    im(q1,q2,1)=b.g;
    im(q1,q2,2)=b.b;
    
    }
  }



// Implementation of member functions
void Image::clamp(void) { clamp_image(*this); }
void Image::shift(int c, float v) { shift_image(*this,c,v); }
void Image::scale(int c, float v) { scale_image(*this,c,v); }

void Image::HSVtoRGB(void) { hsv_to_rgb(*this); }
void Image::RGBtoHSV(void) { rgb_to_hsv(*this); }
void Image::LCHtoRGB(void) { lch_to_rgb(*this); }
void Image::RGBtoLCH(void) { rgb_to_lch(*this); }
