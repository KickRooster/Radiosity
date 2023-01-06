		#version 450 core
	
		in vec3 normal;
		in vec3 tangent;
		in vec3 binormal;
		flat in vec3 color;
		flat in vec4 customData;
		in vec2 uv0;
		in vec2 uv1;
		in vec2 uv2;
		in vec2 uv3;
		in vec2 uv4;
		in vec2 uv5;
		in vec2 uv6;
		in vec2 uv7;
		uniform sampler2D albedoSampler;
		uniform sampler2D lightmapSampler;
		layout (std140, binding = 0) uniform CameraUniformData
		{
			mat4 viewPerspectiveProjectionMatrix;
			mat4 viewOrthoProjectionMatrix;
			vec4 position;
			vec4 NearFar;
			ivec4 FrameCount;
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

		out vec4 out_Color;

		vec2 MapUV(vec2 UVCoord, float Red, float Green, float Blue)
		{
			float fR = round(Red * 255.0);
			int r = int(fR);

			float fG = round(Green * 255.0);
			int g = int(fG);

			float fB = round(Blue * 255.0);
			int b = int(fB);

			vec2 uv = abs(fract(UVCoord));

			if (b == 0 && g != 255) // 255 x 255
			{
    			vec2 origin512 = vec2(r % 4, floor(r / 4)) / 4;
    			vec2 origin256 = vec2(g % 2, floor(g / 2)) / 2 / 4;
    			vec2 origin = origin512 + origin256;

    			return origin + uv / 8;
			}
			else if (b == 0 && g == 255) // 512 x 255
			{
    			vec2 origin512 = vec2(r % 4, floor(r / 4)) / 4;
    			vec2 origin = origin512;

    			return origin + uv / 4;
			}
			else if (b == 255) // 512 x 512
			{
    			vec2 origin512 = vec2(r % 4, floor(r / 4)) / 4;
    			vec2 origin512_256 = vec2(floor(g / 2), g % 2 / vec2(2)) / 4;
    			vec2 origin = origin512 + origin512_256;

    			return origin + vec2(uv.x / 4, uv.y / 4 / 2);
			}

			return vec2(0, 0);
		}

		void main()
		{
			vec2 MappedUV = MapUV(uv0, color.r, color.g, color.b);
			vec3 albedo = texture(albedoSampler, MappedUV).xyz;
			vec3 irradiance = texture(lightmapSampler, uv1).xyz;

			out_Color.xyz = albedo * irradiance;

			float gamma = 2.2;
    		out_Color.rgb = pow(out_Color.rgb, vec3(1.0 / gamma));

			out_Color.w = 1.0;
		};