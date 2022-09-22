#include "minirt.h"

t_info	info_init(t_info info)
{
	t_object	obj;
	t_object	light;
	double		ka;

    // info.canvas = canvas_init(WIN_W, WIN_H);
    // info.camera = camera_init(info.canvas, vec_init(0, 0 ,0));
	// obj = object_init(SP, sphere_init(vec_init(-2, 0, -5), 2), vec_init(0.5, 0, 0));
	
	// obj_add(&obj, object_init(SP, sphere_init(vec_init(0, -1000, 0), 995), vec_init(1, 1, 1)));
	// obj_add(&obj, object_init(SP, sphere_init(vec_init(2, 0, -5), 2), vec_init(0, 0.5, 0)));
	// // obj_add(&obj, object_init(SP, sphere_init(vec_init(0, 0, -5), 1), vec_init(1, 1, 1)));
	// info.obj = obj;
	// // light = object_init(LIGHT_POINT, light_point(vec_init(0, 20, 5), vec_init(1, 1, 1), 0.5), vec_init(0, 0, 0));
	// light = object_init(LIGHT_POINT, light_point(vec_init(0, 20, 0), vec_init(1, 1, 1), 0.5), vec_init(0, 0, 0)); // 더미 albedo

	// info.light = light;
	// ka = 0.1;
	// info.ambient = vec_multi_double(vec_init(1, 1, 1), ka);
	return (info);
}

int main()
{
	t_info	info;
	t_mlx	mlx;

	info = info_init(info);
	mlx.ptr = mlx_init();
	mlx.win = mlx_new_window(mlx.ptr, WIN_W, WIN_H, "HojinyRT");
	mlx.img.img_ptr = mlx_new_image(mlx.ptr, IMG_W, IMG_H);
	mlx.img.addr = (int *)mlx_get_data_addr(mlx.img.img_ptr, \
		&(mlx.img.bits_per_pixel), &(mlx.img.line_length), &(mlx.img.endian));


}