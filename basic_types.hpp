/**
  * simple math utilities and
  * data structs that are POD only
  *
  *
  */

#ifndef BASIC_TYPES_HPP
#define BASIC_TYPES_HPP

#include <cstdint>

namespace Engine
{

typedef uint32_t Flags;
typedef uint16_t BlendingMode;
typedef uint8_t ErrorCode;
typedef float ColourComponent;
typedef float SpatialDimention;


enum eSpace
{
  SPACE_2D,
  SPACE_3D,
};


struct Vector3
{
  SpatialDimention x;
  SpatialDimention y;
  // depending on kind of space this might be
  // either the 3rd axis or the z-order.
  // in case it is z-order it is casted to integer.
  SpatialDimention z;
};


struct Rotation3
{
  // rotation
  SpatialDimention angleXY;
  // these next 2 angles are used only in 3d
  SpatialDimention angleYZ;
  SpatialDimention angleZX;
};


struct Dimension3
{
  SpatialDimention w; // width
  SpatialDimention h; // height
  SpatialDimention d; // depth
};


enum eBound
{
  CIRCLE,
  BOX_TLBR,
  BOX_XYWH,
  // MULTICIRCLE,
  // MULTIBOX,
  // POLYGON,
  // HASH,
};

struct CircleBound
{
  SpatialDimention radius;
};

struct BoxBoundTLBR
{
  Vector3 topLeft;
  Vector3 bottonRight;
};

struct BoxBoundXYWH
{
  Vector3 topLeft;
  Dimension3 size;
};


// #define POLYGON_BOUND_MAX_VERTICES 16
// struct PolygonBound
// {
//   unsigned char numOfVertice;
//   Vector3 * vertices[POLYGON_BOUND_MAX_VERTICES];
// };

// #define MULTICIRCLE_BOUND_MAX_CIRCLES 8
// struct MultiCircleBound
// {
//   unsigned char numOfCircles;
//   CircleBound * bounds[MULTIBOX_BOUND_MAX_BOXES];
// };

// #define MULTIBOX_BOUND_MAX_BOXES 8
// struct MultiBoxBound
// {
//   unsigned char numOfBoxes;
//   BoxBoundTLBR * bounds[MULTIBOX_BOUND_MAX_BOXES];
// };



struct Bound
{
  eBound kind;
  union
  {
    CircleBound  circleBound;
    BoxBoundTLBR     boxBoundTLBR;
    BoxBoundXYWH     boxBoundXYWH;
    // MultiBoxBound multiBoxBound;
    // MultiCircleBound multiCircleBound;
    // PolygonBound polyBound;
  };
};

enum eColour
{
  RGB,
  RGBA,
  HSL,
  HSLA,
  HSV,
  HSVA,
  CMYK,
  HEX,
};

struct ColourRGB
{
  ColourComponent r;
  ColourComponent g;
  ColourComponent b;
};

struct ColourRGBA
{
  ColourComponent r;
  ColourComponent g;
  ColourComponent b;
  ColourComponent a;
};

struct ColourHSL
{
  ColourComponent h;
  ColourComponent s;
  ColourComponent l;
};

struct ColourHSLA
{
  ColourComponent h;
  ColourComponent s;
  ColourComponent l;
  ColourComponent a;
};

struct ColourHSV
{
  ColourComponent h;
  ColourComponent s;
  ColourComponent v;
};

struct ColourHSVA
{
  ColourComponent h;
  ColourComponent s;
  ColourComponent v;
  ColourComponent a;
};

struct ColourCMYK
{
  ColourComponent c;
  ColourComponent m;
  ColourComponent y;
  ColourComponent k;
};

struct ColourHex
{
  char hexVal[8];
};

struct Colour
{
  eColour kind;
  union
  {
    ColourRGB rgb;
    ColourRGBA rgba;
    ColourHSL hsl;
    ColourHSLA hsla;
    ColourHSV hsv;
    ColourHSVA hsva;
    ColourCMYK cmyk;
    ColourHex hex;
  };
};

enum eAudioState
{
  PLAYING,
  PAUSED,
  STOPED,
};

} // end namespace Engine

#endif // BASIC_TYPES_HPP