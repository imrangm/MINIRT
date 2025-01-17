#ifndef MINIRT_H
#define MINIRT_H

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include "../libs/libft/libft.h"
#include "../libs/mlx_mac/mlx.h"
# define MAX(a,b) ((a) > (b) ? (a) : (b))
# define MIN(a,b) ((a) < (b) ? (a) : (b))
# define EPSILON 0.00001
# define	PI 4.0 * atan(1.0)
# define HEIGHT 300
# define WIDTH 300
//KEYS
# define KEY_ESC 53
# define TRUE 1
# define FALSE 0

//object types

enum
{
	SP,
	PL,
	CN,
};

typedef unsigned int t_bool;

//tuple

typedef struct s_tuple
{
	double	x;
	double	y;
	double	z;
	double	w;
}	t_tuple;

//point

typedef struct s_point
{
	double	x;
	double	y;
	double	z;
}	t_point;

//vector

typedef struct s_vector
{
	double	x;
	double	y;
	double	z;
}	t_vector;



//matrices

typedef struct s_mat
{
	int		s;
	double	m[4][4];
}	t_mat;

//ray

typedef struct s_ray
{
	t_point		origin;
	t_vector	direction;
}	t_ray;

//color

typedef struct s_color
{
    double r;
    double g;
    double b;

}               t_color;

// lights information

typedef struct s_light
{
    t_point pos;
    t_color color;
    double  ratio;

}               t_light;

// camera information

typedef struct s_camera
{
    t_point pos;
    t_vector norm_vector;
    double fov;

}               t_camera;

typedef struct s_camera2
{
    double	hsize;
	double	vsize;
    double	field_of_view;
	double	**transform;
	double	half_width;
	double	half_height;
	double	pixel_size;

}	t_camera2;

typedef struct		s_mlx
{
	void			*mlx_ptr;
	void			*win_ptr;
}					t_mlx;

typedef struct		s_img
{
	void			*img_ptr;
 	int				bits_per_pixel;
	int				size_line;
	int				endian;
	unsigned char	*data;
}					t_img;


// typedef struct s_sphere
// {
//     t_list *sphere;    
//     t_vector sphere_center;
//     double diameter;
//     t_color color;

// }   t_sphere

typedef struct s_material
{
	t_color	color;
	double	ambient;
	double	diffuse;
	double	specular;
	double	shininess;
}	t_material;

typedef struct s_sphere
{
	int			id;
	t_point		sp_center;
	double		radius;
	t_color color;

}		       t_sphere;

typedef struct s_plane
{
	t_point xyz;
    t_vector norm_vec;
    t_color color;
}		       t_plane;


typedef struct s_cy
{
	t_point xyz;
    t_vector norm_vec;
    t_color color;

    double diameter;
    double height;
}		       t_cy;

typedef struct s_shape
{
	t_point		position;
	double		**transform;
	t_material  material;
	t_color		color;
	t_ray		ray_in_obj_space;
	t_vector	norm_vector;
	void*		shape;
	char*		shape_name;
}			   t_shape;



typedef struct sobj_list
{
	void		*content;
	struct obj_list	*next;
}		tobj_list;

//intersection
typedef	struct s_intersection
{
	int			count;
	double		t;
	t_shape	   object;
}	t_intersection;


typedef struct s_intersect
{
	int				count;
	double			t[2];
}	t_intersect;

// typedef struct s_obj
// {
// 	t_sphere	*sp;
// 	int			obj_type;
// }	t_obj;

typedef struct s_world
{
	t_shape		*s;
	t_light		l;
	int         shape_count;
}	t_world;

typedef	struct s_comps
{
	double			t;
	t_shape		object;
	t_point			point;
	t_point			over_point;
	t_vector		eyev;
	t_vector		normalv;
	t_bool			inside;
}	t_comps;

typedef struct s_light_var
{
	t_light		l;
	t_point		pos;
	t_vector	eyev;
	t_vector 	normalv; 
	t_bool 		in_shadow;
}	t_light_var;

// main struct
typedef struct s_data
{
    //mlx
    t_mlx		mlx;

	t_img img;

    //CAMERA
    t_camera camera;

    //light
    t_light light_src;

    //sphere
    t_list *sphere_list;

    //plane
    t_list *plane_list;

    //cy
    t_list *cy_list;
    
    //AMBIENCE
    t_color amb_color;
    double  amb_ratio;
    int     amb_set;

	int 	total_shape_count;
	int 	total_sphere_count;
	int 	total_plane_count;
	int		total_cylinder_count;


    // screen info
    int width;
    int height;
    double aspect_ratio;
    double x0;
    double x1;

    double y0;
    double y1;

    double xstep;
    double ystep;
}       t_data;

// PARSE FUNCTIONS
void parse_scene();


// getnextline functions
char	*get_next_line(int fd);
char	*free_memmory(char **ptr);
char	*ft_strchr(const char *str, int c);
size_t	get_current_line_size(char *saved_line);

//parser functions
void parse_scene(char *file_name, t_data *data);
void parse_ambient_lighting(char **info, t_data *scene_data);
void parse_camera(char **info, t_data *scene_data);
void parse_light(char **info, t_data *scene_data);
void parse_sphere(char **info, t_data *scene_data);
void parse_plane(char **info, t_data *scene_data);
void parse_cylinder(char **info, t_data *scene_data);

//lighting functions
t_point	effective_color(t_material m, t_light l);
t_color	color_return(t_color ambient, t_color diffuse, t_color specular);
t_color	calc_ambient(t_comps c, t_light l);
t_color	calc_diffuse(t_comps c, t_light l, t_vector lightv);
t_color	calc_specular(t_material m, t_light l, double reflect_dot_eye);

