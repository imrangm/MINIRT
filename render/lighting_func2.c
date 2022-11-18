#include "../includes/minirt.h"

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

t_color	calc_ambient(t_comps c, t_light l)
{
	t_tuple	tp;
	t_color	ambient;

	tp = multiply(point_tp(effective_color(c.object.material, l)),
			c.object.material.ambient);
	ambient = color(tp.x, tp.y, tp.z);
	return (ambient);
}

t_color	calc_diffuse(t_comps c, t_light l, t_vector lightv)
{
	t_tuple	tp[2];
	t_color	diffuse;

	tp[0] = multiply(point_tp(effective_color(c.object.material, l)),
			c.object.material.diffuse);
	tp[1] = multiply(tp[0], light_dot_normal(lightv, c.normalv));
	diffuse = color(tp[1].x, tp[1].y, tp[1].z);
	return (diffuse);
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
