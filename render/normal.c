#include "../includes/minirt.h"

t_vector	local_normal_at_sphere(t_point obj_point, t_point point)
{
	return (subtract_points(obj_point, point));
}

t_vector	local_normal_at_plane(void)
{
	return (vector(0, 1, 0));
}

t_vector	local_normal_at_cylinder(void *shape, t_point pnt)
{
	double	max;
	double	min;
	double	dist;
	t_cy	*cy;

	cy = (t_cy *) shape;
	max = cy->height / 2.0;
	min = -1.0 * max;
	// max = 2.0;
	// min = 1.0;
	dist = pow(pnt.x, 2) + pow(pnt.z, 2);
	if (dist < 1 && pnt.y >= max - EPSILON)
		return (vector(0, 1, 0));
	else if (dist < 1 && pnt.y <= min + EPSILON)
		return (vector(0, -1, 0));
	return (vector(pnt.x, 0, pnt.z));
}

t_vector	normal_vector(t_shape s, t_point obj_point, t_point origin)
{
	t_vector	obj_normal;

	if (!ft_strncmp(s.shape_name, "sp", 2))
		obj_normal = local_normal_at_sphere(obj_point, origin);
	if (!ft_strncmp(s.shape_name, "pl", 2))
		obj_normal = local_normal_at_plane();
	if (!ft_strncmp(s.shape_name, "cy", 2))
		obj_normal = local_normal_at_cylinder(s.shape, obj_point);
	return (obj_normal);
}

t_vector	object_normal(t_shape s, t_tuple tp)
{
	t_vector	obj_normal;
	t_point		obj_point;
	t_point		origin;

	obj_point = point(tp.x, tp.y, tp.z);
	origin = point(0, 0, 0);
	obj_normal = normal_vector(s, obj_point, origin);
	return (obj_normal);
}