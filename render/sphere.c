#include "../includes/minirt.h"



t_vector		local_normal_at_sphere(t_point obj_point, t_point point)
{
	return (subtract_points(obj_point, point));
}

t_vector		local_normal_at_plane()
{
	return (vector(0, 1 ,0));
}

t_vector		local_normal_at_cylinder(void *shape,  t_point pnt)
{
	double max;
	double min;
	t_cy *cy;
	cy = (t_cy *) shape;

	max = cy->height / 2.0;
	min = -1.0 * max;


	// max = 2.0;
	// min = 1.0;

	double dist;

	dist = pow(pnt.x, 2) + pow(pnt.z, 2);
	if (dist < 1 && pnt.y >= max - EPSILON)
		return vector(0, 1, 0);
	else if (dist < 1 && pnt.y <= min + EPSILON)
		return vector(0, -1, 0);
	else
	return (vector(pnt.x, 0 ,pnt.z));
}

t_vector	normal_at(t_shape s, t_point p)
{
	t_vector	obj_normal;
	t_vector	world_normal;
	t_point		point;
	t_point		obj_point;
	t_tuple		tp1;
	t_tuple		tp2;
	t_tuple		tp3;
	t_tuple		tp4;
	double		**invrs;
	double		**trnspose;

	tp1 = point_tp(p);
	invrs = inverse(s.transform, 4);
	tp2 = matrix_multi_tp(invrs, tp1);
	obj_point.x = tp2.x;
	obj_point.y = tp2.y;
	obj_point.z = tp2.z;
	point.x = 0;
	point.y = 0;
	point.z = 0;
	if (!ft_strncmp(s.shape_name, "sp", 2))
		obj_normal = local_normal_at_sphere(obj_point, point);
	if (!ft_strncmp(s.shape_name, "pl", 2))
		obj_normal = local_normal_at_plane();
	if (!ft_strncmp(s.shape_name, "cy", 2))
		obj_normal = local_normal_at_cylinder(s.shape,obj_point);	
	tp3 = vector_tp(obj_normal);

	trnspose = transpose(invrs);
	tp4 = matrix_multi_tp(trnspose , tp3);
	world_normal.x = tp4.x;
	world_normal.y = tp4.y;
	world_normal.z = tp4.z;

	free_2d_array(invrs,4 );
	free_2d_array(trnspose,4 );
	return (normalize(world_normal));
}

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

// if (pl->norm_vec.x != 0)
// 	shp.transform = rotation_x(pl->norm_vec.x);
// if (pl->norm_vec.y != 0)
// 	set_transform(&shp, matrix_multi(shp.transform , rotation_y(pl->norm_vec.y)));
// if (pl->norm_vec.z != 0)
// 	set_transform(&shp, matrix_multi(shp.transform, rotation_z(pl->norm_vec.z)));


t_light	point_light(t_point pos, t_color intensity)
{
	t_light	l;

	l.pos = pos;
	l.color = intensity;
	return (l);
}

t_material	material()
{
	t_material	m;
	m.shininess = 200.0;
	return (m);
}

t_point	effective_color(t_material m, t_light l)
{
	t_point	effective_color;

	effective_color.x = m.color.r * l.color.r;
	effective_color.y = m.color.g * l.color.g;
	effective_color.z = m.color.b * l.color.b;
	return (effective_color);
}

t_color	color_return(t_color ambient, t_color diffuse, t_color specular)
{
	t_color	final;

	final.r = ambient.r + diffuse.r + specular.r;
	final.g = ambient.g + diffuse.g + specular.g;
	final.b = ambient.b + diffuse.b + specular.b;

	return (final);
}

t_color	calc_specular(t_material m, t_light l, double reflect_dot_eye)
{
	t_color	specular;
	double	factor;

	factor = pow(reflect_dot_eye, m.shininess);
	specular.r = l.color.r * m.specular * factor;
	specular.g = l.color.g * m.specular * factor;
	specular.b = l.color.b * m.specular * factor;
	return (specular);
}

double	light_dot_normal(t_vector lightv, t_vector normalv)
{
	return (dot(vector_tp(lightv), vector_tp(normalv)));
}

double	reflect_dot_eye(t_vector reflectv, t_vector eyev)
{
	return (dot(vector_tp(reflectv), vector_tp(eyev)));
}

t_color	lighting(t_comps c, t_light l, t_bool in_shadow)
{
	t_vector	lightv;
	t_vector	reflectv;
	t_color		ambient;
	t_color		diffuse;
	t_color		specular;
	t_tuple		tp[3];

	lightv = normalize(subtract_points(point(l.pos.x, l.pos.y, l.pos.z), c.over_point));
	tp[0] = multiply(point_tp(effective_color(c.object.material, l)), c.object.material.ambient);
	ambient = color(tp[0].x, tp[0].y, tp[0].z);
	if (light_dot_normal(lightv, c.normalv) < 0 || in_shadow)
	{
		diffuse = color(0, 0, 0);
		specular = color(0, 0, 0);
	}
	else
	{
		tp[1] = multiply(point_tp(effective_color(c.object.material, l)), c.object.material.diffuse);
		tp[2] = multiply(tp[1], light_dot_normal(lightv, c.normalv));
		diffuse = color(tp[2].x, tp[2].y, tp[2].z);
		reflectv = reflect(negate_vector(lightv), c.normalv);
		if (reflect_dot_eye(reflectv, c.eyev) <= 0)
			specular = color(0, 0, 0);
		else
			specular = calc_specular(c.object.material, l, reflect_dot_eye(reflectv, c.eyev));
	}
	return (color_return(ambient, diffuse, specular));
}
