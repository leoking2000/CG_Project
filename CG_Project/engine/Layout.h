#pragma once
#include "defines.h"

namespace VE
{
	enum ElementType
	{
		FLOAT1 = 0,
		FLOAT1_N = 1,

		FLOAT2 = 2,
		FLOAT2_N = 3,

		FLOAT3 = 4,
		FLOAT3_N = 5,

		FLOAT4 = 6,
		FLOAT4_N = 7,

		UCHAR3 = 8,
		UCHAR3_N = 9,

		UCHAR4 = 10,
		UCHAR4_N = 11
	};

	template<u32 ELEMENTS_COUNT>
	class Layout
	{
	public:
		Layout(ElementType* arr)
		{
			assert(arr != nullptr);
			m_stride = 0;

			for (u32 i = 0; i < ELEMENTS_COUNT; i++)
			{
				m_arr[i] = arr[i];

				switch (m_arr[i])
				{
				case FLOAT1:
				case FLOAT1_N:
					m_stride += 1 * sizeof(float);
					break;
				case FLOAT2:
				case FLOAT2_N:
					m_stride += 2 * sizeof(float);
					break;
				case FLOAT3:
				case FLOAT3_N:
					m_stride += 3 * sizeof(float);
					break;
				case FLOAT4:
				case FLOAT4_N:
					m_stride += 4 * sizeof(float);
					break;

				case UCHAR3:
				case UCHAR3_N:
					m_stride += 3 * sizeof(char);
					break;
				case UCHAR4:
				case UCHAR4_N:
					m_stride += 4 * sizeof(char);
					break;
				default:
					assert(false);
					break;
				}
			}
		}

		ElementType operator[](int i) const
		{
			return m_arr[i];
		}

		inline u32 GetStride() const
		{
			return m_stride;
		}

		inline u32 GetCount() const { return ELEMENTS_COUNT; }

	private:
		ElementType m_arr[ELEMENTS_COUNT];
		u32 m_stride;
	};
}