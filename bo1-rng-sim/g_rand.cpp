#include "g_rand.hpp"

#include <windows.h>

namespace rng {

	int __declspec(naked) irand_internal(int min, int max, int holdrand)
	{
		__asm {
			push ebp
			mov ebp, esp
			sub esp, 0xc

			mov eax, [holdrand]
			mov ecx, [max]
			imul eax, eax, 0x343fd
			add eax, 0x269ec3
			push esi
			mov esi, min
			mov[holdrand], eax
			sub ecx, esi
			shr eax, 0x11
			imul ecx
			mov cl, 0xf
			shrd eax, edx, cl
			sar edx, cl
			add eax, esi
			pop esi

			mov esp, ebp
			pop ebp
			ret
		};
	}

	G_Rand::G_Rand()
	{
		holdrand_ = timeGetTime();
	}

	void G_Rand::CycleRand(size_t n)
	{
		for (size_t i = 0; i < n; i++) {
			CycleRand();
		}
	}

	int G_Rand::irand(int min, int max)
	{
		int holdrand = holdrand_;
		CycleRand();
		return irand_internal(min, max, holdrand);
	}

};