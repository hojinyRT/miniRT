#include <stdio.h>
#include "mlx2/mlx.h"

typedef struct
{
	int	r;
	int	g;
	int	b;
}	t_color;

typedef struct img
{
	int				type;
	t_color			color;
	char			*file_name;
	void			*image;
	char			*addr;
	int				height;
	int				width;
	int				bits_per_pixel;
	int				line_length;
	int				endian;
}	img;

typedef struct mlx
{
	img		img;
	void	*ptr;
	void	*win;
}	mlx;

int	main()
{
	mlx	info;
	int	a = 350;
	int	b = 350;
	int	x = 0;
	int	y = 0;

	info.ptr = mlx_init();
	info.win = mlx_new_window(info.ptr, 400, 400, "test wall");
	info.img.image = mlx_png_file_to_image(info.ptr, "wall.png" , &a, &b);
	info.img.addr = mlx_get_data_addr(info.img.image, \
											&(info.img.bits_per_pixel), \
											&(info.img.line_length), \
											&(info.img.endian));
	int i = -1;
	char	*dst;
	
	while (x++ < a)
	{
		y = 0;
		while (y++ < b)
		{
			dst = info.img.addr + (y * info.img.line_length + x * (info.img.bits_per_pixel / 8));
			*(unsigned int *)dst = 0xFF0000;
		}
	}
	mlx_put_image_to_window(info.ptr, info.win, info.img.image, 0, 0);
	mlx_loop(info.ptr);
}