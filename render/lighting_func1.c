#include "../includes/minirt.h"

t_vector	reflect(t_vector vec, t_vector normal)
{
	t_vector	ret;
	double		d;

	d = 2 * dot(vector_tp(vec), vector_tp(normal));
	normal.x = normal.x * d;
	normal.y = normal.y * d;
	normal.z = normal.z * d;
	ret.x = vec.x - normal.x;
	ret.y = vec.y - normal.y;
	ret.z = vec.z - normal.z;
	return (ret);
}

t_light	point_light(t_point pos, t_color intensity)
{
	t_light	l;

	l.pos = pos;
	l.color = intensity;
	return (l);
}

t_material	material(void)
{
	t_material	m;

	m.shininess = 200.0;
	return (m);
}

double	light_dot_normal(t_vector lightv, t_vector normalv)
{
	return (dot(vector_tp(lightv), vector_tp(normalv)));
}

double	reflect_dot_eye(t_vector reflectv, t_vector eyev)
{
	return (dot(vector_tp(reflectv), vector_tp(eyev)));
}
