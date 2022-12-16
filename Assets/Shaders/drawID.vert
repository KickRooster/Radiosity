#version 450 core
		layout(location = 0) in vec4 vPos;
		layout(location = 1) in vec3 vNormal;
		layout(location = 2) in vec3 vTangent;
		layout(location = 3) in vec3 vBinormal;
		layout(location = 4) in vec3 vColor;
		layout(location = 5) in vec4 vCustomData;
		layout(location = 6) in vec2 texCoord0;
		layout(location = 7) in vec2 texCoord1;
		layout(location = 8) in vec2 texCoord2;
		layout(location = 9) in vec2 texCoord3;
		layout(location = 10) in vec2 texCoord4;
		layout(location = 11) in vec2 texCoord5;
		layout(location = 12) in vec2 texCoord6;
		layout(location = 13) in vec2 texCoord7;
		layout (std140, binding = 0) uniform CameraUniformData
		{
			mat4 viewMatrix;
			mat4 perspectiveProjectionMatrix;
			mat4 orthoProjectionMatrix;
			vec4 position;
			vec4 NearFar;
		};
		layout (std140, binding = 1) uniform CubeMatrices
		{
			mat4 ViewProjection_Positive_X;
			mat4 ViewProjection_Negative_X;
			mat4 ViewProjection_Positive_Y;
			mat4 ViewProjection_Negative_Y;
			mat4 ViewProjection_Positive_Z;
			mat4 ViewProjection_Negative_Z;
		};
		layout (std140, binding = 2) uniform ShooterInfo
		{
			vec4 ShooterPosition;
			vec4 ShooterNormal;
			vec4 ShooterEnergy;
			vec4 ShooterSurfaceArea;
		};
		layout (std140, binding = 3) uniform ObjectMatrices
		{
			mat4 object2World;
		};
		layout (std140, binding = 4) uniform ObjectMatricesIT
		{
			mat4 object2WorldIT;
		};
	
		out vec3 normal;
		out vec3 tangent;
		out vec3 binormal;
		out vec3 color;
		flat out vec4 customData;
		out vec2 uv0;
		out vec2 uv1;
		out vec2 uv2;
		out vec2 uv3;
		out vec2 uv4;
		out vec2 uv5;
		out vec2 uv6;
		out vec2 uv7;
		void main()
		{
    		// transform the geometry to camera space    
    		//vec4 mpos = viewMatrix * object2World * vPos;
    		// project to a point on a unit hemisphere      
    		//vec3 hemi_pt = normalize(mpos.xyz);      
    		// Compute (f-n), but let the hardware divide z by this
   			// in the w component (so premultiply x and y)
    		//float f_minus_n = NearFar.y - NearFar.x; 
    		//gl_Position.xy = hemi_pt.xy * f_minus_n;
    		// compute depth proj. independently, using OpenGL orthographic
    		//gl_Position.z = (-2.0 * mpos.z - NearFar.y - NearFar.x);
    		//gl_Position.w = f_minus_n;

			gl_Position = perspectiveProjectionMatrix * viewMatrix * object2World * vPos;
			normal = vNormal;
		    tangent = vTangent;
		    binormal = vBinormal;
		    color = vColor;
			customData = vCustomData;
		    uv0 = texCoord0;
		    uv1 = texCoord1;
		    uv2 = texCoord2;
		    uv3 = texCoord3;
		    uv4 = texCoord4;
		    uv5 = texCoord5;
		    uv6 = texCoord6;
		    uv7 = texCoord7;
		};