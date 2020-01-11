#pragma once

// ÈýÎ¬µã

namespace ns_math
{
	struct Point3
	{
		float m_x;
		float m_y;
		float m_z;

		Point3()
		{
			memset(this, 0, sizeof(Point3));
		}

		Point3(float x, float y, float z)
		{
			m_x = x;
			m_y = y;
			m_z = z;
		}
	};
}
