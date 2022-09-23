# include <limits.h>
# include <stdio.h>
#include <stdlib.h>

int	ft_isdigit(int c)
{
	if ('0' <= c && c <= '9')
		return (1);
	return (0);
}

void	is_sign(char *str, int *idx, double *sign)
{
	*idx = -1;
	*sign = 1.0;
	if (str[++(*idx)] == '+' || str[*idx] == '-')
	{
		if (str[*idx] == '-')
			*sign *= -1.0;
        return ;
	}
    (*idx)--;
}

double	ft_atod(char *str)
{
	int		idx;
	int		flag;
	double	sign;
	double	res;
	double	decimal;
	
	flag = 0;
    res = 0.0;
	is_sign(str, &idx, &sign);
	while (str[++idx])
	{
		if ((flag++ == 0 && str[idx] == '.') || ((!ft_isdigit(str[idx]) && str[idx] != '.')))
			exit(1);
		if (str[idx] == '.')
		{
			idx++;
			break ;
		}
		res = (str[idx] - '0') + (res * 10);
		if ((sign == 1 &&res > 2147483647) || (sign == -1 && res > 2147483648))
			exit(2);
	}
	flag = 0;
	decimal = 0.1;
    if (str[idx - 1] == '.' && !str[idx])
        exit(2);
	while(str[idx])
	{
		if (!ft_isdigit(str[idx]))
			exit(3);
		if (flag++ > 6)
        {
            idx++;
			continue ;
        }
		res = res + (str[idx] - '0') * decimal;
		decimal /= 10;
		idx++;
	}
    if ((sign == -1 && res > 2147483648) || (sign == 1 && res > 2147483647))
        exit(10);
	return (sign * res);
}

int main(int ac, char **av)
{
    if (ac != 2)
        return (0);
    
    double test = ft_atod(av[1]);
    printf("test : %lf\n", 1. + 2.2);
    printf("val : %lf\n", test);
}