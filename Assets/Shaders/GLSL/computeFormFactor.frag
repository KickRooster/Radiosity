		#version 450
	
		in vec4 pos;
		in vec3 normal;
		in vec3 tangent;
		in vec3 binormal;
		in vec3 color;
		flat in vec4 customData;
		in vec2 uv0;
		in vec2 uv1;
		in vec2 uv2;
		in vec2 uv3;
		in vec2 uv4;
		in vec2 uv5;
		in vec2 uv6;
		in vec2 uv7;

		layout(location = 0) uniform sampler2D albedoSampler;
		layout(location = 1) uniform samplerCube IDCubeMapSampler;
		layout(location = 2) uniform sampler2D VisibilitySampler;
		layout(rgba32f, location = 3) uniform restrict image2D RadiosityOutput0;
		layout(rgba32f, location = 4) uniform restrict image2D RadiosityOutput1;
		layout(rgba32f, location = 5) uniform restrict image2D ResidualOutput0;
		layout(rgba32f, location = 6) uniform restrict image2D ResidualOutput1;
		
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
		
		layout(location = 0) out vec4 attch0;

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
			vec4 RecvPos = object2World * pos;
			vec3 r = ShooterPosition.xyz - RecvPos.xyz;
			float distance2 = dot(r, r);
			vec4 Normal4 = vec4(normal.x, normal.y, normal.z, 1.0f);
			vec4 WorldNormal4 = object2WorldIT * Normal4;
			vec3 WorldNormal = normalize(WorldNormal4.xyz);
            float cosi = dot(WorldNormal, normalize(r));    
            float cosj = -dot(ShooterNormal.xyz, normalize(r)); 
			const float pi = 3.1415926535;
			float Fij = max(cosi * cosj, 0) / (pi * distance2 + ShooterSurfaceArea.x);

			float RLVisable = texture(VisibilitySampler, uv1).x;
			
			Fij *= RLVisable;
			
			vec3 Radiosity = ShooterEnergy.xyz * ShooterSurfaceArea.x * Fij;
			vec2 MappedUV = MapUV(uv0, color.r, color.g, color.b);
			vec3 albedo = texture(albedoSampler, MappedUV).xyz;
			vec3 Residual = albedo * Radiosity;

			if (FrameCount.x % 2 == 0)
			{
				//	Read from 0, write to 1
				ivec2 LightmapSize = imageSize(RadiosityOutput0);
				ivec2 TargetLocation = ivec2(LightmapSize * uv1);

				vec4 TotalRadiosity = imageLoad(RadiosityOutput0, TargetLocation);
				TotalRadiosity.xyz += Radiosity;
				TotalRadiosity.w = customData.x;	//	primitive id
				imageStore(RadiosityOutput1, TargetLocation, TotalRadiosity);

				vec4 TotalResidual = imageLoad(ResidualOutput0, TargetLocation);
				TotalResidual.xyz += Residual;
				TotalResidual.w = customData.x;
				imageStore(ResidualOutput1, TargetLocation, TotalResidual);
			}
			else
			{
				//	Read from 1, write to 0
				ivec2 LightmapSize = imageSize(RadiosityOutput1);
				ivec2 TargetLocation = ivec2(LightmapSize * uv1);

				vec4 TotalRadiosity = imageLoad(RadiosityOutput1, TargetLocation);
				TotalRadiosity.xyz += Radiosity;
				TotalRadiosity.w = customData.x;
				imageStore(RadiosityOutput0, TargetLocation, TotalRadiosity);

				vec4 TotalResidual = imageLoad(ResidualOutput1, TargetLocation);
				TotalResidual.xyz += Residual;
				TotalResidual.w = customData.x;
				imageStore(ResidualOutput0, TargetLocation, TotalResidual);
			}
			
			attch0.xyz = Residual;
			attch0.w = 1.0;
		};