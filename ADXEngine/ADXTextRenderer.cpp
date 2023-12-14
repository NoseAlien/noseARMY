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

void ADXTextRenderer::UniqueUpdate()
{
	letters_.remove_if([=](auto& itr)
		{ return itr == nullptr; });

	for (int i = 0; i < text_.size(); i++)
	{
		if (letters_.size() < i)
		{

		}
	}

	int index = 0;
	for (auto& itr : letters_)
	{
		if (index >= text_.size())
		{
			itr->Destroy();
		}
		index++;
	}
}

void ADXTextRenderer::SafetyPhase()
{
	letters_.remove_if([=](auto& itr)
		{ return itr->GetDeleteFlag(); });
}