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
		printf("type : %d \n", obj->type);
		printf("x : %f ", ((t_sphere *)obj->element)->center.x);
		printf("y : %f ", ((t_sphere *)obj->element)->center.y);
		printf("z : %f \n", ((t_sphere *)obj->element)->center.z);
		curr = curr->next;
	}
}

int main()
{
	t_info	info;
	t_color	color;
	t_img	*img;
	int i;
	int	j;
	float	u;
	float	v;
	t_canvas	canv;
	t_camera	camera;
	t_ray		ray;
	// t_sphere	*sp;
	t_object    *obj;

	obj = object_init(SP, (t_object *)sphere_init(vec_init(10, 0, -5), 2));
	obj_add(&obj, object_init(SP, sphere_init(vec_init(0, 0, -5), 1)));
	obj_add(&obj, object_init(SP, sphere_init(vec_init(0, 0, -5), 1)));
	print_obj(obj);

	canv = canvas_init(WIN_W, WIN_H);
	camera = camera_init(canv, vec_init(0, 0 ,0));
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
			u = (float)j / (canv.width - 1);
			v = (float)i / (canv.height - 1);
			ray = ray_primary(camera, u, v);
			color = ray_color(ray, obj);
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
