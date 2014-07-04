module lfant.transform;

public
{
	import gl3n.linalg;
	import gl3n.math;
	import gl3n.util;
}

import lfant.component;

class Transform : Component
{
public:
	void translate(in vec3d pos)
	{
		position = position + pos;
	}

	void rotate(in vec3 rot)
	{
	}

	void rotate(in vec3 axis, in float value)
	{
		rotationQuat.rotate_axis(value, axis);
	}

	void multiplyScale(in vec3 sclt)
	{
		_scale = sclt * _scale;
	}

	@property final
	{
		vec3d position() const
		{
			return _position;
		}
		void position(in vec3d pos)
		{
			_position = pos;
		//	triggerEvent("SetPosition", position);
		}

		vec3d worldPosition()
		{
			return to!vec3d(vec4(matrix[3]).xyz);
		}
		void worldPosition(in vec3d pos)
		{
		}

		vec3d worldPositionRelative()
		{
			return worldPosition;
		}

		vec3 rotation() const
		{
			return _rotation;
		}
		void rotation(in vec3 rot)
		{
		/*	float cap = 360.0f; // degrees
			rollover(rot.x, 0.0f, cap);
			rollover(rot.y, 0.0f, cap);
			rollover(rot.z, 0.0f, cap);*/

			rotation = rot;
			rotationQuat = quat(1, rotation.radians());
		}

		vec3 scale() const
		{
			return _scale;
		}
		void scale(vec3 scl)
		{
			_scale = scl;
		}

		vec3 direction() const
		{
			return vec4(matrix[2]).xyz;
		}
		void direction(in vec3 dir)
		{
		}

		vec3 right() const
		{
			return vec4(matrix[0]).xyz;
		}
		void right(in vec3 dir)
		{
		}

		vec3 up() const
		{
			return vec4(matrix[1]).xyz;
		}
		void up(in vec3 dir)
		{
		}
	}

	quat rotationQuat;
private:
	vec3d _position;
	vec3 _rotation;
	vec3 _scale;

	mat4 matrix;
}