#pragma once

namespace Kawaii
{
	class IBLAux
	{
	private:
		IBLAux() = default;
		~IBLAux() = default;
	public:
		static void convertToCubemap(int width, int height, unsigned int hdrTexIndex,
			unsigned int cuebmapTexIndex);
	
		static void convoluteDiffuseIntegral(int width, int height, unsigned int cubemapTexIndex,
			unsigned int irradianceTexIndex);

		static void convoluteSpecularIntegral(int width, int height, unsigned int cubemapTexIndex,
			unsigned int prefilteredTexIndex);

		static void convoluteSpecularBRDFIntegral(int width, int height, unsigned int brdfLutTexIndex);

	};
}