#pragma once

#include "ADXVector3.h"
#include <string>

class ADXUtility
{
public:
	static const float Pi;

public:
	static ADXVector3 Lerp(const ADXVector3& startValue, const ADXVector3& endValue, float progress);
	static float Lerp(float startValue, float endValue, float progress);
	static float EaseIn(float progress, float pow);
	static float EaseOut(float progress, float pow);
	static float AngleDiff(float angle1, float angle2);
	static float LerpInverse(float nowValue, float startValue, float endValue);
	static float ValueMapping(float nowValue, float inputStart, float inputEnd,
		float outputStart, float outputEnd);
	static wchar_t* StringToWideChar(const std::string& pKey);

	template<typename T = void>
	struct NPManager
	{
		constexpr NPManager() noexcept = default;

		// 別の Deleter オブジェクトから Deleter オブジェクトを構築します。
		// このコンストラクタは U* が T* に暗黙に変換可能な場合にのみ、オーバーロード解決に参加します。
		template<
			typename U,
			typename std::enable_if<std::is_convertible<U*, T*>::value, std::nullptr_t>::type = nullptr
		>
		NPManager(const NPManager<U>&) noexcept {}

		void operator()(T* ptr) const {
			delete ptr;
			ptr = nullptr;
		}
	};

	template<typename T>
	struct NPManager<T[]>
	{
		constexpr NPManager() noexcept = default;

		// 別の Deleter オブジェクトから Deleter オブジェクトを構築します。
		// このコンストラクタは U(*)[] が T(*)[] に暗黙に変換可能な場合にのみ、オーバーロード解決に参加します。
		template<
			typename U,
			typename std::enable_if<std::is_convertible<U(*)[], T(*)[]>::value, std::nullptr_t>::type = nullptr
		>
		NPManager(const NPManager<U[]>&) noexcept {}

		void operator()(T* ptr) const {
			delete[] ptr;
			for (auto& itr : ptr)itr = nullptr;
		}
	};
};
