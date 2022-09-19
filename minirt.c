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
	img->addr[400 * y + x] = convert_color(color);
}


int main()
{
	t_info	info;
	t_color	color;
	t_img	*img;
	int i;
	int	j;

	
	info.mlx = mlx_init();
	info.win = mlx_new_window(info.mlx, 1000, 1000, "HojinyRT");
	img = ft_calloc(1, sizeof(t_img));
	img->ptr = mlx_new_image(info.mlx, 400, 300);
	img->addr = (int *)mlx_get_data_addr(img->ptr, \
		&(img->bits_per_pixel), &(img->line_length), &(img->endian));
	i = 299;
	while (i >= 0)
	{
		j = 0;
		while (j < 400)
		{
			color = vec_init((float)j / 399 * 255, (float) i / 299 * 255, 0.25 * 255);
			// int c = convert_color(color);
			// mlx_pixel_put(info.mlx, info.win, j, i, c);
			my_mlx_pixel_put(img, j, i, color);
			j++;
		}
		i--;
	}
	mlx_put_image_to_window(info.mlx, info.win, img->ptr, 300, 350);
	mlx_hook(info.win, EVENT_KEY_PRESS, 0, key_press, 0);
	mlx_loop(info.mlx);
	return (0);
}
