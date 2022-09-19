#include "minirt.h"

int	convert_color(t_color clr)
{
	return ((int)((clr.x * (float)pow(16, 4)) + (clr.y * (float)pow(16, 2)) + clr.z));
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
	i = 0;
	while (i < 300)
	{
		j = 0;
		while (j < 400)
		{
			color = vec_init((float)j / 399.0 * 255.0, (float) i / 299.0 * 255.0, (float)(0.25 * 255.0));
			// printf("color -> x : |%f|,  y: |%f|, z: |%f|\n", color.x, color.y, color.z);
			int c = convert_color(color);
			printf("HEX : |%X|\n", c);
			// color = vec_init(1, 1, 1);
			mlx_pixel_put(info.mlx, info.win, j, i, c);
			// my_mlx_pixel_put(img, j, i, color);
			j++;
		}
		i++;
	}
	// mlx_put_image_to_window(info.mlx, info.win, img->ptr, 100, 100);
	mlx_loop(info.mlx);	return (0);
}


