#include "ADXTextRenderer.h"
#include "ADXObject.h"

void ADXTextRenderer::AddFonts(const std::vector<fontAndChar>& fontSet)
{
	for (auto& setItr : fontSet)
	{
		bool hitted = false;
		for (auto& itr : fonts_)
		{
			if (setItr.character == itr.character)
			{
				itr = setItr;
				hitted = true;
				break;
			}
		}
		if (!hitted)
		{
			fonts_.push_back(setItr);
		}
	}
}

void ADXTextRenderer::UniqueRendering([[maybe_unused]] ID3D12Device* device, [[maybe_unused]] ID3D12GraphicsCommandList* cmdList)
{

}
