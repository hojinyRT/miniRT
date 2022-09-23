#include "minirt.h"
#include <stdlib.h>

double clamp(double x, double min, double max)
{
    if (x < min)
		return min;
    if (x > max)
		return max;
    return x;
}

int	key_press(int keycode)
{
	if (keycode == KEY_ESC)
		exit(0);
	return (0);
}

int	convert_color(t_vec clr, int samples_per_pixel)
{	
	double scale = 1.0 / (double)samples_per_pixel;

	clr.x *= scale; 
	clr.y *= scale; 
	clr.z *= scale;
	
	// int tmp = (((int)clr.x * 16 * 16 * 16 * 16 + (int)clr.y  * 16 * 16 + (int)clr.z));
	int tmp = (((int)clamp(clr.x, 0.0, 255.0)) * 16 * 16 * 16 * 16 + ((int)clamp(clr.y, 0.0, 255.0))  * 16 * 16 + (int)clamp(clr.z, 0.0, 255.0));
	// printf("color = %#x\n", tmp);
	return (tmp);
}

void  my_mlx_pixel_put(t_img *img, int x, int y, t_color color, int s)
{
	img->addr[WIN_W * y + x] = convert_color(color, s);
}

void	print_obj(t_object *obj)
{
	t_object	*curr;
	
	curr = obj;
	while (curr)
	{
		printf("type : %d \n", curr->type);
		printf("x : %f ", ((t_sphere *)curr->element)->center.x);
		printf("y : %f ", ((t_sphere *)curr->element)->center.y);
		printf("z : %f \n", ((t_sphere *)curr->element)->center.z);
		curr = curr->next;
	}
}

t_scene	*scene_init(void)
{
	t_scene		*scene;
	t_object	*obj;
	t_object	*light;
	double		ka;

	if(!(scene = (t_scene *)malloc(sizeof(t_scene))))
        return (NULL);
    scene->canvas = canvas_init(WIN_W, WIN_H);
    scene->camera = camera_init(scene->canvas, vec_init(0, 0 ,0));
	obj = object_init(SP, sphere_init(vec_init(-2, 0, -5), 2), vec_init(0.5, 0, 0));
	// obj_add(&obj, object_init(SP, sphere_init(vec_init(0, -200, -20), 200), vec_init(0, 0.5, 0)));
	obj_add(&obj, object_init(SP, sphere_init(vec_init(0, -1000, 0), 995), vec_init(1, 1, 1)));
	obj_add(&obj, object_init(SP, sphere_init(vec_init(2, 0, -5), 2), vec_init(0, 0.5, 0)));
	// obj_add(&obj, object_init(SP, sphere_init(vec_init(0, 0, -5), 1), vec_init(1, 1, 1)));
	scene->obj = obj;
	// light = object_init(LIGHT_POINT, light_point(vec_init(0, 20, 5), vec_init(1, 1, 1), 0.5), vec_init(0, 0, 0));
	light = object_init(LIGHT_POINT, light_point(vec_init(0, 20, 0), vec_init(1, 1, 1), 0.5), vec_init(0, 0, 0)); // 더미 albedo

	scene->light = light;
	ka = 0.1;
	scene->ambient = vec_multi_double(vec_init(1, 1, 1), ka);
	return (scene);
}



double	random_double(void)
{
	// printf("rand : %lf\n",(double)rand() / (RAND_MAX + 1.0));
	return ((double)rand() / (RAND_MAX + 1.0));
}

int main()
{
	t_scene	*scene;
	t_info	info;
	t_color	color = vec_init(0,0,0);
	t_img	*img;
	int i;
	int	j;
	double	u;
	double	v;
	t_ray		ray;
	int		samples_per_pixel = 100;

	scene = scene_init();

	// printf("hey : %d\n", (scene->light->type));
	print_obj(scene->obj);
	info.mlx = mlx_init();
	info.win = mlx_new_window(info.mlx, MLX_WW, MLX_HH, "HojinyRT");
	img = ft_calloc(1, sizeof(t_img));
	img->ptr = mlx_new_image(info.mlx, WIN_W, WIN_H);
	img->addr = (int *)mlx_get_data_addr(img->ptr, \
		&(img->bits_per_pixel), &(img->line_length), &(img->endian));
	i = WIN_H - 1;
	while (i >= 0)
	{
		j = 0;
		while (j < WIN_W)
		{
			color = vec_init(0, 0, 0);
			for (int s = 0; s < samples_per_pixel; ++s)
			{
				
				u = ((double)j  + random_double()) / (scene->canvas.width - 1);
				v = ((double)i  + random_double()) / (scene->canvas.height - 1);
				scene->ray = ray_primary(scene->camera, u, v);
				color = vec_add(color, ray_color(scene));
			}
			my_mlx_pixel_put(img, j, WIN_H - 1 - i, color, samples_per_pixel);
			j++;
			
		}
		printf("i : %d\n", i);
		i--;
	}
	printf("DONE\n");
	mlx_put_image_to_window(info.mlx, info.win, img->ptr, 0, 0);
	mlx_hook(info.win, EVENT_KEY_PRESS, 0, key_press, 0);
	mlx_loop(info.mlx);
	return (0);
}
