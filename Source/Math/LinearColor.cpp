#include "LinearColor.h"

#include <algorithm>
#include <glm/common.hpp>
#include <math.h>

#include "MathGlobal.h"

namespace Core
{
   LinearColor::LinearColor()
      :
   R(0),
   G(0),
   B(0),
   A(0)
   {
      
   }
   
   LinearColor::LinearColor(float InR, float InG, float InB, float InA)
      :
   R(InR),
   G(InG),
   B(InB),
   A(InA)
   {
   }

   LinearColor& LinearColor::operator/=(float Scale)
   {
      this->R /= Scale;
      this->G /= Scale;
      this->B /= Scale;
      this->A /= Scale;

      return *this;
   }

   LinearColor& LinearColor::operator*=(float Scale)
   {
      this->R *= Scale;
      this->G *= Scale;
      this->B *= Scale;
      this->A *= Scale;

      return *this;
   }
   
   Color LinearColor::ToRGBM() const
   {
      LinearColor HDR;

      // XXX:  Due to convert to gamma space we get more precision on daker color,
      //       and for lightmap seams, seams get to larger range than store RGBM in linear space.
      //       So if we get perceptible lightmap seams, we store lightmap in linear space.
      // Convert to gamma space
      HDR.R = sqrt(R);
      HDR.G = sqrt(G);
      HDR.B = sqrt(B);
      HDR.A = A;
      
      // Range
      HDR /= 16.0f;

      float MaxValue = std::max(std::max(HDR.R, HDR.G), std::max(HDR.B, 0.000001f) );

      if( MaxValue > 0.75f )
      {
         // Fit to valid range by leveling off intensity
         float Tonemapped = (MaxValue - 0.75f * 0.75f ) / ( MaxValue - 0.5f );
         HDR *= Tonemapped / MaxValue;
         MaxValue = Tonemapped;
      }

      LinearColor LinearRGBM;
      
      LinearRGBM.A = ceil( MaxValue * 255.0f ) / 255;
      LinearRGBM.R = ((HDR.R / LinearRGBM.A ) );
      LinearRGBM.G = ((HDR.G / LinearRGBM.A ) );
      LinearRGBM.B = ((HDR.B / LinearRGBM.A ) );

      Color RGBM;

      RGBM.A = static_cast<uint8>(round( LinearRGBM.A * 255));
      RGBM.R = static_cast<uint8>(round( LinearRGBM.R * 255));
      RGBM.G = static_cast<uint8>(round( LinearRGBM.G * 255));
      RGBM.B = static_cast<uint8>(round( LinearRGBM.B * 255));
      
      return RGBM;
   }
}