//lighting utils
t_vector	reflect(t_vector vec, t_vector normal);
t_light		point_light(t_point pos, t_color intensity);
t_material	material(void);
double		light_dot_normal(t_vector lightv, t_vector normalv);
double		reflect_dot_eye(t_vector reflectv, t_vector eyev);

//normal functions
t_vector	local_normal_at_sphere(t_point obj_point, t_point point);
t_vector	local_normal_at_plane(void);
t_vector	local_normal_at_cylinder(void *shape, t_point pnt);
t_vector	normal_vector(t_shape s, t_point obj_point, t_point origin);
t_vector	object_normal(t_shape s, t_tuple tp);

//FREE FUNCTIONS
void free_scene_data();

// RENDER FUNCTIONS
void render_scene(t_data *scene_data);

// ERROR FUNCTIONS
void print_error_msg_and_exit(char *error_msg, t_data *scene_data);

//UTILS
int get_2darray_size(char **arr);
double parse_double(char *str);
void parse_color(char *str, t_data *scene_data, t_color *colors);
int skip_dot_verify_digits(char *str);
void print_tuple_sam(char *str,t_tuple *tp);
void print_point(char *str,t_point *tp);
void print_vector(char *str,t_vector *tp);




//Coordinates
t_tuple		tuple(double x, double y, double z, double w);
t_vector	vector(double x, double y, double z);
t_point		point(double x, double y, double z);
t_tuple		vector_tp(t_vector vec);
t_tuple		point_tp(t_point p);
int			double_equal(double a, double b);
int			tuple_equal(t_tuple a, t_tuple b);
double		magnitude(t_vector vec);
t_vector	normalize(t_vector vec);
double		dot(t_tuple a, t_tuple b);
t_tuple		add(t_tuple a, t_tuple b);
t_tuple		subtract(t_tuple a, t_tuple b);
t_vector	cross(t_vector vec1, t_vector vec2);
t_vector	negate_vector(t_vector vec);
t_tuple		negate(t_tuple tp);
t_tuple		multiply(t_tuple tp, float num);
t_tuple		divide(t_tuple tp, float num);
t_vector	subtract_points(t_point p1, t_point p2);
t_point		subtract_vector(t_point p, t_vector vec);

//Color
t_color		color(double red, double green, double blue);
t_color		color_add(t_color c1, t_color c2);
t_color		color_subt(t_color c1, t_color c2);
t_color		color_scale(t_color c, double num);
t_color		color_multi(t_color c1, t_color c2);

//Matrices
double		**create_matrix(double *elem, int col);
void		print_matrix(double **mat, int col);
int			matrix_equality(double **mat1, double **mat2, int col);
double		**matrix_multi(double **mat1, double **mat2);
t_tuple		matrix_multi_tp(double **mat, t_tuple tp);
double		**identity_matrix(void);
double		**transpose(double **mat);
double		**submatrix(double **mat, int size, int row, int col);
double		minor(double **mat, int size, int row, int col);
double		cofactor(double **mat, int size, int row, int col);
double		determinant(double **mat, int size);
int			invertible(double **mat, int size);
double		**inverse(double **mat, int size);

//Matrix Transformations
double		**translation(t_tuple tp);
double		**scaling(t_tuple tp);
double		**rotation_x(double rad);
double		**rotation_y(double rad);
double		**rotation_z(double rad);
t_tuple		shearing(t_tuple tp, double *coord);
double		rad(double deg);

//Ray
t_ray		ray(t_point p, t_vector v);
t_point		position(t_ray r, float num);
t_sphere	*sphere();
t_intersect	intersect(t_shape s, t_ray r);
t_intersection	intersection(double value, t_shape object, int count);
t_intersection	*intersections(t_intersection i1, t_intersection i2);
t_intersection	*intersections2(int n, ...);
t_intersection	hit(t_list *xs);
t_ray		transform(t_ray r, double **m);
void		set_transform(t_shape *s, double **t);
t_vector		normal_at(t_shape s, t_point p);

t_vector	reflect(t_vector vec, t_vector normal);

//Scene and lights
t_light		point_light(t_point pos, t_color intensity);
t_material	material(void);
t_color		lighting(t_comps c, t_light l, t_bool in_shadow);
t_world		world(void);
t_world		default_world(t_data *scene_data);
t_list	*intersect_world(t_world w, t_ray r);
t_comps	prepare_computations(t_intersection i, t_ray r);
t_color	shade_hit(t_world w, t_comps comps);
t_color	color_at(t_world w, t_ray r);

//Utility
void	sort_intersections(t_list *xs);

//View transformation
double	**view_transform(t_point from, t_point to, t_vector up);
t_camera2	camera(double hsize, double vsize, double field_of_view);
t_ray	ray_for_pixel(t_camera2 camera, double x, double y);
t_bool	is_shadowed(t_world w, t_point p);

t_world	world(void);

void  render(t_camera2, t_world, t_data *scene_data);



t_intersect	local_intersect_sphere(t_ray r);
t_vector	local_normal_at_sphere(t_point obj_point, t_point point);
t_intersect local_intersect_plane(t_ray r);
t_intersect local_intersect_cylinder(void *cy, t_ray r);
t_vector		local_normal_at_cylinder(void *shape,  t_point pnt);
t_cy	*cylinder(t_cy *c);

void free_2d_array(double **arr, int size);
void free_2d_char_array(char **arr);
#endif