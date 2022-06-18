#ifndef MINIRT_H
#define MINIRT_H

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "../libs/libft/libft.h"

//vector

typedef struct s_vector
{
    double x;
    double y;
    double z;
}              t_vector;


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
    t_vector pos;
    t_color color;
    double  ratio;

}               t_light;

// main struct
typedef struct s_data
{
    t_list  *lights;


    t_color amb_color;
    double  amb_ratio;
}       t_data;

// PARSE FUNCTIONS
void parse_scene();


// getnextline functions
char	*get_next_line(int fd);
static char	*get_current_line(char *saved_line, char *line);
char	*free_memmory(char **ptr);
char	*ft_strchr(const char *str, int c);
size_t	get_current_line_size(char *saved_line);

//parser functions

void parse_scene(char *file_name, t_data *data);


//FREE FUNCTIONS
void free_scene_data();


// ERROR FUNCTIONS
void print_error_msg_and_exit(char *error_msg, t_data *scene_data);

#endif