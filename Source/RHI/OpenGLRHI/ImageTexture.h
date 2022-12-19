#pragma once
#include <BaseInclude.h>
#include "BaseGL.h"

namespace Core
{
    class GLImageTexture
    {
        uint32 m_id;
        GLTextureTarget m_target;
        GLInternalFormat m_internalFormat;
        GLPixelFormat m_pixelFormat;
        GLDataType m_dataType;
        GLImageUnit m_unit;
        
        void setWrapMode(GLTextureWrapMode wrapMode);
        void setFilterMode(GLTextureFilterMode filterMode);

    public:
        GLImageTexture(GLTextureTarget target, GLInternalFormat internalFormat, GLPixelFormat pixelFormat, GLDataType dataType, GLTextureWrapMode wrapMode, GLTextureFilterMode filterMode, GLImageUnit Unit, int32 BoundLevel, GLImageAccess Access);
        uint32 GetID() const;
        uint64 GetID64() const;
        GLTextureTarget GetTarget() const;
        GLImageUnit GetUnit() const;
        void LoadImage(int32 width, int32 height, const void * pData);
        void Fetch(void * pData);
        int32 GetDataSizePerPixel() const;
        ~GLImageTexture();
    };
}
