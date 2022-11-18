#include "../includes/minirt.h"

t_vector	normal_at(t_shape s, t_point p)
{
	t_vector	obj_normal;
	t_vector	world_normal;
	t_tuple		tp[4];
	double		**invrs;
	double		**trnspose;

	tp[0] = point_tp(p);
	invrs = inverse(s.transform, 4);
	tp[1] = matrix_multi_tp(invrs, tp[0]);
	obj_normal = object_normal(s, tp[1]);
	tp[2] = vector_tp(obj_normal);
	trnspose = transpose(invrs);
	tp[3] = matrix_multi_tp(trnspose, tp[2]);
	world_normal = vector(tp[3].x, tp[3].y, tp[3].z);
	free_2d_array(invrs, 4);
	free_2d_array(trnspose, 4);
	return (normalize(world_normal));
}

t_color	lighting(t_comps c, t_light l, t_bool in_shadow)
{
	t_vector	lightv;
	t_vector	reflectv;
	t_color		ambient;
	t_color		diffuse;
	t_color		specular;

	lightv = normalize(subtract_points(point(l.pos.x, l.pos.y, l.pos.z),
				c.over_point));
	ambient = calc_ambient(c, l);
	if (light_dot_normal(lightv, c.normalv) < 0 || in_shadow)
	{
		diffuse = color(0, 0, 0);
		specular = color(0, 0, 0);
	}
	else
	{
		diffuse = calc_diffuse(c, l, lightv);
		reflectv = reflect(negate_vector(lightv), c.normalv);
		if (reflect_dot_eye(reflectv, c.eyev) <= 0)
			specular = color(0, 0, 0);
		else
			specular = calc_specular(c.object.material, l,
					reflect_dot_eye(reflectv, c.eyev));
	}
	return (color_return(ambient, diffuse, specular));
}
