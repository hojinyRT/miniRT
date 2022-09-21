#include "minirt.h"

int	key_press(int keycode)
{
	if (keycode == KEY_ESC)
		exit(0);
	return (0);
}

int	convert_color(t_vec clr)
{	
	int tmp = ((int)clr.x * 16 * 16 * 16 * 16) + ((int)clr.y * 16 * 16) + (int)(clr.z);
	return (tmp);
}

void  my_mlx_pixel_put(t_img *img, int x, int y, t_color color)
{
	img->addr[WIN_W * y + x] = convert_color(color);
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
	float		ka;

	if(!(scene = (t_scene *)malloc(sizeof(t_scene))))
        return (NULL);
    scene->canvas = canvas_init(WIN_W, WIN_H);
    scene->camera = camera_init(scene->canvas, vec_init(0, 0 ,0));
	obj = object_init(SP, sphere_init(vec_init(-2, 0, -5), 2), vec_init(0.5, 0, 0));
	// obj_add(&obj, object_init(SP, sphere_init(vec_init(0, -200, -20), 200), vec_init(0, 0.5, 0)));
	obj_add(&obj, object_init(SP, sphere_init(vec_init(2, 0, -5), 2), vec_init(0, 0.5, 0)));
	// obj_add(&obj, object_init(SP, sphere_init(vec_init(0, 0, -5), 1), vec_init(1, 1, 1)));
	scene->obj = obj;
	light = object_init(LIGHT_POINT, light_point(vec_init(0, 20, 5), vec_init(1, 1, 1), 0.5), vec_init(0, 0, 0));
	scene->light = light;
	ka = 0.1;
	scene->ambient = vec_multi_float(vec_init(1, 1, 1), ka);
	return (scene);
}

int main()
{
	t_scene	*scene;
	t_info	info;
	t_color	color;
	t_img	*img;
	int i;
	int	j;
	float	u;
	float	v;
	t_ray		ray;

	scene = scene_init();

	// printf("hey : %d\n", (scene->light->type));
	print_obj(scene->obj);
	info.mlx = mlx_init();
	info.win = mlx_new_window(info.mlx, MLX_W, MLX_H, "HojinyRT");
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
			u = (float)j / (scene->canvas.width - 1);
			v = (float)i / (scene->canvas.height - 1);
			scene->ray = ray_primary(scene->camera, u, v);
			color = ray_color(scene);
			my_mlx_pixel_put(img, j, WIN_H - 1 - i, color);
			j++;
		}
		i--;
	}
	mlx_put_image_to_window(info.mlx, info.win, img->ptr, 0, 0);
	mlx_hook(info.win, EVENT_KEY_PRESS, 0, key_press, 0);
	mlx_loop(info.mlx);
	return (0);
}
